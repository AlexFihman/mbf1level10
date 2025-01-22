#include <iostream>
#include "MonotoneBooleanFunction.h"

int MonotoneBooleanFunction::bit_count_lookup[(1 << DIMENSION)];
int MonotoneBooleanFunction::max_down[(1 << DIMENSION)];
int MonotoneBooleanFunction::layerSize[(DIMENSION + 1)];
int max_b;

int hammingDistance(uint64_t const x, uint64_t const y)
{
    return std::bitset<64>(x ^ y).count();
}

MonotoneBooleanFunction::MonotoneBooleanFunction(sfmt_t* sfmt) : weight(0), sfmt(sfmt), min_cuts()
{
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < (1 << DIMENSION); i++) {
            max_down[i] = DIMENSION - __builtin_popcount(i);
            bit_count_lookup[i] = __builtin_popcount(i);
        }
        layerSize[0] = 1;
        for (int i = 1; i <= DIMENSION; i++) {
            layerSize[i] = layerSize[i - 1] * (DIMENSION - i + 1) / i;
        }
        max_b = 0;
        for (int i= mid_layer + 1; i<= DIMENSION; i++) {
            max_b += layerSize[i];
        }
        initialized = true;
        countB = -max_b;
    }    

    updateMinCuts();
}

bool MonotoneBooleanFunction::getFunctionValue(int index) const
{
    return functionArray[index];
}

void MonotoneBooleanFunction::setFunctionValue(int index, bool value)
{
    functionArray[index] = value;
    updateMinCuts();
}

void MonotoneBooleanFunction::flip(int index)
{
    functionArray[index] = !functionArray[index];
    weight += functionArray[index] ? 1 : -1;
    int index_bits = bit_count_lookup[index];        

    if (index_bits < mid_layer)
        countA += functionArray[index] ? 1 : -1;
    else if (index_bits > mid_layer)
        countB += functionArray[index] ? 1 : -1;

    updateMinCutsFast(index, functionArray[index]);
}

void MonotoneBooleanFunction::flipRandom()
{
    int index = getRandomMinCut();
    functionArray[index] = !functionArray[index];
    weight += functionArray[index] ? 1 : -1;
    int index_bits = bit_count_lookup[index];

    if (index_bits < mid_layer)
        countA += functionArray[index] ? 1 : -1;
    else if (index_bits > mid_layer)
        countB += functionArray[index] ? 1 : -1;
    
    updateMinCutsFast(index, functionArray[index]);
}

void MonotoneBooleanFunction::step()
{
    uint32_t r;
    do
    {
        flipRandom();
        r = ((uint64_t)sfmt_genrand_uint32(sfmt) * minCutSize()) >> 32;

    } while (r != 0);
}

bool MonotoneBooleanFunction::checkMinCut(int index) const
{
    for (int k = 0; k < DIMENSION; k++)
    {
        int idx2 = index ^ (1 << k);
        // std::cout << "idx2: " << idx2 << "\t" << "value: " << functionArray[index] << std::endl;
        if (idx2 < index && functionArray[idx2])
            return false;
        if (idx2 > index && !functionArray[idx2])
            return false;
    }
    return true;
}

void MonotoneBooleanFunction::updateMinCuts()
{
    min_cuts.clear();
    for (int index = 0; index < (1 << DIMENSION); index++)
    {
        down_count[index] = -max_down[index];
        up_count[index] = 0;
    }

    for (int index = 0; index < (1 << DIMENSION); index++)
    {
        if (functionArray[index])
            for (int k = 0; k < DIMENSION; k++)
            {
                int idx2 = index ^ (1 << k);

                if (idx2 < index)
                    down_count[idx2]++;
                else
                    up_count[idx2]++;
            }
    }

    for (int i = 0; i < (1 << DIMENSION); i++)
    {
        if (up_count[i] == 0 && down_count[i] == 0)
            min_cuts.insert(i);
    }
}

bool MonotoneBooleanFunction::is_mincut(int index)
{
    return up_count[index] == 0 && down_count[index] == 0;
}

void MonotoneBooleanFunction::updateMinCutsFast(int index, bool new_value)
{
    int delta = new_value ? 1 : -1;
    for (int k = 0; k < DIMENSION; k++)
    {
        int idx2 = index ^ (1 << k);

        if (idx2 > index)
        {
            int prev = up_count[idx2];
            up_count[idx2] += delta;
            if (up_count[idx2] == 0)
            {
                min_cuts.insert(idx2);
            }
            else
            {
                if (prev == 0)
                    min_cuts.remove(idx2);
            }
        }
        else
        {
            int prev = down_count[idx2];
            down_count[idx2] += delta;
            if (down_count[idx2] == 0)            
            {
                min_cuts.insert(idx2);
            }
            else
            {
                if (prev == 0)
                    min_cuts.remove(idx2);
            }
        }
    }
}

int MonotoneBooleanFunction::getRandomMinCut() const
{
    return min_cuts.getRandomElement(sfmt);
}

void MonotoneBooleanFunction::printMinCuts() const
{
    min_cuts.print();
}

int MonotoneBooleanFunction::minCutSize() const
{
    return min_cuts.getSize();
}

ShortList MonotoneBooleanFunction::getMinCNF()
{
    ShortList result = ShortList();
    for (int i = 0; i < min_cuts.getSize(); i++)
    {
        int element = min_cuts.getValue(i);

        if (functionArray[element])
            result.insert(element);
    }
    return result;
}

int MonotoneBooleanFunction::getWeight() const
{
    return weight;
}

bool MonotoneBooleanFunction::isOneLevel() const
{
    return (countA == 0) && (countB == 0);
}