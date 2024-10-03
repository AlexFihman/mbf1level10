#include "ShortList.h"
#include <algorithm> // For std::find
#include <iostream>
#include <assert.h>

ShortList::ShortList() : size(0)
{
    std::fill(arr, arr + MAX_SIZE, -1);
    std::fill(indexArr, indexArr + MAX_SIZE, -1);
}

bool ShortList::insert(int num)
{
    if (indexArr[num] >= 0)
        return false;

    arr[size] = num;
    indexArr[num] = size;
    ++size;
    return true;
}

bool ShortList::remove(int num)
{
    if (indexArr[num] >= 0)
    {
        int idx = indexArr[num];
        if (idx != (size - 1))
        {
            int v = arr[idx] = arr[--size]; // Replace the element with the last element and decrement size
            indexArr[v] = idx;              // Update indexArr for the moved element
        }
        else
        {
            size--;
        }
        indexArr[num] = -1; // Invalidate the index of the removed element
        return true;
    }
    return false;
}

int ShortList::getRandomElement(std::mt19937 &rng) const
{
    return arr[rng() % size];
}

int ShortList::getSize() const
{
    return size;
}

int ShortList::getValue(int pos) const
{
    return arr[pos];
}

void ShortList::print() const
{
    std::cout << "List: ";
    for (int i = 0; i < size; ++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void ShortList::clear()
{
    size = 0;
    std::fill(indexArr, indexArr + MAX_SIZE, -1); // Invalid index
    std::fill(arr, arr + MAX_SIZE, -1);           // Invalid index
}