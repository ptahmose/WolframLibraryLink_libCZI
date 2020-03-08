#include "WolframLibrary.h"
#include "wstp.h"

/* Adds one to the input, returning the result  */
EXTERN_C DLLEXPORT int demo_I_I( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res) 
{
	mint I0;
	mint I1;
	I0 = MArgument_getInteger(Args[0]);
	I1 = I0 + 1;
	MArgument_setInteger(Res, I1);
	return LIBRARY_NO_ERROR;
}

EXTERN_C DLLEXPORT int getLibraryInfo(WolframLibraryData libData, WSLINK mlp)
{
	WSNewPacket(mlp);
	//WSPutString(mlp, "HelloWorld");
	WSPutFunction(mlp, "Association", 1);
	WSPutFunction(mlp, "Rule", 2);
	WSPutString(mlp, "test");
	WSPutString(mlp, "quick brown fox");
	WSEndPacket(mlp);
	WSFlush(mlp);
	return LIBRARY_NO_ERROR;
}