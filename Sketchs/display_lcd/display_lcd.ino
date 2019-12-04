// include the library code:
#include <LiquidCrystal.h>

// Viene inizializzata la libreria del Display con i relativi PIN a cui è connesso il Display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {

  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

write();
    //lcd.print("Federica fet e merd. federrica puzz e cacc. federica e na zzozz");   
} 

void loop() {
  for (int positionCounter = 0; positionCounter < 35; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(400);
  }
write();

}

void write(){
    lcd.clear();
      /*  lcd.setCursor(16, 0);
    lcd.print(F("                                                            "));   */

    lcd.setCursor(16, 0);
    lcd.print(F("PoRcO DiO"));   
}
