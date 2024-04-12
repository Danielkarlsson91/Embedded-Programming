/*Implement a thread-safe uncopyable template stack using an array.
When a stack is empty, if a thread tries to pop it shall be suspended.
When a stack is full, if a thread tries to push it shall be suspended.

A piece of code is thread-safe if it functions correctly during simultaneous execution by multiple threads.
In particular, it must satisfy the need for multiple threads to access the same shared data
and the shared data shall be accessed by only one thread at any given time.
*/

#include <ctime>
#include <mutex>
#include <iostream>
#include <thread>
#include <condition_variable>

// Static mutex for output stream
static std::mutex omtx;

// Template class for a thread-safe stack
template <typename T, size_t SIZE>
class Stack
{
    int top{-1};                // Index of the top element (initialized to -1 as the stack is initially empty)
    T array[SIZE];              // Array to hold stack elements
    std::mutex mtx;             // Mutex for thread safety
    std::condition_variable cv; // Condition variable for synchronization

    // Helper function to check if the stack is empty
    bool isempty() { return (top == -1); }

    // Helper function to check if the stack is full
    bool isfull() { return ((top + 1) == SIZE); }

public:
    using Type = T;
    static constexpr size_t Size(SIZE); // Static member function to get the size of the stack

    // Deleted copy constructor and assignment operator to make the class uncopyable
    Stack(const Stack &) = delete;
    Stack &operator=(const Stack &) = delete;

    // Constructor
    Stack()
    {
        static_assert(SIZE > 3, "SIZE shall be at least 4"); // Ensure SIZE is at least 4
    }

    // Push operation to add an item to the stack
    void push(const T &item)
    {
        std::unique_lock lock(mtx);
        cv.wait(lock, [this]
                { return !this->isfull(); }); // Wait until stack is not full
        top++;
        array[top] = item; // Add item to the stack
        lock.unlock();
        cv.notify_all(); // Notify waiting threads
    }

    // Pop operation to remove and return the top item from the stack
    T pop()
    {
        std::unique_lock lock(mtx);
        cv.wait(lock, [this]
                { return !this->isempty(); }); // Wait until stack is not empty
        T item{array[top]};                    // Get top item from the stack
        top--;                                 // Decrement top index
        lock.unlock();
        cv.notify_all(); // Notify waiting threads
        return item;     // Return the popped item
    }
};

// Consumer function that pops items from the stack and prints them
template <typename T, size_t SIZE>
static void consumer(Stack<T, SIZE> &stack)
{
    T value;
    while (1)
    {
        value = stack.pop(); // Pop item from the stack
        // Output the popped item along with the thread ID
        omtx.lock();
        std::cout << "[" << std::this_thread::get_id() << "] Consumed: " << value << std::endl;
        omtx.unlock();
        // Simulate work by sleeping for a random duration
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + (rand() % 100)));
    }
}

// Main function
int main()
{
    int value;
    constexpr int CONSUMERS{5};                  // Number of consumer threads
    Stack<int, 8> stack;                         // Create a stack of integers with capacity 8
    std::array<std::thread, CONSUMERS> cthreads; // Array to hold consumer threads

    std::srand(time(nullptr)); // Seed the random number generator with the current time

    // Create consumer threads
    for (int i = 0; i < CONSUMERS; i++)
    {
        // Each consumer thread runs the consumer function with the stack reference
        cthreads[i] = std::thread{consumer<decltype(stack)::Type, decltype(stack)::Size>, std::ref(stack)};
        cthreads[i].detach(); // Detach the thread to allow it to run independently
    }

    // Producer loop
    while (1)
    {
        value++;                                         // Increment value
        omtx.lock();                                     // Lock the output stream mutex
        stack.push(value);                               // Push value onto the stack
        std::cout << "Produced: " << value << std::endl; // Output the produced value
        omtx.unlock();                                   // Unlock the output stream mutex
        // Simulate work by sleeping for a random duration
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + (rand() % 400)));
    }

    return 0; // Return success
}
