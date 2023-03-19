#include "MusicManager.h"

#include <SDL2/SDL.h>

#include "GameSettings.h"

static const char* lpszaSound_filenames[MusicManager::NUM_OF_SOUNDS] = {
    DATA_PREFIX "music/wolmer-invido.ogg", DATA_PREFIX "music/watermusic.it"};

static const char* lpszaEffects_filenames[MusicManager::NUM_OF_WAV] = {NULL};

MusicManager::MusicManager() {
    for (int i = 0; i < NUM_OF_SOUNDS; i++) {
        _p_Musics[i] = 0;
    }
    _isMusicAvailable = false;
    for (int j = 0; j < NUM_OF_WAV; j++) {
        _p_MusicsWav[j] = 0;
    }
    _currentMusicID = 0;
    _currentLoop = LOOP_ON;
}

MusicManager::~MusicManager() {
    for (int i = 0; i < NUM_OF_SOUNDS; i++) {
        Mix_FreeMusic(_p_Musics[i]);
    }
    for (int j = 0; j < NUM_OF_WAV; j++) {
        Mix_FreeChunk(_p_MusicsWav[j]);
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
            _isMusicAvailable = true;
        }
    }
}

LPErrInApp MusicManager::LoadMusicRes() {
    STRING exeDirPath = GAMESET::GetExeSolitarioFolder();

    for (int i = 0; i < NUM_OF_SOUNDS; i++) {
        STRING strFileTmp2 = lpszaSound_filenames[i];
        STRING strFileFullPath = exeDirPath + '/' + strFileTmp2;
        TRACE_DEBUG("Loading music part %s\n", strFileFullPath.c_str());
        _p_Musics[i] = Mix_LoadMUS(strFileFullPath.c_str());
        if (_p_Musics[i] == NULL) {
            return ERR_UTIL::ErrorCreate(
                "Unable to load %s music resource, error: %s\n",
                strFileFullPath.c_str(), SDL_GetError());
        }
    }

    for (int j = 0; j < NUM_OF_WAV; j++) {
        if (lpszaEffects_filenames[j] != NULL) {
            _p_MusicsWav[j] = Mix_LoadWAV(lpszaEffects_filenames[j]);
            if (_p_MusicsWav[j] == NULL) {
                return ERR_UTIL::ErrorCreate(
                    "Unable to load %s wav resource, error: %s\n",
                    lpszaEffects_filenames[j], SDL_GetError());
            }
        }
    }
    return NULL;
}

void MusicManager::StopMusic(int fadingMs) {
    Mix_FadeOutMusic(fadingMs);
    Mix_HaltMusic();
}

bool MusicManager::IsPlayingMusic() { return Mix_PlayingMusic(); }

bool MusicManager::PlayMusic(int iID, eLoopType eVal) {
    if (iID < 0 || iID >= NUM_OF_SOUNDS || !_isMusicAvailable) {
        return false;
    }
    if (_p_Musics[iID] == 0) {
        return false;
    }

    _currentMusicID = iID;
    _currentLoop = eVal;

    if (eVal == LOOP_OFF) {
        Mix_PlayMusic(_p_Musics[iID], 0);
    } else {
        Mix_PlayMusic(_p_Musics[iID], -1);
    }

    return true;
}

void MusicManager::PlayCurrentMusic() {
    PlayMusic(_currentMusicID, _currentLoop);
}

bool MusicManager::PlayEffect(int iID) {
    if (iID < 0 || iID >= NUM_OF_WAV || !_isMusicAvailable) {
        return false;
    }
    Mix_PlayChannel(-1, _p_MusicsWav[iID], 0);
    return true;
}

void MusicManager::SetVolumeMusic(int iVal) { Mix_VolumeMusic(iVal); }
