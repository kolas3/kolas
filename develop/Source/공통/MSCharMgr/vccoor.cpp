// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "vccoor.h"


/////////////////////////////////////////////////////////////////////////////
// CVcCoor properties

/////////////////////////////////////////////////////////////////////////////
// CVcCoor operations

float CVcCoor::GetX()
{
	float result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_R4, (VOID*)&result, NULL);
	return result;
}

VOID CVcCoor::SetX(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float CVcCoor::GetY()
{
	float result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_R4, (VOID*)&result, NULL);
	return result;
}

VOID CVcCoor::SetY(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

VOID CVcCoor::Set(float X, float Y)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}
