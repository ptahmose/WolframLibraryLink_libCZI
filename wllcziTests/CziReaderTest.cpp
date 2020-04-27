#include <gtest/gtest.h>
#include <string>
#define _WLLCZISTATICLIB
#include <inc/exportedFunctions.h>
#include <src/CziUtilities.h>
#include <src/CziReader.h>
#include "testutilities.h"


using namespace std;
using namespace libCZI;

TEST(CziReaderTests, GetMetadataFromSubBlockTest)
{
    CziReader reader;
    reader.Open(GetSampleFilename("test1.czi"));
    mint h = reader.ReadSubBlock(0);
    auto md = reader.GetMetadataFromSubBlock(h);
    EXPECT_TRUE(md == "<METADATA />");
}