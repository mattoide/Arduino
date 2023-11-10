
void setup() {
Serial.begin(115200);
}

void loop() {
String msg = "";
while(Serial.available()){
   Serial.read();
  Serial.print(Serial.read());

}

delay(3000);
}
