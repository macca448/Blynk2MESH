String getSysAlarms(){
    String ss = "";
    if(systemState == 4){
        ss = "Door Open Too Long!";
    }else if(systemState == 3){
        ss = "Septic Pump Failure";
    }else if(systemState == 2){
        ss = "Septic Bulb Failure";
    }else if(systemState == 1){
        ss = "Septic Level Alarm";
    }else if(systemState == 0){
        ss = "NO Alarms";
    }
    return ss;
}

String getTemps(){
    String tt = "";
    tt += "Room: ";
    tt += String(roomTemp, 1);
    tt += "&#176;C  Yard: ";
    tt += String(osTemp, 1);
    tt += "&#176;C";
    return tt;
}

String getHums(){
  String hh = "";
  hh += "Room: ";
  hh += String(roomHum, 1);
  hh += "%  Yard: ";
  hh += String(osHum, 1);
  hh += "%";
  return hh;
}

String getPres(){
  String pp = "";
  pp += "Room: ";
  pp += (roomPressure);
  pp += "hPa  Yard: ";
  pp += (osPres);
  pp += "hPa";
  return pp;
}

String getTankLevel(){
  return String(tank1Level, 1);  
}

String getMyTime(){
  String bt = "";
  char tBuff[24];
  sprintf(tBuff, "\n%02u:%02u:%02u  %02u/%02u/%02u", hour(),minute(),second(),day(),month(),year());
  bt += (tBuff);
  return bt;
}

String getErrors(){
    String ee = "";
    ee += "Time-Outs: ";
    ee += (t1Errors);
    if(usReadOK == 1){
        ee += "  US OK";
    }else{
        ee += "  US FAIL";  
    }
    if(ttReadOK == 1){
        ee += "  TT OK";
    }else{
        ee += "  TT FAIL";  
    }
    return ee;
}

String getLightState(){
    String ls = "";
    if(lastLightState == 0){
      ls += " OFF";
    }else{
      ls += " ON";
    }
  return ls;
}

String getDoorState(){
    String ds = "";
    const char op[] = "OPENED";
    const char cl[] = "CLOSED";
    char doors_buf[50];
        if(doorStates == 0x7){                    //0b111
            //All Doors Open
            sprintf(doors_buf," Internal: %s, Ranch: %s, Garage: %s", op, op, op);
        }else if(doorStates == 0x6){              //0b110
            //Internal and Ranch Doors Open
            sprintf(doors_buf,"Internal: %s, Ranch: %s, Garage: %s", op, op, cl);
        }else if(doorStates == 0x5){              //0b101
            //Garage and Internal Doors Open
            sprintf(doors_buf,"Internal: %s, Ranch: %s, Garage: %s", op, cl, op);
        }else if(doorStates == 0x3){              //0b011
            //Garage and Ranch Doors Open
            sprintf(doors_buf,"Internal: %s, Ranch: %s, Garage: %s", cl , op, op);
        }else if(doorStates == 0x2){              //0b010
            //Ranch Door Open
            sprintf(doors_buf,"Internal: %s, Ranch: %s, Garage: %s", cl, op, cl);
        }else if(doorStates == 0x1){              //0b001
            //Garage Door Open
            sprintf(doors_buf,"Internal: %s, Ranch: %s, Garage: %s", cl, cl, op);
        }else if(doorStates == 0x4){              //0b100
            //Internal Door Open
            sprintf(doors_buf,"Internal: %s, Ranch: %s, Garage: %s", op, cl, cl);
        }else if(doorStates == 0x0){
            //All Garage Doors Closed
            sprintf(doors_buf,"Internal: %s, Ranch: %s, Garage: %s", cl, cl, cl);
        }
        ds += doors_buf;
        return ds;
}

String getMtState(){
    String mtNow = "";
    char mt_buf[20];
    const char m1[] = "mouse";
    const char m2[] = "mice";
        if(mouseTrap == 1){
            sprintf(mt_buf,"%u %s in trap", mouseTrap, m1);
        }else{
            sprintf(mt_buf,"%u %s in trap", mouseTrap, m2);
        }
    mtNow += mt_buf;
    return mtNow;
}

String getGdAlarm(){
    String gda = "";
    if(gDoorAlarm == 0){
        gda += "No Door Alarm";
    }else{
        gda += "Door Open Alarm";
    }
    return gda;
}

String getSepticAlarms(){
    String sa = "";
    if(sLevelAlarm == 0){
        sa += " Level OK";
    }else{
        sa += " Level Alarm ";
    }
    if(sBulbFail == 0){
        sa += " Lamp OK ";
    }else{
        sa += " Lamp FAIL ";
    }
    if(sPumpFail == 0){
        sa += " Pumps OK ";
    }else{
        sa += " Pump FAIL ";
    }
    return sa;
}

String getValvePos(){
    String valve = "";
    if(lastSvalPos == NORMAL){
        valve += " NORMAL";
    }else{
        valve += " BYPASS";
    }
    return valve;
}

String processor(const String& var){
    if(var == "SYSTEMALARMS"){
        return getSysAlarms();
    }else if(var == "TEMPS"){
        return getTemps();
    }else if(var == "HUMS"){
        return getHums();
    }else if(var == "PRES"){
        return getPres();
    }else if(var == "LEVEL"){
        return getTankLevel();
    }else if(var == "BLYNKTIME"){
        return getMyTime();
    }else if(var == "ERRORS"){
        return getErrors();
    }else if(var == "LIGHT"){
        return getLightState();
    }else if(var == "DOORS"){
        return getDoorState();
    }else if(var == "MOUSE"){
        return getMtState();
    }else if(var == "DOORALARM"){
        return getGdAlarm();
    }else if(var == "SEPTICALARMS"){
        return getSepticAlarms();
    }else if(var == "VALVEPOS"){
        return getValvePos();
    }
}
