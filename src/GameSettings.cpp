#include "GameSettings.h"

GameSettings* g_p_GameSettings;

GameSettings* GAMESET::GetSettings() {
    if (g_p_GameSettings == NULL) {
        g_p_GameSettings = new GameSettings();
    }
    return g_p_GameSettings;
}
