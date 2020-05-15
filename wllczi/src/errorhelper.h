#pragma once
#include <WolframLibrary.h>
#include "inc_libCzi.h"
#include <exception>
#include <string>

class CLastErrorStore
{
private:
    std::string lastErrorInfo;
public:
    void SetLastErrorInfo(const std::string& lastErrorInfo);
    void ClearLastError();
    std::string GetLastErrorInfo() const;
    bool HasLastErrorInfo() const;
};

class ErrHelper
{
private:
    static CLastErrorStore lastError;

    // those strings need to correspond to the message-definitions in CZIReader.m
    static const char* packageError_OpenCZI_failed;
    static const char* packageError_CziReaderInstanceDoesNotExist;
    static const char* packageError_WrongNumberOfArgument;
    static const char* packageError_GetSingleChannelScalingTileCompositeRoiInvalid;
    static const char* packageError_GetSingleChannelScalingTileCompositeParseCoordinateException;
    static const char* packageError_GetSingleChannelScalingTileCompositeBackgroundColorInvalid;
    static const char* packageError_GetSingleChannelScalingTileCompositeException;
    static const char* packageError_GetMultiChannelScalingTileCompositeParseCoordinateException;
    static const char* packageError_GetMultiChannelScalingTileCompositeException;
    static const char* packageError_GetMetadataXmlException;
    static const char* packageError_ReadSubBlockException;
    static const char* packageError_GetBitmapFromSubBlockException;
    static const char* packageError_GetMetadataFromSubBlockException;
    static const char* packageError_GetInfoFromSubBlockException;
    static const char* packageError_ReleaseSubBlockException;
    static const char* packageError_GetSubBlockBitmapException;
public:
    static void ReportError_Success();
    static  std::string GetLastError();

    static void ReportError_WrongNumberOfArguments(WolframLibraryData libData);
    static void ReportError_CziReaderOpenException(WolframLibraryData libData,std::exception& excp);
    static void ReportError_CziReaderOpenException(WolframLibraryData libData,libCZI::LibCZIException& excp);
    static void ReportError_CziReaderInstanceNotExisting(WolframLibraryData libData, mint id);
    static void ReportError_CziReaderGetSingleChannelScalingTileCompositeRoiInvalid(WolframLibraryData libData);
    static void ReportError_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(WolframLibraryData libData, const char* coordinateString, libCZI::LibCZIStringParseException& excp);
    static void ReportError_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(WolframLibraryData libData, const char* coordinateString, std::exception& excp);
    static void ReportError_CziReaderGetSingleChannelScalingTileCompositeBackgroundColorInvalid(WolframLibraryData libData);
    static void ReportError_CziReaderGetSingleChannelScalingTileCompositeException(WolframLibraryData libData, std::exception& excp);
    static void ReportError_CziReaderGetMultiChannelScalingTileCompositeParseCoordinateException(WolframLibraryData libData, const char* coordinateString, libCZI::LibCZIStringParseException& excp);
    static void ReportError_CziReaderGetMultiChannelScalingTileCompositeParseCoordinateException(WolframLibraryData libData, const char* coordinateString, std::exception& excp);
    static void ReportError_CziReaderGetMultiChannelScalingTileCompositeException(WolframLibraryData libData, std::exception& excp);
    static void ReportError_CziGetMetadataXmlException(WolframLibraryData libData, std::exception& excp);
    static void ReportError_CziReaderReadSubBlockException(WolframLibraryData libData, std::exception& excp);
    static void ReportError_CziReaderGetBitmapFromSubBlock(WolframLibraryData libData, std::exception& excp);
    static void ReportError_CziReaderGetMetadataFromSubBlock(WolframLibraryData libData, std::exception& excp);
    static void ReportError_CziReaderGetInfoFromSubBlock(WolframLibraryData libData, std::exception& excp);
    static void ReportError_CziReaderReleaseSubBlock(WolframLibraryData libData, std::exception& excp);
    static void ReportError_CziReaderGetSubBlockBitmapException(WolframLibraryData libData, std::exception& excp);
private:
    static std::string GetErrorText_CziReaderInstanceNotExisting(mint id);
};