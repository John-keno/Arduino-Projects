#include <SoftwareSerial.h>
#include <L298NX2.h>


//Declaration for motor A
#define IN1_A 3
#define IN2_A 4

//Declaration for motor B   
#define IN1_B 5
#define IN2_B 6

//                motor A      motor B
//            |------------||------------|
L298NX2 motors(IN1_A, IN2_A, IN1_B, IN2_B);

//Uncomment this if you do not want to use the arduino Rx and Tx pin for bluetooth module
SoftwareSerial BTSerial(10, 11); // RX | TX

// Variable for data received from Bluetooth module
char data;

void setup() {
//  initialize Arduino Serial for bluetooth using its Baud rate(Mine is 9600) 
  Serial.begin(9600);
//  initialize custom Serial for bluetooth using its Baud rate(Mine is 9600)
  BTSerial.begin(9600);
//  Wait for Serial Monitor to be opened
  while (!Serial)
  {
    //do nothing
  }

}

void loop() {
//  Uncomment BTserial and Comment Serial lines if you want to use the custom Rx / Tx pins
  if (BTSerial.available()>0){
//  if (Serial.available()>0){
//    data = Serial.read();
    data = BTSerial.read();
    if (data == 'F'){
      motors.run(L298N::FORWARD);
      Serial.print(F("Robot Moving forward = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
    }
    if (data == 'B'){
      motors.run(L298N::BACKWARD);
      Serial.print(F("Robot Moving backward = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
    }
    if (data == 'L'){
      motors.runA(L298N::FORWARD);
      Serial.print(F("Robot Turning left = "));
      Serial.println(motors.isMovingA());
    }
    if (data == 'R'){
      motors.runB(L298N::FORWARD);
      Serial.print(F("Robot Turning right"));
      Serial.println(motors.isMovingB());
    }
    if (data == 'S'){
      motors.run(L298N::STOP);
      Serial.print(F("Robot stopped = "));
      Serial.println(motors.isMovingA());
      Serial.println(motors.isMovingB());
    }
    
  }
}
