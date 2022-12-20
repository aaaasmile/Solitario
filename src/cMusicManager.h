// cMusicManager.h: interface for the cMusicManager class.
#if !defined( \
    AFX_CMUSICMANAGER_H__166CCA3D_1404_45EE_821B_544A592F0B4F__INCLUDED_)
#define AFX_CMUSICMANAGER_H__166CCA3D_1404_45EE_821B_544A592F0B4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#include "SDL_mixer.h"

//! class cMusicManager
class cMusicManager {
public:
    cMusicManager();
    virtual ~cMusicManager();

    void Init();
    void StopMusic();
    void StartMusic();

private:
    //! music player
    Mix_Music* m_pMusics;
};

#endif  // !defined(AFX_CMUSICMANAGER_H__166CCA3D_1404_45EE_821B_544A592F0B4F__INCLUDED_)
