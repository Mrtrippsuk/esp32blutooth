#include <BluetoothSerial.h>

BluetoothSerial SerialBT; // Rename BluetoothSerial object to SerialBT

const int touchPin = T0; // Define the touch sensor pin
const int ledPin = 5;     // Define the LED pin

char cmd;

unsigned long lastTriggerTime = 0; // Variable to store the time of the last trigger
const unsigned long debounceDelay = 1000; // Debounce delay in milliseconds

void setup() {
  SerialBT.begin("MY-ESP"); // Start Bluetooth serial communication
  Serial.begin(115200);     // Start serial communication for debugging
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Ensure LED is initially off
}

void loop() {
  int touchValue = touchRead(touchPin);
  Serial.println(touchValue); // Print touch sensor value to Serial Monitor

  if (SerialBT.available()) {
    cmd = SerialBT.read(); // Read the incoming character
    // Check the received command
    if (cmd == '1') { // '1' represents the command to turn Off
      digitalWrite(ledPin, HIGH); // Turn on the LED
      Serial.println("LED Off");   // Print debug message to Serial Monitor
      SerialBT.println("LED is now Off"); // Send reply over Bluetooth
    }
    else if (cmd == '0') { // '0' represents the command to turn On
      digitalWrite(ledPin, LOW); // Turn off the LED
      Serial.println("LED On"); // Print debug message to Serial Monitor
      SerialBT.println("LED is now On"); // Send reply over Bluetooth
    }
    else if (cmd == 's') { // 's' represents the status request
      SerialBT.print("The Value is.. ");
      SerialBT.println(touchValue); // Send touch sensor value over Bluetooth
    }
  }

  // Check if touch sensor value is below 85 and debounce the input
  if (touchValue <= 85 && millis() - lastTriggerTime >= debounceDelay) {
    digitalWrite(ledPin, !digitalRead(ledPin)); // Toggle the LED
    Serial.println("Remote activated");         // Print message to Serial Monitor
    SerialBT.println("Remote activated");       // Send message over Bluetooth
    lastTriggerTime = millis(); // Update the last trigger time
  }

  delay(20);
}
