#pragma once

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

namespace raut {


    inline void delay(std::chrono::milliseconds time) {
        std::this_thread::sleep_for(time);
    }

}
