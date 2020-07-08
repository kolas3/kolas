// COOPERATIVE_STATISTICS_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "COOPERATIVE_STATISTICS_MAIN.h"
#include "COOPERATIVE_STATISTICS_CNT.h"
#include "SBL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_MAIN dialog


CCOOPERATIVE_STATISTICS_MAIN::CCOOPERATIVE_STATISTICS_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCOOPERATIVE_STATISTICS_MAIN::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOOPERATIVE_STATISTICS_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pTmpDlg = NULL;
	m_pSInfoMgr = NULL;
	m_pStaticsMainInfoDM = NULL;
	m_pPreviewDlg = NULL;
}
CCOOPERATIVE_STATISTICS_MAIN::~CCOOPERATIVE_STATISTICS_MAIN()
{	
	if(m_pTmpDlg)
	{	
		delete m_pTmpDlg;
		m_pTmpDlg = NULL;
	}
	if(m_pPreviewDlg)
	{
		delete m_pPreviewDlg;
		m_pPreviewDlg = NULL;
	}
	if(m_pSInfoMgr)
	{
		delete m_pSInfoMgr;
		m_pSInfoMgr = NULL;
	}
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
}


void CCOOPERATIVE_STATISTICS_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOOPERATIVE_STATISTICS_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCOOPERATIVE_STATISTICS_MAIN, CDialog)
	//{{AFX_MSG_MAP(CCOOPERATIVE_STATISTICS_MAIN)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_chkSUMZERODEL, OnchkSUMZERODEL)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RUN_S, OnRunS)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_MAIN message handlers

