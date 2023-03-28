/*******************************************************************************************
ESP8266 Pins used during Boot
The ESP8266 can be prevented from booting if some pins are pulled LOW or HIGH. 
The following list shows the state of the following pins on BOOT:

    GPIO16: pin is high at BOOT
    GPIO0: boot failure if pulled LOW
    GPIO2: pin is high on BOOT, boot failure if pulled LOW
    GPIO15: boot failure if pulled HIGH
    GPIO3: pin is high at BOOT
    GPIO1: pin is high at BOOT, boot failure if pulled LOW
    GPIO10: pin is high at BOOT
    GPIO9: pin is high at BOOT

ESP8266 Pins HIGH at Boot
There are certain pins that output a 3.3V signal when the ESP8266 boots. 
This may be problematic if you have relays or other peripherals connected to those GPIOs. 
The following GPIOs output a HIGH signal on boot:

    GPIO16
    GPIO3
    GPIO1
    GPIO10
    GPIO9

Source https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
END ESP8266...........................................................................

***************************************************************************************

The ESP32 chip has the following strapping pins:

    GPIO 0 (must be LOW to enter boot mode)
    GPIO 2 (must be floating or LOW during boot)
    GPIO 4
    GPIO 5 (must be HIGH during boot)
    GPIO 12 (must be LOW during boot)
    GPIO 15 (must be HIGH during boot)

These are used to put the ESP32 into bootloader or flashing mode. 
On most development boards with built-in USB/Serial, you don’t need to worry 
about the state of these pins. The board puts the pins in the right state for 
flashing or boot mode.

Pins HIGH at Boot on ESP32
Some GPIOs change their state to HIGH or output PWM signals at boot or reset. 
This means that if you have outputs connected to these GPIOs you may get unexpected 
results when the ESP32 resets or boots.

    GPIO 1
    GPIO 3
    GPIO 5
    GPIO 6 to GPIO 11 (connected to the ESP32 integrated SPI flash memory – not recommended to use).
    GPIO 14
    GPIO 15

Source https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
END ESP32................................................................................
********************************************************************************************/
