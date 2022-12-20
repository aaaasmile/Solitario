// gfx_util.h
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

#ifndef __GFX_UTIL__H
#define __GFX_UTIL__H

#include <SDL.h>

#include <string>

#ifdef WIN32
#define DATA_PREFIX "./data/"
#define VERSION "0.1"
#endif

namespace GFX_UTIL {
    /// READ PIXEL FROM A SURFACE
    Uint32 inline GetPixel(SDL_Surface *surface, int x, int y);
    /// WRITE PIXEL TO A SURFACE
    void inline SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

    /// DRAW A NON-FILLED BOX (RGBA)
    void rectangleRGBA(SDL_Surface *screen, int x, int y, int x2, int y2,
                       unsigned char r, unsigned char g, unsigned char b,
                       unsigned char a);
    /// DRAW A FILLED BOX (RGBA)
    void boxRGBA(SDL_Surface *screen, int x1, int y1, int x2, int y2,
                 unsigned char r, unsigned char g, unsigned char b,
                 unsigned char a);

    /// SET TRANSLUCENCY FOR AN AREA
    void ScreenShade(SDL_Surface *surface, SDL_Rect *rect, float opacity,
                     unsigned char r, unsigned char g, unsigned char b);

};

//! namespace for std::string utility functions
namespace STR_UTIL {
    //! convert a number to string
    std::string intToString(int iNumber);
};

#endif
