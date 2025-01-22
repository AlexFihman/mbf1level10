#include <iostream>
#include "MonotoneBooleanFunction.h"

int MonotoneBooleanFunction::bit_count_lookup[(1 << DIMENSION)];
int MonotoneBooleanFunction::max_down[(1 << DIMENSION)];
int MonotoneBooleanFunction::layerSize[(DIMENSION + 1)];

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
        initialized = true;
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
    layerBitsSet[index_bits] += functionArray[index] ? 1 : -1;

    updateMinCutsFast(index, functionArray[index]);
}

void MonotoneBooleanFunction::flipRandom()
{
    int index = getRandomMinCut();
    functionArray[index] = !functionArray[index];
    weight += functionArray[index] ? 1 : -1;
    int index_bits = bit_count_lookup[index];
    layerBitsSet[index_bits] += functionArray[index] ? 1 : -1;
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
        down_count[index] = 0;
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
        if (up_count[i] == 0 && down_count[i] == max_down[i])
            min_cuts.insert(i);
    }
}

bool MonotoneBooleanFunction::is_mincut(int index)
{
    return up_count[index] == 0 && down_count[index] == max_down[index];
}

void MonotoneBooleanFunction::update_counts(int index, bool new_value)
{
    int delta = new_value ? 1 : -1;
    for (int k = 0; k < DIMENSION; k++)
    {
        int idx2 = index ^ (1 << k);

        if (idx2 > index)
        {
            up_count[idx2] += delta;
            if (is_mincut(idx2))
            {
                min_cuts.insert(idx2);
            }
            else
            {
                min_cuts.remove(idx2);
            }
        }
        else
        {
            down_count[idx2] += delta;
            if (is_mincut(idx2))
            {
                min_cuts.insert(idx2);
            }
            else
            {
                min_cuts.remove(idx2);
            }
        }
    }
}

void MonotoneBooleanFunction::updateMinCutsFast(int index, bool new_value)
{
    update_counts(index, new_value);
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

int MonotoneBooleanFunction::lastEmptyLayer() const
{
    int result = -1;
    for (int i = 0; i <= DIMENSION; i++)
    {
        if (layerBitsSet[i] == 0)
        {
            result = i;            
        } 
        else
            break;
    }
    return result;
}

int MonotoneBooleanFunction::firstFullLayer() const
{    
    int result = DIMENSION + 1;
    for (int i = DIMENSION; i >= 0; i--)
    {
        if (layerBitsSet[i] == layerSize[i])
        {
            result = i;            
        } 
        else
            break;
    }
    return result;
}