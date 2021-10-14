#include "CziReader.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "MImageHelper.h"
#include <locale>
#include <codecvt>
#include <sstream>
#include <WolframImageLibrary.h>
#include "CziUtilities.h"

using namespace std;
using namespace rapidjson;
using namespace libCZI;


void CziReader::Open(const std::string& utf8_filename)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wcsconv;
    std::wstring wstr = wcsconv.from_bytes(utf8_filename);
    auto stream = libCZI::CreateStreamFromFile(wstr.c_str());
    this->reader->Open(stream);
}

std::string CziReader::GetInfo()
{
    auto statistics = this->reader->GetStatistics();
    return StatisticsToJson(statistics);
}

std::array<double, 3> CziReader::GetScaling()
{
    const libCZI::ScalingInfo& scaling = this->GetScalingInfoFromCzi();
    return std::array<double, 3>
    {
        scaling.IsScaleXValid() ? scaling.scaleX : -1,
        scaling.IsScaleYValid() ? scaling.scaleY : -1,
        scaling.IsScaleZValid() ? scaling.scaleZ : -1
    };
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
    auto sbBlk = this->reader->ReadSubBlock(no);
    if (!sbBlk)
    {
        std::stringstream ss;
        ss << "SubBlock for id=" << no << " was not found.";
        throw invalid_argument(ss.str());
    }

    auto bm = sbBlk->CreateBitmap();
    return ConvertToMImage(libData->imageLibraryFunctions, bm.get());
}

MImage CziReader::GetSingleChannelScalingTileComposite(WolframLibraryData libData, const libCZI::IntRect& roi, const libCZI::IDimCoordinate* planeCoordinate, float zoom)
{
    const RgbFloatColor backgndCol{ 0,0,0 };
    return this->GetSingleChannelScalingTileComposite(libData, roi, planeCoordinate, zoom, backgndCol);
}

MImage CziReader::GetSingleChannelScalingTileComposite(WolframLibraryData libData, const libCZI::IntRect& roi, const libCZI::IDimCoordinate* planeCoordinate, float zoom, const libCZI::RgbFloatColor& backgroundColor)
{
    auto scsta = this->reader->CreateSingleChannelScalingTileAccessor();
    libCZI::IntSize size = scsta->CalcSize(roi, zoom);

    int c = (numeric_limits<int>::min)();
    planeCoordinate->TryGetPosition(libCZI::DimensionIndex::C, &c);

    // the idea is: for the cornerstone-case where we do not have a C-index, the call to "TryGetSubBlockInfoOfArbitrarySubBlockInChannel"
    // will ignore the specified index _if_ there are no C-indices at all
    libCZI::PixelType pixeltype = Utils::TryDeterminePixelTypeForChannel(this->reader.get(), c);

    // however - if we get here with an invalid pixeltype, then we need to give up
    if (pixeltype == PixelType::Invalid)
    {
        throw invalid_argument("Unable to determine pixeltype.");
    }

    auto mimagedeleter = std::bind(
        [](WolframImageLibrary_Functions imgLibFuncs, MImage mimg)->void {imgLibFuncs->MImage_free(mimg); },
        libData->imageLibraryFunctions, std::placeholders::_1);
    std::unique_ptr<IMAGEOBJ_ENTRY, decltype(mimagedeleter)> spMimg(
        MImageHelper::CreateMImage(libData->imageLibraryFunctions, size, pixeltype),
        mimagedeleter);

    CMImageWrapper mimgWrapper(libData->imageLibraryFunctions, spMimg.get());
    ISingleChannelScalingTileAccessor::Options accessorGetOptions;
    accessorGetOptions.Clear();
    accessorGetOptions.backGroundColor = backgroundColor;
    scsta->Get(&mimgWrapper, roi, planeCoordinate, zoom, &accessorGetOptions);
    MImageHelper::SwapRgb(&mimgWrapper);

    return spMimg.release();
}

