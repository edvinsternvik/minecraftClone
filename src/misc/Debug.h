#pragma once
#include <chrono>
#include <iostream>

class FunctionTimer {
public:
    FunctionTimer() : start(std::chrono::high_resolution_clock::now()) {

    }

    ~FunctionTimer() {
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Function time: " << time.count() << std::endl;
    }

private:
    std::chrono::high_resolution_clock::time_point start;
};
