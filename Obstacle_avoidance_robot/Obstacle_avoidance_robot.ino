
#include <Servo.h>
#include <NewPing.h>
#include <L298NX2.h>

//sensor pins
#define trig_pin A1 //analog input 1
#define echo_pin A0 //analog input 0
#define maximum_distance 200
int posCenter = 87;
int posLeft = 0;
int posRight = 170;
int command;
boolean goesForward = false;
int distance = 100;
boolean autoMode = false;



// Pin definition
const unsigned int EN_A = 3;
const unsigned int IN1_A = 5;
const unsigned int IN2_A = 6;

const unsigned int IN1_B = 7;
const unsigned int IN2_B = 8;
const unsigned int EN_B = 9;

// Initialize both motors
L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);
NewPing sonar(trig_pin, echo_pin, maximum_distance); //sensor function
Servo servo_motor;  // create servo object to control a servo



void setup() {
  Serial.begin(9600);
  servo_motor.attach(4);  // attaches the servo on pin 10 to the servo object
  servo_motor.write(posCenter);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);

  motors.setSpeedA(255);
  motors.setSpeedB(255);
}

void loop() {
  autoDetect();
//  if (Serial.available() > 0) {
//    if (autoMode == false){
//      command = Serial.read();
//      moveStop(); //Initialize with motors stoped.
//      manualDetect();
//    }
//    
//    if (autoMode == true){
//    while (autoMode == true){
//      autoDetect();
//      int com = Serial.read();
//      switch(com){
//        case 'X':
//        autoMode = true;
//        break;
//      case 'x':
//        autoMode = false;
//        break;
//        }
//      }
//    }
//   }
}

void autoDetect(){
//  Serial.println("auto mode");
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);
  if (distance <= 35){
    moveStop();
    delay(300);
    moveBackward();
    delay(300);
    moveStop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    if (distance >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward(); 
  }
    distance = readPing();
}

void manualDetect(){
//  Serial.println("manual mode");
  switch (command) {
      case 'F':
        moveForward();
        break;
      case 'B':
        moveBackward();
        break;
      case 'L':
        turnLeft();
        break;
      case 'R':
        turnRight();
        break;
      case 'G':
        turnLeft();
        break;
      case 'I':
        turnRight();
        break;
      case 'X':
        autoMode = true;
        break;
      case 'x':
        autoMode = false;
        break;
    }
}

int lookRight(){  
  servo_motor.write(posRight);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(posCenter);
  return distance;
}

int lookLeft(){
  servo_motor.write(posLeft);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(posCenter);
  return distance;
  delay(100);
}

int readPing(){
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

void moveStop(){
  motors.stop();
}

void moveForward(){
  if(!goesForward){

    goesForward = true;
    motors.forward();
  }
}

void moveBackward(){

  goesForward = false;
  motors.backward();
  
}

void turnRight(){
  motors.forwardA();
  motors.backwardB();
  delay(250);
  motors.forward();
}

void turnLeft(){
  motors.forwardB();
  motors.backwardA();
  delay(250);
  motors.forward();
}
