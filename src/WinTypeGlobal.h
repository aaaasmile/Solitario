#ifndef _WINTYPEGLOBAL__H_
#define _WINTYPEGLOBAL__H_

#if _MSC_VER > 1000
#include <StdAfx.h>
#endif

#ifdef USEDIALOGTRACE
#include <iostream>
#endif

#if _MSC_VER > 1000
#pragma warning(disable : 4786)
#pragma warning(disable : 4996)
#include <windows.h>
#endif

#include <stdarg.h>

#include <deque>
#include <string>
#include <vector>

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef STRING
typedef std::string STRING;
#endif

#ifndef LPCSTR
typedef const char *LPCSTR, *PCSTR;
#endif

#ifndef TRACE
#include <stdio.h>
extern void TraceInLogFile(char* myBuff);
#ifdef TRACEINDEBUGGER
inline void TRACE(const char* fmt, ...) {
    char myBuff[512];
    va_list args;

    va_start(args, fmt); /* Initialize variable arguments. */

    int result = vsprintf(myBuff, fmt, args);
#ifdef USEDIALOGTRACE
    std::cout << "[TR] " << myBuff;
#else
    ::OutputDebugString(myBuff);
    TraceInLogFile(myBuff);
#endif
}
#else
#ifdef USETRACE
inline void TRACE(const char* fmt, ...) {
    char myBuff[1024];
    va_list args;
    va_start(args, fmt);
    vsprintf(myBuff, fmt, args);
    printf("[TR]%s\n", myBuff);
}
#else
inline void TRACE(const char* fmt, ...) {}
#endif
#endif
#endif

typedef std::vector<std::string> VCT_STRING;

#endif
