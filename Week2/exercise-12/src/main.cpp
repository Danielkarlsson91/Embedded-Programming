/*FRDM-K64F: Make a menu with two options:
G) To get the current internal RTC date and time and print it to the terminal in the format of YYYY-MM-DD hh:mm:ss.
S) To read date and time in the format of YYYY-MM-DD hh:mm:ss from the terminal and initialize the internal RTC.*/

#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

// Function prototypes
void setCurrentTime();
void getCurrentTime();

int main()
{
    char choice;

    while (true)
    {
        // Display the menu
        std::cout << "S) Set date and time: " << std::endl;
        std::cout << "G) Get date and time: " << std::endl;
        std::cout << "Enter a command: ";

        std::cin >> choice; // Get user's choice

        switch (choice)
        {
        case 'S':
            setCurrentTime(); // Call function to set the date and time
            break;
        case 'G':
            getCurrentTime(); // Call function to get date and time
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            break;
        }
    }

    return 0;
}
// Function to set the current date and time
void setCurrentTime()
{
    std::string dateTime;

    // Prompt the user to enter date and time
    std::cout << "Enter the current date and time (YYYY-MM-DD hh:mm:ss): ";
    std::cin.ignore(); // Ignore any previous newline character
    std::getline(std::cin, dateTime);

    // Convert string to time struct
    std::tm tm = {};                               // Initialize tm struct with zeros
    std::istringstream ss(dateTime);               // Create a string stream from user input
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S"); // Parse date and time from string

    // Check if parsing was successful
    if (ss.fail())
    {
        std::cerr << "Error parsing date and time" << std::endl;
        return;
    }

    // Set system time
    time_t t = std::mktime(&tm);
    if (t == -1) // Check if mktime failed
    {
        std::cerr << "Error setting system time!" << std::endl;
        return;
    }

    std::cout << "Time has been set successfully." << std ::endl;
}
// Get current time
void getCurrentTime()
{
    // Get current time
    std::time_t now = std::time(nullptr);

    // Format and print current time
    std::tm *current_time = std::localtime(&now);                             // Get local time
    char buffer[80];                                                          // Buffer to hold formatted time string
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", current_time); // Format time
    std::cout << "Current date and time: " << buffer << std::endl;            // Print formatted time
}