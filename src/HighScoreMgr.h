#ifndef _HIGHTSCORE_H__
#define _HIGHTSCORE_H__

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#include <string>

#include "ErrorInfo.h"

class HighscoreMgr {
public:
    HighscoreMgr();
    virtual ~HighscoreMgr();
    void Load();
    LPErrInApp Save();

public:
    std::string HS_Names[10];
    unsigned int HS_Scores[10];
};

#endif
