// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "hancelmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _Application properties

/////////////////////////////////////////////////////////////////////////////
// _Application operations

LPDISPATCH _ApplicationHanCell::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ApplicationHanCell::GetName()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetCells()
{
	LPDISPATCH result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetActiveCell()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetSelection()
{
	LPDISPATCH result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetActiveWorkbook()
{
	LPDISPATCH result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetActiveSheet()
{
	LPDISPATCH result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetRange(const VARIANT& Cell1, const VARIANT& Cell2)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Cell1, &Cell2);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetRows()
{
	LPDISPATCH result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetColumns()
{
	LPDISPATCH result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

double _ApplicationHanCell::GetLeft()
{
	double result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetLeft(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double _ApplicationHanCell::GetTop()
{
	double result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetTop(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double _ApplicationHanCell::GetWidth()
{
	double result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetWidth(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double _ApplicationHanCell::GetHeight()
{
	double result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetHeight(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _ApplicationHanCell::Quit()
{
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _ApplicationHanCell::GetWorkbooks()
{
	LPDISPATCH result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetWorksheets()
{
	LPDISPATCH result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ApplicationHanCell::GetValue()
{
	CString result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetNames()
{
	LPDISPATCH result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _ApplicationHanCell::GetVisible()
{
	BOOL result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _ApplicationHanCell::GetActiveWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _ApplicationHanCell::GetDisplayAlerts()
{
	BOOL result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetDisplayAlerts(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _ApplicationHanCell::GetCutCopyMode()
{
	long result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetCutCopyMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _ApplicationHanCell::GetActivePrinter()
{
	CString result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetActivePrinter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _ApplicationHanCell::GetBuild()
{
	long result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _ApplicationHanCell::GetDefaultFilePath()
{
	CString result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetDefaultFilePath(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _ApplicationHanCell::GetCellDragAndDrop()
{
	BOOL result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetCellDragAndDrop(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _ApplicationHanCell::GetCustomListCount()
{
	long result;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _ApplicationHanCell::GetIteration()
{
	BOOL result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetIteration(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

double _ApplicationHanCell::GetMaxChange()
{
	double result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetMaxChange(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _ApplicationHanCell::GetMaxIterations()
{
	long result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetMaxIterations(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _ApplicationHanCell::GetDisplayFullScreen()
{
	BOOL result;
	InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetDisplayFullScreen(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _ApplicationHanCell::GetDisplayNoteIndicator()
{
	BOOL result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetDisplayNoteIndicator(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _ApplicationHanCell::GetDisplayCommentIndicator()
{
	long result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetDisplayCommentIndicator(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _ApplicationHanCell::GetEnableAutoComplete()
{
	BOOL result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetEnableAutoComplete(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _ApplicationHanCell::GetMoveAfterReturn()
{
	BOOL result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetMoveAfterReturn(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _ApplicationHanCell::GetMoveAfterReturnDirection()
{
	long result;
	InvokeHelper(0x2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetMoveAfterReturnDirection(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _ApplicationHanCell::GetSheetsInNewWorkbook()
{
	long result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetSheetsInNewWorkbook(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _ApplicationHanCell::GetEnableEvents()
{
	BOOL result;
	InvokeHelper(0x2c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetEnableEvents(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _ApplicationHanCell::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0xc8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCopy()
{
	short result;
	InvokeHelper(0x9c40, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCut()
{
	short result;
	InvokeHelper(0x9c41, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAnd()
{
	short result;
	InvokeHelper(0x9c42, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlBottom10Items()
{
	short result;
	InvokeHelper(0x9c43, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlBottom10Percent()
{
	short result;
	InvokeHelper(0x9c44, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlOr()
{
	short result;
	InvokeHelper(0x9c45, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTop10Items()
{
	short result;
	InvokeHelper(0x9c46, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTop10Percent()
{
	short result;
	InvokeHelper(0x9c47, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPinYin()
{
	short result;
	InvokeHelper(0x9c48, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlStroke()
{
	short result;
	InvokeHelper(0x9c49, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAscending()
{
	short result;
	InvokeHelper(0x9c4a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDescending()
{
	short result;
	InvokeHelper(0x9c4b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGuess()
{
	short result;
	InvokeHelper(0x9c4c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNo()
{
	short result;
	InvokeHelper(0x9c4d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlYes()
{
	short result;
	InvokeHelper(0x9c4e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSortRows()
{
	short result;
	InvokeHelper(0x9c4f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSortColumns()
{
	short result;
	InvokeHelper(0x9c50, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSortNormal()
{
	short result;
	InvokeHelper(0x9c51, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSortTextAsNumbers()
{
	short result;
	InvokeHelper(0x9c52, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlColorIndexAutomatic()
{
	short result;
	InvokeHelper(0x9c53, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlColorIndexNone()
{
	short result;
	InvokeHelper(0x9c54, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlA1()
{
	short result;
	InvokeHelper(0x9c55, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlR1C1()
{
	short result;
	InvokeHelper(0x9c56, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDown()
{
	short result;
	InvokeHelper(0x9c57, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlToLeft()
{
	short result;
	InvokeHelper(0x9c58, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlToRight()
{
	short result;
	InvokeHelper(0x9c59, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlUp()
{
	short result;
	InvokeHelper(0x9c5a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeBlanks()
{
	short result;
	InvokeHelper(0x9c5b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeConstants()
{
	short result;
	InvokeHelper(0x9c5c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeFormulas()
{
	short result;
	InvokeHelper(0x9c5d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeLastCell()
{
	short result;
	InvokeHelper(0x9c5e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeComments()
{
	short result;
	InvokeHelper(0x9c5f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeVisible()
{
	short result;
	InvokeHelper(0x9c60, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeAllFormatConditions()
{
	short result;
	InvokeHelper(0x9c61, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeSameFormatConditions()
{
	short result;
	InvokeHelper(0x9c62, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeAllValidation()
{
	short result;
	InvokeHelper(0x9c63, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCellTypeSameValidation()
{
	short result;
	InvokeHelper(0x9c64, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlExclusive()
{
	short result;
	InvokeHelper(0x9c65, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNoChange()
{
	short result;
	InvokeHelper(0x9c66, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShared()
{
	short result;
	InvokeHelper(0x9c67, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlContinuous()
{
	short result;
	InvokeHelper(0x9c68, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDash()
{
	short result;
	InvokeHelper(0x9c69, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDashDot()
{
	short result;
	InvokeHelper(0x9c6a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDashDotDot()
{
	short result;
	InvokeHelper(0x9c6b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDot()
{
	short result;
	InvokeHelper(0x9c6c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDouble()
{
	short result;
	InvokeHelper(0x9c6d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSlantDashDot()
{
	short result;
	InvokeHelper(0x9c6e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLineStyleNone()
{
	short result;
	InvokeHelper(0x9c6f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHairline()
{
	short result;
	InvokeHelper(0x9c70, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMedium()
{
	short result;
	InvokeHelper(0x9c71, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlThick()
{
	short result;
	InvokeHelper(0x9c72, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlThin()
{
	short result;
	InvokeHelper(0x9c73, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlInsideHorizontal()
{
	short result;
	InvokeHelper(0x9c74, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlInsideVertical()
{
	short result;
	InvokeHelper(0x9c75, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDiagonalDown()
{
	short result;
	InvokeHelper(0x9c76, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDiagonalUp()
{
	short result;
	InvokeHelper(0x9c77, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlEdgeBottom()
{
	short result;
	InvokeHelper(0x9c78, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlEdgeLeft()
{
	short result;
	InvokeHelper(0x9c79, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlEdgeRight()
{
	short result;
	InvokeHelper(0x9c7a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlEdgeTop()
{
	short result;
	InvokeHelper(0x9c7b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlUnderlineStyleDouble()
{
	short result;
	InvokeHelper(0x9c7c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlUnderlineStyleDoubleAccounting()
{
	short result;
	InvokeHelper(0x9c7d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlUnderlineStyleNone()
{
	short result;
	InvokeHelper(0x9c7e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlUnderlineStyleSingle()
{
	short result;
	InvokeHelper(0x9c7f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlUnderlineStyleSingleAccounting()
{
	short result;
	InvokeHelper(0x9c80, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternAutomatic()
{
	short result;
	InvokeHelper(0x9c81, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternChecker()
{
	short result;
	InvokeHelper(0x9c82, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternCrissCross()
{
	short result;
	InvokeHelper(0x9c83, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternDown()
{
	short result;
	InvokeHelper(0x9c84, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternGray16()
{
	short result;
	InvokeHelper(0x9c85, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternGray25()
{
	short result;
	InvokeHelper(0x9c86, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternGray50()
{
	short result;
	InvokeHelper(0x9c87, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternGray75()
{
	short result;
	InvokeHelper(0x9c88, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternGray8()
{
	short result;
	InvokeHelper(0x9c89, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternGrid()
{
	short result;
	InvokeHelper(0x9c8a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternHorizontal()
{
	short result;
	InvokeHelper(0x9c8b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternLightDown()
{
	short result;
	InvokeHelper(0x9c8c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternLightHorizontal()
{
	short result;
	InvokeHelper(0x9c8d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternLightUp()
{
	short result;
	InvokeHelper(0x9c8e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternLightVertical()
{
	short result;
	InvokeHelper(0x9c8f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternNone()
{
	short result;
	InvokeHelper(0x9c90, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternSemiGray75()
{
	short result;
	InvokeHelper(0x9c91, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternSolid()
{
	short result;
	InvokeHelper(0x9c92, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternUp()
{
	short result;
	InvokeHelper(0x9c93, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPatternVertical()
{
	short result;
	InvokeHelper(0x9c94, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShiftToLeft()
{
	short result;
	InvokeHelper(0x9c95, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShiftUp()
{
	short result;
	InvokeHelper(0x9c96, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShiftDown()
{
	short result;
	InvokeHelper(0x9c97, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShiftToRight()
{
	short result;
	InvokeHelper(0x9c98, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFillCopy()
{
	short result;
	InvokeHelper(0x9c99, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFillDays()
{
	short result;
	InvokeHelper(0x9c9a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFillDefault()
{
	short result;
	InvokeHelper(0x9c9b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFillFormats()
{
	short result;
	InvokeHelper(0x9c9c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFillMonths()
{
	short result;
	InvokeHelper(0x9c9d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFillSeries()
{
	short result;
	InvokeHelper(0x9c9e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFillValues()
{
	short result;
	InvokeHelper(0x9c9f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFillWeekdays()
{
	short result;
	InvokeHelper(0x9ca0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFillYears()
{
	short result;
	InvokeHelper(0x9ca1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGrowthTrend()
{
	short result;
	InvokeHelper(0x9ca2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLinearTrend()
{
	short result;
	InvokeHelper(0x9ca3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAll()
{
	short result;
	InvokeHelper(0x9ca4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAutomatic()
{
	short result;
	InvokeHelper(0x9ca5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlBoth()
{
	short result;
	InvokeHelper(0x9ca6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCenter()
{
	short result;
	InvokeHelper(0x9ca7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlChecker()
{
	short result;
	InvokeHelper(0x9ca8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCircle()
{
	short result;
	InvokeHelper(0x9ca9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCorner()
{
	short result;
	InvokeHelper(0x9caa, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCrissCross()
{
	short result;
	InvokeHelper(0x9cab, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCross()
{
	short result;
	InvokeHelper(0x9cac, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDiamond()
{
	short result;
	InvokeHelper(0x9cad, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDistributed()
{
	short result;
	InvokeHelper(0x9cae, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDoubleAccounting()
{
	short result;
	InvokeHelper(0x9caf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFixedValue()
{
	short result;
	InvokeHelper(0x9cb0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFormats()
{
	short result;
	InvokeHelper(0x9cb1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGray16()
{
	short result;
	InvokeHelper(0x9cb2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGray8()
{
	short result;
	InvokeHelper(0x9cb3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGrid()
{
	short result;
	InvokeHelper(0x9cb4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHigh()
{
	short result;
	InvokeHelper(0x9cb5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlInside()
{
	short result;
	InvokeHelper(0x9cb6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlJustify()
{
	short result;
	InvokeHelper(0x9cb7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLightDown()
{
	short result;
	InvokeHelper(0x9cb8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLightHorizontal()
{
	short result;
	InvokeHelper(0x9cb9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLightUp()
{
	short result;
	InvokeHelper(0x9cba, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLightVertical()
{
	short result;
	InvokeHelper(0x9cbb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLow()
{
	short result;
	InvokeHelper(0x9cbc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlManual()
{
	short result;
	InvokeHelper(0x9cbd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMinusValues()
{
	short result;
	InvokeHelper(0x9cbe, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlModule()
{
	short result;
	InvokeHelper(0x9cbf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNextToAxis()
{
	short result;
	InvokeHelper(0x9cc0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNone()
{
	short result;
	InvokeHelper(0x9cc1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNotes()
{
	short result;
	InvokeHelper(0x9cc2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlOff()
{
	short result;
	InvokeHelper(0x9cc3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlOn()
{
	short result;
	InvokeHelper(0x9cc4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPercent()
{
	short result;
	InvokeHelper(0x9cc5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPlus()
{
	short result;
	InvokeHelper(0x9cc6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPlusValues()
{
	short result;
	InvokeHelper(0x9cc7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSemiGray75()
{
	short result;
	InvokeHelper(0x9cc8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShowLabel()
{
	short result;
	InvokeHelper(0x9cc9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShowLabelAndPercent()
{
	short result;
	InvokeHelper(0x9cca, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShowPercent()
{
	short result;
	InvokeHelper(0x9ccb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShowValue()
{
	short result;
	InvokeHelper(0x9ccc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSimple()
{
	short result;
	InvokeHelper(0x9ccd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSingle()
{
	short result;
	InvokeHelper(0x9cce, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSingleAccounting()
{
	short result;
	InvokeHelper(0x9ccf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSolid()
{
	short result;
	InvokeHelper(0x9cd0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSquare()
{
	short result;
	InvokeHelper(0x9cd1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlStar()
{
	short result;
	InvokeHelper(0x9cd2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlStError()
{
	short result;
	InvokeHelper(0x9cd3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlToolbarButton()
{
	short result;
	InvokeHelper(0x9cd4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTriangle()
{
	short result;
	InvokeHelper(0x9cd5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGray25()
{
	short result;
	InvokeHelper(0x9cd6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGray50()
{
	short result;
	InvokeHelper(0x9cd7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGray75()
{
	short result;
	InvokeHelper(0x9cd8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlBottom()
{
	short result;
	InvokeHelper(0x9cd9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLeft()
{
	short result;
	InvokeHelper(0x9cda, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlRight()
{
	short result;
	InvokeHelper(0x9cdb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTop()
{
	short result;
	InvokeHelper(0x9cdc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXl3DBar()
{
	short result;
	InvokeHelper(0x9cdd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXl3DSurface()
{
	short result;
	InvokeHelper(0x9cde, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlBar()
{
	short result;
	InvokeHelper(0x9cdf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlColumn()
{
	short result;
	InvokeHelper(0x9ce0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCombination()
{
	short result;
	InvokeHelper(0x9ce1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCustom()
{
	short result;
	InvokeHelper(0x9ce2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDefaultAutoFormat()
{
	short result;
	InvokeHelper(0x9ce3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMaximum()
{
	short result;
	InvokeHelper(0x9ce4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMinimum()
{
	short result;
	InvokeHelper(0x9ce5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlOpaque()
{
	short result;
	InvokeHelper(0x9ce6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTransparent()
{
	short result;
	InvokeHelper(0x9ce7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlBidi()
{
	short result;
	InvokeHelper(0x9ce8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLatin()
{
	short result;
	InvokeHelper(0x9ce9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlContext()
{
	short result;
	InvokeHelper(0x9cea, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLTR()
{
	short result;
	InvokeHelper(0x9ceb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlRTL()
{
	short result;
	InvokeHelper(0x9cec, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFullScript()
{
	short result;
	InvokeHelper(0x9ced, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPartialScript()
{
	short result;
	InvokeHelper(0x9cee, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMixedScript()
{
	short result;
	InvokeHelper(0x9cef, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMixedAuthorizedScript()
{
	short result;
	InvokeHelper(0x9cf0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVisualCursor()
{
	short result;
	InvokeHelper(0x9cf1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLogicalCursor()
{
	short result;
	InvokeHelper(0x9cf2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSystem()
{
	short result;
	InvokeHelper(0x9cf3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPartial()
{
	short result;
	InvokeHelper(0x9cf4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHindiNumerals()
{
	short result;
	InvokeHelper(0x9cf5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlBidiCalendar()
{
	short result;
	InvokeHelper(0x9cf6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGregorian()
{
	short result;
	InvokeHelper(0x9cf7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlComplete()
{
	short result;
	InvokeHelper(0x9cf8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlScale()
{
	short result;
	InvokeHelper(0x9cf9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlClosed()
{
	short result;
	InvokeHelper(0x9cfa, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlColor1()
{
	short result;
	InvokeHelper(0x9cfb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlColor2()
{
	short result;
	InvokeHelper(0x9cfc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlColor3()
{
	short result;
	InvokeHelper(0x9cfd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlConstants()
{
	short result;
	InvokeHelper(0x9cfe, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlContents()
{
	short result;
	InvokeHelper(0x9cff, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlBelow()
{
	short result;
	InvokeHelper(0x9d00, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCascade()
{
	short result;
	InvokeHelper(0x9d01, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCenterAcrossSelection()
{
	short result;
	InvokeHelper(0x9d02, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlChart4()
{
	short result;
	InvokeHelper(0x9d03, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlChartSeries()
{
	short result;
	InvokeHelper(0x9d04, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlChartShort()
{
	short result;
	InvokeHelper(0x9d05, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlChartTitles()
{
	short result;
	InvokeHelper(0x9d06, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlClassic1()
{
	short result;
	InvokeHelper(0x9d07, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlClassic2()
{
	short result;
	InvokeHelper(0x9d08, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlClassic3()
{
	short result;
	InvokeHelper(0x9d09, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXl3DEffects1()
{
	short result;
	InvokeHelper(0x9d0a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXl3DEffects2()
{
	short result;
	InvokeHelper(0x9d0b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAbove()
{
	short result;
	InvokeHelper(0x9d0c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAccounting1()
{
	short result;
	InvokeHelper(0x9d0d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAccounting2()
{
	short result;
	InvokeHelper(0x9d0e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAccounting3()
{
	short result;
	InvokeHelper(0x9d0f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAccounting4()
{
	short result;
	InvokeHelper(0x9d10, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAdd()
{
	short result;
	InvokeHelper(0x9d11, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDebugCodePane()
{
	short result;
	InvokeHelper(0x9d12, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDesktop()
{
	short result;
	InvokeHelper(0x9d13, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDirect()
{
	short result;
	InvokeHelper(0x9d14, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDivide()
{
	short result;
	InvokeHelper(0x9d15, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDoubleClosed()
{
	short result;
	InvokeHelper(0x9d16, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDoubleOpen()
{
	short result;
	InvokeHelper(0x9d17, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDoubleQuote()
{
	short result;
	InvokeHelper(0x9d18, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlEntireChart()
{
	short result;
	InvokeHelper(0x9d19, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlExcelMenus()
{
	short result;
	InvokeHelper(0x9d1a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlExtended()
{
	short result;
	InvokeHelper(0x9d1b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFill()
{
	short result;
	InvokeHelper(0x9d1c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFirst()
{
	short result;
	InvokeHelper(0x9d1d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFloating()
{
	short result;
	InvokeHelper(0x9d1e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFormula()
{
	short result;
	InvokeHelper(0x9d1f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGeneral()
{
	short result;
	InvokeHelper(0x9d20, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGridline()
{
	short result;
	InvokeHelper(0x9d21, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlIcons()
{
	short result;
	InvokeHelper(0x9d22, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlImmediatePane()
{
	short result;
	InvokeHelper(0x9d23, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlInteger()
{
	short result;
	InvokeHelper(0x9d24, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLast()
{
	short result;
	InvokeHelper(0x9d25, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLastCell()
{
	short result;
	InvokeHelper(0x9d26, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlList1()
{
	short result;
	InvokeHelper(0x9d27, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlList2()
{
	short result;
	InvokeHelper(0x9d28, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlList3()
{
	short result;
	InvokeHelper(0x9d29, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLocalFormat1()
{
	short result;
	InvokeHelper(0x9d2a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLocalFormat2()
{
	short result;
	InvokeHelper(0x9d2b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLong()
{
	short result;
	InvokeHelper(0x9d2c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLotusHelp()
{
	short result;
	InvokeHelper(0x9d2d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMacrosheetCell()
{
	short result;
	InvokeHelper(0x9d2e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMixed()
{
	short result;
	InvokeHelper(0x9d2f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMultiply()
{
	short result;
	InvokeHelper(0x9d30, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNarrow()
{
	short result;
	InvokeHelper(0x9d31, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNoDocuments()
{
	short result;
	InvokeHelper(0x9d32, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlOpen()
{
	short result;
	InvokeHelper(0x9d33, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlOutside()
{
	short result;
	InvokeHelper(0x9d34, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlReference()
{
	short result;
	InvokeHelper(0x9d35, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSemiautomatic()
{
	short result;
	InvokeHelper(0x9d36, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlShort()
{
	short result;
	InvokeHelper(0x9d37, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSingleQuote()
{
	short result;
	InvokeHelper(0x9d38, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlStrict()
{
	short result;
	InvokeHelper(0x9d39, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSubtract()
{
	short result;
	InvokeHelper(0x9d3a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTextBox()
{
	short result;
	InvokeHelper(0x9d3b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTiled()
{
	short result;
	InvokeHelper(0x9d3c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTitleBar()
{
	short result;
	InvokeHelper(0x9d3d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlToolbar()
{
	short result;
	InvokeHelper(0x9d3e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVisible()
{
	short result;
	InvokeHelper(0x9d3f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWatchPane()
{
	short result;
	InvokeHelper(0x9d40, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWide()
{
	short result;
	InvokeHelper(0x9d41, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWorkbookTab()
{
	short result;
	InvokeHelper(0x9d42, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWorksheet4()
{
	short result;
	InvokeHelper(0x9d43, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWorksheetCell()
{
	short result;
	InvokeHelper(0x9d44, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWorksheetShort()
{
	short result;
	InvokeHelper(0x9d45, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAllExceptBorders()
{
	short result;
	InvokeHelper(0x9d46, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLeftToRight()
{
	short result;
	InvokeHelper(0x9d47, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTopToBottom()
{
	short result;
	InvokeHelper(0x9d48, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVeryHidden()
{
	short result;
	InvokeHelper(0x9d49, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDrawingObject()
{
	short result;
	InvokeHelper(0x9d4a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNexcel1()
{
	short result;
	InvokeHelper(0x9d4b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCSV()
{
	short result;
	InvokeHelper(0x9d4c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlExcel9795()
{
	short result;
	InvokeHelper(0x9d4d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHtml()
{
	short result;
	InvokeHelper(0x9d4e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSheetVisible()
{
	short result;
	InvokeHelper(0x9d4f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSheetHidden()
{
	short result;
	InvokeHelper(0x9d50, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSheetVeryHidden()
{
	short result;
	InvokeHelper(0x9d51, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAutoActivate()
{
	short result;
	InvokeHelper(0x9d52, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAutoClose()
{
	short result;
	InvokeHelper(0x9d53, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAutoDeactivate()
{
	short result;
	InvokeHelper(0x9d54, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAutoOpen()
{
	short result;
	InvokeHelper(0x9d55, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVAlignBottom()
{
	short result;
	InvokeHelper(0x9d56, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVAlignCenter()
{
	short result;
	InvokeHelper(0x9d57, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVAlignDistributed()
{
	short result;
	InvokeHelper(0x9d58, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVAlignJustify()
{
	short result;
	InvokeHelper(0x9d59, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVAlignTop()
{
	short result;
	InvokeHelper(0x9d5a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHAlignCenter()
{
	short result;
	InvokeHelper(0x9d5b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHAlignCenterAcrossSelection()
{
	short result;
	InvokeHelper(0x9d5c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHAlignDistributed()
{
	short result;
	InvokeHelper(0x9d5d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHAlignFill()
{
	short result;
	InvokeHelper(0x9d5e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHAlignGeneral()
{
	short result;
	InvokeHelper(0x9d5f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHAlignJustify()
{
	short result;
	InvokeHelper(0x9d60, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHAlignLeft()
{
	short result;
	InvokeHelper(0x9d61, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlHAlignRight()
{
	short result;
	InvokeHelper(0x9d62, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteAll()
{
	short result;
	InvokeHelper(0x9d63, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteAllExceptBorders()
{
	short result;
	InvokeHelper(0x9d64, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteFormats()
{
	short result;
	InvokeHelper(0x9d65, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteFormulas()
{
	short result;
	InvokeHelper(0x9d66, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteComments()
{
	short result;
	InvokeHelper(0x9d67, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteValues()
{
	short result;
	InvokeHelper(0x9d68, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteColumnWidths()
{
	short result;
	InvokeHelper(0x9d69, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteValidation()
{
	short result;
	InvokeHelper(0x9d6a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteFormulasAndNumberFormats()
{
	short result;
	InvokeHelper(0x9d6b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteValuesAndNumberFormats()
{
	short result;
	InvokeHelper(0x9d6c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlColumns()
{
	short result;
	InvokeHelper(0x9d6d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlRows()
{
	short result;
	InvokeHelper(0x9d6e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAutoFill()
{
	short result;
	InvokeHelper(0x9d6f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlChronological()
{
	short result;
	InvokeHelper(0x9d70, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlGrowth()
{
	short result;
	InvokeHelper(0x9d71, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDataSeriesLinear()
{
	short result;
	InvokeHelper(0x9d72, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDay()
{
	short result;
	InvokeHelper(0x9d73, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMonth()
{
	short result;
	InvokeHelper(0x9d74, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWeekday()
{
	short result;
	InvokeHelper(0x9d75, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlYear()
{
	short result;
	InvokeHelper(0x9d76, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNormal()
{
	short result;
	InvokeHelper(0x9d77, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteSpecialOperationAdd()
{
	short result;
	InvokeHelper(0x9d78, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteSpecialOperationDivide()
{
	short result;
	InvokeHelper(0x9d79, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteSpecialOperationMultiply()
{
	short result;
	InvokeHelper(0x9d7a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteSpecialOperationNone()
{
	short result;
	InvokeHelper(0x9d7b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPasteSpecialOperationSubtract()
{
	short result;
	InvokeHelper(0x9d7c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPortrait()
{
	short result;
	InvokeHelper(0x9d7d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLandscape()
{
	short result;
	InvokeHelper(0x9d7e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaper11x17()
{
	short result;
	InvokeHelper(0x9d7f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperA4()
{
	short result;
	InvokeHelper(0x9d80, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperA5()
{
	short result;
	InvokeHelper(0x9d81, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperB5()
{
	short result;
	InvokeHelper(0x9d82, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperDsheet()
{
	short result;
	InvokeHelper(0x9d83, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelope11()
{
	short result;
	InvokeHelper(0x9d84, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelope14()
{
	short result;
	InvokeHelper(0x9d85, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeB4()
{
	short result;
	InvokeHelper(0x9d86, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeB6()
{
	short result;
	InvokeHelper(0x9d87, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeC4()
{
	short result;
	InvokeHelper(0x9d88, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeC6()
{
	short result;
	InvokeHelper(0x9d89, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeDL()
{
	short result;
	InvokeHelper(0x9d8a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeMonarch()
{
	short result;
	InvokeHelper(0x9d8b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEsheet()
{
	short result;
	InvokeHelper(0x9d8c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperFanfoldLegalGerman()
{
	short result;
	InvokeHelper(0x9d8d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperFanfoldUS()
{
	short result;
	InvokeHelper(0x9d8e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperLedger()
{
	short result;
	InvokeHelper(0x9d8f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperLetter()
{
	short result;
	InvokeHelper(0x9d90, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperNote()
{
	short result;
	InvokeHelper(0x9d91, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperStatement()
{
	short result;
	InvokeHelper(0x9d92, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperUser()
{
	short result;
	InvokeHelper(0x9d93, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaper10x14()
{
	short result;
	InvokeHelper(0x9d94, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperA3()
{
	short result;
	InvokeHelper(0x9d95, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperA4Small()
{
	short result;
	InvokeHelper(0x9d96, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperB4()
{
	short result;
	InvokeHelper(0x9d97, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperCsheet()
{
	short result;
	InvokeHelper(0x9d98, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelope10()
{
	short result;
	InvokeHelper(0x9d99, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelope12()
{
	short result;
	InvokeHelper(0x9d9a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelope9()
{
	short result;
	InvokeHelper(0x9d9b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeB5()
{
	short result;
	InvokeHelper(0x9d9c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeC3()
{
	short result;
	InvokeHelper(0x9d9d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeC5()
{
	short result;
	InvokeHelper(0x9d9e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeC65()
{
	short result;
	InvokeHelper(0x9d9f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopeItaly()
{
	short result;
	InvokeHelper(0x9da0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperEnvelopePersonal()
{
	short result;
	InvokeHelper(0x9da1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperExecutive()
{
	short result;
	InvokeHelper(0x9da2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperFanfoldStdGerman()
{
	short result;
	InvokeHelper(0x9da3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperFolio()
{
	short result;
	InvokeHelper(0x9da4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperLegal()
{
	short result;
	InvokeHelper(0x9da5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperLetterSmall()
{
	short result;
	InvokeHelper(0x9da6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperQuarto()
{
	short result;
	InvokeHelper(0x9da7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPaperTabloid()
{
	short result;
	InvokeHelper(0x9da8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlText()
{
	short result;
	InvokeHelper(0x9da9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPrintInPlace()
{
	short result;
	InvokeHelper(0x9daa, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPrintNoComments()
{
	short result;
	InvokeHelper(0x9dab, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPrintSheetEnd()
{
	short result;
	InvokeHelper(0x9dac, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDownThenOver()
{
	short result;
	InvokeHelper(0x9dad, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlOverThenDown()
{
	short result;
	InvokeHelper(0x9dae, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNormalView()
{
	short result;
	InvokeHelper(0x9daf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPageBreakPreview()
{
	short result;
	InvokeHelper(0x9db0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMaximized()
{
	short result;
	InvokeHelper(0x9db1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMinimized()
{
	short result;
	InvokeHelper(0x9db2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWorksheet()
{
	short result;
	InvokeHelper(0x9db3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlChart()
{
	short result;
	InvokeHelper(0x9db4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlExcel4MacroSheet()
{
	short result;
	InvokeHelper(0x9db5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlExcel4IntlMacroSheet()
{
	short result;
	InvokeHelper(0x9db6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDelimited()
{
	short result;
	InvokeHelper(0x9db7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPart()
{
	short result;
	InvokeHelper(0x9db8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWhole()
{
	short result;
	InvokeHelper(0x9db9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlByColumns()
{
	short result;
	InvokeHelper(0x9dba, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlByRows()
{
	short result;
	InvokeHelper(0x9dbb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlExponential()
{
	short result;
	InvokeHelper(0x9dbc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLinear()
{
	short result;
	InvokeHelper(0x9dbd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLogarithmic()
{
	short result;
	InvokeHelper(0x9dbe, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMovingAvg()
{
	short result;
	InvokeHelper(0x9dbf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPolynomial()
{
	short result;
	InvokeHelper(0x9dc0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPower()
{
	short result;
	InvokeHelper(0x9dc1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlColumnThenRow()
{
	short result;
	InvokeHelper(0x9dc2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlRowThenColumn()
{
	short result;
	InvokeHelper(0x9dc3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNext()
{
	short result;
	InvokeHelper(0x9dc4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPrevious()
{
	short result;
	InvokeHelper(0x9dc5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAverage()
{
	short result;
	InvokeHelper(0x9dc6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCount()
{
	short result;
	InvokeHelper(0x9dc7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCountNums()
{
	short result;
	InvokeHelper(0x9dc8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMax()
{
	short result;
	InvokeHelper(0x9dc9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMin()
{
	short result;
	InvokeHelper(0x9dca, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlProduct()
{
	short result;
	InvokeHelper(0x9dcb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlStDev()
{
	short result;
	InvokeHelper(0x9dcc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlStDevP()
{
	short result;
	InvokeHelper(0x9dcd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSum()
{
	short result;
	InvokeHelper(0x9dce, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVar()
{
	short result;
	InvokeHelper(0x9dcf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlVarP()
{
	short result;
	InvokeHelper(0x9dd0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlUnknown()
{
	short result;
	InvokeHelper(0x9dd1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFormulas()
{
	short result;
	InvokeHelper(0x9dd2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlComments()
{
	short result;
	InvokeHelper(0x9dd3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlValues()
{
	short result;
	InvokeHelper(0x9dd4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlErrors()
{
	short result;
	InvokeHelper(0x9dd5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlLogical()
{
	short result;
	InvokeHelper(0x9dd6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNumbers()
{
	short result;
	InvokeHelper(0x9dd7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTextValues()
{
	short result;
	InvokeHelper(0x9dd8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFixedWidth()
{
	short result;
	InvokeHelper(0x9dd9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTextQualifierDoubleQuote()
{
	short result;
	InvokeHelper(0x9dda, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTextQualifierNone()
{
	short result;
	InvokeHelper(0x9ddb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlTextQualifierSingleQuote()
{
	short result;
	InvokeHelper(0x9ddc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlIBeam()
{
	short result;
	InvokeHelper(0x9ddd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDefault()
{
	short result;
	InvokeHelper(0x9dde, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNorthwestArrow()
{
	short result;
	InvokeHelper(0x9ddf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlWait()
{
	short result;
	InvokeHelper(0x9de0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNoRestrictions()
{
	short result;
	InvokeHelper(0x9de1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlUnlockedCells()
{
	short result;
	InvokeHelper(0x9de2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNoSelection()
{
	short result;
	InvokeHelper(0x9de3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCalculationAutomatic()
{
	short result;
	InvokeHelper(0x9de4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCalculationManual()
{
	short result;
	InvokeHelper(0x9de5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCalculationSemiautomatic()
{
	short result;
	InvokeHelper(0x9de6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlNoIndicator()
{
	short result;
	InvokeHelper(0x9de7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCommentIndicatorOnly()
{
	short result;
	InvokeHelper(0x9de8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlCommentAndIndicator()
{
	short result;
	InvokeHelper(0x9de9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAbsolute()
{
	short result;
	InvokeHelper(0x9dea, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlAbsRowRelColumn()
{
	short result;
	InvokeHelper(0x9deb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlRelative()
{
	short result;
	InvokeHelper(0x9dec, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlRelRowAbsColumn()
{
	short result;
	InvokeHelper(0x9ded, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFilterCopy()
{
	short result;
	InvokeHelper(0x9dee, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlFilterInPlace()
{
	short result;
	InvokeHelper(0x9def, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPageBreakAutomatic()
{
	short result;
	InvokeHelper(0x9df0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPageBreakManual()
{
	short result;
	InvokeHelper(0x9df1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlPageBreakNone()
{
	short result;
	InvokeHelper(0x9df2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSrcExternal()
{
	short result;
	InvokeHelper(0x9df3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSrcRange()
{
	short result;
	InvokeHelper(0x9df4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSrcXml()
{
	short result;
	InvokeHelper(0x9df5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlSrcQuery()
{
	short result;
	InvokeHelper(0x9df6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlOpenXMLWorkbook()
{
	short result;
	InvokeHelper(0x9df7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoShapeRectangle()
{
	short result;
	InvokeHelper(0x9df8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoShapeRoundedRectangle()
{
	short result;
	InvokeHelper(0x9df9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoShapeOval()
{
	short result;
	InvokeHelper(0x9dfa, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoShapeCan()
{
	short result;
	InvokeHelper(0x9dfb, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoShapeCube()
{
	short result;
	InvokeHelper(0x9dfc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoShapeFlowchartDecision()
{
	short result;
	InvokeHelper(0x9dfd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoConnectorStraight()
{
	short result;
	InvokeHelper(0x9dfe, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoConnectorCurve()
{
	short result;
	InvokeHelper(0x9dff, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoEditingAuto()
{
	short result;
	InvokeHelper(0x9e00, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoEditingCorner()
{
	short result;
	InvokeHelper(0x9e01, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoEditingSmooth()
{
	short result;
	InvokeHelper(0x9e02, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoEditingSymmetric()
{
	short result;
	InvokeHelper(0x9e03, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoSegmentLine()
{
	short result;
	InvokeHelper(0x9e04, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetMsoSegmentCurve()
{
	short result;
	InvokeHelper(0x9e05, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

CString _ApplicationHanCell::Get_Default()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::CheckXObject(LPCTSTR bstring)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x31, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		bstring);
	return result;
}

void _ApplicationHanCell::Run(LPCTSTR actid)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 actid);
}

void _ApplicationHanCell::LockCommand(LPCTSTR actid, BOOL isLock)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 actid, isLock);
}

BOOL _ApplicationHanCell::IsCommandLock(LPCTSTR actionID)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x34, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		actionID);
	return result;
}

BOOL _ApplicationHanCell::RegisterModule(LPCTSTR ModuleType, const VARIANT& ModuleData)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x35, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		ModuleType, &ModuleData);
	return result;
}

double _ApplicationHanCell::InchesToPoints(double Inches)
{
	double result;
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x36, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		Inches);
	return result;
}

BOOL _ApplicationHanCell::ChDir(const VARIANT& cvarPathName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x37, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&cvarPathName);
	return result;
}

BOOL _ApplicationHanCell::Kill(const VARIANT& varDeleteFilePathName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x38, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&varDeleteFilePathName);
	return result;
}

void _ApplicationHanCell::Echo(BOOL varbEcho)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 varbEcho);
}

double _ApplicationHanCell::CentimetersToPoints(double Centimeters)
{
	double result;
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x3a, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		Centimeters);
	return result;
}

VARIANT _ApplicationHanCell::Evaluate(const VARIANT& Name)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Name);
	return result;
}

void _ApplicationHanCell::AddCustomList(const VARIANT& ListArray, const VARIANT& ByRow)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x3c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &ListArray, &ByRow);
}

void _ApplicationHanCell::DeleteCustomList(long ListNum)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ListNum);
}

VARIANT _ApplicationHanCell::GetCustomListContents(long ListNum)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		ListNum);
	return result;
}

long _ApplicationHanCell::GetCustomListNum(const VARIANT& ListArray)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&ListArray);
	return result;
}

LPDISPATCH _ApplicationHanCell::Intersect(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT 
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x40, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Arg1, Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
	return result;
}

LPDISPATCH _ApplicationHanCell::Union(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11, 
		const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21, 
		const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT 
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x41, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Arg1, Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
	return result;
}

VARIANT _ApplicationHanCell::ConvertFormula(const VARIANT& Formula, long FromReferenceStyle, const VARIANT& ToReferenceStyle, const VARIANT& ToAbsolute, const VARIANT& RelativeTo)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x42, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Formula, FromReferenceStyle, &ToReferenceStyle, &ToAbsolute, &RelativeTo);
	return result;
}

void _ApplicationHanCell::ViewMode()
{
	InvokeHelper(0x1388, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ApplicationHanCell::UnViewMode()
{
	InvokeHelper(0x1389, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _ApplicationHanCell::GetConvertFileMode()
{
	BOOL result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ApplicationHanCell::SetConvertFileMode(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x43, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _ApplicationHanCell::GetMessageBoxMode()
{
	long result;
	InvokeHelper(0x138a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long _ApplicationHanCell::SetMessageBoxMode(long Mode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x138b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Mode);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetSheets()
{
	LPDISPATCH result;
	InvokeHelper(0x1e5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlScriptVBS()
{
	short result;
	InvokeHelper(0x9e06, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlScriptVBA()
{
	short result;
	InvokeHelper(0x9e07, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMT_SheetDocument()
{
	short result;
	InvokeHelper(0x9e08, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMT_BookDocument()
{
	short result;
	InvokeHelper(0x9e09, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMT_AppDocument()
{
	short result;
	InvokeHelper(0x9e0a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMT_Module()
{
	short result;
	InvokeHelper(0x9e0b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlMT_ClassMdoule()
{
	short result;
	InvokeHelper(0x9e0c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateTel()
{
	short result;
	InvokeHelper(0xc350, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateRegident()
{
	short result;
	InvokeHelper(0xc351, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateForeign()
{
	short result;
	InvokeHelper(0xc352, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateEmail()
{
	short result;
	InvokeHelper(0xc353, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateAccount()
{
	short result;
	InvokeHelper(0xc354, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateCredit()
{
	short result;
	InvokeHelper(0xc355, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateIP()
{
	short result;
	InvokeHelper(0xc356, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateBirth()
{
	short result;
	InvokeHelper(0xc357, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateAddress()
{
	short result;
	InvokeHelper(0xc358, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateCustom()
{
	short result;
	InvokeHelper(0xc359, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateEtc()
{
	short result;
	InvokeHelper(0xc35a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateLicense()
{
	short result;
	InvokeHelper(0xc35b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivatePassport()
{
	short result;
	InvokeHelper(0xc35c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateReplaceAll()
{
	short result;
	InvokeHelper(0xc35d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateReplaceType1()
{
	short result;
	InvokeHelper(0xc35e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateReplaceType2()
{
	short result;
	InvokeHelper(0xc35f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateReplaceType3()
{
	short result;
	InvokeHelper(0xc360, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetHncPrivateReplaceType4()
{
	short result;
	InvokeHelper(0xc361, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ApplicationHanCell::GetDialogs()
{
	LPDISPATCH result;
	InvokeHelper(0x2f9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

short _ApplicationHanCell::GetXlDialogOpen()
{
	short result;
	InvokeHelper(0x9e90, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Worksheet properties

/////////////////////////////////////////////////////////////////////////////
// _Worksheet operations

RangeHanCell _WorksheetHanCell::GetRangeFromLong(LONG row, LONG column)
{
	TCHAR str[500] = {NULL};
	if ( row < 1 || column < 1 )
		return NULL;
	if ( column > 0 )
	{
		INT i = 0;
		while(column>0)
		{
			// Find remainder
			int rem = column%26;
			
			// If remainder is 0, then a 'Z' must be there in output
			if (rem==0)
			{
				str[i++] = 'Z';
				column = (column/26)-1;
			}
			else // If remainder is non-zero
			{
				str[i++] = (rem-1) + 'A';
				column = column/26;
			}
		}
		str[i] = '\0';
	}
	CString sRange;
	sRange.Format(_T("%s%d"), str, row);
	return GetRange(COleVariant(sRange), COleVariant(sRange));
}


void RangeHanCell::ChangeRangeToData( VARIANT InVar, CString& Out)
{
	// KU0133-2 JMC 20160826 구입자료관리 등의 화면에서 숫자로 된 컬럼 반입시 NULL로 처리되는 오류
	Out.Format(_T(""));
	/*
	if ( InVar.bstrVal == NULL ) {
		Out.Format(_T(""));		
		return;
	}*/

	
	switch(InVar.vt)
	{
	case VT_EMPTY:
	case VT_NULL:
		Out.Empty();
		break;
	case VT_BSTR:
		Out.Format(_T("%s"), InVar.bstrVal);
		break;
	case VT_BSTR | VT_BYREF:
		Out.Format(_T("%s"), InVar.pbstrVal);
		break;
	case VT_I2:
	case VT_I4:
		Out.Format(_T("%d"), InVar.lVal);
		break;
	case VT_I2 | VT_BYREF:
	case VT_I4 | VT_BYREF:
		Out.Format(_T("%d"), InVar.plVal);
		break;
	case VT_R4:
	case VT_R8:
		if ( InVar.dblVal != (INT)InVar.dblVal )
		{
			Out.Format(_T("%.2f"), InVar.dblVal);
		}
		else
		{
			Out.Format(_T("%d"), (INT)InVar.dblVal);
		}
		break;
	case VT_R4 | VT_BYREF:
	case VT_R8 | VT_BYREF:
		if ( InVar.pdblVal != (double*)(INT)InVar.pdblVal )
		{
			Out.Format(_T("%.2f"), InVar.pdblVal);
		}
		else
		{
			Out.Format(_T("%d"), (INT)InVar.pdblVal);
		}
		break;
	default:
		Out.Empty();
	}
}




LPDISPATCH _WorksheetHanCell::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetCells()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetColumns()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _WorksheetHanCell::GetName()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _WorksheetHanCell::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _WorksheetHanCell::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetRows()
{
	LPDISPATCH result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetRange(const VARIANT& Cell1, const VARIANT& Cell2)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Cell1, &Cell2);
	return result;
}

long _WorksheetHanCell::GetVisible()
{
	long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _WorksheetHanCell::SetVisible(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _WorksheetHanCell::GetPageSetup()
{
	LPDISPATCH result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetUsedRange()
{
	LPDISPATCH result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetNames()
{
	LPDISPATCH result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _WorksheetHanCell::GetIndex()
{
	long result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

double _WorksheetHanCell::GetStandardHeight()
{
	double result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

double _WorksheetHanCell::GetStandardWidth()
{
	double result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void _WorksheetHanCell::SetStandardWidth(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _WorksheetHanCell::GetType()
{
	long result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetNext()
{
	LPDISPATCH result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetPrevious()
{
	LPDISPATCH result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetCircularReference()
{
	LPDISPATCH result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _WorksheetHanCell::GetConsolidationFunction()
{
	long result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT _WorksheetHanCell::GetConsolidationOptions()
{
	VARIANT result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _WorksheetHanCell::GetConsolidationSources()
{
	VARIANT result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

BOOL _WorksheetHanCell::GetProtectContents()
{
	BOOL result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetListObjects()
{
	LPDISPATCH result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _WorksheetHanCell::Activate()
{
	InvokeHelper(0x2710, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _WorksheetHanCell::Copy(const VARIANT& Before, const VARIANT& After)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2711, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Before, &After);
}

void _WorksheetHanCell::Move(const VARIANT& Before, const VARIANT& After)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2712, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Before, &After);
}

void _WorksheetHanCell::Delete()
{
	InvokeHelper(0x2713, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _WorksheetHanCell::Paste(const VARIANT& Destination, const VARIANT& Link)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2714, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Destination, &Link);
}

void _WorksheetHanCell::PrintPreview(const VARIANT& EnableChanges)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2715, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &EnableChanges);
}

void _WorksheetHanCell::Select(const VARIANT& Replace)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2716, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Replace);
}

void _WorksheetHanCell::PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2717, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName);
}

void _WorksheetHanCell::Protect(const VARIANT& Password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly, const VARIANT& AllowFormattingCells, const VARIANT& AllowFormattingColumns, 
		const VARIANT& AllowFormattingRows, const VARIANT& AllowInsertingColumns, const VARIANT& AllowInsertingRows, const VARIANT& AllowInsertingHyperlinks, const VARIANT& AllowDeletingColumns, const VARIANT& AllowDeletingRows, 
		const VARIANT& AllowSorting, const VARIANT& AllowFiltering, const VARIANT& AllowUsingPivotTables)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2719, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Password, &DrawingObjects, &Contents, &Scenarios, &UserInterfaceOnly, &AllowFormattingCells, &AllowFormattingColumns, &AllowFormattingRows, &AllowInsertingColumns, &AllowInsertingRows, &AllowInsertingHyperlinks, &AllowDeletingColumns, 
		&AllowDeletingRows, &AllowSorting, &AllowFiltering, &AllowUsingPivotTables);
}

void _WorksheetHanCell::Unprotect(const VARIANT& Password)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x271a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Password);
}

VARIANT _WorksheetHanCell::Evaluate(const VARIANT& Name)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x271b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Name);
	return result;
}

void _WorksheetHanCell::PasteSpecial(const VARIANT& Format, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel, const VARIANT& NoHTMLFormatting)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x271c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Format, &Link, &DisplayAsIcon, &IconFileName, &IconIndex, &IconLabel, &NoHTMLFormatting);
}

LPDISPATCH _WorksheetHanCell::GetShapes()
{
	LPDISPATCH result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetVPageBreaks()
{
	LPDISPATCH result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetHPageBreaks()
{
	LPDISPATCH result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorksheetHanCell::GetPrivateInfo()
{
	LPDISPATCH result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _WorkbookHanCell; properties

/////////////////////////////////////////////////////////////////////////////
// _WorkbookHanCell; operations

LPDISPATCH _WorkbookHanCell::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorkbookHanCell::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _WorkbookHanCell::GetName()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorkbookHanCell::GetNames()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorkbookHanCell::GetWorksheets()
{
	LPDISPATCH result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _WorkbookHanCell::GetSaved()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _WorkbookHanCell::SetSaved(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _WorkbookHanCell::GetFullName()
{
	CString result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _WorkbookHanCell::GetReadOnly()
{
	long result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorkbookHanCell::GetActiveSheet()
{
	LPDISPATCH result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _WorkbookHanCell::GetDate1904()
{
	BOOL result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _WorkbookHanCell::SetDate1904(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _WorkbookHanCell::GetProtectStructure()
{
	BOOL result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _WorkbookHanCell::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _WorkbookHanCell::Save()
{
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _WorkbookHanCell::Activate()
{
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _WorkbookHanCell::SaveAs(const VARIANT& Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, long AccessMode, const VARIANT& ConflictResolution, 
		const VARIANT& AddToMru, const VARIANT& TextCodepage, const VARIANT& TextVisualLayout, const VARIANT& Local)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Filename, &FileFormat, &Password, &WriteResPassword, &ReadOnlyRecommended, &CreateBackup, AccessMode, &ConflictResolution, &AddToMru, &TextCodepage, &TextVisualLayout, &Local);
}

void _WorkbookHanCell::Close(const VARIANT& SaveChanges, const VARIANT& Filename, const VARIANT& RouteWorkbook)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &SaveChanges, &Filename, &RouteWorkbook);
}

void _WorkbookHanCell::RunAutoMacros(long Which)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Which);
}

void _WorkbookHanCell::PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName);
}

void _WorkbookHanCell::PrintPreview(const VARIANT& EnableChanges)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &EnableChanges);
}

void _WorkbookHanCell::ViewMode()
{
	InvokeHelper(0x1388, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _WorkbookHanCell::UnViewMode()
{
	InvokeHelper(0x1389, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _WorkbookHanCell::DeleteNumberFormat(LPCTSTR NumberFormat)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x138a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NumberFormat);
}

void _WorkbookHanCell::Protect(const VARIANT& Password, const VARIANT& Structure, const VARIANT& Windows)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x138b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Password, &Structure, &Windows);
}

void _WorkbookHanCell::Unprotect(const VARIANT& Password)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x138c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Password);
}

VARIANT _WorkbookHanCell::GetTextFile(LPCTSTR Format, LPCTSTR option)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x2727, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Format, option);
	return result;
}

void _WorkbookHanCell::ConvertToFile(const VARIANT& Filename, const VARIANT& Filetype, const VARIANT& Resolution)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x138d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Filename, &Filetype, &Resolution);
}

void _WorkbookHanCell::ConvertToImage(const VARIANT& Filename, const VARIANT& Filetype, const VARIANT& Resolution, const VARIANT& StartPage, const VARIANT& EndPage)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x138e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Filename, &Filetype, &Resolution, &StartPage, &EndPage);
}

LPDISPATCH _WorkbookHanCell::GetSheets()
{
	LPDISPATCH result;
	InvokeHelper(0x1e5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Range properties

/////////////////////////////////////////////////////////////////////////////
// Range operations

LPDISPATCH RangeHanCell::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetHidden()
{
	VARIANT result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetHidden(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

LPDISPATCH RangeHanCell::GetEntireRow()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetRows()
{
	LPDISPATCH result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetColumns()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetCurrentRegion()
{
	LPDISPATCH result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long RangeHanCell::GetCount()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetEntireColumn()
{
	LPDISPATCH result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetName()
{
	VARIANT result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetName(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

long RangeHanCell::GetRow()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long RangeHanCell::GetColumn()
{
	long result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetColumnWidth()
{
	VARIANT result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetColumnWidth(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetRowHeight()
{
	VARIANT result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetRowHeight(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

LPDISPATCH RangeHanCell::GetBorders()
{
	LPDISPATCH result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetInterior()
{
	LPDISPATCH result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetNumberFormat()
{
	VARIANT result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetNumberFormat(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetNumberFormatLocal()
{
	VARIANT result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetNumberFormatLocal(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetFormulaLocal()
{
	VARIANT result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetFormulaLocal(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetFormula()
{
	VARIANT result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetFormula(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

LPDISPATCH RangeHanCell::GetCells()
{
	LPDISPATCH result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetHeight()
{
	VARIANT result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetWidth()
{
	VARIANT result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetWorksheet()
{
	LPDISPATCH result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetAreas()
{
	LPDISPATCH result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetLeft()
{
	VARIANT result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetTop()
{
	VARIANT result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetHorizontalAlignment()
{
	VARIANT result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetHorizontalAlignment(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetVerticalAlignment()
{
	VARIANT result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetVerticalAlignment(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

LPDISPATCH RangeHanCell::GetMergeArea()
{
	LPDISPATCH result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetMergeCells()
{
	VARIANT result;
	InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetMergeCells(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetValue(const VARIANT& RangeValueDataType)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
		&RangeValueDataType);
	return result;
}

void RangeHanCell::SetValue(const VARIANT& RangeValueDataType, const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &RangeValueDataType, &newValue);
}

LPDISPATCH RangeHanCell::GetOffset(const VARIANT& RowOffset, const VARIANT& ColumnOffset)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&RowOffset, &ColumnOffset);
	return result;
}

VARIANT RangeHanCell::GetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
		&RowIndex, &ColumnIndex);
	return result;
}

void RangeHanCell::SetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &RowIndex, &ColumnIndex, &newValue);
}

LPDISPATCH RangeHanCell::GetRange(const VARIANT& Cell1, const VARIANT& Cell2)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Cell1, &Cell2);
	return result;
}

VARIANT RangeHanCell::GetWrapText()
{
	VARIANT result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetWrapText(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

LPDISPATCH RangeHanCell::GetEnd(long direction)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		direction);
	return result;
}

VARIANT RangeHanCell::GetShrinkToFit()
{
	VARIANT result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetShrinkToFit(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetStyle()
{
	VARIANT result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetStyle(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetValue2()
{
	VARIANT result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetValue2(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetText()
{
	VARIANT result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetFormulaR1C1()
{
	VARIANT result;
	InvokeHelper(0x2a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetFormulaR1C1(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetFormulaR1C1Local()
{
	VARIANT result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetFormulaR1C1Local(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

LPDISPATCH RangeHanCell::GetCurrentArray()
{
	LPDISPATCH result;
	InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetDependents()
{
	LPDISPATCH result;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetDirectDependents()
{
	LPDISPATCH result;
	InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetDirectPrecedents()
{
	LPDISPATCH result;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetFormulaArray()
{
	VARIANT result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetFormulaArray(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetFormulaHidden()
{
	VARIANT result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetFormulaHidden(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetHasArray()
{
	VARIANT result;
	InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetHasFormula()
{
	VARIANT result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetIndentLevel()
{
	VARIANT result;
	InvokeHelper(0x3a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetIndentLevel(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

LPDISPATCH RangeHanCell::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString RangeHanCell::GetAddress(const VARIANT& RowAbsolute, const VARIANT& ColumnAbsolute, long ReferenceStyle, const VARIANT& External, const VARIANT& RelativeTo)
{
	CString result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x3c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		&RowAbsolute, &ColumnAbsolute, ReferenceStyle, &External, &RelativeTo);
	return result;
}

CString RangeHanCell::GetAddressLocal(const VARIANT& RowAbsolute, const VARIANT& ColumnAbsolute, long ReferenceStyle, const VARIANT& External, const VARIANT& RelativeTo)
{
	CString result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		&RowAbsolute, &ColumnAbsolute, ReferenceStyle, &External, &RelativeTo);
	return result;
}

LPDISPATCH RangeHanCell::GetNext()
{
	LPDISPATCH result;
	InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetPrevious()
{
	LPDISPATCH result;
	InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetPrecedents()
{
	LPDISPATCH result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RangeHanCell::GetResize(const VARIANT& RowSize, const VARIANT& ColumnSize)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&RowSize, &ColumnSize);
	return result;
}

BOOL RangeHanCell::GetAllowEdit()
{
	BOOL result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::GetUseStandardHeight()
{
	VARIANT result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetUseStandardHeight(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x43, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetUseStandardWidth()
{
	VARIANT result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetUseStandardWidth(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x44, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

long RangeHanCell::GetPageBreak()
{
	long result;
	InvokeHelper(0x45, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetPageBreak(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x45, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT RangeHanCell::Select()
{
	VARIANT result;
	InvokeHelper(0x2710, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::ClearContents()
{
	VARIANT result;
	InvokeHelper(0x2712, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::ClearFormats()
{
	VARIANT result;
	InvokeHelper(0x2713, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::Clear()
{
	VARIANT result;
	InvokeHelper(0x2714, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::Activate()
{
	VARIANT result;
	InvokeHelper(0x2715, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::Copy(const VARIANT& Destination)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2716, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Destination);
	return result;
}

VARIANT RangeHanCell::Cut(const VARIANT& Destination)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2717, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Destination);
	return result;
}

VARIANT RangeHanCell::FillDown()
{
	VARIANT result;
	InvokeHelper(0x2718, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::FillLeft()
{
	VARIANT result;
	InvokeHelper(0x2719, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::FillRight()
{
	VARIANT result;
	InvokeHelper(0x271a, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::FillUp()
{
	VARIANT result;
	InvokeHelper(0x271b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::Insert(const VARIANT& Shift, const VARIANT& CopyOrigin)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x271c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Shift, &CopyOrigin);
	return result;
}

VARIANT RangeHanCell::Delete(const VARIANT& Shift)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x271d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Shift);
	return result;
}

VARIANT RangeHanCell::Merge(const VARIANT& Across)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x271e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Across);
	return result;
}

VARIANT RangeHanCell::UnMerge()
{
	VARIANT result;
	InvokeHelper(0x271f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::AutoFill(LPDISPATCH Destination, long Type)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_I4;
	InvokeHelper(0x2726, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Destination, Type);
	return result;
}

VARIANT RangeHanCell::AutoFit()
{
	VARIANT result;
	InvokeHelper(0x2727, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT RangeHanCell::PasteSpecial(long Paste, long Operation, const VARIANT& SkipBlanks, const VARIANT& Transpose)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2728, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Paste, Operation, &SkipBlanks, &Transpose);
	return result;
}

VARIANT RangeHanCell::Get_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
		&RowIndex, &ColumnIndex);
	return result;
}

void RangeHanCell::Set_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &RowIndex, &ColumnIndex, &newValue);
}

VARIANT RangeHanCell::Sort(const VARIANT& Key1, long Order1, const VARIANT& Key2, const VARIANT& Type, long Order2, const VARIANT& Key3, long Order3, long Header, const VARIANT& OrderCustom, const VARIANT& MatchCase, long Orientation, long SortMethod, 
		long DataOption1, long DataOption2, long DataOption3)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x2729, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Key1, Order1, &Key2, &Type, Order2, &Key3, Order3, Header, &OrderCustom, &MatchCase, Orientation, SortMethod, DataOption1, DataOption2, DataOption3);
	return result;
}

BOOL RangeHanCell::Replace(const VARIANT& What, const VARIANT& Replacement, const VARIANT& LookAt, const VARIANT& SearchOrder, const VARIANT& MatchCase, const VARIANT& MatchByte, const VARIANT& SearchFormat, const VARIANT& ReplaceFormat)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x272a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&What, &Replacement, &LookAt, &SearchOrder, &MatchCase, &MatchByte, &SearchFormat, &ReplaceFormat);
	return result;
}

VARIANT RangeHanCell::DataSeries(const VARIANT& Rowcol, long Type, long Date, const VARIANT& Step, const VARIANT& Stop, const VARIANT& Trend)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x272b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Rowcol, Type, Date, &Step, &Stop, &Trend);
	return result;
}

VARIANT RangeHanCell::ApplyNames(const VARIANT& Names, const VARIANT& IgnoreRelativeAbsolute, const VARIANT& UseRowColumnNames, const VARIANT& OmitColumn, const VARIANT& OmitRow, long Order, const VARIANT& AppendLast)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT;
	InvokeHelper(0x272e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Names, &IgnoreRelativeAbsolute, &UseRowColumnNames, &OmitColumn, &OmitRow, Order, &AppendLast);
	return result;
}

CString RangeHanCell::AutoComplete(LPCTSTR String)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x272f, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		String);
	return result;
}

LPDISPATCH RangeHanCell::ColumnDifferences(const VARIANT& Comparison)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2730, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Comparison);
	return result;
}

LPDISPATCH RangeHanCell::RowDifferences(const VARIANT& Comparison)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2731, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Comparison);
	return result;
}

LPDISPATCH RangeHanCell::Find(const VARIANT& What, const VARIANT& After, const VARIANT& LookIn, const VARIANT& LookAt, const VARIANT& SearchOrder, long SearchDirection, const VARIANT& MatchCase, const VARIANT& MatchByte, const VARIANT& SearchFormat)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2732, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&What, &After, &LookIn, &LookAt, &SearchOrder, SearchDirection, &MatchCase, &MatchByte, &SearchFormat);
	return result;
}

LPDISPATCH RangeHanCell::FindNext(const VARIANT& After)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2733, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&After);
	return result;
}

VARIANT RangeHanCell::Consolidate(const VARIANT& Sources, const VARIANT& Function, const VARIANT& TopRow, const VARIANT& LeftColumn, const VARIANT& CreateLinks)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2735, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Sources, &Function, &TopRow, &LeftColumn, &CreateLinks);
	return result;
}

void RangeHanCell::InsertIndent(long InsertAmount)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2736, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 InsertAmount);
}

VARIANT RangeHanCell::BorderAround(const VARIANT& LineStyle, long Weight, long ColorIndex, const VARIANT& Color)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT;
	InvokeHelper(0x2737, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&LineStyle, Weight, ColorIndex, &Color);
	return result;
}

VARIANT RangeHanCell::PrintPreview(const VARIANT& EnableChanges)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2738, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&EnableChanges);
	return result;
}

VARIANT RangeHanCell::PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2739, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName);
	return result;
}

LPDISPATCH RangeHanCell::SpecialCells(long Type, const VARIANT& Value)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x273a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, &Value);
	return result;
}

VARIANT RangeHanCell::TextToColumns(const VARIANT& Destination, long DataType, long TextQualifier, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma, const VARIANT& Space, const VARIANT& Other, 
		const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator, const VARIANT& TrailingMinusNumbers)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x273b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Destination, DataType, TextQualifier, &ConsecutiveDelimiter, &Tab, &Semicolon, &Comma, &Space, &Other, &OtherChar, &FieldInfo, &DecimalSeparator, &ThousandsSeparator, &TrailingMinusNumbers);
	return result;
}

VARIANT RangeHanCell::Table(const VARIANT& RowInput, const VARIANT& ColumnInput)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x273c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&RowInput, &ColumnInput);
	return result;
}

VARIANT RangeHanCell::AdvancedFilter(long Action, const VARIANT& CriteriaRange, const VARIANT& CopyToRange, const VARIANT& Unique)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x273d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Action, &CriteriaRange, &CopyToRange, &Unique);
	return result;
}

BOOL RangeHanCell::GoalSeek(const VARIANT& Goal, LPDISPATCH ChangingCell)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT VTS_DISPATCH;
	InvokeHelper(0x273e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&Goal, ChangingCell);
	return result;
}

VARIANT RangeHanCell::Justify()
{
	VARIANT result;
	InvokeHelper(0x273f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::ExportImageFile(LPCTSTR Filename)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2740, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Filename);
}

VARIANT RangeHanCell::GetLocked()
{
	VARIANT result;
	InvokeHelper(0x46, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetLocked(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

VARIANT RangeHanCell::GetFirstLineIndentLevel()
{
	VARIANT result;
	InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void RangeHanCell::SetFirstLineIndentLevel(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}


/////////////////////////////////////////////////////////////////////////////
// Workbooks properties

/////////////////////////////////////////////////////////////////////////////
// Workbooks operations

LPDISPATCH WorkbooksHanCell::Get_Default(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH WorkbooksHanCell::GetItem(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH WorkbooksHanCell::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long WorkbooksHanCell::GetCount()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH WorkbooksHanCell::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPUNKNOWN WorkbooksHanCell::Get_NewEnum()
{
	LPUNKNOWN result;
	InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPDISPATCH WorkbooksHanCell::Open(const VARIANT& Filename, const VARIANT& UpdateLinks, const VARIANT& ReadOnly, const VARIANT& Format, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& IgnoreReadOnlyRecommended, const VARIANT& Origin, 
		const VARIANT& Delimiter, const VARIANT& Editable, const VARIANT& Notify, const VARIANT& Converter, const VARIANT& AddToMru, const VARIANT& Local, const VARIANT& CorruptLoad)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2710, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Filename, &UpdateLinks, &ReadOnly, &Format, &Password, &WriteResPassword, &IgnoreReadOnlyRecommended, &Origin, &Delimiter, &Editable, &Notify, &Converter, &AddToMru, &Local, &CorruptLoad);
	return result;
}

void WorkbooksHanCell::Close()
{
	InvokeHelper(0x2711, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH WorkbooksHanCell::Add(const VARIANT& Template)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2712, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Template);
	return result;
}

void WorkbooksHanCell::OpenText(LPCTSTR Filename, const VARIANT& Origin, const VARIANT& StartRow, const VARIANT& DataType, long TextQualifier, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma, 
		const VARIANT& Space, const VARIANT& Other, const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& TextVisualLayout, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator, const VARIANT& TrailingMinusNumbers, 
		const VARIANT& Local)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2715, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Filename, &Origin, &StartRow, &DataType, TextQualifier, &ConsecutiveDelimiter, &Tab, &Semicolon, &Comma, &Space, &Other, &OtherChar, &FieldInfo, &TextVisualLayout, &DecimalSeparator, &ThousandsSeparator, &TrailingMinusNumbers, &Local);
}

LPDISPATCH WorkbooksHanCell::AddEx(const VARIANT& Visible)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x271a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Visible);
	return result;
}

LPDISPATCH WorkbooksHanCell::OpenEx(const VARIANT& Filename, const VARIANT& UpdateLinks, const VARIANT& ReadOnly, const VARIANT& Format, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& IgnoreReadOnlyRecommended, const VARIANT& Origin, 
		const VARIANT& Delimiter, const VARIANT& Editable, const VARIANT& Notify, const VARIANT& Converter, const VARIANT& AddToMru, const VARIANT& Local, const VARIANT& CorruptLoad, const VARIANT& Visible)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x271b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Filename, &UpdateLinks, &ReadOnly, &Format, &Password, &WriteResPassword, &IgnoreReadOnlyRecommended, &Origin, &Delimiter, &Editable, &Notify, &Converter, &AddToMru, &Local, &CorruptLoad, &Visible);
	return result;
}
