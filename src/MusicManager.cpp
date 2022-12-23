#include "MusicManager.h"

#include <SDL2/SDL.h>

#include "GameSettings.h"
#include "GfxUtil.h"

static const char* lpszMusicFile = DATA_PREFIX "watermusic.it";

MusicManager::MusicManager() { m_pMusics = 0; }

MusicManager::~MusicManager() {}

void MusicManager::Init() {
    const GameSettings* p_GameSettings = GAMESET::GetSettings();
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr,
                "\nWarning: I could not initialize audio!\n"
                "The Simple DirectMedia error that occured was:\n"
                "%s\n\n",
                SDL_GetError());
    }
    if (Mix_OpenAudio(44100, AUDIO_S16, 2, 1024) < 0) {
        fprintf(stderr,
                "\nWarning: I could not set up audio for 44100 Hz "
                "16-bit stereo.\n"
                "The Simple DirectMedia error that occured was:\n"
                "%s\n\n",
                SDL_GetError());
    }

    m_pMusics = Mix_LoadMUS(lpszMusicFile);
    if (p_GameSettings->bMusicEnabled) {
        Mix_PlayMusic(m_pMusics, 0);
    }
}

void MusicManager::StopMusic() {
    Mix_FadeOutMusic(100);
    Mix_HaltMusic();
}

void MusicManager::StartMusic() {
    if (!Mix_PlayingMusic()) {
        Mix_PlayMusic(m_pMusics, 0);
    }
}
