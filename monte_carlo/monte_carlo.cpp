#include "monte_carlo.h"
#include "graph/edge.h"
#include "prufer_graph/prufer.h"
#include "prufer_graph/random_graph.h"
/**
 * @brief Конструктор класса MonteCarlo
 * 
 * @param densities Вектор плотностей графов, которые будут сгенерированы
 * @param numVertices Количество вершин в каждом графе
 * @param numGraphs Количество графов для генерации
 * @param numSearches Количество поисков пути на каждом графе
 * @param log Объект класса Logger для логирования информации и ошибок
 */

MonteCarlo::MonteCarlo(const List<double>& densities, int numVertices, int numGraphs, int numSearches, Logger& log)
    : m_densities(densities), m_numVertices(numVertices), m_numGraphs(numGraphs),
    m_numSearches(numSearches), m_logger(log)
{}

void MonteCarlo::clear() {
    m_graph.clear();
    m_bfsResults.clear();
    m_dfsResults.clear();
    m_dist.clear();
}

// Геттеря для получения результататов о работе программы
const List<int>& MonteCarlo::getBFSResults() const {
    return m_bfsResults;
}

const List<int>& MonteCarlo::getDFSResults() const {
    return m_dfsResults;
}

// Инициализация алгоритма
void MonteCarlo::initialize() {
    for (double curDensity : m_densities)
        for (int graphIndex = 0; graphIndex < m_numGraphs; ++graphIndex) {
        {
            // TODO разделить методы: надо получать не только эти данные
            try
            {
                m_graph = buildGraph(m_numVertices, curDensity);
            }
            catch (std::exception& exc)
            {
                m_logger.errBuild(exc.what(), m_numVertices, curDensity);
            }
            
            for (int searchIndex = 0; searchIndex < m_numSearches; ++searchIndex) {
                // Выполняем поиск пути и обновляем результаты
                searchPath(curDensity);
                
                // Логируем результаты после каждого поиска
                logResults(graphIndex, curDensity, searchIndex);
            }
            std::cout << "\n";
        }
        clear();
    }
}

/**
 * @brief Метод для генерации графа
 * 
 * @param numEdges количество рёбер в графе
 * @param density плотность графа
 * @return список вершин графа
 * 
 * @details
 * 1. Генерирует дерево на numEdges рёбрах
 * 2. Устанавливает плотность графа density
 * 3. Возвращает список вершин
 */
List<Node> MonteCarlo::buildGraph(int numEdges, double density) {

    // TODO : переделать на вызов наиболее оптимального метода
    List<Node> nodes = get_tree(numEdges);
    
    setGraphDensity(nodes, density);
    std::cout << "\n";
    for (const auto &node : nodes) {
        std::cout << "Node " << node.data << ": ";
        for (SizeType neighbor : node.incident) {
            std::cout << neighbor << " ";
        }
        std::cout << "\n";
    }
    return nodes;
}

// Поиск пути на графе (в текущем графе)
void MonteCarlo::searchPath(double curDensity) {

    Randomizer rand;
    Traverser traverser(&m_graph);
    
    SizeType from = rand.uRand(0, m_graph.size() - 1);
    SizeType to = from;

    while (to == from)
        to = rand.uRand(0, m_graph.size() - 1);

    std::cout << "From " << from << " to " << to << std::endl;

    try
    {
        std::cout << "BFS\n";
        traverser.traverse<std::queue<SizeType>>(from, to, curDensity);  // BFS
        m_bfsResults.push_back(traverser.getTraverseOrder().size());
        m_dist.push_back(traverser.getPath().size());
        traverser.clear();
    }
    catch (std::exception& exc)
    {
        m_logger.errSearch(exc.what(), m_numVertices, curDensity, from, to, "BFS");
        m_logger.logErrGraph(m_graph);
    }

    try
    {
        std::cout << "DFS\n";

        traverser.traverse<std::stack<SizeType>>(from, to, curDensity);  // DFS
        m_dfsResults.push_back(traverser.getTraverseOrder().size());
    }
    catch (std::exception& exc)
    {
        m_logger.errSearch(exc.what(), m_numVertices, curDensity, from, to, "DFS");
        m_logger.logErrGraph(m_graph);
    }
}

// Логирование результатов
void MonteCarlo::logResults(int graphIndex, double density, int searchIndex) {

    m_logger.log(m_graph.size(), density, m_dist.back(), getBFSResults().back(), getDFSResults().back());
    // TODO правильное логирование с ипользование геттеров --- done
}