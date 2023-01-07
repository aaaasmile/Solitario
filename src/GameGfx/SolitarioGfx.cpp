#include "SolitarioGfx.h"

#include <SDL_endian.h>
#include <SDL_image.h>
#include <stdio.h>

#include "Fading.h"
#include "GfxUtil.h"
#include "WinTypeGlobal.h"

// card graphics
int g_CardWidth = 0;
int g_CardHeight = 0;
int g_SymbolWidth = 84;
int g_SymbolHeight = 144;

#define FPS 30
#define FRAMETICKS (1000 / FPS)
#define THINKINGS_PER_TICK 1

static const char *lpszBackgroundImgFile = DATA_PREFIX "images/im001537.jpg";
static const char *lpszDeckDir = DATA_PREFIX "mazzi/";
static const char *lpszSymbDir = DATA_PREFIX "images/";

SolitarioGfx::SolitarioGfx() {
    _p_Background = 0;
    _p_Dragface = 0;
    _p_SceneBackground = 0;
    _p_ScreenTexture = 0;
    for (int i = 0; i < NUM_CARDS_ONDECK; i++) {
        _p_CardsSurf[i] = 0;
    }
}

SolitarioGfx::~SolitarioGfx() { ClearSurface(); }

LPErrInApp SolitarioGfx::Initialize(SDL_Surface *s, SDL_Renderer *r) {
    TRACE("Initialize Solitario");
    LPErrInApp err;
    _p_Screen = s;
    _p_sdlRenderer = r;
    _p_ScreenTexture = SDL_CreateTexture(
        r, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, s->w, s->h);
    if (_p_ScreenTexture == NULL) {
        return ERR_UTIL::ErrorCreate("Cannot create texture: %s\n",
                                     SDL_GetError());
    }

    _p_Background = SDL_CreateRGBSurface(SDL_SWSURFACE, _p_Screen->w,
                                         _p_Screen->h, 32, 0, 0, 0, 0);
    SDL_RWops *srcBack = SDL_RWFromFile(lpszBackgroundImgFile, "rb");
    if (srcBack == 0) {
        return ERR_UTIL::ErrorCreate("Error in SDL_RWFromFile: %s\n",
                                     SDL_GetError());
    }
    _p_SceneBackground = IMG_LoadJPG_RW(srcBack);
    if (_p_SceneBackground == 0) {
        return ERR_UTIL::ErrorCreate("Cannot create scene background: %s\n",
                                     SDL_GetError());
    }
    if (_DeckType.IsPacType()) {
        TRACE("Deck Pac stuff");
        err = LoadCardPac();
        if (err != NULL) {
            return err;
        }
        err = LoadSymbolsForPac();
        if (err != NULL) {
            return err;
        }
    } else {
        TRACE("Single deck file stuff");
        err = LoadDeckFromSingleFile();
        if (err != NULL) {
            return err;
        }
        err = LoadSymbolsFromSingleFile();
        if (err != NULL) {
            return err;
        }
    }

    return DrawBackground(true);
}

LPErrInApp SolitarioGfx::DrawCardStack(LPCardRegionGfx pcardRegion) {
    return DrawCardStack(_p_Screen, pcardRegion);
}

LPErrInApp SolitarioGfx::DrawCardStack(SDL_Surface *s,
                                       LPCardRegionGfx pcardRegion) {
    LPErrInApp err;
    if (!(pcardRegion->Attributes & CRD_VISIBLE))
        return NULL;

    DrawSymbol(pcardRegion->XCoord, pcardRegion->YCoord, pcardRegion->Symbol);
    for (int i = 0; i < pcardRegion->InternalStack.Size(); i++) {
        LPCardGfx pCard = pcardRegion->InternalStack.Item(i);
        if (pCard->IsFaceUp()) {
            err = DrawCard(pCard, s);
        } else {
            err = DrawCardBack(pCard->X(), pCard->Y(), s);
        }
        if (err != NULL) {
            return err;
        }
    }
    return NULL;
}

void SolitarioGfx::ClearSurface() {
    if (_p_Background)
        SDL_FreeSurface(_p_Background);
    if (_p_SceneBackground)
        SDL_FreeSurface(_p_SceneBackground);

    if (_p_ScreenTexture != NULL) {
        SDL_DestroyTexture(_p_ScreenTexture);
    }
}

