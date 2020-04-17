#include <WolframLibrary.h>
#include "../inc/exportedFunctions.h"
#include "stringReturnHelper.h"
#include "CziInstanceManager.h"
#include "errorhelper.h"
#include "dbgprint.h"

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

mint WolframLibrary_getVersion( )
{
    DBGPRINT((CDbg::Level::Trace, "WolframLibrary_getVersion: Enter"));
    return (WolframLibraryVersion);
}

int WolframLibrary_initialize( WolframLibraryData libData)
{
    int r = libData->registerLibraryExpressionManager(LibraryExpressionNameCziReader, manage_czi_instance);
    return r;
}

void WolframLibrary_uninitialize(WolframLibraryData libData)
{
    g_stringReturnHelper.Clear();
    int r = libData->unregisterLibraryExpressionManager(LibraryExpressionNameCziReader);
}
