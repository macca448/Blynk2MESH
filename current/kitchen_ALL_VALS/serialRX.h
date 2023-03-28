
String getOutVals(uint8_t dest, uint32_t epoch, bool doorPulse, bool newLightState, uint8_t newValvePos){
  JSONVar jsonVals;
  jsonVals["dest"] = dest;
  jsonVals["epoch"] = epoch;
  jsonVals["doorPulse"] = doorPulse;
  jsonVals["lightState"] = newLightState;
  jsonVals["sValvePos"] = newValvePos;
  String y = JSON.stringify(jsonVals);
  return y;
}

void outbound(uint8_t dest, uint32_t epoch, bool doorPulse, bool newLightState, uint8_t newValvePos){
  String outMSG = getOutVals(dest, epoch, doorPulse, newLightState, newValvePos);
  uint32_t destID = 0;
  bool single = false;
      if(dest == garageNode){
          destID = garageID;
          single = true;
      }else if(dest == tank1Node){
          destID = tank1ID;
          single = true;
      }else if(dest == septicNode){
          destID = septicID;
          single = true;
      }else if(dest == all){
         single = false;
      }
      if(single){
          mesh.sendSingle(destID, outMSG);
      }else{
          mesh.sendBroadcast(outMSG);
      }
  lastDest = dest;
  return;
}

void serialRX(void){
  //Inbound Serial 
  if(myTransfer.available()){
      digitalWrite(LED_PIN, ON);
      uint16_t recSize = 0;
      recSize = myTransfer.rxObj(rxStruct, recSize);
      uint8_t dest = rxStruct.dest;
      uint32_t epoch = rxStruct.epoch;
      bool doorPulse = rxStruct.doorPulse;
      bool newLightState = rxStruct.tankLight;      
      uint8_t newValvePos = rxStruct.sValvePos;
    
      Serial.println("Inbound Serial Start");      
      Serial.print("Dest:\t");
      Serial.println(dest);
      Serial.print("Move Door:\t");
      Serial.println(doorPulse);
      Serial.print("Servo:\t");
      Serial.println(lastServoPos);
      Serial.print("LedState:\t");
      Serial.println(lastLightState);
      Serial.println("Inbound Serial End\n");
      if(dest == all){
          if(epoch > SECS_YR_2000){             //Check that the new time is valid
              setTime(epoch);
              char tBuff[24];
              sprintf(tBuff, "%02u:%02u:%02u  %02u/%02u/%02u", hour(),minute(),second(),day(),month(),year());
              Serial.printf("Time Updated from NTP: %s\n", tBuff);
          }else{
              Serial.println("FAILED to Sync Time............\n");
          }
      }
     outbound(dest, epoch, doorPulse, newLightState, newValvePos);              
   digitalWrite(LED_PIN, OFF);     
  }
  return;
}
