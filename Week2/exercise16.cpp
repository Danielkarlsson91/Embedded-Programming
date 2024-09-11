/*Using two threads make a program to print “Ping - Pong” every second to the terminal.
A thread shall print Ping and the other thread shall print Pong to the terminal.
Using two mutexes ensure the right order so that the output looks like:
              Ping - Pong
              Ping - Pong
              Ping - Pong*/

#include <mutex>
#include <iostream>
#include <thread>

static std::mutex ping, pong;

static void print_ping()
{
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        ping.lock();

        std::cout << "Ping - ";

        pong.unlock();
    }
}

static void print_pong()
{
    while (1)
    {
        pong.lock();

        std::cout << "Pong" << std::endl;

        ping.unlock();
    }
}

int main()
{
    pong.lock();

    std::thread ping_thrd(print_ping);
    std::thread pong_thrd(print_pong);

    ping_thrd.join();
    pong_thrd.join();

    return 0;
}