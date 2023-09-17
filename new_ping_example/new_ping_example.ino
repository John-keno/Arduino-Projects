#include <NewPing.h>
#include <Stepper.h>

#define SONAR_NUM      2 // Number or sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

//unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
//unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(22, 23, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(26, 27, MAX_DISTANCE)
  //NewPing(50, 51, MAX_DISTANCE),
  //NewPing(52, 53, MAX_DISTANCE)
};
