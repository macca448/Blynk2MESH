uint32_t doReading(uint8_t triggerPin, uint8_t echoPin){      //Returns PING time
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(triggerPin, LOW);
    return pulseIn(echoPin, HIGH);
}

double distReading(double sos){                                  //Distance Reading
    
    doItAgain:
    
    //Smoothing Array for Distance reading
    double averageDist = 0;
    const uint8_t numDistReads = 10;
    double totalDist = 0;
    double distReads[numDistReads];
        
        for (uint8_t thisReading = 0; thisReading < numDistReads; thisReading++) {
            distReads[thisReading] = ((double)(sos * doReading(TRIG_PIN, ECHO_PIN))*10);
            totalDist += distReads[thisReading];
            delay(50);                                              //This delay helps Sensor processing to avoid false readings
        }
    averageDist = ((double)totalDist / numDistReads);                //Creates "double" Average from 10 samples    
        for (uint8_t x = 0; x < numDistReads; x++){                 //Check readings are within error limits
            if((distReads[x] < (averageDist - MAX_ERROR)) || (distReads[x] > (averageDist + MAX_ERROR))){
              dTimeOut++;
                  if(dTimeOut >= 10){
                      return -10.0;
                  }
              goto doItAgain;
            }
        }
    return averageDist;
}

double mpSec(){    //This calculates a temperature corrected Speed of Sound factor using a waterproof DS18b20 in the Water Tank
    
    ttSensor.requestTemperatures();                       // Send the command to get temperatures
    tankTemp = ttSensor.getTempCByIndex(0);
       /* if (inTankTemp != DEVICE_DISCONNECTED_C){
            Serial.print("Tank Temp:\t");
            Serial.print(inTankTemp);
            Serial.println("°C");
        }else{
            Serial.println("Error: Could not read temperature data");
        }*/
    
    //Meters per Second Temperatue Correction calculation
    if (tankTemp != DEVICE_DISCONNECTED_C){
        double x = 331.3 + 0.606*(double)tankTemp;
        x = ((x / 10000) / 2);
        ttReadOK = true;
        return x;
    }else{
        ttReadOK = false;
        return 0.0171104;                              //If tank temp fails we call it 18°C
    }
}

double getLevel(double d){
    double i = 1 - ((double)d/(LVAL-HVAL));
        i = i * 100;
            if(i > 100){
                i = 100;
            }else if(i < 0){
                i = 0;
            }
        return i;
}

uint16_t getLiters(double x){
    double i = 1 - ((double)x/(LVAL-HVAL));
    i = FULL-(FULL*((double)1 - i));
    uint16_t y = ((double) i + 0.5);
    return y;
}

void ultraSonic(void){
    double tempAdjSOS = mpSec();                       //Gets a temperature corrected Speed of Sound 
    double dist1 = distReading(tempAdjSOS);            //Distance with Speed of Sound Temperature Correction
        if(dist1 < -9.0){
            readErrors++;
            distance = lastDist;
            usReadOK = false;
        }else{
            usReadOK = true;
            distance = dist1 + 0.55;                  //Round double to two decimal places and cast to INT   
        }
        if(usReadOK){
            tank1Level = getLevel(dist1);
            tank1Liters = getLiters(dist1);    
        }
    dTimeOut = 0;
    return;
}

void wx(void){
    
    //Temperature, Humidity and Barrometric Pressure Readings
    
    //AHT20 Sensor
    sensors_event_t humidity, temp, pressure_event;
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
    osTemp = temp.temperature;
    //Serial.print("Temperature:\t"); Serial.print(tempNow); Serial.println("°C");
    osHum = humidity.relative_humidity;
    //Serial.print("Humidity:\t"); Serial.print(); Serial.println("% rH");
    
    //BMP280
    bmp_pressure->getEvent(&pressure_event);
    osPressure = ((pressure_event.pressure + BPRESS_OFFSET) + 0.55);
    //Serial.print("Pressure:\t"); Serial.print(pressureNow); Serial.println(" hPa");

    return;
}
