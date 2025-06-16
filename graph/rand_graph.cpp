#include <chrono>

#include "graph/rand_graph.h"
#include "randomizer/rand.h"

// генерируем пары случайных чисел и переводим их в ребра
void erdos_renyi_graph(List<Node>& tree, unsigned int edgesToAdd)
{
    Randomizer rand;
    while (edgesToAdd > 0)
    {
        SizeType firstInd = rand.uRand(0, tree.size() - 1);
        SizeType secondInd = rand.uRand(0, tree.size() - 1);

        if (checkEdgeInsertable(tree, firstInd, secondInd)) // пропускаем петли и уже существующие ребра
        {
            insertEdge(tree, firstInd, secondInd);
            if (--edgesToAdd == 0)
                return;
        }
    }
}

void inverseGraph(List<Node>& tree, double density)
{
    auto edges = getTreeEdges(tree);
    for (auto& elem : tree)
        elem.incident.clear();

    unsigned int maxEdges = tree.size() * (tree.size() - 1)/2;
    unsigned int edgesToRemove = std::round(maxEdges * (1-density));
    Randomizer rand;
    while (edgesToRemove > 0)
    {
        SizeType firstInd = rand.uRand(0, tree.size() - 1);
        SizeType secondInd = rand.uRand(0, tree.size() - 1);
        if (edges.count({firstInd, secondInd}) == 0) // удаляем ребра, которых изначально не было
            if (checkEdgeInsertable(tree, firstInd, secondInd)) // пропускаем петли и уже удаленные ребра
            {
                insertEdge(tree, firstInd, secondInd);
                if (--edgesToRemove == 0)
                    return;
            }
    }
}

void setGraphDensity(List<Node>& tree, double density)
{
    if (density >= MIN_INVERSE_DENSITY)
    {
        inverseGraph(tree, density);
        return;
    }
    SizeType curEdges = tree.size() - 1;
    unsigned int maxEdges = tree.size()*(tree.size() - 1)/2;
    unsigned int needMinEdges = std::round(maxEdges * density);
    if (curEdges >= needMinEdges)
        return;
    unsigned int needEdges = needMinEdges - curEdges; // считаем, сколько ребер добавить

    using Clock = std::chrono::steady_clock;

    //std::cerr << "starting edges generating\n";
    Clock::time_point begin = Clock::now();
    erdos_renyi_graph(tree, needEdges);
    Clock::time_point end = Clock::now();
    //std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mcs] = "
    //          << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';
}

List<Node> hilbert_graph(SizeType size, double pi, int& num_edges)
{
    List<Node> graph;
    graph.reserve(size);
    for (SizeType i = 0; i < size; ++i)
        graph.push_back(Node{i, Set<SizeType>{}});
    
    num_edges = 0;
    Randomizer rand;
    
    for (SizeType i = 0; i < size - 1; ++i)
        for (SizeType j = i + 1; j < size; ++j)
            if (rand.randProb(pi))
            {
                graph[i].incident.insert(j);
                graph[j].incident.insert(i);
                ++num_edges;
            }
    std::cout << "number of edges: " << num_edges << '\n';
    std::cout << "density: " << double(num_edges) / (size * (size - 1) / 2) << '\n';
    return graph;
}

// distribute weignts among edges in random order
// List<WNode> to_rand_wgraph(const List<Node>& graph, const List<double>& weights)
// {
//     int edges = 0;
//     for (const auto& node : graph)
//         edges += node.incident.size();
//     if (edges / 2 != weights.size())
//     {
//         std::cout << "edges: " << edges/2 << " weights: " << weights.size() <<'\n';
//         throw std::logic_error("invalid weights number");
//     }
    
//     List<unsigned int> edgesInds;
//     for (const auto& node : graph)
//     {
//         for (const auto& adjNode : node.incident)
//             if (node.data < graph[adjNode].data)
//                 edgesInds.push_back(index_from_pair(graph.size(), node.data, graph[adjNode].data));
//     }
//     if (edgesInds.size() != weights.size())
//         throw std::logic_error("invalid weights number");

//     Randomizer rand;
//     List<double> copyWeights(weights);
//     rand.shuffle(copyWeights);

//     List<WNode> wGraph;
//     for (SizeType i = 0; i < graph.size(); ++i)
//         wGraph.push_back(WNode(i, Map<SizeType, double>{}));
//     //std::cout << "3\n";

//     for (SizeType i = 0; i < edgesInds.size(); ++i)
//     {
//         auto edgePair = pair_from_index(edgesInds.at(i), graph.size());
//         //std::cout << "3.2\n";
//         try{
//             wGraph.at(edgePair.first).incident[edgePair.second] = copyWeights.at(i);
//             wGraph.at(edgePair.second).incident[edgePair.first] = copyWeights.at(i);
//         }
//         catch (std::out_of_range& e)
//         {
//             std::cerr << "ERROR: " << e.what() << '\n';
//             std::cout << edgesInds.at(i) << '\n';
//             exit(1);
//         }
//     }
//     //std::cout << "4\n";

//     return wGraph;
// }

List<WNode> to_rand_wgraph(const List<Node>& graph, const List<double>& weights)
{
    int expected_num_edges = 0;
    for (SizeType i = 0; i < graph.size(); ++i)
    {
        for (SizeType j : graph[i].incident)
            if (i < j)
                ++expected_num_edges;
    }

    if (expected_num_edges != weights.size())
    {
        std::cout << "Expected edges: " << expected_num_edges << ", but got weights: " << weights.size() << '\n';
        throw std::logic_error("Mismatch between number of edges and weights");
    }

    // Создаем пустой взвешенный граф
    List<WNode> wGraph;
    for (SizeType i = 0; i < graph.size(); ++i)
        wGraph.push_back(WNode(i, Map<SizeType, double>{}));

    // Присваиваем веса
    SizeType weightIdx = 0;
    for (SizeType i = 0; i < graph.size(); ++i)
    {
        for (SizeType j : graph[i].incident)
        {
            if (i < j)
            {
                double weight = weights[weightIdx++];
                wGraph[i].incident[j] = weight;
                wGraph[j].incident[i] = weight;
            }
        }
    }

    return wGraph;
}

List<WNode> to_rand_wgraph(const List<Node>& graph)
{
    int expected_num_edges = 0;
    for (SizeType i = 0; i < graph.size(); ++i)
    {
        for (SizeType j : graph[i].incident)
            if (i < j)
                ++expected_num_edges;
    }

    // Создаем пустой взвешенный граф
    List<WNode> wGraph;
    for (SizeType i = 0; i < graph.size(); ++i)
        wGraph.push_back(WNode(i, Map<SizeType, double>{}));


    int n = 5000;
    double p =  0.5;
    std::mt19937 gen(1729);
    double weight = 0;
    std::binomial_distribution<> distr(n, p);

    // Присваиваем веса
    SizeType weightIdx = 0;
    for (SizeType i = 0; i < graph.size(); ++i)
    {
        for (SizeType j : graph[i].incident)
        {
            if (i < j)
            {
                weight = distr(gen);
                wGraph[i].incident[j] = weight;
                wGraph[j].incident[i] = weight;
            }
        }
    }

    return wGraph;
}