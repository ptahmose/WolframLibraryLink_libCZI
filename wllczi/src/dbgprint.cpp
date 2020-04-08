#include "dbgprint.h"
#include <sstream>

#include <Windows.h>

using namespace std;

void CDbg::Print(Level lvl, const char* sz)
{
    stringstream ss;
    ss << CDbg::LevelToString(lvl) << ": " << sz;
    OutputDebugStringA(ss.str().c_str());
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
