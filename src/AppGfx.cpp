#include "AppGfx.h"

#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>

#include "ErrorInfo.h"
#include "GfxUtil.h"
#include "MenuMgr.h"
#include "MusicManager.h"
#include "config.h"
#include "win_type_global.h"

#ifdef _WINDOWS
#include "regkey.h"
#else
#include "libini.h"
#endif
#include "Credits.h"
#include "Fading.h"

#define CRD_PILE 0
#define CRD_FOUNDATION 1
#define CRD_RESERVE 2
#define CRD_WASTE 3

#ifdef _WINDOWS
static LPCSTR strIDS_KEY_LASTPATH = "SOFTWARE\\BredaSoft\\Solitario";
static LPCSTR strIDS_KEY_PLAYERNAME = "PlayerName";
static LPCSTR strIDS_KEY_LANGUAGECURRENT = "Language";
static LPCSTR strIDS_KEY_DECKCURRENT = "DeckCurrent";
static LPCSTR strIDS_KEY_MUSICENABLED = "Musicenabled";
#else
static const char *lpszIniFileName = DATA_PREFIX "options.ini";
static const char *lpszSectAll = "global";
static const char *lpszKeyLang = "Language";
static const char *lpszKeyDeck = "DeckCurrent";
static const char *lpszKeyMusic = "Musicenabled";
#endif

static const char *lpszIconProgFile = DATA_PREFIX "images/icona_asso.bmp";
static const char *lpszFontFile = DATA_PREFIX "images/font.bmp";
static const char *lpszTitleFile = DATA_PREFIX "images/title.png";
// static const char *lpszBackGroundFile = DATA_PREFIX "images/im001537.jpg";
static const char *lpszIniFontAriblk = DATA_PREFIX "font/ariblk.ttf";
static const char *lpszIniFontVera = DATA_PREFIX "font/vera.ttf";
static const char *lpszImageSplash = DATA_PREFIX "images/im001537.jpg";

AppGfx::AppGfx() {
    _p_Screen = NULL;
    _iScreenW = 1024;  // 640;
    _iScreenH = 768;   // 480;
    _iBpp = 0;
    _p_imgBackground = NULL;
    _p_CustomFont = NULL;
    _bStartdrag = FALSE;
    _p_MusicManager = 0;
    _p_Title = 0;
    _bFullScreen = FALSE;
    _p_GameSettings = GAMESET::GetSettings();
}

AppGfx::~AppGfx() { terminate(); }

LPErrInApp AppGfx::Init() {
    TRACE("Init App");
    LPErrInApp err = loadProfile();
    if (err != NULL) {
        return err;
    }

    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
        if (SDL_Init(0) < 0) {
            return ERR_UTIL::ErrorCreate("Couldn't initialize SDL: %s\n",
                                         SDL_GetError());
        }
    }
    err = createWindow();
    if (err != NULL) {
        return err;
    }

    _p_MusicManager = new MusicManager;
    _p_MusicManager->Init();

    _LanguageMgr.SetLang(_p_GameSettings->eLanguageCurrent);

    _p_CustomFont = new CustomFont;
    err = _p_CustomFont->LoadFont(lpszFontFile);
    if (err != NULL) {
        return err;
    }

    if (TTF_Init() == -1) {
        return ERR_UTIL::ErrorCreate("Font init error");
    }
    std::string strFileFontStatus = lpszIniFontAriblk;
    _p_fontAriblk = TTF_OpenFont(strFileFontStatus.c_str(), 22);
    if (_p_fontAriblk == 0) {
        return ERR_UTIL::ErrorCreate("Unable to load font %s, error: %s\n",
                                     strFileFontStatus.c_str(), SDL_GetError());
    }
    strFileFontStatus = lpszIniFontVera;
    _p_fontVera = TTF_OpenFont(strFileFontStatus.c_str(), 11);
    if (_p_fontVera == 0) {
        return ERR_UTIL::ErrorCreate("Unable to load font %s, error: %s\n",
                                     strFileFontStatus.c_str(), SDL_GetError());
    }

    SDL_SetWindowTitle(_p_Window,
                       _LanguageMgr.GetCStringId(Languages::ID_SOLITARIO));

    // icona trasparente
    SDL_Surface *psIcon = SDL_LoadBMP(lpszIconProgFile);
    if (psIcon == 0) {
        return ERR_UTIL::ErrorCreate("Icon not found");
    }
    SDL_SetColorKey(psIcon, TRUE, SDL_MapRGB(psIcon->format, 0, 128, 0));
    SDL_SetWindowIcon(_p_Window, psIcon);

    // initialize random generator. Pay attention to the different version of
    // srand
