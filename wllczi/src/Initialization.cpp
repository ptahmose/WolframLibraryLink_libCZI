#include "WolframLibrary.h"

EXTERN_C DLLEXPORT mint WolframLibrary_getVersion( )
{
    return WolframLibraryVersion;
}

EXTERN_C DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData)
{
    return 0;
}