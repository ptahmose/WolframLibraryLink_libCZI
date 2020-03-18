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
#include "inc_libCzi.h"

using namespace std;
using namespace libCZI;

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
    catch (libCZI::LibCZIException& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderOpenException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }
    catch (exception& excp)
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
        // TODO: check whether "blockNo" is a valid "int"
        auto out = reader->GetSubBlockImage(libData, blockNo);
        MArgument_setMImage(res, out);
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSubBlockBitmapException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}

int CZIReader_GetSingleChannelScalingTileComposite(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    // arguments:
    // 1st: ROI               -> NumericArray of size 4
    // 2nd: plane-coordinate  -> a string (of form "T3C2Z32")
    // 3rd: zoom              -> A float number
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

    char* coordinateString = MArgument_getUTF8String(Args[2]);
    CDimCoordinate planeCoordinate;
    try
    {
        planeCoordinate = CDimCoordinate::Parse(coordinateString);
    }
    catch (libCZI::LibCZIStringParseException& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(coordinateString, excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }
    catch (exception& excp)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    mreal zoom = MArgument_getReal(Args[3]);

    try
    {
        auto out = reader->GetSingleChannelScalingTileComposite(
            libData,
            IntRect{ roiValues[0],roiValues[1],roiValues[2],roiValues[3] },
            &planeCoordinate,
            (float)zoom);
        MArgument_setMImage(res, out);
    }
    catch (libCZI::LibCZIException& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}

EXTERN_C WLLCZI_API int CZIReader_MultiChannelScalingTileComposite(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    // arguments:
// 1st: ROI               -> NumericArray of size 4
// 2nd: plane-coordinate  -> a string (of form "T3C2Z32")
// 3rd: zoom              -> A float number
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

    char* coordinateString = MArgument_getUTF8String(Args[2]);
    CDimCoordinate planeCoordinate;
    try
    {
        planeCoordinate = CDimCoordinate::Parse(coordinateString);
    }
    catch (libCZI::LibCZIStringParseException& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(coordinateString, excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }
    catch (exception& excp)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    mreal zoom = MArgument_getReal(Args[3]);

    try
    {
        auto out = reader->GetMultiChannelScalingTileComposite(
            libData,
            IntRect{ roiValues[0],roiValues[1],roiValues[2],roiValues[3] },
            &planeCoordinate,
            (float)zoom);
        MArgument_setMImage(res, out);
    }
    catch (libCZI::LibCZIException& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}