#ifdef _GLIBCPP_HAVE_DRAND48
    srand48((unsigned)time(0));
#else
    srand((unsigned)time(0));
#endif

    _p_Title = IMG_Load(lpszTitleFile);
    if (_p_Title == 0) {
        return ERR_UTIL::ErrorCreate("Title image not found");
    }
    err = loadSplash();
    if (err != NULL) {
        return err;
    }

    drawSplash();

    err = _p_MusicManager->LoadMusicRes();
    if (err != NULL) {
        return err;
    }
    return NULL;
}

LPErrInApp AppGfx::loadSplash() {
    std::string strFileName = lpszImageSplash;

    SDL_RWops *srcBack = SDL_RWFromFile(strFileName.c_str(), "rb");
    if (srcBack == 0) {
        return ERR_UTIL::ErrorCreate("Unable to load %s background image\n",
                                     strFileName.c_str());
    }
    _p_Splash = IMG_LoadJPG_RW(srcBack);
    if (_p_Splash == 0) {
        return ERR_UTIL::ErrorCreate("Unable to create splash");
    }
    return NULL;
}

LPErrInApp AppGfx::createWindow() {
    int flagwin = 0;
    if (_p_Window != NULL) {
        SDL_DestroyWindow(_p_Window);
    }
    if (_p_Screen) {
        SDL_FreeSurface(_p_Screen);
    }
    if (_bFullScreen) {
        flagwin = SDL_WINDOW_FULLSCREEN_DESKTOP;
    } else {
        flagwin = SDL_WINDOW_SHOWN;
    }

    _p_Window = SDL_CreateWindow("Solitario", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, _iScreenW, _iScreenH,
                                 flagwin);
    if (_p_Window == NULL) {
        return ERR_UTIL::ErrorCreate("Error SDL_CreateWindow: %s\n",
                                     SDL_GetError());
    }

    _p_sdlRenderer =
        SDL_CreateRenderer(_p_Window, -1, SDL_RENDERER_ACCELERATED);

    if (_p_sdlRenderer == NULL) {
        return ERR_UTIL::ErrorCreate("Cannot create renderer: %s\n",
                                     SDL_GetError());
    }

    _p_Screen = SDL_CreateRGBSurface(0, _iScreenW, _iScreenH, 32, 0x00FF0000,
                                     0x0000FF00, 0x000000FF, 0xFF000000);
    if (_p_Screen == NULL) {
        return ERR_UTIL::ErrorCreate("Error SDL_CreateRGBSurface: %s\n",
                                     SDL_GetError());
    }
    if (_p_ScreenTexture != NULL) {
        SDL_DestroyTexture(_p_ScreenTexture);
    }
    _p_ScreenTexture =
        SDL_CreateTexture(_p_sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
                          SDL_TEXTUREACCESS_STREAMING, _iScreenW, _iScreenH);
    if (_p_ScreenTexture == NULL) {
        return ERR_UTIL::ErrorCreate("Error SDL_CreateTexture: %s\n",
                                     SDL_GetError());
    }
    return NULL;
}

