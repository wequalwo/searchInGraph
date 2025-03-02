#pragma once
#include "common/common.h"
#include "common/service.h"
#include "graph/node.h"
#include "graph/edge.h"
#include <algorithm>

/**
 * Любую пару (a, b) можно преобразовать в индекс по формуле:
 * index = (a - 1) * (2 * n - a) / 2 + (b - a - 1)
 * где (a - 1)*(2n - 1)/2 = (n - 1) + (n - 2) + ...  + (n - a + 1)
 * -- количество пар, у которых первый элемент меньше a
 * (b - a - 1) -- порядковый номер элемента b в строке, начинающейся с a
 *
 * Обратное преобразование получается путем решения уравнений
 * (a - 1)*(2n - 1)/2  < index
 * b = a + 1 + (index - (a - 1)*(2n - 1)/2)
 */
// Функция преобразования индекса в пару (a, b)

/**
 * Версия без использования множества доступных индексов
 * Работает значительно быстрее, чем обощенная версия
 * @param n
 * @param existing_pairs
 * Версия без использования индексов
 * 2.1
 **/
std::vector<std::pair<SizeType, SizeType>> generate_new_pairs(
    int n, const std::vector<std::pair<SizeType, SizeType>> &existing_pairs, double density)
{
    using Clock = std::chrono::steady_clock;

    std::cerr << "starting edges generating\n";
    Clock::time_point begin = Clock::now();

    SizeType T = n * (n - 1) / 2; // Общее количество возможных пар

    // Используем unordered_set для быстрого поиска
    std::unordered_set<SizeType> existing_set;
    for (const auto &pair : existing_pairs)
    {
        existing_set.insert((pair.first - 1) * (2 * n - pair.first) / 2 + (pair.second - pair.first - 1));
    }

    // Определяем l — сколько новых пар нужно добавить
    std::random_device rd;
    std::mt19937 gen(rd());

    if (density > 1 || density < 0)
        throw std::invalid_argument("Некорректная плотность");

    int l = int(double(T) * density);
    if (l == 0)
    {
        std::uniform_int_distribution<int> dist(0, T - existing_pairs.size());
        l = dist(gen);
    }

    //std::cout << "Number of new pairs to generate: l = " << l << std::endl;
    //std::cout << "With density = " << double(l + n - 1) / double(T) << std::endl;
    if (l == 0)
        return existing_pairs;

    // Генерация списка доступных индексов (не входящих в existing_pairs)
    std::vector<SizeType> available_indices;
    available_indices.reserve(T - existing_pairs.size());

    for (SizeType i = 0; i < T; i++)
    {
        if (existing_set.find(i) == existing_set.end())
        {
            available_indices.push_back(i);
        }
    }
    // std::cout << "l to generate l = " << l << std::endl;
    // Выбор l случайных индексов с помощью std::sample
    std::vector<SizeType> new_pairs;
    new_pairs.reserve(l);
    std::sample(available_indices.begin(), available_indices.end(), std::back_inserter(new_pairs), l, gen);

    // Формируем итоговый список пар
    std::vector<std::pair<SizeType, SizeType>> graph_pairs;
    graph_pairs.reserve(existing_pairs.size() + l);

    for (SizeType idx : new_pairs)
    {
        graph_pairs.push_back(pair_from_index(idx, n));
    }
    graph_pairs.insert(graph_pairs.end(), existing_pairs.begin(), existing_pairs.end());

    Clock::time_point end = Clock::now();
    std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mcs] = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';

    return graph_pairs;
}

