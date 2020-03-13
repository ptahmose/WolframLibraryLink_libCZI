#include "WolframLibrary.h"
#include "../inc/exportedFunctions.h"
#include "getLibInfo.h"
#include "stringReturnHelper.h"
#include <string>

using namespace std;

int getLibraryInfo(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument Res)
{
    string s = CLibraryInfo::GetAllInfoAsJSON();
    g_stringReturnHelper.StoreString(s);
    MArgument_setUTF8String(Res, g_stringReturnHelper.GetStoredString());

    return LIBRARY_NO_ERROR;
}
