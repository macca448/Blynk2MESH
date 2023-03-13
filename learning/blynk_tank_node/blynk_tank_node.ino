/*
 * Statistical Water Tank Level monitor and basic Weather Data (Temperature Humidity & Barometric Pressure)
 * The sketch runs in a one hour sleep cycle which makes the 24hr SPIFFS update a simple task
 * Every day during the hour of 05:00 we update our readings stored in SPIFFS
 * Data held in SPIFFS is:
 *       Index Value for 30 day usage array
 *       Rain Days Count for current month (This is assumed as I only look at water level change)
 *       Rain Days Month so we can watch for month roll and zero the rain days count
 *       last tank level in liters to let us calculate daily water use
 *       A 30 (day) value array of daily use that lets us calculate from average use the days of water left in the tank
 * "Setup MODE" that zero's all SPIFFS stored sketch variables
 * "PROGRAM MODE" which disables sleep to enable Over The Air (OTA) WiFi firmware updates.
 * There is a battery voltage monitoring option. 
 * Two Jumper Pins have 4 x State possibilities and we create a "bit state" array as follows
 *       MODE          HEX         SETUP PIN      MODE PIN         
 *       Sleep         0x3            1              1         
 *        On           0x2            1              0
 *       Setup         0x1            0              1           
 *       Setup         0x0            0              0
 *        
 * We should get a Liters Level Resolution of approx 9lts per mm based on a 22500lts (5000gal) concrete water tank
 * This is based on a useful measuring distance betweem 100mm (FULL) and 2550mm (EMPTY)
 * 
 * NOTE:  The latest version of ESPFlash library has disabled ESP8266
 *        You can fix this one of two ways
 *        #1 Change the includes in the "ESPFlash" 
 *        #2 Add "#include <FS.h>" in the "#if defined(ESP8266)" below (line 42:)
 */

#define PRINT
#include <ESPFlash.h>               // https://github.com/DaleGia/ESPFlash
#include <limits>
#include "credentials.h"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#ifdef PRINT
  #define BLYNK_PRINT Serial
#endif

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#if defined(ESP8266)
  #include <BlynkSimpleEsp8266_SSL.h>
  #include <ESPAsyncTCP.h>
  ADC_MODE(ADC_VCC);
  #define ON LOW
  #define OFF HIGH
#elif defined(ESP32)
  #include <BlynkSimpleEsp32_SSL.h>
  #include <AsyncTCP.h>
  #define LED_BUILTIN 2
  #define ON HIGH
  #define OFF LOW 
#endif  // ESP32

#include <Wire.h> 
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <utility/BlynkDateTime.h>

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#define MODE 13                       //MODE is either "PGM" (Always ON) or "Deep Sleep"
#define SETUP_MODE 7                  //Only used for first run setups
#define SENSOR_GND 15                 //Best to use a pin that has hardware pull-down     
#define LVAL 2550                     //mm reading for empty tank
#define HVAL 100                      //mm reading for full water tank
#define FULL 22500                    //Approx liters of a full (5000 gal) concrete tank
#define SIZE 30                       //Daily Water Use Array Size
#define SLEEP 0b11
#define PROGRAM 0b10
#define SETUP 0b01

const uint32_t  uShour = 0xD693A400,   //uShour = 1 hour in uS used for sleep period
                mShour = 0x36EE80;     //mShour = 1 hour in mS used for Blynk update timer
uint32_t        totalOnTime = 0, thisOnMillis = 0, currentMillis;
const uint8_t   LED = LED_BUILTIN, jumpers[]{MODE, SETUP_MODE};
bool            pgm = false, ledState = OFF, populate = false, firstRead = false,
                success1 = false, success2 = false, success3 = false, success4 = false,
                success5 = false, success6 = false;
String          pgm_mode = "", data_saved = "", onTime = "";
float           volts, daysAvailable = 0, dailyAvgUse = 0, totalDist = 0; 
uint8_t         rainDays = 0, rainMonth = 0, averageIndex = 0, bootMode,
                onSec, onMin, onHour;
uint16_t        usageArray[SIZE], liters = 0, lastLiters, totalUsage = 0,
                last24hr = 0;   

BlynkTimer timer;
WidgetRTC rtc;

AsyncWebServer server(80);

