#pragma once
#ifndef RAND_H
#define RAND_H

#include <random>

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

    unsigned int uRand(unsigned int min, unsigned int max) 
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
        return dist(rng);
    }
    private:
	std::mt19937 rng; 
};

#endif // RAND_H