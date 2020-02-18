// Throttle Yaw Pitch Roll
const int THROTTLE = 0;
const int YAW = 1;
const int PITCH = 2;
const int ROLL = 3; 
int periods = 0;

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
const int frequency = 55;

//VARS           T     Y     P     R
int iPins [4] = {   2,    3,    4,    5 };
int vals [4] = { mins, defs, defs, defs };
int diff [4] = {   0,    0,    0,    0 };
int pinnnum = 4;


//OUTPUT VALUES   Tleft  Tright Rleft  Rright Eleft  Eright Aleft  Aright
int oPinVals [8] = {  mins,  mins,  defs,  defs,  defs,  defs,  defs,  defs};
const String oPinNames [8] = {"Tleft", "Tright", "Rleft", "Rright", "Eleft", "Eright", "Aleft", "Aright"};
int oPins [8] = {23, 25, 27, 29, 31, 33, 35, 37};

void setup() {
  for (int i=0; i<4; i++) {
    pinMode(iPins[i], INPUT);
  }
  pinMode(oPins[TLEFT], OUTPUT);
  
  Serial.begin(115200);
  
  //oPinVals[TLEFT]
}
//TODO add if everythin is 0, lost connection
void loop() {
  
  input();
  calc();
  
  printValSerial();
  printOutSerial();
  Serial.println(oPinVals[TLEFT]);
  pwmWriteTest();
}

void input(){
  for (int i=0; i<3; i++) {// CHANGE TO 4
    vals[i] = pulseIn(iPins[i], HIGH);
    if(i == 2){
    periods = vals[i] + pulseIn(iPins[i], LOW);
    }
  }
}

void calc(){
  //Calculating differential thrust
  diff[THROTTLE] = vals[THROTTLE] - mins;
  for(int i = 1; i < 4; i++){
    diff[i] = vals[i] - ddiefs;
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
void scale(){
  for(int i = 0; i < 1; i++){ // CHANGE TO 8
    long v = long(oPinVals[i]) * 255 / period;
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
  Serial.print("Period   ");
  Serial.println(periods);
  
}

void printOutSerial(){
  Serial.println();
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
  
  int counter = 0;
  int index = 0;
  for (int i=0; i<8; i++) {
    int tempcounter = 99999;
    int tempindex = 0;
    for(int j = 0; j < 8; j++){
      if(oPinVals[j] < tempcounter){
        tempcounter = oPinVals[j];
        tempindex = j;
      }
    }
    counter = tempcounter - counter;
    oPinVals[index] = 32767;
    delayMicroseconds(counter);
    digitalWrite(index, LOW);
  }
  delayMicroseconds(period - counter);
}
void pwmWriteTest(){
  digitalWrite(4, HIGH);
  delayMicroseconds(oPinVals[TLEFT] * 3);
  digitalWrite(4, LOW);
  delayMicroseconds(period-oPinVals[TLEFT]);
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
