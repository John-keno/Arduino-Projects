#include <Wire.h>
#include "MAX30105.h"
#include <LiquidCrystal_I2C.h>

#include "heartRate.h"
/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
 *************************************************************
  Blynk.Edgent implements:
  - Blynk.Inject - Dynamic WiFi credentials provisioning
  - Blynk.Air    - Over The Air firmware updates
  - Device state indication using a physical LED
  - Credentials reset using a physical Button
 *************************************************************/

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
#define BLYNK_TEMPLATE_ID "TMPL28wZvmx5R"
#define BLYNK_TEMPLATE_NAME "LCD Heart Sensor"

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

MAX30105 particleSensor;
BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte RATE_SIZE = 4;  //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];     //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;  //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void setup() {
  Serial.begin(115200);
  delay(100);
  lcd.init();
  lcd.backlight();
  Serial.println("Initializing...");
  lcd.setCursor(0, 0);
  lcd.println("Initializing.....");
  lcd.setCursor(0, 1);
  lcd.println("..................");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))  //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");

    while (1)
      ;
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup();                     //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A);  //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);   //Turn off Green LED
  BlynkEdgent.begin();
  if (BlynkState::is(MODE_CONNECTING_NET)) {
    lcd.setCursor(0, 0);
    lcd.println(" Connecting to ");
    lcd.setCursor(0, 1);
    lcd.println("     Router     ");
  }
  delay(2000);
  // timer.setInterval(1, sendHeartRate);
  // edgentTimer.setInterval(5000, checkNetwork);
  lcd.clear();
}
void checkNetwork(){
 
}
void sendHeartRate(){
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true) {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;  //Store this reading in the array
      rateSpot %= RATE_SIZE;                     //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
      // Blynk.virtualWrite(V1, beatAvg);
    }
  }
  
  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);
  Serial.println();


  if (irValue < 30000) {
    Blynk.virtualWrite(V0, "NO");
    lcd.setCursor(0, 0);
    lcd.println(" No Heart Beat  ");
    lcd.setCursor(0, 1);
    lcd.println("    Detected    ");
    Blynk.virtualWrite(V1, beatAvg);
    Blynk.virtualWrite(V0, "NO");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("    Heart Rate    ");
    lcd.setCursor(0, 1);
    lcd.print("       ");
    lcd.print(beatsPerMinute, 0);
    lcd.print(" BPM      ");
    Blynk.virtualWrite(V1, beatsPerMinute);
    Blynk.virtualWrite(V0, "YES");
  }
}
void loop() {
   BlynkEdgent.run();
  if (BlynkState::is(MODE_CONNECTING_NET)) {
    lcd.setCursor(0, 0);
    lcd.println(" Connecting to ");
    lcd.setCursor(0, 1);
    lcd.println("     Router     ");
  }
  if (BlynkState::is(MODE_CONNECTING_CLOUD)) {
    lcd.setCursor(0, 0);
    lcd.println(" Connecting to ");
    lcd.setCursor(0, 1);
    lcd.println("    Internet    ");
  }
  while(BlynkState::is(MODE_RUNNING)){
    sendHeartRate();
  }
  
  // timer.run();
}
