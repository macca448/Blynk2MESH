//Items below will configure the ESPFlash Variables and set the data to ZERO

void configSPIFFS(void){

    byte errorCount = 0;
    
    // RESET Functions
        #ifdef PRINT
            Serial.print("\n");
            Serial.println("Resetting ESP Device ...........");Serial.println("");
            Serial.println("Formatting SPIFFS. Will take a while...");
            Serial.println("");Serial.println("LED will start a Slow Blink when RESET complete");
            Serial.println("");                                 // Make sure to configure SPIFFS correctly
        #endif
            SPIFFS.format();                                    // This erases the SPIFFS partition
        #ifdef PRINT
            Serial.println("Done!");                            // but it does not ZERO data
        #endif
            delay(500);                                        // Small delays() are used to give SPIFFS time to save

            tryAgain:
            //Zero data for Index Val
        #ifdef PRINT
            Serial.println("Setting values to ZERO............");
        #endif
            ESPFlash<uint8_t> espIndexVal("/averageIndexVal");
                if(espIndexVal.get() > 0){
                    averageIndex = 0;
                }
            success1 = espIndexVal.set(averageIndex);       // This saves the averageIndex INT to SPIFFS
            delay(500);                                        // You can use the bool to confirm success
            digitalWrite(LED, ledState = !ledState);
        #ifdef PRINT
            Serial.print("Reading RESET Stored 'averageIndex': ");
            Serial.println(espIndexVal.get());                  // Retrieve the stored file to confirm
        #endif

        //Zero Data for Rain Month
            ESPFlash<uint8_t> espRainMonth("/rainMonthVal");
                if(espRainMonth.get() > 0){
                    rainMonth = 0;
                }
            success2 = espRainMonth.set(rainMonth);        // This saves the averageIndex INT to SPIFFS
            delay(500);                                        // You can use the bool to confirm success
            digitalWrite(LED, ledState = !ledState);
        #ifdef PRINT
            Serial.print("Reading Rain Month 'rainMonth': ");
            Serial.println(espRainMonth.get());                  // Retrieve the stored file to confirm
        #endif
            
        //Zero Data for Rain days
            ESPFlash<uint8_t> espRainDays("/rainDaysVal");
                if(espRainDays.get() > 0){
                    rainDays = 0;
                }
            success3 = espRainDays.set(rainDays);      // This saves the averageIndex INT to SPIFFS
            delay(500);                                        // You can use the bool to confirm success
            digitalWrite(LED, ledState = !ledState);
        #ifdef PRINT
            Serial.print("Reading Rain Days 'rainDays': ");
            Serial.println(espRainDays.get());                  // Retrieve the stored file to confirm
        #endif
            
        //Zero data for Last Distance Value
            ESPFlash<uint16_t> espLastVal("/lastValStore");
                if(espLastVal.get() > 0){
                    lastLiters = 0; 
                }
            success4 = espLastVal.set(lastLiters);         // This saves the lastVal INT to SPIFFS
            delay(500);                                        // You can use the bool to confirm success
            digitalWrite(LED, ledState = !ledState);
        #ifdef PRINT
            Serial.print("Reading RESET Stored 'lastVal': ");
            Serial.println(espLastVal.get());Serial.println("");// Retrieve the stored file to confirm
        #endif
            
        //Total ON Time
            ESPFlash<uint32_t> espOnMillis("/millisOnStore");
                if(espOnMillis.get() > 0){
                    totalOnTime = 0;               
                }
            success6 = espOnMillis.set(totalOnTime);         // This saves total ON time as a millis value
            delay(500);
            digitalWrite(LED, ledState = !ledState);
        #ifdef PRINT
            Serial.print("Reading RESET Stored 'espOnMillis': ");
            Serial.println(espOnMillis.get());Serial.println("");// Retrieve the stored file to confirm
        #endif            
            
        //Zero data for Usage Array Values
            ESPFlash<uint16_t> espArray("/arrayStore");
                for(uint8_t i = 0; i < SIZE; i++){
                    usageArray[i] = 0;
                    totalUsage = totalUsage + usageArray[i];
                }
            success5 = espArray.setElements(usageArray, sizeof(usageArray)); // This save the avgArray to SPIFFS
            delay(500);
            espArray.getFrontElements(usageArray, sizeof(usageArray));           // Retrieve the stored file to confirm
        #ifdef PRINT
            Serial.print("Reading RESET Stored 'usageArray' values: ");
        #endif
                for(uint8_t i = 0; i < SIZE; i++){
                  #ifdef PRINT
                    Serial.print(usageArray[i]);Serial.print("  ");
                  #endif
                    totalUsage = totalUsage + usageArray[i];
                }
        #ifdef PRINT
            Serial.print("\t TOTAL: ");Serial.println(totalUsage);
        #endif
    delay(500);
    //We will now blink the LED for 10 seconds so we know SPIFFS setup is done for when there is no serial connection
    for(uint8_t x = 0; x <= 40; x++){
            digitalWrite(LED, ledState = !ledState);
            delay(250);
    }

      if(success1 && success2 && success3 && success4 && success5 && success6){
          digitalWrite(LED, ON);
        #ifdef PRINT
          Serial.println("SPIFFS Reset Now Complete & OTA can be used\n");
        #endif 
      }else{
        #ifdef PRINT
          Serial.println("There was an error....repeating Setup now\n");
        #endif
          errorCount++;
          if(errorCount > 9){
            #ifdef PRINT
              Serial.println("UNABLE TO CONFIGURE FLASH....TERMINATING PROGRAM\n");
            #endif
              while(1){
                  digitalWrite(LED, ledState = !ledState);
                  delay(100);
              }
          }
          goto tryAgain;
      }
}
