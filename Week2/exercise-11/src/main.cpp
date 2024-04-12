/*ESP32: Make a menu with two options:

G) To get the current internal RTC date and time and print it to the terminal in the format of YYYY-MM-DD hh:mm:ss.

S) To read date and time in the format of YYYY-MM-DD hh:mm:ss from the terminal and initialize the internal RTC.*/

#include <Arduino.h>
#include <time.h>

void setTime(const char *input);
String getTime();

void setup()
{
  Serial.begin(115200); // Initialize serial communication
  delay(2000);          // Wait for serial monitor to open
  Serial.println("Enter the current date and time (YYYY-MM-DD hh:mm:ss): ");

  while (!Serial.available());                  // Wait for user input
  String input = Serial.readStringUntil('\n'); // Read the user input
  setTime(input.c_str());                      // Set the internal RTC time
}

void loop()
{
  // Get current date and time from the internal RTC
  String currentTime = getTime();
  Serial.println("Current date and time: " + currentTime);
  delay(1000); // Wait for 1 second
}

void setTime(const char* input)
{
  int year, month, day, hour, minute, second;
  sscanf(input, "%d-%d-%d %d:%d:%d:%d", &year, &month, &day, &hour, &minute, &second);

  struct tm tmStruct;
  tmStruct.tm_year = year - 1900; // Years since 1900
  tmStruct.tm_mon = month - 1;    // Month (0-11)
  tmStruct.tm_mday = day;         // Day of month (1 - 31)
  tmStruct.tm_hour = hour;        // Hours (0 - 23)
  tmStruct.tm_min = minute;       // Minutes (0 - 59)
  tmStruct.tm_sec = second;       // Seconds (0 - 59)
  time_t t = mktime(&tmStruct);   // Convert struct tm to time_t            
}

String getTime()
{
  struct tm timeinfo;
  time_t now;
  time(&now);
  localtime_r(&now, &timeinfo);

  char currentTime[20];
  strftime(currentTime, sizeof(currentTime), "%Y-%m-%d %H:%M:%S", &timeinfo); // Format date and time
  return String(currentTime);
}
