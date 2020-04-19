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

    EXPECT_EQ(r.displaySettings.size(), 1);
    const auto& chds = r.displaySettings.at(0);

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
    EXPECT_DOUBLE_EQ(chds.channel_display_settings.splineCtrlPoints[0].x, 0.19685038924217225);
    EXPECT_DOUBLE_EQ(chds.channel_display_settings.splineCtrlPoints[0].y, 0.7571428418159485);
    EXPECT_DOUBLE_EQ(chds.channel_display_settings.splineCtrlPoints[1].x, 0.34645670652389529);
    EXPECT_DOUBLE_EQ(chds.channel_display_settings.splineCtrlPoints[1].y, 0.04285714402794838);

    EXPECT_FALSE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::GradationCurveMode));
    EXPECT_FALSE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::TintingColor));
}

TEST(CziUtilitiesTests, ParseDisplaySettingsTest2)
{
    // the "ch" node must present, otherwise we expect an empty result
    static const char* sz =
        R"(
    {
        "channels": [
                        {
                            "enabled": true,
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

    EXPECT_EQ(r.displaySettings.size(), 0);
}

TEST(CziUtilitiesTests, ParseDisplaySettingsTest3)
{
    // with some garbage, we expect an exception
    static const char* sz =
        R"(
    Blah dfsjkl sdasfadf
        )";

    try
    {
        auto r = CziUtilities::ParseDisplaySettings(sz);
        FAIL() << "expected an exception";
    }
    catch (const invalid_argument&)
    {
        SUCCEED();
    }
}

TEST(CziUtilitiesTests, ParseDisplaySettingsTest4)
{
    // the "ch" node must present, otherwise we expect an empty result
    static const char* sz =
        R"(
    {
        "channels": [
                        {
                            "ch": 0,
                            "enabled": true,
                            "black-point" : 0.1,
                            "white-point" : 0.9,
                            "gamma" : 0.16091187298297886,
                            "tinting-color" : "#123456",
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

    EXPECT_EQ(r.displaySettings.size(), 1);
    const auto& chds = r.displaySettings.at(0);

    EXPECT_TRUE(chds.validity.Get(ChannelDisplaySettingsValidity::Property::TintingColor));
    EXPECT_EQ(chds.channel_display_settings.tintingColor.r, 0x12);
    EXPECT_EQ(chds.channel_display_settings.tintingColor.g, 0x34);
    EXPECT_EQ(chds.channel_display_settings.tintingColor.b, 0x56);
}