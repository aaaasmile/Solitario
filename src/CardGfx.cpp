#include "CardGfx.h"

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
    int nCdIndex = Index();

    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    if (_width == 0 || _height == 0) {
        return ERR_UTIL::ErrorCreate(
            "CardGfx - DrawCardPac size is not defined");
    }

    int iSegnoIx = nCdIndex / 10;
    int iCartaIx = nCdIndex % 10;
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