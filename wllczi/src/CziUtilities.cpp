#include "CziUtilities.h"
#include <rapidjson/document.h>
#include "utils.h"

using namespace libCZI;
using namespace std;

void ChannelDisplaySettingsValidity::Set(Property prop, bool value)
{
    this->flags.set(static_cast<size_t>(prop) - 1, value);
}

bool ChannelDisplaySettingsValidity::Get(Property prop) const
{
    return this->flags.test(static_cast<size_t>(prop) - 1);
}

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

/*static*/std::map<int, ChannelDisplaySettingsAndValidity> CziUtilities::ParseDisplaySettings(const char* sz)
{
    rapidjson::Document document;
    document.Parse(sz);
    if (document.HasParseError())
    {
        throw std::logic_error("Invalid JSON");
    }

    bool isObj = document.IsObject();
    bool hasChannels = document.HasMember(CziUtilities::JsonKey_Channels);
    bool isChannelsArray = document[CziUtilities::JsonKey_Channels].IsArray();
    const auto& channels = document[CziUtilities::JsonKey_Channels];

    map<int, ChannelDisplaySettingsAndValidity> result;
    for (decltype(channels.Size()) i = 0; i < channels.Size(); ++i)
    {
        try
        {
            auto r = CziUtilities::ParseChannelDisplaySettings(channels[i]);

            result[get<0>(r)] = get<1>(r);
        }
        catch (logic_error& err)
        {
        }
    }

    return result;
}

/*static*/std::tuple<int, ChannelDisplaySettingsAndValidity> CziUtilities::ParseChannelDisplaySettings(const rapidjson::Value& v)
{
    if (v.HasMember(CziUtilities::JsonKey_Ch) == false)
    {
        throw std::logic_error("No channel specified");
    }

    ChannelDisplaySettingsAndValidity cds;
    cds.channel_display_settings.Clear();

    int chNo = v[CziUtilities::JsonKey_Ch].GetInt();

    if (v.HasMember(CziUtilities::JsonKey_Enabled))
    {
        cds.channel_display_settings.isEnabled = v[CziUtilities::JsonKey_Enabled].GetBool();
        cds.validity.Set(ChannelDisplaySettingsValidity::Property::IsEnabled, true);
    }

    if (v.HasMember(CziUtilities::JsonKey_BlackPoint))
    {
        cds.channel_display_settings.blackPoint = v[CziUtilities::JsonKey_BlackPoint].GetFloat();
        cds.validity.Set(ChannelDisplaySettingsValidity::Property::BlackPoint, true);
    }

    if (v.HasMember(CziUtilities::JsonKey_WhitePoint))
    {
        cds.channel_display_settings.whitePoint = v[CziUtilities::JsonKey_WhitePoint].GetFloat();
        cds.validity.Set(ChannelDisplaySettingsValidity::Property::WhitePoint, true);
    }

    if (v.HasMember(CziUtilities::JsonKey_TintingMode))
    {
        if (v[CziUtilities::JsonKey_TintingMode].IsString())
        {
            const auto& str = ::Utils::trim(v[CziUtilities::JsonKey_TintingMode].GetString());

            if (::Utils::icasecmp(str, "none"))
            {
                cds.channel_display_settings.tintingMode = IDisplaySettings::TintingMode::None;
                cds.validity.Set(ChannelDisplaySettingsValidity::Property::TintingMode, true);
            }
            else if (::Utils::icasecmp(str, "color"))
            {
                cds.channel_display_settings.tintingMode = IDisplaySettings::TintingMode::Color;
                cds.validity.Set(ChannelDisplaySettingsValidity::Property::TintingMode, true);
            }
        }
    }

    if (v.HasMember(CziUtilities::JsonKey_GradationCurveMode))
    {
        if (v[CziUtilities::JsonKey_GradationCurveMode].IsString())
        {
            const auto& str = ::Utils::trim(v[CziUtilities::JsonKey_GradationCurveMode].GetString());
            if (::Utils::icasecmp(str, "linear"))
            {
                cds.channel_display_settings.gradationCurveMode = IDisplaySettings::GradationCurveMode::Linear;
                cds.validity.Set(ChannelDisplaySettingsValidity::Property::GradationCurveMode, true);
            }
            else if (::Utils::icasecmp(str, "gamma"))
            {
                cds.channel_display_settings.gradationCurveMode = IDisplaySettings::GradationCurveMode::Gamma;
                cds.validity.Set(ChannelDisplaySettingsValidity::Property::GradationCurveMode, true);
            }
            else if (::Utils::icasecmp(str, "spline"))
            {
                cds.channel_display_settings.gradationCurveMode = IDisplaySettings::GradationCurveMode::Spline;
                cds.validity.Set(ChannelDisplaySettingsValidity::Property::GradationCurveMode, true);
            }
        }
    }

    if (v.HasMember(CziUtilities::JsonKey_Gamma))
    {
        cds.channel_display_settings.gamma = v[CziUtilities::JsonKey_Gamma].GetFloat();
        cds.validity.Set(ChannelDisplaySettingsValidity::Property::Gamma, true);
    }

    if (v.HasMember(CziUtilities::JsonKey_SplinePoints))
    {
        const rapidjson::Value& sp = v[CziUtilities::JsonKey_SplinePoints];
        if (!sp.IsArray())
        {
            throw std::logic_error("Invalid JSON");
        }

        for (rapidjson::Value::ConstValueIterator it = sp.Begin(); it != sp.End(); ++it)
        {
            double d1 = it->GetDouble();
            ++it;
            if (it == sp.End())
                break;
            double d2 = it->GetDouble();
            cds.channel_display_settings.splineCtrlPoints.push_back(IDisplaySettings::SplineControlPoint(d1, d2));
        }

        if (!cds.channel_display_settings.splineCtrlPoints.empty())
        {
            cds.validity.Set(ChannelDisplaySettingsValidity::Property::SplineCtrlPts, true);
        }
    }

    return make_tuple(chNo, cds);
}

/*static*/const char* CziUtilities::JsonKey_Channels = "channels";
/*static*/const char* CziUtilities::JsonKey_Ch = "ch";
/*static*/const char* CziUtilities::JsonKey_Enabled = "enabled";
/*static*/const char* CziUtilities::JsonKey_BlackPoint = "black-point";
/*static*/const char* CziUtilities::JsonKey_WhitePoint = "white-point";;
/*static*/const char* CziUtilities::JsonKey_TintingMode = "tinting-mode";
/*static*/const char* CziUtilities::JsonKey_TintingColor = "tinting-color";
/*static*/const char* CziUtilities::JsonKey_GradationCurveMode = "gradation-curve-mode";
/*static*/const char* CziUtilities::JsonKey_Gamma = "gamma";
/*static*/const char* CziUtilities::JsonKey_SplinePoints = "spline-control-points";