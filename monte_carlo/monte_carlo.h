#pragma once
#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H

#include "common/common.h"
#include "graph/tree/rand_tree.h"
#include "graph/traversal/traversal.h"
#include "logger/logger.h"

class MonteCarlo {
public:
    MonteCarlo(int numVertices, int numSearches, const std::string& log, const std::string& err, SizeType mcIdx);

    MonteCarlo(const MonteCarlo& other) = delete;
    MonteCarlo(MonteCarlo&& other) = default;

    // Очищение графов и результатов
    void clear();

    // Геттеры для результатов
    const List<int>& getBFSResults() const;
    const List<int>& getDFSResults() const;

    // Инициализация алгоритма, запускает метод
    void initHilbert(int numGraphs, double pi);
    void initErdosRenyi(const List<double>& densities, int numGraphs);

private:
    // Метод для построения графа
    List<Node> buildGraph(int numEdges, double density);

    // Метод для выполнения поиска пути на графе
    void searchPath(double curDensity, bool auto_inv = true);

    // логирование результатов
    void logResults(int graphIndex, double density, int searchIndex);

    int m_numVertices;                    // Количество вершин в графе
    int m_numSearches;                    // Количество поисков на каждом графе

    List<Node> m_graph;                   // Граф
    List<int> m_bfsResults;        // Результаты поиска в ширину
    List<int> m_dfsResults;        // Результаты поиска в глубину
    List<int> m_dist;              // Геодезическое расстояние
    // TODO: добавить доп. данные методов

    Logger m_logger;
};

#endif // MONTE_CARLO_H