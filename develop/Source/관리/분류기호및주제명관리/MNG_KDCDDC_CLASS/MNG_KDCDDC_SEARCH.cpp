// MNG_KDCDDC_SEATCH.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_KDCDDC_SEARCH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KDCDDC_SEATCH dialog


CMNG_KDCDDC_SEARCH::CMNG_KDCDDC_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KDCDDC_SEATCH)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_KDCDDC_SEARCH::~CMNG_KDCDDC_SEARCH()
{
	
}

BOOL CMNG_KDCDDC_SEARCH::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_KDCDDC_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KDCDDC_SEATCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_KDCDDC_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CMNG_KDCDDC_SEATCH)
	
	ON_WM_CTLCOLOR() 
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KDCDDC_SEATCH message handlers

BOOL CMNG_KDCDDC_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_관리_분류기호_주기표_검색")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if(OpenMode == 1) //조기표검색
	{
		GetDlgItem(IDC_strCLASS_HELP_CODE)->ShowWindow(TRUE);
		GetDlgItem(IDC_CLASS_HELP_CODE)->ShowWindow(TRUE);
	
		SetCode();
	}
	if(OpenMode == 2) //연동색인어 검색
	{
		GetDlgItem(IDC_strKDCDDC_CODE)->ShowWindow(FALSE);
		GetDlgItem(IDC_KDCDDC_CODE)->ShowWindow(FALSE);
	}
	if(OpenMode == 3)	//주제명 검색
	{
		GetDlgItem(IDC_strKDCDDC_CODE)->ShowWindow(FALSE);
		GetDlgItem(IDC_KDCDDC_CODE)->ShowWindow(FALSE);
	}


	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_KDCDDC_SEARCH::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	if ( OpenMode == 0 ) //주기표
	{
		SEARCH_KDCDDC();
	}
	else if ( OpenMode == 1 ) //조기표
	{
		SEARCH_HELP();
	}
	else if ( OpenMode == 2 ) 	//연동색인어
	{
		SEARCH_INDEX();
	}
	else if ( OpenMode == 3 ) 	//주제명
	{
		SEARCH_KEYWORD();
	}
}

VOID CMNG_KDCDDC_SEARCH::SEARCH_KDCDDC()
{
	CString strQry,strClassCode,strKeyword,strUpdateDate1,strUpdateDate2,strHelpCode;


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_주기표_메인"));
		//분류표 구분 
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("분류구분"), strClassCode);

	if(strClassCode == _T("KDC"))
		pDM->TBL_NAME = _T("CO_KDC4_TBL");
	else if(strClassCode == _T("DDC"))
		pDM->TBL_NAME = _T("CO_DDC20_TBL");
	else
		return;

	//검색어 만들기 

	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("검색어"), strKeyword);
	
	strKeyword.TrimLeft();
	strKeyword.TrimRight();

	INT AndFlag=0;
	//키워드 
	if( strKeyword != _T(""))
	{
		strQry += _T(" CLASS_TITLE LIKE '%")+strKeyword+_T("%'");
		AndFlag = 1;
	}

	// 수정일자 
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("수정일자시작"), strUpdateDate1);
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("수정일자끝"), strUpdateDate2);
	
	strUpdateDate1.TrimLeft();
	strUpdateDate1.TrimRight();
	strUpdateDate2.TrimLeft();
	strUpdateDate2.TrimRight();

	if( strUpdateDate1 != _T("") && strUpdateDate2 !=_T("") )
	{
		if( AndFlag == 1) strQry += _T(" AND ");

		strQry += _T(" UPDATE_DATE >= '")+strUpdateDate1+_T("' AND UPDATE_DATE <='")+strUpdateDate2+_T("'");

	}
	if( strUpdateDate1 != _T("") && strUpdateDate2 ==_T("") )
	{
		if( AndFlag == 1) strQry += _T(" AND ");

		strQry += _T(" UPDATE_DATE >= '")+strUpdateDate1+_T("'");

	}

	strQry.TrimLeft();
	strQry.TrimRight();

	if( strQry == _T(""))
	{
		if( IDCANCEL == ESLAfxMessageBox(_T("자료가 많습니다. 그래도 검색 하시겠습니까?"), MB_OKCANCEL)  )
		{
			return;
		}
	}
	pDM->RefreshDataManager(strQry);

	CESL_Grid *pGrid;

	pGrid = ( CESL_Grid * )FindControl(_T("CM_관리_분류기호_주기표_메인"), _T("MAIN_GRID"));
	pGrid->Display();

	INT n_cnt = pDM->GetRowCount();
	if ( n_cnt <= 0 ) 
	{
		ESLAfxMessageBox(_T("검색 결과가 없습니다."));
	}

}