LPErrInApp AppGfx::startGameLoop() {
    TRACE("Start Game Loop");
    _p_MusicManager->StopMusic();

    LPErrInApp err;
    if (_p_SolitarioGfx != NULL) {
        delete _p_SolitarioGfx;
    }
    _p_SolitarioGfx = new SolitarioGfx();

    _p_SolitarioGfx->SetDeckType(_p_GameSettings->deckTypeVal);
    _p_SolitarioGfx->ClearSurface();
    _p_SolitarioGfx->Clear();
    err = _p_SolitarioGfx->Initialize(_p_Screen, _p_sdlRenderer);
    if (err != NULL)
        return err;

    // index 0
    _p_SolitarioGfx->CreateRegion(CRD_PILE, CRD_VISIBLE | CRD_3D, 0, 0,
                                  CRD_OSYMBOL, 35, 10, 2, 2);
    // index 1-7
    int i;
    for (i = 1; i <= 7; i++)
        _p_SolitarioGfx->CreateRegion(
            CRD_FOUNDATION, CRD_VISIBLE | CRD_DODRAG | CRD_DODROP,
            CRD_DOOPCOLOR | CRD_DOLOWER | CRD_DOLOWERBY1 | CRD_DOKING,
            CRD_DRAGFACEUP, CRD_HSYMBOL, (g_CARDWIDTH * (i - 1)) + (i * 17),
            g_CARDHEIGHT + 40, 0, 32);
    // index 8
    _p_SolitarioGfx->CreateRegion(
        CRD_RESERVE, CRD_VISIBLE | CRD_FACEUP | CRD_DODRAG | CRD_3D, CRD_DOALL,
        CRD_DRAGTOP, CRD_NSYMBOL, g_CARDWIDTH + 65, 10, 0, 0);
    // index 9-12
    for (i = 4; i <= 7; i++)
        _p_SolitarioGfx->CreateRegion(
            CRD_WASTE, CRD_VISIBLE | CRD_3D | CRD_DODRAG | CRD_DODROP,
            CRD_DOSINGLE | CRD_DOHIGHER | CRD_DOHIGHERBY1 | CRD_DOACE |
                CRD_DOSUIT,
            CRD_DRAGTOP, CRD_HSYMBOL, (g_CARDWIDTH * (i - 1)) + (i * 17), 10, 0,
            0);

    err = newGame();
    if (err != NULL)
        return err;
    _p_SolitarioGfx->DrawStaticScene();

    SDL_Event event;
    int done = 0;

    while (done == 0) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    fade(_p_Screen, _p_Screen, 1, 1, _p_sdlRenderer);
                    return NULL;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        done = 1;
                    }
                    err = handleGameLoopKeyDownEvent(event);
                    if (err != NULL)
                        return err;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    handleGameLoopMouseDownEvent(event);
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
        if (_p_GameSettings->bMusicEnabled) {
            _p_MusicManager->PlayMusic(MusicManager::MUSIC_GAME_SND,
                                       MusicManager::LOOP_ON);
        }
    }
    LeaveMenu();
    return NULL;
}

LPErrInApp AppGfx::newGame() {
    LPErrInApp err;
    _p_SolitarioGfx->SetSymbol(0, CRD_OSYMBOL);

    _p_SolitarioGfx->EmptyStacks();

    err = _p_SolitarioGfx->NewDeck(0);
    if (err != NULL) {
        return err;
    }
    _p_SolitarioGfx->Shuffle(0);

    // deal
    int i;
    for (i = 1; i <= 7; i++) {
        _p_SolitarioGfx->PushInRegion(i, _p_SolitarioGfx->PopFromRegion(0));
    }

    _p_SolitarioGfx->InitAllCoords();

    for (i = 1; i <= 7; i++) {
        _p_SolitarioGfx->SetCardFaceUp(i, TRUE,
                                       _p_SolitarioGfx->RegionSize(i) - 1);
    }
    return NULL;
}

LPErrInApp AppGfx::handleGameLoopKeyDownEvent(SDL_Event &event) {
    LPErrInApp err;
    if (event.key.keysym.sym == SDLK_n) {
        err = newGame();
        if (err != NULL) {
            return err;
        }
        _p_SolitarioGfx->DrawBackground(FALSE);
        _p_SolitarioGfx->DrawStaticScene();
    }
    if (event.key.keysym.sym == SDLK_a) {
        _p_SolitarioGfx->AnimateCards();
    }
    if (event.key.keysym.sym == SDLK_r) {
        _p_SolitarioGfx->DrawStaticScene();
    }
    return NULL;
}

