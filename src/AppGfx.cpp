#include "AppGfx.h"

#include <SDL_image.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "Config.h"
#include "ErrorInfo.h"
#include "GfxUtil.h"
#include "MainOptionGfx.h"
#include "MenuMgr.h"
#include "MusicManager.h"
#include "WinTypeGlobal.h"

#define OPT_WIN_GENERAL_WIDTH 500
#define OPT_WIN_GENERAL_HEIGHT 480

#include "Credits.h"
#include "Fading.h"
#include "libini.h"

static const char *g_lpszSolitarioDir = ".solitario";
static const char *g_lpszIniFileName = "options.ini";
static const char *g_lpszDefaultIniFileName = DATA_PREFIX "default_options.ini";
static const char *g_lpszHelpFileName = DATA_PREFIX "solitario.pdf";
static const char *g_lpszSectAll = "global";
static const char *g_lpszKeyLang = "Language";
static const char *g_lpszKeyDeck = "DeckCurrent";
static const char *g_lpszKeyMusic = "Musicenabled";

static const char *g_lpszIconProgFile = DATA_PREFIX "images/icona_asso.bmp";
static const char *g_lpszTitleFile = DATA_PREFIX "images/title.png";
static const char *g_lpszIniFontAriblk = DATA_PREFIX "font/ariblk.ttf";
static const char *g_lpszIniFontVera = DATA_PREFIX "font/vera.ttf";
static const char *g_lpszImageSplash = DATA_PREFIX "images/im001537.jpg";

AppGfx::AppGfx() {
    _p_Window = NULL;
    _p_ScreenTexture = NULL;
    _p_Screen = NULL;
    _iScreenW = 1024;
    _iScreenH = 768;
    _iBpp = 0;
    _p_MusicManager = 0;
    _p_CreditTitle = 0;
    _bFullScreen = false;
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

    _Languages.SetLang(_p_GameSettings->eLanguageCurrent);

    if (TTF_Init() == -1) {
        return ERR_UTIL::ErrorCreate("Font init error");
    }
    std::string strFileFontStatus = g_lpszIniFontAriblk;
    _p_fontAriblk = TTF_OpenFont(strFileFontStatus.c_str(), 22);
    if (_p_fontAriblk == 0) {
        return ERR_UTIL::ErrorCreate("Unable to load font %s, error: %s\n",
                                     strFileFontStatus.c_str(), SDL_GetError());
    }
    strFileFontStatus = g_lpszIniFontVera;
    _p_fontVera = TTF_OpenFont(strFileFontStatus.c_str(), 11);
    if (_p_fontVera == 0) {
        return ERR_UTIL::ErrorCreate("Unable to load font %s, error: %s\n",
                                     strFileFontStatus.c_str(), SDL_GetError());
    }

    const char *title = _Languages.GetCStringId(Languages::ID_SOLITARIO);
    SDL_SetWindowTitle(_p_Window, title);

    SDL_Surface *psIcon = SDL_LoadBMP(g_lpszIconProgFile);
    if (psIcon == 0) {
        return ERR_UTIL::ErrorCreate("Icon not found");
    }
    SDL_SetColorKey(psIcon, true, SDL_MapRGB(psIcon->format, 0, 128, 0));
    SDL_SetWindowIcon(_p_Window, psIcon);

#ifdef _GLIBCPP_HAVE_DRAND48
    srand48((unsigned)time(0));
#else
    srand((unsigned)time(0));
#endif

    _p_CreditTitle = IMG_Load(g_lpszTitleFile);
    if (_p_CreditTitle == 0) {
        return ERR_UTIL::ErrorCreate("Title image not found");
    }
    err = loadSceneBackground();
    if (err != NULL) {
        return err;
    }

    drawSceneBackground();

    err = _p_MusicManager->LoadMusicRes();
    if (err != NULL) {
        return err;
    }
    return NULL;
}

LPErrInApp AppGfx::loadSceneBackground() {
    std::string strFileName = g_lpszImageSplash;

    SDL_RWops *srcBack = SDL_RWFromFile(strFileName.c_str(), "rb");
    if (srcBack == 0) {
        return ERR_UTIL::ErrorCreate("Unable to load %s background image\n",
                                     strFileName.c_str());
    }
    _p_SceneBackground = IMG_LoadJPG_RW(srcBack);
    if (_p_SceneBackground == 0) {
        return ERR_UTIL::ErrorCreate("Unable to create splash");
    }
    return NULL;
}

