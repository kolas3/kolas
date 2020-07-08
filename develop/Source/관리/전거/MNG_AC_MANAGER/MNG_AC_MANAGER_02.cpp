// MNG_AC_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_AC_MANAGER_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_02 dialog


CMNG_AC_MANAGER_02::CMNG_AC_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_AC_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	m_pDM = NULL;
	//}}AFX_DATA_INIT
}

CMNG_AC_MANAGER_02::~CMNG_AC_MANAGER_02()
{
}

BOOL CMNG_AC_MANAGER_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_AC_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_AC_MANAGER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_AC_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_AC_MANAGER_02)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_02 message handlers

BOOL CMNG_AC_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if( InitESL_Mgr( _T("MNG_AC_MANAGER_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_AC_MANAGER_02") );
		return FALSE;
	}

	if(m_pDM  != NULL) m_pDM = NULL;
	m_pDM = FindDM(_T("DM_MNG_AC_MANAGER_01_MASTER"));
	if(m_pDM == NULL)
	{
		AfxMessageBox( _T("Not Found : DM_MNG_AC_MANAGER_01") );
		return FALSE;
	}

	//전거 종류 콤보박스 셋팅
	SetAcCombo();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	
}

VOID CMNG_AC_MANAGER_02::OnOK() 
{
EFS_BEGIN
	//저장시
	if  ( !ISCheckInputValue() ) 
		return;

	OnSearchAC();
	// CDialog::OnOK();

EFS_END
}

VOID CMNG_AC_MANAGER_02::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


VOID CMNG_AC_MANAGER_02::SetAcCombo()
{
EFS_BEGIN
	
	m_pMCombo = new CJComboBox;
	m_pMCombo->SubclassDlgItem( IDC_cACKind, this );
	m_pMCombo->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMCombo->RequireFlag = FALSE;
	
	INT defaultCel = 0;
	CString code, desc;
	
	m_pMCombo->AddItem( _T("저자명"), 0, 0 );
	m_pMCombo->AddItem( _T("서명"), 0, 1 );
	m_pMCombo->AddItem( _T("주제명"), 0, 2 );
	
	m_pMCombo->SetCurSel( 0 );

	((CEdit*)GetDlgItem(IDC_eControlNoStart))->SetFocus();
	
	EFS_END
}

BOOL CMNG_AC_MANAGER_02::ISCheckInputValue()
{
EFS_BEGIN
	// 검색조건 체크
	CString sACConStart = _T("");
	CString sACConEnd = _T("");

	CString sACSearchStart = _T("");
	CString sACSearchEnd = _T("");

	GetDlgItem(IDC_eControlNoStart)->GetWindowText(sACConStart);
	GetDlgItem(IDC_eControlNoEnd)->GetWindowText(sACConEnd);
	
	GetDlgItem(IDC_eACSearchStart)->GetWindowText(sACSearchStart);
	GetDlgItem(IDC_eACSearchEnd)->GetWindowText(sACSearchEnd);
	
	if ( sACConStart == _T("") && sACConEnd == _T("") && sACSearchStart == _T("") && sACSearchEnd  == _T("") )
	{
		AfxMessageBox(_T("항목을 입력해 주세요"));
		return FALSE;
	}
	
	if ( sACConStart != _T("") || sACConEnd != _T("") )
	{
		if ( sACConStart.Left(3) != _T("KAC") && sACConEnd.Left(3) != _T("KAC") )
		{
			AfxMessageBox(_T("전거통제의 제어번호는 KAC로 시작합니다"));
			return FALSE;
		}
	}

	m_pDM->FreeData();

	return TRUE;
EFS_END
return FALSE;
}

//검색
VOID CMNG_AC_MANAGER_02::OnSearchAC()
{
EFS_BEGIN

	//검색
	////////////////////////////////////////////////////////////
	// 0 - 저자명
	// 1 - 서명
	// 2 - 주제명
	
	INT iACKind;
	iACKind = m_pMCombo->GetCurSel();

	if ( iACKind < 0 ) 
	{
		AfxMessageBox(_T("전거 종류를 선택해 주세요"));
		return;
	}
	
	CString sControlAcStart = _T("");
	CString sControlAcEnd	= _T("");
	CString sSearchStart	= _T("");
	CString sSearchEnd		= _T("");

	GetDlgItem(IDC_eControlNoStart)->GetWindowText(sControlAcStart);
	GetDlgItem(IDC_eControlNoEnd)->GetWindowText(sControlAcEnd);

	GetDlgItem(IDC_eACSearchStart)->GetWindowText(sSearchStart);
	GetDlgItem(IDC_eACSearchEnd)->GetWindowText(sSearchEnd);
	
	if ( sControlAcStart != _T("") || sControlAcEnd != _T("") )
	{
		// 전거 제어번호로 검색
		OnSearchControlAC( iACKind, sControlAcStart, sControlAcEnd );
	}
	else if ( sSearchStart != _T("") || sSearchEnd != _T("") )
	{
		// 전거 조회표목으로 검색
		OnSearchACSearch( iACKind, sSearchStart, sSearchEnd );
	}
	else
		return;

EFS_END
}


