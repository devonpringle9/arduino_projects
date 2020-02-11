/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

#include <String.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
String in;
int val;
#define STEP_SPEED_END 10
#define STEP_SPEED_MID 100

 
// defines pins numbers
const int stepPinMid = 22; 
const int dirPinMid = 23; 
const int stepPinEnd = 19; 
const int dirPinEnd = 18; 

// global state variables
int stepperMidPos = 0;
int stepperEndPos = 0;
int stepperMidPosCurrent = 0;
int stepperEndPosCurrent = 0;


void move_stepper(int stepPin, int dirPin, int steps, int step_speed) {
  if (steps == 0) {
    return;
  } else if (steps < 0) {
    digitalWrite(dirPin, LOW); 
    steps = steps * -1;
  } else {
    digitalWrite(dirPin, HIGH); 
  }
  for(int x = 0; x < steps; x++) {
    digitalWrite(stepPin,HIGH); 
    delay(step_speed);
    digitalWrite(stepPin,LOW); 
    delay(step_speed);
  }
}


void update_stepper_position(String stepper) {
  if (stepper.equals("MID")) {
    if (stepperMidPos == stepperMidPosCurrent) { return; }
    else if (stepperMidPos < stepperMidPosCurrent) { Serial.println("mid backward inc"); move_stepper(stepPinMid, dirPinMid, -1, STEP_SPEED_MID); stepperMidPosCurrent--; }
    else { Serial.println("mid forward inc"); move_stepper(stepPinMid, dirPinMid, 1, STEP_SPEED_MID); stepperMidPosCurrent++; }
  } else if (stepper.equals("END")) {
    if (stepperEndPos == stepperEndPosCurrent) { return; }
    else if (stepperEndPos < stepperEndPosCurrent) { Serial.println("end backward inc"); move_stepper(stepPinEnd, dirPinEnd, -1, STEP_SPEED_END); stepperEndPosCurrent--; }
    else { Serial.println("end forward inc"); move_stepper(stepPinEnd, dirPinEnd, 1, STEP_SPEED_END); stepperEndPosCurrent++; }
  }
}

void read_cli() {
  if (!SerialBT.available()) { return; }
  // read input
  in = SerialBT.readString();
  SerialBT.println(in);
  val = in.toInt();

  // interpret action
  if (val == 0) { return; }
//  else if (val == 1) { Serial.println("mid forward"); move_stepper(stepPinMid, dirPinMid, 3, STEP_SPEED_MID); }
//  else if (val == 2) { Serial.println("mid backward"); move_stepper(stepPinMid, dirPinMid, -3, STEP_SPEED_MID); }
//  else if (val == 3) { Serial.println("end forward"); move_stepper(stepPinEnd, dirPinEnd, 10, STEP_SPEED_END); }
//  else if (val == 4) { Serial.println("end backward"); move_stepper(stepPinEnd, dirPinEnd, -10, STEP_SPEED_END); }
  else if (val == 1) { Serial.println("mid forward"); stepperMidPos += 3; }
  else if (val == 2) { Serial.println("mid backward"); stepperMidPos -= 3; }
  else if (val == 3) { Serial.println("end forward"); stepperEndPos += 10; }
  else if (val == 4) { Serial.println("end backward"); stepperEndPos -= 10; }
}
 
void setup() {
  //Bluetooth setup
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  // Sets the two pins as Outputs
  pinMode(stepPinMid,OUTPUT); 
  pinMode(dirPinMid,OUTPUT);
  pinMode(stepPinEnd,OUTPUT); 
  pinMode(dirPinEnd,OUTPUT);
}


void loop() {
  // Read input
  read_cli();
  update_stepper_position((String)"MID");
  update_stepper_position((String)"END");
  delay(10);
}
