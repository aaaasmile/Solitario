/*
*
*	This program is free software; you can redistribute it and/or
*	modify it under the terms of the GNU General Public License
*	as published by the Free Software Foundation; either version 2
*	of the License, or (at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program; if not, write to the Free Software
*	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*
**/


/*
	filename:		CCardStack.h
	author:			Rico Roberto Zuñiga
	last modified:	9/12/03

	description:	Interface for card stack data structure and subs.
*/

#ifndef CCARDSTACK_H
#define CCARDSTACK_H

#include "CCard.h"

#include <vector>
#include <algorithm>

#include <time.h>
#include <stdlib.h>



class CCardStack : public std::vector<CCard>
{

public:

    void		NewDeck();
    void		Shuffle();
    void		Clear();
    void		Sort();
    void		Reverse();
    bool		Empty() { return this->empty(); }
    int			Size() { return this->size(); }

    void		Push(const CCard card);
    void		Push(CCardStack cardstack);

    CCard		Pop();
    CCardStack	Pop(int items);

    CCard		RemoveCard(int index);
    void		InsertCard(int index, CCard card);
    
    int			GetClickedCard(int x, int y);
	CCard		GetCard(int index);

	void		SetCardsFaceUp(bool fTrue);
	
	
};

#endif //CCARDSTACK_H
