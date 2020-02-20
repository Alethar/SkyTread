// Throttle Yaw Pitch Roll
const int THROTTLE = 0;
const int YAW = 1;
const int PITCH = 2;
const int ROLL = 3; 
const int VRA = 4;
const int VRB = 5;

//PWM values
const int mins = 980;
const int defs = 1470;
const int maxs = 1960;
const int period = 18220;
const int deadzone = 5;

//Relay Pin values
const int TLEFT = 0;
const int TRIGHT = 1;
const int RLEFT = 2;
const int RRIGHT = 3;
const int ELEFT = 4;
const int ERIGHT = 5;
const int ALEFT = 6;
const int ARIGHT = 7;
const int lowperiod = 1830;

//PINS              T     Y     P     R   VRA  VRB
int iPins [6] = {  23,   25,   27,   29,   31,  33 };
int vals [6] = { mins, defs, defs, defs, defs, defs};
int diff [6] = {   0,     0,    0,    0,    0,   0 };
//RELAY PINS       
int oPins[8] = {    2,    3,    4,    5,    6,    7 };

//TEST VARS
int inpwm = 0;

void setup() {
  for (int i=0; i<2; i++) {
    pinMode(iPins[i], INPUT);
    
  }
  pinMode(oPins[0], OUTPUT);
  pinMode(oPins[1], INPUT);
  Serial.begin(115200);
}

void loop(){
  input();
  scale();
  //relayRaw();
  relayRawTest();
  unscale();
}

void input(){
  for (int i = 0; i < 2; i++) {// CHANGE TO 6
    vals[i] = pulseIn(iPins[i], HIGH);
    //Serial.print("Pulse in ");
    //Serial.println(vals[i]);
  }
}


void scale(){
  for (int i = 0; i < 6; i++) {
    Serial.print("Pin number ");
    Serial.print(i);
    Serial.print("  :  ");
    Serial.println(vals[i]);
  }
  for(int i = 0; i < 2; i++){
    long v = (long(vals[i]) - mins) * 251 / (maxs - mins) + 1;
    vals[i] = v;
  }
  
}

void unscale(){
  long v = long(inpwm) * (maxs - mins) / lowperiod + mins;
  Serial.println(v);
}

void relayRaw(){
  for(int i = 0; i < 6; i++){
    analogWrite(oPins[i], vals[i]);
  }
}
void relayRawTest(){
  analogWrite(oPins[0], vals[0]);
  Serial.print("it works sorta    :     ");
  Serial.println(vals[0]);

  inpwm = pulseIn(oPins[1], HIGH);
  Serial.print("recieved    :     ");
  Serial.println(inpwm);
  
}
