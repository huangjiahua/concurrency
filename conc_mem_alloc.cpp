//
// Created by jiahua on 2019/3/20.
//

#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <array>

using namespace std;
using namespace chrono;

constexpr size_t TOTAL_MEM = (size_t)(1 << 29);
size_t TOTAL_TIMES = 0;

struct Allocator {
    vector<unsigned char*> data;
    explicit Allocator(size_t m): data(m, nullptr) {}
    void alloc(size_t idx, size_t sz) { data[idx] = new unsigned char[sz]; }
    ~Allocator() { for (auto p : data) delete [] p; }
};

int main(int argc, const char *argv[]) {
    size_t thread_num = 8, gran = 8;
    if (argc >= 3) {
        thread_num = stoull(string(argv[1]));
        gran = stoull(string(argv[2]));
    }

    TOTAL_TIMES = TOTAL_MEM / (gran + thread_num);
    size_t tt = TOTAL_TIMES;
    vector<thread> threads(thread_num);
    vector<size_t> time(thread_num, 0);
    Allocator allocator(thread_num * TOTAL_TIMES);

    auto t1 = steady_clock::now();
    for (size_t i = 0; i < thread_num; i++) {
        threads[i] = thread([i, thread_num](vector<size_t> &time, Allocator &alloc, size_t g) {
            auto ts = steady_clock::now();
            size_t j = i;
            for (size_t k = 0; k < TOTAL_TIMES; k++, j += thread_num) {
                alloc.alloc(j, g);
            }
            auto te = steady_clock::now();
            time[i] = static_cast<unsigned long long int>(duration_cast<microseconds>(te - ts).count());
        }, ref(time), ref(allocator), gran);
    }
    for (auto &t : threads) t.join();
    auto t2 = steady_clock::now();

    size_t total_time  = static_cast<size_t>(duration_cast<microseconds>(t2 - t1).count());
    size_t average_time = accumulate(time.begin(), time.end(), (size_t)0) / time.size();

    double total_tp = (double)(TOTAL_TIMES * thread_num * gran) / (double)(total_time) * 1000.0 / 1024.0;
    double ave_tp = (double)(TOTAL_TIMES * thread_num * gran) / (double)(average_time) * 1000.0 / 1024.0;

    cout << "thread count: " << setw(3) << thread_num << "  granularity: " << setw(4) << gran << " B"
         << "  average throughput: " << fixed << ave_tp << " MB/s " << "  total throughput: " << fixed
         << total_tp << " MB/s" << endl;
}

