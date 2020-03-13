#include "CziReader.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <codecvt>
#include <WolframImageLibrary.h>

using namespace std;
using namespace rapidjson;
using namespace libCZI;

void CziReader::Open(const std::string& utf8_filename)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> ucs2conv;
    std::wstring ucs2 = ucs2conv.from_bytes(utf8_filename);

    auto stream = libCZI::CreateStreamFromFile(ucs2.c_str());

    this->reader->Open(stream);
}

std::string CziReader::GetInfo()
{
    auto statistics = this->reader->GetStatistics();
    return StatisticsToJson(statistics);
}

std::string CziReader::StatisticsToJson(const libCZI::SubBlockStatistics& statistics)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    writer.StartObject();
    writer.Key("SubBlockCount");
    writer.Int(statistics.subBlockCount);

    if (statistics.IsMIndexValid())
    {
        writer.Key("MinMindex");
        writer.Int(statistics.minMindex);
        writer.Key("MaxMindex");
        writer.Int(statistics.maxMindex);
    }

    writer.Key("BoundingBox");
    writer.StartArray();
    writer.Int(statistics.boundingBox.x);
    writer.Int(statistics.boundingBox.y);
    writer.Int(statistics.boundingBox.w);
    writer.Int(statistics.boundingBox.h);
    writer.EndArray();

    writer.Key("BoundingBoxLayer0");
    writer.StartArray();
    writer.Int(statistics.boundingBoxLayer0Only.x);
    writer.Int(statistics.boundingBoxLayer0Only.y);
    writer.Int(statistics.boundingBoxLayer0Only.w);
    writer.Int(statistics.boundingBoxLayer0Only.h);
    writer.EndArray();

    writer.Key("DimBounds");
    writer.StartObject();
    statistics.dimBounds.EnumValidDimensions(
        [&](libCZI::DimensionIndex dim, int start, int size) -> bool
    {
        char dimensionStr[2];
        dimensionStr[0] = libCZI::Utils::DimensionToChar(dim);
        dimensionStr[1] = '\0';
        writer.Key(dimensionStr);
        writer.StartArray();
        writer.Int(start);
        writer.Int(size);
        writer.EndArray();
        return true;
    });
    writer.EndObject();

    if (!statistics.sceneBoundingBoxes.empty())
    {
        writer.Key("SceneBoundingBoxes");
        writer.StartObject();
        for (std::map<int, libCZI::BoundingBoxes>::const_iterator it = statistics.sceneBoundingBoxes.cbegin(); it != statistics.sceneBoundingBoxes.cend(); ++it)
        {
            string key = to_string(it->first);
            writer.Key(key.c_str());

            writer.StartObject();
            writer.Key("BoundingBox");
            writer.StartArray();
            writer.Int(it->second.boundingBox.x);
            writer.Int(it->second.boundingBox.y);
            writer.Int(it->second.boundingBox.w);
            writer.Int(it->second.boundingBox.h);
            writer.EndArray();
            writer.Key("BoundingBoxLayer0");
            writer.StartArray();
            writer.Int(it->second.boundingBoxLayer0.x);
            writer.Int(it->second.boundingBoxLayer0.y);
            writer.Int(it->second.boundingBoxLayer0.w);
            writer.Int(it->second.boundingBoxLayer0.h);
            writer.EndArray();

            writer.EndObject();
        }
        writer.EndObject();
    }

    writer.EndObject();

    return buffer.GetString();
}

MImage CziReader::GetSubBlockImage(WolframLibraryData libData, int no)
{
    WolframImageLibrary_Functions imgFuns = libData->imageLibraryFunctions;

    auto sbBlk = this->reader->ReadSubBlock(no);
    auto bm = sbBlk->CreateBitmap();

    return ConvertToMImage(libData->imageLibraryFunctions, bm.get());
}

MImage CziReader::ConvertToMImage(WolframImageLibrary_Functions imgLibFunctions, libCZI::IBitmapData* bitmapData)
{
    MImage mimg;
    int r; void* pDst;
    switch (bitmapData->GetPixelType())
    {
    case PixelType::Gray8:
        r = (imgLibFunctions->MImage_new2D)(bitmapData->GetWidth(), bitmapData->GetHeight(), 1, MImage_Type_Bit8, MImage_CS_Gray, False, &mimg);
        pDst = imgLibFunctions->MImage_getRawData(mimg);
        CopyStrided(bitmapData, pDst);
        break;
    case PixelType::Gray16:
        r = (imgLibFunctions->MImage_new2D)(bitmapData->GetWidth(), bitmapData->GetHeight(), 1, MImage_Type_Bit16, MImage_CS_Gray, False, &mimg);
        pDst = imgLibFunctions->MImage_getRawData(mimg);
        CopyStrided(bitmapData, pDst);
        break;
    case PixelType::Bgr24:
        r = (imgLibFunctions->MImage_new2D)(bitmapData->GetWidth(), bitmapData->GetHeight(), 1, MImage_Type_Bit8, MImage_CS_RGB, True, &mimg);
        pDst = imgLibFunctions->MImage_getRawData(mimg);
        CopyStrided(bitmapData, pDst);
        break;
    case PixelType::Bgr48:
        r = (imgLibFunctions->MImage_new2D)(bitmapData->GetWidth(), bitmapData->GetHeight(), 1, MImage_Type_Bit16, MImage_CS_RGB, True, &mimg);
        pDst = imgLibFunctions->MImage_getRawData(mimg);
        CopyStrided(bitmapData, pDst);
        break;
    case PixelType::Gray32Float:
        r = (imgLibFunctions->MImage_new2D)(bitmapData->GetWidth(), bitmapData->GetHeight(), 1, MImage_Type_Real32, MImage_CS_Gray, False, &mimg);
        pDst = imgLibFunctions->MImage_getRawData(mimg);
        CopyStrided(bitmapData, pDst);
        break;
    default:
        throw std::invalid_argument("unsupported pixeltype");
    }

    return mimg;
}

/*static*/void CziReader::CopyStrided(libCZI::IBitmapData* bitmapData, void* pDst)
{
    size_t lengthOfLine = size_t(bitmapData->GetWidth()) * GetBytesPerPel(bitmapData->GetPixelType());
    auto height = bitmapData->GetHeight();
    ScopedBitmapLocker<IBitmapData*> lckBm{ bitmapData };
    for (decltype(height) y = 0; y < height; ++y)
    {
        memcpy(
            ((char*)pDst) + y * lengthOfLine,
            ((const char*)lckBm.ptrDataRoi) + y * lckBm.stride,
            lengthOfLine);
    }
}


/*static*/int CziReader::GetBytesPerPel(libCZI::PixelType pixelType)
{
    switch (pixelType)
    {
    case PixelType::Gray8:              return 1;
    case PixelType::Gray16:             return 2;
    case PixelType::Gray32Float:        return 4;
    case PixelType::Bgr24:              return 3;
    case PixelType::Bgr48:              return 6;
    case PixelType::Bgr96Float:         return 12;
    case PixelType::Bgra32:             return 4;
    case PixelType::Gray64ComplexFloat: return 16;
    case PixelType::Bgr192ComplexFloat: return 48;
    case PixelType::Gray32:             return 4;
    case PixelType::Gray64Float:        return 8;
    default:
        throw std::invalid_argument("illegal pixeltype");
    }
}