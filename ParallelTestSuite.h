//
// Created by jiahua on 2019/3/8.
//

#ifndef CONCURRENCY_PARALLELTESTSUITE_H
#define CONCURRENCY_PARALLELTESTSUITE_H

#include <thread>
#include <chrono>
#include <vector>
#include <limits>

struct ParallelTestSuite {
    using ST = unsigned long long;
    ST thread_num;
    ST destination;
    std::chrono::microseconds time{};

    explicit ParallelTestSuite(ST td, ST dest = 1000000000): thread_num(td), destination(dest) {
        doTest();
    }
    
    void doTest() {
        std::vector<std::thread> threads;
        threads.reserve(thread_num);
        auto t1 = std::chrono::steady_clock::now();
        for (int i = 0; i < thread_num; i++) 
            threads.emplace_back([](ST dest) {
                for (ST j = 0; j < dest; j++) ;
            }, destination);
        for (auto &t : threads) t.join();
        auto t2 = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
    }
    
    ST getTime() { return static_cast<ST>(time.count()); }
};


#endif //CONCURRENCY_PARALLELTESTSUITE_H
