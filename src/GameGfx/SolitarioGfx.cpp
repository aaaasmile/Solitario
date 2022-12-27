#include "SolitarioGfx.h"

#include <SDL_endian.h>
#include <SDL_image.h>
#include <stdio.h>

#include "Fading.h"
#include "GfxUtil.h"
#include "win_type_global.h"

// card graphics
unsigned int g_CARDWIDTH = 0;
unsigned int g_CARDHEIGHT = 0;
unsigned int g_SYMBOLWIDTH = 84;
unsigned int g_SYMBOLHEIGHT = 144;

int const CARDBACKLOC = 40 * g_CARDWIDTH;  // modify to allow custom card backs

#define FPS 30
#define FRAMETICKS (1000 / FPS)
#define THINKINGS_PER_TICK 1

static const char *lpszBackgroundImgFile = DATA_PREFIX "images/im001537.jpg";
static const char *lpszDeckDir = DATA_PREFIX "mazzi/";
static const char *lpszSymbDir = DATA_PREFIX "images/";

SolitarioGfx::SolitarioGfx() {
    _p_background = 0;
    _p_scene_background = 0;
    _p_ScreenTexture = 0;
    for (int i = 0; i < NUM_CARDS_ONDECK; i++) {
        _p_CardsSurf[i] = 0;
    }
}

SolitarioGfx::~SolitarioGfx() { ClearSurface(); }

LPErrInApp SolitarioGfx::Initialize(SDL_Surface *s, SDL_Renderer *r) {
    TRACE("Initialize Solitario");
    LPErrInApp err;
    _p_screen = s;
    _p_sdlRenderer = r;
    _p_ScreenTexture = SDL_CreateTexture(
        r, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, s->w, s->h);
    if (_p_ScreenTexture == NULL) {
        return ERR_UTIL::ErrorCreate("Cannot create texture: %s\n",
                                     SDL_GetError());
    }

    _p_background = SDL_CreateRGBSurface(SDL_SWSURFACE, _p_screen->w,
                                         _p_screen->h, 32, 0, 0, 0, 0);
    SDL_RWops *srcBack = SDL_RWFromFile(lpszBackgroundImgFile, "rb");
    if (srcBack == 0) {
        return ERR_UTIL::ErrorCreate("Error in SDL_RWFromFile: %s\n",
                                     SDL_GetError());
    }
    _p_scene_background = IMG_LoadJPG_RW(srcBack);
    if (_p_scene_background == 0) {
        return ERR_UTIL::ErrorCreate("Cannot create scene background: %s\n",
                                     SDL_GetError());
    }

    err = LoadCardPac();
    if (err != NULL) {
        return err;
    }
    err = LoadSymbolsForPac();
    if (err != NULL) {
        return err;
    }

    DrawBackground(TRUE);
    return 0;
}

LPErrInApp SolitarioGfx::DrawCardStack(rVI vi) {
    CardRegionGfx *cardRegion = &(*vi);
    return DrawCardStack(_p_screen, cardRegion);
}

LPErrInApp SolitarioGfx::DrawCardStack(SDL_Surface *s,
                                       CardRegionGfx *pcardRegion) {
    LPErrInApp err;
    if (!(pcardRegion->Attributes & CRD_VISIBLE))
        return NULL;

    DrawSymbol(pcardRegion->XCoord, pcardRegion->YCoord, pcardRegion->Symbol);
    for (VI vi = pcardRegion->InternalStack.begin();
         vi != pcardRegion->InternalStack.end(); ++vi) {
        if (vi->FaceUp()) {
            err = DrawCard(vi, s);
        } else {
            err = DrawCardBack(vi->x, vi->y, s);
        }
        if (err != NULL) {
            return err;
        }
    }
    return NULL;
}

