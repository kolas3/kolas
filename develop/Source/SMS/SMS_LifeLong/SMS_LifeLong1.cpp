// SMS_LifeLong1.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_LifeLong1.h"
#include "SMS_LifeLong_Sender.h"
#include "..\\SMS_06\\SMS_061.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong dialog


CSMS_LifeLong::CSMS_LifeLong(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_LifeLong)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nDisplayMode = 0;
	m_sSMSType = _T("");
	m_sDMAlias = _T("DM_SMS_LifeLong");
	m_sCMAlias = _T("MainGrid");	
}

CSMS_LifeLong::~CSMS_LifeLong()
{
	FreeAllDM();
}

VOID CSMS_LifeLong::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_LifeLong)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_LifeLong, CDialog)
	//{{AFX_MSG_MAP(CSMS_LifeLong)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong message handlers

BOOL CSMS_LifeLong::Create(CWnd* pParentWnd) 
{
EFS_BEGIN
	
	return CDialog::Create(IDD, pParentWnd);
EFS_END
	return -1;
}

VOID CSMS_LifeLong::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	/////////////////////////////////////////////////////////////////
	// 1. 회원용 그리드	
	/////////////////////////////////////////////////////////////////
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gMAIN_GRID);
	if( pGrid->GetSafeHwnd() == NULL )
		return;
	//pGrid->MoveWindow(0, 0, cx+4, cy+4);
	pGrid->MoveWindow(0, 0, cx, cy);
	
	/////////////////////////////////////////////////////////////////
	// 2. 비회원용 그리드	
	/////////////////////////////////////////////////////////////////
	pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gMAIN_GRID2);
	if( pGrid->GetSafeHwnd() == NULL )
		return;
	//pGrid->MoveWindow(0, 0, cx+4, cy+4);
	pGrid->MoveWindow(0, 0, cx, cy);

	/////////////////////////////////////////////////////////////////
	// 3. 회원용 그리드	
	/////////////////////////////////////////////////////////////////
	pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gMAIN_GRID3);
	if( pGrid->GetSafeHwnd() == NULL )
		return;
	//pGrid->MoveWindow(0, 0, cx+4, cy+4);
	pGrid->MoveWindow(0, 0, cx, cy);

	EFS_END	
}

