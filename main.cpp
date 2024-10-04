#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <chrono>
#include <array>
#include <iomanip>
#include "ShortList.h"
#include "MonotoneBooleanFunction.h"
#include <string>

bool cmp(uint64_t x1, uint64_t x2)
{
    return (x1 & x2) == x2;
}

double time_seconds()
{
    auto now = std::chrono::system_clock::now();
    return 1e-9 * std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}

std::array<int, 1024> bit_count_lookup;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Error: No argument provided. Please provide an integer argument." << std::endl;
        return EXIT_FAILURE;
    }

    int batchNo;
    std::stringstream ss(argv[1]);
    if (!(ss >> batchNo)) {
        std::cerr << "Error: The argument is not a valid integer." << std::endl;
        return EXIT_FAILURE;
    }



    std::mt19937 rng(batchNo);
    std::string filename = "result." +  std::to_string(batchNo) + ".txt";
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error creating file '" << filename << "'" << std::endl;
        return EXIT_FAILURE;
    }
    outfile << std::scientific << std::setprecision(17);
    
    const int dim = 10;

    for (int i = 0; i < (1 << dim); i++)
        max_down[i] = dim - __builtin_popcount(i);

    for (int i = 0; i < (1 << dim); i++)
    {
        bit_count_lookup[i] = __builtin_popcount(i);
    }

    MonotoneBooleanFunction mbf1(dim, rng);

    for (int i = 0; i < 30000; i++)
    {
        mbf1.flipRandom();
    }

    for (int loop = 0; loop < 10; loop++)
    {
        double startTime = time_seconds();

        double p1L = 0;
        double pNot1L = 0;

        for (int i = 0; i < 100000000; i++)
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
        double endTime = time_seconds();

        outfile << batchNo << "\t" << loop << "\t" << (endTime - startTime) << "\t" << p1L << "\t" << (p1L + pNot1L) << std::endl;
    }
    outfile.close();

    return 0;
}
