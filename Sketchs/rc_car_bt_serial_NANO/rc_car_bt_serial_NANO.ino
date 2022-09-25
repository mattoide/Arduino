#include <SoftwareSerial.h>

#define DEBUG false

#define bluetoothSerial_TX_PIN 11
#define bluetoothSerial_RX_PIN 12

/*
  l29
  #define MOTORE1_1 6
  #define MOTORE1_2 5
*/


/*
  drv
*/
#define MOTORE1_1 5
#define MOTORE1_2 6


#define MOTORE2_1 9
#define MOTORE2_2 10

#define STATE 3

#define MOTOR_STOP 0
#define MAX 255

#define MAX_BYTES 5
#define STOP 88
#define STOPT 89
#define FORWARD 87
#define BACKWARD 83
#define LEFT 65
#define RIGHT 68
#define L_FORWARD 81
#define R_FORWARD 69
#define L_BACKWARD 90
#define R_BACKWARD 67
#define RIGHT 68
#define SPEED 70

#define STEER_SPEED 255

char bt_bytes[MAX_BYTES];
String toSpeed;

int motor_speed = 200;
int motor_L_speed = 0;
int motor_R_speed = 0;

SoftwareSerial bluetoothSerial = SoftwareSerial(bluetoothSerial_RX_PIN, bluetoothSerial_TX_PIN);

void setup() {

  Serial.begin(9600);
  bluetoothSerial.begin(9600);
  bluetoothSerial.setTimeout(5);


  pinMode(MOTORE1_1, OUTPUT);
  pinMode(MOTORE1_2, OUTPUT);

  pinMode(MOTORE2_1, OUTPUT);
  pinMode(MOTORE2_2, OUTPUT);

  pinMode(bluetoothSerial_RX_PIN, INPUT);
  pinMode(bluetoothSerial_TX_PIN, OUTPUT);

  pinMode(STATE, INPUT);


  analogWrite(MOTORE1_1, MOTOR_STOP);
  analogWrite(MOTORE1_2, MOTOR_STOP);
  analogWrite(MOTORE2_1, MOTOR_STOP);
  analogWrite(MOTORE2_2, MOTOR_STOP);

  Serial.println("Pronto!");
  bluetoothSerial.println("Pronto!");
}

void loop() {


  if (digitalRead(STATE) == 1) {

    if (bluetoothSerial.available() > 0) {

      memset(bt_bytes, 0, sizeof bt_bytes);
      bluetoothSerial.readBytesUntil('\n', bt_bytes, MAX_BYTES);

#if DEBUG
      Serial.println(bt_bytes);
#endif


      switch (bt_bytes[0]) {

        case FORWARD:
          forward();
          break;

        case BACKWARD:
          backward();
          break;

        case LEFT:
          left();
          break;

        case RIGHT:
          right();
          break;

        case STOP:
          stop_motor();
          break;



        // case L_FORWARD:

        //   forwardL();
        //   break;

        // case R_FORWARD:
        //   forwardR();
        //   break;

        // case R_BACKWARD:
        //   backwardR();
        //   break;

        // case L_BACKWARD:
        //   backwardL();
        //   break;


        case SPEED:

          motor_speed = (String(bt_bytes[1]) + String(bt_bytes[2]) + String(bt_bytes[3])).toInt();

          if (motor_speed > 255)
            motor_speed = 255;
          break;


        case STOPT:
          stop_motor_t();
          break;

        default:
          stop_motor();
          break;
      }
    }
  } else {
    stop_motor();
  }

  delay(1);
}

void forwardR() {
  right();
  forward();
}

void forwardL() {
  left();
  forward();
}

void backwardR() {
  right();
  backward();
}

void backwardL() {
  left();
  backward();
}

void stop_motor() {
  analogWrite(MOTORE1_1, MOTOR_STOP);
  analogWrite(MOTORE1_2, MOTOR_STOP);

  analogWrite(MOTORE2_1, MOTOR_STOP);
  analogWrite(MOTORE2_2, MOTOR_STOP);
}


void stop_motor_t() {
  analogWrite(MOTORE2_1, MOTOR_STOP);
  analogWrite(MOTORE2_2, MOTOR_STOP);
}

void forward() {
  analogWrite(MOTORE1_1, MOTOR_STOP);
  analogWrite(MOTORE1_2, motor_speed);
}


void backward() {
  analogWrite(MOTORE1_1, motor_speed);
  analogWrite(MOTORE1_2, MOTOR_STOP);
}


void left() {
  analogWrite(MOTORE2_1, 0);
  analogWrite(MOTORE2_2, STEER_SPEED);
}

void right() {
  analogWrite(MOTORE2_1, STEER_SPEED);
  analogWrite(MOTORE2_2, 0);
}
