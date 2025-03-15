#include "traversal.h"
#include <stdexcept>

// Шаблонный метод traverse
template <class StorageType>
void Traverser::traverse(SizeType from, SizeType to)
{
    // СД для хранения порядка обхода
    StorageType toVisit;
    toVisit.push(from);
    SizeType cur = from;
    
    while (cur != to)
    {
        // У stack и queue разные методы, поэтому завернули в шаблон
        cur = extractElem(toVisit);
        // Запоминаем, что зашли в вершину, и помещаем ее в историю
        m_visited.insert(cur);
        m_visitOrder.push_back(cur);
        const auto& curNode = m_pNodes->at(cur);
        for (auto elem : curNode.incident)
            if (!m_visited.count(elem)) //< все вершины, которые еще не посещали
            {
                toVisit.push(elem); //< помещаем в СД обхода
                m_visited.insert(elem); //< отмечаем, что посетили
                m_prev[elem] = cur; //< и запоминаем, откуда в них пришли
            }
        
    }
}

template <class StorageType>
void Traverser::traverseInv(SizeType from, SizeType to)
{
    // СД для хранения порядка обхода
    StorageType toVisit;
    toVisit.push(from);
    SizeType cur = from;

    while (cur != to)
    {
        // У stack и queue разные методы, поэтому завернули в шаблон
        cur = extractElem(toVisit);
        // Запоминаем, что зашли в вершину, и помещаем ее в историю
        m_visited.insert(cur);
        m_visitOrder.push_back(cur);
        const auto& curNode = m_pNodes->at(cur);
        for (SizeType i = 0; i < m_pNodes->size(); ++i)
        if (curNode.incident.count(i) == 0) //< если ребро не удалено
            if (!m_visited.count(i)) //< все вершины, которые еще не посещали
            {
                toVisit.push(i); //< помещаем в СД обхода
                m_visited.insert(i); //< отмечаем, что посетили
                m_prev[i] = cur; //< и запоминаем, откуда в них пришли
            }
        
    }
}

// Шаблонный метод traverse
template <class StorageType>
void Traverser::traverse(SizeType from, SizeType to, double density)
{
    if (density >= MIN_INVERSE_DENSITY)
        traverseInv<StorageType>(from, to);
    else
        traverse<StorageType>(from, to);
}

// Шаблонный метод traverseRand
template <class StorageType>
void Traverser::traverseRand(double density)
{
    Randomizer rand;
    SizeType from = rand.uRand(0, m_pNodes->size() - 1);
    SizeType to = from;
    while (from == to)
        to = rand.uRand(0, m_pNodes->size() - 1);
    
    traverse<StorageType>(from, to, density);
}

// Метод getTraverseOrder
const List<SizeType>& Traverser::getTraverseOrder()
{
    return m_visitOrder;
}

SizeType Traverser::getFirst()
{
    return *m_visitOrder.begin();
}

SizeType Traverser::getLast()
{
    return m_visitOrder.back();
}

// Метод getPath
List<SizeType> Traverser::getPath()
{
    SizeType cur = m_visitOrder.back();
    List<SizeType> path{cur};
    while (m_prev.count(cur))
    {
        cur = m_prev[cur];
        path.push_back(cur);
    }
    return path;
}

// Метод clear
void Traverser::clear()
{
    m_visited.clear();
    m_prev.clear();
    m_visitOrder.clear();
}

// ========== Реализация метода extractElem ==========

/// Специализация для std::queue
template<>
SizeType Traverser::extractElem(std::queue<SizeType>& storage)
{
    SizeType elem = storage.front();
    storage.pop();
    return elem;
}

// Специализация для std::stack
template<>
SizeType Traverser::extractElem(std::stack<SizeType>& storage)
{
    SizeType elem = storage.top();
    storage.pop();
    return elem;
}

// Общая версия extractElem (для неподдерживаемых типов)
template<class StorageType>
SizeType Traverser::extractElem(StorageType& storage)
{
    throw std::logic_error("extractElem is not specialized for this type");
}


template void Traverser::traverse<class std::queue<SizeType>>(SizeType from, SizeType to, double);
template void Traverser::traverse<class std::stack<SizeType>>(SizeType from, SizeType to, double);