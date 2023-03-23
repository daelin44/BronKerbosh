#include "graphviz_utility.h"

GraphvizUtility::GraphvizUtility(bool directed) {
    m_directed = directed;
}

void GraphvizUtility::add_node(int id, std::string label, std::string options) {
    remove_node(id);
    struct GraphvizNode node;
    node.id = id;
    node.label = label;
    node.options = options;

    std::pair<int, struct GraphvizNode> node_pair(id, node);
    m_nodes.insert(node_pair);
}

void GraphvizUtility::edit_node(int id, std::string label,
                                std::string options) {
    std::map<int, struct GraphvizNode>::iterator it;
    it = m_nodes.find(id);

    if (it != m_nodes.end()) {
        m_nodes[id].label = label;
        m_nodes[id].options = options;
    }
}

void GraphvizUtility::remove_node(int id) {
    std::map<int, struct GraphvizNode>::iterator it;
    it = m_nodes.find(id);

    if (it != m_nodes.end()) {
        for (unsigned int w = 0; w < m_nodes[id].incoming_edges.size(); w++) {
            int from_id = m_nodes[id].incoming_edges[w];
            remove_edges_directed(from_id, id);
        }
        for (unsigned int w = 0; w < m_nodes[id].incoming_edges.size(); w++) {
            int to_id = m_nodes[id].edges[w].to_id;
            remove_edges_directed(id, to_id);
        }
        m_nodes.erase(id);
    }
}

void GraphvizUtility::add_edge(int from_id, int to_id, std::string label) {
    std::map<int, struct GraphvizNode>::iterator from_it, to_it;
    from_it = m_nodes.find(from_id);
    to_it = m_nodes.find(to_id);

    if (from_it != m_nodes.end() && to_it != m_nodes.end()) {
        struct GraphvizEdge edge;
        edge.to_id = to_id;
        edge.label = label;
        m_nodes[from_id].edges.push_back(edge);
        m_nodes[to_id].incoming_edges.push_back(from_id);
    }
}

void GraphvizUtility::remove_edges(int id_1, int id_2) {
    remove_edges_directed(id_1, id_2);
    remove_edges_directed(id_2, id_1);
}

void GraphvizUtility::remove_edges_directed(int from_id, int to_id) {
    unsigned int x = 0;
    while (x < m_nodes[from_id].edges.size()) {
        if (m_nodes[from_id].edges[x].to_id == to_id) {
            m_nodes[from_id].edges.erase(m_nodes[from_id].edges.begin() + x);
        } else {
            x++;
        }
    }
    x = 0;
    while (x < m_nodes[to_id].incoming_edges.size()) {
        if (m_nodes[to_id].incoming_edges[x] == from_id) {
            m_nodes[to_id].incoming_edges.erase(
                m_nodes[to_id].incoming_edges.begin() + x);
        } else {
            x++;
        }
    }
}

void GraphvizUtility::save(std::string filename) {
    std::ofstream out(filename);

    std::string edge_repr;
    if (m_directed) {
        out << "\ndigraph{";
        edge_repr = "->";
    } else {
        out << "\ngraph{";
        edge_repr = "--";
    }

    for (auto& node : m_nodes) {
        out << "\n_" << node.second.id << " [ " << node.second.options
            << " label=\"" << node.second.label << "\"];";
    }
    for (auto& node : m_nodes) {
        for (auto& edge : node.second.edges) {
            out << "\n_" << node.second.id << " " << edge_repr << " _"
                << edge.to_id << " [label=\"" << edge.label << "\"];";
        }
    }
    out << "\n}";
    out.close();
}

void GraphvizUtility::clear_graph() {
    m_nodes.clear();
}
