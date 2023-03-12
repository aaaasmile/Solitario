#ifndef _CURRTIME_H
#define _CURRTIME_H

#include "WinTypeGlobal.h"

class CurrentTime {
public:
    CurrentTime() { Reset(); }
    STRING ToString();
    STRING ToStringMinSec();
    bool Update();
    void Reset();
    int GetNumOfSeconds() { return _hour * 3600 + _min * 60 + _sec; };
    int GetDeltaFromLastUpdate() { return _deltaSec; }

private:
    int _sec;
    int _min;
    int _hour;
    int _deltaSec;
    uint32_t _startSecondTick;
};

#endif