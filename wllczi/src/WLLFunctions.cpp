#include <WolframLibrary.h>
#include <WolframNumericArrayLibrary.h>
#include "../inc/exportedFunctions.h"
#include "getLibInfo.h"
#include "stringReturnHelper.h"
#include <string>
#include "CziInstanceManager.h"
#include "stringReturnHelper.h"
#include "errorhelper.h"
#include "WolframLibLinkUtils.h"

using namespace std;

int getLibraryInfo(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument Res)
{
    string s = CLibraryInfo::GetAllInfoAsJSON();
    g_stringReturnHelper.StoreString(s);
    MArgument_setUTF8String(Res, g_stringReturnHelper.GetStoredString());

    return LIBRARY_NO_ERROR;
}

int CZIReader_Open(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 2)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    mint id = MArgument_getInteger(Args[0]);
    auto reader = CziReaderManager::Instance.GetInstance(id);
    char* filename = MArgument_getUTF8String(Args[1]);

    try
    {
        reader->Open(filename);
    }
    catch (libCZI::LibCZIException & excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderOpenException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }
    catch (exception & excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderOpenException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    libData->UTF8String_disown(filename);
    return LIBRARY_NO_ERROR;
}

int CZIReader_GetInfo(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 1)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    mint id = MArgument_getInteger(Args[0]);

    std::shared_ptr<CziReader> reader;
    try
    {
        reader = CziReaderManager::Instance.GetInstance(id);
    }
    catch (out_of_range&)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderInstanceNotExisting(id).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    string s = reader->GetInfo();

    g_stringReturnHelper.StoreString(s);
    MArgument_setUTF8String(res, g_stringReturnHelper.GetStoredString());

    return LIBRARY_NO_ERROR;
}

int CZIReader_GetSubBlockBitmap(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 2)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    mint id = MArgument_getInteger(Args[0]);
    mint blockNo = MArgument_getInteger(Args[1]);

    std::shared_ptr<CziReader> reader;
    try
    {
        reader = CziReaderManager::Instance.GetInstance(id);
    }
    catch (out_of_range&)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderInstanceNotExisting(id).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    try
    {
        auto out = reader->GetSubBlockImage(libData, blockNo);
        MArgument_setMImage(res, out);
    }
    catch (exception & excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSubBlockBitmapException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}

int CZIReader_GetSingleChannelScalingTileComposite(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    // arguments:
    // 1st: ROI
    // 2nd: plane-coordinate
    // 3rd: zoom
    if (Argc != 4)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    mint id = MArgument_getInteger(Args[0]);
    std::shared_ptr<CziReader> reader;
    try
    {
        reader = CziReaderManager::Instance.GetInstance(id);
    }
    catch (out_of_range&)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderInstanceNotExisting(id).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    int roiValues[4];
    WolframNumericArrayLibrary_Functions naFuncs = libData->numericarrayLibraryFunctions;
    MNumericArray numArrayRegionOfInterest = MArgument_getMNumericArray(Args[1]);
    WolframLibLinkUtils::TryGetAsInt32(roiValues, sizeof(roiValues) / sizeof(roiValues[0]), numArrayRegionOfInterest, naFuncs);
    WolframNumericArrayLibrary_Functions naFuns = libData->numericarrayLibraryFunctions;

    return LIBRARY_FUNCTION_ERROR;
}