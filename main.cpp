#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <random>

// Определяем удобные псевдонимы с помощью using
template <class T>
using SharedPtr = std::shared_ptr<T>;

template <class T>
using Set = std::unordered_set<T>;

// Хеш-таблица (ассоциативный массив)
template <class T, class V>
using Map = std::unordered_map<T, V>;

template <class T>
using List = std::vector<T>;

// Объявление структуры узла графа
struct Node;

// Определение псевдонима для указателя на Node
using PNode = SharedPtr<Node>;

// Структура узла графа
struct Node
{
	// Конструктор принимает ID узла и список инцидентных узлов
	Node(int id, List<PNode> nodes) : data(id), incident(nodes)
		{ }

	int data;               // Значение узла
	List<PNode> incident;   // Список соседних узлов (смежность)
};

// Функция для генерации случайного числа в диапазоне [min, max]
int rand(std::mt19937& rng, int min, int max)
{
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
	return dist(rng);
}

// Функция обхода графа: TODO
template<class T>
List<int> traverse(const List<PNode>& nodes)
{
	Set<PNode> visited; // Множество посещенных узлов
	// TOTDO: написать код
}

// Функция для генерации случайного дерева
List<PNode> get_tree(unsigned int size)
{
	if (size == 0)
		return {};

	std::random_device dev;
	std::mt19937 rng(dev()); 

	List<PNode> nodes;
	nodes.reserve(size); // Резервируем память под узлы
	nodes.push_back(std::make_shared<Node>(0, List<PNode>{})); // Корневой узел

	// Создание дерева
	for (int i = 1; i < size; ++i)
	{
		int ancestorNode = rand(rng, 0, i - 1); // Выбираем случайного предка
		nodes.push_back(std::make_shared<Node>(i, List<PNode>{nodes[ancestorNode]}));
		nodes[ancestorNode]->incident.push_back(nodes.back()); // Добавляем связь
	}

	return nodes;
}

int main()
{
	Set<PNode> visited; // Множество посещенных узлов
	int N = 1000; // Количество вершин
	List<PNode> nodes = get_tree(N);
	
	return 0;
}