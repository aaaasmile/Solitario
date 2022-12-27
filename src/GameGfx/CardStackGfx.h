#ifndef CARDSTACKGFX__H
#define CARDSTACKGFX__H

#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <vector>

#include "CardGfx.h"
#include "ErrorInfo.h"

class CardStackGfx : public std::vector<CardGfx> {
public:
    LPErrInApp NewDeck();
    void Shuffle();
    void Clear();
    void Sort();
    void Reverse();
    bool Empty() { return this->empty(); }
    int Size() { return this->size(); }

    void Push(const CardGfx card);
    void Push(CardStackGfx cardstack);

    CardGfx Pop();
    CardStackGfx Pop(int items);

    CardGfx RemoveCard(int index);
    void InsertCard(int index, CardGfx card);

    int GetClickedCard(int x, int y);
    CardGfx GetCard(int index);

    void SetCardsFaceUp(bool fTrue);
};

#endif
