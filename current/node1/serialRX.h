
String getOutVals(uint8_t dest, uint32_t epoch, bool doorPulse, bool lightState, uint8_t servoPos){
  JSONVar jsonVals;
  jsonVals["dest"] = dest;
  jsonVals["epoch"] = epoch;
  jsonVals["doorPulse"] = doorPulse;
  jsonVals["lightState"] = lightState;
  jsonVals["sValvePos"] = servoPos;
  String y = JSON.stringify(jsonVals);
  return y;
}

void outbound(uint8_t dest, uint32_t epoch, bool doorPulse, bool lightState, uint8_t servoPos){
  String outMSG = getOutVals(dest, epoch, doorPulse, lightState, servoPos);
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
  rotate = false;
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("\n");
  display.println("Vals Sent To Node: ");
  display.println("");
      if(dest == all){
          display.println("ALL NODES");
      }else if(dest == septicNode){
          display.println("SEPTIC NODE");
          if(servoPos != lastServoPos){
              lastServoPos = servoPos;
              if(servoPos == 0){
                  display.println("Valve Pos: NORMAL\n");
              }else{
                  display.println("Valve Pos: BYPASS\n");
              }
          }
      }else if(dest == tank1Node){
          display.println("TANK NODE");
          if(lightState != lastLightState){
              lastLightState = lightState;
          }
          if(lightState){
              display.println("Tank Light is: ON\n");
          }else{
             display.println("Tank Light is: OFF\n");
          } 
      }else if(dest == garageNode){
          display.println("GARAGE NODE");
      }
  char tBuff[24];
  sprintf(tBuff, "%02u:%02u:%02u  %02u/%02u/%02u", hour(),minute(),second(),day(),month(),year());
  display.println(tBuff);
  display.display();
  rotateTimer = 0;  
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
      bool lightState = rxStruct.tankLight;      
      uint8_t servoPos = rxStruct.sValvePos;
    
      Serial.println("Inbound Serial Start");      
      Serial.print("Dest:\t");
      Serial.println(dest);
      Serial.print("Move Door:\t");
      Serial.println(doorPulse);
      Serial.print("Servo:\t");
      Serial.println(servoPos);
      Serial.print("LedState:\t");
      Serial.println(lightState);
      Serial.println("Inbound Serial End\n");
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
      }
     outbound(dest, epoch, doorPulse, lightState, servoPos);              
   digitalWrite(LED_PIN, OFF);     
  }
  return;
}
