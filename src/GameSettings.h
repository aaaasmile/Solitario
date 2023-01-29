#ifndef __GAMESETTINGS__H_
#define __GAMESETTINGS__H_

#include <string>

#include "DeckType.h"
#include "ErrorInfo.h"
#include "Languages.h"

class GameSettings {
public:
    std::string strPlayerName;
    long lDelayTimer;
    bool bAutoTerminate;
    long lLevel;
    long lGuestId;
    long lIpRemote_1;
    long lIpRemote_2;
    long lIpRemote_3;
    long lIpRemote_4;
    DeckType deckTypeVal;
    Languages::eLangId eLanguageCurrent;
    bool bMusicEnabled;

public:
    GameSettings() {
        strPlayerName = "Anonimo";
        lDelayTimer = 3;
        bAutoTerminate = true;
        lLevel = 1;
        lGuestId = 0;
        lIpRemote_1 = 127;
        lIpRemote_2 = 0;
        lIpRemote_3 = 0;
        lIpRemote_4 = 1;
        deckTypeVal.SetType(DeckType::PIACENTINA);
        eLanguageCurrent = Languages::LANG_ITA;
        bMusicEnabled = false;
    }
};

namespace GAMESET {
    GameSettings* GetSettings();
}
#endif