void AppGfx::handleGameLoopMouseDownEvent(SDL_Event &event) {
    CardRegionGfx *srcReg;
    if (event.button.button == SDL_BUTTON_LEFT) {
        srcReg = _p_SolitarioGfx->OnMouseDown(event.button.x, event.button.y);
        if (srcReg == NULL)
            return;
        // clicked on the top of the foundations
        if ((srcReg->Id == CRD_FOUNDATION) &&
            srcReg->PtOnTop(event.button.x, event.button.y)) {
            srcReg->SetCardFaceUp(TRUE, srcReg->Size() - 1);
        }
        // clicked on the foundations, reserve, wastes for dragging
        if (((srcReg->Id == CRD_FOUNDATION) || (srcReg->Id == CRD_RESERVE) ||
             (srcReg->Id == CRD_WASTE)) &&
            _p_SolitarioGfx->InitDrag(event.button.x, event.button.y)) {
            _bStartdrag = TRUE;
            SDL_SetRelativeMouseMode(SDL_TRUE);  // SDL 2.0
        }
        // clicked on the pile
        if (srcReg->Id == CRD_PILE) {
            CardStackGfx *cs = new CardStackGfx;
            if (srcReg->Empty() &&
                !_p_SolitarioGfx->Empty(8))  // Bring back the cards
            {
                *cs = _p_SolitarioGfx->PopFromRegion(
                    8, _p_SolitarioGfx->RegionSize(8));
                cs->SetCardsFaceUp(FALSE);
                _p_SolitarioGfx->InitDrag(cs, -1, -1);
                _p_SolitarioGfx->DoDrop(_p_SolitarioGfx->GetRegion(0));
                _p_SolitarioGfx->Reverse(0);
                _p_SolitarioGfx->InitCardCoords(0);
            } else if (!srcReg->Empty() && (!_p_SolitarioGfx->Empty(8) ||
                                            _p_SolitarioGfx->Empty(8))) {
                *cs = _p_SolitarioGfx->PopFromRegion(0, 1);
                cs->SetCardsFaceUp(TRUE);
                _p_SolitarioGfx->InitDrag(cs, -1, -1);
                _p_SolitarioGfx->DoDrop(_p_SolitarioGfx->GetRegion(8));
            }
        }
    }

    // substitute right-click for double-click event
    if (event.button.button == SDL_BUTTON_RIGHT) {
        srcReg = _p_SolitarioGfx->OnMouseDown(event.button.x, event.button.y);
        if (srcReg == NULL)
            return;
        CardRegionGfx *cr;
        CardGfx card = srcReg->GetCard(srcReg->Size() - 1);

        // clicked on the top of the foundations
        if (((srcReg->Id == CRD_FOUNDATION) || (srcReg->Id == CRD_RESERVE)) &&
            card.FaceUp() && srcReg->PtOnTop(event.button.x, event.button.y)) {
            cr = _p_SolitarioGfx->FindDropRegion(CRD_WASTE, card);
            if (cr) {
                CardStackGfx *cs = new CardStackGfx;
                *cs = srcReg->Pop(1);
                _p_SolitarioGfx->InitDrag(cs, -1, -1);
                _p_SolitarioGfx->DoDrop(cr);
            }
        }
    }
}

void AppGfx::handleGameLoopMouseMoveEvent(SDL_Event &event) {
    if (event.motion.state == SDL_BUTTON(1) && _bStartdrag)
        _p_SolitarioGfx->DoDrag(event.motion.x, event.motion.y);
}

