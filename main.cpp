#include "graph.h"
#include <iostream>

int main() {
    std::cout << "Enter the size of adjacency matrix: ";
    std::size_t vertexes_num;
    std::cin >> vertexes_num;

    Graph graph(vertexes_num);
    bool is_connected;

    std::cout
        << "Enter the adjacency matrix cell by cell with space-separation:\n";

    for (std::size_t i = 0; i < vertexes_num; ++i) {
        for (std::size_t j = 0; j < vertexes_num; ++j) {
            std::cin >> is_connected;
            if (is_connected) {
                graph.add_edge(i, j);
            }
        }
    }

    auto cliques = graph.find_all_cliques();
    std::cout << "\nCliques found: \n";
    for (std::size_t i = 0; i < cliques.size(); ++i) {
        std::cout << i + 1 << ". {" << cliques[i] << "}" << std::endl;
    }
    std::cout << std::endl;

    auto compare_cliques = [](Graph& a, Graph& b) {
        return a.size() < b.size();
    };
    auto max_clique =
        *std::max_element(cliques.begin(), cliques.end(), compare_cliques);

    graph.mark_subgraph(max_clique);

    graph.save_to_png("graph_with_max_clique.png");
    return 0;
}
