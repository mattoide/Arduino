#include <Joystick.h>

#define EBRAKE A0

int force; 
int mappedForce;

Joystick_ Joystick;

void setup() {
  
  pinMode(EBRAKE, INPUT);
  Joystick.begin();           
  
  }

void loop() {

  force = analogRead(EBRAKE);



  if( force >= 507 && force <= 1023 ){
    
    mappedForce = map(force,507,1023,0,1023);
    Joystick.setThrottle(mappedForce);

  }
  
  
  delay(1);
  
}
