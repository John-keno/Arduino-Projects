/*

  ESP8266 WiFi Server with SPIFFS
  by Roland Pelayo

  Full tutorial: https://www.teachmemicro.com/esp8266-spiffs-web-server-nodemcu/

  Rev 1.0 - Initial code - September 19, 2020

*/

//Declare libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

//provide your own WiFi SSID and password
const char* ssid     = "CEE-SERVERLESS-SWITCH";
const char* password = "hope123456";

const int light = 2;

//Create WebServer instance
ESP8266WebServer server(80);

void setup() {
  //Setup on-board LED
  pinMode(light, OUTPUT);
  digitalWrite(light, HIGH);

  //For debugging
  Serial.begin(115200);
  //Initiate WiFi Connection
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  if (!SPIFFS.begin())
  {
    // Serious problem
    Serial.println("SPIFFS Mount failed");
  } else {
    Serial.println("SPIFFS Mount succesfull");
  }

  server.onNotFound([]() { // If the client requests any URI
    if (!handleFileRead(server.uri())) // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  //start web server
  server.begin();
  //Just stating things

  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); //make the ESP32 respond to web clients
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html"; // If a folder is requested, send the index file
  String contentType = getContentType(path); // Get the MIME type
  if (SPIFFS.exists(path)) { // If the file exists
    File file = SPIFFS.open(path, "r"); // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    if (path == "/lightOn.html") {
      digitalWrite(light, LOW);
    } else if (path == "/lightOff.html") {
      digitalWrite(light, HIGH);
    }
    file.close(); // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false; // If the file doesn't exist, return false
}

String getContentType(String filename) {
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  return "text/plain";
}
