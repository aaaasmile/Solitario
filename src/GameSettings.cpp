#include "GameSettings.h"

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _MSC_VER
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif

static LPGameSettings _p_GameSettings = NULL;
static char _settingsRootDir[1024] = "";

LPGameSettings GAMESET::GetSettings() {
    if (_p_GameSettings == NULL) {
        _p_GameSettings = new GameSettings();
    }
    return _p_GameSettings;
}

const char* GAMESET::GetHomeSolitarioFolder() {
    if (strlen(_settingsRootDir) > 0) {
        return _settingsRootDir;
    }
#ifdef WIN32
    sprintf(_settingsRootDir, "%s/%s/.solitario", getenv("HOMEDRIVE"),
            getenv("HOMEPATH"));
#else
    sprintf(_settingsRootDir, "%s/.solitario", getenv("HOME"));
#endif
    return _settingsRootDir;
}

LPErrInApp GAMESET::CreateHomeSolitarioFolderIfNotExists(bool& dirCreated) {
    struct stat st = {0};
    int io_res;
    dirCreated = false;
    GetHomeSolitarioFolder();
    if (stat(_settingsRootDir, &st) == -1) {
#ifdef WIN32
        io_res = mkdir(_settingsRootDir);
#else
        io_res = mkdir(_settingsRootDir, 0700);
#endif
        if (io_res == -1) {
            return ERR_UTIL::ErrorCreate("Cannot create dir %s",
                                         _settingsRootDir);
        } else {
            dirCreated = true;
        }
    }
    return NULL;
}
