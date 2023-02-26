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
                  DeckType &deckType)
        : Id(id),
          Attributes(attribs),
          _acceptMode(amode),
          _dragMode(dmode),
          Symbol(symbol),
          XCoord(x),
          YCoord(y),
          _xOffset(xoff),
          _yOffset(yoff),
          _deckType(deckType) {}

    ~CardRegionGfx() {}

    LPCardStackGfx GetCardStack() { return &InternalStack; }

    int GetxOffset() { return _xOffset; }
    int GetyOffset() { return _yOffset; }

    void InitCardFaces() {
        InternalStack.SetCardsFaceUp(Attributes & CRD_FACEUP);
    }

    void SetCardFaceUp(bool bVal, int idx) {
        InternalStack.Item(idx)->SetFaceUp(bVal);
    }
    bool IsCardFaceUp(int idx) { return InternalStack.Item(idx)->IsFaceUp(); }

    int GetDragMode() const { return _dragMode; }

    void SetSymbol(int symbol) { Symbol = symbol; }

    void SetAcceptMode(unsigned int mode) { _acceptMode = mode; }
    void SetAttributes(unsigned int attr) { Attributes = attr; }
    int GetAttributes() { return Attributes; }

    bool CanDrop(LPCardStackGfx stack);

    void InitCardCoords();
    int GetOverlapRatio(int x, int y, int width, int height);

    bool PtInStack(int x, int y);
    bool PtOnTop(int x, int y);

    int GetStackWidth();
    int GetStackHeight();

    LPErrInApp NewDeck() { return InternalStack.NewDeck(_deckType); }
    void Shuffle() { InternalStack.Shuffle(); }
    void Clear() { InternalStack.Clear(); }
    void CleanUp() { InternalStack.CleanUp(); }
    void Reverse() { InternalStack.Reverse(); }

    void PushCard(LPCardGfx pCard) { InternalStack.PushCard(pCard); }
    void PushStack(LPCardStackGfx pStack) { InternalStack.PushStack(pStack); }

    bool IsEmpty() { return InternalStack.IsEmpty(); }
    int Size() { return InternalStack.Size(); }

    LPCardGfx PopCard() { return InternalStack.PopCard(); }
    LPCardStackGfx PopStack(int items) { return InternalStack.PopStack(items); }

    LPCardGfx RemoveCard(int index) { return InternalStack.RemoveCard(index); }

    int GetClickedCard(int x, int y) {
        return InternalStack.GetCardWherePointIsInside(x, y);
    }
    LPCardGfx GetCard(int index) { return InternalStack.GetCard(index); }

    int Id;
    unsigned int Attributes;
    CardStackGfx InternalStack;
    int XCoord;
    int YCoord;
    int Symbol;

private:
    int _xOffset;
    int _yOffset;

    int _dragMode;

    unsigned int _acceptMode;
    DeckType _deckType;
};

typedef CardRegionGfx *LPCardRegionGfx;

#endif
