#include "WolframLibrary.h"
#include "stringReturnHelper.h"

static void manage_czi_instance(WolframLibraryData libData, mbool mode, mint id)
{
    if (mode == 0)  // creation
    {

    }
    else if (mode == 1)
    {

    }
}

EXTERN_C DLLEXPORT mint WolframLibrary_getVersion( )
{
    return WolframLibraryVersion;
}

EXTERN_C DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData)
{
    int r = libData->registerLibraryExpressionManager("CZI", manage_czi_instance);
    return r;
}

DLLEXPORT void WolframLibrary_uninitialize(WolframLibraryData libData)
{
    g_stringReturnHelper.Clear();
    int r = libData->unregisterLibraryExpressionManager("CZI");
}