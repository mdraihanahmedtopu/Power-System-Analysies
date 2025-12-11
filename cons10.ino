#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Display settings (Address 0x27 for a 16x2 LCD)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions for voltage, current sensors, and relays
const int voltagePin1 = 32;  // ADC pin for voltage 1
const int voltagePin2 = 33;  // ADC pin for voltage 2
const int currentPin1 = 2;  // ADC pin for current 1
const int currentPin2 = 5;  // ADC pin for current 2
const int relay1Pin = 18;    // Relay 1 control pin
const int relay2Pin = 19;    // Relay 2 control pin

// Variables for voltage, current, and load side sensors
float voltage1, voltage2, current1, current2, current3;

void setup() {
  // Start the I2C communication
  Wire.begin();
  lcd.begin(16,2);
  lcd.backlight();
  
  // Initialize serial communication for debugging
  Serial.begin(115200);
  
  // Set up ADC pins and relay pins
  pinMode(voltagePin1, INPUT);
  pinMode(voltagePin2, INPUT);
  pinMode(currentPin1, INPUT);
  pinMode(currentPin2, INPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  
  // Set initial state of relays (low)
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
}

void loop() {
  // Read analog values (assuming you have a voltage divider or other sensors to scale the readings)
  voltage1 = analogRead(voltagePin1) * (3.3 / 4095.0) * 10;  // Convert ADC to voltage (with scaling factor)
  voltage2 = analogRead(voltagePin2) * (3.3 / 4095.0) * 10;  // Convert ADC to voltage (with scaling factor)
  current1 = analogRead(currentPin1) * (3.3 / 4095.0) * 5;   // Convert ADC to current (with scaling factor)
  current2 = analogRead(currentPin2) * (3.3 / 4095.0) * 5;   // Convert ADC to current (with scaling factor)

  // Simulate gathering current3 (load side sensor)
  current3 = (current1 + current2) / 2;  // Example logic for current3 calculation

  // Debugging: Print the readings to the serial monitor
  Serial.print("Voltage1: ");
  Serial.print(voltage1);
  Serial.print("V, Current1: ");
  Serial.print(current1);
  Serial.print("A, Voltage2: ");
  Serial.print(voltage2);
  Serial.print("V, Current2: ");
  Serial.print(current2);
  Serial.print("A, Current3: ");
  Serial.println(current3);

  // Clear LCD and set the cursor
  lcd.clear();

  // Check conditions and update the display and relays
  if (voltage1 < 15 && current1 > 0) {
    lcd.setCursor(0, 0);
    lcd.print("L-G in R");
    digitalWrite(relay1Pin, HIGH);  // Turn on Relay 1
    digitalWrite(relay2Pin, LOW);   // Turn off Relay 2
  } else if (voltage2 < 15 && current2 > 0) {
    lcd.setCursor(0, 0);
    lcd.print("L-G in Y");
    digitalWrite(relay1Pin, LOW);   // Turn off Relay 1
    digitalWrite(relay2Pin, HIGH);  // Turn on Relay 2
  } else if (voltage1 < 10 && voltage2 < 10 && current1 > 0 && current2 > 0) {
    lcd.setCursor(0, 0);
    lcd.print("L-L fault");
    digitalWrite(relay1Pin, HIGH);  // Turn on Relay 1
    digitalWrite(relay2Pin, HIGH);  // Turn on Relay 2
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Normal");
    digitalWrite(relay1Pin, LOW);   // Turn off Relay 1
    digitalWrite(relay2Pin, LOW);   // Turn off Relay 2
  }

  // Display voltage and current values
  lcd.setCursor(0, 1);
  lcd.print("V1:");
  lcd.print(voltage1, 1);
  lcd.print("V C1:");
  lcd.print(current1, 1);
  lcd.print("A");

  // Add a delay to update the display every second
  delay(1000);
}
