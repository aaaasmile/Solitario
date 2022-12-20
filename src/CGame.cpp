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

CGame::CGame() {
    _p_background = 0;
    _p_scene_background = 0;
    for (int i = 0; i < NUM_CARDS_ONDECK; i++) {
        _p_CardsSurf[i] = 0;
    }
}

CGame::~CGame() { ClearSurface(); }

void CGame::Initialize(SDL_Surface *s) {
    _p_screen = s;
    _p_background = SDL_CreateRGBSurface(SDL_SWSURFACE, _p_screen->w,
                                         _p_screen->h, 32, 0, 0, 0, 0);
    SDL_Surface *Temp;
    SDL_RWops *srcBack = SDL_RWFromFile(lpszBackgroundImgFile, "rb");
    Temp = IMG_LoadJPG_RW(srcBack);
    // _p_scene_background = SDL_DisplayFormat(Temp); // TODO SDL 2.0
    SDL_FreeSurface(Temp);

    DrawBackground(TRUE);
}

int CGame::InitDeck(SDL_Surface *s) {
    LoadDeckFromPac();
    LoadSymbols();
    _p_srfDeck = s;
    return 0;
}

void CGame::ClearSurface() {
    if (_p_background)
        SDL_FreeSurface(_p_background);
    if (_p_scene_background)
        SDL_FreeSurface(_p_scene_background);
}

void CGame::CreateRegion(int id, unsigned int attribs, unsigned int amode,
                         int dmode, int symbol, int x, int y, int xoffset,
                         int yoffset) {
    CCardRegion *cr = new CCardRegion(id, attribs, amode, dmode, symbol, x, y,
                                      xoffset, yoffset, this);
    this->push_back(*cr);
}

bool CGame::DeleteRegion(CCardRegion *pRegion) {
    for (rVI vi = this->begin(); vi != this->end(); ++vi) {
        if (&(*vi) == pRegion) {
            this->erase(vi);
            return true;
        }
    }

    return false;
}

void CGame::EmptyStacks() {
    for (rVI vi = this->begin(); vi != this->end(); ++vi) vi->Clear();
}

void CGame::InitAllCoords() {
    for (rVI vi = this->begin(); vi != this->end(); ++vi) {
        vi->InitCardCoords();
        vi->InitCardFaces();
    }
}

//------------------------------------------------------------------------------------------------
// Drag and drop
CCardRegion *CGame::OnMouseDown(int x, int y) {
    for (rVI vi = this->begin(); vi != this->end(); ++vi) {
        if (vi->PtInStack(x, y)) {
            _p_sourceRegion = &(*vi);
            return _p_sourceRegion;
        }
    }
    return NULL;
}

bool CGame::InitDrag(int x, int y) { return InitDrag(NULL, x, y); }

// optimization needed
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
                           _p_sourceRegion->GetyOffset(), this);
    DragRegion.Push(_dragStack);

    _dragCard.x = _dragStack[0].x;
    _dragCard.y = _dragStack[0].y;
    _dragCard.width = DragRegion.GetStackWidth();
    _dragCard.height = DragRegion.GetStackHeight();

    _p_dragface = SDL_CreateRGBSurface(SDL_SWSURFACE, _dragCard.width,
                                       _dragCard.height, 32, 0, 0, 0, 0);
    SDL_FillRect(_p_dragface, NULL, SDL_MapRGB(_p_dragface->format, 0, 255, 0));
    SDL_SetColorKey(_p_dragface, TRUE,
                    SDL_MapRGB(_p_dragface->format, 0, 255, 0));  // SDL 2.0

    DragRegion.DrawCardStack(_p_screen);
    DragRegion.InitCardCoords();
    DragRegion.DrawCardStack(_p_dragface);

    _oldx = x;
    _oldy = y;

    // SDL_Flip(_p_screen); // TODO Sdl 2.0
    return true;
}

// optimization needed
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

    // SDL_Flip(_p_screen); // TODO Sdl 2.0
}

void CGame::DoDrop() { DoDrop(NULL); }
// optimization needed
void CGame::DoDrop(CCardRegion *DestRegion) {
    CCardStack *DestStack;
    CCardRegion *BestRegion;

    if (DestRegion != NULL)
        BestRegion = DestRegion;
    else
        BestRegion = this->GetBestStack(_dragCard.x, _dragCard.y, g_CARDWIDTH,
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

    // when no movement
    if (_dragCard.x == vi->x && _dragCard.y == vi->y)
        return;

    ZoomCard(_dragCard.x, _dragCard.y, vi->x, vi->y, _dragCard.width,
             _dragCard.height, _p_background, _p_dragface);
}
CCardRegion *CGame::FindDropRegion(int Id, CCard card) {
    CCardStack stack;
    stack.Push(card);
    return FindDropRegion(Id, stack);
}

CCardRegion *CGame::FindDropRegion(int Id, CCardStack stack) {
    for (rVI vi = this->begin(); vi != this->end(); ++vi) {
        if ((vi->Id == Id) && vi->CanDrop(&stack))
            return &(*vi);
    }
    return NULL;
}

void calcPt(int x0, int y0, int x1, int y1, float t, int &X, int &Y) {
    X = int(x0 + t * (x1 - x0) + .5);
    Y = int(y0 + t * (y1 - y0) + .5);
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

        // SDL_Flip(_p_screen); // TODO Sdl 2.0
    }
    DrawStaticScene();
    SDL_FreeSurface(_p_dragface);
}

