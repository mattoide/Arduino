#include <SoftwareSerial.h>
#include "ELMduino.h"
#include <LiquidCrystal.h>


#define bluetoothSerial_RX_PIN 7
#define bluetoothSerial_TX_PIN 8

#define PREVIOUS_SENSOR 10
#define NEXT_SENSOR 11

#define DELAY_TIME 10

#define DEBUG false
#define OBD_DEBUG false
#define DEBUG_ODB_ERROR_MESSAGES false
#define PLOTTER true

#define OBD_TIMEOUT 1000
#define BAUD_RATE 9600
#define OBD_BAUD_RATE 115200
#define ELM_PORT Serial

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

  mySerial.begin(BAUD_RATE);
  ELM_PORT.begin(BAUD_RATE);
  lcd.begin(16, 2);

  myELM327.begin(ELM_PORT, OBD_DEBUG, OBD_TIMEOUT);

}

void loop() {

  // mySerial.println("test");
   /*delay(10);
   return;*/

  // write_lcd("String messageRow1", "String messageRow2");

  switch (sensors[sensor_to_read]) {
    case ENGINE_COOLANT_TEMP:
      get_engine_coolant_temp();
     // mySerial.print("Engine Coolant Temp: "); mySerial.print(engineCoolantTemp); mySerial.println(" C°");
    break;

    case ENGINE_OIL_TEMP:
      get_engine_oil_temp();
    //  mySerial.print("Engine Oil Temp: "); mySerial.print(engineOilTemp); mySerial.println(" C°");
    break;


    case FUEL_RAIL_GUAGE_PRESSURE:
      get_fuel_rail_gauge_pressure();
    //  mySerial.print("Fuel Pressure: "); mySerial.print(fuelPressure); mySerial.println(" Bar");
    break;

    case INTAKE_AIR_TEMP:
      get_inake_air_temp();
    //  mySerial.print("Intake Air Temp: "); mySerial.print(intakeAirTemp); mySerial.println(" C°");
    break;

    case ENGINE_RPM:
      get_rpm();
   //   mySerial.print("RPM: "); mySerial.println(rpm);    
    break;

    case VEHICLE_SPEED:
      get_vehicle_speed();
    //  mySerial.print("Vehicle Speed: "); mySerial.print(vehicleSpeed); mySerial.println(" km/h");
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
    mySerial.print("sensor_to_read: "); mySerial.println(sensor_to_read);
    mySerial.print("NEXT_SENSOR: ");  mySerial.println(digitalRead(NEXT_SENSOR));
    mySerial.print("PREVIOUS_SENSOR: ");  mySerial.println(digitalRead(PREVIOUS_SENSOR));
    mySerial.print("Array Lenght: ");  mySerial.println(sizeof(sensors)/sizeof(sensors[0]));
    mySerial.print("Sensor[sensor_to_read]: ");  mySerial.println(sensors[sensor_to_read]);
  }

    if(PLOTTER){

      //TODO: fare ciclo per chiamarli tutti e controllare prendere valore solo se != 0.0 e da error

    /*  mySerial.print("Engine_Coolant_Temp(C°):"); mySerial.println(engineCoolantTemp);

      mySerial.print("Engine_Oil_Temp(C°):"); mySerial.println(engineOilTemp); 


      mySerial.print("Fuel_Pressure(Bar):"); mySerial.println(fuelPressure);

      mySerial.print("Intake_Air_Temp(C°):"); mySerial.println(intakeAirTemp);

      mySerial.print("RPM:"); mySerial.println(rpm);    

      mySerial.print("Vehicle_Speed(km/h):"); mySerial.println(vehicleSpeed);
      */
      if(sensor_to_read<number_of_sensors()){
              if(calc_finished == true){
          sensor_to_read++;
          calc_finished = false; 
      }  
      }else{
sensor_to_read = 0;

      } 
        
  }

 // delay(DELAY_TIME);
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
if(!PLOTTER)
    mySerial.println(messageRow1 + " " + messageRow2 );
    else
    mySerial.println(messageRow1 + messageRow2 );

 // mySerial.println("message from unoooo");


}

void get_all_sensors(){
    get_engine_coolant_temp();
    get_engine_oil_temp();
    get_fuel_rail_gauge_pressure();
    get_inake_air_temp();
    get_rpm();
    get_vehicle_speed();
}

void check_elm_error(String sensor, String value, String unit){
   
   if (myELM327.nb_rx_state == ELM_SUCCESS){
  
  calc_finished = true;
      write_lcd(sensor + "(" + unit +"):", value);
    
    } else if (myELM327.nb_rx_state != ELM_GETTING_MSG){
      
      //write_lcd(sensor ,"ERROR");
      if (myELM327.nb_rx_state == ELM_NO_DATA){
      
      write_lcd(sensor ,"N/A");
    }
      
      if(DEBUG_ODB_ERROR_MESSAGES) 
        myELM327.printError();
    

  calc_finished = true;
    
    } else if (myELM327.nb_rx_state == ELM_NO_DATA){
      
  calc_finished = true;
      write_lcd(sensor ,"NO_DATA");
    }
}

void get_rpm(){

  rpm = myELM327.rpm();
  
  check_elm_error("RPM", String(rpm), "");
  
}

void get_engine_coolant_temp(){

  engineCoolantTemp = myELM327.engineCoolantTemp();
  
  check_elm_error("Eng_Coolant_Tmp", String(engineCoolantTemp), "C°");

}

void get_fuel_rail_gauge_pressure(){

  fuelPressure = myELM327.fuelRailGuagePressure();
  fuelPressure = kpa_to_bar(fuelPressure);

  check_elm_error("Rail_Gauge_Press", String(fuelPressure), "Bar");

}

void get_engine_oil_temp(){

    engineOilTemp = myELM327.oilTemp();
    
    check_elm_error("Engine_Oil_Temp", String(engineOilTemp), "C°");

}

void get_inake_air_temp(){

  intakeAirTemp = myELM327.intakeAirTemp();
  
  check_elm_error("Intake_Air_Temp", String(intakeAirTemp), "C°");

}

void get_vehicle_speed(){
  
  vehicleSpeed = myELM327.kph();

  check_elm_error("Vehicle_Speed", String(vehicleSpeed), "km/h");

}
