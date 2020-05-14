#include "errorhelper.h"
#include "dbgprint.h"
#include <sstream>

using namespace std;

/*static*/const char* ErrHelper::packageError_OpenCZI_failed = "OpenCZIopenreadfail";
/*static*/const char* ErrHelper::packageError_CziReaderInstanceDoesNotExist = "CziReaderInstanceDoesNotExist";
/*static*/const char* ErrHelper::packageError_WrongNumberOfArgument = "CziReaderWrongNumberOfArguments";
/*static*/const char* ErrHelper::packageError_GetSingleChannelScalingTileCompositeRoiInvalid = "CziReaderGetSingleChannelScalingTileCompositeRoiInvalid";
/*static*/const char* ErrHelper::packageError_GetSingleChannelScalingTileCompositeParseCoordinateException = "CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException";
/*static*/const char* ErrHelper::packageError_GetSingleChannelScalingTileCompositeBackgroundColorInvalid = "CziReaderGetSingleChannelScalingTileCompositeBackgroundColorInvalid";
/*static*/const char* ErrHelper::packageError_GetSingleChannelScalingTileCompositeException = "CziReaderGetSingleChannelScalingTileCompositeException";
/*static*/const char* ErrHelper::packageError_GetMultiChannelScalingTileCompositeParseCoordinateException = "CziReaderGetMultiChannelScalingTileCompositeParseCoordinateException";

/*static*/CLastErrorStore ErrHelper::lastError;

// ----------------

/*static*/void ErrHelper::ReportError_WrongNumberOfArguments(WolframLibraryData libData)
{
    libData->Message(ErrHelper::packageError_WrongNumberOfArgument);
}

/*static*/void ErrHelper::ReportError_CziReaderOpenException(WolframLibraryData libData, std::exception& excp)
{
    CDbg::PrintL(CDbg::Level::Error,
        [&]()->string
        {
            stringstream ss;
            ss << "Error in 'CZIReader_Open': \"" << excp.what() << "\"";
            return ss.str();
        });
    libData->Message(ErrHelper::packageError_OpenCZI_failed);
}

/*static*/void ErrHelper::ReportError_CziReaderOpenException(WolframLibraryData libData, libCZI::LibCZIException& excp)
{
    CDbg::PrintL(CDbg::Level::Error,
        [&]()->string
        {
            stringstream ss;
            ss << "Error in 'CZIReader_Open': \"" << excp.what() << "\"";
            return ss.str();
        });
    libData->Message(ErrHelper::packageError_OpenCZI_failed);
}

/*static*/void ErrHelper::ReportError_CziReaderInstanceNotExisting(WolframLibraryData libData, mint id)
{
    CDbg::PrintL(CDbg::Level::Error,
        [=]()->string
        {
            return ErrHelper::GetErrorText_CziReaderInstanceNotExisting(id);
        });
    libData->Message(ErrHelper::packageError_OpenCZI_failed);
}

/*static*/void ErrHelper::ReportError_CziReaderGetSingleChannelScalingTileCompositeRoiInvalid(WolframLibraryData libData)
{
    ErrHelper::lastError.SetLastErrorInfo("Error in 'CZIReader_GetSingleChannelScalingTileComposite': Error with ROI-argument");
    libData->Message(ErrHelper::packageError_GetSingleChannelScalingTileCompositeRoiInvalid);
}

/*static*/void ErrHelper::ReportError_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(WolframLibraryData libData, const char* coordinateString, libCZI::LibCZIStringParseException& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_GetSingleChannelScalingTileComposite': Coordinate \""
        << coordinateString << "\" could not be parsed, stopped at character no " <<
        1 + excp.GetNumberOfCharsParsedOk() << ".";
    ErrHelper::lastError.SetLastErrorInfo(ss.str());
    libData->Message(ErrHelper::packageError_GetSingleChannelScalingTileCompositeParseCoordinateException);
}

/*static*/void ErrHelper::ReportError_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(WolframLibraryData libData, const char* coordinateString, std::exception& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_GetSingleChannelScalingTileComposite': Coordinate \""
        << coordinateString << "\" could not be parsed -> " << excp.what() << ".";
    ErrHelper::lastError.SetLastErrorInfo(ss.str());
    libData->Message(ErrHelper::packageError_GetSingleChannelScalingTileCompositeParseCoordinateException);
}

