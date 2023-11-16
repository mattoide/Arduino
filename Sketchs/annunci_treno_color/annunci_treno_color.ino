#include <PCM.h>
#include "AudioMessages.h"


#define S3 9
#define S2 8
#define OUT 7
#define S0 3
#define S1 2

#define WHITE_THRESHOLD 40
#define BLACK_THRESHOLD 250
#define MISURATIONS_NUMBER 5


int red, green, blue, clear;





/**
* Source code:
* https://www.italiantechproject.it/sensori-con-arduino/colore-tcs3200
*/

void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Power down
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
}

void loop() {
  updateRGB();
 /* Serial.print("Rosso: ");
  Serial.print(red);
  Serial.print(" - ");
  Serial.print("Verde: ");
  Serial.print(green);
  Serial.print(" - ");
  Serial.print("Blue: ");
  Serial.print(blue);
  Serial.print(" - ");
  Serial.print("Luce: ");
  Serial.println(clear);
   String color = getColor();
 Serial.println(color);*/
  int sum = red + green + blue;
Serial.println(sum);
//detectLoco(sum);

  delay(500);
}

void updateRGB() {
  // Frequency 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  delay(10);

  red = 0;
  green = 0;
  blue = 0;
  clear = 0;

  for (int i = 0; i < 5; i++) {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red += pulseIn(OUT, LOW);
    delay(1);

    digitalWrite(S2, HIGH);
    clear += pulseIn(OUT, LOW);
    delay(1);

    digitalWrite(S3, HIGH);
    green += pulseIn(OUT, LOW);
    delay(1);

    digitalWrite(S2, LOW);
    blue += pulseIn(OUT, LOW);
    delay(1);
  }

  red /= 5;
  green /= 5;
  blue /= 5;
  clear /= 5;

  // Power down
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
}

String getColor() {
  float R_G = (float)red / green;
  float G_B = (float)green / blue;
  float B_R = (float)blue / red;

  if (R_G > 0.75 && R_G < 1.1
      && G_B > 0.9 && G_B < 1.4
      && B_R > 0.75 && B_R < 1.5) {
    if (clear <= WHITE_THRESHOLD) {
      return "Bianco";
    } else if (clear >= BLACK_THRESHOLD) {
      return "Nero";
    }
    return "Grigio";
  } else if (R_G <= 0.8 && G_B >= 1.1 && G_B <= 1.4 && B_R >= 1.2) {
    return "Rosso";
  } else if (R_G > 0.9 && G_B < 1.2 && B_R < 1.45) {
    return "Verde";
  } else if (R_G >= 0.9 && G_B >= 1.2 && B_R < 0.8) {
    return "Blu";
  } else if (R_G >= 0.7 && G_B < 1.2 && B_R >= 1) {
    return "Giallo";
  } else if (R_G < 0.8 && G_B < 1.3 && B_R >= 1) {
    return "Arancione";
  } else if (G_B > 1.35 && B_R >= 1.1) {
    return "Rosa";
  } else if (R_G < 0.9 && B_R < 1.2) {
    return "Viola";
  }

  return "";
}

void detectLoco(int sum) {

  if (sum > 95 && sum < 105){
    Serial.println("Merci");
  }else if(sum> 75 && sum < 95){
    Serial.println("Passeggeri");
    startPlayback(sample, sizeof(sample));

  } else if(sum> 40 && sum < 60){
    Serial.println("Storico");
  }
}



/*
void setup() {
  Serial.begin(9600);

  pinMode(S3, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(OUT, INPUT);

  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
}

void loop() {

  updateRgb();

  Serial.print("Rosso: ");
  Serial.print(red);
  Serial.print(" - ");
  Serial.print("Verde: ");
  Serial.print(green);
  Serial.print(" - ");
  Serial.print("Blue: ");
  Serial.print(blue);
  Serial.print(" - ");
  Serial.print("Luce: ");
  Serial.println(clear);




  String color = getColor();
  Serial.println(color);

  delay(100);
  // 40 47 43 47 white  
  // 210 240 210 240 black
}


void updateRgb() {
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  delay(10);

  red = 0;
  green = 0;
  blue = 0;
  clear = 0;


  for (int i = 0; i < MISURATIONS_NUMBER; i++) {

    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);

    red += pulseIn(OUT, LOW);
    delay(1);


    digitalWrite(S2, HIGH);

    clear += pulseIn(OUT, LOW);
    delay(1);



    digitalWrite(S3, HIGH);

    green += pulseIn(OUT, LOW);
    delay(1);


    digitalWrite(S2, LOW);

    blue += pulseIn(OUT, LOW);
    delay(1);
  }

  red /= MISURATIONS_NUMBER;
  green /= MISURATIONS_NUMBER;
  blue /= MISURATIONS_NUMBER;
  clear /= MISURATIONS_NUMBER;

  red = map(red, 30, 255, 255,0);
  green = map(green,30, 255, 255,0);
  blue = map(blue, 30, 255, 255,0);

  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
}


String getColor() {
  float R_G = (float)red / green;
  float G_B = (float)green / blue;
  float B_R = (float)blue / red;

  Serial.print("R_G: ");
  Serial.print(R_G);
  Serial.print(" __ ");

  Serial.print("G_B: ");
  Serial.print(G_B);

  Serial.print(" __ ");

  Serial.print("B_R:");
  Serial.println(B_R);
  

if (R_G > 0.75 && R_G < 1.1
      && G_B > 0.9 && G_B < 1.4
      && B_R > 0.75 && B_R < 1.5) {
    if (clear <= WHITE_THRESHOLD) {
      return "Bianco";
    } else if (clear >= BLACK_THRESHOLD) {
      return "Nero";
    }
    return "Grigio";
  } else if (R_G <= 0.8 && G_B >= 1.1 && G_B <= 1.4 && B_R >= 1.2) {
    return "Rosso";
  } else if (R_G > 0.9 && G_B < 1.2 && B_R < 1.45) {
    return "Verde";
  } else if (R_G >= 0.9 && G_B >= 1.2 && B_R < 0.8) {
    return "Blu";
  } else if (R_G >= 0.7 && G_B < 1.2 && B_R >= 1) {
    return "Giallo";
  } else if (R_G < 0.8 && G_B < 1.3 && B_R >= 1) {
    return "Arancione";
  } else if (G_B > 1.35 && B_R >= 1.1) {
    return "Rosa";
  } else if (R_G < 0.9 && B_R < 1.2) {
    return "Viola";
  }
 
  return "";
}*/