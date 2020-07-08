// Request_SearchMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Request_SearchMgr.h"
#include "../../../공통/INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequest_SearchMgr dialog


CRequest_SearchMgr::CRequest_SearchMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequest_SearchMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequest_SearchMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_bShowMode = FALSE;
	m_strWhereStmt = _T("");
	m_bCreate = FALSE;
}


VOID CRequest_SearchMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_SearchMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequest_SearchMgr, CDialog)
	//{{AFX_MSG_MAP(CRequest_SearchMgr)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnSHOWHIDE, OnbtnSHOWHIDE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequest_SearchMgr message handlers

BOOL CRequest_SearchMgr::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CRequest_SearchMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CRequest_SearchMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_비치희망자료검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_비치희망자료검색"));	
	m_pDM = FindDM(_T("DM_비치희망자료관리"));

	CString furnish_status[5][2] = {
		_T(" "), _T("적용안함"),
		_T("1"), _T("신청중"),
		_T("2"), _T("처리중"),
		_T("3"), _T("소장중"),
		_T("4"), _T("취소됨")
	};
	
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("적용안함"), furnish_status, _T("비치상태"), 5);

	GetWindowRect(m_rcWindow);
	ShowControlByShowMode();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequest_SearchMgr::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CRequest_SearchMgr::OnOK() 
{
	// TODO: Add extra validation here
	CString status  = _T("");
	CString sosok = _T("");
	CString requestor = _T("");
	CString req_date1 = _T("");
	CString req_date2 = _T("");
	CString title = _T("");
	CString author = _T("");
	CString publisher = _T("");
	CString pub_year = _T("");

	m_pCM->GetControlMgrData(_T("비치상태"), status);
	m_pCM->GetControlMgrData(_T("신청자"), requestor);
	m_pCM->GetControlMgrData(_T("신청일자1"), req_date1);
	m_pCM->GetControlMgrData(_T("신청일자2"), req_date2);
	if(m_bShowMode)
	{
		m_pCM->GetControlMgrData(_T("본표제"), title);
		m_pCM->GetControlMgrData(_T("저작자"), author);
		m_pCM->GetControlMgrData(_T("발행자"), publisher);
		m_pCM->GetControlMgrData(_T("발행년"), pub_year);
	}
	
	status.TrimLeft();		status.TrimRight();
//	sosok.TrimLeft();		sosok.TrimRight();
	req_date1.TrimLeft();	req_date1.TrimRight();	
	req_date2.TrimLeft();	req_date2.TrimRight();	
	pub_year.TrimLeft();	pub_year.TrimRight();
	title.TrimLeft();		title.TrimRight();
	author.TrimLeft();		author.TrimRight();		
	publisher.TrimLeft();	publisher.TrimRight();
	requestor.TrimLeft();	requestor.TrimRight();

	CString strWhere = _T("");
	CString strOption = _T("");
	CString result;
	//발행년 값 검사
	INT ids = makeSearchData.IsValidYearData(pub_year);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("발행년 정보가 올바르지 않습니다."));
		CTime t = CTime::GetCurrentTime();
		result.Format(_T("%04d"), t.GetYear());
		m_pCM->SetControlMgrData(_T("발행년"), result);
		return;
	}

	if(!status.IsEmpty())
	{
		strOption.Format(_T(" FURNISH_STATUS = '%s'"), status);
		strWhere += strOption;
	}
	
	strWhere += MakeDateQuery(req_date1, req_date2, strWhere);

	if(!(requestor.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND APPLICANT_NAME LIKE '%s%%'"), requestor);
		}
		else
		{
			strOption.Format(_T("APPLICANT_NAME LIKE '%s%%'"), requestor);
		}
		strWhere += strOption;
	}
	
	if(!(title.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND &ECOSEARCH(INDEX_TITLE, '%s*')>NULL"), title);
			//strOption.Format(_T(" AND CATSEARCH(INDEX_TITLE, '%s*')>NULL"), title);
		}
		else
		{
			strOption.Format(_T("&ECOSEARCH(INDEX_TITLE, '%s*')>NULL"), title);
			//strOption.Format(_T("CATSEARCH(INDEX_TITLE, '%s*')>NULL"), title);
		}
		strWhere += strOption;
	}

	if(!(author.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND &ECOSEARCH(INDEX_AUTHOR, '%s*')>NULL"), author);
			//strOption.Format(_T(" AND CATSEARCH(INDEX_AUTHOR, '%s*')>NULL"), author);
		}
		else
		{
			strOption.Format(_T("&ECOSEARCH(INDEX_AUTHOR, '%s*')>NULL"), author);
			//strOption.Format(_T("CATSEARCH(INDEX_AUTHOR, '%s*')>NULL"), author);
		}
		strWhere += strOption;
	}
	
	if(!(publisher.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND &ECOSEARCH(INDEX_PUBLISHER, '%s*')>NULL"), publisher);
			//strOption.Format(_T(" AND CATSEARCH(INDEX_PUBLISHER, '%s*')>NULL"), publisher);
		}
		else
		{
			strOption.Format(_T("&ECOSEARCH(INDEX_PUBLISHER, '%s*')>NULL"), publisher);
			//strOption.Format(_T("CATSEARCH(INDEX_PUBLISHER, '%s*')>NULL"), publisher);
		}
		strWhere += strOption;
	}

	if(!(pub_year.IsEmpty()))
	{
		if(!(strWhere.IsEmpty()))
		{
			strOption.Format(_T(" AND PUBLISH_YEAR = '%s'"), pub_year);
		}
		else
		{
			strOption.Format(_T("PUBLISH_YEAR = '%s'"), pub_year);
		}
		strWhere += strOption;
	}	
	m_strWhereStmt = strWhere;
	::SendMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

