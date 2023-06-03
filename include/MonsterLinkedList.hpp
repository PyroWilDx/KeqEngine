//
// Created by pyrowildx on 03/06/23.
//

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "Monster.hpp"

// First cell has (monster == nullptr), the list really start after this first cell.
class MonsterLinkedList {

public:
    MonsterLinkedList();

    void insert(Monster *monster);

    void operateAllCells(bool (*operation)(Monster *, void *, void *),
                         void *params, void *retVal);

    void deleteSelf();

    void deleteAllCells();

private:
    Monster *monster;
    MonsterLinkedList *prev;
    MonsterLinkedList *next;

};

#endif
