#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <string_view>

const uint8_t n = 5;
std::mutex f[n];
std::mutex lockprint;

void think(int i) {
    std::unique_lock<std::mutex> printer(lockprint);
    std::cout << "Philosopher " << i << " is thinking ...\n";
    printer.unlock();
}

void eat(int i) {
    std::unique_lock<std::mutex> printer(lockprint);
    std::cout << "Philosopher " << i << " is eating ...\n";
    printer.unlock();
}

void dinner(int i) {
    while (true) {
        think(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        auto one = std::min(i, (i + 1) % n);
        auto two = std::max(i, (i + 1) % n);
        std::unique_lock<std::mutex> R(f[one]);
        std::unique_lock<std::mutex> L(f[two]);
        eat(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        R.unlock();
        L.unlock();
    }
}

int main(){
    std::thread p0(dinner, 0);
    std::thread p1(dinner, 1);
    std::thread p2(dinner, 2);
    std::thread p3(dinner, 3);
    std::thread p4(dinner, 4);
    p0.join();
    p1.join();
    p2.join();
    p3.join();
    p4.join();
    return 0;
}