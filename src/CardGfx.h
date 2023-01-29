#ifndef CARDGFX__H
#define CARDGFX__H

#include <SDL.h>

#include <vector>

#include "ErrorInfo.h"

// if will be using custom cards then these should be initialized outside
extern int g_CardWidth;
extern int g_CardHeight;
extern int g_SymbolWidth;
extern int g_SymbolHeight;

enum eSUIT { BASTONI = 0, COPPE = 1, DENARI = 2, SPADE = 3, UNDEF = 4 };

enum { PLAYER_ME = 0, NUM_CARDS = 40, NOT_VALID_INDEX = -1 };

extern int g_PointsSolitario[];

class CardGfx {
public:
    CardGfx() {
        _idx = NOT_VALID_INDEX;
        _faceUp = true;
        _iValue = 0;
        _eSuit = UNDEF;
    }
    CardGfx(int index) {
        if (index < 0)
            index = 0;

        _idx = index;
        _faceUp = true;
        _iValue = 0;
        _eSuit = UNDEF;
    }

    eSUIT Suit() const { return _eSuit; }
    const char* SuitStr();
    int Rank() const { return _iValue; }

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

    void SetFaceUp(bool bval) { _faceUp = bval; }
    LPErrInApp DrawCardPac(SDL_Surface* s);

    int Index() { return _idx; }
    LPErrInApp SetIdx(int nIdx) {
        if (nIdx >= NUM_CARDS || nIdx < 0) {
            return ERR_UTIL::ErrorCreate("Error SetIdx %d is out of range",
                                         nIdx);
        }

        _idx = nIdx;
        _iValue = g_PointsSolitario[nIdx];
        if (_idx >= 0 && _idx <= 9)
            _eSuit = BASTONI;
        else if (_idx > 9 && _idx <= 19)
            _eSuit = COPPE;
        else if (_idx > 19 && _idx <= 29)
            _eSuit = DENARI;
        else if (_idx > 29 && _idx <= 39)
            _eSuit = SPADE;
        else
            _eSuit = UNDEF;

        return NULL;
    }

    void SetCardLoc(int lx, int ly) {
        _x = lx;
        _y = ly;
    }

    bool PtInCard(int lx, int ly) {
        if (lx >= _x && lx <= g_CardWidth + _x && ly >= _y &&
            ly <= g_CardHeight + _y)
            return true;
        else
            return false;
    }

private:
    bool _faceUp;
    eSUIT _eSuit;
    int _iValue;

    int _x;
    int _y;
    int _idx;
    int _width;
    int _height;

    SDL_Surface* _pPacDeck;
};

typedef CardGfx* LPCardGfx;
typedef std::vector<LPCardGfx>::iterator VI;

#endif
