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

    /**
     * Функция для обхода графа между двумя заданными вершинами
     * 
     * @tparam StorageType тип стека или очереди, используемый для хранения порядка обхода
     * @param from начальная вершина
     * @param to конечная вершина
     */
    template <class StorageType>
    void traverse(SizeType from, SizeType to);

        /**
     * Функция для обхода инвертированного графа между двумя заданными вершинами
     * 
     * @tparam StorageType тип стека или очереди, используемый для хранения порядка обхода
     * @param from начальная вершина
     * @param to конечная вершина
     */
    template <class StorageType>
    void traverseInv(SizeType from, SizeType to);

    /**
     * Генерирует случайный путь между двумя случайными вершинами
     * 
     * @tparam StorageType тип стека или очереди, используемый для хранения порядка обхода
     */
    template <class StorageType>
    void traverseRand(bool inverse = false);

    // Позволяет восстановить начало и конец маршрута: первая вершина — откуда начали, последняя вершина — куда пришли.
    const List<SizeType>& getTraverseOrder();

    // Позволяет установить расстояние между вершинами для обхода в ширину.
    // Для этого переходим в предыдущую вершину, пока не окажемся в первой. 
    List<SizeType> getPath();

    SizeType getFirst();

    SizeType getLast();

    // очистка всех СД для запуска нового обхода
    void clear();

private:
    // у стека и очереди разные интерфейсы, поэтому завернул извлечение элемента в специализированный шаблон
    template<class StorageType>
    SizeType extractElem(StorageType& storage);

    List<Node>* m_pNodes;
    Set<SizeType> m_visited;
    Map<SizeType, SizeType> m_prev;
    List<SizeType> m_visitOrder;
};

// ================== Реализация методов вне класса ==================

// Шаблонный метод traverse
template <class StorageType>
void Traverser::traverse(SizeType from, SizeType to)
{
    // СД для хранения порядка обхода
    StorageType toVisit;
    toVisit.push(from);
    SizeType cur = from;
    // Запоминаем, что зашли в вершину, и помещаем ее в историю
    m_visited.insert(cur);
    m_visitOrder.push_back(cur);
    
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
    // Запоминаем, что зашли в вершину, и помещаем ее в историю
    m_visited.insert(cur);
    m_visitOrder.push_back(cur);
    
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

// Шаблонный метод traverseRand
template <class StorageType>
void Traverser::traverseRand(bool inverse = false)
{
    Randomizer rand;
    SizeType from = rand.uRand(0, m_pNodes->size() - 1);
    SizeType to = from;
    while (from == to)
        to = rand.uRand(0, m_pNodes->size() - 1);
    
    return inverse ? traverseInv<StorageType>(from, to) : traverse<StorageType>(from, to);
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

#endif // TRAVERSAL_H