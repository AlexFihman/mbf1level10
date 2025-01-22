#pragma once

#include <random>
#define DIMENSION 10

class ShortList
{
private:
    static const int MAX_SIZE = 1 << DIMENSION;
    int arr[MAX_SIZE];
    int indexArr[MAX_SIZE];
    int size;

public:
    ShortList();

    bool insert(int num);

    bool remove(int num);

    int getRandomElement(std::mt19937 &rng) const;

    int getSize() const;

    int getValue(int pos) const;

    void clear();

    void print() const;
};
