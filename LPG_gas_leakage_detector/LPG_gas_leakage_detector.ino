#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define lpg_sensor 7
#define buzzer 13

void setup() 
{
  pinMode(lpg_sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("LPG Gas Detector");
  lcd.setCursor(0,1);
  lcd.print("");
  delay(2000);
}

void loop() 
{
  if(digitalRead(lpg_sensor))
  {
    offBuzzer();
    lcd.clear();
    lcd.print("   NO LPG GAS   ");
    lcd.setCursor(0, 1);
    lcd.print("    LEAKAGE     ");
    delay(400);
    offBuzzer();
    delay(500);
  }
  
  else 
  {
    onBuzzer();
    lcd.clear();
    lcd.print(" LPG Gas Leakage ");
    lcd.setCursor(0,1);
    lcd.print("     Alert    ");
    delay(1000);
  }
}
void onBuzzer(){
  // generates a 440Hz, 494Hz, 523Hz tones in output pin 7 with 2000ms of duration
//
//  tone(13, 523, 2000); //C
//
//  delay(1000);
//
//  tone(13, 494, 2000); //B
//
//  delay(1000);
//
//  tone(13, 523, 2000); //C
//
//  delay(1000);

// You can use the notone() function to stop the tone instead of using delay()
delay(300);
tone(buzzer,494,500);
delay(300);

noTone(buzzer);
tone(buzzer,523,300);
delay(200);


}
void offBuzzer(){
  noTone(buzzer);
}