BOOL CCOOPERATIVE_STATISTICS_MAIN::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CCOOPERATIVE_STATISTICS_MAIN::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	INT ids;

	ids = InitESL_Mgr( _T("COOPERATIVE_STATISTICS_MAIN") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	// DM생성
	MakeDMInfo();
	// 화면초기화
	InitScreen();
	// 통계화면의 검색조건,미리보기 화면생성
	InitEtcDialog();
	// 중복포함 합계
	((CButton*)GetDlgItem(IDC_chkDUP_DEL))->SetCheck(1);
	// 행의 합계가 0인 항목 감추기
	((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->SetCheck(1);
	
	// XP Theme
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	return TRUE;

EFS_END
return FALSE;
}

void CCOOPERATIVE_STATISTICS_MAIN::OnSize(UINT nType, int cx, int cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(-2, 35, cx+2, cy-35);	

	CMSHFlexGrid * pGrid2 = (CMSHFlexGrid*)GetDlgItem(IDC_gridCNT);
	if(pGrid2->GetSafeHwnd() == NULL) return;
	pGrid2->MoveWindow(-2, 35, cx+2, cy-35);	

EFS_END
}

/*
@Create
 - 
@Description
 - 
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. 
*/
INT CCOOPERATIVE_STATISTICS_MAIN::ShowTempDlg(BOOL bReadOnly)
{
EFS_BEGIN
	
	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN"), _T("통계종류"), sStaticsClass, -1, 0 );
		
	if( _T("제공횟수통계") == sStaticsClass || _T("신청횟수통계") == sStaticsClass )
	{
		CCOOPERATIVE_STATISTICS_CNT dlg(this);
		dlg.m_pParentGrid=(CMSHFlexGrid*)GetDlgItem(IDC_gridCNT);
		dlg.DoModal();
		return 0;
	}

	if( !m_pTmpDlg )
	{
		m_pTmpDlg = new CCOOPERATIVE_STATISTICS_TEMP( this, m_pSearchDlg, m_pPreviewDlg );
		m_pTmpDlg->pMain = this->pMain;
		m_pTmpDlg->pParentMgr = (CESL_Mgr*)this;
		if (bReadOnly) m_pTmpDlg->SM_STYLE = 'R';
		m_pTmpDlg->m_pInfo = this->m_pInfo;
		if (m_pTmpDlg->Create(this))
		{
			m_pTmpDlg->CenterWindow();
			m_pTmpDlg->ShowWindow(SW_SHOW);
			m_pTmpDlg->UpdateWindow();				
		}
		else
		{
			delete m_pTmpDlg;		
		}
	}
	else
	{
		m_pTmpDlg->ShowWindow(SW_SHOW);
		m_pTmpDlg->SetFocus();
	}
	
	return 0;

EFS_END
return -1;
}

// ** 통계 DM 정보 생성
INT CCOOPERATIVE_STATISTICS_MAIN::MakeDMInfo()
{
EFS_BEGIN

	m_pSInfoMgr = NULL;
	m_pSInfoMgr = new CStaticsInfoMgr(this, COOPERATIVE_STATICS);
	m_pSInfoMgr->MakeDMInfo();

	m_pStaticsMainInfoDM = m_pSInfoMgr->m_pStaticsMainInfoDM;

	return 0;

EFS_END
return -1;
}

// ** 화면초기화
INT CCOOPERATIVE_STATISTICS_MAIN::InitScreen()
{
EFS_BEGIN

	INT ids;

	// 통계종류 콤보박스에 구성
	ids = MakeStaticsClass();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	// 통계종류에따른 화면구성
	ChangeScreen();

	return 0;

EFS_END
return -1;
}

// ** 통계종류 콤보박스에 구성
INT CCOOPERATIVE_STATISTICS_MAIN::MakeStaticsClass()
{
EFS_BEGIN

	INT ids;

	CComboBox *pCombo = ((CComboBox*)GetDlgItem(IDC_cmbSTATICS_CLASS));

	CString sJobClass , sStaticsClass;
	for( INT i = 0 ; i < m_pStaticsMainInfoDM->GetRowCount() ; i++ )
	{
		ids = m_pStaticsMainInfoDM->GetCellData( _T("통계종류") , i , sStaticsClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeStaticsClass") );

		pCombo->AddString( sStaticsClass );
	}

	pCombo->SetCurSel(0);

	return 0;

EFS_END
return -1;
}

// ** 통계종류에따른 화면구성
INT CCOOPERATIVE_STATISTICS_MAIN::ChangeScreen()
{
EFS_BEGIN

	INT ids;

	CString sStaticsClass;
	
	ids = GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN"), _T("통계종류"), sStaticsClass, -1, 0 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeScreen") );
	
	/*
	if ( sStaticsClass==_T("자료실별이용통계") || sStaticsClass==_T("신규구입도서이용통계") ) 
	{
		((CStatic*) GetDlgItem (IDC_stc_UNIT))->SetWindowText(_T("단위:대출이력이있는책(전체책)"));
	}
	else
	{
		((CStatic*) GetDlgItem (IDC_stc_UNIT))->SetWindowText(_T("단위:종(책)"));
	}
	*/
	
	return 0;

EFS_END
return -1;
}

// ** 통계화면(STATISTICS_TEMP)에서 사용될 검색조건과 미리보기 차일드초기화
VOID CCOOPERATIVE_STATISTICS_MAIN::InitEtcDialog()
{
EFS_BEGIN

	// 포인터 초기화
	m_pSearchDlg = NULL;
	m_pPreviewDlg = NULL;

	// 검색조건 화면 초기화
	m_pSearchDlg = new CCOOPERATIVE_STATISTICS_WHERE(this, m_pSInfoMgr);
	m_pSearchDlg->pMain = this->pMain;
	m_pSearchDlg->pParentMgr = (CESL_Mgr*)this;
	m_pSearchDlg->m_pInfo = this->m_pInfo;

	// 미리보기 화면 초기화
	m_pPreviewDlg = new CCOOPERATIVE_STATISTICS_PREVIEW(this, m_pSInfoMgr);
	m_pPreviewDlg->pMain = this->pMain;
	m_pPreviewDlg->pParentMgr = (CESL_Mgr*)this;
	m_pPreviewDlg->m_pInfo = this->m_pInfo;

EFS_END
}

LRESULT CCOOPERATIVE_STATISTICS_MAIN::OnRunS(WPARAM wParam, LPARAM lParam) 
{
EFS_BEGIN
		
	INT ids;
	
	AfxGetApp()->DoWaitCursor( 1 );
	ids = RunStatics();
	AfxGetApp()->DoWaitCursor( -1 );
	
	if( ids > 0 )
	{
		if( ids == 101 )
			ViewSearchWindow();
		else if( ids == 102 )
			ViewPreviewWindow();
	}	
		
	return 0;
	
EFS_END
return -1;
}

INT CCOOPERATIVE_STATISTICS_MAIN::ViewSearchWindow()
{
	if( m_pSearchDlg->GetSafeHwnd() == NULL )
	{
		m_pSearchDlg->Create(this);
		m_pSearchDlg->ShowWindow(SW_SHOW);
		m_pSearchDlg->UpdateWindow();
		m_pSearchDlg->CenterWindow();
	}
	else
	{
		m_pSearchDlg->ShowWindow(SW_SHOW);
	}

	return 0;
	
}

INT CCOOPERATIVE_STATISTICS_MAIN::ViewPreviewWindow()
{
	if( m_pPreviewDlg->GetSafeHwnd() == NULL )
	{
		m_pPreviewDlg->Create(this);
		m_pPreviewDlg->ShowWindow(SW_SHOW);
		m_pPreviewDlg->UpdateWindow();
		m_pPreviewDlg->CenterWindow();
	}
	else
		m_pPreviewDlg->ShowWindow(SW_SHOW);

	return 0;
}

INT CCOOPERATIVE_STATISTICS_MAIN::RunStatics()
{
EFS_BEGIN

	BEGIN_SBL();

	INT ids;
	
	// 통계시 사용될 기초 정보
	CESL_ControlMgr *pCM = FindCM( _T("CM_COOPERATIVE_STATISTICS_MAIN") );
	if ( !pCM ) ERROR_MSG_RETURN_ERROR_CODE2( -41, _T("RunStatics") );

	CString sStaticsClass;
	ids = pCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	CProgressCtrl *pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgSTATICS);
	CProgressCtrl *pTotalProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgSTATICS2);

	pGrid->Clear();
	pGrid->SetRow(1);
	pGrid->SetCol(1);
	
	// 1. 조건 구성
	// 1.5. 조건이 구성 안되어 있으면 머가 안되었다고 알려준다.
	ids = m_pSInfoMgr->MakeStaticsCondition(sStaticsClass);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	if ( ids > 0 ) return ids;

	// 2. DB에서 가져온다.
	ids = m_pSInfoMgr->RunLoanStatics(sStaticsClass,pProgressCtrl,pTotalProgressCtrl);
	//ids = m_pSInfoMgr->RunCooperativeStatics(sStaticsClass,pProgressCtrl,pTotalProgressCtrl);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );

	// 3. 화면에 보여준다.
	ShowGrid();

	INT nRowCount = m_pSInfoMgr->m_pRealResultDM->GetRowCount();
	END_SBL(nRowCount);

	AfxMessageBox( _T("완료되었습니다."));

	m_sLastStat = sStaticsClass;

	return 0;

