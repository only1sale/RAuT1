#pragma once

#include <thread>
#include <array>
#include <cmath>
#include <chrono>
#include <climits>

using std::array;
using namespace std::chrono_literals;

namespace raut {

    struct Rand {
    private:
        //Park & Miller standardni generator
        const unsigned a = 16807u;
        const unsigned m = 2147483647u;

        //Schrage-ov algoritam za faktorizaciju, da se izbegne overflow
        const unsigned q = 127773u;
        const unsigned r = 2836u;

        unsigned seed;

        //niz od 32 rng broja
        array<unsigned, 32> loto;

        auto minStdRand() {
            //Schrage-ov algoritam za faktorizaciju
            auto x = seed / q;
            long long tmp = a * (seed - x * q) - r * x;
            if(tmp < 0)
                return seed = tmp + m; //random broj od 0 do m
            return seed = tmp % m;
        }

        //Potrebna funkcija zbog static niza...
        //Potrebno koriscenje array-a zbog static niza i vracanja vrednosti funkcije
        array<unsigned, 32> init() {
            array<unsigned, 32> res = {};
            for(auto &r : res) {
                minStdRand();
                r = seed;
            }
            return res;
        }

    public:
        //Konstruktor
        Rand(unsigned s = 42u) { seed = s; loto = init(); }

        //Bays and Durham shuffling (loto sistem)
        auto rand() {
            minStdRand();
            auto i = seed / (1 + (m-1) / 32); //broj od 0 do 31
            double y = loto[i];
            loto[i] = seed;
            return y / m;
        }

        //Box Muller metod za generisanje broja po gausovoj raspodeli
        auto randn() {
            double x, y, m, z;
            do {
                x = 2. * rand() - 1.;
                y = 2. * rand() - 1.;
                m = x * x + y * y;
            } while(m >= 1. || m == 0);
            z = sqrt((-2. * log(m)) / m);

            return x * z;
        }

        //Slucajan ceo broj od 0 do 4294967295
        unsigned randi(unsigned M = UINT_MAX-1) {
            return rand() * (M+1);    
        }

        auto srand(unsigned u) {
            seed = u;
        }

    };

    inline void delay(std::chrono::milliseconds time) {
        std::this_thread::sleep_for(time);
    }

}

