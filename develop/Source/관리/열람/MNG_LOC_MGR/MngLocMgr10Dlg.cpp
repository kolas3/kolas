// MngLocMgr10Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr10Dlg.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr10Dlg dialog


CMngLocMgr10Dlg::CMngLocMgr10Dlg(CESL_Mgr* pParent , CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr10Dlg)
	//}}AFX_DATA_INIT
	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
	m_pEditCtrl = NULL;
	// 16/10/11 김혜영 : 공통>관리>업무환경설정>대출환경설정 메뉴 실행 후 종료시 메모리누수 현상 수정
//*/ ADD ---------------------------------------------------------------------------
	m_pEditCtrl1 = NULL;
//*/ END ---------------------------------------------------------------------------
}
CMngLocMgr10Dlg::~CMngLocMgr10Dlg()
{
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	if(m_pEditCtrl)
	{
		delete m_pEditCtrl;
		m_pEditCtrl = NULL;
	}
	// 16/10/11 김혜영 : 공통>관리>업무환경설정>대출환경설정 메뉴 실행 후 종료시 메모리누수 현상 수정
//*/ ADD ---------------------------------------------------------------------------
	if(m_pEditCtrl1)
	{
		delete m_pEditCtrl1;
		m_pEditCtrl1 = NULL;
	}
//*/ END ---------------------------------------------------------------------------
	
}

void CMngLocMgr10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr10Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr10Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr10Dlg)
		ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_eMNG10_BASIC_ARREAR, OnKillfocuseMNG10BASICARREAR)
	ON_EN_KILLFOCUS(IDC_eMNG10_MAX_LOAN_STOP_DAY, OnKillfocuseMNG10MAXLOANSTOPDAY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr10Dlg message handlers

BOOL CMngLocMgr10Dlg::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
	return FALSE;
}
INT CMngLocMgr10Dlg::ViewScreen()
{
EFS_BEGIN	

	INT ids;
	// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 4;
*///AFTER------------------------------------------------------------------------------
	const int nCnt = 5;
/*//END-------------------------------------------------------------------------------*/
	// 구분
	CString sSection[nCnt] = 
			{	_T("연체료"), _T("대출정지일"), _T("대출정지일"), _T("대출정지일") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
				, _T("연체관리")
//*/ END ---------------------------------------------------------------------------
			};
	// 기능
	CString sAbility[nCnt] = 
			{	_T("기본 연체료"), 
				_T("부록연체시 대출정지일 부여"),
				_T("대출정지일 휴관일 포함유무"),
				_T("최대대출정지부여일수") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
			   , _T("연체기간 휴관일 포함유무") 
//*/ END ---------------------------------------------------------------------------
			};
	// 설명
	CString sExplain[nCnt] = 
			{	_T("연체일 하루당 지불해야 하는 금액"), 
				_T("부록자료가 연체되었을 경우 대출정지일을 부여할지 결정"),
				_T("대출정지일에 휴관일을 포함한다"),
				_T("대출정지일 부여시 부여할 수 있는 최대일수 (0일:제한안함)") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
			   , _T("연체일에 휴관일을 포함한 기간을 적용한다") 
//*/ END ---------------------------------------------------------------------------
			};
	// Alias
	CString sAlias[nCnt] = 
			{	_T("연체관리"), _T("부록관리"), _T("연체관리"), _T("연체관리") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
				, _T("연체관리")
//*/ END ---------------------------------------------------------------------------
			};
	// 설정값
	CString sManage[nCnt] = 
			{	_T("연체료"),
				_T("부록연체적용여부"),
				_T("대출정지휴관일포함유무"),
				_T("최대대출정지부여일수") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
			   , _T("연체기간휴관일포함유무") 
//*/ END ---------------------------------------------------------------------------
			};

	// Display
	m_pDM->FreeData();
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("구분"), sSection[i], i ); 
		m_pDM->SetCellData( _T("기능"), sAbility[i], i );
		m_pDM->SetCellData( _T("설명"), sExplain[i], i );

		CString sGetData;
		ids = this->GetManageValue( _T("열람"), _T("공통"), sAlias[i], sManage[i], sGetData);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("관리설정값을 가져오는데 실패하였습니다."));
			return -1;
		}

		m_pDM->SetCellData( _T("설정값"), sGetData, i );
	}
	m_pCM->AllControlDisplay();
	/*
	CString sBasicArrear;
	CString sSupplement;
	CString sHolyday;
	INT ids = 0;
 
	ids = this->GetManageValue( "열람", "공통", "연체관리", "연체료", sBasicArrear );
	if(ids<0) return -1;
	ids = this->GetManageValue( "열람", "공통", "부록관리", "부록연체적용여부", sSupplement );
	if(ids<0) return -2;
	ids = this->GetManageValue( "열람", "공통", "연체관리", "대출정지휴관일포함유무", sHolyday );
	if(ids<0) return -3;

	((CEdit*)GetDlgItem(IDC_edtBASIC_ARREAR))->SetWindowText( sBasicArrear );
	
	if( sSupplement == "Y")
	{
		((CButton*)GetDlgItem(IDC_radSUPPLEMENT_Y))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_radSUPPLEMENT_N))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_radSUPPLEMENT_Y))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_radSUPPLEMENT_N))->SetCheck(1);
	}

	if( sHolyday == "Y" )
	{
		((CButton*)GetDlgItem(IDC_radHOLYDAY_Y))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_radHOLYDAY_N))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_radHOLYDAY_Y))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_radHOLYDAY_N))->SetCheck(1);
	}
	*/
		
	return 0;

