#pragma once

#include <string>
#include <cstdint>
#include <WolframLibrary.h>

class Utils
{
public:
    static std::string trim(const std::string& str, const std::string& whitespace = " \t");
    static bool icasecmp(const std::string& l, const std::string& r);
    static std::uint8_t HexCharToInt(char c);
    static int ConvertToInt32Clamp(mint i);
};