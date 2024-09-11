/*FRDM-K64F: Connect an RGB LED to series with three 120Ω resistors to three pwm pins.
Every second make a random color by writing random pwm values to the pins.*/

#include <mbed.h>
#include "rtos/ThisThread.h"

// Define PWM pins for the RGB LED
PwmOut rLed(PTC2);
PwmOut gLed(PTC3);
PwmOut bLed(PTC4);

int main()
{
    // Set the PWM period to 20ms (standard for servos, adjust if needed)
    rLed.period_ms(20);
    gLed.period_ms(20);
    bLed.period_ms(20);

    while (true)
    {
        // Generate random duty cycles for each LED
        float rDuty = rand() % 101 / 100.0f;
        float gDuty = rand() % 101 / 100.0f;
        float bDuty = rand() % 101 / 100.0f;

        // Set the duty cycle for each LED
        rLed.write(rDuty);
        gLed.write(gDuty);
        bLed.write(bDuty);

        // Wait for a second
        ThisThread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}