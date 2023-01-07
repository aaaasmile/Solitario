#include "CardRegionGfx.h"

void CardRegionGfx::InitCardCoords() {
    if (InternalStack.Empty())
        return;

    int x = XCoord;
    int y = YCoord;

    int pos = 0;
    int it = 0;

    for (VI vi = InternalStack.begin(); vi != InternalStack.end(); ++vi) {
        LPCardGfx pCard = *vi;
        pCard->SetCardLoc(x, y);
        if (Attributes & CRD_3D) {
            if (!((it++) % 4))
                pos++;
            x = (pos * 2) + XCoord;
            y = pos + YCoord;
        } else {
            x += _xOffset;
            y += _yOffset;
        }
    }
}
//__________________________________________________________
/*															|
|  Imperfect, doesn't work well with diagonal, reversed,  	|
|  and 3d stacks... Needs tweaking							|
|_________________________________________________________*/

bool CardRegionGfx::PtInStack(int x, int y) {
    if (!(Attributes & CRD_VISIBLE))
        return false;

    if (Attributes & CRD_3D && !this->IsEmpty()) {
        LPCardGfx pCard = InternalStack[InternalStack.Size() - 1];
        return pCard->PtInCard(x, y);
    }

    int StackWidth = GetStackWidth();
    int StackHeight = GetStackHeight();

    return (x >= XCoord && x <= XCoord + StackWidth && y >= YCoord &&
            y <= YCoord + StackHeight);
}

bool CardRegionGfx::PtOnTop(int x, int y) {
    if (IsEmpty())
        return false;

    LPCardGfx pCard = InternalStack[InternalStack.Size() - 1];
    return pCard->PtInCard(x, y);
}

int CardRegionGfx::GetOverlapRatio(int x, int y, int w, int h) {
    if (!(Attributes & CRD_DODROP))
        return 0;

    int StackWidth = GetStackWidth();
    int StackHeight = GetStackHeight();

    int wi, hi;

    /*
     _______________________________
    |								|
    |			 _______			|
    |			|		|			|
    |			|		|			|
    |___________|_______|___________|
                |		|
                |_______|
    */
    if (PtInStack(x, y)) {
        if (XCoord + StackWidth > x + w)
            wi = w;
        else
            wi = (XCoord + StackWidth) - x;

        hi = (YCoord + StackHeight) - y;
    }
    /*
         _______________________________
        |								|
     ___|___				 			|
    |	|	|							|
    |	|	|							|
    |	|___|___________________________|
    |		|
    |_______|
    */
    else if (PtInStack(x + w, y)) {
        wi = (x + w) - XCoord;
        hi = (YCoord + StackHeight) - y;
    }
    /*
                 _______
                |		|
     ___________|_______|___________
    |			|		|			|
    |			| 		|			|
    |			|_______|			|
    |			 					|
    |_______________________________|
    */
    else if (PtInStack(x, y + h)) {
        if (XCoord + StackWidth > x + w)
            wi = w;
        else
            wi = (XCoord + StackWidth) - x;

        hi = (y + h) - YCoord;
    }
    /*
     _______
    |		|
    |	 ___|___________________________
    |	|	|							|
    |	|	|		 					|
    |___|___|       					|
        |			 					|
        |_______________________________|
    */
    else if (PtInStack(x + w, y + h)) {
        wi = (x + w) - XCoord;
        hi = (y + h) - YCoord;
    }
    /*
     _______________________________
    |								|
    |								|
    |								|
    |								|
    |_______________________________|
    */
    else
        return 0;

    return wi * hi;
}

bool CardRegionGfx::CanDrop(LPCardStackGfx stack) {
    if (InternalStack.Empty() &&
        (!(_acceptMode & CRD_DOKING) && !(_acceptMode & CRD_DOACE)))
        return false;
    // LastCard is the destination stack
    // StartCard is the dragging card that is dropping

    VI vi = stack->begin();
    LPCardGfx pStartCard = *vi;

    if (InternalStack.Empty() && (_acceptMode & CRD_DOKING) &&
        (pStartCard->Rank() != 10))
        return false;
    else if (InternalStack.Empty() && (_acceptMode & CRD_DOACE) &&
             (pStartCard->Rank() != 1))
        return false;
    else if (InternalStack.Empty())
        return true;

    VI lastvi = InternalStack.end() - 1;
    LPCardGfx pLastCard = *lastvi;

    if (pLastCard->IsFaceDown())
        return false;

    if (!(Attributes & CRD_DODROP))
        return false;

    if (_acceptMode & CRD_DOALL)
        return true;

    if ((_acceptMode & CRD_DOSINGLE) && (stack->Size() > 1)) {
        return false;
    }

    if ((_acceptMode & CRD_DOOPCOLOR) &&
        ((pStartCard->IsBlack() && pLastCard->IsBlack()) ||
         (pStartCard->IsRed() && pLastCard->IsRed()))) {
        return false;
    }

    if (((_acceptMode & CRD_DOHIGHER) && !(_acceptMode & CRD_DOLOWER)) &&
        (pStartCard->Rank() <= pLastCard->Rank()))
        return false;
    if ((!(_acceptMode & CRD_DOHIGHER) && (_acceptMode & CRD_DOLOWER)) &&
        (pStartCard->Rank() >= pLastCard->Rank()))
        return false;
    if ((!(_acceptMode & CRD_DOHIGHER) && !(_acceptMode & CRD_DOLOWER)) &&
        (pStartCard->Rank() == pLastCard->Rank()))
        return false;
    if (((_acceptMode & CRD_DOHIGHERBY1) && !(_acceptMode & CRD_DOLOWERBY1)) &&
        ((pStartCard->Rank() - pLastCard->Rank()) > 1))
        return false;
    if ((!(_acceptMode & CRD_DOHIGHERBY1) && (_acceptMode & CRD_DOLOWERBY1)) &&
        ((pStartCard->Rank() - pLastCard->Rank()) < -1))
        return false;

    if ((_acceptMode & CRD_DOSUIT) &&
        (pStartCard->Suit() != pLastCard->Suit())) {
        return false;
    }

    return true;
}

int CardRegionGfx::GetStackWidth() {
    if (InternalStack.Empty())
        return g_CardWidth;
    LPCardGfx pStartCard = InternalStack[0];
    LPCardGfx pLastCard = InternalStack[InternalStack.Size() - 1];

    return pLastCard->X() + g_CardWidth - pStartCard->X();
}

int CardRegionGfx::GetStackHeight() {
    if (InternalStack.Empty())
        return g_CardHeight;

    LPCardGfx pStartCard = InternalStack[0];
    LPCardGfx pLastCard = InternalStack[InternalStack.Size() - 1];

    return pLastCard->Y() + g_CardHeight - pStartCard->Y();
}
