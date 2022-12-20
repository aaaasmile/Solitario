#ifndef CCARDSTACK_H
#define CCARDSTACK_H

#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <vector>

#include "CCard.h"

class CCardStack : public std::vector<CCard> {
public:
    void NewDeck();
    void Shuffle();
    void Clear();
    void Sort();
    void Reverse();
    bool Empty() { return this->empty(); }
    int Size() { return this->size(); }

    void Push(const CCard card);
    void Push(CCardStack cardstack);

    CCard Pop();
    CCardStack Pop(int items);

    CCard RemoveCard(int index);
    void InsertCard(int index, CCard card);

    int GetClickedCard(int x, int y);
    CCard GetCard(int index);

    void SetCardsFaceUp(bool fTrue);
};

#endif  // CCARDSTACK_H
