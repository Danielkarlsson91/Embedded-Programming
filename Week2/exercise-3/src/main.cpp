/*ESP32: Toggle a led series with a 120Ω resistor connected to a digital pin(21) every 500 ms

For timing, use the delay function.*/

#include <Arduino.h> // Include the Arduino framework header file

#define LED_PIN 21 // Define the pin number for the LED

void setup()
{
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
}

void loop()
{
  digitalWrite(LED_PIN, HIGH); // Turn on the LED
  delay(500);                  // Wait for 5 seconds

  digitalWrite(LED_PIN, LOW); // Turn off the LED

  delay(500); // Wait for 0.5 seconds before repeating the loop
}
