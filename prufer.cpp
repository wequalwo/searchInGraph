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
 
 template <class T>
 using Set = std::unordered_set<T>;
 
 // Функция для генерации последовательности Пруфера
 List<int> prufer_gen(int n)
 {
     List<int> prufer_sequence(n - 2);
 
     // Используем текущее время как начальное значение для генератора
     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
     std::mt19937 gen(seed);
     std::uniform_int_distribution<> dist(1, n);
 
     for (int i = 0; i < n - 2; ++i) {
         prufer_sequence[i] = dist(gen);
     }
 
     return prufer_sequence;
 }
 
 /**
  * Функция для восстановления дерева из последовательности Прюфера
  * Оптимизированная реализация
  * @param prufer_sequence Последовательность Пруфера
  * @param n Количество вершин в дереве
  * @return Вектор пар, представляющий дерево
  * @complexity O(n)
  * @time O(n)
  * @space O(n)
  * Версия с сортировкой вершин
  */
 std::vector<std::pair<uint64_t, uint64_t>> prufer_unpack(const std::vector<int>& prufer_sequence, int n)
 {
     std::vector<std::pair<uint64_t, uint64_t>> edges;
 
     // Массив для отслеживания количества появлений каждой вершины в последовательности Прюфера
     std::vector<uint64_t> degree(n + 1, 1);  // Все вершины изначально имеют степень 1
     for (uint64_t v : prufer_sequence) {
         degree[v]++;
     }
 
     // Мин-куча для хранения минимальных вершин
     std::priority_queue<uint64_t, std::vector<uint64_t>, std::greater<uint64_t>> min_heap;
     for (int i = 1; i <= n; ++i) {
         if (degree[i] == 1) {  // Если вершина встречается только один раз
             min_heap.push(i);
         }
     }
 
     // Восстанавливаем дерево
     for (int i = 0; i < n - 2; ++i) {
         uint64_t v = min_heap.top();
         min_heap.pop();
 
         uint64_t u = prufer_sequence[i];
 
         // Добавляем ребро, гарантируя порядок (меньшее число первым)
         edges.emplace_back(std::min(u, v), std::max(u, v));
 
         // Уменьшаем степень вершины
         if (--degree[u] == 1) {
             min_heap.push(u);
         }
     }
 
     // Добавляем последнее ребро
     uint64_t v1 = min_heap.top();
     min_heap.pop();
     uint64_t v2 = min_heap.top();
 
     edges.emplace_back(std::min(v1, v2), std::max(v1, v2));
 
     return edges;
 }
 
 void write_dot_file(const List<std::pair<uint64_t, uint64_t>>& edges, const std::string& filename)
 {
     std::ofstream file(filename);
 
     if (file.is_open()) {
         file << "graph G {\n";
         for (const auto& edge : edges) {
             file << "    " << edge.first << " -- " << edge.second << ";\n";
         }
         file << "}\n";
         file.close();
         std::cout << "Graph written to " << filename << std::endl;
     } else {
         std::cerr << "Error opening file " << filename << std::endl;
     }
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
 std::pair<int, int> pair_from_index(int index, int n)
 {
     // Решаем квадратное уравнение a^2 - a - 2 * index = 0
     // Берем ближайшее целое слева
     int a = (2 * n + 1 - std::sqrt((2 * n + 1) * (2 * n + 1) - 4 * (2 * index + 2 * n))) / 2;
     // Вычисляем оставшийся индекс
     int index_in_row = index - (a - 1) * (2 * n - 1) / 2;
 
     int b = a + 1 + index_in_row;
 
     return {a, b};
 }
 
 /*
 // Функция генерации новых пар
 // Работает долго, делает лишннюю разномизацию
 List<std::pair<int, int>> generate_new_pairs(int n, const List<std::pair<int, int>>& existing_pairs) {
     int T = n * (n - 1) / 2;  // Всего возможных пар
     Set<int> used_indices;  // Храним занятые индексы
 
     // Преобразуем существующие пары в индексы
     for (const auto& p : existing_pairs) {
         int a = std::min(p.first, p.second), b = std::max(p.first, p.second);
         int index = (a - 1) * (2 * n - a) / 2 + (b - a - 1);
         used_indices.insert(index);
     }
 
     // Создаём список доступных индексов
     std::vector<int> available_indices;
     for (int i = 0; i < T; ++i) {
         if (used_indices.find(i) == used_indices.end()) {
             available_indices.push_back(i);
         }
     }
 
     // Определяем l — количество новых пар
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<int> dist(0, available_indices.size() - 1);
     int l = dist(gen);
     printf("Number of new pairs to generate: l = %d\n", l);
 
     // Выбираем l случайных индексов
     std::shuffle(available_indices.begin(), available_indices.end(), gen);
     std::vector<int> chosen_indices(available_indices.begin(), available_indices.begin() + l);
 
     // Преобразуем индексы обратно в пары
     List<std::pair<int, int>> new_pairs;
     for (int idx : chosen_indices) {
         new_pairs.push_back(pair_from_index(idx, n));
     }
 
     return new_pairs;
 }
 */
 
 /**
  * Версия без использования множества доступных индексов
  * Работает значительно быстрее, чем обощенная версия
  * Пока отключена, т.к. показала себя минимально хуже, чем версия без использования индексов
  * v1.9
 **/
 /*
 List<std::pair<uint64_t, uint64_t>> generate_new_pairs(uint64_t n, const List<std::pair<uint64_t, uint64_t>>& existing_pairs) {
     uint64_t T = n * (n - 1) / 2;  // Всего возможных пар
     Set<uint64_t> used_indices;  // Храним занятые индексы
 
     // Преобразуем существующие пары в индексы
     for (const auto& p : existing_pairs) {
         uint64_t a = std::min(p.first, p.second), b = std::max(p.first, p.second);
         uint64_t index = (a - 1) * (2 * n - a) / 2 + (b - a - 1);
         used_indices.insert(index);
     }
 
     // Определяем l — количество новых пар
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<int> dist(0, n*(n - 1)/2 - used_indices.size() - 1);
     uint64_t l = dist(gen);
     printf("Number of new pairs to generate: l = %d\n", l);
 
     // Преобразуем индексы обратно в пары, выбирая l случайных индексов
     List<std::pair<uint64_t, uint64_t>> new_pairs;
     for (uint64_t idx = 0; idx <= l; ++idx) {
         if (used_indices.find(idx) != used_indices.end()) {
             l++; // Увеличиваем l, если индекс уже занят
         }
         // printf("Index: %d\n", idx);
         new_pairs.push_back(pair_from_index(idx, n));
         // printf("Pair: (%d, %d)\n", new_pairs.back().first, new_pairs.back().second);
     }
 
     return new_pairs;
 }
 */
 
 /**
  * Версия без использования множества доступных индексов
  * Работает значительно быстрее, чем обощенная версия
  * @param n
  * @param existing_pairs
  * Версия без использования индексов
  * 2.1
 **/
std::vector<std::pair<uint64_t, uint64_t>> generate_new_pairs(
    uint64_t n,
    const std::vector<std::pair<uint64_t, uint64_t>>& existing_pairs)
{
    uint64_t T = n * (n - 1) / 2;  // Общее количество возможных пар

    // Сортируем существующие пары для быстрого поиска
    std::set<std::pair<uint64_t, uint64_t>> existing_set(existing_pairs.begin(), existing_pairs.end());

    // Определяем l — сколько новых пар нужно добавить
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, T - existing_pairs.size());
    uint64_t l = dist(gen);
    //l = n * (n - 1) / 2;
    if (l == 0) {
        return existing_pairs;
    }
    std::vector<std::pair<uint64_t, uint64_t>> new_pairs(l + existing_pairs.size());
    uint64_t new_count = 0;  // Счётчик новых пар

    // Перебираем все возможные пары (i, j), где 1 <= i < j <= n
    for (uint64_t i = 1; i <= n; ++i) {
        for (uint64_t j = i + 1; j <= n; ++j) {
            std::pair<uint64_t, uint64_t> p = {i, j};

            if (existing_set.count(p)) {
                // Если пара уже есть, пропускаем её
                continue;
            }
    
            // Если пары нет, добавляем её и увеличиваем счётчик
            new_pairs[new_count++] = p;
            printf("Pair: (%d, %d)\n", new_pairs[new_count - 1].first, new_pairs[new_count - 1].second);
            // Если достигли l новых элементов, выходим
            printf("new_count = %d, l = %d\n", new_count, l);
            if (new_count == l) {
                // Копируем оставшиеся элементы из existing_pairs в конец new_pairs
                for (auto it = existing_pairs.begin(); it != existing_pairs.end(); ++it) {
                    new_pairs[new_count++] = *it;
                }
                return new_pairs;
            }
        }
    }
    throw std::runtime_error("Failed to generate new pairs");
}
 
 
 int main(int argc, char* argv[])
 {
     if (argc != 2) {
         std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
         return 1;
     }
     int n = std::atoi(argv[1]);  // Получаем значение N из аргументов командной строки
     List<int> prufer_sequence = prufer_gen(n);  // Генерация последовательности Пруфера
     // Восстановление дерева из последовательности Пруфера
     List<std::pair<uint64_t, uint64_t>> edges = prufer_unpack(prufer_sequence, n);
     printf("Tree has generaded\n");
     write_dot_file(edges, "tree.dot");

     // Генерация новых пар (добавление новых рёбер)
     List<std::pair<uint64_t, uint64_t>> new_pairs = generate_new_pairs(n, edges);
     printf("Graph has generaded\n");
     write_dot_file(new_pairs, "graph.dot");
     return 0;
}