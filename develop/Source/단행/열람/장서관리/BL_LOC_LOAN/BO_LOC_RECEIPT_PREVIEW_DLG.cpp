#include "stdafx.h"
#include "BO_LOC_RECEIPT_PREVIEW_DLG.h"
#include "CharConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_RECEIPT_PREVIEW_DLG::CBO_LOC_RECEIPT_PREVIEW_DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CBO_LOC_RECEIPT_PREVIEW_DLG::IDD, pParent)
{
	
	AfxInitRichEdit();
	
}
CBO_LOC_RECEIPT_PREVIEW_DLG::~CBO_LOC_RECEIPT_PREVIEW_DLG()
{
	m_Dlg_brush.DeleteObject();
}

VOID CBO_LOC_RECEIPT_PREVIEW_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_MSG_MAP(CBO_LOC_RECEIPT_PREVIEW_DLG)	
	DDX_Control(pDX, IDC_edtPREVIEW, m_ctlPreview);
	//}}AFX_MSG_MAP	
}


BEGIN_MESSAGE_MAP(CBO_LOC_RECEIPT_PREVIEW_DLG, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_RECEIPT_PREVIEW_DLG)
	ON_BN_CLICKED(IDC_btnPRINT_RECEIPT, OnbtnPRINTRECEIPT)
	ON_BN_CLICKED(IDC_btnLocloanCLOSE, OnbtnCLOSE)
	ON_NOTIFY(NM_SETFOCUS, IDC_edtPREVIEW, OnSetfocusedtPREVIEW)
	ON_NOTIFY(NM_CLICK, IDC_edtPREVIEW, OnClickedtPREVIEW)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(EN_SETFOCUS, IDC_edtPREVIEW, OnSetfocusedtPREVIEW)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


VOID CBO_LOC_RECEIPT_PREVIEW_DLG::SetPrintData(CString strData)
{
	m_strData = strData;
}

BOOL CBO_LOC_RECEIPT_PREVIEW_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ShowReceipt();	

	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239)); 

	return TRUE;  
	              
}

VOID CBO_LOC_RECEIPT_PREVIEW_DLG::InitFont()
{   

	m_defaultcf.cbSize = sizeof(CHARFORMAT); 
	m_defaultcf.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE ^ CFM_BOLD;	
	m_defaultcf.dwEffects = 0;			
	m_defaultcf.yHeight = 200;	
	m_defaultcf.crTextColor =RGB(0,0,0); 
	m_defaultcf.bCharSet = ANSI_CHARSET; 
	m_defaultcf.bPitchAndFamily = 2; 
	
	strcpy(m_defaultcf.szFaceName, "±¼¸²Ã¼" );
	m_ctlPreview.SetDefaultCharFormat( m_defaultcf );

	return;
}

VOID CBO_LOC_RECEIPT_PREVIEW_DLG::OnbtnPRINTRECEIPT() 
{
	CDialog::OnOK();	
}

VOID CBO_LOC_RECEIPT_PREVIEW_DLG::OnbtnCLOSE() 
{


	CDialog::OnCancel();	
}

VOID CBO_LOC_RECEIPT_PREVIEW_DLG::OnSetfocusedtPREVIEW(NMHDR* pNMHDR, LRESULT* pResult) 
{
	( (CButton*) GetDlgItem(IDC_btnPRINT_RECEIPT) )->SetFocus();
	*pResult = 0;
}

VOID CBO_LOC_RECEIPT_PREVIEW_DLG::OnClickedtPREVIEW(NMHDR* pNMHDR, LRESULT* pResult) 
{
	( (CButton*) GetDlgItem(IDC_btnPRINT_RECEIPT) )->SetFocus();
	
	*pResult = 0;
}

BOOL CBO_LOC_RECEIPT_PREVIEW_DLG::ShowReceipt()
{
	m_ctlPreview.HideSelection(TRUE, FALSE);
	InitFont();
	if ( !m_strData.IsEmpty() )
	{		
		m_strData.Replace(_T("<!FH><!FC>"),_T("   "));
		m_strData.Replace(_T("<!FC><!FF>"),_T(""));
		m_strData.Replace(_T("<!FF>"),_T(""));
		m_strData.Replace(_T("<!LF>"),_T(""));
		m_strData.Replace(_T("<!LC>"),_T(""));		
		m_strData.Replace(_T("<!FN>"),_T(""));
		m_strData.Replace(_T("<!LN>"),_T(""));				
		m_strData.Replace(_T("<!FR>"),_T("                         "));		
		m_strData.Replace(_T("<!LR>"),_T(""));
		m_strData.Replace(_T("<!LH>"),_T(""));
		m_strData.Replace(_T("<!FP>"),_T("\r\n"));
		m_ctlPreview.SetWindowText(m_strData);
		
		
		CharConvert cv;
		CHAR*       szTmp = NULL;
		INT         len   = 0;

		cv.UnicodeToMultiByte( m_strData.GetBuffer(0), &szTmp, &len );
		if( 0 < len )
		{
			strlen(szTmp);

			m_ctlPreview.SetSel(0,strlen(szTmp));
			m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
			m_ctlPreview.SetSel(0,(int)(strstr(szTmp,"\r\n")-szTmp+1));	
					
			delete [] szTmp;
		}
		else
		{
			m_ctlPreview.SetSel(0,m_strData.GetLength());
			m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
			m_ctlPreview.SetSel(0, m_strData.Find(_T("\r\n"),0) );
		}

		m_defaultcf.dwMask = CFM_BOLD | CFM_SIZE;	
		m_defaultcf.dwEffects = CFE_BOLD;			
		m_defaultcf.yHeight = 300;
		m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
		m_defaultcf.yHeight = 270;		
			
		CString	strTmp;
		strTmp = m_strData;
		strTmp.MakeReverse();

		cv.UnicodeToMultiByte( strTmp.GetBuffer(0), &szTmp, &len );
		if( 0 < len )
		{			
			m_ctlPreview.SetSel(strlen(szTmp) - (int)(strstr(szTmp,"\r\n")-szTmp+1) , strlen(szTmp) );
			m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
			m_ctlPreview.SetSel(0,0);			
			
			delete [] szTmp;
		}
		else
		{
			m_ctlPreview.SetSel(strTmp.GetLength() - strTmp.Find(_T("\r\n"),0), strTmp.GetLength() );
			m_ctlPreview.SetSelectionCharFormat(m_defaultcf);
			m_ctlPreview.SetSel(0,0);
		}
		
	}
	m_ctlPreview.SetSel(0,0);
	m_ctlPreview.HideSelection(TRUE, FALSE);		


		
	return TRUE;
}

BOOL CBO_LOC_RECEIPT_PREVIEW_DLG::PreTranslateMessage(MSG* pMsg) 
{	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_LOC_RECEIPT_PREVIEW_DLG::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if ( m_strDevice == _T("D") ) 
	{
		keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), 0, 0);  
		keybd_event(VK_F8,MapVirtualKey(VK_F8,0),0, 0); 
		keybd_event(VK_F8,MapVirtualKey(VK_F8,0),KEYEVENTF_KEYUP,0);  
		keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL,0),KEYEVENTF_KEYUP,0); 
	}
}

HBRUSH CBO_LOC_RECEIPT_PREVIEW_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = NULL;
	if(pWnd->m_hWnd == this->m_hWnd) hbr = HBRUSH(m_Dlg_brush);
	else hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(239, 239, 239));

		return m_Dlg_brush;
	}

	return hbr;
}