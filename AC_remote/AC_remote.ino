/* rawSend.ino Example sketch for IRLib2
 *  Illustrates how to send a code Using raw timings which were captured
 *  from the "rawRecv.ino" sample sketch.  Load that sketch and
 *  capture the values. They will print in the serial monitor. Then you
 *  cut and paste that output into the appropriate section below.
 */
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender

#define POW_btn 4
#define UP_btn 7
#define DOWN_btn 8

int acState = 0;
int btnNewState;
int btnOldState = 1;
int incrTemp;
int decrTemp;

IRsendRaw mySender;

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  Serial.println(F("Every time you press a key is a serial monitor we will send."));
}
/* Cut and paste the output from "rawRecv.ino" below here. It will 
 * consist of a #define RAW_DATA_LEN statement and an array definition
 * beginning with "uint16_t rawData[RAW_DATA_LEN]= {…" and concludes
 * with "…,1000};"
 */
#define RAW_DATA_LEN 168
uint16_t rawDataOn[RAW_DATA_LEN]={
  8974, 4490, 562, 634, 586, 610, 558, 634, 
  590, 606, 562, 630, 562, 634, 562, 1722, 
  562, 1726, 558, 1722, 586, 610, 562, 634, 
  558, 634, 558, 638, 558, 634, 586, 610, 
  558, 1722, 586, 610, 558, 638, 558, 634, 
  562, 634, 558, 1726, 586, 610, 558, 634, 
  562, 634, 586, 606, 562, 1722, 562, 1726, 
  558, 1726, 562, 630, 562, 1722, 562, 634, 
  558, 1730, 582, 1000};
  uint16_t rawDataOn2[RAW_DATA_LEN]={
   454, 634, 454, 638, 
  454, 634, 454, 634, 470, 622, 454, 638, 
  478, 610, 454, 638, 454, 634, 454, 638, 
  454, 634, 482, 606, 454, 638, 454, 638, 
  454, 1722, 454, 638, 454, 634, 482, 610, 
  454, 634, 454, 634, 454, 638, 454, 634, 
  482, 610, 454, 634, 454, 638, 454, 634, 
  454, 638, 454, 634, 454, 638, 454, 634, 
  454, 638, 454, 634, 454, 638, 454, 634, 
  454, 638, 454, 634, 462, 630, 454, 634, 
  454, 638, 510, 578, 454, 638, 454, 634, 
  454, 638, 478, 610, 454, 634, 454, 638, 
  454, 634, 454, 638, 454, 634, 454, 1000
  };

uint16_t rawDataOff[RAW_DATA_LEN]={
  8970, 4518, 502, 694, 530, 662, 506, 690, 
  502, 690, 506, 690, 506, 690, 562, 1722, 
  530, 1754, 506, 1778, 502, 694, 502, 690, 
  502, 690, 506, 690, 566, 630, 530, 662, 
  534, 1754, 534, 658, 530, 666, 474, 718, 
  566, 630, 530, 1754, 502, 694, 502, 690, 
  530, 662, 530, 666, 474, 1810, 538, 1746, 
  530, 1754, 530, 662, 530, 1758, 530, 662, 
  506, 1778, 522, 1000, 454, 634, 478, 610, 482, 610, 478, 610, 
  478, 614, 478, 614, 474, 614, 478, 614, 
  474, 614, 478, 614, 450, 638, 478, 614, 
  478, 610, 478, 610, 454, 638, 478, 610, 
  454, 638, 478, 1702, 478, 610, 454, 638, 
  474, 614, 454, 638, 450, 638, 454, 634, 
  462, 630, 454, 634, 478, 1702, 482, 1698, 
  454, 638, 454, 638, 474, 614, 450, 642, 
  450, 638, 454, 634, 454, 634, 482, 610, 
  454, 638, 466, 622, 454, 638, 450, 638, 
  478, 610, 454, 638, 450, 642, 478, 610, 
  478, 614, 450, 634, 454, 638, 478, 610, 
  478, 614, 474, 1000};

