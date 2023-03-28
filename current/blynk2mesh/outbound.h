
//This sens a Time update to all the MESH nodes
void udMeshTime(void){
    digitalWrite(LED, ON);
    uint8_t sendSize = 0;
    txStruct.dest       = all;
    txStruct.epoch      = now();
    txStruct.doorPulse  = 0;
    txStruct.tankLight = 0;
    txStruct.sValvePos = 0;
    Serial.printf("Message Sent to Node: %u\n", txStruct.dest);
    sendSize = myTransfer.txObj(txStruct, sendSize);
    myTransfer.sendData(sendSize);
    char tBuff[24];
    sprintf(tBuff, "\n%02u:%02u:%02u  %02u/%02u/%02u", hour(),minute(),second(),day(),month(),year());
    Serial.println(tBuff);
    Serial.println("Message Send Success!!\n");
    digitalWrite(LED, OFF);
    return;  
}

//Below was initially used in testing using Serial Monitor and No Blynk Setup
/*
void outboundVals(void){ 
  while(Serial.available()) {
      digitalWrite(LED, HIGH);
      uint8_t sendSize = 0;
      String outStr=Serial.readStringUntil('\n');
      uint8_t outVal = outStr.toInt();
          if(outVal == 0){
              txStruct.dest = 3;
              txStruct.doorPulse = 0;
              txStruct.servoPos = currentPos;
              txStruct.ledState = 0;
          }else if(outVal == 1){
              txStruct.dest = 3;
              txStruct.doorPulse = 0;
              txStruct.servoPos = currentPos;
              txStruct.ledState = 1;
          }else if(outVal == 2){
              txStruct.dest = 2;
              txStruct.doorPulse = 1;
              txStruct.servoPos = currentPos;
              txStruct.ledState = 0;
          }else if(outVal == 3){
              txStruct.dest = 2;
              txStruct.doorPulse = 0;
              if(currentPos == 0){
                  txStruct.servoPos = 180;
                  currentPos = 180;
              }else{
                  txStruct.servoPos = 0;
                  currentPos = 0;
              }          
          }else{
              Serial.println("Invalid Value - Please Try Again");
              Serial.println("Valid Commands are 0 to 5\n");
              return;
          }
      Serial.printf("Message Sent to Node: %u\n", txStruct.dest);
      sendSize = myTransfer.txObj(txStruct, sendSize);
      myTransfer.sendData(sendSize);
      Serial.println("Message Send Success!!\n");
      digitalWrite(LED, LOW);
  }
  return;
} */
