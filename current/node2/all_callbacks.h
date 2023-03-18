void sendAK(uint32_t msgTo) {
  String msg = "  MSG OK";
  mesh.sendSingle( msgTo, msg );
  Serial.printf("Receipt sent to: %u\n", msgTo);
  Serial.println("");
  return;
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
    digitalWrite(LED, ON);
    //Serial.printf("Received from %u msg= %s\n", from, msg.c_str());
    String inMSG = msg.c_str();
        if(inMSG == "  MSG OK"){
            Serial.printf("%s from %u\n", inMSG, from);
            digitalWrite(LED, OFF);
        }else{
            if(from == kitchenID){
                JSONVar myObject = JSON.parse(msg.c_str());
                uint8_t   dest        = myObject["dest"];
                uint32_t  epoch       = myObject["epoch"];                
                          doorPulse   = myObject["doorPulse"];
                bool      lightState  = myObject["lightState"];
                uint8_t   servoPos    = myObject["servoPos"];                          
                
                Serial.println("New In Message");
                    if(dest == thisNode){
                        if(doorPulse){
                            digitalWrite(RELAY, HIGH);
                            Serial.println("Doing Door Pulse");
                            delayCount = 0;                            
                        }
                    }else if(dest == all){
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
            }else if(from == tank1ID){
                checkMSG(inMSG);
            }
            sendAK(from);
        }
    digitalWrite(LED, OFF);
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
