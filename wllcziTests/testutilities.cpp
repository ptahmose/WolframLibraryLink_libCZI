#include "testutilities.h"

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)

using namespace std;

string GetSampleFilename(const char* sz)
{
    string s = EXPAND_AND_QUOTE(WLLCZITESTSAMPLEFILEDIR);
    s += "/";
    s += sz;
    return s;
}
