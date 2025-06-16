#ifndef GRAPH_RAND_WEIGHT_H
#define GRAPH_RAND_WEIGHT_H

#include "randomizer/rand.h"

// independent distributed weights
List<double> normal_weights(double mean, double dev, int count)
{
    List<double> res;
    Randomizer rand;
    res.reserve(count);
    for (SizeType i = 0; i < count; ++i)
        res.push_back(rand.randNorm(mean, dev));

    return res;
}


List<double> uniform_weights(double min, double max, int count)
{
    List<double> res;
    Randomizer rand;
    res.reserve(count);
    for (SizeType i = 0; i < count; ++i)
        res.push_back(rand.uniformRand(min, max)); //rand.randNorm(mean, dev));

    return res;
}


List<double> ones_weights(int count)
{
    return List<double>(count, 1.0);
}

// weights in range: [min, min + 1, ..., max - 1, max]
List<double> range_weights(SizeType min, SizeType max)
{
    List<double> res;
    res.reserve(max - min + 1);
    for (SizeType i = min; i <= max; ++i)
        res.push_back(i);
    return res;
}

// repeating weights w1, w2, ..., wn, w1, w2, ... until total number = count
List<double> repeated_weights(const List<double>& weights, SizeType count)
{
    List<double> res;
    res.reserve(count);
    SizeType toRepeat = count / weights.size();
    SizeType trailing = count % weights.size();
    for (SizeType i = 0; i < toRepeat; ++i)
        res.insert(res.end(), weights.begin(), weights.end());
    for (SizeType i = 0; ++i; i < trailing)
        res.push_back(weights[i]);

    return res;
}

// select a value randomly uniformly from a list count times
List<double> sample_weights(const List<double>& weights, SizeType count)
{
    List<double> res;
    Randomizer rand;
    res.reserve(count);
    for (SizeType i = 0; i < count; ++i)
        res.push_back(weights[rand.uRand(0, weights.size() - 1)]);
    return res;
}

List<double> sample_bernulli(double p, uint32_t count)
{
    List<double> res;
    Randomizer rand;
    res.reserve(count);
    for (SizeType i = 0; i < count; ++i)
    {
        if(rand.uniformRand(0, 1) < p)
            res.push_back(1);
        else
            res.push_back(0.5);
    }
    return res;
}

List<double> sample_binomial(double p, int n, uint32_t count)
{
    std::mt19937 gen(1729);
    std::binomial_distribution<> distr(n, p);
    std::cout << count <<"\n";
    List<double> res;
    res.reserve(count);

    for (SizeType i = 0; i < count; ++i)
        res.push_back(distr(gen));
    return res;
}

#endif // GRAPH_RAND_WEIGHT_H