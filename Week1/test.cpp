#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>
#include <chrono>

// Vehicle base class
class Vehicle
{
public:
    Vehicle(int id, std::string model, std::string type) : id_(id), model_(model), type_(type) {}
    virtual void printInfo() const = 0; // Pure virtual function to print vehicle information
protected:
    int id_;            // Unique auto-generated serial number
    std::string model_; // Model of the vehicle
    std::string type_;  // Type of the vehicle (car or truck)
};

// Car class, derived from Vehicle
class Car : public Vehicle
{
public:
    Car(int id, std::string model, int maxPassengers) : Vehicle(id, model, "car"), maxPassengers_(maxPassengers) {}
    void printInfo() const override
    {
        std::cout << "Car ID: " << id_ << ", Model: " << model_ << ", Max Passengers: " << maxPassengers_ << std::endl;
    }

private:
    int maxPassengers_; // Maximum number of passengers the car can carry
};

// Truck class, derived from Vehicle
class Truck : public Vehicle
{
public:
    Truck(int id, std::string model, double maxLoad) : Vehicle(id, model, "truck"), maxLoad_(maxLoad) {}
    void printInfo() const override
    {
        std::cout << "Truck ID: " << id_ << ", Model: " << model_ << ", Max Load: " << maxLoad_ << " tons" << std::endl;
    }

private:
    double maxLoad_; // Maximum weight of load the truck can carry
};

// Circular Buffer class template
template <typename T>
class CircularBuffer
{
public:
    CircularBuffer(size_t capacity) : capacity_(capacity), buffer_(capacity), front_(0), rear_(0), count_(0) {}

    void produce(const T &item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        notFull_.wait(lock, [this]()
                      { return count_ != capacity_; }); // Wait until buffer is not full

        buffer_[rear_] = item;           // Add item to the buffer
        rear_ = (rear_ + 1) % capacity_; // Update rear index
        ++count_;                        // Increment count of elements
        notEmpty_.notify_one();          // Notify consumer that buffer is not empty
    }

    T consume()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        notEmpty_.wait(lock, [this]()
                       { return count_ != 0; }); // Wait until buffer is not empty

        T item = buffer_[front_];          // Remove item from the buffer
        front_ = (front_ + 1) % capacity_; // Update front index
        --count_;                          // Decrement count of elements
        notFull_.notify_one();             // Notify producer that buffer is not full
        return item;
    }

private:
    size_t capacity_;                  // Capacity of the circular buffer
    std::vector<T> buffer_;            // Vector to hold buffer elements
    size_t front_;                     // Index of the front element
    size_t rear_;                      // Index of the rear element
    size_t count_;                     // Number of elements in the buffer
    std::mutex mutex_;                 // Mutex for thread safety
    std::condition_variable notFull_;  // Condition variable for buffer not full
    std::condition_variable notEmpty_; // Condition variable for buffer not empty
};

// Function to generate random model names
std::string generateModelName()
{
    std::vector<std::string> models = {"SUV", "Sedan", "Truck", "Hatchback", "Convertible"};
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, models.size() - 1);
    return models[dist(gen)];
}

// Function to generate random number of passengers for cars
int generateMaxPassengers()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(2, 8); // Random number between 2 and 8
    return dist(gen);
}

// Function to generate random load weight for trucks
double generateMaxLoad()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(1.0, 10.0); // Random number between 1.0 and 10.0
    return dist(gen);
}

// Producer function
void manufacturerFunc(CircularBuffer<std::shared_ptr<Vehicle>> &warehouse, size_t numVehicles)
{
    for (int i = 0; i < numVehicles; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulate manufacturing time
        std::string model = generateModelName();
        if (model == "Truck")
        {
            warehouse.produce(std::make_shared<Truck>(1000 + i, model, generateMaxLoad())); // Create and store a truck in the warehouse
        }
        else
        {
            warehouse.produce(std::make_shared<Car>(1000 + i, model, generateMaxPassengers())); // Create and store a car in the warehouse
        }
        std::cout << "Manufacturer produced a vehicle." << std::endl;
    }
}

// Consumer function
void consumerFunc(CircularBuffer<std::shared_ptr<Vehicle>> &warehouse)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Simulate consumption time
        auto vehicle = warehouse.consume();                           // Retrieve a vehicle from the warehouse
        vehicle->printInfo();                                         // Print information about the retrieved vehicle
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <warehouse_capacity> <num_consumers>" << std::endl;
        return 1;
    }

    size_t capacity = std::stoi(argv[1]);     // Get warehouse capacity from command line argument
    size_t numConsumers = std::stoi(argv[2]); // Get number of consumers from command line argument

    if (capacity <= 7 || numConsumers < 2)
    {
        std::cerr << "Warehouse capacity must be greater than 7 and number of consumers must be at least 2." << std::endl;
        return 1;
    }

    CircularBuffer<std::shared_ptr<Vehicle>> warehouse(capacity); // Create circular buffer with specified capacity

    // Create consumer threads
    std::vector<std::thread> consumers;
    for (size_t i = 0; i < numConsumers; ++i)
    {
        consumers.emplace_back(consumerFunc, std::ref(warehouse));
    }

    // Create producer thread
    std::thread manufacturer(manufacturerFunc, std::ref(warehouse), numConsumers * 5); // Produces 5 vehicles per consumer

    manufacturer.join(); // Wait for producer thread to finish
    for (auto &consumer : consumers)
    {
        consumer.join(); // Wait for each consumer thread to finish
    }

    return 0;
}