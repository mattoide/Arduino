#include <SoftwareSerial.h>

#define bluetoothSerial_TX_PIN 3
#define bluetoothSerial_RX_PIN 2

SoftwareSerial bluetoothSerial =  SoftwareSerial(bluetoothSerial_RX_PIN, bluetoothSerial_TX_PIN);


void setup() {
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  Serial.begin(9600);
  bluetoothSerial.begin(38400);
  
  Serial.println("Enter AT commands:");

}

void loop() {

  if (bluetoothSerial.available() > 0) {
    Serial.println(bluetoothSerial.readString());

  }

  if (Serial.available() > 0) {
    bluetoothSerial.write(Serial.read());
  }
}
