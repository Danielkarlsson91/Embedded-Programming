/*ESP32: Connect an LED series with a 120 resistor to a pwm pin (21).

Make the LED fading in/out with 5 steps using a PWM signal.

For timing use the millis function with interval 50 ms.*/

#include <Arduino.h>

const int LED_PIN = 21; // Define the LED PWM pin

unsigned long previousMillis = 0; // Variable to store the previous millis value
unsigned long interval = 50;      // Interval for fading steps

int brightness = 0; // LED brightness level
int fadeAmount = 5; // Step size for fading

void setup()
{
  // Set the LED pin as an OUTPUT
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  unsigned long currentMillis = millis(); // Get the current millis value

  // Check if it's time to change the brightness
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; // Update the previousMillis value

    // Increment or decrement the brightness by the fade amount
    brightness += fadeAmount;

    // If brightness reaches 0 or 255, reverse the fade direction
    if (brightness <= 0 || brightness >= 255)
    {
      fadeAmount = -fadeAmount;
    }

    // Set the brightness level to the LED
    analogWrite(LED_PIN, brightness);
  }
}
