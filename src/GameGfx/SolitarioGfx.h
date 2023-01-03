#ifndef SOLIATRIO_GFX__H
#define SOLIATRIO_GFX__H

#include <vector>

#include "CardRegionGfx.h"
#include "ErrorInfo.h"
#include "GameSettings.h"
#include "WinTypeGlobal.h"

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
    void Clear(int regionNo) { _cardRegionList[regionNo].Clear(); }
    int Size(int regionNo) { return _cardRegionList[regionNo].Size(); }
    void ClearAll();

    void CreateRegion(int id, unsigned int attribs, unsigned int amode,
                      int dmode, int symbol, int x, int y, int xoffset,
                      int yoffset);
    bool DeleteRegion(CardRegionGfx *pRegion);

    void EmptyStacks();
    void InitAllCoords();

    CardRegionGfx *OnMouseDown(int x, int y);

    LPErrInApp InitDrag(int x, int y, bool &isInitDrag);
    LPErrInApp InitDrag(CardStackGfx *CargoStack, int x, int y,
                        bool &isInitDrag);

    void DoDrag(int x, int y);

    void DoDrop();
    void DoDrop(CardRegionGfx *DestRegion);

    CardRegionGfx *FindDropRegion(int Id, CardGfx card);
    CardRegionGfx *FindDropRegion(int Id, CardStackGfx stack);

    void DrawStaticScene();
    LPErrInApp DrawBackground(BOOL bIsInit);

    CardRegionGfx *GetBestStack(int x, int y, int width, int height,
                                CardStackGfx *stack);

    LPErrInApp DrawCardStack(SDL_Surface *s, CardRegionGfx *pcardRegion);
    LPErrInApp DrawCardStack(rVI vi);

    LPErrInApp DrawCard(int x, int y, int nCdIndex);
    LPErrInApp DrawCard(int x, int y, int nCdIndex, SDL_Surface *s);
    LPErrInApp DrawCardPac(int x, int y, int nCdIndex, SDL_Surface *s);
    LPErrInApp DrawCard(VI vi, SDL_Surface *s);
    LPErrInApp DrawCardPac(VI vi, SDL_Surface *s);

    LPErrInApp DrawCardBack(int x, int y);
    LPErrInApp DrawCardBack(int x, int y, SDL_Surface *s);
    LPErrInApp DrawCardBackPac(int x, int y, SDL_Surface *s);

    LPErrInApp DrawSymbol(int x, int y, int nSymbol);
    LPErrInApp DrawSymbol(int x, int y, int nSymbol, SDL_Surface *s);
    LPErrInApp DrawSymbolPac(int x, int y, int nSymbol, SDL_Surface *s);

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
    void PushCardInRegion(int regionNo, CardGfx card) {
        _cardRegionList[regionNo].Push(card);
    }
    void PushStackInRegion(int regionNo, CardStackGfx cs) {
        _cardRegionList[regionNo].Push(cs);
    }
    CardGfx PopCardFromRegion(int regionNo) {
        return _cardRegionList[regionNo].PopCard();
    }
    CardStackGfx PopStackFromRegion(int regionNo, int items) {
        return _cardRegionList[regionNo].PopStack(items);
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
    void updateTextureAsFlipScreen();
    void zoomCard(int &sx, int &sy, VI vi, int width, int height,
                  SDL_Surface *bg, SDL_Surface *fg);

private:
    CardStackGfx _dragStack;
    DRAGCARD _dragCard;
    CardRegionGfx *_p_sourceRegion;

    SDL_Surface *_p_Screen;
    SDL_Texture *_p_ScreenTexture;
    SDL_Surface *_p_Background;
    SDL_Surface *_p_Dragface;
    SDL_Surface *_p_SceneBackground;
    SDL_Renderer *_p_sdlRenderer;

    int _oldx;
    int _oldy;

    SDL_Surface *_p_Deck;  // for pac
    SDL_Surface *_p_Symbols;
    DeckType _DeckType;
    SDL_Surface *_p_CardsSurf[NUM_CARDS_ONDECK];  // for single file
    SDL_Surface *_p_SymbolsSurf[NUM_SYMBOLS];

    SDL_Rect _rctSrcCard;

    std::vector<CardRegionGfx> _cardRegionList;
};

#endif
