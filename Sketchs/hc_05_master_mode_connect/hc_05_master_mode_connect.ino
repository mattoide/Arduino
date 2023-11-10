#include <SoftwareSerial.h>

#define RX_BT_PIN 2
#define TX_BT_PIN 3

#define BAUDRATE 38400
#define BAUDRATE_SERIAL 9600

String ELM32_MACADDRESS = "001D,A5,68988B";

String atCommandStart = "AT+";
//String atCommands[] = {"ROLE=1", "ROLE", "UART=115200,0,0","UART", "BIND=" + ELM32_MACADDRESS, "BIND", "PAIR=" + ELM32_MACADDRESS + ",2", "STATE"};
String atCommands[] = {"RMAAD","ROLE=1", "ROLE","INQM=0,5,9", "CMODE=0", "CMODE", "CLASS=0", "CLASS", "UART=115200,0,0","UART", "BIND=" + ELM32_MACADDRESS, "BIND","INIT", "PAIR=" + ELM32_MACADDRESS + ",10", "STATE"};

int commandNumber = 0;
int numberOfATCommands = sizeof(atCommands) / sizeof(atCommands[0]);
String cmd = "";

SoftwareSerial bluetoothSerial(RX_BT_PIN, TX_BT_PIN);

void setup() {
    pinMode(RX_BT_PIN, INPUT);
    pinMode(TX_BT_PIN, OUTPUT);

    Serial.begin(BAUDRATE_SERIAL);
    bluetoothSerial.begin(BAUDRATE);

    Serial.println("Sending AT Commands...\n\n");

    sendFirsATCheckCommand();
}

void sendFirsATCheckCommand() {
    Serial.println("Command sent: ");
    Serial.println("AT");
    Serial.println("-----");
    bluetoothSerial.println("AT");
}

void loop() {
    sendAtCommands();
    delay(10);
}

void sendAtCommands() {
    while (bluetoothSerial.available()) {
        Serial.println("Response from BT module: ");

        Serial.print(bluetoothSerial.readString());

        Serial.println("-----");

        if (commandNumber < numberOfATCommands) {
            cmd = atCommandStart + atCommands[commandNumber];

            bluetoothSerial.println(cmd);
            Serial.println("Command sent: ");
            Serial.println(cmd);
            Serial.println("-----");
            commandNumber++;
        }

        else 
            while (1);
              
        
    }
}
