// stdafx.h : include file for standard _tsystem include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D5C7DFCF_E018_4B6D_8088_F54E177A2C2A__INCLUDED_)
#define AFX_STDAFX_H__D5C7DFCF_E018_4B6D_8088_F54E177A2C2A__INCLUDED_

#include "SystemDefine.h"

#ifdef _ECOLAS_S
#pragma message("### [StdAfx.h] ### _ECOLAS_S Version !!!")
#endif
#ifdef _ECOLAS
#pragma message("### [StdAfx.h] ### _ECOLAS Version !!!")
#endif
#ifdef _KOLASIII
#pragma message("### [StdAfx.h] ### _KOLASIII Version !!!")
#endif
#ifdef _KOLASIII_TL
#pragma message("### [StdAfx.h] ### _KOLASIII_TL Version !!!")
#endif
#ifdef _LIGHT
#pragma message("### [StdAfx.h] ### _LIGHT Version !!!")
#endif
#if !defined(_ECOLAS_S) && !defined(_KOLASIII) && !defined(_KOLASIII_TL) && !defined(_LIGHT) && !defined(_ECOLAS)
#error ### [StdAfx.h] ### _ECOLAS && _ECOLAS_S && _KOLASIII && _KOLASIII_TL && _LIGHT
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma comment(lib, "msimg32.lib")

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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D5C7DFCF_E018_4B6D_8088_F54E177A2C2A__INCLUDED_)
