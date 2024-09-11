/*FRDM-K64F: Connect an LDR to an analog pin and a LED series with a 120Ω to a digital pin.
Look at the image.
Turn the LED on if the ambient light intensity is lesser than 10 lux(1.0 ftc)*/

#include <Arduino.h>

// Define the pin numbers
const int LDR_PIN = AO; // Analog pin for LDR
const int LED_PIN = 8;  // Digital pin for LED

void setup()
{ // Initialize serial communication
    Serial.begin(9600);

    // Set the LED pin as an OUTPUT
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{                                       // Read the analog value from the LDR
    int ldrValue = analogRead(LDR_PIN); // Define the pin numbers

    // Convert analog value to lux (assuming a linear relationship)
    // You may need to adjust this conversion based on the characteristics
    float lux = map(ldrValue, 0, 1023, 0, 100);

    // Print the light intensity in lux
    Serial.print("Light Intensity (lux): ");
    Serial.println(lux);

    // Check if light intensity is lesser than 10 lux
    if (lux < 10.0)
    {
        // Turn on the LED
        digitalWrite(LED_PIN, HIGH);
    }
    else
    {
        // Turn off the LED
        digitalWrite(LED_PIN, LOW);
    }

    // Delay for some before reading the LDR again
    delay(1000); // Adjust delay as needed
}