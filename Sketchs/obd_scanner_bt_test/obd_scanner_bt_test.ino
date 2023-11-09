#include <SoftwareSerial.h>
#include "ELMduino.h"
#include <LiquidCrystal.h>


#define bluetoothSerial_RX_PIN 2
#define bluetoothSerial_TX_PIN 3
#define PREVIOUS_SENSOR 10
#define NEXT_SENSOR 11
//#define HIGH 1
//#define LOW 0
#define DEBUG false
#define OBD_DEBUG false
#define PLOTTER false
#define BAUD_RATE 9600
#define OBD_BAUD_RATE 38400
#define TIMEDELAY 0
SoftwareSerial mySerial(bluetoothSerial_RX_PIN, bluetoothSerial_TX_PIN); // RX, TX


LiquidCrystal lcd(9, 8, 7, 6, 5, 4); //TODO: sistemare pin

ELM327 myELM327;

int sensors[] = {ENGINE_COOLANT_TEMP, ENGINE_OIL_TEMP, FUEL_PRESSURE};
int sensor_to_read = 0;

float rpm, engineCoolantTemp, engineOilTemp, fuelPressure, intakeAirTemp, vehicleSpeed = 0;


int calculatetime = 0;
int currentTime = 0;

//00:1D:A5:68:98:8B
//ELM - 001D,A5,68988B
//TEL - 684A,E9,D8125C
//CUFFIE - 0023,02,180A23


void setup() {

  pinMode(NEXT_SENSOR, INPUT);
  pinMode(PREVIOUS_SENSOR, INPUT);

  Serial.begin(BAUD_RATE);
  //mySerial.begin(OBD_BAUD_RATE);
  //lcd.begin(16, 2);

 // myELM327.begin(mySerial, OBD_DEBUG, 10000);

}

void loop() {
  
  if(calculatetime==1){
      currentTime = millis();
      calculatetime = 0;
  }
  
  switch (sensors[sensor_to_read]) {
    case ENGINE_COOLANT_TEMP:
      get_engine_coolant_temp();
     // Serial.print("Engine Coolant Temp: "); Serial.print(engineCoolantTemp); Serial.println(" C°");
      //write_lcd("Eng Coolant Tmp: ", String(engineCoolantTemp) + " C°");
    break;

    case ENGINE_OIL_TEMP:
      get_engine_oil_temp();
    //  Serial.print("Engine Oil Temp: "); Serial.print(engineOilTemp); Serial.println(" C°");
      //write_lcd("Engine Oil Temp: ", String(engineOilTemp) + " C°");

    break;


    case FUEL_PRESSURE:
      get_fuel_pressure();
    //  Serial.print("Fuel Pressure: "); Serial.print(fuelPressure); Serial.println(" Bar");
    break;

    default:
    sensor_to_read--;
    break;

  }
  
  if(digitalRead(NEXT_SENSOR) == HIGH && sensor_to_read < number_of_sensors()){
    if(millis()-currentTime > 1000){
        sensor_to_read++;
        calculatetime = 1;
        
    }

  }

  if(digitalRead(PREVIOUS_SENSOR) == HIGH && sensor_to_read > 0){
    if(millis()-currentTime > 1000){
        sensor_to_read--;
        calculatetime  = 1;
    }

  }

  if(DEBUG){
    Serial.print("sensor_to_read: "); Serial.println(sensor_to_read);
    Serial.print("NEXT_SENSOR: ");  Serial.println(digitalRead(NEXT_SENSOR));
    Serial.print("PREVIOUS_SENSOR: ");  Serial.println(digitalRead(PREVIOUS_SENSOR));
    Serial.print("Array Lenght: ");  Serial.println(sizeof(sensors)/sizeof(sensors[0]));
    Serial.print("Sensor[sensor_to_read]: ");  Serial.println(sensors[sensor_to_read]);
  }

    if(PLOTTER){

      get_engine_coolant_temp();
      Serial.print("Engine_Coolant_Temp(C°):"); Serial.println(engineCoolantTemp);

      get_engine_oil_temp();
      Serial.print("Engine_Oil_Temp(C°):"); Serial.println(engineOilTemp); 


      get_fuel_pressure();
      Serial.print("Fuel_Pressure(Bar):"); Serial.println(fuelPressure);

      get_inake_air_temp();
      Serial.print("Intake_Air_Temp(C°):"); Serial.println(intakeAirTemp);

      get_rpm();
      Serial.print("RPM:"); Serial.println(rpm);    

      get_vehicle_speed();
      Serial.print("Vehicle_Speed(km/h):"); Serial.println(vehicleSpeed);

  }

  delay(10);
}

int number_of_sensors(){
  return sizeof(sensors)/sizeof(sensors[0]);
}

float kpa_to_bar(int kpa){
  return kpa * 0.01;
}

void display_scroll(){
    for (int positionCounter = 0; positionCounter < 35; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(400);
  }
}

void write_lcd(String messageRow1, String messageRow2){
  /*
  lcd.clear();
  lcd.print(messageRow1);   
  lcd.setCursor(0, 1);
  lcd.print(messageRow2);
  */

  Serial.println(messageRow1 + " " + messageRow2 );

}
/*

void get_engine_coolant_temp(){
  engineCoolantTemp = random(100);
}

void get_rpm(){
  rpm = random(5000);
}
void get_fuel_pressure(){
  fuelPressure = kpa_to_bar(random(500));
}

void get_engine_oil_temp(){
  engineOilTemp = random(80);
}

void get_inake_air_temp(){
  intakeAirTemp = random(50);
}

void get_vehicle_speed(){
  vehicleSpeed = random(200);
}


*/
void get_rpm(){

  rpm = random(100);

   if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    write_lcd("RPM: ", String(rpm));
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    write_lcd("RPM:" ,"ERROR");
    myELM327.printError();
  }
}

void get_engine_coolant_temp(){

Serial.println("Aspetto 2 secpndi...");
delay(TIMEDELAY);
  engineCoolantTemp = random(100);

Serial.println("Fine attesa!");
  write_lcd("Eng Coolant Tmp:", String(engineCoolantTemp) + " C°");

 
}

void get_fuel_pressure(){

Serial.println("Aspetto 2 secpndi...");
delay(TIMEDELAY);
  fuelPressure = kpa_to_bar(random(500));

Serial.println("Fine attesa!");

    write_lcd("Fuel Pressure: ", String(fuelPressure) + "  Bar");

}

void get_engine_oil_temp(){

 
    
Serial.println("Aspetto 2 secpndi...");
delay(TIMEDELAY);
  engineOilTemp = random(100);

Serial.println("Fine attesa!");

write_lcd("Engine Oil Temp:" , String(engineOilTemp) + " C°");

}

void get_inake_air_temp(){

  intakeAirTemp = myELM327.intakeAirTemp();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    write_lcd("Intake Air Temp:" , String(intakeAirTemp) + " C°");
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    write_lcd("Intake Air Temp:", "ERROR");
    myELM327.printError();
  }
}

void get_vehicle_speed(){
  
  vehicleSpeed = myELM327.kph();

   if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    write_lcd("Vehicle Speed:" , String(vehicleSpeed) + "  km/h");

  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    write_lcd("Vehicle Speed:", "ERROR");
    myELM327.printError();
  }
}
