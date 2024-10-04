#include <iostream>
#include "MonotoneBooleanFunction.h"

int max_down[1024];

int hammingDistance(uint64_t x, uint64_t y)
{
    return std::bitset<64>(x ^ y).count();
}

MonotoneBooleanFunction::MonotoneBooleanFunction(int dim, std::mt19937 &r) : dimension(dim), weight(0), rng(r), min_cuts()
{

    functionArray = new bool[1 << dim](); // Initialize all values to false
    up_count = new int[1 << dim]();
    down_count = new int[1 << dim]();
    countA = 0;
    countB = 0;
    updateMinCuts();
}

MonotoneBooleanFunction::~MonotoneBooleanFunction()
{
    delete[] functionArray;
    delete[] up_count;
    delete[] down_count;
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
    if (index_bits < 5)
        countA += functionArray[index] ? 1 : -1;
    else if (index_bits > 5)
        countB += functionArray[index] ? 1 : -1;

    updateMinCutsFast(index, functionArray[index]);
}

void MonotoneBooleanFunction::flipRandom()
{
    int index = getRandomMinCut();
    functionArray[index] = !functionArray[index];
    weight += functionArray[index] ? 1 : -1;
    int index_bits = bit_count_lookup[index];
    if (index_bits < 5)
        countA += functionArray[index] ? 1 : -1;
    else if (index_bits > 5)
        countB += functionArray[index] ? 1 : -1;
    updateMinCutsFast(index, functionArray[index]);
}

void MonotoneBooleanFunction::step()
{
    do
    {
        flipRandom();
    } while (rng() % minCutSize() != 0);
}

bool MonotoneBooleanFunction::checkMinCut(int index) const
{
    for (int k = 0; k < dimension; k++)
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
    for (int index = 0; index < (1 << dimension); index++)
    {
        down_count[index] = 0;
        up_count[index] = 0;
    }

    for (int index = 0; index < (1 << dimension); index++)
    {
        if (functionArray[index])
            for (int k = 0; k < dimension; k++)
            {
                int idx2 = index ^ (1 << k);

                if (idx2 < index)
                    down_count[idx2]++;
                else
                    up_count[idx2]++;
            }
    }

    for (int i = 0; i < (1 << dimension); i++)
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
    for (int k = 0; k < dimension; k++)
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
    return min_cuts.getRandomElement(rng);
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

bool MonotoneBooleanFunction::isOneLevel()
{
    return (countA == 0) && (countB == 386);
}

int MonotoneBooleanFunction::getWeight() const
{
    return weight;
}