MImage CziReader::GetMultiChannelScalingTileComposite(WolframLibraryData libData, const libCZI::IntRect& roi, const libCZI::IDimCoordinate* planeCoordinate, float zoom, const char* displaySettingsJson)
{
    if (displaySettingsJson == nullptr || *displaySettingsJson == '\0')
    {
        return CziReader::GetMultiChannelScalingTileComposite(libData, roi, planeCoordinate, zoom, this->GetDisplaySettingsFromCzi().get());
    }

    ChannelDisplaySettingsInfo dsinfo = CziUtilities::ParseDisplaySettings(displaySettingsJson);

    if (dsinfo.isToBeMerged == true)
    {
        const auto displaySettingsFromCzi = this->GetDisplaySettingsFromCzi();
        const auto combinedDisplaySettings = CziUtilities::CombineDisplaySettings(displaySettingsFromCzi.get(), dsinfo.displaySettings);

        return CziReader::GetMultiChannelScalingTileComposite(libData, roi, planeCoordinate, zoom, combinedDisplaySettings.get());
    }
    else
    {
        const auto resultingDisplaySettings = CziUtilities::ConvertToDisplaySettings(dsinfo.displaySettings);

        return CziReader::GetMultiChannelScalingTileComposite(libData, roi, planeCoordinate, zoom, resultingDisplaySettings.get());
    }
}

MImage CziReader::GetMultiChannelScalingTileComposite(WolframLibraryData libData, const libCZI::IntRect& roi, const libCZI::IDimCoordinate* planeCoordinate, float zoom, const libCZI::IDisplaySettings* displaySettings)
{
    std::vector<int> activeChannels = libCZI::CDisplaySettingsHelper::GetActiveChannels(displaySettings);

    // we need to deal with the pathological case that all channels are disabled
    if (activeChannels.empty())
    {
        return CziReader::GetMultiChannelScalingTileCompositeAllChannelsDisabled(libData, roi, zoom);
    }

    std::vector<shared_ptr<IBitmapData>> channelBitmaps;
    IntSize sizeResult;
    try
    {
        channelBitmaps = CziUtilities::GetBitmapsFromSpecifiedChannels(
            this->reader.get(),
            planeCoordinate,
            roi,
            zoom,
            [&](int idx, int& chNo)->bool
        {
            if (idx < (int)activeChannels.size())
            {
                chNo = activeChannels.at(idx);
                return true;
            }

            return false;
        },
            &sizeResult);
    }
    catch (LibCZIInvalidPlaneCoordinateException& /*invalidCoordExcp*/)
    {
        return nullptr;
    }

    libCZI::CDisplaySettingsHelper dsplHlp;
    dsplHlp.Initialize(displaySettings, [&](int chIndx)->libCZI::PixelType
    {
        const auto idx = std::distance(activeChannels.cbegin(), std::find(activeChannels.cbegin(), activeChannels.cend(), chIndx));
        return channelBitmaps[idx]->GetPixelType();
    });

    std::vector<IBitmapData*> vecBm; vecBm.reserve(channelBitmaps.size());
    for (int i = 0; i < channelBitmaps.size(); ++i)
    {
        vecBm.emplace_back(channelBitmaps[i].get());
    }

    auto mimagedeleter = std::bind(
        [](WolframImageLibrary_Functions imgLibFuncs, MImage mimg)->void {imgLibFuncs->MImage_free(mimg); },
        libData->imageLibraryFunctions, std::placeholders::_1);
    std::unique_ptr<IMAGEOBJ_ENTRY, decltype(mimagedeleter)> spMimg(
        MImageHelper::CreateMImage(libData->imageLibraryFunctions, sizeResult, libCZI::PixelType::Bgr24),
        mimagedeleter);

    CMImageWrapper mimgWrapper(libData->imageLibraryFunctions, spMimg.get());
    libCZI::Compositors::ComposeMultiChannel_Bgr24(
        &mimgWrapper,
        (int)channelBitmaps.size(),
        &vecBm[0],
        dsplHlp.GetChannelInfosArray());

    MImageHelper::SwapRgb(&mimgWrapper);

    return spMimg.release();
}

MImage CziReader::GetMultiChannelScalingTileCompositeAllChannelsDisabled(WolframLibraryData libData, const libCZI::IntRect& roi, float zoom)
{
    auto accessor = reader->CreateSingleChannelScalingTileAccessor();
    const auto sizeResult = accessor->CalcSize(roi, zoom);
    RgbFloatColor color{ 0,0,0 };

    auto mimagedeleter = std::bind(
        [](WolframImageLibrary_Functions imgLibFuncs, MImage mimg)->void {imgLibFuncs->MImage_free(mimg); },
        libData->imageLibraryFunctions, std::placeholders::_1);
    std::unique_ptr<IMAGEOBJ_ENTRY, decltype(mimagedeleter)> spMimg(
        MImageHelper::CreateMImage(libData->imageLibraryFunctions, sizeResult, libCZI::PixelType::Bgr24),
        mimagedeleter);
    CMImageWrapper mimgWrapper(libData->imageLibraryFunctions, spMimg.get());
    libCZI::Utils::FillBitmap(&mimgWrapper, color);
    return spMimg.release();
}

