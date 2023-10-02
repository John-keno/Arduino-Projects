#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "kejo"
#define WIFI_PASSWORD "ced$$mon$$ghost~"

/* 2. Define the API Key */
#define API_KEY "AIzaSyCQkb21Q7i1ze38_xIudTeVC0Ti0fI0qlM"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://rfid-web-2a373-default-rtdb.firebaseio.com"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "attendance@kejoteck.com"
#define USER_PASSWORD "kejoteckattender"

// Define Firebase Data object
FirebaseData fbdo;


FirebaseAuth auth;
FirebaseConfig config;

RFID rfid(D8, D0);                   //D10:pin of tag reader SDA. D9:pin of tag reader RST
unsigned char str[MAX_LEN];          //MAX_LEN is 16: size of the array
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 3600;  //(UTC+1:00)
NTPClient timeClient(ntpUDP, "pool.ntp.org");

FirebaseJson json;

const int red = D4;
const int green = D3;
String alertMsg;
String errorMsg;
String device_id = "Device_JK11";
boolean checkIn = true;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\n connected!");
}

void setup() {

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  lcd.init();  // initialize the lcd
  lcd.clear();
  lcd.backlight();

  SPI.begin();
  rfid.init();

  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
  connect();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the API key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL */
  config.database_url = DATABASE_URL;

  lcd.setCursor(1, 0);
  lcd.print("LOADING");
  lcd.setCursor(1, 1);
  lcd.print("CONN TO SERVER");

  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h
  
  Firebase.reconnectNetwork(true);

  fbdo.setBSSLBufferSize(4096, 1024);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  config.timeout.serverResponse = 10 * 1000;
  lcd.clear();
}
void checkAccess(String temp)  //Function to check if an identified tag is registered to allow access
{
  lcd.setCursor(1, 0);
  lcd.print("SCANNING RFID");

  if (Firebase.RTDB.getInt(&fbdo, "/users/" + temp) && Firebase.ready()) {
    String path = "/attendence";

    if (fbdo.intData() == 0)  //If firebaseData.intData() == checkIn
    {
      alertMsg = "CHECKING IN";
      lcd.setCursor(2, 1);
      lcd.print(alertMsg);
      delay(2000);

      json.add("time", getDate());
      json.add("id", device_id);
      json.add("uid", temp);
      json.add("status", 1);

      Firebase.RTDB.setInt(&fbdo, "/users/" + temp, 1);


      if (Firebase.RTDB.pushJSON(&fbdo, path, &json)) {
        Serial.println(fbdo.dataPath() + fbdo.pushName());
        alertMsg = "DONE AND SENT";
        lcd.setCursor(1, 1);
        lcd.print(alertMsg);
        delay(2000);
      } else {
        Serial.println(fbdo.errorReason());
        errorMsg = "ERROR SENDING";
        lcd.setCursor(1, 1);
        lcd.print(errorMsg);
        delay(2000);
      }
    } else if (fbdo.intData() == 1)  //If the lock is open then close it
    {
      alertMsg = "CHECKING OUT";
      lcd.setCursor(2, 1);
      lcd.print(alertMsg);
      delay(2000);

      Firebase.RTDB.setInt(&fbdo, "/users/" + temp, 0);

      json.add("time", getDate());
      json.add("id", device_id);
      json.add("uid", temp);
      json.add("status", 0);
      if (Firebase.RTDB.pushJSON(&fbdo, path, &json)) {
        Serial.println(fbdo.dataPath() + fbdo.pushName());
        alertMsg = "DONE AND SENT";
        lcd.setCursor(1, 1);
        lcd.print(alertMsg);
        delay(2000);
      } else {
        Serial.println(fbdo.errorReason());
        errorMsg = "ERROR SENDING";
        lcd.setCursor(1, 1);
        lcd.print(errorMsg);
        delay(2000);
      }
    }

  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
    lcd.setCursor(1, 0);
    lcd.print("FAILED TO CONN");
    lcd.setCursor(1, 1);
    lcd.print("ERROR CODE 503");
    delay(2000);
    delay(500);
    lcd.clear();
  }
}
void loop() {
  timeClient.update();
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)  //Wait for a tag to be placed near the reader
  {
    Serial.println("Card found");
    String temp = "";                 //Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)  //Anti-collision detection, read tag serial number
    {
      Serial.print("The card's ID number is : ");
      for (int i = 0; i < 4; i++)  //Record and display the tag serial number
      {
        temp = temp + (0x0F & (str[i] >> 4));
        temp = temp + (0x0F & str[i]);
      }
      Serial.println(temp);
      checkAccess(temp);  //Check if the identified tag is an allowed to open tag
    }
    rfid.selectTag(str);  //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  rfid.halt();

  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(1, 0);
    lcd.print("SCAN YOUR RFID");
    lcd.setCursor(2, 1);
    lcd.print("DEVICE READY");
    delay(500);
    lcd.clear();
  } else {
    lcd.setCursor(1, 0);
    lcd.print("NO CONNECTION");
    lcd.setCursor(1, 1);
    lcd.print("ERROR CODE 503");
    delay(500);
    lcd.clear();
  }
}

String getDate() {
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  String formattedTime = timeClient.getFormattedTime();
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  int monthDay = ptm->tm_mday;

  String yy, mm, dd;
  if (currentYear < 10) {
    yy = "0";
    yy += String(currentYear);
  } else {
    yy = String(currentYear);
  }
  if (currentMonth < 10) {
    mm = "0";
    mm += String(currentMonth);
  } else {
    mm = String(currentMonth);
  }
  if (monthDay < 10) {
    dd = "0";
    dd += String(monthDay);
  } else {
    dd = String(monthDay);
  }

  String currentDate = yy + "-" + mm + "-" + dd + "T" + formattedTime + "Z";
  Serial.println(currentDate);
  return currentDate;
}