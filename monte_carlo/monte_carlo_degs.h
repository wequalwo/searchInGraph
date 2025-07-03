#pragma once
#ifndef MONTE_CARLO_DEGS_H
#define MONTE_CARLO_DEGS_H

#include "monte_carlo/monte_carlo.h"
#include "logger/degree_logger.h"

class MonteCarloDegs : public MonteCarlo
{
public:
    MonteCarloDegs(int numGraphs, int numVertices, int numRuns, const List<double>& densities, std::unique_ptr<DegreeLogger>&& pLogger)
        : MonteCarlo(numGraphs, numVertices, numRuns, densities, std::move(pLogger))
    {}

    MonteCarloDegs(const MonteCarloDegs& other) = delete;
    MonteCarloDegs(MonteCarloDegs&& other) = default;

    virtual void runMonteCarlo() override;

    // Очищение графов и результатов
    virtual void clear() override;

    static Map<SizeType, SizeType> countDegrees(const List<Node>& graph);
private:
    // логирование результатов
    void logResults() override;

    Map<SizeType, SizeType> m_degsCounter;
};

#endif // MONTE_CARLO_DEGS_H