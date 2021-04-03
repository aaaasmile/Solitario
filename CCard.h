/*
	filename:		CCard.h
	author:			Rico Roberto Zuñiga
	last modified:	9/12/03

	description:	Interface for the CCard class. Handles the card data structure.
 					Some functions based on CardLib.
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


#ifndef CCARD_H
#define CCARD_H

#include <assert.h>
#include <vector>

// if will be using custom cards then these should be initialized outside
extern unsigned int g_CARDWIDTH	;
extern unsigned int g_CARDHEIGHT	;
extern unsigned int g_SYMBOLWIDTH	;
extern unsigned int g_SYMBOLHEIGHT	;

//! type of cards
enum eSUIT
{
    BASTONI = 0,
    COPPE = 1,
    DENARI = 2,
    SPADE = 3,
    UNDEF = 4
};

//! useful constants
enum
{
    //! index of me
    PLAYER_ME = 0,
    //! index of opponent
    PLAYER_OPPONENT = 1,
    //! number of cards
    NUM_CARDS = 40,
    NUM_CARDS_HAND = 3,
    SCORE_GOAL = 60,
    NOT_VALID_INDEX = -1,
    TOTAL_POINTS = 120,
    NUM_SEEDS = 5,
    BUFF_NAME = 64,
    //!card on deck
    NUM_DECK_CARD = 33,
    //! segni on match
    SEGNI_ON_MATCH = 4
};

extern int g_PointsSolitario[];

//! class CCard 
class CCard
{
public:

    CCard()
    {
        //undefined index as default
        Idx = NOT_VALID_INDEX; 
        fFaceUp = true;
        m_iValue = 0;
        m_eSuit = UNDEF;
    }
    CCard(int index) 
    {
        if(index < 0)  
            index = 0;

        Idx = index;
        fFaceUp = true;
        m_iValue = 0;
        m_eSuit = UNDEF;
    }

    eSUIT Suit() const			{ return m_eSuit; }
    int Rank() const			{ return m_iValue; }

    bool IsBlack() const 		{ return m_eSuit == BASTONI || m_eSuit == SPADE; }
    bool IsRed() const			{ return !IsBlack(); }

    bool FaceUp() const			{ return fFaceUp; }
    bool FaceDown() const		{ return !fFaceUp; }
    
   
    void SetFaceUp(bool fTrue)	{ fFaceUp = fTrue; }
    void SetIdx(int nIdx)		{   assert(nIdx < NUM_CARDS && nIdx >=0);Idx = nIdx; 
                                    m_iValue = g_PointsSolitario[nIdx]; 
                                    if (Idx >= 0 && Idx <= 9 )
                                      m_eSuit = BASTONI;
                                    else if (Idx > 9 && Idx <= 19 )
                                      m_eSuit = COPPE;
                                    else if (Idx > 19 && Idx <= 29 )
                                      m_eSuit = DENARI;
                                    else if (Idx > 29 && Idx <= 39 )
                                      m_eSuit = SPADE;
                                    else
                                      m_eSuit = UNDEF;}

    void SetCardLoc(int lx, int ly)	{ x = lx; y = ly; }

	bool PtInCard(int lx, int ly)
	{
		if(lx >= x && lx <= g_CARDWIDTH + x && ly >= y && ly <= g_CARDHEIGHT + y)
			return true;
		else
			return false;
	}


	int x;
	int y;
    int Idx;

private:

    bool	fFaceUp;
	//! suit of the card
	eSUIT	m_eSuit;
    int     m_iValue;
};


typedef std::vector<CCard>::iterator VI;

#endif //CCARD_H
