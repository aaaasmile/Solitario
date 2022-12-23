#ifndef CCARDSTACK_H
#define CCARDSTACK_H

#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <vector>

#include "CardGfx.h"

class CCardStack : public std::vector<CardGfx> {
public:
    void NewDeck();
    void Shuffle();
    void Clear();
    void Sort();
    void Reverse();
    bool Empty() { return this->empty(); }
    int Size() { return this->size(); }

    void Push(const CardGfx card);
    void Push(CCardStack cardstack);

    CardGfx Pop();
    CCardStack Pop(int items);

    CardGfx RemoveCard(int index);
    void InsertCard(int index, CardGfx card);

    int GetClickedCard(int x, int y);
    CardGfx GetCard(int index);

    void SetCardsFaceUp(bool fTrue);
};

#endif  // CCARDSTACK_H