void SolitarioGfx::CreateRegion(int id, unsigned int attribs,
                                unsigned int amode, int dmode, int symbol,
                                int x, int y, int xoffset, int yoffset) {
    CardRegionGfx *cr = new CardRegionGfx(id, attribs, amode, dmode, symbol, x,
                                          y, xoffset, yoffset);
    _cardRegionList.push_back(*cr);
}

bool SolitarioGfx::DeleteRegion(LPCardRegionGfx pRegion) {
    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        if (&(*vir) == pRegion) {
            _cardRegionList.erase(vir);
            return true;
        }
    }
    return false;
}

void SolitarioGfx::EmptyStacks() {
    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir)
        vir->Clear();
}

void SolitarioGfx::InitAllCoords() {
    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        vir->InitCardCoords();
        vir->InitCardFaces();
    }
}

LPCardRegionGfx SolitarioGfx::GetRegionOnPoint(int x, int y) {
    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        if (vir->PtInStack(x, y)) {
            _p_cardRegion = &(*vir);
            return _p_cardRegion;
        }
    }
    return NULL;
}

void SolitarioGfx::ClearAll() {
    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        vir->Clear();
    }
    _cardRegionList.clear();
}

LPErrInApp SolitarioGfx::InitDrag(int x, int y, bool &isInitDrag) {
    return InitDrag(NULL, x, y, isInitDrag);
}

LPErrInApp SolitarioGfx::InitDrag(LPCardStackGfx pCargoStack, int x, int y,
                                  bool &isInitDrag) {
    isInitDrag = false;
    LPErrInApp err;
    if (pCargoStack == NULL) {
        if (_p_cardRegion->IsEmpty())
            return NULL;

        int idx = _p_cardRegion->GetClickedCard(x, y);
        if (idx == -1) {
            return NULL;  // no card found
        }
        int dm = _p_cardRegion->GetDragMode();
        switch (dm) {
            case CRD_DRAGTOP: {
                if (_p_cardRegion->Size() - 1 == idx)
                    _dragStack.PushCard(_p_cardRegion->PopCard());
                else
                    return NULL;
                break;
            }
            case CRD_DRAGFACEUP: {
                if (_p_cardRegion->IsCardFaceUp(idx)) {
                    LPCardStackGfx pStack =
                        _p_cardRegion->PopStack(_p_cardRegion->Size() - idx);
                    _dragStack.PushStack(pStack);
                    delete pStack;
                } else
                    return NULL;
                break;
            }
            default: {
                return ERR_UTIL::ErrorCreate(
                    "InitDrag error: Drag mode %d not supported\n", dm);
            }
        }
    } else
        _dragStack.PushStack(pCargoStack);

    _p_cardRegion->InitCardCoords();

    err = DrawBackground(false);
    if (err != NULL) {
        return err;
    }
    SDL_BlitSurface(_p_Screen, NULL, _p_Background, NULL);

    CardRegionGfx DragRegion(0, _p_cardRegion->GetAttributes() | CRD_FACEUP, 0,
                             0, 0, 0, 0, _p_cardRegion->GetxOffset(),
                             _p_cardRegion->GetyOffset());
    DragRegion.PushStack(&_dragStack);

    _dragCard.x = _dragStack.First()->X();
    _dragCard.y = _dragStack.First()->Y();
    _dragCard.width = DragRegion.GetStackWidth();
    _dragCard.height = DragRegion.GetStackHeight();

    if (_p_Dragface != NULL) {
        SDL_FreeSurface(_p_Dragface);
    }

    _p_Dragface = SDL_CreateRGBSurface(SDL_SWSURFACE, _dragCard.width,
                                       _dragCard.height, 32, 0, 0, 0, 0);
    SDL_FillRect(_p_Dragface, NULL, SDL_MapRGB(_p_Dragface->format, 0, 255, 0));
    SDL_SetColorKey(_p_Dragface, true,
                    SDL_MapRGB(_p_Dragface->format, 0, 255, 0));

    err = DrawCardStack(_p_Screen, &DragRegion);
    if (err != NULL) {
        return err;
    }
    DragRegion.InitCardCoords();
    err = DrawCardStack(_p_Dragface, &DragRegion);
    if (err != NULL) {
        return err;
    }

    _oldx = x;
    _oldy = y;

    updateTextureAsFlipScreen();
    isInitDrag = true;
    return NULL;
}

