//Include the Arduino Stepper Library
#include <Stepper.h>

// Define Constants
int led = 13;

// Number of steps per internal motor revolution 
const float STEPS_PER_REV = 32; 

//  Amount of Gear Reduction
const float GEAR_RED = 64;

// Number of steps per geared output rotation
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;

// Define Variables

// Number of Steps Required
int StepsRequired;

// Create Instance of Stepper Class
// Specify Pins used for motor coils
// The pins used are 8,9,10,11 
// Connected to ULN2003 Motor Driver In1, In2, In3, In4 
// Pins entered in sequence 1-3-2-4 for proper step sequencing

Stepper steppermotor(STEPS_PER_REV, 8, 10, 9, 11);

void setup()
{
// Nothing  (Stepper Library sets pins as outputs)
Serial.begin(115200);
pinMode(led, OUTPUT);
}

void loop()
{

   // Rotate CCW 1 turn slowly and dispense food
   digitalWrite(led, HIGH);
   Serial.println("lllll");
  StepsRequired  =  - STEPS_PER_OUT_REV ; 
  steppermotor.setSpeed(500);   
  steppermotor.step(StepsRequired);
  delay(2000);

   // stops the stepper motor to stop dispensing food
   digitalWrite(led, LOW);
   Serial.println("kkkk");
  StepsRequired  =  - 4; 
  steppermotor.setSpeed(1);   
  steppermotor.step(StepsRequired);
  delay(5000);


}
