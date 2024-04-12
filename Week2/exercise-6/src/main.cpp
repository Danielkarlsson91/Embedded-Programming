/*ESP32: Make a class named Metro to run a task intervally using millis.
The class shall have a function to check if the interval has passed or not.
This function is called in the loop function.

Make a program to run two tasks by creating two instances of the Metro class.
The first task: toggle a led series with a 120Ω resistor connected to digital pin(21) every 500 ms
The second task: every 1000 ms print hello world to the terminal.*/

#include <Arduino.h>

// Class for managing tasks with intervals
class Metro
{
public:
  // Constructor takes the interval in milliseconds
  Metro(unsigned long _interval) : interval(_interval), previousMillis(0)
  {
  }

  // Check if the interval has passed since the last call
  bool check()
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      return true; // Return true if the interval has passed
    }
    return false; // Return false otherwise
  }

private:
  unsigned long interval;       // Interval between task executions
  unsigned long previousMillis; // Time of the last task execution
};

// Define the LED pin number
#define LED_PIN 21

// Define the time intervals for tasks
constexpr unsigned long LED_INTERVAL = 500;
constexpr unsigned long PRINT_INTERVAL = 1000;

// Create instances of the Metro class for each task
Metro ledMetro(LED_INTERVAL);     // For toggling the LED
Metro printMetro(PRINT_INTERVAL); // For printing "Hello World"

void setup()
{
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  // Start serial communication
  Serial.begin(115200);
}

void loop()
{
  // Check if it's time to toggle the LED
  if (ledMetro.check())
  {
    // Toggle the LED
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }

  // Check if it's time to print "Hello World"
  if (printMetro.check())
  {
    // Print "Hello World" to the serial terminal
    Serial.println("Hello World");
  }
}
