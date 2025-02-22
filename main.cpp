#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <random>

template <class T>
using SharedPtr = std::shared_ptr<T>;

template <class T>
using Set = std::unordered_set<T>;

template <class T, class V>
using Map = std::unordered_map<T, V>;

template <class T>
using List = std::vector<T>;

struct Node;

using PNode = SharedPtr<Node>;


struct Node
{
	Node(int id, List<PNode> nodes) : data(id), incident(nodes)
		{ }

	int data;
	List<PNode> incident;
};


int rand(std::mt19937& rng, int min, int max)
{
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
	return dist(rng);
}


template<class T>
List<int> traverse(const List<PNode>& nodes)
{
	Set<PNode> visited;
}


List<PNode> get_tree(unsigned int size)
{
	if (size == 0)
		return {};

	std::random_device dev;
	std::mt19937 rng(dev());

	List<PNode> nodes;
	nodes.reserve(size);
	nodes.push_back(std::make_shared<Node>(0, List<PNode>{}));

	for (int i = 1; i < size; ++i)
	{
		int ancestorNode = rand(rng, 0, i - 1);
		nodes.push_back(std::make_shared<Node>(i, List<PNode>{nodes[ancestorNode]}));
		nodes[ancestorNode]->incident.push_back(nodes.back());
	}

	return nodes;
}


int main()
{
	Set<PNode> visited;

	List<PNode> nodes = get_tree(100);
	
	return 0;
}
