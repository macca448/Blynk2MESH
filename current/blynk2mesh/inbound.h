void inboundVals(void){
     if(myTransfer.available()){
        digitalWrite(LED, ON);
        uint16_t recSize = 0;
        recSize = myTransfer.rxObj(rxStruct, recSize);
        uint8_t   node = rxStruct.node;
        bool      heartBeat = rxStruct.heartBeat;
                  osTemp = rxStruct.osTemp;
                  roomTemp = rxStruct.roomTemp;
                  osHum = rxStruct.osHum;
                  roomHum = rxStruct.roomHum;
                  osPres = rxStruct.osPres;
                  roomPressure = rxStruct.roomPressure;
                  tank1Level = rxStruct.tank1Level;
                  tank1Liters = rxStruct.tank1Liters;
                  t1Last24hrs = rxStruct.t1Last24hrs;
                  t1AvgUse = rxStruct.t1AvgUse;
                  t1DaysAvail = rxStruct.t1DaysAvail;
                  usReadOK = rxStruct.usReadOK;
                  ttReadOK = rxStruct.ttReadOK;
                  t1Errors = rxStruct.t1Errors;
        bool      lightState = rxStruct.lightState;
                  doorStates = rxStruct.doorStates;
                  mouseTrap = rxStruct.mouseTrap;
                  gDoorAlarm = rxStruct.gDoorAlarm;
                  sLevelAlarm = rxStruct.sLevelAlarm;
        uint8_t   sValvePos = rxStruct.sValvePos;
                  sBulbFail = rxStruct.sBulbFail;
                  sPumpFail = rxStruct.sPumpFail;
                systemState = rxStruct.systemState;    
          if(node == garageNode){
              Serial.println("Message Received from Garage (Node: 2)");
              Serial.printf("Garage Door State is: %u", doorStates); 
          }
          if(node == tank1Node){
              Serial.println("Message Received from Tank1 (Node: 3)");      
              Serial.print("Liters:\t\t");Serial.println(tank1Liters);
              Serial.print("Days of Water Avail:\t");Serial.println(t1DaysAvail ,1);
              Serial.print("Light State:\t\t"); Serial.println(lightState);
              Serial.println("\n");
              lastLightState = lightState;
          }
          if(node == septicNode){
              Serial.println("Message Received from Septic Tank (Node: 4)");
              Serial.print("Septic Valve Pos:\t");Serial.print(sValvePos);
              lastSvalPos = sValvePos;
          }
          Serial.printf("System State: %u \n\n", systemState);
          Serial.printf("Bytes received: %u \n\n", recSize);
          serialInOn = false;
          updateCloud(node, heartBeat, lightState, sValvePos);
          prevTime = millis();
          digitalWrite(LED, OFF);
          return;
  }
}
