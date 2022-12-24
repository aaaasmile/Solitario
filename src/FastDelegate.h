#ifndef FASTDELEGATE__H_
#define FASTDELEGATE__H_

#include "Languages.h"

namespace fastdelegate {
    typedef struct {
        void (*SetNextMenu)(int iVal);
        void (*LeaveMenu)();
        TTF_Font* (*const GetFontVera)(void* self);
        TTF_Font* (*GetFontAriblk)();
        Languages* (*GetLanguageMan)();
    } MenuDelegator, *LPMenuDelegator;

    typedef struct {
        MenuDelegator const* tc;
        void* self;
    } MenuDelegatorable, *LPMenuDelegatorable;

    typedef struct {
        void (*Click)(int iButID);
    } CLICKEVENT, *LPCLICKEVENT;
};

#endif