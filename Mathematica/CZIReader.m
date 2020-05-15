(* ::Package:: *)

BeginPackage[ "CZIReader`"]

(* Export section *)

(* Every function that the package provides will have a usage message here. *)
(* There will be no function definitions in this section, only usage messages. *)
(* Public functions will have names starting with capitals, by convention. *)

CZIReader`GetCZIReaderLibraryInfo::usage = 
    "Get version/build information about 'CZIReader'."

CZIReader`OpenCZI::usage = 
	  "OpenCZI[filename] opens a CZI file.";

CZIReader`ReleaseCZI::usage = 
    "Releases the CZI-object. Note that if the object gets garbage-collected, then it will be released automatically. See e.g. https://reference.wolfram.com/language/ref/CreateManagedLibraryExpression.html";

CZIReader`CZIGetInfo::usage =
    "CZIGetInfo[fileobj] gets statistics about the document.";
	
CZIReader`CZIGetSubBlock::usage =
    "CZIGetSubBlock[fileobj,n] read the specified subblock.";

CZIReader`CZISingleChannelScaledComposite::usage = 
    "CZISingleChannelScaledComposite[fileobj,x,y,w,h,zoom,coord,backgroundColor] gets a single-channel tile-composite.";

CZIReader`CZIMultiChannelScaledComposite::usage = 
    "CZIMultiChannelScaledComposite[fileobj,x,y,w,h,zoom,coord,displaySettings] gets the multi-channel multi-tile composite.";

CZIReader`CZIGetMetadataXml::usage = 
    "CZIGetMetadataXml[fileobj] gets the XML-metadata.";

CZIReader`CZIGetScaling::usage = 
    "CZIGetScaling[fileobj] gets the scaling in X, Y and Z in units of meter.";

CZIReader`CZIGetSubBlockData::usage =
    "CZIGetSubBlockData[fileobj, n,  options] gets various data from the subblock with index 'n'."

(* Messages *)

LibraryFunction::OpenCZIopenreadfail = "Could not open the specified file.";
LibraryFunction::CziReaderInstanceDoesNotExist = "No instance exists for the specified id.";
LibraryFunction::CziReaderWrongNumberOfArguments = "Wrong number of arguments in call to function.";
LibraryFunction::CziReaderGetSingleChannelScalingTileCompositeRoiInvalid = "Error with parsing the ROI-argument in the function 'CZISingleChannelScaledComposite'.";
LibraryFunction::CziReaderGetSingleChannelScalingTileCompositeParseCoordinateException = "Error with parsing the coordinate-argument in the function 'CZISingleChannelScaledComposite'.";
LibraryFunction::CziReaderGetSingleChannelScalingTileCompositeBackgroundColorInvalid = "Error with parsing the background-color-argument in the function 'CZISingleChannelScaledComposite'.";
LibraryFunction::CziReaderGetSingleChannelScalingTileCompositeException = "Error occurred generating the tile-composite in the function 'CZISingleChannelScaledComposite'.";
LibraryFunction::CziReaderGetMultiChannelScalingTileCompositeParseCoordinateException = "Error with parsing the coordinate-argument in the function 'CZIMultiChannelScaledComposite'.";
LibraryFunction::CziReaderGetMultiChannelScalingTileCompositeException = "Error occurred generating the tile-composite in the function 'CZIMultiChannelScaledComposite'.";
LibraryFunction::CziReaderGetMetadataXmlException = "Error in 'CZIGetMetadataXml'.";
LibraryFunction::CziReaderReadSubBlockException = "Error in reading subblock.";
LibraryFunction::CziReaderGetBitmapFromSubBlockException = "Error in 'GetBitmapFromSubBlock'.";
LibraryFunction::CziReaderGetMetadataFromSubBlockException = "Error in 'GetMetadataFromSubBlock'.";
LibraryFunction::CziReaderGetInfoFromSubBlockException = "Error in 'GetInfoFromSubBlock'.";
LibraryFunction::CziReaderReleaseSubBlockException = "Error in 'ReleaseSubBlock'.";
LibraryFunction::CziReaderGetSubBlockBitmapException = "Error occurred in 'GetSubBlockBitmapException'.";


Begin["`Private`"]

(* Implementation section *)

$wllczilibrary = $Failed

