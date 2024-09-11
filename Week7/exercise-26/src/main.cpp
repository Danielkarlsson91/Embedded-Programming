/*Connect a push button to pin 34 of ESP32 and debounce the button using an interrupt.
Assume that the uncertainty time of the button is 250 ms.
When the button is pressed, on the falling edges print “Falling edge” to the terminal
*/

#include <Arduino.h> // Include the Arduino core library

// Declare a volatile static boolean variable 'falling_edge' and initialize it to false.
// 'volatile' tells the compiler that the value of this variable may change at any time,
// preventing certain optimizations that might assume its value remains constant between accesses.
volatile static bool falling_edge = false;

// Interrupt Service Routine (ISR) for the button interrupt
void isr_button(void)
{
  // Disable the interrupt for GPIO pin 34 to prevent re-entry while processing
  gpio_intr_disable(GPIO_NUM_34);

  // Static variable to store the time of the last interrupt in milliseconds
  static uint32_t prev_ms = 0;

  // Get the current time in milliseconds since the program started
  uint32_t now = millis();

  // Check if at least 250 milliseconds have passed since the last interrupt
  if (now - prev_ms > 250)
  {
    // Update the time of the last interrupt
    prev_ms = now;
    // Set the 'falling_edge' flag to true to indicate a falling edge was detected
    falling_edge = true;
  }

  // Re-enable the interrupt for GPIO pin 34
  gpio_intr_enable(GPIO_NUM_34);
}

// Setup function runs once at the beginning
void setup()
{
  // Set GPIO pin 34 as an input with an internal pull-up resistor
  pinMode(GPIO_NUM_34, INPUT_PULLUP);
  // Initialize serial communication at a baud rate of 115200
  Serial.begin(115200);

  // Attach the interrupt service routine 'isr_button' to GPIO pin 34
  // The interrupt is triggered on a falling edge (high-to-low transition)
  attachInterrupt(digitalPinToInterrupt(GPIO_NUM_34), isr_button, FALLING);
}

// Main loop function runs repeatedly
void loop()
{
  // Temporarily disable all interrupts to safely access shared variables
  noInterrupts();
  // Check if a falling edge was detected by the ISR
  if (falling_edge)
  {
    // Reset the 'falling_edge' flag to false
    falling_edge = false;
    // Print "Falling Edge" to the serial monitor
    Serial.println("Falling Edge");
  }
  // Re-enable all interrupts after accessing shared variables
  interrupts();
}
