void checkMSG(String &msg){
    String inMSG = msg.c_str();
    JSONVar myObject     = JSON.parse(msg.c_str());
          uint8_t  node        = myObject["node"];
          bool     heartBeat   = myObject["heartBeat"];
          double   osTemp      = myObject["osTemp"];
          double   osHum       = myObject["osHum"];
          uint16_t osPres      = myObject["osPres"];          
          double   tank1Level  = myObject["tank1Level"];
          double   tank1Liters = myObject["tank1Liters"];
          uint16_t t1Last24hrs = myObject["t1Last24hrs"];
          uint16_t t1AvgUse    = myObject["t1AvgUse"];
          double   t1DaysAvail = myObject["t1DaysAvail"];
          bool     usReadOK    = myObject["usReadOK"];
          bool     ttReadOK    = myObject["ttReadOK"];
          uint16_t t1Errors    = myObject["t1Errors"];
          bool     lightState  = myObject["lightState"];
          uint8_t  ds          = myObject["doorStates"];
          uint8_t  mouseTrap   = myObject["mouseTrap"];
          bool     gDoorAlarm  = myObject["gDoorAlarm"];
          bool     sLevelAlarm = myObject["sLevelAlarm"];
          uint8_t  sValvePos   = myObject["sValvePos"];
          bool     sBulbFail   = myObject["sBulbFail"];
          bool     sPumpFail   = myObject["sPumpFail"];
        if(heartBeat){
            node = thisNode;
            uint8_t hbDoorStates = getDoorStates();
            mouseTrap = lastMTstate;
            gDoorAlarm = doorAlarm;
            
            hbUpdate(node, heartBeat, osTemp, osHum, osPres, tank1Level, tank1Liters, t1Last24hrs, 
                     t1AvgUse, t1DaysAvail, usReadOK, ttReadOK, t1Errors, lightState, hbDoorStates, 
                     mouseTrap, gDoorAlarm, sLevelAlarm, sValvePos, sBulbFail, sPumpFail);
            return;
        }                
}
