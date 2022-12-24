#ifndef FASTDELEGATE__H_
#define FASTDELEGATE__H_

#include "Languages.h"

namespace fastdelegate {
    typedef struct {
        void (*SetNextMenu)(int iVal);
        void (*LeaveMenu)();
        TTF_Font* (*GetFontVera)();
        TTF_Font* (*GetFontAriblk)();
        Languages* (*GetLanguageMan)();
    } MenuDelegator, *LPMenuDelegator;

    typedef struct {
        void* self;
        MenuDelegator const* tc;
    } MenuDelegatorable;

    typedef struct {
        void (*Click)(int iButID);
    } CLICKEVENT, *LPCLICKEVENT;
};

#endif