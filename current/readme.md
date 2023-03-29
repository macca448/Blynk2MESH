# Current working sketch's

### List of functions by Node:

### Node 4: Septic Tank:
  1.  Level Alarm 
  2.  Purge Servo Control
  3.  Level Alarm Bulb Failure
  4.  Pump Failure
  
### Node 3: Tank 1 (House Water Supply)
  1.  Water Level as percentage full
  2.  Liters of Water available
  3.  Water used in the last 24 hours
  4.  30 Day Average daily use
  5.  Day's of Water on hand based on Average Daily Use
  6.  Yard Lighting
  7.  Tank sensors read error feedback (per single event)
  8.  Cumulative Time-out count (see main description)
  9.  Time of day SPIFFS save statistical variables function
  
### Node 2: Garage
  1.  States for Internal, Ranch Slider and Garage Doors
  2.  Door has been left Open Alarm (Seasonal variations)
  3.  Mouse Trap State
  4.  Open / Close main garage Door
      ### Mouse Trap
      1.  A simple sketch that uses two IR beams to detect a mose entering the trap
      2.  Currently running on a NANO but will be fine tuned to run on an ATTiny85
      3.  LED State indication (Solid = Armed, Blinking = Mouse in Trap)
      4.  HIGH / LOW state passed to Garage Node

### Node 1: Kitchen
  1.  Garage Door Button
  2.  Tank Light Button
  3.  Septic Purge Servo Switching
  4.  OLED display of key data with screen off time-out
  5.  Buzzer for Audible Alarm
  6.  Rotary Encoder for Wake / Manual / Auto screen rotation.
  7.  SD Card for MESH OTA Updates
  8.  UART Bi-directional Bridge to Blynk Node.

### Blynk Node: Kitchen
  1.  UART Bi-directional Bridge to MESH
  2.  NTP Master for MESH (push)
  3.  Servo for Water Level Meter
  4.  Addressable Leds for Sysyem States
  5.  ElegantOTA for Blynk Node
  6.  Webpage for all current variable states and values
      ### Blynk App
      1.  Garage Door Pulse Button
      2.  States of All Doors in Garage
      3.  Mouse Trap State
      4.  Water in Liters meter
      5.  Average Daily Use Value
      6.  Yard Light button
      7.  Current State of yard Light
      8.  Septic Tank Purge valve button
      9.  Current State of Septic Purge Valve
      10.  System Alarms as int with PUSH (automations) notifications
  
