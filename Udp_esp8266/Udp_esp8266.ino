/*
  UDPSendReceive.pde:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.

  created 21 Aug 2010
  by Michael Margolis

  This code is in the public domain.

  adapted from Ethernet library examples
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <L298NX2.h>

// Pin definition
const unsigned int IN1_A = 16;
const unsigned int IN2_A = 5;

const unsigned int IN1_B = 0;
const unsigned int IN2_B = 2;

L298NX2 motors(IN1_A, IN2_A, IN1_B, IN2_B);

#ifndef STASSID
#define STASSID "keno"
#define STAPSK  "cedb176e5e10"
#endif

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "0";       // a string to send back

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
}

void loop() {
  int i = 0;
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
                  packetSize,
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                  Udp.destinationIP().toString().c_str(), Udp.localPort(),
                  ESP.getFreeHeap());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    String val = (String) packetBuffer;
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();

    if (val.equals("auto mode")){
      runAutoMode();
    }
    else if (val.equals("manual mode")){
      runManualMode();
    }
  }
}

void moveForward(){
  motors.forward();
  
}
void moveBackwards(){
  motors.backward();
}
void turnLeft(){
  motors.backwardA();
  motors.forwardB();
}
void turnRight(){
  motors.backwardB();
  motors.forwardA();
}
void runAutoMode(){
  int a = 0;
  if 
}
void runManualMode(){
   if (val.equals("move forward")){
      moveForward();
    }
    if (val.equals ("move backwards")){
      moveBackwards();
    }

    else if (val.equals("turn left")){
      turnLeft();
    }
    else if (val.equals("turn right")){
      turnRight();
    }
    else if (val.equals("pick trash")){
//      pickTrash();
    }
    else{
      motors.stop();
    }
    i = 0;
}
/*
  test (shell/netcat):
  --------------------
	  nc -u 192.168.esp.address 8888
*/
