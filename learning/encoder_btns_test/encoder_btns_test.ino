#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//For SPI OLED
#define OLED_DC     4
#define OLED_CS     5
#define OLED_RESET  27

// Rotary Encoder Inputs
#define CLK 32
#define DT 33
#define SW 25

//Buttons
#define LEFT_BTN    39
#define CENTER_BTN  34
#define RIGHT_BTN   35

#define OTA         36

//Onboard LED
#define LED 2

//Display Object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);

int counter = 0;
bool ledState = 0, leftPress = 0, centerPress = 0, rightPress = 0, encPress = 0, oledON = 1,
      otaState, lastOTA = 1, left, lastLeft = 1, center, lastCenter = 1,
      right, lastRight = 1, encBtn, lastEncBtn = 1, currentStateCLK, lastStateCLK = 1;
String currentDir ="";
uint32_t previousTime = 0;

void setup() {
        
  // Set encoder pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(OTA, INPUT_PULLUP);
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(CENTER_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);
  
  pinMode(LED, OUTPUT);

  // Setup Serial Monitor
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(20,20);
  display.println("Starting Up.......\n");
  display.display();

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
}

void encoderChange(void){
  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0,8);
  display.printf("Encoder Turned: %s\n\n", currentDir);
  display.printf("New Encoder Val: %i", counter);
  display.display();
  return; 
}

void encBtnChange(void){
    if(oledON){
        display.ssd1306_command(SSD1306_DISPLAYALLON_RESUME);
        display.clearDisplay();
        display.setTextSize(1);               // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);  // Draw white text
        display.setCursor(0,20);
        display.println("Display Back ON!!");
        display.display();
         
    }else{
        display.ssd1306_command(SSD1306_DISPLAYOFF);
    }
  return;
}

void leftBtnDisplay(void){
  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0,16);
  display.println("Left Button Press!!");
  display.display();
  return;
}

void centerBtnDisplay(void){
  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0,16);
  display.println("Center Button Press!!");
  display.display();
  return;
}

void rightBtnDisplay(void){
  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0,16);
  display.println("Right Button Press!!");
  display.display();
  return;
}

void loop() {
        
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);
  encBtn = digitalRead(SW);
  right = digitalRead(RIGHT_BTN);
  left = digitalRead(LEFT_BTN);
  center = digitalRead(CENTER_BTN);
  
  if(encBtn != lastEncBtn){
    encPress = true;
    previousTime = millis();
  }

  if(right != lastRight){
    rightPress = true;
    previousTime = millis();
  }

  if(left != lastLeft){
    leftPress = true;
    previousTime = millis();
  }

  if(center != lastCenter){
    centerPress = true;
    previousTime = millis();
  }
  
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      counter ++;
      currentDir ="CW";
      counter %= 11;
    } else {
      // Encoder is rotating CW so increment
      counter --;
      currentDir ="CCW";
      if(counter < 0){
         counter = 10;
      }
    }
    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
    encoderChange();
  }

  if (millis() -previousTime > 50) {
    if(encPress){      
      if(encBtn == LOW){
          Serial.println("Encoder Button pressed!");
          digitalWrite(LED, ledState = !ledState);
          oledON = !oledON;
          encBtnChange();
      }
      encPress = false;
    }
    if(leftPress){      
      if(left == LOW){
          Serial.println("Left Button pressed!");
          digitalWrite(LED, ledState = !ledState);
          leftBtnDisplay();
      }
      leftPress = false;
    }
    if(centerPress){      
      if(center == LOW){
          Serial.println("Center Button pressed!");
          digitalWrite(LED, ledState = !ledState);
          centerBtnDisplay();
      }
      centerPress = false;
    }
    if(rightPress){      
      if(right == LOW){
          Serial.println("Right Button pressed!");
          digitalWrite(LED, ledState = !ledState);
          rightBtnDisplay();
      }
      rightPress = false;
    }
    previousTime = millis();
  }
  lastRight = right;
  lastStateCLK = currentStateCLK;
  lastLeft = left;
  lastCenter = center;
}
