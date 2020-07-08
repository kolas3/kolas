// Recycle_SearchMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Recycle_SearchMgr.h"
#include "../../../공통/INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecycle_SearchMgr dialog


CRecycle_SearchMgr::CRecycle_SearchMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRecycle_SearchMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecycle_SearchMgr)
	m_nMyData = -1;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_bShowMode = FALSE;
	m_strWhereStmt = _T("");
	m_bCreate = FALSE;
}


VOID CRecycle_SearchMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecycle_SearchMgr)
	DDX_Radio(pDX, IDC_radRECYC_MYDATA, m_nMyData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecycle_SearchMgr, CDialog)
	//{{AFX_MSG_MAP(CRecycle_SearchMgr)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnSHOWHIDE, OnbtnSHOWHIDE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecycle_SearchMgr message handlers

VOID CRecycle_SearchMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CRecycle_SearchMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_재활용자료검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_재활용자료검색"));	
	m_pDM = FindDM(_T("DM_재활용자료관리"));	

	GetWindowRect(m_rcWindow);
	ShowControlByShowMode();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRecycle_SearchMgr::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CRecycle_SearchMgr::OnbtnSHOWHIDE() 
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

VOID CRecycle_SearchMgr::ShowControlByShowMode()
{
	INT i = 0;
	INT nCodeResourcesHeight = 173;
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
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_HIDE1;
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_AUTHOR;
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_PUBLISHER;
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_ISBN;
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_PUBYEAR;
	nArrCodeResourceID[nCount++] = IDC_edtRECYC_PUBYEAR;
	nArrCodeResourceID[nCount++] = IDC_edtRECYC_ISBN1;
	nArrCodeResourceID[nCount++] = IDC_edtRECYC_PUBLISHER;
	nArrCodeResourceID[nCount++] = IDC_edtRECYC_AUTHOR;
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_HIDE2;
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_WORKSTATUS;
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_FORMCODE;
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_INPUTTYPE;
	nArrCodeResourceID[nCount++] = IDC_stcRECYCLE_USEOBJCODE;
	nArrCodeResourceID[nCount++] = IDC_radRECYC_MYDATA;
	nArrCodeResourceID[nCount++] = IDC_radRECYC_ALL;
	nArrCodeResourceID[nCount++] = IDC_cmbRECYC_USE_OBJ_CODE;
	nArrCodeResourceID[nCount++] = IDC_cmbRECYC_INPUT_TYPE;
	nArrCodeResourceID[nCount++] = IDC_cmbRECYC_FORM_CODE;
	nArrCodeResourceID[nCount++] = IDC_cmbRECYC_LAST_WORKING_STATUS;
	
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

BOOL CRecycle_SearchMgr::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CRecycle_SearchMgr::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CString workCode, classCodeType, inputType, UseObjCode, FormCode, pub_year, errstr, result;
	CString title, delDate1, delDate2, author, publisher, isbn1, isbn2, workingst, dataClass ;
	m_pCM->GetControlMgrData(_T("업무구분"), workCode);
	m_pCM->GetControlMgrData(_T("분류표구분"), classCodeType);
	m_pCM->GetControlMgrData(_T("본표제"), title);
	m_pCM->GetControlMgrData(_T("날짜1"), delDate1);
	m_pCM->GetControlMgrData(_T("날짜2"), delDate2);
	delDate1.TrimLeft();		delDate1.TrimRight();
	delDate2.TrimLeft();		delDate2.TrimRight();
	if(delDate1.IsEmpty() && !delDate2.IsEmpty())
	{
		ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
		return ;
	}
	if(m_bShowMode)
	{
		m_pCM->GetControlMgrData(_T("ISBN1"), isbn1);
		m_pCM->GetControlMgrData(_T("발행년"),pub_year);
		pub_year.TrimLeft();		pub_year.TrimRight();
		INT ids = makeSearchData.IsValidYearData(pub_year);
		if( ids < 0)
		{
			errstr.Format(_T("입력하신 입수년도 값(%s)이 올바르지 않습니다."), pub_year);
			ESLAfxMessageBox(errstr.GetBuffer(0));
			CTime t = CTime::GetCurrentTime();
			result.Format(_T("%04d"), t.GetYear());
			m_pCM->SetControlMgrData(_T("수입년도"), result);		
			return ;
		}
		m_pCM->GetControlMgrData(_T("저작자"), author);
		m_pCM->GetControlMgrData(_T("발행자"), publisher);
		m_pCM->GetControlMgrData(_T("최종작업상태"), workingst);
		m_pCM->GetControlMgrData(_T("입력구분"), inputType);
		m_pCM->GetControlMgrData(_T("이용대상구분"), UseObjCode);
		m_pCM->GetControlMgrData(_T("형식구분"), FormCode);
	}
	workCode.TrimLeft();		workCode.TrimRight();
	classCodeType.TrimLeft();	classCodeType.TrimRight();
	isbn1.TrimLeft();			isbn1.TrimRight();
	workingst.TrimLeft();		workingst.TrimRight();
	inputType.TrimLeft();		inputType.TrimRight();
	UseObjCode.TrimLeft();		UseObjCode.TrimRight();
	FormCode.TrimLeft();		FormCode.TrimRight();
	title.TrimLeft();			title.TrimRight();
	author.TrimLeft();			author.TrimRight();
	publisher.TrimLeft();		publisher.TrimRight();

	//본표제, 저작자, 발행자 색인어 생성형식으로 변환
	makeSearchData.GetKeyWord(title);	makeSearchData.ChangeHanjaToHangul(title);
	makeSearchData.GetKeyWord(author);	makeSearchData.ChangeHanjaToHangul(author);
	makeSearchData.GetKeyWord(publisher);	makeSearchData.ChangeHanjaToHangul(publisher);

	INT dataType = 0;
	CString strWhere, strOption;
	ConvertWorkingStatusDescToCode(workingst);

	strWhere = _T("(I.SPECIES_CLASS = '2' OR I.SPECIES_CLASS = '3') AND I.REC_KEY = P.SPECIES_KEY(+) AND I.MANAGE_CODE = UDF_MANAGE_CODE");
	dataType = 1;
	strWhere += MakeDateQuery(delDate1, delDate2, strWhere.IsEmpty(), true);
	if(!workingst.IsEmpty())
	{
		strOption.Format(_T(" AND I.REC_KEY IN (SELECT SPECIES_KEY FROM BO_DELETE_BOOK_TBL WHERE WORKING_STATUS = '%s' AND BOOK_APPENDIX_FLAG='B')"), workingst);
		strWhere += strOption;
		strOption.Format(_T(" AND P.REC_KEY IN (SELECT ACCU_KEY FROM BO_DELETE_BOOK_TBL WHERE WORKING_STATUS = '%s' AND BOOK_APPENDIX_FLAG='B')"), workingst);
		strWhere += strOption;
	}
	
	if(!classCodeType.IsEmpty() && classCodeType != _T("적용안함"))
	{
		strOption.Format(_T(" AND I.CLASS_NO_TYPE = '%s'"), classCodeType);
		strWhere += strOption;
	}
		
	if(!title.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), title);
		//strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), title);
		strWhere += strOption;
	}

	if(!author.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.IDX_AUTHOR, '%s*', NULL)>0"), author);
		//strOption.Format(_T(" AND CATSEARCH(I.IDX_AUTHOR, '%s*', NULL)>0"), author);
		strWhere += strOption;
	}

	if(!publisher.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.IDX_PUBLISHER, '%s*', NULL)>0"), publisher);
		//strOption.Format(_T(" AND CATSEARCH(I.IDX_PUBLISHER, '%s*', NULL)>0"), publisher);
		strWhere += strOption;
	}

	if(!workCode.IsEmpty())
	{
		strOption.Format(_T(" AND I.WORK_CODE = '%s'"), workCode);
		strWhere += strOption;
	}
	if(m_nMyData)
	{
		strOption.Format(_T(" AND I.WORKER = '%s'"), m_pInfo->USER_ID);
		strWhere += strOption;
	}
	if(!inputType.IsEmpty())
	{
		strOption.Format(_T(" AND P.INPUT_TYPE = '%s'"), inputType);
		strWhere += strOption;
	}

	if(!UseObjCode.IsEmpty() && UseObjCode != _T("0"))
	{
		strOption.Format(_T(" AND I.USE_OBJ_CODE = '%s'"), UseObjCode);
		strWhere += strOption;
	}
	if(!FormCode.IsEmpty() && FormCode != _T("0"))
	{
		strOption.Format(_T(" AND I.FORM_CODE = '%s'"), FormCode);
		strWhere += strOption;
	}
	if(!pub_year.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.IDX_IPUB_YEAR, '%s*', NULL)>0"), pub_year);
		//strOption.Format(_T(" AND CATSEARCH(I.IDX_IPUB_YEAR, '%s*', NULL)>0"), pub_year);
		strWhere += strOption;
	}
	if(!isbn1.IsEmpty())
	{
		strOption.Format(_T(" AND &ECOSEARCH(I.ST_CODE, '%s*', NULL)>0"), isbn1);
		//strOption.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s*', NULL)>0"), isbn1);
		strWhere += strOption;
	}
	
	m_strWhereStmt = strWhere;
	::SendMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

