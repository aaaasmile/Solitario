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


// cHightScoreMgr.h: interface for the cHightScoreMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHIGHTSCOREMGR_H__6D4D1807_CD5D_4D2F_AB63_A5A94EE9CD50__INCLUDED_)
#define AFX_CHIGHTSCOREMGR_H__6D4D1807_CD5D_4D2F_AB63_A5A94EE9CD50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class cHightScoreMgr  
{
public:
	cHightScoreMgr();
	virtual ~cHightScoreMgr();

     //! load hightscore
    void    Load();
    //! save hight score
    void    Save();

public:
    std::string       HS_Names[10];
    unsigned int HS_Scores[10];


};

#endif // !defined(AFX_CHIGHTSCOREMGR_H__6D4D1807_CD5D_4D2F_AB63_A5A94EE9CD50__INCLUDED_)
