#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <unordered_set>

template <class T>
using List = std::vector<T>;

template <class T>
using Set = std::unordered_set<T>;

// Функция для генерации последовательности Пруфера
List<int> prufer_gen(int n) {
    List<int> prufer_sequence(n - 2);

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
    List<std::pair<int, int>> edges;

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
            // TODO: здесь можно оптимизировать поиск минимума

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


// Функция преобразования индекса в пару (a, b)
std::pair<int, int> pair_from_index(int index, int n) {
    int a = 1;
    while (index >= (n - a)) {
        index -= (n - a);
        a++;
    }
    int b = a + 1 + index;
    return {a, b};
}

// Функция генерации новых пар
List<std::pair<int, int>> generate_new_pairs(int n, const List<std::pair<int, int>>& existing_pairs) {
    int T = n * (n - 1) / 2;  // Всего возможных пар
    Set<int> used_indices;  // Храним занятые индексы
    
    // Преобразуем существующие пары в индексы
    for (const auto& p : existing_pairs) {
        int a = p.first, b = p.second;
        int index = (a - 1) * (2 * n - a) / 2 + (b - a - 1);
        used_indices.insert(index);
    }

    // Создаём список доступных индексов
    std::vector<int> available_indices;
    for (int i = 0; i < T; ++i) {
        if (used_indices.find(i) == used_indices.end()) {
            available_indices.push_back(i);
        }
    }

    // Определяем l — количество новых пар
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, available_indices.size());
    int l = dist(gen);

    // Выбираем l случайных индексов
    std::shuffle(available_indices.begin(), available_indices.end(), gen);
    std::vector<int> chosen_indices(available_indices.begin(), available_indices.begin() + l);

    // Преобразуем индексы обратно в пары
    List<std::pair<int, int>> new_pairs;
    for (int idx : chosen_indices) {
        new_pairs.push_back(pair_from_index(idx, n));
    }

    return new_pairs;
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


    std::cout << "Tree edges: " << std::endl;
    for (const auto& edge : edges) {
        std::cout << edge.first << " - " << edge.second << std::endl;
    }

    // Генерация новых пар (добавление новых рёбер)
    List<std::pair<int, int>> all_pairs = edges;  // Все существующие пары
    List<std::pair<int, int>> new_pairs = generate_new_pairs(n, edges);
    all_pairs.insert(all_pairs.end(), new_pairs.begin(), new_pairs.end());

    std::cout << "All pairs (existing + new): " << std::endl;
    for (const auto& pair : all_pairs) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }

    // Запись графа в файл
    write_dot_file(all_pairs, "tree_with_new_edges.dot");
    return 0;
}
