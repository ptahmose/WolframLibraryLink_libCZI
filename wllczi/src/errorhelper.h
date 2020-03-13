#pragma once
#include <WolframLibrary.h>
#include "inc_libCzi.h"
#include <exception>
#include <string>

class ErrHelper
{
public:
    static std::string GetErrorText_CziReaderInstanceNotExisting(mint id);
    static std::string GetErrorText_CziReaderOpenException(std::exception& excp);
    static std::string GetErrorText_CziReaderOpenException(libCZI::LibCZIException& excp);
};