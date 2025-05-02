#include <algorithm>

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
                                              { return weights[left] > weights[right]; });

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
void Spanner::clearComps(List<Set<SizeType>*>& comps)
{
    delete comps[0];
}
