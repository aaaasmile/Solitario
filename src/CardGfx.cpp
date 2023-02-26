#include "CardGfx.h"

CardGfx::CardGfx() {
    _idx = 0;
    _faceUp = true;
    _rank = 0;
    _eSuit = eSUIT::BASTONI;
    _x = _y = _width = _height = 0;
    _pPacDeck = 0;
}

const char* CardGfx::SuitStr() {
    switch (_eSuit) {
        case BASTONI:
            return "Bastoni";
        case COPPE:
            return "Coppe";
        case DENARI:
            return "Denari";
        case SPADE:
            return "Spade";
        default:
            break;
    }
    return "";
}

LPErrInApp CardGfx::DrawCardPac(SDL_Surface* s) {
    int index = Index();
    int num_cards_insuit = _deckType.GetNumCardInSuit();

    if (index < 0 || index > _deckType.GetNumCards())
        return ERR_UTIL::ErrorCreate("Card index outside the range %d", index);
    if (_width == 0 || _height == 0) {
        return ERR_UTIL::ErrorCreate(
            "CardGfx - DrawCardPac size is not defined");
    }

    int iSegnoIx = index / num_cards_insuit;
    int iCartaIx = index % num_cards_insuit;
    SDL_Rect srcCard;

    srcCard.x = iSegnoIx * _width;
    srcCard.y = iCartaIx * _height;
    srcCard.w = _width;
    srcCard.h = _height;

    SDL_Rect dest;
    dest.x = X();
    dest.y = Y();

    if (SDL_BlitSurface(_pPacDeck, &srcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawCardPac with Iterator error: %s\n",
            SDL_GetError());
    }
    return NULL;
}