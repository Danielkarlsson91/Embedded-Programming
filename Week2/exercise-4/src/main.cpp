/*ESP32: Toggle a led series with a 120Ω resistor connected to a digital pin(21)  every 500 ms

To measure time, use the millis function.*/

#include <Arduino.h>

// Define the pin number connected to the LED
#define LED_PIN 21

// Define the time interval for toggling the LED state (in milliseconds)
constexpr uint32_t INTERVAL{500};

// Setup function runs once when the microcontroller starts
void setup()
{
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

// Loop function runs repeatedly after setup
void loop()
{
  // Static variables to keep track of time and LED state between function calls
  static uint32_t prev_millis = 0; // Stores the previous time the LED state was changed
  static bool ledLight = LOW;      // Stores the current state of the LED (LOW or HIGH)

  // Get the current time in milliseconds since the microcontroller started
  uint32_t now = millis();

  // Check if it's time to toggle the LED based on the defined interval
  if (now - prev_millis >= INTERVAL)
  {
    // Update the previous time to the current time
    prev_millis = now;

    // Toggle the LED state from LOW to HIGH or from HIGH to LOW
    ledLight = !ledLight;

    // Set the LED pin to the new state
    digitalWrite(LED_PIN, ledLight);
  }
}
