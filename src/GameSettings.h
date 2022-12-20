#ifndef __GAMESETTINGS__H_
#define __GAMESETTINGS__H_

#include <string>

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
    void SetType(eDeckType eType);
    std::string GetResFileName() { return m_strResFileName; }
    std::string GetMazzoName() { return m_strMazzoName; }
    std::string GetSymbolFileName() { return m_strSymbolName; }
    eDeckType GetType() { return m_eType; }
    void Clone(DeckType &Val);
    void SetTypeIndex(int iVal);
    int GetTypeIndex() { return (int)m_eType; }

private:
    eDeckType m_eType;
    std::string m_strMazzoName;
    std::string m_strResFileName;
    std::string m_strSymbolName;
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
    DeckType DeckType;
    LanguageMgr::eLangId eLanguageCurrent;
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
        DeckType.SetType(DeckType::PIACENTINA);
        eLanguageCurrent = LanguageMgr::LANG_ITA;
        bMusicEnabled = false;
    }
};

#endif