VOID CMNG_KDCDDC_SEARCH::SEARCH_HELP()
{
	CString strQry,strClassCode,strKeyword,strUpdateDate1,strUpdateDate2,strHelpCode;


	CESL_DataMgr *pDM;

	pDM = FindDM(_T("DM_관리_분류기호_조기표_메인"));
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("분류구분"), strClassCode);

	if(strClassCode == _T("KDC"))
		pDM->TBL_NAME = _T("CO_KDC_MEMHELP_TBL");
	else if(strClassCode == _T("DDC"))
		pDM->TBL_NAME = _T("CO_DDC_MEMHELP_TBL");
	else
		return;	

	//조기표 구분 
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2004.06.11 김소정 수정
	strHelpCode.Format(_T("%d"), m_pMCombo->GetCurSel()+1 );
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if( strHelpCode != _T(""))
	{
		strQry += _T(" MEMHELP_CODE='")+strHelpCode+_T("'");
	}


	//검색어 만들기 

	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("검색어"), strKeyword);
	
	strKeyword.TrimLeft();
	strKeyword.TrimRight();

	//키워드 
	if( strKeyword != _T(""))
	{
		strQry += _T(" AND ");
		strQry += _T(" MEMHELP_TITLE LIKE '%")+strKeyword+_T("%'");
	}

	// 수정일자 
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("수정일자시작"), strUpdateDate1);
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("수정일자끝"), strUpdateDate2);
	
	strUpdateDate1.TrimLeft();
	strUpdateDate1.TrimRight();
	strUpdateDate2.TrimLeft();
	strUpdateDate2.TrimRight();

	if( strUpdateDate1 != _T("") && strUpdateDate2 !=_T("") )
	{
		strQry += _T(" AND ");

		strQry += _T(" UPDATE_DATE >= '")+strUpdateDate1+_T("' AND UPDATE_DATE <='")+strUpdateDate2+_T("'");

	}
	if( strUpdateDate1 != _T("") && strUpdateDate2 ==_T("") )
	{
		strQry += _T(" AND ");

		strQry += _T(" UPDATE_DATE >= '")+strUpdateDate1+_T("'");

	}

	strQry.TrimLeft();
	strQry.TrimRight();

	if( strQry == _T(""))
	{
		if( IDCANCEL == ESLAfxMessageBox(_T("자료가 많습니다. 그래도 검색 하시겠습니까?"), MB_OKCANCEL)  )
		{
			return;
		}
	}
	pDM->RefreshDataManager(strQry);

	CESL_Grid *pGrid;

	pGrid = ( CESL_Grid * )FindControl(_T("CM_관리_분류기호_조기표_메인"), _T("MAIN_GRID"));

	pGrid->Display();

	INT n_cnt = pDM->GetRowCount();
	if ( n_cnt <= 0 ) 
	{
		ESLAfxMessageBox(_T("검색 결과가 없습니다."));
	}

}

VOID CMNG_KDCDDC_SEARCH::SEARCH_INDEX()
{
	CString strQry,strKeyword,strUpdateDate1,strUpdateDate2;


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_연동색인어_메인"));

	//검색어 만들기 

	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("검색어"), strKeyword);
	
	strKeyword.TrimLeft();
	strKeyword.TrimRight();

	INT AndFlag=0;
	//키워드 
	if( strKeyword != _T(""))
	{
		strQry += _T(" INDEX_WORD LIKE '%")+strKeyword+_T("%'");
		AndFlag = 1;
	}

	// 수정일자 
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("수정일자시작"), strUpdateDate1);
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("수정일자끝"), strUpdateDate2);
	
	strUpdateDate1.TrimLeft();
	strUpdateDate1.TrimRight();
	strUpdateDate2.TrimLeft();
	strUpdateDate2.TrimRight();

	if( strUpdateDate1 != _T("") && strUpdateDate2 !=_T("") )
	{
		if( AndFlag == 1) strQry += _T(" AND ");

		strQry += _T(" UPDATE_DATE >= '")+strUpdateDate1+_T("' AND UPDATE_DATE <='")+strUpdateDate2+_T("'");

	}
	if( strUpdateDate1 != _T("") && strUpdateDate2 ==_T("") )
	{
		if( AndFlag == 1) strQry += _T(" AND ");

		strQry += _T(" UPDATE_DATE >= '")+strUpdateDate1+_T("'");

	}

	strQry.TrimLeft();
	strQry.TrimRight();

	if( strQry == _T(""))
	{
		if( IDCANCEL == ESLAfxMessageBox(_T("자료가 많습니다. 그래도 검색 하시겠습니까?"), MB_OKCANCEL)  )
		{
			return;
		}
	}
	pDM->RefreshDataManager(strQry);

	CESL_Grid *pGrid;

	pGrid = ( CESL_Grid * )FindControl(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));
	pGrid->Display();

	INT n_cnt = pDM->GetRowCount();
	if ( n_cnt <= 0 ) 
	{
		ESLAfxMessageBox(_T("검색 결과가 없습니다."));
	}

}

