// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0F61C54F_607F_4302_9434_6EF515AF0476__INCLUDED_)
#define AFX_STDAFX_H__0F61C54F_607F_4302_9434_6EF515AF0476__INCLUDED_

#include "SystemDefine.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef _UNICODE
	#ifndef __WFILE__
	#define WIDEN2(x)   L ## x		 
	#define WIDEN(x)   WIDEN2(x) 
	#define __WFILE__ WIDEN(__FILE__)
	#endif
#else
	#ifndef __WFILE__
	#define __WFILE__ __FILE__
	#endif
#endif

#include "..\\LOC_MACRO\\ErrorMacro.h"

#define CANCEL_DIALOG(x,y)\
	{\
		SHOW_MESSAGE2(x,y);\
		CDialog::OnCancel();\
		return FALSE;\
	}\

#define UPDATE_OPTION_DM WM_USER+2
#define WM_CLICK_DATE_BTN WM_USER+10
#define WM_SEARCH WM_USER+3

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0F61C54F_607F_4302_9434_6EF515AF0476__INCLUDED_)
