// ESP32    (Kitchen) chipID = 535417509  (Node: 1) OTA role: sender
// ESP8266  (Garage)  chipID = 110208915  (Node: 2) (This Node) OTA role: garage
// ESP8266  (Tank1)   chipID = 1298134600 (Node: 3) OTA role: maintank
// ESP32    (Septc)   chipID = 2883624069 (Node: 4) OTA role: septic


#include "painlessMesh.h"
#include <Arduino_JSON.h>
#include <TimeLib.h>

// MESH Details
#define   MESH_PREFIX     "EL_RANCHO" //name for your MESH
#define   MESH_PASSWORD   "MESHpassword" //password for your MESH
#define   MESH_PORT       5555 //default port

#define   TRAP            D0  //GPIO16
#define   RELAY           D1  //GPIO5
#define   G_DOOR          D5  //GPIO14
#define   R_DOOR          D6  //GPIO12
#define   I_DOOR          D7  //GPIO13
#define   DOOR_SW         D3  //GPIO0
#define   DOORS           3   //Door count
#define   DUTY            50  //millis() loop Duty period
#define   ONE_SEC         20  //Duty x 20 = 1000mS
#define   ONE_MIN         1200
#define   ONE_HOUR        72000

#if defined (ESP8266)
  #define LED LED_BUILTIN
  #define ON LOW
  #define OFF HIGH
#elif defined (ESP32)
  #define LED 2
  #define ON HIGH
  #define OFF LOW
#endif

//Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;


const uint32_t  kitchenID = 535417509, tank1ID = 1298134600, septicID = 2883624069;

//Node Numbers
const uint8_t   thisNode = 2, kitchenNode = 1, tank1Node = 3, septicNode = 4, all = 5,
                doors[]={I_DOOR, R_DOOR, G_DOOR};
byte            doorStates, lastDoorStates = 7;
uint8_t         delayCount = 0, mtState, lastMTstate = 1; 
uint16_t        daCount = 0;
bool            stateChange = 0, doorPulse = 0, swPress = 0, swState, lastSwState = 1,
                doorStateChange = 0, timeSync = 0, doorAlarm = 0;
uint32_t        previousTime = 0;                                
String          readings;

// User stub
void receivedCallback( uint32_t from, String &msg );

//Task taskSendMessage( TASK_SECOND * HEARTBEAT , TASK_FOREVER, &sendUpdate );

byte getDoorStates(){
    uint8_t i, j = 0;
    for(i = 0; i < DOORS; i++){
      j = (j << 1) | digitalRead(doors[i]);   // read each input pin
    }
  return j;
}

#include "updates.h"
#include "processStrings.h"
#include "all_callbacks.h"

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, ON);
  pinMode(RELAY, OUTPUT);
  pinMode(DOOR_SW, INPUT);
  pinMode(TRAP, INPUT_PULLUP);
  digitalWrite(RELAY, 0);
      for(byte i = 0; i < DOORS; i++){
          pinMode(doors[i], INPUT_PULLUP);
      }

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, /*&userScheduler,*/ MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.initOTAReceive("garage");
  //userScheduler.addTask( taskSendMessage );
  //taskSendMessage.enable();
  delay(1000);            //Let things settle
  digitalWrite(LED, OFF);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();

  //We only process the loop when NTP time has been set
  //An epoch update is sent from Blynk Node onece a minute
  if(timeSync){
  
    //Door States Watch only when no door pulse running
    if(!doorPulse){
        doorStates = getDoorStates();
            if(doorStates != lastDoorStates){
                if(doorStates > 0){           //Door State Decision trigger
                  doorStateChange = true;
                  daCount = 0;
                }else{                        //Doors are all closed
                  stateChange = true;
                  doorAlarm = false;
                  delayCount = 0;
                }
            }
        swState = digitalRead(DOOR_SW);
            if(swState != lastSwState){
              swPress = true;
              previousTime = millis();
            }
        mtState = digitalRead(TRAP);
            if(mtState != lastMTstate){
              stateChange = true;
              delayCount = 0;
            }
    }
    if(millis() - previousTime >= DUTY){
      delayCount++; daCount++;
          if(doorPulse){
              if( delayCount >= 20){
                  doorPulse = false;
                  digitalWrite(RELAY, LOW);
              }
          }
          if(doorStateChange){
              if(month() > 3 && month() < 10){    //Cold Months and mice are the problem
                  if(daCount >= ONE_MIN){
                      doorAlarm = true;
                      stateChange = true;
                      doorStateChange = false;
                  }
              }else{
                  if(daCount >= ONE_HOUR){        //Warm months so no mice problem
                    doorAlarm = true;
                    stateChange = true;
                    doorStateChange = false;
                  }
              }
          }
          
          if(stateChange){
              if( delayCount >= 20){   
                  digitalWrite(LED, ON);
                  doorStates = getDoorStates();
                  mtState = digitalRead(TRAP);
                  Serial.printf("State Change Door State = %u\n", doorStates);
                  scUpdate(doorStates, mtState, doorAlarm);
                  stateChange = false;
                  doorAlarm = false;
                  digitalWrite(LED, OFF);
              }
          }
          if(swPress){
              if(swState == LOW){
                  delayCount = 0;
                  digitalWrite(RELAY, HIGH);
                  doorPulse = true;
              }
            swPress = false;
          }
      previousTime = millis(); 
    }
  lastMTstate = mtState;
  lastDoorStates = doorStates;
  lastSwState = swState;
  }
}
