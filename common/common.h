#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <unordered_set>
#include <unordered_map>

#define MIN_INVERSE_DENSITY 0.5
// Определяем удобные псевдонимы с помощью using
template <class T>
using Set = std::unordered_set<T>;

// Хеш-таблица (ассоциативный массив)
template <class T, class V>
using Map = std::unordered_map<T, V>;

template <class T>
using List = std::vector<T>;

using SizeType = uint16_t;

// ребро кодируем как пару индексов
struct EdgeType
{
    SizeType first;
    SizeType second;
};

namespace std {
    template<>
    struct hash<EdgeType> {
        size_t operator()(const EdgeType &edge) const {
            return std::hash<SizeType>()(edge.first) ^ (std::hash<SizeType>()(edge.second) << 1);
        }
    };

    template<>
    struct equal_to<EdgeType> {
        bool operator()(const EdgeType &first, const EdgeType &second) const {
            return first.first == second.first && first.second == second.second;
        }
    };
}

#endif //COMMON_H