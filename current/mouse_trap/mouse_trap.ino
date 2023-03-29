//A simplistic first build
//Both trap design and this sketch need more work and fine tuning
//This is configured for the Uno/Nano so you can use the Serial monitor
//Final Solution will be on an ATTiny85

//8bit Servo library for ATTiny85
//#include "Servo8Bit.h"                //For ATTiny85
#include <Servo.h>                      //For testing using NANO/Uno
//Input/Output pin constants
#define PRINT                           //Only in testing on NANO/Uno otherwise comment out
#define LED   13        //PB0 Pin 5     These are the ATTiny pins
#define SERVO 11        //PB1 Pin 6
#define ENTRY A1        //PB2 Pin 7
#define TRAP  A2        //PB4 Pin 3
#define STATE 3         //PB3 Pin 2
//#define RESET 5       //PB5 Pin 1
//Macro's
#define OPEN 160        
#define CLOSED 30
#define DUTY 500     
//Servo object
//Servo8Bit servo;                      //For ATTiny85
Servo servo;                            //For testing using NANO/Uno  
//All the variables
uint32_t  previousTime = 0;       
bool      update = 0, watch = 0, armed = 0, ledState = 0, blink = 0, udServo = 0;
uint16_t  eReading, tReading, lastEreading, lastTreading;
uint8_t   pos = OPEN, timer1 = 0, printTimer = 0;

void setup(){
  #ifdef PRINT
      Serial.begin(115200);
  #endif
  pinMode(LED, OUTPUT);
  digitalWrite(LED, ledState);
  pinMode(STATE, OUTPUT);
  digitalWrite(STATE, ledState);
  //pinMode(ENTRY, INPUT);
  //pinMode(TRAP, INPUT);
  servo.attach(SERVO);
  servo.write(pos);
  delay(3000);
  //servo.detach();    
}

void loop (){

    if(millis() - previousTime >= DUTY){       //Half second loop duty period
        previousTime = millis();
        eReading = analogRead(ENTRY);
        tReading = analogRead(TRAP);

            if(eReading != lastEreading){
                if(eReading > 500 ){
                    ledState = HIGH;          //So we can see a mouse has entered the tunnel
                    update = true;
                    watch = true;             //Arm the Trap
                }
            }

            if(watch){
                  if(tReading != lastTreading){
                      if(tReading > 500){          //Mouse now entering main chamber of trap
                          //servo.attach(SERVO);    //Get ready to close the trap door
                          timer1 = 0;             //Reset a counter for a small delay
                          armed = true;
                      }
                  }
            }
            if(armed){
                if(tReading && eReading < 500 ){            //The mouse should now be in the main chamber                             
                    timer1++;
                }  
                if(timer1 >= 10 && (tReading && eReading < 500 )){  //5 second delay and mouse must not back in the entry
                    pos = CLOSED;                   //Set servo position
                    digitalWrite(STATE, HIGH);      //This will update BLYNK about the capture
                    timer1 = 0;                     //Start new timer
                    udServo = true;                 //Update servo position
                    blink = true;                   //Blink the led to signal mouse in trap
                }
            }

            #ifdef PRINT
                printTimer++;
                if(printTimer >= 6){
                    printTimer = 0;
                    eReading = analogRead(ENTRY);
                    tReading = analogRead(TRAP);
                    Serial.println((String)"Entrance: " + eReading + "\tTrap: " + tReading + "\n");
                }
            #endif
            if(blink){
                ledState = !ledState;
                update = true;        
            }
            if(update){
                digitalWrite(LED, ledState);
                update = false;
            }
        lastEreading = eReading;
        lastTreading = tReading;
    }
    if(udServo){
        servo.write(pos);               //GOTYA you little varment!
            if(timer1 >= 6){
                armed = false;          //Disables loop Armed functions
                servo.detach();         //detach servo as hold torque not needed
                udServo = false;        //If we get here we have caught a mouse and the program ends till next restart
            }                           //Todo: Design a trap with multi chambers to catch more than one mouse.
    }
}
    
