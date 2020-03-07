#include "WolframLibrary.h"

DLLEXPORT mint WolframLibrary_getVersion( ) 
{
    return WolframLibraryVersion;
}

DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData) 
{
    return 0;
}