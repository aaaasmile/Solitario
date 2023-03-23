#include "HighScore.h"

#include <SDL2/SDL.h>
#include <memory.h>
#include <stdlib.h>

#include "GameSettings.h"
#include "WinTypeGlobal.h"

using namespace std;

static const char *g_lpszScore = "solitario_score.bin";

HighScore::HighScore() {
    for (int k = 0; k < 10; k++) {
        _scoreInfo[k].Score = 3450 - (k * 150);
        _scoreInfo[k].Name = "Re dal sulitari";
        _scoreInfo[k].NumCard = 40;
    }
}

LPErrInApp HighScore::Save() {
    LPGameSettings pGameSettings = GAMESET::GetSettings();
    char filepath[PATH_MAX + strlen(g_lpszScore)];

    snprintf(filepath, sizeof(filepath), "%s/%s", pGameSettings->SettingsDir,
             g_lpszScore);
    TRACE("Save score file %s\n", filepath);

    SDL_RWops *dst = SDL_RWFromFile(filepath, "rb");
    if (dst == 0) {
        return ERR_UTIL::ErrorCreate("Unable to save high score file %s",
                                     filepath);
    }

    for (int k = 0; k < 10; k++) {
        char name[16];
        memset(name, 0, 16);
        memcpy(name, _scoreInfo[k].Name.c_str(), 15);
        if (SDL_RWwrite(dst, name, 16, 1) < 16) {
            return ERR_UTIL::ErrorCreate("SDL_RWwrite name highscore %s\n",
                                         SDL_GetError());
        }
        if (SDL_WriteLE16(dst, _scoreInfo[k].Score) == 0) {
            return ERR_UTIL::ErrorCreate("SDL_RWwrite score highscore %s\n",
                                         SDL_GetError());
        }
        if (SDL_RWwrite(dst, &_scoreInfo[k].NumCard, 1, 1) < 1) {
            return ERR_UTIL::ErrorCreate("SDL_RWwrite numCard highscore %s\n",
                                         SDL_GetError());
        }
    }
    SDL_RWclose(dst);
    return NULL;
}

LPErrInApp HighScore::Load() {
    LPGameSettings pGameSettings = GAMESET::GetSettings();
    char filepath[PATH_MAX + strlen(g_lpszScore)];

    snprintf(filepath, sizeof(filepath), "%s/%s", pGameSettings->SettingsDir,
             g_lpszScore);
    TRACE("Load score file %s\n", filepath);
    SDL_RWops *src = SDL_RWFromFile(filepath, "rb");
    if (src == 0) {
        TRACE("No score file found, ignore it\n");
    }

    for (int k = 0; k < 10; k++) {
        char name[16];
        uint16_t score = 0;
        uint8_t numCard;
        if (SDL_RWread(src, name, 16, 1) == 0) {
            return ERR_UTIL::ErrorCreate(
                "SDL_RWread on highscore file error (file %s): %s\n", filepath,
                SDL_GetError());
        }
        score = SDL_ReadLE16(src);
        if (SDL_RWread(src, &numCard, 1, 1) == 0) {
            return ERR_UTIL::ErrorCreate(
                "SDL_RWread on highscore file error (file %s): %s\n", filepath,
                SDL_GetError());
        }

        _scoreInfo[k].Name = name;
        _scoreInfo[k].Score = score;
        _scoreInfo[k].NumCard = numCard;
    }
    SDL_RWclose(src);

    return NULL;
}
