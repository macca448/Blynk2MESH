### blynk_tank_node
#### This sketch is a standalone project that has the following features
    1:  Designed for battery setup
    2:  Store's needed variables for statical data to SPIFFS
    3:  30 day water usage averaging to give days of water available
    4:  In Config or PGM mode a local OTA update is possible
    5:  Normal operation is 1 hour deep sleep cycles to conserve battery power
    6:  NTP for time of day events
    7:  An ESP-12F was used in testing
##

### ESPflash_array_config
####This sketch was for learning how to save variables (index value and a 30 value array) to SPIFFS.
    1.  It uses random numbers for each array update. 
    2.  There are two switch's that control setup and run modes
    3.  The array configures on first pass and populates the array with the first value. On the next pass it steps along one index value and adds a new value. It repeats this till it reaches the last array index position then wraps back to the first index and repeats thus creating an Averaging Array
    4.  In run mode it uses a 15 second sleep cycle for the for the value update cycle.
##

### Serial_transfer_testing
To learn how to create what becam the "Bridge" link between Blynk and the MESH root Node (Assumes there is a MESH network in place and this is on the root node).
##

### encoder_btns_test
#### A simple test sketch to understand the Rotary Encoder using an SSD1306 OLED with Adafruit SSD1306 library
    1.  Encoder button toggles screen On / Off
    2.  Every click toggles the on-board LED
    3.  Encoder direction (CW - CCW) and count displayed (0 - 10 - 0)
    4.  Left, Center and Right button click's displayed
##
