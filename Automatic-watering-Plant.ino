#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define SOIL_MOISTURE_SENSOR A0
#define RELAY_PIN 7
#define BUZZER 6
#define LED 5
 
SoftwareSerial gsm(9, 10);  // RX, TX for GSM module
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD address 0x27

void setup() {
  pinMode(SOIL_MOISTURE_SENSOR, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  
  Serial.begin(9600);
  gsm.begin(9600);
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  delay(2000);
}

void loop() {
  int moistureLevel = analogRead(SOIL_MOISTURE_SENSOR);
  int moisturePercent = map(moistureLevel, 1023, 0, 0, 100);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture:");
  lcd.setCursor(0, 1);
  lcd.print(moisturePercent);
  lcd.print("%");

  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  if (moisturePercent < 30) {  // If soil is dry
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    sendSMS(moisturePercent); // Send SMS
    delay(5000);  // Pump runs for 5 seconds
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
  }
  
  delay(5000);  // Check every 5 seconds
}

void sendSMS(int moisture) {
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CMGS=\"+918918952076\"");
  delay(1000);
  gsm.print("Water given to the plant. Current moisture level: ");
  gsm.print(moisture);
  gsm.println("%");
  delay(1000);
  gsm.write(26);  
  delay(1000);
}
