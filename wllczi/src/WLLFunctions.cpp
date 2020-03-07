#include "WolframLibrary.h"

/* Adds one to the input, returning the result  */
DLLEXPORT int demo_I_I( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) {
	mint I0;
	mint I1;
	I0 = MArgument_getInteger(Args[0]);
	I1 = I0 + 1;
	MArgument_setInteger(Res, I1);
	return LIBRARY_NO_ERROR;
}