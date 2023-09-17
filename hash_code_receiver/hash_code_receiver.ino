/* hashDecode.ino Example sketch for IRLib2
* Illustrate the use of the hash decoder.
*/
//First will create a decoder that handles only NEC, Sony and
// the hash decoder. If it is not NEC or Sony that it will
// return a 32 bit hash.
#include <IRLibDecodeBase.h>
#include <IRLib_P01_NEC.h> 
#include <IRLib_P02_Sony.h> 
#include <IRLib_HashRaw.h> //Must be last protocol
#include <IRLibCombo.h> // After all protocols, include this
IRdecode myDecoder;
#include <IRLibRecv.h>
IRrecv myReceiver(2); //create a receiver on pin number 2
void setup() {
 Serial.begin(9600);
 delay(2000); while (!Serial); //delay for Leonardo
 myReceiver.enableIRIn(); // Start the receiver
 Serial.println(F("Ready to receive IR signals"));
}
void loop() {
 if(myReceiver.getResults()) {
 myDecoder.decode();
// if(myDecoder.protocolNum==UNKNOWN) {
// Serial.print(F("Unknown protocol. Hash value is: 0x"));
// Serial.println(myDecoder.value,HEX);
// } else {
 myDecoder.dumpResults(false);
// };
 myReceiver.enableIRIn();
 }
}