void SolitarioGfx::ClearSurface() {
    if (_p_background)
        SDL_FreeSurface(_p_background);
    if (_p_scene_background)
        SDL_FreeSurface(_p_scene_background);

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

bool SolitarioGfx::DeleteRegion(CardRegionGfx *pRegion) {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        if (&(*vi) == pRegion) {
            _cardRegionList.erase(vi);
            return true;
        }
    }
    return false;
}

void SolitarioGfx::EmptyStacks() {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi)
        vi->Clear();
}

void SolitarioGfx::InitAllCoords() {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        vi->InitCardCoords();
        vi->InitCardFaces();
    }
}

CardRegionGfx *SolitarioGfx::OnMouseDown(int x, int y) {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        if (vi->PtInStack(x, y)) {
            _p_sourceRegion = &(*vi);
            return _p_sourceRegion;
        }
    }
    return NULL;
}

LPErrInApp SolitarioGfx::InitDrag(int x, int y, bool &isInitDrag) {
    return InitDrag(NULL, x, y, isInitDrag);
}

LPErrInApp SolitarioGfx::InitDrag(CardStackGfx *CargoStack, int x, int y,
                                  bool &isInitDrag) {
    isInitDrag = false;
    LPErrInApp err;
    if (CargoStack == NULL) {
        if (_p_sourceRegion->Empty())
            return NULL;

        int idx = _p_sourceRegion->GetClickedCard(x, y);
        if (idx == -1) {
            return NULL;  // no card found
        }

        switch (_p_sourceRegion->GetDragMode()) {
            case CRD_DRAGCARDS:
                _dragStack.Push(_p_sourceRegion->RemoveCard(idx));
                break;

            case CRD_DRAGTOP:
                if (_p_sourceRegion->Size() - 1 == idx)
                    _dragStack.Push(_p_sourceRegion->Pop());
                else
                    return NULL;
                break;

            case CRD_DRAGFACEUP:
                if (_p_sourceRegion->CardFaceUp(idx))
                    _dragStack.Push(
                        _p_sourceRegion->Pop(_p_sourceRegion->Size() - idx));
                else
                    return NULL;
                break;

            default:
                _dragStack.Push(
                    _p_sourceRegion->Pop(_p_sourceRegion->Size() - idx));
        }
    } else
        _dragStack.Push(*CargoStack);

    _p_sourceRegion->InitCardCoords();

    err = DrawBackground(FALSE);
    if (err != NULL) {
        return err;
    }
    SDL_BlitSurface(_p_screen, NULL, _p_background, NULL);

    CardRegionGfx DragRegion(0, _p_sourceRegion->GetAttributes() | CRD_FACEUP,
                             0, 0, 0, 0, 0, _p_sourceRegion->GetxOffset(),
                             _p_sourceRegion->GetyOffset());
    DragRegion.Push(_dragStack);

    _dragCard.x = _dragStack[0].x;
    _dragCard.y = _dragStack[0].y;
    _dragCard.width = DragRegion.GetStackWidth();
    _dragCard.height = DragRegion.GetStackHeight();

    if (_p_dragface != NULL) {
        SDL_FreeSurface(_p_dragface);
    }

    _p_dragface = SDL_CreateRGBSurface(SDL_SWSURFACE, _dragCard.width,
                                       _dragCard.height, 32, 0, 0, 0, 0);
    SDL_FillRect(_p_dragface, NULL, SDL_MapRGB(_p_dragface->format, 0, 255, 0));
    SDL_SetColorKey(_p_dragface, TRUE,
                    SDL_MapRGB(_p_dragface->format, 0, 255, 0));

    err = DrawCardStack(_p_screen, &DragRegion);
    if (err != NULL) {
        return err;
    }
    DragRegion.InitCardCoords();
    err = DrawCardStack(_p_dragface, &DragRegion);
    if (err != NULL) {
        return err;
    }

    _oldx = x;
    _oldy = y;

    UpdateTextureAsFlipScreen();
    isInitDrag = true;
    return NULL;
}

