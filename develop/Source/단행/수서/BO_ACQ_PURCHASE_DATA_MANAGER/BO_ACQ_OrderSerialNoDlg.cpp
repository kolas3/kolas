// BO_ACQ_OrderSerialNoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_OrderSerialNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderSerialNoDlg dialog


CBO_ACQ_OrderSerialNoDlg::CBO_ACQ_OrderSerialNoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_OrderSerialNoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_OrderSerialNoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_arrSort1.Add(_T("구입처"));
	m_arrSort1.Add(_T("주제명"));
	m_arrSort1.Add(_T("발행자"));
	m_arrSort1.Add(_T("서명"));
	m_arrSort1.Add(_T("이용대상구분"));
	m_arrSort1.Add(_T("관리구분"));
	m_arrSort1.Add(_T("사용안함"));
	m_arrSort2.Add(_T("주제명"));
	m_arrSort2.Add(_T("발행자"));
	m_arrSort2.Add(_T("서명"));
	m_arrSort2.Add(_T("이용대상구분"));
	m_arrSort2.Add(_T("관리구분"));
	m_arrSort2.Add(_T("사용안함"));
	m_arrSort3.Add(_T("발행자"));
	m_arrSort3.Add(_T("서명"));
	m_arrSort3.Add(_T("이용대상구분"));
	m_arrSort3.Add(_T("관리구분"));
	m_arrSort3.Add(_T("사용안함"));
	m_arrSort4.Add(_T("서명"));
	m_arrSort4.Add(_T("이용대상구분"));
	m_arrSort4.Add(_T("관리구분"));
	m_arrSort4.Add(_T("사용안함"));
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_arrSortCond.Add(_T("오름차순"));
	m_arrSortCond.Add(_T("내림차순"));
	m_MapSortData.SetAt(_T("서명"), _T("IBS_본표제"));
	m_MapSortData.SetAt(_T("주제명"), _T("BP_주제구분"));
	m_MapSortData.SetAt(_T("발행자"), _T("IBS_발행자"));
	m_MapSortData.SetAt(_T("이용대상구분"), _T("IBS_이용대상구분"));
	m_MapSortData.SetAt(_T("관리구분"), _T("BB_관리구분"));
	m_MapSortData.SetAt(_T("구입처"), _T("BP_구입처부호"));
}

CBO_ACQ_OrderSerialNoDlg::~CBO_ACQ_OrderSerialNoDlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
}

VOID CBO_ACQ_OrderSerialNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_OrderSerialNoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_OrderSerialNoDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_OrderSerialNoDlg)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_1, OnSelchangecmbORDERSORT1)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_2, OnSelchangecmbORDERSORT2)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_3, OnSelchangecmbORDERSORT3)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_4, OnSelchangecmbORDERSORT4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderSerialNoDlg message handlers

BOOL CBO_ACQ_OrderSerialNoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_주문일련번호")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_주문일련번호"));	
	if(!Init())
	{
		ESLAfxMessageBox(_T("화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_OrderSerialNoDlg::OnSelchangecmbORDERSORT1() 
{
	// TODO: Add your control notification handler code here
	CString strCurSel, strTmpData;
	m_pCM->GetControlMgrData(_T("제1조건"), strCurSel);
	m_arrSort2.RemoveAll();
	INT cnt = m_arrSort1.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrSort1.GetAt(i);
		if(strTmpData == strCurSel && strTmpData != _T("사용안함")) continue;
		else m_arrSort2.Add(strTmpData);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort2.GetAt(0), &m_arrSort2, _T("제2조건"));
	OnSelchangecmbORDERSORT2();
}

VOID CBO_ACQ_OrderSerialNoDlg::OnSelchangecmbORDERSORT2() 
{
	// TODO: Add your control notification handler code here
	CString strCurSel, strTmpData;
	m_pCM->GetControlMgrData(_T("제2조건"), strCurSel);
	m_arrSort3.RemoveAll();
	INT cnt = m_arrSort2.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrSort2.GetAt(i);
		if(strTmpData == strCurSel && strTmpData != _T("사용안함")) continue;
		else m_arrSort3.Add(strTmpData);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort3.GetAt(0), &m_arrSort3, _T("제3조건"));
	OnSelchangecmbORDERSORT3();	
}

VOID CBO_ACQ_OrderSerialNoDlg::OnSelchangecmbORDERSORT3() 
{
	// TODO: Add your control notification handler code here
	CString strCurSel, strTmpData;
	m_pCM->GetControlMgrData(_T("제3조건"), strCurSel);
	m_arrSort4.RemoveAll();
	INT cnt = m_arrSort3.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrSort3.GetAt(i);
		if(strTmpData == strCurSel && strTmpData != _T("사용안함")) continue;
		else m_arrSort4.Add(strTmpData);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort4.GetAt(0), &m_arrSort4, _T("제4조건"));
}

