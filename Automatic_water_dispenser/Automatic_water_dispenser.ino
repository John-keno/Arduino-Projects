#include <LiquidCrystal.h>


#define hotSensor 8
#define coldSensor 9
#define tapHot 10
#define tapCold 11
#define led 13

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// make some custom characters:
byte arrowLeft[8] = {
  B00001,
  B00001,
  B00001,
  B10011,
  B11110,
  B11100,
  B11110,
  B11111
};
byte arrowRight[8] = {
  B10000,
  B10000,
  B10000,
  B11001,
  B01111,
  B00111,
  B01111,
  B11111
};
void setup() {
  //Setup pin mode
  pinMode(hotSensor, INPUT);
  pinMode(coldSensor, INPUT);
  pinMode(tapHot, OUTPUT);
  pinMode(tapCold, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(tapHot, LOW);
  digitalWrite(tapCold, LOW);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.createChar(0, arrowLeft);
  lcd.createChar(1, arrowRight);
//   Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("AUTOMATIC WATER");
  lcd.setCursor(0,1);
  lcd.print("    DISPENSER   ");
  delay(2000);
  lcd.clear();

}

void loop() {
  
  int sensorA = digitalRead(hotSensor);
  int sensorB = digitalRead(coldSensor);

  if ((sensorA == HIGH)&&(sensorB == LOW)){
    digitalWrite(led, LOW);
    digitalWrite(tapHot, HIGH);
    digitalWrite(tapCold, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Hot |When full");
    lcd.setCursor(0,1);
    lcd.print(" ON  |Remove Cup");
  }
  else if ((sensorA == LOW)&&(sensorB == HIGH)){
    digitalWrite(led, LOW);
    digitalWrite(tapHot, LOW);
    digitalWrite(tapCold, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cold |When full");
    lcd.setCursor(0,1);
    lcd.print(" ON  |Remove Cup");
  }
  else if ((sensorA == HIGH)&&(sensorB == HIGH)){
    digitalWrite(led, LOW);
    digitalWrite(tapHot, HIGH);
    digitalWrite(tapCold, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Hot tap|cold tap");
    lcd.setCursor(0,1);
    lcd.print("  ON   |  ON  ");

}
else if ((sensorA == LOW)&&(sensorB == LOW)){ 
  digitalWrite(led, HIGH);
   digitalWrite(tapHot, LOW);
    digitalWrite(tapCold, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Hot   |  Cold  ");
    lcd.setCursor(2,1);
    lcd.write((byte) 0);
    lcd.setCursor(8,1);
    lcd.print("| ");
    lcd.setCursor(13,1);
    lcd.write(1);
}
  delay(250); 

}
