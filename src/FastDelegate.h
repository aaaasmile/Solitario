#ifndef FASTDELEGATE__H_
#define FASTDELEGATE__H_

#include "Languages.h"

namespace fastdelegate {
    typedef struct {
        void (*const SetNextMenu)(int iVal);
        void (*const LeaveMenu)();
        TTF_Font* (*const GetFontVera)();
        TTF_Font* (*const GetFontAriblk)();
        Languages* (*const GetLanguageMan)();
    } MenuDelegator, *LPMenuDelegator;

    typedef struct {
        void (*const Click)(int iButID);
    } CLICKEVENT, *LPCLICKEVENT;
};

#endif