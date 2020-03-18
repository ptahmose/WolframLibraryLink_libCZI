#include "CziUtilities.h"

using namespace libCZI;

/*static*/std::vector<std::shared_ptr<libCZI::IBitmapData>> CziUtilities::GetBitmapsFromSpecifiedChannels(libCZI::ICZIReader* reader, const libCZI::CDimCoordinate& planeCoordinate, const libCZI::IntRect& roi, float zoom, std::function<bool(int index, int& channelNo)> getChannelNo, libCZI::IntSize* ptrPixelSize)
{
    std::vector<std::shared_ptr<IBitmapData>> chBitmaps;
    libCZI::CDimCoordinate coordinate = planeCoordinate;

    auto subBlockStatistics = reader->GetStatistics();

    libCZI::ISingleChannelScalingTileAccessor::Options sctaOptions; sctaOptions.Clear();
    sctaOptions.backGroundColor = RgbFloatColor{ 0, 0, 0 };
    sctaOptions.drawTileBorder = false;

    auto accessor = reader->CreateSingleChannelScalingTileAccessor();

    for (int i = 0;; ++i)
    {
        int chNo;
        if (getChannelNo(i, chNo) == false)
        {
            break;
        }

        if (subBlockStatistics.dimBounds.IsValid(DimensionIndex::C))
        {
            // That's a cornerstone case - or a loophole in the specification: if the document
            // does not contain C-dimension (=none of the sub-blocks has a valid C-dimension),
            // then we must not set the C-dimension here. I suppose we should define that a
            // valid C-dimension is mandatory...
            coordinate.Set(DimensionIndex::C, chNo);
        }

        chBitmaps.emplace_back(accessor->Get(roi, &coordinate, zoom, &sctaOptions));
    }

    if (ptrPixelSize != nullptr)
    {
        if (!chBitmaps.empty())
        {
            *ptrPixelSize = chBitmaps[0]->GetSize();
        }
        else
        {
            *ptrPixelSize = accessor->CalcSize(roi, zoom);
        }
    }

    return chBitmaps;
}