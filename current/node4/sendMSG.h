void sendAK(uint32_t msgTo) {
  String msg = "  MSG OK";
  mesh.sendSingle( msgTo, msg );
  Serial.printf("Message Received Receipt sent to: %u\n", msgTo);
  Serial.println("");
  digitalWrite(LED, OFF);
}

String getReadings (bool heartBeat) {
  JSONVar jsonReadings;
  jsonReadings["node"]        = thisNode;
  jsonReadings["heartBeat"]   = heartBeat;
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
  jsonReadings["t1Errors"]    = (bool)0;
  jsonReadings["lightState"]  = (bool)0;
  jsonReadings["doorStates"]  = (uint8_t)0;
  jsonReadings["mouseTrap"]   = (int8_t)0;
  jsonReadings["gDoorAlarm"]  = (bool)0;
  jsonReadings["sLevelAlarm"] = sLevelAlarm;
  jsonReadings["sValvePos"]   = moveTo;
  jsonReadings["sBulbFail"]   = sBulbFail;
  jsonReadings["sPumpFail"]   = sPumpFail;
  readings = JSON.stringify(jsonReadings);
  return readings;
}

void heartbeatSend(void){
    digitalWrite(LED, ON);
    bool heartBeat = true;
    String msg = getReadings(heartBeat);
    mesh.sendSingle(tank1ID, msg);
    digitalWrite(LED, OFF);
}

void stateChangeSend(void){
    digitalWrite(LED, ON);
    bool heartBeat = false;
    String msg = getReadings(heartBeat);
    mesh.sendSingle(kitchenID, msg);
    digitalWrite(LED, OFF);
    return;
}
