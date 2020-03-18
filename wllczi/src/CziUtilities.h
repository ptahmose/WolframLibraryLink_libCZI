#include "inc_libCzi.h"


class CziUtilities
{
public:
    static std::vector<std::shared_ptr<libCZI::IBitmapData>> GetBitmapsFromSpecifiedChannels(libCZI::ICZIReader* reader, const libCZI::CDimCoordinate& planeCoordinate, const libCZI::IntRect& roi, float zoom, std::function<bool(int index, int& channelNo)> getChannelNo, libCZI::IntSize* ptrPixelSize);
};