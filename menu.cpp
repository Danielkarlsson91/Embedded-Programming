// A list of different kind programs 

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <vector>
#include <stdexcept>

// Circular buffer class
template <typename T>
class CircularBuffer
{
public:
    explicit CircularBuffer(size_t size) : buffer(size), max_size(size), count(0), head(0), tail(0) {}

    // Push element into buffer
    void push(const T &item)
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_full.wait(lock, [this]
                      { return count != max_size; });
        buffer[tail] = item;
        tail = (tail + 1) % max_size;
        ++count;
        not_empty.notify_one();
    }

    // Pop element from buffer
    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_empty.wait(lock, [this]
                       { return count != 0; });
        T item = buffer[head];
        head = (head + 1) % max_size;
        --count;
        not_full.notify_one();
        return item;
    }

private:
    std::vector<T> buffer;
    size_t max_size;
    size_t count;
    size_t head;
    size_t tail;
    std::mutex mutex;
    std::condition_variable not_full;
    std::condition_variable not_empty;
};

// Worker thread function
void worker_func(std::shared_ptr<CircularBuffer<int>> buffer)
{
    try
    {
        for (int i = 0; i < 10; ++i)
        {
            buffer->push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in worker thread: " << e.what() << std::endl;
    }
}

int main()
{
    // Create circular buffer with capacity 5
    auto buffer = std::make_shared<CircularBuffer<int>>(5);

    // Create a worker thread
    std::thread worker(worker_func, buffer);

    // Read from circular buffer
    for (int i = 0; i < 10; ++i)
    {
        int item = buffer->pop();
        std::cout << "Item popped: " << item << std::endl;
    }

    // Join worker thread
    worker.join();

    return 0;
}



#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <vector>
#include <stdexcept>

// Base class
class Shape
{
public:
    virtual void draw() const = 0;
    virtual ~Shape() {}
};

// Derived class
class Circle : public Shape
{
public:
    void draw() const override
    {
        std::cout << "Drawing Circle" << std::endl;
    }
};

// Circular buffer class
template <typename T>
class CircularBuffer
{
public:
    explicit CircularBuffer(size_t size) : buffer(size), max_size(size), count(0), head(0), tail(0) {}

    // Push element into buffer
    void push(const T &item)
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_full.wait(lock, [this]
                      { return count != max_size; });
        buffer[tail] = item;
        tail = (tail + 1) % max_size;
        ++count;
        not_empty.notify_one();
    }

    // Pop element from buffer
    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_empty.wait(lock, [this]
                       { return count != 0; });
        T item = buffer[head];
        head = (head + 1) % max_size;
        --count;
        not_full.notify_one();
        return item;
    }

private:
    std::vector<T> buffer;
    size_t max_size;
    size_t count;
    size_t head;
    size_t tail;
    std::mutex mutex;
    std::condition_variable not_full;
    std::condition_variable not_empty;
};

// Worker thread function
void worker_func(std::shared_ptr<CircularBuffer<std::shared_ptr<Shape>>> buffer)
{
    try
    {
        for (int i = 0; i < 5; ++i)
        {
            std::shared_ptr<Shape> shape = std::make_shared<Circle>();
            buffer->push(shape);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in worker thread: " << e.what() << std::endl;
    }
}

int main()
{
    // Create circular buffer with capacity 5
    auto buffer = std::make_shared<CircularBuffer<std::shared_ptr<Shape>>>(5);

    // Create a worker thread
    std::thread worker(worker_func, buffer);

    // Read from circular buffer
    for (int i = 0; i < 5; ++i)
    {
        std::shared_ptr<Shape> shape = buffer->pop();
        shape->draw();
    }

    // Join worker thread
    worker.join();

    return 0;
}



#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <vector>
#include <stdexcept>

// Base class
class Shape
{
public:
    virtual void draw() const = 0;
    virtual ~Shape() {}
};

// Derived class
class Circle : public Shape
{
public:
    void draw() const override
    {
        std::cout << "Drawing Circle" << std::endl;
    }
};

// Circular buffer class
template <typename T>
class CircularBuffer
{
public:
    explicit CircularBuffer(size_t size) : buffer(size), max_size(size), count(0), head(0), tail(0) {}

    // Push element into buffer
    void push(const T &item)
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_full.wait(lock, [this]
                      { return count != max_size; });
        buffer[tail] = item;
        tail = (tail + 1) % max_size;
        ++count;
        not_empty.notify_one();
    }

    // Pop element from buffer
    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_empty.wait(lock, [this]
                       { return count != 0; });
        T item = buffer[head];
        head = (head + 1) % max_size;
        --count;
        not_full.notify_one();
        return item;
    }

