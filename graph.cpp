#include "graph.h"

// конструктор для создания графа на vertex_num вершинах
Graph::Graph(std::size_t vertexes_num) {
    m_vertexes.resize(vertexes_num);
    for (std::size_t i = 0; i < m_vertexes.size(); ++i) {
        m_vertexes[i].id = i;
        m_vertexes[i].label = std::to_string(i);
    }
}

// функция добавления ребра
void Graph::add_edge(std::size_t from_i, std::size_t to_i) {
    m_vertexes[from_i].neighbours.insert(to_i);
}

std::size_t Graph::size() {
    return m_vertexes.size();
}

void Graph::mark_subgraph(Graph& graph) {
    for (Vertex& sub_vertex : graph.m_vertexes) {
        for (Vertex& vertex : m_vertexes) {
            if (sub_vertex.id == vertex.id) {
                vertex.color = "red";
            }
        }
    }
}

// пользователь вызывает данную функцию для поиска всех клик
std::vector<Graph> Graph::find_all_cliques() {
    // массив для хранения клик
    std::vector<Graph> result;
    // множество вершин текущей клики
    std::set<std::size_t> compsub;
    // множество вершин-кандидатов на содержание в клике
    std::set<std::size_t> candidates;
    // изначально - все вершины графа
    for (std::size_t i = 0; i < m_vertexes.size(); ++i) {
        candidates.insert(i);
    }
    // множество вершин, которые точно не попадут в текущую клику
    std::set<std::size_t> not_candidates;

    // вызываем рекурсивно алгоритм Брона-Кербоша
    extend(candidates, not_candidates, compsub, result);

    // возвращаем массив клик
    return std::move(result);
}

void Graph::save_to_png(std::string file_name) {
    // создаем неориентированный текстовый граф для graphviz
    GraphvizUtility text_graph(false);

    // передаем ему множество вершин
    for (std::size_t i = 0; i < m_vertexes.size(); ++i) {
        text_graph.add_node(m_vertexes[i].id, m_vertexes[i].label,
                            "color=\"" + m_vertexes[i].color + "\", ");
    }

    // и множество ребер
    for (std::size_t i = 0; i < m_vertexes.size(); ++i) {
        for (std::size_t neighbour_i : m_vertexes[i].neighbours) {
            if (neighbour_i > i) {
                text_graph.add_edge(m_vertexes[i].id,
                                    m_vertexes[neighbour_i].id, "");
            }
        }
    }

    // сохраняем текстовый файл для graphviz
    std::string dot_file = "tmp.dot";
    text_graph.save(dot_file);

    // вызываем рисование графа
    std::ofstream out("tmp.bat");
    out << "dot.exe -Tpng -Gsize=8,4! -Gdpi=100 tmp.dot >" << file_name;
    out.close();

    system("tmp.bat");
}

// конструктор подграфа графа other
Graph::Graph(const Graph& other, std::set<std::size_t> vertexes) {
    // меняем размер массива вершин
    m_vertexes.resize(vertexes.size());
    // создаем отображение старых индексов вершин в новые
    std::map<std::size_t, std::size_t> index_map;
    std::size_t current_i = 0;

    // переносим вершины из графа в подграф
    for (std::size_t vertex_i : vertexes) {
        index_map[vertex_i] = current_i;
        m_vertexes[current_i].id = other.m_vertexes[vertex_i].id;
        m_vertexes[current_i].label = other.m_vertexes[vertex_i].label;
        current_i += 1;
    }
    // переносим ребра из графа в подграф
    for (std::size_t from_i = 0; from_i < other.m_vertexes.size(); ++from_i) {
        if (!vertexes.count(from_i)) continue;
        for (std::size_t to_i : other.m_vertexes[from_i].neighbours) {
            if (vertexes.count(to_i)) {
                add_edge(index_map[from_i], index_map[to_i]);
            }
        }
    }
}

// функция, реализующая алгоритм Брона-Кербоша
void Graph::extend(std::set<std::size_t>& candidates,
                   std::set<std::size_t>& not_candidates,
                   std::set<std::size_t>& compsub, std::vector<Graph>& result) {

    // флаг, который указывает, есть ли в множестве вершин not_candidates
    // вершина, соединенная со всем множеством вершин candidates
    bool not_connects_full_candidates = false;
    // узнаем значение флага
    for (std::size_t not_elem_i : not_candidates) {
        std::vector<std::size_t> intersection;
        std::set_intersection(m_vertexes[not_elem_i].neighbours.begin(),
                              m_vertexes[not_elem_i].neighbours.end(),
                              candidates.begin(), candidates.end(),
                              std::back_inserter(intersection));
        if (intersection.size() == candidates.size()) {
            not_connects_full_candidates = true;
        }
    }

    // ПОКА candidates НЕ пусто И not_candidates НЕ содержит вершины,
    // СОЕДИНЕННОЙ СО ВСЕМИ вершинами из candidates
    while (!candidates.empty() && !not_connects_full_candidates) {
        // Выбираем вершину с номером vertex_i из candidates и добавляем её в
        // compsub
        std::size_t vertex_i = *candidates.begin();
        compsub.insert(vertex_i);

        // Формируем new_candidates, добавляя из candidates
        // только вершины, СОЕДИНЕННЫЕ с вершиной с номером vertex_i
        std::set<std::size_t> new_candidates;
        for (std::size_t candidate_i : candidates) {
            if (m_vertexes[candidate_i].neighbours.count(vertex_i) &&
                candidate_i != vertex_i) {
                new_candidates.insert(candidate_i);
            }
        }

        // Формируем new_not_candidates, добавляя из not_candidates
        // только вершины, СОЕДИНЕННЫЕ с вершиной с номером vertex_i
        std::set<std::size_t> new_not_candidates;
        for (std::size_t not_candidate_i : not_candidates) {
            if (m_vertexes[not_candidate_i].neighbours.count(vertex_i) &&
                not_candidate_i != vertex_i) {
                new_not_candidates.insert(not_candidate_i);
            }
        }

        // ЕСЛИ new_candidates и new_not_candidates пусты
        if (new_candidates.empty() && new_not_candidates.empty()) {
            // ТО compsub – клика
            result.push_back(Graph(*this, compsub));
        } else {
            // ИНАЧЕ рекурсивно вызываем extend (new_candidates,
            // new_not_candidates)
            extend(new_candidates, new_not_candidates, compsub, result);
        }

        // Удаляем вершину с номером vertex_i из compsub и candidates, и
        // помещаем в not_candidates
        candidates.erase(vertex_i);
        compsub.erase(vertex_i);
        not_candidates.insert(vertex_i);
    }
}

// оператор для вывода графа в консоль
std::ostream& operator<<(std::ostream& out, Graph& graph) {
    for (Graph::Vertex& vertex : graph.m_vertexes) {
        out << vertex.label << ", ";
    }
    return out;
}
