#include "stdafx.h"
#include "BO_LOC_3192.h"
#include "Imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3192::CBO_LOC_3192(CESL_Mgr* pParent /*=NULL*/,CLoanShareManager* pLoanShareManager, CLonaManageValue *pLona , INT nMode )
	: CESL_Mgr(IDD, pParent)
{	
	m_pLona = pLona;
	m_nMode = nMode;

	m_pLoanShareManager = pLoanShareManager;
	
	m_pNonDBBookSpfWork = NULL;
}


CBO_LOC_3192::~CBO_LOC_3192()
{
	if( m_pNonDBBookSpfWork != NULL ) 
		delete m_pNonDBBookSpfWork;
	m_pNonDBBookSpfWork = NULL;
}

VOID CBO_LOC_3192::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3192)
	//}}AFX_DATA_MAP	
	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3192, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3192)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnNON_BOOK_ADD, OnbtnNONBOOKADD)
	ON_BN_CLICKED(IDC_btnNON_BOOK_EDIT, OnbtnNONBOOKEDIT)
	ON_BN_CLICKED(IDC_btnNON_BOOK_DELETE, OnbtnNONBOOKDELETE)
	ON_BN_CLICKED(IDC_btnNON_BOOK_CLEAR, OnbtnNONBOOKCLEAR)
	ON_BN_CLICKED(IDC_btnNON_BOOK_LOAN, OnbtnNONBOOKLOAN)
	ON_BN_CLICKED(IDC_btnNON_BOOK_CLOSE, OnbtnNONBOOKCLOSE)
	ON_BN_CLICKED(IDC_btnMOVECAT, OnbtnMOVECAT)
	ON_EN_SETFOCUS(IDC_edtAUTHOR, OnSetfocusedtAUTHOR)
	ON_EN_SETFOCUS(IDC_edtCALLNO, OnSetfocusedtCALLNO)
	ON_EN_SETFOCUS(IDC_edtPUBLISHER, OnSetfocusedtPUBLISHER)
	ON_EN_SETFOCUS(IDC_edtREG_NO, OnSetfocusedtREGNO)
	ON_EN_SETFOCUS(IDC_edtTITLE, OnSetfocusedtTITLE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


VOID CBO_LOC_3192::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	INT ids;

	ids = InitScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnShowWindow") );

	EFS_END
}

