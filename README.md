# WolframLibraryLink_libCZI

read [CZI-documents](https://www.zeiss.com/microscopy/int/products/microscope-software/zen/czi.html) from [Mathematica](https://www.wolfram.com/mathematica) / [Wolfram Language](https://www.wolfram.com/language/)

# Description

This project provides a [Wolfram Library Link](https://reference.wolfram.com/language/LibraryLink/tutorial/Overview.html) implementation (based on [libCZI](https://github.com/ptahmose/libCZI)) allowing to directly open and read a CZI-document.

See the module in action -> [here](https://youtu.be/ApOlKVpkWD0) and [here](https://youtu.be/lTkrwYfyjK0).

# Installing

A Paclet is provided which includes binaries for ARM (e.g. Raspberry Pi), Windows x86 & x64 and Linux x64 on the [release-page](https://github.com/ptahmose/WolframLibraryLink_libCZI/releases) here on GitHub. Please check this page for the latest version.

![Paclet installation](./doc/content/pacletinstall.png "paclet_installation")

```
PacletInstall["https://github.com/ptahmose/WolframLibraryLink_libCZI/releases/download/v0.1.2/CZIReader-0.1.2.paclet"]

FindFile["CZIReader`"]    (* see where the paclet was downloaded to and installed *)
-> "/home/pi/.Mathematica/Paclets/Repository/CZIReader-0.1.2/Kernel/CZIReader.m"

PacletUninstall["CZIReader"] (* uninstall the CZIReader-paclet *)
```

# Usage

The following commands are provided:

|command                         |description                                            |
|:-------------------------------|:------------------------------------------------------|
|GetCZIReaderLibraryInfo         | gather information about the CZIReader-package        |
|[OpenCZI](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage#openczifilename)| open a CZI-file                                       |
|[ReleaseCZI](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage#releaseczifileobj)| destroy the CZI-document object                       |
|[CZIGetInfo](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage#czigetinfofileobj)| get information about the content of the CZI-document |
|[CZIGetSubBlock](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage#czigetsubblockfileobj-index)| get the bitmap of the specified subblock              |
|[CZISingleChannelScaledComposite](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage#czisinglechannelscaledcompositefileobjxywhzoomcoordbackgroundcolor)| get a (scaled) multi-tile composite                   |
|[CZIMultiChannelScaledComposite](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage#czimultichannelscaledcompositefileobjxywhzoomcoorddisplaysettings)| get a (scaled) multi-tile multi-channel composite     |
|CZIGetMetadataXml               | get the XML-information                               |
|[CZIGetScaling](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage#czigetscalingfileobj)| get the scaling of the document                       |
|[CZIGetSubBlockData](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage#czigetsubblockdatafileobj-index-options)              | get content of a subblock                             |

See the [wiki](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage) for more information.

# Building

The wllczi-shared-library can be built using cmake. [Here](https://asciinema.org/a/326514) is a recording of a build on a Raspberry Pi.

There is a build-pipeline configured [here](https://dev.azure.com/ptahmoseGithub/Github-Projects/_build?definitionId=1&_a=summary).
