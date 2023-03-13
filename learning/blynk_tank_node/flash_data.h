/*
 * ESPFlash Read and Write Variables for Water Usage in case of power loss
 * These variables will only be updated once a day.
 * The variables are:
 *      last distance reading (lastVal)
 *      useage array index "0 to 29" (averageIndex)
 *      last 24hr water usage array (usageArray[SIZE]) from a 30day average we can guestimate days of water remaining
 *      current month rain day's (rainDays) defined by increase in tank level
 *      rain days month (so we can detect a new month and zero number of days     
 */

void readSavedVals(void){
        //ESP Flash Index
    ESPFlash<uint8_t> espIndexVal("/averageIndexVal");
        if(espIndexVal.getFileName()){
            averageIndex = espIndexVal.get();          // Read saved averageIndex value     
        }
        //Rain Month
    ESPFlash<uint8_t> espRainMonth("/rainMonthVal");
        if(espRainMonth.getFileName()){
            rainMonth = espRainMonth.get(); 
        }
        //Rain days
    ESPFlash<uint8_t> espRainDays("/rainDaysVal");
        if(espRainDays.getFileName()){
            rainDays = espRainDays.get(); 
        }
        //Last Liters Value
    ESPFlash<uint16_t> espLastLiters("/lastLitersVal");
        if(espLastLiters.getFileName()){
            lastLiters = espLastLiters.get();
            if(lastLiters == 0){
                firstRead = true;
            }else{
                firstRead = false;
            }
        }
        //Usage Array Values
    if(!firstRead){
        ESPFlash<uint16_t> espArray("/arrayStore");
            espArray.getFrontElements(usageArray, sizeof(usageArray));           // Retrieve the stored file to confirm
            for(uint8_t i = 0; i < SIZE; i++){
                totalUsage = totalUsage + usageArray[i];
            }
            dailyAvgUse = totalUsage / SIZE;
            if(totalUsage == 0){
                populate = true;
            }
        }
        return;
}

void updateSPIFFS(void){
    lastLiters = liters;
    ESPFlash<uint16_t> espLastLiters("/lastLitersVal");
        success1 = espLastLiters.set(lastLiters);
    delay(40);
    
    ESPFlash<uint8_t> espRainMonth("/rainMonthVal");
        success2 = espRainMonth.set(rainMonth);
    delay(40);
    
    ESPFlash<uint8_t> espRainDays("/rainDaysVal");
        success3 = espRainDays.set(rainDays);
    delay(40);
    
    ESPFlash<uint16_t> espArray("/arrayStore");
    ESPFlash<uint8_t> espIndexVal("/averageIndexVal");
    if(last24hr > 0){
        if(populate){
            for(int i = 0; i < SIZE; i++){
                usageArray[i] = last24hr;
            }
            averageIndex = 1;
            success4 = espIndexVal.set(averageIndex);
            delay(40);
            success5 = espArray.setElements(usageArray, sizeof(usageArray));  // Save updated avgArray values
            delay(40); 
            return;
        }else{
            totalUsage = totalUsage - usageArray[averageIndex];
            usageArray[averageIndex] = last24hr;
            averageIndex += 1;
            averageIndex %= SIZE;                     //Wraps index val to Zero when we hit array size
            success4 = espIndexVal.set(averageIndex);
            delay(40);
            success5 = espArray.setElements(usageArray, sizeof(usageArray));  // Save updated avgArray values 
            delay(40);
            return;
        }
    }
    success5 = true;      //We only hit this on the first save.
    return;
}

void saveTotalOn(void){
    ESPFlash<uint32_t> espOnMillis("/millisOnStore");
    totalOnTime += currentMillis;
    redoTime:
    success6 = espOnMillis.set(totalOnTime);
    delay(40);                                          //Small delay to let update finish
    if(success6){
      #ifdef PRINT
        Serial.println("Going to Sleep Now............");Serial.println(""); 
        Serial.flush();
      #endif
        digitalWrite(LED, OFF);
      #if defined(ESP8266)
        ESP.deepSleep(uShour);  //Create a 1 hour sleep period
      #elif defined(ESP32)
        esp_deep_sleep_start(); 
      #endif
    }else{
        goto redoTime;
    }
}

    
