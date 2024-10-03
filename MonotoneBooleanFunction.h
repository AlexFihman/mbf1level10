#pragma once

#include <bitset>
#include <array>

#include "ShortList.h"

extern int max_down[1024];

extern std::array<int, 1024> bit_count_lookup;

class MonotoneBooleanFunction
{
private:
    int dimension; // Dimension of the boolean function
    int weight;
    int countA;
    int countB;
    bool *functionArray; // Array to store the boolean function
    int *up_count;
    int *down_count;
    std::mt19937 &rng;  // Reference to Mersenne Twister random number generator
    ShortList min_cuts; // ShortList to store the minimum cuts
    bool checkMinCut(int index) const;
    void updateMinCuts();
    void updateMinCutsFast(int index, bool new_value);

public:
    MonotoneBooleanFunction(int dim, std::mt19937 &r);

    MonotoneBooleanFunction(const MonotoneBooleanFunction &) = delete;

    MonotoneBooleanFunction &operator=(const MonotoneBooleanFunction &) = delete;

    void fill_middle();

    ~MonotoneBooleanFunction();

    bool getFunctionValue(int index) const;

    void setFunctionValue(int index, bool value);

    int getWeight() const;

    void flip(int index);

    void flipRandom();

    void step();

    int getRandomMinCut() const;

    bool is_mincut(int index);

    void update_counts(int index, bool new_value);

    void printMinCuts() const;

    int minCutSize() const;

    ShortList getMinCNF();

    bool isOneLevel();
};
