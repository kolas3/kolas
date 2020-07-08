// DivisionNo_List_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_List_Dlg dialog


CDivisionNo_List_Dlg::CDivisionNo_List_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_List_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_List_Dlg)
	m_nMyData = -1;
	//}}AFX_DATA_INIT
	m_strAcqYear = _T("");
	m_strStatus = _T("");
	m_strSelDivNo = _T("");
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGroupCollection = NULL;
	m_arrDataStatus.RemoveAll();
	m_pComboMgr = NULL;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
	m_strWorkCode = _T("1");
}

CDivisionNo_List_Dlg::~CDivisionNo_List_Dlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
}

VOID CDivisionNo_List_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_List_Dlg)
	DDX_Radio(pDX, IDC_radDIVNO_MYDATA, m_nMyData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_List_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_List_Dlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnDIVNO_SEARCH, OnbtnDIVNOSEARCH)
	ON_BN_CLICKED(IDC_btnDIVNO_INFO, OnbtnDIVNOINFO)
	ON_CBN_SELCHANGE(IDC_cmbDIVNO_WORK_STATUS, OnSelchangecmbDIVNOWORKSTATUS)
	ON_CBN_SELCHANGE(IDC_cmbDIVNO_WORK_CODE, OnSelchangecmbDIVNOWORKCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_List_Dlg message handlers

BOOL CDivisionNo_List_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_구입차수보기")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_구입차수보기"));
	m_pDM = FindDM(_T("DM_구입차수보기"));

	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	m_pComboMgr = new CMulitComboBoxManager(this);

	m_nMyData = 0;
	UpdateData(FALSE);

	if(!Init())
	{
		ESLAfxMessageBox(_T("차수보기 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDivisionNo_List_Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

INT CDivisionNo_List_Dlg::MakeDM() 
{
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료상태"));
	INT nCurSel = pCMB->GetCurSel();
	if(nCurSel<0) return -1;
	m_strStatus = m_arrDataStatus.GetAt(nCurSel);
	CString strWorkCode;
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);

	CString strCreator;
	if(m_nMyData == 0) strCreator = m_pInfo->USER_NAME;
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempData(_T("AcquisitYear"), m_strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("Creator"), strCreator.GetBuffer(0));
	mgr.SetTempData(_T("WorkCode"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("DIVNO_MGR"), FALSE);
	INT ids = mgr.SPFExecutePath(_T("DM구성"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("차수정보를 구축할 수 없습니다."));
		return -1;
	}
	CString strQuery;
	mgr.GetTempData(_T("조건"), strQuery);

	mgr.InitTempData();	mgr.InitTempPtr();
	mgr.SetTempData(_T("조건"), strQuery.GetBuffer(0));
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	mgr.SetTempData(_T("SPECIES_TYPE"), (INT)m_nSpeciesType);
	mgr.SetTempData(_T("WORK_CODE"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("ACQ_YEAR"), m_strAcqYear.GetBuffer(0));
	ids = mgr.SPFExecutePath(_T("차수번호책수설정"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("책수정보를 얻어올 수 없습니다."));
		return -1;
	}
	
	DisplayGrid(_T("MainGrid"));
	return ids;
}

INT CDivisionNo_List_Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData;
	if(strDMFieldAlias == _T("UDF_작업상태설명"))
	{	
		strTmpData = m_pDM->GetCellData(_T("작업상태"), nRow);
		if(strTmpData == _T("0")) str = _T("구입대상");
		else if(strTmpData == _T("1")) str = _T("주문");
		else if(strTmpData == _T("2")) str = _T("검수완료");
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("업무구분"))
	{	
		if(str.IsEmpty()) str = _T("적용안함");
		else if(str == _T("1")) str = _T("동양서");
		else if(str == _T("2")) str = _T("서양서");
		else if(str == _T("3")) str = _T("비도서");
	}
	return 0;
}

VOID CDivisionNo_List_Dlg::DisplayGrid(CString gridAlias)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(gridAlias);
	if(!pGrid) return;
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
}

VOID CDivisionNo_List_Dlg::OnbtnDIVNOSEARCH() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pCM->GetControlMgrData(_T("수입년도"), m_strAcqYear);
	INT ids = MakeDM();
	if(ids < 0)
	{
		return;
	}
	DisplayGrid(_T("MainGrid"));
}

VOID CDivisionNo_List_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)pGrid);
	mgr.SetTempData(_T("InfoAlias"), _T("차수번호"));
	mgr.SetTempData(_T("YearAlias"), _T("수입년도"));
	mgr.SetTempData(_T("KeyAlias"), _T("업무구분"));	
	mgr.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	INT ids = mgr.SPFExecutePath(_T("선정자료추출"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	mgr.GetTempData(_T("SelInfo"), m_strSelDivNo);
	CDialog::OnOK();
}

VOID CDivisionNo_List_Dlg::OnbtnDIVNOINFO() 
{
	// TODO: Add your control notification handler code here
	BOOL bError = FALSE;
	CESL_Grid * m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) bError = TRUE;
	INT i = m_pGrid->SelectGetHeadPosition();
	if(i < 0) bError = TRUE;

	if(bError)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	
	CDivisionNo_DataView_Dlg dlg(this);
	dlg.SetMainCM(m_pCM);
	dlg.SetMainDM(m_pDM);
	dlg.DoModal();
}

