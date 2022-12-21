#ifndef __ERROR_INFO_INC
#define __ERROR_INFO_INC

// #include <memory>
#include <string>

typedef struct ErrInApp {
    std::string ErrorText;
} ErrInApp, *LPErrInApp;

namespace ERR_UTIL {
    // template <typename... Args>
    // LPErrInApp ErrorCreate(const std::string& format, Args... args);
    LPErrInApp ErrorCreate(const char* errtext);
    LPErrInApp ErrorCreate(const char* format, const char* errtext);
};

#endif