private:
    std::vector<T> buffer;
    size_t max_size;
    size_t count;
    size_t head;
    size_t tail;
    std::mutex mutex;
    std::condition_variable not_full;
    std::condition_variable not_empty;
};

// Worker thread function
void worker_func(std::shared_ptr<CircularBuffer<std::shared_ptr<Shape>>> buffer)
{
    try
    {
        for (int i = 0; i < 5; ++i)
        {
            std::shared_ptr<Shape> shape = std::make_shared<Circle>();
            buffer->push(shape);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in worker thread: " << e.what() << std::endl;
    }
}

int main()
{
    // Create circular buffer with capacity 5
    auto buffer = std::make_shared<CircularBuffer<std::shared_ptr<Shape>>>(5);

    // Create a worker thread
    std::thread worker(worker_func, buffer);

    // Read from circular buffer
    for (int i = 0; i < 5; ++i)
    {
        std::shared_ptr<Shape> shape = buffer->pop();
        shape->draw();
    }

    // Join worker thread
    worker.join();

    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <vector>
#include <stdexcept>

// Base class for shapes
class Shape
{
public:
    virtual void draw() const = 0;
    virtual ~Shape() {}
};

// Derived class representing a circle
class Circle : public Shape
{
public:
    void draw() const override
    {
        std::cout << "Drawing Circle" << std::endl;
    }
};

// Circular buffer class template
template <typename T>
class CircularBuffer
{
public:
    explicit CircularBuffer(size_t size) : buffer(size), maxSize(size), count(0), head(0), tail(0) {}

    // Push an element into the circular buffer
    void push(const T &item)
    {
        std::unique_lock<std::mutex> lock(mutex);
        notFull.wait(lock, [this]
                     { return count != maxSize; });
        buffer[tail] = item;
        tail = (tail + 1) % maxSize;
        ++count;
        notEmpty.notify_one();
    }

    // Pop an element from the circular buffer
    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        notEmpty.wait(lock, [this]
                      { return count != 0; });
        T item = buffer[head];
        head = (head + 1) % maxSize;
        --count;
        notFull.notify_one();
        return item;
    }

private:
    std::vector<T> buffer;
    size_t maxSize;
    size_t count;
    size_t head;
    size_t tail;
    std::mutex mutex;
    std::condition_variable notFull;
    std::condition_variable notEmpty;
};

// Worker function for the thread
void workerFunc(std::shared_ptr<CircularBuffer<std::shared_ptr<Shape>>> buffer)
{
    try
    {
        for (int i = 0; i < 5; ++i)
        {
            std::shared_ptr<Shape> shape = std::make_shared<Circle>();
            buffer->push(shape);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in worker thread: " << e.what() << std::endl;
    }
}

int main()
{
    // Create a circular buffer with capacity 5
    auto buffer = std::make_shared<CircularBuffer<std::shared_ptr<Shape>>>(5);

    // Create a worker thread
    std::thread worker(workerFunc, buffer);

    // Read from the circular buffer
    for (int i = 0; i < 5; ++i)
    {
        std::shared_ptr<Shape> shape = buffer->pop();
        shape->draw();
    }

    // Join the worker thread
    worker.join();

    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <stdexcept>
#include <vector>

// Circular Buffer class template
template <typename T>
class CircularBuffer
{
private:
    std::vector<T> buffer;            // Vector to hold buffer elements
    size_t capacity;                  // Capacity of the buffer
    size_t front;                     // Index of the front element
    size_t rear;                      // Index of the rear element
    size_t count;                     // Number of elements in the buffer
    std::mutex mtx;                   // Mutex for thread safety
    std::condition_variable notFull;  // Condition variable for buffer not full
    std::condition_variable notEmpty; // Condition variable for buffer not empty

public:
    // Constructor to initialize the circular buffer with given capacity
    CircularBuffer(size_t size) : buffer(size), capacity(size), front(0), rear(0), count(0) {}

    // Producer method to add an item to the buffer
    void produce(const T &item)
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            notFull.wait(lock, [this]()
                         { return count != capacity; }); // Wait until buffer is not full
            buffer[rear] = item;                         // Add item to the buffer
            rear = (rear + 1) % capacity;                // Update rear index
            ++count;                                     // Increment count of elements
        }
        notEmpty.notify_one(); // Notify consumer that buffer is not empty
    }

    // Consumer method to remove an item from the buffer
    T consume()
    {
        T item;
        {
            std::unique_lock<std::mutex> lock(mtx);
            notEmpty.wait(lock, [this]()
                          { return count != 0; }); // Wait until buffer is not empty
            item = buffer[front];                  // Remove item from the buffer
            front = (front + 1) % capacity;        // Update front index
            --count;                               // Decrement count of elements
        }
        notFull.notify_one(); // Notify producer that buffer is not full
        return item;
    }
};

// Base class for shapes
class Shape
{
public:
    // Virtual function to compute area of shape
    virtual double area() const = 0;
    // Virtual function to draw shape
    virtual void draw() const = 0;
    // Virtual destructor
    virtual ~Shape() {}
};

// Derived class for circle shape
class Circle : public Shape
{
private:
    double radius;

public:
    // Constructor to initialize circle with given radius
    Circle(double r) : radius(r) {}

    // Function to compute area of circle
    double area() const override
    {
        return 3.14159 * radius * radius; // Area of circle formula
    }

    // Function to draw circle
    void draw() const override
    {
        std::cout << "Drawing a circle with radius " << radius << std::endl;
    }
};

// Producer function that generates Circle objects and adds them to the buffer
void producerFunc(CircularBuffer<std::shared_ptr<Shape>> &buffer)
{
    try // Exception handling
    {
        for (int i = 1; i <= 5; ++i)
        {
            buffer.produce(std::make_shared<Circle>(i));          // Create Circle object and add to buffer (Shared pointer)
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Producer exception: " << e.what() << std::endl;
    }
}

// Consumer function that retrieves Circle objects from the buffer and processes them
void consumerFunc(CircularBuffer<std::shared_ptr<Shape>> &buffer)
{
    try
    {
        for (int i = 0; i < 5; ++i)
        {
            std::shared_ptr<Shape> shape = buffer.consume();                         // Retrieve Circle object from buffer
            std::cout << "Consumed shape with area: " << shape->area() << std::endl; // Compute and print area
            shape->draw();                                                           // Draw the shape
            std::this_thread::sleep_for(std::chrono::seconds(2));                    // Simulate work
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Consumer exception: " << e.what() << std::endl;
    }
}

// Main function
int main()
{
    // Create a circular buffer of shapes with capacity 3
    CircularBuffer<std::shared_ptr<Shape>> buffer(3);

    // Create producer and consumer threads
    std::thread producerThread(producerFunc, std::ref(buffer));
    std::thread consumerThread(consumerFunc, std::ref(buffer));

    // Join threads
    producerThread.join();
    consumerThread.join();

    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <stdexcept>

// Class representing a basic circular buffer
template <typename T>
class CircularBuffer
{
private:
    std::unique_ptr<T[]> buffer;                                     // Pointer to buffer
    size_t capacity;                                                 // Capacity of the buffer
    size_t head;                                                     // Index of the head element
    size_t tail;                                                     // Index of the tail element
    std::mutex mutex;                                                // Mutex for thread safety
    std::condition_variable not_empty;                               // Condition variable for not empty
    std::condition_variable not_full;                                // Condition variable for not full
    bool is_full() const { return ((head + 1) % capacity) == tail; } // Check if buffer is full
    bool is_empty() const { return head == tail; }                   // Check if buffer is empty
public:
    explicit CircularBuffer(size_t capacity) : buffer(new T[capacity]), capacity(capacity), head(0), tail(0) {}

    // Function to push an item into the buffer
    void push(T item)
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_full.wait(lock, [this]()
                      { return !is_full(); }); // Wait until buffer is not full
        buffer[head] = item;                   // Insert item
        head = (head + 1) % capacity;          // Update head index
        not_empty.notify_one();                // Notify one waiting thread
    }

    // Function to pop an item from the buffer
    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_empty.wait(lock, [this]()
                       { return !is_empty(); }); // Wait until buffer is not empty
        T item = buffer[tail];                   // Retrieve item
        tail = (tail + 1) % capacity;            // Update tail index
        not_full.notify_one();                   // Notify one waiting thread
        return item;
    }

    // Function to get the size of the buffer
    size_t size() const
    {
        std::unique_lock<std::mutex> lock(mutex);
        if (head >= tail)
        {
            return head - tail;
        }
        else
        {
            return capacity - (tail - head);
        }
    }
};

// Example class hierarchy demonstrating inheritance and polymorphism
class Shape
{
public:
    virtual double area() const = 0; // Virtual function for calculating area
    virtual ~Shape() = default;      // Virtual destructor
};

class Circle : public Shape
{
private:
    double radius;

public:
    explicit Circle(double r) : radius(r) {}                           // Constructor
    double area() const override { return 3.14159 * radius * radius; } // Area calculation
};

class Square : public Shape
{
private:
    double side;

public:
    explicit Square(double s) : side(s) {}               // Constructor
    double area() const override { return side * side; } // Area calculation
};

// Operator overloading example
class Complex
{
private:
    double real;
    double imag;

public:
    Complex(double r, double i) : real(r), imag(i) {} // Constructor
    Complex operator+(const Complex &other) const
    { // Operator overloading for addition
        return Complex(real + other.real, imag + other.imag);
    }
    friend std::ostream &operator<<(std::ostream &os, const Complex &c)
    { // Operator overloading for output
        os << c.real << " + " << c.imag << "i";
        return os;
    }
};

int main()
{
    // Multithreading example with CircularBuffer
    CircularBuffer<int> buffer(5);
    std::thread producer([&]()
                         {
        for (int i = 0; i < 10; ++i) {
            buffer.push(i);                     // Push items into the buffer
            std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Sleep for simulation
        } });
    std::thread consumer([&]()
                         {
        for (int i = 0; i < 10; ++i) {
            int item = buffer.pop();           // Pop items from the buffer
            std::cout << "Consumed: " << item << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(700));  // Sleep for simulation
        } });
    producer.join(); // Join producer thread
    consumer.join(); // Join consumer thread

    // Shared pointer example with Shape hierarchy
    std::shared_ptr<Shape> circle = std::make_shared<Circle>(5.0); // Create shared_ptr of Circle
    std::shared_ptr<Shape> square = std::make_shared<Square>(4.0); // Create shared_ptr of Square
    std::cout << "Circle area: " << circle->area() << std::endl;   // Print circle area
    std::cout << "Square area: " << square->area() << std::endl;   // Print square area

    // Exception handling example with Complex numbers
    try
    {
        Complex c1(1.0, 2.0); // Create Complex objects
        Complex c2(3.0, 4.0);
        Complex c3 = c1 + c2;                    // Add two Complex objects
        std::cout << "Sum: " << c3 << std::endl; // Print sum
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl; // Handle exception
    }

    return 0; // Return success
}

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <stdexcept>
#include <vector>

// Class representing a basic circular buffer
template <typename T>
class CircularBuffer
{
private:
    std::unique_ptr<T[]> buffer;                                     // Pointer to buffer
    size_t capacity;                                                 // Capacity of the buffer
    size_t head;                                                     // Index of the head element
    size_t tail;                                                     // Index of the tail element
    std::mutex mutex;                                                // Mutex for thread safety
    std::condition_variable not_empty;                               // Condition variable for not empty
    std::condition_variable not_full;                                // Condition variable for not full
    bool is_full() const { return ((head + 1) % capacity) == tail; } // Check if buffer is full
    bool is_empty() const { return head == tail; }                   // Check if buffer is empty
public:
    explicit CircularBuffer(size_t capacity) : buffer(new T[capacity]), capacity(capacity), head(0), tail(0) {}

    // Constructor, copy constructor, and copy assignment operator deleted, made it uncopyable
    CircularBuffer(const CircularBuffer &) = delete;
    CircularBuffer &operator=(const CircularBuffer &) = delete;

    // Function to push an item into the buffer
    void push(T item)
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_full.wait(lock, [this]()
                      { return !is_full(); }); // Wait until buffer is not full
        buffer[head] = item;                   // Insert item
        head = (head + 1) % capacity;          // Update head index
        not_empty.notify_one();                // Notify one waiting thread
    }

    // Function to pop an item from the buffer
    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        not_empty.wait(lock, [this]()
                       { return !is_empty(); }); // Wait until buffer is not empty
        T item = buffer[tail];                   // Retrieve item
        tail = (tail + 1) % capacity;            // Update tail index
        not_full.notify_one();                   // Notify one waiting thread
        return item;
    }

    // Function to get the size of the buffer
    size_t size() const
    {
        std::unique_lock<std::mutex> lock(mutex);
        if (head >= tail)
        {
            return head - tail;
        }
        else
        {
            return capacity - (tail - head);
        }
    }
};

