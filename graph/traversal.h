
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

    Traverser(List<Node>* nodes) : m_pNodes(nodes)
    {}

    template <class StorageType>
    void traverse(SizeType from, SizeType to)
    {
        StorageType toVisit;
        toVisit.push(from);
        SizeType cur = from;
        while (cur != to)
        {
            cur = extractElem<StorageType>(toVisit);
            const auto& curNode = m_pNodes->at(cur);
            m_visited.insert(cur);
            m_visitOrder.push_back(cur);
            for (auto elem : curNode.incident)
                if (!m_visited.count(elem))
                {
                    toVisit.push(elem);
                    m_prev[elem] = cur;
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

    const List<SizeType>& getTraverseOrder()
    {
        return m_visitOrder;
    }

    // длина обхода: х
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

    // очистка для запуска нового обхода
    void clear()
    {
        m_visited.clear();
        m_prev.clear();
        m_visitOrder.clear();
    }
    private:

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