#pragma once
#ifndef MONTE_CARLO_TRAVERSAL_H
#define MONTE_CARLO_TRAVERSAL_H

#include "monte_carlo/monte_carlo.h"
#include "logger/traversal_logger.h"

class MonteCarloTraversal : public MonteCarlo
{
public:
    MonteCarloTraversal(int numGraphs, int numVertices, int numRuns, const List<double>& densities, std::unique_ptr<TraversalLogger>&& pLogger)
        : MonteCarlo(numGraphs, numVertices,numRuns,  densities, std::move(pLogger))
    {}

    MonteCarloTraversal(const MonteCarloTraversal& other) = delete;
    MonteCarloTraversal(MonteCarloTraversal&& other) = default;

    virtual void runMonteCarlo() override;

    // Очищение графов и результатов
    virtual void clear() override;

    // Геттеры для результатов
    const List<int>& getBFSResults() const;
    const List<int>& getDFSResults() const;

private:
    // Метод для выполнения поиска пути на графе
    void searchPath(bool auto_inv = true);

    // логирование результатов
    void logResults() override;


    List<int> m_bfsResults;        // Результаты поиска в ширину
    List<int> m_dfsResults;        // Результаты поиска в глубину
    List<int> m_dist;              // Геодезическое расстояние

};

#endif // MONTE_CARLO_TRAVERSAL_H