/*static*/void ErrHelper::ReportError_CziReaderGetSingleChannelScalingTileCompositeBackgroundColorInvalid(WolframLibraryData libData)
{
    ErrHelper::lastError.SetLastErrorInfo("Error in 'CZIReader_GetSingleChannelScalingTileComposite': Error with BackgroundColor-argument");
    libData->Message(ErrHelper::packageError_GetSingleChannelScalingTileCompositeBackgroundColorInvalid);
}

/*static*/void ErrHelper::ReportError_CziReaderGetSingleChannelScalingTileCompositeException(WolframLibraryData libData, std::exception& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_GetSingleChannelScalingTileComposite': \"" << excp.what() << "\"";
    ErrHelper::lastError.SetLastErrorInfo(ss.str());
    libData->Message(ErrHelper::packageError_GetSingleChannelScalingTileCompositeException);
}

/*static*/void ErrHelper::ReportError_CziReaderGetMultiChannelScalingTileCompositeParseCoordinateException(WolframLibraryData libData, const char* coordinateString, libCZI::LibCZIStringParseException& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_GetMultiChannelScalingTileComposite': Coordinate \""
        << coordinateString << "\" could not be parsed, stopped at character no " <<
        1 + excp.GetNumberOfCharsParsedOk() << ".";
    ErrHelper::lastError.SetLastErrorInfo(ss.str());
    libData->Message(ErrHelper::packageError_GetMultiChannelScalingTileCompositeParseCoordinateException);
}

/*static*/void ErrHelper::ReportError_CziReaderGetMultiChannelScalingTileCompositeParseCoordinateException(WolframLibraryData libData, const char* coordinateString, std::exception& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_GetMultiChannelScalingTileComposite': Coordinate \""
        << coordinateString << "\" could not be parsed -> " << excp.what() << ".";
    ErrHelper::lastError.SetLastErrorInfo(ss.str());
    libData->Message(ErrHelper::packageError_GetMultiChannelScalingTileCompositeParseCoordinateException);
}

// ----------------

/*static*/std::string ErrHelper::GetErrorText_CziReaderInstanceNotExisting(mint id)
{
    stringstream ss;
    ss << "No CziReader-object found for Id=\"" << id << "\"";
    return ss.str();
}

/*static*/std::string ErrHelper::GetErrorText_CziReaderGetSubBlockBitmapException(std::exception& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_GetSubBlockBitmap': \"" << excp.what() << "\"";
    return ss.str();
}

/*static*/std::string ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeException(std::exception& excp)
{
    stringstream ss;
    ss << "Error in 'CZIReader_GetSingleChannelScalingTileComposite': \"" << excp.what() << "\"";
    return ss.str();
}

///*static*/std::string ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(const char* coordinateString, libCZI::LibCZIStringParseException& excp)
//{
//    stringstream ss;
//    ss << "Error in 'CZIReader_GetSingleChannelScalingTileComposite': Coordinate \""
//        << coordinateString << "\" could not be parsed, stopped at character no " <<
//        1 + excp.GetNumberOfCharsParsedOk() << ".";
//    return ss.str();
//}
//
///*static*/std::string ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException(const char* coordinateString, std::exception& excp)
//{
//    stringstream ss;
//    ss << "Error in 'CZIReader_GetSingleChannelScalingTileComposite': Coordinate \""
//        << coordinateString << "\" could not be parsed -> " << excp.what() << ".";
//    return ss.str();
//}

/*static*/std::string ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeRoiInvalid()
{
    return string("Error in 'CZIReader_GetSingleChannelScalingTileComposite': Error with ROI-argument");
}

/*static*/std::string ErrHelper::GetErrorText_CziReaderGetSingleChannelScalingTileCompositeBackgroundColorInvalid()
{
    return string("Error in 'CZIReader_GetSingleChannelScalingTileComposite': Error with BackgroundColor-argument");
}

/*static*/std::string ErrHelper::GetErrorText_CziGetMetadataXml(std::exception& excp)
{
    stringstream ss;
    ss << "Error retrieving XML-metadata: \"" << excp.what() << "\"";
    return ss.str();
}

/*static*/std::string ErrHelper::GetErrorText_CziGetMetadataXml(libCZI::LibCZIException& excp)
{
    stringstream ss;
    ss << "Error retrieving XML-metadata: \"" << excp.what() << "\"";
    return ss.str();
}
