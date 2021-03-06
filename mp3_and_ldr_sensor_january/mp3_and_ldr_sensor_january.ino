#include "DFRobotDFPlayerMini.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "runningaverage.h"


#define LDR_READ_EVERY_MS 200

RunningAverage avgL( (1000/LDR_READ_EVERY_MS)*10 );
RunningAverage avgR( (1000/LDR_READ_EVERY_MS)*10 );

int leftEye, rightEye;

SoftwareSerial mySoftwareSerial(11, 10); // RX, TX
//SoftwareSerial mySoftwareSerial(6, 10); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value); 

int LDR_Left = 6;
int LDR_Right = 1;


void setup() { 
   mySoftwareSerial.begin(9600);
 Serial.begin(115200);

 Serial.println();
 Serial.println(F("DFRobot DFPlayer Mini Demo"));
 Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
 
 if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
   Serial.println(F("Unable to begin:"));
   Serial.println(F("1.Please recheck the connection!"));
   Serial.println(F("2.Please insert the SD card!"));
   while(true);
 }
 Serial.println(F("DFPlayer Mini online."));
}
 void debug_graph() {
  Serial.print(leftEye);
  Serial.print(",");
  Serial.print(rightEye);
  Serial.print(",");
  Serial.print( avgL.getAverage() );
  Serial.print(",");
  Serial.println( avgR.getAverage() );
}


void loop() {

  rightEye = readEye(LDR_Right); 
  avgR.addValue( rightEye );
  
  leftEye = readEye(LDR_Left);
  avgL.addValue( leftEye );
  
  debug_graph();
  
// if reading from lef eye and right eye are low then turn the sound on. //need the percetage
if ((rightEye>100) && (leftEye>100)) {
    myDFPlayer.volume(30);  //Set volume value. From 0 to 30
    myDFPlayer.play(1);  //Play the first mp3
}

//if the reading is high (light shines) then turn the sound off
else 
  myDFPlayer.stop();
}

 double readEye(int eye) { //def. of function 
 double reading = .0;
  // read 4 times to get a stable reading
  for(int i = 0; i < 4; i++) {
    reading += analogRead(eye);
    delay(5);
  }

  return (reading / 4.0); 
}


