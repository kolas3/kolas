// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "ecom.htmleditorcontrol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HtmlEditorControl properties

/////////////////////////////////////////////////////////////////////////////
// HtmlEditorControl operations

CString HtmlEditorControl::GetToString()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::Equals(const VARIANT& obj)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&obj);
	return result;
}

long HtmlEditorControl::GetHashCode()
{
	long result;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPUNKNOWN HtmlEditorControl::GetType()
{
	LPUNKNOWN result;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

VARIANT HtmlEditorControl::GetLifetimeService()
{
	VARIANT result;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT HtmlEditorControl::InitializeLifetimeService()
{
	VARIANT result;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

LPDISPATCH HtmlEditorControl::CreateObjRef(LPUNKNOWN requestedType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		requestedType);
	return result;
}

LPDISPATCH HtmlEditorControl::GetSite()
{
	LPDISPATCH result;
	InvokeHelper(0x60020007, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefSite(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020007, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

void HtmlEditorControl::add_Disposed(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Disposed(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::Dispose()
{
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH HtmlEditorControl::GetContainer()
{
	LPDISPATCH result;
	InvokeHelper(0x6002000c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetAllowDrop()
{
	BOOL result;
	InvokeHelper(0x6002000d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetAllowDrop(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6002000d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL HtmlEditorControl::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0x60020011, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020011, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPUNKNOWN HtmlEditorControl::GetLayoutEngine()
{
	LPUNKNOWN result;
	InvokeHelper(0x60020015, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

unsigned long HtmlEditorControl::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH HtmlEditorControl::GetBackgroundImage()
{
	LPDISPATCH result;
	InvokeHelper(0x60020018, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefBackgroundImage(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020018, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

LPUNKNOWN HtmlEditorControl::GetContextMenu()
{
	LPUNKNOWN result;
	InvokeHelper(0x6002001c, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefContextMenu(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x6002001c, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH HtmlEditorControl::GetContextMenuStrip()
{
	LPDISPATCH result;
	InvokeHelper(0x6002001e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefContextMenuStrip(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002001e, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

LPUNKNOWN HtmlEditorControl::GetCursor()
{
	LPUNKNOWN result;
	InvokeHelper(0x60020020, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefCursor(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020020, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL HtmlEditorControl::GetFocused()
{
	BOOL result;
	InvokeHelper(0x60020024, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPUNKNOWN HtmlEditorControl::GetFont()
{
	LPUNKNOWN result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefFont(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long HtmlEditorControl::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString HtmlEditorControl::GetText()
{
	CString result;
	InvokeHelper(0x6002002f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002002f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void HtmlEditorControl::ResetBackColor()
{
	InvokeHelper(0x60020032, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::ResetCursor()
{
	InvokeHelper(0x60020033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::ResetFont()
{
	InvokeHelper(0x60020034, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::ResetForeColor()
{
	InvokeHelper(0x60020035, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::ResetRightToLeft()
{
	InvokeHelper(0x60020036, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::Refresh()
{
	InvokeHelper(0x60020037, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::ResetText()
{
	InvokeHelper(0x60020038, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL HtmlEditorControl::GetInvokeRequired()
{
	BOOL result;
	InvokeHelper(0x60020039, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

VARIANT HtmlEditorControl::EndInvoke(LPDISPATCH asyncResult)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002003b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		asyncResult);
	return result;
}

long HtmlEditorControl::GetHandle()
{
	long result;
	InvokeHelper(DISPID_HWND, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPUNKNOWN HtmlEditorControl::GetDataBindings()
{
	LPUNKNOWN result;
	InvokeHelper(0x60020041, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPUNKNOWN HtmlEditorControl::GetBindingContext()
{
	LPUNKNOWN result;
	InvokeHelper(0x60020042, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefBindingContext(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020042, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH HtmlEditorControl::GetAccessibilityObject()
{
	LPDISPATCH result;
	InvokeHelper(0x60020044, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString HtmlEditorControl::GetAccessibleDefaultActionDescription()
{
	CString result;
	InvokeHelper(0x60020045, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetAccessibleDefaultActionDescription(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020045, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString HtmlEditorControl::GetAccessibleDescription()
{
	CString result;
	InvokeHelper(0x60020047, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetAccessibleDescription(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020047, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString HtmlEditorControl::GetAccessibleName()
{
	CString result;
	InvokeHelper(0x60020049, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetAccessibleName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020049, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void HtmlEditorControl::add_AutoSizeChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002004d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_AutoSizeChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002004e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_BackColorChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002004f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_BackColorChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020050, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_BackgroundImageChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_BackgroundImageChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020052, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_BackgroundImageLayoutChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020053, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_BackgroundImageLayoutChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020054, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::ResetBindings()
{
	InvokeHelper(0x60020055, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::add_BindingContextChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020056, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_BindingContextChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020057, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

long HtmlEditorControl::GetBottom()
{
	long result;
	InvokeHelper(0x60020058, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetCanFocus()
{
	BOOL result;
	InvokeHelper(0x6002005a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetCanSelect()
{
	BOOL result;
	InvokeHelper(0x6002005b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetCapture()
{
	BOOL result;
	InvokeHelper(0x6002005c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetCapture(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6002005c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL HtmlEditorControl::GetCausesValidation()
{
	BOOL result;
	InvokeHelper(0x6002005e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetCausesValidation(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6002005e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void HtmlEditorControl::add_CausesValidationChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020060, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_CausesValidationChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020061, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_ClientSizeChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020065, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_ClientSizeChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020066, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

CString HtmlEditorControl::GetCompanyName()
{
	CString result;
	InvokeHelper(0x60020067, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetContainsFocus()
{
	BOOL result;
	InvokeHelper(0x60020068, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::add_ContextMenuChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020069, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_ContextMenuChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002006a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_ContextMenuStripChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002006b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_ContextMenuStripChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002006c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

LPUNKNOWN HtmlEditorControl::GetControls()
{
	LPUNKNOWN result;
	InvokeHelper(0x6002006d, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetCreated()
{
	BOOL result;
	InvokeHelper(0x6002006e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::add_CursorChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002006f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_CursorChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020070, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

BOOL HtmlEditorControl::GetIsDisposed()
{
	BOOL result;
	InvokeHelper(0x60020071, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetDisposing()
{
	BOOL result;
	InvokeHelper(0x60020072, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::add_DockChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020073, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_DockChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020074, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

BOOL HtmlEditorControl::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void HtmlEditorControl::add_EnabledChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020077, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_EnabledChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020078, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_FontChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020079, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_FontChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002007a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_ForeColorChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002007b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_ForeColorChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002007c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

BOOL HtmlEditorControl::GetHasChildren()
{
	BOOL result;
	InvokeHelper(0x6002007d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long HtmlEditorControl::GetHeight()
{
	long result;
	InvokeHelper(0x6002007e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002007e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL HtmlEditorControl::GetIsHandleCreated()
{
	BOOL result;
	InvokeHelper(0x60020080, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetIsAccessible()
{
	BOOL result;
	InvokeHelper(0x60020081, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetIsAccessible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020081, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL HtmlEditorControl::GetIsMirrored()
{
	BOOL result;
	InvokeHelper(0x60020083, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long HtmlEditorControl::GetLeft()
{
	long result;
	InvokeHelper(0x60020084, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetLeft(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020084, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void HtmlEditorControl::add_LocationChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020088, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_LocationChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020089, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MarginChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002008c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MarginChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002008d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

CString HtmlEditorControl::GetName()
{
	CString result;
	InvokeHelper(0x6002008e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002008e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH HtmlEditorControl::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x60020090, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefParent(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020090, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

CString HtmlEditorControl::GetProductName()
{
	CString result;
	InvokeHelper(0x60020092, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString HtmlEditorControl::GetProductVersion()
{
	CString result;
	InvokeHelper(0x60020093, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetRecreatingHandle()
{
	BOOL result;
	InvokeHelper(0x60020094, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPUNKNOWN HtmlEditorControl::GetRegion()
{
	LPUNKNOWN result;
	InvokeHelper(0x60020095, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefRegion(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020095, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

void HtmlEditorControl::add_RegionChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020097, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_RegionChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020098, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

long HtmlEditorControl::GetRight()
{
	long result;
	InvokeHelper(0x60020099, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::add_RightToLeftChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002009a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_RightToLeftChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002009b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_SizeChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002009e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_SizeChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002009f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

long HtmlEditorControl::GetTabIndex()
{
	long result;
	InvokeHelper(0x600200a0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetTabIndex(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x600200a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void HtmlEditorControl::add_TabIndexChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200a2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_TabIndexChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200a3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

BOOL HtmlEditorControl::GetTabStop()
{
	BOOL result;
	InvokeHelper(0xfffffdfc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetTabStop(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffdfc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void HtmlEditorControl::add_TabStopChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_TabStopChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200a7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

VARIANT HtmlEditorControl::GetTag()
{
	VARIANT result;
	InvokeHelper(0x600200a8, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefTag(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x600200a8, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 &newValue);
}

void HtmlEditorControl::add_TextChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200aa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_TextChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200ab, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

long HtmlEditorControl::GetTop()
{
	long result;
	InvokeHelper(0x600200ac, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetTop(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x600200ac, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH HtmlEditorControl::GetTopLevelControl()
{
	LPDISPATCH result;
	InvokeHelper(0x600200ae, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::GetUseWaitCursor()
{
	BOOL result;
	InvokeHelper(0x600200af, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetUseWaitCursor(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x600200af, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL HtmlEditorControl::GetVisible()
{
	BOOL result;
	InvokeHelper(0x600200b1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x600200b1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void HtmlEditorControl::add_VisibleChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200b3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_VisibleChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200b4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

long HtmlEditorControl::GetWidth()
{
	long result;
	InvokeHelper(0x600200b5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x600200b5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPUNKNOWN HtmlEditorControl::GetWindowTarget()
{
	LPUNKNOWN result;
	InvokeHelper(0x600200b7, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefWindowTarget(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200b7, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

void HtmlEditorControl::add_Click(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200b9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Click(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200ba, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_ControlAdded(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200bb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_ControlAdded(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200bc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_ControlRemoved(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200bd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_ControlRemoved(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200be, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_DragDrop(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200bf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_DragDrop(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200c0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_DragEnter(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200c1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_DragEnter(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200c2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_DragOver(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200c3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_DragOver(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200c4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_DragLeave(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200c5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_DragLeave(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200c6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_GiveFeedback(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200c7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_GiveFeedback(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200c8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_HandleCreated(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200c9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_HandleCreated(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200ca, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_HandleDestroyed(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200cb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_HandleDestroyed(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200cc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_HelpRequested(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200cd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_HelpRequested(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200ce, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_Invalidated(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200cf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Invalidated(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200d0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_PaddingChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200d4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_PaddingChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200d5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_Paint(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Paint(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200d7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_QueryContinueDrag(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200d8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_QueryContinueDrag(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200d9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_QueryAccessibilityHelp(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200da, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_QueryAccessibilityHelp(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200db, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_DoubleClick(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200dc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_DoubleClick(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200dd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_Enter(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200de, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Enter(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200df, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_GotFocus(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200e0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_GotFocus(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200e1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_KeyDown(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200e2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_KeyDown(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200e3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_KeyPress(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200e4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_KeyPress(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200e5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_KeyUp(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200e6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_KeyUp(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200e7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_Layout(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200e8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Layout(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200e9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_Leave(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200ea, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Leave(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200eb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_LostFocus(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200ec, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_LostFocus(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200ed, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseClick(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200ee, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseClick(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200ef, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseDoubleClick(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200f0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseDoubleClick(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200f1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseCaptureChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200f2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseCaptureChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200f3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseDown(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200f4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseDown(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200f5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseEnter(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200f6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseEnter(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200f7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseLeave(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200f8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseLeave(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200f9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseHover(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200fa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseHover(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x600200fb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseMove(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200fc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseMove(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200fd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseUp(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200fe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseUp(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x600200ff, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_MouseWheel(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020100, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_MouseWheel(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020101, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_Move(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020102, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Move(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020103, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_PreviewKeyDown(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020104, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_PreviewKeyDown(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020105, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_Resize(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020106, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Resize(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020107, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_ChangeUICues(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020108, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_ChangeUICues(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020109, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_StyleChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002010a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_StyleChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002010b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_SystemColorsChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002010c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_SystemColorsChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002010d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_Validating(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x6002010e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Validating(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x6002010f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_Validated(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020110, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Validated(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020111, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_ParentChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020112, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_ParentChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020113, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

LPDISPATCH HtmlEditorControl::BeginInvoke_2(LPDISPATCH method)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020114, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		method);
	return result;
}

void HtmlEditorControl::BringToFront()
{
	InvokeHelper(0x60020115, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL HtmlEditorControl::Contains(LPDISPATCH ctl)
{
	BOOL result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020116, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		ctl);
	return result;
}

LPUNKNOWN HtmlEditorControl::CreateGraphics()
{
	LPUNKNOWN result;
	InvokeHelper(0x60020117, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::CreateControl()
{
	InvokeHelper(0x60020118, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH HtmlEditorControl::FindForm()
{
	LPDISPATCH result;
	InvokeHelper(0x6002011b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::Focus()
{
	BOOL result;
	InvokeHelper(0x6002011c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPUNKNOWN HtmlEditorControl::GetContainerControl()
{
	LPUNKNOWN result;
	InvokeHelper(0x6002011f, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPDISPATCH HtmlEditorControl::GetNextControl(LPDISPATCH ctl, BOOL forward)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_BOOL;
	InvokeHelper(0x60020120, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ctl, forward);
	return result;
}

void HtmlEditorControl::Hide()
{
	InvokeHelper(0x60020121, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::Invalidate(LPUNKNOWN Region)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020122, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Region);
}

void HtmlEditorControl::Invalidate_2(LPUNKNOWN Region, BOOL invalidateChildren)
{
	static BYTE parms[] =
		VTS_UNKNOWN VTS_BOOL;
	InvokeHelper(0x60020123, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Region, invalidateChildren);
}

void HtmlEditorControl::Invalidate_3()
{
	InvokeHelper(0x60020124, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::Invalidate_4(BOOL invalidateChildren)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020125, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 invalidateChildren);
}

VARIANT HtmlEditorControl::Invoke_3(LPDISPATCH method)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020128, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		method);
	return result;
}

void HtmlEditorControl::PerformLayout()
{
	InvokeHelper(0x60020129, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::PerformLayout_2(LPDISPATCH affectedControl, LPCTSTR affectedProperty)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_BSTR;
	InvokeHelper(0x6002012a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 affectedControl, affectedProperty);
}

void HtmlEditorControl::ResumeLayout()
{
	InvokeHelper(0x60020130, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::ResumeLayout_2(BOOL PerformLayout)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020131, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PerformLayout);
}

void HtmlEditorControl::Scale(float ratio)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x60020132, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ratio);
}

void HtmlEditorControl::Scale_2(float dx, float dy)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4;
	InvokeHelper(0x60020133, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dx, dy);
}

void HtmlEditorControl::Select()
{
	InvokeHelper(0x60020135, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL HtmlEditorControl::SelectNextControl(LPDISPATCH ctl, BOOL forward, BOOL tabStopOnly, BOOL nested, BOOL wrap)
{
	BOOL result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL;
	InvokeHelper(0x60020136, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		ctl, forward, tabStopOnly, nested, wrap);
	return result;
}

void HtmlEditorControl::SendToBack()
{
	InvokeHelper(0x60020137, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::SetBounds(long x, long y, long width, long height)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60020138, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, width, height);
}

void HtmlEditorControl::Show()
{
	InvokeHelper(0x6002013a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::SuspendLayout()
{
	InvokeHelper(0x6002013b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void HtmlEditorControl::Update()
{
	InvokeHelper(0x6002013c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long HtmlEditorControl::GetImeMode()
{
	long result;
	InvokeHelper(0x6002013d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetImeMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002013d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void HtmlEditorControl::add_ImeModeChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002013f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_ImeModeChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020140, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::ResetImeMode()
{
	InvokeHelper(0x60020141, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL HtmlEditorControl::GetAutoScroll()
{
	BOOL result;
	InvokeHelper(0x60020142, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetAutoScroll(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020142, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPUNKNOWN HtmlEditorControl::GetHorizontalScroll()
{
	LPUNKNOWN result;
	InvokeHelper(0x6002014a, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPUNKNOWN HtmlEditorControl::GetVerticalScroll()
{
	LPUNKNOWN result;
	InvokeHelper(0x6002014b, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPUNKNOWN HtmlEditorControl::GetDockPadding()
{
	LPUNKNOWN result;
	InvokeHelper(0x6002014c, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::ScrollControlIntoView(LPDISPATCH activeControl)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002014d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 activeControl);
}

void HtmlEditorControl::add_Scroll(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x6002014e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Scroll(LPUNKNOWN value)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x6002014f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::SetAutoScrollMargin(long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020150, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y);
}

BOOL HtmlEditorControl::ValidateChildren()
{
	BOOL result;
	InvokeHelper(0x60020153, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH HtmlEditorControl::GetActiveControl()
{
	LPDISPATCH result;
	InvokeHelper(0x60020155, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetRefActiveControl(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020155, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

void HtmlEditorControl::add_AutoValidateChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002015b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_AutoValidateChanged(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002015c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

LPDISPATCH HtmlEditorControl::GetParentForm()
{
	LPDISPATCH result;
	InvokeHelper(0x6002015e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::PerformAutoScale()
{
	InvokeHelper(0x6002015f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL HtmlEditorControl::Validate()
{
	BOOL result;
	InvokeHelper(0x60020160, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL HtmlEditorControl::Validate_2(BOOL checkAutoValidate)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020161, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		checkAutoValidate);
	return result;
}

void HtmlEditorControl::add_AutoSizeChanged_2(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020162, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_AutoSizeChanged_2(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020163, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_AutoValidateChanged_2(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020166, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_AutoValidateChanged_2(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020167, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

long HtmlEditorControl::GetBorderStyle()
{
	long result;
	InvokeHelper(0x60020168, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void HtmlEditorControl::SetBorderStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020168, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void HtmlEditorControl::add_Load(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002016a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_Load(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002016b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::add_TextChanged_2(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002016c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

void HtmlEditorControl::remove_TextChanged_2(LPDISPATCH value)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6002016d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 value);
}

long HtmlEditorControl::setHtmlDoc(LPCTSTR htmlDoc)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002016e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		htmlDoc);
	return result;
}

CString HtmlEditorControl::getHtmlDoc()
{
	CString result;
	InvokeHelper(0x6002016f, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long HtmlEditorControl::setDocStyle(LPCTSTR style)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020170, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		style);
	return result;
}

CString HtmlEditorControl::getDocStyle()
{
	CString result;
	InvokeHelper(0x60020171, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long HtmlEditorControl::setVisible(BOOL visible)
{
	long result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020172, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		visible);
	return result;
}

long HtmlEditorControl::setLeft(long left)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020173, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		left);
	return result;
}

long HtmlEditorControl::setTop(long top)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020174, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		top);
	return result;
}

long HtmlEditorControl::setWidth(long width)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020175, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		width);
	return result;
}

long HtmlEditorControl::setHeight(long height)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020176, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		height);
	return result;
}

BOOL HtmlEditorControl::getVisible()
{
	BOOL result;
	InvokeHelper(0x60020177, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long HtmlEditorControl::getleft()
{
	long result;
	InvokeHelper(0x60020178, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long HtmlEditorControl::getTop()
{
	long result;
	InvokeHelper(0x60020179, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long HtmlEditorControl::getWidth()
{
	long result;
	InvokeHelper(0x6002017a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long HtmlEditorControl::getHeight()
{
	long result;
	InvokeHelper(0x6002017b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
