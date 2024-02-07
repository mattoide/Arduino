#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>

#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>

#define SELECT_TRAIN_PIN A1
#define PLAY_BUTTON_PIN 2


int selectTrainValue = 0;
int value = 0;
int previous_value = -1;

char* audio;

TMRpcm tmrpcm;
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(PLAY_BUTTON_PIN, INPUT_PULLUP);
  tmrpcm.speakerPin = 9;

  if (!SD.begin()) {
    Serial.println("SD init failed!");
    while (true)
      ;
  }
  lcd.clear();

  delay(10);
}

void loop() {

  selectTrainValue = analogRead(SELECT_TRAIN_PIN);

  detectLoco(selectTrainValue);
  isPLaying();

  if(digitalRead(PLAY_BUTTON_PIN) == LOW)
    playSound();

  delay(100);
}

void isPLaying() {
  if (!tmrpcm.isPlaying()) {
    digitalWrite(9, LOW);
  }
}


void detectLoco(int value) {

  value = map(value, 0, 1022, 0, 3);

  if (value != previous_value) {
    previous_value = value;

    switch (value) {
      case 0:
        setSoundFor("trenom~1.wav", "Merci");
        break;
      case 1:
        setSoundFor("danapoli.wav", "Passeggeri");
        break;
      case 2:
        setSoundFor("pernor~1.wav", "Storico");
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        break;
      case 8:
        break;
      case 9:
        break;
    }
  }
}

void setSoundFor(char* p_audio, String loco) {
  audio = p_audio;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Train selected");
  lcd.setCursor(0, 1);
  lcd.print(loco);
}

void playSound() {
  digitalWrite(9, HIGH);
  delay(500);
  tmrpcm.setVolume(6);
  tmrpcm.play(audio);
}
