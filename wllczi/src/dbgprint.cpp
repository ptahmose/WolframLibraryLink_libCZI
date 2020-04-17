#include "dbgprint.h"
#include <sstream>
#include <stdarg.h>

#if _WIN32API
 #include <Windows.h>
#endif

using namespace std;

void CDbg::vPrint(Level lvl, const char* sz, ...)
{
    char buf[4096];
    va_list ap;
    va_start(ap, sz);
    vsnprintf(buf, sizeof(buf), sz, ap);
    va_end(ap);
    CDbg::Print(lvl, buf);
}

void CDbg::Print(Level lvl, const char* sz)
{
    if (static_cast<int>(lvl) < WLLCZI_LOGLEVEL)
    {
        return;
    }

    stringstream ss;
    ss << CDbg::LevelToString(lvl) << ": " << sz;
#if _WIN32API
    OutputDebugStringA(ss.str().c_str());
#else
    puts(str().c_str());
#endif
}

/*static*/const char* CDbg::LevelToString(Level lvl)
{
    switch (lvl)
    {
    case Level::Trace:return "Trace";
    case Level::Debug:return "Debug";
    case Level::Info:return "Info";
    case Level::Warn:return "Warn";
    case Level::Error:return "Error";
    case Level::Fatal:return "Fatal";
    }

    return "Invalid";
}
