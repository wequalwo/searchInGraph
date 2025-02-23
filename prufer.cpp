#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <unordered_set>
#include <queue>
#include <functional>

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

// Функция для восстановления дерева из последовательности Прюфера
// Оптимизированная реализация
List<std::pair<int, int>> prufer_unpack(const List<int>& prufer_sequence, int n) {
    List<std::pair<int, int>> edges;

    // Массив для отслеживания количества появлений каждой вершины в последовательности Прюфера
    std::vector<int> degree(n + 1, 1);  // Начальная степень для всех вершин = 1, т.к. все вершины присутствуют
    for (int v : prufer_sequence) {
        degree[v]++;
    }

    // Очередь с приоритетом (min-heap) для хранения минимальных вершин
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
    for (int i = 1; i <= n; ++i) {
        if (degree[i] == 1) {  // Вершина только один раз встречается
            min_heap.push(i);
        }
    }

    // Восстанавливаем дерево
    for (int i = 0; i < n - 2; ++i) {
        // Извлекаем минимальную вершину
        int v = min_heap.top();
        min_heap.pop();

        // Добавляем ребро
        edges.push_back({v, prufer_sequence[i]});

        // Уменьшаем степень вершины, которая была использована
        if (--degree[prufer_sequence[i]] == 1) {
            min_heap.push(prufer_sequence[i]);
        }
    }

    // Добавляем последнее ребро между оставшимися двумя вершинами
    int v1 = min_heap.top();
    min_heap.pop();
    int v2 = min_heap.top();
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

/**
 * Любую пару (a, b) можно преобразовать в индекс по формуле:
 * index = (a - 1) * (2 * n - a) / 2 + (b - a - 1)
 * где (a - 1)*(2n - 1)/2 = (n - 1) + (n - 2) + ...  + (n - a + 1)
 * -- количество пар, у которых первый элемент меньше a
 * (b - a - 1) -- порядковый номер элемента b в строке, начинающейся с a
 * 
 * Обратное преобразование получается путем решения уравнений
 * (a - 1)*(2n - 1)/2  < index
 * b = a + 1 + (index - (a * (a + 1) / 2))
 */
// Функция преобразования индекса в пару (a, b)
std::pair<int, int> pair_from_index(int index, int n) {
    // Решаем квадратное уравнение a^2 - a - 2 * index = 0
    int a = (2*n + 1 - std::sqrt((2*n + 1)*(2*n + 1) - 4*(2*index + 2*n)))/2;
    // Вычисляем оставшийся индекс
    int index_in_row = index - (a - 1)*(2*n - 1)/2;
    
    int b = a + 1 + index_in_row;
    
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


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        return 1;
    }

    int n = std::atoi(argv[1]);  // Получаем значение N из аргументов командной строки

    List<int> prufer_sequence = prufer_gen(n);  // Генерация последовательности Пруфера
    std::cout << "Generated Prufer sequence with n = " << n << std::endl;
    if (n < 20)
    {
        for (int num : prufer_sequence) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
 
    // Восстановление дерева из последовательности Пруфера
    List<std::pair<int, int>> edges = prufer_unpack(prufer_sequence, n);
    std::cout << "Tree has generaded" << std::endl;
    write_dot_file(edges, "tree.dot");

    // Генерация новых пар (добавление новых рёбер)
    List<std::pair<int, int>> all_pairs = edges;  // Все существующие пары
    List<std::pair<int, int>> new_pairs = generate_new_pairs(n, edges);
    all_pairs.insert(all_pairs.end(), new_pairs.begin(), new_pairs.end());
    std::cout << "Graph has generaded" << std::endl;
    write_dot_file(all_pairs, "graph.dot");
    return 0;
}
