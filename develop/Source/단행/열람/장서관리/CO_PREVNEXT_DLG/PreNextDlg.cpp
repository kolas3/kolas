// PreNextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PreNextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreNextDlg dialog


CPreNextDlg::CPreNextDlg(CESL_Mgr* pParent,UINT nIDD)
	: CESL_Mgr(nIDD, pParent)
{
	//{{AFX_DATA_INIT(CPreNextDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPreNextDlg::~CPreNextDlg()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CPreNextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreNextDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreNextDlg, CDialog)
	//{{AFX_MSG_MAP(CPreNextDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreNextDlg message handlers


INT CPreNextDlg::MoveNext()
{
	INT ids;

	/// 현재 위치가 제일 처음인지 검사
	if( m_nLastIndex == m_nCurrentIndex )
	{
		AfxMessageBox( _T("다음 자료가 존재하지 않습니다.") );
		return 0;
	}

	/// 위치 이동
	if( m_nModifyMoveMethod == 0 )
	{
		m_nCurrentIndex++;
	}
	else
	{
		m_nCurrentIndex = m_pParentGrid->SelectGetNext();
	}

	ids = ShowInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MovePrev > ShowInfo") );

	return 0;
}



INT CPreNextDlg::MovePrev()
{
	INT ids;

	/// 현재 위치가 제일 처음인지 검사
	if( m_nFirstIndex == m_nCurrentIndex )
	{
		AfxMessageBox( _T("이전 자료가 존재하지 않습니다.") );
		return 0;
	}

	/// 위치 이동
	if( m_nModifyMoveMethod == 0 )
	{
		m_nCurrentIndex--;
	}
	else
	{
		m_nCurrentIndex = m_pParentGrid->SelectGetPrev();
	}

	ids = ShowInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MovePrev > ShowInfo") );

	return 0;
}



INT CPreNextDlg::ShowInfo()
{
	INT ids;

	if( m_nDlgType == __ECO_NOT_GRID_MODE ) return 0;

	ids = m_pParentGrid->SetReverse(m_nCurrentIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 ,  _T("ShowInfo > SetReverse") );

	return 0;
}


INT CPreNextDlg::InitData()
{
	INT ids;


	/// 그리드가 아닐 경우는 할 필요가 없다.
	if( m_nDlgType == __ECO_NOT_GRID_MODE ) return 0;

	/// 그리드 정보 셋팅
	m_pParentGrid = (CESL_Grid*)FindControl( m_sParentCMAlias , m_sParentGridAlias );
	if( m_pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitData > FindControl") );

	/// DM의 갯수 체크
	CESL_DataMgr* pParentDM = FindDM( m_sParentDMAlias );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitData > FindDM") );

	if( pParentDM->GetRowCount() == 0 )
		return 1;


	m_pParentGrid->SelectMakeList();
	INT nSelectedCnt = m_pParentGrid->SelectGetCount();

	/// 수정 모드 결정
	if( nSelectedCnt > 0 )
		m_nModifyMoveMethod = 1;
	else
		m_nModifyMoveMethod = 0;


	/// 첫번째 인덱스 , 마지막 인덱스 , 현재 선택된 인덱스 셋팅
	if( m_nModifyMoveMethod == 0 )
	{
		m_nLastIndex = pParentDM->GetRowCount()-1;
		m_nFirstIndex = 0;
		

//		if( m_nDlgType == __ECO_MODIFY_TOOLBAR_MODE )
//			m_nCurrentIndex = m_nFirstIndex;	
//		else
		m_nCurrentIndex = m_pParentGrid->GetIdx();

	}
	else
	{
		if( m_nDlgType == __ECO_MODIFY_TOOLBAR_MODE )
			m_nCurrentIndex = m_pParentGrid->SelectGetHeadPosition();
		else
			m_nCurrentIndex = m_pParentGrid->GetIdx();

		m_nLastIndex = m_pParentGrid->SelectGetTailPosition();
		m_nFirstIndex = m_pParentGrid->SelectGetHeadPosition();

		if( m_nDlgType == __ECO_MODIFY_DBCLICK_MODE && m_nFirstIndex != m_nCurrentIndex)
		{
			for( INT i = 0 ; i < nSelectedCnt ; i++ )
			{
				if( m_nCurrentIndex == m_pParentGrid->SelectGetNext() ) break;
			}
		}
		
	}

	/// 화면에 적용한다.
	ids = ShowInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitData > ShowInfo") );

	return 0;

}

BOOL CPreNextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids = InitData();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog > InitData") );

	if( ids == 1 )
	{
		AfxMessageBox( _T("검색을 먼저 해주십시오.") );
		OnCancel();
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CPreNextDlg::SetDlgType( INT nDlgType , CString sParentCMAlias , CString sParentGridAlias , CString sParentDMAlias )
{
	m_nDlgType = nDlgType;
	m_sParentCMAlias = sParentCMAlias;
	m_sParentGridAlias = sParentGridAlias;
	m_sParentDMAlias = sParentDMAlias;

	return 0;
}
HBRUSH CPreNextDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
