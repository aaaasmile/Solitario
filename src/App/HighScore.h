#ifndef _HIGHSCORE_H__
#define _HIGHSCORE_H__

#include <string>

#include "ErrorInfo.h"

typedef struct {
    std::string Name;
    uint16_t Score;
    uint8_t NumCard;
} ScoreInfo;

class HighScore {
public:
    HighScore();
    LPErrInApp Load();
    LPErrInApp Save();

private:
    ScoreInfo _scoreInfo[10];
};

#endif
