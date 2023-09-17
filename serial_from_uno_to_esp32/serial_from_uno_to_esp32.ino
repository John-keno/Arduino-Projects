
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  String s = Serial.readString();
  Serial.print("recieved:");
  Serial.println(s);
  delay(200);

}
