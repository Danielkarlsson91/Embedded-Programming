/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of bouncing. Connect a pushbutton to pin 34. Look at image.png.
 *
 * @version 0.1
 * @date 2023-04-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>

static uint32_t count = 0;
static uint32_t previous_count = 0;
static uint8_t previous_state = HIGH;

void setup()
{
    pinMode(GPIO_NUM_34, INPUT_PULLUP);
    Serial.begin(115200);
    delay(2000);
}

void loop()
{
    uint8_t current_state = digitalRead(GPIO_NUM_34);

    if (current_state != previous_state)
    {
        if (current_state == LOW)
        {
            count++;
        }
        previous_state = current_state;
    }
    else if (count != previous_count)
    {
        Serial.printf("count: %d\n", count);
        previous_count = count;
    }
    else
    {
        ;
    }
}