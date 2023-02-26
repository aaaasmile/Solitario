#ifndef CARDGFX__H
#define CARDGFX__H

#include <SDL.h>

#include <vector>

#include "DeckType.h"
#include "ErrorInfo.h"

extern int g_CardWidth;
extern int g_CardHeight;
extern int g_SymbolWidth;
extern int g_SymbolHeight;

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

    void SetFaceUp(bool bval) { _faceUp = bval; }
    LPErrInApp DrawCardPac(SDL_Surface* s);

    int Index() { return _idx; }
    LPErrInApp SetIdx(int nIdx, DeckType& deckType) {
        _idx = nIdx;
        _deckType = deckType;
        _rank = _deckType.GetRank(nIdx);
        if (deckType.GetType() == eDeckType::TAROCK_PIEMONT) {
            if (_idx >= 0 && _idx <= 13)
                _eSuit = BASTONI;
            else if (_idx > 13 && _idx <= 27)
                _eSuit = COPPE;
            else if (_idx > 27 && _idx <= 41)
                _eSuit = DENARI;
            else if (_idx > 41 && _idx <= 55)
                _eSuit = SPADE;
            else
                return ERR_UTIL::ErrorCreate("Error SetIdx %d is out of range",
                                             nIdx);
        } else {
            if (_idx >= 0 && _idx <= 9)
                _eSuit = BASTONI;
            else if (_idx > 9 && _idx <= 19)
                _eSuit = COPPE;
            else if (_idx > 19 && _idx <= 29)
                _eSuit = DENARI;
            else if (_idx > 29 && _idx <= 39)
                _eSuit = SPADE;
            else
                return ERR_UTIL::ErrorCreate("Error SetIdx %d is out of range",
                                             nIdx);
        }

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
    int _rank;

    int _x;
    int _y;
    int _idx;
    int _width;
    int _height;

    SDL_Surface* _pPacDeck;
    DeckType _deckType;
};

typedef CardGfx* LPCardGfx;
typedef std::vector<LPCardGfx>::iterator VI;

#endif
