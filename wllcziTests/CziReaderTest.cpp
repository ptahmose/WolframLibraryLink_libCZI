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
    auto _ = finally([h,&reader]()->void { reader.ReleaseSubBlock(h); });
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

    EXPECT_TRUE(document["CompressionMode"].IsString());
    EXPECT_TRUE(strcmp(document["CompressionMode"].GetString(), "uncompressed") == 0);
    EXPECT_TRUE(document["Pixeltype"].IsString());
    EXPECT_TRUE(strcmp(document["Pixeltype"].GetString(), "bgr24") == 0);
    EXPECT_TRUE(document["Coordinate"].IsObject());
    EXPECT_TRUE(document["Coordinate"].GetObject().HasMember("T"));
    EXPECT_TRUE(document["Coordinate"].GetObject()["T"].IsInt());
    EXPECT_TRUE(document["Coordinate"].GetObject()["T"].GetInt() == 0);

    EXPECT_TRUE(document["LogicalRect"].IsArray());
    EXPECT_TRUE(document["LogicalRect"].GetArray().Size() == 4);
    EXPECT_TRUE(document["LogicalRect"].GetArray()[0] == 0);
    EXPECT_TRUE(document["LogicalRect"].GetArray()[1] == 0);
    EXPECT_TRUE(document["LogicalRect"].GetArray()[2] == 1024);
    EXPECT_TRUE(document["LogicalRect"].GetArray()[3] == 768);

    EXPECT_TRUE(document["PhysicalSize"].IsArray());
    EXPECT_TRUE(document["PhysicalSize"].GetArray().Size() == 2);
    EXPECT_TRUE(document["PhysicalSize"].GetArray()[0] == 1024);
    EXPECT_TRUE(document["PhysicalSize"].GetArray()[1] == 768);
}