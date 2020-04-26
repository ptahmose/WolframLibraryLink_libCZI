#pragma once

#include "inc_libCzi.h"
#include <mutex>
#include <memory>
#include <array>
#include <WolframLibrary.h>
#include <WolframImageLibrary.h>

class CziReader
{
private:
    std::shared_ptr<libCZI::ICZIReader> reader;

    std::once_flag flagInfoFromCziMetadata;

    std::shared_ptr<libCZI::IDisplaySettings> displaySettingsFromCzi;
    libCZI::ScalingInfoEx scalingInfoFromCzi;

public:
    CziReader() : reader(libCZI::CreateCZIReader())
    {}

    void Open(const std::string& utf8_filename);

    std::string GetInfo();
    std::string GetMetadataXml();

    MImage GetSubBlockImage(WolframLibraryData libData,int no);
    MImage GetSingleChannelScalingTileComposite(WolframLibraryData libData, const libCZI::IntRect& roi, const libCZI::IDimCoordinate* planeCoordinate, float zoom);
    MImage GetMultiChannelScalingTileComposite(WolframLibraryData libData, const libCZI::IntRect& roi, const libCZI::IDimCoordinate* planeCoordinate, float zoom, const char* displaySettingsJson);

    std::array<double, 3>   GetScaling();
private:
    /// Initializes the members "displaySettingsFromCzi" and "scalingInfoFromCzi".
    void InitializeInfoFromCzi();
    std::shared_ptr<libCZI::IDisplaySettings> GetDisplaySettingsFromCzi();
    const libCZI::ScalingInfo& GetScalingInfoFromCzi();

    std::string StatisticsToJson(const libCZI::SubBlockStatistics& statistics);

    static MImage ConvertToMImage(WolframImageLibrary_Functions imgLibFunctions, libCZI::IBitmapData* bitmapData);
    static void CopyStrided(libCZI::IBitmapData* bitmapData, void* pDst);
    static void CopyStrided_RGB24_to_BGR24(libCZI::IBitmapData* bitmapData, void* pDst);
    static void CopyStrided_RGB48_to_BGR48(libCZI::IBitmapData* bitmapData, void* pDst);

    MImage GetMultiChannelScalingTileComposite(WolframLibraryData libData, const libCZI::IntRect& roi, const libCZI::IDimCoordinate* planeCoordinate, float zoom, const libCZI::IDisplaySettings* displaySettings);
    MImage GetMultiChannelScalingTileCompositeAllChannelsDisabled(WolframLibraryData libData, const libCZI::IntRect& roi, float zoom);
};