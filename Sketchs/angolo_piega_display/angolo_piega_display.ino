/*
 
##########    ##########        
#  LCD   #    #ARDUINO #
##########    ##########
  
  + = 5V

  - 16  ->  GND
  - 15  ->  +
  - 14  ->  2
  - 13  ->  3
  - 12  ->  4
  - 11  ->  5
  - 10  ->  N/A
  - 09  ->  N/A
  - 08  ->  N/A
  - 07  ->  N/A
  - 06  ->  11
  - 05  ->  GND
  - 04  ->  12
  - 03  ->  GND
  - 02  ->  +
  - 01  ->  GND


##############    ##########        
#Aukru GY-521#    #ARDUINO #
#  MPU-6050  #    ##########
##############    ##########

  + = 3.3V

  - VCC ->  +
  - GND ->  GND
  - SCL ->  A5
  - SDA ->  A4
  

RED-LED PIN 7
GREEN-LED PIN 8
 
 */
 
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define RESET_BUTTON 13
#define KO_LED 7
#define OK_LED 8

MPU6050 mpu6050(Wire);

long timer = 0;
float x_ang = 0;
float x_ang_offset = 0;
int val_reset = 0;
 
void setup() {
  //pinMode(RESET_BUTTON, INPUT);
  pinMode(KO_LED, OUTPUT);
  pinMode(OK_LED, OUTPUT);

  digitalWrite(KO_LED, HIGH);
  digitalWrite(OK_LED, LOW);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
   lcd.print("Calibro! ");
  lcd.print("Tenere");
  lcd.setCursor(0, 1);
  lcd.print("la moto ferma");

  Serial.begin(9600);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  x_ang_offset = mpu6050.getAngleX();

  Serial.println(""); 
  Serial.println("OFFSET "); Serial.print(x_ang_offset);
  lcd.begin(16, 2);
    
  digitalWrite(KO_LED, LOW);
  digitalWrite(OK_LED, HIGH);

}

void loop() {
  val_reset = digitalRead(RESET_BUTTON);
  //Serial.println(val_reset);

  mpu6050.update();

  /*if(millis() - timer > 1000){
    
    Serial.println("=======================================================");
    Serial.print("temp : ");Serial.println(mpu6050.getTemp());
    Serial.print("accX : ");Serial.print(mpu6050.getAccX());
    Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
    Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
  
    Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
    Serial.print("\tgyroY : ");Serial.print(mpu6050.getGyroY());
    Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());
  
    Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
    Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
  
    Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
    Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
    Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());
    
    Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
    Serial.print("\tangleY : ");Serial.print(mpu6050.getAngleY());
    Serial.print("\tangleZ : ");Serial.println(mpu6050.getAngleZ());
    Serial.println("=======================================================\n");
    timer = millis();
    
  }*/
      lcd.setCursor(0, 0);

  if(millis() - timer > 1000){

    x_ang = mpu6050.getAngleX();

  /*
   *per calibrare in ogni posizione 
   */
   /* if(x_ang_offset < 0){
            x_ang = mpu6050.getAngleX() + x_ang_offset;
    } else {
            x_ang = mpu6050.getAngleX() - x_ang_offset;
    }*/
              lcd.print("Piega ");

      lcd.setCursor(0, 1);

        if(x_ang < 0) {
          x_ang = x_ang * -1;
              lcd.print("Sx: ");
        } else {
              lcd.print("Dx: ");
        }

    lcd.print(x_ang);
    lcd.print(" Gradi ");

    Serial.println("\nGradi: ");Serial.print(x_ang);

    timer = millis();

    

    }

}
