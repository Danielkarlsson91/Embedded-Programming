/*ESP32: Toggle a led series with a 120Ω resistor connected to a digital pin(21)  every 500 ms

For timing, use the first timer interrupt.*/

#include <Arduino.h>

// Define the pin number connected to the LED
#define LED_PIN 21

// Define the time interval for toggling the LED state (in millliseconds)
constexpr uint32_t INTERVAL{500};

// Define the LED state variable
volatile bool ledState = LOW;

// Timer ISR to toggle LED state
void IRAM_ATTR timerISR()
{
  // Toggle the LED state
  ledState = !ledState;

  // Write the new state to the LED pin
  digitalWrite(LED_PIN, ledState);
}

void setup()
{
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  // Configure and start the timer interrupt
  hw_timer_t *timer{timerBegin(0, 80, true)};    // Timer 0, prescaler 80, counting up
  timerAttachInterrupt(timer, &timerISR, true);  // Attach the ISR function
  timerAlarmWrite(timer, INTERVAL * 1000, true); // Set the alarm value
  timerAlarmEnable(timer);                       // Enable the timer interrupt
}

void loop()
{
  delay(1000);
}
