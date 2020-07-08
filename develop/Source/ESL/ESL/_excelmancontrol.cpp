// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "_excelmancontrol.h"

/////////////////////////////////////////////////////////////////////////////
// C_ExcelManControl

IMPLEMENT_DYNCREATE(C_ExcelManControl, CWnd)

/////////////////////////////////////////////////////////////////////////////
// C_ExcelManControl properties

/////////////////////////////////////////////////////////////////////////////
// C_ExcelManControl operations

BOOL C_ExcelManControl::OpenManager()
{
	BOOL result;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, NULL);
	return result;
}

BOOL C_ExcelManControl::CloseManager()
{
	BOOL result;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, NULL);
	return result;
}

BOOL C_ExcelManControl::SaveAs(LPCTSTR fName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		fName);
	return result;
}

BOOL C_ExcelManControl::Close_WorkSheets()
{
	BOOL result;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, NULL);
	return result;
}

CString C_ExcelManControl::GetTextMatrix(LPCTSTR row, LPCTSTR col)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_BSTR, (VOID*)&result, parms,
		row, col);
	return result;
}

BOOL C_ExcelManControl::SetTextMatrix(LPCTSTR row, LPCTSTR col, LPCTSTR newvalue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		row, col, newvalue);
	return result;
}

BOOL C_ExcelManControl::SetTextMatrix_By_Name(LPCTSTR name, LPCTSTR newvalue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		name, newvalue);
	return result;
}

BOOL C_ExcelManControl::SetTextMatrix_By_Range(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, LPCTSTR newvalue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, newvalue);
	return result;
}

BOOL C_ExcelManControl::Print_Excel()
{
	BOOL result;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, NULL);
	return result;
}

VOID C_ExcelManControl::SetMode(short new_mode)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 new_mode);
}

VOID C_ExcelManControl::SetFileName(LPCTSTR new_file_name)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 new_file_name);
}

CString C_ExcelManControl::Find_String(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, LPCTSTR value)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_BSTR, (VOID*)&result, parms,
		startx, starty, endx, endy, value);
	return result;
}

CString C_ExcelManControl::FindNext_String(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_BSTR, (VOID*)&result, parms,
		startx, starty, endx, endy);
	return result;
}

BOOL C_ExcelManControl::Replace_String(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, LPCTSTR search_str, LPCTSTR replace_str)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, search_str, replace_str);
	return result;
}

BOOL C_ExcelManControl::Insert_Rows(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, short direction)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, direction);
	return result;
}

BOOL C_ExcelManControl::Insert_Entire_Row(LPCTSTR startx, LPCTSTR starty, short direction)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, direction);
	return result;
}

BOOL C_ExcelManControl::Insert_Entire_Column(LPCTSTR startx, LPCTSTR starty, short direction)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x60030010, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, direction);
	return result;
}

BOOL C_ExcelManControl::Delete_Entire_Row(LPCTSTR startx, LPCTSTR starty, short direction)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x60030011, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, direction);
	return result;
}

BOOL C_ExcelManControl::Merge(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030012, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy);
	return result;
}

BOOL C_ExcelManControl::Font_Bold(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, BOOL bold)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030013, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, bold);
	return result;
}

BOOL C_ExcelManControl::Font_Italic(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, BOOL italic)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030014, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, italic);
	return result;
}

BOOL C_ExcelManControl::Font_Size(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, short size)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x60030015, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, size);
	return result;
}

BOOL C_ExcelManControl::Font_Color(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, short color)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x60030016, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, color);
	return result;
}

BOOL C_ExcelManControl::Border(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, short style, short weight, short color)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x60030017, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, style, weight, color);
	return result;
}

BOOL C_ExcelManControl::Fill_Border(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, short style, short weight, short color)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x60030018, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, style, weight, color);
	return result;
}

BOOL C_ExcelManControl::Cell_Format(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, LPCTSTR format)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030019, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, format);
	return result;
}

BOOL C_ExcelManControl::DrawLine(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, short border_type, short style, short weight, short color)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x6003001a, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, border_type, style, weight, color);
	return result;
}

BOOL C_ExcelManControl::Vertical_Alignment(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, short alignment)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x6003001b, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, alignment);
	return result;
}

BOOL C_ExcelManControl::Horizontal_Alignment(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, short alignment)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x6003001c, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, alignment);
	return result;
}

BOOL C_ExcelManControl::Cell_Color(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, short color)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x6003001d, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, color);
	return result;
}

BOOL C_ExcelManControl::WorkSheet_Add(short number_of_sheet, BOOL isBefore)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_BOOL;
	InvokeHelper(0x6003001e, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		number_of_sheet, isBefore);
	return result;
}

BOOL C_ExcelManControl::WorkSheet_Delete(short index_of_sheet)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6003001f, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		index_of_sheet);
	return result;
}

short C_ExcelManControl::WorkSheet_Count()
{
	short result;
	InvokeHelper(0x60030020, DISPATCH_METHOD, VT_I2, (VOID*)&result, NULL);
	return result;
}

BOOL C_ExcelManControl::WorkSheet_Activate(short index_of_sheet)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030021, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		index_of_sheet);
	return result;
}

short C_ExcelManControl::WorkSheet_Index()
{
	short result;
	InvokeHelper(0x60030022, DISPATCH_METHOD, VT_I2, (VOID*)&result, NULL);
	return result;
}

BOOL C_ExcelManControl::Set_Column_Width(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, double size)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_R8;
	InvokeHelper(0x60030023, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, size);
	return result;
}

double C_ExcelManControl::Get_Column_Width(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy)
{
	double result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030024, DISPATCH_METHOD, VT_R8, (VOID*)&result, parms,
		startx, starty, endx, endy);
	return result;
}

BOOL C_ExcelManControl::Set_Row_Width(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, double size)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_R8;
	InvokeHelper(0x60030025, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, size);
	return result;
}

double C_ExcelManControl::Get_Row_Width(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy)
{
	double result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030026, DISPATCH_METHOD, VT_R8, (VOID*)&result, parms,
		startx, starty, endx, endy);
	return result;
}

BOOL C_ExcelManControl::Set_Name(LPCTSTR startx, LPCTSTR starty, LPCTSTR endx, LPCTSTR endy, LPCTSTR name)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030027, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		startx, starty, endx, endy, name);
	return result;
}

BOOL C_ExcelManControl::Set_Name_By_Range(LPCTSTR Range_Str, LPCTSTR name)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030028, DISPATCH_METHOD, VT_BOOL, (VOID*)&result, parms,
		Range_Str, name);
	return result;
}
