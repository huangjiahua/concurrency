//
// Created by jiahua on 2019/2/26.
//

#include <iostream>
#include <thread>
#include <vector>
#include "ts_queue.h"

using namespace std;

int main() {
    ts_queue<int> qu;
    vector<thread> threads;
    int ret[6];


    threads.reserve(12);
    for (int &i : ret) {
        threads.emplace_back([&](){
            qu.wait_and_pop(i);
        });
    }

//    for (int i = 0; i < 6; i++)
//        qu.push(i);

    for (int i = 0; i < 6; i++) {
        threads.emplace_back([&](int k){
            qu.push(k);
        }, i);
    }

    for (auto &thread : threads)
        thread.join();

    for (auto &i : ret)
        cout << i << endl;
}
