// MNG_ACCESS_CONTROL_5000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_ACCESS_CONTROL_5000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MNG_ACCESS_CONTROL_6000.h"

/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_5000 dialog


CMNG_ACCESS_CONTROL_5000::CMNG_ACCESS_CONTROL_5000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_ACCESS_CONTROL_5000)
	m_strMethodName = _T("");
	//}}AFX_DATA_INIT
}
CMNG_ACCESS_CONTROL_5000::~CMNG_ACCESS_CONTROL_5000()
{	
	
}


VOID CMNG_ACCESS_CONTROL_5000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_ACCESS_CONTROL_5000)
	DDX_Control(pDX, IDC_PROGRESS1, m_pProgress1);
	DDX_Control(pDX, IDC_cIMPORT_METHOD_NAME, m_cImportMethodName);
	DDX_Text(pDX, IDC_eMETHOD_NAME, m_strMethodName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_ACCESS_CONTROL_5000, CDialog)
	//{{AFX_MSG_MAP(CMNG_ACCESS_CONTROL_5000)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_bADD_METHOD, OnbADDMETHOD)
	ON_BN_CLICKED(IDC_bDELETE_METHOD, OnbDELETEMETHOD)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bTEST, OnbTEST)
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDBLCLK()
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_ACCESS_CONTROL_5000 message handlers

BOOL CMNG_ACCESS_CONTROL_5000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("MNG_ACCESS_CONTROL_5000")) < 0) {
		AfxMessageBox(_T("화면 초기화에 실패하였습니다. : [MNG_ACCESS_CONTROL_5000]"));
		return TRUE;
	}

	SetTimer(0, 100, NULL);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_ACCESS_CONTROL_5000::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		REFRESH_List();
	}
	CDialog::OnTimer(nIDEvent);
}

VOID CMNG_ACCESS_CONTROL_5000::OnbADDMETHOD() 
{
	UpdateData(TRUE);
	m_strMethodName.TrimLeft();			m_strMethodName.TrimRight();
	if (m_strMethodName.GetLength() == 0) {
		AfxMessageBox(_T("새 디폴트권한 이름을 입력하셔야 합니다."));
		return;
	}
	
	CString strImportSourceName;
	m_cImportMethodName.GetWindowText(strImportSourceName);
	strImportSourceName.TrimLeft();				strImportSourceName.TrimRight();
	INT ids = METHOD_New('M', m_strMethodName, 'M', strImportSourceName, &m_pProgress1);
	m_pProgress1.SetPos(100);
	UpdateData(FALSE);
	if (ids == -10100) {
		AfxMessageBox(_T("이미 쓰이는 이름입니다. 중복된 이름을 쓰일 수 없습니다."));
		return;
	}
	if (ids < 0) {
		AfxMessageBox(_T("DB 작업중 에러가 발생하였습니다."));
		return;
	}
	if (ids == 0) {
		REFRESH_List();
		AfxMessageBox(_T("추가하였습니다."));
		return;
	}
}

VOID CMNG_ACCESS_CONTROL_5000::OnbDELETEMETHOD() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_ACCESS_CONTROL_5000"));
	if (pDM == NULL) return;
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_MNG_ACCESS_CONTROL_5000"), _T("목록"));
	if (pGrid == NULL) return;

	if (pDM->GetRowCount() == 0) {
		AfxMessageBox(_T("디폴트권한이 없습니다."));
		return;
	}
	INT nSelectedRow = pGrid->GetRow() - 1;
	CString strMethodName = pDM->GetCellData(_T("METHOD_NAME"), nSelectedRow);
	if (strMethodName == _T("기본권한")) {
		AfxMessageBox(_T("[기본권한]은 삭제할 수 없습니다."));
		return;
	}
	CString strMsg;
	strMsg.Format(_T("디폴트권한 [%s]을(를) 정말로 지우시겠습니까?"), strMethodName);
	if (AfxMessageBox(strMsg, MB_YESNO) != IDYES) return;

	INT ids = METHOD_Delete('M', strMethodName);
	if (ids == 0) {
		REFRESH_List();
		AfxMessageBox(_T("디폴트권한을 삭제하였습니다."));
	} else {
		AfxMessageBox(_T("디폴트권한을 삭제도중 에러가 발생하였습니다."));
	}

	return;
}

VOID CMNG_ACCESS_CONTROL_5000::OnbCLOSE() 
{
	CDialog::OnOK();
}


INT CMNG_ACCESS_CONTROL_5000::REFRESH_List(VOID)
{
	m_strMethodName = _T("");
	UpdateData(FALSE);
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_ACCESS_CONTROL_5000"));
	if (pDM == NULL) return -1;
	CString strOption = _T("");
	pDM->RefreshDataManager(strOption);

	AllControlDisplay(_T("CM_MNG_ACCESS_CONTROL_5000"));
	REFRESH_ComboList();
	return 0;
}

INT CMNG_ACCESS_CONTROL_5000::REFRESH_ComboList(VOID)
{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_ACCESS_CONTROL_5000"));
	if (pDM == NULL) return -1;

	m_cImportMethodName.ResetContent();
	INT nRows = pDM->GetRowCount();
	for (INT i = 0; i < nRows; i++) {
		m_cImportMethodName.AddString(pDM->GetCellData(_T("METHOD_NAME"), i));
	}
	m_cImportMethodName.SetCurSel(0);

	UpdateData(FALSE);
	return 0;
}