VOID CBO_ACQ_OrderSerialNoDlg::OnSelchangecmbORDERSORT4() 
{
	// TODO: Add your control notification handler code here
	
}

VOID CBO_ACQ_OrderSerialNoDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strTmpData, strAlias, strTmpCond;
	m_pCM->GetControlMgrData(_T("제1조건"), strTmpData);
	if(strTmpData != _T("사용안함")) 
	{
		m_pCM->GetControlMgrData(_T("제1정렬조건"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strAlias);
		m_SortCondition.m_strFirstAlias = strAlias;
		if(strTmpCond == _T("내림차순")) m_SortCondition.m_nFirstType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nFirstType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nFirstType = 0;

	m_pCM->GetControlMgrData(_T("제2조건"), strTmpData);
	if(strTmpData != _T("사용안함")) 
	{
		m_pCM->GetControlMgrData(_T("제2정렬조건"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strAlias);
		m_SortCondition.m_strSecondAlias= strAlias;
		if(strTmpCond == _T("내림차순")) m_SortCondition.m_nSecondType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nSecondType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nSecondType = 0;

	m_pCM->GetControlMgrData(_T("제3조건"), strTmpData);
	if(strTmpData != _T("사용안함")) 
	{
		m_pCM->GetControlMgrData(_T("제3정렬조건"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strAlias);
		m_SortCondition.m_strThirdAlias = strAlias;
		if(strTmpCond == _T("내림차순")) m_SortCondition.m_nThirdType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nThirdType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nThirdType = 0;
	
	m_pCM->GetControlMgrData(_T("제4조건"), strTmpData);
	if(strTmpData != _T("사용안함")) 
	{
		m_pCM->GetControlMgrData(_T("제4정렬조건"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strAlias);
		m_SortCondition.m_strFourthAlias = strAlias;
		if(strTmpCond == _T("내림차순")) m_SortCondition.m_nFourthType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nFourthType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nFourthType = 0;
	
	CDialog::OnOK();
}

BOOL CBO_ACQ_OrderSerialNoDlg::Init()
{
	SetComboData();
	return TRUE;
}

VOID CBO_ACQ_OrderSerialNoDlg::SetComboData()
{
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort1.GetAt(0), &m_arrSort1, _T("제1조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort2.GetAt(0), &m_arrSort2, _T("제2조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort3.GetAt(0), &m_arrSort3, _T("제3조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort4.GetAt(0), &m_arrSort4, _T("제4조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSortCond.GetAt(0), &m_arrSortCond, _T("제1정렬조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSortCond.GetAt(0), &m_arrSortCond, _T("제2정렬조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSortCond.GetAt(0), &m_arrSortCond, _T("제3정렬조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSortCond.GetAt(0), &m_arrSortCond, _T("제4정렬조건"));
	
}

CString CBO_ACQ_OrderSerialNoDlg::GetFieldName(CString strAlias)
{
	CString strFieldName;
	if(strAlias == _T("주제명")) strFieldName = _T("P.SUBJECT_CODE");
	else if(strAlias == _T("구입처")) strFieldName = _T("P.PROVIDER_CODE");
	else if(strAlias == _T("발행자")) strFieldName = _T("I.PUBLISHER");
	else if(strAlias == _T("서명")) strFieldName = _T("I.TITLE");
	else if(strAlias == _T("이용대상구분")) strFieldName = _T("I.USE_OBJ_CODE");
	else if(strAlias == _T("관리구분")) strFieldName = _T("I.MANAGE_CODE");
	else strFieldName.Empty();
	return strFieldName;
}

CString CBO_ACQ_OrderSerialNoDlg::GetSortCondition(CString strAlias)
{
	CString strCondition;
	if(strAlias == _T("오름차순")) strCondition = _T("DESC");
	else if(strAlias == _T("내림차순")) strCondition = _T("ASC");
	else strCondition.Empty();
	return strCondition;
}