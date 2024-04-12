/*ESP32: Connect an RGB LED series with three 120Ω resistors to pwm pins 16, 18 and 21.

Every second make a random color by writing random pwm values to the pins.

For timing, use the delay function. To generate random numbers use the random function.*/

#include <Arduino.h>

// Define RGB LED pin mappings
const int RED_PIN = 16;
const int GREEN_PIN = 18;
const int BLUE_PIN = 21;

void setup()
{
  // Set RGB LED pins as OUTPUT
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Initialize random seed
  randomSeed(analogRead(0)); // Use an analog pin reading as seed
}

void loop()
{
  // Generate random PWM values for each color component
  int redValue = random(0, 256);
  int greenValue = random(0, 256);
  int blueValue = random(0, 256);

  // Apply random PWM values to the RGB LED pins
  analogWrite(RED_PIN, redValue);
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(BLUE_PIN, blueValue);

  // Delay for one second before changing colors again
  delay(1000);
}
