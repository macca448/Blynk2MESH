void readBME(void){
  roomTemp = bme.readTemperature();
  roomHum = bme.readHumidity();
  roomPressure = round((bme.readPressure()/100.0F) + ALT_OFFSET);
  return;
}

void sendOK(uint32_t from) {
  String msg = "  MSG OK";
  mesh.sendSingle( from, msg );
  Serial.printf("Message Receipt sent to: %u\n", from);
  Serial.println("");
  return;
}

void blynkUpdate(uint8_t node, bool heartBeat, bool lightState, uint8_t systemState){
  uint16_t sendSize = 0;
  txStruct.node         = node;
  txStruct.heartBeat    = heartBeat;
  txStruct.osTemp       = osTemp;
  txStruct.roomTemp     = roomTemp;
  txStruct.osHum        = osHum;
  txStruct.roomHum      = roomHum;
  txStruct.osPres       = osPres;
  txStruct.roomPressure = roomPressure;
  txStruct.tank1Level   = tank1Level;
  txStruct.tank1Liters  = tank1Liters;
  txStruct.t1Last24hrs  = t1Last24hrs;
  txStruct.t1AvgUse     = t1AvgUse;  
  txStruct.t1DaysAvail  = t1DaysAvail;
  txStruct.usReadOK     = usReadOK;
  txStruct.ttReadOK     = ttReadOK;
  txStruct.t1Errors     = t1Errors;  
  txStruct.lightState   = lightState;
  txStruct.doorStates   = doorStates;
  txStruct.mouseTrap    = mouseTrap;
  txStruct.gDoorAlarm   = gDoorAlarm;
  txStruct.sLevelAlarm  = sLevelAlarm;
  txStruct.sValvePos    = sValvePos;
  txStruct.sBulbFail    = sBulbFail;
  txStruct.sPumpFail    = sPumpFail;
  txStruct.systemState  = systemState;
  sendSize = myTransfer.txObj(txStruct, sendSize);
  myTransfer.sendData(sendSize);
  Serial.printf("Serial Transfer bytes: %u\n\n", sendSize);
  digitalWrite(LED_PIN, OFF);
  return;
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  digitalWrite(LED_PIN, ON);
  String inMSG = msg.c_str();
      if(inMSG != "  MSG OK"){
          JSONVar myObject = JSON.parse(msg.c_str());
 uint8_t  node        = myObject["node"];
 bool     heartBeat   = myObject["heartBeat"];
          osTemp      = myObject["osTemp"];
          osHum       = myObject["osHum"];
          osPres      = myObject["osPres"];          
          tank1Level  = myObject["tank1Level"];
          tank1Liters = myObject["tank1Liters"];
          t1Last24hrs = myObject["t1Last24hrs"];
          t1AvgUse    = myObject["t1AvgUse"];
          t1DaysAvail = myObject["t1DaysAvail"];
          usReadOK    = myObject["usReadOK"];
          ttReadOK    = myObject["ttReadOK"];
          t1Errors    = myObject["t1Errors"];
 bool     lightState  = myObject["lightState"];
          doorStates  = myObject["doorStates"];
          mouseTrap   = myObject["mouseTrap"];
          gDoorAlarm  = myObject["gDoorAlarm"];
          sLevelAlarm = myObject["sLevelAlarm"];
          sValvePos   = myObject["sValvePos"];
          sBulbFail   = myObject["sBulbFail"];
          sPumpFail   = myObject["sPumpFail"];
          uint8_t  systemState = 0;
          if(node == tank1Node || heartBeat){
              lastLightState = lightState;
          }
          if(node == septicNode || heartBeat){
              lastServoPos = sValvePos;
                  if(sLevelAlarm){
                      systemState = 1;
                  }else if(sBulbFail){
                      systemState = 2;
                  }else if(sPumpFail){
                      systemState = 3;
                  }
          }
          if(node == garageNode || heartBeat){
              if(gDoorAlarm){
                  systemState = 4;
              }
          }
          readBME();
              if(heartBeat){                
                  Serial.println("REMOTE READINGS START ");
                  Serial.print("Node: ");
                  Serial.println(node);
                  Serial.print("Temperature: ");
                  Serial.print(osTemp);
                  Serial.println(" C");
                  Serial.print("Humidity: ");
                  Serial.print(osHum);
                  Serial.println(" %");
                  Serial.print("Pressure: ");
                  Serial.print(osPres);
                  Serial.println(" hpa");
                  Serial.print("TANK Level: ");
                  Serial.print(tank1Level, 1);
                  Serial.print(" %   ");
                  Serial.print("TT Sensor OK: ");
                  Serial.print(ttReadOK);
                  Serial.print("  Ultra Success: ");
                  Serial.println(usReadOK);
                  Serial.print("Tank Read Errors: ");
                  Serial.println(t1Errors);
                  Serial.print("Tank Light State: ");
                  Serial.println(lightState);                  
                  Serial.printf("Garage door state: %u\n",  doorStates);
                  Serial.printf("Current Servo Pos: %u\n",  sValvePos);
              }else{
                  if(node == septicNode){
                      Serial.println("REMOTE READINGS START ");
                      Serial.printf("Septic Level State: %u\n", sLevelAlarm);
                      Serial.printf("Septic Valve Pos: %u\n",  sValvePos);
                      Serial.printf("Septic Bulb State: %u\n",  sBulbFail);
                      Serial.printf("Septic Pump State: %u\n",  sPumpFail);
                      Serial.println("END REMOTE READINGS\n");
                  }else if(node == tank1Node){
                      Serial.println("Message From Tank1");
                      Serial.printf("Tank Light State: %u\n",  lightState);
                      Serial.printf("Local Light State: %u\n",  lastLightState);
                      Serial.println("End Message\n");                   
                  }else if(node == garageNode){
                      Serial.println("MESH GARAGE UPDATE ");
                      Serial.printf("Garage door state: %u\n",  doorStates);
                      Serial.println("END REMOTE READINGS\n");
                  }
              }
          blynkUpdate(node, heartBeat, lightState, systemState);            
          sendOK(from); 
      }else{             
          Serial.println(inMSG + ((String)"\n"));
          digitalWrite(LED_PIN, OFF);
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
