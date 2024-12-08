//#include <STM32F1.h>  // STM32 Blue Pill library

// Define pin for LED
#define LED_PIN PB1  // Example pin, choose an appropriate pin for your setup

void setup() {
  // Start Serial communication at 9600 baud
  Serial.begin(9600);

  // Configure PA0 (ADC1 Channel 0) for the MQ3 sensor output
  pinMode(PA0, INPUT_ANALOG);  // Set PA0 as analog input
  
  // Configure LED_PIN as output
  pinMode(LED_PIN, OUTPUT);
  
  // Warm-up time for the MQ3 sensor
  delay(20000); // Wait 20 seconds for sensor to stabilize
}

void loop() {
  // Read the analog value from the MQ3 sensor on PA0 (12-bit ADC, range 0-4095)
  int adcValue = analogRead(PA0);  
  
  // Check the value and classify the smell level
  if (adcValue < 100) {
    Serial.print("No smell detected - ADC Value: ");
    digitalWrite(LED_PIN, LOW);  // Turn off LED
  } 
  else if (adcValue >= 100 && adcValue < 150) {
    Serial.print("Low rotten smell detected - ADC Value: ");
    digitalWrite(LED_PIN, LOW);  // Turn off LED
  } 
  else if (adcValue >= 150 && adcValue < 300) {
    Serial.print("Moderate rotten smell detected - ADC Value: ");
    digitalWrite(LED_PIN, LOW);  // Turn off LED
  } 
  else if (adcValue >= 400) {
    Serial.print("High rotten smell detected - ADC Value: ");
    
    // Blink LED continuously to indicate high alcohol smell
    digitalWrite(LED_PIN, HIGH);
    delay(250);  // Adjust delay for blink speed
    digitalWrite(LED_PIN, LOW);
    delay(250);
  }
  
  // Print the ADC value in the same line
  Serial.println(adcValue);

  // Delay between readings, adjust as needed
  delay(1000);  
}
