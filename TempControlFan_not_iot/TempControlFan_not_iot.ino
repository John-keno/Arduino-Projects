#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 5       // What digital pin we're connected to
#define DHTTYPE DHT11  // DHT 11
#define Fan_pin 4
#define incButton 6
#define decButton 7

DHT_Unified dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);                             // set the LCD address to 0x27 for a 16 chars and 2 line display
uint8_t DegreeBitmap[] = { 0x6, 0x9, 0x9, 0x6, 0x0, 0, 0, 0 };  // create a dot character pattern for degree symbol
sensors_event_t event;
uint32_t delayMS;
sensor_t sensor;
int threshold = 30;
int fanState = -1;

void setup() {
  Serial.begin(9600);
  delay(100);
  dht.begin();
  lcd.init();
  lcd.createChar(1, DegreeBitmap);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("setting up");
  lcd.setCursor(0, 1);
  lcd.print("device.........");
  pinMode(Fan_pin, OUTPUT);
  pinMode(incButton, INPUT_PULLUP);
  pinMode(decButton, INPUT_PULLUP);
  digitalWrite(Fan_pin, HIGH);
}

void loop() {
  float Humd = readHumd();
  float Temp = readTemp();
  if (Temp > threshold) {
    fanState = 1;
    showFanState(fanState);
    
  }
  showSensor(Humd, Temp);
}

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
    // Serial.print(F("Humidity: "));
    // Serial.print(event.relative_humidity);
    // Serial.println(F("%"));
    return event.relative_humidity;
  }
}

void showSensor(float Humd, float Temp) {
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

showFanState(int val) {
  // process received value
  Serial.print("Fan state: ");
  Serial.println(pinValue);
  if (val == 0) {
    digitalWrite(Fan_pin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Fan state  ");
    lcd.setCursor(0, 1);
    lcd.print("     OFF  ");
    delay(2000);
    fanState = -1;
  } else if (val == 1) {
    digitalWrite(Fan_pin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Fan state  ");
    lcd.setCursor(0, 1);
    lcd.print("     ON  ");
    delay(2000);
    fanState = -1;
  }
}