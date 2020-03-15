#include "errorhelper.h"
#include <sstream>

using namespace std;

/*static*/std::string ErrHelper::GetErrorText_CziReaderInstanceNotExisting(mint id)
{
    stringstream ss;
    ss << "No CziReader-object found for Id=\"" << id << "\"";
    return ss.str();
}

/*static*/std::string ErrHelper::GetErrorText_CziReaderOpenException(std::exception& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_Open': \"" << excp.what() << "\"";
    return ss.str();
}

/*static*/std::string ErrHelper::GetErrorText_CziReaderOpenException(libCZI::LibCZIException& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_Open': \"" << excp.what() << "\"";
    return ss.str();
}

/*static*/std::string ErrHelper::GetErrorText_CziReaderGetSubBlockBitmapException(std::exception& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_GetSubBlockBitmap': \"" << excp.what() << "\"";
    return ss.str();
}