#pragma once
#include "randomizer/rand.h"


/**
 * Функция для генерации последовательности Прюфера
 * @param n Количество вершин в дереве
 * @return Вектор, содержащий последовательность Прюфера
 * @complexity O(n)
 * @time O(n)
 * @space O(n)
 */
List<int> prufer_gen(int n)
{
    List<int> prufer_sequence(n - 2);

    // Используем текущее время как начальное значение для генератора
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(1, n);

    for (int i = 0; i < n - 2; ++i)
    {
        prufer_sequence.at(i) = dist(gen);
    }

    return prufer_sequence;
}


/**
 * @brief Генерирует последовательность из n уникальных чисел от 1 до diap
 * 
 * @details
 *      Генерирует последовательность из n уникальных чисел от 1 до diap
 *      по следующему алгоритму:
 *      1. Генерируется упорядоченный список
 *      2. Генератором случайных чисел список перемешивается
 *      3. Берутся первые n элементов
 * 
 * @param[in] n Количество запрошенных уникальных чисел
 * @param[in] diap Диапазон, из которого берутся числа
 * @return std::vector<SizeType> - последовательность из n уникальных чисел
 * @complexity O(n)
 * @time O(n)
 * @space O(n)
 * @throw std::invalid_argument - если n > diap
 */
std::vector<SizeType> seq_gen(int n, SizeType diap)
{
    if (n > diap)
    {
        throw std::invalid_argument("Запрошено больше уникальных чисел, чем есть в диапазоне");
    }

    std::vector<SizeType> seq(diap);
    for (SizeType i = 0; i < diap - 1; ++i)
    {
        seq.at(i) = i + 1; // Заполняем диапазон значениями от 1 до diap
    }

    // Инициализируем генератор случайных чисел
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(seq.begin(), seq.end(), std::mt19937(seed));

    // Берём первые n уникальных элементов
    return std::vector<SizeType>(seq.begin(), seq.begin() + n);
}

/**
 * Функция для восстановления дерева из последовательности Прюфера
 * Оптимизированная реализация
 * @param prufer_sequence Последовательность Прюфера
 * @param n Количество вершин в дереве
 * @return Вектор пар, представляющий дерево
 * @complexity O(n)
 * @time O(n)
 * @space O(n)
 * Версия с сортировкой вершин
 */
List<EdgeType> prufer_unpack(const std::vector<int> &prufer_sequence, int n)
{
    List<EdgeType> edges;

    // Массив для отслеживания количества появлений каждой вершины в последовательности Прюфера
    std::vector<SizeType> degree(n + 1, 1); // Все вершины изначально имеют степень 1
    for (SizeType v : prufer_sequence)
    {
        degree.at(v)++;
    }

    // Мин-куча для хранения минимальных вершин
    std::priority_queue<SizeType, std::vector<SizeType>, std::greater<SizeType>> min_heap;
    for (int i = 1; i <= n; ++i)
    {
        if (degree.at(i) == 1)
        { // Если вершина встречается только один раз
            min_heap.push(i);
        }
    }

    // Восстанавливаем дерево
    for (int i = 0; i < n - 2; ++i)
    {
        SizeType v = min_heap.top();
        min_heap.pop();

        SizeType u = prufer_sequence.at(i);

        // Добавляем ребро, гарантируя порядок (меньшее число первым)
        edges.emplace_back(EdgeType{std::min(u, v), std::max(u, v)});

        // Уменьшаем степень вершины
        if (--degree.at(u) == 1)
        {
            min_heap.push(u);
        }
    }

    // Добавляем последнее ребро
    SizeType v1 = min_heap.top();
    min_heap.pop();
    SizeType v2 = min_heap.top();

    edges.emplace_back(EdgeType{std::min(v1, v2), std::max(v1, v2)});
    for (auto& edge : edges)
    {
        --edge.first;
        --edge.second;
    }
    return edges;
}