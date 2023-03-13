

void mainActions(){
  digitalWrite(LED, ON);              //Visually see how long the loop takes
  //Set Blynk MODE string IE Always ON for OTA programming or sleep mode
  if(pgm){
    pgm_mode = ("true");
    digitalWrite(SENSOR_GND, HIGH);
    delay(500);
  }else{
    pgm_mode = ("false");
  }
  //Read SPIFFS Stored Values
  readSavedVals();
  //Take new sensor readings
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float pres = round((bme.readPressure()/100.0F) + pressure_offset);
  float currentTankreading = tankReading();
  digitalWrite(SENSOR_GND, LOW);                              //Power OFF the Sensors
  uint16_t distance = ((float)currentTankreading + 0.5);      //Rounds down/up
  float level = getLevel(distance);
  uint16_t liters = getLitres(distance);

  //See if the Month has rolled over
  if(month() != rainMonth){
      rainDays = 0;
      rainMonth = month();
  }

  //Confirm is we have Statistical data. This is crude and not accurate
  //Will work on more accurate method in a future update
  //A rain day is true if the tank level has increased in the last 24 hours
  //Obviously we may miss a rain day that adds less water than the 24hr usage
  //Probably need a rain sensor to be accurate
  if(!firstRead){
      if(liters >= lastLiters){
          rainDays += 1;
      }else{
          last24hr = lastLiters - liters;
          daysAvailable = ((float)liters / dailyAvgUse);
      }
  }

   //Battery Monitor
  #if defined(ESP8266)
      volts = ((float)ESP.getVcc()/1000);
  #endif

  char on_buf[15];
  getOnVals();
  sprintf(on_buf,"%02u:%02u:%02u",onHour,onMin,onSec);
  
  //Unicde seperator \xe2\x81\x91
  char time_buf[24]; // size big enough for string
  sprintf(time_buf,"%02u/%02u/%02u   %02u:%02u:%02u", day(),month(),year(),hour(),minute(),second());

  //BLYNK Virtual Pin Assignments
  Blynk.virtualWrite(V0, pres);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, temp);
  Blynk.virtualWrite(V3, time_buf);
  Blynk.virtualWrite(V4, pgm_mode);
  Blynk.virtualWrite(V5, liters);
  Blynk.virtualWrite(V6, level);
  Blynk.virtualWrite(V7, volts);
  Blynk.virtualWrite(V8, last24hr);
  Blynk.virtualWrite(V9, totalUsage);
  Blynk.virtualWrite(V10, dailyAvgUse);
  Blynk.virtualWrite(V11, rainDays);
  Blynk.virtualWrite(V12, daysAvailable);
  Blynk.virtualWrite(V13, averageIndex);
  Blynk.virtualWrite(V14, on_buf);
 
  if(hour() == 5){
      redo:
      updateSPIFFS();
      //Check all valuse were saved to SPIFFS
      if(!success1 || !success2 || !success3 || !success4 || !success5){
          //Retry saving the data
          data_saved = "Failed";
          Blynk.virtualWrite(V15, data_saved);
          goto redo;          
      }else{
          //Values Saved Successfully
          data_saved = "Success";
          char spiffs_ts_buf[6];
          sprintf(spiffs_ts_buf,"%02u/%02u",day(),month());
          Blynk.virtualWrite(V15, spiffs_ts_buf);
          Blynk.virtualWrite(V16, data_saved);
      }
  }
    
  if(pgm){
    #ifdef PRINT
    Serial.println("---------------------------------------------------------------------------------------------------------------");
    Serial.print("PGM MODE: "); Serial.print(pgm_mode);Serial.print("\t\t");
    Serial.print("Vcc: "); Serial.print(volts);Serial.print("v \t\t");
    Serial.print("Time Stamp: "); Serial.println(time_buf);
    Serial.print("Temperature: ");Serial.print(temp, 1);Serial.print(" \xe2\x84\x83\t");
    Serial.print("Humidity: ");Serial.print(hum, 1);Serial.print("% \t");
    Serial.print("Pressure: ");Serial.print(pres, 0);Serial.print("hPa \t");
    Serial.print("Liters avail: ");Serial.print(liters);Serial.print("\t");
    Serial.print("Current Level: ");Serial.print(level);Serial.println("% full");
    Serial.print("Last 24hr Water Used: ");Serial.print(last24hr);Serial.print("\t");
    Serial.print("30 Day Average: ");Serial.print(dailyAvgUse);Serial.print("\t");
    Serial.print("30 Day Total: ");Serial.print(totalUsage);Serial.println(" liters");
    Serial.print("Rain Days this month: ");Serial.print(rainDays);Serial.println("");
    Serial.println("---------------------------------------------------------------------------------------------------------------");
    Serial.println("");
    #endif
    digitalWrite(LED, OFF);  
    return;
  }else{
    saveTotalOn();
  }
}