LPErrInApp AppGfx::handleGameLoopMouseUpEvent(SDL_Event &event) {
    LPErrInApp err;
    if (_bStartdrag) {
        _bStartdrag = FALSE;
        _p_SolitarioGfx->DoDrop();
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
    if (_p_SolitarioGfx->Empty(0) && _p_SolitarioGfx->Empty(8)) {
        _p_SolitarioGfx->SetSymbol(0, 1);
        _p_SolitarioGfx->DrawStaticScene();
    }
    // victory
    if ((_p_SolitarioGfx[9].Size() == 10) &&
        (_p_SolitarioGfx[10].Size() == 10) &&
        (_p_SolitarioGfx[11].Size() == 10) &&
        (_p_SolitarioGfx[12].Size() == 10)) {
        _p_SolitarioGfx->AnimateCards();
        err = newGame();
        if (err != NULL) {
            return err;
        }
        _p_SolitarioGfx->DrawStaticScene();
    }
    return NULL;
}

void AppGfx::terminate() {
    writeProfile();
    SDL_ShowCursor(SDL_ENABLE);

    if (_p_imgBackground != NULL) {
        SDL_FreeSurface(_p_imgBackground);
        _p_imgBackground = NULL;
    }
    delete _p_CustomFont;
    if (_p_Screen != NULL) {
        SDL_FreeSurface(_p_Screen);
        _p_Screen = NULL;
    }
    if (_p_Splash) {
        SDL_FreeSurface(_p_Splash);
        _p_Splash = NULL;
    }
    if (_p_Title) {
        SDL_FreeSurface(_p_Title);
        _p_Title = NULL;
    }
    if (_p_ScreenTexture != NULL) {
        SDL_DestroyTexture(_p_ScreenTexture);
    }

    delete _p_MusicManager;
    delete _p_SolitarioGfx;
    _p_SolitarioGfx = NULL;
    _p_MusicManager = NULL;

    SDL_DestroyWindow(_p_Window);
    SDL_Quit();
}

LPErrInApp AppGfx::loadProfile() {
#ifdef _WINDOWS
    RegistryKey RegKey;
    LONG lRes;

    lRes = RegKey.Open(HKEY_CURRENT_USER, strIDS_KEY_LASTPATH);

    if (!lRes) {
        // player name
        _p_GameSettings->strPlayerName = RegKey.getRegStringValue(
            _p_GameSettings->strPlayerName.c_str(), strIDS_KEY_PLAYERNAME);
        // deck type
        int iVal =
            RegKey.getRegDWordValue(_p_GameSettings->deckTypeVal.GetTypeIndex(),
                                    strIDS_KEY_DECKCURRENT);
        _p_GameSettings->deckTypeVal.SetTypeIndex(iVal);

        // language
        iVal = RegKey.getRegDWordValue(_p_GameSettings->eLanguageCurrent,
                                       strIDS_KEY_LANGUAGECURRENT);
        switch (iVal) {
            case 0:
                _p_GameSettings->eLanguageCurrent = LanguageMgr::LANG_ITA;
                break;
            case 1:
                _p_GameSettings->eLanguageCurrent =
                    LanguageMgr::LANG_DIAL_BREDA;
                break;
        }
        // music
        iVal = RegKey.getRegDWordValue(0, strIDS_KEY_MUSICENABLED);
        if (!_bOverride) {
            if (iVal == 0) {
                _p_GameSettings->bMusicEnabled = FALSE;
            } else {
                _p_GameSettings->bMusicEnabled = TRUE;
            }
        }
    } else {
        //  create a key
        RegKey.Create(HKEY_CURRENT_USER, strIDS_KEY_LASTPATH);
    }
    RegKey.Close();
#else
    // linux version
    ini_fd_t pIni = ini_open(lpszIniFileName, "r", "#");

    if (pIni == NULL)
        return ERR_UTIL::ErrorCreate("Ini file error %s", lpszIniFileName);

    int iVal;

    // deck type
    iVal = _p_GameSettings->deckTypeVal.GetTypeIndex();
    if (pIni) {
        ini_locateHeading(pIni, lpszSectAll);
        ini_locateKey(pIni, lpszKeyDeck);
        ini_readInt(pIni, &iVal);
    }
    _p_GameSettings->deckTypeVal.SetTypeIndex(iVal);
    // language
    iVal = _p_GameSettings->eLanguageCurrent;
    if (pIni) {
        ini_locateHeading(pIni, lpszSectAll);
        ini_locateKey(pIni, lpszKeyLang);
        ini_readInt(pIni, &iVal);
    }
    _p_GameSettings->eLanguageCurrent = (Languages::eLangId)iVal;
    // music
    iVal = FALSE;
    if (pIni) {
        ini_locateHeading(pIni, lpszSectAll);
        ini_locateKey(pIni, lpszKeyMusic);
        ini_readInt(pIni, &iVal);
    }
    if (!_bOverride) {
        _p_GameSettings->bMusicEnabled = iVal;
    }

    ini_close(pIni);
    return NULL;
#endif
}

void AppGfx::writeProfile() {
#ifdef _WINDOWS
    RegistryKey RegKey;
    LONG lRes;
    lRes = RegKey.Open(HKEY_CURRENT_USER, strIDS_KEY_LASTPATH);
    if (!lRes) {
        RegKey.setRegStringValue(_p_GameSettings->strPlayerName.c_str(),
                                 strIDS_KEY_PLAYERNAME);
        RegKey.setRegDWordValue(_p_GameSettings->eLanguageCurrent,
                                strIDS_KEY_LANGUAGECURRENT);
        RegKey.setRegDWordValue(_p_GameSettings->deckTypeVal.GetType(),
                                strIDS_KEY_DECKCURRENT);
        RegKey.setRegDWordValue(_p_GameSettings->bMusicEnabled,
                                strIDS_KEY_MUSICENABLED);
        RegKey.Close();
    }
#else

    ini_fd_t pIni = ini_open(lpszIniFileName, "w", "#");
    if (pIni == 0)
        return;

    // deck type
    ini_locateHeading(pIni, lpszSectAll);
    ini_locateKey(pIni, lpszKeyDeck);
    ini_writeInt(pIni, (int)_p_GameSettings->deckTypeVal.GetType());

    // language
    ini_locateHeading(pIni, lpszSectAll);
    ini_locateKey(pIni, lpszKeyLang);
    ini_writeInt(pIni, _p_GameSettings->eLanguageCurrent);

    // music
    ini_locateHeading(pIni, lpszSectAll);
    ini_locateKey(pIni, lpszKeyMusic);
    ini_writeInt(pIni, _p_GameSettings->bMusicEnabled);

    ini_close(pIni);
#endif
}

TTF_Font *fncBind_GetFontVera(void *self) {
    AppGfx *pApp = (AppGfx *)self;
    return pApp->GetFontVera();
}

TTF_Font *fncBind_GetFontAriblk(void *self) {
    AppGfx *pApp = (AppGfx *)self;
    return pApp->GetFontAriblk();
}

Languages *fncBind_GetLanguageMan(void *self) {
    AppGfx *pApp = (AppGfx *)self;
    return pApp->GetLanguageMan();
}

void fncBind_LeaveMenu(void *self) {
    AppGfx *pApp = (AppGfx *)self;
    pApp->LeaveMenu();
}

void fncBind_SetNextMenu(void *self, int iVal) {
    AppGfx *pApp = (AppGfx *)self;
    pApp->SetNextMenu(iVal);
}

MenuDelegator AppGfx::prepMenuDelegator() {
    // Use only static otherwise you loose it
    static VMenuDelegator const tc = {
        .GetFontVera = (&fncBind_GetFontVera),
        .GetFontAriblk = (&fncBind_GetFontAriblk),
        .GetLanguageMan = (&fncBind_GetLanguageMan),
        .LeaveMenu = (&fncBind_LeaveMenu),
        .SetNextMenu = (&fncBind_SetNextMenu)};

    return (MenuDelegator){.tc = &tc, .self = this};
}

void AppGfx::LeaveMenu() {
    drawSplash();
    _Histmenu.pop();
}

void AppGfx::drawSplash() {
    SDL_BlitSurface(_p_Splash, NULL, _p_Screen, NULL);
    updateScreenTexture();
}

LPErrInApp AppGfx::MainLoop() {
    LPErrInApp err;
    bool bquit = false;

    cMenuMgr *pMenuMgr = new cMenuMgr();
    MenuDelegator delegator = prepMenuDelegator();
    pMenuMgr->Initialize(_p_Screen, _p_sdlRenderer, delegator);

    // set main menu
    _Histmenu.push(cMenuMgr::QUITAPP);
    _Histmenu.push(cMenuMgr::MENU_ROOT);

    pMenuMgr->SetBackground(_p_Splash);

    while (!bquit && !_Histmenu.empty()) {
        switch (_Histmenu.top()) {
            case cMenuMgr::MENU_ROOT:
                if (_p_GameSettings->bMusicEnabled &&
                    !_p_MusicManager->IsPLayingMusic()) {
                    _p_MusicManager->PlayMusic(MusicManager::MUSIC_INIT_SND,
                                               MusicManager::LOOP_ON);
                }
                err = pMenuMgr->HandleRootMenu();
                if (err != NULL)
                    return err;

                break;

            case cMenuMgr::MENU_GAME:
                err = startGameLoop();
                if (err != NULL)
                    return err;
                break;

            case cMenuMgr::MENU_HELP:
                err = showHelp();
                if (err != NULL)
                    return err;
                break;

            case cMenuMgr::MENU_CREDITS:
                err = showCredits();
                if (err != NULL)
                    return err;
                break;

            case cMenuMgr::MENU_OPTIONS:
                err = showOptionGeneral();
                if (err != NULL)
                    return err;
                break;

            case cMenuMgr::QUITAPP:
            default:
                bquit = true;
                break;
        }

        updateScreenTexture();
    }
    delete pMenuMgr;
    return NULL;
}

LPErrInApp AppGfx::showHelp() {
    // TODO showHelp
    LeaveMenu();
    return NULL;
}

LPErrInApp AppGfx::showCredits() {
    credits(_p_Screen, _p_Title, _p_sdlRenderer);
    LeaveMenu();
    return NULL;
}

LPErrInApp AppGfx::showOptionGeneral() {
    // TODO showOptionGeneral
    LeaveMenu();
    return NULL;
}

int AppGfx::waitKeyLoop() {
    SDL_Event event;
    while (1) {
        if (SDL_WaitEvent(&event) == 1) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    return event.key.keysym.scancode;
                    break;
                case SDL_MOUSEBUTTONUP:
                    return event.button.button;
                    break;
            }
        }
    }
}

