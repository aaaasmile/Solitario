//CustomMenu.cpp
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


#include "CustomMenu.h"
#include "gfx_util.h"

static const char * lpszFontFile = DATA_PREFIX"images/font.bmp";


sdlMenu::sdlMenu()
{
	LabelMenu="Menu";
	LabelExit="Exit";
	Bitmap=NULL;
	Font=new sdlFont;
	Font->LoadFont(lpszFontFile);
    Screen = NULL;
}
sdlMenu::~sdlMenu()
{
	if (Bitmap!=NULL)
		SDL_FreeSurface(Bitmap);
	delete Font;
}

void sdlMenu::AddItems(string item)
{
	Items.push_back(item);
}

void sdlMenu::ClearItems()
{
	Items.clear();
}


////////////////////////////////////////
//       Run
/*! Run menu
*/
int sdlMenu::Run()
{
    bool bEnd=false;
    bool bRedraw=true;
    SDL_Rect trect;
    SDL_Event event;
    string name,temp;
    Uint32 tcolor;
    int counter=0;
	Items.push_back(LabelExit);
	iCurPos=0;
	trect.x=Rect.x;
	trect.w=Rect.w;
	trect.h=SDLFONTSIZE;
	LoadBackground();
    while(bEnd==false)
    {  
		if (bRedraw==true)
		{
			if (FilenameBackground.size()>0)
            {
				DrawBitmap(3,SDL_MapRGB(Screen->format, 0, 0, 0));
            }

			SDL_FillRect(Screen, &Rect ,ColorBack);
			Font->DrawString(Screen,LabelMenu,TEXTMIXED,TEXTALIGNCENTER,0,Rect.y+5,0);			

			for (unsigned int k=0;k< Items.size();k++)
			{
				trect.y=Rect.y+(25+SDLFONTSIZE)+((SDLFONTSIZE+5)*k);

				if (k==iCurPos)
                {
					tcolor=ColorHighlight;
                }
				else
                {
					tcolor=ColorBack;
                }

				SDL_FillRect(Screen,&trect,tcolor);
				Font->DrawString(Screen, Items[k]  ,TEXTMIXED,TEXTALIGNCENTER,0,Rect.y+(25+SDLFONTSIZE)+((SDLFONTSIZE+5)*k),0);			

			}
			SDL_Flip(Screen);
			bRedraw=false;
            
		}
		if (SDL_WaitEvent(&event)==1)
		{
			switch(event.type)
			{  
	            case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							iCurPos=-1;
							bEnd=true;
							break;
						case SDLK_RETURN:
							if (iCurPos==Items.size()-1)
								iCurPos=-1;
							bEnd=true;
							break;
						case SDLK_UP:
							if (iCurPos==0)
								iCurPos=(signed int)Items.size()-1;
							else
								iCurPos--;
							bRedraw=true;
							break;
						case SDLK_DOWN:
							if (iCurPos==Items.size()-1)
								iCurPos=0;
							else
								iCurPos++;
							bRedraw=true;
							break;
					};
					break;
                case SDL_QUIT:
                    iCurPos=-1;
					bEnd=true;
					break;
			}
        }
		SDL_Delay(50);
		counter++;
		if (counter%10==0)
			bRedraw=true;
    }
	Items.pop_back();
	return iCurPos;
	
}

void sdlMenu::SetLabels(string menu,string exit)
{
	LabelMenu=menu;
	LabelExit=exit;
}

void sdlMenu::SetBitmap(string filename)
{
	FilenameBackground=filename;
}
void sdlMenu::SetArea(int x, int y, int w, int h)
{
	Rect.x=x;
	Rect.y=y;
	Rect.w=w;
	Rect.h=h;
}
void sdlMenu::SetColors(Uint32 back, Uint32 high)
{
	ColorBack=back;
	ColorHighlight=high;
}

SDL_Rect * sdlMenu::GetArea()
{
	return &Rect;
}

void sdlMenu::LoadBackground()
{
	if (Bitmap!=NULL)
		SDL_FreeSurface(Bitmap);

	Bitmap=IMG_Load(FilenameBackground.c_str());
}

/** *****************************************************************************************************************************
	DRAW BITMAP AT POSITION X,Y
***************************************************************************************************************************** */
void sdlMenu::DrawBitmap(unsigned char alignment,  Uint32 color )
{
SDL_Rect s,t;
	
SDL_Surface *screen=SDL_GetVideoSurface();

	if ((Bitmap==NULL) || (screen==NULL))
			return;

	s.w =Bitmap->w;
	t.w=Bitmap->w;
	s.h=Bitmap->h;
	t.h=Bitmap->h;
	s.x=0;
	s.y=0;
	switch(alignment)
	{
		case 0:	//LEFT
			t.x=0;
			t.y=0;
			break;
		case 1:	//CENTER
			t.x=(screen->w-Bitmap->w)/2;
			t.y=(screen->h-Bitmap->h)/2;
			break;
		case 2:	//RIGHT
			t.x=(screen->w-Bitmap->w);
			t.y=(screen->h-Bitmap->h);
			break;
		case 3:	//HORIZONTAL CENTER ONLY
			t.x=(screen->w-Bitmap->w)/2;
			t.y=0;
			break;
		case 4:	//VERTICAL CENTER ONLY
			t.x=0;
			t.y=(screen->h-Bitmap->h)/2;
			break;
	}
	if (Bitmap->format->palette && screen->format->palette) 
	    SDL_SetColors(screen, Bitmap->format->palette->colors, 0,Bitmap->format->palette->ncolors);

	SDL_FillRect(screen,&screen->clip_rect,color);

//    SDL_BlitSurface(Bitmap, &s, screen, &t);
	while ( SDL_BlitSurface(Bitmap, &s, screen, &t) == -2 ) 
			SDL_Delay(50);	


}


////////////////////////////////////////
//       SetAlpha
/*! Set alpha blending level
// \param unsigned char alpha : 
*/
void sdlMenu::SetAlpha(unsigned char alpha)
{
    SDL_SetAlpha(Screen, SDL_SRCALPHA, alpha);
}
