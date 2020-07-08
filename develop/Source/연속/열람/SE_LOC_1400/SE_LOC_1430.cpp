// SE_LOC_1430.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1430.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1430 dialog


CSE_LOC_1430::CSE_LOC_1430(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1430)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1430::~CSE_LOC_1430()
{
}

VOID CSE_LOC_1430::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1430)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1430, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1430)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnREQUEST, OnbtnREQUEST)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_EN_KILLFOCUS(IDC_edtSEQ_YEAR, OnKillfocusedtSEQYEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1430 message handlers

BOOL CSE_LOC_1430::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSE_LOC_1430::OnClose() 
{
	if( m_pSpfWork != NULL ) 
		delete m_pSpfWork;
	m_pSpfWork = NULL;

	CDialog::OnClose();
}

BOOL CSE_LOC_1430::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1430")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBL_SE_LOC_1400(this);

	INT ids = m_pSpfWork->SelectCheckRegStatus();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	ids = InitControlData();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1430::OnbtnREQUEST() 
{
	RequestRepair();
	OnCancel();
}

VOID CSE_LOC_1430::OnbtnCLOSE() 
{

	OnCancel();

}


INT CSE_LOC_1430::RequestRepair()
{
	INT ids;
	CString sCMAlias = _T("CM_SE_LOC_1430");

	// 1. 입력조건이 다 들어가있는지 검사
	// 책수가 존재하는지 검사.
	CString sBookCnt;
	ids = GetControlData( sCMAlias , _T("책수") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetControlData") );
	
	if( _ttoi( sBookCnt ) == 0 )
	{
		AfxMessageBox( _T("자료를 선택해 주십시오.") );
		return 1;
	}

	// 차수년도 , 차수번호 , 의뢰일자가 들어가 있는지 검사
	const INT nCnt = 3;
	CString sControlAlias[nCnt] = 
	{
		_T("차수년도") , _T("차수번호") , _T("제본의뢰일자") 
	};
	CString sGetData;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sControlAlias[i] , sGetData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetControlData") );
		sGetData.TrimLeft(); sGetData.TrimRight();

		if( sGetData.IsEmpty() )
		{
			CString sEMsg;
			sEMsg.Format( _T("%s를 입력해 주십시오.") , sControlAlias[i] );
			AfxMessageBox( sEMsg );
			return 2;
		}

	}

	// 2. DB 작업 실행
	m_pSpfWork->SPFExcutePath( REQUEST_REPAIR );

	return 0;
}

INT CSE_LOC_1430::InitControlData()
{
	// 1. 차수 년도 생성
	InputTodayYear();

	// 2. 차수 번호 생성
	MakeSeqNo();

	// 3. 책수 입력
	InputBookCnt();

	return 0;
}


INT CSE_LOC_1430::InputTodayYear()
{
	CString sYear;
	sYear.Format(_T("%d") , CLocCommonAPI::GetDBTime(this).GetYear());

	INT ids = SetControlData( _T("CM_SE_LOC_1430") , _T("차수년도") , sYear );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InputTodayYear") );

	return 0;
}

INT CSE_LOC_1430::MakeSeqNo()
{
	INT ids;

	CString sYear;
	CString sSeqNo;

	ids = GetControlData( _T("CM_SE_LOC_1430") , _T("차수년도") , sYear );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSeqNo") );

	ids = m_pSpfWork->MakeSeqNo( sYear , sSeqNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSeqNo") );

	ids = SetControlData( _T("CM_SE_LOC_1430") , _T("차수번호") , sSeqNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeSeqNo") );

	return 0;
}


INT CSE_LOC_1430::InputBookCnt()
{
	CString sBookCnt;
	sBookCnt.Format(_T("%d"),m_pSpfWork->GetSelectMainGridCnt());

	INT ids = SetControlData( _T("CM_SE_LOC_1430") , _T("책수") , sBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InputBookCnt") );

	return 0;
}



VOID CSE_LOC_1430::OnKillfocusedtSEQYEAR() 
{
	// 1. 차수 번호 생성
	MakeSeqNo();	
}

HBRUSH CSE_LOC_1430::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
