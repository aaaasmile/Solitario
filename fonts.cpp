#include "fonts.h"

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


sdlFont::sdlFont()
{
	Bitmap=NULL;
}
sdlFont::~sdlFont()
{
	ClearFont();
}
void sdlFont::ClearFont()
{
	if (Bitmap!=NULL)
    {
		SDL_FreeSurface(Bitmap);
        Bitmap = NULL;
    }
}


////////////////////////////////////////
//       LoadFont
/*! 
// \param string filename : 
*/
bool sdlFont::LoadFont(string filename)
{
	ClearFont();
	Bitmap=SDL_LoadBMP(filename.c_str());
    if ( Bitmap == 0)
    {
        fprintf(stderr, "Font %s not  found\n", filename.c_str());
        exit(1);
    }
	SDL_SetColorKey(Bitmap,SDL_SRCCOLORKEY ,SDL_MapRGB(Bitmap->format,0,0,0));
		
	if (Bitmap!=NULL)
	{
		Filename=filename;
		return true;
	}
	return false;
}
void sdlFont::DrawString(SDL_Surface *surface,string message,unsigned char textcase,char alignment,int x,int y, unsigned int color)
{
unsigned char c;
int ax;
	rSource.w=SDLFONTSIZE;
	rSource.h=SDLFONTSIZE;
	rTarget.w=SDLFONTSIZE;
	rTarget.h=SDLFONTSIZE;

	switch(alignment)
	{
		case TEXTALIGNLEFT:
			ax=0;
			break;
		case TEXTALIGNRIGHT:
			ax=surface->w-((int)message.size()*SDLFONTSIZE);
			break;
		case TEXTALIGNCENTER:
			ax=(surface->w/2)-(((int)message.size()*SDLFONTSIZE)/2);
			break;
		default:
			ax=0;
			break;
	};


	for (unsigned int k=0;k<message.size();k++)
	{
		rTarget.y=y;
		rTarget.x=x+k*SDLFONTSIZE+ax;
		c=message[k];
		switch(textcase)
		{
			case TEXTUPPER:
				if ((c>='a') && (c<='z'))
					c-=32;
				break;
			case TEXTLOWER:
				if ((c>='A') && (c<='Z'))
					c+=32;
				break;
			default:
				break;
		};
		if (c<32)
			c=0;
		if (c!=0)
		{
			//c-=32;
			rSource.x=SDLFONTSIZE*(c%16);
			rSource.y=SDLFONTSIZE*(c/16);
			SDL_BlitSurface(Bitmap, &rSource, surface, &rTarget);
		}
	}
	//SDL_UpdateRect(surface,x+ax,y,SDLFONTSIZE*(int)message.size(),SDLFONTSIZE);
}
