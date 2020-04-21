#include "ibus.h"

#define EBRAKE A0
#define UPSHIFT 12
#define DOWNSHIFT 9

int force; 
int mappedForce;
int upshift;
int downshift;

IBus ibus(3);

void setup() {
  
  pinMode(EBRAKE, INPUT);
  pinMode(UPSHIFT, INPUT);
  pinMode(DOWNSHIFT, INPUT);
  
  Serial.begin(115200);           
  
  }

void loop() {


  force = analogRead(EBRAKE);
  upshift = digitalRead(UPSHIFT);
  downshift = digitalRead(DOWNSHIFT);
  
  ibus.begin();

  if( force >= 507 && force <= 1023 ){
      
    mappedForce = map(force,507,1023,0,255);
    ibus.write(mappedForce);
    
  } else {
    ibus.write(0);
  }
  
  ibus.write(upshift);
  ibus.write(downshift);
  
  ibus.end();

  delay(1);
  
}
