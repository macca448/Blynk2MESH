#include "SerialTransfer.h"
#define RXPIN 16
#define TXPIN 17
#define BAUD 19200
SerialTransfer myTransfer;

uint32_t previousTime = 0;

struct STRUCT1 {
  uint8_t   node = 1;
  float    osTemp = 23.55;
  double    roomTemp = 23.55;
  float    osHum = 55.55;
  float    roomHum = 55.55;
  uint16_t  osPressure = 1000;
  uint16_t  roomPressure = 1000;
  float    t1Temp = 18.75;
  uint16_t  t1Dist = 1453;
  bool      usReadOK = 1;
  bool      ttReadOK = 1;
  float    tank1Level = 45.67;
  uint16_t  tank1Liters = 10522;
  uint16_t  t1Errors = 0;
  uint8_t   doorStates = 1;
  uint8_t   currentPos = 180;
} txStruct;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  myTransfer.begin(Serial1);
}
void loop() {

    if(millis() - previousTime >= 10000){
        uint16_t sendSize = 0;
        sendSize = myTransfer.txObj(txStruct, sendSize);
        myTransfer.sendData(sendSize);
        Serial.printf("Serial Transfer bytes: %u\n\n", sendSize);
        previousTime = millis();
    }
}
