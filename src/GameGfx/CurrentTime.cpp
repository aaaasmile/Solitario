#include "CurrentTime.h"

#include <SDL.h>

STRING CurrentTime::ToString() {
    STRING strOut;
    char buff[128];
    sprintf(buff, "%02d:%02d:%02d", _hour, _min, _sec);
    strOut = buff;
    return strOut;
}

STRING CurrentTime::ToStringMinSec() {
    STRING strOut;
    char buff[128];
    sprintf(buff, "%02d:%02d", _min, _sec);
    strOut = buff;
    return strOut;
}

bool CurrentTime::IsMoreThenOneSecElapsed() {
    if (_updateStopped) {
        return false;
    }
    bool res = false;
    Uint32 currTick = SDL_GetTicks();
    Uint32 diff = currTick - _startSecondTick;
    Uint32 secGap = diff / 1000;
    _deltaSec = secGap;
    if (secGap > 0) {
        res = true;
        Uint32 remaind = diff % 1000;
        _startSecondTick = currTick - remaind;
        _sec += secGap;
        if (_sec > 59) {
            _sec = 0;
            _min++;
            if (_min > 59) {
                _min = 0;
                _hour++;
                if (_hour > 23) {
                    _hour = 0;
                }
            }
        }
    }
    return res;
}

void CurrentTime::Reset() {
    _sec = _min = _hour = 0;
    _updateStopped = false;
    _startSecondTick = SDL_GetTicks();
}
