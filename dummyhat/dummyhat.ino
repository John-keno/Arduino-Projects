#include <SoftwareSerial.h>
SoftwareSerial sim800L(3,2);

void setup() {
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  sim800L.begin(9600);

  Serial.println("Initializing...");
  
  sim800L.println("AT");
  waitForResponse();

  sim800L.println("ATE1");
  waitForResponse();

  sim800L.println("AT+CMGF=1");
  waitForResponse();

  sim800L.println("AT+CNMI=1,2,0,0,0");
  waitForResponse();

}

void loop() {
make_call("+2348064810572");

}
void make_call(String phoneNo){
  sim800L.println("ATD"+phoneNo+";");
  waitForResponse();
}

void waitForResponse(){
  delay(1000);
  while(sim800L.available()){
    Serial.println(sim800L.readString());
  }
//  sim800L.read();
}
