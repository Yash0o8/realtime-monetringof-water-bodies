// This code runs on the Arduino Uno.
// It reads all sensors, displays data on the LCD, and sends the data to the ESP32 via Serial (TX/RX).

// --- Required Library: LiquidCrystal I2C ---
// Install the 'LiquidCrystal I2C' library via the Arduino Library Manager.
#include <LiquidCrystal_I2C.h>

// Initialize the LCD (adjust I2C address and screen size if necessary)
// Common I2C addresses are 0x27 or 0x3F.
// This example assumes a 16x2 screen (16 columns, 2 rows).
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Sensor Pin Definitions (Analog Pins) ---
const int phSensorPin = A0;
const int turbiditySensorPin = A1;
const int waterLevelSensorPin = A2;
const int tdsSensorPin = A3;

// --- Calibration/Scaling Variables (Simplified for demonstration) ---
// In a real application, replace these placeholder mappings with actual sensor library code.
const float VREF = 5.0; // Arduino operating voltage

float readPH() {
    // Simulated pH reading (e.g., 6.5 - 8.5)
    int rawValue = analogRead(phSensorPin);
    // Simple linear scaling for demonstration (In reality, use a pH library and calibration)
    return map(rawValue, 0, 1023, 650, 850) / 100.0;
}

int readTurbidity() {
    // Simulated Turbidity reading (e.g., 0 - 100 NTU)
    int rawValue = analogRead(turbiditySensorPin);
    // Invert reading for typical turbidity sensor (higher voltage = lower turbidity)
    int mappedValue = map(rawValue, 0, 1023, 100, 0);
    return constrain(mappedValue, 0, 100); // NTU (Nephelometric Turbidity Units)
}

int readWaterLevel() {
    // --- UPDATED: Water Level now returns 0% to 100% ---
    int rawValue = analogRead(waterLevelSensorPin);
    return map(rawValue, 0, 1023, 0, 100); // 0% (Dry) to 100% (Full)
}

int readTDS() {
    // Simulated TDS reading (e.g., 100 - 500 ppm)
    int rawValue = analogRead(tdsSensorPin);
    // Simple scaling for demonstration (In reality, use a TDS sensor library and temperature compensation)
    return map(rawValue, 0, 1023, 100, 500); // PPM (Parts Per Million)
}

void setup() {
    // Initialize Serial communication with the ESP32
    Serial.begin(9600);
    
    // Initialize the LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    // Initial display message
    lcd.print("Water Quality Mon");
    lcd.setCursor(0, 1);
    lcd.print("Data Rate: 5s");
    delay(3000);
    lcd.clear();
}

void loop() {
    // 1. Read all sensor data
    float phValue = readPH();
    int turbidityValue = readTurbidity();
    int waterLevel = readWaterLevel(); // Now 0-100%
    int tdsValue = readTDS();

    // 2. Format and Send Data to ESP32
    // Format: PH,TURBIDITY,LEVEL,TDS
    String dataToSend = String(phValue) + "," +
                        String(turbidityValue) + "," +
                        String(waterLevel) + "," +
                        String(tdsValue);
                        
    Serial.println(dataToSend);
    
    // 3. Display Data on LCD (Optimized for 16x2)
    lcd.clear(); // Clear for crisp update
    
    // Line 1: PH and Turbidity
    // Example: PH:7.2 T:15NTU
    lcd.setCursor(0, 0);
    lcd.print("PH:");
    lcd.print(phValue, 1); // PH with 1 decimal place
    lcd.print(" T:");
    lcd.print(turbidityValue);
    lcd.print("NTU");

    // Line 2: Level (%) and TDS
    // Example: Lvl:75% TDS:450ppm
    lcd.setCursor(0, 1);
    lcd.print("Lvl:");
    lcd.print(waterLevel);
    lcd.print("%"); // Displays percentage
    lcd.print(" TDS:");
    lcd.print(tdsValue);
    lcd.print("ppm");

    // --- UPDATED: Faster Update Rate ---
    delay(5000); // Read, send, and refresh data every 5 seconds
}
