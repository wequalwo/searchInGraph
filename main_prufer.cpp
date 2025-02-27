/**
 * Ускоренная версия файла prufer.cpp (за счет отсутсвия записи в файл)
 * Метрики:
 * 1k вершин: 0.02
 * 10k вершин: 0.7
 * 100k вершин: 25
 */

#include <iostream>
#include "common/common.h"
#include "common/service.h"

#include "prufer_graph/prufer.h"
#include "prufer_graph/random_graph.h"
#include "prufer_graph/hist.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        return 1;
    }
    int n = std::atoi(argv[1]);          // Получаем значение N из аргументов командной строки
    double density = std::atoi(argv[2]); // Получаем значение плотности из аргументов командной строки

    // основной алгоритм построения дерева
    List<int> prufer_sequence = prufer_gen(n);
    List<std::pair<SizeType, SizeType>> edges = prufer_unpack(prufer_sequence, n);
    List<std::pair<SizeType, SizeType>> new_pairs = generate_new_pairs(n, edges, density);
    write_dot_file(new_pairs, "graph.dot");

    // построение гистограммы
    /*std::vector<int> hist(n);

    for (int i = 0; i < 10000; i++)
    {
        List<int> prufer_sequence = prufer_gen(n);
        List<std::pair<SizeType, SizeType>> edges = prufer_unpack(prufer_sequence, n);
        get_hist(n, generate_degree_sample(n, edges, density), hist); // получаем гистограмму степеней
    }
    write_vector_to_file(hist, "histogram.bin");
    */
    return 0;
}