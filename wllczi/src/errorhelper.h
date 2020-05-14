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
    void SetLastErrorInfo(const std::string& lastErrorInfo)
    {
        this->lastErrorInfo = lastErrorInfo;
    }

    void ClearLastError();

    bool GetLastErrorInfo(std::string& info);
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
public:
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


    static std::string GetErrorText_CziReaderGetSubBlockBitmapException(std::exception& excp);
    static std::string GetErrorText_CziReaderGetSingleChannelScalingTileCompositeException(std::exception& excp);
    //static std::string GetErrorText_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(const char* coordinateString, libCZI::LibCZIStringParseException& excp);
    //static std::string GetErrorText_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(const char* coordinateString, std::exception& excp);
    static std::string GetErrorText_CziReaderGetSingleChannelScalingTileCompositeRoiInvalid();
    static std::string GetErrorText_CziReaderGetSingleChannelScalingTileCompositeBackgroundColorInvalid();
    static std::string GetErrorText_CziGetMetadataXml(std::exception& excp);
    static std::string GetErrorText_CziGetMetadataXml(libCZI::LibCZIException& excp);
private:
    static std::string GetErrorText_CziReaderInstanceNotExisting(mint id);
};