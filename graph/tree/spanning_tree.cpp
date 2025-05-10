#include <algorithm>
#include <queue> // see my functions below
#include <limits>// see my functions below

#include "graph/tree/spanning_tree.h"

List<WNode> Spanner::pSpan(const List<WNode>& graph) // Prim algorithm
{
    Set<SizeType> curSpan;
    List<WNode> res;
    for (SizeType i = 0; i < graph.size(); ++i)
        res.push_back(WNode{i, Map<SizeType, double>{}});
    // start from vertex 0
    curSpan.insert(0);
    while (curSpan.size() < graph.size())
    {
        double minW = -1;
        EdgeType minEdge;
        // for all verts in span
        for (const auto& node : curSpan)
            // check edges
            for (const auto& adjV : graph[node].incident)
                // select cheapest edge with one vertex not in span
                if (curSpan.count(adjV.first) == 0 && (adjV.second < minW || minW == -1))
                {
                    minEdge = EdgeType{node, adjV.first};
                    minW = adjV.second;
                }

        curSpan.insert(minEdge.second);
        res[minEdge.first].incident[minEdge.second] = minW;
        res[minEdge.second].incident[minEdge.first] = minW;
    }

    return res;
} 

List<WNode> Spanner::kSpan(const List<WNode>& graph) // Kruskal algorithm
{
    List<EdgeType> edges;
    Map<EdgeType, double> weights;

    for (const auto& node : graph)
        for (const auto adjV : node.incident)
            if (node.data < adjV.first)
            {
                EdgeType edge{node.data, adjV.first};
                weights[edge] = adjV.second;
                edges.push_back(std::move(edge));
            }
    
    // sort edges according to their weights
    std::sort(edges.begin(), edges.end(), [&weights](EdgeType left, EdgeType right)
                                              { return weights[left] < weights[right]; });

    // use pointers for fast comparison
    List<Set<SizeType>*> comps;
    // each component consists of 1 vertex
    initComps(comps, graph.size());

    List<WNode> res;
    for (SizeType i = 0; i < graph.size(); ++i)
        res.push_back(WNode{i, Map<SizeType, double>{}});
        
    // starting from cheapest edge
    for (const auto& edge : edges)
        // if vertices are in different components
        if (comps[edge.first] != comps[edge.second])
        {
            mergeComps(comps, edge.first, edge.second);
            res[edge.first].incident[edge.second] = weights[edge];
            res[edge.second].incident[edge.first] = weights[edge];
            // if graph became connected
            if (comps[edge.first]->size() == graph.size())
                break;
        }
    clearComps(comps);
    
    return res;
}


void Spanner::initComps(List<Set<SizeType>*>& comps, SizeType size)
{
    for (SizeType i = 0; i < size; ++i)
        comps.push_back(new Set<SizeType>{i});
}

void Spanner::mergeComps(List<Set<SizeType>*>& comps, SizeType first, SizeType second)
{
    if (first > second)
        std::swap(first, second);
    
    // extend component 1 with verts from component 2
    comps[first]->insert(comps[second]->begin(), comps[second]->end());

    // we need to delete component with larger index
    Set<SizeType>* pToDelete = comps[second];
    // all nodes in component 2 now belong to component 1
    for (auto& pComp : comps)
        if (pComp == pToDelete)
            pComp = comps[first];

    // delete component 2
    delete pToDelete;
}

// all but one coponents must've been deleted 
// There is a memory leak here. Changed to the function below.
// void Spanner::clearComps(List<Set<SizeType>*>& comps)
// {

//     delete comps[0];
// }

void Spanner::clearComps(List<Set<SizeType>*>& comps)
{
    Set<Set<SizeType>*> uniqueSets;
    for (auto p : comps)
        uniqueSets.insert(p); // unique only

    for (auto p : uniqueSets)
        delete p;
}


/////////////////////////////////
// here is my try
/////////////////////////////////
List<WNode> Spanner::primSpan(const List<WNode>& graph)
{
    const SizeType n = graph.size();
    std::vector<bool> inTree(n, false);
    std::vector<double> minWeight(n, std::numeric_limits<double>::max());
    std::vector<SizeType> parent(n, -1);

    using PQItem = std::pair<double, SizeType>; // (weight, vertex)
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<>> pq;

    minWeight[0] = 0;
    pq.emplace(0, 0); // (weight, vertex)

    while (!pq.empty()) {
        SizeType u = pq.top().second;
        pq.pop();

        if (inTree[u]) continue;
        inTree[u] = true;

        for (const auto& [v, weight] : graph[u].incident) {
            if (!inTree[v] && weight < minWeight[v]) {
                minWeight[v] = weight;
                parent[v] = u;
                pq.emplace(weight, v);
            }
        }
    }

    List<WNode> res;
    for (SizeType i = 0; i < n; ++i)
        res.push_back(WNode{i, Map<SizeType, double>{}});

    for (SizeType v = 1; v < n; ++v) {
        SizeType u = parent[v];
        double w = graph[u].incident.at(v);
        res[u].incident[v] = w;
        res[v].incident[u] = w;
    }

    return res;
}


// using direct DSU: 

struct DSU {
    std::vector<int> parent;
    DSU(int n) : parent(n) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }
    bool merge(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        parent[px] = py;
        return true;
    }
};



List<WNode> Spanner::kruskalSpan(const List<WNode>& graph)
{
    List<EdgeType> edges;
    Map<EdgeType, double> weights;

    for (const auto& node : graph) {
        for (const auto& [v, w] : node.incident) {
            if (node.data < v) {
                EdgeType e{node.data, v};
                edges.push_back(e);
                weights[e] = w;
            }
        }
    }

    std::sort(edges.begin(), edges.end(), [&weights](const EdgeType& a, const EdgeType& b) {
        return weights[a] < weights[b];
    });

    List<WNode> res;
    for (SizeType i = 0; i < graph.size(); ++i)
        res.push_back(WNode{i, Map<SizeType, double>{}});

    DSU dsu(graph.size());
    for (const auto& e : edges) {
        if (dsu.merge(e.first, e.second)) {
            double w = weights[e];
            res[e.first].incident[e.second] = w;
            res[e.second].incident[e.first] = w;
        }
    }

    return res;
}
