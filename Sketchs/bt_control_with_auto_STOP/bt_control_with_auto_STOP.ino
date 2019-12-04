#include <SoftwareSerial.h>

#define bluetoothSerial_TX_PIN 3
#define bluetoothSerial_RX_PIN 2

#define MOTORE1_1 10
#define MOTORE1_2 11
#define MOTORE2_1 5
#define MOTORE2_2 6
#define STOP 0
#define MAX 255

String incomingData;
int motor_speed = 0;
int motor_L_speed = 0;
int motor_R_speed = 0;

int ms = 0;

SoftwareSerial bluetoothSerial =  SoftwareSerial(bluetoothSerial_RX_PIN, bluetoothSerial_TX_PIN);

void setup() {

  Serial.begin(9600);
  bluetoothSerial.begin(9600);


  pinMode(MOTORE1_1, OUTPUT);
  pinMode(MOTORE1_2, OUTPUT);

  pinMode(MOTORE2_1, OUTPUT);
  pinMode(MOTORE2_2, OUTPUT);

  pinMode(bluetoothSerial_RX_PIN, INPUT);
  pinMode(bluetoothSerial_TX_PIN, OUTPUT);


  analogWrite(MOTORE1_1, STOP);
  analogWrite(MOTORE1_2, STOP);
  analogWrite(MOTORE2_1, STOP);
  analogWrite(MOTORE2_2, STOP);

  Serial.println("Pronto!");
  bluetoothSerial.println("Pronto!");
bluetoothSerial.setTimeout(15);
}

int incomingByte = 0;

void loop() {

if(ms == 10){
   Serial.println("stop_motor");
  stop_motor();
  ms=0;
}

if(bluetoothSerial.available() > 0){
ms = 0;
   incomingData = bluetoothSerial.readStringUntil('\n');
   incomingData.trim();
    Serial.println(incomingData);

     if (incomingData == "W") {
      forward();
    } else if (incomingData == "S") {
      backward();
    } else if (incomingData == "A") {
      left();
    } else if (incomingData == "D") {
      right();
    } else if (incomingData == "X") {
      stop_motor();
    }  else if (incomingData.substring(0, 5) == "SPEED") {
      motor_speed = incomingData.substring(5, incomingData.length()).toInt();
      if (motor_speed > 255)
        motor_speed = 255;
    } else if (incomingData.substring(0, 1) == "L") {
      motor_L_speed = incomingData.substring(1, incomingData.length()).toInt();
      forwardL();
    } else if (incomingData.substring(0, 1) == "R") {
      motor_R_speed = incomingData.substring(1, incomingData.length()).toInt();
      forwardR();
    } else if(digitalRead(2) == 0){
            stop_motor();
               Serial.println("stop_motor segnaleeeee");
    }

} else{
 ms++;
}
delay(5);

  /*while (bluetoothSerial.available() > 0) {
    
    incomingData = bluetoothSerial.readStringUntil('\n');
    incomingData.trim();

    if (incomingData == "W") {
      forward();
    } else if (incomingData == "S") {
      backward();
    } else if (incomingData == "A") {
      left();
    } else if (incomingData == "D") {
      right();
    } else if (incomingData == "X") {
      stop_motor();
    }  else if (incomingData.substring(0, 5) == "SPEED") {
      motor_speed = incomingData.substring(5, incomingData.length()).toInt();
      if (motor_speed > 255)
        motor_speed = 255;
    } else if (incomingData.substring(0, 1) == "L") {
      motor_L_speed = incomingData.substring(1, incomingData.length()).toInt();
      forwardL();
    } else if (incomingData.substring(0, 1) == "R") {
      motor_R_speed = incomingData.substring(1, incomingData.length()).toInt();
      forwardR();
    }
  }*/

}

void forwardR() {
  analogWrite(MOTORE1_1, STOP);
  analogWrite(MOTORE1_2, motor_R_speed);
}

void forwardL() {
  analogWrite(MOTORE2_1, motor_L_speed);
  analogWrite(MOTORE2_2, STOP);
}

void stop_motor() {
  analogWrite(MOTORE1_1, STOP);
  analogWrite(MOTORE1_2, STOP);

  analogWrite(MOTORE2_1, STOP);
  analogWrite(MOTORE2_2, STOP);
}

void forward() {
  analogWrite(MOTORE1_1, STOP);
  analogWrite(MOTORE1_2, motor_speed);

  analogWrite(MOTORE2_1, motor_speed);
  analogWrite(MOTORE2_2, STOP);
}


void backward() {
  analogWrite(MOTORE1_1, motor_speed);
  analogWrite(MOTORE1_2, STOP);

  analogWrite(MOTORE2_1, STOP);
  analogWrite(MOTORE2_2, motor_speed);
}


void right() {
  analogWrite(MOTORE1_1, STOP);
  analogWrite(MOTORE1_2, motor_speed);

  analogWrite(MOTORE2_1, STOP);
  analogWrite(MOTORE2_2, motor_speed);
}

void left() {
  analogWrite(MOTORE1_1, motor_speed);
  analogWrite(MOTORE1_2, STOP);

  analogWrite(MOTORE2_1, motor_speed);
  analogWrite(MOTORE2_2, STOP);
}
