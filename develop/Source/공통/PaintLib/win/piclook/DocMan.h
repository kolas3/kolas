/*The zlib/libpng License

Copyright (c) <year> <copyright holders>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.*/
/*
/--------------------------------------------------------------------
|
|      $Id: DocMan.h,v 1.1 2009/12/09 05:34:50 박종운 Exp $
|
\--------------------------------------------------------------------
*/

#if !defined(AFX_DOCMANAGER_H__188308B4_0AAD_11D2_8A47_0000E81D3D27__INCLUDED_)
#define AFX_DOCMANAGER_H__188308B4_0AAD_11D2_8A47_0000E81D3D27__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CDocManagerEx

class CDocManagerEx : public CDocManager
{
    DECLARE_DYNAMIC(CDocManagerEx)

    // Construction
public:
    CDocManagerEx()
	{} ;
    virtual ~CDocManagerEx()
	{} ;

    // Overrides
    // helper for standard commdlg dialogs
    virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
                                  DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: DocMan.h,v $
|      Revision 1.1  2009/12/09 05:34:50  박종운
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:09  박대우
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:03:20  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:31  박우람
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:29  황정영
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:48  황정영
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:49:01  황정영
|      2006.05.23 초기화
|
|      Revision 1.1  2006/05/18 01:31:09  황정영
|      2006.05.17
|
|      Revision 1.6  2000/03/28 21:05:03  Ulrich von Zadow
|      Added zoom capability.
|
|      Revision 1.5  2000/01/10 23:53:01  Ulrich von Zadow
|      Changed formatting & removed tabs.
|
|      Revision 1.4  1999/12/02 17:07:35  Ulrich von Zadow
|      Changes by bdelmee.
|
|      Revision 1.3  1999/10/19 21:32:53  Ulrich von Zadow
|      no message
|
|
--------------------------------------------------------------------
*/
