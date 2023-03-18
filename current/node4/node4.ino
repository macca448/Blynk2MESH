// ESP32    (Kitchen) chipID = 535417509  (Node: 1) OTA role: sender
// ESP8266  (Garage)  chipID = 110208915  (Node: 2) OTA role: garage
// ESP8266  (Tank1)   chipID = 1298134600 (Node: 3) OTA role: maintank
// ESP32    (Septc)   chipID = 2883624069 (Node: 4) (This Node) OTA role: septic

/**************NODE: 4 (SEPTIC SYSTEM)***********/
#define  LED 2
#include <TimeLib.h>
#include "painlessMesh.h"
#include <Arduino_JSON.h>
#include <ESP32_Servo.h>

// MESH Details
#define   MESH_PREFIX     "EL_RANCHO" //name for your MESH
#define   MESH_PASSWORD   "MESHpassword" //password for your MESH
#define   MESH_PORT       5555 //default port

// ONLY ADC1 available when using WiFi
#define SERVO     22
#define LEVEL     12      //Digital Input - ACTIVE LOW
#define BULB      36      //Analog Input 
#define PUMP1     39      //Analog Input
#define PUMP2     34      //Analog Input
#define ON        HIGH
#define OFF       LOW
#define BYPASS    180
#define NORMAL    0
#define DUTY      15
#define SEC       66      // 66 x DUTY = 1000mS
#define HEARTBEAT 60      // 60 Second Heatbeat

Scheduler userScheduler; // to control your personal task

painlessMesh  mesh;
Servo servo;

const uint32_t  tank1ID = 1298134600, garageID = 110208915, kitchenID = 535417509;
const uint16_t  ten_min = 0x9C40;
const byte      thisNode = 4, tank1Node = 3, garageNode = 2, kitchenNode = 1, all = 5;
bool            sLevelAlarm = 0, sBulbFail = 0, sPumpFail = 0, servoMove = 1, timeSync = 0, ledState,
                levelState = 0, lastLevelState = 0, alarmResend = 0, doorPulse = 0, lightState = 0;
uint8_t         purgeDelay = 0, moveTo = NORMAL, pos = BYPASS, stateTimer = 0;
uint16_t        levelTimer = 0;
uint32_t        previousTime = 0;
String          readings;

// User stub
void heartbeatSend(); // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * HEARTBEAT , TASK_FOREVER, &heartbeatSend );

#include "sendMSG.h"

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  digitalWrite(LED, ON);
  Serial.printf("Received from %u msg= %s\n", from, msg.c_str());
  JSONVar myObject = JSON.parse(msg.c_str());
  uint8_t  dest     = myObject["dest"];
  uint32_t epoch    = myObject["epoch"];
          doorPulse = myObject["doorPulse"];  
         lightState = myObject["lightState"];
  uint8_t sValvePos = myObject["sValvePos"];
  String inMSG =  msg.c_str(); 
    if(inMSG == "  MSG OK"){
        Serial.println((String)"MSG from: " + from + " was:" + inMSG + "\n");
        digitalWrite(LED, OFF);
    }else{
      if(dest == all){
          if(epoch > SECS_YR_2000){             //Check that the new time is valid
              setTime(epoch);
              char tBuff[24];
              sprintf(tBuff, "%02u:%02u:%02u  %02u/%02u/%02u", hour(),minute(),second(),day(),month(),year());
              Serial.printf("Time Updated from NTP: %s\n", tBuff);
              timeSync = true;                  //Starts the main loop user functions 
          }else{
              Serial.println("FAILED to Sync Time............\n");
              timeSync = false;                 //Stops the main loop user functions
          } 
      }else if(dest == thisNode){
          if(sValvePos != moveTo){
              moveTo = sValvePos;
              servo.attach(SERVO); 
              servoMove = true;
          }
      }
    sendAK(from);
    }
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, ON);
  pinMode(LEVEL, INPUT);
  servo.attach(SERVO);
  servo.write(0);
  Serial.begin(115200);
  while(!Serial){};

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  mesh.setContainsRoot(true);
  mesh.initOTAReceive("septic");
  
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  delay(3000);
  servo.detach();
  digitalWrite(LED, OFF);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  
  if(timeSync){          //User Loop functions run when Valid timeSync update is sucessful
  
    if(millis() - previousTime >= DUTY){
        stateTimer++;

        if(stateTimer >= SEC){
            stateTimer = 0;
            uint16_t bulbState   = analogRead(BULB);
            uint16_t pump1State  = analogRead(PUMP1);
            uint16_t pump2State  = analogRead(PUMP2);
                      levelState = digitalRead(LEVEL);
            //Serial.printf("LS=%u, BS=%u, P1=%u, P2=%u\n\n",levelState,bulbState,pump1State,pump2State);

            if(levelState != lastLevelState){
                  if(!levelState){              //Because its "ACTIVE LOW"
                      sLevelAlarm = true;
                          if(bulbState > 0){
                              sBulbFail = false;
                          }else{
                              sBulbFail = true;
                          }
                          if(pump1State > 0 || pump2State > 0){
                              sPumpFail = false;
                          }else{
                              sPumpFail = true;
                          }
                  }else{
                      sLevelAlarm = false;
                      sBulbFail = false;
                      sPumpFail = false;
                  }
               stateChangeSend(); 
            }
            lastLevelState = levelState;          
        }        
        if(servoMove){
                  if(moveTo == BYPASS){
                      pos++;
                      servo.write(pos);
                          if(pos >= BYPASS){
                              servo.detach();
                              servoMove = false;
                              stateChangeSend();
                          }
                  }else{
                      pos--;
                      servo.write(pos);
                          if(pos <= NORMAL){
                              servo.detach();
                              servoMove = false;
                              stateChangeSend();
                          }
                  }
        }
      previousTime = millis(); 
    }
  }
}
