#pragma once
#ifndef PRUFER_H
#define PRUFER_H

#include "common/common.h"

/**
 * Функция для генерации последовательности Прюфера
 * @param n Количество вершин в дереве
 * @return Вектор, содержащий последовательность Прюфера
 * @complexity O(n)
 * @time O(n)
 * @space O(n)
 */
List<int> prufer_gen(int n);

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
std::vector<SizeType> seq_gen(int n, SizeType diap);

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
List<EdgeType> prufer_unpack(const std::vector<int> &prufer_sequence, int n);

#endif // PRUFER_H