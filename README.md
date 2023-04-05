# Blynk2MESH

## Lifestyle Property Asset Monitoring and Control System

This project started with developing a simple water tank monitoring system. In the [learning folder](https://github.com/macca448/Blynk2MESH/tree/main/learning) there is a standalone version of the water tank node that uses SPIFFS to save key data so statistical information can be displayed like "Water used in the last 24 hours", "Average Daily Use" and so on. Other items in the [learning folder](https://github.com/macca448/Blynk2MESH/tree/main/learning) are sketch's that are for calibrating or were used in learning how to do different tasks.

Here is a link to a short video demo of the system in action [here](https://youtu.be/xZrsctHoBcQ)

I then realised that there were other assets that needed monitoring and/or control so the project was expanded with "painlessMesh" to create interactive "Monitoring and Control" via Blynk or the ROOT MESH node which will be located in the kitchen.

### Basic Flow Chart of System

![Blynk2MESH flowchart](https://github.com/macca448/Blynk2MESH/blob/main/assets/mesh_flowchart_sm.jpg)

## General System Features
The [current folder](https://github.com/macca448/Blynk2MESH/tree/main/current) contains working sketch's that have or will have the following functionality

### List of functions by Node:

### Node 4: Septic Tank:
  1.  Level Alarm 
  2.  Purge Servo Control
  3.  Level Alarm Bulb Failure
  4.  Pump Failure
  
  ![Septic PIN Map](https://github.com/macca448/Blynk2MESH/blob/main/assets/septic_pin_map.jpg)

  
### Node 3: Tank 1 (House Water Supply)
  1.  Water Level as percentage full
  2.  Litre's of Water available
  3.  Water used in the last 24 hours
  4.  30 Day Average daily use
  5.  Day's of Water on hand based on Average Daily Use
  6.  Yard Lighting
  7.  Tank sensors read error feedback
      
      If the Tank Temp Sensor fails ttReadOK returns false (single event)
      
      For a tank temp false reading a defined temp is used
      
      If the Ultrasonic sensor has a read failure usReadOK returns False (single event)
      
      A max of 10 false readings before it times out.
      
  8.  Cumulative Time-out count. 
      
      To improve accuracy each value in the smoothing array is tested
      
      If any single reading is outside the tollerance defined the process repeats
      
      There is a 10 count timeout that will increment the time-out count
      
  9.  Time of day SPIFFS save statistical variables function

  ![Tank1 PIN Map](https://github.com/macca448/Blynk2MESH/blob/main/assets/tank1_pin_map.jpg)
  
### Node 2: Garage
  1.  States for Internal, Ranch Slider and Garage Doors
  2.  Door has been left Open Alarm (Seasonal variations)
  3.  Mouse Trap State
  4.  Open / Close main garage Door

![Garage PIN Map](https://github.com/macca448/Blynk2MESH/blob/main/assets/node2_garage_schematic.jpg)

![Mouse_Trap Circuit](https://github.com/macca448/Blynk2MESH/blob/main/assets/mouse_trap_schematic.jpg)

### Node 1: Kitchen
  1.  Garage Door Button
  2.  Tank Light Button
  3.  Septic Purge Servo Switching
  4.  OLED display of key data with screen off time-out
  5.  Buzzer for Audible Alarm
  6.  Rotary Encoder for Wake / Manual / Auto screen rotation.
  7.  SD Card for MESH OTA Updates
  8.  UART Bi-directional Bridge to Blynk Node.

![Kitchen PIN Map](https://github.com/macca448/Blynk2MESH/blob/main/assets/node1_kitchen_schematic.jpg)

### Blynk Node: Kitchen
  1.  UART Bi-directional Bridge to MESH
  2.  NTP Master for MESH (push)
  3.  Servo for Water Level Meter
  4.  Addressable LED's for System States
  5.  ElegantOTA for Blynk Node
  6.  Web-page for all current variable states and values
  
      ### Blynk App
      1.  Garage Door Pulse Button
      2.  States of All Doors in Garage
      3.  Mouse Trap State
      4.  Water in Litre's meter
      5.  Average Daily Use Value
      6.  Yard Light button
      7.  Current State of yard Light
      8.  Septic Tank Purge valve button
      9.  Current State of Septic Purge Valve
      10.  System Alarms as integer with PUSH (automations) notifications
      
![Blynk PIN Map](https://github.com/macca448/Blynk2MESH/blob/main/assets/blynk_schematic.jpg)


## Blynk App
![Blynk App](https://github.com/macca448/Blynk2MESH/blob/main/assets/Screenshot_20230330_191601_Blynk%20IoT.jpg)


## Blynk Dashboard
![Blynk Dashboard](https://github.com/macca448/Blynk2MESH/blob/main/assets/dashboard.jpg)


## Async Server of All States 
![All States Async Server on Blynk](https://github.com/macca448/Blynk2MESH/blob/main/assets/MESH_WEBSITE.jpg)

## To-do Items   
  1.  Improve OLED Screen Layouts
  2.  Add Barometer Mechanical Gauge
  3.  Merge Weather Forecasting from this [Weather Predictor Project](https://github.com/macca448/ESP_Weather_Predictor/) and use LED's to show forecast
  4.  Functional Block Schematics for all nodes


