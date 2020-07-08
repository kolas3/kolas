// AccessControlUserDuplicate.cpp : implementation file
//

#include "stdafx.h"
#include "accesscontrol.h"
#include "AccessControlUserDuplicate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlUserDuplicate dialog


CAccessControlUserDuplicate::CAccessControlUserDuplicate(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlUserDuplicate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlUserDuplicate)
	m_strDeleteName = _T("");
	m_strSourceName = _T("");
	m_strTargetName = _T("");
	//}}AFX_DATA_INIT

	m_pGrid = NULL;
	m_BkBrush.CreateSolidBrush(RGB(255, 0, 0));
}


VOID CAccessControlUserDuplicate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlUserDuplicate)
	DDX_Control(pDX, IDC_PROGRESS1, m_pProgress1);
	DDX_Text(pDX, IDC_eDELETENAME, m_strDeleteName);
	DDX_Text(pDX, IDC_eSOURCENAME, m_strSourceName);
	DDX_Text(pDX, IDC_eTARGETNAME, m_strTargetName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlUserDuplicate, CDialog)
	//{{AFX_MSG_MAP(CAccessControlUserDuplicate)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bCOPY, OnbCOPY)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bREFRESH, OnbREFRESH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlUserDuplicate message handlers

BOOL CAccessControlUserDuplicate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pGrid = new CESL_Grid(NULL);
	m_pGrid->SubclassDlgItem(IDC_gLIST, this);
	m_pGrid->m_bFroceReverse = FALSE;

	ReadCfgFile.ReadCfgFile(_T(""));
	OnbREFRESH();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlUserDuplicate::OnbCLOSE() 
{	
	CDialog::OnOK();
}

VOID CAccessControlUserDuplicate::OnbCOPY() 
{
	UpdateData(TRUE);	

	TCHAR cSourceFlag = ' ', cTargetFlag = ' ';
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_rSOURCE1);
	CButton *pButton2 = (CButton*)GetDlgItem(IDC_rSOURCE2);
	CButton *pButton3 = (CButton*)GetDlgItem(IDC_rSOURCE3);
	CButton *pButton4 = (CButton*)GetDlgItem(IDC_rTARGET1);
	CButton *pButton5 = (CButton*)GetDlgItem(IDC_rTARGET2);
	CButton *pButton6 = (CButton*)GetDlgItem(IDC_rTARGET3);

	if (pButton1->GetCheck() == TRUE) cSourceFlag = 'U';
	if (pButton2->GetCheck() == TRUE) cSourceFlag = 'M';
	if (pButton3->GetCheck() == TRUE) cSourceFlag = 'S';
	if (pButton4->GetCheck() == TRUE) cTargetFlag = 'U';
	if (pButton5->GetCheck() == TRUE) cTargetFlag = 'M';
	if (pButton6->GetCheck() == TRUE) cTargetFlag = 'S';
	if (m_strSourceName == _T("Administrator")) {
		if (cSourceFlag != 'S') {
			AfxMessageBox(_T("Source의 Name이 [Administrator]인경우는 Source Flag가 [Super]이어야 합니다."));
			return;
		}
	}
	if (m_strTargetName == _T("Administrator")) {
		if (cTargetFlag != 'S') {
			AfxMessageBox(_T("Target의 Name이 [Administrator]인경우는 Target Flag가 [Super]이어야 합니다."));
			return;
		}
	}

	if (cSourceFlag == ' ') {
		AfxMessageBox(_T("Source의 Flag를 선택하여 주십시오."));
		return;
	}
	if (cTargetFlag == ' ') {
		AfxMessageBox(_T("Target의 Flag를 선택하여 주십시오."));
		return;
	}
	m_strSourceName.TrimLeft();			m_strSourceName.TrimRight();
	if (m_strSourceName.GetLength() == 0) {
		AfxMessageBox(_T("Source의 name을 입력하여 주십시오."));
		return;
	}
	m_strTargetName.TrimLeft();			m_strTargetName.TrimRight();
	if (m_strTargetName.GetLength() == 0) {
		AfxMessageBox(_T("Target의 name을 입력하여 주십시오."));
		return;
	}

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	INT ids = METHOD_New(&dm, cTargetFlag, m_strTargetName, cSourceFlag, m_strSourceName, &m_pProgress1);

	OnbREFRESH();

	CString strMsg;
	strMsg.Format(_T("Copy : Return value : [%d]"), ids);
	AfxMessageBox(strMsg);
	return;			
}

