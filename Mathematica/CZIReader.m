BeginPackage[ "CZIReader`"]

 OpenCZI::usage = 
	"OpenCZI[ x] opems a CZI file.";
	
 CZIGetSubBlock::usage =
    "CZIGetSubBlock[ x , n]";
	
Begin[ "Private`"]

$wllczilibrary = "D:\\Dev\\GitHub\\WolframLibraryLink_libCZI\\out\\build\\x64-Debug\\wllczi\\wllczi.dll";

CziReaderOpen = LibraryFunctionLoad[
  $wllczilibrary,
  "CZIReader_Open",
  {Integer, {UTF8String}}, Integer];
  
  CziReaderInfo = LibraryFunctionLoad[
  $wllczilibrary,
  "CZIReader_GetInfo",
  {Integer}, UTF8String];

	CziGetSubBlockBitmap = LibraryFunctionLoad[
  $wllczilibrary,
  "CZIReader_GetSubBlockBitmap",
  {Integer, Integer}, LibraryDataType[Image]];

CziGetSingleChannelScaledBitmap = LibraryFunctionLoad[
  $wllczilibrary,
  "CZIReader_GetSingleChannelScalingTileComposite",
  {Integer, LibraryDataType[MNumericArray], UTF8String, 
   LibraryDataType[Real]}, LibraryDataType[Image]];

CziGetMultiChannelScalingTileCompositeBitmap = LibraryFunctionLoad[
  $wllczilibrary,
  "CZIReader_MultiChannelScalingTileComposite",
  {Integer, LibraryDataType[MNumericArray], UTF8String, 
   LibraryDataType[Real]}, LibraryDataType[Image]];

  OpenCZI[ x_] :=
    Module[ {exp},
      exp = CreateManagedLibraryExpression["CZIReader", reader1];
	  CziReaderOpen[
			ManagedLibraryExpressionID[exp], 
			x];
	  exp
    ]
	
  CZIGetSubBlock[c_,n_] :=
    Module[ {image},
      CziGetSubBlockBitmap[ManagedLibraryExpressionID[c],n]
    ]
	



  End[]

  EndPackage[]