void CziReader::InitializeInfoFromCzi()
{
    std::call_once(
        this->flagInfoFromCziMetadata,
        [this]()
    {
        auto mds = this->reader->ReadMetadataSegment();
        auto md = mds->CreateMetaFromMetadataSegment();
        const auto docInfo = md->GetDocumentInfo();
        this->displaySettingsFromCzi = docInfo->GetDisplaySettings();
        this->scalingInfoFromCzi = docInfo->GetScalingInfoEx();
    });
}

std::shared_ptr<libCZI::IDisplaySettings> CziReader::GetDisplaySettingsFromCzi()
{
    this->InitializeInfoFromCzi();
    return this->displaySettingsFromCzi;
}

const libCZI::ScalingInfo& CziReader::GetScalingInfoFromCzi()
{
    this->InitializeInfoFromCzi();
    return this->scalingInfoFromCzi;
}

/*static*/MImage CziReader::ConvertToMImage(WolframImageLibrary_Functions imgLibFunctions, libCZI::IBitmapData* bitmapData)
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
        r = (imgLibFunctions->MImage_new2D)(bitmapData->GetWidth(), bitmapData->GetHeight(), 3, MImage_Type_Bit8, MImage_CS_RGB, True, &mimg);
        pDst = imgLibFunctions->MImage_getRawData(mimg);
        CopyStrided_RGB24_to_BGR24(bitmapData, pDst);
        break;
    case PixelType::Bgr48:
        r = (imgLibFunctions->MImage_new2D)(bitmapData->GetWidth(), bitmapData->GetHeight(), 3, MImage_Type_Bit16, MImage_CS_RGB, True, &mimg);
        pDst = imgLibFunctions->MImage_getRawData(mimg);
        CopyStrided_RGB48_to_BGR48(bitmapData, pDst);
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
    size_t lengthOfLine = size_t(bitmapData->GetWidth()) * MImageHelper::GetBytesPerPel(bitmapData->GetPixelType());
    auto height = bitmapData->GetHeight();
    ScopedBitmapLocker<IBitmapData*> lckBm{ bitmapData };
    for (decltype(height) y = 0; y < height; ++y)
    {
        memcpy(
            ((char*)pDst) + y * lengthOfLine,
            ((const char*)lckBm.ptrDataRoi) + y * (size_t)lckBm.stride,
            lengthOfLine);
    }
}

/*static*/void CziReader::CopyStrided_RGB24_to_BGR24(libCZI::IBitmapData* bitmapData, void* pDst)
{
    size_t lengthOfLine = size_t(bitmapData->GetWidth()) * MImageHelper::GetBytesPerPel(bitmapData->GetPixelType());
    auto height = bitmapData->GetHeight();
    auto width = bitmapData->GetWidth();
    ScopedBitmapLocker<IBitmapData*> lckBm{ bitmapData };
    for (decltype(height) y = 0; y < height; ++y)
    {
        const uint8_t* ptrSrc = ((const uint8_t*)lckBm.ptrDataRoi) + y * (size_t)lckBm.stride;
        uint8_t* ptrDst = ((uint8_t*)pDst) + y * lengthOfLine;
        for (decltype(width) x = 0; x < width; ++x)
        {
            uint8_t b = *ptrSrc++;
            uint8_t g = *ptrSrc++;
            uint8_t r = *ptrSrc++;
            *ptrDst++ = r;
            *ptrDst++ = g;
            *ptrDst++ = b;
        }
    }
}

/*static*/void CziReader::CopyStrided_RGB48_to_BGR48(libCZI::IBitmapData* bitmapData, void* pDst)
{
    size_t lengthOfLine = size_t(bitmapData->GetWidth()) * MImageHelper::GetBytesPerPel(bitmapData->GetPixelType());
    auto height = bitmapData->GetHeight();
    auto width = bitmapData->GetWidth();
    ScopedBitmapLocker<IBitmapData*> lckBm{ bitmapData };
    for (decltype(height) y = 0; y < height; ++y)
    {
        const uint16_t* ptrSrc = (const uint16_t*)(((const uint8_t*)lckBm.ptrDataRoi) + y * (size_t)lckBm.stride);
        uint16_t* ptrDst = (uint16_t*)(((uint8_t*)pDst) + y * lengthOfLine);
        for (decltype(width) x = 0; x < width; ++x)
        {
            uint16_t b = *ptrSrc++;
            uint16_t g = *ptrSrc++;
            uint16_t r = *ptrSrc++;
            *ptrDst++ = r;
            *ptrDst++ = g;
            *ptrDst++ = b;
        }
    }
}

