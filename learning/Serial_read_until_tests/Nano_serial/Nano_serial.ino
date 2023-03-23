#include <SoftwareSerial.h>
#include <Arduino_JSON.h>
#include "SerialTransfer.h"
#define RXPIN 10
#define TXPIN 11 
SoftwareSerial ssLink(RXPIN, TXPIN);
SerialTransfer myTransfer;
const int fileSize = 100;
uint32_t previousTime = 0;
char outFile[fileSize];
char fileName[] = "outJSON";
String outString;



String sendString1(){
   uint32_t current = millis();
   char m1[6] = "Hello";
   double v1 = 5234.999999999;
   uint8_t v2 = 235;
   char m2[5] = "NANO";
   JSONVar jsonVals;
   jsonVals["current"] = current;
   jsonVals["m1"] = m1;
   jsonVals["v1"] = v1;
   jsonVals["v2"] = v2;
   jsonVals["m2"] = m2;
   String y = JSON.stringify(jsonVals);
   return y;
}


void setup() {
  Serial.begin(115200);
  while(!Serial){};
  ssLink.begin(9600);
  myTransfer.begin(ssLink);
  Serial.println("Links are UP and ready\n");
}

void loop() {
  
    if(millis() - previousTime >= 5000){
        outFile += sendString1();
        myTransfer.sendDatum(fileName); // Send filename
        uint16_t numPackets = fileSize / (MAX_PACKET_SIZE - 2);
            if (fileSize % MAX_PACKET_SIZE) // Add an extra transmission if needed
                numPackets++;
                    for (uint16_t i=0; i<numPackets; i++){ // Send all data within the file across multiple packets
                        uint16_t fileIndex = i * MAX_PACKET_SIZE; // Determine the current file index
                        uint8_t dataLen = MAX_PACKET_SIZE - 2;
                            if ((fileIndex + (MAX_PACKET_SIZE - 2)) > fileSize) // Determine data length for the last packet if file length is not an exact multiple of MAX_PACKET_SIZE-2
                                dataLen = fileSize - fileIndex;
                                uint8_t sendSize = myTransfer.txObj(fileIndex); // Stuff the current file index
                                sendSize = myTransfer.txObj(outFile[fileIndex], sendSize, dataLen); // Stuff the current file data
                                myTransfer.sendData(sendSize, 1); // Send the current file index and data
                                delay(100);
                            }
    }
}
