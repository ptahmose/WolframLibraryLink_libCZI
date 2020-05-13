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
#include "finally.h"
#include "dbgprint.h"
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
    DBGPRINT((CDbg::Level::Trace, "CZIReader_Open: Enter"));
    if (Argc != 2)
    {
        ErrHelper::ReportError_WrongNumberOfArguments(libData);
        return LIBRARY_FUNCTION_ERROR;
    }

    mint id = MArgument_getInteger(Args[0]);
    auto reader = CziReaderManager::Instance.GetInstance(id);
    char* filename = MArgument_getUTF8String(Args[1]);
    auto _ = finally([filename, libData]()->void
    {
        libData->UTF8String_disown(filename);
    });

    try
    {
        VDBGPRINT((CDbg::Level::Trace, "CZIReader_Open: attempt to open file \"%s\".", filename));
        reader->Open(filename);
    }
    catch (libCZI::LibCZIException& libCziExcp)
    {
        ErrHelper::ReportError_CziReaderOpenException(libData, libCziExcp);
        //libData->Message("OpenCZIopenreadfail");
        //libData->Message(ErrHelper::GetErrorText_CziReaderOpenException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }
    catch (exception& excp)
    {
        ErrHelper::ReportError_CziReaderOpenException(libData, excp);
        //libData->Message("OpenCZIopenreadfail");
        //libData->Message(ErrHelper::GetErrorText_CziReaderOpenException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}

int CZIReader_GetInfo(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 1)
    {
        ErrHelper::ReportError_WrongNumberOfArguments(libData);
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
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
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
        ErrHelper::ReportError_WrongNumberOfArguments(libData);
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
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    try
    {
        WolframLibLinkUtils::ThrowIfValueIsNotInt32OrNegative(blockNo);
        auto out = reader->GetSubBlockImage(libData, static_cast<int>(blockNo));
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
    // 4th: background-color  -> optional, RgbGFloat (numeric array of size 3)
    if (Argc != 4 && Argc != 5)
    {
        ErrHelper::ReportError_WrongNumberOfArguments(libData);
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
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    int roiValues[4];
    WolframNumericArrayLibrary_Functions naFuncs = libData->numericarrayLibraryFunctions;
    MNumericArray numArrayRegionOfInterest = MArgument_getMNumericArray(Args[1]);
    const bool b = WolframLibLinkUtils::TryGetAsInt32(roiValues, sizeof(roiValues) / sizeof(roiValues[0]), numArrayRegionOfInterest, naFuncs);
    if (!b)
    {
        ErrHelper::ReportError_CziReaderGetSingleChannelScalingTileCompositeRoiInvalid(libData);
        //libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeRoiInvalid().c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    char* coordinateString = MArgument_getUTF8String(Args[2]);
    auto _ = finally([coordinateString, libData]()->void { libData->UTF8String_disown(coordinateString); });
    CDimCoordinate planeCoordinate;
    try
    {
        planeCoordinate = CDimCoordinate::Parse(coordinateString);
    }
    catch (libCZI::LibCZIStringParseException& parseExcp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(coordinateString, parseExcp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(coordinateString, excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    const mreal zoom = MArgument_getReal(Args[3]);

    bool haveBackgroundColorArgs = false;
    float rgbFloatValues[3];
    if (Argc >= 5)
    {
        MNumericArray numArrayBackgndColor = MArgument_getMNumericArray(Args[4]);
        bool b = WolframLibLinkUtils::TryGetAsFloat32(rgbFloatValues, sizeof(rgbFloatValues) / sizeof(rgbFloatValues[0]), numArrayBackgndColor, libData->numericarrayLibraryFunctions);
        if (b == false)
        {
            libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeBackgroundColorInvalid().c_str());
            return LIBRARY_FUNCTION_ERROR;
        }

        haveBackgroundColorArgs = true;
    }

    try
    {
        MImage out;
        if (haveBackgroundColorArgs == false)
        {
            out = reader->GetSingleChannelScalingTileComposite(
                libData,
                IntRect{ roiValues[0],roiValues[1],roiValues[2],roiValues[3] },
                &planeCoordinate,
                static_cast<float>(zoom));
        }
        else
        {
            const libCZI::RgbFloatColor bckGndColor{ rgbFloatValues[0],rgbFloatValues[1],rgbFloatValues[2] };
            out = reader->GetSingleChannelScalingTileComposite(
                libData,
                IntRect{ roiValues[0],roiValues[1],roiValues[2],roiValues[3] },
                &planeCoordinate,
                static_cast<float>(zoom),
                bckGndColor);
        }

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

int CZIReader_MultiChannelScalingTileComposite(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    // arguments:
    // 1st: ROI               -> NumericArray of size 4
    // 2nd: plane-coordinate  -> a string (of form "T3C2Z32")
    // 3rd: zoom              -> A float number
    // 4th: display-settings  -> [optional] a string containing JSON
    VDBGPRINT((CDbg::Level::Trace, "CZIReader_MultiChannelScalingTileComposite: Enter (Argc=%" MINTFMT ")", Argc));
    if (Argc < 4)
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
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    int roiValues[4];
    WolframNumericArrayLibrary_Functions naFuncs = libData->numericarrayLibraryFunctions;
    MNumericArray numArrayRegionOfInterest = MArgument_getMNumericArray(Args[1]);
    WolframLibLinkUtils::TryGetAsInt32(roiValues, sizeof(roiValues) / sizeof(roiValues[0]), numArrayRegionOfInterest, naFuncs);

    char* coordinateString = MArgument_getUTF8String(Args[2]);
    auto _1 = finally([coordinateString, libData]()->void { libData->UTF8String_disown(coordinateString); });
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

    const mreal zoom = MArgument_getReal(Args[3]);

    char* displaySettingsString = nullptr;
    if (Argc > 4)
    {
        displaySettingsString = MArgument_getUTF8String(Args[4]);
    }

    auto _2 = finally([displaySettingsString, libData]()->void
    {
        if (displaySettingsString != nullptr)
        {
            libData->UTF8String_disown(displaySettingsString);
        }
    });

    int returnValue = LIBRARY_NO_ERROR;
    try
    {
        auto out = reader->GetMultiChannelScalingTileComposite(
            libData,
            IntRect{ roiValues[0],roiValues[1],roiValues[2],roiValues[3] },
            &planeCoordinate,
            (float)zoom,
            displaySettingsString);
        MArgument_setMImage(res, out);
    }
    catch (libCZI::LibCZIException& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeException(excp).c_str());
        returnValue = LIBRARY_FUNCTION_ERROR;
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeException(excp).c_str());
        returnValue = LIBRARY_FUNCTION_ERROR;
    }

    return returnValue;
}

int CZIReader_GetMetadataXml(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
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
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    try
    {
        string s = reader->GetMetadataXml();
        g_stringReturnHelper.StoreString(s);
        MArgument_setUTF8String(res, g_stringReturnHelper.GetStoredString());
    }
    catch (libCZI::LibCZIException& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziGetMetadataXml(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziGetMetadataXml(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}

int CZIReader_GetScaling(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 1)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    const mint id = MArgument_getInteger(Args[0]);

    std::shared_ptr<CziReader> reader;
    try
    {
        reader = CziReaderManager::Instance.GetInstance(id);
    }
    catch (out_of_range&)
    {
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    mint dims[1];
    dims[0] = 3;
    MTensor tensor(nullptr);
    libData->MTensor_new(MType_Real, /*rank*/1, dims, &tensor);

    auto s = reader->GetScaling();

    for (mint i = 1; i < 4; ++i)
    {
        libData->MTensor_setReal(tensor, &i, s[i - 1]);
    }

    MArgument_setMTensor(res, tensor);
    return LIBRARY_NO_ERROR;
}

int CZIReader_ReadSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 2)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    const mint id = MArgument_getInteger(Args[0]);
    const mint blockNo = MArgument_getInteger(Args[1]);

    std::shared_ptr<CziReader> reader;
    try
    {
        reader = CziReaderManager::Instance.GetInstance(id);
    }
    catch (out_of_range&)
    {
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    try
    {
        WolframLibLinkUtils::ThrowIfValueIsNotInt32OrNegative(blockNo);
        const mint handle = reader->ReadSubBlock(static_cast<int>(blockNo));
        MArgument_setInteger(res, handle);
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSubBlockBitmapException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}

int CZIReader_GetBitmapFromSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 2)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    mint id = MArgument_getInteger(Args[0]);
    mint handle = MArgument_getInteger(Args[1]);

    std::shared_ptr<CziReader> reader;
    try
    {
        reader = CziReaderManager::Instance.GetInstance(id);
    }
    catch (out_of_range&)
    {
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    try
    {
        MImage img = reader->GetBitmapFromSubBlock(handle, libData);
        MArgument_setMImage(res, img);
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSubBlockBitmapException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}

int CZIReader_GetMetadataFromSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 2)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    const mint id = MArgument_getInteger(Args[0]);
    const mint handle = MArgument_getInteger(Args[1]);

    std::shared_ptr<CziReader> reader;
    try
    {
        reader = CziReaderManager::Instance.GetInstance(id);
    }
    catch (out_of_range&)
    {
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    try
    {
        auto xml = reader->GetMetadataFromSubBlock(handle);
        g_stringReturnHelper.StoreString(xml);
        MArgument_setUTF8String(res, g_stringReturnHelper.GetStoredString());
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSubBlockBitmapException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}

int CZIReader_GetInfoFromSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 2)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    const mint id = MArgument_getInteger(Args[0]);
    const mint handle = MArgument_getInteger(Args[1]);

    std::shared_ptr<CziReader> reader;
    try
    {
        reader = CziReaderManager::Instance.GetInstance(id);
    }
    catch (out_of_range&)
    {
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    try
    {
        auto xml = reader->GetInfoFromSubBlock(handle);
        g_stringReturnHelper.StoreString(xml);
        MArgument_setUTF8String(res, g_stringReturnHelper.GetStoredString());
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSubBlockBitmapException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}

int CZIReader_ReleaseSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res)
{
    if (Argc != 2)
    {
        return LIBRARY_FUNCTION_ERROR;
    }

    const mint id = MArgument_getInteger(Args[0]);
    const mint handle = MArgument_getInteger(Args[1]);

    std::shared_ptr<CziReader> reader;
    try
    {
        reader = CziReaderManager::Instance.GetInstance(id);
    }
    catch (out_of_range&)
    {
        ErrHelper::ReportError_CziReaderInstanceNotExisting(libData, id);
        return LIBRARY_FUNCTION_ERROR;
    }

    try
    {
        bool success = reader->ReleaseSubBlock(handle);
    }
    catch (exception& excp)
    {
        libData->Message(ErrHelper::GetErrorText_CziReaderGetSubBlockBitmapException(excp).c_str());
        return LIBRARY_FUNCTION_ERROR;
    }

    return LIBRARY_NO_ERROR;
}