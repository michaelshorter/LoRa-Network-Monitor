#include "Adafruit_Thermal.h"


// UART1 for LoRa (Heltec)
HardwareSerial HeltecSerial(1);


// UART2 for Thermal Printer
HardwareSerial PrinterSerial(2);


// Thermal printer object
Adafruit_Thermal printer(&PrinterSerial);


const int LED_PIN = 13;


void setup() {
 Serial.begin(9600);


 // UART1 (Heltec): RX=D4, TX=D5
 HeltecSerial.begin(115200, SERIAL_8N1, 4, 5);


 // UART2 (Thermal Printer): TX=D9 (RX unused)
 PrinterSerial.begin(19200, SERIAL_8N1, /*RX=*/-1, /*TX=*/9);
 printer.begin();


 pinMode(LED_PIN, OUTPUT);


 printer.println(F("=== Printer Ready ==="));
 printer.feed(2);
}


void loop() {


 while (HeltecSerial.available()) {
   String msg = HeltecSerial.readStringUntil('\n');
   msg.trim();


   if (msg.length() > 0) {


     // LED flash feedback
     digitalWrite(LED_PIN, HIGH);
     delay(200);
     digitalWrite(LED_PIN, LOW);


     // Timestamp
     String timestamp = getTimestamp();


     // Print to serial monitor
     Serial.println("[" + timestamp + "] " + msg);


     // Print to thermal printer
     printer.boldOn();
     printer.println("\n--- NEW MESSAGE ---");
     printer.boldOff();
     printer.print("[");
     printer.print(timestamp);
     printer.print("] ");
     printer.println(msg);


     printer.println();
     printer.println();
     printer.feed(2);
   }
 }
}




// Format Time
String getTimestamp() {
 unsigned long totalSeconds = millis() / 1000;
 unsigned long hours   = totalSeconds / 3600;
 unsigned long minutes = (totalSeconds % 3600) / 60;
 unsigned long seconds = totalSeconds % 60;


 char buf[9];
 sprintf(buf, "%02lu:%02lu:%02lu", hours, minutes, seconds);
 return String(buf);
}
