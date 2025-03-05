#pragma once
#ifndef SERVICE_H
#define SERVICE_H

#include <fstream>

#include "common.h"

/**
 * Служебные функции, необходимые для работы алгоритмов
 */

/**
 *
 * Записывает данный список рёбер в файл в формате dot.
 * Файл является бинарным и будет перезаписан, если он уже существует.
 *
 * @param edges Список рёбер, которые нужно записать в файл. Каждое ребро — это пара типа SizeType, 
 * где первый элемент — исходная вершина, а второй элемент — целевая вершина.
 * @param filename Имя файла для записи.
 *
 * @throws std::runtime_error Если файл не удаётся открыть.
 */
void write_dot_file(const List<std::pair<SizeType, SizeType>> &edges, const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Error opening file " + filename);
    }
    if (file.is_open())
    {
        file << "graph G {\n";
        for (const auto &edge : edges)
        {
            file << "    " << edge.first << " -- " << edge.second << ";\n";
            // std::cout << "    " << edge.first << " -- " << edge.second << ";\n";
        }
        file << "}\n";
        file.close();
        file.flush();
    }
    // std::cout << "Graph written to " << filename << std::endl;
    file.flush();
    return;
};


/**
 * Записывает вектор целых чисел в бинарный файл. Файл будет содержать размер вектора,
 * за которым следуют элементы вектора.
 *
 * @param vec Вектор целых чисел, который нужно записать в файл.
 * @param filename Имя бинарного файла для записи данных вектора.
 * @throw std::runtime_error ошибка потока записи в файл
 */
void write_vector_to_file(const std::vector<int> &vec, const std::string &filename)
{
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile)
    {
        throw std::runtime_error("Ошибка при открытии файла для записи.");
    }

    size_t size = vec.size();
    outFile.write(reinterpret_cast<const char *>(&size), sizeof(size));
    outFile.write(reinterpret_cast<const char *>(vec.data()), size * sizeof(int));

    outFile.close();
}

// Вычисление квадратного корня с точностью до одного одного знака после запятой
double sq(SizeType N)
{
    SizeType a = static_cast<SizeType>(std::sqrt(N)); // Целая часть корня
    double remainder = N - a * a;                     // Остаток при возведении в квадрат
    return a + (remainder) / (2 * a);                 // Формула для первой десятичной цифры
}


/**
 * @brief Вычисляет индекс пары (a, b) в прямом порядке
 * 
 * @details
 *      (a, b) = (a, a + 1 + index - (a-1)*(2n-a)/2)
 * 
 *      where 0 <= index < (n*(n-1))/2
 *      and a is the largest integer such that a*(2n-a)/2 <= index
 * 
 * @param[in] index индекс пары в прямом порядке
 * @param[in] n количество вершин
 * @return пара (a, b)
 * @throw std::runtime_error при ошибке out of range
 */
EdgeType pair_from_index(SizeType index, SizeType n)
{
    // Решаем квадратное уравнение a^2 - a - 2 * index = 0
    // Берем ближайшее целое слева
    SizeType a = (2 * n + 1 - sq((2 * n + 1) * (2 * n + 1) - 4 * (2 * index + 2 * n))) / 2;

    // Вычисляем оставшийся индекс
    SizeType b = index + a + 1 - (a - 1) * (2 * n - a) / 2;
    if (a >= b || a < 0 || b < 0)
    {
        //std::cout << a << " " << b << std::endl;
        throw std::runtime_error("validation error");
    }
    return {a, b};
}


#endif //SERVICE_H