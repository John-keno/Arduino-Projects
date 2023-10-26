// ---------------------------------------------------------------------------
// Before attempting to use this sketch, please read the "Help with 15 Sensors Example Sketch":
// https://bitbucket.org/teckel12/arduino-new-ping/wiki/Help%20with%2015%20Sensors%20Example%20Sketch
//
// This example code was used to successfully communicate with 15 ultrasonic sensors. You can adjust
// the number of sensors in your project by changing SONAR_NUM and the number of NewPing objects in the
// "sonar" array. You also need to change the pins for each sensor for the NewPing objects. Each sensor
// is pinged at 33ms intervals. So, one cycle of all sensors takes 495ms (33 * 15 = 495ms). The results
// are sent to the "oneSensorCycle" function which currently just displays the distance data. Your project
// would normally process the sensor results in this function (for example, decide if a robot needs to
// turn and call the turn function). Keep in mind this example is event-driven. Your complete sketch needs
// to be written so there's no "delay" commands and the loop() cycles at faster than a 33ms rate. If other
// processes take longer than 33ms, you'll need to increase PING_INTERVAL so it doesn't get behind.
// ---------------------------------------------------------------------------
#include <NewPing.h>

#define SONAR_NUM 2       // Number of sensors.
#define MAX_DISTANCE 200  // Maximum distance (in cm) to ping.
#define PING_INTERVAL 29  // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
#define pump1 8
#define pump2 10


unsigned long pingTimer[SONAR_NUM];  // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];          // Where the ping distances are stored.
uint8_t currentSensor = 0;           // Keeps track of which sensor is active.

long currentTime;
long previousTime = 0;

NewPing sonar[SONAR_NUM] = {
  // Sensor object array.
  NewPing(4, 5, MAX_DISTANCE),  // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(6, 7, MAX_DISTANCE),
};

void setup() {
  Serial.begin(115200);
  pingTimer[0] = millis() + 75;            // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++)  // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;

  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  digitalWrite(pump1, HIGH);
  digitalWrite(pump2, HIGH);
}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) {                            // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {                                    // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;                       // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle();  // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();                               // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                                               // Sensor being accessed.
      cm[currentSensor] = 0;                                           // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck);                      // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  // Other code that *DOESN'T* analyze ping results can go here.

  currentTime = millis();
  if (currentTime - previousTime >= 1000) {
    if (cm[0] > 0 && cm[0] <= 10) {
      Serial.print("water pump 1 on");
      digitalWrite(pump1, LOW);
    } else {
      Serial.print(" water pump 1 off");
      digitalWrite(pump1, HIGH);
    }

    if (cm[1] > 0 && cm[1] <= 10) {
      Serial.print(" water pump 2 on");
      digitalWrite(pump2, LOW);
    } else {
      Serial.print(" water pump 2 off");
      digitalWrite(pump2, HIGH);
    }

    Serial.println();
    previousTime = currentTime;
  }
}

void echoCheck() {  // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() {  // Sensor ping cycle complete, do something with the results.
  // The following code would be replaced with your code that does something with the ping results.
  // for (uint8_t i = 0; i < SONAR_NUM; i++) {
  //   Serial.print(i);
  //   Serial.print("=");
  //   Serial.print(cm[i]);
  //   Serial.print("cm ");

  // }
  // Serial.println();
}