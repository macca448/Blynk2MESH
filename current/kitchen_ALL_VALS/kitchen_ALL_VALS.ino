
/********************************NODE: 1 (KITCHEN)********************************
 * Current Nodes
 * ESP32    (Kitchen)   Node: 1   chipID = 535417509  (This Node & ROOT of MESH & OTA Sender)
 * ESP8266  (Garage)    Node: 2   chipID = 110208915  OTA role: garage
 * ESP8266  (Tank1)     Node: 3   chipID = 1298134600 OTA role: maintank
 * ESP32    (Septic)    Node: 4   chipID = 2883624069 OTA role: septic
 *
 * DATA Path HeartBeat Inbound - Node 4 sens current states to Node 3. Node 3 adds
 * it's current state's to the message and forwards to Node 2 and the same for Node 1.
 * 
 * PIN's Used:                                           OTHERS:
 * OLED VSPI: MOSI : 23    SD CARD HSPI: MOSI : 13       LEFT   BTN: 39   LED    : 2  (BuiltIN)
 *            MISO : 19                  MISO : 12       CENTER BTN: 34   ENC BTN: 25
 *            CLK  : 18                  CLK  : 14       RIGHT  BTN: 35   ENC CLK: 33
 *            DC   : 4                                   OTA ENABLE: 39   ENC  DT: 32  
 *            CS   : 5                   CS   : 15                          
 * 
 * To make use of the unused pins on the Blynk node we will push some states
 * for device action as follows:
 *    Level gauge (servo)
 *    Pressure gauge (servo)
 *    Individual System Alarm States (Addressable led strip)
 *    One Hour Weather Prediction with Addressable led strip 
 *    Three Hour Weather Prediction with Addressable led strip
 *    Buzzer
 * 
 **********************************************************************************/

#include <painlessMesh.h>
#include <Arduino_JSON.h>
#include "SerialTransfer.h"
#include <FS.h>
#include "SD.h"
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OTA_PART_SIZE 1024 //How many bytes to send per OTA data packet

//For SPI OLED
#define OLED_DC     4
#define OLED_CS     5
#define OLED_RESET  27

//Buttons
#define LEFT_BTN    39
#define CENTER_BTN  34
#define RIGHT_BTN   35

//Encoder
#define ENC_BTN     25
#define ENC_CLK     32
#define ENC_DT      33

//Buzzer
//#define BUZZER      26
//#define BUZZ_ON     LOW
//#define BUZZ_OFF    HIGH

//OTA Enable
#define OTA         36

//Serial1 PINs
#define RXPIN       16
#define TXPIN       17

//Display Object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);
//Adafruit_SSD1305 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS, 7000000UL);

//Constants
#define BAUD 19200
#define DUTY 50
#define FIVE_SEC 100
#define THREE_MIN 3600

// MESH Details
#define   MESH_PREFIX     "EL_RANCHO" //name for your MESH
#define   MESH_PASSWORD   "MESHpassword" //password for your MESH
#define   MESH_PORT       5555 //default port

#if defined (ESP8266)
  #define LED_PIN LED_BUILTIN
  #define ON LOW
  #define OFF HIGH
#elif defined (ESP32)
  #define LED_PIN 2
  #define ON HIGH
  #define OFF LOW
#endif

//Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
SerialTransfer myTransfer;

Adafruit_BME280 bme;
#define ALT_OFFSET (float)3.8

//Sent to Chip ID's
const uint32_t garageID = 110208915, tank1ID = 1298134600, septicID = 2883624069;

//Node Numbers and Buttons
const uint8_t thisNode = 1, garageNode = 2, tank1Node = 3, septicNode = 4, all = 5;

//Variables
bool      leftBtnPress = 0, leftBtnState, lastStateLeftBtn = 1,
          centerBtnPress = 0, centerBtnState, lastStateCenterBtn = 1,
          rightBtnPress = 0, rightBtnState, lastStateRightBtn = 1, screenON = 1, screenStart = 0,
          lastLightState = 0, usReadOK = 0, ttReadOK = 0, sBulbFail = 0, sPumpFail = 0,
          gDoorAlarm = 0, sLevelAlarm = 0, autoRotate = 1,
          encClk, encLastClk, encPress = 0, encBtn, encLastBtn = 1;
