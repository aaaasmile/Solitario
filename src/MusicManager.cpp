#include "MusicManager.h"

#include <SDL2/SDL.h>

static const char* lpszaSound_filenames[MusicManager::NUM_OF_SOUNDS] = {
    DATA_PREFIX "music/wolmer-invido.ogg", DATA_PREFIX "music/watermusic.it"};

static const char* lpszaEffects_filenames[MusicManager::NUM_OF_WAV] = {NULL};

MusicManager::MusicManager() {
    for (int i = 0; i < NUM_OF_SOUNDS; i++) {
        m_pMusics[i] = 0;
    }
    m_bMusicAvailable = false;
    for (int j = 0; j < NUM_OF_WAV; j++) {
        m_pMusicsWav[j] = 0;
    }
    _currentMusicID = 0;
    _currentLoop = LOOP_ON;
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
            m_bMusicAvailable = true;
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
        if (lpszaEffects_filenames[j] != NULL) {
            m_pMusicsWav[j] = Mix_LoadWAV(lpszaEffects_filenames[j]);
            if (m_pMusicsWav[j] == NULL) {
                return ERR_UTIL::ErrorCreate("Unable to load %s wav resource\n",
                                             lpszaEffects_filenames[j]);
            }
        }
    }
    return NULL;
}

void MusicManager::StopMusic() {
    Mix_FadeOutMusic(100);
    Mix_HaltMusic();
}

bool MusicManager::IsPLayingMusic() { return Mix_PlayingMusic(); }

bool MusicManager::PlayMusic(int iID, eLoopType eVal) {
    if (iID < 0 || iID >= NUM_OF_SOUNDS || !m_bMusicAvailable) {
        return false;
    }
    if (m_pMusics[iID] == 0) {
        return false;
    }

    _currentMusicID = iID;
    _currentLoop = eVal;

    if (eVal == LOOP_OFF) {
        Mix_PlayMusic(m_pMusics[iID], 0);
    } else {
        Mix_PlayMusic(m_pMusics[iID], -1);
    }

    return true;
}

void MusicManager::PlayCurrentMusic() {
    PlayMusic(_currentMusicID, _currentLoop);
}

bool MusicManager::PlayEffect(int iID) {
    if (iID < 0 || iID >= NUM_OF_WAV || !m_bMusicAvailable) {
        return false;
    }
    Mix_PlayChannel(-1, m_pMusicsWav[iID], 0);
    return true;
}

void MusicManager::SetVolumeMusic(int iVal) { Mix_VolumeMusic(iVal); }
