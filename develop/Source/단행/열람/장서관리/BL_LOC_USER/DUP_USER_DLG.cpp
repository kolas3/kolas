#include "stdafx.h"

#include "DUP_USER_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDUP_USER_DLG::CDUP_USER_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pDM = NULL ;
	m_bDupUserView = TRUE;
}


CDUP_USER_DLG::~CDUP_USER_DLG()
{	
	
}
BOOL CDUP_USER_DLG::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}


VOID CDUP_USER_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDUP_USER_DLG) 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDUP_USER_DLG, CDialog)
	//{{AFX_MSG_MAP(CDUP_USER_DLG)
	ON_BN_CLICKED(IDC_btnDupUserView, OnbtnDupUserView)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

VOID CDUP_USER_DLG::OnOK() 
{	
	CDialog::OnCancel();
}

BOOL CDUP_USER_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("SM_DUP_USER_DLG"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Failed")) ;
		return FALSE ;
	}

	ShowDupUserData() ;
	if(FALSE == m_bDupUserView)
	{
		GetDlgItem(IDC_btnDupUserView)->ShowWindow(SW_HIDE);
	}

	return TRUE;  
	              
}

VOID CDUP_USER_DLG::ShowDupUserData()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_DUP_USER_DLG")) ;
	if(pCM == NULL)
	{
		AfxMessageBox(_T("FindCM() Failed")) ;
		return ;
	}

    
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("GRID_USER")) ;
	if(pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Failed")) ;
		return ;
	}
  
	CString strCIVIL_NO = _T("");
	CString strUSER_NO  = _T("");
	CString strUSER_ID  = _T("");

	INT nRowCnt = m_pDM->GetRowCount () ;
	
	pGrid->SetRows(nRowCnt+1) ;

	INT ids;CString sUserCivilNo;
	CString sCivilNoDisplayCheck;
	CString sUserNoCivilNoCheck;
	
	this->GetManageValue(	_T("열람"), _T("공통"), _T("대출자주민등록번호보여주기"), 
							_T("대출자주민번호"), sUserCivilNo );
		
	this->GetManageValue(	_T("열람"), _T("공통"), _T("이용자관리"), 
							_T("주민등록번호사용유무"), sCivilNoDisplayCheck );
	CString strIpinFlag;
	CString strMypinFlag;
	this->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("아이핀사용여부"), strIpinFlag);
	this->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("MYPIN사용여부"), strMypinFlag);
	if (sCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0) 
	{
		if (strIpinFlag.CompareNoCase(_T("Y")) != 0 && strMypinFlag.CompareNoCase(_T("Y")) != 0) 
		{
			if (m_pInfo->m_bCertifyUse == FALSE && m_pInfo->m_bKLUse == TRUE)
			{
				
				sCivilNoDisplayCheck = _T("Y");
			}
		}
	}

	this->GetManageValue(	_T("열람"), _T("공통"), _T("이용자관리"), 
							_T("대출화면주민번호표시유무"), sUserNoCivilNoCheck );

	sUserCivilNo.TrimLeft(); sUserCivilNo.TrimRight(); sUserCivilNo.MakeUpper();	
	sCivilNoDisplayCheck.TrimLeft(); sCivilNoDisplayCheck.TrimRight(); sCivilNoDisplayCheck.MakeUpper();
	sUserNoCivilNoCheck.TrimLeft(); sUserNoCivilNoCheck.TrimRight(); sUserNoCivilNoCheck.MakeUpper();
		
	for(INT i = 0 ; i < nRowCnt ; i++)
	{
		strCIVIL_NO = m_pDM->GetCellData (i,0) ;
		
		if(_T("Y")==m_sIsUnityLoanService)
		{
			this->ESL_DES_Decryption(strCIVIL_NO,strCIVIL_NO);
		}
		strUSER_NO	= m_pDM->GetCellData (i,1)  ;	
		strUSER_ID	= m_pDM->GetCellData (i,2)  ;

		if( sUserNoCivilNoCheck != _T("Y") )
		{
			CLocCommonAPI::ChangeSecuriyUserNo( this , strUSER_NO , m_sIsUnityLoanService );
		}			
		
		ids = CLocCommonAPI::ChangeSecuriyCivilNo( NULL, strCIVIL_NO , m_sIsUnityLoanService );
		if( 0 > ids ) 
		{
			AfxMessageBox( _T("FindControl() Failed") );
			return;
		}
		if( sCivilNoDisplayCheck == _T("N") )
		{			
			CString strColName;
			pGrid->SetColWidth( 3, 0, 0 );				
		}

		pGrid->SetTextMatrix ( i + 1, 1, strUSER_ID );
		pGrid->SetTextMatrix ( i + 1, 2, strUSER_NO );
		pGrid->SetTextMatrix ( i + 1, 3, strCIVIL_NO );
	}

	return ;
}

VOID CDUP_USER_DLG::SetDataManager(CESL_DataMgr *pDM)
{
	m_pDM = pDM ;
}

VOID CDUP_USER_DLG::OnbtnDupUserView() 
{
	CDialog::OnOK();	
}

HBRUSH CDUP_USER_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
