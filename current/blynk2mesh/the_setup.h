void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, ON);
  pinMode(STATUS, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("\n");
  Serial.println("ESP Starting Up............\n");

  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
    while (!Serial1) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    myTransfer.begin(Serial1);

    level_svo.attach(LEV_SVO);
    level_svo.write(90);
    hpa_svo.attach(HPA_SVO);
    hpa_svo.write(90);
    gaugeDelay = true;

    // Initialize SPIFFS
    if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
    }

    bool wifiConnect = connectToWiFi();
      if(wifiConnect == true){
          Serial.println("WiFi Connection Success..........\n");
          Serial.print("IP address: ");
          Serial.println(WiFi.localIP());
          Serial.println("\n");

          Blynk.config(BLYNK_AUTH_TOKEN);
          bool result = Blynk.connect();
          if(result){
              Serial.println("Blynk Connected............\n");
          }else{
              Serial.println("Blynk Failed to connect........!!!!!!!!\n");
              while(1){
                  digitalWrite(LED, OFF);
                  delay(1000);
                  digitalWrite(LED, ON);
                  delay(1000);                
              }
          }
      }else{
          Serial.println("WiFi Connection Failure......!!!!!!!!!!!!!\n");
          while(1){
                digitalWrite(LED, OFF);
                delay(500);
                digitalWrite(LED, ON);
                delay(500);                
          }
      }
  
    setSyncInterval(30 * 60);                             //Sync RTC interval in seconds (30 minutes)
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.printf("Connected using WiFi CH:%u", WiFi.channel());
  
    // Route for root Index HTML
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    // Route for root gauge javaScript doc
    server.on("/gauge.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/gauge.min.js", "text/javascript");
    });

    // Route for root gauge AJAX doc
    server.on("/gauge_ajax.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/gauge_ajax.js", "text/javascript");
    });

    // Route for root place holders AJAX doc
    server.on("/page_ajax.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/page_ajax.js", "text/javascript");
    });

    //Values for Place-holders
    server.on("/tankLevel", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTankLevel().c_str());
    });
    
    server.on("/sysalarms", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getSysAlarms().c_str());
    });
    
    server.on("/systemps", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemps().c_str());
    });

    server.on("/syshums", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getHums().c_str());
    });
    
    server.on("/syspres", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getPres().c_str());
    });

    server.on("/errors", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getErrors().c_str());
    });

    server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getLightState().c_str());
    });

    server.on("/doors", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getDoorState().c_str());
    });

    server.on("/mouse", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getMtState().c_str());
    });

    server.on("/galarm", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getGdAlarm().c_str());
    });

    server.on("/salarms", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getSepticAlarms().c_str());
    });

    server.on("/vpos", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getValvePos().c_str());
    });

    server.on("/mytime", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getMyTime().c_str());
    });

    AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
    server.begin();
    Serial.println("HTTP server started");

    
    pixels.begin();
        for(byte i = 0; i < NUMPIXELS; i++){
            pixels.setPixelColor(i, pixels.Color(0,0,0));
        }
    pixels.show();
    pixels.setBrightness(50);
    
    // 60sec Blynk Timer function that gives the MESH an EPOCH NTP update.
    timer.setInterval(60000UL, udMeshTime); 
    //We boot this node last and want it to do a MESH time update 
    //so we'll ad a short wait to let things settle
    delay(2000);
    prevTime=millis();    
}
