/*FRDM-K64F: Connect a potentiometer to an analog pin and a LED series with a 120Ω to a pwm pin.
Look at the image. Control brightness of the LED using the potentiometer.*/

#include <mbed.h> // Include the Mbed library for microcontroller functionalities

int main()
{
    // Declare an AnalogIn object 'pot' connected to pin A0 with a reference voltage of 3.3V
    AnalogIn pot{A0, 3.3f};
    // Declare a PwmOut object 'led' connected to pin PTC11
    PwmOut led{PTC11};

    // Set the PWM period for the LED to 5 millseconds
    led.period_ms(1);

    while (true)
    {
        // Read the analog value from the potentiometer and write it as the PWM value to the LED
        // 'pot.read()' returns a float value between 0 and 1 based on the input voltage (0V to 3.3V)
        // 'led.write()' expects a float value between 0 and 1 to set the duty cycle of the PWM
        led.write(pot.read());

        // Wait for 1 millisecond before repeating the loop to avoid unnecessary CPU usage
        ThisThread::sleep_for(std::chrono::milliseconds(1));
    }

    // Return 0 to indicate successful program execution (not necessary in some enviroments)
    return 0;
}