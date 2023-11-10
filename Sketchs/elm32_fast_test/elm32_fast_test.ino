#include <SoftwareSerial.h>
#include "ELMduino.h"

#define bluetoothSerial_RX_PIN 2
#define bluetoothSerial_TX_PIN 3

#define ODB_TIMEOUT 1000
#define OBD_DEBUG false

#define DELAY_TIME 1000

SoftwareSerial mySerial(bluetoothSerial_RX_PIN, bluetoothSerial_TX_PIN); // RX, TX


ELM327 myELM327;


float value = 0;


void setup()
{

  Serial.begin(9600);
  mySerial.begin(115200);

  Serial.println("Attempting to connect to ELM327...");

  if (!myELM327.begin(mySerial, OBD_DEBUG, ODB_TIMEOUT))
  {
    Serial.println("Couldn't connect to OBD scanner");
    while (1);
  }

  Serial.println("Connected to ELM327");
}


void loop()
{
  uint32_t temp_value = myELM327.supportedPIDs_1_20();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    value = temp_value;
    Serial.print("supportedPIDs_1_20: "); Serial.println(temp_value);
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    myELM327.printError();


  delay(DELAY_TIME);
}
