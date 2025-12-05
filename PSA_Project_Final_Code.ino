#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
const int relay1Pin = 18;  // Relay 1
const int relay2Pin = 19;  // Relay 2
const int ir1 = 13;  // IR Sensor 1
const int ir2 = 12;  // IR Sensor 2
const int ct1 = 2;  // Current Sensor CT1
const int ct2 = 5;
const int ct3 = 6;   // Current Sensor CT3
const int vt1 = 15;  // Voltage Sensor VT1
const int vt2 = 4;  // Voltage Sensor VT2
const int vt3 = 7;

// Display setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensor values (Assume these are analog values for current/voltage)
int CT1, CT2, CT3, VT1, VT2, VT3;
int irSensor1State, irSensor2State;

void setup() {
  // Setup for pins
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("System Initialized");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Read sensor values
  irSensor1State = digitalRead(ir1);
  irSensor2State = digitalRead(ir2);

  CT1 = analogRead(ct1);
  CT2 = analogRead(ct2);
  CT3 = analogRead(ct3);
  VT1 = analogRead(vt1);
  VT2 = analogRead(vt2);
  VT3 = analogRead(vt3);

  // Check IR sensors and activate relays
  if (irSensor1State == HIGH) {  // Object detected by IR Sensor 1
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Object Detected");
    lcd.setCursor(0, 1);
    lcd.print("Poll 1 to 2");
    delay(1000);
  }
  else if (irSensor2State == HIGH) {  // Object detected by IR Sensor 2
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Object Detected");
    lcd.setCursor(0, 1);
    lcd.print("Poll 2 to 3");
    delay(1000);
  }

  // Check for faults and update relays and display
  else if (CT1 > CT3 && CT2 > CT3 && VT1 < 13 && VT2 < 15) {  // L-L fault condition
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, HIGH);
    lcd.clear();
    lcd.print("L-L Fault");
    delay(1000);
  }
  else if (CT1 > CT3 && VT1 < 13) {  // L-G fault condition
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, LOW);
    lcd.clear();
    lcd.print("L-G Fault");
    delay(1000);
  }
  else if (CT2 > CT3 && VT2 < 15) {  // L-G fault condition
    digitalWrite(relay1Pin, LOW);
    digitalWrite(relay2Pin, HIGH);
    lcd.clear();
    lcd.print("L-G Fault");
    delay(1000);
  }
  else {
    digitalWrite(relay1Pin, LOW);
    digitalWrite(relay2Pin, LOW);
  }

  delay(100);  // Small delay to avoid overload
}
