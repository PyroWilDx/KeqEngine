//
// Created by pyrow on 27/07/2023.
//

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

typedef struct LinkedList {
    void *value;
    LinkedList *prev;
    LinkedList *next;
} LinkedList;

[[nodiscard]] LinkedList *LLInsertHead(LinkedList *ll, void *value);

[[nodiscard]] LinkedList *LLRemoveCell(LinkedList *rmCell);

void LLIterate(LinkedList *ll, void (*f)(void *, void *), void *fParams);

void LLFree(LinkedList *llHead, void (*freeValueF)(void *));

#endif
