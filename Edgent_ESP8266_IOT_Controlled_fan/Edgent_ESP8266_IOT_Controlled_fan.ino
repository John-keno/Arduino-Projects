
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL3P8qENCy"
#define BLYNK_DEVICE_NAME "IOT Temperaure controlled Fan"


#define BLYNK_FIRMWARE_VERSION "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define DHTPIN D5      // What digital pin we're connected to
#define DHTTYPE DHT11  // DHT 11
#define Fan_pin 13

DHT_Unified dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);                             // set the LCD address to 0x27 for a 16 chars and 2 line display
uint8_t DegreeBitmap[] = { 0x6, 0x9, 0x9, 0x6, 0x0, 0, 0, 0 };  // create a dot character pattern for degree symbol
BlynkTimer timer;
sensors_event_t event;
uint32_t delayMS;
sensor_t sensor;

float readTemp() {
  // Get Temperature event and print its value.
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    return event.temperature;
  }
}

float readHumd() {
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    return event.relative_humidity;
  }
}
BLYNK_WRITE(V2) {
  int pinValue = param.asInt();  // assigning incoming value from pin V1 to a variable

  // process received value
  Serial.print("Fan state: ");
  Serial.println(pinValue);
  if (pinValue == 1) {
    digitalWrite(Fan_pin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Fan state  ");
    lcd.setCursor(0, 1);
    lcd.print("     OFF  ");
    delay(2000);
  } else {
    digitalWrite(Fan_pin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Fan state  ");
    lcd.setCursor(0, 1);
    lcd.print("     ON  ");
    delay(2000);
  }
}

void sendSensor() {
  float Humd = readHumd();
  float Temp = readTemp();
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, Humd);
  Blynk.virtualWrite(V0, Temp);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(Temp);
  lcd.print("\001C");  // degree symbol
  lcd.setCursor(7, 0);
  lcd.setCursor(0, 1);
  lcd.print("Humd: ");
  lcd.print(Humd);
  lcd.print("%");
}

void setup() {
  Serial.begin(9600);
  delay(100);
  dht.begin();
  lcd.init();
  lcd.createChar(1, DegreeBitmap);
  BlynkEdgent.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("setting up");
  lcd.setCursor(0, 1);
  lcd.print("device.........");
  pinMode(Fan_pin, OUTPUT);
  digitalWrite(Fan_pin, HIGH);


  if (BlynkState::is(MODE_CONNECTING_NET)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting to");
    lcd.setCursor(0, 1);
    lcd.print("Router Hotspot.........");
  }
  timer.setInterval(100L, sendSensor);
  delay(2000);
}


void loop() {
  BlynkEdgent.run();
  timer.run();

  if (BlynkState::is(MODE_CONNECTING_NET)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Router link lost");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Retrying.......");
    delay(2000);
  }
  if (BlynkState::is(MODE_CONNECTING_CLOUD)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Internet");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Retrying........");
    delay(2000);
  }
}