#include "WolframLibrary.h"
//#include "wstp.h"
#include "getLibInfo.h"
#include "stringReturnHelper.h"
#include <string>

using namespace std;

/* Adds one to the input, returning the result  */
EXTERN_C DLLEXPORT int demo_I_I(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument Res)
{
    mint I0;
    mint I1;
    I0 = MArgument_getInteger(Args[0]);
    I1 = I0 + 1;
    MArgument_setInteger(Res, I1);
    return LIBRARY_NO_ERROR;
}

EXTERN_C DLLEXPORT int getLibraryInfo(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument Res)
{
    string s = CLibraryInfo::GetAllInfoAsJSON();
    g_stringReturnHelper.StoreString(s);
    MArgument_setUTF8String(Res, g_stringReturnHelper.GetStoredString());

    return LIBRARY_NO_ERROR;
}
