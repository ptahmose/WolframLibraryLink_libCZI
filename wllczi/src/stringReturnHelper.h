#pragma once

#include <string>

class CStringReturnHelper
{
private:
    char* returnedString;
    size_t sizeOfString;
public:
    CStringReturnHelper() :returnedString(nullptr), sizeOfString(0)
    {}

    void  StoreString(const char* sz);
    void  StoreString(const std::string& str) 
    {
        this->StoreString(str.c_str());
    }

    char* GetStoredString();

    void Clear();
};

extern CStringReturnHelper g_stringReturnHelper;