uint8_t   displayTimer = 0, lastServoPos = 0, sValvePos = 0, doorStates = 0, mouseTrap = 0, 
          rotateTimer = 0, gaugeTimer = 0, lastDest, scCount = 0;
int8_t    screen = 0;
uint16_t  roomPressure, osPres, tank1Liters, t1Last24hrs, t1AvgUse = 0, t1Errors = 0, screenTimer = 0;
double    roomTemp, osTemp, roomHum, osHum, tank1Level, t1DaysAvail;
uint32_t  previousTime = 0;

struct STRUCT1 {                //Send to Blynk 
  uint8_t   node;
  bool      heartBeat;
  double    osTemp;
  double    roomTemp;
  double    osHum;
  double    roomHum;
  uint16_t  osPres;
  uint16_t  roomPressure;
  double    tank1Level;
  uint16_t  tank1Liters;
  uint16_t  t1Last24hrs;
  uint16_t  t1AvgUse;
  double    t1DaysAvail;
  bool      usReadOK;
  bool      ttReadOK;
  uint16_t  t1Errors;
  bool      lightState;
  uint8_t   doorStates;
  uint8_t   mouseTrap;
  bool      gDoorAlarm;
  bool      sLevelAlarm;
  uint8_t   sValvePos;
  bool      sBulbFail;
  bool      sPumpFail;
  uint8_t   systemState;
} txStruct;

struct STRUCT2 {                //Received from Blynk
  uint8_t   dest;
  uint32_t  epoch;    
  bool      doorPulse;
  bool      tankLight;
  uint8_t   sValvePos; 
} rxStruct;

#include "display_pages.h"
#include "all_callbacks.h"
#include "serialRX.h"
#include "ota_config.h"

void setup() {
  pinMode(LEFT_BTN, INPUT);
  pinMode(CENTER_BTN, INPUT);
  pinMode(RIGHT_BTN, INPUT);
  pinMode(OTA, INPUT);
  pinMode(ENC_BTN, INPUT_PULLUP);
  pinMode(ENC_CLK, INPUT);
  pinMode(ENC_DT, INPUT);
  //pinMode(BUZZER, OUTPUT);
  //digitalWrite(BUZZER, BUZZ_OFF);
  
  Serial.begin(115200);
  while(!Serial){};
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  while(!Serial1){};
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ON);
  myTransfer.begin(Serial1);
   
  if (! bme.begin(0x76, &Wire)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  //for I2C OLED
  //if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //fpr SPI OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
   /* For SSD1305 Library
      if ( ! display.begin(0x3C) ) {
     Serial.println("Unable to initialize OLED");
     while (1) yield();
  }*/
  
//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init(MESH_PREFIX, MESH_PASSWORD, /*&userScheduler,*/ MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.setRoot(true);
  mesh.setContainsRoot(true);
  
  //userScheduler.addTask( taskSendMessage );
  //taskSendMessage.enable();

  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  digitalWrite(LED_PIN, OFF);
  //OTA Decision
  bool noOTA = digitalRead(OTA);
      if(noOTA){
          display.setCursor(20,20);
          display.println("Starting Up.......\n");
          display.display();
          digitalWrite(LED_PIN, OFF);
      }else{
        display.setCursor(0,0);
        display.println("STARTING OTA......\n");
        display.display();
        runOTA();
      }
}

