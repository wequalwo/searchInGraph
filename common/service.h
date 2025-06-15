#pragma once
#ifndef SERVICE_H
#define SERVICE_H

#include "common/common.h"

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
void write_dot_file(const List<EdgeType> &edges, const std::string &filename);

/**
 * Записывает вектор целых чисел в бинарный файл. Файл будет содержать размер вектора,
 * за которым следуют элементы вектора.
 *
 * @param vec Вектор целых чисел, который нужно записать в файл.
 * @param filename Имя бинарного файла для записи данных вектора.
 * @throw std::runtime_error ошибка потока записи в файл
 */
void write_vector_to_file(const std::vector<int> &vec, const std::string &filename);

// Вычисление квадратного корня с точностью до одного одного знака после запятой
double sq(SizeType N);

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
EdgeType pair_from_index(SizeType index, SizeType n);

SizeType index_from_pair(SizeType graphSize, SizeType first, SizeType second);

#endif //SERVICE_H