void SolitarioGfx::UpdateTextureAsFlipScreen() {
    SDL_UpdateTexture(_p_ScreenTexture, NULL, _p_screen->pixels,
                      _p_screen->pitch);
    SDL_RenderCopy(_p_sdlRenderer, _p_ScreenTexture, NULL, NULL);
    SDL_RenderPresent(_p_sdlRenderer);
}

void SolitarioGfx::DoDrag(int x, int y) {
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

    SDL_BlitSurface(_p_background, &rcs, _p_screen, &rcd);
    SDL_BlitSurface(_p_dragface, NULL, _p_screen, &dest);
    UpdateTextureAsFlipScreen();
}

void SolitarioGfx::DoDrop() { DoDrop(NULL); }

void SolitarioGfx::DoDrop(CardRegionGfx *DestRegion) {
    CardStackGfx *DestStack;
    CardRegionGfx *BestRegion;

    if (DestRegion != NULL)
        BestRegion = DestRegion;
    else
        BestRegion = GetBestStack(_dragCard.x, _dragCard.y, g_CARDWIDTH,
                                  g_CARDHEIGHT, &_dragStack);
    if (BestRegion == NULL)
        BestRegion = _p_sourceRegion;

    DestStack = BestRegion->GetCardStack();
    DestStack->Push(_dragStack);
    BestRegion->InitCardCoords();

    VI vi;
    switch (_p_sourceRegion->GetDragMode()) {
        case 2:
        case 3:
            vi = DestStack->end() - 1;
            break;
        default:  // 1 and 4
            vi = DestStack->end() - _dragStack.Size();
    }

    _dragStack.Clear();

    if (_dragCard.x == vi->x && _dragCard.y == vi->y)
        return;  // when no movement

    ZoomCard(_dragCard.x, _dragCard.y, vi->x, vi->y, _dragCard.width,
             _dragCard.height, _p_background, _p_dragface);

    SDL_FreeSurface(_p_dragface);
    _p_dragface = NULL;
}

void calcPt(int x0, int y0, int x1, int y1, float t, int &X, int &Y) {
    X = int(x0 + t * (x1 - x0) + .5);
    Y = int(y0 + t * (y1 - y0) + .5);
}

void SolitarioGfx::ZoomCard(int &sx, int &sy, int &dx, int &dy, int w, int h,
                            SDL_Surface *bg, SDL_Surface *fg) {
    SDL_Rect rcs;
    SDL_Rect rcd;
    SDL_Rect dest;

    int px, py;
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

        SDL_BlitSurface(bg, &rcs, _p_screen, &rcd);
        SDL_BlitSurface(fg, NULL, _p_screen, &dest);

        UpdateTextureAsFlipScreen();
    }
    DrawStaticScene();
}

CardRegionGfx *SolitarioGfx::FindDropRegion(int Id, CardGfx card) {
    CardStackGfx stack;
    stack.Push(card);
    return FindDropRegion(Id, stack);
}

CardRegionGfx *SolitarioGfx::FindDropRegion(int Id, CardStackGfx stack) {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        if ((vi->Id == Id) && vi->CanDrop(&stack))
            return &(*vi);
    }
    return NULL;
}

void SolitarioGfx::DrawStaticScene() {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        SDL_PumpEvents();
        DrawCardStack(vi);
    }
    UpdateTextureAsFlipScreen();
}

LPErrInApp SolitarioGfx::DrawBackground(BOOL bIsInit) {
    LPErrInApp err;
    if (_p_scene_background) {
        if (!bIsInit) {
            SDL_BlitSurface(_p_scene_background, NULL, _p_screen, NULL);
        } else {
            fade(_p_screen, _p_scene_background, 2, 0, _p_sdlRenderer);
        }
    }

    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        err = DrawCardStack(vi);
        if (err != NULL) {
            return NULL;
        }
    }
    return NULL;
}

