/*ESP32: Connect a potentiometer to an analog pin(32) and
a LED series with a 120Ω to a pwm pin(21). Look at the image.
Control brightness of the LED using the potentiometer. */

#include <Arduino.h>

const int POTENTIOMETER_PIN = 32; // Analog pin connected to the potentiometer
const int LED_PIN = 21;           // PWM pin connected to the LED

void setup()
{
  pinMode(LED_PIN, OUTPUT); // Set LED pin as OUTPUT
}

void loop()
{
  // Read the analog value from the potentiometer
  int potValue = analogRead(POTENTIOMETER_PIN);

  // Map the analog value to the PWM range (0-255)
  int brightness = map(potValue, 0, 4095, 0, 255);

  // Apply the mapped PWM value to the LED pin
  analogWrite(LED_PIN, brightness);

  // Add a short delay to avoid rapid changes
  delay(10);
}