VOID CAccessControlUserDuplicate::OnbDELETE() 
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	
	UpdateData(TRUE);	

	TCHAR cDeleteFlag = ' ';
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_rDELETE1);
	CButton *pButton2 = (CButton*)GetDlgItem(IDC_rDELETE2);
	CButton *pButton3 = (CButton*)GetDlgItem(IDC_rDELETE3);

	if (pButton1->GetCheck()) cDeleteFlag = 'U';
	if (pButton2->GetCheck()) cDeleteFlag = 'M';
	if (pButton3->GetCheck()) cDeleteFlag = 'S';

	CString strQuery;
	dm.StartFrame();

	// ESL_MENU_METHOD_TBL 에서 삭제한다.
	if (cDeleteFlag == 'U') {
		strQuery.Format(_T("DELETE FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' ANd USER_ID='%s';"), m_strDeleteName);
		dm.AddFrame(strQuery);
	}
	if (cDeleteFlag == 'M') {
		strQuery.Format(_T("DELETE FROM ESL_MENU_METHOD_TBL WHERE FLAG='M' ANd METHOD_NAME='%s';"), m_strDeleteName);
		dm.AddFrame(strQuery);
	}

	// ESL_MENU_TBL에서 삭제
	strQuery.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG='%c' AND USER_ID='%s';"), cDeleteFlag, m_strDeleteName);
	dm.AddFrame(strQuery);

	// ESL_TOOLBAR_TBL에서 삭제
	strQuery.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE FLAG='%c' AND USER_ID='%s';"), cDeleteFlag, m_strDeleteName);
	dm.AddFrame(strQuery);

	INT ids = dm.SendFrame();
	dm.EndFrame();

	OnbREFRESH();

	CString strMsg;
	strMsg.Format(_T("Delete : Return value : [%d]"), ids);
	AfxMessageBox(strMsg);
}

