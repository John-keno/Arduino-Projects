/*Arduino Voice Control Robot 
 *Version 1.0
 *Created By John keno Iweh
 *Before Uploading the sketch you must install the required libraries first. 
 *Unless you'll get compilation error message.
 *Also remove the bluetooth and servo connector before uploading the sketch.
 *You can contact me here iwehjohn@gmail.com
 */
 



#include <L298NX2.h>

#define ledCommand1 11
#define ledCommand0 12
String command;

// Pin definition
const unsigned int IN1_A = 5;
const unsigned int IN2_A = 6;

const unsigned int IN1_B = 7;
const unsigned int IN2_B = 8;

// Initialize both motors
L298NX2 motors(IN1_A, IN2_A, IN1_B, IN2_B);



void setup() {
  
Serial.begin(9600);
pinMode(ledCommand0, OUTPUT);
pinMode(ledCommand1, OUTPUT);

}

void loop() {
 delay(10);
 while(Serial.available()) {
  command = "";
  command = Serial.readString();
  digitalWrite(ledCommand1,LOW);
  digitalWrite(ledCommand0,HIGH);
  
    Serial.print(command);
}

  if(command == "*move forward#"){
    digitalWrite(ledCommand1,HIGH);
    digitalWrite(ledCommand0,LOW);
    forward();
    digitalWrite(ledCommand1,LOW);
  }else if(command == "*move backward#"){
    digitalWrite(ledCommand1,HIGH);
    digitalWrite(ledCommand0,LOW);
    backward();
    digitalWrite(ledCommand1,LOW);
  }else if(command == "*turn left#"){
    digitalWrite(ledCommand1,HIGH);
    digitalWrite(ledCommand0,LOW);
    left();
    digitalWrite(ledCommand1,LOW);
  }else if(command == "*turn right#"){
    digitalWrite(ledCommand0,LOW);
    digitalWrite(ledCommand1,HIGH);
    right();
    digitalWrite(ledCommand1,LOW);
  }else if(command == "*stop#") {
    digitalWrite(ledCommand1,HIGH);
    digitalWrite(ledCommand0,LOW);
    delay(500);
    digitalWrite(ledCommand1,LOW);
    Stop();
 }
  command = "";
  digitalWrite(ledCommand1,LOW);
  digitalWrite(ledCommand0,HIGH);
}

void forward() {
  motors.forward();
  delay(1000);
  motors.stop();
  
}

void backward() {
 motors.backward();
  delay(1000);
  motors.stop();
}

void left() {
  
  motors.forwardB();
  motors.backwardA();
  delay(500);
  motors.stop();
}

void right() {
  motors.forwardA();
  motors.backwardB();
  delay(500);
  motors.stop();
}

void Stop() {
  motors.stop();
}
