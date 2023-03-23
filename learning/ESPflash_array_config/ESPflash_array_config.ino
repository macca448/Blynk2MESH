/* Example sketch using ESPFlash SPIFFS helper library to save 
 * two variables and one 10 value averaging array 
 * Author: R McCleery <macca448@gmail.com>
 * 
 * NOTE1: To avoid needing to include SPIFFS.h in your config change 
 *        #include <FS.h> to #include <SPIFFS.h> in the "ESPFlash.h" library
 * 
 * NOTE2: This sketch was tested on an ESP32 but should work on other
 *        ESP devices, just check your PIN functions 
 * 
 * NOTE3: The idea of this sketch is to execuite all functions in setup()
 *        and use "Deep Sleep" mode to ultimately build BLYNK sketches from it.
 * 
 * This Arduino ESP sketch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. https://www.gnu.org/licenses/gpl-3.0.en.html
 */

#include "ESPFlash.h"                                // https://github.com/DaleGia/ESPFlash
#include <limits>
#define LED 2                                        // Provides a Visual idication of State (Solid = Running)
#define RESET 4                                      // This is to erase SPIFFS and ZERO all Values
#define SEED 36                                      // This Seed is based on an ESP32 Analog Pin
#define HOLD 15                                      // Optional, I use it just to "PAUSE" the loop 
#define uS_TO_S_FACTOR 1000000                       // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  15                            // Time ESP32 will go to sleep (in seconds)
#define SIZE 10
#define SAVE 500
int avgArray[SIZE], averageIndex, lastVal, total,
    ranVal, average;
