void sendAK(uint32_t msgTo) {
  String msg = "  MSG OK";
  mesh.sendSingle( msgTo, msg );
  Serial.printf("Receipt sent to: %u\n\n", msgTo);
  return;
}

void receivedCallback( uint32_t from, String &msg ) {
  digitalWrite(LED, ON);
  String inString = msg.c_str();
      if(inString != "  MSG OK"){
        if(from == kitchenID){
          JSONVar myObject = JSON.parse(msg.c_str());
          uint8_t dest      = myObject["dest"];
          uint32_t epoch    = myObject["epoch"];
          bool    doorPulse = myObject["doorPulse"];
          bool    lightState  = myObject["lightState"];
          uint8_t servoPos  = myObject["servoPos"];
              if(lightState != lastLightState){
                  digitalWrite(LIGHT, lightState);
                  lastLightState = lightState;
                  Serial.printf("Led State Now is: %u\n\n", lastLightState);
                  doStateUpDate = true;
                  previousTime = millis();
              }
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
      }else if(from == septicID){
          JSONVar myObject = JSON.parse(msg.c_str());
          uint8_t  node        = myObject["node"];
          bool     heartBeat   = myObject["heartBeat"];
          double   osT         = myObject["osTemp"];
          double   osH         = myObject["osHum"];
          uint16_t osP         = myObject["osPres"];          
          double   tank1Le     = myObject["tank1Level"];
          double   tank1Li     = myObject["tank1Liters"];
          uint16_t t1Last24    = myObject["t1Last24hrs"];
          uint16_t t1au        = myObject["t1AvgUse"];
          double   t1Days      = myObject["t1DaysAvail"];
          bool     usRe        = myObject["usReadOK"];
          bool     ttR         = myObject["ttReadOK"];
          uint16_t t1E         = myObject["t1Errors"];
          bool     ls          = myObject["lightState"];
          uint8_t  ds          = myObject["doorStates"];
          uint8_t  mt          = myObject["mouseTrap"];
          bool     gDoorAlarm  = myObject["gDoorAlarm"];
          bool     sLevelAlarm = myObject["sLevelAlarm"];
          uint8_t  sValvePos   = myObject["sValvePos"];
          bool     sBulbFail   = myObject["sBulbFail"];
          bool     sPumpFail   = myObject["sPumpFail"];
          uint8_t  systemState = myObject["systemState"];
          //add local sensor valuse to heartbeaat MSG
          heartBeatMessage(heartBeat, sLevelAlarm, sValvePos, sBulbFail, sPumpFail);
      }
      sendAK(from);
    }else{
      #ifdef PRINT
        Serial.printf("Received from %u msg=%s\n\n", from, msg.c_str());
      #endif
    }
  digitalWrite(LED, OFF);
}

#ifdef PRINT
void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    Serial.printf("Changed connections\n");
    doStateUpDate = true;
    previousTime = millis();
}

void nodeTimeAdjustedCallback(int32_t offset) { 
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
    doStateUpDate = true;
    previousTime = millis();
}
#endif
