#include "DeckType.h"

LPErrInApp DeckType::SetType(eDeckType eType) {
    _eType = eType;
    switch (eType) {
        case PIACENTINA:
            _strDeckName = "Piacentine";
            _strResFileName = "mazzo_piac.pac";
            _strSymbolFileName = "symb_336.bmp";
            _ePacType = PacTypePac;
            break;
        case BERGAMO:
            _strDeckName = "Bergamasche";
            _strResFileName = "bergamo_cards.pac";
            _strSymbolFileName = "symb_320.bmp";
            _ePacType = PacTypePac;
            break;
        case BOLOGNA:
            _strDeckName = "Bolognesi";
            _strResFileName = "bologna_cards.pac";
            _strSymbolFileName = "symb_320.bmp";
            _ePacType = PacTypePac;
            break;
        case GENOVA:
            _strDeckName = "Genovesi";
            _strResFileName = "genova_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            _ePacType = PacTypePac;
            break;
        case MILANO:
            _strDeckName = "Milanesi";
            _strResFileName = "milano_cards.pac";
            _strSymbolFileName = "symb_320.bmp";
            _ePacType = PacTypePac;
            break;
        case NAPOLI:
            _strDeckName = "Napoletane";
            _strResFileName = "napoli_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            _ePacType = PacTypePac;
            break;
        case PIEMONTE:
            _strDeckName = "Piemontesi";
            _strResFileName = "piemonte_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            _ePacType = PacTypePac;
            break;
        case ROMAGNA:
            _strDeckName = "Romagnole";
            _strResFileName = "romagna_cards.pac";
            _strSymbolFileName = "symb_336.bmp";
            _ePacType = PacTypePac;
            break;
        case SARDEGNA:
            _strDeckName = "Sarde";
            _strResFileName = "sardegna_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            _ePacType = PacTypePac;
            break;
        case SICILIA:
            _strDeckName = "Siciliane";
            _strResFileName = "sicilia_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            _ePacType = PacTypePac;
            break;
        case TOSCANA:
            _strDeckName = "Toscane";
            _strResFileName = "toscana_cards.pac";
            _strSymbolFileName = "symb_368.bmp";
            _ePacType = PacTypePac;
            break;
        case TRENTO:
            _strDeckName = "Trentine";
            _strResFileName = "trento_cards.pac";
            _strSymbolFileName = "symb_336.bmp";
            _ePacType = PacTypePac;
            break;
        case TREVISO:
            _strDeckName = "Trevigiane";
            _strResFileName = "carte_trevisane.pac";
            _strSymbolFileName = "symb_320.bmp";
            _ePacType = PacTypePac;
            break;
        case TRIESTE:
            _strDeckName = "Triestine";
            _strResFileName = "trieste_cards.pac";
            _strSymbolFileName = "symb_320.bmp";
            _ePacType = PacTypePac;
            break;
        case TAROCK_PIEMONT:
            _strDeckName = "Tarock";
            _strResFileName = "tarock_piemonte.pac";
            _strSymbolFileName = "symbols_tarock.png";
            _ePacType = PacTypePac;
            break;
        default:
            return ERR_UTIL::ErrorCreate("Error deck type %d not supported",
                                         eType);
    }
    return NULL;
}

void DeckType::CopyFrom(DeckType& Val) {
    _eType = Val.GetType();
    _ePacType = Val.GetPacType();
    _strDeckName = Val.GetDeckName();
    _strResFileName = Val.GetResFileName();
    _strSymbolFileName = Val.GetSymbolFileName();
}

LPErrInApp DeckType::SetTypeIndex(int index) {
    LPErrInApp err;
    switch (index) {
        case 0:
            err = SetType(PIACENTINA);
            break;
        case 1:
            err = SetType(BERGAMO);
            break;
        case 2:
            err = SetType(BOLOGNA);
            break;
        case 3:
            err = SetType(GENOVA);
            break;
        case 4:
            err = SetType(MILANO);
            break;
        case 5:
            err = SetType(NAPOLI);
            break;
        case 6:
            err = SetType(PIEMONTE);
            break;
        case 7:
            err = SetType(ROMAGNA);
            break;
        case 8:
            err = SetType(SARDEGNA);
            break;
        case 9:
            err = SetType(SICILIA);
            break;
        case 10:
            err = SetType(TOSCANA);
            break;
        case 11:
            err = SetType(TRENTO);
            break;
        case 12:
            err = SetType(TREVISO);
            break;
        case 13:
            err = SetType(TRIESTE);
            break;
        case 14:
            err = SetType(TAROCK_PIEMONT);
            break;
        default:
            err = ERR_UTIL::ErrorCreate("Deck Type Index %d not found", index);
            break;
    }
    return err;
}

int DeckType::GetNumCardInSuit() {
    if (_eType == eDeckType::TAROCK_PIEMONT)
        return 14;
    return 10;
}
