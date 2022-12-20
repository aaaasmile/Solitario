#include <stdio.h>
#include <stdlib.h>

#include "AppGfx.h"
#include "StdAfx.h"

int main(int argc, char* argv[]) {
#ifdef _WINDOWS
    std::string strAppName;
    strAppName = argv[0];
    int iPos = strAppName.find_last_of('\\');
    int iLen = strAppName.length();
    std::string strDirname = strAppName.substr(0, 38);
    ::SetCurrentDirectory(strDirname.c_str());
#endif
    AppGfx* app = new AppGfx();
    app->ParseCmdLine(argc, argv);
    app->Init();
    app->MainMenu();

    return EXIT_SUCCESS;
}
