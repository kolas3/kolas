// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "vctextlayout.h"


/////////////////////////////////////////////////////////////////////////////
// CVcTextLayout properties

/////////////////////////////////////////////////////////////////////////////
// CVcTextLayout operations

long CVcTextLayout::GetHorzAlignment()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (VOID*)&result, NULL);
	return result;
}

VOID CVcTextLayout::SetHorzAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CVcTextLayout::GetVertAlignment()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (VOID*)&result, NULL);
	return result;
}

VOID CVcTextLayout::SetVertAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CVcTextLayout::GetOrientation()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (VOID*)&result, NULL);
	return result;
}

VOID CVcTextLayout::SetOrientation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CVcTextLayout::GetWordWrap()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (VOID*)&result, NULL);
	return result;
}

VOID CVcTextLayout::SetWordWrap(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}
