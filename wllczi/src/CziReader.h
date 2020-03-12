#pragma once

#include "inc_libCzi.h"
#include <WolframLibrary.h>
#include <WolframImageLibrary.h>

class CziReader
{
private:
    std::shared_ptr<libCZI::ICZIReader> reader;
public:
    CziReader() : reader(libCZI::CreateCZIReader())
    {}

    void Open(const std::string& utf8_filename);

    std::string GetInfo();

    MImage GetSubBlockImage(WolframLibraryData libData,int no);

private:
    std::string StatisticsToJson(const libCZI::SubBlockStatistics& statistics);

    MImage ConvertToMImage(WolframImageLibrary_Functions imgLibFunctions, libCZI::IBitmapData* bitmapData);
    static void CopyStrided(libCZI::IBitmapData* bitmapData, void* pDst);
    static int GetBytesPerPel(libCZI::PixelType pixelType);
};