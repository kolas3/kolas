// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "vcaxisscale.h"


/////////////////////////////////////////////////////////////////////////////
// CVcAxisScale properties

/////////////////////////////////////////////////////////////////////////////
// CVcAxisScale operations

BOOL CVcAxisScale::GetHide()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (VOID*)&result, NULL);
	return result;
}

VOID CVcAxisScale::SetHide(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CVcAxisScale::GetType()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (VOID*)&result, NULL);
	return result;
}

VOID CVcAxisScale::SetType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CVcAxisScale::GetLogBase()
{
	short result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (VOID*)&result, NULL);
	return result;
}

VOID CVcAxisScale::SetLogBase(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CVcAxisScale::GetPercentBasis()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (VOID*)&result, NULL);
	return result;
}

VOID CVcAxisScale::SetPercentBasis(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}
