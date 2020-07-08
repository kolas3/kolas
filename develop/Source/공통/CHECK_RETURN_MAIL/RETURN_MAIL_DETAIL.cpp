// RETURN_MAIL_DETAIL.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "RETURN_MAIL_DETAIL.h"
#include "RETURN_CODE_DLG.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRETURN_MAIL_DETAIL dialog


CRETURN_MAIL_DETAIL::CRETURN_MAIL_DETAIL(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRETURN_MAIL_DETAIL)
	//}}AFX_DATA_INIT
//JOB.2019.0073 : 충돌 License 제거
//	m_pReturnCodeMgr = NULL ;
	AfxInitRichEdit () ;

	// KOL.UDF.022 시큐어코딩 보완
	m_Idx = -1;
}
CRETURN_MAIL_DETAIL::~CRETURN_MAIL_DETAIL()
{
}

BOOL CRETURN_MAIL_DETAIL::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CRETURN_MAIL_DETAIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRETURN_MAIL_DETAIL)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRETURN_MAIL_DETAIL, CDialog)
	//{{AFX_MSG_MAP(CRETURN_MAIL_DETAIL)
	ON_BN_CLICKED(IDC_bNext, OnbNext)
	ON_BN_CLICKED(IDC_bPrev, OnbPrev)
	ON_BN_CLICKED(IDC_bCODE, OnbCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRETURN_MAIL_DETAIL message handlers

BOOL CRETURN_MAIL_DETAIL::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DisplayData(0) ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CRETURN_MAIL_DETAIL::DisplayData(INT ndir)
{
	INT  ids ;
	switch (ndir)
	{
	case 0 :                                                                         
		
		SelectMakeList(m_strParentCM ,m_strParentGrid );
		ids = SelectGetHeadPosition(m_strParentCM ,m_strParentGrid);
		if (ids < 0)
		{
			AfxMessageBox(_T("선택 목록을 구성할 수 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return -222;
		}
		break;
		
	case -1 :
		
        ids = SelectGetPrev (m_strParentCM ,m_strParentGrid);
		if (ids == -1) {
			ids = SelectGetHeadPosition(m_strParentCM ,m_strParentGrid);
			AfxMessageBox(_T(" 처음 자료입니다."));
			return -223;
		}
		break;
		
	case 1 :
		
		ids = SelectGetNext (m_strParentCM ,m_strParentGrid);
		if (ids == -1) {
			ids = SelectGetTailPosition(m_strParentCM ,m_strParentGrid);
			AfxMessageBox(_T(" 마지막 자료입니다."));
			return -224;
		}
		break;
	}
	
	
    m_Idx = ids; 
    SetReverse (m_strParentCM ,m_strParentGrid ,m_Idx) ;
	
	CString strUserMail ;
	CString strMessage ;

	GetDataMgrData (m_strParentDM ,_T("TRANSCRIPT"),strMessage,m_Idx ) ;
	GetDataMgrData (m_strParentDM ,_T("BODYTO"),strUserMail,m_Idx ) ;

	strUserMail.Replace (_T("<"),_T("")) ;
	strUserMail.Replace (_T(">"),_T("")) ;


	CRichEditCtrl *pRichCtrl = (CRichEditCtrl *)GetDlgItem (IDC_RICHEDIT_MSG) ;
	pRichCtrl->SetWindowText (strMessage) ;

	CEdit *pEdit = (CEdit*)GetDlgItem (IDC_EDIT_MAIL) ;
	pEdit->SetWindowText (strUserMail) ;



	return 0 ;
}

VOID CRETURN_MAIL_DETAIL::SetManger(CString strDM, CString strCM,CString strGrid)
{
	m_strParentDM = strDM ;
	m_strParentCM = strCM ;
	m_strParentGrid = strGrid ;

	return ;
}

VOID CRETURN_MAIL_DETAIL::OnbNext() 
{
	// TODO: Add your control notification handler code here
	DisplayData (1) ;
}

VOID CRETURN_MAIL_DETAIL::OnbPrev() 
{
	// TODO: Add your control notification handler code here
	DisplayData (-1) ;
}

VOID CRETURN_MAIL_DETAIL::OnbCODE() 
{
	// TODO: Add your control notification handler code here
	//CRETURN_MAIL_MAIN* 
	//JOB.2019.0073 : 충돌 License 제거	
	CRETURN_CODE_DLG dlg(this) ;
//	if(m_pReturnCodeMgr == NULL) return ;
//	dlg.SetReturnCodeMgr (m_pReturnCodeMgr  ) ;
	dlg.DoModal () ;
	
}

HBRUSH CRETURN_MAIL_DETAIL::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
