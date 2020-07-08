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
#if !defined(AFX_INTENSITYDLG_H__E39430BC_2863_4B3E_8DDF_BBBFAF3E15A6__INCLUDED_)
#define AFX_INTENSITYDLG_H__E39430BC_2863_4B3E_8DDF_BBBFAF3E15A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IntensityDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIntensityDlg dialog

class CIntensityDlg : public CDialog
{
// Construction
public:
	CIntensityDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIntensityDlg)
	enum { IDD = IDD_INTENSITY };
	double	m_Exponent;
	double	m_Intensity;
	int		m_Offset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntensityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIntensityDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTENSITYDLG_H__E39430BC_2863_4B3E_8DDF_BBBFAF3E15A6__INCLUDED_)
