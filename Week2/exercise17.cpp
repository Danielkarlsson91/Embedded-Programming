/*Using a mutex, a condition variable and 3 threads every secode print 1 – 2 – 3 to the terminal.
The first thread shall print 1 – , the second thread shall print 2 – and the third thread shall print 3 and newline.
The threads shall be synchronized.
*/

#include <mutex>
#include <iostream>
#include <thread>
#include <condition_variable>

static std::mutex mtx;
static volatile int number{1};
static std::condition_variable cv;

void print_number(const int NUM)
{
    while (1)
    {
        if (NUM == 1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::unique_lock lock(mtx);

        cv.wait(lock, [NUM]
                { return (number == NUM); });

        if (number == 3)
        {
            std::cout << number << std::endl;
            number = 1;
        }
        else
        {
            std::cout << number << " - ";
            number++;
        }

        lock.unlock();
        cv.notify_all();
    }
}

int main()
{
    std::thread thrd1(print_number, 1);
    std::thread thrd2(print_number, 2);
    std::thread thrd3(print_number, 3);

    thrd1.join();
    thrd2.join();
    thrd3.join();

    return 0;
}