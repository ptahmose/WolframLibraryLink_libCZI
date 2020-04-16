#pragma once

#include <string>
#include <functional>

class CLibraryInfo
{
private:
    static const char* VersionString;
    static const char* LibraryName;
public:
    static const char* KeyVersionString;
    static const char* KeyLibraryName;
    static const char* KeyCompilerIdentification;

    static void EnumKeys(std::function<bool(const char*)> enumFunc);

    static bool GetValue(const std::string& key, std::string& value);

    static std::string GetAllInfoAsJSON();
};