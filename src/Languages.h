// Languages.h

#ifndef __CLANGUAGES__H__
#define __CLANGUAGES__H__

#include <string>

//! class LanguageMgr
class LanguageMgr
{
public:
    enum eLangId
    {
        LANG_ITA = 0,
        LANG_DIAL_MN,
        LANG_ENG,
        TOT_LANG // it mus be  always on the last place
    };
    enum eStringID
    {
        ID_START = 0,
        ID_LANGUAGESEL,
        ID_HISCORE,
        ID_VIDEOSET,
        ID_MAINMENU,
        ID_EXIT,
        ID_CHOOSELEVEL,
        ID_BACKMAINMENU,
        ID_EASY,
        ID_HARD,
        ID_HARDER,
        ID_BACK,
        ID_SELECTDISPLAYSETTINGS,
        ID_INVIDO,
        ID_CHOOSELANGUA,
        ID_ITALIANO,
        ID_ENGLISH,
        ID_DIALETMN,
        ID_ANONIM,
        ID_PUSHBUTTON,
        ID_CMD_UNKNOWN,
        ID_CMD_HELP,
        ID_CMD_CHATWELCOME,
        ID_CMD_CHATLIST,
        ID_SOLITARIO,
        ID_MAZZONAME,
        ID_CHOOSEMAZZO,
        ID_PIACENTINA,
        ID_BERGAMO,
        ID_BOLOGNA,
        ID_GENOVA,
        ID_MILANO,
        ID_NAPOLI,
        ID_PIEMONTE,
        ID_ROMAGNA,
        ID_SARDEGNA,
        ID_SICILIA,
        ID_TOSCANA,
        ID_TRENTO,
        ID_TREVISO,
        ID_TRIESTE,
        ID_SOUNDOPT,
        ID_ABILITATO,
        ID_DISABILITATO,
        ID_CREDITS,
        TOT_STRINGS // it mus be always on the last place
    };

public:
    LanguageMgr();
    void  SetLang(eLangId eVal){m_eLangid = eVal;}
    std::string  GetStringId(eStringID eId);
    char*        GetCStringId(eStringID eId);

private:
    eLangId      m_eLangid;
    std::string  m_matStringsLang[TOT_LANG][TOT_STRINGS];
};  

#endif
