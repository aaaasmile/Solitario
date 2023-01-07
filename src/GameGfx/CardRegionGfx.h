#ifndef CARDREGION_GFX__H
#define CARDREGION_GFX__H

#include <SDL2/SDL.h>

#include "CardStackGfx.h"
#include "ErrorInfo.h"

// Drag Modes
const unsigned int CRD_DRAGTOP = 3;  // Drag only top of stack
const unsigned int CRD_DRAGFACEUP =
    4;  // Variation of 1, but only cards facing up

// Attributes
const unsigned int CRD_VISIBLE = (1 << 0);  // card region is drawn or not
const unsigned int CRD_FACEUP = (1 << 1);  // draw the front or back of the card
const unsigned int CRD_3D = (1 << 2);      // draw card region as a 3d stack
const unsigned int CRD_DODRAG = (1 << 3);  // card region can/cannot be dragged
const unsigned int CRD_DODROP = (1 << 4);  // card region (doesn't)accept drops
// Accept/Drop flags
const unsigned int CRD_DONOTHING = 0;
const unsigned int CRD_DOALL =
    (1 << 0);  // TRUE:accept anything					FALSE:check other flags
const unsigned int CRD_DOSINGLE = (1 << 1);  // TRUE:accept only single cards
                                             // FALSE:accept any cardstack size
const unsigned int CRD_DOOPCOLOR =
    (1 << 2);  // TRUE:accept only opposite colored		FALSE:check next 2 flags
const unsigned int CRD_DOHIGHER = (1 << 5);  // TRUE:accept higher valued
                                             // FALSE:don't accept higher valued
const unsigned int CRD_DOLOWER = (1 << 6);   // TRUE:accept lower valued
                                             // FALSE:don't accept lower valued
const unsigned int CRD_DOHIGHERBY1 =
    (1 << 7);  // TRUE:accept 1 rank higher				FALSE:don't accept
               // higher valued
const unsigned int CRD_DOLOWERBY1 =
    (1 << 8);  // TRUE:accept 1 rank lower				FALSE:don't accept lower
               // valued
const unsigned int CRD_DOSUIT =
    (1 << 10);  // TRUE:accept only cards of same suit	FALSE:accept any suit
// On empty stacks, used in most solitaire games
const unsigned int CRD_DOACE =
    (1
     << 11);  // TRUE:accept only aces					FALSE:don't accept ace
const unsigned int CRD_DOKING =
    (1 << 12);  // TRUE:accept only kings				FALSE:don't accept king

class CardRegionGfx {
public:
    CardRegionGfx(int id, unsigned int attribs, unsigned int amode, int dmode,
                  int symbol, int x, int y, int xoff, int yoff)
        : Id(id),
          Attributes(attribs),
          _acceptMode(amode),
          _dragMode(dmode),
          Symbol(symbol),
          XCoord(x),
          YCoord(y),
          _xOffset(xoff),
          _yOffset(yoff) {}

    CardRegionGfx() {}
    ~CardRegionGfx() {}

    LPCardStackGfx GetCardStack() { return &InternalStack; }

    int GetxOffset() { return _xOffset; }
    int GetyOffset() { return _yOffset; }

    void InitCardFaces() {
        InternalStack.SetCardsFaceUp(Attributes & CRD_FACEUP);
    }

    void SetCardFaceUp(bool bVal, int idx) {
        InternalStack[idx]->SetFaceUp(bVal);
    }
    bool IsCardFaceUp(int idx) { return InternalStack[idx]->IsFaceUp(); }

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

    LPErrInApp NewDeck() { return InternalStack.NewDeck(); }
    void Shuffle() { InternalStack.Shuffle(); }
    void Clear() { InternalStack.Clear(); }
    void Reverse() { InternalStack.Reverse(); }

    void PushCard(LPCardGfx pCard) { InternalStack.PushCard(pCard); }
    void PushStack(LPCardStackGfx pStack) { InternalStack.PushStack(pStack); }

    bool IsEmpty() { return InternalStack.Empty(); }
    int Size() { return InternalStack.Size(); }

    LPCardGfx PopCard() { return InternalStack.PopCard(); }
    LPCardStackGfx PopStack(int items) { return InternalStack.PopStack(items); }

    LPCardGfx RemoveCard(int index) { return InternalStack.RemoveCard(index); }

    int GetClickedCard(int x, int y) {
        return InternalStack.GetClickedCard(x, y);
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
};

typedef CardRegionGfx *LPCardRegionGfx;

#endif
