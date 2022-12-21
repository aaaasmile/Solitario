#include "error_info.h"

// template <typename... Args>
// LPErrInApp errorCreateInternal(const std::string& format, Args... args) {
//     LPErrInApp res = new ErrInApp();

//     int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
//     if (size_s <= 0) {
//         res->ErrorText = "Error during formatting";
//     }
//     auto size = static_cast<size_t>(size_s);
//     std::unique_ptr<char[]> buf(new char[size]);
//     std::snprintf(buf.get(), size, format.c_str(), args...);

//     res->ErrorText = std::string(buf.get(), buf.get() + size - 1);

//     return res;
// }

LPErrInApp ERR_UTIL::ErrorCreate(const char* errtext) {
    LPErrInApp res = new ErrInApp();
    res->ErrorText = errtext;
    return res;
}

LPErrInApp ERR_UTIL::ErrorCreate(const char* format, const char* errtext) {
    LPErrInApp res = new ErrInApp();
    char buff[1024];
    snprintf(buff, sizeof(buff), format, errtext);
    res->ErrorText = buff;
    return res;
}