CString CRequest_SearchMgr::MakeDateQuery(CString Date1, CString Date2, CString WhereStmt)
{
	CString query = _T("");
	
	if(!WhereStmt.IsEmpty())
	{
		if(Date1.IsEmpty() && Date2.IsEmpty())
			return _T("");
		else if(Date1.IsEmpty() && !(Date2.IsEmpty()))
		{
			ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
			return _T("");
		}
		else if(!(Date1.IsEmpty()) && Date2.IsEmpty())
		{
			//20070615  UPDATE PJW : 검색조건에 00:00:00~23:59:59 추가
			query.Format(_T(" AND APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), Date1, Date1);
			return query;
		}
		else if(!(Date1.IsEmpty()) && !(Date2.IsEmpty()))
		{
			//20070615  UPDATE PJW : 검색조건에 00:00:00~23:59:59 추가
			query.Format(_T(" AND APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), Date1, Date2);
			return query;
		}
	}
	else
	{
		if(Date1.IsEmpty() && Date2.IsEmpty())
			return _T("");
		else if(Date1.IsEmpty() && !(Date2.IsEmpty()))
		{
			ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
			return _T("");
		}
		else if(!(Date1.IsEmpty()) && Date2.IsEmpty())
		{
			//20070615  UPDATE PJW : 검색조건에 00:00:00~23:59:59 추가
			query.Format(_T("APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), Date1, Date1);
			return query;
		}
		else if(!(Date1.IsEmpty()) && !(Date2.IsEmpty()))
		{
			//20070615  UPDATE PJW : 검색조건에 00:00:00~23:59:59 추가
			query.Format(_T("APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), Date1, Date2);
			return query;
		}
	}
	return _T("");
}

VOID CRequest_SearchMgr::OnbtnSHOWHIDE() 
{
	// TODO: Add your control notification handler code here
	if (m_bShowMode)
	{
		m_bShowMode = FALSE;
	}
	else
	{
		m_bShowMode = TRUE;
	}	
	ShowControlByShowMode();	
}

VOID CRequest_SearchMgr::ShowControlByShowMode()
{
	INT i = 0;
	INT nCodeResourcesHeight = 85;
	CWnd *pWnd = NULL;

	if (m_bShowMode)
		GetDlgItem(IDC_btnSHOWHIDE)->SetWindowText(_T("간략검색"));
	else
		GetDlgItem(IDC_btnSHOWHIDE)->SetWindowText(_T("상세검색"));


	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (m_bShowMode && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_stcREQUEST_HIDE;
	nArrCodeResourceID[nCount++] = IDC_stcREQUEST_TITLE;
	nArrCodeResourceID[nCount++] = IDC_stcREQUEST_AUTHOR;
	nArrCodeResourceID[nCount++] = IDC_stcREQUEST_PUBLISHER;
	nArrCodeResourceID[nCount++] = IDC_stcREQUEST_PUBYEAR;
	nArrCodeResourceID[nCount++] = IDC_edtREQUEST_TITLE;
	nArrCodeResourceID[nCount++] = IDC_edtREQUEST_AUTHOR;
	nArrCodeResourceID[nCount++] = IDC_edtREQUEST_PUBLISHER;
	nArrCodeResourceID[nCount++] = IDC_edtREQUEST_PUBLICATE_YEAR;
	
	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_bShowMode)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	if (m_bShowMode)
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();
}

BOOL CRequest_SearchMgr::PreTranslateMessage(MSG* pMsg) 
{
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