void SolitarioGfx::updateTextureAsFlipScreen() {
    SDL_UpdateTexture(_p_ScreenTexture, NULL, _p_Screen->pixels,
                      _p_Screen->pitch);
    SDL_RenderCopy(_p_sdlRenderer, _p_ScreenTexture, NULL, NULL);
    SDL_RenderPresent(_p_sdlRenderer);
}

void SolitarioGfx::DoDrag(int x, int y) {
    // TRACE("DoDrag (x=%d, y=%d) => drag_x=%d, drag_y=%d. old_x=%d, old_y=%d",
    // x,
    //       y, _dragCard.x, _dragCard.y, _oldx, _oldy);
    SDL_Rect rcs;
    SDL_Rect rcd;

    rcs.x = _dragCard.x - 2;
    rcs.y = _dragCard.y - 2;
    rcs.w = _dragCard.width + 4;
    rcs.h = _dragCard.height + 4;

    rcd.x = _dragCard.x - 2;
    rcd.y = _dragCard.y - 2;

    if (_dragCard.x < 0)
        rcs.x = rcd.x = 0;
    if (_dragCard.y < 0)
        rcs.y = rcd.y = 0;

    if (x < _oldx)
        _dragCard.x -= _oldx - x;
    else
        _dragCard.x += x - _oldx;
    if (y < _oldy)
        _dragCard.y -= _oldy - y;
    else
        _dragCard.y += y - _oldy;

    _oldx = x;
    _oldy = y;

    SDL_Rect dest;
    dest.x = _dragCard.x;
    dest.y = _dragCard.y;

    SDL_BlitSurface(_p_Background, &rcs, _p_Screen, &rcd);
    SDL_BlitSurface(_p_Dragface, NULL, _p_Screen, &dest);

    updateTextureAsFlipScreen();
}

void SolitarioGfx::DoDrop() { DoDrop(NULL); }

void SolitarioGfx::DoDrop(LPCardRegionGfx pDestRegion) {
    LPCardStackGfx pDestStack;
    LPCardRegionGfx pBestRegion;

    if (pDestRegion != NULL)
        pBestRegion = pDestRegion;
    else
        pBestRegion = GetBestStack(_dragCard.x, _dragCard.y, g_CardWidth,
                                   g_CardHeight, &_dragStack);
    if (pBestRegion == NULL)
        pBestRegion = _p_cardRegion;  // drop go back to the source, no stack
                                      // found to recive the drag

    pDestStack = pBestRegion->GetCardStack();
    pDestStack->PushStack(&_dragStack);
    pBestRegion->InitCardCoords();

    LPCardGfx pCard = NULL;
    switch (_p_cardRegion->GetDragMode()) {
        case CRD_DRAGTOP:
            pCard = pDestStack->Last();
            break;
        default:
            pCard = pDestStack->Item(pDestStack->Size() - _dragStack.Size());
    }

    _dragStack.Clear();

    if (_dragCard.x == pCard->X() && _dragCard.y == pCard->Y())
        return;  // when no movement

    zoomDropCard(_dragCard.x, _dragCard.y, pCard, _dragCard.width,
                 _dragCard.height, _p_Background, _p_Dragface);

    SDL_FreeSurface(_p_Dragface);
    _p_Dragface = NULL;
}

void calcPt(int x0, int y0, int x1, int y1, float t, int &xf, int &yf) {
    xf = int(x0 + t * (x1 - x0) + .5);
    yf = int(y0 + t * (y1 - y0) + .5);
}

