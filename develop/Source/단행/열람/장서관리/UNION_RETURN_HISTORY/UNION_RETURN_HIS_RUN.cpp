// UNION_RETURN_HIS_RUN.cpp : implementation file
//

#include "stdafx.h"
#include "UNION_RETURN_HIS_MAIN.h"
#include "UNION_RETURN_HIS_RUN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_HIS_RUN dialog


CUNION_RETURN_HIS_RUN::CUNION_RETURN_HIS_RUN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pEditReturnDateStart	=	NULL;
	m_pEditReturnDateEnd	=	NULL;
	m_nHistoryMode			=	0;
}

CUNION_RETURN_HIS_RUN::~CUNION_RETURN_HIS_RUN()
{
	if( NULL != m_pEditReturnDateStart )
	{
		delete	m_pEditReturnDateStart;
		m_pEditReturnDateStart	=	NULL;
	}
	if( NULL != m_pEditReturnDateEnd )
	{
		delete	m_pEditReturnDateEnd;
		m_pEditReturnDateEnd	=	NULL;
	}
}

void CUNION_RETURN_HIS_RUN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUNION_RETURN_HIS_RUN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUNION_RETURN_HIS_RUN, CDialog)
	//{{AFX_MSG_MAP(CUNION_RETURN_HIS_RUN)
	ON_BN_CLICKED(IDC_btnHISTORY, OnBtnHistory)
	ON_BN_CLICKED(IDC_btnCLOSE, OnBtnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_HIS_RUN message handlers

BOOL CUNION_RETURN_HIS_RUN::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());

	// SM 생성
	if( 0 > InitESL_Mgr(_T("UNION_RETURN_HISTORY_RUN")) )
	{
		AfxMessageBox( _T("InitESL_Mgr ERROR !!") );
		return FALSE;
	}

	// 도서관 정보 초기화
	InitLibList();
	
	// 기본 모드 설정
	CButton* pBtn = (CButton*)GetDlgItem(IDC_radREQUEST_RETURN_HISTORY);
	pBtn->SetCheck(BST_CHECKED);

	// 날짜 컨트롤 설정
	if( NULL == m_pEditReturnDateStart )
	{
		m_pEditReturnDateStart = new CECO_ControlDateEdit(NULL);
		m_pEditReturnDateStart->SubclassDlgItem(IDC_edtDATE_START, this);
		m_pEditReturnDateStart->m_ControlStyle = 0;
		m_pEditReturnDateStart->InitEdit();
	}

	if( NULL == m_pEditReturnDateEnd )
	{
		m_pEditReturnDateEnd = new CECO_ControlDateEdit(NULL);
		m_pEditReturnDateEnd->SubclassDlgItem(IDC_edtDATE_END, this);
		m_pEditReturnDateEnd->m_ControlStyle = 0;
		m_pEditReturnDateEnd->InitEdit();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUNION_RETURN_HIS_RUN::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{			
			OnBtnHistory();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - 이력 조회 조건 값 저장
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 - 이력 종류 , 도서관 선택 , 조회 기간 범위 등을 저장
*/
void CUNION_RETURN_HIS_RUN::OnBtnHistory() 
{
	INT		ids;

	// 반납일 입력값 확인
	CString strReturnDateStart;
	m_pEditReturnDateStart->GetData(strReturnDateStart);
	if( strReturnDateStart.IsEmpty() )
	{
		AfxMessageBox( _T("반납일을 입력해야 합니다.") , MB_ICONINFORMATION );
		return;
	}

	// 이력 종류 - 0 : 타관 반납 이력 1 : 타관 반납 대행 이력
	CButton* pBtn	=	(CButton*)GetDlgItem(IDC_radREQUEST_RETURN_HISTORY);
	if( NULL == pBtn->GetSafeHwnd() )
	{
		return;
	}
	m_nHistoryMode	=	( BST_CHECKED == pBtn->GetCheck() ) ? 0 : 1;
	
	// 선택한 도서관 정보 저장
	ids = GetControlData( _T("CM_UNION_RETURN_HISTORY_RUN") , _T("도서관") , m_strLibCode , -1 , 0 );
	if( 0 > ids )
	{
		return;
	}
	
	ids = GetControlData( _T("CM_UNION_RETURN_HISTORY_RUN") , _T("도서관") , m_strLibName , -1 , 1 );
	if( 0 > ids )
	{
		return;
	}
		
	// 조회 기간 범위
	m_pEditReturnDateStart->GetData(m_strReturnDateStart);
	m_pEditReturnDateEnd->GetData(m_strReturnDateEnd);

	if( m_strReturnDateEnd.IsEmpty() )
	{// 입력을 안 했을 경우, 검색 당일 날짜까지 검색
		SYSTEMTIME time;
		GetLocalTime(&time);

		m_strReturnDateEnd.Format( _T("%04d/%02d/%02d") , time.wYear , time.wMonth , time.wDay );
	}

	OnOK();
}
void CUNION_RETURN_HIS_RUN::OnBtnClose() 
{
	OnCancel();
}

/*
@Create
 - 2009.02.25 BY KSJ
@Description
 - 도서관 리스트 초기화
@Parameter
 - 
@Return Value
 - 0 : 성공
@Mini Spec
 1. 센터에서 도서관 리스트를 가져온다
 2. ComboBox에 도서관부호와 도서관명 Set
*/
INT CUNION_RETURN_HIS_RUN::InitLibList()
{
	INT ids = 0;

	CESL_DataMgr*	pLibraryDM = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	if( NULL == pLibraryDM )
	{
		return -1;
	}
	
	CESL_ControlMultiComboBox* cmbLibCode;
	cmbLibCode = ((CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbLIB_LIST));
	if( NULL == cmbLibCode->GetSafeHwnd() )
	{
		return -2;
	}

	CString strLibCode;
	CString strLibName;

	cmbLibCode->AddItem( _T("     "), 0, 0);
	cmbLibCode->AddItem( _T("적용안함"), 1, 0);

	INT i , nRowCnt;
	nRowCnt		=	pLibraryDM->GetRowCount();
	// 2009.08.17 UPDATE BY KSJ : 도서관정보 1개가 표시 안 되는 문제로 수정
//DEL 	for ( i=1 ; i<nRowCnt ; i++ )
//DEL 	{
//DEL 		pLibraryDM->GetCellData(_T("도서관코드"), i, strLibCode );
//DEL 		pLibraryDM->GetCellData(_T("도서관명"), i, strLibName );
//DEL 		cmbLibCode->AddItem(strLibCode, 0, i);
//DEL 		cmbLibCode->AddItem(strLibName, 1, i);			
//DEL 	}
	for ( i=1 ; i<nRowCnt+1 ; i++ )
	{
		pLibraryDM->GetCellData( _T("도서관코드") , i-1, strLibCode );
		pLibraryDM->GetCellData( _T("도서관명") , i-1 , strLibName );
		cmbLibCode->AddItem( strLibCode , 0 , i );
		cmbLibCode->AddItem( strLibName , 1 , i );			
	}

	cmbLibCode->SetCurSel(0);
	
	return 0;
}
