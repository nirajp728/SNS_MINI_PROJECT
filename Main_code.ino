//Moving Average Filter

#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include "ThingSpeak.h"

// ThingSpeak details
const int channelID = 2477469;
const char* writeAPIKey = "EGGNEU840XED4NHA";
const char* server = "api.thingspeak.com";

// Wi-Fi credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// LCD and ThingSpeak initialization
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient client;

// Moving average filter variables
const int sampleSize = 10;
int values[sampleSize] = {0};
int total = 0;
int index = 0;

void setup() {
  Wire.begin(23, 22);
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();

  // Wi-Fi connection setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");
  Serial.println("Local IP: " + String(WiFi.localIP()));

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

int getFilteredMoistureLevel() {
  int rawMoisture = analogRead(34);  // Read moisture sensor
  total -= values[index];
  values[index] = rawMoisture;
  total += values[index];
  index = (index + 1) % sampleSize; // Wrap index around
  return total / sampleSize;  // Return the average of the last readings
}

void loop() {
  int filteredMoistureLevel = getFilteredMoistureLevel();
  String msg = filteredMoistureLevel <= 50 ? "DRY" : "WET";

  // Display on LCD
  lcd.clear();
  lcd.print("Soil: ");
  lcd.print(msg);
  lcd.setCursor(0, 1); // Move to the second line
  lcd.print("Level: ");
  lcd.print(filteredMoistureLevel);
  lcd.print("%");

  // Send data to ThingSpeak
  ThingSpeak.setField(1, filteredMoistureLevel);  // Set moisture level to field 1
  int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (responseCode == 200) {
    Serial.println("Data sent successfully to ThingSpeak");
  } else {
    Serial.println("Error sending data: " + String(responseCode));
  }

  delay(1000);  // Delay to avoid rate limit issues
}