// return values
// -10100 : 이미 같은 이름을 가진 method(user) 가 있음
INT CMNG_ACCESS_CONTROL_5000::METHOD_New(TCHAR cFlag, CString strName, TCHAR cImportSourceFlag, CString strImportSourceName, CProgressCtrl *pBar)
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_ACCESS_CONTROL_5000"));
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
	dm.MakeRecKey(strPK);
	dm.InitDBFieldData();
	dm.AddDBFieldData(_T("PK"),						_T("NUMERIC"),			strPK);
	str.Format(_T("%c"), cFlag);
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
	if (strImportSourceName.GetLength() == 0) {
		return ids;
	}
	if (pBar) pBar->SetPos(20);

	dm.StartFrame();
	// ESL_MENU_TBL 을 복사함
	strQuery.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG = '%c' AND USER_ID='%s';") ,cFlag, strName);
	dm.AddFrame(strQuery);
	//                       0    1        2         3      4        5       6      7        8     9         10               11             12
	strQuery.Format(_T("SELECT PK, ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, FLAG, SPF_USE_FLAG, PROGRAME_USE_FLAG, TITLE_DESC FROM ESL_MENU_TBL WHERE FLAG='%c' AND USER_ID='%s'"), cImportSourceFlag, strImportSourceName);
	dm.RestructDataManager(strQuery);
	nRows = dm.GetRowCount();
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
		dm.AddDBFieldData(_T("TARGET"),					_T("STRING"),			dm.GetCellData(i, 8));
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

	ids = dm.SendFrame();
	if (ids < 0) return ids;
	dm.EndFrame();
	if (pBar) pBar->SetPos(60);

	// ESL_TOOLBAR_TBL 을 복사함
	dm.StartFrame();
	strQuery.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE FLAG='%c' AND USER_ID='%s';"), cFlag, strName);
	dm.AddFrame(strQuery);
	//                         0       1        2             3            4    5      6       7      8    9      10           11            12         13              14              15     
	strQuery.Format(_T("SELECT USER_ID, ALIAS, MENU_ALIAS, TOOLGROUP_ALIAS, FLAG, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED, TOOLTIP, SPF_USE_FLAG, PROGRAME_USE_FLAG FROM ESL_TOOLBAR_TBL WHERE FLAG='%c' AND USER_ID='%s'"), cImportSourceFlag, strImportSourceName);
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
		dm.AddDBFieldData(_T("BITMAP_NORMAL"),			_T("STRING"),			dm.GetCellData(i, 10));
		dm.AddDBFieldData(_T("BITMAP_HOT"),				_T("STRING"),			dm.GetCellData(i, 11));
		dm.AddDBFieldData(_T("BITMAP_DISABLED"),		_T("STRING"),			dm.GetCellData(i, 12));
		dm.AddDBFieldData(_T("TOOLTIP"),				_T("STRING"),			dm.GetCellData(i, 13));
		dm.AddDBFieldData(_T("SPF_USE_FLAG"),			_T("STRING"),			dm.GetCellData(i, 14));
		dm.AddDBFieldData(_T("PROGRAME_USE_FLAG"),		_T("STRING"),			dm.GetCellData(i, 15));
		dm.MakeInsertFrame(_T("ESL_TOOLBAR_TBL"));

		if (i % 100 == 0) {
			ids = dm.SendFrame();
			if (ids < 0) return ids;
			dm.EndFrame();
			dm.StartFrame();
		}
	}

	ids = dm.SendFrame();
	if (ids < 0) return ids;
	dm.EndFrame();
	if (pBar) pBar->SetPos(100);

	return 0;
}

INT CMNG_ACCESS_CONTROL_5000::String_to_Integer(CString str)
{
	if (str.GetLength() == 0) return 0;
	
	return _ttoi(str);
}

INT CMNG_ACCESS_CONTROL_5000::METHOD_Delete(TCHAR cFlag, CString strName)
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_ACCESS_CONTROL_5000"));
	if (pDM == NULL) return -1;
	CString strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	dm.StartFrame();
	// ESL_MENU_METHOD_TBL 에서 삭제함
	strQuery = _T("");
	if (cFlag == 'U') strQuery.Format(_T("DELETE FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' AND USER_ID='%s';"), strName);
	if (cFlag == 'M') strQuery.Format(_T("DELETE FROM ESL_MENU_METHOD_TBL WHERE FLAG='M' AND METHOD_NAME='%s';"), strName);
	if (strQuery.GetLength() > 0) dm.AddFrame(strQuery);

	// ESL_MENU_TBL 을 삭제함
	strQuery.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG = '%c' AND USER_ID='%s';") ,cFlag, strName);
	dm.AddFrame(strQuery);

	// ESL_TOOLBAR_TBL 을 삭제함
	strQuery.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE FLAG='%c' AND USER_ID='%s';"), cFlag, strName);
	dm.AddFrame(strQuery);

	ids = dm.SendFrame();
	dm.EndFrame();
	return ids;
}

VOID CMNG_ACCESS_CONTROL_5000::OnbTEST() 
{
	FILE *fd = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if (fd) {
		fclose(fd);
	} else {
		return;
	}
	CMNG_ACCESS_CONTROL_6000 dlg(this);
	dlg.DoModal();
}

VOID CMNG_ACCESS_CONTROL_5000::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

VOID CMNG_ACCESS_CONTROL_5000::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	OnbTEST();
	
	CDialog::OnRButtonDblClk(nFlags, point);
}




HBRUSH CMNG_ACCESS_CONTROL_5000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
