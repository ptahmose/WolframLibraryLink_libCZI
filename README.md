# WolframLibraryLink_libCZI

read [CZI-documents](https://www.zeiss.com/microscopy/int/products/microscope-software/zen/czi.html) from [Mathematica](https://www.wolfram.com/mathematica) / [Wolfram Language](https://www.wolfram.com/language/)

# Description

This project provides a [Wolfram Library Link](https://reference.wolfram.com/language/LibraryLink/tutorial/Overview.html) implementation (based on [libCZI](https://github.com/ptahmose/libCZI)) allowing to directly open and read a CZI-document.
See the module in action -> [here](https://youtu.be/ApOlKVpkWD0[) and [here](https://youtu.be/lTkrwYfyjK0).

# Installing

A Paclet is provided which includes binaries for ARM (e.g. Raspberry Pi), Windows x86 & x64 and Linux x64 on the [release-page](https://github.com/ptahmose/WolframLibraryLink_libCZI/releases) here on GitHub.

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
|OpenCZI                         | open a CZI-file                                       |
|ReleaseCZI                      | destroy the CZI-document object                       |
|CZIGetInfo                      | get information about the content of the CZI-document |
|CZIGetSubBlock                  | get the bitmap of the specified subblock              |
|CZISingleChannelScaledComposite | get a (scaled) multi-tile composite                   |
|CZIMultiChannelScaledComposite  | get a (scaled) multi-tile multi-channel composite     |
|CZIGetMetadataXml               | get the XML-information                               |
|CZIGetScaling                   | get the scaling of the document                       |
|[CZIGetSubBlockData](https://github.com/ptahmose/WolframLibraryLink_libCZI/wiki/Usage#czigetsubblockdatafileobj-index-options)              | get content of a subblock                             |
