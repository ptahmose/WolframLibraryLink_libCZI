#include <gtest/gtest.h>
#include <string>
#define _WLLCZISTATICLIB
#include <inc/exportedFunctions.h>

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)

using namespace std;

TEST(WLLFunctionsTests, WolframLibrary_getVersion)
{
    auto v = WolframLibrary_getVersion();
    EXPECT_EQ(v, WolframLibraryVersion);
}

static string GetSampleFilename(const char* sz)
{
    string s = EXPAND_AND_QUOTE(WLLCZITESTSAMPLEFILEDIR);
    s += "/";
    s += sz;
    return s;
}

static void (*pfnRegisterInstance)(WolframLibraryData, mbool, mint);

static int CallbackRegisterLibraryExpressionManager(const char* mname, void (*mfun)(WolframLibraryData, mbool, mint))
{
    pfnRegisterInstance = mfun;
    return 0;
}

static int CallbackUnregisterLibraryExpressionManager(const char* mname)
{
    return 0;
}

static void Callback_Null_UTF8String_disown(char*)
{}

TEST(WLLFunctionsTests, CZIReaderOpen)
{
    const int InstanceID = 1;
    st_WolframLibraryData wld;
    memset(&wld, 0, sizeof(st_WolframLibraryData));
    wld.registerLibraryExpressionManager = CallbackRegisterLibraryExpressionManager;
    wld.unregisterLibraryExpressionManager = CallbackUnregisterLibraryExpressionManager;
    wld.UTF8String_disown = Callback_Null_UTF8String_disown;

    WolframLibrary_initialize(&wld);
    pfnRegisterInstance(&wld, 0, InstanceID);

    mint arg0Integer;
    char* arg1String;
    MArgument inArgs[2];
    inArgs[0].integer = &arg0Integer;
    inArgs[1].utf8string = &arg1String;
    MArgument_setInteger(inArgs[0], 1);

    string filename = GetSampleFilename("test1.czi");
    char* szFilename = (char*)filename.c_str();
    MArgument_setUTF8String(inArgs[1], szFilename);
    MArgument out = { 0 };
    int r = CZIReader_Open(&wld, 2, inArgs, out);

    EXPECT_EQ(r, LIBRARY_NO_ERROR);

    pfnRegisterInstance(&wld, 1, InstanceID);

    WolframLibrary_uninitialize(&wld);
}

TEST(WLLFunctionsTests, CZIReaderOpenAndGetInfo)
{
    const int InstanceID = 1;
    st_WolframLibraryData wld;
    memset(&wld, 0, sizeof(st_WolframLibraryData));
    wld.registerLibraryExpressionManager = CallbackRegisterLibraryExpressionManager;
    wld.unregisterLibraryExpressionManager = CallbackUnregisterLibraryExpressionManager;
    wld.UTF8String_disown = Callback_Null_UTF8String_disown;

    WolframLibrary_initialize(&wld);
    pfnRegisterInstance(&wld, 0, InstanceID);

    mint arg0Integer;
    char* arg1String;
    MArgument inArgs[2];
    inArgs[0].integer = &arg0Integer;
    inArgs[1].utf8string = &arg1String;
    MArgument_setInteger(inArgs[0], 1);

    string filename = GetSampleFilename("test1.czi");
    char* szFilename = (char*)filename.c_str();
    MArgument_setUTF8String(inArgs[1], szFilename);
    MArgument out = { 0 };
    int r = CZIReader_Open(&wld, 2, inArgs, out);

    EXPECT_EQ(r, LIBRARY_NO_ERROR);

    char* resultString = nullptr;
    out.utf8string = &resultString;
    r = CZIReader_GetInfo(&wld,1, inArgs, out);

    EXPECT_EQ(r, LIBRARY_NO_ERROR);
    EXPECT_NE(resultString, nullptr);
    EXPECT_GE(strlen(resultString), 2);

    pfnRegisterInstance(&wld, 1, InstanceID);

    WolframLibrary_uninitialize(&wld);
}