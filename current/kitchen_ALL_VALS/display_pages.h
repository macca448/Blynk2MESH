String getNow(){
  String t = "";
  char tBuff[24];
  sprintf(tBuff, "%02u:%02u:%02u  %02u/%02u/%02u", hour(),minute(),second(),day(),month(),year());
  t += tBuff;
  return t;
}

void t1Display(void){
    display.clearDisplay();
    display.setTextSize(1);               // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);  // Draw white text
    display.setCursor(0,0);               // Start at top-left corner
    display.println(F("     Inside   Outside"));
    display.print(F("Temp:  "));
    display.print(roomTemp, 1);
    display.print(F("    "));
    display.println(osTemp, 1);
    display.print(F("Hum:   "));
    display.print(roomHum, 1);
    display.print("    ");
    display.println(osHum, 1);
    display.print("Pres:  ");
    display.print(roomPressure);
    display.print("    ");
    display.println(osPres);
    display.println("");
    display.println("Tank  Liters  Level");
    display.print("       ");
    display.print(tank1Liters);
    display.print("    ");
    display.println(tank1Level, 1);
    display.display();
    return;
}

void garageDisp(void){
    display.clearDisplay();
    display.setTextSize(1);               // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);  // Draw white text
    display.setCursor(0,8);               // Start at top-left corner
    display.println("Garage Doors States");
    display.println("");
        if(doorStates == 0){                        //0b000
            display.println("ALL DOORS CLOSED");
        }else if(doorStates == 1){                  //0b001
            display.println("  Garage Door OPEN");
            display.println("   Ranch Door CLOSED");
            display.println("Internal Door CLOSED");
        }else if(doorStates == 2){                  //0b010
            display.println("  Garage Door CLOSED");
            display.println("   Ranch Door OPEN");
            display.println("Internal Door CLOSED");
        }else if(doorStates == 3){                  //0b011
            display.println("  Garage Door OPEN");
            display.println("   Ranch Door OPEN");
            display.println("Internal Door CLOSED");
        }else if(doorStates == 4){                  //0b100
            display.println("  Garage Door CLOSED");
            display.println("   Ranch Door CLOSED");
            display.println("Internal Door OPEN");
        }else if(doorStates == 5){                  //0b101
            display.println("  Garage Door OPEN");
            display.println("   Ranch Door CLOSED");
            display.println("Internal Door OPEN");
        }else if(doorStates == 6){                  //0b110
            display.println("  Garage Door CLOSED");
            display.println("   Ranch Door OPEN");
            display.println("Internal Door OPEN");
        }else{
            display.println("ALL DOORS OPEN");   //0b111
        }
    char tBuff[24];
    sprintf(tBuff, "%02u/%02u/%02u  %02u:%02u:%02u", day(),month(),year(),hour(),minute(),second());
    display.println("");
    display.print(tBuff);
    display.display();
    return;
}

void septicDisplay(void){
    display.clearDisplay();
    display.setTextSize(1);               // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);  // Draw white text
    display.setCursor(0,8);               // Start at top-left corner
    display.println("Septic Tank States");
    display.printf("Level Alarm: %u\n", sLevelAlarm);
    display.printf("Valve Pos:   %u\n", sValvePos);
    display.printf("Bulb Fail:   %u\n", sBulbFail);
    display.printf("Pump Fail:   %u\n", sPumpFail);
    display.println("");
    char tBuff[24];
    sprintf(tBuff, "%02u/%02u/%02u  %02u:%02u:%02u", day(),month(),year(),hour(),minute(),second());
    display.println(tBuff);
    display.display();
    return;
}

void lastMSGsend(void){
  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0,0);
  display.println("\n");
  display.println("Vals Sent To Node: ");
  display.println("");
      if(lastDest == all){
          display.println("ALL NODES\n");
      }else if(lastDest == septicNode){
          display.println("SEPTIC NODE");
          if(sValvePos != lastServoPos){
              lastServoPos = sValvePos;
              if(sValvePos == 0){
                  display.println("Valve Pos: NORMAL\n");
              }else{
                  display.println("Valve Pos: BYPASS\n");
              }
          }
      }else if(lastDest == tank1Node){
          display.println("TANK NODE");
          if(lastLightState){
              display.println("Tank Light is: ON\n");
          }else{
             display.println("Tank Light is: OFF\n");
          } 
      }else if(lastDest == garageNode){
          display.println("GARAGE NODE\n");
      }
  char tBuff[24];
  sprintf(tBuff, "%02u:%02u:%02u  %02u/%02u/%02u", hour(),minute(),second(),day(),month(),year());
  display.println(tBuff);
  display.display();
  return;  
}

void screenChoice(void){
    if(screen == 0){
        t1Display();
    }else if(screen == 1){
        garageDisp();
    }else if(screen == 2){
        septicDisplay();
    }else if(screen == 3){
        lastMSGsend();
    }
  return;
}
