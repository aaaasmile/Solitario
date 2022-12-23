#ifndef CARDGFX__H
#define CARDGFX__H

#include <assert.h>

#include <vector>

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
        Idx = NOT_VALID_INDEX;
        _fFaceUp = true;
        _iValue = 0;
        _eSuit = UNDEF;
    }
    CardGfx(int index) {
        if (index < 0)
            index = 0;

        Idx = index;
        _fFaceUp = true;
        _iValue = 0;
        _eSuit = UNDEF;
    }

    eSUIT Suit() const { return _eSuit; }
    int Rank() const { return _iValue; }

    bool IsBlack() const { return _eSuit == BASTONI || _eSuit == SPADE; }
    bool IsRed() const { return !IsBlack(); }

    bool FaceUp() const { return _fFaceUp; }
    bool FaceDown() const { return !_fFaceUp; }

    void SetFaceUp(bool fTrue) { _fFaceUp = fTrue; }
    void SetIdx(int nIdx) {
        assert(nIdx < NUM_CARDS && nIdx >= 0);
        Idx = nIdx;
        _iValue = g_PointsSolitario[nIdx];
        if (Idx >= 0 && Idx <= 9)
            _eSuit = BASTONI;
        else if (Idx > 9 && Idx <= 19)
            _eSuit = COPPE;
        else if (Idx > 19 && Idx <= 29)
            _eSuit = DENARI;
        else if (Idx > 29 && Idx <= 39)
            _eSuit = SPADE;
        else
            _eSuit = UNDEF;
    }

    void SetCardLoc(int lx, int ly) {
        x = lx;
        y = ly;
    }

    bool PtInCard(int lx, int ly) {
        if (lx >= x && lx <= g_CARDWIDTH + x && ly >= y &&
            ly <= g_CARDHEIGHT + y)
            return true;
        else
            return false;
    }

    int x;
    int y;
    int Idx;

private:
    bool _fFaceUp;
    eSUIT _eSuit;
    int _iValue;
};

typedef std::vector<CardGfx>::iterator VI;

#endif
