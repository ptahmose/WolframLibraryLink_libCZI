#include <gtest/gtest.h>
#define _WLLCZISTATICLIB
#include <inc/exportedFunctions.h>

TEST(WLLFunctionsTests, WolframLibrary_getVersion)
{
    auto v = WolframLibrary_getVersion();
    EXPECT_EQ(v, WolframLibraryVersion);
}