// Example class hierarchy demonstrating inheritance and polymorphism
class Shape
{
public:
    virtual double area() const = 0; // Virtual function for calculating area
    virtual ~Shape() = default;      // Virtual destructor

    // Copy constructor and copy assignment operator deleted
    Shape(const Shape &) = delete;
    Shape &operator=(const Shape &) = delete;
};

class Circle : public Shape
{
private:
    double radius;

public:
    explicit Circle(double r) : radius(r) {}                           // Constructor
    double area() const override { return 3.14159 * radius * radius; } // Area calculation

    // Copy constructor and copy assignment operator deleted
    Circle(const Circle &) = delete;
    Circle &operator=(const Circle &) = delete;
};

class Square : public Shape
{
private:
    double side;

public:
    explicit Square(double s) : side(s) {}               // Constructor
    double area() const override { return side * side; } // Area calculation

    // Copy constructor and copy assignment operator deleted
    Square(const Square &) = delete;
    Square &operator=(const Square &) = delete;
};

// Operator overloading example
class Complex
{
private:
    double real;
    double imag;

public:
    Complex(double r, double i) : real(r), imag(i) {} // Constructor
    Complex operator+(const Complex &other) const
    { // Operator overloading for addition
        return Complex(real + other.real, imag + other.imag);
    }
    friend std::ostream &operator<<(std::ostream &os, const Complex &c)
    { // Operator overloading for output
        os << c.real << " + " << c.imag << "i";
        return os;
    }