void CGame::DrawStaticScene() {
    for (rVI vi = this->begin(); vi != this->end(); ++vi) {
        SDL_PumpEvents();
        vi->DrawCardStack();
    }

    // SDL_Flip(_p_screen); // TODO Sdl 2.0
}

void CGame::DrawBackground(BOOL bIsInit) {
    if (_p_scene_background) {
        if (!bIsInit) {
            SDL_BlitSurface(_p_scene_background, NULL, _p_screen, NULL);
        } else {
            fade(_p_screen, _p_scene_background, 2, 0);
        }
    }

    for (rVI vi = this->begin(); vi != this->end(); ++vi) vi->DrawCardStack();
}

CCardRegion *CGame::GetBestStack(int x, int y, int w, int h,
                                 CCardStack *stack) {
    int maxoverlap = 0;
    int percent = 0;
    CCardRegion *best = 0;

    for (rVI vi = this->begin(); vi != this->end(); ++vi) {
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

////////////////////////////////////////
//       DrawCard
/*!
// \param int x :
// \param int y :
// \param int nCdIndex :
*/
int CGame::DrawCard(int x, int y, int nCdIndex) {
    return DrawCard(x, y, nCdIndex, NULL);
}

////////////////////////////////////////
//       DrawCard
/*! Disegna una carta da briscola. Il mazzo è composto dai segni disposti in
modo verticale.
// L'ordine dei segni è: bastoni, coppe, denari, spade.
// \param int x : x coordinata destination
// \param int y : y coordinate
// \param int nCdIndex : card index
// \param SDL_Surface *s : _p_screen surface
*/
int CGame::DrawCard(int x, int y, int nCdIndex, SDL_Surface *s) {
    if (s == NULL)
        s = _p_srfDeck;

    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    int iSegnoIx = nCdIndex / 10;
    int iCartaIx = nCdIndex % 10;
    TRACE("Segno %d, carta: %d", iSegnoIx, iCartaIx);

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_CardsSurf[nCdIndex]->clip_rect.w;
    _rctSrcCard.h = _p_CardsSurf[nCdIndex]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    return SDL_BlitSurface(_p_CardsSurf[nCdIndex], &_rctSrcCard, s, &dest);
}

////////////////////////////////////////
//       DrawCard
/*!
// \param VI vi :
// \param SDL_Surface *s :
*/
int CGame::DrawCard(VI vi, SDL_Surface *s) {
    if (s == NULL)
        s = _p_srfDeck;

    int nCdIndex = vi->Idx;

    if (nCdIndex < 0)
        nCdIndex = 0;
    if (nCdIndex > NUM_CARDS)
        nCdIndex = NUM_CARDS - 1;

    // _rctSrcCard.x = iSegnoIx * g_CARDWIDTH;
    // _rctSrcCard.y = iCartaIx * g_CARDHEIGHT;
    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_CardsSurf[nCdIndex]->clip_rect.w;
    _rctSrcCard.h = _p_CardsSurf[nCdIndex]->clip_rect.h;

    SDL_Rect dest;
    dest.x = vi->x;
    dest.y = vi->y;

    TRACE("Draw card ix = %d, segno = %d, valore %d \n", vi->Idx, vi->Suit(),
          vi->Rank());

    return SDL_BlitSurface(_p_CardsSurf[nCdIndex], &_rctSrcCard, s, &dest);
}

////////////////////////////////////////
//       DrawCardBack
/*! Disegna il retro della carta
// \param int x :
// \param int y :
*/
int CGame::DrawCardBack(int x, int y) { return DrawCardBack(x, y, NULL); }

////////////////////////////////////////
//       DrawCardBack
/*! Disegna il dorso della carta
// \param int x :
// \param int y :
// \param SDL_Surface *s :
*/
int CGame::DrawCardBack(int x, int y, SDL_Surface *s) {
    if (s == NULL)
        s = _p_srfDeck;

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_Symbol[0]->clip_rect.w;
    _rctSrcCard.h = _p_Symbol[0]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    return SDL_BlitSurface(_p_Symbol[0], &_rctSrcCard, s, &dest);
}

////////////////////////////////////////
//       DrawSymbol
/*!
Symbols:
    0:	Blank, no symbol
    1:	X symbol
    2:	O symbol
    3:	Holder symbol
// \param int x :
// \param int y :
// \param int nSymbol :
*/
int CGame::DrawSymbol(int x, int y, int nSymbol) {
    return DrawSymbol(x, y, nSymbol, NULL);
}

////////////////////////////////////////
//       DrawSymbol
/*! Disegna uno dei simboli dalla tabella dei simboli. 4 simboli numerati da 0 a
3
// \param int x :
// \param int y :
// \param int nSymbol :
// \param SDL_Surface *s :
*/
int CGame::DrawSymbol(int x, int y, int nSymbol, SDL_Surface *s) {
    if (nSymbol < 1)
        return 0;
    if (nSymbol > 3)
        nSymbol = 3;

    if (s == NULL)
        s = _p_srfDeck;

    _rctSrcCard.x = 0;
    _rctSrcCard.y = 0;
    _rctSrcCard.w = _p_Symbol[nSymbol]->clip_rect.w;
    _rctSrcCard.h = _p_Symbol[nSymbol]->clip_rect.h;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    return SDL_BlitSurface(_p_Symbol[nSymbol], &_rctSrcCard, s, &dest);
}

////////////////////////////////////////
//       AnimateCards
/*! Mostra una piccola animazione delle carte
 */
int CGame::AnimateCards() {
    srand((unsigned)time(NULL));

    int rot;
    int id;
    int x;
    unsigned int y;
    int xspeed;
    int yspeed;

    int GRAVITY = 1;
    unsigned int MAXY = _p_srfDeck->h;
    float BOUNCE = 0.8f;

    do {
        rot = rand() % 2;
        id = rand() % 51;
        x = rand() % _p_srfDeck->w;
        y = rand() % _p_srfDeck->h / 2;

        if (rot)
            xspeed = -4;
        else
            xspeed = 4;

        yspeed = 0;

        do  // while card is within the _p_screen
        {
            SDL_PumpEvents();
            if (SDL_GetMouseState(NULL, NULL))
                return -1;  // stop the animation

            yspeed = yspeed + GRAVITY;
            x += xspeed;
            y += yspeed;

            if (y + g_CARDHEIGHT > MAXY) {
                y = MAXY - g_CARDHEIGHT;
                yspeed = int(-yspeed * BOUNCE);
            }

            DrawCard(x, y, id, _p_srfDeck);
            // SDL_Flip(_p_srfDeck); // TODO Sdl 2.0
        }
        // 73 here is CARDWIDTH, but when using CARDWIDTH, it doesn't work
        while ((x + 73 > 0) && (x < _p_srfDeck->w));
        //		while((x + CARDWIDTH > 0) && (x < _p_srfDeck->w));
    } while (1);  // or while within specified time

    return 0;
}

void CGame::LoadDeckFromPac() {
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
                Temp = IMG_LoadJPG_RW(srcBack);
                if (Temp) {
                    // _p_CardsSurf[i + k * 10] = SDL_DisplayFormat(Temp); //
                    // TODO SDL 2.0
                } else {
                    ASSERT(0);
                }
            } else {
                ASSERT(0);
            }
        }
    }
    SDL_FreeSurface(Temp);

    if (_p_CardsSurf[0]) {
        g_CARDWIDTH = _p_CardsSurf[0]->clip_rect.w;
        g_CARDHEIGHT = _p_CardsSurf[0]->clip_rect.h;
    }
}

