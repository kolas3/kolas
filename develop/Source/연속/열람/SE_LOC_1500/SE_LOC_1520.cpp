// SE_LOC_1520.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1520.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1520 dialog


CSE_LOC_1520::CSE_LOC_1520(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1520)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1520::~CSE_LOC_1520()
{
}

VOID CSE_LOC_1520::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1520)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1520, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1520)
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabOPTION, OnSelchangetabOPTION)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnRESTORE, OnbtnRESTORE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1520 message handlers

BOOL CSE_LOC_1520::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSE_LOC_1520::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

BOOL CSE_LOC_1520::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1520")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pSpfWork = new CBL_SE_LOC_15(this);

	InitControl();

	DisplayDefaultValue();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSE_LOC_1520::InitControl()
{
	// 탭 콘트롤 초기화
	InitTabCtrl();

	// 탭 콘트롤에 따라 화면 초기화
	InitScreenShape();

	// 버튼 초기화
	InitButtonName();

	return 0;
}


INT CSE_LOC_1520::InitTabCtrl()
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabOPTION);

	pTab->InsertItem( 0 , _T("데이터 복귀") );
	pTab->InsertItem( 1 , _T("복귀 취소")  );

	pTab->SetCurSel(0);

	return 0;
}

INT CSE_LOC_1520::InitScreenShape()
{

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabOPTION);

	const INT nVisibleCnt = 13;
	INT nVisibleIDC[nVisibleCnt] = 
	{
		IDC_staINFO , IDC_staCOPYSPECIES , IDC_edtREPAIR_SPECIES , IDC_staCOPYBOOK , IDC_edtREPAIR_BOOK , 
		IDC_staRESTORE_DATE ,IDC_staRESTORE_DATE2 , IDC_datRESTORE , IDC_staRESTORE_DATE3 , IDC_staSHELFDATE , 
		IDC_stsWORKER , IDC_datSHELF_DATE2 , IDC_edtSHELF_WORKER
	};

	for( INT i = 0 ; i < nVisibleCnt ; i++ )
	{
		GetDlgItem(nVisibleIDC[i])->ShowWindow(SW_SHOW);
	}



	if( pTab->GetCurSel() == 0 )
	{
		const INT nUnVisibleCnt = 5;
		INT nUnVisibleIDC[nVisibleCnt] = 
		{
			IDC_staRESTORE_DATE3 , IDC_staSHELFDATE , IDC_stsWORKER , IDC_datSHELF_DATE2 , IDC_edtSHELF_WORKER
		};

		for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
		{
			GetDlgItem(nUnVisibleIDC[i])->ShowWindow(SW_HIDE);
		}

	}	
	else 
	{
		const INT nUnVisibleCnt = 3;
		INT nUnVisibleIDC[nVisibleCnt] = 
		{
			IDC_staRESTORE_DATE ,IDC_staRESTORE_DATE2 , IDC_datRESTORE 
		};

		for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
		{
			GetDlgItem(nUnVisibleIDC[i])->ShowWindow(SW_HIDE);
		}

	}
		


	return 0;
}

INT CSE_LOC_1520::InitButtonName()
{

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabOPTION);

	CString sButtonName;
	if( pTab->GetCurSel() == 0 )
		sButtonName = _T("데이터복귀");
	else 
		sButtonName = _T("복귀취소");


	GetDlgItem(IDC_btnRESTORE)->SetWindowText( sButtonName );

	return 0;
}

VOID CSE_LOC_1520::OnSelchangetabOPTION(NMHDR* pNMHDR, LRESULT* pResult) 
{
	InitScreenShape();
	InitButtonName();
	
	*pResult = 0;
}

VOID CSE_LOC_1520::OnbtnCLOSE() 
{
	OnCancel();	
}

VOID CSE_LOC_1520::OnbtnRESTORE() 
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabOPTION);

	CString sPathAlias;
	if( pTab->GetCurSel() == 0 )
		sPathAlias = RESTORE_DATA;
	else 
		sPathAlias = CANCEL_RESTORE;

	m_pSpfWork->SPFExcutePath( sPathAlias );
	
}


/// 화면의 종,책 배가 담당자를 보여준다.
INT CSE_LOC_1520::DisplayDefaultValue()
{
	INT ids;

	CString sMainDMAlias = _T("DM_SE_LOC_1500");

	CESL_DataMgr *pMainDM = FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayDefaultValue") );

	CESL_Grid *pMainGrid = (CESL_Grid*)FindControl(_T("CM_SE_LOC_1500"),_T("메인그리드"));
	if( pMainGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayDefaultValue") );

	CString sSpeciesCnt,sBookCnt;
	ids = CLocCommonAPI::GetDMEqualRemoveCnt( this  , pMainDM , _T("종정보KEY") , pMainGrid , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DisplayDefaultValue") ); 

	pMainGrid->SelectMakeList();

	sBookCnt.Format(_T("%d") , pMainGrid->SelectGetCount() );

	CString sCMAlias = _T("CM_SE_LOC_1520");
	ids = SetControlData( sCMAlias , _T("종수") , sSpeciesCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DisplayDefaultValue") ); 

	ids = SetControlData( sCMAlias , _T("책수") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DisplayDefaultValue") ); 

	// 작업자
	/*
	TCHAR sUserName[200];
	DWORD nUserLength = 200;
	GetUserName(sUserName,&nUserLength);
	*/

	CString sUserName = GetUserID();

	ids = SetControlData( sCMAlias , _T("배가담당자") , sUserName  );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DisplayDefaultValue") ); 

	return 0;
}

HBRUSH CSE_LOC_1520::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
