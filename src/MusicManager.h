#ifndef _MUSICMANAGER__H_
#define _MUSICMANAGER__H_

#include "ErrorInfo.h"
#include "SDL_mixer.h"
#include "WinTypeGlobal.h"

class MusicManager {
public:
    enum { MUSIC_INIT_SND, MUSIC_GAME_SND, NUM_OF_SOUNDS };
    enum eLoopType { LOOP_ON, LOOP_OFF };
    enum { NUM_OF_WAV };
    MusicManager();
    virtual ~MusicManager();

    void Init();
    void StopMusic();
    bool PlayMusic(int iID, eLoopType eVal);
    bool PlayEffect(int iID);
    LPErrInApp LoadMusicRes();
    bool IsPLayingMusic();
    void SetVolumeMusic(int iVal);

private:
    Mix_Chunk* m_pMusicsWav[NUM_OF_WAV];
    Mix_Music* m_pMusics[NUM_OF_SOUNDS];
    bool m_bMusicAvailable;
};

#endif
