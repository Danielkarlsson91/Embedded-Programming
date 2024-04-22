/*Using TDD and Unity test and develop a function to convert a string to an integer.
To build the test, link the installed unity on your computer to the test.
*/

#include <unity.h>
// Includes the Unity library, which provides functions for writing and running unit tests.

int string_to_int(const char *str)
// Function to convert a string to an integer.
// Takes a C-style string as input and returns an integer.
{
    int sign = 1;
    int result = 0;
    // Initializes the variable 'result' to 0 to accumulate the integer value.

    for (int i = 0; str[i] != '\0'; i++)
    // A loop that iterates through each character in the string until it reaches the end of the string ('\0').
    {
        if (str[0] == '-' && i == 0)
        {
            sign = -1;
            continue;
        }

        else if (str[i] < '0' || str[i] > '9')
        // Checks if the character is outside the range '0' to '9'.
        {
            return 0;
            // If the character is outside the range, the function returns 0, indicating an invalid string.
        }

        result = result * 10 + (str[i] - '0');
        // Multiplies 'result' by 10 and adds the numeric equivalent of the character 'str[i]'.
        // The character code for digits is between '0' and '9', so 'str[i]' - '0' gives the numeric value.
    }

    return result * sign;
}

void test2()
{
    TEST_ASSERT_EQUAL_INT(1, string_to_int("1"));
}

void test_fail()
{
    TEST_ASSERT_EQUAL_INT(0, string_to_int("abc"));
    TEST_ASSERT_EQUAL_INT(0, string_to_int("1bc"));
    TEST_ASSERT_EQUAL_INT(0, string_to_int("ab3"));
}

void test3()
{
    TEST_ASSERT_EQUAL_INT(12, string_to_int("12"));
}

void test_negative_number()
{
    TEST_ASSERT_EQUAL_INT(-5, string_to_int("-5"));
    TEST_ASSERT_EQUAL_INT(-53, string_to_int("-53"));
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_fail);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test_negative_number);

    return UNITY_END();
}