CardRegionGfx *SolitarioGfx::GetBestStack(int x, int y, int w, int h,
                                          CardStackGfx *stack) {
    int maxoverlap = 0;
    int percent = 0;
    CardRegionGfx *best = 0;

    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        SDL_PumpEvents();
        if (vi->CanDrop(stack))
            percent = vi->GetOverlapRatio(x, y, w, h);

        if (percent > maxoverlap) {
            maxoverlap = percent;
            best = &(*vi);
        }
    }
    return best;
}

LPErrInApp SolitarioGfx::DrawCard(int x, int y, int nCdIndex) {
    return DrawCard(x, y, nCdIndex, _p_screen);
}

LPErrInApp SolitarioGfx::DrawCard(int x, int y, int nCdIndex, SDL_Surface *s) {
    if (s == NULL) {
        return ERR_UTIL::ErrorCreate(
            "Draw a card on NULL surface. This is wrong");
    }
    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    int iSegnoIx = nCdIndex / 10;
    int iCartaIx = nCdIndex % 10;
    TRACE("Segno %d, carta: %d", iSegnoIx, iCartaIx);

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_CardsSurf[nCdIndex]->clip_rect.w;  // TODO use a funtion
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

LPErrInApp SolitarioGfx::DrawCard(VI vi, SDL_Surface *s) {
    if (s == NULL) {
        return ERR_UTIL::ErrorCreate(
            "Error in draw card with Iterator, surface is NULL\n");
    }
    if (_DeckType.IsPacType()) {
        return DrawCardPac(vi, s);
    }
    int nCdIndex = vi->Idx;

    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_CardsSurf[nCdIndex]->clip_rect.w;
    _rctSrcCard.h = _p_CardsSurf[nCdIndex]->clip_rect.h;

    SDL_Rect dest;
    dest.x = vi->x;
    dest.y = vi->y;

    TRACE("Draw card ix = %d, segno = %d, valore %d \n", vi->Idx, vi->Suit(),
          vi->Rank());
    if (SDL_BlitSurface(_p_CardsSurf[nCdIndex], &_rctSrcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in draw card with Iterator error: %s\n",
            SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawCardPac(VI vi, SDL_Surface *s) {
    int nCdIndex = vi->Idx;

    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    int iSegnoIx = nCdIndex / 10;
    int iCartaIx = nCdIndex % 10;
    SDL_Rect SrcCard;

    SrcCard.x = iSegnoIx * g_CARDWIDTH;
    SrcCard.y = iCartaIx * g_CARDHEIGHT;
    SrcCard.w = g_CARDWIDTH;
    SrcCard.h = g_CARDHEIGHT;

    SDL_Rect dest;
    dest.x = vi->x;
    dest.y = vi->y;

    TRACE("Draw card ix = %d, segno = %d, valore %d \n", vi->Idx, vi->Suit(),
          vi->Rank());

    if (SDL_BlitSurface(_p_srfDeck, &SrcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawCardPac with Iterator error: %s\n",
            SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawCardBack(int x, int y) {
    return DrawCardBack(x, y, _p_screen);
}

LPErrInApp SolitarioGfx::DrawCardBack(int x, int y, SDL_Surface *s) {
    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_Symbol[0]->clip_rect.w;  // TODO use a funtion
    _rctSrcCard.h = _p_Symbol[0]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (SDL_BlitSurface(_p_Symbol[0], &_rctSrcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawCardBack error: %s\n", SDL_GetError());
    }
    return NULL;
}

LPErrInApp SolitarioGfx::DrawSymbol(int x, int y, int nSymbol) {
    return DrawSymbol(x, y, nSymbol, _p_screen);
}

LPErrInApp SolitarioGfx::DrawSymbol(int x, int y, int nSymbol, SDL_Surface *s) {
    if (nSymbol < 1) {
        return ERR_UTIL::ErrorCreate("Symbol index %d out of range", nSymbol);
    }
    if (nSymbol > 3)
        nSymbol = 3;

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_Symbol[nSymbol]->clip_rect.w;  // TODO use a funtion
    _rctSrcCard.h = _p_Symbol[nSymbol]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (SDL_BlitSurface(_p_Symbol[nSymbol], &_rctSrcCard, s, &dest) == -1) {
        return ERR_UTIL::ErrorCreate(
            "SDL_BlitSurface in DrawSymbol error: %s\n", SDL_GetError());
    }
    return NULL;
}

int SolitarioGfx::AnimateCards() {
    srand((unsigned)time(NULL));

    int rot;
    int id;
    int x;
    unsigned int y;
    int xspeed;
    int yspeed;

    int GRAVITY = 1;
    unsigned int MAXY = _p_screen->h;
    float BOUNCE = 0.8f;

    do {
        rot = rand() % 2;
        id = rand() % 51;
        x = rand() % _p_screen->w;
        y = rand() % _p_screen->h / 2;

        if (rot)
            xspeed = -4;
        else
            xspeed = 4;

        yspeed = 0;

        do  // while card is within the _p_screen
        {
            SDL_PumpEvents();
            if (SDL_GetMouseState(NULL, NULL))
                return 1;  // stop the animation

            yspeed = yspeed + GRAVITY;
            x += xspeed;
            y += yspeed;

            if (y + g_CARDHEIGHT > MAXY) {
                y = MAXY - g_CARDHEIGHT;
                yspeed = int(-yspeed * BOUNCE);
            }

            DrawCard(x, y, id, _p_screen);
            UpdateTextureAsFlipScreen();
        }
        // 73 here is CARDWIDTH, but when using CARDWIDTH, it doesn't
        // work
        while ((x + 73 > 0) && (x < _p_screen->w));
        //		while((x + CARDWIDTH > 0) && (x < _p_screen->w));
    } while (1);  // or while within specified time

    return 0;
}

LPErrInApp SolitarioGfx::LoadDeckFromSingleFile() {
    std::string strTmp;
    std::string strSuffix;
    CHAR buff[128];

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
    g_CARDWIDTH = _p_CardsSurf[0]->clip_rect.w;
    g_CARDHEIGHT = _p_CardsSurf[0]->clip_rect.h;

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
        _p_Symbol[i] = IMG_LoadJPG_RW(srcBack);
        if (_p_Symbol[i] == NULL) {
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

    _p_srfDeck = IMG_LoadPNG_RW(src);
    if (!_p_srfDeck) {
        return ERR_UTIL::ErrorCreate(
            "IMG_LoadPNG_RW on pac file error (file %s): %s\n",
            strFileName.c_str(), SDL_GetError());
    }

    SDL_SetColorKey(_p_srfDeck, TRUE,
                    SDL_MapRGB(_p_srfDeck->format, 0, 128, 0));

    g_CARDWIDTH = w / 4;
    g_CARDHEIGHT = h / 10;

    return NULL;
}

LPErrInApp SolitarioGfx::LoadSymbolsForPac() {
    std::string strFileSymbName = lpszSymbDir;
    strFileSymbName += _DeckType.GetSymbolFileName();

    _p_symbols = SDL_LoadBMP(strFileSymbName.c_str());
    if (_p_symbols == 0) {
        return ERR_UTIL::ErrorCreate("LOad bitmap failed: %s\n",
                                     SDL_GetError());
    }

    if (_DeckType.GetSymbolFileName() == "symb_336.bmp") {
        SDL_SetColorKey(_p_symbols, TRUE,
                        SDL_MapRGB(_p_symbols->format, 242, 30, 206));
    } else {
        SDL_SetColorKey(_p_symbols, TRUE,
                        SDL_MapRGB(_p_symbols->format, 0, 128, 0));
    }

    g_SYMBOLWIDTH = _p_symbols->w / 4;
    g_SYMBOLHEIGHT = _p_symbols->h;

    return NULL;
}
