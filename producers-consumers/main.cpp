#include <iostream>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <thread>

std::mutex m;
std::condition_variable c;
std::deque<int> buffer;
const uint8_t max_buffer_size = 32;
const uint8_t empty_buffer = 0;

int produce(int n) {
    return n;
}

void consume(std::deque<int>& q) {
    q.pop_back();
}

void producer(int n){
    while (n) {
        std::unique_lock<std::mutex> L(m);
        c.wait(L, [](){
            return buffer.size() < max_buffer_size;
        });
        int x = produce(n);
        buffer.push_front(x);
        std::cout << "producer: " << n << std::endl;
        --n;
        L.unlock();
        c.notify_one();
    }
}
void consumer(){
    while (true) {
        std::unique_lock<std::mutex> L(m);
        c.wait(L, [](){
            return buffer.size() > empty_buffer;
        });
        int n = buffer.back();
        consume(buffer);
        std::cout << "consumer: " << n << std::endl;
        L.unlock();
        c.notify_one();
    }
}
int main(){
    std::thread t1(producer, 100);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}