#ifndef _HIGHSCORE_H__
#define _HIGHSCORE_H__

#include <SDL.h>

#include <string>

#include "ErrorInfo.h"

class MusicManager;

typedef struct {
    std::string Name;
    uint16_t Score;
    uint8_t NumCard;
} ScoreInfo;

class HighScore {
public:
    HighScore();
    LPErrInApp Load();
    LPErrInApp Save();
    LPErrInApp Show(SDL_Surface* screen, SDL_Surface* pSurfTitle,
                    SDL_Renderer* psdlRenderer, MusicManager* pMusicManager);

private:
    ScoreInfo _scoreInfo[10];
};

#endif
