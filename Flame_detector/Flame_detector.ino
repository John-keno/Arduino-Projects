#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define flame_sensor 7
#define buzzer 13

void setup() 
{
  Serial.begin(9600);
  pinMode(flame_sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);
  lcd.print(" Flame Detector ");
  lcd.setCursor(0,1);
  lcd.print("");
  delay(2000);
}

void loop() 
{
  int sensor = digitalRead(flame_sensor);
  Serial.println(digitalRead(flame_sensor));
  if(sensor == 0)
  {
    offBuzzer();
    lcd.clear();
    lcd.print("    NO Flame    ");
    lcd.setCursor(0, 1);
    lcd.print("    Detected     ");
    delay(400);
    offBuzzer();
    delay(500);
  }
  
  else 
  {
    lcd.clear();
    onBuzzer();
    lcd.print(" Flame Detected ");
    lcd.setCursor(0,1);
    lcd.print("     Alert!!!    ");
    delay(250);
    lcd.setCursor(0,1);
    lcd.print("                 ");
    delay(250);
    
  }
}
void onBuzzer(){
  digitalWrite(buzzer, HIGH);


}
void offBuzzer(){
  digitalWrite(buzzer, LOW);
}
