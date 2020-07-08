// DonatorNameGetter_Search.cpp : implementation file
//

#include "stdafx.h"
#include "DonatorNameGetter_Search.h"
#include "../K2UP_Common/MakeSearchData.h"
#include "../K2UP_Common/MulitComboBoxManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetter_Search dialog


CDonatorNameGetter_Search::CDonatorNameGetter_Search(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonatorNameGetter_Search::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorNameGetter_Search)
	m_nSearchType = -1;
	m_bDataSortYN = FALSE;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_bCreate = FALSE;
	pMakeSearchData = new CMakeSearchData(pParent); 
	m_pSortItem = NULL;
}

CDonatorNameGetter_Search::~CDonatorNameGetter_Search()
{
	if (pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL; 
	}
}


VOID CDonatorNameGetter_Search::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorNameGetter_Search)
	DDX_Radio(pDX, IDC_radID, m_nSearchType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorNameGetter_Search, CDialog)
	//{{AFX_MSG_MAP(CDonatorNameGetter_Search)
	ON_BN_CLICKED(IDC_radID, OnradID)
	ON_BN_CLICKED(IDC_radNAME, OnradNAME)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetter_Search message handlers


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - _T("기증자 검색창")을 초기화 한다.
//
// [ 미니스펙 ]
// 1. SM/CM/DM 설정
// 2. 검색타입에 따라 리소스를 표시한다.
//		- 기본값은 0 : ID로 검색이다.
// 3. 설정된 OpenMode가 0보다 작으면 정렬부분에 리소스를 Disable 시킨다.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CDonatorNameGetter_Search::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//=======================================================================
	// 1. SM/CM/DM 설정
	//=======================================================================
	if( InitESL_Mgr(_T("K2UPGRADE_기증자검색")) < 0 )
	{
		ESLAfxMessageBox( _T("InitESL_Mgr Error : K2UPGRADE_기증자검색") );
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_기증자검색"));
	m_pDM = FindDM(_T("DM_기증자관리"));
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox( _T("[CM_기증자검색]을 설정할 수 없습니다.") );
		return FALSE;
	}
	if( m_pDM == NULL )
	{
		ESLAfxMessageBox( _T("[DM_기증자관리]을 설정할 수 없습니다.") );
		return FALSE;
	}

	
	//=======================================================================
	// 2. 검색타입에 따라 리소스를 표시한다.
	//		- 기본값은 0 : ID로 검색이다.
	//=======================================================================
	m_nSearchType = 0;
	UpdateData(FALSE);
	SetConditionContrl();

	//=======================================================================
	// 3. 설정된 OpenMode가 0보다 작으면 정렬부분에 리소스를 Disable 시킨다.
	//=======================================================================
	if(m_nOpenMode < 0)
	{
		GetDlgItem(IDC_chkSORT_YN)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSET_SORT_CONDITION)->EnableWindow(FALSE);
	}

	//기증자구분 부호관리에서 할 수 있도록 추가
	/*
	CString arrDonatorType[2] = {_T("PN"),_T("GR")};	
	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("PN"), arrDonatorType, _T("기증자구분"), 2);
	*/

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 검색타입에 따라 리소스를 표시한다.
//
// [ 미니스펙 ]
// 1. 검색타입에 따라 리소스의 표시여부를 설정한다.
//		- 0 : ID로 검색
//		- 1 : 이름으로 검색
// 2. 설정된 리소스의 표시여부로 화면에 표시한다.
//
/////////////////////////////////////////////////////////////////////////////
VOID CDonatorNameGetter_Search::SetConditionContrl()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	CWnd *pWnd;
	INT i;
	UINT wndID[ 6 ] = { IDC_cmbDONATOR_CLASS, 
						IDC_edtDONATOR_REG_YEAR, 
						IDC_edtDONATOR_SERIALNO1, 
						IDC_edtDONATOR_SERIALNO2, 
						IDC_edtDONATOR_NAME1, 
						IDC_edtDONATOR_NAME2 };
	bool wndOption[ 6 ] = { false, false, false, false, false, false };
	
	UpdateData(TRUE);
	//=======================================================================
	// 1. 검색타입에 따라 리소스의 표시여부를 설정한다.
	//		- 0 : ID로 검색
	//		- 1 : 이름으로 검색
	//=======================================================================
	switch( m_nSearchType )
	{
		case 0 :
		{
			for( i=0; i<4; i++ )	wndOption[ i ] = true;
		}
			break;
		case 1 :
		{
			for( i=4; i<6; i++ )	wndOption[ i ] = true;
		}
			break;
	default :		return;
			break;
	}


	//=======================================================================
	// 2. 설정된 리소스의 표시여부로 화면에 표시한다.
	//=======================================================================
	for( i=0; i<6; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->EnableWindow( wndOption[ i ] );
	}
	//=====================================================
	//2009.01.13 ADD BY PJW : 기증자 구분코드는 선택을 하수없도록 한다.
	//2009.02.03 REM BY PJW : 기증자 구분코드는 MANAGE_CODE 와 상관없어서 주석처리함
