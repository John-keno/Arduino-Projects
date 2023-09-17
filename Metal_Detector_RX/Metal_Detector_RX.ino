#include <L298NX2.h>

//Arduino Metal Detector Robot 
//Receiver Sktech
//Created by DIY Builder
// Contact me https://www.instagram.com/diy.builder/
// Before uploading the sketch you have to install the required libraries
// First go to sketch section >> include library >> manage libaries >> Search RF24 and LiquidCrystal >> Install it >> Done

#include<RF24.h>
#include<nRF24L01.h>
#include<SPI.h>
#define sensor A7
// for motor A
#define EN_A 2  
#define IN1_A 3
#define IN2_A 4
//for motor B
#define EN_B 7   
#define IN1_B 5
#define IN2_B 6
                                                                                      
//int PWM1 = 6;
//int DIR1 = 7;
//int PWM2 = 5;
//int DIR2 = 4;

int sensorValue = 0;
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
int motorSpeedA = 0;
int motorSpeedB = 0;
// With Enable pin to control speed

//                     motor A            motor B
//              |------------------||------------------|
L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);

RF24 radio(10,9);// CE pin 10, CSN pin 9
const byte address[6] = "00001";
const byte address1[6] = "00003";

struct Data_Package {
  byte x1Value;
  byte y1Value;
  byte x2Value;
  byte y2Value;
  byte sValue;
  
  
};
 Data_Package data;
 


void setup() {
  Serial.begin(9600);

//  pinMode(DIR1, OUTPUT);
//  pinMode(DIR2, OUTPUT);
  pinMode(sensor, INPUT);

 
 
  radio.begin();
  radio.openReadingPipe(1,address);
  radio.openWritingPipe(address1);
  radio.setPALevel(RF24_PA_MIN);

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }
  
  
  resetData();
}

void loop() {
   
    radio.startListening();
    
    if(radio.available()) {
  radio.read(&data, sizeof(Data_Package));
  lastReceiveTime = millis();
    }
    currentTime = millis();
    if(currentTime - lastReceiveTime > 1000) {
      resetData();
    }
//  Serial.print("j1PotX: ");
//  Serial.println(data.x1Value);
//  Serial.print("j1PotY: ");
//  Serial.println(data.y1Value);
  
  if(data.x1Value > 200 ) {
    // move robot forward
    Serial.print("moving forward ----- ");
    Serial.print(" j1PotX: ");
    Serial.println(data.x1Value);
    motors.setSpeed(100);
    motors.forward();
    
    
  }else if(data.x1Value < 100) {
    Serial.print("moving backwards ----- ");
    Serial.print(" j1PotX: ");
    Serial.println(data.x1Value);
    motors.setSpeed(100);
    motors.backward();
 
  }
  else if(data.y1Value < 200 ) {
    Serial.print("moving left ----- ");
    Serial.print("j1PotY: ");
    Serial.println(data.y1Value);
    motors.setSpeedA(100);
    motors.forwardA();

    
    
  }else if(data.y1Value > 100 ) {
    Serial.print("moving right ----- ");
    Serial.print("j1PotX: ");
    Serial.println(data.y1Value);
    motors.setSpeedB(100);
    motors.forwardB();
    
  }else if(data.y2Value > 200) {
    Serial.print("increase forward speed ----- ");
    Serial.print("j1Poty2: ");
    Serial.println(data.y2Value);
    motors.setSpeed(255);
    motors.forward();
    
  }else if(data.y2Value < 100) {
    Serial.print("increase backward speed ----- ");
    Serial.print("j1Poty2: ");
    Serial.println(data.y2Value);
    motors.setSpeed(255);
    motors.backward();
    
  }else if(data.x2Value > 200) {
    Serial.print("increase left turn speed ----- ");
    Serial.print("j1Potx2: ");
    Serial.println(data.x2Value);
    motors.setSpeedA(255);
    motors.forwardA();
    
  }else if(data.x2Value < 100) {
    Serial.print("increase left right speed ----- ");
    Serial.print("j1Potx2: ");
    Serial.println(data.x2Value);
    motors.setSpeedB(255);
    motors.forwardB();

  }
    
    else {
      Serial.println("robot stopped ----- ");
      motors.stop();

  }
 delay(5);
  
  radio.stopListening();
  sensorValue = analogRead(sensor);
  data.sValue = sensorValue;
  Serial.print("sensor ====== ");
  Serial.println(sensorValue);
  radio.write(&data, sizeof(Data_Package));
 
 delay(5);
}
    void resetData() {
      data.x1Value = 127;
      data.y1Value = 127;
      data.x2Value = 127;
      data.y2Value = 127;
      data.sValue = 0;    
      }
    