void loop(){
    leftBtnState   = digitalRead(LEFT_BTN);
    centerBtnState = digitalRead(CENTER_BTN);
    rightBtnState  = digitalRead(RIGHT_BTN);
    encBtn    = digitalRead(ENC_BTN);
    encClk         = digitalRead(ENC_CLK);
    bool dtState   = digitalRead(ENC_DT);
    
        if(leftBtnState != lastStateLeftBtn){
            leftBtnPress = true;
            previousTime = millis();
        }
        if(centerBtnState != lastStateCenterBtn){
          centerBtnPress = true;
          previousTime = millis();
        }
        if(rightBtnState != lastStateRightBtn){
          rightBtnPress = true;
          previousTime = millis();
        }
        if(encBtn != encLastBtn){
          encPress = true;
          previousTime = millis();
        }
        
        if(screenON && !autoRotate){
            if (encClk != encLastClk  && encClk == 1){
                if (dtState != encClk) {
                    screen++;
                    screen %= 4;
                }else{
                    screen--;
                        if(screen < 0){
                            screen = 3;
                        }
                }
              screenChoice();
            }
        }
        
        if(millis() - previousTime >= DUTY){
          rotateTimer++; screenTimer++; scCount++;
          previousTime = millis();
          
           if(encPress){   
              if(encBtn == LOW){
                if(screenON){                  
                  screenTimer = 0;
                  autoRotate = !autoRotate; 
                  Serial.printf("Auto Rotate is: %u\n\n", autoRotate);
              }else{
                  screenStart = true;
                  Serial.println("Turning Screen On\n");
                  scCount = 0;
              }
            }
          }
          if(screenStart){
              String currentTime = getNow();
              if(scCount >= 2){
                  display.ssd1306_command(SSD1306_DISPLAYON);
                  display.clearDisplay();
                  display.setTextSize(1);               // Normal 1:1 pixel scale
                  display.setTextColor(SSD1306_WHITE);  // Draw white text
                  display.setCursor(0,8);
                  display.println("Display Back ON!!");
                  display.setCursor(0,24);
                  display.println(currentTime);
                  display.display();
                  screenON = true;
                  screenTimer = 0;
                  rotateTimer = 0;
                  screen = 0;
                  screenStart = false;          
              }
          }
          if(screenON){               
            if(autoRotate){
                if(rotateTimer >= FIVE_SEC){
                  screenChoice();
                  screen++;
                  screen %= 4;
                  rotateTimer = 0;
                  }
            }                
            if(screenTimer >= THREE_MIN){
                screenON = false;
                autoRotate = true;
                display.ssd1306_command(SSD1306_DISPLAYOFF);
            }
          }
          if(leftBtnPress){
              if(leftBtnState == LOW){
                  digitalWrite(LED_PIN, ON);
                  uint8_t dest = garageNode;
                  uint32_t newEpoch = now();
                  bool doorPulse = true;
                  uint8_t newServoPos = lastServoPos;
                  bool newLightState = lastLightState;
                  Serial.println("Open/Close Garage Door\n");
                  outbound(dest, newEpoch, doorPulse, newLightState, newServoPos);
               }
            }
            if(centerBtnPress){
                if(centerBtnState == LOW){
                    digitalWrite(LED_PIN, ON);
                    uint8_t dest = septicNode;
                    uint32_t newEpoch = now();
                    bool doorPulse = false;
                    bool newLightState = lastLightState;
                        if(lastServoPos == 0){
                            lastServoPos = 180;
                        }else{
                            lastServoPos = 0;
                        }
                    Serial.printf("Move Servo to: %u angle\n\n", lastServoPos);
                    outbound(dest, newEpoch, doorPulse, newLightState, lastServoPos);
                    digitalWrite(LED_PIN, OFF);
                }
            }
            if(rightBtnPress){
                if(rightBtnState == LOW){
                    digitalWrite(LED_PIN, ON);
                    uint8_t dest = tank1Node;
                    uint32_t newEpoch = now();
                    bool doorPulse = false;
                    lastLightState = !lastLightState;
                    uint8_t newServoPos = lastServoPos;
                    Serial.printf("Tank LED is: %u\n\n", lastLightState);
                    outbound(dest, newEpoch, doorPulse, lastLightState, newServoPos);
                    digitalWrite(LED_PIN, OFF);
                }
            }
            encPress = false;
            leftBtnPress = false;
           centerBtnPress = false;
          rightBtnPress = false;
        }
        encLastClk = encClk;
        encLastBtn = encBtn;
      lastStateLeftBtn = leftBtnState;
    lastStateCenterBtn = centerBtnState;
  lastStateRightBtn = rightBtnState;
  
  // it will run the user scheduler as well
  mesh.update();  
  
  //Inbound Serial
  serialRX();
}
