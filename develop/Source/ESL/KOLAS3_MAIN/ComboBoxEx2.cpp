// ComboBoxEx2.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "ComboBoxEx2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboBoxEx2

CComboBoxEx2::CComboBoxEx2()
{
	m_bDrawIconToEdit = TRUE;
	m_hPrevSelectItemIcon = NULL;
}

CComboBoxEx2::~CComboBoxEx2()
{
}


BEGIN_MESSAGE_MAP(CComboBoxEx2, CComboBoxEx)
	//{{AFX_MSG_MAP(CComboBoxEx2)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxEx2 message handlers

BOOL CComboBoxEx2::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//DEL 	return CComboBoxEx::OnEraseBkgnd(pDC);
	return TRUE;
}

LRESULT CComboBoxEx2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	LRESULT lResult = CComboBoxEx::WindowProc(message, wParam, lParam);

	if(TRUE == m_bDrawIconToEdit && NULL != GetSafeHwnd())
	{		
		CDC* pDC = GetDC();
		HICON hDrawIcon = (NULL == m_hPrevSelectItemIcon) ? 
			m_hDefaultIcon : m_hPrevSelectItemIcon;
		DrawIconEx(
			pDC->m_hDC,
			3,
			3,
			hDrawIcon,
			16,
			16,
			NULL,
			(HBRUSH)RGB(255, 255, 255),
			DI_NORMAL
			);
		ReleaseDC(pDC);	
	}

	return lResult;
}

INT CComboBoxEx2::Set_DefaultIcon(HICON hIcon)
{
	m_hDefaultIcon = hIcon;
	return 0;
}

INT CComboBoxEx2::Set_DrawIconToEdit(BOOL bDraw)
{
	m_bDrawIconToEdit = bDraw;
	return 0;
}

int CComboBoxEx2::SetCurSel(int nSelect)
{		
	INT nResult = CComboBox::SetCurSel(nSelect);
 	if(CB_ERR != nResult)
	{			
 		if(0 <= nSelect)
		{
			m_hPrevSelectItemIcon = 
				(GetImageList())->ExtractIcon(nSelect);		

			CString strText;
			GetLBText(nSelect, strText);
	//DEL 		TRACE(_T("%d - %s\r\n"), nSelect, strText);
		}
	//DEL 	Set_DrawIconToEdit(FALSE);
	}
	else 
	{
	//DEL 	TRACE(_T("m_hPrevSelectItemIcon = NULL\r\n"));
	 	m_hPrevSelectItemIcon = NULL;
	//DEL 	Set_DrawIconToEdit(TRUE);
	}
	return nResult;
}

//DEL void CComboBoxEx2::OnDropdown() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	Set_DrawIconToEdit(TRUE);
//DEL }
