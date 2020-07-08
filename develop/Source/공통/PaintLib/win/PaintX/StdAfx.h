/*
/--------------------------------------------------------------------
|
|      $Id: StdAfx.h,v 1.1 2009/12/09 05:34:38 박종운 Exp $
|
\--------------------------------------------------------------------
*/
// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__B69FE5C8_9F53_11D2_B13F_444553540000__INCLUDED_)
#define AFX_STDAFX_H__B69FE5C8_9F53_11D2_B13F_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

/* NO MFC
#include <afxwin.h>
#include <afxdisp.h>
*/

#include <atlbase.h>
#include <comdef.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B69FE5C8_9F53_11D2_B13F_444553540000__INCLUDED)
/*
/--------------------------------------------------------------------
|
|      $Log: StdAfx.h,v $
|      Revision 1.1  2009/12/09 05:34:38  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:03  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:36  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:16  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:25  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:40  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:54  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:07  황정영
|      2006.05.17
|
|      Revision 1.6  2000/09/01 14:53:34  Administrator
|      no message
|
|
\--------------------------------------------------------------------
*/
