/**
 * This is the final version of the flight controller on SkyTread
 */

 
//PWM VALUES
const int mins = 980;
const int defs = 1470;
const int maxs = 1960;
const int lowperiod = 1830;
const int deadzone = 10;
const int period = 18220;
const int frequency = 55;

//INPINS: Throttle Yaw Pitch Roll VRA (right analog knob) VRB (left analog knob)
const int THROTTLE = 3;
const int YAW = 2;
const int PITCH = 18;
const int ROLL = 19; 
const int VRA = 20;
const int VRB = 21;



//OUTPINS: Left and Right: Throttle Rudder Elevator Aileron
const int LTHROTTLE = 6;
const int RTHROTTLE = 7;
const int LRUDDER = 8;
const int RRUDDER = 9;
const int LELEVATOR = 10;
const int RELEVATOR = 11;
const int LAILERON = 12;
const int RAILERON = 13;



//INPINS            
int iPins [6] = {     THROTTLE,    YAW,    PITCH,   ROLL,  VRA,  VRB };
int iVals [6] = { mins, defs, defs, defs, defs, defs};
int pVals [6] = { mins, defs, defs, defs, defs, defs};
int diff [6] = {   0,     0,    0,    0,    0,   0 };

//OUTPINS
int oVals [8] = {  mins,  mins,  defs,  defs,  defs,  defs,  defs,  defs};
int oPins [8] = {LTHROTTLE, RTHROTTLE, LRUDDER, RRUDDER, LELEVATOR, RELEVATOR, LAILERON, RAILERON};

void setup() {
    for (int i = 0; i < 6; i++) {
    pinMode(iPins[i], INPUT);
    
  }
  for(int i = 0; i < 8; i++){
    pinMode(oPins[i], OUTPUT);
  }
  
  /*
   * testing code below
   * Serial.begin(115200);
   * oVals[0] = 50;
   * oVals[1] = 80;
   * oVals[2] = 70;
   * oVals[3] = 30;
   * oVals[4] = 10;
   * oVals[5] = 20;
   * oVals[6] = 60;
   * oVals[7] = 40;
   * Serial.println("Working");
   */
   output();

}

void loop() {
  // put your main code here, to run repeatedly:

}

/**
 * Sorts the pwm values to output in order, from lowest to highest. 
 * Sets all pins to HIGH at start of period, and sets each pin to LOW as they approaches the end of their duty cycle.
 * 
 * uses millis() to calculate time
 */
void output(){
  int sortedVals [8];
  int sortedPins [8];
  //Serial.println("called");
  
  for(int i = 0; i < 8; i++){
    sortedVals[i] = oVals[i];
    sortedPins[i] = oPins[i];
  }

  for(int first = 0; first < 7; first++){ //swap sorting sorted vals into smallest first
    //Serial.print("Loop number: ");
    //Serial.println(first);
    //Serial.print("Value: ");
    //Serial.println(sortedVals[first]);
    int smallestval = sortedVals[first];
    int smallestind = first;
    for(int compared = first + 1; compared < 8; compared++){
      if(sortedVals[compared] < smallestval){
        smallestval = sortedVals[compared];
        smallestind = compared;
      }
    }
    //Serial.print("Smallest: " );
    //Serial.println(smallestval);
    //Serial.print("Index: ");
    //Serial.println(smallestind);
    int swapVal = sortedVals[first]; //swap the first (of the set of variables you are iterating through) and smallest element
    int swapPin = sortedPins[first];
    sortedVals[first] = sortedVals[smallestind];
    sortedPins[first] = sortedPins[smallestind];
    sortedVals[smallestind] = swapVal;
    sortedPins[smallestind] = swapPin;
  }
  //for(int i: sortedVals){
  //  Serial.println(i);
  //}
  int startTime = millis();
  for(int i = 0; i < 8; i++){
    pinMode(sortedPins[i], HIGH);
  }

  int i = 0;
  while(i < 8){ // loops infinitely until all 8 pins have finished duty cycle
    if(sortedVals[i] + startTime >= millis()){ // if start time + intended duration is greater than or equal to the current time
      pinMode(sortedPins[i], LOW); // disable the pin
      i++; // and move on to the next pin
    }
  }
  //Serial.println("finished");
}
