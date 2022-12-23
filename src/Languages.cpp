#include "Languages.h"

#include <assert.h>

Languages::Languages() {
    for (int i = 0; i < TOT_STRINGS; i++) {
        for (int j = 0; j < TOT_LANG; j++) {
            _mtxLangString[j][i] = "No TRanslation!";
        }
    }
    _eLangid = LANG_ITA;
    // dialetto
    _mtxLangString[LANG_DIAL_BREDA][ID_START] = "Cümincia";
    _mtxLangString[LANG_DIAL_BREDA][ID_LANGUAGESEL] = "Lengui furesti";
    _mtxLangString[LANG_DIAL_BREDA][ID_HISCORE] = "Al mèi di mèi";
    _mtxLangString[LANG_DIAL_BREDA][ID_VIDEOSET] = "Giosta al display";
    _mtxLangString[LANG_DIAL_BREDA][ID_MAINMENU] = "Menu principal";
    _mtxLangString[LANG_DIAL_BREDA][ID_EXIT] = "Fnomla che";
    _mtxLangString[LANG_DIAL_BREDA][ID_CHOOSELEVEL] = "Cata fòra al jeuq";
    _mtxLangString[LANG_DIAL_BREDA][ID_BACKMAINMENU] = "Al menu principal";
    _mtxLangString[LANG_DIAL_BREDA][ID_EASY] = "Facil";
    _mtxLangString[LANG_DIAL_BREDA][ID_HARD] = "Dificilisim";
    _mtxLangString[LANG_DIAL_BREDA][ID_HARDER] = "Mia tant facil";
    _mtxLangString[LANG_DIAL_BREDA][ID_BACK] = "Turna indrè";
    _mtxLangString[LANG_DIAL_BREDA][ID_SELECTDISPLAYSETTINGS] =
        "Cata föra al display";
    _mtxLangString[LANG_DIAL_BREDA][ID_INVIDO] = "Invidu";
    _mtxLangString[LANG_DIAL_BREDA][ID_CHOOSELANGUA] = "Cata föra la lengua";
    _mtxLangString[LANG_DIAL_BREDA][ID_ENGLISH] = "L'ingles";
    _mtxLangString[LANG_DIAL_BREDA][ID_DIALETMN] = "Al dialet";
    _mtxLangString[LANG_DIAL_BREDA][ID_ITALIANO] = "L'italian";
    _mtxLangString[LANG_DIAL_BREDA][ID_ANONIM] = "Anonim";
    _mtxLangString[LANG_DIAL_BREDA][ID_PUSHBUTTON] = "Schesa an buton";
    _mtxLangString[LANG_DIAL_BREDA][ID_CMD_UNKNOWN] = "Cumand mia cunusi";
    _mtxLangString[LANG_DIAL_BREDA][ID_CMD_HELP] = "Cumand cat po catar fora:";
    _mtxLangString[LANG_DIAL_BREDA][ID_CMD_CHATWELCOME] =
        "Benarriva' in dla chat";
    _mtxLangString[LANG_DIAL_BREDA][ID_CMD_CHATLIST] =
        "La lista adla gent in dla chat:";
    _mtxLangString[LANG_DIAL_BREDA][ID_SOLITARIO] = "Sulitari";
    _mtxLangString[LANG_DIAL_BREDA][ID_MAZZONAME] = "Mas ad li carti";
    _mtxLangString[LANG_DIAL_BREDA][ID_SOUNDOPT] = "Müsica";
    _mtxLangString[LANG_DIAL_BREDA][ID_ABILITATO] = "Impisada";
    _mtxLangString[LANG_DIAL_BREDA][ID_DISABILITATO] = "Smorsa";

    _mtxLangString[LANG_DIAL_BREDA][ID_CHOOSEMAZZO] =
        "Cata föra al mas ad lì carti";
    _mtxLangString[LANG_DIAL_BREDA][ID_PIACENTINA] = "Piacentini";
    _mtxLangString[LANG_DIAL_BREDA][ID_BERGAMO] = "Bergum";
    _mtxLangString[LANG_DIAL_BREDA][ID_BOLOGNA] = "Bulugnesi";
    _mtxLangString[LANG_DIAL_BREDA][ID_GENOVA] = "Genuesi";
    _mtxLangString[LANG_DIAL_BREDA][ID_MILANO] = "Milanesi";
    _mtxLangString[LANG_DIAL_BREDA][ID_NAPOLI] = "Napuletani";
    _mtxLangString[LANG_DIAL_BREDA][ID_PIEMONTE] = "Piemuntesi";
    _mtxLangString[LANG_DIAL_BREDA][ID_ROMAGNA] = "Rumagnoli";
    _mtxLangString[LANG_DIAL_BREDA][ID_SARDEGNA] = "Sardagnöli";
    _mtxLangString[LANG_DIAL_BREDA][ID_TOSCANA] = "Tuscani";
    _mtxLangString[LANG_DIAL_BREDA][ID_TRENTO] = "Trentini";
    _mtxLangString[LANG_DIAL_BREDA][ID_TREVISO] = "Trevisani";
    _mtxLangString[LANG_DIAL_BREDA][ID_TRIESTE] = "triestini";
    _mtxLangString[LANG_DIAL_BREDA][ID_SICILIA] = "Siciliani";
    _mtxLangString[LANG_DIAL_BREDA][ID_CREDITS] = "Ringrasiament";

    _mtxLangString[LANG_DIAL_BREDA][ID_WELCOMETITLEBAR] =
        "At capiti giost al Sulitari";
    _mtxLangString[LANG_DIAL_BREDA][ID_MEN_OPTIONS] = "Upsion";
    _mtxLangString[LANG_DIAL_BREDA][ID_MN_HELP] = "Ad du na man";

    // italiano
    _mtxLangString[LANG_ITA][ID_START] = "Inizia";
    _mtxLangString[LANG_ITA][ID_LANGUAGESEL] = "Lingua";
    _mtxLangString[LANG_ITA][ID_HISCORE] = "Hight Score";
    _mtxLangString[LANG_ITA][ID_VIDEOSET] = "Settings Display";
    _mtxLangString[LANG_ITA][ID_MAINMENU] = "Menu principale";
    _mtxLangString[LANG_ITA][ID_EXIT] = "Esci";
    _mtxLangString[LANG_ITA][ID_CHOOSELEVEL] = "Seleziona il gioco";
    _mtxLangString[LANG_ITA][ID_BACKMAINMENU] = "Vai al menu principale";
    _mtxLangString[LANG_ITA][ID_EASY] = "Facile";
    _mtxLangString[LANG_ITA][ID_HARD] = "Difficilissimo";
    _mtxLangString[LANG_ITA][ID_HARDER] = "Non facile";
    _mtxLangString[LANG_ITA][ID_BACK] = "Torna indietro";
    _mtxLangString[LANG_ITA][ID_SELECTDISPLAYSETTINGS] = "Seleziona il display";
    _mtxLangString[LANG_ITA][ID_INVIDO] = "Invidu";
    _mtxLangString[LANG_ITA][ID_CHOOSELANGUA] = "Seleziona la lingua";
    _mtxLangString[LANG_ITA][ID_ENGLISH] = "Inglese";
    _mtxLangString[LANG_ITA][ID_DIALETMN] = "Dialetto Bredese";
    _mtxLangString[LANG_ITA][ID_ITALIANO] = "Italiano";
    _mtxLangString[LANG_ITA][ID_ANONIM] = "Anonimo";
    _mtxLangString[LANG_ITA][ID_PUSHBUTTON] = "Premi un tasto";
    _mtxLangString[LANG_ITA][ID_CMD_UNKNOWN] = "Comando non riconosciuto";
    _mtxLangString[LANG_ITA][ID_CMD_HELP] = "Comandi che puoi usare";
    _mtxLangString[LANG_ITA][ID_CMD_CHATWELCOME] = "Bene arrivato nella chat";
    _mtxLangString[LANG_ITA][ID_CMD_CHATLIST] =
        "La lista dei presenti nella chat";
    _mtxLangString[LANG_ITA][ID_SOUNDOPT] = "Suono";
    _mtxLangString[LANG_ITA][ID_ABILITATO] = "Abilitato";
    _mtxLangString[LANG_ITA][ID_DISABILITATO] = "Disabilitato";

    _mtxLangString[LANG_ITA][ID_SOLITARIO] = "Solitario";
    _mtxLangString[LANG_ITA][ID_MAZZONAME] = "Mazzi di carte";
    _mtxLangString[LANG_ITA][ID_CHOOSEMAZZO] = "Scegli il mazzo di carte";
    _mtxLangString[LANG_ITA][ID_PIACENTINA] = "Piacentine";
    _mtxLangString[LANG_ITA][ID_BERGAMO] = "Bergamesche";
    _mtxLangString[LANG_ITA][ID_BOLOGNA] = "Bolognesi";
    _mtxLangString[LANG_ITA][ID_GENOVA] = "Genovesi";
    _mtxLangString[LANG_ITA][ID_MILANO] = "Milanesi";
    _mtxLangString[LANG_ITA][ID_NAPOLI] = "Napoletane";
    _mtxLangString[LANG_ITA][ID_PIEMONTE] = "Piemontesi";
    _mtxLangString[LANG_ITA][ID_ROMAGNA] = "Romagnole";
    _mtxLangString[LANG_ITA][ID_SARDEGNA] = "Sarde";
    _mtxLangString[LANG_ITA][ID_TOSCANA] = "Toscane";
    _mtxLangString[LANG_ITA][ID_TRENTO] = "Trentine";
    _mtxLangString[LANG_ITA][ID_TREVISO] = "Trevigiane";
    _mtxLangString[LANG_ITA][ID_TRIESTE] = "Triestine";
    _mtxLangString[LANG_ITA][ID_SICILIA] = "Siciliane";
    _mtxLangString[LANG_ITA][ID_CREDITS] = "Credits";
    _mtxLangString[LANG_ITA][ID_WELCOMETITLEBAR] = "Benvenuti";
    _mtxLangString[LANG_ITA][ID_MEN_OPTIONS] = "Opzioni";
    _mtxLangString[LANG_ITA][ID_MN_HELP] = "Guida";

    // English
    _mtxLangString[LANG_ENG][ID_START] = "Start";
    _mtxLangString[LANG_ENG][ID_LANGUAGESEL] = "Language";
    _mtxLangString[LANG_ENG][ID_HISCORE] = "Hight Score";
    _mtxLangString[LANG_ENG][ID_VIDEOSET] = "Settings Display";
    _mtxLangString[LANG_ENG][ID_MAINMENU] = "Main menu";
    _mtxLangString[LANG_ENG][ID_EXIT] = "Exit";
    _mtxLangString[LANG_ENG][ID_CHOOSELEVEL] = "Select the game";
    _mtxLangString[LANG_ENG][ID_BACKMAINMENU] = "Go to main menu";
    _mtxLangString[LANG_ENG][ID_EASY] = "Easy";
    _mtxLangString[LANG_ENG][ID_HARD] = "Very difficult";
    _mtxLangString[LANG_ENG][ID_HARDER] = "Not easy";
    _mtxLangString[LANG_ENG][ID_BACK] = "Back";
    _mtxLangString[LANG_ENG][ID_SELECTDISPLAYSETTINGS] = "Select the display";
    _mtxLangString[LANG_ENG][ID_INVIDO] = "Invido";
    _mtxLangString[LANG_ENG][ID_CHOOSELANGUA] = "Select language";
    _mtxLangString[LANG_ENG][ID_ENGLISH] = "English";
    _mtxLangString[LANG_ENG][ID_DIALETMN] = "Bredese dialect";
    _mtxLangString[LANG_ENG][ID_ITALIANO] = "Italian";
    _mtxLangString[LANG_ENG][ID_ANONIM] = "Anonymous";
    _mtxLangString[LANG_ENG][ID_PUSHBUTTON] = "Press a key";
    _mtxLangString[LANG_ENG][ID_CMD_UNKNOWN] = "Unrecognized command";
    _mtxLangString[LANG_ENG][ID_CMD_HELP] = "Commands you can use";
    _mtxLangString[LANG_ENG][ID_CMD_CHATWELCOME] = "Well arrived in the chat";
    _mtxLangString[LANG_ENG][ID_CMD_CHATLIST] =
        "The list of those present in the chat";
    _mtxLangString[LANG_ENG][ID_SOUNDOPT] = "Sound";
    _mtxLangString[LANG_ENG][ID_ABILITATO] = "Enabled";
    _mtxLangString[LANG_ENG][ID_DISABILITATO] = "Disabled";

    _mtxLangString[LANG_ENG][ID_SOLITARIO] = "Solitaire";
    _mtxLangString[LANG_ENG][ID_MAZZONAME] = "Card decks";
    _mtxLangString[LANG_ENG][ID_CHOOSEMAZZO] = "Choose the deck of cards";
    _mtxLangString[LANG_ENG][ID_PIACENTINA] = "Piacentine";
    _mtxLangString[LANG_ENG][ID_BERGAMO] = "Bergamesche";
    _mtxLangString[LANG_ENG][ID_BOLOGNA] = "Bolognesi";
    _mtxLangString[LANG_ENG][ID_GENOVA] = "Genovesi";
    _mtxLangString[LANG_ENG][ID_MILANO] = "Milanesi";
    _mtxLangString[LANG_ENG][ID_NAPOLI] = "Napoletane";
    _mtxLangString[LANG_ENG][ID_PIEMONTE] = "Piemontesi";
    _mtxLangString[LANG_ENG][ID_ROMAGNA] = "Romagnole";
    _mtxLangString[LANG_ENG][ID_SARDEGNA] = "Sarde";
    _mtxLangString[LANG_ENG][ID_TOSCANA] = "Toscane";
    _mtxLangString[LANG_ENG][ID_TRENTO] = "Trentine";
    _mtxLangString[LANG_ENG][ID_TREVISO] = "Trevigiane";
    _mtxLangString[LANG_ENG][ID_TRIESTE] = "Triestine";
    _mtxLangString[LANG_ENG][ID_SICILIA] = "Siciliane";
    _mtxLangString[LANG_ENG][ID_CREDITS] = "Credits";
    _mtxLangString[LANG_ENG][ID_WELCOMETITLEBAR] = "";
    _mtxLangString[LANG_ENG][ID_MEN_OPTIONS] = "";
    _mtxLangString[LANG_ENG][ID_MN_HELP] = "";
}

std::string Languages::GetStringId(eStringID eId) {
    std::string strRet = "Err trans.";

    assert(eId < TOT_STRINGS);

    if (eId < TOT_STRINGS) {
        strRet = _mtxLangString[_eLangid][eId];
    }

    return strRet;
}

char* Languages::GetCStringId(eStringID eId) {
    std::string strRet = "Err trans.";

    assert(eId < TOT_STRINGS);

    if (eId < TOT_STRINGS) {
        strRet = _mtxLangString[_eLangid][eId];
    }

    return const_cast<char*>(strRet.c_str());
}
