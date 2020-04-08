#pragma once

class CDbg
{
public:
    enum class Level
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };

    static void Print(Level lvl, const char* sz);

    static const char* LevelToString(Level lvl);
};