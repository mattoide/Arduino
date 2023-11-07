#include <SoftwareSerial.h>
#include "ELMduino.h"
#include <LiquidCrystal.h>


#define NEXT_SENSOR 12
#define PREVIOUS_SENSOR 11
#define HIGH 1
#define LOW 0
#define DEBUG false
#define PLOTTER true

SoftwareSerial mySerial(2, 3); // RX, TX

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //TODO: sistemare pin

ELM327 myELM327;

int sensors[] = {ENGINE_COOLANT_TEMP, ENGINE_OIL_TEMP, FUEL_PRESSURE,INTAKE_AIR_TEMP, ENGINE_RPM, VEHICLE_SPEED};
int sensor_to_read = 0;

float rpm, engineCoolantTemp, engineOilTemp, fuelPressure, intakeAirTemp, vehicleSpeed = 0;

void setup() {

  pinMode(NEXT_SENSOR, INPUT);
  pinMode(PREVIOUS_SENSOR, INPUT);

  Serial.begin(115200);
  mySerial.begin(115200);
  lcd.begin(16, 2);

  //myELM327.begin(mySerial, true, 2000);
}

void loop() {

  switch (sensors[sensor_to_read]) {
    case ENGINE_COOLANT_TEMP:
      get_engine_coolant_temp();
      Serial.print("Engine Coolant Temp: "); Serial.print(engineCoolantTemp); Serial.println(" C°");
     // write_lcd("Engine Coolant Temp: " + String(engineCoolantTemp) + " C°");
    break;

    case ENGINE_OIL_TEMP:
      get_engine_oil_temp();
      Serial.print("Engine Oil Temp: "); Serial.print(engineOilTemp); Serial.println(" C°");
    break;


    case FUEL_PRESSURE:
      get_fuel_pressure();
      Serial.print("Fuel Pressure: "); Serial.print(fuelPressure); Serial.println(" Bar");
    break;

    case INTAKE_AIR_TEMP:
      get_inake_air_temp();
      Serial.print("Intake Air Temp: "); Serial.print(intakeAirTemp); Serial.println(" C°");
    break;

    case ENGINE_RPM:
      get_rpm();
      Serial.print("RPM: "); Serial.println(rpm);    
    break;

    case VEHICLE_SPEED:
      get_vehicle_speed();
      Serial.print("Vehicle Speed: "); Serial.print(vehicleSpeed); Serial.println(" km/h");
    break;

    default:
    sensor_to_read--;
    break;

  }
  

  if(digitalRead(NEXT_SENSOR) == HIGH && sensor_to_read < number_of_sensors())
    sensor_to_read++;

  if(digitalRead(PREVIOUS_SENSOR) == HIGH && sensor_to_read > 0)
    sensor_to_read--;

  if(DEBUG){
    Serial.print("sensor_to_read: "); Serial.println(sensor_to_read);
    Serial.print("NEXT_SENSOR: ");  Serial.println(digitalRead(NEXT_SENSOR));
    Serial.print("PREVIOUS_SENSOR: ");  Serial.println(digitalRead(PREVIOUS_SENSOR));
    Serial.print("Array Lenght: ");  Serial.println(sizeof(sensors)/sizeof(sensors[0]));
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

  delay(500);
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

void write_lcd(String message){
      lcd.clear();
      lcd.setCursor(16, 0);
      lcd.print(F("PoRcO DiO"));   

}


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


/*
void get_rpm(){

  rpm = myELM327.rpm();

   if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    write_lcd("RPM: " + String(rpm));
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    write_lcd("RPM: ERROR");
    myELM327.printError();
  }
}

void get_engine_coolant_temp(){

  engineCoolantTemp = myELM327.engineCoolantTemp();

   if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    write_lcd("Engine Coolant Temp: " + String(engineCoolantTemp) + " C°");
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    write_lcd("Engine Coolant Temp: ERROR");
    myELM327.printError();
  }
}

void get_fuel_pressure(){

  fuelPressure = kpa_to_bar(myELM327.fuelPressure());

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    write_lcd("Fuel Pressure: " + String(fuelPressure) + "  Bar");
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    write_lcd("Fuel Pressure: ERROR");
    myELM327.printError();
  }
}

void get_engine_oil_temp(){

  engineOilTemp = myELM327.oilTemp();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
   if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    write_lcd("Engine Oil Temp: " + String(engineOilTemp) + " C°");
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    write_lcd("Engine Oil Temp: ERROR");
    myELM327.printError();
  }
}

void get_inake_air_temp(){

  intakeAirTemp = myELM327.intakeAirTemp();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    write_lcd("Intake Air Temp: " + String(intakeAirTemp) + " C°");
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    write_lcd("Intake Air Temp: ERROR");
    myELM327.printError();
  }
}

void get_vehicle_speed(){
  
  vehicleSpeed = myELM327.kph();

   if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    write_lcd("Vehicle Speed: " + String(vehicleSpeed) + "  km/h");

  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    write_lcd("Vehicle Speed: ERROR");
    myELM327.printError();
  }
}
*/
