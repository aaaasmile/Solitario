#ifndef __GAMESETTINGS__H_
#define __GAMESETTINGS__H_

#include <string>

#include "DeckType.h"
#include "ErrorInfo.h"
#include "Languages.h"

class GameSettings {
public:
    std::string PlayerName;
    int Level;
    DeckType DeckTypeVal;
    Languages::eLangId CurrentLanguage;
    bool MusicEnabled;
    std::string SettingsDir;

public:
    GameSettings() {
        PlayerName = "Anonimo";
        Level = 1;
        DeckTypeVal.SetType(DeckType::PIACENTINA);
        CurrentLanguage = Languages::LANG_ITA;
        MusicEnabled = false;
        SettingsDir = "";
    }
};

typedef GameSettings* LPGameSettings;

namespace GAMESET {
    GameSettings* GetSettings();
}
#endif