uint16_t rawDataUp[RAW_DATA_LEN]={
  8994, 4474, 578, 638, 558, 634, 558, 638, 
  558, 638, 554, 638, 558, 638, 554, 1730, 
  554, 1710, 574, 1730, 554, 638, 558, 638, 
  554, 638, 558, 638, 558, 638, 558, 634, 
  558, 638, 558, 1726, 502, 690, 558, 618, 
  578, 638, 554, 1730, 502, 690, 558, 638, 
  502, 690, 558, 1726, 562, 634, 558, 1726, 
  558, 1726, 558, 638, 554, 1730, 554, 642, 
  554, 638, 558, 502, 590, 502, 586, 506, 586, 502, 586, 
  506, 586, 454, 638, 506, 586, 502, 586, 
  502, 590, 502, 586, 506, 586, 450, 642, 
  502, 586, 454, 638, 502, 586, 506, 586, 
  506, 562, 526, 586, 506, 1674, 506, 586, 
  506, 582, 454, 638, 506, 586, 450, 638, 
  502, 590, 502, 586, 506, 586, 506, 586, 
  502, 586, 498, 594, 502, 586, 506, 586, 
  502, 586, 506, 586, 506, 586, 450, 638, 
  454, 614, 526, 586, 506, 586, 506, 582, 
  454, 638, 502, 590, 506, 582, 506, 586, 
  454, 634, 454, 638, 506, 586, 502, 566, 
  526, 586, 506, 1000};

 uint16_t rawDataDown[RAW_DATA_LEN]={
 8946, 4526, 534, 662, 506, 690, 502, 694, 
 502, 690, 506, 690, 530, 666, 502, 1778, 
  530, 1754, 534, 1754, 506, 686, 534, 662, 
  506, 690, 502, 690, 506, 690, 530, 666, 
  530, 666, 502, 690, 530, 662, 506, 694, 
  502, 690, 526, 1758, 530, 662, 506, 690, 
  506, 690, 530, 662, 530, 1758, 502, 1782, 
  530, 1754, 530, 666, 530, 1758, 526, 670, 
  526, 666, 502, 450, 638, 478, 614, 474, 614, 454, 638, 
  450, 638, 478, 610, 454, 638, 450, 642, 
  474, 614, 454, 638, 474, 610, 482, 614, 
  450, 638, 454, 634, 478, 614, 450, 642, 
  450, 1730, 446, 638, 454, 638, 478, 614, 
  474, 614, 478, 614, 450, 638, 450, 638, 
  478, 614, 450, 638, 478, 614, 450, 638, 
  478, 614, 450, 638, 450, 642, 450, 638, 
  454, 638, 478, 614, 450, 634, 478, 614, 
  454, 638, 478, 606, 482, 610, 454, 642, 
  474, 614, 450, 642, 474, 614, 478, 610, 
  454, 638, 450, 638, 474, 618, 474, 614, 
  478, 614, 474, 1000};







/*
 * Cut-and-paste into the area above.
 */
   
void loop() {
  btnNewState = digitalRead(POW_btn);
   if (acState == 1){
      incrTemp = digitalRead(UP_btn);
      decrTemp = digitalRead(DOWN_btn);
      if (incrTemp == 1){
        mySender.send(rawDataUp, RAW_DATA_LEN, 36); //Pass the buffer,length, optionally frequency
      Serial.println(F("AC temperature increased"));
      delay(100);
      }
      else if (decrTemp == 1){
        mySender.send(rawDataDown, RAW_DATA_LEN, 36); //Pass the buffer,length, optionally frequency
      Serial.println(F("AC temperature decreased"));
      delay(100);
      }
    }

  if (btnOldState == 0 && btnNewState == 1) {

    if (acState == 0) {
      digitalWrite(13, HIGH);
      mySender.send(rawDataOn, RAW_DATA_LEN, 36); //Pass the buffer,length, optionally frequency
      mySender.send(rawDataOn2, RAW_DATA_LEN, 36);
      Serial.println(F("AC Switched On"));
      acState = 1;
          
    }
    else {
      digitalWrite(13, LOW);
      mySender.send(rawDataOff, RAW_DATA_LEN, 36); //Pass the buffer,length, optionally frequency
      Serial.println(F("AC Switched Off"));
      acState = 0;
    }

  }

  btnOldState = btnNewState;
  delay(100);

}
