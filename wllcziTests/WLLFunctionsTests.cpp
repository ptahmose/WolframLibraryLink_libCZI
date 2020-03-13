#include <gtest/gtest.h>
#define _WLLCZISTATICLIB
#include <inc/exportedFunctions.h>

TEST(WLLFunctionsTests, WolframLibrary_getVersion)
{
    auto v = WolframLibrary_getVersion();
    EXPECT_EQ(v, WolframLibraryVersion);
}

static void (*pfnRegisterInstance)(WolframLibraryData, mbool, mint);

static int RegisterInstanceCallback(const char* mname, void (*mfun)(WolframLibraryData, mbool, mint))
{
    pfnRegisterInstance = mfun;
    return 0;
}

static void Callback_UTF8String_disown(char*)
{}

TEST(WLLFunctionsTests, CZIReaderOpen)
{
    st_WolframLibraryData wld;
    memset(&wld, 0, sizeof(st_WolframLibraryData));
    wld.registerLibraryExpressionManager = RegisterInstanceCallback;
    wld.UTF8String_disown = Callback_UTF8String_disown;

    WolframLibrary_initialize(&wld);
    pfnRegisterInstance(&wld, 0, 1);

    mint arg0Integer;
    char* arg1String;
    MArgument inArgs[2];
    inArgs[0].integer = &arg0Integer;
    inArgs[1].utf8string = &arg1String;
    MArgument_setInteger(inArgs[0], 1);
    MArgument_setUTF8String(inArgs[1], R"(D:\Dev\GitHub\WolframLibraryLink_libCZI\wllcziTests\samples\test1.czi)");
    MArgument out = { 0 };
    CZIReader_Open(&wld, 2, inArgs, out);
}