EFS_END
return -1;
}

VOID CCOOPERATIVE_STATISTICS_MAIN::ShowGrid()
{
EFS_BEGIN

	INT ids;

	CString sStaticsClass;

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);

	ids = GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN") , _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("ShowGrid") );

	bool bDupDel = TRUE;
	bool bZeroShow = TRUE;

	if( ((CButton*)GetDlgItem(IDC_chkDUP_DEL))->GetCheck() == 1 )
		bDupDel = FALSE;
	
	if( ((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->GetCheck() == 1 )
		bZeroShow = FALSE;

	ids = m_pSInfoMgr->StaticsResultViewGrid(sStaticsClass,pGrid,bDupDel , bZeroShow , COOPERATIVE_STATICS);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("ShowGrid") );	

EFS_END
}

void CCOOPERATIVE_STATISTICS_MAIN::OnchkSUMZERODEL() 
{
EFS_BEGIN

	ShowGrid();	

EFS_END
}

VOID CCOOPERATIVE_STATISTICS_MAIN::SetGridVisible()
{
	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN") , _T("통계종류") , sStaticsClass , -1 , 0 );

	if(sStaticsClass == _T("제공횟수통계") || sStaticsClass == _T("신청횟수통계"))
	{
		GetDlgItem(IDC_gridMAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridCNT)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_gridMAIN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_gridCNT)->ShowWindow(SW_HIDE);
	}
}