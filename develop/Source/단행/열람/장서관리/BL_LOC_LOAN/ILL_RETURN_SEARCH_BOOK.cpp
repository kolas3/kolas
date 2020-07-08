// ILL_RETURN_SEARCH_BOOK.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ILL_RETURN_SEARCH_BOOK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILL_RETURN_SEARCH_BOOK dialog


CILL_RETURN_SEARCH_BOOK::CILL_RETURN_SEARCH_BOOK(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CILL_RETURN_SEARCH_BOOK)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CILL_RETURN_SEARCH_BOOK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_RETURN_SEARCH_BOOK)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_RETURN_SEARCH_BOOK, CDialog)
	//{{AFX_MSG_MAP(CILL_RETURN_SEARCH_BOOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILL_RETURN_SEARCH_BOOK message handlers

BOOL CILL_RETURN_SEARCH_BOOK::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 

	InitESL_Mgr(_T("SM_ILL_RETURN_SEARCH_BOOK"));

	CESL_DataMgr* pDM = FindDM(_T("DM_ILL_RETURN_SEARCH_BOOK"));


	CString strTblName;
	if(_T("D") != m_sBookCooperativeMode)
	{
		strTblName.Format(
			_T("(SELECT A.*, CASE A.PUBLISH_FORM_CODE WHEN 'MO' THEN (SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REC_KEY = BOOK_KEY) WHEN 'SE' THEN (SELECT MANAGE_CODE FROM SE_BOOK_TBL WHERE REC_KEY = BOOK_KEY) WHEN 'NB' THEN (SELECT MANAGE_CODE FROM CO_NON_DB_BOOK_TBL WHERE REC_KEY = BOOK_KEY) END BOOK_MANAGE_CODE FROM LS_WORK_T01 A WHERE %s) T"),
			m_strOption);
	}
	else
	{
		strTblName.Format(
			_T("(SELECT A.*, A.MANAGE_CODE AS BOOK_MANAGE_CODE FROM LS_WORK_T01 A WHERE %s) T"),
			m_strOption);
	}

	pDM->TBL_NAME = strTblName;
	pDM->RefreshDataManager(_T(""));

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_ILL_RETURN_SEARCH_BOOK"), _T("그리드_책목록"));
	pGrid->Display();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CILL_RETURN_SEARCH_BOOK::OnOK() 
{
	// TODO: Add extra validation here
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_ILL_RETURN_SEARCH_BOOK"), _T("그리드_책목록"));
	pGrid->SelectMakeList();
	if(0 == pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("책을 선정하여 주십시요."));
		return;
	}
	else if (1 < pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("하나만 선정하여 주십시요."));
		return;
	}
	int nIdx = pGrid->SelectGetHeadPosition();
	CESL_DataMgr* pDM = FindDM(_T("DM_ILL_RETURN_SEARCH_BOOK"));
	pDM->GetCellData(_T("유저키"), nIdx, m_strUserKey);
	pDM->GetCellData(_T("관리구분"), nIdx, m_strBookManageCode);
	pDM->GetCellData(_T("대출상태"), nIdx, m_strStatus);

	CDialog::OnOK();
}


BOOL CILL_RETURN_SEARCH_BOOK::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			OnOK();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