// 	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_cmbDONATOR_CLASS);
// 	pEdit->EnableWindow(FALSE);
	//=====================================================
}

VOID CDonatorNameGetter_Search::OnradID() 
{
	SetConditionContrl();
}

VOID CDonatorNameGetter_Search::OnradNAME() 
{
	SetConditionContrl();
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 조건에 해당하는 기증자를 검색한다.
//
// [ 미니스펙 ]
//
/////////////////////////////////////////////////////////////////////////////
VOID CDonatorNameGetter_Search::OnOK() 
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids, nTmpData = -1;
	CString strQuery, strOption, strTmpData, strTmpData1, strData;	

	
	//=======================================================================
	// 1. 검색타입에 따라 쿼리문을 작성한다.
	//		- 0 : ID로 검색
	//		- 1 : 이름으로 검색
	//=======================================================================	
	switch( m_nSearchType )
	{
	case 0 :	// ID 검색
		{
			// 기증자구분 쿼리추가
			m_pCM->GetControlMgrData(_T("기증자구분"), strTmpData);
			if( strTmpData.IsEmpty() == FALSE )
			{
				strOption.Format(_T("DONATOR_FLAG = '%s' AND "), strTmpData);			
				strQuery += strOption;
			}				
			
			// 등록연도 쿼리 추가
			strOption.Empty();		strTmpData.Empty();
			m_pCM->GetControlMgrData(_T("등록연도"), strTmpData);
			if( strTmpData.IsEmpty() == FALSE )		
			{
				strOption.Format(_T("YEAR = '%s' AND "), strTmpData);
				strQuery += strOption;
			}
			
			// 일련번호 쿼리추가
			strOption.Empty();		strTmpData.Empty();		strTmpData1.Empty();
			m_pCM->GetControlMgrData(_T("일련번호1"), strTmpData);
			m_pCM->GetControlMgrData(_T("일련번호2"), strTmpData1);
			if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE )
			{
				strOption.Format(_T("SERIAL_NUM BETWEEN %s AND %s AND "), strTmpData, strTmpData1);
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == TRUE )
			{
				strOption.Format(_T("SERIAL_NUM = %s AND "), strTmpData);
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == TRUE && strTmpData1.IsEmpty() == FALSE )
			{
				ESLAfxMessageBox(_T("시작 일련번호를 설정해 주십시요."));
				return;	
			}
			else
			{
				
			}
		}
		break;

	case 1 :	// 이름검색
		{
			m_pCM->GetControlMgrData(_T("기증자명1"), strTmpData);
			m_pCM->GetControlMgrData(_T("기증자명2"), strTmpData1);
			if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE && strTmpData1 == strTmpData )
			{
				pMakeSearchData->GetKeyWord(strTmpData);
				strOption.Format( _T("INDEX_DONATOR = '%s' AND "), strTmpData );
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE && strTmpData1 != strTmpData )
			{
				pMakeSearchData->GetKeyWord(strTmpData);
				pMakeSearchData->GetKeyWord(strTmpData1);
				strOption.Format( _T("INDEX_DONATOR BETWEEN '%s' AND '%s' AND "), strTmpData, strTmpData1 );
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == TRUE )
			{
				pMakeSearchData->GetKeyWord(strTmpData);
				strOption.Format( _T("INDEX_DONATOR BETWEEN '%s' AND '%s힝' AND "), strTmpData, strTmpData );
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == TRUE && strTmpData1.IsEmpty() == FALSE )
			{
				ESLAfxMessageBox(_T("시작 기증자명을 입력해 주십시요."));
				return ;
			}
			else ; // 쿼리문 추가하지 않음

		}
			break;

	default :	return;
			break;
	}
	

	//=======================================================================
	// 2. 1번에서 검색한 쿼리문에 등록일자 쿼리문을 조건절에 추가한다.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("등록일1"), strTmpData);
	m_pCM->GetControlMgrData(_T("등록일2"), strTmpData1);
	//===============================================================
	//20080522 UPDATE BY PJW : 날짜 검색 수정.
	/*
	if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE && strTmpData1 == strTmpData )
	{
		strOption.Format(_T("INPUT_DATE = '%s' "), strTmpData );
		strQuery += strOption;
	}
	else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE && strTmpData1 != strTmpData )
	{
		strOption.Format( _T("INPUT_DATE BETWEEN '%s' AND '%s' "), strTmpData, strTmpData1 );
		strQuery += strOption;
	}
	else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == TRUE )
	{
		strOption.Format( _T("INPUT_DATE = '%s' "), strTmpData );
		strQuery += strOption;
	}
	else if( strTmpData.IsEmpty() == TRUE && strTmpData1.IsEmpty() ==FALSE )
	{
		ESLAfxMessageBox(_T("시작 등록일을 입력해 주십시요."));
		return ;
	}
	*/
	if( !strTmpData.IsEmpty() && !strTmpData1.IsEmpty() )
	{
		strOption.Format(_T("INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')+0.99999 "), strTmpData, strTmpData1 );		
		strQuery += strOption;
	}
	else if( !strTmpData.IsEmpty() )
	{
		strOption.Format(_T("INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')+0.99999 "), strTmpData, strTmpData );
		strQuery += strOption;
	}
	else if( !strTmpData1.IsEmpty() )
	{
		strOption.Format(_T("INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')+0.99999 "), strTmpData1, strTmpData1 );
		strQuery += strOption;
	}	
	//===============================================================

	if( _T("AND ") == strQuery.Right( 4 ) )
		strQuery = strQuery.Left( strQuery.GetLength() - 4 );

	//=====================================================
	//2009.01.12 ADD BY PJW : 기증자 검색할경우 MANAGE_CODE를 조건에 넣는다.
	//2010.05.24 ADD BY PJW : 김지연대리 요구사항으로 기증자검색은 MANAGE_CODE제거함
