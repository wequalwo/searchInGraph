#include "monte_carlo.h"
#include "graph/edge.h"

MonteCarlo::MonteCarlo(const std::vector<double>& densities, int numEdges, int numGraphs, int numSearches)
    : m_densities(densities), m_numEdges(numEdges), m_numGraphs(numGraphs), m_numSearches(numSearches) {}

void MonteCarlo::clear() {
    m_graph.clear();
    m_bfsResults.clear();
    m_dfsResults.clear();
}

const std::vector<int>& MonteCarlo::getBFSResults() const {
    return m_bfsResults;
}

const std::vector<int>& MonteCarlo::getDFSResults() const {
    return m_dfsResults;
}

// Инициализация алгоритма
void MonteCarlo::initialize() {
    for (int graphIndex = 0; graphIndex < m_numGraphs; ++graphIndex) {
        // Выбираем плотность для текущего графа
        double currentDensity = m_densities[graphIndex % m_densities.size()];
        // TODO разделить методы: надо получать не только эти данные
        m_graph = buildGraph(m_numEdges, currentDensity);
        
        for (int searchIndex = 0; searchIndex < m_numSearches; ++searchIndex) {
            // Выполняем поиск пути и обновляем результаты
            searchPath(graphIndex);
            
            // Логируем результаты после каждого поиска
            logResults(graphIndex, searchIndex);
        }
    }
}

List<Node> MonteCarlo::buildGraph(int numEdges, double density) {

    // TODO : переделать на вызов наиболее оптимального метода
    List<Node> nodes = get_tree(numEdges);
    
    setGraphDensity(nodes, density);
    
    return nodes;
}

// Поиск пути на графе (в текущем графе)
void MonteCarlo::searchPath(int graphIndex) {

    Traverser traverser(&m_graph);
    
    traverser.traverseRand<std::queue<SizeType>>(true);  // BFS
    m_bfsResults.push_back(traverser.getTraverseOrder().size());

    traverser.clear();
    traverser.traverseRand<std::stack<SizeType>>(false);  // DFS
    m_dfsResults.push_back(traverser.getTraverseOrder().size());
}

// Логирование результатов
void MonteCarlo::logResults(int graphIndex, int searchIndex) {
    // TODO правильное логирование с ипользование геттеров
}