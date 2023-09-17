#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define rxPin 8
#define txPin 9
SoftwareSerial sim800L(rxPin,txPin); 
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

#define lpg_sensor 12
#define buzzer 13

unsigned long smsPreviousTime = 0;
unsigned long smsInterval = 15000;
unsigned long callPreviousTime = 0;
unsigned long callInterval = 10000;


void setup() 
{
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  sim800L.begin(9600);

  Serial.println("Initializing...");
  
  sim800L.println("AT");
  waitForResponse();

  sim800L.println("ATE1");
  waitForResponse();

  sim800L.println("AT+CMGF=1");
  waitForResponse();

  sim800L.println("AT+CNMI=1,2,0,0,0");
  waitForResponse();
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
    digitalWrite(buzzer,LOW);
    lcd.clear();
    lcd.print("   NO LPG GAS   ");
    lcd.setCursor(0, 1);
    lcd.print("    LEAKAGE     ");
    delay(1000);
  }
  
  else 
  {
    digitalWrite(buzzer,HIGH);
    lcd.clear();
    lcd.print(" LPG Gas Leakage ");
    lcd.setCursor(0,1);
    lcd.print("     Alert    ");
    delay(1000);
    make_call("+2348064810572");
    delay(2000);
    make_call("+2348037748204");
    delay(2000);
    make_call("+2349090826533");
    delay(2000);
    send_sms("+2348064810572");
    delay(2000);
    send_sms("+2348037748204");
    delay(2000);
    send_sms("+2349090826533");
    delay(2000);
    digitalWrite(buzzer,LOW);
    delay(1000);
  }
}

void send_sms(String phoneNo){
  sim800L.print("AT+CMGS=\""+phoneNo+"\"\r");
  waitForResponse();
  
  sim800L.print("Alert there is a Gas leakage");
  sim800L.write(0x1A);
  waitForResponse();
}


void make_call(String phoneNo){
  sim800L.println("ATD"+phoneNo+";");
  waitForResponse();
}

void waitForResponse(){
  delay(1000);
  while(sim800L.available()){
    Serial.println(sim800L.readString());
  }
//  sim800L.read();
}
