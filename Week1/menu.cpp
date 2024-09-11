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
    std::thread worker(worker_func(), buffer);

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

// Circular Buffer class template
template <typename T, size_t Capacity>
class CircularBuffer
{
private:
    T buffer[Capacity];               // Array to hold buffer elements
    size_t front;                     // Index of the front element
    size_t rear;                      // Index of the rear element
    size_t count;                     // Number of elements in the buffer
    std::mutex mtx;                   // Mutex for thread safety
    std::condition_variable notFull;  // Condition variable for buffer not full
    std::condition_variable notEmpty; // Condition variable for buffer not empty

public:
    // Constructor to initialize the circular buffer
    CircularBuffer() : front(0), rear(0), count(0) {}

    // Producer method to add an item to the buffer
    void produce(const T &item)
    {
        std::unique_lock<std::mutex> lock(mtx);
        notFull.wait(lock, [this]()
                     { return count != Capacity; }); // Wait until buffer is not full
        buffer[rear] = item;                         // Add item to the buffer
        rear = (rear + 1) % Capacity;                // Update rear index
        ++count;                                     // Increment count of elements
        notEmpty.notify_one();                       // Notify consumer that buffer is not empty
    }

    // Consumer method to remove an item from the buffer
    T consume()
    {
        std::unique_lock<std::mutex> lock(mtx);
        notEmpty.wait(lock, [this]()
                      { return count != 0; }); // Wait until buffer is not empty
        T item = buffer[front];                // Remove item from the buffer
        front = (front + 1) % Capacity;        // Update front index
        --count;                               // Decrement count of elements
        notFull.notify_one();                  // Notify producer that buffer is not full
        return item;
    }
};

// Producer function that generates integers and adds them to the buffer
void producerFunc(CircularBuffer<int, 10> &buffer)
{
    try
    {
        for (int i = 1; i <= 5; ++i)
        {
            buffer.produce(i);                                    // Add integer to the buffer
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Producer exception: " << e.what() << std::endl;
    }
}

// Consumer function that retrieves integers from the buffer and prints them
void consumerFunc(CircularBuffer<int, 10> &buffer)
{
    try
    {
        for (int i = 0; i < 5; ++i)
        {
            int item = buffer.consume();                          // Retrieve integer from buffer
            std::cout << "Consumed item: " << item << std::endl;  // Print item
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Consumer exception: " << e.what() << std::endl;
    }
}

int main()
{
    // Create a circular buffer of integers with capacity 10
    CircularBuffer<int, 10> buffer;

    // Create producer and consumer threads
    std::thread producerThread(producerFunc, std::ref(buffer));
    std::thread consumerThread(consumerFunc, std::ref(buffer));

    // Join threads
    producerThread.join();
    consumerThread.join();

    return 0;
}

// A program with linked list

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <stdexcept>

// Node class for linked list
template <typename T>
struct Node
{
    T data;     // Data stored in the node
    Node *next; // Pointer to the next node

    // Constructor
    Node(const T &newData) : data(newData), next(nullptr) {}
};

// Circular Buffer class template
template <typename T, size_t Capacity>
class CircularBuffer
{
private:
    Node<T> *frontNode;               // Pointer to the front node
    Node<T> *rearNode;                // Pointer to the rear node
    size_t count;                     // Number of elements in the buffer
    std::mutex mtx;                   // Mutex for thread safety
    std::condition_variable notFull;  // Condition variable for buffer not full
    std::condition_variable notEmpty; // Condition variable for buffer not empty

public:
    // Constructor to initialize the circular buffer
    CircularBuffer() : frontNode(nullptr), rearNode(nullptr), count(0) {}

    // Producer method to add an item to the buffer
    void produce(const T &item)
    {
        std::unique_lock<std::mutex> lock(mtx);
        notFull.wait(lock, [this]()
                     { return count != Capacity; }); // Wait until buffer is not full

        Node<T> *newNode = new Node<T>(item); // Create a new node with the given item
        if (!frontNode)
        {
            frontNode = newNode; // If buffer is empty, set front to the new node
        }
        else
        {
            rearNode->next = newNode; // Otherwise, link the new node to the rear
        }
        rearNode = newNode;    // Update rear node
        ++count;               // Increment count of elements
        notEmpty.notify_one(); // Notify consumer that buffer is not empty
    }

    // Consumer method to remove an item from the buffer
    T consume()
    {
        std::unique_lock<std::mutex> lock(mtx);
        notEmpty.wait(lock, [this]()
                      { return count != 0; }); // Wait until buffer is not empty

        T item = frontNode->data;    // Get data from the front node
        Node<T> *temp = frontNode;   // Keep a reference to the front node
        frontNode = frontNode->next; // Move front to the next node
        if (!frontNode)
        {
            rearNode = nullptr; // If buffer becomes empty, update rear
        }
        delete temp;          // Free memory of the consumed node
        --count;              // Decrement count of elements
        notFull.notify_one(); // Notify producer that buffer is not full
        return item;
    }

    // Destructor to release memory
    ~CircularBuffer()
    {
        Node<T> *current = frontNode;
        while (current)
        {
            Node<T> *next = current->next;
            delete current;
            current = next;
        }
    }
};

// Producer function that generates integers and adds them to the buffer
void producerFunc(CircularBuffer<int, 10> &buffer)
{
    try
    {
        for (int i = 1; i <= 5; ++i)
        {
            buffer.produce(i);                                    // Add integer to the buffer
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Producer exception: " << e.what() << std::endl;
    }
}

// Consumer function that retrieves integers from the buffer and prints them
void consumerFunc(CircularBuffer<int, 10> &buffer)
{
    try
    {
        for (int i = 0; i < 5; ++i)
        {
            int item = buffer.consume();                          // Retrieve integer from buffer
            std::cout << "Consumed item: " << item << std::endl;  // Print item
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Consumer exception: " << e.what() << std::endl;
    }
}

int main()
{
    // Create a circular buffer of integers with capacity 10
    CircularBuffer<int, 10> buffer;

    // Create producer and consumer threads
    std::thread producerThread(producerFunc, std::ref(buffer));
    std::thread consumerThread(consumerFunc, std::ref(buffer));

    // Join threads
    producerThread.join();
    consumerThread.join();

    return 0;
}

