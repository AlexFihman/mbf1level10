#include <iostream>
#include <fstream>
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

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Error: No argument provided. Please provide batch number - an integer argument." << std::endl;
        return EXIT_FAILURE;
    }

    int batchNo;
    std::stringstream ss(argv[1]);
    if (!(ss >> batchNo)) {
        std::cerr << "Error: The argument is not a valid integer." << std::endl;
        return EXIT_FAILURE;
    }

    sfmt_t* sfmt = new sfmt_t;
    sfmt_init_gen_rand(sfmt, batchNo);

    std::string filename = "result." +  std::to_string(batchNo) + ".csv";
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error creating file '" << filename << "'" << std::endl;
        return EXIT_FAILURE;
    }
    outfile << std::scientific << std::setprecision(17);

    MonotoneBooleanFunction mbf1(sfmt);

    for (int i = 0; i < 90000; i++)
    {
        mbf1.flipRandom();
    }

    for (int loop = 0; loop < 10; loop++)
    {
        double startTime = time_seconds();
        double stat[(DIMENSION + 2)*(DIMENSION + 2)] = {0.0};

        for (int64_t i = 0; i < 100000000; i++)
        {
            mbf1.flipRandom();
            int l0 = mbf1.lastEmptyLayer();
            int l1 = mbf1.firstFullLayer();
            stat[(l0 + 1) * (DIMENSION + 2) + l1] += 1.0/mbf1.minCutSize();
        }
        double endTime = time_seconds();
        
        for (int i = 0; i < (DIMENSION + 2)*(DIMENSION + 2); i++)
        {
            if (stat[i] > 0)
            {
                outfile <<  batchNo << "," << loop << "," << (endTime - startTime) << "," << (i / (DIMENSION + 2) - 1) << "," << i % (DIMENSION + 2) << "," << (stat[i]) << std::endl;
            }
        }
    }
    outfile.close();

    return 0;
}
