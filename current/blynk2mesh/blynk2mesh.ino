// ESP32    (Kitchen) chipID = 535417509  (Node: 1)
// ESP8266  (Garage)  chipID = 110208915  (Node: 2) 
// ESP8266  (Tank1)   chipID = 1298134600 (Node: 3)
// ESP32    (Septc)   chipID = 2883624069 (Node: 4)

/**************BLYNK NODE (Kitchen) includes Elegant OTA***********/

#include "credentials.h"
#define BLYNK_FIRMWARE_VERSION        "1.2.0"
#define BLYNK_PRINT Serial

#include "SerialTransfer.h"
#include <ESP32_Servo.h>


#if defined(ESP8266)
  #include <BlynkSimpleEsp8266_SSL.h>
  #include <ESPAsyncTCP.h>
  #include <FS.h>
  #define LED LED_BUILTIN
  #define ON  LOW
  #define OFF HIGH
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <BlynkSimpleEsp32_SSL.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
  #define LED 2
  #define ON  HIGH
  #define OFF LOW
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <Blynk/BlynkDateTime.h>

#define LEV_SVO 26
#define HPA_SVO 27
#define STATUS  14
#define HOUR1   12
#define HOUR3   13
#define RXPIN   16
#define TXPIN   17
#define BAUD    19200
#define BYPASS 180
#define NORMAL 0

SerialTransfer myTransfer;
BlynkTimer timer;
WidgetRTC rtc;

Servo level_svo, hpa_svo;

AsyncWebServer server(80);

const uint8_t   kitchenNode = 1, garageNode = 2, tank1Node = 3, septicNode = 4, all = 5;
uint32_t        prevTime = 0;
uint8_t         lastSvalPos, doorStates, systemState, mouseTrap, gaugeTimer;
uint16_t        osPres, roomPressure, tank1Liters, t1Last24hrs, t1AvgUse, t1Errors;  
double          osTemp, roomTemp, osHum, roomHum, tank1Level, t1DaysAvail; 
bool            serialInOn = 1, lastLightState, sendStartUD = 0, usReadOK, ttReadOK,
                gDoorAlarm, sLevelAlarm, sBulbFail, sPumpFail, gaugeDelay;

struct STRUCT1 {
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
} rxStruct;

struct STRUCT2 {
  uint8_t   dest;
  uint32_t  epoch;    
  bool      doorPulse;
  bool      tankLight;
  uint8_t   sValvePos;  
} txStruct;

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
  sendStartUD = true;
}

bool connectToWiFi(void){   
    Serial.println("Connecting to WiFi..........\n");
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

#include "html_vals.h"
#include "cloudIO.h"
#include "inbound.h"
#include "outbound.h"
#include "the_setup.h"

void loop(){
    Blynk.run();
    timer.run();
        if(serialInOn){                   //This disables Serial1 RX to allow data PUSH to BLYNK
            inboundVals();
        }
        if(millis() - prevTime >= 2000){  //This timer will change when the LEDs are added
            gaugeTimer++;
            
            if(sendStartUD){
                sendStartUD = false;
                udMeshTime();             //So the MESH gets Current (now()) Time on start-up.
            }
            if(gaugeDelay){
                if(gaugeTimer >= 5){
                    level_svo.detach();
                    hpa_svo.detach();
                    gaugeDelay = false;
                }              
            }
            prevTime=millis();
        }
}
