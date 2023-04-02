#ifndef _HIGHSCORE_H__
#define _HIGHSCORE_H__

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <string>

#include "ErrorInfo.h"

class MusicManager;

typedef struct {
    std::string Name;
    uint16_t Score;
    uint8_t NumCard;
} ScoreInfo;

class HighScore {
    enum { NUMOFSCORE = 10 };

public:
    HighScore();
    LPErrInApp Load();
    LPErrInApp Save();
    LPErrInApp Show(SDL_Surface* screen, SDL_Surface* pSurfTitle,
                    SDL_Renderer* psdlRenderer, MusicManager* pMusicManager,
                    TTF_Font* pFont);

private:
    ScoreInfo _scoreInfo[NUMOFSCORE];
};

#endif
