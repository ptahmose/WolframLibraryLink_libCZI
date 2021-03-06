#pragma once
#include <WolframLibrary.h>

// if linking with the static wllczi-library, the variable "_WLLCZISTATICLIB" should be defined.
#if !defined(_WLLCZISTATICLIB)

#ifdef WLLCZI_EXPORTS
#ifdef __GNUC__
#define WLLCZI_API __attribute__ ((visibility ("default")))
#else
#define WLLCZI_API __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define WLLCZI_API
#else
#define WLLCZI_API __declspec(dllimport)
#endif
#endif

#else

#define WLLCZI_API 

#endif


EXTERN_C WLLCZI_API int getLibraryInfo(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument Res);

EXTERN_C WLLCZI_API mint WolframLibrary_getVersion();

EXTERN_C WLLCZI_API int WolframLibrary_initialize(WolframLibraryData libData);

EXTERN_C WLLCZI_API void WolframLibrary_uninitialize(WolframLibraryData libData);

EXTERN_C WLLCZI_API int CZIReader_Open(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);

EXTERN_C WLLCZI_API int CZIReader_GetInfo(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);

EXTERN_C WLLCZI_API int CZIReader_GetSubBlockBitmap(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);

EXTERN_C WLLCZI_API int CZIReader_GetSingleChannelScalingTileComposite(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);

EXTERN_C WLLCZI_API int CZIReader_GetMultiChannelScalingTileComposite(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);

EXTERN_C WLLCZI_API int CZIReader_ReleaseInstance(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);

EXTERN_C WLLCZI_API int CZIReader_GetMetadataXml(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);

EXTERN_C WLLCZI_API int CZIReader_GetScaling(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);


EXTERN_C WLLCZI_API int CZIReader_ReadSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);
EXTERN_C WLLCZI_API int CZIReader_GetBitmapFromSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);
EXTERN_C WLLCZI_API int CZIReader_GetMetadataFromSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);
EXTERN_C WLLCZI_API int CZIReader_GetInfoFromSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);
EXTERN_C WLLCZI_API int CZIReader_ReleaseSubBlock(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);

EXTERN_C WLLCZI_API int CZIReader_GetLastErrorInfo(WolframLibraryData libData, mint Argc, MArgument* Args, MArgument res);


