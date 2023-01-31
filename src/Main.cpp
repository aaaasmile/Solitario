#include <stdio.h>
#include <stdlib.h>

#include "AppGfx.h"
#include "ErrorInfo.h"

int main(int argc, char* argv[]) {
    TRACE("[Main] - start\n");
    AppGfx* app = new AppGfx();
    app->ParseCmdLine(argc, argv);
    LPErrInApp err = app->Init();
    if (err != NULL) {
        printf("Init error: %s\n", err->ErrorText.c_str());
        exit(1);
    }
    err = app->MainLoop();
    if (err != NULL) {
        printf("App error: %s\n", err->ErrorText.c_str());
        exit(1);
    }

    return EXIT_SUCCESS;
}
