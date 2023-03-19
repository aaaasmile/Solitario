#ifndef CARDGFX__H
#define CARDGFX__H

#include <SDL.h>

#include <vector>

#include "DeckType.h"
#include "ErrorInfo.h"
#include "WinTypeGlobal.h"

enum eSUIT { BASTONI = 0, COPPE = 1, DENARI = 2, SPADE = 3 };

class CardGfx {
public:
    CardGfx();
    eSUIT Suit() const { return _eSuit; }
    const char* SuitStr();
    int Rank() const { return _rank; }

    bool IsBlack() const { return _eSuit == BASTONI || _eSuit == SPADE; }
    bool IsRed() const { return !IsBlack(); }
    int X() { return _x; }
    int Y() { return _y; }
    int Width() { return _width; }
    void SetWidth(int val) { _width = val; }
    void SetHeight(int val) { _height = val; }
    int Height() { return _height; }
    void SetDeckSurface(SDL_Surface* pVal) { _pPacDeck = pVal; }

    bool IsFaceUp() const { return _faceUp; }
    bool IsFaceDown() const { return !_faceUp; }
    LPCSTR Name() { return _name.c_str(); }
    LPCSTR String();

    void SetFaceUp(bool bval) { _faceUp = bval; }
    LPErrInApp DrawCardPac(SDL_Surface* s);

    int Index() { return _idx; }
    LPErrInApp SetIdx(int nIdx, DeckType& deckType);
    void SetCardLoc(int lx, int ly) {
        _x = lx;
        _y = ly;
    }

    bool PtInCard(int lx, int ly) {
        SDL_assert(_width != 0);
        SDL_assert(_height != 0);
        if (lx >= _x && lx <= _width + _x && ly >= _y && ly <= _height + _y)
            return true;
        else
            return false;
    }

private:
    bool _faceUp;
    eSUIT _eSuit;
    int _rank;

    int _x;
    int _y;
    int _idx;
    int _width;
    int _height;

    SDL_Surface* _pPacDeck;
    DeckType _deckType;
    STRING _name;
    STRING _nameFull;
};

typedef CardGfx* LPCardGfx;
typedef std::vector<LPCardGfx>::iterator VI;

#endif