void AppGfx::hightScoreMenu() {
    int tx, ty;
    std::string temp;

    SDL_FillRect(_p_Screen, &_p_Screen->clip_rect,
                 SDL_MapRGBA(_p_Screen->format, 0, 0, 0, 0));
    tx = (_p_Screen->w - 400) / 2;
    ty = (_p_Screen->h - 260) / 2;

    _p_CustomFont->DrawString(
        _p_Screen, _LanguageMgr.GetStringId(Languages::ID_HISCORE), TEXTMIXED,
        TEXTALIGNCENTER, 0, ty - 10 - SDLFONTSIZE * 2, 0);
    _p_CustomFont->DrawString(_p_Screen, "-------------", TEXTMIXED,
                              TEXTALIGNCENTER, 0, ty - 10 - SDLFONTSIZE, 0);

    GFX_UTIL::boxRGBA(_p_Screen, tx, ty, tx + 400, ty + 260, 0, 0, 48, 255);
    for (int k = 0; k < 10; k++) {
        temp = STR_UTIL::intToString(_HScoreMgr.HS_Scores[k]);
        _p_CustomFont->DrawString(_p_Screen, temp, TEXTMIXED, TEXTALIGNLEFT,
                                  tx + 5, ty + 10 + (SDLFONTSIZE + 8) * k, 0);
        if (_HScoreMgr.HS_Names[k].size() == 0) {
            temp = _LanguageMgr.GetStringId(Languages::ID_ANONIM);
        } else {
            temp = _HScoreMgr.HS_Names[k];
        }
        _p_CustomFont->DrawString(_p_Screen, temp, TEXTMIXED, TEXTALIGNLEFT,
                                  tx + 400 - 15 * SDLFONTSIZE,
                                  ty + 10 + (SDLFONTSIZE + 8) * k, 0);
    }
    _p_CustomFont->DrawString(
        _p_Screen, _LanguageMgr.GetStringId(Languages::ID_PUSHBUTTON),
        TEXTMIXED, TEXTALIGNCENTER, 0, ty + 260 + SDLFONTSIZE, 0);
    updateScreenTexture();
    waitKeyLoop();
}

