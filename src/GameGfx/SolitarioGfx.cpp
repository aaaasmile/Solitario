#include "SolitarioGfx.h"

#include <SDL_endian.h>
#include <SDL_image.h>
#include <stdio.h>

#include "CompGfx/cButtonGfx.h"
#include "Fading.h"
#include "GfxUtil.h"
#include "WinTypeGlobal.h"

// card graphics
int g_CardWidth = 0;
int g_CardHeight = 0;
int g_SymbolWidth = 84;
int g_SymbolHeight = 144;

#define CRD_DECKPILE 0
#define CRD_FOUNDATION 1
#define CRD_DECK_FACEUP 2
#define CRD_ACE 3

#define FPS 30
#define FRAMETICKS (1000 / FPS)
#define THINKINGS_PER_TICK 1

static const char *lpszBackgroundImgFile = DATA_PREFIX "images/im001537.jpg";
static const char *lpszSymbDir = DATA_PREFIX "images/";
extern const char *lpszDeckDir;

const int MYIDQUIT = 0;
const int MYIDNEWGAME = 1;

SolitarioGfx::SolitarioGfx() {
    _p_ScreenBackbuffer = 0;
    _bStartdrag = false;
    _p_Dragface = 0;
    _p_SceneBackground = 0;
    _p_ScreenTexture = 0;
    for (int i = 0; i < NUM_CARDS_ONDECK; i++) {
        _p_CardsSurf[i] = 0;
    }
    _p_BtQuit = NULL;
    _p_BtNewGame = NULL;
}

SolitarioGfx::~SolitarioGfx() {
    CleanUpRegion();
    clearSurface();
    delete _p_BtNewGame;
    delete _p_BtQuit;
}

void SolitarioGfx::clearSurface() {
    if (_p_ScreenBackbuffer != NULL) {
        SDL_FreeSurface(_p_ScreenBackbuffer);
        _p_ScreenBackbuffer = NULL;
    }

    if (_p_SceneBackground != NULL) {
        SDL_FreeSurface(_p_SceneBackground);
        _p_SceneBackground = NULL;
    }

    if (_p_ScreenTexture != NULL) {
        SDL_DestroyTexture(_p_ScreenTexture);
        _p_ScreenTexture = NULL;
    }
}

// command buttons
void fncBind_ButtonQuitClick(void *self, int val) {
    SolitarioGfx *pApp = (SolitarioGfx *)self;
    pApp->BtQuitClick();
}

ClickCb SolitarioGfx::prepClickQuitCb() {
    static VClickCb const tc = {.Click = (&fncBind_ButtonQuitClick)};
    return (ClickCb){.tc = &tc, .self = this};
}

void fncBind_ButtonNewGameClick(void *self, int val) {
    SolitarioGfx *pApp = (SolitarioGfx *)self;
    pApp->BtNewGameClick();
}

ClickCb SolitarioGfx::prepClickNewGameCb() {
    static VClickCb const tc = {.Click = (&fncBind_ButtonNewGameClick)};
    return (ClickCb){.tc = &tc, .self = this};
}

LPErrInApp SolitarioGfx::Initialize(SDL_Surface *s, SDL_Renderer *r,
                                    SDL_Window *w, DeckType &dt,
                                    LPLanguages planguages,
                                    TTF_Font *pfontText) {
    TRACE("Initialize Solitario\n");
    setDeckType(dt);
    _p_Languages = planguages;
    _p_FontText = pfontText;
    LPErrInApp err;
    _p_Screen = s;
    _p_sdlRenderer = r;
    _p_Window = w;
    _p_ScreenTexture = SDL_CreateTexture(
        r, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, s->w, s->h);
    if (_p_ScreenTexture == NULL) {
        return ERR_UTIL::ErrorCreate("Cannot create texture: %s\n",
                                     SDL_GetError());
    }

    _p_ScreenBackbuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, _p_Screen->w,
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
        TRACE("Deck Pac stuff\n");
        err = LoadCardPac();
        if (err != NULL) {
            return err;
        }
        err = LoadSymbolsForPac();
        if (err != NULL) {
            return err;
        }
    } else {
        TRACE("Single deck file stuff\n");
        err = LoadDeckFromSingleFile();
        if (err != NULL) {
            return err;
        }
        err = LoadSymbolsFromSingleFile();
        if (err != NULL) {
            return err;
        }
    }

    return DrawInitialScene();
}

void SolitarioGfx::InitAllCoords() {
    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        vir->InitCardCoords();
        vir->InitCardFaces();
    }
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

