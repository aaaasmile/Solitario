#include "HightScoreMgr.h"

#include <stdlib.h>

#include "StdAfx.h"
#ifdef _WINDOWS
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>

using namespace std;

HightScoreMgr::HightScoreMgr() {}

HightScoreMgr::~HightScoreMgr() {}

void HightScoreMgr::Save() {
    int f;
    char buffer[16];
    unsigned int score, k, nb;
    string name;

    f = open("scores.bin", O_TRUNC | O_WRONLY | O_CREAT);

    if (f > 0) {
        for (k = 0; k < 10; k++) {
            score = HS_Scores[k];
            memset(buffer, 0, 16);
            memcpy(buffer, HS_Names[k].c_str(), 15);
            nb = write(f, &score, 4);
            nb = write(f, buffer, 15);
        }
        close(f);
    }
}

void HightScoreMgr::Load() {
    int f;
    char buffer[16];
    unsigned int score, k;
    string name;

    f = open("scores.bin", O_RDONLY);
    if (f > 0) {
        for (k = 0; k < 10; k++) {
            if (read(f, &score, 4) == 0)
                score = 0;
            if (read(f, buffer, 15) == 0)
                name = "";
            else
                name = buffer;
            HS_Scores[k] = score;
            HS_Names[k] = name;
        }
        close(f);
    } else {
        for (k = 0; k < 10; k++) {
            HS_Scores[k] = (10 - k) * 1000;
            HS_Names[k] = "Re dal sulitari";
        }
    }
}
