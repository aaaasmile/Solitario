#include "CardRegionGfx.h"

void CardRegionGfx::InitCardCoords() {
    if (InternalStack.Empty())
        return;

    int x = XCoord;
    int y = YCoord;

    int pos = 0;
    int it = 0;

    if (Attributes & CRD_3D) {
        for (VI vi = InternalStack.begin(); vi != InternalStack.end(); ++vi) {
            vi->SetCardLoc(x, y);
            if (!((it++) % 4))
                pos++;
            x = (pos * 2) + XCoord;
            y = pos + YCoord;
        }
    }

    else {
        for (VI vi = InternalStack.begin(); vi != InternalStack.end(); ++vi) {
            vi->SetCardLoc(x, y);
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

    if (Attributes & CRD_3D && !this->Empty())
        return InternalStack[InternalStack.Size() - 1].PtInCard(x, y);

    int StackWidth = this->GetStackWidth();
    int StackHeight = this->GetStackHeight();

    if (x >= XCoord && x <= XCoord + StackWidth && y >= YCoord &&
        y <= YCoord + StackHeight)
        return true;
    else
        return false;
}

bool CardRegionGfx::PtOnTop(int x, int y) {
    if (this->Empty())
        return false;

    return InternalStack[InternalStack.Size() - 1].PtInCard(x, y);
}

int CardRegionGfx::GetOverlapRatio(int x, int y, int w, int h) {
    if (!(Attributes & CRD_DODROP))
        return 0;

    int StackWidth = this->GetStackWidth();
    int StackHeight = this->GetStackHeight();

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

bool CardRegionGfx::CanDrop(CardStackGfx *stack) {
    if (InternalStack.Empty() &&
        (!(_acceptMode & CRD_DOKING) && !(_acceptMode & CRD_DOACE)))
        return false;

    VI vi = stack->begin();

    // if empty
    if (InternalStack.Empty() && (_acceptMode & CRD_DOKING) &&
        (vi->Rank() != 10))
        return false;
    else if (InternalStack.Empty() && (_acceptMode & CRD_DOACE) &&
             (vi->Rank() != 1))
        return false;
    else if (InternalStack.Empty())
        return true;

    VI myvi = InternalStack.end() - 1;

    if (myvi->FaceDown())
        return false;

    //	printf("vi->Suit() = %i, vi->Rank() = %i, myvi->Suit() = %i,
    // myvi->Rank() = %i\n",vi->Suit(),vi->Rank(),myvi->Suit(),myvi->Rank());

    if (!(Attributes & CRD_DODROP))
        return false;

    if (_acceptMode & CRD_DOALL)
        return true;

    if ((_acceptMode & CRD_DOSINGLE) && (stack->Size() > 1)) {
        //		printf("fSingle returned false\n");
        return false;
    }

    if ((_acceptMode & CRD_DOOPCOLOR) && ((myvi->IsBlack() && vi->IsBlack()) ||
                                          (myvi->IsRed() && vi->IsRed()))) {
        //		printf("fOpColor returned false\n");
        return false;
    }

    if (((_acceptMode & CRD_DORED) && !(_acceptMode & CRD_DOBLACK)) &&
        vi->IsBlack()) {
        //		printf("fRed && !fBlack returned false\n");
        return false;
    }

    if ((!(_acceptMode & CRD_DORED) && (_acceptMode & CRD_DOBLACK)) &&
        vi->IsRed()) {
        //		printf("!fRed && fBlack returned false\n");
        return false;
    }

    if (!(_acceptMode & CRD_DOHIGHER) && !(_acceptMode & CRD_DOLOWER))
        _acceptMode |= CRD_DOEQUAL;

    if (_acceptMode & CRD_DOEQUAL) {
        //		printf("in fEqual\n");
        if (((_acceptMode & CRD_DOHIGHER) && !(_acceptMode & CRD_DOLOWER)) &&
            (vi->Rank() < myvi->Rank()))
            return false;
        if ((!(_acceptMode & CRD_DOHIGHER) && (_acceptMode & CRD_DOLOWER)) &&
            (vi->Rank() > myvi->Rank()))
            return false;
        if ((!(_acceptMode & CRD_DOHIGHER) && !(_acceptMode & CRD_DOLOWER)) &&
            (vi->Rank() != myvi->Rank()))
            return false;
        // by 1
        if (((_acceptMode & CRD_DOHIGHERBY1) &&
             !(_acceptMode & CRD_DOLOWERBY1)) &&
            ((vi->Rank() - myvi->Rank()) > 1))
            return false;
        if ((!(_acceptMode & CRD_DOHIGHERBY1) &&
             (_acceptMode & CRD_DOLOWERBY1)) &&
            ((vi->Rank() - myvi->Rank()) < -1))
            return false;
    }
    if (!(_acceptMode & CRD_DOEQUAL)) {
        //		printf("in !fEqual\n");
        if (((_acceptMode & CRD_DOHIGHER) && !(_acceptMode & CRD_DOLOWER)) &&
            (vi->Rank() <= myvi->Rank()))
            return false;
        if ((!(_acceptMode & CRD_DOHIGHER) && (_acceptMode & CRD_DOLOWER)) &&
            (vi->Rank() >= myvi->Rank()))
            return false;
        if ((!(_acceptMode & CRD_DOHIGHER) && !(_acceptMode & CRD_DOLOWER)) &&
            (vi->Rank() == myvi->Rank()))
            return false;
        // by 1
        if (((_acceptMode & CRD_DOHIGHERBY1) &&
             !(_acceptMode & CRD_DOLOWERBY1)) &&
            ((vi->Rank() - myvi->Rank()) > 1))
            return false;
        if ((!(_acceptMode & CRD_DOHIGHERBY1) &&
             (_acceptMode & CRD_DOLOWERBY1)) &&
            ((vi->Rank() - myvi->Rank()) < -1))
            return false;
    }

    if ((_acceptMode & CRD_DOSUIT) && (vi->Suit() != myvi->Suit())) {
        //		printf("fSuit returned false\n");
        return false;
    }

    return true;
}

int CardRegionGfx::GetStackWidth() {
    if (InternalStack.Empty())
        return g_CARDWIDTH;

    return (InternalStack[InternalStack.Size() - 1].X() + g_CARDWIDTH) -
           InternalStack[0].X();
}

int CardRegionGfx::GetStackHeight() {
    if (InternalStack.Empty())
        return g_CARDHEIGHT;

    return (InternalStack[InternalStack.Size() - 1].Y() + g_CARDHEIGHT) -
           InternalStack[0].Y();
}