std::string CziReader::GetMetadataXml()
{
    auto mds = this->reader->ReadMetadataSegment();
    auto m = mds->CreateMetaFromMetadataSegment();
    if (!m->IsXmlValid())
    {
        throw runtime_error("Metadata-XML found to be invalid.");
    }

    return m->GetXml();
}

mint CziReader::ReadSubBlock(int no)
{
    auto sbBlk = this->reader->ReadSubBlock(no);
    if (!sbBlk)
    {
        std::stringstream ss;
        ss << "SubBlock for id=" << no << " was not found.";
        throw invalid_argument(ss.str());
    }

    return static_cast<mint>(this->sbBlkStore.AddSubBlock(sbBlk));
}

MImage CziReader::GetBitmapFromSubBlock(mint handle, WolframLibraryData libData)
{
    auto sbBlk = this->sbBlkStore.GetForHandle(handle);
    if (!sbBlk)
    {
        std::stringstream ss;
        ss << "SubBlock for handle=" << handle << " is not present.";
        throw invalid_argument(ss.str());
    }

    auto bm = sbBlk->CreateBitmap();
    return ConvertToMImage(libData->imageLibraryFunctions, bm.get());
}

std::string CziReader::GetMetadataFromSubBlock(mint handle)
{
    auto sbBlk = this->sbBlkStore.GetForHandle(handle);
    if (!sbBlk)
    {
        std::stringstream ss;
        ss << "SubBlock for handle=" << handle << " is not present.";
        throw invalid_argument(ss.str());
    }

    size_t sizeData;
    auto ptrData = sbBlk->GetRawData(ISubBlock::MemBlkType::Metadata, &sizeData);
    if (!ptrData)
    {
        return "";
    }

    string metadataXml(static_cast<const char*>(ptrData.get()), sizeData);
    return metadataXml;
}

std::string CziReader::GetInfoFromSubBlock(mint handle)
{
    auto sbBlk = this->sbBlkStore.GetForHandle(handle);
    if (!sbBlk)
    {
        std::stringstream ss;
        ss << "SubBlock for handle=" << handle << " is not present.";
        throw invalid_argument(ss.str());
    }

    auto sbInfo = sbBlk->GetSubBlockInfo();
    return this->SubblockInfoToJson(sbInfo);
}
 
bool CziReader::ReleaseSubBlock(mint handle)
{
    return this->sbBlkStore.RemoveSubBlock(handle);
}

std::string CziReader::SubblockInfoToJson(const libCZI::SubBlockInfo& subblockInfo)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    writer.StartObject();
    writer.Key("CompressionMode");
    writer.String(libCZI::Utils::CompressionModeToInformalString(subblockInfo.GetCompressionMode()));
    writer.Key("Pixeltype");
    writer.String(libCZI::Utils::PixelTypeToInformalString(subblockInfo.pixelType));

    writer.Key("Coordinate");
    writer.StartObject();
    subblockInfo.coordinate.EnumValidDimensions(
        [&](DimensionIndex dim, int v)->bool
    {
        char dimensionStr[2];
        dimensionStr[0] = libCZI::Utils::DimensionToChar(dim);
        dimensionStr[1] = '\0';
        writer.Key(dimensionStr);
        writer.Int(v);
        return true;
    });

    writer.EndObject();

    if (subblockInfo.mIndex != std::numeric_limits<int>::max())
    {
        writer.Key("Mindex");
        writer.Int(subblockInfo.mIndex);
    }

    writer.Key("LogicalRect");
    writer.StartArray();
    writer.Int(subblockInfo.logicalRect.x);
    writer.Int(subblockInfo.logicalRect.y);
    writer.Int(subblockInfo.logicalRect.w);
    writer.Int(subblockInfo.logicalRect.h);
    writer.EndArray();

    writer.Key("PhysicalSize");
    writer.StartArray();
    writer.Int(subblockInfo.physicalSize.w);
    writer.Int(subblockInfo.physicalSize.h);
    writer.EndArray();

    writer.EndObject();
    return buffer.GetString();
}