


#include "stdafx.h"
#include "resource.h"
#include "REFUSE_VIEW_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CREFUSE_VIEW_DLG::CREFUSE_VIEW_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
	
	m_pCMB = NULL;
}
CREFUSE_VIEW_DLG::~CREFUSE_VIEW_DLG()
{
	if(NULL != m_pCMB) delete m_pCMB;
}

void CREFUSE_VIEW_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EDIT_REFUSE_CONTENT_VIEW, mCtrl_edtContentView);
	
}


BEGIN_MESSAGE_MAP(CREFUSE_VIEW_DLG, CDialog)
	//{{AFX_MSG_MAP(CREFUSE_VIEW_DLG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




HBRUSH CREFUSE_VIEW_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
  	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

BOOL CREFUSE_VIEW_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	Init_MultiCombo();
	mCtrl_edtContentView.SetWindowText(m_strContent);

	return TRUE;  
	              
}

int CREFUSE_VIEW_DLG::Init_MultiCombo()
{
 	int ids = 0;

  	CString strQuery = _T("");
	CString strValue = _T("");

  	strQuery.Format(
  		_T("SELECT DESCRIPTION FROM ILL_CODE_TBL ")
  		_T("WHERE CLASS = '%s' AND CODE = '%s'"),
  		m_strClass, m_strCode);
	
  	ids = m_DM.GetOneValueQuery(strQuery, strValue);
  	if(0 > ids) return ids;

 	if(NULL == m_pCMB)
	{
		m_pCMB = new CESL_ControlMultiComboBox(NULL);	
 		m_pCMB->SubclassDlgItem(IDC_cmb_REFUSE_VIEW, this);	
 		m_pCMB->InitMultiComboBox(2);
	}

  	CString strData[2] = { _T(""), _T("") };
	strData[0] = m_strCode;
	strData[1] = strValue;
  	m_pCMB->SetData(strData, 2);  
  	m_pCMB->SetCurSel(0);	

	return 0;
}

BOOL CREFUSE_VIEW_DLG::PreTranslateMessage(MSG* pMsg) 
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
