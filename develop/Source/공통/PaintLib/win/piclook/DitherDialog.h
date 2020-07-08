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
// DitherDialog.h: interface for the CDitherDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DITHERDIALOG_H__722D7101_F010_11D3_AD70_005004AE55E5__INCLUDED_)
#define AFX_DITHERDIALOG_H__722D7101_F010_11D3_AD70_005004AE55E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDitherDialog : public CDialog  
{
public:
	CDitherDialog(CWnd* pParent = NULL);

	CComboBox m_DitherPaletteTypeBox;
	CComboBox m_DitherTypeBox;

  // Dialog Data
  //{{AFX_DATA(CCropFilterDlg)
  enum { IDD = IDD_CONVERTTO8BPP};
  UINT m_iDitherType;
  UINT m_iDitherPaletteType;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCropFilterDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CCropFilterDlg)
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DITHERDIALOG_H__722D7101_F010_11D3_AD70_005004AE55E5__INCLUDED_)
