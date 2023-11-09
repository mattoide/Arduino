#include <SoftwareSerial.h>
#include "ELMduino.h"


SoftwareSerial mySerial(2, 3); // RX, TX


ELM327 myELM327;


float rpm = 0;


void setup()
{
/*#if LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif*/

  Serial.begin(9600);
  mySerial.begin(115200);

  Serial.println("Attempting to connect to ELM327...");

  if (!myELM327.begin(mySerial, false, 1000))
  {
    Serial.println("Couldn't connect to OBD scanner");
    while (1);
  }

  Serial.println("Connected to ELM327");
}


void loop()
{
  float tempRPM = myELM327.fuelRailGuagePressure();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    rpm = tempRPM;
    Serial.print("Fuel pressure: "); Serial.println(rpm);
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    myELM327.printError();


  delay(1);
}
