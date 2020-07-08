


#include "stdafx.h"
#include "resource.h"
#include "REFUSE_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




CREFUSE_DLG::CREFUSE_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
	
	m_pCMB = NULL;
}

CREFUSE_DLG::~CREFUSE_DLG()
{
	if(NULL != m_pCMB) delete m_pCMB;
}
void CREFUSE_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EDIT_REFUSE_CONTENT, mCtrl_edtContent);
	
}


BEGIN_MESSAGE_MAP(CREFUSE_DLG, CDialog)
	//{{AFX_MSG_MAP(CREFUSE_DLG)
	ON_BN_CLICKED(IDC_btnREFUSE_OK, OnbtnREFUSEOK)
	ON_BN_CLICKED(IDC_btnREFUSE_CANCEL, OnbtnREFUSECANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CREFUSE_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	Init_MultiCombo();
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if(_T("") != m_strCaption) SetWindowText(m_strCaption);
	if(_T("") != m_strCauseCaption) 
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_CAUSE);
		if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
			pWnd->SetWindowText(m_strCauseCaption);
	}
	return TRUE;  
	              
}

void CREFUSE_DLG::OnbtnREFUSEOK() 
{
	
	int nIdx = m_pCMB->GetCurSel();
	m_strCancelCode = m_pCMB->GetItemDataString(nIdx, 0);

	
	CString strContent, strCancelDescription;
	
	strCancelDescription = m_pCMB->GetItemDataString(nIdx, 1);
	mCtrl_edtContent.GetWindowText(strContent);

	m_strContent.Empty();
	m_strContent += _T("»çÀ¯:") + strCancelDescription;
	if(strContent.IsEmpty() == FALSE){
		m_strContent += _T(" / ") + strContent;
	}


	
	CDialog::OnOK();
}

void CREFUSE_DLG::OnbtnREFUSECANCEL() 
{
	
	CDialog::OnCancel();
}


int CREFUSE_DLG::Init_MultiCombo()
{
 	int ids = 0;
 	CString strQuery = _T("");
 	strQuery.Format(
 		_T("SELECT CODE, DESCRIPTION FROM ILL_CODE_TBL ")
 		_T("WHERE CLASS = '%s' AND CODE != '0000'"),
 		m_strClass);
 
 	ids = m_DM.RestructDataManager(strQuery);
 	if(0 > ids) return ids;
		
	if(NULL == m_pCMB)
	{
		m_pCMB = new CESL_ControlMultiComboBox(NULL);	
 		m_pCMB->SubclassDlgItem(IDC_cmb_REFUSE, this);	
 		m_pCMB->InitMultiComboBox(2);
	}

 	CString strData[2] = { _T(""), _T("") };
 	int ROW = m_DM.GetRowCount();
 	for(int nCnt = 0; nCnt < ROW; nCnt++)
 	{
 		ids = m_DM.GetCellData(nCnt, 0, strData[0]);
 		if(0 > ids) return ids;
 
 		ids = m_DM.GetCellData(nCnt, 1, strData[1]);
 		if(0 > ids) return ids;
 
 		m_pCMB->SetData(strData, 2);
 	}
 	m_pCMB->SetCurSel(0);

	return 0;
}

HBRUSH CREFUSE_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
 	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	return hbr;
}

BOOL CREFUSE_DLG::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
