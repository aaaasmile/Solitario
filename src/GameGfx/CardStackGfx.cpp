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
    this->clear();
    this->resize(NUM_CARDS);

    int Id = 0;
    for (VI vi = this->begin(); vi != this->end(); ++vi) {
        err = vi->SetIdx(Id);
        if (err != NULL) {
            return err;
        }
        Id++;
    }
    return NULL;
}

void CardStackGfx::Shuffle() {
    std::random_shuffle(this->begin(), this->end());
}
void CardStackGfx::Clear() { this->clear(); }
void CardStackGfx::Reverse() { std::reverse(this->begin(), this->end()); }
bool SortRank(const CardGfx& l, const CardGfx& r) {
    return l.Rank() < r.Rank();
}
void CardStackGfx::Sort() { std::sort(this->begin(), this->end(), SortRank); }
void CardStackGfx::PushCard(const CardGfx card) { this->push_back(card); }
void CardStackGfx::PushStack(CardStackGfx cardstack) {
    for (VI vi = cardstack.begin(); vi != cardstack.end(); ++vi)
        this->push_back(*vi);
}

CardGfx CardStackGfx::PopCard() {
    if (Empty())
        return 0;

    CardGfx card = this->back();
    this->pop_back();

    return card;
}

CardStackGfx CardStackGfx::PopStack(int items) {
    if (items > Size())
        items = Size();

    CardStackGfx cardstack;

    while (items > 0) {
        cardstack.PushCard(this->PopCard());
        items--;
    }
    cardstack.Reverse();
    return cardstack;
}

CardGfx CardStackGfx::RemoveCard(int index) {
    if (this->Empty())
        return 0;

    VI vi = this->begin() + index;
    CardGfx card = *vi;
    this->erase(vi);

    return card;
}

void CardStackGfx::InsertCard(int index, CardGfx card) {
    VI vi = this->begin() + index;
    this->insert(vi, card);
}

int CardStackGfx::GetClickedCard(int x, int y) {
    int i = this->Size() - 1;

    for (VI vi = this->end() - 1; vi >= this->begin(); --vi) {
        if (vi->PtInCard(x, y))
            return i;
        i--;
    }

    return -1;
}

CardGfx CardStackGfx::GetCard(int index) {
    if (this->Empty())
        return 0;

    VI vi = this->begin() + index;
    CardGfx card = *vi;

    return card;
}

void CardStackGfx::SetCardsFaceUp(bool bval) {
    for (VI vi = this->begin(); vi != this->end(); ++vi) vi->SetFaceUp(bval);
}
