// This code runs on the ESP8266.
// It connects to WiFi, listens for data from the Arduino Uno on the main Serial port (RX pin),
// and uploads the sensor readings to ThingSpeak using direct WiFiClient for maximum speed.

// --- Required Libraries ---
#include <ESP8266WiFi.h>

// --- WiFi Configuration ---
const char* ssid = "hotspot_01";         // <<< IMPORTANT: Replace with your WiFi network name
const char* password = "22222222"; // <<< IMPORTANT: Replace with your WiFi password

// --- ThingSpeak Configuration ---
// Write API Key for your Channel
const char* thingSpeakApiKey = "FMR1YTXU5UX0Z7E7"; 
const char* thingSpeakHost = "api.thingspeak.com";
const int thingSpeakPort = 80; // Standard HTTP Port

// ThingSpeak Field Mapping:
// field1 = PH-Sensor
// field2 = Turbidity Sensor
// field3 = Water Level Sensor
// field4 = TDS Meter Sensor

// Note: On ESP8266, we use the main 'Serial' object (connected to GPIO3/RX) for Uno data.
const int serialBaudRate = 9600;

// --- State Variables ---
String incomingData = "";
float ph, turbidity, waterLevel, tds;

// Define the WiFiClient globally
WiFiClient client;

void connectToWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    // Keep trying until connected
    int maxAttempts = 20; 
    while (WiFi.status() != WL_CONNECTED && maxAttempts > 0) {
        delay(500);
        Serial.print(".");
        maxAttempts--;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println(" Failed to connect to WiFi!");
        // We will try again on the next loop
    }
}

void setup() {
    // Initialize main Serial for both debugging (USB) AND data reception (GPIO3/RX)
    Serial.begin(9600);
    Serial.println("\nESP8266 ThingSpeak Gateway Starting...");

    // Connect to WiFi once during setup
    connectToWiFi();
}

void parseAndUploadData(String dataString) {
    if (dataString.length() == 0) return;

    // The incoming data format is: PH,TURBIDITY,LEVEL,TDS

    // Use a counter to track position
    int lastIndex = 0;
    int currentIndex = 0;
    
    // 1. PH (Field 1)
    currentIndex = dataString.indexOf(',', lastIndex);
    ph = dataString.substring(lastIndex, currentIndex).toFloat();
    lastIndex = currentIndex + 1;

    // 2. Turbidity (Field 2)
    currentIndex = dataString.indexOf(',', lastIndex);
    turbidity = dataString.substring(lastIndex, currentIndex).toFloat();
    lastIndex = currentIndex + 1;

    // 3. Water Level (Field 3)
    currentIndex = dataString.indexOf(',', lastIndex);
    waterLevel = dataString.substring(lastIndex, currentIndex).toFloat(); // Now in %
    lastIndex = currentIndex + 1;

    // 4. TDS (Field 4)
    tds = dataString.substring(lastIndex).toFloat();

    Serial.println("\n--- Parsed Data for ThingSpeak Update (5s Cycle) ---");
    Serial.print("PH (F1): "); Serial.println(ph);
    Serial.print("Turbidity (F2): "); Serial.println(turbidity);
    Serial.print("Level (F3): "); Serial.print(waterLevel); Serial.println("%");
    Serial.print("TDS (F4): "); Serial.println(tds);
    
    // --- Construct the ThingSpeak URL (the request payload) ---
    // Using C-style char array for speed in URL construction
    char postData[256];
    
    // Format the URL request string
    snprintf(postData, 256,
             "/update?api_key=%s&field1=%.1f&field2=%.0f&field3=%.0f&field4=%.0f",
             thingSpeakApiKey,
             ph,
             turbidity,
             waterLevel,
             tds
            );

    // --- Send HTTP Request via raw WiFiClient for speed ---
    if (!client.connect(thingSpeakHost, thingSpeakPort)) {
        Serial.println("Connection to ThingSpeak failed.");
        return;
    }

    // Send the GET request
    client.print("GET ");
    client.print(postData);
    client.println(" HTTP/1.1");
    
    // Send required headers
    client.print("Host: ");
    client.println(thingSpeakHost);
    client.println("Connection: close"); // Important: closes connection after request
    client.println(); // End of headers

    // Read the response from ThingSpeak (optional, but good for debugging)
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) { // 5 second timeout
            Serial.println(">>> ThingSpeak Client Timeout!");
            client.stop();
            return;
        }
    }

    // Skip the HTTP response headers and read the ThingSpeak entry ID
    while (client.available()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
            // Found the end of headers
            break;
        }
    }
    
    // Read the entry ID (ThingSpeak response, e.g., "12345")
    if (client.available()) {
        String entryId = client.readStringUntil('\n');
        entryId.trim();
        Serial.print("ThingSpeak Update Successful. Entry: ");
        Serial.println(entryId);
    } else {
        Serial.println("ThingSpeak Update Successful (No ID received).");
    }

    // Close the connection
    client.stop();
}


void loop() {
    // Check WiFi status and attempt reconnection if needed
    if (WiFi.status() != WL_CONNECTED) {
        connectToWiFi();
        // Continue loop to wait for data or reconnect
        return; 
    }
    
    // 1. Check for incoming serial data from Arduino Uno
    while (Serial.available()) {
        char c = Serial.read();
        incomingData += c;

        // Check for newline character, which indicates the end of a message
        if (c == '\n') {
            // Remove the newline/carriage return characters from the end
            incomingData.trim(); 
            
            // --- Upload immediately upon reception (fastest possible) ---
            if (incomingData.length() > 0) {
                parseAndUploadData(incomingData);
            }
            
            // Clear the buffer for the next incoming message
            incomingData = ""; 
        }
    }
    // ESP8266 needs small delays for smooth background operation
    yield(); 

}
