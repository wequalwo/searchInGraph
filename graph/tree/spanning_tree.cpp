#include <algorithm>

#include "graph/tree/spanning_tree.h"

List<WNode> Spanner::pSpan(const List<WNode>& graph) // Prim algorithm
{
    Set<SizeType> curSpan;
    List<WNode> res;
    for (SizeType i = 0; i < graph.size(); ++i)
        res.push_back(WNode{i, Map<SizeType, double>{}});
    curSpan.insert(0);
    while (curSpan.size() < graph.size())
    {
        double minW = -1;
        EdgeType minEdge;
        for (const auto& node : curSpan)
            for (const auto& adjV : graph[node].incident)
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
    
    std::sort(edges.begin(), edges.end(), [&weights](EdgeType left, EdgeType right)
                                              { return weights[left] > weights[right]; });

    List<Set<SizeType>*> comps;
    initComps(comps, graph.size());

    List<WNode> res;
    for (SizeType i = 0; i < graph.size(); ++i)
        res.push_back(WNode{i, Map<SizeType, double>{}});
    for (const auto& edge : edges)
        if (comps[edge.first] != comps[edge.second])
        {
            mergeComps(comps, edge.first, edge.second);
            res[edge.first].incident[edge.second] = weights[edge];
            res[edge.second].incident[edge.first] = weights[edge];
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
    
    comps[first]->insert(comps[second]->begin(), comps[second]->end());
    Set<SizeType>* pToDelete = comps[second];

    for (auto& pComp : comps)
        if (pComp == pToDelete)
            pComp = comps[first];

    delete pToDelete;
}

void Spanner::clearComps(List<Set<SizeType>*>& comps)
{
    delete comps[0];
}
