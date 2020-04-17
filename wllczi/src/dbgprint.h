#pragma once

#include <inttypes.h>
#include "wllczi_Config.h"

class CDbg
{
public:
    enum class Level
    {
        Trace = 1,
        Debug = 2,
        Info = 3,
        Warn = 4,
        Error = 30,
        Fatal = 64
    };

    static void vPrint(Level lvl, const char* sz, ...);
    static void Print(Level lvl, const char* sz);

    static const char* LevelToString(Level lvl);
};

#if WLLCZI_ENABLELOGGING
 #define DBGPRINT( x )  do { CDbg::Print x; } while (0)
 #define VDBGPRINT( x )  do { CDbg::vPrint x; } while (0)
#else
 #define DBGPRINT( x )
 #define VDBGPRINT( x )
#endif

#ifdef MINT_32
    #define MINTFMT  PRId32
#else
    #define MINTFMT  PRId64 
#endif