// stdafx.h : include file for standard _tsystem include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__75CA1495_5AD1_4827_90D6_4CC428342B54__INCLUDED_)
#define AFX_STDAFX_H__75CA1495_5AD1_4827_90D6_4CC428342B54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
// 2017/09/22 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// KOLIS-NET OpenAPI
/*//NEW------------------------------------------------------------------------*/
#pragma warning(disable:4786)
/*//END------------------------------------------------------------------------*/
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


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__75CA1495_5AD1_4827_90D6_4CC428342B54__INCLUDED_)
