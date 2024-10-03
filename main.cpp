#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <chrono>
#include <array>
#include <iomanip>
#include "ShortList.h"
#include "MonotoneBooleanFunction.h"

bool cmp(uint64_t x1, uint64_t x2)
{
    return (x1 & x2) == x2;
}

int64_t time_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

std::array<int, 1024> bit_count_lookup;

int main()
{
    std::cout << std::scientific << std::setprecision(17);
    std::random_device rd;
    std::mt19937 rng(rd());
    const int dim = 10;

    for (int i = 0; i < (1 << dim); i++)
        max_down[i] = dim - std::bitset<32>(i).count();

    for (int i = 0; i <= 1023; i++)
    {
        bit_count_lookup[i] = __builtin_popcount(i); // Built-in function for counting bits
    }

    MonotoneBooleanFunction mbf1(dim, rng);

    for (int i = 0; i < 30000; i++)
    {
        mbf1.flipRandom();
    }

    for (int loop = 0; loop < 1000; loop++)
    {
        double startTime = time_ms();

        double p1L = 0;
        double pNot1L = 0;

        for (int i = 0; i < 10000000; i++)
        {
            mbf1.flipRandom();
            if (mbf1.isOneLevel())
            {
                p1L += 1.0 / mbf1.minCutSize();
            }
            else
            {
                pNot1L += 1.0 / mbf1.minCutSize();
            }
        }
        double endTime = time_ms();

        std::cout << loop << "\t" << (endTime - startTime) << "\t" << p1L << "\t" << (p1L + pNot1L) << std::endl;
    }

    return 0;
}
