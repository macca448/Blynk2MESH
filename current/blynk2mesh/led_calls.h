void doDoorLed(void){
    byte r, g, b;
    if(gDoorAlarm){
       r=255; g=0; b=0;
    }else{
       r=0; g=255; b=0;
    }
    pixels.setPixelColor(daStatus, pixels.Color(r,g,b));
    pixels.show();
    return;
}

void doLightLed(void){
    byte r, g, b;
    if(lastLightState){
       r=0; g=0; b=255;
    }else{
       r=0; g=0; b=0;;
    }
    pixels.setPixelColor(lightStatus, pixels.Color(r,g,b));
    pixels.show();
    return;
}

void doMouse(void){
    byte r, g, b;
    if(mouseTrap > 0){
      r=255; g=0; b=0;
    }else{
       r=0; g=0; b=0;
    }
    pixels.setPixelColor(mouseStatus, pixels.Color(r,g,b));
    pixels.show();
    return;
}

void doSepticLeds(void){
    byte r1, g1, b1;
    if(lastSvalPos == NORMAL){
        r1=0; g1=255; b1=0;
    }else{
        r1=255; g1=0; b1=0;
    }
    pixels.setPixelColor(valveStatus, pixels.Color(r1,g1,b1));
    pixels.show();
    
    byte r2, g2, b2;
    if(sLevelAlarm){
        r2=255; g2=0; b2=0;
    }else{
        r2=0; g2=255; b2=0;
    }
    pixels.setPixelColor(slStatus, pixels.Color(r2,g2,b2));
    pixels.show();

    byte r3, g3, b3;
    if(sBulbFail){
        r3=255; g3=0; b3=0;
    }else{
        r3=0; g3=255; b3=0;
    }
    pixels.setPixelColor(sbStatus, pixels.Color(r3,g3,b3));
    pixels.show();

    byte r4, g4, b4;
    if(sPumpFail){
        r4=255; g4=0; b4=0;
    }else{
        r4=0; g4=255; b4=0;
    }
    pixels.setPixelColor(spStatus, pixels.Color(r4,g4,b4));
    pixels.show();
    return;
}
