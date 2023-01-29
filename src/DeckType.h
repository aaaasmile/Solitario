#ifndef _DECKTYPE_H_
#define _DECKTYPE_H_

#include <string>

#include "ErrorInfo.h"

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
        NUM_OF_DECK
    };
    enum ePacType { PacTypePac, PacTypeSingleFile };

    DeckType() {}

    LPErrInApp SetType(eDeckType eType);
    std::string GetDeckName() { return _strDeckName; }
    std::string GetResFileName() { return _strResFileName; }
    std::string GetSymbolFileName() { return _strSymbolFileName; }
    eDeckType GetType() { return _eType; }
    ePacType GetPacType() { return _ePacType; }
    bool IsPacType() { return _ePacType == PacTypePac; }
    void CopyFrom(DeckType& Val);
    void SetTypeIndex(int iVal);
    int GetTypeIndex() { return (int)_eType; }

private:
    eDeckType _eType;
    ePacType _ePacType;
    std::string _strDeckName;
    std::string _strSymbolFileName;
    std::string _strResFileName;
};

#endif