#include "CGame.h"

#include <SDL_endian.h>
#include <SDL_image.h>
#include <stdio.h>

#include "fading.h"
#include "gfx_util.h"
#include "win_type_global.h"

typedef std::vector<std::string> VCT_STRINGS;

// card graphics
unsigned int g_CARDWIDTH = 0;
unsigned int g_CARDHEIGHT = 0;
unsigned int g_SYMBOLWIDTH = 84;
unsigned int g_SYMBOLHEIGHT = 144;

int const CARDBACKLOC = 40 * g_CARDWIDTH;  // modify to allow custom card backs

#define FPS 30
#define FRAMETICKS (1000 / FPS)
#define THINKINGS_PER_TICK 1

static const char *lpszBackgroundImgFile = DATA_PREFIX "im001537.jpg";
static const char *lpszDeckDir = DATA_PREFIX "mazzo/";
static const char *lpszDataDir = DATA_PREFIX;

CGame::CGame() {
    _p_background = 0;
    _p_scene_background = 0;
    _p_ScreenTexture = 0;
    for (int i = 0; i < NUM_CARDS_ONDECK; i++) {
        _p_CardsSurf[i] = 0;
    }
}

CGame::~CGame() { ClearSurface(); }

int CGame::Initialize(SDL_Surface *s, SDL_Renderer *r) {
    int errno;
    _p_screen = s;
    _p_sdlRenderer = r;
    _p_ScreenTexture = SDL_CreateTexture(
        r, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, s->w, s->h);

    _p_background = SDL_CreateRGBSurface(SDL_SWSURFACE, _p_screen->w,
                                         _p_screen->h, 32, 0, 0, 0, 0);
    SDL_RWops *srcBack = SDL_RWFromFile(lpszBackgroundImgFile, "rb");
    if (srcBack == 0) {
        // TODO error handling
    }
    _p_scene_background = IMG_LoadJPG_RW(srcBack);

    errno = LoadCardPac();
    if (errno < 0) {
        return errno;
    }
    errno = LoadSymbolsForPac();
    if (errno < 0) {
        return errno;
    }

    DrawBackground(TRUE);
    return 0;
}

void CGame::DrawCardStack(rVI vi) {
    CCardRegion *cardRegion = &(*vi);
    DrawCardStack(NULL, cardRegion);
}

void CGame::DrawCardStack(SDL_Surface *s, CCardRegion *pcardRegion) {
    if (!(pcardRegion->Attributes & CRD_VISIBLE))
        return;

    DrawSymbol(pcardRegion->XCoord, pcardRegion->YCoord, pcardRegion->Symbol);
    for (VI vi = pcardRegion->InternalStack.begin();
         vi != pcardRegion->InternalStack.end(); ++vi) {
        if (vi->FaceUp()) {
            DrawCard(vi, s);
        } else {
            DrawCardBack(vi->x, vi->y, s);
        }
    }
}

void CGame::ClearSurface() {
    if (_p_background)
        SDL_FreeSurface(_p_background);
    if (_p_scene_background)
        SDL_FreeSurface(_p_scene_background);

    if (_p_ScreenTexture != NULL) {
        SDL_DestroyTexture(_p_ScreenTexture);
    }
}

void CGame::CreateRegion(int id, unsigned int attribs, unsigned int amode,
                         int dmode, int symbol, int x, int y, int xoffset,
                         int yoffset) {
    CCardRegion *cr = new CCardRegion(id, attribs, amode, dmode, symbol, x, y,
                                      xoffset, yoffset);
    _cardRegionList.push_back(*cr);
}

bool CGame::DeleteRegion(CCardRegion *pRegion) {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        if (&(*vi) == pRegion) {
            _cardRegionList.erase(vi);
            return true;
        }
    }

    return false;
}

void CGame::EmptyStacks() {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi)
        vi->Clear();
}

void CGame::InitAllCoords() {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        vi->InitCardCoords();
        vi->InitCardFaces();
    }
}

CCardRegion *CGame::OnMouseDown(int x, int y) {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        if (vi->PtInStack(x, y)) {
            _p_sourceRegion = &(*vi);
            return _p_sourceRegion;
        }
    }
    return NULL;
}

bool CGame::InitDrag(int x, int y) { return InitDrag(NULL, x, y); }