// return values
// -10100 : 이미 같은 이름을 가진 method(user) 가 있음
INT CAccessControlUserDuplicate::METHOD_New(CESL_DataMgr *pDM, TCHAR cFlag, CString strName, TCHAR cImportSourceFlag, CString strImportSourceName, CProgressCtrl *pBar)
{
	INT ids;
	if (pDM == NULL) return -1;
	CString strQuery, str, strPK;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	INT i, nRows;

	if (cFlag == 'U') strQuery.Format(_T("SELECT COUNT(*) FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' AND USER_ID='%s'"), strName);
	if (cFlag == 'M') strQuery.Format(_T("SELECT COUNT(*) FROM ESL_MENU_METHOD_TBL WHERE FLAG='M' AND METHOD_NAME='%s'"), strName);
	
	if (pBar) pBar->SetPos(10);
	str = _T("");
	dm.GetOneValueQuery(strQuery, str);
	if (String_to_Integer(str) > 0) return -10100;			// 이미 같은 이름을 가진 method(user) 가 있음

	dm.StartFrame();
	// ESL_MENU_METHOD_TBL에 Insert
	str.Format(_T("%c"), cFlag);
	if (strName != _T("Administrator")) {
		dm.MakeRecKey(strPK);
		dm.InitDBFieldData();
		dm.AddDBFieldData(_T("PK"),						_T("NUMERIC"),			strPK);
		dm.AddDBFieldData(_T("FLAG"),					_T("STRING"),			str);
		if (cFlag == 'U') {
			dm.AddDBFieldData(_T("USER_ID"),				_T("STRING"),			strName);
			dm.AddDBFieldData(_T("METHOD_NAME"),			_T("STRING"),			_T(""));
			dm.AddDBFieldData(_T("USER_METHOD_NAME"),		_T("STRING"),			strImportSourceName);
		}
		if (cFlag == 'M') {
			dm.AddDBFieldData(_T("USER_ID"),				_T("STRING"),			_T(""));
			dm.AddDBFieldData(_T("METHOD_NAME"),			_T("STRING"),			strName);
			dm.AddDBFieldData(_T("USER_METHOD_NAME"),		_T("STRING"),			strImportSourceName);
		}
		dm.MakeInsertFrame(_T("ESL_MENU_METHOD_TBL"));
		ids = dm.SendFrame();
		dm.EndFrame();
		if (ids < 0) return ids;
	}
	if (strImportSourceName.GetLength() == 0) {
		return ids;
	}
	if (pBar) pBar->SetPos(20);

	dm.StartFrame();
	// ESL_MENU_TBL 을 복사함
	strQuery.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG = '%c' AND USER_ID='%s';") ,cFlag, strName);
	dm.AddFrame(strQuery);
	//                       0    1        2         3      4        5       6      7        8     9         10               11              12
	strQuery.Format(_T("SELECT PK, ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, FLAG, SPF_USE_FLAG, PROGRAME_USE_FLAG, TITLE_DESC FROM ESL_MENU_TBL WHERE FLAG='%c' AND USER_ID='%s'"), cImportSourceFlag, strImportSourceName);
	dm.RestructDataManager(strQuery);
	nRows = dm.GetRowCount();
	CString strTmp;
	for (i = 0;i < nRows; i++) {
		dm.MakeRecKey(strPK);
		dm.InitDBFieldData();
		dm.AddDBFieldData(_T("PK"),						_T("NUMERIC"),			strPK);
		str.Format(_T("%c"), cFlag);
		dm.AddDBFieldData(_T("FLAG"),					_T("STRING"),			str);
		dm.AddDBFieldData(_T("USER_ID"),				_T("STRING"),			strName);
		dm.AddDBFieldData(_T("ALIAS"),					_T("STRING"),			dm.GetCellData(i, 1));
		dm.AddDBFieldData(_T("GROUP_ALIAS"),			_T("STRING"),			dm.GetCellData(i, 2));
		dm.AddDBFieldData(_T("IDX"),					_T("STRING"),			dm.GetCellData(i, 3));
		dm.AddDBFieldData(_T("CAPTION"),				_T("STRING"),			dm.GetCellData(i, 4));
		dm.AddDBFieldData(_T("VISIBLE"),				_T("STRING"),			dm.GetCellData(i, 5));
		dm.AddDBFieldData(_T("STYLE"),					_T("STRING"),			dm.GetCellData(i, 6));
		dm.AddDBFieldData(_T("ACTION"),					_T("STRING"),			dm.GetCellData(i, 7));
		dm.AddDBFieldData(_T("TARGET"),					_T("STRING"),			GetMysqlString(&dm, dm.GetCellData(i, 8)));
		dm.AddDBFieldData(_T("SPF_USE_FLAG"),			_T("STRING"),			dm.GetCellData(i, 10));
		dm.AddDBFieldData(_T("PROGRAME_USE_FLAG"),		_T("STRING"),			dm.GetCellData(i, 11));
		dm.AddDBFieldData(_T("TITLE_DESC"),				_T("STRING"),			dm.GetCellData(i, 12));
		dm.MakeInsertFrame(_T("ESL_MENU_TBL"));

		if (i % 100 == 0) {
			ids = dm.SendFrame();
			if (ids < 0) return ids;
			dm.EndFrame();
			dm.StartFrame();
		}
	}
	// 남아있는 쿼리를 날린다.
	ids = dm.SendFrame();
	if (ids < 0) return ids;
	dm.EndFrame();

	if (pBar) pBar->SetPos(60);

	// ESL_TOOLBAR_TBL 을 복사함
	dm.StartFrame();
	strQuery.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE FLAG='%c' AND USER_ID='%s';"), cFlag, strName);
	dm.AddFrame(strQuery);
	//                         0       1        2             3            4    5      6       7      8    9      10           11            12         13              14              15               16
	strQuery.Format(_T("SELECT USER_ID, ALIAS, MENU_ALIAS, TOOLGROUP_ALIAS, FLAG, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED, TOOLTIP, SPF_USE_FLAG, PROGRAME_USE_FLAG, ACCECLERATOR FROM ESL_TOOLBAR_TBL WHERE FLAG='%c' AND USER_ID='%s'"), cImportSourceFlag, strImportSourceName);
	ids = dm.RestructDataManager(strQuery);

	nRows = dm.GetRowCount();
	str.Format(_T("%c"), cFlag);
	for (i = 0; i < nRows; i++) {
		dm.MakeRecKey(strPK);
		dm.InitDBFieldData();
		dm.AddDBFieldData(_T("PK"),						_T("NUMERIC"),			strPK);
		dm.AddDBFieldData(_T("USER_ID"),				_T("STRING"),			strName);
		dm.AddDBFieldData(_T("ALIAS"),					_T("STRING"),			dm.GetCellData(i, 1));
		dm.AddDBFieldData(_T("MENU_ALIAS"),				_T("STRING"),			dm.GetCellData(i, 2));
		dm.AddDBFieldData(_T("TOOLGROUP_ALIAS"),		_T("STRING"),			dm.GetCellData(i, 3));
		dm.AddDBFieldData(_T("FLAG"),					_T("STRING"),			str);
		dm.AddDBFieldData(_T("IDX"),					_T("STRING"),			dm.GetCellData(i, 5));
		dm.AddDBFieldData(_T("CAPTION"),				_T("STRING"),			dm.GetCellData(i, 6));
		dm.AddDBFieldData(_T("STYLE"),					_T("STRING"),			dm.GetCellData(i, 7));
		dm.AddDBFieldData(_T("TYPE"),					_T("STRING"),			dm.GetCellData(i, 8));
		dm.AddDBFieldData(_T("SM"),						_T("STRING"),			dm.GetCellData(i, 9));
		dm.AddDBFieldData(_T("BITMAP_NORMAL"),			_T("STRING"),			GetMysqlString(&dm, dm.GetCellData(i, 10)));
		dm.AddDBFieldData(_T("BITMAP_HOT"),				_T("STRING"),			GetMysqlString(&dm, dm.GetCellData(i, 11)));
		dm.AddDBFieldData(_T("BITMAP_DISABLED"),		_T("STRING"),			GetMysqlString(&dm, dm.GetCellData(i, 12)));
		dm.AddDBFieldData(_T("TOOLTIP"),				_T("STRING"),			dm.GetCellData(i, 13));
		dm.AddDBFieldData(_T("SPF_USE_FLAG"),			_T("STRING"),			dm.GetCellData(i, 14));
		dm.AddDBFieldData(_T("PROGRAME_USE_FLAG"),		_T("STRING"),			dm.GetCellData(i, 15));
		dm.AddDBFieldData(_T("ACCECLERATOR"),			_T("STRING"),			dm.GetCellData(i, 16));
		dm.MakeInsertFrame(_T("ESL_TOOLBAR_TBL"));

		if (i % 100 == 0) {
			ids = dm.SendFrame();
			if (ids < 0) return ids;
			dm.EndFrame();
			dm.StartFrame();
		}
	}
	// 남아있는 쿼리를 실행해준다.
	ids = dm.SendFrame();
	if (ids < 0) return ids;
	dm.EndFrame();

	if (pBar) pBar->SetPos(100);

	return 0;
}

