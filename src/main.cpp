#include <stdio.h>
#include <stdlib.h>

#include "AppGfx.h"
#include "ErrorInfo.h"

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
    LPErrInApp err = app->Init();
    if (err != NULL) {
        fprintf(stderr, "Init error: %s\n", err->ErrorText.c_str());
        exit(1);
    }
    err = app->StartMainMenu();
    if (err != NULL) {
        fprintf(stderr, "App error: %s\n", err->ErrorText.c_str());
        exit(1);
    }

    return EXIT_SUCCESS;
}
