#include "SerialTransfer.h"
#define RXPIN 16
#define TXPIN 17
#define BAUD 19200
SerialTransfer myTransfer;

struct STRUCT1 {
  uint8_t   node;
  float    osTemp;
  double    roomTemp;
  float    osHum;
  float    roomHum;
  uint16_t  osPressure;
  uint16_t  roomPressure;
  float    t1Temp;
  uint16_t  t1Dist;
  bool      usReadOK;
  bool      ttReadOK;
  float    tank1Level;
  uint16_t  tank1Liters;
  uint16_t  t1Errors;
  uint8_t   doorStates;
  uint8_t   currentPos;
} rxStruct;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  myTransfer.begin(Serial1);
}

void loop() {
  if(myTransfer.available()){
      uint16_t recSize = 0;
      recSize = myTransfer.rxObj(rxStruct, recSize);
      uint8_t node = rxStruct.node;
      float osTemp = rxStruct.osTemp;
      double roomTemp = rxStruct.roomTemp;
      float osHum = rxStruct.osHum;
      float roomHum = rxStruct.roomHum;
      uint16_t osPressure = rxStruct.osPressure;
      uint16_t roomPressure = rxStruct.roomPressure;
      float t1Temp = rxStruct.t1Temp;
      uint16_t t1Dist = rxStruct.t1Dist;
      bool usReadOK = rxStruct.usReadOK;
      bool ttReadOK = rxStruct.ttReadOK;
      float tank1Level = rxStruct.tank1Level;
      uint16_t tank1Liters = rxStruct.tank1Liters;
      uint16_t t1Errors = rxStruct.t1Errors;
      uint8_t doorStates = rxStruct.doorStates;
      uint8_t currentPos = rxStruct.currentPos;
      Serial.printf("Message Received from Node: %u\n", node);
      Serial.print("Room Temp: ");Serial.println(roomTemp, 2);     
      Serial.printf("Room Temp: %d  Outside Temp: %d\n", roomTemp, osTemp);
      Serial.printf("Room Hum: %d  Outside Hum: %d\n", roomHum, osHum);
      Serial.printf("Room Pres: %u  Outside Pres: %u\n", roomPressure, osPressure);
      Serial.printf("Tank Temp: %d  Water Dist: %u mm\n", t1Temp, t1Dist);
      Serial.printf("Read Sucess Ultra: %u  Tank Temp: %u\n", usReadOK, ttReadOK);
      Serial.printf("Tank Level: %d  Tank Liters: %u\n", tank1Level, tank1Liters);
      Serial.printf("Tank Sensor Read Error Count: %u\n", t1Errors);
      Serial.printf("Current Door States %u: \n", doorStates);
      Serial.printf("Current Servo Position %u: \n", currentPos);
      Serial.printf("Bytes received: %u \n\n", recSize);
  }
}