BEGIN_EVENTSINK_MAP(CDivisionNo_List_Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDivisionNo_List_Dlg)
	ON_EVENT(CDivisionNo_List_Dlg, IDC_gridDIVNO_LIST, -601 /* DblClick */, OnDblClickgridDIVNOLIST, VTS_NONE)
	ON_EVENT(CDivisionNo_List_Dlg, IDC_gridDIVNO_LIST, -607 /* MouseUp */, OnMouseUpgridDIVNOLIST, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDivisionNo_List_Dlg::OnDblClickgridDIVNOLIST() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if(pGrid->GetMouseRow() == 0) return ;

	if(m_pDM->GetRowCount() == 0) return ;

	pGrid->SetAt(pGrid->GetRow()-1, 0, _T("V"));
	OnOK();
}

INT CDivisionNo_List_Dlg::SetDataStatus()
{
	if(m_strStatus.IsEmpty()) m_strStatus=_T("전체자료");

	m_arrDataStatus.Add(_T("전체자료"));
	m_arrDataStatus.Add(_T("구입대상"));
	m_arrDataStatus.Add(_T("주문"));
	m_arrDataStatus.Add(_T("검수완료"));
	
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_strStatus, &m_arrDataStatus, _T("자료상태"));

	if(m_strStatus == _T("전체자료")) GetDlgItem(IDC_cmbDIVNO_WORK_STATUS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbDIVNO_WORK_STATUS)->EnableWindow(FALSE);

	return 0;
}

BOOL CDivisionNo_List_Dlg::Init()
{
	SetDataStatus();
	m_pCM->SetControlMgrData(_T("업무구분"), m_strWorkCode);
	if(!m_strAcqYear.IsEmpty()) 
	{
		INT ids = MakeDM();
		if(ids < 0) return FALSE;
		m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	}
	
	return TRUE;
}

VOID CDivisionNo_List_Dlg::OnSelchangecmbDIVNOWORKSTATUS() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료상태"));
	INT nCurSel = pCMB->GetCurSel();
	if(nCurSel<0) return ;
	if(nCurSel == 0) m_nSpeciesType = BO_PURCHASE;
	else if(nCurSel == 1) m_nSpeciesType = BO_ORDER;
	else if(nCurSel == 2) m_nSpeciesType = SPECIES_TYPE_INVALID;
	
	MakeDM();
}

INT CDivisionNo_List_Dlg::SetBookCountDMField()
{
	CString strFunction;
	if(m_nSpeciesType == SPECIES_TYPE_INVALID)	strFunction = _T("GetPurchaseGroupBookCount('1', ACQ_YEAR, SEQ_NO, WORK_CODE, NULL)");
	else if(m_nSpeciesType == BO_PURCHASE)		strFunction = _T("GetPurchaseGroupBookCount('1', ACQ_YEAR, SEQ_NO, WORK_CODE, NULL)");
	else if(m_nSpeciesType == BO_ORDER)			strFunction = _T("GetPurchaseGroupBookCount('1', ACQ_YEAR, SEQ_NO, WORK_CODE, NULL)");
	else if(m_nSpeciesType == BO_MISSING)		strFunction = _T("GetPurchaseGroupBookCount('1', ACQ_YEAR, SEQ_NO, WORK_CODE, 'BOA113O')");
	else if(m_nSpeciesType == BO_REGOBJ)		strFunction = _T("GetPurchaseGroupBookCount('1', ACQ_YEAR, SEQ_NO, WORK_CODE, 'BOA114O')");
	
	INT nColCnt = m_pDM->m_nCols;
	if(nColCnt < 0) nColCnt = m_pDM->GetRefColumnCount();

	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	INT col = m_pDM->FindColumn(_T("책수"));
	if(col < 0 ) return -1;
	INT ids = m_pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
	if(ids != 0) return -1;
	FIELD_NAME = strFunction;
	ids = m_pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
	if(ids < 0 ) return -1;
	return 0;
}

VOID CDivisionNo_List_Dlg::OnSelchangecmbDIVNOWORKCODE() 
{
	// TODO: Add your control notification handler code here
	MakeDM();
}

BOOL CDivisionNo_List_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDC_btnDIVNO_INFO)->m_hWnd == pMsg->hwnd)
		{
			GetDlgItem(IDC_btnDIVNO_INFO)->SetFocus();
			PostMessage(WM_COMMAND, IDC_btnDIVNO_INFO, BN_CLICKED);
		}
		else if(GetDlgItem(IDOK)->m_hWnd == pMsg->hwnd)
		{
			GetDlgItem(IDOK)->SetFocus();
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		}
		else if(GetDlgItem(IDCANCEL)->m_hWnd == pMsg->hwnd)
		{
			GetDlgItem(IDCANCEL)->SetFocus();
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		}
		else
		{
			GetDlgItem(IDC_btnDIVNO_SEARCH)->SetFocus();
			PostMessage(WM_COMMAND, IDC_btnDIVNO_SEARCH, BN_CLICKED);
		}
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CDivisionNo_List_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CDivisionNo_List_Dlg::OnMouseUpgridDIVNOLIST(short Button, short Shift, long x, long y) 
{
	INT nSelectRow, nSelectCol;
	INT nRows;
	CString strChk;
	CMSHFlexGrid *pGrid;
	CESL_DataMgr *pTempDm;
	

	pGrid		= ( CMSHFlexGrid * )GetDlgItem( IDC_gridDIVNO_LIST );
	nSelectRow	= pGrid->GetRow();
	nSelectCol	= pGrid->GetCol();
	nRows		= pGrid->GetRows();
	strChk		= pGrid->GetText();
	pTempDm		= FindDM(_T("DM_주문메일_책"));
	
	if( NULL == pTempDm  )
	{
		return;
	}

	if ( 1 == Button && 1 == nSelectCol ) 
	{
		for( INT i = 1; i < nRows ; i++ )
		{
			if( nSelectRow == i )
				continue;
			
			pGrid->SetTextMatrix( i, 1, _T("") );
		}

		pGrid->SetTextMatrix( nSelectRow, nSelectCol, _T("V") );
	}
}