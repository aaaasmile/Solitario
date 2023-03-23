#include "HighScore.h"

#include <fcntl.h>
#include <memory.h>
#include <stdlib.h>

#ifdef _WINDOWS
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#else
#include <unistd.h>
#endif

using namespace std;

static const char *g_lpszScore = "solitario_score.bin";

HighScore::HighScore() {
    for (int k = 0; k < 10; k++) {
        _scoreInfo[k].Score = 3450 - (k * 150);
        _scoreInfo[k].Name = "Re dal sulitari";
        _scoreInfo[k].NumCard = 40;
    }
}

LPErrInApp HighScore::Save() {
    int f;
    char filepath[PATH_MAX + strlen(g_lpszScore)];

    f = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, O_WRONLY);

    if (f > 0) {
        for (int k = 0; k < 10; k++) {
            char buffer[16];
            uint16_t score = _scoreInfo[k].Score;
            memset(buffer, 0, 16);
            memcpy(buffer, _scoreInfo[k].Name.c_str(), 15);
            int nb = write(f, &score, 2);
            if (nb == -1) {
                return ERR_UTIL::ErrorCreate("Error in write for score");
            }
            nb = write(f, buffer, 15);
            if (nb == -1) {
                return ERR_UTIL::ErrorCreate("Error in write for buffer");
            }
        }
        close(f);
    } else {
        return ERR_UTIL::ErrorCreate(
            "Unable to open for writing the score file '%s'", filepath);
    }
    return NULL;
}

LPErrInApp HighScore::Load() {
    int f;
    char filepath[PATH_MAX + strlen(g_lpszScore)];

    // snprintf(filepath, sizeof(filepath), "%s/%s",
    // pGameSettings->SettingsDir,
    //          g_lpszScore);

    f = open(filepath, O_RDONLY);
    if (f > 0) {
        for (int k = 0; k < 10; k++) {
            int score = 0;
            char buffer[16];
            read(f, &score, 4);
            char buffer[16];
            memset(buffer, 0, 16);
            read(f, buffer, 15);
            string name = buffer;
            _scoreInfo[k].Name = name;
            _scoreInfo[k].Score = score;
        }
        close(f);
    } else {
        return ERR_UTIL::ErrorCreate(
            "Unable to open for writing the score file '%s'", filepath);
    }
    return NULL;
}