long ct, rate;
bool jump = false;

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(0);
    while(!Serial);
    randomSeed(SEED);
    pinMode(LED, OUTPUT);                                        // LED ON to give visual that loop is running
    digitalWrite(LED, HIGH);
    pinMode(RESET, INPUT_PULLUP);
    pinMode(HOLD, INPUT_PULLUP);    
    ranVal = (random(200, 401));
    Serial.println("");
    Serial.println("ESP Booting Up............");
    Serial.println("");

        // RESET Functions
        if(digitalRead(RESET) == LOW){
            Serial.print("\n");
            Serial.println("Resetting ESP Device ...........");Serial.println("");
            Serial.println("Formatting SPIFFS. Will take a while...");
            Serial.println("");Serial.println("LED will start a Slow Blink when RESET complete");
            Serial.println("");                                 // Make sure to configure SPIFFS correctly
            SPIFFS.format();                                    // This erases the SPIFFS partition
            Serial.println("Done!");                            // but it does not ZERO data
            delay(SAVE);                                        // Small delays() are used to give SPIFFS time to save
            
            //Zero data for Index Val
            Serial.println("Setting values to ZERO............");
            ESPFlash<int> espIndexVal("/averageIndexVal");
                if(espIndexVal.get() > 0){
                    averageIndex = 0;
                }
            bool sucess1 = espIndexVal.set(averageIndex);       // This saves the averageIndex INT to SPIFFS
            delay(SAVE);                                        // You can use the bool to confirm sucess
            Serial.print("Reading RESET Stored 'averageIndex': ");
            Serial.println(espIndexVal.get());                  // Retrieve the stored file to confirm
            
            //Zero data for Last Val
            ESPFlash<int> espLastVal("/lastValStore");
                if(espLastVal.get() > 0){
                    lastVal = 0; 
                }
            bool sucess2 = espLastVal.set(lastVal);             // This saves the lastVal INT to SPIFFS
            delay(SAVE);                                        // You can use the bool to confirm sucess
            Serial.print("Reading RESET Stored 'lastVal': ");
            Serial.println(espLastVal.get());Serial.println("");// Retrieve the stored file to confirm

            //Zero data for Array Values
            ESPFlash<int> espArray("/arrayStore");
                for(int i = 0; i < SIZE; i++){
                    avgArray[i] = 0;
                    total = total + avgArray[i];
                }
            bool sucess3 = espArray.setElements(avgArray, sizeof(avgArray)); // This save the avgArray to SPIFFS
            delay(SAVE);
            espArray.getFrontElements(avgArray, sizeof(avgArray));           // Retrieve the stored file to confirm
            Serial.print("Reading RESET Stored 'avgArray' values: ");
                for(int i = 0; i < SIZE; i++){
                    Serial.print(avgArray[i]);Serial.print("  ");
                    total = total + avgArray[i];
                }
            Serial.print("\t TOTAL: ");Serial.println(total);
            Serial.println("");
            rate = SAVE;                                                      // Sets LED blink rate
            jump = true;                                                      // Bypass update functions
        }                                                                     // to loop() and blink LED
      
        // PAUSE Loop 
        if(digitalRead(HOLD) == LOW){                                         // Optional PAUSE statement
            Serial.println("LOOP is on PAUSED and LED will FLASH Fast");
            rate = 150;                                                       // Sets LED blink rate
            jump = true;                                                      // Bypass update functions
        }

    if(jump == false){  
    
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);            // Setup Deep Sleep function            
    
    //ESP Flash Index
    ESPFlash<int> espIndexVal("/averageIndexVal");
        if(espIndexVal.getFileName()){
            averageIndex = espIndexVal.get();                                // Reead saved averageIndex value
                if(averageIndex >= SIZE){                                    // Rest index to zero when index max is reached
                    averageIndex = 0;
                }     
        }

    //ESP Flash Last Val
    ESPFlash<int> espLastVal("/lastValStore");    
        if(espLastVal.getFileName()){
            lastVal = espLastVal.get();                                     // Retrieve saved lastVal
                if(espLastVal.get() == 0){                                  // For testing we pass an initial value
                    lastVal = 23500;                                        // to last Value after reset.
                    bool sucess2 = espLastVal.set(lastVal);                 // Save new value to SPIFFS
                    delay(SAVE);
                }else{
                    lastVal = lastVal - ranVal;
                    bool sucess2 = espLastVal.set(lastVal);                 // Save updated value to SPIFFS
                    delay(SAVE);
                }     
        }

    //ESP Flash avgARRAY
    ESPFlash<int> espArray("/arrayStore");
    espArray.getFrontElements(avgArray, sizeof(avgArray));                 // Retrieve avgArry Values
        for(int i = 0; i < SIZE; i++){
            total = total + avgArray[i];
        }   
        if(total == 0){
            for(int i = 0; i < SIZE; i++){
                avgArray[i] = ranVal;                                     // After a RESET populate the avgArray with
            }                                                             // a single value
            averageIndex = 1;                                             // Set Index to One for next update
            bool sucess1 = espIndexVal.set(averageIndex);                 // Save new array values to SPIFFS
            delay(SAVE); 
        }else{
            total = total - avgArray[averageIndex];                       // Once the avgArray is > 0 we step along the         
            avgArray[averageIndex] = ranVal;                              // array adding a new random value
            total = total + avgArray[averageIndex];
            averageIndex = averageIndex + 1;                              // Increment Index value for next update
                if(averageIndex >= SIZE){
                    averageIndex = 0;                                 
                }
                bool sucess1 = espIndexVal.set(averageIndex);             // Save new Index value
                delay(SAVE); 
        }
        bool sucess3 = espArray.setElements(avgArray, sizeof(avgArray));  // Save updated avgArray values 
        delay(SAVE);                                                      // We now read all the saved files back
        Serial.print("Reading after save Index Value: ");                 // for Print debugging 
        Serial.println(espIndexVal.get());
        Serial.print("Reading after save Last Value: "); 
        Serial.println(espLastVal.get());
        Serial.print("Reading after save Array Values: ");
        espArray.getFrontElements(avgArray, sizeof(avgArray));
            for(int i = 0; i < SIZE; i++){
                Serial.print(avgArray[i]);Serial.print("  ");
                total = total + avgArray[i];
            }
        delay(SAVE);    
        Serial.print("\n");
        average = total / SIZE;                                           // Create an Average value from updated avgArray Total
        Serial.print("TOTAL: ");Serial.print(total);
        Serial.print("\t Average: ");Serial.println(average);
        Serial.println("");
        Serial.println("ESP Going to Sleep.............");
        Serial.println("");
        digitalWrite(LED, LOW);
        delay(SAVE);
        esp_deep_sleep_start();                                          // Put's the ESP32 into Deep Sleep mode.
    }
    ct = millis();
}

void loop() {
    if(millis() - ct > rate){                                           // We only hit the main loop() in RESET or PAUSE modes
      int state = digitalRead(LED);
      ct = millis();
      digitalWrite(LED, state = !state);
        if(digitalRead(RESET) == HIGH && digitalRead(HOLD) == HIGH){   // Device will do a restart when condition met
          ESP.restart();
        }
    }
}
