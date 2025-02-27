#pragma once
#include "common/common.h"
#include "common/service.h"

///////////////////////////////////////////////
//построение гистограммы
// Функция для анализа распределения степеней вершин
std::vector<SizeType> calculate_deg(int n, const std::vector<std::pair<SizeType, SizeType>> &pairs)
{
    std::vector<SizeType> deg(n, 0);
    for (int i = 0; i < pairs.size(); i++)
    {
        deg.at(pairs.at(i).first - 1) += 1; // индексация вершин с 1, а не с нуля
        deg.at(pairs.at(i).second - 1) += 1;
    }
    return deg;
}

std::vector<SizeType> generate_degree_sample(
    int n, const std::vector<std::pair<SizeType, SizeType>> &existing_pairs, double density)
{
    SizeType T = n * (n - 1) / 2; // Общее количество возможных пар
    std::vector<SizeType> deg(n, 0);

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

    // std::cout << "Number of new pairs to generate: l = " << l << std::endl;
    // std::cout << "With density = " << double(l + n - 1)/double(T) << std::endl;
    if (l == 0)
    {
        return calculate_deg(n, existing_pairs);
    }
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
    //  Выбор l случайных индексов с помощью std::sample
    std::vector<SizeType> new_pairs;
    new_pairs.reserve(l);
    std::sample(available_indices.begin(), available_indices.end(), std::back_inserter(new_pairs), l, gen);

    // Формируем итоговый список пар
    std::vector<std::pair<SizeType, SizeType>> graph_pairs;
    graph_pairs.reserve(existing_pairs.size() + l);

    for (SizeType idx : new_pairs)
    {
        auto pair = pair_from_index(idx, n);
        deg.at(pair.first - 1) += 1; // индексация вершин с 1, а не с 0
        deg.at(pair.second - 1) += 1;
    }
    for (auto idx : existing_pairs)
    {
        deg.at(idx.first - 1) += 1;
        deg.at(idx.second - 1) += 1;
    }
    return deg;
}

std::vector<int> get_hist(int n, const std::vector<SizeType> &deg, std::vector<int> &hist)
{

    for (auto i : deg)
    {
        hist.at(i)++;
    }
    return hist;
}
///////////////////////////////////////////////////////////////////////////