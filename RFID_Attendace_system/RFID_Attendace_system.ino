// CONNECTIONS :
// DS1302 CLK/SCLK --> 6
// DS1302 DAT/IO --> 7
// DS1302 RST/CE --> 8
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include <MFRC522.h> // for the RFID
#include <SPI.h> // for the RFID and SD card module
#include <SD.h> // for the SD card
#include <RTClib.h> // for the RTC
 
// define pins for RFID
#define CS_RFID 10
#define RST_RFID 9
// define select pin for SD card module
#define CS_SD 4 
 
// Create a file to store the data
File myFile;
 
// Instance of the class for RFID
MFRC522 rfid(CS_RFID, RST_RFID); 

// Init array that will store new NUID 
byte nuidPICC[4];
 
// Variable to hold the tag's UID
String uidString;
 
// Instance of the class for RTC
ThreeWire myWire(7,6,8); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
 
// Define check in time
const int checkInHour = 9;
const int checkInMinute = 5;
 
//Variable to hold user check in
int userCheckInHour;
int userCheckInMinute;
 
// Pins for LEDs and buzzer
const int redLED = 6;
const int greenLED = 7;
const int buzzer = 5;
 
void setup() {
  
  // Set LEDs and buzzer as outputs
  pinMode(redLED, OUTPUT);  
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // Init Serial port
  Serial.begin(9600);

  //init LCD
  lcd.init();
  lcd.backlight();
  
  // Init SPI bus
  SPI.begin();

  //Init RTC
  Rtc.Begin();

  // Init MFRC522 
  rfid.PCD_Init(); 
 
  // Setup for the SD card
  Serial.print("Initializing SD card...");
  lcd.print("Initializing ");
  lcd.setCursor(0, 1);
  lcd.print("SD card...");
  delay(3000);
  lcd.clear();
  if(!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    lcd.print("Initializing ");
    lcd.setCursor(0, 1);
    lcd.print("failed!");
    return;
  }
  Serial.println("initialization done.");
  lcd.print("Initialization ");
  lcd.setCursor(0, 1);
  lcd.print("Done...");
 
  // Setup for the RTC  
    setupRTC();

}
 
void loop() {
  //look for new cards
  if(!rfid.PICC_IsNewCardPresent())
    return;

  if(!rfid.PICC_ReadCardSerial())
    return;

  readRFID();
  logCard();
  verifyCheckIn();

}
 
void readRFID() {
   // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    }
  // String uidString = "";
  lcd.clear();
  Serial.print("Tag UID: ");
  lcd.print("Tag UID: ");
  uidString = (String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + 
    String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]));
  // for (byte i = 0; i < rfid.uid.size; i++) {
  //   uidString += rfid.uid.uidByte[i];
  //   Serial.print(' ');
  //   Serial.print(rfid.uid.uidByte[i], DEC);
  //   }
  
  lcd.setCursor(0, 1);
  lcd.print(uidString);
  delay(2000);
 
  // Sound the buzzer when a card is read
  tone(buzzer, 2000); 
  delay(200);        
  noTone(buzzer);
  
  delay(200);
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
 
void logCard() {
  
  
  // Open file
  myFile=SD.open("log.txt", FILE_WRITE);
 
  // If the file opened ok, write to it
  if (myFile) {
    Serial.println("File opened ok");
    lcd.clear();
    lcd.print("File opened ok");
    delay(2000);
    myFile.print(uidString);
    myFile.print(", ");   
    
    // Save time on SD card
    RtcDateTime now = Rtc.GetDateTime();
    myFile.print(now.Year(), DEC);
    myFile.print('/');
    myFile.print(now.Month(), DEC);
    myFile.print('/');
    myFile.print(now.Day(), DEC);
    myFile.print(',');
    myFile.print(now.Hour(), DEC);
    myFile.print(':');
    myFile.println(now.Minute(), DEC);
    
    // Print time on Serial monitor
    Serial.print(now.Year(), DEC);
    Serial.print('/');
    Serial.print(now.Month(), DEC);
    Serial.print('/');
    Serial.print(now.Day(), DEC);
    Serial.print(' ');
    Serial.print(now.Hour(), DEC);
    Serial.print(':');
    Serial.println(now.Minute(), DEC);
    Serial.println("sucessfully written on SD card");
 
    lcd.clear();
    lcd.print(now.Year(), DEC);
    lcd.print(':');
    lcd.print(now.Month(), DEC);
    lcd.print(':');
    lcd.print(now.Day(), DEC);
    lcd.print(' ');
    lcd.setCursor(11, 0);
    lcd.print(now.Hour(), DEC);
    lcd.print(':');
    lcd.print(now.Minute(), DEC);
    lcd.setCursor(0, 1);
    lcd.print("Written on SD...");
    delay(2000);
    
    myFile.close();
 
    // Save check in time;
    userCheckInHour = now.Hour();
    userCheckInMinute = now.Minute();
  }
  else {
    
    Serial.println("error saving file");  
    lcd.clear();
    lcd.print("error saving file");
  }
  
}
 
void verifyCheckIn(){
  if((userCheckInHour < checkInHour)||((userCheckInHour==checkInHour) && (userCheckInMinute <= checkInMinute))){
    digitalWrite(greenLED, HIGH);
    delay(2000);
    digitalWrite(greenLED,LOW);
    Serial.println("You're welcome!");
    lcd.clear();
    lcd.print("You're Welcome!");
  }
  else{
    digitalWrite(redLED, HIGH);
    delay(2000);
    digitalWrite(redLED,LOW);
    Serial.println("You are late...");
    lcd.clear();
    lcd.print("You are Late...");
    delay(3000);
    lcd.clear();
    lcd.print("Put RFID to Scan");
    
  }
}

void setupRTC(){
  
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("RTC Not Running!");
        delay(1000);
        lcd.setCursor(0, 1);
        lcd.print("starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i+1) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
    
  }
}
