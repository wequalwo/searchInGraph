#include <iostream>
#include "tree/spanning_tree.h"

void printGraph(const List<WNode>& tree) {
    for (const auto& node : tree) {
        for (const auto& [to, weight] : node.incident) {
            if (node.data < to) // to prevent edge duplication
                std::cout << node.data << " -- " << to << " [weight=" << weight << "]\n";
        }
    }
    std::cout << "-------------------------\n";
}

int main() {
    int p = 6;
    List<WNode> graph(p);
    for (SizeType i = 0; i < p; ++i)
        graph[i].data = i;

    // manual input to check
    graph[0].incident[1] = 7;
    graph[1].incident[0] = 7;

    graph[0].incident[2] = 8;
    graph[2].incident[0] = 8;

    graph[1].incident[2] = 11;
    graph[2].incident[1] = 11;

    graph[1].incident[3] = 2;
    graph[3].incident[1] = 2;

    graph[2].incident[4] = 9;
    graph[4].incident[2] = 9;

    graph[2].incident[3] = 6;
    graph[3].incident[2] = 6;

    graph[3].incident[4] = 11;
    graph[4].incident[3] = 11;

    graph[3].incident[5] = 9.0;
    graph[5].incident[3] = 9.0;

    graph[4].incident[5] = 10;
    graph[5].incident[4] = 10;

    Spanner sp;

    std::cout << "Prim's MST:\n";
    auto primTree = sp.pSpan(graph);
    printGraph(primTree);

    std::cout << "Kruskal's MST:\n";
    auto kruskalTree = sp.kSpan(graph);
    printGraph(kruskalTree);

    return 0;
}