INT CAccessControlUserDuplicate::String_to_Integer(CString str)
{
	if (str.GetLength() == 0) return 0;
	
	return _ttoi(str);
}

CString CAccessControlUserDuplicate::GetMysqlString(CESL_DataMgr *pDM, CString str)
{
	if (pDM == NULL) return str;

	if (pDM->m_dbkind == 2) {
		str.Replace(_T("\\"), _T("\\\\"));
		return str;
	} else {
		return str;
	}
}

VOID CAccessControlUserDuplicate::OnbREFRESH() 
{
	InitGrid();

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	
	INT ids = dm.RestructDataManager(_T("SELECT FLAG, USER_ID, METHOD_NAME FROM ESL_MENU_METHOD_TBL ORDER BY FLAG"));
	if (ids < 0) return;

	INT nRows = dm.GetRowCount();
	if (nRows == 0) m_pGrid->SetRows(1);
		else m_pGrid->SetRows(nRows);

	COLORREF cMETHOD = RGB(255, 255, 156);
	COLORREF cUSER = RGB(206, 255, 206);

	INT i, j;
	CString strFlag;
	for (i = 0; i < nRows; i++) {
		strFlag = dm.GetCellData(i, 0);
		m_pGrid->SetTextMatrix(i, 0, strFlag);
		if (strFlag == _T("M")) {
			m_pGrid->SetTextMatrix(i, 1, dm.GetCellData(i, 2));
		} else {
			m_pGrid->SetTextMatrix(i, 1, dm.GetCellData(i, 1));
		}

		m_pGrid->SetRow(i);
		for (j = 0; j < 2; j++) {
			m_pGrid->SetCol(j);
			if (strFlag == _T("M")) m_pGrid->SetCellBackColor(cMETHOD);
			if (strFlag == _T("U")) m_pGrid->SetCellBackColor(cUSER);
		}
	}

	return;
}

VOID CAccessControlUserDuplicate::InitGrid(VOID)
{
	m_pGrid->SetCols(0, 2);
	m_pGrid->SetRows(2);
	m_pGrid->SetFixedCols(0);
	m_pGrid->SetFixedRows(0);

	SetColumnInfo(m_pGrid, 0, _T(""), 40, 4);
	SetColumnInfo(m_pGrid, 1, _T(""), 100, 1);
	return;
}

BOOL CAccessControlUserDuplicate::DestroyWindow() 
{
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}

	return CDialog::DestroyWindow();
}

INT CAccessControlUserDuplicate::SetColumnInfo(CESL_Grid *pGrid, INT col, CString str, INT width, INT alignment)
{
	if (pGrid == NULL) return -1;

	pGrid->SetColAlignmentFixed(col, 4);
	if (str.GetLength() > 0) pGrid->SetTextMatrix(0, col, str);
	pGrid->SetColWidth(col, 0, width*15);
	pGrid->SetColAlignment(col, alignment);
	return 0;
}


HBRUSH CAccessControlUserDuplicate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_MSG01) {
		pDC->SetTextColor(RGB(255, 0, 0));
		return hbr;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