void SolitarioGfx::zoomDropCard(int &sx, int &sy, LPCardGfx pCard, int w, int h,
                                SDL_Surface *bg, SDL_Surface *fg) {
    TRACE("Zoom card x=%d, y=%d", sx, sy);
    SDL_Rect rcs;
    SDL_Rect rcd;
    SDL_Rect dest;

    int px, py;
    int dx = pCard->X();
    int dy = pCard->Y();
    float precision = 0.1f;

    for (float i = 0.0; i <= 1.0; i += precision) {
        SDL_PumpEvents();
        calcPt(sx, sy, dx, dy, i, px, py);

        rcs.x = sx - 2;
        rcs.y = sy - 2;
        rcs.w = w + 4;
        rcs.h = h + 4;

        rcd.x = sx - 2;
        rcd.y = sy - 2;

        if (sx < 0)
            rcs.x = rcd.x = 0;
        if (sy < 0)
            rcs.y = rcd.y = 0;

        sx = dest.x = px;
        sy = dest.y = py;

        SDL_BlitSurface(bg, &rcs, _p_Screen, &rcd);
        SDL_BlitSurface(fg, NULL, _p_Screen, &dest);

        updateTextureAsFlipScreen();
    }
    DrawStaticScene();
}

LPCardRegionGfx SolitarioGfx::FindDropRegion(int id, LPCardGfx pCard) {
    CardStackGfx stack;
    stack.PushCard(pCard);
    return FindDropRegion(id, &stack);
}

LPCardRegionGfx SolitarioGfx::FindDropRegion(int id, LPCardStackGfx pStack) {
    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        if ((vir->Id == id) && vir->CanDrop(pStack))
            return &(*vir);
    }
    return NULL;
}

void SolitarioGfx::DrawStaticScene() {
    if (_p_SceneBackground != NULL) {
        SDL_FillRect(_p_Screen, &_p_Screen->clip_rect,
                     SDL_MapRGBA(_p_Screen->format, 0, 0, 0, 0));
        SDL_Rect rctTarget;
        rctTarget.x = (_p_Screen->w - _p_SceneBackground->w) / 2;
        rctTarget.y = (_p_Screen->h - _p_SceneBackground->h) / 2;
        rctTarget.w = _p_SceneBackground->w;
        rctTarget.h = _p_SceneBackground->h;
        SDL_BlitSurface(_p_SceneBackground, NULL, _p_Screen, &rctTarget);
    }

    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        SDL_PumpEvents();
        CardRegionGfx cardRegion = *vir;
        DrawCardStack(&cardRegion);
    }
    updateTextureAsFlipScreen();
}

LPErrInApp SolitarioGfx::DrawBackground(bool bIsInit) {
    LPErrInApp err;
    if (_p_SceneBackground != NULL) {
        SDL_FillRect(_p_Screen, &_p_Screen->clip_rect,
                     SDL_MapRGBA(_p_Screen->format, 0, 0, 0, 0));
        SDL_Rect rctTarget;
        rctTarget.x = (_p_Screen->w - _p_SceneBackground->w) / 2;
        rctTarget.y = (_p_Screen->h - _p_SceneBackground->h) / 2;
        rctTarget.w = _p_SceneBackground->w;
        rctTarget.h = _p_SceneBackground->h;

        if (!bIsInit) {
            SDL_BlitSurface(_p_SceneBackground, NULL, _p_Screen, &rctTarget);
        } else {
            fade(_p_Screen, _p_SceneBackground, 2, 0, _p_sdlRenderer,
                 &rctTarget);
        }
    }

    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        CardRegionGfx cardRegion = *vir;
        err = DrawCardStack(&cardRegion);
        if (err != NULL) {
            return NULL;
        }
    }
    return NULL;
}

LPCardRegionGfx SolitarioGfx::GetBestStack(int x, int y, int w, int h,
                                           LPCardStackGfx pStack) {
    int maxoverlap = 0;
    int percent = 0;
    LPCardRegionGfx pBest = NULL;

    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        SDL_PumpEvents();
        if (vir->CanDrop(pStack))
            percent = vir->GetOverlapRatio(x, y, w,
                                           h);  // stack candidate for dropping
        else
            percent = 0;

        if (percent > maxoverlap) {
            maxoverlap = percent;
            pBest = &(*vir);
        }
    }
    return pBest;
}

LPErrInApp SolitarioGfx::DrawCard(int x, int y, int nCdIndex) {
    return DrawCard(x, y, nCdIndex, _p_Screen);
}

