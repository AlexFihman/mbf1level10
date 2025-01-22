#pragma once

#include <bitset>
#include <array>

#include "ShortList.h"

extern std::array<int, (1 << DIMENSION)> bit_count_lookup;

class MonotoneBooleanFunction
{
private:    
    int weight = 0;    
    bool functionArray[1 << DIMENSION]{0}; //The function (value of the function for each input)
    int up_count[1 << DIMENSION]{0};
    int down_count[1 << DIMENSION]{0};
    int layerBitsSet[DIMENSION + 1]{0}; // Number of bits set in each layer
    
    static int bit_count_lookup[(1 << DIMENSION)];
    static int max_down[(1 << DIMENSION)];
    static int layerSize[DIMENSION + 1];

    sfmt_t* sfmt;
    ShortList min_cuts;
    bool checkMinCut(int index) const;
    void updateMinCuts();
    void updateMinCutsFast(int index, bool new_value);

public:
    MonotoneBooleanFunction(sfmt_t* sfmt);

    MonotoneBooleanFunction(const MonotoneBooleanFunction &) = delete;

    MonotoneBooleanFunction &operator=(const MonotoneBooleanFunction &) = delete;    

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

    int lastEmptyLayer() const;

    int firstFullLayer() const;

    ShortList getMinCNF();
};
