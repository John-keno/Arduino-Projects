/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 6
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String val = "No Data";
String oldval;
String newval;
int i = 0;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Wireless Notice");
  lcd.setCursor(0, 1);
  lcd.print("     Board     ");
  delay(3000);
  lcd.clear();
  lcd.print("Welcome!");
  tone(12,10);                                        //And tone the buzzer for a 100ms you can reduce it it will be better
  delay(500);
  noTone(12); 
  lcd.clear();
  lcd.print("Send in text from device");
}

void loop(){
//  if (val.length() <= 0){
 if (Serial.available()){
  val = Serial.readString();
  val.trim();
  newval = val;
  lcd.clear();
  lcd.setCursor(16,0);
  lcd.print(newval);
  tone(12,10);                                        //And tone the buzzer for a 100ms you can reduce it it will be better
  delay(100);
  noTone(12);
  delay(100);
  tone(12,10);                                        //And tone the buzzer for a 100ms you can reduce it it will be better
  delay(100);
  noTone(12);
//  delay(1000);
  }

  lcd.scrollDisplayLeft();
  delay(700);

//  if (Serial.available()){
//    newval = val;
//    //TODO: beep
//    lcd.clear();
//    lcd.print(newval);
//    while (Serial.available() > 0){
//      lcd.scrollDisplayLeft();
//      // wait a bit:
//      delay(150);
//    }
//  }
}
