#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#define lpg_sensor 7
#define buzzer 13
void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(lpg_sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.print("LPG Gas Detector");
  lcd.setCursor(0,1);
  lcd.print("");
  delay(2000);
  lcd.clear();
}

void loop() {
  
  if(digitalRead(lpg_sensor))
  {
    lcd.clear();
    offBuzzer();
    lcd.print("   NO LPG GAS   ");
    lcd.setCursor(0, 1);
    lcd.print("    LEAKAGE     ");
    delay(1000);
  }
  
  else 
  {
    onBuzzer();
    lcd.clear();
    lcd.print(" LPG Gas Leakage ");
    lcd.setCursor(0,1);
    lcd.print("     Alert    ");
    delay(100);
    
  }
}

void onBuzzer(){
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(100);
}
void offBuzzer(){
  digitalWrite(buzzer, LOW);
}