void CGame::LoadSymbols() {
    // _p_symbols
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
            Temp = IMG_LoadJPG_RW(srcBack);
            if (Temp) {
                // _p_Symbol[i] = SDL_DisplayFormat(Temp); // TODO SDL 2.0
            } else {
                ASSERT(0);
            }
        } else {
            ASSERT(0);
        }
    }
    SDL_FreeSurface(Temp);
}

int CGame::LoadCardPac() {
    char describtion[100];
    Uint8 num_anims;
    Uint16 w, h;
    Uint16 frames;
    Uint16 *delays;

    std::string strFileName = lpszDeckDir;
    strFileName += _DeckType.GetResFileName();

    SDL_RWops *src = SDL_RWFromFile(strFileName.c_str(), "rb");
    SDL_RWread(src, describtion, 100, 1);
    SDL_RWread(src, &num_anims, 1, 1);
    w = SDL_ReadLE16(src);  // witdh of the picture (pac of 4 cards)
    h = SDL_ReadLE16(src);  // height of the picture (pac of 10 rows of cards)
    frames = SDL_ReadLE16(src);

    delays = (Uint16 *)malloc(sizeof(Uint16) * frames);
    if (!delays)
        return 2;

    for (int i = 0; i < frames; i++) {
        // file format stores delays in 1/100th of second
        // we will convert them to game ticks
        delays[i] =
            THINKINGS_PER_TICK * ((10 * SDL_ReadLE16(src)) / FRAMETICKS);
    }

    SDL_Surface *s;
    SDL_Surface *temp;
    temp = IMG_LoadPNG_RW(src);
    if (!temp)
        return 0;

    if (SDL_WasInit(SDL_INIT_VIDEO) != 0) {
        // converte l'immagine al formato video scelto
        // s = SDL_DisplayFormat(temp);  // TODO SDL 2.0
        SDL_FreeSurface(temp);
        SDL_SetColorKey(s, TRUE, SDL_MapRGB(s->format, 0, 128, 0));  // SDL 2.0
    } else
        s = temp;  // we are dedicated windows traybar server

    g_CARDWIDTH = w / 4;
    g_CARDHEIGHT = h / 10;

    return 0;
}
