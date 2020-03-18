#pragma once

#include "inc_libCzi.h"
#include <mutex>
#include <memory>
#include <WolframLibrary.h>
#include <WolframImageLibrary.h>

class CziReader
{
private:
    std::shared_ptr<libCZI::ICZIReader> reader;
    std::once_flag flagDispaySettingsFromCzi;
    std::shared_ptr<libCZI::IDisplaySettings> displaySettingsFromCzi;
public:
    CziReader() : reader(libCZI::CreateCZIReader())
    {}

    void Open(const std::string& utf8_filename);

    std::string GetInfo();

    MImage GetSubBlockImage(WolframLibraryData libData,int no);
    MImage GetSingleChannelScalingTileComposite(WolframLibraryData libData, const libCZI::IntRect& roi, const libCZI::IDimCoordinate* planeCoordinate, float zoom);
    MImage GetMultiChannelScalingTileComposite(WolframLibraryData libData, const libCZI::IntRect& roi, const libCZI::IDimCoordinate* planeCoordinate, float zoom);
private:
    std::string StatisticsToJson(const libCZI::SubBlockStatistics& statistics);
    std::shared_ptr<libCZI::IDisplaySettings> GetDispaySettingsFromCzi();

    static MImage ConvertToMImage(WolframImageLibrary_Functions imgLibFunctions, libCZI::IBitmapData* bitmapData);
    static void CopyStrided(libCZI::IBitmapData* bitmapData, void* pDst);
    static void CopyStrided_RGB24_to_BGR24(libCZI::IBitmapData* bitmapData, void* pDst);
    static void CopyStrided_RGB48_to_BGR48(libCZI::IBitmapData* bitmapData, void* pDst);
};