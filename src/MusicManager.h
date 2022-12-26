#ifndef _MUSICMANAGER__H_
#define _MUSICMANAGER__H_

#include "SDL_mixer.h"
#include "win_type_global.h"

class MusicManager {
public:
    enum { MUSIC_INIT_SND, MUSIC_GAME_SND, NUM_OF_SOUNDS };
    enum eLoopType { LOOP_ON, LOOP_OFF };
    enum { NUM_OF_WAV };
    MusicManager();
    virtual ~MusicManager();

    void Init();
    void StopMusic();
    BOOL PlayMusic(int iID, eLoopType eVal);
    BOOL PlayEffect(int iID);
    void LoadMusicRes();
    BOOL IsPLayingMusic();
    void SetVolumeMusic(int iVal);

private:
    Mix_Chunk* m_pMusicsWav[NUM_OF_WAV];
    Mix_Music* m_pMusics[NUM_OF_SOUNDS];
    BOOL m_bMusicAvailable;
};

#endif
