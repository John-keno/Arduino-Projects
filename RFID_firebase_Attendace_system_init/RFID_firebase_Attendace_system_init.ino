#include <ESP8266WiFi.h>
#include <SPI.h>
#include <RFID.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "kejoteck"
#define WIFI_PASSWORD "ghost1234"

/* 2. Define the API Key */
#define API_KEY "AIzaSyCQkb21Q7i1ze38_xIudTeVC0Ti0fI0qlM"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://rfid-web-2a373-default-rtdb.firebaseio.com/"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "admin@kejoteck.com"
#define USER_PASSWORD "kejoteckadmin"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

RFID rfid(D8, D0);           //D8:pin of tag reader SDA. D0:pin of tag reader RST
unsigned char str[MAX_LEN];  //MAX_LEN is 16: size of the array

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

  SPI.begin();
  rfid.init();

  connect();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;
}
void pushUser(String temp)  //Function to check if an identified tag is registered to allow access
{
  if (Firebase.ready()) {
    // dataMillis = millis();
    String path = "/users/";  //<- user uid
    path += temp;
    Serial.println("USER ID: " + temp);
    Serial.printf("Adding USER  ... %s\n", Firebase.RTDB.setInt(&fbdo, path, 0) ? "ok" : fbdo.errorReason().c_str());
  }
  // Serial.println("PUSHING USER ID: " + temp);

  // Firebase.setInt(firebaseData, uidPath + "users/" + temp, 0);
}
void loop() {
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
      pushUser(temp);  //Check if the identified tag is an allowed to open tag
    }
    rfid.selectTag(str);  //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  rfid.halt();
}
