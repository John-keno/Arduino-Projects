
#define hotSensor 8
#define coldSensor 9
#define led 13
void setup() {
  // put your setup code here, to run once:
  pinMode(hotSensor, INPUT_PULLUP);
  pinMode(coldSensor, INPUT_PULLUP);
//  pinMode(hotSensor, INPUT);
//  pinMode(coldSensor, INPUT);
  pinMode (led , OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(hotSensor);
  int val2 = digitalRead(coldSensor);

  Serial.print(val);
  Serial.print("==========");
  Serial.println(val2);

  if (val == HIGH){
    digitalWrite(led, HIGH);
  }
  else if (val == LOW){
    digitalWrite(led, LOW);
  }
  
}