EFS_END
	return -1;
}

INT CMngLocMgr10Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 4;
*///AFTER------------------------------------------------------------------------------
	const int nCnt = 5;
/*//END-------------------------------------------------------------------------------*/
	
	// Alias
	CString sAlias[nCnt] = 
			{	_T("연체관리"), _T("부록관리"), _T("연체관리"), _T("연체관리") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
				, _T("연체관리")
//*/ END ---------------------------------------------------------------------------
			};
	// 설정값
	CString sManage[nCnt] = 
			{	_T("연체료"),
				_T("부록연체적용여부"),
				_T("대출정지휴관일포함유무"),
				_T("최대대출정지부여일수") 
				// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
//*/ ADD ---------------------------------------------------------------------------
				, _T("연체기간휴관일포함유무")
//*/ END ---------------------------------------------------------------------------
			};

	CString sCode;
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->GetCellData( _T("설정값"), i, sCode );
		ids = this->SetManageValue( _T("열람"), _T("공통"), sAlias[i], sManage[i], sCode );
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("관리설정값을 저장하는데 실패하였습니다."));
			return -1;
		}
	}
	/*
	CString sBasicArrear;
	CString sSupplement;
	CString sHolyday;
	INT		ids;
	
	((CEdit*)GetDlgItem(IDC_edtBASIC_ARREAR))->GetWindowText(sBasicArrear);

	if( ((CButton*)GetDlgItem(IDC_radSUPPLEMENT_Y))->GetCheck())
		sSupplement = "Y";
	else
		sSupplement = "N";

	if( ((CButton*)GetDlgItem(IDC_radHOLYDAY_Y))->GetCheck() )
		sHolyday = "Y";
	else
		sHolyday = "N";

	ids = this->SetManageValue( "열람", "공통", "연체관리", "연체료", sBasicArrear );
	if(ids<0) return -1;
	ids = this->SetManageValue( "열람", "공통", "부록관리", "부록연체적용여부", sSupplement );
	if(ids<0) return -2;
	ids = this->SetManageValue( "열람", "공통", "연체관리", "대출정지휴관일포함유무", sHolyday );
	if(ids<0) return -3;
	*/
	return 0;

EFS_END
	return -1;	
}

BOOL CMngLocMgr10Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_대출환경설정_수수료관리")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_대출환경설정_수수료관리"));
	m_pDM = FindDM(_T("DM_대출환경설정_수수료관리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// 재대출제한일
	m_pEditCtrl = new CEdit();
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG10_BASIC_ARREAR);
	m_pEditCtrl->ShowWindow(SW_HIDE);
	// 최대대출정지부여일수
	m_pEditCtrl1 = new CEdit();
	m_pEditCtrl1->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG10_MAX_LOAN_STOP_DAY);
	m_pEditCtrl1->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE; 
}

HBRUSH CMngLocMgr10Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CMngLocMgr10Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_10);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

void CMngLocMgr10Dlg::OnKillfocuseMNG10BASICARREAR() 
{
	SetFocus();
	if (m_pEditCtrl == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl->GetSafeHwnd())) return ;

	m_pEditCtrl->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

void CMngLocMgr10Dlg::OnKillfocuseMNG10MAXLOANSTOPDAY() 
{
	SetFocus();
	if (m_pEditCtrl1 == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl1->GetSafeHwnd())) return ;

	m_pEditCtrl1->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl1->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