LPErrInApp AppGfx::createWindow() {
    int flagwin = 0;
    if (_p_Window != NULL) {
        _p_Window = NULL;
        SDL_DestroyWindow(_p_Window);
    }
    if (_p_Screen != NULL) {
        SDL_FreeSurface(_p_Screen);
        _p_Screen = NULL;
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

    err = _p_SolitarioGfx->Initialize(_p_Screen, _p_sdlRenderer, _p_Window,
                                      _p_GameSettings->deckTypeVal);
    if (err != NULL)
        return err;

    return _p_SolitarioGfx->StartGameLoop();
}

void AppGfx::terminate() {
    writeProfile();
    SDL_ShowCursor(SDL_ENABLE);

    if (_p_Screen != NULL) {
        SDL_FreeSurface(_p_Screen);
        _p_Screen = NULL;
    }
    if (_p_SceneBackground) {
        SDL_FreeSurface(_p_SceneBackground);
        _p_SceneBackground = NULL;
    }
    if (_p_CreditTitle) {
        SDL_FreeSurface(_p_CreditTitle);
        _p_CreditTitle = NULL;
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

LPErrInApp CopyFile(const char *src_path, const char *dst_path) {
    TRACE("Copy file from %s to %s\n", src_path, dst_path);

    int src_fd, dst_fd, n, io_res;
    unsigned char buffer[4096];
    src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) {
        return ERR_UTIL::ErrorCreate("Cannot open file for read  %s", src_path);
    }
    dst_fd = open(dst_path, O_CREAT | O_WRONLY | O_EXCL, 0666);
    if (dst_fd == -1) {
        return ERR_UTIL::ErrorCreate("Cannot open file for write  %s",
                                     dst_path);
    }

    while (1) {
        io_res = read(src_fd, buffer, 4096);
        if (io_res == -1) {
            return ERR_UTIL::ErrorCreate("Error reading file %s.\n", src_path);
        }
        n = io_res;

        if (n == 0)
            break;

        io_res = write(dst_fd, buffer, n);
        if (io_res == -1) {
            return ERR_UTIL::ErrorCreate("Error writing to file.\n");
        }
    }
    close(src_fd);
    close(dst_fd);

    return NULL;
}

LPErrInApp AppGfx::loadProfile() {
    struct stat st = {0};
    LPErrInApp err;
    int io_res;

    char dirpath[PATH_MAX - strlen(g_lpszIniFileName)];
    char filepath[PATH_MAX];

    snprintf(dirpath, sizeof(dirpath), "%s/%s", getenv("HOME"),
             g_lpszSolitarioDir);

    if (stat(dirpath, &st) == -1) {
        io_res = mkdir(dirpath, 0700);
        if (io_res == -1) {
            return ERR_UTIL::ErrorCreate("Cannot create dir %s", dirpath);
        }
        TRACE("Create dir %s\n", dirpath);
    }
    snprintf(filepath, PATH_MAX, "%s/%s", dirpath, g_lpszIniFileName);

    if (stat(filepath, &st) == -1) {
        err = CopyFile(g_lpszDefaultIniFileName, filepath);
        if (err != NULL) {
            return err;
        }
        TRACE("Default ini file created in %s\n", filepath);
    }

    ini_fd_t pIni = ini_open(filepath, "r", "#");

    if (pIni == NULL)
        return ERR_UTIL::ErrorCreate("loadProfile: Ini file error %s",
                                     filepath);

    int iVal;

    // deck type
    iVal = _p_GameSettings->deckTypeVal.GetTypeIndex();
    if (pIni) {
        ini_locateHeading(pIni, g_lpszSectAll);
        ini_locateKey(pIni, g_lpszKeyDeck);
        ini_readInt(pIni, &iVal);
    }
    _p_GameSettings->deckTypeVal.SetTypeIndex(iVal);
    // language
    iVal = _p_GameSettings->eLanguageCurrent;
    if (pIni) {
        ini_locateHeading(pIni, g_lpszSectAll);
        ini_locateKey(pIni, g_lpszKeyLang);
        ini_readInt(pIni, &iVal);
    }
    _p_GameSettings->eLanguageCurrent = (Languages::eLangId)iVal;
    // music
    iVal = false;
    if (pIni) {
        ini_locateHeading(pIni, g_lpszSectAll);
        ini_locateKey(pIni, g_lpszKeyMusic);
        ini_readInt(pIni, &iVal);
    }
    if (!_bOverride) {
        _p_GameSettings->bMusicEnabled = iVal;
    }

    ini_close(pIni);
    return NULL;
}

void AppGfx::writeProfile() {
    char filepath[PATH_MAX];
    snprintf(filepath, PATH_MAX, "%s/%s/%s", getenv("HOME"), g_lpszSolitarioDir,
             g_lpszIniFileName);

    ini_fd_t pIni = ini_open(filepath, "w", "#");
    if (pIni == 0)
        return;

    // deck type
    ini_locateHeading(pIni, g_lpszSectAll);
    ini_locateKey(pIni, g_lpszKeyDeck);
    ini_writeInt(pIni, (int)_p_GameSettings->deckTypeVal.GetTypeIndex());

    // language
    ini_locateHeading(pIni, g_lpszSectAll);
    ini_locateKey(pIni, g_lpszKeyLang);
    ini_writeInt(pIni, _p_GameSettings->eLanguageCurrent);

    // music
    ini_locateHeading(pIni, g_lpszSectAll);
    ini_locateKey(pIni, g_lpszKeyMusic);
    ini_writeInt(pIni, _p_GameSettings->bMusicEnabled);

    ini_close(pIni);
    TRACE("Settings file %s written", filepath);
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

void fncBind_PersistSettings(void *self) {
    AppGfx *pApp = (AppGfx *)self;
    pApp->PersistSettings();
}

MenuDelegator AppGfx::prepMenuDelegator() {
    // Use only static otherwise you loose it
    static VMenuDelegator const tc = {
        .GetFontVera = (&fncBind_GetFontVera),
        .GetFontAriblk = (&fncBind_GetFontAriblk),
        .GetLanguageMan = (&fncBind_GetLanguageMan),
        .LeaveMenu = (&fncBind_LeaveMenu),
        .SetNextMenu = (&fncBind_SetNextMenu),
        .PersistSettings = (&fncBind_PersistSettings)};

    return (MenuDelegator){.tc = &tc, .self = this};
}

void AppGfx::LeaveMenu() {
    drawSceneBackground();
    _Histmenu.pop();
}

void AppGfx::PersistSettings() {
    TRACE("Persist settings");
    writeProfile();
}

void AppGfx::drawSceneBackground() {
    TRACE("drawSceneBackground %dx%d", _p_SceneBackground->w,
          _p_SceneBackground->h);
    SDL_FillRect(_p_Screen, &_p_Screen->clip_rect,
                 SDL_MapRGBA(_p_Screen->format, 0, 0, 0, 0));
    SDL_Rect rctTarget;
    rctTarget.x = (_p_Screen->w - _p_SceneBackground->w) / 2;
    rctTarget.y = (_p_Screen->h - _p_SceneBackground->h) / 2;
    rctTarget.w = _p_SceneBackground->w;
    rctTarget.h = _p_SceneBackground->h;

    SDL_BlitSurface(_p_SceneBackground, NULL, _p_Screen, &rctTarget);
    updateScreenTexture();
}

LPErrInApp AppGfx::MainLoop() {
    LPErrInApp err;
    bool bquit = false;

    MenuMgr *pMenuMgr = new MenuMgr();
    MenuDelegator delegator = prepMenuDelegator();
    pMenuMgr->Initialize(_p_Screen, _p_sdlRenderer, delegator);

    // set main menu
    _Histmenu.push(MenuMgr::QUITAPP);
    _Histmenu.push(MenuMgr::MENU_ROOT);

    pMenuMgr->SetBackground(_p_SceneBackground);

    while (!bquit && !_Histmenu.empty()) {
        switch (_Histmenu.top()) {
            case MenuMgr::MENU_ROOT:
                if (_p_GameSettings->bMusicEnabled &&
                    !_p_MusicManager->IsPLayingMusic()) {
                    _p_MusicManager->PlayMusic(MusicManager::MUSIC_INIT_SND,
                                               MusicManager::LOOP_ON);
                }
                err = pMenuMgr->HandleRootMenu();
                if (err != NULL)
                    return err;

                break;

            case MenuMgr::MENU_GAME:
                err = startGameLoop();
                if (err != NULL)
                    return err;
                LeaveMenu();
                break;

            case MenuMgr::MENU_HELP:
                err = showHelp();
                if (err != NULL)
                    return err;
                break;

            case MenuMgr::MENU_CREDITS:
                err = showCredits();
                if (err != NULL)
                    return err;
                break;

            case MenuMgr::MENU_OPTIONS:
                err = showOptionGeneral();
                if (err != NULL)
                    return err;
                break;

            case MenuMgr::QUITAPP:
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
    const char *cmd = NULL;
    char cmdpath[PATH_MAX];

    cmd = "zathura";
    snprintf(cmdpath, sizeof(cmdpath), "%s ./%s", cmd, g_lpszHelpFileName);

    system(cmdpath);

    LeaveMenu();
    return NULL;
}

LPErrInApp AppGfx::showCredits() {
    if (_p_MusicManager->IsPLayingMusic()) {
        _p_MusicManager->StopMusic();
        _p_MusicManager->PlayMusic(MusicManager::MUSIC_GAME_SND,
                                   MusicManager::LOOP_ON);
    }

    credits(_p_Screen, _p_CreditTitle, _p_sdlRenderer);
    LeaveMenu();
    if (_p_MusicManager->IsPLayingMusic()) {
        _p_MusicManager->StopMusic();
        _p_MusicManager->PlayMusic(MusicManager::MUSIC_INIT_SND,
                                   MusicManager::LOOP_ON);
    }
    return NULL;
}

LPErrInApp AppGfx::showOptionGeneral() {
    TRACE("Show option general");
    MainOptionGfx optGfx;

    SDL_Rect rctOptionWin;
    rctOptionWin.w = OPT_WIN_GENERAL_WIDTH;
    rctOptionWin.h = OPT_WIN_GENERAL_HEIGHT;
    rctOptionWin.x = (_p_Screen->w - rctOptionWin.w) / 2;
    rctOptionWin.y = (_p_Screen->h - rctOptionWin.h) / 2;
    MenuDelegator delegator = prepMenuDelegator();
    STRING caption = _Languages.GetStringId(Languages::ID_MEN_OPTIONS);
    LPErrInApp err = optGfx.Initialize(&rctOptionWin, _p_Screen, _p_sdlRenderer,
                                       _p_MusicManager, delegator);
    if (err) {
        return err;
    }
    optGfx.Show(_p_SceneBackground, caption);

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

void AppGfx::updateScreenTexture() {
    SDL_UpdateTexture(_p_ScreenTexture, NULL, _p_Screen->pixels,
                      _p_Screen->pitch);
    SDL_RenderClear(_p_sdlRenderer);
    SDL_RenderCopy(_p_sdlRenderer, _p_ScreenTexture, NULL, NULL);
    SDL_RenderPresent(_p_sdlRenderer);
}

void AppGfx::ParseCmdLine(int argc, char *argv[]) {
    _bOverride = false;

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
                "\n\"Solitario\" version %s, Copyright (C) 2004-2023 "
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
            _bFullScreen = true;
        } else if (strcmp(argv[i], "--nosound") == 0 ||
                   strcmp(argv[i], "--quiet") == 0 ||
                   strcmp(argv[i], "-q") == 0) {
            _p_GameSettings->bMusicEnabled = false;
            _bOverride = true;
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
                }
                i++;
            }
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(1, argv[0]);
        }
    }
}

bool AppGfx::parseScreenSize(LPCSTR strInput) {
    char strBuffer[2048];
    memset(strBuffer, 0, 2048);
    char seps[] = " ,\t\n";
    char *token;
    VCT_STRING vct_String;
    bool bRet = false;

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
        bRet = true;
    }
    return bRet;
}

void AppGfx::usage(int errOut, char *cmd) {
    FILE *f;

    if (errOut == 0)
        f = stdout;
    else
        f = stderr;

    fprintf(f,
            "\nUsage: %s {--help | --usage | --copyright}\n"
            "       %s [--fullscreen] [--nosound] [--size x,y] "
            "\n",
            cmd, cmd);

    exit(errOut);
}