VOID CRecycle_SearchMgr::ConvertWorkingStatusDescToCode(CString &workingst)
{
	if(workingst == _T("적용안함"))
		workingst = _T("");
	else if(workingst == _T("구입대상자료"))
		workingst = _T("BOA111N");
	else if(workingst == _T("검수대상자료"))
		workingst = _T("BOA113N");
	else if(workingst == _T("미납자료"))
		workingst = _T("BOA114O");
	else if(workingst == _T("검수완료자료"))
		workingst = _T("BOA141N");
	else if(workingst == _T("편목인계자료"))
		workingst = _T("BOC111N");
	else if(workingst == _T("편목대상자료"))
		workingst = _T("BOC113N");
}

CString CRecycle_SearchMgr::MakeDateQuery(CString Date1, CString Date2, BOOL isEmpty, BOOL isSpecies)
{
	CString query;

	if(isEmpty)
	{
		if(Date1.IsEmpty() && Date2.IsEmpty())
			return _T("");
		else if(!(Date1.IsEmpty()) && Date2.IsEmpty())
		{
			if(isSpecies)
				query.Format(_T("I.DELETE_DATE = '%s'"), Date1);
			else 
				query.Format(_T("B.DELETE_DATE = '%s'"), Date1);
			return query;
		}
		else if(!(Date1.IsEmpty()) && !(Date2.IsEmpty()))
		{
			if(isSpecies)
			{
				if(Date1 == Date2)
					query.Format(_T("I.DELETE_DATE = '%s'"), Date1);
				else
					query.Format(_T("I.DELETE_DATE >= '%s' AND I.DELETE_DATE <= '%s'"), Date1, Date2);
			}
			else
			{
				if(Date1 == Date2)
					query.Format(_T("B.DELETE_DATE = '%s'"), Date1);
				else
					query.Format(_T("B.DELETE_DATE >= '%s' AND B.DELETE_DATE <= '%s'"), Date1, Date2);
			}
			return query;
		}
	}
	else
	{
		if(Date1.IsEmpty() && Date2.IsEmpty())
			return _T("");
		else if(!(Date1.IsEmpty()) && Date2.IsEmpty())
		{
			if(isSpecies)
				query.Format(_T(" AND I.DELETE_DATE = '%s'"), Date1);
			else
				query.Format(_T(" AND B.DELETE_DATE = '%s'"), Date1);
			return query;
		}
		else if(!(Date1.IsEmpty()) && !(Date2.IsEmpty()))
		{
			if(isSpecies)
			{
				if(Date1 == Date2)
					query.Format(_T(" AND I.DELETE_DATE = '%s'"), Date1);
				else
					query.Format(_T(" AND I.DELETE_DATE >= '%s' AND I.DELETE_DATE <= '%s'"), Date1, Date2);
			}
			else
			{
				if(Date1 == Date2)
					query.Format(_T(" AND B.DELETE_DATE = '%s'"), Date1);
				else
					query.Format(_T(" AND B.DELETE_DATE >= '%s' AND B.DELETE_DATE <= '%s'"), Date1, Date2);
			}
			return query;
		}
	}
	return _T("");
}
