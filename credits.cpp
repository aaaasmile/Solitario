/*
  credits.cpp
 
  For TuxMath
  Contains the text of the credits display, as well
  as the function which displays the credits in the game window.

  by Bill Kendrick
  bill@newbreedsoftware.com
  http://www.newbreedsoftware.com/


  Part of "Tux4Kids" Project
  http://www.tux4kids.org/
      
  August 26, 2001 - September 6, 2001
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "fading.h"

char * credit_text[] = {
  "-SOLITARIO",  /* '-' at beginning makes highlighted: */
  "",
  "PART OF THE 'BRISCOLA_NET' PROJECT",
  "",
  "-LEAD PROGRAMMER",
  "IGOR SARZI SARTORI",
  "",
  "",
  "-CONTRIBUTING ",
  "FABIO SPATARO",
  "",
  "",
  "-SPECIAL THANK",
  "IL SAN",
  "",
  "",
  "-GAME ENGINE",
  "RICO ROBERTO ZUNIGA",
  "",
  "",
  "-MAZZO",
  "DOMENICO STARNA",
  "",
  "",
  "-MUSIC",
  "AUTHOR OF WATERMUSIC",
  "",
  "", /* The following blanks cause the screen to scroll to complete blank: */
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  NULL
};


/* Some simple pixel-based characters we can blit quickly: */

char chars[38][5][6] = {
  {".###.",
   "#..##",
   "#.#.#",
   "##..#",
   ".###."},

  {"..#..",
   ".##..",
   "..#..",
   "..#..",
   ".###."},

  {".###.",
   "....#",
   "..##.",
   ".#...",
   "#####"},

  {".###.",
   "....#",
   "..##.",
   "....#",
   ".###."},

  {"...#.",
   "..##.",
   ".#.#.",
   "#####",
   "...#."},

  {"#####",
   "#....",
   "####.",
   "....#",
   "####."},

  {".###.",
   "#....",
   "####.",
   "#...#",
   ".###."},

  {"#####",
   "....#",
   "...#.",
   "..#..",
   ".#..."},

  {".###.",
   "#...#",
   ".###.",
   "#...#",
   ".###."},

  {".###.",
   "#...#",
   ".####",
   "....#",
   ".###."},

  {".###.",
   "#...#",
   "#####",
   "#...#",
   "#...#"},

  {"####.",
   "#...#",
   "####.",
   "#...#",
   "####."},
  
  {".###.",
   "#....",
   "#....",
   "#....",
   ".###."},
  
  {"####.",
   "#...#",
   "#...#",
   "#...#",
   "####."},
  
  {"#####",
   "#....",
   "###..",
   "#....",
   "#####"},
  
  {"#####",
   "#....",
   "###..",
   "#....",
   "#...."},
  
  {".###.",
   "#....",
   "#.###",
   "#...#",
   ".###."},
  
  {"#...#",
   "#...#",
   "#####",
   "#...#",
   "#...#"},
  
  {".###.",
   "..#..",
   "..#..",
   "..#..",
   ".###."},
  
  {"....#",
   "....#",
   "....#",
   "#...#",
   ".###."},
  
  {"#..#.",
   "#.#..",
   "##...",
   "#.#..",
   "#..#."},
  
  {"#....",
   "#....",
   "#....",
   "#....",
   "#####"},
  
  {"#...#",
   "##.##",
   "#.#.#",
   "#...#",
   "#...#"},
  
  {"#...#",
   "##..#",
   "#.#.#",
   "#..##",
   "#...#"},
  
  {".###.",
   "#...#",
   "#...#",
   "#...#",
   ".###."},
  
  {"####.",
   "#...#",
   "####.",
   "#....",
   "#...."},
  
  {".###.",
   "#...#",
   "#.#.#",
   "#..#.",
   ".##.#"},
  
  {"####.",
   "#...#",
   "####.",
   "#...#",
   "#...#"},
  
  {".###.",
   "#....",
   ".###.",
   "....#",
   ".###."},
  
  {"#####",
   "..#..",
   "..#..",
   "..#..",
   "..#.."},
  
  {"#...#",
   "#...#",
   "#...#",
   "#...#",
   ".###."},
  
  {"#...#",
   "#...#",
   ".#.#.",
   ".#.#.",
   "..#.."},
  
  {"#...#",
   "#...#",
   "#.#.#",
   "##.##",
   "#...#"},
  
  {"#...#",
   ".#.#.",
   "..#..",
   ".#.#.",
   "#...#"},

  {"#...#",
   ".#.#.",
   "..#..",
   "..#..",
   "..#.."},
  
  {"#####",
   "...#.",
   "..#..",
   ".#...",
   "#####"},
  
  {".....",
   ".....",
   ".....",
   "..#..",
   ".#..."},
  
  {"..#..",
   "..#..",
   ".....",
   ".....",
   "....."}
};


