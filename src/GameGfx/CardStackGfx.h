#ifndef CARDSTACKGFX__H
#define CARDSTACKGFX__H

#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <vector>

#include "CardGfx.h"
#include "ErrorInfo.h"

class CardStackGfx : public std::vector<LPCardGfx> {
public:
    LPErrInApp NewDeck();
    void Shuffle();
    void Clear() { this->clear(); }
    void Sort();
    void Reverse();
    bool IsEmpty() { return this->empty(); }
    int Size() { return this->size(); }

    void PushCard(const LPCardGfx pCard);
    void PushStack(CardStackGfx* pCardstack);

    LPCardGfx PopCard();
    CardStackGfx* PopStack(int items);

    LPCardGfx RemoveCard(int index);
    void InsertCard(int index, LPCardGfx pCard);

    int GetCardWherePointIsInside(int x, int y);
    LPCardGfx GetCard(int index);

    void SetCardsFaceUp(bool bVal);
};

typedef CardStackGfx* LPCardStackGfx;

#endif
