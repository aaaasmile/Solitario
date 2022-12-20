// File         : main.cpp
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "EngineApp.h"
#include "StdAfx.h"

cEngineApp TheApp;

////////////////////////////////////////
//       main
/*!
// \param int argc :
// \param char *argv[] :
*/
int main(int argc, char *argv[]) {
    // app name and set the directory on installed application
#ifdef _WINDOWS
    std::string strAppName;
    strAppName = argv[0];
    int iPos = strAppName.find_last_of('\\');
    int iLen = strAppName.length();
    std::string strDirname = strAppName.substr(0, 38);
    ::SetCurrentDirectory(strDirname.c_str());
#endif

    TheApp.Setup(argc, argv);
    TheApp.Init();

    TheApp.MainMenu();

    return EXIT_SUCCESS;
}