////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 폼 초기화 및 기본정보 초기화
////////////////////////////////////////////////////////////////////////////////////
BOOL CSMS_LifeLong::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//////////////////////////////////////////////////////////////////
	// 1. SM 초기화
	//////////////////////////////////////////////////////////////////
	if( InitESL_Mgr( _T("SMS_DLG_LifeLong") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_LifeLong") );
		return false;
	}

	//////////////////////////////////////////////////////////////////
	// 2. 메인 그리드 초기화
	//////////////////////////////////////////////////////////////////
	InitDisplayGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 메인 그리드 결정
////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::InitDisplayGrid()
{
	if (m_nDisplayMode == 0)		// 프로그램&&회원
	{
		GetDlgItem(IDC_gMAIN_GRID)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID2)->ShowWindow(FALSE);		
		GetDlgItem(IDC_gMAIN_GRID3)->ShowWindow(FALSE);
	}
	else if (m_nDisplayMode == 1)	// 비회원
	{
		GetDlgItem(IDC_gMAIN_GRID)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID2)->ShowWindow(TRUE);		
		GetDlgItem(IDC_gMAIN_GRID3)->ShowWindow(FALSE);
	}	
	else if (m_nDisplayMode == 2)	// 회원
	{
		GetDlgItem(IDC_gMAIN_GRID)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID2)->ShowWindow(FALSE);		
		GetDlgItem(IDC_gMAIN_GRID3)->ShowWindow(TRUE);
	}	

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 모드를 SET 
// 모드는 검색 방식과 그리드에 영향을 끼친다.
////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::SetDisplayMode(INT nMode)
{
	if ( m_nDisplayMode != nMode )
	{
		FreeAllDM();	
	}

	m_nDisplayMode = nMode;

	if (m_nDisplayMode == 0)
	{
		m_sDMAlias = _T("DM_SMS_LifeLong");
		m_sCMAlias = _T("MainGrid");
	}
	else if (m_nDisplayMode == 1)
	{
		m_sDMAlias = _T("DM_SMS_LifeLong_NonMember");
		m_sCMAlias = _T("NonMemberGrid");
	}
	else if (m_nDisplayMode == 2)
	{
		m_sDMAlias = _T("DM_SMS_LifeLong_Member");
		m_sCMAlias = _T("MemberGrid");
	}

	InitDisplayGrid();
	ControlDisplay( _T("CM_SMS_LifeLong"), m_sCMAlias );

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 검색결과 DM 초기화
////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::FreeAllDM()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong") );
	pDM->FreeData();
	
	pDM = FindDM( _T("DM_SMS_LifeLong_NonMember") );
	pDM->FreeData();

	pDM = FindDM( _T("DM_SMS_LifeLong_Member") );
	pDM->FreeData();

	return 0;
	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 검색창에서 생성된 WHERE를 사용하여 회원을 검색한다.
/////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::SearchUSER(CString sQuery)
{
	EFS_BEGIN

	INT ids=0, cnt=0;

	//////////////////////////////////////////////////////////////////
	// 1. 회원 DM을 초기화 한다.
	//////////////////////////////////////////////////////////////////
	CESL_DataMgr *pDM = FindDM( m_sDMAlias );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong::SearchUSER") );
	pDM->FreeData();

	ids = pDM->RefreshDataManager(sQuery);
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong::SearchUSER") );
	
	cnt = pDM->GetRowCount();
	if(cnt < 1)		
	{
		AfxMessageBox(_T("검색 결과가 없습니다."));
		ControlDisplay( _T("CM_SMS_LifeLong"), m_sCMAlias );
		return 0;
	}

	//////////////////////////////////////////////////////////////////
	// 2. 회원 주소를 입력 한다.
	//    비회원 모드 예외
	//////////////////////////////////////////////////////////////////
	for (INT i = 0;i < cnt;i++)
	{
		if (m_nDisplayMode == 1) break;
		ids = ViewAddress(i);
		if(ids<0) return ids;
	}

	ControlDisplay( _T("CM_SMS_LifeLong"), m_sCMAlias );	
	AfxMessageBox(_T("검색이 완료 되었습니다"));

	return 0;
	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 주소와 상세주소 정보를 합쳐서 그리드용 회원주소를 만든다.
/////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::ViewAddress(INT nRow)
{
	EFS_BEGIN
	
	CESL_DataMgr *pDM = FindDM( m_sDMAlias );
	if(pDM==NULL) if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong::ViewAddress") );

	CString Address=_T(""), sTemp=_T("");
	
	pDM->GetCellData(_T("주소"), nRow, sTemp);
	Address = sTemp + _T(" ");
	pDM->GetCellData(_T("상세주소"), nRow, sTemp);
	Address += sTemp;
	pDM->SetCellData(_T("회원주소"), Address, nRow);

	return 0;
	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 선택된 대상자의 목록을 만들고 발송한다.
/////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::SendSMS()
{
EFS_BEGIN
	
	INT ids;
	INT nCnt = 0, nRowCnt = 0;
	/////////////////////////////////////////////////////////////////////////
	// 1. 현재 DM을 찾는다.
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr *pDMSrc = FindDM( m_sDMAlias );
	if(pDMSrc==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong::SendSMS") );

	nRowCnt = pDMSrc->GetRowCount();
	if(nRowCnt < 1){
		AfxMessageBox(_T("메일발송 대상자가 없습니다"));
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// 2. 선정된 대상자를 확인한다.
	/////////////////////////////////////////////////////////////////////////
	CESL_ControlMgr *pCM = FindCM(_T("CM_SMS_LifeLong"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong::SendSMS") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( m_sCMAlias ));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong::SendSMS") );
	
	pGrid->SelectMakeList();
	nCnt = pGrid->SelectGetCount();
	if(nCnt < 1){
		AfxMessageBox(_T("선정된 대상자가 없습니다."));
		return 0;
	}
	
	/////////////////////////////////////////////////////////////////////////
	// 3. 선정된 대상자만 발송용 DM으로 복사한다.
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDMSend = new CESL_DataMgr;
	ids = CLocCommonAPI::MakeCopyDM( this , pDMSrc , pDMSend );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSMS_LifeLong::SendSMS") );

	pDMSend->FreeData();
	nCnt = 0;

	CString sTemp =_T("");
	for(INT i=0;i<nRowCnt;i++){
		sTemp = pGrid->GetTextMatrix(i+1, 1);
		if(sTemp == _T("V")){
			pDMSend->InsertRow(-1);
			ids = CLocCommonAPI::AddDM(pDMSrc, pDMSend, i, nCnt, this );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSMS_LifeLong::SendSMS") );
			nCnt++;
		}		
	}

	if(nCnt < 1){
		AfxMessageBox(_T("선정된 대상자가 없습니다."));
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// 4. 발송 다이얼로그를 띄운다.
	/////////////////////////////////////////////////////////////////////////
	CSMS_LifeLong_Sender SenderDlg(this);
	SenderDlg.SetInfo(pDMSend);
	SenderDlg.DoModal();
	
	delete pDMSend;
	return 0;
EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 단일 발송
/////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::SendOneSMS()
{
	CSMS_06 dlg(this);
	dlg.DoModal();
	return 0;
}	
