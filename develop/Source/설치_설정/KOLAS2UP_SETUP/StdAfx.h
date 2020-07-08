// stdafx.h : include file for standard _tsystem include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__08FFD3D4_7FB0_4BC4_95B9_51A358EEA9F8__INCLUDED_)
#define AFX_STDAFX_H__08FFD3D4_7FB0_4BC4_95B9_51A358EEA9F8__INCLUDED_

#include "SystemDefine.h"

#define _UP
//#define _LIGHT
//#define _ECOLAS_S
//#define _ECOLAS

#ifdef _ECOLAS_S
#pragma message("### [StdAfx.h] ### _ECOLAS_S Version !!!")
#endif
#ifdef _UP
#pragma message("### [StdAfx.h] ### _UP Version !!!")
	#if defined(_ECOLAS_S)
		#error ### [StdAfx.h] ### _ECOLAS_S is already defined
	#endif
#endif
#ifdef _LIGHT
#pragma message(_T("### [StdAfx.h] ### _LIGHT Version !!!"))
	#if defined(_ECOLAS_S)
		#error ### [StdAfx.h] ### _ECOLAS_S is already defined
	#endif
	#if defined(_UP)
		#error ### [StdAfx.h] ### _UP is already defined
	#endif
#endif
#ifdef _ECOLAS
#pragma message(_T("### [StdAfx.h] ### _ECOLAS Version !!!"))
	#if defined(_ECOLAS_S)
		#error ### [StdAfx.h] ### _ECOLAS_S is already defined
	#endif
	#if defined(_UP)
		#error ### [StdAfx.h] ### _UP is already defined
	#endif
	#if defined(_LIGHT)
		#error ### [StdAfx.h] ### _LIGHT is already defined
	#endif
#endif

#if !defined(_ECOLAS_S) && !defined(_UP) && !defined(_LIGHT) && !defined(_ECOLAS)
#error ### [StdAfx.h] ### _ECOLAS_S && _UP && _LIGHT && _ECOLAS Un#defined !!!
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

#endif // !defined(AFX_STDAFX_H__08FFD3D4_7FB0_4BC4_95B9_51A358EEA9F8__INCLUDED_)
