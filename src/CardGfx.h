#ifndef CARDGFX__H
#define CARDGFX__H

#include <vector>

#include "ErrorInfo.h"

// if will be using custom cards then these should be initialized outside
extern unsigned int g_CARDWIDTH;
extern unsigned int g_CARDHEIGHT;
extern unsigned int g_SYMBOLWIDTH;
extern unsigned int g_SYMBOLHEIGHT;

enum eSUIT { BASTONI = 0, COPPE = 1, DENARI = 2, SPADE = 3, UNDEF = 4 };

enum { PLAYER_ME = 0, NUM_CARDS = 40, NOT_VALID_INDEX = -1 };

extern int g_PointsSolitario[];

class CardGfx {
public:
    CardGfx() {
        _Idx = NOT_VALID_INDEX;
        _fFaceUp = true;
        _iValue = 0;
        _eSuit = UNDEF;
    }
    CardGfx(int index) {
        if (index < 0)
            index = 0;

        _Idx = index;
        _fFaceUp = true;
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
    int Index() { return _Idx; }

    bool FaceUp() const { return _fFaceUp; }
    bool FaceDown() const { return !_fFaceUp; }

    void SetFaceUp(bool fTrue) { _fFaceUp = fTrue; }
    LPErrInApp SetIdx(int nIdx) {
        if (nIdx >= NUM_CARDS || nIdx < 0) {
            return ERR_UTIL::ErrorCreate("Error SetIdx %d is out of range",
                                         nIdx);
        }

        _Idx = nIdx;
        _iValue = g_PointsSolitario[nIdx];
        if (_Idx >= 0 && _Idx <= 9)
            _eSuit = BASTONI;
        else if (_Idx > 9 && _Idx <= 19)
            _eSuit = COPPE;
        else if (_Idx > 19 && _Idx <= 29)
            _eSuit = DENARI;
        else if (_Idx > 29 && _Idx <= 39)
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
        if (lx >= _x && lx <= g_CARDWIDTH + _x && ly >= _y &&
            ly <= g_CARDHEIGHT + _y)
            return true;
        else
            return false;
    }

private:
    bool _fFaceUp;
    eSUIT _eSuit;
    int _iValue;

    int _x;
    int _y;
    int _Idx;
};

typedef std::vector<CardGfx>::iterator VI;

#endif
