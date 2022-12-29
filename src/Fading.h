#include <stdio.h>

#include "SDL.h"

void fade(SDL_Surface* p_surf_screen, SDL_Surface* p_surf_img,
          Uint32 ui_seconds, int i_fade_out, SDL_Renderer* psdlRenderer,
          SDL_Rect* prctTarget);