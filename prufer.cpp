#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>  // Для записи в файл

template <class T>
using List = std::vector<T>;

// Функция для генерации последовательности Пруфера
List<int> prufer_gen(int n) {
    List<int> prufer_sequence(n - 2);  // Последовательность Пруфера будет иметь длину n-2

    // Используем текущее время как начальное значение для генератора
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(1, n);

    for (int i = 0; i < n - 2; ++i) {
        prufer_sequence[i] = dist(gen);
    }

    return prufer_sequence;
}

// Функция для восстановления дерева из последовательности Пруфера
List<std::pair<int, int>> prufer_unpack(const List<int>& prufer_sequence, int n) {
    List<std::pair<int, int>> edges;  // Множество рёбер дерева

    // Множество неиспользуемых номеров вершин
    std::set<int> unused_vertices;
    for (int i = 1; i <= n; ++i) {
        unused_vertices.insert(i);
    }

    // Восстанавливаем дерево
    for (int i = 0; i < n - 2; ++i) {
        // Ищем минимальную неиспользуемую вершину, которая не встречается в остатке последовательности Прюфера
        int v = -1;
        for (auto it = unused_vertices.begin(); it != unused_vertices.end(); ++it) {
            bool is_unused = true;
            // Проверяем, встречается ли вершина в оставшихся элементах последовательности Пруфера
            for (int j = i; j < prufer_sequence.size(); ++j) {
                if (prufer_sequence[j] == *it) {
                    is_unused = false;
                    break;
                }
            }
            // Если не встречается, выбираем её как минимальную
            if (is_unused) {
                v = *it;
                break;
            }
        }

        // Добавляем ребро (v, A[i])
        edges.push_back({v, prufer_sequence[i]});

        // Удаляем вершину v из множества неиспользуемых вершин
        unused_vertices.erase(v);
    }

    // Добавляем последнее ребро между оставшимися двумя вершинами
    auto it = unused_vertices.begin();
    int v1 = *it;
    ++it;
    int v2 = *it;
    edges.push_back({v1, v2});

    return edges;
}

void write_dot_file(const List<std::pair<int, int>>& edges, const std::string& filename) {
    std::ofstream file(filename);
    
    if (file.is_open()) {
        file << "graph G {\n";
        for (const auto& edge : edges) {
            file << "    " << edge.first << " -- " << edge.second << ";\n";
        }
        file << "}\n";
        file.close();
        std::cout << "Graph written to " << filename << std::endl;
    } else {
        std::cerr << "Error opening file " << filename << std::endl;
    }
}

int main() {
    int n = 10;  // Количество вершин
    List<int> prufer_sequence = prufer_gen(n);  // Генерация последовательности Пруфера

    std::cout << "Generated Prufer sequence with n = " << n << ": ";
    for (int num : prufer_sequence) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Восстановление дерева из последовательности Пруфера
    List<std::pair<int, int>> edges = prufer_unpack(prufer_sequence, n);

    std::cout << "Tree edges: " << std::endl;
    for (const auto& edge : edges) {
        std::cout << edge.first << " - " << edge.second << std::endl;
    }
    write_dot_file(edges, "tree.dot");
    return 0;
}
