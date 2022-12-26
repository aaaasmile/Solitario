#include "MusicManager.h"

#include <SDL2/SDL.h>

static const char* lpszaSound_filenames[MusicManager::NUM_OF_SOUNDS] = {
    "data/music/wolmer-invido.ogg", "data/music/watermusic.it"};

static const char* lpszaEffects_filenames[MusicManager::NUM_OF_WAV] = {};

MusicManager::MusicManager() {
    for (int i = 0; i < NUM_OF_SOUNDS; i++) {
        m_pMusics[i] = 0;
    }
    m_bMusicAvailable = FALSE;
    for (int j = 0; j < NUM_OF_WAV; j++) {
        m_pMusicsWav[j] = 0;
    }
}

MusicManager::~MusicManager() {
    for (int i = 0; i < NUM_OF_SOUNDS; i++) {
        Mix_FreeMusic(m_pMusics[i]);
    }
    for (int j = 0; j < NUM_OF_WAV; j++) {
        Mix_FreeChunk(m_pMusicsWav[j]);
    }
}

void MusicManager::Init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr,
                "\nWarning: I could not initialize audio!\n"
                "The Simple DirectMedia error that occured was:\n"
                "%s\n\n",
                SDL_GetError());

    } else {
        if (Mix_OpenAudio(44100, AUDIO_S16, 2, 1024) < 0) {
            fprintf(stderr,
                    "\nWarning: I could not set up audio for 44100 Hz "
                    "16-bit stereo.\n"
                    "The Simple DirectMedia error that occured was:\n"
                    "%s\n\n",
                    SDL_GetError());

        } else {
            m_bMusicAvailable = TRUE;
        }
    }
}

LPErrInApp MusicManager::LoadMusicRes() {
    for (int i = 0; i < NUM_OF_SOUNDS; i++) {
        STRING strFileTmp2 = lpszaSound_filenames[i];
        m_pMusics[i] = Mix_LoadMUS(strFileTmp2.c_str());
        if (m_pMusics[i] == NULL) {
            return ERR_UTIL::ErrorCreate("Unable to load %s music resource\n",
                                         strFileTmp2.c_str());
        }
    }

    for (int j = 0; j < NUM_OF_WAV; j++) {
        m_pMusicsWav[j] = Mix_LoadWAV(lpszaEffects_filenames[j]);
        if (m_pMusicsWav[j] == NULL) {
            return ERR_UTIL::ErrorCreate("Unable to load %s wav resource\n",
                                         lpszaEffects_filenames[j]);
        }
    }
    return NULL;
}

void MusicManager::StopMusic() {
    Mix_FadeOutMusic(100);
    Mix_HaltMusic();
}

BOOL MusicManager::IsPLayingMusic() { return Mix_PlayingMusic(); }

BOOL MusicManager::PlayMusic(int iID, eLoopType eVal) {
    if (iID < 0 || iID >= NUM_OF_SOUNDS || !m_bMusicAvailable) {
        return FALSE;
    }
    if (m_pMusics[iID] == 0) {
        return FALSE;
    }

    if (eVal == LOOP_OFF) {
        Mix_PlayMusic(m_pMusics[iID], 0);
    } else {
        Mix_PlayMusic(m_pMusics[iID], -1);
    }

    return TRUE;
}

BOOL MusicManager::PlayEffect(int iID) {
    if (iID < 0 || iID >= NUM_OF_WAV || !m_bMusicAvailable) {
        return FALSE;
    }
    Mix_PlayChannel(-1, m_pMusicsWav[iID], 0);
    return TRUE;
}

void MusicManager::SetVolumeMusic(int iVal) { Mix_VolumeMusic(iVal); }
