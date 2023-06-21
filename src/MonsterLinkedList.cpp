//
// Created by pyrowildx on 03/06/23.
//

#include <cstdio>
#include "MonsterLinkedList.hpp"

MonsterLinkedList::MonsterLinkedList() {
    monster = nullptr;
    prev = nullptr;
    next = nullptr;
}

void MonsterLinkedList::insert(Monster *monster_) {
    if (monster == nullptr) {
        MonsterLinkedList *formerHead = this->next;
        auto *newHead = new MonsterLinkedList();
        newHead->monster = monster_;
        newHead->prev = this;
        newHead->next = formerHead;
        if (formerHead != nullptr) {
            formerHead->prev = newHead;
        }
        this->next = newHead;
    } else {
        printf("WARNING, trying to insert a new cell in a MonsterLinkedList that is not the head!\n");
    }
}

void MonsterLinkedList::operateAllCells(bool (*operation)(Monster *, void *, void *),
                                        void *params, void *retVal) {
    MonsterLinkedList *tmp = this->next;
    MonsterLinkedList *tmpNext;
    while (tmp != nullptr) {
        tmpNext = tmp->next;
        bool shouldDelete = operation(tmp->monster, params, retVal);
        if (shouldDelete) tmp->deleteSelf();
        tmp = tmpNext;
    }
}

void MonsterLinkedList::deleteSelf() {
    if (monster != nullptr) {
        MonsterLinkedList *prevLL = this->prev;
        MonsterLinkedList *nextLL = this->next;
        this->monster->destroy();
        delete this->monster;
        delete this;
        prevLL->next = nextLL;
        if (nextLL != nullptr) {
            nextLL->prev = prevLL;
        }
    } else {
        printf("WARNING, trying to delete the first cell that does not represent a monster!\n");
    }

}

void MonsterLinkedList::deleteAllCells() {
    if (monster == nullptr) {
        MonsterLinkedList *tmp = this->next;
        MonsterLinkedList *tmpNext;
        while (tmp != nullptr) {
            tmpNext = tmp->next;
            tmp->monster->destroy();
            delete tmp->monster;
            delete tmp;
            tmp = tmpNext;
        }
        this->next = nullptr;
    } else {
        printf("WARNING, trying to delete all cells not from the head!\n");
    }
}
