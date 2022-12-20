////////////////////////////////////////////////////////////////////////////////
// File         : win_type_global.h
// Project      : 
// Subsystem    : 
// Component    : 
// Author       : Igor Sarzi Sartori
// Description  : global type definition
////////////////////////////////////////////////////////////////////////////////
// History      : 01.10.2001   Igor Sarzi Sartori    Created


#ifndef _AIGLOBAL_TYPE_H
#define _AIGLOBAL_TYPE_H

#ifdef USEDIALOGTRACE
    #include <iostream>
#endif

#if _MSC_VER > 1000
    #pragma warning(disable:4786)
	#pragma warning(disable:4996)
    #include <deque>
    #include <valarray>
    #include <windows.h>
#else
    #include <deque>
    #include <valarray>
#endif

#ifndef BOOL 
    typedef int BOOL;
#endif

#ifndef BYTE
    typedef unsigned char BYTE;
#endif

#ifndef TRUE 
    #define TRUE 1
#endif

#ifndef FALSE 
    #define FALSE 0
#endif

#ifndef CHAR 
    #define CHAR char
#endif

#ifndef ASSERT
    #include <assert.h>
    #define ASSERT(f) \
	assert(f);
#endif

#ifndef LPCSTR
    typedef CONST CHAR *LPCSTR, *PCSTR;
#endif

#ifndef TRACE
    #include <stdio.h>
    #ifdef _WINDOWS
        inline void TRACE(const char* fmt, ...)
        {
            char myBuff[512];
            va_list args;

            va_start( args, fmt );     /* Initialize variable arguments. */

            int result = vsprintf(myBuff, fmt, args); 
#ifdef USEDIALOGTRACE
            std::cout << "[TR] " <<myBuff;
#else
            ::OutputDebugString(myBuff);
#endif
        }
    #else
	    inline void TRACE(const char* fmt, ...)
        {
     	    // todo: implement trace for anjuta
        }
    #endif
#endif



#endif