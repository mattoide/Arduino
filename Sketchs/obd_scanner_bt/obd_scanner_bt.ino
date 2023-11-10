#include <SoftwareSerial.h>
#include "ELMduino.h"
#include <LiquidCrystal.h>


#define bluetoothSerial_RX_PIN 2
#define bluetoothSerial_TX_PIN 3

#define PREVIOUS_SENSOR 10
#define NEXT_SENSOR 11

#define DELAY_TIME 1

#define DEBUG false
#define OBD_DEBUG false
#define DEBUG_ODB_ERROR_MESSAGES false
#define PLOTTER false

#define OBD_TIMEOUT 1000
#define BAUD_RATE 9600
#define OBD_BAUD_RATE 115200

SoftwareSerial mySerial(bluetoothSerial_RX_PIN, bluetoothSerial_TX_PIN); // RX, TX


LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

ELM327 myELM327;

int sensors[] = {ENGINE_COOLANT_TEMP, ENGINE_OIL_TEMP, FUEL_RAIL_GUAGE_PRESSURE,INTAKE_AIR_TEMP, ENGINE_RPM, VEHICLE_SPEED};
int sensor_to_read = 0;

float rpm, engineCoolantTemp, engineOilTemp, fuelPressure, intakeAirTemp, vehicleSpeed = 0;

int prev_sensor_btn_last_state = LOW;
int next_sensor_btn_last_state = LOW;

int calculatetime = 0;
int currentTime = 0;
bool calc_finished = true;

//00:1D:A5:68:98:8B

//ELM - 001D,A5,68988B
//TEL - 684A,E9,D8125C
//CUFFIE - 0023,02,180A23
//PC - 0C96,E6,B56CDC


void setup() {

  pinMode(NEXT_SENSOR, INPUT);
  pinMode(PREVIOUS_SENSOR, INPUT);

  Serial.begin(BAUD_RATE);
  mySerial.begin(OBD_BAUD_RATE);
  lcd.begin(16, 2);

  myELM327.begin(mySerial, OBD_DEBUG, OBD_TIMEOUT);

}

void loop() {

   

  switch (sensors[sensor_to_read]) {
    case ENGINE_COOLANT_TEMP:
      get_engine_coolant_temp();
     // Serial.print("Engine Coolant Temp: "); Serial.print(engineCoolantTemp); Serial.println(" C°");
    break;

    case ENGINE_OIL_TEMP:
      get_engine_oil_temp();
    //  Serial.print("Engine Oil Temp: "); Serial.print(engineOilTemp); Serial.println(" C°");
    break;


    case FUEL_RAIL_GUAGE_PRESSURE:
      get_fuel_rail_gauge_pressure();
    //  Serial.print("Fuel Pressure: "); Serial.print(fuelPressure); Serial.println(" Bar");
    break;

    case INTAKE_AIR_TEMP:
      get_inake_air_temp();
    //  Serial.print("Intake Air Temp: "); Serial.print(intakeAirTemp); Serial.println(" C°");
    break;

    case ENGINE_RPM:
      get_rpm();
   //   Serial.print("RPM: "); Serial.println(rpm);    
    break;

    case VEHICLE_SPEED:
      get_vehicle_speed();
    //  Serial.print("Vehicle Speed: "); Serial.print(vehicleSpeed); Serial.println(" km/h");
    break;

    default:
    //sensor_to_read--;
    break;

  }
  
  int next_sensor_btn_state = digitalRead(NEXT_SENSOR);
  if(next_sensor_btn_state == HIGH && sensor_to_read < number_of_sensors()-1){
    if(next_sensor_btn_last_state == LOW)
        sensor_to_read++;
  }
  next_sensor_btn_last_state =  next_sensor_btn_state;
  
  int prev_sensor_btn_state = digitalRead(PREVIOUS_SENSOR);
  if(prev_sensor_btn_state == HIGH && sensor_to_read > 0){
    if(prev_sensor_btn_last_state == LOW)
        sensor_to_read--;    
  }
  prev_sensor_btn_last_state = prev_sensor_btn_state; 
  
  if(DEBUG){
    Serial.print("sensor_to_read: "); Serial.println(sensor_to_read);
    Serial.print("NEXT_SENSOR: ");  Serial.println(digitalRead(NEXT_SENSOR));
    Serial.print("PREVIOUS_SENSOR: ");  Serial.println(digitalRead(PREVIOUS_SENSOR));
    Serial.print("Array Lenght: ");  Serial.println(sizeof(sensors)/sizeof(sensors[0]));
    Serial.print("Sensor[sensor_to_read]: ");  Serial.println(sensors[sensor_to_read]);
  }

    if(PLOTTER){

      //TODO: fare ciclo per chiamarli tutti e controllare prendere valore solo se != 0.0 e da error

      get_engine_coolant_temp();
      Serial.print("Engine_Coolant_Temp(C°):"); Serial.println(engineCoolantTemp);

      get_engine_oil_temp();
      Serial.print("Engine_Oil_Temp(C°):"); Serial.println(engineOilTemp); 


      get_fuel_rail_gauge_pressure();
      Serial.print("Fuel_Pressure(Bar):"); Serial.println(fuelPressure);

      get_inake_air_temp();
      Serial.print("Intake_Air_Temp(C°):"); Serial.println(intakeAirTemp);

      get_rpm();
      Serial.print("RPM:"); Serial.println(rpm);    

      get_vehicle_speed();
      Serial.print("Vehicle_Speed(km/h):"); Serial.println(vehicleSpeed);

  }

  delay(DELAY_TIME);
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


void check_elm_error(String sensor, String value, String unit){
   
   if (myELM327.nb_rx_state == ELM_SUCCESS){
    
      write_lcd(sensor + ": ", value + " " + unit);
    
    } else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
      
      //write_lcd(sensor ,"ERROR");
      if (myELM327.nb_rx_state == ELM_NO_DATA){
      
      write_lcd(sensor ,"N/A");
    }
      
      if(DEBUG_ODB_ERROR_MESSAGES) 
        myELM327.printError();
    
    } else if (myELM327.nb_rx_state == ELM_NO_DATA){
      
      write_lcd(sensor ,"NO_DATA");
    }
}

void get_rpm(){

  rpm = myELM327.rpm();
  
  check_elm_error("RPM", String(rpm), "");
  
}

void get_engine_coolant_temp(){

  engineCoolantTemp = myELM327.engineCoolantTemp();
  
  check_elm_error("Eng Coolant Tmp", String(engineCoolantTemp), "C°");

}

void get_fuel_rail_gauge_pressure(){

  fuelPressure = myELM327.fuelRailGuagePressure();
  fuelPressure = kpa_to_bar(fuelPressure);

  check_elm_error("Rail Gauge Press", String(fuelPressure), "Bar");

}

void get_engine_oil_temp(){

    engineOilTemp = myELM327.oilTemp();
    
    check_elm_error("Engine Oil Temp:", String(engineOilTemp), "C°");

}

void get_inake_air_temp(){

  intakeAirTemp = myELM327.intakeAirTemp();
  
  check_elm_error("Intake Air Temp", String(intakeAirTemp), "C°");

}

void get_vehicle_speed(){
  
  vehicleSpeed = myELM327.kph();

  check_elm_error("Vehicle Speed", String(vehicleSpeed), "km/h");

}
