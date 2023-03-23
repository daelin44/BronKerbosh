#ifndef GRAPH_H
#define GRAPH_H

#include "graphviz_utility.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <set>
#include <stdio.h>
#include <vector>

class Graph {
private:
    struct Vertex {
        std::size_t id{0};
        std::string label;
        std::set<std::size_t> neighbours;
        std::string color{"black"};
    };

public:
    Graph(std::size_t vertexes_num);
    ~Graph() = default;

    void add_edge(std::size_t from_i, std::size_t to_i);
    std::size_t size();

    void mark_subgraph(Graph& graph);

    std::vector<Graph> find_all_cliques();

    void save_to_png(std::string file_name);
    friend std::ostream& operator<<(std::ostream& out, Graph& graph);

private:
    Graph(const Graph& other, std::set<std::size_t>);
    void extend(std::set<std::size_t>& candidates,
                std::set<std::size_t>& not_candidates,
                std::set<std::size_t>& compsub, std::vector<Graph>& result);

private:
    std::vector<Vertex> m_vertexes;
};

std::ostream& operator<<(std::ostream& out, Graph& graph);

#endif // GRAPH_H
