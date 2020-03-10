#pragma once

#include "inc_libCzi.h"

class CziReader
{
private:
    std::shared_ptr<libCZI::ICZIReader> reader;
public:
    CziReader() : reader(libCZI::CreateCZIReader())
    {
    }

    void Open(const std::string& utf8_filename);
};