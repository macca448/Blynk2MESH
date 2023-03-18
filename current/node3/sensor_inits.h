void sensorINTS(void){
    //Start OneWire for DS18b20
    ttSensor.begin();
    byte owCount = ttSensor.getDeviceCount();
  #ifdef PRINT  
    Serial.println((String)"Number of OneWire Sensors = " + owCount);
  #endif 
    //Start AHT20
    if (! aht.begin()) {
      #ifdef PRINT 
        Serial.println("Could not find AHT? Check wiring");
      #endif
        while (1) delay(10); 
    }
  #ifdef PRINT
    Serial.println("AHT20 Sensor Found");
  #endif  
    //Start BMP280
    unsigned statusBMP;
    statusBMP = bmp.begin();
    
        if (!statusBMP) {
          #ifdef PRINT  
            Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
            Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
            Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
            Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
            Serial.print("        ID of 0x60 represents a BME 280.\n");
            Serial.print("        ID of 0x61 represents a BME 680.\n");
          #endif
            while (1) delay(10);
        }
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  #ifdef PRINT  
    //Sensors all Up 
    Serial.println("BMP280 Sensor Found");
  #endif
    return; 
}
