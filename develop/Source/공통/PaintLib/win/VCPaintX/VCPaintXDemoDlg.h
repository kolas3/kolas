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
// VCPaintXDemoDlg.h : header file
//
//{{AFX_INCLUDES()
#include "image.h"
//}}AFX_INCLUDES

#ifdef _DEBUG
  #import "..\..\bin\debug\PaintX.dll"
#else
  #import "..\..\bin\release\PaintX.dll"
#endif

#if !defined(AFX_VCPAINTXDEMODLG_H__9B270259_CDCA_11D3_8258_0020AF58A97B__INCLUDED_)
#define AFX_VCPAINTXDEMODLG_H__9B270259_CDCA_11D3_8258_0020AF58A97B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVCPaintXDemoDlg dialog

class CVCPaintXDemoDlg : public CDialog
{
// Construction
public:
	CVCPaintXDemoDlg(CWnd* pParent = NULL);	// standard constructor
	void LoadPictureTest(CString strFilename);
	void LoadResPictureTest(int iResourceID);

// Dialog Data
	//{{AFX_DATA(CVCPaintXDemoDlg)
	enum { IDD = IDD_VCPAINTXDEMO_DIALOG };
	CImage	m_Image;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCPaintXDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    PAINTXLib::IPictureDecoderPtr MyPictureDecoder;
    void HandleErrors(_com_error comerr);

	// Generated message map functions
	//{{AFX_MSG(CVCPaintXDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCPAINTXDEMODLG_H__9B270259_CDCA_11D3_8258_0020AF58A97B__INCLUDED_)
