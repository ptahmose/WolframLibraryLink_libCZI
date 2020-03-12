#include "WolframLibrary.h"
#include "stringReturnHelper.h"
#include "CziInstanceManager.h"
#include "stringReturnHelper.h"

using namespace std;

static const char* LibraryExpressionNameCziReader = "CZIReader";

static void manage_czi_instance(WolframLibraryData libData, mbool mode, mint id)
{
    if (mode == 0)  // creation
    {
        CziReaderManager::Instance.AddInstance(id);
    }
    else if (mode == 1)
    {
        CziReaderManager::Instance.RemoveInstance(id);
    }
}

EXTERN_C DLLEXPORT mint WolframLibrary_getVersion( )
{
    return (WolframLibraryVersion);
}

EXTERN_C DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData)
{
    int r = libData->registerLibraryExpressionManager(LibraryExpressionNameCziReader, manage_czi_instance);
    return r;
}

EXTERN_C DLLEXPORT void WolframLibrary_uninitialize(WolframLibraryData libData)
{
    g_stringReturnHelper.Clear();
    int r = libData->unregisterLibraryExpressionManager(LibraryExpressionNameCziReader);
}

EXTERN_C DLLEXPORT int CZIReader_Open(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 2) 
    {
        return LIBRARY_FUNCTION_ERROR; 
    }

    mint id = MArgument_getInteger(Args[0]);
    auto reader = CziReaderManager::Instance.GetInstance(id);
    char* filename = MArgument_getUTF8String(Args[1]);
    //libData->Message("rankerror");
    

    reader->Open(filename);

    libData->UTF8String_disown(filename);
    return LIBRARY_NO_ERROR;
}

EXTERN_C DLLEXPORT int CZIReader_GetInfo(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 1)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    mint id = MArgument_getInteger(Args[0]);

    auto reader = CziReaderManager::Instance.GetInstance(id);
    string s = reader->GetInfo();

    g_stringReturnHelper.StoreString(s);
    MArgument_setUTF8String(res, g_stringReturnHelper.GetStoredString());

    return LIBRARY_NO_ERROR;
}

EXTERN_C DLLEXPORT int CZIReader_GetSubBlockBitmap(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 2)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    mint id = MArgument_getInteger(Args[0]);
    mint blockNo = MArgument_getInteger(Args[1]);

    auto reader = CziReaderManager::Instance.GetInstance(id);
    
    auto out = reader->GetSubBlockImage(libData, blockNo);
    MArgument_setMImage(res, out);

    return LIBRARY_NO_ERROR;
}