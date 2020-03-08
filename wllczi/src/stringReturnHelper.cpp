#include "stringReturnHelper.h"
#include <string.h>
#include <malloc.h>

CStringReturnHelper g_stringReturnHelper;

void CStringReturnHelper::StoreString(const char* sz)
{
    size_t size = strlen(sz) + 1;
    if (size > this->sizeOfString)
    {
        this->returnedString = (char*)realloc(this->returnedString, size);
        // TODO: check for out-of-memory

        this->sizeOfString = size;
    }

    memcpy(this->returnedString, sz, size);
}

char* CStringReturnHelper::GetStoredString()
{
    return this->returnedString;
}

void CStringReturnHelper::Clear()
{
    this->returnedString = nullptr;
    this->sizeOfString = 0;
}
