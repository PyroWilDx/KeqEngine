//
// Created by pyrowildx on 27/07/2023.
//

#include "Utils/LinkedList.hpp"

LinkedList *LLInsertHead(LinkedList *ll, void *value) {
    auto *newCell = new LinkedList;
    newCell->value = value;
    newCell->prev = nullptr;
    newCell->next = ll;
    if (ll != nullptr) ll->prev = newCell;
    return newCell;
}

LinkedList *LLRemoveCell(LinkedList *rmCell, void (*freeValueF)(void *)) {
    LinkedList *newHead = (rmCell->prev != nullptr) ? rmCell->prev : rmCell->next;

    if (rmCell->prev != nullptr) { // Not First
        rmCell->prev->next = rmCell->next;
    }
    if (rmCell->next != nullptr) { // Not Last
        rmCell->next->prev = rmCell->prev;
    }
    if (freeValueF != nullptr) freeValueF(rmCell->value);
    delete rmCell;

    return newHead;
}

void LLIterate(LinkedList *ll, void (*f)(void *, void *), void *fParams) {
    LinkedList *currCell = ll;
    while (currCell != nullptr) {
        f(currCell->value, fParams);
        currCell = currCell->next;
    }
}

LinkedList *LLIterateMayRemove(LinkedList *ll, bool (*doRM)(void *, void *), void *fParams,
                               void (*freeValueF)(void *)) {
    LinkedList *newHead = ll;
    LinkedList *currCell = ll;
    LinkedList *nextCell;
    while (currCell != nullptr) {
        nextCell = currCell->next;
        if (doRM(currCell->value, fParams)) {
            newHead = LLRemoveCell(currCell, freeValueF);
        }
        currCell = nextCell;
    }
    return newHead;
}

void LLFree(LinkedList *llHead, void (*freeValueF)(void *)) {
    LinkedList *ll = llHead;
    LinkedList *llNext;
    while (ll != nullptr) {
        llNext = ll->next;
        if (freeValueF != nullptr) freeValueF(ll->value);
        delete ll;
        ll = llNext;
    }
}
