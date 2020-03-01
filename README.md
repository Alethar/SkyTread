# SkyTreader
__3D Files__
https://myhub.autodesk360.com/ue2c047eb/g/projects/20190515194007701/data/dXJuOmFkc2sud2lwcHJvZDpmcy5mb2xkZXI6Y28uWHpNNE00Z1VUa0t4cF8xX2l4cllmZw
TODO: upload files directly to github

__Arduino__

Code for the Arduino Mega 2560 on board my RC plane

Transmitter has 6 channels
CH 3 - Throttle, left stick Y
CH 4 - Roll, left stick X
CH 2 - Pitch, right stick Y
CH 1 - Yaw, right stick X
// Note: may switch Roll and Yaw channels

Purpose: 
-Link throttle to both ESCs (right EDF and left EDF) FINISHED nvm (now has bugs)

-Set up differential thrust   FINISHED

-Set up servo bindings UNTESTED

-Later, set up gyroscope and PID balancing code

-Integration of Ardupilot? Auto flight and GPS/mapping? Camera gimbal? future plans




TODO Reminder List:
1. Set up binding code
  -remove jank binding code from megaout 
  -setup binding code for megain
  -tune default values for megaout
2. Create relay confirmation system
  -megain assigns value from 0 to 4 for each value
  -add prev values
  -if curr values diff to prev values by deadzone amount, do not increment checkval
  -else increment checkval, set prev values to curr values
  -megaout reads checkval vars before reading vals
  -if checkval diff from current checkcal val, check vals
  -else do not
  -MAKE SURE TO BRANCH
3. Fix pwm output and motor differential
4. Test and tune servo output