VOID CMNG_KDCDDC_SEARCH::SEARCH_KEYWORD()
{
	CString strQry,strKeyword,strUpdateDate1,strUpdateDate2;


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_주제명_메인"));

	//검색어 만들기 

	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("검색어"), strKeyword);
	
	strKeyword.TrimLeft();
	strKeyword.TrimRight();

	INT AndFlag=0;
	//키워드 
	if( strKeyword != _T(""))
	{
		strQry += _T(" SUBJECT_NAME LIKE '%")+strKeyword+_T("%'");
		AndFlag = 1;
	}

	// 수정일자 
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("수정일자시작"), strUpdateDate1);
	GetControlData(_T("CM_관리_분류기호_주기표_검색"), _T("수정일자끝"), strUpdateDate2);
	
	strUpdateDate1.TrimLeft();
	strUpdateDate1.TrimRight();
	strUpdateDate2.TrimLeft();
	strUpdateDate2.TrimRight();

	if( strUpdateDate1 != _T("") && strUpdateDate2 !=_T("") )
	{
		if( AndFlag == 1) strQry += _T(" AND ");

		strQry += _T(" UPDATE_DATE >= '")+strUpdateDate1+_T("' AND UPDATE_DATE <='")+strUpdateDate2+_T("'");

	}
	if( strUpdateDate1 != _T("") && strUpdateDate2 ==_T("") )
	{
		if( AndFlag == 1) strQry += _T(" AND ");

		strQry += _T(" UPDATE_DATE >= '")+strUpdateDate1+_T("'");

	}

	strQry.TrimLeft();
	strQry.TrimRight();

	if( strQry == _T(""))
	{
		if( IDCANCEL == ESLAfxMessageBox(_T("자료가 많습니다. 그래도 검색 하시겠습니까?"), MB_OKCANCEL)  )
		{
			return;
		}
	}
	pDM->RefreshDataManager(strQry);

	CESL_Grid *pGrid;

	pGrid = ( CESL_Grid * )FindControl(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));
	pGrid->Display();

	INT n_cnt = pDM->GetRowCount();
	if ( n_cnt <= 0 ) 
	{
		ESLAfxMessageBox(_T("검색 결과가 없습니다."));
	}

}

BOOL CMNG_KDCDDC_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) {
	
		if ( pMsg->wParam == VK_RETURN ) {
			OnbSEARCH();
			return TRUE;
		} 
	}
	
	return CDialog::PreTranslateMessage(pMsg);	
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.11 김소정 추가
VOID CMNG_KDCDDC_SEARCH::SetCode()
{
EFS_BEGIN
	m_pMCombo = new CJComboBox;

	m_pMCombo->SubclassDlgItem( IDC_CLASS_HELP_CODE, this );
	m_pMCombo->FormatList( 2, DT_LEFT, false , EDGE );
	m_pMCombo->RequireFlag = FALSE;

	m_pMCombo->AddItem( _T("1"), 0, 0 );
	m_pMCombo->AddItem( _T("標準區分表"), 1, 0 );

	m_pMCombo->AddItem( _T("2"), 0, 1 );
	m_pMCombo->AddItem( _T("地域區分表"), 1, 1 );

	m_pMCombo->AddItem( _T("3"), 0, 2 );
	m_pMCombo->AddItem( _T("韓國地域區分表"), 1, 2 );

	m_pMCombo->AddItem( _T("4"), 0, 3 );
	m_pMCombo->AddItem( _T("韓國時代區分表"), 1, 3 );

	m_pMCombo->AddItem( _T("5"), 0, 4 );
	m_pMCombo->AddItem( _T("國語區分表"), 1, 4 );

	m_pMCombo->AddItem( _T("6"), 0, 5 );
	m_pMCombo->AddItem( _T("文學型式區分表"), 1, 5 );

	m_pMCombo->AddItem( _T("7"), 0, 6 );
	m_pMCombo->AddItem( _T("言語共通區分表"), 1, 6 );

	m_pMCombo->AddItem( _T("8"), 0, 7 );
	m_pMCombo->AddItem( _T("宗敎共通區分表"), 1, 7 );
	
	m_pMCombo->SetCurSel( 0 );

	return ;
EFS_END
return ;
}

HBRUSH CMNG_KDCDDC_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