//	if( strQuery.IsEmpty() )
//	{
//		strQuery = _T("MANAGE_CODE = UDF_MANAGE_CODE");
//	}
//	else
//	{
//		strQuery += _T(" AND MANAGE_CODE = UDF_MANAGE_CODE");
//	}
	//=====================================================
	

	if( m_nOpenMode < 0 )
	{
		ids = m_pDM->RefreshDataManager(strQuery);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("기증자정보를 검색할 수 없습니다."));
			return ;
		}

		//===================================================
		//2009.03.09 ADD BY PJW : 연속책수와 당행책수의 합을 UDF_책수에 넣는다.
		INT nDmRowCnt;
		INT nSumBookCnt;
		CString strSumBookCnt;
		nDmRowCnt = m_pDM->GetRowCount();
		for ( INT i = 0 ; i < nDmRowCnt ; i++ )
		{
			nSumBookCnt = _ttoi(m_pDM->GetCellData(_T("단행책수"), i ));
			nSumBookCnt += _ttoi(m_pDM->GetCellData(_T("연속책수"), i ));
			strSumBookCnt.Format( _T("%d"), nSumBookCnt );
			m_pDM->SetCellData(_T("UDF_책수"),strSumBookCnt, i );
		}
		//===================================================

		CDialog::OnOK();
	}
	else
	{
		m_strQuery = strQuery;
		::PostMessage(pParentMgr->GetSafeHwnd(), DONATOR_QUERY_CREATE, 0, 0);
	}
}

BOOL CDonatorNameGetter_Search::Create(CWnd *pParenWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParenWnd);
}

BOOL CDonatorNameGetter_Search::GetCreate()
{
	return m_bCreate;
}

CString CDonatorNameGetter_Search::GetWhereStmt()
{
	return m_strQuery;
}

BOOL CDonatorNameGetter_Search::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDCANCEL)->m_hWnd != pMsg->hwnd)
		{
			GetDlgItem(IDOK)->SetFocus();
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		}
		else
		{
			GetDlgItem(IDCANCEL)->SetFocus();
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		}
		return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CDonatorNameGetter_Search::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	if(m_pSortItem == NULL) return;
	CDataSortDlg dlg(this);
	dlg.SetSortItemSet(m_pSortItem);
	dlg.SetOpenMode(-1);
	dlg.DoModal();	
}

VOID CDonatorNameGetter_Search::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton*)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;
}

HBRUSH CDonatorNameGetter_Search::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
