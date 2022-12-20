#include "GameSettings.h"

#include <assert.h>

void DeckType::SetType(eDeckType eType) {
    _eType = eType;
    switch (eType) {
        case PIACENTINA:
            _strDeckName = "Piacentine";
            _strResFileName = "mazzo_piac.pac";
            _strSymbolFileName = "symb_336.bmp";
            break;
        case BERGAMO:
            _strDeckName = "Bergamasche";
            _strResFileName = "bergamo_cards.pac";
            _strSymbolFileName = "symb_320.bmp";
            break;
        case BOLOGNA:
            _strDeckName = "Bolognesi";
            _strResFileName = "bologna_cards.pac";
            _strSymbolFileName = "symb_320.bmp";
            break;
        case GENOVA:
            _strDeckName = "Genovesi";
            _strResFileName = "genova_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            break;
        case MILANO:
            _strDeckName = "Milanesi";
            _strResFileName = "milano_cards.pac";
            _strSymbolFileName = "symb_320.bmp";
            break;
        case NAPOLI:
            _strDeckName = "Napoletane";
            _strResFileName = "napoli_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            break;
        case PIEMONTE:
            _strDeckName = "Piemontesi";
            _strResFileName = "piemonte_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            break;
        case ROMAGNA:
            _strDeckName = "Romagnole";
            _strResFileName = "romagna_cards.pac";
            _strSymbolFileName = "symb_336.bmp";
            break;
        case SARDEGNA:
            _strDeckName = "Sarde";
            _strResFileName = "sardegna_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            break;
        case SICILIA:
            _strDeckName = "Siciliane";
            _strResFileName = "sicilia_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            break;
        case TOSCANA:
            _strDeckName = "Toscane";
            _strResFileName = "toscana_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            break;
        case TRENTO:
            _strDeckName = "Trentine";
            _strResFileName = "trento_cards.pac";
            _strSymbolFileName = "symb_336.bmp";
            break;
        case TREVISO:
            _strDeckName = "Trevigiane";
            _strResFileName = "carte_trevisane.pac";
            _strSymbolFileName = "symb_320.bmp";
            break;
        case TRIESTE:
            _strDeckName = "Triestine";
            _strResFileName = "trieste_cards.pac";
            _strSymbolFileName = "symb_320.bmp";
            break;
        default:
            assert(0);
    }
}

void DeckType::CopyFrom(DeckType &Val) {
    _eType = Val.GetType();
    _strDeckName = Val.GetDeckName();
    _strResFileName = Val.GetResFileName();
    _strSymbolFileName = Val.GetSymbolFileName();
}

void DeckType::SetTypeIndex(int iVal) {
    switch (iVal) {
        case 0:
            SetType(PIACENTINA);
            break;
        case 1:
            SetType(BERGAMO);
            break;
        case 2:
            SetType(BOLOGNA);
            break;
        case 3:
            SetType(GENOVA);
            break;
        case 4:
            SetType(MILANO);
            break;
        case 5:
            SetType(NAPOLI);
            break;
        case 6:
            SetType(PIEMONTE);
            break;
        case 7:
            SetType(ROMAGNA);
            break;
        case 8:
            SetType(SARDEGNA);
            break;
        case 9:
            SetType(SICILIA);
            break;
        case 10:
            SetType(TOSCANA);
            break;
        case 11:
            SetType(TRENTO);
            break;
        case 12:
            SetType(TREVISO);
            break;
        case 13:
            SetType(TRIESTE);
            break;
        default:
            SetType(PIACENTINA);
            break;
    }
}
