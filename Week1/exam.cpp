/*Implement a thread safe and synchronized producer-consumer program.

The producer is a thread and it manufactures vehicles and store them in a warehouse.

Every vehicle has the following properties:

    ID : a unique auto-generated serial number greater than 1000

    Model: to specify model of vehicles

    Type : car or truck

Every car has a property to specify the max. number of passengers.
Every truck has a property to specify the max. weight of load.

Vehicle, Car and Truck are classes to model the properties and functions to print the properties.

The warehouse is an uncopyable circular buffer class implemented using an array whose capacity is
specified by a macro in the command line when you compile the program. The capacity shall be greater than 7.

A consumer is a thread. When a consumer gets a vehicle from the warehouse, it shall print all the properties of the vehicle to the terminal.
Number of consumers shall be at least 2 and it is specified as an argument when the program is run in the terminal.

The producer manufactures cars and trucks randomly and stores them into the warehouse.
When the warehouse is full the manufacturing thread shall sleep.

A consumer gets a vehicle at a time from the warehouse and prints all the properties of the vehicle to the terminal.
When the warehouse is empty the consuming threads shall sleep until a vehicle is manufactured by the manufacturer thread
and stored in the warehouse.

The manufacturer and the consumers operate at different rates.
It means that the program shall work correctly in both of the following cases

    The manufacturer is faster than the consumers
    The consumers are faster than the manufacturer

To get G(godkänd), all the above requirements shall be fulfilled.
To get VG(väl godkänd), all the above requirements shall be fulfilled and the classes shall be well-designed.*/

#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <ctime>
#include <queue>

#define capacity 8 // Define a constant capacity value as 8

// Base class for vehicles
class Vehicle
{
protected:
    int id;            // Unique identifier for the vehicle
    static int serial; // Static variable to keep track of vehicle ID
    std::string model; // Model of the vehicle
    std::string type;  // Type of the vehicle(Car or truck)

public:
    Vehicle(const std::string &model, const std::string &type) : model(model), type(type) // Contructor to initialize vehicles properties
    {
        id = ++serial; // Increment the serial number and assign it as the vehicle ID
    }

    virtual void printProperties() const // Virtual function to print vehicle properties (to be overridden by dervied classes)
    {
        std::cout << "ID: " << id << ", Model: " << model << ", Type: " << type << std::endl; // Output vehicle properties
    }
};

class Truck : public Vehicle // Derived class Truck inheriting from base class Vehicle
{
private:
    int maximum_loadWeight; // Maximum load weight of the truck

public:
    Truck(const std::string &model, int maximum_loadWeight) : Vehicle(model, "truck"), maximum_loadWeight(maximum_loadWeight) {} // Constructor to initialize truck properties

    void printProperties() const override
    {
        std::cout << "ID: " << id << "\nModel: " << model << "\nType: Truck\nMax Load Weight: " << maximum_loadWeight << "\n"
                  << std::endl; // Output car properties
    }
};

class Car : public Vehicle // Derived class Car inheriting from base class Vehicle
{
private:
    int maximum_of_passengers; // Maximum number of passengers the car can carry

public:
    Car(const std::string &model, int maximum_of_passengers) : Vehicle(model, "car"), maximum_of_passengers(maximum_of_passengers) {} // Constructor to initialize car properties

    void printProperties() const override // Override function to print car properties
    {
        std::cout << "ID: " << id << "\nModel: " << model << "\nType: Car\nMax Passengers: " << maximum_of_passengers << "\n"
                  << std::endl;
    }
};

// Static mutex for output stream to prevent mixed output from multiple threads
static std::mutex omtx;

// Circular buffer warehouse class to manage a fixed-size buffer of vehicles
class CircularBufferWarehouse
{
    int counter{0};                                       // Counter to track the number of elements in the buffer
    int head{0};                                          // Index of the head of the buffer
    int tail{0};                                          // Index of the tail of the buffer
    std::array<std::shared_ptr<Vehicle>, capacity> array; // Array to hold elements
    std::mutex mtx;                                       // Mutex for thread safety
    std::condition_variable cv;                           // Condition variable for synchronization
    size_t cap{capacity};                                 // Capacity of the buffer

public:
    // Default contructor
    CircularBufferWarehouse() {}

    // Deleted copy constructor and assignment operator to make the class uncopyable
    CircularBufferWarehouse(const CircularBufferWarehouse &) = delete;
    CircularBufferWarehouse &operator=(const CircularBufferWarehouse &) = delete;

    // Function to push a vehicle into the buffer
    void push(const std::shared_ptr<Vehicle> &item)
    {
        // Acquire the lock using unique_lock
        std::unique_lock lock{mtx};
        // Wait until the buffer is not full
        cv.wait(lock, [this]
                { return counter < cap; });

        counter++;          // Increment the top index
        array[tail] = item; // Push the item
        tail++;

        // If tail reaches the end of the buffer, reset it to 0
        if (tail == cap)
        {
            tail = 0;
        }

        lock.unlock();   // Unlock the mutex
        cv.notify_all(); // Notify all waiting threads
    }

    std::shared_ptr<Vehicle> pop() // Function to pop a vehicle from the buffer
    {
        std::unique_lock lock{mtx}; // Acquire the lock using unique_lock
        cv.wait(lock, [this]
                { return counter > 0; }); // Wait until it's not empty

        std::shared_ptr<Vehicle> item{array[head]}; // Retrieve the top element from the buffer

        counter--; // Decrement the counter
        head++;    // Increment the head index

        // If head reaches the end of the buffer, reset it to 0
        if (head == cap)
        {
            head = 0;
        }

        lock.unlock();   // Unlock the mutex
        cv.notify_all(); // Notify all waiting threads

        return item; // Return the popped item
    }
};

// Function for consumer threads to pop vehicles from the warehouse
void consumers(CircularBufferWarehouse &warehouse)
{
    static std::mutex mtx; // Mutex for thread safety in consumer function
    while (true)
    {
        auto vehicle{warehouse.pop()};                                // Pop a vehicle from the warehouse
        mtx.lock();                                                   // Lock the mutex to print vehicle properties
        vehicle->printProperties();                                   // Print the properties of the popped vehicle
        mtx.unlock();                                                 // Unlock the mutex
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Sleep for 1000 milliseconds to simulate processing time
    }
}

int Vehicle::serial{1000}; // Initialize the serial number for vehicles

int main()
{
    int value;
    constexpr int CONSUMERS{8};        // Number of consumer threads
    CircularBufferWarehouse warehouse; // Create a CircularBufferWarehouse object

    std::array<std::thread, CONSUMERS> cthreads; // Array to hold consumer threads
    std::srand(time(nullptr));                   // Seed the random number generator with the current time

    // Create consumer threads
    for (int i = 0; i < CONSUMERS; i++)
    {
        cthreads[i] = std::thread(consumers, std::ref(warehouse));
    }

    // Producer loop to continuosly push vehicles into the warehouse
    while (true)
    {
        // Randomly push either a car or a truck into the warehouse
        if (std::rand() % 2 == 0)
        {
            warehouse.push(std::make_shared<Car>("Golf", 5));
        }

        else
        {
            warehouse.push(std::make_shared<Truck>("Vera", 1000));
        }

        // Sleep for 1000 milliseconds before pushing the next vehicle
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Producer
    }

    // Join all consumer threads
    for (int i = 0; i < CONSUMERS; i++)
    {
        cthreads[i].join();
    }

    return 0;
}