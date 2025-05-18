#pragma once
#ifndef MONTE_CARLO_COLORER_H
#define MONTE_CARLO_COLORER_H

#include "monte_carlo/monte_carlo.h"
#include "logger/coloring_logger.h"

class MonteCarloColorer : public MonteCarlo
{
public:
    MonteCarloColorer(int numGraphs, int numVertices, int numRuns, const List<double>& densities, std::unique_ptr<ColoringLogger>&& pLogger)
        : MonteCarlo(numGraphs, numVertices,numRuns,  densities, std::move(pLogger))
    {}

    MonteCarloColorer(const MonteCarloColorer& other) = delete;
    MonteCarloColorer(MonteCarloColorer&& other) = default;

    virtual void runMonteCarlo() override;

    // Очищение графов и результатов
    virtual void clear() override;

private:
    // логирование результатов
    void logResults() override;

    List<uint32_t> m_tries;
    SizeType m_naiveColors;
    SizeType m_greedyColors;
};

#endif // MONTE_CARLO_TRAVERSAL_H