LPCardRegionGfx SolitarioGfx::SelectRegionOnPoint(int x, int y) {
    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        if (vir->PtInStack(x, y)) {
            _p_selectedCardRegion = &(*vir);
            return _p_selectedCardRegion;
        }
    }
    return NULL;
}

void SolitarioGfx::CleanUpRegion() {
    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        vir->CleanUp();
        vir->Clear();
    }
}

LPErrInApp SolitarioGfx::InitDrag(int x, int y, bool &isInitDrag) {
    return InitDrag(NULL, x, y, isInitDrag);
}

LPErrInApp SolitarioGfx::InitDrag(LPCardStackGfx pCargoStack, int x, int y,
                                  bool &isInitDrag) {
    isInitDrag = false;
    LPErrInApp err;
    if (pCargoStack == NULL) {
        if (_p_selectedCardRegion->IsEmpty())
            return NULL;

        int idx = _p_selectedCardRegion->GetClickedCard(x, y);
        if (idx == -1) {
            return NULL;  // no card found
        }
        int dm = _p_selectedCardRegion->GetDragMode();
        switch (dm) {
            case CRD_DRAGTOP: {
                if (_p_selectedCardRegion->Size() - 1 == idx)
                    _dragStack.PushCard(_p_selectedCardRegion->PopCard());
                else
                    return NULL;
                break;
            }
            case CRD_DRAGFACEUP: {
                if (_p_selectedCardRegion->IsCardFaceUp(idx)) {
                    LPCardStackGfx pStack = _p_selectedCardRegion->PopStack(
                        _p_selectedCardRegion->Size() - idx);
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

    _p_selectedCardRegion->InitCardCoords();

    DrawStaticScene();
    CardRegionGfx DragRegion(
        0, _p_selectedCardRegion->GetAttributes() | CRD_FACEUP, 0, 0, 0, 0, 0,
        _p_selectedCardRegion->GetxOffset(),
        _p_selectedCardRegion->GetyOffset());
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
    // x, y, _dragCard.x, _dragCard.y, _oldx, _oldy);
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

    SDL_BlitSurface(_p_ScreenBackbuffer, &rcs, _p_Screen, &rcd);
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
        pBestRegion = _p_selectedCardRegion;  // drop go back to the source, no
                                              // stack found to recive the drag

    pDestStack = pBestRegion->GetCardStack();
    pDestStack->PushStack(&_dragStack);
    pBestRegion->InitCardCoords();

    LPCardGfx pCard = NULL;
    switch (_p_selectedCardRegion->GetDragMode()) {
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
                 _dragCard.height);

    SDL_FreeSurface(_p_Dragface);
    _p_Dragface = NULL;
}

void calcPt(int x0, int y0, int x1, int y1, float t, int &xf, int &yf) {
    xf = int(x0 + t * (x1 - x0) + .5);
    yf = int(y0 + t * (y1 - y0) + .5);
}

void SolitarioGfx::zoomDropCard(int &sx, int &sy, LPCardGfx pCard, int w,
                                int h) {
    // TRACE("Zoom card x=%d, y=%d", sx, sy);
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

        SDL_BlitSurface(_p_ScreenBackbuffer, &rcs, _p_Screen, &rcd);
        SDL_BlitSurface(_p_Dragface, NULL, _p_Screen, &dest);

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
    // static scene is drawn direct into the screen.
    // Then the screen is copied into the back buffer for animations
    SDL_FillRect(_p_Screen, &_p_Screen->clip_rect,
                 SDL_MapRGBA(_p_Screen->format, 0, 0, 0, 0));
    SDL_Rect rctTarget;
    rctTarget.x = (_p_Screen->w - _p_SceneBackground->w) / 2;
    rctTarget.y = (_p_Screen->h - _p_SceneBackground->h) / 2;
    rctTarget.w = _p_SceneBackground->w;
    rctTarget.h = _p_SceneBackground->h;
    SDL_BlitSurface(_p_SceneBackground, NULL, _p_Screen, &rctTarget);

    for (regionVI vir = _cardRegionList.begin(); vir != _cardRegionList.end();
         ++vir) {
        SDL_PumpEvents();
        CardRegionGfx cardRegion = *vir;
        DrawCardStack(&cardRegion);
    }
    _p_BtNewGame->DrawButton(_p_Screen);
    _p_BtQuit->DrawButton(_p_Screen);
    // it seams here that SDL_BlitSurface copy only the bitmap and not the fill
    // rect, do it also into the backbuffer
    SDL_FillRect(_p_ScreenBackbuffer, &_p_ScreenBackbuffer->clip_rect,
                 SDL_MapRGBA(_p_ScreenBackbuffer->format, 0, 0, 0, 0));
    SDL_BlitSurface(_p_Screen, NULL, _p_ScreenBackbuffer, NULL);

    updateTextureAsFlipScreen();
}

LPErrInApp SolitarioGfx::DrawInitialScene() {
    TRACE("DrawInitialScene\n");
    SDL_FillRect(_p_Screen, &_p_Screen->clip_rect,
                 SDL_MapRGBA(_p_Screen->format, 0, 0, 0, 0));
    SDL_Rect rctTarget;
    rctTarget.x = (_p_Screen->w - _p_SceneBackground->w) / 2;
    rctTarget.y = (_p_Screen->h - _p_SceneBackground->h) / 2;
    rctTarget.w = _p_SceneBackground->w;
    rctTarget.h = _p_SceneBackground->h;

    fade(_p_Screen, _p_SceneBackground, 2, 0, _p_sdlRenderer, &rctTarget);
    SDL_Rect rctBt1;
    // button Quit
    ClickCb cbBtQuit = prepClickQuitCb();
    _p_BtQuit = new cButtonGfx;
    rctBt1.w = 120;
    rctBt1.h = 28;
    rctBt1.y = _p_Screen->h - 70;
    rctBt1.x = 30;
    _p_BtQuit->Initialize(&rctBt1, _p_Screen, _p_FontText, MYIDQUIT,
                          _p_sdlRenderer, cbBtQuit);
    _p_BtQuit->SetState(cButtonGfx::INVISIBLE);

    // button new game
    ClickCb cbBtNewGame = prepClickNewGameCb();
    _p_BtNewGame = new cButtonGfx;
    rctBt1.x = rctBt1.x + rctBt1.w + 30;
    _p_BtNewGame->Initialize(&rctBt1, _p_Screen, _p_FontText, MYIDNEWGAME,
                             _p_sdlRenderer, cbBtNewGame);
    _p_BtNewGame->SetState(cButtonGfx::INVISIBLE);
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
    TRACE_DEBUG("Suit %d, card: %d\n", iSegnoIx, iCartaIx);

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
    TRACE_DEBUG("Suit %d, card: %d\n", iSegnoIx, iCartaIx);

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
        return ERR_UTIL::ErrorCreate("Error in draw card, surface is NULL\n");
    }
    // TRACE("Draw card ix = %d, suit = %s, rank %d, x,y %d,%d", pCard->Index(),
    //       pCard->SuitStr(), pCard->Rank(), pCard->X(), pCard->Y());

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
        return ERR_UTIL::ErrorCreate("SDL_BlitSurface in draw card error: %s\n",
                                     SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawCardPac(LPCardGfx pCard, SDL_Surface *s) {
    pCard->SetDeckSurface(_p_Deck);
    pCard->SetWidth(g_CardWidth);
    pCard->SetHeight(g_CardHeight);

    return pCard->DrawCardPac(s);
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
    VCT_STRING vct_Strings;
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
    Uint16 w, h;
    GFX_UTIL::LoadCardPac(&_p_Deck, _DeckType, &w, &h);
    TRACE("Pac size  w = %d, h = %d\n", w, h);
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

LPErrInApp SolitarioGfx::newGame() {
    TRACE("New Game\n");
    LPErrInApp err;
    SetSymbol(DeckPile_Ix, CRD_OSYMBOL);
    CleanUpRegion();

    err = NewDeck(DeckPile_Ix);
    if (err != NULL) {
        return err;
    }
    Shuffle(DeckPile_Ix);

    // deal
    int i;
    for (i = Found_Ix1; i <= Found_Ix7; i++) {
        LPCardStackGfx pStack = PopStackFromRegion(DeckPile_Ix, i);
        PushStackInRegion(i, pStack);
        delete pStack;
    }

    InitAllCoords();

    for (i = Found_Ix1; i <= Found_Ix7; i++) {
        SetCardFaceUp(i, true, RegionSize(i) - 1);
    }
    return NULL;
}

LPErrInApp SolitarioGfx::handleGameLoopKeyDownEvent(SDL_Event &event) {
    LPErrInApp err;
    if (event.key.keysym.sym == SDLK_n) {
        err = newGame();
        if (err != NULL) {
            return err;
        }
        DrawStaticScene();
    }
    if (event.key.keysym.sym == SDLK_a) {
        err = VictoryAnimation();
        if (err != NULL) {
            return err;
        }
        DrawStaticScene();
    }
    if (event.key.keysym.sym == SDLK_r) {
        DrawStaticScene();
    }
    return NULL;
}

LPErrInApp SolitarioGfx::handleGameLoopMouseDownEvent(SDL_Event &event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        return handleLeftMouseDown(event);
    } else if (event.button.button == SDL_BUTTON_RIGHT) {
        return handleRightMouseDown(event);
    }
    return NULL;
}

LPErrInApp SolitarioGfx::handleLeftMouseDown(SDL_Event &event) {
    LPErrInApp err;
    CardRegionGfx *srcReg;
    bool isInitDrag = false;
    srcReg = SelectRegionOnPoint(event.button.x, event.button.y);
    if (srcReg == NULL)
        return NULL;
    if ((srcReg->Id == CRD_FOUNDATION) &&
        srcReg->PtOnTop(event.button.x, event.button.y)) {
        srcReg->SetCardFaceUp(true, srcReg->Size() - 1);
    }

    if ((srcReg->Id == CRD_FOUNDATION) || (srcReg->Id == CRD_DECK_FACEUP) ||
        (srcReg->Id == CRD_ACE)) {
        // clicked on region that can do dragging
        err = InitDrag(event.button.x, event.button.y, isInitDrag);
        if (err != NULL) {
            return err;
        }
        if (isInitDrag) {
            _bStartdrag = true;
            SDL_ShowCursor(SDL_DISABLE);
            SDL_SetWindowGrab(_p_Window, SDL_TRUE);
        }
    } else if (srcReg->Id == CRD_DECKPILE) {
        if (srcReg->IsEmpty() && !IsRegionEmpty(DeckFaceUp)) {
            // Bring back the cards on the deck
            LPCardStackGfx pCardStack =
                PopStackFromRegion(DeckFaceUp, RegionSize(DeckFaceUp));
            pCardStack->SetCardsFaceUp(false);
            err = InitDrag(pCardStack, -1, -1, isInitDrag);
            if (err != NULL) {
                return err;
            }
            DoDrop(GetRegion(DeckPile_Ix));
            Reverse(DeckPile_Ix);
            InitCardCoords(DeckPile_Ix);
            delete pCardStack;
        } else if (!srcReg->IsEmpty()) {
            // the next card goes to the deck face up region
            LPCardStackGfx pCardStack = PopStackFromRegion(DeckPile_Ix, 1);
            pCardStack->SetCardsFaceUp(true);
            err = InitDrag(pCardStack, -1, -1, isInitDrag);
            if (err != NULL) {
                return err;
            }
            DoDrop(GetRegion(DeckFaceUp));
            delete pCardStack;
        } else {
            TRACE("No more card on the pile deck\n");
        }
    }
    return NULL;
}

LPErrInApp SolitarioGfx::handleRightMouseDown(SDL_Event &event) {
    LPErrInApp err;
    CardRegionGfx *srcReg;
    bool isInitDrag = false;
    srcReg = SelectRegionOnPoint(event.button.x, event.button.y);
    if (srcReg == NULL)
        return NULL;
    LPCardGfx pCard = srcReg->GetCard(srcReg->Size() - 1);

    if (((srcReg->Id == CRD_FOUNDATION) || (srcReg->Id == CRD_DECK_FACEUP)) &&
        pCard->IsFaceUp() && srcReg->PtOnTop(event.button.x, event.button.y)) {
        LPCardRegionGfx pDropRegion = FindDropRegion(CRD_ACE, pCard);
        if (pDropRegion != NULL) {
            LPCardStackGfx pCardStack = srcReg->PopStack(1);
            err = InitDrag(pCardStack, -1, -1, isInitDrag);
            if (err != NULL) {
                return err;
            }
            DoDrop(pDropRegion);
            delete pCardStack;
        }
    }
    return NULL;
}

void SolitarioGfx::handleGameLoopMouseMoveEvent(SDL_Event &event) {
    if (event.motion.state == SDL_BUTTON(1) && _bStartdrag) {
        DoDrag(event.motion.x, event.motion.y);
    }
}

LPErrInApp SolitarioGfx::handleGameLoopMouseUpEvent(SDL_Event &event) {
    _p_BtQuit->MouseUp(event);
    _p_BtNewGame->MouseUp(event);

    if (_bStartdrag) {
        _bStartdrag = false;
        DoDrop();
        SDL_ShowCursor(SDL_ENABLE);
        SDL_SetWindowGrab(_p_Window, SDL_FALSE);
    }
    if (IsRegionEmpty(DeckPile_Ix) && IsRegionEmpty(DeckFaceUp)) {
        SetSymbol(DeckPile_Ix, CRD_XSYMBOL);
        DrawStaticScene();
    }
    // victory
    LPErrInApp err;
    if ((Size(Ace_Ix1) == 10) && (Size(Ace_Ix2) == 10) &&
        (Size(Ace_Ix3) == 10) && (Size(Ace_Ix4) == 10)) {
        VictoryAnimation();
        err = newGame();
        if (err != NULL) {
            return err;
        }
        DrawStaticScene();
    }

    return NULL;
}

LPErrInApp SolitarioGfx::StartGameLoop() {
    // button Quit
    STRING strTextBt;
    strTextBt = _p_Languages->GetStringId(Languages::ID_EXIT);
    _p_BtQuit->SetWindowText(strTextBt.c_str());
    _p_BtQuit->SetState(cButtonGfx::VISIBLE);
    // button New Game
    strTextBt = _p_Languages->GetStringId(Languages::ID_START);
    _p_BtNewGame->SetWindowText(strTextBt.c_str());
    _p_BtNewGame->SetState(cButtonGfx::VISIBLE);

    // index 0 (deck with face down)
    CreateRegion(CRD_DECKPILE,          // ID
                 CRD_VISIBLE | CRD_3D,  // attributes
                 CRD_DONOTHING,         // Accept mode
                 CRD_DONOTHING,         // drag mode
                 CRD_OSYMBOL,           // symbol
                 35, 10, 2, 2);         // x, y, x offset, yoffset
    // index 1-7
    int i;
    for (i = 1; i <= 7; i++) {
        CreateRegion(CRD_FOUNDATION,                         // ID
                     CRD_VISIBLE | CRD_DODRAG | CRD_DODROP,  // attributes
                     CRD_DOOPCOLOR | CRD_DOLOWER | CRD_DOLOWERBY1 |
                         CRD_DOKING,  // accept mode
                     CRD_DRAGFACEUP,  // drag mode
                     CRD_HSYMBOL,     // symbol
                     (g_CardWidth * (i - 1)) + (i * 17), g_CardHeight + 40, 0,
                     32);  // x, y, x offset, yoffset
    }

    // index 8 (deck face up)
    CreateRegion(CRD_DECK_FACEUP,                                 // ID
                 CRD_VISIBLE | CRD_FACEUP | CRD_DODRAG | CRD_3D,  // Attributes
                 CRD_DOALL,                                       // accept mode
                 CRD_DRAGTOP,                                     // drag mode
                 CRD_NSYMBOL,                                     // symbol
                 g_CardWidth + 65, 10, 0, 0);  // x, y, x offset, yoffset

    // index 9-12 (4 aces place on the top)
    for (i = 4; i <= 7; i++) {
        CreateRegion(
            CRD_ACE,                                         // ID
            CRD_VISIBLE | CRD_3D | CRD_DODRAG | CRD_DODROP,  // Attributes
            CRD_DOSINGLE | CRD_DOHIGHER | CRD_DOHIGHERBY1 | CRD_DOACE |
                CRD_DOSUIT,  // Accept mode
            CRD_DRAGTOP,     // drop mode
            CRD_HSYMBOL,     // symbol
            (g_CardWidth * (i - 1)) + (i * 17), 10, 0,
            0);  // x, y, x offset, yoffset
    }

    LPErrInApp err = newGame();
    if (err != NULL)
        return err;
    DrawStaticScene();

    SDL_Event event;
    _terminated = false;
    while (!_terminated) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    fade(_p_Screen, _p_Screen, 1, 1, _p_sdlRenderer, NULL);
                    return NULL;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        return NULL;
                    }
                    err = handleGameLoopKeyDownEvent(event);
                    if (err != NULL)
                        return err;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    err = handleGameLoopMouseDownEvent(event);
                    if (err != NULL)
                        return err;
                    break;

                case SDL_MOUSEMOTION:
                    handleGameLoopMouseMoveEvent(event);
                    break;

                case SDL_MOUSEBUTTONUP:
                    err = handleGameLoopMouseUpEvent(event);
                    if (err != NULL)
                        return err;
                    break;
            }
        }
    }
    return NULL;
}

void SolitarioGfx::BtQuitClick() {
    TRACE("Quit with user button\n");
    _terminated = true;
}

void SolitarioGfx::BtNewGameClick() { TRACE("New Game with user button\n"); }
