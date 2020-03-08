#pragma once

#include "WolframLibrary.h"
#include <string>

class CStringReturnHelper
{
private:
    char* returnedString;
    size_t sizeOfString;
public:
    CStringReturnHelper() :returnedString(nullptr), sizeOfString(0)
    {}

    void  StoreString(WolframLibraryData libData,const char* sz);
    void  StoreString(WolframLibraryData libData, const std::string& str)
    {
        this->StoreString(libData,str.c_str());
    }

    char* GetStoredString();

    void Clear(WolframLibraryData libData);
};

extern CStringReturnHelper g_stringReturnHelper;