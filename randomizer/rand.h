#pragma once
#ifndef RAND_H
#define RAND_H

#include <random>
#include <algorithm>

#include "common/common.h"

class Randomizer
{
    public:
    Randomizer() : rng(std::random_device()())
    {}

    int rand(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    SizeType uRand(SizeType min, SizeType max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
        return dist(rng);
    }

    template<class T>
    void shuffle(List<T>& target)
        { std::shuffle(target.begin(), target.end(), rng); }

    private:
	std::mt19937 rng; 
};

#endif // RAND_H