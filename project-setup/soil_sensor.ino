#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize LCD with I2C address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  Wire.begin(23, 22);        // SDA on GPIO 23, SCL on GPIO 22
  Serial.begin(9600);        // Use a more common baud rate like 9600
  lcd.init();                // Initialize the LCD
  lcd.backlight();           // Turn on the backlight
}

void loop() {
  int16_t moistureValue = analogRead(34);   // Read from analog pin 34
  String msg = moistureValue >= 50 ? "WET" : "DRY";  // Determine message

  // Display moisture level and status on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);                       // Set cursor to the first row
  lcd.print("Moisture level:");              // Print moisture level text
  lcd.setCursor(0, 1);                       // Move to the second row
  lcd.print(moistureValue);                  // Print the actual value
  lcd.print(" ");                            // Space between the value and message
  lcd.print(msg);                            // Print WET or DRY based on the value
  
  Serial.print("Moisture level: ");          // Also print the value on Serial Monitor
  Serial.println(moistureValue);

  delay(500);                                // Delay for 500 ms to avoid flickering
}
