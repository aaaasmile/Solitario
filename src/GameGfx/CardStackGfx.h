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

    void PushCard(const CardGfx card);
    void PushStack(CardStackGfx cardstack);

    CardGfx PopCard();
    CardStackGfx PopStack(int items);

    CardGfx RemoveCard(int index);
    void InsertCard(int index, CardGfx card);

    int GetClickedCard(int x, int y);
    CardGfx GetCard(int index);

    void SetCardsFaceUp(bool fTrue);
};

#endif
