/*Using Serial make a program for ESP32 to read date and time in the format of YYYY-MM-DD hh:mm:ss from
a terminal and check if the entered date and time matches the format or not.
If the entered date and time format matches print an ok message to the terminal; otherwise print an error message to the terminal.
Enter date and time (YYYY-MM-DD hh:mm:ss): 2023-03-29 12:02:12
The date and time format is valid!

Enter date and time (YYYY-MM-DD hh:mm:ss): 2023-3-29 12:2:12
The date and time format is invalid!

Enter date and time (YYYY-MM-DD hh:mm:ss):*/

#include <Arduino.h>

bool isValidDateTimeFormat(const char *dateTime)
{
  if (strlen(dateTime) != 19)
    return false;

  for (int i = 0; i < 19; i++)
  {
    if (i == 4 || i == 7)
    {
      if (dateTime[i] != '-')
        return false;
    }
    else if (i == 10)
    {
      if (dateTime[i] != ' ')
        return false;
    }
    else if (i == 13 || i == 16)
    {
      if (dateTime[i] != ':')
        return false;
    }
    else
    {
      if (!isdigit(dateTime[i]))
        return false;
    }
  }

  return true;
}

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  int index = 0;
  char input[20];

  while (index < 19)
  {
    if (Serial.available() > 0)
    {
      char c = Serial.read();
      if (c >= '0' && c <= '9' || c == '-' || c == ' ' || c == ':')
      {
        Serial.print(c);
        input[index] = c;
        index++;
      }
      else if (c == '\n')
      {
        break;
      }
    }
  }
  Serial.write("\n");

  if (isValidDateTimeFormat(input))
  {
    Serial.println("The date and time format is valid");
  }
  else
  {
    Serial.println("The date and time format is invalid");
  }
}
