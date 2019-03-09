#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cassert>

using namespace std;

class task {
public:
    void operator()() {
        cout << "hey" << endl;
    }
};

auto order = std::memory_order_acq_rel;

std::atomic<int> data[5];
std::atomic<bool> sync1(false),sync2(false);
void thread_1() {
    data[0].store(42,std::memory_order_relaxed);
    data[1].store(97,std::memory_order_relaxed);
    data[2].store(17,std::memory_order_relaxed);
    data[3].store(-141,std::memory_order_relaxed);
    data[4].store(2003,std::memory_order_relaxed);
    sync1.store(true, order);
}

void thread_2() {
    while(!sync1.load(order));
    sync2.store(true,order);
}

void thread_3() {
    while(!sync2.load(order));
    assert(data[0].load(std::memory_order_relaxed)==42);
    assert(data[1].load(std::memory_order_relaxed)==97);
    assert(data[2].load(std::memory_order_relaxed)==17);
    assert(data[3].load(std::memory_order_relaxed)==-141);
    assert(data[4].load(std::memory_order_relaxed)==2003);
}


int main() {
    thread thread1(thread_1), thread2(thread_2), thread3(thread_3);
    thread1.join();
    thread2.join();
    thread3.join();

    return 0;
}