bool CGame::InitDrag(CCardStack *CargoStack, int x, int y) {
    if (CargoStack == NULL) {
        if (_p_sourceRegion->Empty())
            return false;

        int idx = _p_sourceRegion->GetClickedCard(x, y);
        if (idx == -1)
            return false;  // no card found

        switch (_p_sourceRegion->GetDragMode()) {
            case CRD_DRAGCARDS:
                _dragStack.Push(_p_sourceRegion->RemoveCard(idx));
                break;

            case CRD_DRAGTOP:
                if (_p_sourceRegion->Size() - 1 == idx)
                    _dragStack.Push(_p_sourceRegion->Pop());
                else
                    return false;
                break;

            case CRD_DRAGFACEUP:
                if (_p_sourceRegion->CardFaceUp(idx))
                    _dragStack.Push(
                        _p_sourceRegion->Pop(_p_sourceRegion->Size() - idx));
                else
                    return false;
                break;

            default:
                _dragStack.Push(
                    _p_sourceRegion->Pop(_p_sourceRegion->Size() - idx));
        }
    } else
        _dragStack.Push(*CargoStack);

    _p_sourceRegion->InitCardCoords();

    DrawBackground(FALSE);
    SDL_BlitSurface(_p_screen, NULL, _p_background, NULL);

    CCardRegion DragRegion(0, _p_sourceRegion->GetAttributes() | CRD_FACEUP, 0,
                           0, 0, 0, 0, _p_sourceRegion->GetxOffset(),
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

    DrawCardStack(_p_screen, &DragRegion);
    DragRegion.InitCardCoords();
    DrawCardStack(_p_dragface, &DragRegion);

    _oldx = x;
    _oldy = y;

    UpdateTextureAsFlipScreen();

    return true;
}

void CGame::UpdateTextureAsFlipScreen() {
    SDL_UpdateTexture(_p_ScreenTexture, NULL, _p_screen->pixels,
                      _p_screen->pitch);
    SDL_RenderCopy(_p_sdlRenderer, _p_ScreenTexture, NULL, NULL);
    SDL_RenderPresent(_p_sdlRenderer);
}

void CGame::DoDrag(int x, int y) {
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

void CGame::DoDrop() { DoDrop(NULL); }

void CGame::DoDrop(CCardRegion *DestRegion) {
    CCardStack *DestStack;
    CCardRegion *BestRegion;

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

void CGame::ZoomCard(int &sx, int &sy, int &dx, int &dy, int w, int h,
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

CCardRegion *CGame::FindDropRegion(int Id, CCard card) {
    CCardStack stack;
    stack.Push(card);
    return FindDropRegion(Id, stack);
}

CCardRegion *CGame::FindDropRegion(int Id, CCardStack stack) {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        if ((vi->Id == Id) && vi->CanDrop(&stack))
            return &(*vi);
    }
    return NULL;
}

void calcPt(int x0, int y0, int x1, int y1, float t, int &X, int &Y) {
    X = int(x0 + t * (x1 - x0) + .5);
    Y = int(y0 + t * (y1 - y0) + .5);
}

void CGame::DrawStaticScene() {
    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi) {
        SDL_PumpEvents();
        DrawCardStack(vi);
    }
    UpdateTextureAsFlipScreen();
}

void CGame::DrawBackground(BOOL bIsInit) {
    if (_p_scene_background) {
        if (!bIsInit) {
            SDL_BlitSurface(_p_scene_background, NULL, _p_screen, NULL);
        } else {
            fade(_p_screen, _p_scene_background, 2, 0, _p_sdlRenderer);
        }
    }

    for (rVI vi = _cardRegionList.begin(); vi != _cardRegionList.end(); ++vi)
        DrawCardStack(vi);
}

CCardRegion *CGame::GetBestStack(int x, int y, int w, int h,
                                 CCardStack *stack) {
    int maxoverlap = 0;
    int percent = 0;
    CCardRegion *best = 0;

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

int CGame::DrawCard(int x, int y, int nCdIndex) {
    s = _p_screen;
    return DrawCard(x, y, nCdIndex, _p_screen);
}

int CGame::DrawCard(int x, int y, int nCdIndex, SDL_Surface *s) {
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

    return SDL_BlitSurface(_p_CardsSurf[nCdIndex], &_rctSrcCard, s, &dest);
}

int CGame::DrawCard(VI vi, SDL_Surface *s) {
    int nCdIndex = vi->Idx;

    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_CardsSurf[nCdIndex]->clip_rect.w;  // TODO use a funtion
    _rctSrcCard.h = _p_CardsSurf[nCdIndex]->clip_rect.h;

    SDL_Rect dest;
    dest.x = vi->x;
    dest.y = vi->y;

    TRACE("Draw card ix = %d, segno = %d, valore %d \n", vi->Idx, vi->Suit(),
          vi->Rank());

    return SDL_BlitSurface(_p_CardsSurf[nCdIndex], &_rctSrcCard, s, &dest);
}

int CGame::DrawCardBack(int x, int y) { return DrawCardBack(x, y, _p_screen); }

int CGame::DrawCardBack(int x, int y, SDL_Surface *s) {
    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_Symbol[0]->clip_rect.w;  // TODO use a funtion
    _rctSrcCard.h = _p_Symbol[0]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    return SDL_BlitSurface(_p_Symbol[0], &_rctSrcCard, s, &dest);
}

int CGame::DrawSymbol(int x, int y, int nSymbol) {
    return DrawSymbol(x, y, nSymbol, _p_screen);
}

int CGame::DrawSymbol(int x, int y, int nSymbol, SDL_Surface *s) {
    if (nSymbol < 1)
        return -1;  // TODO error handling
    if (nSymbol > 3)
        nSymbol = 3;

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_Symbol[nSymbol]->clip_rect.w;  // TODO use a funtion
    _rctSrcCard.h = _p_Symbol[nSymbol]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    return SDL_BlitSurface(_p_Symbol[nSymbol], &_rctSrcCard, s, &dest);
}

int CGame::AnimateCards() {
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
        // 73 here is CARDWIDTH, but when using CARDWIDTH, it doesn't work
        while ((x + 73 > 0) && (x < _p_screen->w));
        //		while((x + CARDWIDTH > 0) && (x < _p_screen->w));
    } while (1);  // or while within specified time

    return 0;
}

void CGame::LoadDeckFromSingleFile() {
    SDL_Surface *Temp;

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

            if (srcBack) {
                _p_CardsSurf[i + k * 10] = IMG_LoadJPG_RW(srcBack);
            } else {
                ASSERT(0);  // TODO use error handling
            }
        }
    }

    if (_p_CardsSurf[0]) {
        g_CARDWIDTH = _p_CardsSurf[0]->clip_rect.w;
        g_CARDHEIGHT = _p_CardsSurf[0]->clip_rect.h;
    }
}

