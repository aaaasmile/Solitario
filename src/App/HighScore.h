#ifndef _HIGHSCORE_H__
#define _HIGHSCORE_H__

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
