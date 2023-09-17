#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MAX30105.h"   //MAX3010x library
#include "heartRate.h"  //Heart rate calculating algorithm

MAX30105 particleSensor;

const byte RATE_SIZE = 4;  //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];     //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;  //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void setup() {
  lcd.init();
  particleSensor.begin(Wire, I2C_SPEED_FAST);  //Use default I2C port, 400kHz speed
  particleSensor.setup();                      //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A);   //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);    //Turn off Green LED
}

void loop() {
  // put your main code here, to run repeatedly:
}

void sendHeartRate() {
  long irValue = particleSensor.getIR();  //Reading the IR value it will permit us to know if there's a finger on the sensor or not
                                          //Also detecting a heartbeat
  if (irValue > 10000) {                  //If a finger is detected
    lcd.Clear();
    lcd.setCursor(0, 0);
    lcd.println("BPM: ");
    lcd.println(beatAvg);
      display.drawBitmap(5, 5, logo2_bmp, 24, 21, WHITE);  //Draw the first bmp picture (little heart)
    display.setTextSize(2);                                //Near it display the average BPM you can display the BPM if you want
    display.setTextColor(WHITE);
    display.setCursor(50, 0);
    display.println("BPM");
    display.setCursor(50, 18);
    display.println(beatAvg);
    display.display();

    if (checkForBeat(irValue) == true)  //If a heart beat is detected
    {
      Blynk.virtualWrite(V0, "Yes");
      display.clearDisplay();                              //Clear the display
      display.drawBitmap(0, 0, logo3_bmp, 32, 32, WHITE);  //Draw the second picture (bigger heart)
      display.setTextSize(2);                              //And still displays the average BPM
      display.setTextColor(WHITE);
      display.setCursor(50, 0);
      display.println("BPM");
      display.setCursor(50, 18);
      display.println(beatAvg);
      display.display();

      tone(14, 1000);  //And tone the buzzer for a 100ms you can reduce it it will be better
      delay(100);
      noTone(14);  //Deactivate the buzzer to have the effect of a "bip"

      //We sensed a beat!
      long delta = millis() - lastBeat;  //Measure duration between two beats
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);  //Calculating the BPM

      if (beatsPerMinute < 255 && beatsPerMinute > 20)  //To calculate the average we strore some values (4) then do some math to calculate the average
      {
        rates[rateSpot++] = (byte)beatsPerMinute;  //Store this reading in the array
        rateSpot %= RATE_SIZE;                     //Wrap variable

        //Take average of readings
        beatAvg = 0;
        for (byte x = 0; x < RATE_SIZE; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
        Blynk.virtualWrite(V1, beatAvg);
      }
    }
  }
  if (irValue < 7000) {  //If no finger is detected it inform the user and put the average BPM to 0 or it will be stored for the next measure
    Blynk.virtualWrite(V0, "No");
    beatAvg = 0;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(30, 5);
    display.println("Please Place ");
    display.setCursor(30, 15);
    display.println("your finger ");
    display.display();
    noTone(14);
    Blynk.virtualWrite(V1, beatAvg);
  }
}
