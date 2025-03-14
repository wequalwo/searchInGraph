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
     * Функция для обхода графа с заданной плотностью
     * 
     * @tparam StorageType тип стека или очереди, используемый для хранения порядка обхода
     * @param from начальная вершина
     * @param to конечная вершина
     * @param density плотность графа: в случае большой плотности будет выбран инвертированный обход
     */
    template <class StorageType>
    void traverse(SizeType from, SizeType to, double density);

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
    void traverseRand(double density);

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

//template<class StorageType> void
//Traverser::traverse(SizeType from, SizeType to, double density);

#endif // TRAVERSAL_H