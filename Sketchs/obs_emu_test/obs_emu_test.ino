#include <SoftwareSerial.h>
#include "ELMduino.h"


//SoftwareSerial mySerial(2, 3); // RX, TX
#define ELM_PORT Serial


ELM327 myELM327;


uint32_t rpm = 0;


void setup()
{
#if LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

 // Serial.begin(115200);
  ELM_PORT.begin(115200);

  Serial.println("Attempting to connect to ELM327...");

  if (!myELM327.begin(ELM_PORT, false, 2000))
  {
    Serial.println("Couldn't connect to OBD scanner");

    while (1);
  } 

    digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Connected to ELM327");
}


void loop(){

  /*if(Serial.available()){
    Serial.println(Serial.readString());
  }*/

  get_rpm();

  /*
 get_fuel_rail_gauge_pressure();
  get_engine_oil_temp();
delay(50);*/
/*

for(int i = 0; i < 20; i++){
 get_rpm();
}

delay(1000);


for(int i = 0; i < 20; i++){
 get_fuel_rail_gauge_pressure();
}

delay(1000);


for(int i = 0; i < 20; i++){
 get_engine_oil_temp();
}
*/
//delay(1000);
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
      
     //   myELM327.printError();
    
    }/* else if (myELM327.nb_rx_state != ELM_NO_DATA){
      
      write_lcd(sensor ,"NO_DATA");
    }*/
}

float kpa_to_bar(int kpa){
  return kpa * 0.01;
}
void get_rpm(){

  float rpm = myELM327.rpm();

  
  check_elm_error("RPM", String(rpm), "");

  
}

void get_engine_coolant_temp(){

float  engineCoolantTemp = myELM327.engineCoolantTemp();
  
  check_elm_error("Eng Coolant Tmp", String(engineCoolantTemp), "C°");

}

void get_fuel_rail_gauge_pressure(){

 float fuelPressure = myELM327.fuelRailGuagePressure();
  fuelPressure = kpa_to_bar(fuelPressure);

  check_elm_error("Rail Gauge Press", String(fuelPressure), "Bar");

}

void get_engine_oil_temp(){

   float engineOilTemp = myELM327.oilTemp();
    
    check_elm_error("Engine Oil Temp", String(engineOilTemp), "C°");

}

void get_inake_air_temp(){

 float intakeAirTemp = myELM327.intakeAirTemp();
  
  check_elm_error("Intake Air Temp", String(intakeAirTemp), "C°");

}

void get_vehicle_speed(){
  
 float vehicleSpeed = myELM327.kph();

  check_elm_error("Vehicle Speed", String(vehicleSpeed), "km/h");

}
