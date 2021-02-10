#include <Keyboard.h>

#define EBRAKE A0

int force; 
int mappedForce;


void setup() {
  
  pinMode(EBRAKE, INPUT);
  Keyboard.begin();           
  
  }

void loop() {

  force = analogRead(EBRAKE);



  if( force >= 507 && force <= 1023 ){

    Keyboard.press('L');

  }else{
        Keyboard.release('L');

  }
  
  delay(1);
  
}
