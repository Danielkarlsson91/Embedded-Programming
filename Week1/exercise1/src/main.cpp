/*Using BufferedSerial make a program for FRDM-K64F to read a string of max. 20 lowercase letters from terminal
and then reverse and convert the string to uppercase and
print it to the terminal. The read string from terminal shall only contain a to z.*/

#include <mbed.h> // Include mbed library for using mbed features

#define MAX_LENGTH 20 // Define the maximum length of the input string

static BufferedSerial serial{USBTX, USBRX, 9600}; // Initialize BufferedSerial for communication

int main()
{
    char input[MAX_LENGTH + 1]; // Define an array to store the input string

    int index = 0; // Initialize an index variable to keep track of the position in the input array

    while (1) // Infinite loop for continuous operation
    {
        serial.write("Enter a string of max 20 lowercase letters (a-z):\n", 50); // Prompt the user to enter a string

        while (index < MAX_LENGTH) // Loop to read characters until the maximum length is reached
        {
            char c;
            serial.read(&c, 1); // Read a character from the terminal

            if (c >= 'a' && c <= 'z') // Check if the character is a lowercase letter
            {
                serial.write(&c, 1);       // Echo the character back to the terminal
                input[index] = toupper(c); // Convert the character to uppercase and store it in the input array
                index++;                   // Move to the next position in the input array
            }
            else if (c == '\n') // Check if Enter key is pressed
            {
                break; // Exit the loop if Enter key is pressed
            }
        }
        serial.write("\n", 1); // Send newline character to the terminal to move to the next line

        input[index] = '\0'; // Terminate the input string with a null character

        serial.write("Reversed and converted: ", 25 - 1); // Send message indicating the reversed and converted string

        for (int i = 0; i < strlen(input) / 2; i++) // Loop to reverse the input string
        {
            swap(input[0 + i], input[strlen(input) - 1 - i]); // Swap characters at the beginning and end of the string
        }
        serial.write(input, index); // Send the reversed and converted string to the terminal
        serial.write("\n", 1);      // Send newline character to the terminal to move to the next line
    }

    return 0; // End of program
}