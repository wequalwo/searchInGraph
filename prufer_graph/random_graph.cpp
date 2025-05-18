
#include <random>
#include <algorithm>

#include "prufer_graph/random_graph.h"
#include "common/service.h"
#include "graph/edge.h"

// Модификация без отображения ребер в числа. Работа выполняется сразу над парами чисел.
// Также исключены некоторые промежуточные копирования.
// Предполагается, что небольшие изменения в используемых типах позволят ускорить работу без увеличения расхода памяти.
void generate_new_pairs_unpacked(int n, List<EdgeType>& existing_pairs, double density)
{
    // using Clock = std::chrono::steady_clock;

    // std::cerr << "starting edges generating\n";
    // Clock::time_point begin = Clock::now();

    int T = n * (n - 1) / 2; // Общее количество возможных пар

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

    if (l - n <= 0)
        return;

    //l = l - n;

    // Генерация списка доступных индексов (не входящих в existing_pairs)
    List<EdgeType> available_indices;
    available_indices.reserve(T - existing_pairs.size());

    for (SizeType i = 0; i < n - 1; i++)
        for (SizeType j = i + 1; j < n; ++j) // <- генерируем упорядоченные пары с пропуском петель
        {
            EdgeType pair{i, j};
            if (existing_set.count(pair) == 0)
                available_indices.push_back(std::move(pair));
        }
    // std::cout << "l to generate l = " << l << std::endl;
    // Выбор l случайных индексов с помощью std::sample
    // вставляем сразу в existing_pairs
    std::sample(available_indices.begin(), available_indices.end(), std::back_inserter(existing_pairs), l, gen);

    // Clock::time_point end = Clock::now();
    // std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mcs] = "
    //           << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';
}

/**
 * Функция для преобразования списка ребер в список узлов
 * @param edges список ребер
 * @param n количество вершин
 * @return список узлов
 * @complexity O(n + m)
 * @time O(n + m)
 * @space O(n + m)
 */
List<Node> transform(const List<EdgeType>& edges, int n)
{
    // using Clock = std::chrono::steady_clock;
    // Clock::time_point begin = Clock::now();

    // std::cout << "transformation init \n";

    // Создаём список узлов с предвыделенной памятью
    List<Node> nodes;
    nodes.reserve(n);

    // Заполняем его нодами
    for (int i = 0; i < n; ++i)
        nodes.emplace_back(i, Set<SizeType>{});

    // Добавляем рёбра напрямую в множества инцидентных вершин
    for (const auto& edge : edges)
    {
        nodes[edge.first].incident.insert(edge.second);
        nodes[edge.second].incident.insert(edge.first);
    }

    // Clock::time_point end = Clock::now();
    // std::cerr << "Time difference = "
    //          << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
    //          << "[mcs] = "
    //          << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0
    //          << " sec" << '\n';

    return nodes;
}
