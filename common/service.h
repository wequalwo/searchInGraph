#pragma once
#include "common.h"


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


void write_vector_to_file(const std::vector<int>& vec, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Ошибка при открытии файла для записи." << std::endl;
        return;
    }

    size_t size = vec.size();
    outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
    outFile.write(reinterpret_cast<const char*>(vec.data()), size * sizeof(int));

    outFile.close();
}

// Вычисление квадратного корня с точностью до одного одного знака после запятой
double sq(SizeType N)
{
    SizeType a = static_cast<SizeType>(std::sqrt(N)); // Целая часть корня
    double remainder = N - a * a;                     // Остаток при возведении в квадрат
    return a + (remainder) / (2 * a);                 // Формула для первой десятичной цифры
}

// получение пары по индексу
std::pair<SizeType, SizeType> pair_from_index(SizeType index, SizeType n)
{
    // Решаем квадратное уравнение a^2 - a - 2 * index = 0
    // Берем ближайшее целое слева
    SizeType a = (2 * n + 1 - sq((2 * n + 1) * (2 * n + 1) - 4 * (2 * index + 2 * n))) / 2;

    // Вычисляем оставшийся индекс
    SizeType b = index + a + 1 - (a - 1) * (2 * n - a) / 2;
    if (a >= b || a < 0 || b < 0)
    {
        // std::cout << a << " " << b << std::endl;
        throw std::runtime_error("validation error");
    }
    return {a, b};
}