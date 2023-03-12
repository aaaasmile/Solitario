#ifndef CARDREGION_GFX__H
#define CARDREGION_GFX__H

#include <SDL2/SDL.h>

#include "CardStackGfx.h"
#include "DeckType.h"
#include "ErrorInfo.h"

// Drag Modes
const unsigned int CRD_DRAGTOP = 3;     // Drag only top of stack
const unsigned int CRD_DRAGFACEUP = 4;  // only cards facing up

// Attributes
const unsigned int CRD_VISIBLE = (1 << 0);  // card region is drawn or not
const unsigned int CRD_FACEUP = (1 << 1);  // draw the front or back of the card
const unsigned int CRD_3D = (1 << 2);      // draw card region as a 3d stack
const unsigned int CRD_DODRAG = (1 << 3);  // card region can/cannot be dragged
const unsigned int CRD_DODROP = (1 << 4);  // card region (doesn't)accept drops
// Accept/Drop flags
const unsigned int CRD_DONOTHING = 0;
const unsigned int CRD_DOALL =
    (1 << 0);  // true:accept anything					false:check other flags
const unsigned int CRD_DOSINGLE = (1 << 1);  // true:accept only single cards
                                             // false:accept any cardstack size
const unsigned int CRD_DOOPCOLOR =
    (1 << 2);  // true:accept only opposite colored		false:check next 2 flags
const unsigned int CRD_DOHIGHER = (1 << 5);  // true:accept higher valued
                                             // false:don't accept higher valued
const unsigned int CRD_DOLOWER = (1 << 6);   // true:accept lower valued
                                             // false:don't accept lower valued
const unsigned int CRD_DOHIGHERBY1 =
    (1 << 7);  // true:accept 1 rank higher				false:don't accept
               // higher valued
const unsigned int CRD_DOLOWERBY1 =
    (1 << 8);  // true:accept 1 rank lower				false:don't accept lower
               // valued
const unsigned int CRD_DOSUIT =
    (1 << 10);  // true:accept only cards of same suit	false:accept any suit
// On empty stacks, used in most solitaire games
const unsigned int CRD_DOACE =
    (1
     << 11);  // true:accept only aces					false:don't accept ace
const unsigned int CRD_DOKING =
    (1 << 12);  // true:accept only kings				false:don't accept king

class CardRegionGfx {
public:
    CardRegionGfx(int id, unsigned int attribs, unsigned int amode, int dmode,
                  int symbol, int x, int y, int xoff, int yoff,
                  DeckType &deckType, int width, int height)
        : _id(id),
          _attributes(attribs),
          _acceptMode(amode),
          _dragMode(dmode),
          _symbol(symbol),
          _xCoord(x),
          _yCoord(y),
          _xOffset(xoff),
          _yOffset(yoff),
          _deckType(deckType),
          _widthEmpty(width),
          _heightEmpty(height) {}

    ~CardRegionGfx() {}

    LPCardStackGfx GetCardStack() { return &_internalStack; }

    int GetxOffset() { return _xOffset; }
    int GetyOffset() { return _yOffset; }

    void InitCardFaces() {
        _internalStack.SetCardsFaceUp(_attributes & CRD_FACEUP);
    }

    void SetCardFaceUp(bool bVal, int idx) {
        _internalStack.Item(idx)->SetFaceUp(bVal);
    }
    bool IsCardFaceUp(int idx) { return _internalStack.Item(idx)->IsFaceUp(); }

    int GetDragMode() const { return _dragMode; }

    void SetSymbol(int symbol) { _symbol = symbol; }

    void SetAcceptMode(unsigned int mode) { _acceptMode = mode; }
    void SetAttributes(unsigned int attr) { _attributes = attr; }
    int GetAttributes() { return _attributes; }

    bool CanDrop(LPCardStackGfx stack);

    void InitCardCoords();
    int GetOverlapRatio(int x, int y, int width, int height);

    bool PtInStack(int x, int y);
    bool PtOnTop(int x, int y);

    int GetStackWidth();
    int GetStackHeight();

    LPErrInApp NewDeck() {
        return _internalStack.NewDeck(_deckType, _widthEmpty, _heightEmpty);
    }
    void Shuffle() { _internalStack.Shuffle(); }
    void Clear() { _internalStack.Clear(); }
    void CleanUp() { _internalStack.CleanUp(); }
    void Reverse() { _internalStack.Reverse(); }

    void PushCard(LPCardGfx pCard) { _internalStack.PushCard(pCard); }
    void PushStack(LPCardStackGfx pStack) { _internalStack.PushStack(pStack); }

    bool IsEmpty() { return _internalStack.IsEmpty(); }
    int Size() { return _internalStack.Size(); }
    LPCardGfx Item(int ix) { return _internalStack.Item(ix); }
    LPCardGfx PopCard() { return _internalStack.PopCard(); }
    LPCardStackGfx PopStack(int items) {
        return _internalStack.PopStack(items);
    }

    LPCardGfx RemoveCard(int index) { return _internalStack.RemoveCard(index); }

    int GetClickedCard(int x, int y) {
        return _internalStack.GetCardWherePointIsInside(x, y);
    }
    LPCardGfx GetCard(int index) { return _internalStack.GetCard(index); }
    bool IsVisible() { return _attributes & CRD_VISIBLE; }
    int X() { return _xCoord; }
    int Y() { return _yCoord; }
    int Symbol() { return _symbol; }
    int Id() { return _id; }
    int GetSavedSize() { return _savedSize; }
    void SaveSize() { _savedSize = _internalStack.Size(); }

private:
    int _id;
    unsigned int _attributes;
    CardStackGfx _internalStack;
    int _xCoord;
    int _yCoord;
    int _symbol;
    int _xOffset;
    int _yOffset;
    int _widthEmpty;
    int _heightEmpty;
    int _dragMode;
    unsigned int _acceptMode;
    DeckType _deckType;
    int _savedSize;
};

typedef CardRegionGfx *LPCardRegionGfx;

#endif
