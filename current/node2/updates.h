String scReadings(uint8_t newDoorStates, uint8_t mouseTrap, bool openDoorAlarm){
  JSONVar jsonReadings;
  jsonReadings["node"]        = thisNode;
  jsonReadings["heartBeat"]   = (bool)0;
  jsonReadings["osTemp"]      = (double)0;
  jsonReadings["osHum"]       = (double)0;
  jsonReadings["osPres"]      = (uint16_t)0;
  jsonReadings["tank1Level"]  = (double)0;
  jsonReadings["tank1Liters"] = (uint16_t)0;
  jsonReadings["t1Last24hrs"] = (uint16_t)0;
  jsonReadings["t1AvgUse"]    = (uint16_t)0;
  jsonReadings["t1DaysAvail"] = (double)0;
  jsonReadings["usReadOK"]    = (bool)0;
  jsonReadings["ttReadOK"]    = (bool)0;
  jsonReadings["t1Errors"]    = (uint16_t)0;
  jsonReadings["lightState"]  = (bool)0;
  jsonReadings["doorStates"]  = newDoorStates;
  jsonReadings["mouseTrap"]   = mouseTrap;
  jsonReadings["gDoorAlarm"]  = openDoorAlarm;
  jsonReadings["sLevelAlarm"] = (bool)0;
  jsonReadings["sValvePos"]   = (uint8_t)0;
  jsonReadings["sBulbFail"]   = (bool)0;
  jsonReadings["sPumpFail"]   = (bool)0;
  readings = JSON.stringify(jsonReadings);
  return readings;
}

String hbReadings (uint8_t node, bool heartBeat, double osTemp, double osHum, uint16_t osPres, 
  double tank1Level, uint16_t tank1Liters, uint16_t t1Last24hrs, uint16_t t1AvgUse, double t1DaysAvail,  
  bool usReadOK, bool ttReadOK, bool t1Errors, bool ledState, uint8_t hbDoorStates, uint8_t mouseTrap, 
  bool hbDoorAlarm, bool sLevelAlarm, uint8_t sValvePos, bool sBulbFail, bool sPumpFail) {
  JSONVar jsonReadings;
  jsonReadings["node"]        = node;
  jsonReadings["heartBeat"]   = heartBeat;
  jsonReadings["osTemp"]      = osTemp;
  jsonReadings["osHum"]       = osHum;
  jsonReadings["osPres"]      = osPres;
  jsonReadings["tank1Level"]  = tank1Level;
  jsonReadings["tank1Liters"] = tank1Liters;
  jsonReadings["t1Last24hrs"] = t1Last24hrs;
  jsonReadings["t1AvgUse"]    = t1AvgUse;
  jsonReadings["t1DaysAvail"] = t1DaysAvail;
  jsonReadings["usReadOK"]    = usReadOK;
  jsonReadings["ttReadOK"]    = ttReadOK;
  jsonReadings["t1Errors"]    = t1Errors;
  jsonReadings["lightState"]  = ledState;
  jsonReadings["doorStates"]  = hbDoorStates;
  jsonReadings["mouseTrap"]   = mouseTrap;
  jsonReadings["gDoorAlarm"]  = hbDoorAlarm;
  jsonReadings["sLevelAlarm"] = sLevelAlarm;
  jsonReadings["sValvePos"]   = sValvePos;
  jsonReadings["sBulbFail"]   = sBulbFail;
  jsonReadings["sPumpFail"]   = sPumpFail;
  readings = JSON.stringify(jsonReadings);
  return readings;
}

void hbUpdate(uint8_t node, bool heartBeat, double osTemp, double osHum, uint16_t osPres, double tank1Level, 
     uint16_t tank1Liters, uint16_t t1Last24hrs, uint16_t t1AvgUse, double t1DaysAvail, bool usReadOK, 
     bool ttReadOK, bool t1Errors, bool ledState, uint8_t hbDoorStates, uint8_t mouseTrap, bool hbDoorAlarm,
     bool sLevelAlarm, uint8_t sValvePos, bool sBulbFail, bool sPumpFail){
     String msg = hbReadings(node, heartBeat, osTemp, osHum, osPres, tank1Level, tank1Liters,
                  t1Last24hrs, t1AvgUse, t1DaysAvail, usReadOK, ttReadOK, t1Errors, ledState, 
                  hbDoorStates, mouseTrap, hbDoorAlarm, sLevelAlarm, sValvePos, sBulbFail, sPumpFail);
    mesh.sendSingle(kitchenID , msg );
    Serial.printf("Heartbeat Door State: %u TrapState: %u Door Alarm: %u\n", hbDoorStates, mouseTrap, hbDoorAlarm);
    Serial.printf("Node Update Sent to Node: %u\n\n", kitchenNode);
    digitalWrite(LED, OFF);
    return;
}


void scUpdate(uint8_t newDoorState, uint8_t mouseTrap, bool openDoorAlarm){
    String msg = scReadings(newDoorState, mouseTrap, openDoorAlarm);
    mesh.sendSingle(kitchenID , msg);
    Serial.printf("State Change Door State = %u\n", doorStates);
    Serial.printf("Node Update Sent to Node: %u\n\n", kitchenNode);
    return;
}
