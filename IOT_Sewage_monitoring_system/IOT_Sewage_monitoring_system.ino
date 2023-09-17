#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
const int trigPin1 = D3;
const int echoPin1 = D2;
#define redled D4
int duration1,distance1;
int count=0;
char auth[]="yDLmNkUIgLAIJ7BFhrKXQrnTKTnL7fKE";// Blynk Auth code sent to your mail after creating the account
//char auth[]="xH5NUn-0uGvX3HGJX2HD0YtZQ-iy-u-F";// Blynk Auth code sent to your mail after creating the account
char ssid[] = "SAM";// Router or Hotspot name
//char ssid[] = "Airtel 4G MiFi_78FC";// Router or Hotspot name
//char pass[] = "iweh4life";// Router or Hotspot Password
char pass[] = "peterp419";// Router or Hotspot Password
void setup()
{
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(A0, INPUT);
digitalWrite(A0, OUTPUT);
pinMode(redled, OUTPUT);
digitalWrite(redled, LOW);
Serial.begin(9600);
Blynk.begin(auth, ssid, pass);
}
void loop(){
  
  int float1=analogRead(A0);
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;

  
  Serial.print(distance1);
  Serial.println("Centimeter:");
  delay(1000);
  Serial.println(float1);
   Blynk.virtualWrite(V1,distance1);
   
   if(distance1<=5)
   {
    Blynk.notify("Blockage Alert!");
    digitalWrite(redled, HIGH);
   }
   else if(float1 <=800)
   {
    Blynk.notify("Overflow Alert !");
     digitalWrite(redled, HIGH);
   }
      else
   {
    digitalWrite(redled, LOW);
   }
    Blynk.run();
 }
