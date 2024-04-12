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

#define capacity 8

class Warehouse
{
private:
    std::mutex mtx;
    std::queue<Vehicle *> buffer;
    int _capacity;
    int count;
    std::condition_variable warehouseEmpty;
    std::condition_variable warehouseFull;
};

class Vehicle
{
protected:
    int id;
    std::string model;
    std::string type;

public:
    Vehicle(int id, const std::string &model, const std::string &type) : id(id), model(model), type(type) {}

    virtual void printProperties()
    {
        std::cout << "ID: " << id << ", Model: " << model << ", Type: " << type << std::endl;
    }
};

class Truck : public Vehicle
{
private:
    int maximum_loadWeight;

public:
    Truck(int id, const std::string &model, int maximum_loadWeight) : Vehicle(id, model, "truck"), maximum_loadWeight(maximum_loadWeight) {}
};

class Car : public Vehicle
{
private:
    int maximum_of_passengers;

public:
    Car(int id, const std::string &model, int maximum_of_passengers) : Vehicle(id, model, "car"), maximum_of_passengers(maximum_of_passengers) {}

    void printProperties() const override
    {
        std::cout << "ID: " << id << ", Model: " << model << ", Type: Car, Max Passengers: " << maximum_of_passengers << std::endl;
    }
};

// Static mutex for output stream
static std::mutex omtx;

// Template class for thread-safe
template <typename T, size_t Capacity>
class CircularBufferWarehouse
{
    int top[-1];                // Index of the top element(initialized to -1 as it's initially empty)
    T array[Capacity];          // Array to hold elements
    std::mutex mtx;             // Mutex for thread safety
    std::condition_variable cv; // Condition variable for synchronization

    bool isempty() { return ((top + 1) == Capacity); }

    bool isfull() { return ((top + 1) == Capacity); }

public:
    using Type = T;
    static constexpr size_t cap(Capacity); // Static member function to get the size

    // Deleted copy constructor and assignment operator to make the class uncopyable
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;

    void produce(const T &item)
    {
        std::unique_lock<std::mutex> lock(mtx);
        notFull.wait(lock, [this]()
                     { return count != Capacity; }); // Wait until buffer is not full
    }
    // Consumer function that pops items and prints them
    template <typename T, size_t SIZE>
    static void consumer(Stack<T, SIZE> &stack)
    {
        std::unique_lock<std::mutex> lock(mtx);
        notEmpty.wait(lock, [this]()
                      { return count != 0; });
        T value;
        while (true)
        {
        }
    }
};

int main()
{
    int value;
    constexpr int CONSUMERS{8}; // Number of consumer threads

    std::array<std::thread, CONSUMERS> cthreads; // Array to hold consumer threads
    std::srand(time(nullptr));                   // Seed the random number generator with the current time

    // Create consumer threads
    for (int i = 0; i < CONSUMERS; i++)
    {
    }

    while (true)
    {
        value++;
        omtx.lock();
        std::cout << "Produced: " << value << std::endl;
        omtx.unlock();
    }

    return 0;
}