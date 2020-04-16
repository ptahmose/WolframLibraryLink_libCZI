#include "getLibInfo.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "inc_libCzi.h"

using namespace std;
using namespace rapidjson;
using namespace libCZI;

/*static*/const char* CLibraryInfo::KeyVersionString = "VersionString";
/*static*/const char* CLibraryInfo::KeyLibraryName = "LibraryName";
/*static*/const char* CLibraryInfo::KeyCompilerIdentification = "CompilerIdentification";


/*static*/const char* CLibraryInfo::VersionString = "0.0.1-dev";
/*static*/const char* CLibraryInfo::LibraryName = "wllczi";

/*static*/void CLibraryInfo::EnumKeys(std::function<bool(const char*)> enumFunc)
{
    if (!enumFunc(CLibraryInfo::KeyVersionString)) return;
    if (!enumFunc(CLibraryInfo::KeyLibraryName)) return;
    if (!enumFunc(CLibraryInfo::KeyCompilerIdentification)) return;
}

/*static*/bool CLibraryInfo::GetValue(const std::string& key, std::string& value)
{
    if (key == CLibraryInfo::KeyVersionString)
    {
        value = CLibraryInfo::VersionString;
        return true;
    }

    if (key == CLibraryInfo::KeyLibraryName)
    {
        value = CLibraryInfo::LibraryName;
        return true;
    }

    if (key == CLibraryInfo::KeyCompilerIdentification)
    {
        BuildInformation info;
        libCZI::GetLibCZIBuildInformation(info);
        value = info.compilerIdentification;
        return true;
    }

    return false;
}

/*static*/std::string CLibraryInfo::GetAllInfoAsJSON()
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    writer.StartObject();

    CLibraryInfo::EnumKeys(
        [&](const char* key)->bool
    {
        string value;
        if (CLibraryInfo::GetValue(key, value))
        {
            writer.Key(key);
            writer.String(value.c_str());
        }

        return true;
    }
    );

    writer.EndObject();

    return buffer.GetString();
}