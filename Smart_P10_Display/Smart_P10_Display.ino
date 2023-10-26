/*--------------------------------------------------------------------------------------

 dmd_test.cpp 
   Demo and example project for the Freetronics DMD, a 512 LED matrix display
   panel arranged in a 32 x 16 layout.

 Copyright (C) 2011 Marc Alexander (info <at> freetronics <dot> com)

 See http://www.freetronics.com/dmd for resources and a getting started guide.

 Note that the DMD library uses the SPI port for the fastest, low overhead writing to the
 display. Keep an eye on conflicts if there are any other devices running from the same
 SPI port, and that the chip select on those devices is correctly set to be inactive
 when the DMD is being written to.

 USAGE NOTES
 -----------

 - Place the DMD library folder into the "arduino/libraries/" folder of your Arduino installation.
 - Get the TimerOne library from here: http://code.google.com/p/arduino-timerone/downloads/list
   or download the local copy from the DMD library page (which may be older but was used for this creation)
   and place the TimerOne library folder into the "arduino/libraries/" folder of your Arduino installation.
 - Restart the IDE.
 - In the Arduino IDE, you can open File > Examples > DMD > dmd_demo, or dmd_clock_readout, and get it
   running straight away!

 * The DMD comes with a pre-made data cable and DMDCON connector board so you can plug-and-play straight
   into any regular size Arduino Board (Uno, Freetronics Eleven, EtherTen, USBDroid, etc)
  
 * Please note that the Mega boards have SPI on different pins, so this library does not currently support
   the DMDCON connector board for direct connection to Mega's, please jumper the DMDCON pins to the
   matching SPI pins on the other header on the Mega boards.

 This example code is in the public domain.
 The DMD library is open source (GPL), for more see DMD.cpp and DMD.h

--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------
  Includes
--------------------------------------------------------------------------------------*/
#include <SPI.h>       //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>       //
#include <TimerOne.h>  //
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#include "Arial14.h"


//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 3
#define DISPLAYS_DOWN 2
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

char welcome_screen[] = "Welcome to IoT Design";
char message[200];
char char_read;
byte index = 0;
int i;
int x;

/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
void ScanDMD() {
  dmd.scanDisplayBySPI();
}

/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/
void setup(void) {
  // begin serial communication
  Serial.begin(9600);

  //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
  Timer1.initialize(5000);          //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt(ScanDMD);  //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  //clear/init the DMD pixels held in RAM
  dmd.clearScreen(true);  //true is normal (all pixels off), false is negative (all pixels on)
  strcpy(message, welcome_screen);
  // welcomeMessage();
  welcomeMessage();
}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/
void loop(void) {
  if (Serial.available()) {
    for (i = 0; i < 199; i++) {
      message[i] = '\0';
      Serial.print(message[i]);
    }
    index = 0;
  }
  while (Serial.available() > 0) {
    dmd.clearScreen(true);
    if (index < 199) {
      char_read = Serial.read();
      message[index] = char_read;
      index++;
    }
  }

  dmd.clearScreen(true);
  dmd.drawBox(0, 0, (32 * DISPLAYS_ACROSS) - 1, (16 * DISPLAYS_DOWN) - 1, GRAPHICS_NORMAL);
  dmd.selectFont(SystemFont5x7);
  dmd.drawString(19, 2, "Welcome to", 10, GRAPHICS_NORMAL);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(5, 14, "CEE DEPT.", 9, GRAPHICS_NORMAL);
  delay(10000);
  dmd.selectFont(Arial_Black_16);
  dmd.drawMarquee("              ", 14, 0, -16);
  clearPixelTopDown();

  dmd.drawMarquee(message, 200, (32 * DISPLAYS_ACROSS) - 1, 8);
  scrollRightToLeft(50);
}

void displayScrollFromBottom(int terminateStep, int stepTime, int textDisplayTime){
  // dmd.drawMarquee(message, 9, 0, (16 * DISPLAYS_DOWN) - 1);
  long start = millis();
  long timer = start;
  boolean ret = false;
  
  while (!ret) {
    if(x == terminateStep){
      x=0;
      break;
    }
    if ((timer + stepTime) < millis()) {
      ret = dmd.stepMarquee(0, -1);
      timer = millis();
      x++;
    }
    
  }
  delay(textDisplayTime);
}

void scrollRightToLeft( int stepTime){
   long start = millis();
  long timer = start;
  boolean ret = false;
  
  while (!ret) {
    if ((timer + stepTime) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
    }
  }
}

void clearPixelTopDown(){
  long start = millis();
  long timer = start;
  boolean ret = false;
  while (!ret) {
    if ((timer + 100) < millis()) {
      ret = dmd.stepMarquee(0, 1);
      timer = millis();
    }
  }
}

void welcomeMessage(){
  dmd.clearScreen(true);
  dmd.drawBox(0, 0, (32 * DISPLAYS_ACROSS) - 1, (16 * DISPLAYS_DOWN) - 1, GRAPHICS_NORMAL);
  dmd.selectFont(SystemFont5x7);
  dmd.drawString(17, 2, "SMART (KJ02)", 12, GRAPHICS_NORMAL);
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(5, 14, " DISPLAY ", 9, GRAPHICS_NORMAL);
  delay(5000);
}

