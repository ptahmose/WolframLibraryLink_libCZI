#include <gtest/gtest.h>
#include <string>
#define _WLLCZISTATICLIB
#include <inc/exportedFunctions.h>
#include <src/CziUtilities.h>

using namespace std;
using namespace libCZI;

TEST(CziUtilitiesTests, ParseDisplaySettingsTest1)
{
    static const char* sz =
        R"(
    {
        "channels": [
                        {
                            "enabled": true,
                            "ch": 0,
                            "black-point" : 0.1,
                            "white-point" : 0.9,
                            "gamma" : 0.16091187298297886,
                            "spline-control-points" : [
                                    0.19685038924217225,
                                    0.7571428418159485,
                                    0.34645670652389529,
                                    0.04285714402794838]
                        }
                    ]
    }
        )";

    auto r = CziUtilities::ParseDisplaySettings(sz);

    EXPECT_EQ(r.size(), 1);
    const auto& chds = r.at(0);

    EXPECT_TRUE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::IsEnabled));
    EXPECT_TRUE(chds.channel_display_settings.isEnabled);

    EXPECT_TRUE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::BlackPoint));
    EXPECT_FLOAT_EQ(chds.channel_display_settings.blackPoint, 0.1f);

    EXPECT_TRUE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::WhitePoint));
    EXPECT_FLOAT_EQ(chds.channel_display_settings.whitePoint, 0.9f);

    EXPECT_TRUE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::Gamma));
    EXPECT_FLOAT_EQ(chds.channel_display_settings.gamma, 0.16091187298297886f);

    EXPECT_TRUE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::SplineCtrlPts));
    EXPECT_EQ(chds.channel_display_settings.splineCtrlPoints.size(), 2);
    EXPECT_FLOAT_EQ(chds.channel_display_settings.splineCtrlPoints[0].x, 0.19685038924217225f);
    EXPECT_FLOAT_EQ(chds.channel_display_settings.splineCtrlPoints[0].y, 0.7571428418159485f);
    EXPECT_FLOAT_EQ(chds.channel_display_settings.splineCtrlPoints[1].x, 0.34645670652389529f);
    EXPECT_FLOAT_EQ(chds.channel_display_settings.splineCtrlPoints[1].y, 0.04285714402794838f);

    EXPECT_FALSE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::GradationCurveMode));
    EXPECT_FALSE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::TintingColor));
}