    // Copy constructor and copy assignment operator deleted
    Complex(const Complex &) = delete;
    Complex &operator=(const Complex &) = delete;
};

int main()
{
    // Multithreading example with CircularBuffer
    CircularBuffer<int> buffer(5);
    std::thread producer([&]()
                         {
        for (int i = 0; i < 10; ++i) {
            buffer.push(i);                     // Push items into the buffer
            std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Sleep for simulation
        } });
    std::thread consumer([&]()
                         {
        for (int i = 0; i < 10; ++i) {
            int item = buffer.pop();           // Pop items from the buffer
            std::cout << "Consumed: " << item << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(700));  // Sleep for simulation
        } });
    producer.join(); // Join producer thread
    consumer.join(); // Join consumer thread

    // Shared pointer example with Shape hierarchy
    std::shared_ptr<Shape> circle = std::make_shared<Circle>(5.0); // Create shared_ptr of Circle
    std::shared_ptr<Shape> square = std::make_shared<Square>(4.0); // Create shared_ptr of Square
    std::cout << "Circle area: " << circle->area() << std::endl;   // Print circle area
    std::cout << "Square area: " << square->area() << std::endl;   // Print square area

    // Exception handling example with Complex numbers
    try
    {
        Complex c1(1.0, 2.0); // Create Complex objects
        Complex c2(3.0, 4.0);
        Complex c3 = c1 + c2;                    // Add two Complex objects
        std::cout << "Sum: " << c3 << std::endl; // Print sum
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl; // Handle exception
    }

    return 0; // Return success
}
