/**
 * Ускоренная версия файла prufer.cpp (за счет отсутсвия записи в файл)
 * Метрики:
 * 1k вершин: 0.037
 * 10k вершин: 2.16
 * 100k вершин: 25
 */
//#pragma onces
#include <iostream>
#include <string>   // Для std::stod
//#include "monte_carlo/monte_carlo.h"
#include "prufer_graph/prufer.h"
#include "prufer_graph/random_graph.h"
#include "common/service.h"

#include "graph/tree.h"
#include "graph/edge.h"
#include "graph/traversal.h"
#include "graph/node.h"

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
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        return 1;
    }
    int n = std::atoi(argv[1]);          // Получаем значение N из аргументов командной строки
    double density = std::atof(argv[2]); // Получаем значение плотности из аргументов командной строки
    int trials = std::atoi(argv[3]);     // Количество запусков
    // основной алгоритм построения дерева
    //List<int> prufer_sequence = prufer_gen(n);
    List<EdgeType> edges = prufer_unpack(prufer_gen(n), n);
       
    //int trials = 1000;
    
    std::vector<int> hist(n, 0); 
    List<Node> graph;

    for (int i = 0; i < trials; i++)
    {
        edges = prufer_unpack(prufer_gen(n), n);
        //generate_new_pairs_unpacked(n, edges, density);
        graph = transform(edges, n);
        //graph = get_tree(n);
        for (auto &edges : graph)
        {
            ++hist.at(edges.incident.size());
        }
    }
    //std::string filename = "graph.dot";
    //write_dot_file(edges, filename);

    write_vector_to_file(hist, "histogram2.bin");

    //generate_new_pairs_unpacked(n, edges, density);


    //List<Node> graph = transform(edges, n); // конвертация графа в матрицу инцедентности (можно считать, что бесплатно, по сравнению с самой генерацией)
    /*
    std::cout << "Try path find\n";

    Traverser traverser(&graph);
	traverser.traverseRand<std::queue<SizeType>>(density);
	traverser.getPath();
    SizeType first = traverser.getFirst();
    SizeType last = traverser.getLast();
    std::cout << "\nfirst = " << first << ", last = " << last << "\n";
    List<SizeType> path = traverser.getPath();
    for (auto id : path)
    {
        std::cout << id << " ";
    }
    std::cout << "\n";

	traverser.clear();
	traverser.traverseRand<std::stack<SizeType>>(density);
	traverser.getPath();
    first = traverser.getFirst();
    last = traverser.getLast();
    std::cout << "first = " << first << ", last = " << last << "\n";
    path = traverser.getPath();
    for (auto id : path)
    {
        std::cout << id << " ";
    }
    std::cout << "\n"; 
    

 

    /*
    if (n <= 100)
    {
        for (const auto &node : graph) {
            std::cout << "Node " << node.data << ": ";
            for (SizeType neighbor : node.incident) {
                std::cout << neighbor << " ";
            }
            std::cout << "\n";
        }
    }
    */
    return 0;
}