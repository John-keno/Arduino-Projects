#include <L298NX2.h>
#include <Servo.h>

#define DEBUG

#ifdef DEBUG
  #define logd_ln(x) Serial.println(x)
#else
  #define logd_ln(x)
#endif



// Pin definition
// const unsigned int EN_A = 6;
const unsigned int IN1_A = 5;
const unsigned int IN2_A = 6;

const unsigned int IN1_B = 7;
const unsigned int IN2_B = 8;
// const unsigned int EN_B = 9;

// Initialize both motors
// L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);

// Initialize both motors
L298NX2 motors(IN1_A, IN2_A, IN1_B, IN2_B);

Servo servo1, servo2, servo3;



int Speed = 255; // 0 - 255. FB
int Speed2 = 255; // Steer
int pos;    // variable to store the servo position
int pos2;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

servo1.attach(10);  // attaches the servo on pin 9 to the servo object
servo2.attach(11);  // attaches the servo on pin 9 to the servo object
servo3.attach(3);  // attaches the servo on pin 9 to the servo object
motors.setSpeedA(Speed);
motors.setSpeedB(Speed);


Stop();
     
}

void loop() {
String val = "";

  if (Serial.available()>0){
    
    val = Serial.readStringUntil('#');
    if (val == "F"){
      forward();
    }

    if (val == "B"){
      back();
    }

    if (val == "L"){
      left();
    }

    if (val == "R"){
      right();
    }

    if (val == "G"){
      forwardleft();
    }

    if (val == "I"){
      forwardright();
    }

    if (val == "H"){
      backleft();
    }

    if (val == "J"){
      backright();
    }

    if (val == "stop"){
      Stop();
      }
    if(val == "PT"){
      Stop();
      pickTrash();
      Stop();
    }
  }
  
}

void forward() {
  motors.run(L298N::FORWARD);
      logd_ln("Robot Moving forward = ");
      logd_ln(motors.isMovingA());
      logd_ln(motors.isMovingB());
  
}

void back() {
  motors.run(L298N::BACKWARD);
      logd_ln("Robot Moving backward = ");
      logd_ln(motors.isMovingA());
      logd_ln(motors.isMovingB());
}

void right() {
  motors.runA(L298N::FORWARD);
      logd_ln("Robot Turning left = ");
      logd_ln(motors.isMovingA());
}

void left() {
  motors.runB(L298N::FORWARD);
      logd_ln("Robot Turning right");
      logd_ln(motors.isMovingB());
}

void forwardleft() {
  motors.run(L298N::FORWARD);
  motors.setSpeedB(Speed2);
      logd_ln("Robot Moving forward = ");
      logd_ln(motors.isMovingA());
      logd_ln(motors.isMovingB());
}

void forwardright() {
  motors.run(L298N::FORWARD);
  motors.setSpeedA(Speed2);
      logd_ln("Robot Moving forward = ");
      logd_ln(motors.isMovingA());
      logd_ln(motors.isMovingB());
}

void backright() {
  motors.run(L298N::BACKWARD);
  motors.setSpeedB(Speed2);
      logd_ln("Robot Moving forward = ");
      logd_ln(motors.isMovingA());
      logd_ln(motors.isMovingB());
}
void backleft() {
  motors.run(L298N::BACKWARD);
  motors.setSpeedA(Speed2);
      logd_ln("Robot Moving forward = ");
      logd_ln(motors.isMovingA());
      logd_ln(motors.isMovingB());
}

void Stop() {
  motors.run(L298N::STOP);
      Serial.print("Robot stopped = ");
      logd_ln(motors.isMovingA());
      logd_ln(motors.isMovingB());
}

void pickTrash(){
  
  logd_ln("servo started");
  int i = 90;
  while(i <= 180) { // goes from 180 degrees to 0 degrees
    // tell servo to go to position in variable 'pos'
    servo3.write(i);              // tell servo to go to position in variable 'pos'
    i++;
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  
  pos = 90;
  pos2 = 90;  
  while( pos <= 180 && pos2 >= 0  ) { // goes from 0 degrees to 180 degrees
  
    // in steps of 1 degree
    servo1.write(pos2);              // tell servo to go to position in variable 'pos'
    servo2.write(pos);              // tell servo to go to position in variable 'pos'
    // servo3.write(pos);              // tell servo to go to position in variable 'pos'    
    pos2--;
    pos++;
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  delay(1000);

  int j = 180;
  while (j >= 120) { // goes from 180 degrees to 0 degrees
    // tell servo to go to position in variable 'pos'
    servo3.write(j);              // tell servo to go to position in variable 'pos'
    j--;
    delay(15);                       // waits 15 ms for the servo to reach the position
  }

  servo3.write(65);              // tell servo to go to position in variable 'pos'
  delay(1000);
  servo3.write(90);              // tell servo to go to position in variable 'pos'
  servo1.write(90);              // tell servo to go to position in variable 'pos'
  servo2.write(90);              // tell servo to go to position in variable 'pos'
  delay(100);
  logd_ln("servo ended");
}
