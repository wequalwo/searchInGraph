#pragma once
#include "common/common.h"
#include "common/service.h"


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

    return graph_pairs;
}


