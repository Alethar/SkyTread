// Throttle Yaw Pitch Roll
const int THROTTLE = 0;
const int YAW = 1;
const int PITCH = 2;
const int ROLL = 3; 
const int lowperiod = 2030;
const int lowperiodtwo = 1020;

const int TLEFT = 0;
const int TRIGHT = 1;
const int RLEFT = 2;
const int RRIGHT = 3;
const int ELEFT = 4;
const int ERIGHT = 5;
const int ALEFT = 6;
const int ARIGHT = 7;

//DEFAULT VALUES, PWM (delay on high)
const int mins = 980;
const int defone = 1475;
int defs = 1475;
const int maxs = 1960;
const int deadzone = 5;
const int period = 18220;
const int frequency = 55;

//VARS              T     Y     P     R   VRA   VRB
int iPins [6] = {   2,    3,    4,    5,    6,    7 };
int vals [6] = { mins, defs, defs, defs, defs, defs };
int diff [6] = {   0,    0,    0,    0,     0,    0 };
int pinnnum = 4;


//OUTPUT VALUES   Tleft  Tright Rleft  Rright Eleft  Eright Aleft  Aright
int oPinVals [8] = {  mins,  mins,  defs,  defs,  defs,  defs,  defs,  defs};
String oPinNames[8] = {"TLEFT","TRIGHT","RLEFT","RRIGHT","ELEFT","ERIGHT","ALEFT","ARIGHT",};
int oPins [8] = {23, 25, 27, 29, 31, 33, 35, 37};

void setup() {
  for (int i=0; i<6; i++) {
    pinMode(iPins[i], INPUT);
  }
  for(int i = 0; i < 8; i++){
    pinMode(oPins[i], OUTPUT);
  }
  //pinMode(oPins[TLEFT], OUTPUT);
  
  Serial.begin(115200);
  delay(5000);
  defs = pulseIn(iPins[1], HIGH);
  if(abs(defone - defs) > 40){
    defs = defone;
  }
}
//TODO add if everythin is 0, lost connection
void loop() {
  
  input();
  unscale();
  calc();
  printOutSerial();
  pwmWrite();
  delay(50);
} 

void input(){
  for (int i=0; i<6; i++) {// CHANGE TO 4
    vals[i] = pulseIn(iPins[i], HIGH);
  }
}

void calc(){
  //Calculating differential thrust
  diff[THROTTLE] = vals[THROTTLE] - mins;
  for(int i = 1; i < 6; i++){
    diff[i] = vals[i] - defs;
  }

  //Throttle output
  oPinVals[TLEFT] = vals[THROTTLE] + diff[YAW] / 2;
  oPinVals[TRIGHT] = vals[THROTTLE] - diff[YAW] / 2;

  if(diff[YAW] > deadzone) {
    if (oPinVals[TLEFT] > maxs) {
      oPinVals[TRIGHT] -= oPinVals[TLEFT] - maxs;
      oPinVals[TLEFT] = maxs;
    }
    else if (oPinVals[TRIGHT] < mins) {
      oPinVals[TLEFT] += mins - oPinVals[TRIGHT];
      oPinVals[TRIGHT] = mins;
    }
  }

  else if(diff[YAW] < -deadzone) {
    if (oPinVals[TRIGHT] > maxs) {
      oPinVals[TLEFT] -= oPinVals[TRIGHT] - maxs;
      oPinVals[TRIGHT] = maxs;
    }
    else if (oPinVals[TLEFT] < mins) {
      oPinVals[TRIGHT] += mins - oPinVals[TLEFT];
      oPinVals[TLEFT] = mins;
    }
  }
  
  oPinVals[2] = vals[YAW]; // rudders
  oPinVals[3] = vals[YAW];
  oPinVals[4] = vals[2]; // elevators
  oPinVals[5] = vals[2];
  oPinVals[6] = vals[3]; // ailerons
  oPinVals[7] = vals[3];
}
void unscale(){
  for(int i = 0; i < 2; i++){
    long v = long(vals[i]) * (maxs - mins) / lowperiod + mins;
    vals[i] = v;
  }
  long a = long(vals[2]) * (maxs - mins) / lowperiodtwo + mins;
    vals[2] = a;
  for(int i = 3; i < 6; i++){
    long v = long(vals[i]) * (maxs - mins) / lowperiod + mins;
    vals[i] = v;
  }
}

void printValSerial(){
  Serial.print("T  ");
  Serial.println(vals[THROTTLE]);
  Serial.print("Y  ");
  Serial.println(vals[YAW]);
  Serial.print("P  ");
  Serial.println(vals[PITCH]);
  Serial.print("R  ");
  Serial.println(vals[ROLL]);
  Serial.print("Period   ");
  Serial.println(lowperiod);
  Serial.print("Period Two    ");
  Serial.println(lowperiodtwo);
  
}

void printOutSerial(){
  
  for (int i=0; i<8; i++) {
    Serial.print(oPinNames[i]);
    Serial.print(" ");
    Serial.println(oPinVals[i]);
  }
}

/*void rising() {
  attachInterrupt(0, falling, FALLING);
  prev_time = micros();
}
 
void falling() {
  attachInterrupt(0, rising, RISING);
  pwm_value = micros()-prev_time;
  Serial.println(pwm_value);
}*/

void pwmWrite(){ // Takes one period to write to all pins , (18220 ms)
  for(int i = 0; i < 8; i++) { // Sets everything to high at beginning
    digitalWrite(oPins[i], HIGH);
  }
  
  int totaltime = 0;
  int index = 0;
  int counter = 0;
  for (int i=0; i<8; i++) { // for each of the 8 pins
    int tempcounter = 32000; // find largest value
    int tempindex = 0;
    for(int j = 0; j < 8; j++){
      if(oPinVals[j] < tempcounter){
        tempcounter = oPinVals[j];
        tempindex = j;
      }
    }
    index = tempindex;
    
    
    delayMicroseconds(oPinVals[index] - totaltime);
    totaltime += oPinVals[index] - totaltime;
    oPinVals[index] = 32767;
    digitalWrite(oPins[index], LOW);
    for(int j = 0; j < 8; j++){
      if(oPinVals[j] < tempcounter + deadzone){
        i++;
        digitalWrite(oPins[j], LOW);
        oPinVals[j] = 32767;
      }
    }
  }
  delayMicroseconds(period - totaltime);
  
 // digitalWrite(oPins[0], HIGH);
 // Serial.println(oPinVals[0]);
 // delayMicroseconds(oPinVals[0]);
 //   digitalWrite(oPins[0], LOW);
 // delayMicroseconds(lastdelay);
}







/*
NOTE: MAY ONLY NEED TO SETTLE ON tick based, no need for interrupt
Throttle minimum: 984 CH 3
Throttle maximum: 1945
Throttle 50%: 1470
Yaw default : 1479 CH 1
Yaw right : 1956
Yaw left: 999
Pitch def : 1470 CH 2
Pitch up : 1956
Pitch down: 979
Roll def: 1470 CH 4
Roll right: 1940
Roll left: 987
RK (CH 5) right:1958
LK (CH 6)
*/
