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
#if !defined(AFX_PALVIEWDLG_H__B222EB8D_89CE_464B_A3F7_39A5B9C22891__INCLUDED_)
#define AFX_PALVIEWDLG_H__B222EB8D_89CE_464B_A3F7_39A5B9C22891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PalViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPalViewDlg dialog

class CPalViewDlg : public CDialog
{
// Construction
public:
	CPalViewDlg(CWnd* pParent, PLPixel32 * pPal);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPalViewDlg)
	enum { IDD = IDD_PALETTE };
	CStatic	m_PalFrameCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPalViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPalViewDlg)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  PLPixel32 * m_pPal;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALVIEWDLG_H__B222EB8D_89CE_464B_A3F7_39A5B9C22891__INCLUDED_)
