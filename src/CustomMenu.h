// CustomMenu.h
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
 *	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 *USA.
 *
 *
 **/

#ifndef SDLCUSTOMMENU
#define SDLCUSTOMMENU

#ifdef _WINDOWS
#pragma warning(disable : 4786)
#endif

#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <vector>

#include "fonts.h"

using namespace std;

class sdlMenu {
 private:
  SDL_Surface *Bitmap;
  string LabelMenu;
  string LabelExit;

  sdlFont *Font;

  signed int iCurPos;

  vector<string> Items;

  SDL_Rect Rect;
  Uint32 ColorBack;
  Uint32 ColorHighlight;
  SDL_Surface *Screen;

 public:
  string FilenameBackground;

  sdlMenu();
  ~sdlMenu();
  void AddItems(string item);
  void ClearItems();
  void DrawBitmap(unsigned char alignment, Uint32 color, SDL_Surface *screen);
  SDL_Rect *GetArea();
  void LoadBackground();
  int Run();
  void SetLabels(string menu, string exit);
  void SetBitmap(string filename);
  void SetArea(int x, int y, int w, int h);
  void SetColors(Uint32 back, Uint32 high);
  void SetScreen(SDL_Surface *pVal) { Screen = pVal; }
  void SetAlpha(unsigned char alpha);
};

#endif
