/*Using only semaphores to make a thread safe single instance circular buffer module to store and restore data of type int.

    The Producer thread stores integers in the buffer
    The consumer threads consume the integers (4 consumers)
    The producer and the consumers operate at different rates
    Capacity of the circular buffer is 8 and it can hold max. 8 integers
    If the circular buffer is empty, the consumers shall sleep
    If the circular is full, the producer shall sleep

*/
#include <iostream>    // Standard input/output stream library
#include <thread>      // Thread support library
#include <vector>      // Vector support library
#include <semaphore.h> // Semaphore support library
#include <unistd.h>    // UNIX standard function definitions (usleep)

// Define the size of the circular buffer
#define BUFFER_SIZE 8

// CircularBuffer class to handle buffer operations
class CircularBuffer
{
public:
    CircularBuffer() : head(0), tail(0), count(0) // Contructor: Clean up the semaphores
    {
        sem_init(&emptySlots, 0, BUFFER_SIZE); // Initialize emptySlots semaphore to BUFFER_SIZE
        sem_init(&fullSlots, 0, 0);            // Initialize fullSlots semaphore to 0
        sem_init(&mutex, 0, 1);                // Initialize mutex semaphore
    }

    ~CircularBuffer()
    {
        sem_destroy(&emptySlots); // Destroy the emptySlots semaphore
        sem_destroy(&fullSlots);  // Destroy the fullSlots semaphore
        sem_destroy(&mutex);      // Destroy the mutex semaphore
    }

    // Function to add data to the buffer
    void add(int item)
    {
        sem_wait(&emptySlots); // Wait if there are no empty slots
        sem_wait(&mutex);      // Wait for access to the buffer (critical section)

        buffer[tail] = item;             // Add the item to the buffer
        tail = (tail + 1) % BUFFER_SIZE; // Move tail pointer to next position
        count++;                         // Increment the count of items in the buffer

        sem_post(&mutex);     // Release access to the buffer
        sem_post(&fullSlots); // Signal that there is a new full slot
    }

    // Function to remove data from the buffer
    int remove()
    {
        sem_wait(&fullSlots); // Wait if there are no full slots
        sem_wait(&mutex);     // Wait for access to the buffer (critical section)

        int item = buffer[head];         // Remove the item from the buffer
        head = (head + 1) % BUFFER_SIZE; // Move head pointer to the next position
        count--;                         // Decrement the count of items in the buffer

        sem_post(&mutex);      // Release access to the buffer
        sem_post(&emptySlots); // Signal that there is a new empty slot

        return item; // Return the removed item
    }

private:
    int buffer[BUFFER_SIZE];            // Array to store buffer items
    int head, tail, count;              // Pointers and counter for the buffer
    sem_t emptySlots, fullSlots, mutex; // Semaphores for synchronization
};

void producer(CircularBuffer &cb)
{
    int item = 0;
    while (true)
    {
        item++;       // Produce an item
        cb.add(item); // Add the item to the buffer
        std::cout << "Produced: " << item << std::endl;
        usleep(100000); // Sleep for 100 milliseconds
    }
}

void consumer(CircularBuffer &cb, int id)
{
    while (true)
    {
        int item = cb.remove(); // Remove an item from the buffer
        std::cout << "Consumer " << id << "consumed: " << item << std::endl;
        usleep(200000); // Sleep for 200 milliseconds
    }
}

int main()
{
    CircularBuffer cb; // Create an instance of the circular buffer

    std::thread prodThread(producer, std::ref(cb)); // Create the producer thread

    std::vector<std::thread> consThreads; // Vector to store consumer threads
    for (int i = 0; i < 4; i++)
    {
        consThreads.push_back(std::thread(consumer, std::ref(cb), i + 1)); // Create consumer threads
    }

    prodThread.join(); // Join the producer thread
    for (auto &t : consThreads)
    {
        t.join(); // Join the consumer threads
    }

    return 0;
}