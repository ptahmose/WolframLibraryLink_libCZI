#include <gtest/gtest.h>
#include <string>
#define _WLLCZISTATICLIB
#include <inc/exportedFunctions.h>
#include <src/CziUtilities.h>
#include <src/CziReader.h>
#include "testutilities.h"
#include "rapidjson/document.h"

using namespace std;
using namespace libCZI;

TEST(CziReaderTests, GetMetadataFromSubBlockTest)
{
    CziReader reader;
    reader.Open(GetSampleFilename("test1.czi"));
    mint h = reader.ReadSubBlock(0);
    auto md = reader.GetMetadataFromSubBlock(h);
    EXPECT_TRUE(md == "<METADATA />");
    bool b = reader.ReleaseSubBlock(h);
    EXPECT_TRUE(b);
}

TEST(CziReaderTests, GetInfoSubBlockTest)
{
    CziReader reader;
    reader.Open(GetSampleFilename("test1.czi"));
    mint h = reader.ReadSubBlock(0);
    auto info = reader.GetInfoFromSubBlock(h);

    rapidjson::Document document;
    document.Parse(info.c_str());
    EXPECT_FALSE(document.HasParseError());
    EXPECT_TRUE(document.IsObject());
    EXPECT_TRUE(document.HasMember("CompressionMode"));
    EXPECT_TRUE(document.HasMember("Pixeltype"));
    EXPECT_TRUE(document.HasMember("Coordinate"));
    EXPECT_TRUE(document.HasMember("LogicalRect"));
    EXPECT_TRUE(document.HasMember("PhysicalSize"));

    bool b = reader.ReleaseSubBlock(h);
    EXPECT_TRUE(b);

}