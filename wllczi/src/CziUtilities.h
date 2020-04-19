#include "inc_libCzi.h"
#include <bitset>
#include <tuple>

#include <rapidjson/document.h>

struct ChannelDisplaySettingsValidity
{
    enum class Property
    {
        IsEnabled = 1,
        Weight = 2,
        TintingMode = 3,
        TintingColor = 4,
        BlackPoint = 5,
        WhitePoint = 6,
        GradationCurveMode = 7,
        Gamma = 8,
        SplineCtrlPts = 9,

        PropertyMax = 9
    };
private:
    std::bitset<(size_t)Property::PropertyMax> flags;
public:
    void Set(Property prop, bool value);
    bool Get(Property prop) const;
};

struct ChannelDisplaySettingsAndValidity
{
    libCZI::ChannelDisplaySettingsPOD channel_display_settings;
    ChannelDisplaySettingsValidity  validity;
};


class CziUtilities
{
public:
    static std::vector<std::shared_ptr<libCZI::IBitmapData>> GetBitmapsFromSpecifiedChannels(libCZI::ICZIReader* reader, const libCZI::CDimCoordinate& planeCoordinate, const libCZI::IntRect& roi, float zoom, std::function<bool(int index, int& channelNo)> getChannelNo, libCZI::IntSize* ptrPixelSize);

    static std::map<int, ChannelDisplaySettingsAndValidity> ParseDisplaySettings(const char* sz);
private:
    static const char* JsonKey_Channels;
    static const char* JsonKey_Ch;
    static const char* JsonKey_Enabled;
    static const char* JsonKey_BlackPoint;
    static const char* JsonKey_WhitePoint;
    static const char* JsonKey_TintingMode;
    static const char* JsonKey_TintingColor;
    static const char* JsonKey_GradationCurveMode;
    static const char* JsonKey_Gamma;
    static const char* JsonKey_SplinePoints;
    static std::tuple<int, ChannelDisplaySettingsAndValidity> ParseChannelDisplaySettings(const rapidjson::Value& v);
    static bool TryParseColor(const std::string& str, libCZI::Rgb8Color* ptrRgbColor);
};