// Модификация без отображения ребер в числа. Работа выполняется сразу над парами чисел.
// Также исключены некоторые промежуточные копирования.
// Предполагается, что небольшие изменения в используемых типах позволят ускорить работу без увеличения расхода памяти.
void generate_new_pairs_unpacked(int n, List<EdgeType>& existing_pairs, double density)
{
    using Clock = std::chrono::steady_clock;

    std::cerr << "starting edges generating\n";
    Clock::time_point begin = Clock::now();

    SizeType T = n * (n - 1) / 2; // Общее количество возможных пар

    // Используем Set для быстрого поиска
    Set<EdgeType> existing_set(existing_pairs.begin(), existing_pairs.end());

    // Определяем l — сколько новых пар нужно добавить
    std::random_device rd;
    std::mt19937 gen(rd());

    if (density > 1 || density < 0)
        throw std::invalid_argument("Некорректная плотность");

    int l = int(double(T) * density);
    if (l == 0)
    {
        std::uniform_int_distribution<int> dist(0, T - existing_pairs.size());
        l = dist(gen);
    }

    // Генерация списка доступных индексов (не входящих в existing_pairs)
    List<EdgeType> available_indices;
    available_indices.reserve(T - existing_pairs.size());

    for (SizeType i = 0; i < T - 1; i++)
        for (SizeType j = i + 1; j < T; ++j) // <- генерируем упорядоченные пары с пропуском петель
        {
            auto pair = std::make_pair(i, j);
            if (existing_set.count(pair) == 0)
                available_indices.push_back(std::move(pair));
        }
    // std::cout << "l to generate l = " << l << std::endl;
    // Выбор l случайных индексов с помощью std::sample
    // вставляем сразу в existing_pairs
    std::sample(available_indices.begin(), available_indices.end(), std::back_inserter(existing_pairs), l, gen);

    Clock::time_point end = Clock::now();
    std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mcs] = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';
}


/*
List<Node> setGraphDensity2(
    int n, const std::vector<std::pair<SizeType, SizeType>> &existing_pairs, double density)
{
    using Clock = std::chrono::steady_clock;
    List<Node> nodes;
    nodes.reserve(n); // резервируем память под узлы

    std::cerr << "starting edges generating\n";
    Clock::time_point begin = Clock::now();

    SizeType T = n * (n - 1) / 2; // Общее количество возможных пар

    // Используем unordered_map для хранения инцидентных узлов
    List<Set<SizeType>> adjacency(n);

    // Используем unordered_set для быстрого поиска
    std::unordered_set<SizeType> existing_set;
    for (const auto &pair : existing_pairs)
    {
        existing_set.insert((pair.first - 1) * (2 * n - pair.first) / 2 + (pair.second - pair.first - 1));
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    if (density > 1 || density < 0)
        throw std::invalid_argument("Некорректная плотность");

    int l = int(double(T) * density);

    if (l == 0)
    {
        std::uniform_int_distribution<int> dist(0, T - existing_pairs.size());
        l = dist(gen);
    }
    std::cout << l << std::endl;
    if (l == 0)
        return nodes;

    std::vector<SizeType> available_indices;
    available_indices.reserve(T - existing_pairs.size());

    for (SizeType i = 0; i < T; i++)
    {
        if (existing_set.find(i) == existing_set.end())
        {
            available_indices.push_back(i);
        }
    }

    std::vector<SizeType> new_pairs;
    new_pairs.reserve(l);
    std::sample(available_indices.begin(), available_indices.end(), std::back_inserter(new_pairs), l, gen);

    for (SizeType idx : new_pairs)
    {
        auto [u, v] = pair_from_index(idx, n);
        adjacency.at(u - 1).insert(v);
        adjacency.at(v - 1).insert(u);
    }

    for (auto edge : existing_pairs)
    {
        adjacency.at(edge.first - 1).insert(edge.second);
        adjacency.at(edge.second - 1).insert(edge.first);
    }

    for (SizeType i = 0; i < n; i++)
    {
        nodes.emplace_back(i, adjacency.at(i));
    }

    Clock::time_point end = Clock::now();
    std::cerr << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mcs] = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';

    return nodes;
}
*/



/**
 * Функция для преобразования списка ребер в список узлов
 * @param edges список ребер
 * @param n количество вершин
 * @return список узлов
 * @complexity O(n + m)
 * @time O(n + m)
 * @space O(n + m)
 */
List<Node> transform(std::vector<std::pair<SizeType, SizeType>> edges, int n)
{
    using Clock = std::chrono::steady_clock;
    Clock::time_point begin = Clock::now();

    std::cout << "transformation init \n";
    List<Node> nodes;
    nodes.reserve(n);

    // создаем вершины
    for (int i = 0; i < n; ++i)
    {
        nodes.push_back(Node(i, Set<SizeType>{}));
    }

    // Вставляем ребра в граф
    for (auto edge : edges)
    {
        // Регулируем показатель так, чтобы он начинался с нуля
        insertEdge(nodes, edge.first - 1, edge.second - 1);
    }

    Clock::time_point end = Clock::now();
    std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mcs] = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';
    return nodes;
}
