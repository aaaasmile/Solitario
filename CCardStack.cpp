/*
	filename:		CCardStack.cpp
	author:			Rico Roberto Zuñiga
	last modified:	9/12/03

	description:	Card stack data structure subs.
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

#include "CCardStack.h"

int g_PointsBriscola[] = {
          /*Asso*/11, /*Due*/0, /*Tre*/10, /*Quattro*/0, /*cinque*/0, /*Sei*/0, /*Sette*/0, /*Fante*/2, /*Cavallo*/3, /*Re*/4,
          /*Asso*/11, /*Due*/0, /*Tre*/10, /*Quattro*/0, /*cinque*/0, /*Sei*/0, /*Sette*/0, /*Fante*/2, /*Cavallo*/3, /*Re*/4,
          /*Asso*/11, /*Due*/0, /*Tre*/10, /*Quattro*/0, /*cinque*/0, /*Sei*/0, /*Sette*/0, /*Fante*/2, /*Cavallo*/3, /*Re*/4,
          /*Asso*/11, /*Due*/0, /*Tre*/10, /*Quattro*/0, /*cinque*/0, /*Sei*/0, /*Sette*/0, /*Fante*/2, /*Cavallo*/3, /*Re*/4 }; 

int g_PointsSolitario[] = {
          /*Asso*/1, /*Due*/2, /*Tre*/3, /*Quattro*/4, /*cinque*/5, /*Sei*/6, /*Sette*/7, /*Fante*/8, /*Cavallo*/9, /*Re*/10,
          /*Asso*/1, /*Due*/2, /*Tre*/3, /*Quattro*/4, /*cinque*/5, /*Sei*/6, /*Sette*/7, /*Fante*/8, /*Cavallo*/9, /*Re*/10,
          /*Asso*/1, /*Due*/2, /*Tre*/3, /*Quattro*/4, /*cinque*/5, /*Sei*/6, /*Sette*/7, /*Fante*/8, /*Cavallo*/9, /*Re*/10,
          /*Asso*/1, /*Due*/2, /*Tre*/3, /*Quattro*/4, /*cinque*/5, /*Sei*/6, /*Sette*/7, /*Fante*/8, /*Cavallo*/9, /*Re*/10 }; 




////////////////////////////////////////
//       NewDeck
/*! Nuovo mazzo
*/
void CCardStack::NewDeck()
{

	this->clear();
	this->resize(NUM_CARDS);

	int Id = 0;
    for(VI vi = this->begin(); vi != this->end(); ++vi)
	{
        vi->SetIdx(Id);
		Id++;
	}
}


////////////////////////////////////////
//       Shuffle
/*! Mescola il mazzo
*/
void CCardStack::Shuffle()
{
	std::random_shuffle(this->begin(), this->end());
}

void CCardStack::Clear()
{
	this->clear();
}

void CCardStack::Reverse()
{
	std::reverse(this->begin(), this->end());
}
// predicate for sorting
bool SortRank(const CCard& l, const CCard& r)
{
	return l.Rank() < r.Rank();
}

void CCardStack::Sort()
{
	std::sort(this->begin(), this->end(), SortRank);
}

void CCardStack::Push(const CCard card)
{
	this->push_back(card);
}

void CCardStack::Push(CCardStack cardstack)
{
	for(VI vi = cardstack.begin(); vi != cardstack.end(); ++vi)
		this->push_back(*vi);
}

CCard CCardStack::Pop()
{
	if(Empty())
		return 0;

	CCard card = this->back();
	this->pop_back();

	return card;
}

CCardStack CCardStack::Pop(int items)
{
	if(items > Size())
		items = Size();

	CCardStack cardstack;

	while(items > 0)
	{
		cardstack.Push(this->Pop());
		items--;
	}
	cardstack.Reverse();
	return cardstack;
}

CCard CCardStack::RemoveCard(int index)
{
	if(this->Empty())
		return 0;

	VI vi = this->begin() + index;
	CCard card = *vi;
	this->erase(vi);

	return card;
}

void CCardStack::InsertCard(int index, CCard card)
{
	VI vi = this->begin() + index;
 	this->insert(vi, card);
}

int CCardStack::GetClickedCard(int x, int y)
{
	int i = this->Size() - 1;

	for(VI vi = this->end() - 1; vi >= this->begin(); --vi)
    {
    	if( vi->PtInCard(x, y) )
    		return i;
		i--;
	}

	return -1;
}

CCard CCardStack::GetCard(int index)
{
	if(this->Empty())
		return 0;

	VI vi = this->begin() + index;
	CCard card = *vi;

	return card;
}

void CCardStack::SetCardsFaceUp(bool fTrue)
{
	for(VI vi = this->begin(); vi != this->end(); ++vi)
		vi->SetFaceUp(fTrue);
}
