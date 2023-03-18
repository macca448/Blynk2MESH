String getHbReadings (bool heartBeat, bool sLevelAlarm, uint8_t sValvePos, bool sBulbFail, bool sPumpFail) {
  wx();
  ultraSonic();
  t1DaysAvail = ((double)tank1Liters / t1AvgUse);
  lastLightState = digitalRead(LIGHT);
  JSONVar jsonReadings;
  jsonReadings["node"]        = thisNode;
  jsonReadings["heartBeat"]   = heartBeat;
  jsonReadings["osTemp"]      = osTemp;
  jsonReadings["osHum"]       = osHum;
  jsonReadings["osPres"]      = osPressure;
  jsonReadings["tank1Level"]  = tank1Level;
  jsonReadings["tank1Liters"] = tank1Liters;
  jsonReadings["t1Last24hrs"] = t1Last24hrs;
  jsonReadings["t1AvgUse"]    = t1AvgUse;
  jsonReadings["t1DaysAvail"] = t1DaysAvail;
  jsonReadings["usReadOK"]    = usReadOK;
  jsonReadings["ttReadOK"]    = ttReadOK;
  jsonReadings["t1Errors"]    = readErrors;
  jsonReadings["lightState"]  = lastLightState;
  jsonReadings["doorStates"]  = (int8_t)0;
  jsonReadings["mouseTrap"]   = (int8_t)0;
  jsonReadings["gDoorAlarm"]  = (bool)0;
  jsonReadings["sLevelAlarm"] = sLevelAlarm;
  jsonReadings["sValvePos"]   = sValvePos;
  jsonReadings["sBulbFail"]   = sBulbFail;
  jsonReadings["sPumpFail"]   = sPumpFail;
  readings = JSON.stringify(jsonReadings);
  return readings;
}

String getScReadings () {
  wx();
  ultraSonic();
  t1DaysAvail = ((double)tank1Liters / t1AvgUse);
  lastLightState = digitalRead(LIGHT);
  JSONVar jsonReadings;
  jsonReadings["node"]        = thisNode;
  jsonReadings["heartBeat"]   = (bool)0;
  jsonReadings["osTemp"]      = osTemp;
  jsonReadings["osHum"]       = osHum;
  jsonReadings["osPres"]      = osPressure;
  jsonReadings["tank1Level"]  = tank1Level;
  jsonReadings["tank1Liters"] = tank1Liters;
  jsonReadings["t1Last24hrs"] = t1Last24hrs;
  jsonReadings["t1AvgUse"]    = t1AvgUse;
  jsonReadings["t1DaysAvail"] = t1DaysAvail;
  jsonReadings["usReadOK"]    = usReadOK;
  jsonReadings["ttReadOK"]    = ttReadOK;
  jsonReadings["t1Errors"]    = readErrors;
  jsonReadings["lightState"]  = lastLightState;
  jsonReadings["doorStates"]  = (uint8_t)0;
  jsonReadings["mouseTrap"]   = (int8_t)0;
  jsonReadings["gDoorAlarm"]  = (bool)0;
  jsonReadings["sLevelAlarm"] = (bool)0;
  jsonReadings["sValvePos"]   = (uint8_t)0;
  jsonReadings["sBulbFail"]   = (bool)0;
  jsonReadings["sPumpFail"]   = (bool)0;
  readings = JSON.stringify(jsonReadings);
  return readings;
}

void heartBeatMessage(bool heartBeat, bool sLevelAlarm, uint8_t sValvePos, bool sBulbFail, bool sPumpFail){
    String msg = getHbReadings(heartBeat, sLevelAlarm, sValvePos, sBulbFail, sPumpFail);
    mesh.sendSingle(garageID, msg);
    Serial.println("HeatBeat MSG sent to Garage Node\n");
    digitalWrite(LED, OFF);
}

void sendMessage(void){
    digitalWrite(LED, ON);
    String msg = getScReadings();
    mesh.sendSingle(kitchenID, msg);   
  #ifdef PRINT
    Serial.println("");
    Serial.println("LOCAL READINGS ");
    Serial.print("Node: ");
    Serial.println(thisNode);
    Serial.print("Temperature: ");
    Serial.print(osTemp);
    Serial.println(" C");
    Serial.print("Humidity: ");
    Serial.print(osHum);
    Serial.println(" %");
    Serial.print("Pressure: ");
    Serial.print(osPressure);
    Serial.println(" hpa");
    Serial.print("TANK Level: ");
    Serial.print(distance);
    Serial.print(" mm  ||  Ultra Read: ");  
    Serial.println(usReadOK);
    Serial.print("Tank Temp: ");
    Serial.print(tankTemp);
    Serial.print(" *C  ||  Tank Temp Sensor OK: ");
    Serial.println(ttReadOK);
    Serial.println("END LOCAL READINGS ");
  #endif
    lastDist = distance;
    lastLevel = tank1Level;
    lastLiters = tank1Liters;
    return; 
}
