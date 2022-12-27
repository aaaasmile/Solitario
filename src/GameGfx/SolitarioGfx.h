#ifndef SOLIATRIO_GFX__H
#define SOLIATRIO_GFX__H

#include <vector>

#include "CardRegionGfx.h"
#include "ErrorInfo.h"
#include "GameSettings.h"
#include "win_type_global.h"

typedef std::vector<CardRegionGfx>::iterator rVI;

struct DRAGCARD {
    int x;
    int y;
    int width;
    int height;
};

const unsigned int CRD_NSYMBOL = 0;
const unsigned int CRD_XSYMBOL = 1;
const unsigned int CRD_OSYMBOL = 2;
const unsigned int CRD_HSYMBOL = 3;

class SolitarioGfx {
    enum { NUM_CARDS_ONDECK = 40, NUM_SYMBOLS = 4 };

public:
    SolitarioGfx();
    ~SolitarioGfx();

    LPErrInApp Initialize(SDL_Surface *s, SDL_Renderer *r);
    void Clear() { _cardRegionList.clear(); }
    int Size() { return _cardRegionList.size(); }

    void CreateRegion(int id, unsigned int attribs, unsigned int amode,
                      int dmode, int symbol, int x, int y, int xoffset,
                      int yoffset);
    bool DeleteRegion(CardRegionGfx *pRegion);

    void EmptyStacks();
    void InitAllCoords();

    CardRegionGfx *OnMouseDown(int x, int y);

    bool InitDrag(int x, int y);
    bool InitDrag(CardStackGfx *CargoStack, int x, int y);

    void DoDrag(int x, int y);

    void DoDrop();
    void DoDrop(CardRegionGfx *DestRegion);

    CardRegionGfx *FindDropRegion(int Id, CardGfx card);
    CardRegionGfx *FindDropRegion(int Id, CardStackGfx stack);

    void ZoomCard(int &sx, int &sy, int &dx, int &dy, int width, int height,
                  SDL_Surface *bg, SDL_Surface *fg);

    void DrawStaticScene();
    void DrawBackground(BOOL bIsInit);

    CardRegionGfx *GetBestStack(int x, int y, int width, int height,
                                CardStackGfx *stack);

    LPErrInApp DrawCardStack(SDL_Surface *s, CardRegionGfx *pcardRegion);
    LPErrInApp DrawCardStack(rVI vi);

    LPErrInApp DrawCard(int x, int y, int nCdIndex);
    LPErrInApp DrawCard(int x, int y, int nCdIndex, SDL_Surface *s);
    LPErrInApp DrawCard(VI vi, SDL_Surface *s);

    LPErrInApp DrawCardBack(int x, int y);
    LPErrInApp DrawCardBack(int x, int y, SDL_Surface *s);

    LPErrInApp DrawSymbol(int x, int y, int nSymbol);
    LPErrInApp DrawSymbol(int x, int y, int nSymbol, SDL_Surface *s);

    int AnimateCards();
    void SetDeckType(DeckType &Val) { _DeckType.CopyFrom(Val); }
    void ClearSurface();

    LPErrInApp LoadCardPac();
    LPErrInApp LoadSymbolsForPac();
    LPErrInApp LoadDeckFromSingleFile();
    LPErrInApp LoadSymbolsFromSingleFile();

    void SetSymbol(int regionNo, int symbol) {
        _cardRegionList[regionNo].SetSymbol(symbol);
    }
    LPErrInApp NewDeck(int regionNo) {
        return _cardRegionList[regionNo].NewDeck();
    }
    bool Empty(int regionNo) { return _cardRegionList[regionNo].Empty(); }
    void Shuffle(int regionNo) { _cardRegionList[regionNo].Shuffle(); }
    void PushInRegion(int regionNo, CardGfx card) {
        _cardRegionList[regionNo].Push(card);
    }
    void PushInRegion(int regionNo, CardStackGfx cs) {
        _cardRegionList[regionNo].Push(cs);
    }
    CardGfx PopFromRegion(int regionNo) {
        return _cardRegionList[regionNo].Pop();
    }
    CardStackGfx PopFromRegion(int regionNo, int items) {
        return _cardRegionList[regionNo].Pop(items);
    }
    void SetCardFaceUp(int regionNo, bool fTrue, int idx) {
        _cardRegionList[regionNo].SetCardFaceUp(fTrue, idx);
    }
    int RegionSize(int regionNo) { return _cardRegionList[regionNo].Size(); }
    void Reverse(int regionNo) { _cardRegionList[regionNo].Reverse(); }
    void InitCardCoords(int regionNo) {
        _cardRegionList[regionNo].InitCardCoords();
    }
    CardRegionGfx *GetRegion(int regionNo) {
        return &_cardRegionList[regionNo];
    }

private:
    void UpdateTextureAsFlipScreen();

private:
    CardStackGfx _dragStack;
    DRAGCARD _dragCard;
    CardRegionGfx *_p_sourceRegion;

    SDL_Surface *_p_screen;
    SDL_Texture *_p_ScreenTexture;
    SDL_Surface *_p_background;
    SDL_Surface *_p_dragface;
    SDL_Surface *_p_scene_background;
    SDL_Renderer *_p_sdlRenderer;

    int _oldx;
    int _oldy;

    SDL_Surface *_p_srfDeck;  // for pac
    SDL_Surface *_p_symbols;
    DeckType _DeckType;
    SDL_Surface *_p_CardsSurf[NUM_CARDS_ONDECK];  // for single file
    SDL_Surface *_p_Symbol[NUM_SYMBOLS];

    SDL_Rect _rctSrcBack;
    SDL_Rect _rctSrcCard;

    std::vector<CardRegionGfx> _cardRegionList;
};

#endif
