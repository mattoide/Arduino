#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>

#include "AudioMessages.h"
#include <SPI.h>
#include <SD.h>


#define OUT 3
#define S0 5
#define S1 6
#define S2 8
#define S3 7



#define WHITE_THRESHOLD 40
#define BLACK_THRESHOLD 250
#define MISURATIONS_NUMBER 5

int passeggeri_range_values[] = {59, 62};
int merci_range_values[] = {345, 355};
int storico_range_values[] = {52, 55};


int red, green, blue, clear;

TMRpcm tmrpcm; 


void setup() {
  Serial.begin(9600);

    tmrpcm.speakerPin = 9;

    if (!SD.begin()) {
    Serial.println("SD init failed!");
    while(true);
  }

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Power down
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);

  delay(10);
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
  Serial.println(clear);*/
  /* String color = getColor();
 Serial.println(color);*/
 
 int sum = red + green + blue;
Serial.println(sum);
 detectLoco(sum);
isPLaying();

delay(100);
}

void isPLaying(){
  if(!tmrpcm.isPlaying()){
    digitalWrite(9, LOW);
  }
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
    if (clear < WHITE_THRESHOLD) {
      return "Bianco";
    } else if (clear > BLACK_THRESHOLD) {
      return "Nero";
    }
    return "Grigio";
  } else if (R_G < 0.8 && G_B > 1.1 && G_B < 1.4 && B_R > 1.2) {
    return "Rosso";
  } else if (R_G > 0.9 && G_B < 1.2 && B_R < 1.45) {
    return "Verde";
  } else if (R_G > 0.9 && G_B > 1.2 && B_R < 0.8) {
    return "Blu";
  } else if (R_G > 0.7 && G_B < 1.2 && B_R > 1) {
    return "Giallo";
  } else if (R_G < 0.8 && G_B < 1.3 && B_R > 1) {
    return "Arancione";
  } else if (G_B > 1.35 && B_R > 1.1) {
    return "Rosa";
  } else if (R_G < 0.9 && B_R < 1.2) {
    return "Viola";
  }

  return "";
}

void detectLoco(int sum) {

  if (sum > merci_range_values[0] && sum < merci_range_values[1]){
    Serial.println("Merci");
    playSoundFor("trenom~1.wav");
  }else if(sum > passeggeri_range_values[0] && sum < passeggeri_range_values[1]){
    Serial.println("Passeggeri");
    playSoundFor("danapoli.wav");
  } else if(sum > storico_range_values[0] && sum < storico_range_values[1]){
    Serial.println("Storico");
      playSoundFor("pernor~1.wav");
  }
}

void playSoundFor(char* loco){
  digitalWrite(9, HIGH);

 /* tmrpcm.setVolume(6);
  tmrpcm.play(loco);
  delay(3000);*/
}