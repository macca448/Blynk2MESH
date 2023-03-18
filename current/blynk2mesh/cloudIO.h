void adjustGauges(void){
    uint8_t hpaPos = map(osPres, 870, 1090, 0, 180);
    uint8_t levelPos = ((double)tank1Level + 0.55);
    levelPos = map(levelPos, 0, 100, 0, 180);
    level_svo.attach(LEV_SVO);
    hpa_svo.attach(HPA_SVO);
    level_svo.write(levelPos);
    hpa_svo.write(hpaPos);
    gaugeTimer = 0;
    gaugeDelay = true;
    return;
}

void updateCloud(uint8_t node, bool heartBeat, bool lightState, uint8_t sValvePos){
    digitalWrite(LED, ON);
    bool doT1tank = false, doSeptic = false, doGarage = false;
    if(heartBeat){
        doT1tank = true; doSeptic = true; doGarage = true;
    }else{
        if(node == tank1Node){
            doT1tank = true;
        }else if(node == garageNode){
            doGarage = true;
        }else if(node == septicNode){
            doSeptic = true;
        }
    }
    if(doT1tank){
        Blynk.virtualWrite(V0, tank1Liters);  //(uint16_t) Estinated Liters of Water Available
        Blynk.virtualWrite(V1, lightState);  //(uint16_t) Liters of water used in the last 24hrs
        Blynk.virtualWrite(V8, t1DaysAvail);  //(string/double)   Estimated days of water remaining
    }
    if(doGarage){
        String dsNow = getDoorState();
        Blynk.virtualWrite(V5, dsNow);
        String mtNow = getMtState();
        Blynk.virtualWrite(V6, mtNow);    //MouseTrap
    }
    if(doSeptic){
        Serial.printf("Servo Sate is now: %u\n\n", sValvePos);
        if(sValvePos == NORMAL){
            String norm = "NORMAL";
            Blynk.virtualWrite(V7, norm);
        }else{
            String bp = "BYPASS";
            Blynk.virtualWrite(V7, bp);
        } 
        
    }
    Blynk.virtualWrite(V9, systemState);
    Serial.println("Blynk Cloud Update Sucess!!\n\n");
    serialInOn = true;
    adjustGauges();
    return;
}

BLYNK_WRITE(V2){            //Tank Light
  digitalWrite(LED, ON);
  uint8_t pinValue = param.asInt();
  uint8_t sendSize = 0;  
      if (pinValue == 1){
          txStruct.dest = tank1Node;
          txStruct.epoch = 0;
          txStruct.doorPulse = 0;
          lastLightState = !lastLightState;              
          txStruct.tankLight = lastLightState;
          txStruct.sValvePos = 0;
          sendSize = myTransfer.txObj(txStruct, sendSize);
          myTransfer.sendData(sendSize);    
          Serial.println("Message Send Success!!\n");
          digitalWrite(LED, OFF);    
      }
}

BLYNK_WRITE(V3){               //Garage Door
  digitalWrite(LED, ON);
  uint8_t pinValue = param.asInt();
  uint8_t sendSize = 0; 
      if (pinValue == 1){
          txStruct.dest = garageNode;
          txStruct.epoch = 0;
          txStruct.doorPulse = 1;
          txStruct.tankLight = 0;
          txStruct.sValvePos = 0;
          sendSize = myTransfer.txObj(txStruct, sendSize);
          myTransfer.sendData(sendSize);    
          Serial.println("Message Send Success!!\n");
          digitalWrite(LED, OFF);
      }
}

BLYNK_WRITE(V4){               //Septic Valve
  digitalWrite(LED, ON);
  uint8_t pinValue = param.asInt();
  uint8_t sendSize = 0; 
      if (pinValue == 1){
          txStruct.dest = septicNode;
          txStruct.epoch = now();
          txStruct.doorPulse = 0;
              if(lastSvalPos == NORMAL){
                  txStruct.sValvePos = BYPASS;
              }else{
                  txStruct.sValvePos = NORMAL;
              }
          lastSvalPos = txStruct.sValvePos;
          sendSize = myTransfer.txObj(txStruct, sendSize);
          myTransfer.sendData(sendSize);    
          Serial.println("Message Send Success!!\n");
          digitalWrite(LED, OFF);
      }
}
