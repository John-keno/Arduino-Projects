#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
// #define FanPin 7       // relay pin of fan
#define heaterPin 8    // relay pin of bulb
#define MenuButton 10  // menu button
#define DecButton 11   // decrement button
#define IncButton 12   // increment button

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT_Unified dht(DHTPIN, DHTTYPE);

const int stateMemAddr = 0;
const int sTempMemAddr = 10;
const int sHumdMemAddr = 20;
uint8_t DegreeBitmap[] = { 0x6, 0x9, 0x9, 0x6, 0x0, 0, 0, 0 };
byte fanChar[] = { 0x08, 0x13, 0x1A, 0x0C, 0x06, 0x0B, 0x19, 0x02 };
byte heatChar[] = { 0x04, 0x0C, 0x0E, 0x1E, 0x1D, 0x1B, 0x0D, 0x06 };

float sTempValue;
float sHumdValue;
int sStateValue;

sensors_event_t event;
uint32_t delayMS;
sensor_t sensor;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
int interval = 1000;


void setup() {
  // initialize serial communications at 9600 bps:
  // Serial.begin(9600);
  // Initialize temperature/humidity device.
  dht.begin();
  // initialize lcd module


  pinMode(MenuButton, INPUT);
  pinMode(DecButton, INPUT);
  pinMode(IncButton, INPUT);
  // pinMode(FanPin, OUTPUT);
  pinMode(heaterPin, OUTPUT);

  EEPROM.get(stateMemAddr, sStateValue);
  delay(500);
  // Serial.println(sStateValue);
  lcd.init();
  lcd.createChar(1, DegreeBitmap);
  lcd.createChar(2, fanChar);
  lcd.createChar(3, heatChar);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("loading settings");
  delay(3000);
  lcd.setCursor(0, 1);
  lcd.print("Retriving values");
  delay(3000);

  if (sStateValue == HIGH) {
    EEPROM.get(sTempMemAddr, sTempValue);
    delay(100);
    EEPROM.get(sHumdMemAddr, sHumdValue);
    delay(100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("------Done------");
    delay(2000);


  } else {
    float initialTempValue = 38.00;
    float initialHumdValue = 55.00;
    int setStateValue = 1;
    EEPROM.put(sTempMemAddr, initialTempValue);
    delay(100);
    EEPROM.put(sHumdMemAddr, initialHumdValue);
    delay(100);
    EEPROM.put(stateMemAddr, setStateValue);
    delay(100);
    EEPROM.get(sTempMemAddr, sTempValue);
    delay(100);
    EEPROM.get(sHumdMemAddr, sHumdValue);
    delay(100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("?--Unsucessful--");
    lcd.setCursor(0, 1);
    lcd.print(" Using defaults ");
    delay(3000);
  }
  lcd.clear();
}

void loop() {
  float temperature = readTemp();
  float humidity = readHumd();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("\001C");
  lcd.setCursor(8, 0);
  lcd.print("~ ");
  lcd.print((int)sTempValue);
  lcd.print("\001C");
 
  if (temperature <= (sTempValue - 0.5f)) {
    lcd.print(" \003");
    digitalWrite(heaterPin, LOW);
  }

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity, 1);
  lcd.print("%");
  lcd.setCursor(8, 1);
  lcd.print("~ ");
  lcd.print((int)sHumdValue);
  lcd.print("%");
  if (temperature >= (sTempValue + 0.5f)) {
    lcd.print(" \002");
    digitalWrite(heaterPin, HIGH);
  }

  delay(300);

  if (buttonPressed(MenuButton)) {
    delay(300);
    int itMenu = 0;
    int setTemp = sTempValue;
    int setHumd = sHumdValue;
    do {
      if (itMenu == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Set Temperature ");
        lcd.setCursor(0, 1);
        lcd.print(" Limit = ");
        lcd.print(sTempValue);
        lcd.print("\001C");  // degree symbol

        // increment function 0
        if (buttonPressed(IncButton) && itMenu == 0 && sTempValue < 50) {
          do {
            Serial.println("increasing temp");
            sTempValue++;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Set Temperature ");
            lcd.setCursor(0, 1);
            lcd.print(" Limit = ");
            lcd.print(sTempValue);
            lcd.print("\001C");
            delay(100);
          } while (buttonPressed(IncButton) && itMenu == 0 && sTempValue < 50);
        }
        // decrement function 0
        if (buttonPressed(DecButton) && itMenu == 0 && sTempValue > 0) {
          do {
            Serial.println("decreasing temp");
            sTempValue--;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Set Temperature ");
            lcd.setCursor(0, 1);
            lcd.print(" Limit = ");
            lcd.print(sTempValue);
            lcd.print("\001C");
            delay(100);
          } while (buttonPressed(DecButton) && itMenu == 0 && sTempValue > 0);
        }
      }

      if (itMenu == 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Set Humidity ");
        lcd.setCursor(0, 1);
        lcd.print(" Limit = ");
        lcd.print(sHumdValue);
        lcd.print("%");

        // increment function 1
        if (buttonPressed(IncButton) && itMenu == 1 && sHumdValue < 100) {
          do {
            Serial.println("increasing humidity");
            sHumdValue++;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Set Humidity ");
            lcd.setCursor(0, 1);
            lcd.print(" Limit = ");
            lcd.print(sHumdValue);
            lcd.print("%");
            delay(100);
          } while (buttonPressed(IncButton) && itMenu == 1 && sHumdValue < 100);
        }
        // decrement function 1
        if (buttonPressed(DecButton) && itMenu == 1 && sHumdValue > 0) {
          do {
            Serial.println("decreasing humidity");
            sHumdValue--;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Set Humidity ");
            lcd.setCursor(0, 1);
            lcd.print(" Limit = ");
            lcd.print(sHumdValue);
            lcd.print("%");
            delay(100);
          } while (buttonPressed(DecButton) && itMenu == 1 && sHumdValue > 0);
        }
      }
      if (itMenu == 2) {

        do {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Save settings ");
          lcd.setCursor(0, 1);
          lcd.print("+ ~yes     - ~No");
          lcd.print(sHumdValue);
          lcd.print("%");
          if (buttonPressed(DecButton)) {
            sHumdValue = setHumd;
            sTempValue = setTemp;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("----Discarded---");
            delay(3000);
            itMenu = 4;
            break;
          }

          if (buttonPressed(IncButton)) {
            EEPROM.put(sTempMemAddr, sTempValue);
            delay(100);
            EEPROM.put(sHumdMemAddr, sHumdValue);
            delay(100);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("------Saved-----");
            delay(3000);
            itMenu = 4;
            break;
          }
        } while (itMenu == 2 && (buttonPressed(DecButton) || buttonPressed(IncButton)));
      }

      if (buttonPressed(MenuButton)) {
        delay(300);
        itMenu++;
      }
    } while (itMenu < 4);
  }
}

float readTemp() {
  // Get Temperature event and print its value.
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print("Err");
    lcd.print("\001C");
    lcd.setCursor(8, 0);
    lcd.print("~ ");
    lcd.print((int)sTempValue);
    lcd.print("\001C");
  } else {
    // Serial.print(F("Temperature: "));
    // Serial.print(event.temperature);
    // Serial.println(F("°C"));
    return event.temperature;
  }
}

float readHumd() {
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    // Serial.println(F("Error reading humidity!"));

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print("Err");
    lcd.print("%");
    lcd.setCursor(8, 1);
    lcd.print("~ ");
    lcd.print((int)sHumdValue);
    lcd.print("%");
  } else {
    // Serial.print(F("Humidity: "));
    // Serial.print(event.relative_humidity);
    // Serial.println(F("%"));
    return event.relative_humidity;
  }
}
boolean buttonPressed(int button) {
  delay(100);
  if (digitalRead(button) == HIGH) {
    return true;
  } else {
    return false;
  }
}
