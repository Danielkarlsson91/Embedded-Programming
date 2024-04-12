/*Connect an LED series with a 120 resistor to a pwm pin(21).

Make the LED fading in/out with 5 steps using a pwm signal.

For timing use the first timer interrupt with interval 50 ms.*/

#include <Arduino.h>

const int LED_PIN = 21; // Define the LED PWM pin

volatile int brightness = 0; // LED brightness level
int fadeAmount = 25;         // Step size for fading

// Timer ISR to update PWM signal
void IRAM_ATTR timerISR()
{
  // Update the brightness level by the fade amount
  brightness += fadeAmount;

  // If brightness reaches 0 or 255, reverse the fade direction
  if (brightness <= 0 || brightness >= 255)
  {
    fadeAmount = -fadeAmount;
  }

  // Update PWM signal
  ledcWrite(0, brightness);
}

void setup()
{
  // Set the LED pin as an OUTPUT
  pinMode(LED_PIN, OUTPUT);

  ledcSetup(0, 5000, 8);     // Channel 0, 5 kHz frequenzy, 8-bit resolution
  ledcAttachPin(LED_PIN, 0); // Attach PWM channel to LED pin

  hw_timer_t *timer{timerBegin(0, 80, true)};   // Timer 0, prescaler 80, counting up
  timerAttachInterrupt(timer, &timerISR, true); // Attach the ISR function
  timerAlarmWrite(timer, 50000, true);          // Set the alarm value (50 ms interval)
  timerAlarmEnable(timer);                      // Enable the timer interrupt

  Serial.begin(115200); // Set the baud rate to 115200
}

void loop()
{
}
