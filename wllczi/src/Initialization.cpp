#include "WolframLibrary.h"
#include "stringReturnHelper.h"

EXTERN_C DLLEXPORT mint WolframLibrary_getVersion( )
{
    return WolframLibraryVersion;
}

EXTERN_C DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData)
{
    int r = libData->registerLibraryExpressionManager("CZI", manage_instance);
    return r;
}

DLLEXPORT void WolframLibrary_uninitialize(WolframLibraryData libData)
{
    g_stringReturnHelper.Clear();
}