libraryfunctionload[func_,argtype_,rettype_] :=
    Module[{},
        If[
         FailureQ[$wllczilibrary],
         $wllczilibrary = FindLibrary["D:\\Dev\\GitHub\\WolframLibraryLink_libCZI\\out\\build\\x64-Debug\\wllczi\\wllczi.dll"]
	    ];

        If[
         FailureQ[$wllczilibrary],
         $wllczilibrary = FindLibrary["/home/pi/dev/BuildWolframLibraryLink_libCZI/wllczi/libwllczi.so"]
	    ];

        Return[
          LibraryFunctionLoad[$wllczilibrary, func,  argtype, rettype]
	    ];
      ]

GetLibraryInfo = libraryfunctionload[
  "getLibraryInfo",
  {}, UTF8String];

CziReaderOpen = libraryfunctionload[
  "CZIReader_Open",
  {Integer, {UTF8String}}, Integer];

CziReaderReleaseInstance = libraryfunctionload[
  "CZIReader_ReleaseInstance",
  {Integer}, "Void"];
  
CziReaderInfo = libraryfunctionload[
  "CZIReader_GetInfo",
  {Integer}, UTF8String];

CziGetSubBlockBitmap = libraryfunctionload[
  "CZIReader_GetSubBlockBitmap",
  {Integer, Integer}, LibraryDataType[Image]];

CziGetSingleChannelScaledBitmap2 = libraryfunctionload[
  "CZIReader_GetSingleChannelScalingTileComposite",
  {Integer, LibraryDataType[MNumericArray], UTF8String, LibraryDataType[Real], LibraryDataType[MNumericArray]}, 
  LibraryDataType[Image]];

CziGetMultiChannelScalingTileCompositeBitmap = libraryfunctionload[
  "CZIReader_GetMultiChannelScalingTileComposite",
  {Integer, LibraryDataType[MNumericArray], UTF8String, LibraryDataType[Real], UTF8String}, 
  LibraryDataType[Image]];

CziGetMetadataXml = libraryfunctionload[
  "CZIReader_GetMetadataXml",
  {Integer}, UTF8String];

CziGetScaling = libraryfunctionload[
  "CZIReader_GetScaling",
  {Integer}, {Real, 1}];

CziHandleForSubBlock = libraryfunctionload[
  "CZIReader_ReadSubBlock",
  {Integer, Integer}, Integer]; 

CziGetInfoForSubBlockHandle = libraryfunctionload[
  "CZIReader_GetInfoFromSubBlock",
  {Integer, Integer}, UTF8String]; 

CziGetMetadataXmlForSubBlockHandle = libraryfunctionload[
  "CZIReader_GetMetadataFromSubBlock",
  {Integer, Integer}, UTF8String]; 

CziGetBitmapForSubBlockHandle = libraryfunctionload[
  "CZIReader_GetBitmapFromSubBlock",
   {Integer, Integer}, LibraryDataType[Image]];

CziReleaseSubBlockHandle = libraryfunctionload[
  "CZIReader_ReleaseSubBlock",
  {Integer, Integer}, "Void"]; 

CziGetLastError = libraryfunctionload[
  "CZIReader_GetLastErrorInfo",
  {}, UTF8String];

RetOrPrintError[x_] :=
  Return[If[Head[x]===LibraryFunctionError,Print[CziGetLastError[]];$Failed,x]];

CZIReader`GetCZIReaderLibraryInfo[] :=
  Module[{},
    Return[GetLibraryInfo[]];
  ]

CZIReader`OpenCZI[ x_] :=
    Module[ {exp,r, msg},
          exp = CreateManagedLibraryExpression["CZIReader", reader1];
	        r = CziReaderOpen[ManagedLibraryExpressionID[exp], x];
	        Return[If[Head[r]===LibraryFunctionError,Print[CziGetLastError[]];$Failed,exp]];
    ]

CZIReader`ReleaseCZI[ c_] :=
    Module[{},
      CziReaderReleaseInstance[ManagedLibraryExpressionID[c]];
    ]

CZIReader`CZIGetInfo[ c_ ] :=
    Module[{},
      RetOrPrintError[CziReaderInfo[ManagedLibraryExpressionID[c]]];
    ]
	
CZIReader`CZIGetSubBlock[c_,n_] :=
    Module[ {bitmap},
      bitmap = CziGetSubBlockBitmap[ManagedLibraryExpressionID[c],n];
      RetOrPrintError[bitmap];
    ]

CZIReader`CZISingleChannelScaledComposite[c_,x_,y_,w_,h_,zoom_,coord_] := CZISingleChannelScaledComposite[c,x,y,w,h,zoom,coord,{0,0,0}];

