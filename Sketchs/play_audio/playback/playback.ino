#include <PCM.h>
//#include "audiomessages.h"

void setup()
{
  Serial.begin(9600);
 // pinMode(11, OUTPUT);
  // startPlayback(sample, sizeof(sample));
}

void loop()
{
   unsigned char s[30000] = {123,321,123,31,232,123};
  while(Serial.available()>0){
   Serial.readBytes(s, sizeof(s));
  }
  Serial.write(s);
   // startPlayback(s, sizeof(s));
    delay(2000); 
strcpy(s, "");
Serial.write(s);
       //stopPlayback();

//digitalWrite(11, HIGH);
//delay(1000);
}


