// Water Tank Sonar Ping time measurement 
uint32_t readUltrasonicDistance(uint8_t triggerPin, uint8_t echoPin){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);   //Sets the trigger pin to HIGH state for 20 microseconds
  delayMicroseconds(20);            //Normally 10uS but 20uS seems more stable
  digitalWrite(triggerPin, LOW);        
  return pulseIn(echoPin, HIGH);    //Reads the echo pin, and returns the sound wave travel time in microseconds
}

//Water Tank Reading
float tankReading(){
  //Smoothing Array for Distance reading
  const uint8_t numDistReads = 10;
  float distReads[numDistReads];
  if(pgm){
    for(int i = 0; i < numDistReads; i++){     //Zero Array Values in "PGM" mode
        distReads[i] = ((float)0.0);
    }
  } 
  for (uint8_t thisReading = 0; thisReading < numDistReads; thisReading++) {
    distReads[thisReading] = ((float)(0.01723 * readUltrasonicDistance(15, 14))*10);
    totalDist = totalDist + distReads[thisReading];
    delay(40);                                                //This delay helps Sensor processing and avoids false readings
  }
  float averageDist = ((float)totalDist / numDistReads);               //Creates "float" Average from 10 samples
  return averageDist;
}

// Calculate Water Level as percentage full from distance reading
float getLevel(float distance){
    float i = 1 - ((float)distance/(LVAL-HVAL));
    i = i * 100;
        if(i > 100){
            i = 100;
        }else if(i < 0.0){
           i = 0.0;
        }
        return i;
}

// Calculate Level in Litres from distance reading
uint16_t getLitres(float distance){
    float i = 1 - ((float)distance/(LVAL-HVAL));
    i = FULL-(FULL*((float)1 - i));
    int y = ((float) i + 0.5);
    return y;
}

void getOnVals(){
    //Total Up Time in Millis
    ESPFlash<uint32_t> espOnMillis("/millisOnStore");
        if(espOnMillis.getFileName()){
          totalOnTime = espOnMillis.get();
        }
     uint16_t totSec = totalOnTime / 1000;
     onHour = totSec / 3600;
     uint16_t remainingSec = totSec % 3600;
     onMin = remainingSec / 60;
     onSec = remainingSec % 60;
     return;  
}
