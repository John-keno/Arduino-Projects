#include <SoftwareSerial.h>

// Include the (new) library
#include <L298NX2.h>

// Pin definition
const unsigned int EN_A = 3;
const unsigned int IN1_A = 5;
const unsigned int IN2_A = 6;

const unsigned int IN1_B = 7;
const unsigned int IN2_B = 8;
const unsigned int EN_B = 9;

// Initialize both motors
L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);



int data; //Int to store app command state.
int Speed = 100; // 0 - 255. FB
int Speed2 = 100; // Steer
int sensorData;
#define sensor A7


// initialize bluetooth
SoftwareSerial BTSerial (10, 11); // RX | Tx

void setup()
{
  // Used to display information
  Serial.begin(9600);
  BTSerial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial)
  {
    //do nothing
  }

  // Set initial speed for both motors
  motors.setSpeed(Speed);
}

void loop()
{
  sensorData = analogRead(sensor);
  Serial.println("sensor = ");
  Serial.println(sensorData);
  if (BTSerial.available()>0){
    data = BTSerial.read();
    Stop(); //Initialize with motors stoped.
    
  if (sensorData > 2){
    Stop();
  }
  else{
    switch (data) {
      case 'F':
        forward();
        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'G':
        forwardleft();
        break;
      case 'I':
        forwardright();
        break;
      case 'H':
        backleft();
        break;
      case 'J':
        backright();
        break;
      case '0':
        Speed = 100;
        motors.setSpeed(Speed);
        break;
      case '1':
        Speed = 140;
        motors.setSpeed(Speed);
        break;
      case '2':
        Speed = 153;
        motors.setSpeed(Speed);
        break;
      case '3':
        Speed = 165;
        motors.setSpeed(Speed);
        break;
      case '4':
        Speed = 178;
        motors.setSpeed(Speed);
        break;
      case '5':
        Speed = 191;
        motors.setSpeed(Speed);
        break;
      case '6':
        Speed = 204;
        motors.setSpeed(Speed);
        break;
      case '7':
        Speed = 216;
        motors.setSpeed(Speed);
        break;
      case '8':
        Speed = 229;
        motors.setSpeed(Speed);
        break;
      case '9':
        Speed = 242;
        motors.setSpeed(Speed);
        break;
      case 'q':
        Speed = 255;
        motors.setSpeed(Speed);
        break;  
        }
    }
  }  
}

void forward() {
  motors.run(L298N::FORWARD);
      Serial.print(F("Robot Moving forward = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
  
}

void back() {
  motors.run(L298N::BACKWARD);
      Serial.print(F("Robot Moving backward = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
}

void right() {
  motors.runA(L298N::FORWARD);
      Serial.print(F("Robot Turning left = "));
      Serial.println(motors.isMovingA());
}

void left() {
  motors.runB(L298N::FORWARD);
      Serial.print(F("Robot Turning right"));
      Serial.println(motors.isMovingB());
}

void forwardleft() {
  motors.run(L298N::FORWARD);
  motors.setSpeedB(Speed2);
      Serial.print(F("Robot Moving forward = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
}

void forwardright() {
  motors.run(L298N::FORWARD);
  motors.setSpeedA(Speed2);
      Serial.print(F("Robot Moving forward = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
}

void backright() {
  motors.run(L298N::BACKWARD);
  motors.setSpeedB(Speed2);
      Serial.print(F("Robot Moving forward = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
}
void backleft() {
  motors.run(L298N::BACKWARD);
  motors.setSpeedA(Speed2);
      Serial.print(F("Robot Moving forward = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
}

void Stop() {
  motors.run(L298N::STOP);
      Serial.print(F("Robot stopped = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
}