LPErrInApp SolitarioGfx::DrawCard(int x, int y, int nCdIndex, SDL_Surface *s) {
    if (s == NULL) {
        return ERR_UTIL::ErrorCreate(
            "Draw a card on NULL surface. This is wrong");
    }
    if (_DeckType.IsPacType()) {
        return DrawCardPac(x, y, nCdIndex, s);
    }
    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    int iSegnoIx = nCdIndex / 10;
    int iCartaIx = nCdIndex % 10;
    TRACE("Suit %d, card: %d", iSegnoIx, iCartaIx);

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_CardsSurf[nCdIndex]->clip_rect.w;
    _rctSrcCard.h = _p_CardsSurf[nCdIndex]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (SDL_BlitSurface(_p_CardsSurf[nCdIndex], &_rctSrcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate("SDL_BlitSurface in draw card error: %s\n",
                                     SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawCardPac(int x, int y, int nCdIndex,
                                     SDL_Surface *s) {
    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    int iSegnoIx = nCdIndex / 10;
    int iCartaIx = nCdIndex % 10;
    TRACE("Suit %d, card: %d", iSegnoIx, iCartaIx);

    SDL_Rect srcCard;
    srcCard.x = iSegnoIx * g_CardWidth;
    srcCard.y = iCartaIx * g_CardHeight;
    srcCard.w = g_CardWidth;
    srcCard.h = g_CardHeight;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    if (SDL_BlitSurface(_p_Deck, &srcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawCardPac with Iterator error: %s\n",
            SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawCard(LPCardGfx pCard, SDL_Surface *s) {
    if (s == NULL) {
        return ERR_UTIL::ErrorCreate(
            "Error in draw card with Iterator, surface is NULL\n");
    }
    TRACE("Draw card ix = %d, suit = %s, rank %d, x,y %d,%d", pCard->Index(),
          pCard->SuitStr(), pCard->Rank(), pCard->X(), pCard->Y());

    if (_DeckType.IsPacType()) {
        return DrawCardPac(pCard, s);
    }
    int nCdIndex = pCard->Index();

    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_CardsSurf[nCdIndex]->clip_rect.w;
    _rctSrcCard.h = _p_CardsSurf[nCdIndex]->clip_rect.h;

    SDL_Rect dest;
    dest.x = pCard->X();
    dest.y = pCard->Y();

    if (SDL_BlitSurface(_p_CardsSurf[nCdIndex], &_rctSrcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in draw card with Iterator error: %s\n",
            SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawCardPac(LPCardGfx pCard, SDL_Surface *s) {
    int nCdIndex = pCard->Index();

    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    int iSegnoIx = nCdIndex / 10;
    int iCartaIx = nCdIndex % 10;
    SDL_Rect srcCard;

    srcCard.x = iSegnoIx * g_CardWidth;
    srcCard.y = iCartaIx * g_CardHeight;
    srcCard.w = g_CardWidth;
    srcCard.h = g_CardHeight;

    SDL_Rect dest;
    dest.x = pCard->X();
    dest.y = pCard->Y();

    if (SDL_BlitSurface(_p_Deck, &srcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawCardPac with Iterator error: %s\n",
            SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawCardBack(int x, int y) {
    return DrawCardBack(x, y, _p_Screen);
}

LPErrInApp SolitarioGfx::DrawCardBack(int x, int y, SDL_Surface *s) {
    if (s == NULL) {
        return ERR_UTIL::ErrorCreate(
            "Error in DrawCardBack, surface is NULL\n");
    }

    if (_DeckType.IsPacType()) {
        return DrawCardBackPac(x, y, s);
    }

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_SymbolsSurf[0]->clip_rect.w;
    _rctSrcCard.h = _p_SymbolsSurf[0]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (SDL_BlitSurface(_p_SymbolsSurf[0], &_rctSrcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawCardBack error: %s\n", SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawCardBackPac(int x, int y, SDL_Surface *s) {
    SDL_Rect dest, srcBack;
    dest.x = x;
    dest.y = y;

    srcBack.x = 0;
    srcBack.y = 0;
    srcBack.w = g_SymbolWidth;
    srcBack.h = g_SymbolHeight;

    if (SDL_BlitSurface(_p_Symbols, &srcBack, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawCardBackPac error: %s\n", SDL_GetError());
    }

    return NULL;
}

LPErrInApp SolitarioGfx::DrawSymbol(int x, int y, int nSymbol) {
    return DrawSymbol(x, y, nSymbol, _p_Screen);
}

LPErrInApp SolitarioGfx::DrawSymbol(int x, int y, int nSymbol, SDL_Surface *s) {
    if (nSymbol < 1) {
        return ERR_UTIL::ErrorCreate("Symbol index %d out of range", nSymbol);
    }
    if (nSymbol > 3)
        nSymbol = 3;

    if (_DeckType.IsPacType()) {
        return DrawSymbolPac(x, y, nSymbol, s);
    }

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_SymbolsSurf[nSymbol]->clip_rect.w;
    _rctSrcCard.h = _p_SymbolsSurf[nSymbol]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (SDL_BlitSurface(_p_SymbolsSurf[nSymbol], &_rctSrcCard, s, &dest) ==
        -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawSymbol error: %s\n", SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawSymbolPac(int x, int y, int nSymbol,
                                       SDL_Surface *s) {
    SDL_Rect srcCard;
    srcCard.x = nSymbol * g_SymbolWidth;
    srcCard.y = 0;
    srcCard.w = g_SymbolWidth;
    srcCard.h = g_SymbolHeight;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    if (SDL_BlitSurface(_p_Symbols, &srcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawSymbolPac error: %s\n", SDL_GetError());
    }

    return NULL;
}

LPErrInApp SolitarioGfx::VictoryAnimation() {
    srand((unsigned)time(NULL));
    LPErrInApp err;
    int rot;
    int id;
    int x;
    unsigned int y;
    int xspeed;
    int yspeed;

    int gravity = 1;
    unsigned int max_y = _p_Screen->h;
    float bounce = 0.8f;
    SDL_Event event;

    while (1) {
        rot = rand() % 2;
        id = rand() % NUM_CARDS;
        x = rand() % _p_Screen->w;
        y = rand() % _p_Screen->h / 2;

        if (rot)
            xspeed = -4;
        else
            xspeed = 4;

        yspeed = 0;

        do {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        return NULL;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            return NULL;
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        return NULL;
                }
            }
            yspeed = yspeed + gravity;
            x += xspeed;
            y += yspeed;

            if (y + g_CardHeight > max_y) {
                y = max_y - g_CardHeight;
                yspeed = int(-yspeed * bounce);
            }
            err = DrawCard(x, y, id, _p_Screen);
            if (err != NULL) {
                return err;
            }
            updateTextureAsFlipScreen();
        } while ((x + g_CardWidth > 0) && (x < _p_Screen->w));
    }
    return NULL;
}

LPErrInApp SolitarioGfx::LoadDeckFromSingleFile() {
    std::string strTmp;
    std::string strSuffix;
    char buff[128];

    for (int k = 0; k < 4; k++) {
        std::string strFileName = lpszDeckDir;
        switch (k) {
            case 0:
                strSuffix = "bastoni";
                strFileName += "bastoni/";
                break;
            case 1:
                strSuffix = "coppe";
                strFileName += "coppe/";
                break;
            case 2:
                strSuffix = "denari";
                strFileName += "denari/";
                break;
            case 3:
                strSuffix = "spade";
                strFileName += "spade/";
                break;
        }
        for (int i = 0; i < 10; i++) {
            sprintf(buff, "%02d_%s.jpg", i + 1, strSuffix.c_str());
            strTmp = buff;
            std::string strComplete = strFileName + strTmp;
            SDL_RWops *srcBack = SDL_RWFromFile(strComplete.c_str(), "rb");

            if (srcBack == NULL) {
                return ERR_UTIL::ErrorCreate(
                    "SDL_RWFromFile in LoadDeckFromSingleFile error: "
                    "%s\n",
                    SDL_GetError());
            }
            _p_CardsSurf[i + k * 10] = IMG_LoadJPG_RW(srcBack);
            if (_p_CardsSurf[i + k * 10] == NULL) {
                return ERR_UTIL::ErrorCreate(
                    "IMG_LoadJPG_RW in LoadDeckFromSingleFile error: "
                    "%s\n",
                    SDL_GetError());
            }
        }
    }
    g_CardWidth = _p_CardsSurf[0]->clip_rect.w;
    g_CardHeight = _p_CardsSurf[0]->clip_rect.h;

    return NULL;
}

LPErrInApp SolitarioGfx::LoadSymbolsFromSingleFile() {
    VCT_STRINGS vct_Strings;
    vct_Strings.push_back("dorso.jpg");
    vct_Strings.push_back("fine_1.jpg");
    vct_Strings.push_back("fine_2.jpg");
    vct_Strings.push_back("fine_3.jpg");

    if (NUM_SYMBOLS != vct_Strings.size()) {
        return ERR_UTIL::ErrorCreate(
            "Error: NUM_SYMBOLS is not the symbol vector size\n");
    }

    for (int i = 0; i < NUM_SYMBOLS; i++) {
        std::string strDir = lpszDeckDir;
        std::string strFileSymbName;
        strFileSymbName = lpszDeckDir + vct_Strings[i];
        SDL_RWops *srcBack = SDL_RWFromFile(strFileSymbName.c_str(), "rb");
        if (!srcBack) {
            return ERR_UTIL::ErrorCreate(
                "SDL_RWFromFile in LoadSymbolsFromSingleFile error: "
                "%s\n",
                SDL_GetError());
        }
        _p_SymbolsSurf[i] = IMG_LoadJPG_RW(srcBack);
        if (_p_SymbolsSurf[i] == NULL) {
            return ERR_UTIL::ErrorCreate(
                "IMG_LoadJPG_RW in LoadSymbolsFromSingleFile error: "
                "%s\n",
                SDL_GetError());
        }
    }
    return NULL;
}

LPErrInApp SolitarioGfx::LoadCardPac() {
    TRACE("Load card Pac");
    Uint32 timetag;
    char description[100];
    Uint8 num_anims;
    Uint16 w, h;
    Uint16 frames;

    std::string strFileName = lpszDeckDir;
    strFileName += _DeckType.GetResFileName();

    SDL_RWops *src = SDL_RWFromFile(strFileName.c_str(), "rb");
    if (src == 0) {
        return ERR_UTIL::ErrorCreate(
            "SDL_RWFromFile on pac file error (file %s): %s\n",
            strFileName.c_str(), SDL_GetError());
    }
    SDL_RWread(src, description, 100, 1);
    timetag = SDL_ReadLE32(src);
    TRACE("Timetag is %d", timetag);
    SDL_RWread(src, &num_anims, 1, 1);
    // witdh of the picture (pac of 4 cards)
    w = SDL_ReadLE16(src);
    // height of the picture (pac of 10 rows of cards)
    h = SDL_ReadLE16(src);
    frames = SDL_ReadLE16(src);

    for (int i = 0; i < frames; i++) {
        SDL_ReadLE16(src);
    }

    _p_Deck = IMG_LoadPNG_RW(src);
    if (!_p_Deck) {
        return ERR_UTIL::ErrorCreate(
            "IMG_LoadPNG_RW on pac file error (file %s): %s\n",
            strFileName.c_str(), SDL_GetError());
    }

    SDL_SetColorKey(_p_Deck, true, SDL_MapRGB(_p_Deck->format, 0, 128, 0));

    g_CardWidth = w / 4;
    g_CardHeight = h / 10;

    return NULL;
}

LPErrInApp SolitarioGfx::LoadSymbolsForPac() {
    std::string strFileSymbName = lpszSymbDir;
    strFileSymbName += _DeckType.GetSymbolFileName();

    _p_Symbols = SDL_LoadBMP(strFileSymbName.c_str());
    if (_p_Symbols == 0) {
        return ERR_UTIL::ErrorCreate("LOad bitmap failed: %s\n",
                                     SDL_GetError());
    }

    if (_DeckType.GetSymbolFileName() == "symb_336.bmp") {
        SDL_SetColorKey(_p_Symbols, true,
                        SDL_MapRGB(_p_Symbols->format, 242, 30, 206));
    } else {
        SDL_SetColorKey(_p_Symbols, true,
                        SDL_MapRGB(_p_Symbols->format, 0, 128, 0));
    }

    g_SymbolWidth = _p_Symbols->w / 4;
    g_SymbolHeight = _p_Symbols->h;

    return NULL;
}
