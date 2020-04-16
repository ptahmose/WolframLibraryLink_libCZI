(* ::Package:: *)

BeginPackage[ "CZIReader`"]

(* Export section *)

(* Every function that the package provides will have a usage message here. *)
(* There will be no function definitions in this section, only usage messages. *)
(* Public functions will have names starting with capitals, by convention. *)

GetCZIReaderLibraryInfo::usage = 
  "Get version/build information about 'CZIReader'."

 OpenCZI::usage = 
	"OpenCZI[ x] opens a CZI file.";

 CZIGetInfo::usage =
  "Get statistics.";
	
 CZIGetSubBlock::usage =
    "CZIGetSubBlock[ c , n]";

 CZISingleChannelScaledComposite::usage = 
    "CZISingleChannelScaledComposite[c,x,y,w,h,zoom,coord]";

 CZIMultiChannelScaledComposite::usage = 
    "CZIMultiChannelScaledComposite[c,x,y,w,h,zoom,coord]";
	
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
  
CziReaderInfo = libraryfunctionload[
  "CZIReader_GetInfo",
  {Integer}, UTF8String];

CziGetSubBlockBitmap = libraryfunctionload[
  "CZIReader_GetSubBlockBitmap",
  {Integer, Integer}, LibraryDataType[Image]];

CziGetSingleChannelScaledBitmap = libraryfunctionload[
  "CZIReader_GetSingleChannelScalingTileComposite",
  {Integer, LibraryDataType[MNumericArray], UTF8String, LibraryDataType[Real]}, 
  LibraryDataType[Image]];

CziGetMultiChannelScalingTileCompositeBitmap = libraryfunctionload[
  "CZIReader_MultiChannelScalingTileComposite",
  {Integer, LibraryDataType[MNumericArray], UTF8String, LibraryDataType[Real]}, 
  LibraryDataType[Image]];

GetCZIReaderLibraryInfo[] :=
  Module[{},
    Return[GetLibraryInfo[]];
  ]

OpenCZI[ x_] :=
    Module[ {exp},
      exp = CreateManagedLibraryExpression["CZIReader", reader1];
	  CziReaderOpen[
			ManagedLibraryExpressionID[exp], 
			x];
	  Return[exp];
    ]

CZIGetInfo[ c_ ] :=
    Module[{},
      Return[CziReaderInfo[ManagedLibraryExpressionID[c]]];
    ]
	
CZIGetSubBlock[c_,n_] :=
    Module[ {bitmap},
      bitmap = CziGetSubBlockBitmap[ManagedLibraryExpressionID[c],n];
      Return[bitmap];
    ]


CZISingleChannelScaledComposite[c_,x_,y_,w_,h_,zoom_,coord_]  :=
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

      img = CziGetSingleChannelScaledBitmap[
        ManagedLibraryExpressionID[c],
        roi,
        coordstr,
        zoom];
      Return[img];
    ]

CZIMultiChannelScaledComposite[c_,x_,y_,w_,h_,zoom_,coord_]  :=
    Module[{roi,img,coordstr},
      roi = NumericArray[{x,y,w,h},"Integer32"];
      coordstr = coordArgumentToString[coord];

      img = CziGetMultiChannelScalingTileCompositeBitmap[
        ManagedLibraryExpressionID[c],
        roi,
        coordstr,
        zoom];
      Return[img];
    ]

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



