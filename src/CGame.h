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

class CGame : public std::vector<CCardRegion> {
    enum { NUM_CARDS_ONDECK = 40, NUM_SYMBOLS = 4 };

public:
    CGame();
    ~CGame();

    void Initialize(SDL_Surface *s);

    void Clear() { this->clear(); }
    int Size() { return this->size(); }

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

    int AnimateCards();  // might take seconds as parameter
    void SetDeckType(DeckType &Val) { m_DeckType.Clone(Val); }
    void ClearSurface();

    void LoadCustomDeck();
    void LoadSymbols();

private:
    CCardStack DragStack;
    CCardRegion *SourceRegion;

    SDL_Surface *screen;
    SDL_Surface *background;
    SDL_Surface *dragface;
    SDL_Surface *scene_background;

    DRAGCARD dcard;

    int oldx;
    int oldy;

    SDL_Surface *deck;
    SDL_Surface *scr;
    SDL_Surface *symbols;

    SDL_Rect SrcBack;
    SDL_Rect SrcCard;

    DeckType m_DeckType;
    SDL_Surface *m_pCardsSurf[NUM_CARDS_ONDECK];
    SDL_Surface *m_pSymbol[NUM_SYMBOLS];
};

#endif  // CGAME_H
