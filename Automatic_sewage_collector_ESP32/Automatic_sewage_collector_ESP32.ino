
#include <WiFi.h>
#include <WiFiUdp.h>
#include "HC_SR04.h"
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 4

int timeout = 180; // seconds to run for

#define RXD2 16
#define TXD2 17
#define trigPin1 18
#define echoPin1 19
#define echoint 0

WiFiManager wm; 

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address

// buffers for receiving and sending data
const int udpPort = 8888;
char packetBuffer[20]; //buffer to hold incoming packet,
// char  ReplyBuffer[] = "00";       // a string to send back
int duration1, distance1;
String reply = "connected";

unsigned long previousMicros = 0;
unsigned long previousMicros2 = 0;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

TaskHandle_t Task1;
// QueueHandle_t queue;



HC_SR04 sensor(trigPin1, echoPin1, echoint);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  
  
  //Connect to the WiFi network
  // connectToWiFi();
  Serial.println("Connecting to WiFi network... ");
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // queue = xQueueCreate(5, sizeof(int));
  // Initialize your task (1nd loop)
  xTaskCreatePinnedToCore(
    binTask,          // name of the task function
    "getBinLevel",  // name of the task
    1000,           // memory assigned for the task
    NULL,           // parameter to pass if any
    1,              // priority of task, starting from 0(Highestpriority) *IMPORTANT*( if set to 1 and there is no activity in your 2nd loop, it will reset the esp32)
    &Task1,         // Reference name of taskHandle variable
    1);             // choose core (0 or 1)

sensor.begin();

bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("Robotic Bin", "roboticdustbin"); // password protected ap

  if(!res) {
    Serial.println("Failed to connect or hit timeout");
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
    udp.begin(udpPort);
    connected = true;
  }
}

void loop() {

  checkButton();
  int packetSize = udp.parsePacket();
      if(connected){
        if (packetSize) {
          // Serial.printf("Received packet of size %d from %s:%d\n",
          //               packetSize,
          //               udp.remoteIP().toString().c_str(), udp.remotePort());

          // read the packet into packetBufffer
          int n = udp.read(packetBuffer, 20);
          packetBuffer[n] = 0;
          // Serial.print("Contents: ");
          // Serial.println(packetBuffer);

          String val = (String) packetBuffer;
          // send a reply, to the IP address and port that sent us the packet we received
          udp.beginPacket(udp.remoteIP(), udp.remotePort());
          
          udp.println(reply);
          // Serial.print(reply);
          // Serial.println("task 1");
          udp.endPacket();
        
          if (val.equals("turn right")){
            goRight();
            }
            else if (val.equals("turn left")){
            goleft();
            }
            else if (val.equals("move forward")) {
            goForward();
            }
            else if (val.equals("move backwards")) {
            goBackward();
            }
            else if (val.equals("pick trash")){
              goPickTrash();
            }
            else if(val.equals("stop")){
              Serial2.write("stop#");
            }else{
              Serial2.write("stop#");
            }
          }
        }
    
  }
void goRight(){
  Serial2.write("R#");
}

void goleft(){
  Serial2.write("L#");
}

void goForward(){
  Serial2.write("F#");
}

void goBackward(){
  Serial2.write("B#");
}

void goPickTrash(){
  Serial2.write("PT#");
}

void binTask(void * parameter){
  for(;;){
    sensor.start();
    while(!sensor.isFinished()) continue;
    unsigned int bin = sensor.getRange();
    // sensor.isFinished();
    unsigned long currentMicros = millis();
    (bin < 9)? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW);
    Serial.print(bin);
    Serial.println("task 0");
    delay(1000);
  }
}

void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal("onDemand(Robotic bin)", "roboticdustbin")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
      }
    }
  }
}
