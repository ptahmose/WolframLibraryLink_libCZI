#pragma once

#include <WolframLibrary.h>
#include <WolframNumericArrayLibrary.h>

class WolframLibLinkUtils
{
public:
    static bool TryGetAsInt32(int* out, size_t outCount, MNumericArray numArray, WolframNumericArrayLibrary_Functions naFuns);
};