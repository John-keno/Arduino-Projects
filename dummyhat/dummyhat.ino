#define switchButton 4
#define fanPin 5
#define alertButton 6
#define buzzerButton 7

int buttonState = 0;

void setup() {
  pinMode(switchButton, INPUT_PULLUP);
  pinMode(fanPin, OUTPUT);
  pinMode(alertButton, INPUT_PULLUP);
  pinMode(buzzerButton, INPUT_PULLUP);

}

void loop() {

  if(buttonPressed(switchButton) && buttonState==0){}
  if(buttonPressed(switchButton) && buttonState==1){}
  if(buttonPressed(switchButton) && buttonState==2){}
  if(buttonPressed(switchButton) && buttonState==3){}

}

boolean buttonPressed(int button) {
  delay(100);
  if (digitalRead(button) == LOW) {
    return true;
  } else {
    return false;
  }