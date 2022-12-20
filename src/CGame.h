#ifndef CGAME_H
#define CGAME_H

#include <vector>

#include "CCardRegion.h"
#include "GameSettings.h"
#include "win_type_global.h"

typedef std::vector<CCardRegion>::iterator rVI;

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

class CGame {
    enum { NUM_CARDS_ONDECK = 40, NUM_SYMBOLS = 4 };

public:
    CGame();
    ~CGame();

    void Initialize(SDL_Surface *s);
    void Clear() { _cardRegionList.clear(); }
    int Size() { return _cardRegionList.size(); }

    void CreateRegion(int id, unsigned int attribs, unsigned int amode,
                      int dmode, int symbol, int x, int y, int xoffset,
                      int yoffset);
    bool DeleteRegion(CCardRegion *pRegion);

    void EmptyStacks();
    void InitAllCoords();

    CCardRegion *OnMouseDown(int x, int y);

    bool InitDrag(int x, int y);
    bool InitDrag(CCardStack *CargoStack, int x, int y);

    void DoDrag(int x, int y);

    void DoDrop();
    void DoDrop(CCardRegion *DestRegion);

    CCardRegion *FindDropRegion(int Id, CCard card);
    CCardRegion *FindDropRegion(int Id, CCardStack stack);

    void ZoomCard(int &sx, int &sy, int &dx, int &dy, int width, int height,
                  SDL_Surface *bg, SDL_Surface *fg);

    void DrawStaticScene();
    void DrawBackground(BOOL bIsInit);

    CCardRegion *GetBestStack(int x, int y, int width, int height,
                              CCardStack *stack);
    int InitDeck(SDL_Surface *s);
    int LoadCardPac();

    int DrawCard(int x, int y, int nCdIndex);
    int DrawCard(int x, int y, int nCdIndex, SDL_Surface *s);
    int DrawCard(VI vi, SDL_Surface *s);

    int DrawCardBack(int x, int y);
    int DrawCardBack(int x, int y, SDL_Surface *s);

    int DrawSymbol(int x, int y, int nSymbol);
    int DrawSymbol(int x, int y, int nSymbol, SDL_Surface *s);

    int AnimateCards();
    void SetDeckType(DeckType &Val) { _DeckType.CopyFrom(Val); }
    void ClearSurface();

    void LoadDeckFromPac();
    void LoadSymbols();

private:
    CCardStack _dragStack;
    DRAGCARD _dragCard;
    CCardRegion *_p_sourceRegion;

    SDL_Surface *_p_screen;
    SDL_Surface *_p_background;
    SDL_Surface *_p_dragface;
    SDL_Surface *_p_scene_background;

    int _oldx;
    int _oldy;

    SDL_Surface *_p_srfDeck;
    SDL_Surface *_p_symbols;

    SDL_Rect _rctSrcBack;
    SDL_Rect _rctSrcCard;

    DeckType _DeckType;
    SDL_Surface *_p_CardsSurf[NUM_CARDS_ONDECK];
    SDL_Surface *_p_Symbol[NUM_SYMBOLS];
    std::vector<CCardRegion> _cardRegionList;
};

#endif  // CGAME_H
