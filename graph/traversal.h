
#pragma once
#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include <stack>
#include <queue>

#include "graph/node.h"
#include "randomizer/rand.h"

class Traverser
{
public:
    // указатель, чтобы не копировать список
    Traverser(List<Node>* nodes) : m_pNodes(nodes)
    {} //< вероятно, имеет смысл зарезервировать место для некоторого числа вершин во вспомогательных структурах 

    template <class StorageType>
    void traverse(SizeType from, SizeType to)
    {
        // СД для хранения порядка обхода
        StorageType toVisit;
        toVisit.push(from);
        SizeType cur = from;
        while (cur != to)
        {
            // У stack и queue разные методы, поэтому завернули в шаблон
            cur = extractElem<StorageType>(toVisit);
            const auto& curNode = m_pNodes->at(cur);
            // Запоминаем, что зашли в вершину, и помещаем ее в историю
            m_visited.insert(cur);
            m_visitOrder.push_back(cur);
            for (auto elem : curNode.incident)
                if (!m_visited.count(elem)) //< все вершины, которые еще не посещали
                {
                    toVisit.push(elem); //< помещаем в СД обхода
                    m_prev[elem] = cur; //< и запоминаем, откуда в них пришли
                }
        }
    }

    template <class StorageType>
    void traverseRand()
    {
        Randomizer rand;
        SizeType from = rand.uRand(0, m_pNodes->size() - 1);
        SizeType to = from;
        while (from == to)
            to = rand.uRand(0, m_pNodes->size() - 1);
        return traverse<StorageType>(from, to);
    }

    // Позволяет восстановить начало и конец маршрута: первая вершина — откуда начали, последняя вершина — куда пришли.
    const List<SizeType>& getTraverseOrder()
    {
        return m_visitOrder;
    }

    // Позволяет установить расстояние между вершинами для обхода в ширину.
    // Для этого переходим в предыдущую вершину, пока не окажемся в первой. 
    SizeType getDist()
    {
        SizeType cur = m_visitOrder.back();
        SizeType dist = 0;
        while (m_prev.count(cur))
        {
            cur = m_prev[cur];
            ++dist;
        }
        return dist;
    }

    // очистка всех СД для запуска нового обхода
    void clear()
    {
        m_visited.clear();
        m_prev.clear();
        m_visitOrder.clear();
    }
    private:

    // у стека и очереди разные интерфейсы, поэтому завернул извлечение элемента в специализированный шаблон
    template<class StorageType>
    SizeType extractElem(StorageType& storage);

    template<>
    SizeType extractElem(std::queue<SizeType>& storage)
    {
        SizeType elem = storage.front();
        storage.pop();
        return elem;
    }

    template<>
    SizeType extractElem(std::stack<SizeType>& storage)
    {
        SizeType elem = storage.top();
        storage.pop();
        return elem;
    }

    List<Node>* m_pNodes;
    Set<SizeType> m_visited;
    Map<SizeType, SizeType> m_prev;
    List<SizeType> m_visitOrder;
};


#endif // TRAVERSAL_H