#include <WolframLibrary.h>
#include "../inc/exportedFunctions.h"
#include "stringReturnHelper.h"
#include "CziInstanceManager.h"
#include "errorhelper.h"
#include "dbgprint.h"
#include "inc_libCzi.h"

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

mint WolframLibrary_getVersion()
{
    DBGPRINT((CDbg::Level::Trace, "WolframLibrary_getVersion: Enter"));
    return (WolframLibraryVersion);
}

int WolframLibrary_initialize(WolframLibraryData libData)
{
#if _WIN32API
    static bool alreadyInitialized = false;
    if (!alreadyInitialized)
    {
        // on Windows, prefer to use the Microsoft-WIC-JPGXR-codec
        const auto site = libCZI::GetDefaultSiteObject(libCZI::SiteObjectType::WithWICDecoder);
        if (site != nullptr)
        {
            libCZI::SetSiteObject(site);
        }

        alreadyInitialized = true;
    }
#endif
    int r = libData->registerLibraryExpressionManager(LibraryExpressionNameCziReader, manage_czi_instance);
    return r;
}

void WolframLibrary_uninitialize(WolframLibraryData libData)
{
    g_stringReturnHelper.Clear();
    int r = libData->unregisterLibraryExpressionManager(LibraryExpressionNameCziReader);
}

int CZIReader_ReleaseInstance(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 1) return LIBRARY_FUNCTION_ERROR;
    mint id = MArgument_getInteger(Args[0]);
    return libData->releaseManagedLibraryExpression(LibraryExpressionNameCziReader, id);
}
