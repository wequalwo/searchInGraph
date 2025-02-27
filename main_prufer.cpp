/**
 * Ускоренная версия файла prufer.cpp (за счет отсутсвия записи в файл)
 * Метрики:
 * 1k вершин: 0.037
 * 10k вершин: 2.16
 * 100k вершин: 24.31
 */

#include <iostream>
#include "common/common.h"
#include "common/service.h"

#include "prufer_graph/prufer.h"
#include "prufer_graph/random_graph.h"
#include "prufer_graph/hist.h"

/**
 * @brief Главная функция, запускающая алгоритм
 * 
 * @param argc Количество аргументов командной строки
 * @param argv Аргументы командной строки:
 *             1. n - количество вершин в графе
 *             2. density - плотность графа (от 0 до 1)
 * 
 * @return 0 - успех, 1 - ошибка
 * 
 * @details
 * 1. Получает из аргументов командной строки количество вершин n и плотность density
 * 2. Генерирует последовательность Прюфера длины n - 2 (дерево на n вершинах)
 * 3. Генерирует случайный граф вершинами и плотностью density на основе остовного дерева
 * 4. Записывает граф в файл graph.dot
 * 5. (опционально) Строит гистограмму степеней вершин
 * 6. (опционально) Записывает гистограмму в файл histogram.bin
 */
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