//전거제어번호로 검색
VOID CMNG_AC_MANAGER_02::OnSearchControlAC(INT iACKind, CString sControlAcStart, CString sControlAcEnd)
{
EFS_BEGIN
	// 전거 제어번호로 검색
	CESL_DataMgr *pTmpDM = new CESL_DataMgr;
	pTmpDM->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	if ( pTmpDM == NULL ) return;

	CESL_ControlMgr *pCM = NULL;
	pCM = FindCM(_T("CM_MNG_AC_MANAGER_01"));
	if ( pCM == NULL ) return;

	CString sSearchQuery = _T("");

	//f ( iACKind < 0  || sControlAcStart == _T("") || sControlAcEnd == _T("") ) return;
	if( sControlAcStart == _T("") && sControlAcEnd != _T("") )
	{
		AfxMessageBox(_T("첫번째 전거제어번호를 넣어주십시오"));
		return;
	}
	else if(sControlAcStart != _T("") && sControlAcEnd == _T("") )
	{
		CString sTmpChar0;
		if ( sControlAcStart.GetLength() < 12 )
		{
			sTmpChar0 = _T("");
			for ( INT i = sControlAcStart.GetLength(); i < 12 ; i++ )
				sTmpChar0 += _T("0");

			sControlAcStart = sControlAcStart.Left(3) + sTmpChar0 + sControlAcStart.Mid(3);		
		}

		sSearchQuery.Format(_T(" SELECT REC_KEY, AC_CONTROL_NO, MARC, AC_TYPE, AC_CLASS, SEE_ALSO_REFERENCE_YN,'' FROM AC_MASTER_MAT_TBL WHERE AC_CONTROL_NO = '%s'"), sControlAcStart );
	}
	else
	{

		//자릿수 맞추기
		CString sTmpChar0;
		if ( sControlAcStart.GetLength() < 12 )
		{
			sTmpChar0 = _T("");
			for ( INT i = sControlAcStart.GetLength(); i < 12 ; i++ )
				sTmpChar0 += _T("0");

			sControlAcStart = sControlAcStart.Left(3) + sTmpChar0 + sControlAcStart.Mid(3);		
		}
		
		if ( sControlAcEnd.GetLength() < 12 )
		{
			sTmpChar0 = _T("");
			for ( INT i = sControlAcEnd.GetLength(); i < 12 ; i++ )
				sTmpChar0 += _T("0");
			
			sControlAcEnd = sControlAcEnd.Left(3) + sTmpChar0 + sControlAcEnd.Mid(3);		
		}
			
		sSearchQuery.Format(_T(" SELECT REC_KEY, AC_CONTROL_NO, MARC, AC_TYPE, AC_CLASS, SEE_ALSO_REFERENCE_YN,'' FROM AC_MASTER_MAT_TBL WHERE AC_CONTROL_NO between '%s' and '%s' "), sControlAcStart, sControlAcEnd );
	}

	pTmpDM->RestructDataManager(sSearchQuery);
	
	INT nRow;
	nRow = pTmpDM->GetRowCount();

	if (  nRow < 1 ) 
	{
		AfxMessageBox(_T("검색된 결과가 없습니다."));
		return;
	}

	for( INT i = 0; i < nRow ; i++ )
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("REC_KEY"), pTmpDM->GetCellData(i,0), i);
		m_pDM->SetCellData(_T("AC_CONTROL_NO"), pTmpDM->GetCellData(i,1), i);
		m_pDM->SetCellData(_T("MARC"), pTmpDM->GetCellData(i,2), i);
		m_pDM->SetCellData(_T("AC_TYPE"), pTmpDM->GetCellData(i,3), i);
		m_pDM->SetCellData(_T("AC_CLASS"), pTmpDM->GetCellData(i,4), i);
		m_pDM->SetCellData(_T("SEE_ALSO_REFERENCE"), pTmpDM->GetCellData(i,5), i);
	}

	for ( i = 0 ; i < nRow ; i++)
	{
		CString sACTYPE, smarc, strTmp, strTmp2, strTmp3;
		CMarc marc;
		strTmp = strTmp2 = strTmp3 = _T("");
	
		m_pDM->GetCellData(_T("MARC"), i, smarc);
		m_pInfo->pMarcMgr->Decoding(smarc, &marc);
		
		m_pDM->GetCellData( _T("AC_TYPE"), i, sACTYPE);
		
		if ( sACTYPE == _T("0") || sACTYPE == _T("1") || sACTYPE == _T("2") )
		{
			m_pDM->SetCellData(_T("전거종류"), _T("저자명"), i );
			if( sACTYPE == _T("0") )
			{
				m_pInfo->pMarcMgr->GetItem(&marc, _T("100$a"), strTmp);
				m_pInfo->pMarcMgr->GetItem(&marc, _T("100$d"), strTmp2);
			}
			else if ( sACTYPE == _T("1") )
			{
				m_pInfo->pMarcMgr->GetItem(&marc,_T("110$a"), strTmp);
				m_pInfo->pMarcMgr->GetItem(&marc, _T("110$d"), strTmp2);
			}
			else
			{
				m_pInfo->pMarcMgr->GetItem(&marc,_T("111$a"), strTmp);
				m_pInfo->pMarcMgr->GetItem(&marc, _T("111$d"), strTmp2);
			}
			
			m_pDM->SetCellData(_T("채택표목"), strTmp, i);			
			m_pDM->SetCellData(_T("생몰년"), strTmp2, i);
		}			
		else if ( sACTYPE == _T("3") )
		{
			m_pDM->SetCellData(_T("전거종류"), _T("서명"), i );
			m_pInfo->pMarcMgr->GetItem(&marc, _T("130$a"), strTmp);
			m_pDM->SetCellData(_T("채택표목"), strTmp, i);
		}
		else if ( sACTYPE == _T("4") )
		{
			m_pDM->SetCellData(_T("전거종류"), _T("주제명"), i );
			m_pInfo->pMarcMgr->GetItem(&marc, _T("150$a"), strTmp);
			m_pDM->SetCellData(_T("채택표목"), strTmp, i);
		}
		else 
			m_pDM->SetCellData(_T("전거종류"), _T(""), i );
		
		m_pInfo->pMarcMgr->GetItem(&marc, _T("678$a"), strTmp3);
		m_pDM->SetCellData(_T("주기"), strTmp3, i);
	}

	pCM->AllControlDisplay();
	
	CDialog::OnOK();


