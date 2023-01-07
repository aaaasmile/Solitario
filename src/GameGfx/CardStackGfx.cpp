#include "CardStackGfx.h"

#include "WinTypeGlobal.h"

int g_PointsBriscola[] = {
    /*Asso*/ 11, /*Due*/ 0,   /*Tre*/ 10,  /*Quattro*/ 0, /*cinque*/ 0,
    /*Sei*/ 0,   /*Sette*/ 0, /*Fante*/ 2, /*Cavallo*/ 3, /*Re*/ 4,
    /*Asso*/ 11, /*Due*/ 0,   /*Tre*/ 10,  /*Quattro*/ 0, /*cinque*/ 0,
    /*Sei*/ 0,   /*Sette*/ 0, /*Fante*/ 2, /*Cavallo*/ 3, /*Re*/ 4,
    /*Asso*/ 11, /*Due*/ 0,   /*Tre*/ 10,  /*Quattro*/ 0, /*cinque*/ 0,
    /*Sei*/ 0,   /*Sette*/ 0, /*Fante*/ 2, /*Cavallo*/ 3, /*Re*/ 4,
    /*Asso*/ 11, /*Due*/ 0,   /*Tre*/ 10,  /*Quattro*/ 0, /*cinque*/ 0,
    /*Sei*/ 0,   /*Sette*/ 0, /*Fante*/ 2, /*Cavallo*/ 3, /*Re*/ 4};

int g_PointsSolitario[] = {
    /*Asso*/ 1, /*Due*/ 2,   /*Tre*/ 3,   /*Quattro*/ 4, /*cinque*/ 5,
    /*Sei*/ 6,  /*Sette*/ 7, /*Fante*/ 8, /*Cavallo*/ 9, /*Re*/ 10,
    /*Asso*/ 1, /*Due*/ 2,   /*Tre*/ 3,   /*Quattro*/ 4, /*cinque*/ 5,
    /*Sei*/ 6,  /*Sette*/ 7, /*Fante*/ 8, /*Cavallo*/ 9, /*Re*/ 10,
    /*Asso*/ 1, /*Due*/ 2,   /*Tre*/ 3,   /*Quattro*/ 4, /*cinque*/ 5,
    /*Sei*/ 6,  /*Sette*/ 7, /*Fante*/ 8, /*Cavallo*/ 9, /*Re*/ 10,
    /*Asso*/ 1, /*Due*/ 2,   /*Tre*/ 3,   /*Quattro*/ 4, /*cinque*/ 5,
    /*Sei*/ 6,  /*Sette*/ 7, /*Fante*/ 8, /*Cavallo*/ 9, /*Re*/ 10};

LPErrInApp CardStackGfx::NewDeck() {
    LPErrInApp err;
    TRACE("NewDeck: delete card stack %d", _vct_lpCardGfx.size());
    for (VI vi = _vct_lpCardGfx.begin(); vi != _vct_lpCardGfx.end(); ++vi) {
        LPCardGfx pCard = *vi;
        delete pCard;
    }

    for (size_t i = 0; i < NUM_CARDS; i++) {
        LPCardGfx pCard = new CardGfx;
        err = pCard->SetIdx(i);
        if (err != NULL) {
            return err;
        }
        _vct_lpCardGfx.push_back(pCard);
    }

    return NULL;
}

void CardStackGfx::Shuffle() {
    std::random_shuffle(_vct_lpCardGfx.begin(), _vct_lpCardGfx.end());
}
void CardStackGfx::Reverse() {
    std::reverse(_vct_lpCardGfx.begin(), _vct_lpCardGfx.end());
}

bool SortRank(const LPCardGfx& l, const LPCardGfx& r) {
    return l->Rank() < r->Rank();
}
void CardStackGfx::Sort() {
    std::sort(_vct_lpCardGfx.begin(), _vct_lpCardGfx.end(), SortRank);
}

void CardStackGfx::PushCard(const LPCardGfx pCard) {
    _vct_lpCardGfx.push_back(pCard);
}
void CardStackGfx::PushStack(LPCardStackGfx pCardstack) {
    for (VI vi = pCardstack->_vct_lpCardGfx.begin();
         vi != pCardstack->_vct_lpCardGfx.end(); ++vi)
        _vct_lpCardGfx.push_back(*vi);
}

LPCardGfx CardStackGfx::PopCard() {
    if (IsEmpty())
        return NULL;

    LPCardGfx pCard = _vct_lpCardGfx.back();
    _vct_lpCardGfx.pop_back();

    return pCard;
}

LPCardStackGfx CardStackGfx::PopStack(int items) {
    if (items > Size())
        items = Size();

    LPCardStackGfx pCardStack = new CardStackGfx;

    while (items > 0) {
        LPCardGfx pCard = PopCard();
        pCardStack->PushCard(pCard);
        items--;
    }
    if (pCardStack->Size() > 1) {
        pCardStack->Reverse();
    }
    return pCardStack;
}

LPCardGfx CardStackGfx::RemoveCard(int index) {
    if (_vct_lpCardGfx.empty())
        return NULL;

    VI vi = _vct_lpCardGfx.begin() + index;
    LPCardGfx card = *vi;
    _vct_lpCardGfx.erase(vi);

    return card;
}

void CardStackGfx::InsertCard(int index, LPCardGfx card) {
    VI vi = _vct_lpCardGfx.begin() + index;
    _vct_lpCardGfx.insert(vi, card);
}

int CardStackGfx::GetCardWherePointIsInside(int x, int y) {
    int i = _vct_lpCardGfx.size() - 1;

    for (VI vi = _vct_lpCardGfx.end() - 1; vi >= _vct_lpCardGfx.begin(); --vi) {
        LPCardGfx pCard = *vi;
        if (pCard->PtInCard(x, y))
            return i;
        i--;
    }

    return -1;
}

LPCardGfx CardStackGfx::GetCard(int index) {
    if (_vct_lpCardGfx.empty())
        return NULL;

    if (index >= _vct_lpCardGfx.size())
        return NULL;

    VI vi = _vct_lpCardGfx.begin() + index;
    LPCardGfx pCard = *vi;

    return pCard;
}

void CardStackGfx::SetCardsFaceUp(bool bval) {
    for (VI vi = _vct_lpCardGfx.begin(); vi != _vct_lpCardGfx.end(); ++vi) {
        LPCardGfx pCard = *vi;
        pCard->SetFaceUp(bval);
    }
}
