#ifndef TRAITS_FASTDELEGATE__H_
#define TRAITS_FASTDELEGATE__H_

#include <SDL_ttf.h>

#include "Languages.h"

namespace traits {
    // trait for menu
    typedef struct {
        TTF_Font* (*const GetFontVera)(void* self);
        TTF_Font* (*const GetFontAriblk)(void* self);
        Languages* (*const GetLanguageMan)(void* self);
        void (*const LeaveMenu)(void* self);
        void (*const SetNextMenu)(void* self, int iVal);
    } VMenuDelegator, *LPVMenuDelegator;

    typedef struct {
        VMenuDelegator const* tc;
        void* self;
    } MenuDelegator, *LPMenuDelegator;

    // trait for button click
    typedef struct {
        void (*const Click)(void* self, int iButID);
    } VClickCb, *LPVClickCb;

    typedef struct {
        VClickCb const* tc;
        void* self;
    } ClickCb, *LPClickCb;
};

#endif