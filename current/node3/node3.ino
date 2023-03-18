// ESP32    (Kitchen) chipID = 535417509  (Node: 1) OTA role: sender
// ESP8266  (Garage)  chipID = 110208915  (Node: 2) OTA role: garage
// ESP8266  (Tank1)   chipID = 1298134600 (Node: 3) (This Node) OTA role: maintank
// ESP32    (Septc)   chipID = 2883624069 (Node: 4) OTA role: septic

/**************NODE: 3 (TANK)***********/

#define PRINT                   //Uncomment for Serial Monitor debugging
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESPFlash.h>
#include "painlessMesh.h"
#include <Arduino_JSON.h>
#include <ESPFlash.h>
#include <TimeLib.h>

// MESH Details
#define   MESH_PREFIX     "EL_RANCHO" //name for your MESH
#define   MESH_PASSWORD   "MESHpassword" //password for your MESH
#define   MESH_PORT       5555 //default port

// Constants
#define MAX_ERROR (double)2.0
#define BPRESS_OFFSET (double)3.9 
#define LVAL 2550
#define HVAL 100
#define FULL 22500
#define AUTO_SEND 17

#if defined (ESP8266)
  #define LED LED_BUILTIN
  #define ON LOW
  #define OFF HIGH
  #define ONE_WIRE_BUS D5
  #define LIGHT D8
  #define TRIG_PIN D6
  #define ECHO_PIN D7
#elif defined (ESP32)
  #define LED 2
  #define ON HIGH
  #define OFF LOW
  #define ONE_WIRE_BUS 25
  #define LIGHT 33
  #define TRIG_PIN 26
  #define ECHO_PIN 27
#endif

//Sensor Objects
Adafruit_AHTX0 aht;                                       // Combo I2C
Adafruit_BMP280 bmp;                                      // Combo I2C
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();  // Combo I2C
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ttSensor(&oneWire);

//Variables
const uint32_t  septicID = 2883624069, garageID = 110208915, kitchenID = 535417509;
const uint8_t   thisNode = 3, kitchenNode = 1, garageNode  = 2, septicNode  = 4, all = 5;
uint8_t         dTimeOut = 0;
uint16_t        readErrors  = 0, osPressure, distance, lastDist, tank1Liters, lastLiters,
                t1Last24hrs = 147, t1AvgUse = 93;
uint32_t        previousTime = 0;
double          osHum, osTemp, tankTemp, tank1Level, lastLevel, t1DaysAvail = 0;
bool            usReadOK, ttReadOK, doStateUpDate = 0, startLoop = 0, timeSync = 0;
volatile bool   lastLightState = 0;
String          readings;

//Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
void heartBeatMessage(bool heartBeat, bool sLevelAlarm, uint8_t sValvePos, bool sBulbFail, bool sPumpFail) ;
String getHbReadings(); // Prototype for sending sensor readings
String getScReadings();

//Create tasks: to send messages and get readings;
//Task taskSendMessage(TASK_HOUR * 1, TASK_FOREVER, &hbSendMessage);

// My Sketch Headers
#include "sensor_inits.h"
#include "Sensor_Data.h"
#include "data_traffic.h"
#include "all_callbacks.h"

void setup() {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, ON);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LIGHT, OUTPUT);
    digitalWrite(LIGHT, LOW);
  #ifdef PRINT
    Serial.begin(115200);
    while(!Serial){};
    Serial.println("\n\rStarting Sensors.......\n");  
  #endif
    
    sensorINTS();

    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

    mesh.init( MESH_PREFIX, MESH_PASSWORD, /*&userScheduler,*/ MESH_PORT );
    mesh.onReceive(&receivedCallback);
  #ifdef PRINT
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  #endif    
    mesh.setContainsRoot(true);
    
    //userScheduler.addTask(taskSendMessage);
    //taskSendMessage.enable();
    digitalWrite(LED, OFF);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  if(timeSync){
    if(millis() - previousTime >= 1000){
      if(doStateUpDate){
          digitalWrite(LED, ON);
          sendMessage();
          doStateUpDate = false;
          Serial.println("Sending State Update from Loop\n");
          digitalWrite(LED, OFF);
      }
    previousTime = millis();
    }
  }
}