void draw_text(char * str, int offset, SDL_Surface* screen);


int line;


int credits(SDL_Surface* screen, SDL_Surface *pSurfTitle)
{
  int done, quit, scroll;
  SDL_Rect src, dest;
  SDL_Event event;
  Uint32 last_time, now_time;
  SDLKey key;
  
  
  /* Clear window: */
  fade (screen, screen, 2, 1) ;
  //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  
  
  /* Draw title: */
  
  dest.x = (screen->w - pSurfTitle->w) / 2;
  dest.y = 0;
  dest.w = pSurfTitle->w;
  dest.h = pSurfTitle->h;
  
  SDL_BlitSurface(pSurfTitle, NULL, screen, &dest);
  
  
  /* --- MAIN OPTIONS SCREEN LOOP: --- */
  
  done = 0;
  quit = 0;
  scroll = 0;
  line = 0;
  
  do
    {
      last_time = SDL_GetTicks();
      
      
      /* Handle any incoming events: */
      
      while (SDL_PollEvent(&event) > 0)
	{
	  if (event.type == SDL_QUIT)
	    {
	      /* Window close event - quit! */
	      
	      quit = 1;
	      done = 1;
	    }
	  else if (event.type == SDL_KEYDOWN)
	    {
	      key = event.key.keysym.sym;
	      
	      if (key == SDLK_ESCAPE)
		{
		  /* Escape key - quit! */
		  
		  done = 1;
		}
	    }
	}

      
      /* Scroll: */

      src.x = 0;
      src.y = (pSurfTitle->h) + 2;
      src.w = screen->w;
      src.h = screen->h - (pSurfTitle->h);
      
      dest.x = 0;
      dest.y = (pSurfTitle->h);
      dest.w = src.w;
      dest.h = src.h;
      
      SDL_BlitSurface(screen, &src, screen, &dest);

      dest.x = 0;
      dest.y = (screen->h) - 2;
      dest.w = screen->w;
      dest.h = 2;

      SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 0, 0, 0));
      
      
      scroll++;
      
      draw_text(credit_text[line], scroll, screen);
      

      if (scroll >= 9)
	{
	  scroll = 0;
	  line++;
	  
	  if (credit_text[line] == NULL)
	    done = 1;
	}
      
      
      SDL_Flip(screen);
      
      
      /* Pause (keep frame-rate event) */
      
      now_time = SDL_GetTicks();
      if (now_time < last_time + (1000 / 20))
	{
	  SDL_Delay(last_time + (1000 / 20) - now_time);
	}
    }
  while (!done);
  
  fade (screen, screen, 1, 1) ;
  /* Return the chosen command: */
  
  return quit;
}


void draw_text(char * str, int offset, SDL_Surface* screen)
{
  int i, c, x, y, cur_x, start, hilite;
  SDL_Rect dest;
  Uint8 r, g, b;


  if (str[0] == '-')
  {
    start = 1;
    hilite = 1;
  }
  else
  {
    start = 0;
    hilite = 0;
  }
  
  
  cur_x = (screen->w - ((strlen(str) - start) * 18)) / 2;
  
  for (i = start; i < (int)strlen(str); i++)
    {
      c = -1;
      
      if (str[i] >= '0' && str[i] <= '9')
	c = str[i] - '0';
      else if (str[i] >= 'A' && str[i] <= 'Z')
	c = str[i] - 'A' + 10;
      else if (str[i] == ',')
	c = 36;
      else if (str[i] == '\'')
	c = 37;
      
      
      if (c != -1)
	{
	  for (y = 0; y < 5; y++)
	    {
	      if (hilite == 0)
	      {
	        r = 255 - ((line * y) % 256);
	        g = 255 / (y + 2);
	        b = (line * line * 2) % 256;
	      }
	      else
	      {
		r = 128;
		g = 192;
		b = 255 - (y * 40);
	      }
	      
	      for (x = 0; x < 5; x++)
		{
		  if (chars[c][y][x] == '#')
		    {
		      dest.x = cur_x + (x * 3);
		      dest.y = ((screen->h - (5 * 3)) + (y * 3) +
				(18 - offset * 2));
		      dest.w = 3;
		      dest.h = 3;
		      
		      SDL_FillRect(screen, &dest,
				   SDL_MapRGB(screen->format, r, g, b));
		    }
		}
	    }
	}
      
      
      /* Move virtual cursor: */
      
      cur_x = cur_x + 18;
    }
}
