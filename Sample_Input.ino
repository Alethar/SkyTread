// Throttle Yaw Pitch Roll
const int THROTTLE = 0;
const int YAW = 1;
const int PITCH = 2;
const int ROLL = 3; 

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
const int defs = 1470;
const int maxs = 1960;
const int deadzone = 5;
const int period = 18220;

//VARS           T     Y     P     R
int iPins [4] = {   2,    3,    4,    5 };
int vals [4] = { mins, defs, defs, defs };
int diff [4] = {   0,    0,    0,    0 };
int pinnnum = 4;


//OUTPUT VALUES   Tleft  Tright Rleft  Rright Eleft  Eright Aleft  Aright
int oValsRaw [8] = {  mins,  mins,  defs,  defs,  defs,  defs,  defs,  defs};
const String oPinNames [8] = {"Tleft", "Tright", "Rleft", "Rright", "Eleft", "Eright", "Aleft", "Aright"};
int oPins [8] = {23, 25, 27, 29, 31, 33, 35, 37};
int oPinVals [8] = {0, 0, 125, 125, 125, 125, 125, 125};

void setup() {
  for (int i=0; i<4; i++) {
    pinMode(iPins[i], INPUT);
  }
  pinMode(oPins[TLEFT], OUTPUT);
  
  Serial.begin(115200);
  
  
}
//TODO add if everythin is 0, lost connection
void loop() {
  
  input();
  //calc();
  
  //printValSerial();
  //printOutSerial();
  scale();
  Serial.println(oPinVals[0]);
  //output();
}

void input(){
  for (int i=0; i<4; i++) {
    vals[i] = pulseIn(iPins[i], HIGH);
  }
}

void calc(){
  //Calculating differential thrust
  diff[THROTTLE] = vals[THROTTLE] - mins;
  for(int i = 1; i < 4; i++){
    diff[i] = vals[i] - defs;
  }

  //Throttle output
  oValsRaw[TLEFT] = vals[THROTTLE] + diff[YAW] / 2;
  oValsRaw[TRIGHT] = vals[THROTTLE] - diff[YAW] / 2;

  if(diff[YAW] > deadzone) {
    if (oValsRaw[TLEFT] > maxs) {
      oValsRaw[TRIGHT] -= oValsRaw[TLEFT] - maxs;
      oValsRaw[TLEFT] = maxs;
    }
    else if (oValsRaw[TRIGHT] < mins) {
      oValsRaw[TLEFT] += mins - oValsRaw[TRIGHT];
      oValsRaw[TRIGHT] = mins;
    }
  }

  else if(diff[YAW] < -deadzone) {
    if (oValsRaw[TRIGHT] > maxs) {
      oValsRaw[TLEFT] -= oValsRaw[TRIGHT] - maxs;
      oValsRaw[TRIGHT] = maxs;
    }
    else if (oValsRaw[TLEFT] < mins) {
      oValsRaw[TRIGHT] += mins - oValsRaw[TLEFT];
      oValsRaw[TLEFT] = mins;
    }
  }
  
  oValsRaw[2] = vals[YAW]; // rudders
  oValsRaw[3] = vals[YAW];
  oValsRaw[4] = vals[2]; // elevators
  oValsRaw[5] = vals[2];
  oValsRaw[6] = vals[3]; // ailerons
  oValsRaw[7] = vals[3];
}
void scale(){
  for(int i = 0; i < 1; i++){ // CHANGE TO 8
    long v = oValsRaw[i] - mins;
    v = v*255;
    v  = v / (maxs - mins);
    oPinVals[i] = v;
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
}

void printOutSerial(){
  Serial.println();
  for (int i=0; i<8; i++) {
    Serial.print(oPinNames[i]);
    Serial.print(" ");
    Serial.println(oValsRaw[i]);
  }
}
void output(){
  analogWrite(oPins[TLEFT], oPinVals[TLEFT]);
  Serial.print("Output:   ");
  Serial.println(oPinVals[TLEFT]);
  //analogWrite(oPins[TRIGHT], oPinVals[TRIGHT]);
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