void AppGfx::updateScreenTexture() {
    // SDL 2.0
    SDL_UpdateTexture(_p_ScreenTexture, NULL, _p_Screen->pixels,
                      _p_Screen->pitch);
    SDL_RenderClear(_p_sdlRenderer);
    SDL_RenderCopy(_p_sdlRenderer, _p_ScreenTexture, NULL, NULL);
    SDL_RenderPresent(_p_sdlRenderer);
}

LPErrInApp AppGfx::menuSelectDeck() {
    // CustomMenu *menuDecks = new CustomMenu;
    // menuDecks->Initialize(_p_Screen, _p_sdlRenderer,
    // lpszBackGroundFile); int menuMazziw = 445; int menuMazzih = 500;
    // int menuMazzix = (_p_Screen->w - menuMazziw) / 2;
    // int menuMazziy = _p_Screen->h - menuMazzih - 10;
    // menuDecks->SetArea(menuMazzix, menuMazziy, menuMazziw,
    // menuMazzih); menuDecks->SetColors(SDL_MapRGBA(_p_Screen->format,
    // 0, 0, 0, 0),
    //                      SDL_MapRGBA(_p_Screen->format, 128, 0, 0,
    //                      255));

    // menuDecks->ClearItems();
    // menuDecks->SetLabels(_LanguageMgr.GetStringId(Languages::ID_CHOOSEMAZZO),
    //                      _LanguageMgr.GetStringId(Languages::ID_BACKMAINMENU));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_PIACENTINA));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_BERGAMO));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_BOLOGNA));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_GENOVA));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_MILANO));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_NAPOLI));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_PIEMONTE));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_ROMAGNA));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_SARDEGNA));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_SICILIA));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_TOSCANA));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_TRENTO));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_TREVISO));
    // menuDecks->AddItems(_LanguageMgr.GetStringId(Languages::ID_TRIESTE));

    // int result2;
    // LPErrInApp err = menuDecks->Run(result2);
    // if (err != NULL)
    //     return err;
    // TRACE("Menu deck selected %d", result2);
    // switch (result2) {
    //     case 0:
    //         // piacentine
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::PIACENTINA);
    //         break;
    //     case 1:
    //         // bergamo
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::BERGAMO);
    //         break;
    //     case 2:
    //         // bologna
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::BOLOGNA);
    //         break;
    //     case 3:
    //         // genova
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::GENOVA);
    //         break;
    //     case 4:
    //         // milano
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::MILANO);
    //         break;
    //     case 5:
    //         // napoli
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::NAPOLI);
    //         break;
    //     case 6:
    //         // piemonte
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::PIEMONTE);
    //         break;
    //     case 7:
    //         // romagna
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::ROMAGNA);
    //         break;
    //     case 8:
    //         // sardegna
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::SARDEGNA);
    //         break;
    //     case 9:
    //         // toscana
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::TOSCANA);
    //         break;
    //     case 10:
    //         // sicilia
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::SICILIA);
    //         break;
    //     case 11:
    //         // trento
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::TRENTO);
    //         break;
    //     case 12:
    //         // treviso
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::TREVISO);
    //         break;
    //     case 13:
    //         // trieste
    //         _p_GameSettings->deckTypeVal.SetType(DeckType::TRIESTE);
    //         break;
    // }
    // delete menuDecks;
    return NULL;
}