void CGame::LoadSymbolsFromSingleFile() {
    VCT_STRINGS vct_Strings;
    vct_Strings.push_back("dorso.jpg");
    vct_Strings.push_back("fine_1.jpg");
    vct_Strings.push_back("fine_2.jpg");
    vct_Strings.push_back("fine_3.jpg");
    ASSERT(NUM_SYMBOLS == vct_Strings.size());

    SDL_Surface *Temp = 0;
    for (int i = 0; i < NUM_SYMBOLS; i++) {
        std::string strDir = lpszDeckDir;
        std::string strFileSymbName;
        strFileSymbName = lpszDeckDir + vct_Strings[i];
        SDL_RWops *srcBack = SDL_RWFromFile(strFileSymbName.c_str(), "rb");
        if (srcBack) {
            _p_Symbol[i] = IMG_LoadJPG_RW(srcBack);
        } else {
            ASSERT(0);
        }
    }
}

int CGame::LoadCardPac() {
    // TODO Error handling properly
    char describtion[100];
    Uint8 num_anims;
    Uint16 w, h;
    Uint16 frames;
    Uint16 *delays;

    std::string strFileName = lpszDeckDir;
    strFileName += _DeckType.GetResFileName();

    SDL_RWops *src = SDL_RWFromFile(strFileName.c_str(), "rb");
    SDL_RWread(src, describtion, 100, 1);
    SDL_RWread(src, &num_anims, 1, 1);  // TODO Error handling
    w = SDL_ReadLE16(src);              // witdh of the picture (pac of 4 cards)
    h = SDL_ReadLE16(src);  // height of the picture (pac of 10 rows of cards)
    frames = SDL_ReadLE16(src);

    delays = (Uint16 *)malloc(sizeof(Uint16) * frames);
    if (!delays)
        return -1;

    for (int i = 0; i < frames; i++) {
        // file format stores delays in 1/100th of second
        // we will convert them to game ticks
        delays[i] =
            THINKINGS_PER_TICK * ((10 * SDL_ReadLE16(src)) / FRAMETICKS);
    }

    SDL_Surface *s;
    s = IMG_LoadPNG_RW(src);

    SDL_SetColorKey(s, TRUE, SDL_MapRGB(s->format, 0, 128, 0));

    g_CARDWIDTH = w / 4;
    g_CARDHEIGHT = h / 10;
    _p_srfDeck = s;

    free(delays);
    return 0;
}

int CGame::LoadSymbolsForPac() {
    std::string strFileSymbName = lpszDataDir;
    strFileSymbName += _DeckType.GetSymbolFileName();

    _p_symbols = SDL_LoadBMP(strFileSymbName.c_str());

    if (_DeckType.GetSymbolFileName() == "symb_336.bmp") {
        SDL_SetColorKey(_p_symbols, TRUE,
                        SDL_MapRGB(_p_symbols->format, 242, 30, 206));
    } else {
        SDL_SetColorKey(_p_symbols, TRUE,
                        SDL_MapRGB(_p_symbols->format, 0, 128, 0));
    }

    g_SYMBOLWIDTH = _p_symbols->w / 4;
    g_SYMBOLHEIGHT = _p_symbols->h;

    return 0;
}
