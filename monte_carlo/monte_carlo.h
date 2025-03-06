#pragma once
#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H

#include "common/common.h"
#include "graph/tree.h"
#include "graph/traversal.h"
#include "logger/logger.h"

class MonteCarlo {
public:
    MonteCarlo(const List<double>& densities, int numVertices, int numGraphs, int numSearches, Logger& log);

    // Очищение графов и результатов
    void clear();

    // Геттеры для результатов
    const List<int>& getBFSResults() const;
    const List<int>& getDFSResults() const;

    // Инициализация алгоритма, запускает метод
    void initialize();

private:
    // Метод для построения графа
    List<Node> buildGraph(int numEdges, double density);

    // Метод для выполнения поиска пути на графе
    void searchPath(double curDensity);

    // логирование результатов
    void logResults(int graphIndex, double density, int searchIndex);

    List<double> m_densities;      // Вектор плотностей
    int m_numVertices;                    // Количество вершин в графе
    int m_numGraphs;                      // Количество графов для генерации
    int m_numSearches;                    // Количество поисков на каждом графе

    List<Node> m_graph;                   // Граф
    List<int> m_bfsResults;        // Результаты поиска в ширину
    List<int> m_dfsResults;        // Результаты поиска в глубину
    SizeType m_dist;               // Геодезическое расстояние 
    // TODO: добавить доп. данные методов

    Logger& m_logger;
};

#endif // MONTE_CARLO_H