#define D2 4  // Define D2 as GPIO4 for DHT11 sensor
#define DHTPIN D2        // DHT11 data pin connected to D2 (GPIO4)
#define DHTTYPE DHT11    // DHT 11 sensor

#define flame_PIN D3    // Flame sensor connected to digital pin D3
#define LED_PIN D1      // LED connected to GPIO5 (D1)

#define BLYNK_TEMPLATE_ID "TMPL32wv_kL8W"
#define BLYNK_TEMPLATE_NAME "flame sensor"
#define BLYNK_AUTH_TOKEN "CqxIxjYFB4Ihb3sTL2wiIVSCpzmcq7jH"

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <BlynkSimpleEsp8266.h>

// Wi-Fi credentials
char ssid[] = "USER";       // WiFi SSID
char pass[] = "user@123";   // WiFi Password

// ThingSpeak credentials
unsigned long myChannelNumber = 2747576;    // ThingSpeak Channel ID
const char* myWriteAPIKey = "QIRQU2H3R264UGHS";    // ThingSpeak Write API Key

// Blynk credentials
char auth[] = "CqxIxjYFB4Ihb3sTL2wiIVSCpzmcq7jH";  // Blynk Auth Token

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor

bool flameDetected = false;  // Track fire detection state

void setup() {
  Serial.begin(115200);
  delay(10);

  // Set up pins
  pinMode(LED_PIN, OUTPUT);   // LED pin as output
  pinMode(flame_PIN, INPUT);  // Flame sensor pin as input
  digitalWrite(LED_PIN, LOW); // Turn off LED initially

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak and Blynk
  ThingSpeak.begin(client);
  Blynk.begin(auth, ssid, pass);
  dht.begin();  // Start DHT sensor
}

void loop() {
  // Read data from DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read data from flame sensor digital pin
  int flameValue = digitalRead(flame_PIN);

  // Check if the DHT sensor is giving valid readings
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print readings to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Flame Detected: ");
  Serial.println(flameValue == LOW ? "YES" : "NO");

  // Update ThingSpeak with DHT11 data (Temperature and Humidity)
  ThingSpeak.setField(1, temperature);  // Field 1 = Temperature
  ThingSpeak.setField(2, humidity);     // Field 2 = Humidity
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // Send flame data to Blynk app (using virtual pin V1)
  Blynk.virtualWrite(V1, flameValue == LOW ? 1 : 0); // 1 for flame detected, 0 otherwise

  // Check if fire is detected
  if (flameValue == LOW) {
    if (!flameDetected) {
      // Send Blynk notification only once when flame is first detected
      Blynk.logEvent("flame_detected", "Flame detected!");  // Updated notification
      flameDetected = true;  // Set the flame detected state
    }

    // Hazardous LED Blink
    Serial.println("Flame detected! Hazard light blinking...");
    digitalWrite(LED_PIN, HIGH);
    delay(200);  // LED on for 200ms
    digitalWrite(LED_PIN, LOW);
    delay(200);  // LED off for 200ms
  } else {
    Serial.println("No flame detected.");
    digitalWrite(LED_PIN, LOW);  // Ensure LED is off
    flameDetected = false;       // Reset the flame detected state
  }

  // Run Blynk
  Blynk.run();

  // Wait 2 seconds before the next loop iteration
  delay(2000);
}