BOOL CBO_LOC_3192::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	if(InitESL_Mgr(_T("BO_LOC_3192")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pNonDBBookSpfWork = new CBL_LOC_NON_DB_BOOK_PROC(this, m_pLoanShareManager ,  m_pLona);

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_3192_EDIT_INFO"), _T("등록번호"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);	

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	
	return TRUE;
	EFS_END
	return FALSE;

}


BOOL CBO_LOC_3192::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3192::OnClose() 
{
	EFS_BEGIN
	OnCancel();

	EFS_END
}


INT CBO_LOC_3192::InitScreen()
{
	EFS_BEGIN

	INT ids;

	ids = m_pNonDBBookSpfWork->SPFExcutePath( INIT_SCREEN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	if( m_nMode == 1 )
		MakeInsertControl();


	return 0;

	EFS_END
	return -1;

}


VOID CBO_LOC_3192::OnbtnNONBOOKADD() 
{
	EFS_BEGIN

	INT ids;

	ids = m_pNonDBBookSpfWork->SPFExcutePath( INISERT_NONBOOK );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnNONBOOKADD") );	
	if( ids > 0 ) return;

	GetDlgItem(IDC_btnNON_BOOK_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNON_BOOK_DELETE)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNON_BOOK_LOAN)->EnableWindow(TRUE);	
	
	CESL_DataMgr* pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if( pMemberDM == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnbtnNONBOOKADD") );

	if( pMemberDM->GetRowCount() < 1 )
		return ;

	ids = MessageBox( _T("긴급대출하시겠습니까?") , _T("긴급대출") , MB_YESNO | MB_ICONQUESTION );
	
	if( ids == IDNO ) return;

	ids = m_pNonDBBookSpfWork->SPFExcutePath( LOAN_NONBOOK );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("OnbtnNONBOOKLOAN") );		
	if( ids > 0 ) return;

	if( m_nMode == 1 )
		OnCancel();

	EFS_END
}


VOID CBO_LOC_3192::OnbtnNONBOOKEDIT() 
{
	EFS_BEGIN

	INT ids;

	ids = m_pNonDBBookSpfWork->SPFExcutePath( EDIT_NONBOOK );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnNONBOOKEDIT") );		

	EFS_END
}


VOID CBO_LOC_3192::OnbtnNONBOOKDELETE() 
{
	EFS_BEGIN

	INT ids;

	ids = m_pNonDBBookSpfWork->SPFExcutePath( DELETE_NONBOOK );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnNONBOOKDELETE") );		

	EFS_END
}


VOID CBO_LOC_3192::OnbtnNONBOOKCLEAR() 
{
	EFS_BEGIN

	INT ids;
	ids = m_pNonDBBookSpfWork->SPFExcutePath( CLEAR_NONBOOK );

	EFS_END
}


VOID CBO_LOC_3192::OnbtnNONBOOKLOAN() 
{
	EFS_BEGIN

	INT ids;

	ids = m_pNonDBBookSpfWork->SPFExcutePath( LOAN_NONBOOK );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnNONBOOKLOAN") );		

	EFS_END
}

VOID CBO_LOC_3192::OnbtnNONBOOKCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

BEGIN_EVENTSINK_MAP(CBO_LOC_3192, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3192)
	ON_EVENT(CBO_LOC_3192, IDC_gridNonDBBookList, -600 /* Click */, OnClickgridNonDBBookList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3192::OnClickgridNonDBBookList() 
{
	EFS_BEGIN

	INT ids;

	ids = SetSelect();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-1,_T("OnClickgridNonDBBookList"));

	EFS_END
}

INT CBO_LOC_3192::SetSelect()
{
	EFS_BEGIN

	INT nIndex;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetSelect") );
	
	nIndex = pGrid->GetIdx();

	if( nIndex < 0 ) return 0;

	pGrid->SetReverse(nIndex);

	INT ids = m_pNonDBBookSpfWork->ViewDetailInfo(nIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetSelect") );

	GetDlgItem(IDC_btnNON_BOOK_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNON_BOOK_DELETE)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNON_BOOK_LOAN)->EnableWindow(TRUE);

	return 0;

	EFS_END
	return -1;

}

CESL_Grid* CBO_LOC_3192::GetMainGridPtr()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3192");
	CString sGridAlias = _T("긴급대출자료목록");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}


INT CBO_LOC_3192::MakeInsertControl()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3192_EDIT_INFO");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3170::MakeInsertControl()") ); 

	pCM->AllControlClear();

	CESL_Grid *pGrid = GetMainGridPtr();

	pGrid->SetReverse(-1);

	
	CString sParentCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");

	CString sRegNo;
	ids = GetControlData( sParentCMAlias , _T("대출자번호") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3170::MakeInsertControl()") ); 

	ids = SetControlData( sCMAlias , _T("등록번호") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3170::MakeInsertControl()") ); 

	ids = SetControlData( sCMAlias , _T("상태") , _T("비치자료") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CBO_LOC_3170::MakeInsertControl()") ); 
	
	GetDlgItem(IDC_btnNON_BOOK_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnNON_BOOK_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnNON_BOOK_LOAN)->EnableWindow(FALSE);

	return 0;


	EFS_END
	return -1;

}

VOID CBO_LOC_3192::OnbtnMOVECAT() 
{
	EFS_BEGIN

	m_pNonDBBookSpfWork->SPFExcutePath(MOVE_TO_CAT);
		
	EFS_END
}

VOID CBO_LOC_3192::OnSetfocusedtAUTHOR() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtAUTHOR)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtAUTHOR)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3192::OnSetfocusedtCALLNO() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtCALLNO)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtCALLNO)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3192::OnSetfocusedtPUBLISHER() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtPUBLISHER)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtPUBLISHER)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3192::OnSetfocusedtREGNO() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtREG_NO)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtREG_NO)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3192::OnSetfocusedtTITLE() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtTITLE)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtTITLE)->m_hWnd,hIME); 
    }
}

HBRUSH CBO_LOC_3192::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
