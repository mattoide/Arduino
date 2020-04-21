
#include "ibus.h"

#define EBRAKE A0

int force; 
int mappedForce;

IBus ibus(1);

void setup() {
  
  pinMode(EBRAKE, INPUT);
  Serial.begin(115200);           
  
  }

void loop() {

  force = analogRead(EBRAKE);
  
  ibus.begin();

  if( force >= 507 && force <= 1023 ){
    
    mappedForce = map(force,507,1023,0,255);
    ibus.write(mappedForce);
  }
  
  ibus.end();

  
  delay(1);
  
}
