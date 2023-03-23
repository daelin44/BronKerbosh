
#ifndef GRAPHVIZ_UTILITY_H
#define GRAPHVIZ_UTILITY_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

class GraphvizUtility {
public:
    GraphvizUtility(bool directed = true);
    ~GraphvizUtility() = default;

    // Adds a node to the visualizer. If a node with the same ID is
    // present, it and its edges are removed, and the new node takes its place.
    void add_node(int id, std::string label, std::string options = "");

    // Updates the node for the given id without removing its edges.
    void edit_node(int id, std::string label, std::string options = "");

    // Remove the node and all of its edges
    void remove_node(int id);

    // Add an edge connecting two nodes by their IDs
    void add_edge(int from_id, int to_id, std::string label);

    // Remove all edges between two nodes
    void remove_edges(int id_1, int id_2);

    // Remove edges between two nodes by direction
    void remove_edges_directed(int from_id, int to_id);

    // save the graph to Graphviz .dot format
    void save(std::string filename);

    // Clear the nodes in the graph
    void clear_graph();

private:
    struct GraphvizEdge {
        int to_id;
        std::string label;
    };

    struct GraphvizNode {
        int id;
        std::vector<struct GraphvizEdge> edges;
        std::vector<int> incoming_edges;
        std::string label;
        std::string options;

        GraphvizNode(const int& s_id = 0)
            : id(s_id) {
        }
        bool operator==(const GraphvizNode& a) const {
            return (id == a.id);
        }
        bool operator<(const GraphvizNode& a) const {
            return (id < a.id);
        }
    };

private:
    std::map<int, struct GraphvizNode> m_nodes;
    bool m_directed;
};

#endif
