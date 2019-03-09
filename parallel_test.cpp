//
// Created by jiahua on 2019/3/8.
//
#include <iostream>
#include "ParallelTestSuite.h"
#include <string>
#include <cstdlib>
#include <fstream>
using namespace std;

int main(int argc, const char *argv[]) {
    int td = 1;
    if (argc > 4) {
        exit(EXIT_FAILURE);
    }
    if (argc == 2) {
        td = stoi(string(argv[1]));
    }

    ParallelTestSuite pts(static_cast<ParallelTestSuite::ST>(td));

    size_t tm = pts.getTime();

    if (argc == 3) {
        cout << fixed << tm << endl;
    } else if (argc == 2){
        cout << "Thread count: " << td << endl;
        cout << "Time:         " << fixed << tm << endl;
        ofstream of("result.txt", ios_base::app);
        of << fixed << tm << endl;
    } else if (argc == 4) {
        ofstream of("result.txt", ios_base::app);
        of << fixed << tm << endl;
        cout << "done" << endl;
    }
}