void AppGfx::ParseCmdLine(int argc, char *argv[]) {
    _bOverride = FALSE;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf(
                "Solitario versione %s (c) 2004-2022 Invido.it\nFai "
                "partire "
                "il "
                "solitario con le seguenti opzioni:\n"
                "--nosound      - to disable sound/music\n"
                "--fullscreen   - to run in fullscreen, if possible "
                "(vs. "
                "windowed)\n"
                "--size x,y Fa partire il programma ad una risoluzione "
                "x,y \n",
                VERSION);

            exit(0);
        } else if (strcmp(argv[i], "--copyright") == 0 ||
                   strcmp(argv[i], "-c") == 0) {
            printf(
                "\n\"Solitario\" version %s, Copyright (C) 2004-2022 "
                "Invido.it\n"
                "This program is free software; you can redistribute "
                "it "
                "and/or\n"
                "modify it under the terms of the GNU General Public "
                "License\n"
                "as published by the Free Software Foundation.  See "
                "COPYING.txt\n"
                "\n"
                "This program is distributed in the hope that it will "
                "be "
                "useful,\n"
                "but WITHOUT ANY WARRANTY; without even the implied "
                "warranty "
                "of\n"
                "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
                "\n",
                VERSION);

            exit(0);
        } else if (strcmp(argv[i], "--usage") == 0 ||
                   strcmp(argv[i], "-u") == 0) {
            usage(0, argv[0]);
        } else if (strcmp(argv[i], "--fullscreen") == 0 ||
                   strcmp(argv[i], "-f") == 0) {
            _bFullScreen = TRUE;
        } else if (strcmp(argv[i], "--nosound") == 0 ||
                   strcmp(argv[i], "--quiet") == 0 ||
                   strcmp(argv[i], "-q") == 0) {
            _p_GameSettings->bMusicEnabled = false;
            _bOverride = TRUE;
        } else if (strcmp(argv[i], "--version") == 0 ||
                   strcmp(argv[i], "-v") == 0) {
            printf("Solitario versione %s\n", VERSION);
            exit(0);
        } else if (strcmp(argv[i], "--size") == 0 ||
                   strcmp(argv[i], "-s") == 0) {
            if (i >= argc - 1) {
                fprintf(stderr, "%s opzione  richiede 2 argomenti\n", argv[i]);
                usage(1, argv[0]);
            } else {
                if (parseScreenSize(argv[i + 1])) {
                } else {
                    fprintf(stderr, "Parametri non corretti: %s\n",
                            argv[i + 1]);
                    usage(1, argv[0]);
                    exit(1);
                }
                i++;
            }
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(1, argv[0]);
        }
    }
}

BOOL AppGfx::parseScreenSize(LPCSTR strInput) {
    char strBuffer[2048];
    memset(strBuffer, 0, 2048);
    char seps[] = " ,\t\n";
    char *token;
    VCT_STRINGS vct_String;
    BOOL bRet = FALSE;

    int iNumChar = strlen(strInput);
    strncpy(strBuffer, strInput, iNumChar);
    token = strtok(strBuffer, seps);
    while (token != NULL) {
        vct_String.push_back(token);
        token = strtok(NULL, seps);
    }

    int iNumElemntArr = vct_String.size();

    if (iNumElemntArr == 2) {
        sscanf((LPCSTR)vct_String[0].c_str(), "%d", &_iScreenW);
        sscanf((LPCSTR)vct_String[1].c_str(), "%d", &_iScreenH);
        bRet = TRUE;
    }
    return bRet;
}

void AppGfx::usage(int err, char *cmd) {
    FILE *f;

    if (err == 0)
        f = stdout;
    else
        f = stderr;

    fprintf(f,
            "\nUsage: %s {--help | --usage | --copyright}\n"
            "       %s [--fullscreen] [--nosound] [--size x,y] "
            "\n",
            cmd, cmd);

    exit(err);
}
