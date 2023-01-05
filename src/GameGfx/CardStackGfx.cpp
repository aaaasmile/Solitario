#include "CardStackGfx.h"

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
    Clear();

    for (size_t i = 0; i < NUM_CARDS; i++) {
        LPCardGfx pCard = new CardGfx;
        err = pCard->SetIdx(i);
        if (err != NULL) {
            return err;
        }
        this->push_back(pCard);
    }

    return NULL;
}

void CardStackGfx::Shuffle() {
    std::random_shuffle(this->begin(), this->end());
}
void CardStackGfx::Clear() {
    for (VI vi = this->begin(); vi != this->end(); ++vi) {
        LPCardGfx pCard = *vi;
        delete pCard;
    }
    this->clear();
}
void CardStackGfx::Reverse() { std::reverse(this->begin(), this->end()); }

bool SortRank(const LPCardGfx& l, const LPCardGfx& r) {
    return l->Rank() < r->Rank();
}
void CardStackGfx::Sort() { std::sort(this->begin(), this->end(), SortRank); }

void CardStackGfx::PushCard(const LPCardGfx pCard) { this->push_back(pCard); }
void CardStackGfx::PushStack(LPCardStackGfx pCardstack) {
    for (VI vi = pCardstack->begin(); vi != pCardstack->end(); ++vi)
        this->push_back(*vi);
}

LPCardGfx CardStackGfx::PopCard() {
    if (Empty())
        return NULL;

    LPCardGfx pCard = this->back();
    this->pop_back();

    return pCard;
}

LPCardStackGfx CardStackGfx::PopStack(int items) {
    if (items > Size())
        items = Size();

    LPCardStackGfx pCardStack = new CardStackGfx;

    while (items > 0) {
        LPCardGfx pCard = this->PopCard();
        pCardStack->PushCard(pCard);
        items--;
    }
    if (pCardStack->Size() > 1) {
        pCardStack->Reverse();
    }
    return pCardStack;
}

LPCardGfx CardStackGfx::RemoveCard(int index) {
    if (this->Empty())
        return NULL;

    VI vi = this->begin() + index;
    LPCardGfx card = *vi;
    this->erase(vi);

    return card;
}

void CardStackGfx::InsertCard(int index, LPCardGfx card) {
    VI vi = this->begin() + index;
    this->insert(vi, card);
}

int CardStackGfx::GetClickedCard(int x, int y) {
    int i = this->Size() - 1;

    for (VI vi = this->end() - 1; vi >= this->begin(); --vi) {
        LPCardGfx pCard = *vi;
        if (pCard->PtInCard(x, y))
            return i;
        i--;
    }

    return -1;
}

LPCardGfx CardStackGfx::GetCard(int index) {
    if (this->Empty())
        return NULL;

    VI vi = this->begin() + index;
    LPCardGfx pCard = *vi;

    return pCard;
}

void CardStackGfx::SetCardsFaceUp(bool bval) {
    for (VI vi = this->begin(); vi != this->end(); ++vi) {
        LPCardGfx pCard = *vi;
        pCard->SetFaceUp(bval);
    }
}