CZIReader`CZISingleChannelScaledComposite[c_,x_,y_,w_,h_,zoom_,coord_,backGroundColor_] :=
	Module[{roi,img,coordstr},
      roi = NumericArray[{x,y,w,h},"Integer32"];
      coordstr = If[ 
                    StringQ[coord],
                    coord,
                    If[
                        ListQ[coord],
                        StringJoin[Map[StringJoin[Part[#, 1], ToString[Part[#, 2]]] &, coord, 1]],
                        ""
                    ]
                    ];

      img = CziGetSingleChannelScaledBitmap2[
        ManagedLibraryExpressionID[c],
        roi,
        coordstr,
        zoom,
        NumericArray[Take[Join[backGroundColor,{Last[backGroundColor],Last[backGroundColor]}],3],"Real32","ClipAndCoerce"]];
      RetOrPrintError[img];
    ]

CZIReader`CZIMultiChannelScaledComposite[c_,x_,y_,w_,h_,zoom_,coord_,displaySettings_:""] :=
    Module[{roi,img,coordstr},
      roi = NumericArray[{x,y,w,h},"Integer32"];
      coordstr = coordArgumentToString[coord];

      img = Which[
              StringQ[displaySettings]         (* if "displaySettings_" is a string, then it needs to be JSON and suitably composed *),
              CziGetMultiChannelScalingTileCompositeBitmap[ManagedLibraryExpressionID[c],roi,coordstr,zoom,displaySettings],
              ListQ[displaySettings],
              CziGetMultiChannelScalingTileCompositeBitmap[ManagedLibraryExpressionID[c],roi,coordstr,zoom,ExportString[displaySettings,"JSON"]],
              True,
                Message("Argument 'displaySettings' must either be a string containing JSON, or a List which can be converted to JSON");
                $Failed
              ];
      RetOrPrintError[img];
    ]

CZIReader`CZIGetMetadataXml[c_] :=
   Module[{},
      RetOrPrintError[CziGetMetadataXml[ManagedLibraryExpressionID[c]]];
    ]

CZIReader`CZIGetScaling[c_] :=
    Module[{scalingsXYZ,assoc},
      scalingsXYZ = CziGetScaling[ManagedLibraryExpressionID[c]];
      assoc = Append[<| |>,If[scalingsXYZ[[1]]>=0,"X"->scalingsXYZ[[1]],{}]];
      assoc = Append[assoc,If[scalingsXYZ[[2]]>=0,"Y"->scalingsXYZ[[2]],{}]];
      assoc = Append[assoc,If[scalingsXYZ[[3]]>=0,"Z"->scalingsXYZ[[3]],{}]];
      Return[assoc];
      ]

CZIReader`CZIGetSubBlockData[c_, no_, options_] :=
    Module[{sbblkHandle,assoc},
        sbblkHandle = CziHandleForSubBlock[ManagedLibraryExpressionID[c],no];
        assoc = <| |>;
        assoc = If[
                  TrueQ["XML" /. options],
                  Append[assoc,"XML"->CziGetMetadataXmlForSubBlockHandle[ManagedLibraryExpressionID[c],sbblkHandle]],
                  assoc];
        assoc = If[
                  TrueQ["Info" /. options],
                  Append[assoc,"Info"->CziGetInfoForSubBlockHandle[ManagedLibraryExpressionID[c],sbblkHandle]],
                  assoc];
        assoc = If[
                  TrueQ["Image" /. options],
                  Append[assoc,"Image"->CziGetBitmapForSubBlockHandle[ManagedLibraryExpressionID[c],sbblkHandle]],
                  assoc];
        CziReleaseSubBlockHandle[ManagedLibraryExpressionID[c],sbblkHandle];
        Return[assoc];
    ]

CZIReader`CZIGetSubBlockData[c_, no_] := CZIGetSubBlockData[c,no,{"XML"->True,"Info"->True,"Image"->True}];

  (* All functions which are not public, and are only used in the 
   internal implementation of the package, go into this section.
   These have non-capital names by convention. *)

coordArgumentToString[coord_] :=
    If[ 
        StringQ[coord],
        coord,
        If[
            ListQ[coord],
            StringJoin[Map[StringJoin[Part[#, 1], ToString[Part[#, 2]]] &, coord, 1]],
            ""
        ]
      ];    

  End[]

  EndPackage[]



