// EXCHANGERATE_INSERT.cpp : implementation file
//

#include "stdafx.h"
#include "EXCHANGERATE_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORK_LOG GetWorkLogMsg(_T("환율관리"), _T(__FILE__), __LINE__)

/////////////////////////////////////////////////////////////////////////////
// CEXCHANGERATE_INSERT dialog


CEXCHANGERATE_INSERT::CEXCHANGERATE_INSERT(CESL_Mgr* pParent)
	: CESL_Mgr(CEXCHANGERATE_INSERT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEXCHANGERATE_INSERT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	m_idx = -1;
	m_nOpenMode = -1;
	m_firstidx = -1;
	m_lastidx = -1;
}
CEXCHANGERATE_INSERT::~CEXCHANGERATE_INSERT()
{
}

VOID CEXCHANGERATE_INSERT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEXCHANGERATE_INSERT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEXCHANGERATE_INSERT, CDialog)
	//{{AFX_MSG_MAP(CEXCHANGERATE_INSERT)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnEX_NEXT, OnbtnEXNEXT)
	ON_BN_CLICKED(IDC_btnEX_PREV, OnbtnEXPREV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEXCHANGERATE_INSERT message handlers

VOID CEXCHANGERATE_INSERT::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

BOOL CEXCHANGERATE_INSERT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_환율정보")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pDM = FindDM(_T("DM_환율관리"));
	m_pCM = FindCM(_T("CM_환율정보"));
	
	if(!m_pDM || m_strCurrencyCode.IsEmpty() || !m_pMainGrid)
	{
		ESLAfxMessageBox(_T("초기 설정정보가 올바르지 않습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	if(m_nOpenMode == 1)
	{
		Display(0);
	}
	else 
	{
		CString result;
		CTime t = CTime::GetCurrentTime();
		result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
		m_pCM->SetControlMgrData(_T("입력일자"), result);
		GetDlgItem(IDC_btnEX_PREV)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnEX_NEXT)->EnableWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEXCHANGERATE_INSERT::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CEXCHANGERATE_INSERT::OnOK() 
{
	CString key, inputdate, exchangerate, currencycode, query, currencycodekey, firstwork, lastwork;
	
	m_pCM->GetControlMgrData(_T("입력일자"), inputdate);
	m_pCM->GetControlMgrData(_T("환율"), exchangerate);

	// 2006-02-14 KBS - 국방대요구사항, 한국화폐에 환율은 무조건 1로 저장되도록 한다.
	if( m_strCurrencyCode == _T("\\") && exchangerate != _T("1") )
	{
		ESLAfxMessageBox(_T("한국화폐구분의 환율은 \"1\"이어야 합니다."));
		return;
	}

	query.Format(_T("select REC_KEY from CD_CODE_TBL where CLASS = 23 AND CODE LIKE '%s%%'"), m_strCurrencyCode);
	m_pDM->GetOneValueQuery(query, currencycodekey);
	firstwork = WORK_LOG;
	lastwork = WORK_LOG;

	m_pDM->StartFrame();

	m_pDM->InitDBFieldData();
	
	if( m_nOpenMode == -1 )
	{
		m_pDM->MakeRecKey(key);
		m_pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), key);
		m_pDM->AddDBFieldData(_T("INPUT_DATE"), _T("STRING"), inputdate);
		m_pDM->AddDBFieldData(_T("EXCHANGE_RATE"), _T("NUMERIC"), exchangerate);
		m_pDM->AddDBFieldData(_T("USER_CODE_MNG_KEY"), _T("NUMERIC"), currencycodekey);
		m_pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), firstwork);
		m_pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), lastwork);
		//++2008.10.17 ADD BY CJY {{++
		// 통합시스템
		m_pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));
		//--2008.10.17 ADD BY CJY --}}
		m_pDM->MakeInsertFrame(m_pDM->TBL_NAME);
		m_pDM->InsertRow(-1);
		INT nRow = m_pDM->GetRowCount() -1 ;
		m_pDM->SetCellData(_T("환율정보KEY"), key, nRow);
		m_pDM->SetCellData(_T("입력날짜"), inputdate, nRow);
		m_pDM->SetCellData(_T("환율"), exchangerate, nRow);
		m_pDM->SetCellData(_T("적용여부"), _T("N"), nRow);
		m_idx = nRow;
	}
	else
	{
		key = m_pDM->GetCellData(_T("환율정보KEY"), m_idx);
		m_pDM->AddDBFieldData(_T("INPUT_DATE"), _T("STRING"), inputdate);
		m_pDM->AddDBFieldData(_T("EXCHANGE_RATE"), _T("NUMERIC"), exchangerate);
		m_pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), lastwork);
		m_pDM->MakeUpdateFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), key);
		m_pDM->SetCellData(_T("입력날짜"), inputdate, m_idx);
		m_pDM->SetCellData(_T("환율"), exchangerate, m_idx);
	}
	INT ids = m_pDM->SendFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("환율정보를 저장할 수 없습니다."));
		return;
	}
	ids = m_pDM->EndFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("환율정보를 저장할 수 없습니다."));
		return;
	}

	CDialog::OnOK();
}


VOID CEXCHANGERATE_INSERT::OnbtnEXNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CEXCHANGERATE_INSERT::OnbtnEXPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CEXCHANGERATE_INSERT::Display(INT idx)
{
	CString errstr;
	
	switch(idx)
	{
	case 0:
		InitFirstLastIndex();
		m_idx = m_pMainGrid->GetRow() - 1;
		ShowNextPrevButton(m_idx);
		break;
	case -1:
		if(m_firstidx < m_idx) m_idx--;
		ShowNextPrevButton(m_idx);
		break;
	case 1:
		if(m_lastidx > m_idx) m_idx++;
		ShowNextPrevButton(m_idx);
		break;
	}

	m_pCM->AllControlDisplay(m_idx);
	m_pMainGrid->SetReverse(m_idx);
}

VOID CEXCHANGERATE_INSERT::InitFirstLastIndex()
{
	m_firstidx = 0;
	m_lastidx = m_pDM->GetRowCount()-1;
}

VOID CEXCHANGERATE_INSERT::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnEX_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnEX_NEXT)->EnableWindow(TRUE);

	if(m_firstidx == idx || idx < 0)
		GetDlgItem(IDC_btnEX_PREV)->EnableWindow(FALSE);
	if(m_lastidx == idx || idx < 0)
		GetDlgItem(IDC_btnEX_NEXT)->EnableWindow(FALSE);
}

HBRUSH CEXCHANGERATE_INSERT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
