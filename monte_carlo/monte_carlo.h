#pragma once

#include "common.h"
#include "graph/node.h"
#include "graph/tree.h"
#include "graph/traversal.h"

class MonteCarlo {
public:
    MonteCarlo(const std::vector<double>& densities, int numEdges, int numGraphs, int numSearches);

    // Очищение графов и результатов
    void clear();

    // Геттеры для результатов
    const std::vector<int>& getBFSResults() const;
    const std::vector<int>& getDFSResults() const;

    // Инициализация алгоритма, запускает метод
    void initialize();

private:
    // Метод для построения графа
    List<Node> buildGraph(int numEdges, double density);

    // Метод для выполнения поиска пути на графе
    void searchPath(int graphIndex);

    // логирование результатов
    void logResults(int graphIndex, int searchIndex);

    std::vector<double> m_densities;      // Вектор плотностей
    int m_numEdges;                       // Количество рёбер в графе
    int m_numGraphs;                      // Количество графов для генерации
    int m_numSearches;                    // Количество поисков на каждом графе

    List<Node> m_graph;                   // Граф
    std::vector<int> m_bfsResults;        // Результаты поиска в ширину
    std::vector<int> m_dfsResults;        // Результаты поиска в глубину
    // TODO: добавить доп. данные методов
};