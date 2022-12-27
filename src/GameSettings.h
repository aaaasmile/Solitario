#ifndef __GAMESETTINGS__H_
#define __GAMESETTINGS__H_

#include <string>

#include "ErrorInfo.h"
#include "Languages.h"

class DeckType {
public:
    enum eDeckType {
        PIACENTINA = 0,
        BERGAMO,
        BOLOGNA,
        GENOVA,
        MILANO,
        NAPOLI,
        PIEMONTE,
        ROMAGNA,
        SARDEGNA,
        SICILIA,
        TOSCANA,
        TRENTO,
        TREVISO,
        TRIESTE,
    };

public:
    DeckType() {}

public:
    LPErrInApp SetType(eDeckType eType);
    std::string GetDeckName() { return _strDeckName; }
    std::string GetResFileName() { return _strResFileName; }
    std::string GetSymbolFileName() { return _strSymbolFileName; }
    eDeckType GetType() { return _eType; }
    void CopyFrom(DeckType& Val);
    void SetTypeIndex(int iVal);
    int GetTypeIndex() { return (int)_eType; }

private:
    eDeckType _eType;
    std::string _strDeckName;
    std::string _strSymbolFileName;
    std::string _strResFileName;
};

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
