#include <iostream>
#include "tree/spanning_tree.h"
#include "monte_carlo/monte_carlo.h"
#include "monte_carlo/monte_carlo_trees.h"

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
    // int p = 6;
    // List<WNode> graph(p);
    // for (SizeType i = 0; i < p; ++i)
    //     graph[i].data = i;

    // // manual input to check
    // graph[0].incident[1] = 7;
    // graph[1].incident[0] = 7;

    // graph[0].incident[2] = 8;
    // graph[2].incident[0] = 8;

    // graph[1].incident[2] = 11;
    // graph[2].incident[1] = 11;

    // graph[1].incident[3] = 2;
    // graph[3].incident[1] = 2;

    // graph[2].incident[4] = 9;
    // graph[4].incident[2] = 9;

    // graph[2].incident[3] = 6;
    // graph[3].incident[2] = 6;

    // graph[3].incident[4] = 11;
    // graph[4].incident[3] = 11;

    // graph[3].incident[5] = 9;
    // graph[5].incident[3] = 9;

    // graph[4].incident[5] = 10;
    // graph[5].incident[4] = 10;

    // Spanner sp;
    // SizeType waste = 0;
    // std::cout << "Prim's MST:\n";
    // auto primTree = sp.primSpan(graph, waste);
    // printGraph(primTree);
    // std::cout << "\nPrim's waste: " << waste << "\n\n";

    // waste = 0;
    // std::cout << "Kruskal's MST:\n";
    // auto kruskalTree = sp.kruskalSpan(graph, waste);
    // printGraph(kruskalTree);
    // std::cout << "\nKruskal's waste: " << waste << "\n\n";
    List<double> dens = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    std::string log = "logger/log", err = "logger/err";
    int i = 0;
    MonteCarloTrees mc(5, 40, 5, dens, std::make_unique<SpanningLogger>(log, err, i));
    mc.initErdosRenyi();
    mc.runMonteCarlo();

    return 0;
}