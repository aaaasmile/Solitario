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


/** **********************************************************************
	SDL BITMAP FONT CLASS BY LUC RAYMOND
*************************************************************************/

/** **********************************************************************

*************************************************************************/

#ifndef SDLFONT
#define SDLFONT

#ifdef _WINDOWS
 #pragma warning( disable : 4786  ) 
#endif


#include <SDL.h>
#include <string>

using namespace std;


#define TEXTUPPER 0
#define TEXTLOWER 1
#define TEXTMIXED 2
#define TEXTALIGNLEFT	10
#define TEXTALIGNRIGHT	11
#define TEXTALIGNCENTER	12

#define SDLFONTSIZE	16



class sdlFont
{
private:
	SDL_Surface *Bitmap;
	string Filename;
	SDL_Rect rSource;
	SDL_Rect rTarget;

public:
	void ClearFont();
	bool LoadFont(string filename);
	void DrawString(SDL_Surface *surface, string message,unsigned char textcase,char alignment,int x,int y, unsigned int color);
	sdlFont();
	~sdlFont();
};

#endif
