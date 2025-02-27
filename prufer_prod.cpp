/**
 * Ускоренная версия файла prufer.cpp (за счет отсутсвия записи в файл)
 * Метрики:
 * 1k вершин: 0.02
 * 10k вершин: 0.7
 * 100k вершин: 25
 */

 #include <iostream>
 #include <vector>
 #include <set>
 #include <algorithm>
 #include <random>
 #include <chrono>
 #include <fstream>
 #include <unordered_set>
 #include <queue>
 #include <functional>
 
 template <class T>
 using List = std::vector<T>;
 using SizeType = uint32_t;
 
 template <class T>
 using Set = std::unordered_set<T>;
 
 // Функция для генерации последовательности Прюфера
 List<int> prufer_gen(int n)
 {
     List<int> prufer_sequence(n - 2);
 
     // Используем текущее время как начальное значение для генератора
     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
     std::mt19937 gen(seed);
     std::uniform_int_distribution<> dist(1, n);
 
     for (int i = 0; i < n - 2; ++i)
     {
         prufer_sequence.at(i) = dist(gen);
     }
 
     return prufer_sequence;
 }
 
 std::vector<SizeType> seq_gen(int n, SizeType diap)
 {
     if (n > diap)
     {
         throw std::invalid_argument("Запрошено больше уникальных чисел, чем есть в диапазоне");
     }
 
     std::vector<SizeType> seq(diap);
     for (SizeType i = 0; i < diap - 1; ++i)
     {
         seq.at(i) = i + 1; // Заполняем диапазон значениями от 1 до diap
     }
 
     // Инициализируем генератор случайных чисел
     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
     std::shuffle(seq.begin(), seq.end(), std::mt19937(seed));
 
     // Берём первые n уникальных элементов
     return std::vector<SizeType>(seq.begin(), seq.begin() + n);
 }
 
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
 std::vector<std::pair<SizeType, SizeType>> prufer_unpack(const std::vector<int>& prufer_sequence, int n)
 {
     std::vector<std::pair<SizeType, SizeType>> edges;
 
     // Массив для отслеживания количества появлений каждой вершины в последовательности Прюфера
     std::vector<SizeType> degree(n + 1, 1); // Все вершины изначально имеют степень 1
     for (SizeType v : prufer_sequence)
     {
         degree.at(v)++;
     }
 
     // Мин-куча для хранения минимальных вершин
     std::priority_queue<SizeType, std::vector<SizeType>, std::greater<SizeType>> min_heap;
     for (int i = 1; i <= n; ++i)
     {
         if (degree.at(i) == 1)
         { // Если вершина встречается только один раз
             min_heap.push(i);
         }
     }
 
     // Восстанавливаем дерево
     for (int i = 0; i < n - 2; ++i)
     {
         SizeType v = min_heap.top();
         min_heap.pop();
 
         SizeType u = prufer_sequence.at(i);
 
         // Добавляем ребро, гарантируя порядок (меньшее число первым)
         edges.emplace_back(std::min(u, v), std::max(u, v));
 
         // Уменьшаем степень вершины
         if (--degree.at(u) == 1)
         {
             min_heap.push(u);
         }
     }
 
     // Добавляем последнее ребро
     SizeType v1 = min_heap.top();
     min_heap.pop();
     SizeType v2 = min_heap.top();
 
     edges.emplace_back(std::min(v1, v2), std::max(v1, v2));
 
     return edges;
 }
 
 void write_dot_file(const List<std::pair<SizeType, SizeType>>& edges, const std::string& filename)
 {
     std::ofstream file(filename);
     if (!file)
     {
         throw std::runtime_error("Error opening file " + filename);
     }
     if (file.is_open())
     {
         file << "graph G {\n";
         for (const auto& edge : edges)
         {
             file << "    " << edge.first << " -- " << edge.second << ";\n";
             //std::cout << "    " << edge.first << " -- " << edge.second << ";\n";
         }
         file << "}\n";
         file.close();
         file.flush();
     }
     //std::cout << "Graph written to " << filename << std::endl;
     file.flush();
     return;
 }
 
 // Вычисление квадратного корня с точностью до одного одного знака после запятой
 double sq(SizeType N)
 {
     SizeType a = static_cast<SizeType>(std::sqrt(N)); // Целая часть корня
     double remainder = N - a * a;                     // Остаток при возведении в квадрат
     return a + (remainder) / (2 * a);                 // Формула для первой десятичной цифры
 }
 
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
     return { a, b };
 }
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
    int n, const std::vector<std::pair<SizeType, SizeType>>& existing_pairs, double density)
{
    SizeType T = n * (n - 1) / 2; // Общее количество возможных пар

    // Используем unordered_set для быстрого поиска
    std::unordered_set<SizeType> existing_set;
    for (const auto& pair : existing_pairs) {
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

    std::cout << "Number of new pairs to generate: l = " << l << std::endl;
    std::cout << "With density = " << double(l + n - 1)/double(T) << std::endl;
    if (l == 0) return existing_pairs;

    // Генерация списка доступных индексов (не входящих в existing_pairs)
    std::vector<SizeType> available_indices;
    available_indices.reserve(T - existing_pairs.size());

    for (SizeType i = 0; i < T; i++) {
        if (existing_set.find(i) == existing_set.end()) {
            available_indices.push_back(i);
        }
    }
    std::cout << "l to generate l = " << l << std::endl;
    // Выбор l случайных индексов с помощью std::sample
    std::vector<SizeType> new_pairs;
    new_pairs.reserve(l);
    std::sample(available_indices.begin(), available_indices.end(), std::back_inserter(new_pairs), l, gen);

    // Формируем итоговый список пар
    std::vector<std::pair<SizeType, SizeType>> graph_pairs;
    graph_pairs.reserve(existing_pairs.size() + l);

    for (SizeType idx : new_pairs) {
        graph_pairs.push_back(pair_from_index(idx, n));
    }
    graph_pairs.insert(graph_pairs.end(), existing_pairs.begin(), existing_pairs.end());

    return graph_pairs;
}


 int main(int argc, char* argv[])
 {
     if (argc != 3) {
         std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
         return 1;
     }
     int n = std::atoi(argv[1]);  // Получаем значение N из аргументов командной строки
     double density = std::atoi(argv[2]);  // Получаем значение плотности из аргументов командной строки

     List<int> prufer_sequence = prufer_gen(n);
     List<std::pair<SizeType, SizeType>> edges = prufer_unpack(prufer_sequence, n);
     List<std::pair<SizeType, SizeType>> new_pairs = generate_new_pairs(n, edges, density);
     //write_dot_file(new_pairs, "graph.dot");

     return 0;
 }