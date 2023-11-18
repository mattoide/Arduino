#include <SoftwareSerial.h>
#include "ELMduino.h"
#include <LiquidCrystal_I2C.h>

#define bluetoothSerial_RX_PIN 2  //TODO: 7 sistemare per real 2
#define bluetoothSerial_TX_PIN 3  //TODO: 8 sistemare per real 3

#define PREVIOUS_SENSOR 10
#define NEXT_SENSOR 11

#define DELAY_TIME 10

#define DEBUG false
#define OBD_DEBUG false
#define DEBUG_ODB_ERROR_MESSAGES false
#define PLOTTER false

#define OBD_TIMEOUT 1000
#define BAUD_RATE 9600
#define OBD_BAUD_RATE 115200
#define ELM_PORT Serial

SoftwareSerial mySerial(bluetoothSerial_RX_PIN, bluetoothSerial_TX_PIN);  // RX, TX
LiquidCrystal_I2C lcd(0x3F, 16, 2);


ELM327 myELM327;

int sensors[] = { ENGINE_COOLANT_TEMP, ENGINE_OIL_TEMP, FUEL_RAIL_GUAGE_PRESSURE, INTAKE_AIR_TEMP, ENGINE_RPM, VEHICLE_SPEED, SUPPORTED_PIDS_1_20 };
int sensor_to_read = 0;

float rpm, engineCoolantTemp, engineOilTemp, fuelPressure, intakeAirTemp, vehicleSpeed = 0;

uint32_t pids = 0;

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

  mySerial.begin(OBD_BAUD_RATE);  //TODO: OBD_BAUD_RATE for real
  ELM_PORT.begin(BAUD_RATE);
  
  lcd.init();       
  lcd.backlight();
                //OBD_PORT for emu
  myELM327.begin(mySerial, OBD_DEBUG, OBD_TIMEOUT);
}

void loop() {

  switch (sensors[sensor_to_read]) {
    case ENGINE_COOLANT_TEMP:
      get_engine_coolant_temp();
      break;

    case ENGINE_OIL_TEMP:
      get_engine_oil_temp();
      break;


    case FUEL_RAIL_GUAGE_PRESSURE:
      get_fuel_rail_gauge_pressure();
      break;

    case INTAKE_AIR_TEMP:
      get_inake_air_temp();
      break;

    case ENGINE_RPM:
      get_rpm();
      break;

    case VEHICLE_SPEED:
      get_vehicle_speed();
      break;


    case SUPPORTED_PIDS_1_20:
      get_supported_PIDs_1_20();
      break;
      

    default:
      //sensor_to_read--;
      break;
  }

  int next_sensor_btn_state = digitalRead(NEXT_SENSOR);

  if (next_sensor_btn_state == HIGH && sensor_to_read < number_of_sensors() - 1) {
    if (next_sensor_btn_last_state == LOW)
      sensor_to_read++;
  }
  next_sensor_btn_last_state = next_sensor_btn_state;

  int prev_sensor_btn_state = digitalRead(PREVIOUS_SENSOR);

  if (prev_sensor_btn_state == HIGH && sensor_to_read > 0) {
    if (prev_sensor_btn_last_state == LOW)
      sensor_to_read--;
  }
  prev_sensor_btn_last_state = prev_sensor_btn_state;

  if (DEBUG) {
    Serial.print("sensor_to_read: ");
    Serial.println(sensor_to_read);
    Serial.print("NEXT_SENSOR: ");
    Serial.println(digitalRead(NEXT_SENSOR));
    Serial.print("PREVIOUS_SENSOR: ");
    Serial.println(digitalRead(PREVIOUS_SENSOR));
    Serial.print("Array Lenght: ");
    Serial.println(sizeof(sensors) / sizeof(sensors[0]));
    Serial.print("Sensor[sensor_to_read]: ");
    Serial.println(sensors[sensor_to_read]);
  }

  if (PLOTTER) {

    if (sensor_to_read < number_of_sensors()) {
      if (calc_finished == true) {
        sensor_to_read++;
        calc_finished = false;
      }
    } else {
      sensor_to_read = 0;
    }
  }

   delay(DELAY_TIME);
}

int number_of_sensors() {
  return sizeof(sensors) / sizeof(sensors[0]);
}

float kpa_to_bar(int kpa) {
  return kpa * 0.01;
}

void display_scroll() {
  for (int positionCounter = 0; positionCounter < 35; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(400);
  }
}

void write_lcd(String sensor, String value, String unit) {

    if(unit == "C")
      unit += char(223);

    String messageRow1 = sensor +  " ";
    String messageRow2 = value +  " " + unit;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(messageRow1);
    lcd.setCursor(0, 1);
    lcd.print(messageRow2);

  if (PLOTTER){
    String sensor_replaced = sensor;
    sensor_replaced.replace(" ", "_");
    
    String string_plotter = sensor_replaced + "(" + unit + "):" + value;
    Serial.println(string_plotter); //TOD:Serial
  } else{
    Serial.println(messageRow1 + messageRow2);

  }
   

}

void check_elm_error(String sensor, String value, String unit) {

  if (myELM327.nb_rx_state == ELM_SUCCESS) {

    calc_finished = true;
          write_lcd(sensor, value, unit);


  } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {

    if (myELM327.nb_rx_state == ELM_NO_DATA) {

      write_lcd(sensor, "N/A", "");

    } else{

      write_lcd(sensor ,"ERROR", "");

    }

    if (DEBUG_ODB_ERROR_MESSAGES)
      myELM327.printError();


    calc_finished = true;

  } else if (myELM327.nb_rx_state == ELM_NO_DATA) {

    calc_finished = true;
    write_lcd(sensor, "NO_DATA", "");
  }
}

void get_supported_PIDs_1_20(){
   pids = myELM327.supportedPIDs_1_20();
   check_elm_error("PIDs_1_20", String(pids), "");
}

void get_rpm() {

  rpm = myELM327.rpm();

  check_elm_error("RPM", String(rpm), "");
}

void get_engine_coolant_temp() {

  engineCoolantTemp = myELM327.engineCoolantTemp();

  check_elm_error("Eng Coolant Tmp", String(engineCoolantTemp), "C");
}

void get_fuel_rail_gauge_pressure() {

  fuelPressure = myELM327.fuelRailGuagePressure();
  fuelPressure = kpa_to_bar(fuelPressure);

  check_elm_error("Rail Gauge Press", String(fuelPressure), "Bar");
}

void get_engine_oil_temp() {

  engineOilTemp = myELM327.oilTemp();

  check_elm_error("Engine Oil Temp", String(engineOilTemp), "C");
}

void get_inake_air_temp() {

  intakeAirTemp = myELM327.intakeAirTemp();

  check_elm_error("Intake Air Temp", String(intakeAirTemp), "C");
}

void get_vehicle_speed() {

  vehicleSpeed = myELM327.kph();

  check_elm_error("Vehicle Speed", String(vehicleSpeed), "km/h");
}