//BME object on the default I2C pins
Adafruit_BME280 bme;
#define pressure_offset 3.9 

bool connectToWiFi(void){   
  #ifdef PRINT
    Serial.println("Connecting to WiFi..........\n");
  #endif
    WiFi.begin(ssid, pass);
    int i = 0;
    while(WiFi.status() != WL_CONNECTED){
        i++;
        delay(500);
        if(i >= 60){
            return false;
        }
    }
    WiFi.mode(WIFI_STA);
    return true;
}

//Calls and Functions NOTE: Be Careful as order of these header files matter
//Main Actions must be after Readings and Flash Data or you'll get a compile error
#include "readings.h"
#include "flash_data.h"
#include "runSetup.h"
#include "mainActions.h"

BLYNK_CONNECTED() {
  // Synchronize time on connection to Blynk Server
  rtc.begin();
}

byte getState(){
  uint8_t i,j=0;
    for(i = 0; i < 2; i++){
      j = (j << 1) | digitalRead(jumpers[i]);   // read each input pin
    }
  return j;
}

void setup(){
  currentMillis = millis();
  #ifdef PRINT
    Serial.begin(115200);
    while(!Serial);
  #endif
  
  //Configure GPIO's
  pinMode(LED, OUTPUT);
  digitalWrite(LED, ON);                    //Visually see how long the loop takes
  pinMode(12, OUTPUT);                      //Trigger Pin
  pinMode(14, INPUT);                       //Echo Pin
  pinMode(MODE, INPUT_PULLUP);              //Sleep mode = HIGH, pgm mode = LOW
  pinMode(SETUP_MODE, INPUT_PULLUP);        //Setup = LOW, Normal mode = HIGH
  pinMode(SENSOR_GND, OUTPUT);              //To switch all sensors power ON/OFF
  digitalWrite(SENSOR_GND, LOW);            //Start with sensors off
  
  SPIFFS.begin();  
  
  bootMode = getState();
  
  if(bootMode >= PROGRAM){
    //Connect to WiFi then Blynk
    bool wifiConnect = connectToWiFi();
      if(wifiConnect == true){
        #ifdef PRINT
          Serial.print("WiFi Connection Success..........");
          Serial.print("IP address: ");
          Serial.println(WiFi.localIP());
          Serial.println("");
        #endif
        Blynk.config(auth);
        bool result = Blynk.connect();
          if(result){
            #ifdef PRINT
              Serial.println("\n");
              Serial.println("Blynk Connected............\n");
            #endif
          }else{
            #ifdef PRINT
              Serial.println("Blynk Failed to connect but we will Update the data anyway\n");
            #endif
          }
      }else{
        #ifdef PRINT
          Serial.println("WiFi Connection Failure......We will Update the data anyway\n");
        #endif
      }
      if(bootMode == SLEEP){
          //Setup Sleep Timer
          #if defined(ESP32)
              esp_sleep_enable_timer_wakeup(uShour); //ESP32
          #endif
          pgm = false;
      }else if(bootMode == PROGRAM){
          //If in Program (Always ON) MODE
          setSyncInterval(30 * 60);                             //Sync RTC interval in seconds (30 minutes)
          // Setup a function to be called every hour
          timer.setInterval(mShour, mainActions);              //when in "Allways ON (pgm) mode" this timer runs
          pgm = true;
      }
      //Start Sensors
      digitalWrite(SENSOR_GND, HIGH);
      delay(200);                                               //Small delay needed to let sensors power up
      if (! bme.begin(0x76, &Wire)) {
        #ifdef PRINT
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        #endif
        while (1);
      }
    #ifdef PRINT 
      Serial.println("ESP Awake Now............\n");
    #endif
  }
  delay(500); //Let things settle
      
      if(bootMode != SLEEP){          //Any MODE other than SLEEP
          //OTA Setup Items
          server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
              request->send(200, "text/plain", "Hi! This is a sample response.");
          });
          AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
          server.begin();
          #ifdef PRINT
              Serial.println("HTTP server started");
          #endif
      }
      if(bootMode <= SETUP){
          configSPIFFS();
      }
      mainActions();
}

void loop(){
   while(bootMode >= PROGRAM){       //So Blink doesn't run in SETUP mode.
      Blynk.run();
      timer.run();
   }
}