EFS_END
}

//표목으로 검색
VOID CMNG_AC_MANAGER_02::OnSearchACSearch( INT iACKind, CString sControlAcStart, CString sControlAcEnd )
{
EFS_BEGIN

	// 전거 조회표목으로 검색
	// 0 - 저자명
	// 1 - 서명
	// 2 - 주제명

	CESL_ControlMgr *pCM = NULL;
	pCM = FindCM(_T("CM_MNG_AC_MANAGER_01"));
	if ( pCM == NULL ) return;
/*
	CESL_DataMgr *pDM = NULL;
	pDM = FindDM(_T("DM_MNG_AC_MANAGER_01_MASTER"));
	if ( pDM == NULL ) return;

*/
	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	if ( pDM == NULL ) return;
	
	//if ( iACKind < 0  || sControlAcStart == _T("") || sControlAcEnd == _T("") ) return;
	

	INT iSelCombo =  m_pMCombo->GetCurSel();
	CString sTblName = _T("");
	CString sACTypeQuery = _T("");

	if ( iSelCombo == 0 )
	{
		sTblName = _T("AC_INDEX_AUTHOR_TBL");
		sACTypeQuery = _T("AC_TYPE IN ('0', '1', '2')");
	}
	else if ( iSelCombo == 1 )
	{
		sTblName = _T("AC_INDEX_TITLE_TBL");
		sACTypeQuery = _T("AC_TYPE IN ('3')");
	}
	else if ( iSelCombo == 2 )
	{
		sTblName = _T("AC_INDEX_SUBJECT_TBL");
		sACTypeQuery = _T("AC_TYPE IN ('4')");
	}

	CString sSearchQuery = _T("");
	
	if( sControlAcStart == _T("") && sControlAcEnd != _T("") )
	{
		AfxMessageBox(_T("첫번째 검색어가 없습니다."));
		return;
	}
	else if( sControlAcStart != _T("") && sControlAcEnd == _T("") )
	{
		sSearchQuery.Format(_T(" SELECT REC_KEY, AC_CONTROL_NO, MARC, AC_TYPE, AC_CLASS, SEE_ALSO_REFERENCE_YN,'' FROM AC_MASTER_MAT_TBL WHERE %s AND REC_KEY IN (SELECT DISTINCT AC_MASTER_MAT_KEY FROM %s WHERE KEYWORD = '%s' ) "), sACTypeQuery, sTblName, sControlAcStart );
	}
	else
		sSearchQuery.Format(_T(" SELECT REC_KEY, AC_CONTROL_NO, MARC, AC_TYPE, AC_CLASS, SEE_ALSO_REFERENCE_YN,'' FROM AC_MASTER_MAT_TBL WHERE %s AND REC_KEY IN (SELECT DISTINCT AC_MASTER_MAT_KEY FROM %s WHERE KEYWORD between '%s' and '%s') "), sACTypeQuery, sTblName, sControlAcStart, sControlAcEnd );
	
	pDM->RestructDataManager(sSearchQuery);
	
	INT nRow;
	nRow = pDM->GetRowCount();
	if ( nRow < 1 ) 
	{
		AfxMessageBox(_T("검색된 결과가 없습니다."));
		return; 
	}

	for( INT i = 0; i < nRow ; i++ )
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("REC_KEY"), pDM->GetCellData(i,0), i);
		m_pDM->SetCellData(_T("AC_CONTROL_NO"), pDM->GetCellData(i,1), i);
		m_pDM->SetCellData(_T("MARC"), pDM->GetCellData(i,2), i);
		m_pDM->SetCellData(_T("AC_TYPE"), pDM->GetCellData(i,3), i);
		m_pDM->SetCellData(_T("AC_CLASS"), pDM->GetCellData(i,4), i);
		m_pDM->SetCellData(_T("SEE_ALSO_REFERENCE"), pDM->GetCellData(i,5), i);
	}

	for ( i = 0 ; i < nRow ; i++)
	{
		CString sACTYPE, smarc, strTmp, strTmp2, strTmp3;
		CMarc marc;
		strTmp = strTmp2 = strTmp3 = _T("");
	
		m_pDM->GetCellData(_T("MARC"), i, smarc);
		m_pInfo->pMarcMgr->Decoding(smarc, &marc);
		
		m_pDM->GetCellData( _T("AC_TYPE"), i, sACTYPE);
		
		if ( sACTYPE == _T("0") || sACTYPE == _T("1") || sACTYPE == _T("2") )
		{
			m_pDM->SetCellData(_T("전거종류"), _T("저자명"), i );
			
			if( sACTYPE == _T("0") )
			{
				m_pInfo->pMarcMgr->GetItem(&marc,_T("100$a"), strTmp);
				m_pInfo->pMarcMgr->GetItem(&marc, _T("100$d"), strTmp2);
			}
			else if( sACTYPE == _T("1") )
			{
				m_pInfo->pMarcMgr->GetItem(&marc,_T("110$a"), strTmp);
				m_pInfo->pMarcMgr->GetItem(&marc, _T("110$d"), strTmp2);
			}
			else
			{
				m_pInfo->pMarcMgr->GetItem(&marc,_T("111$a"), strTmp);
				m_pInfo->pMarcMgr->GetItem(&marc, _T("111$d"), strTmp2);
			}
						
			m_pDM->SetCellData(_T("채택표목"), strTmp, i);			
			m_pDM->SetCellData(_T("생몰년"), strTmp2, i);
		}			
		else if ( sACTYPE == _T("3") )
		{
			m_pDM->SetCellData(_T("전거종류"), _T("서명"), i );
			m_pInfo->pMarcMgr->GetItem(&marc, _T("130$a"), strTmp);
			m_pDM->SetCellData(_T("채택표목"), strTmp, i);
		}
		else if ( sACTYPE == _T("4") )
		{
			m_pDM->SetCellData(_T("전거종류"), _T("주제명"), i );
			m_pInfo->pMarcMgr->GetItem(&marc, _T("130$a"), strTmp);
			m_pDM->SetCellData(_T("채택표목"), strTmp, i);
		}
		else 
			m_pDM->SetCellData(_T("전거종류"), _T(""), i );
		
		m_pInfo->pMarcMgr->GetItem(&marc, _T("687$a"), strTmp3);
		m_pDM->SetCellData(_T("주기"), strTmp3, i);
	}
	/*
	for ( INT i = 0 ; i < nRow; i++)
	{
		CString sACTYPE;
		pDM->GetCellData(_T("AC_TYPE"), i, sACTYPE);
		if ( sACTYPE == _T("0") || sACTYPE == _T("1") || sACTYPE == _T("2") )
			pDM->SetCellData(_T("전거종류"), _T("저자명"), i );
		else if ( sACTYPE == _T("3") )
			pDM->SetCellData(_T("전거종류"), _T("서명"), i );
		else if ( sACTYPE == _T("4") )
			pDM->SetCellData(_T("전거종류"), _T("주제명"), i );
		else 
			pDM->SetCellData(_T("전거종류"), _T(""), i );
	}
	*/
	pCM->AllControlDisplay();
		
	return;
EFS_END
}


BOOL CMNG_AC_MANAGER_02::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->lParam == VK_RETURN)
	{
		OnOK();
	}
	else if(pMsg->lParam == VK_ESCAPE)
	{
		OnCancel();
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CMNG_AC_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
