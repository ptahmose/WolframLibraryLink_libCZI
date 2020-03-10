#include "CziReader.h"
#include <codecvt>

void CziReader::Open(const std::string& utf8_filename)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> ucs2conv;
    std::wstring ucs2 = ucs2conv.from_bytes(utf8_filename);
    auto stream = libCZI::CreateStreamFromFile(ucs2.c_str());

    this->reader->Open(stream);
}