#define BLYNK_TEMPLATE_ID "TMPL3P8qENCy"
#define BLYNK_DEVICE_NAME "IOT Temperaure controlled Fan"
#define BLYNK_AUTH_TOKEN "UNthXpQppp9OgutyR6Wpw0zr3N2-b-kc"
#define BLYNK_PRINT Serial

#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "sensor.h"
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#define TRIGGER_PIN D3


bool res;
int timeout = 180;
WiFiManager wm;

char auth[] = BLYNK_AUTH_TOKEN;


void setup() {
  Serial.begin(115200);
  delay(100);
  // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  lcd.init();
  lcd.backlight();
  // get Wifi credentials from wifiManger
  String stringSSID = wm.getWiFiSSID().c_str();
  String stringPASS = wm.getWiFiPass().c_str();
  int str_len_ssid = stringSSID.length() + 1;
  int str_len_pass = stringPASS.length() + 1;
  char ssid[str_len_ssid];
  char pass[str_len_pass];
  stringSSID.toCharArray(ssid, str_len_ssid);
  stringPASS.toCharArray(pass, str_len_pass);

  initSensor();

  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connected to");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  delay(2000);

  
  Blynk.begin(auth, ssid, pass);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  // is configuration portal requested?
 Blynk.run();  
  if (digitalRead(TRIGGER_PIN) == LOW) {
    wm.resetSettings();
    wm.setConfigPortalTimeout(timeout);
    lcd.setCursor(0,0);
    lcd.clear();

    lcd.print("Resetting.....");
  
    res = wm.autoConnect("IOT Temp Fan", "prophet0604"); // password protected ap
    if(!res){
        Serial.println("failed to connect and hit timeout");
        lcd.clear();
        lcd.print("Error Resetting");
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        ESP.restart();
        delay(5000);
        }
        lcd.clear();
        lcd.print("Done Resetting");
        delay(1000);
        lcd.clear();
        lcd.print("Connected to");
        lcd.setCursor(0, 1);
        lcd.print(wm.getWiFiSSID());
        delay(2000);
        }
  
  delay(delayMS);
  String Temp = readTemp();
  String Humd = readHumd();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(Temp);
  lcd.setCursor(0,1);
  lcd.print("Humd: ");
  lcd.print(Humd);


}
