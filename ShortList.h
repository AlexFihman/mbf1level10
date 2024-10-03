#pragma once

#include <random> // For std::mt19937

class ShortList
{
private:
    static const int MAX_SIZE = 1024; // Maximum size of the list
    int arr[MAX_SIZE];                // Array to store integers
    int indexArr[MAX_SIZE];           // Array to store the index of each integer
    int size;                         // Current size of the list

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