BEGIN_EVENTSINK_MAP(CMngLocMgr10Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr10Dlg)
	ON_EVENT(CMngLocMgr10Dlg, IDC_gridMNG_LOC_MGR_10, -600 /* Click */, OnClickgridMNGLOCMGR10, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr10Dlg::OnClickgridMNGLOCMGR10() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	if( col == 4 ) CreateComboBoxInGridSell();
}

// ** 콤보박스 생성 및 값 변경
VOID CMngLocMgr10Dlg::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData( _T("설정값"), nRow - 1 );
	CString strElement = m_pDM->GetCellData( _T("기능"), nRow - 1 );

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();

	// 1:기본연체료
	if( nRow == 1 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl);
		return;
	}
	// 17/04/17 김혜영 : 연체기간에 휴관일포함여부 속성 추가
/*//BEFORE-----------------------------------------------------------------------------
	// 2:부록연체적용여부, 3:대출정지휴관일포함유무
	else if( nRow == 2 || nRow == 3 )
*///AFTER------------------------------------------------------------------------------
	// 2:부록연체적용여부, 3:대출정지휴관일포함유무, 5:연체기간휴관일포함유무
	else if( nRow == 2 || nRow == 3 || nRow == 5 )
/*//END-------------------------------------------------------------------------------*/	
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("사용함"));
		ArrCmbData.Add(_T("사용안함"));
	}
	// 4:최대대출정지부여일수
	else if( nRow == 4 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl1);
		return;
	}
	else
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("적용함"));
		ArrCmbData.Add(_T("적용안함"));
	}

	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->InitMngLocMgr(10);
	m_pComboBox->SetIndex(nRow-1);
	m_pComboBox->SetDataMgr(m_pDM, m_pGrid);
	m_pComboBox->SetParent(m_pGrid);
	m_pComboBox->SetCurSelData(strData);
	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);
	
	CRect rect,gridRect;
	RECT parentRect;
	m_pGrid->GetWindowRect(&gridRect);
	this->GetClientRect(&parentRect);

	::ClientToScreen(this->m_hWnd,(POINT*)&(parentRect.left));
	
	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;
	rect.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - gridRect.left;
	rect.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - gridRect.top;
	rect.right = rect.left + (long)((float)m_pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)m_pGrid->GetCellHeight()/15.0f);

	m_pComboBox->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();
}

INT CMngLocMgr10Dlg::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
{
EFS_BEGIN

	pEditCtrl->GetSafeHwnd();

	if (pGrid == NULL) return -1;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if (nRow < 1 || nCol < 1) return -1;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// 텍스트 설정
	CString sValue = pGrid->GetTextMatrix(nRow, nCol);
	sValue.Replace( _T("원"), _T("") );
	sValue.Replace( _T("일"), _T("") );
	pEditCtrl->SetWindowText(sValue);

	pEditCtrl->MoveWindow(rcCell);
	pEditCtrl->ShowWindow(SW_SHOW);
	pEditCtrl->SetFocus();
	pEditCtrl->BringWindowToTop();
	
	return 0;

EFS_END
return -1;
}

INT CMngLocMgr10Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
EFS_BEGIN

	if (pGrid == NULL) return -1;

	CRect rcGrid;
	CRect rcClient;

	pGrid->GetWindowRect(rcGrid);

	GetClientRect(rcClient);

	ClientToScreen((POINT*)&(rcClient.left));

	rcGrid.top -= rcClient.top;
	rcGrid.left -= rcClient.left;

	rect.left	= rcGrid.left	+ (long)((float)pGrid->GetCellLeft() / 15.0f);
	rect.top	= rcGrid.top	+ (long)((float)pGrid->GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)pGrid->GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)pGrid->GetCellHeight() / 15.0f) - 1;

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr10Dlg::ApplyDataChange(LPCTSTR lpszData)
{
EFS_BEGIN

	if (lpszData == NULL) return -1;
	if (m_pDM == NULL || m_pGrid == NULL) return -1;

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();

	// 그리드에 변경을 적용시킨다.
	m_pGrid->SetFocus();
	m_pGrid->SetTextMatrix(nRow, nCol, lpszData);

	// 데이터 메니저에 변경을 적용시킨다.
	m_pDM->SetCellData( _T("설정값"), lpszData, nRow - 1 );
	m_pGrid->Display();

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr10Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("설정값"))
	{
		CString sAbility;
		m_pDM->GetCellData( _T("기능"), nRow, sAbility );

		// 해당기능의 설정값의 코드를 설명으로 변경한다
		CChangeValue *pMng = new CChangeValue();
		str = pMng->CodeToDesc( sAbility, str );

		delete pMng;
		pMng = NULL;
	}
	
	return 0;
}


