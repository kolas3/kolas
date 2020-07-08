// MNG_ILL_OPTION_MGR.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "MNG_ILL_OPTION_MGR.h"
#include "MNG_ILL_PERMISSION_LIST_DLG.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CANCEL_DIALOG(x,y)\
	{\
		SHOW_MESSAGE2(x,y);\
		CDialog::OnCancel();\
	}\

/////////////////////////////////////////////////////////////////////////////
// CMNG_ILL_OPTION_MGR dialog


CMNG_ILL_OPTION_MGR::CMNG_ILL_OPTION_MGR(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_ILL_OPTION_MGR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_ILL_OPTION_MGR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCmb = NULL;
	m_pEdit = NULL;
	m_pGrid = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
}
CMNG_ILL_OPTION_MGR::~CMNG_ILL_OPTION_MGR()
{	
	FreeComboBox();
	FreeEditBox();
}


void CMNG_ILL_OPTION_MGR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_ILL_OPTION_MGR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_ILL_OPTION_MGR, CDialog)
	//{{AFX_MSG_MAP(CMNG_ILL_OPTION_MGR)
	ON_WM_SIZE()
 	ON_MESSAGE(UPDATE_OPTION_DM, UpdateDM)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_ILL_OPTION_MGR message handlers

void CMNG_ILL_OPTION_MGR::CreateComboBoxInGridSell()
{
	int ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::CreateComboBoxInGridSell()");

	if(NULL == m_pGrid) return;
	if(NULL == m_pDM) return;

	FreeComboBox();

	///////////////////////////////////////////////////////////////////////////
	// 여기서부터 콤보박스 크기 지정
	CRect rGridClient, rGridWindow;
		
	m_pGrid->GetWindowRect(&rGridWindow);
	ScreenToClient(&rGridWindow);
	m_pGrid->GetClientRect(&rGridClient);

	const WIDTH_OFFSET = (rGridWindow.Width() - rGridClient.Width())/2;
	const HEIGHT_OFFSET = (rGridWindow.Height() - rGridClient.Height())/2;
	
	CRect rCmb;
	rCmb.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - WIDTH_OFFSET-2;
	rCmb.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - HEIGHT_OFFSET-2;
	rCmb.right = rCmb.left + (long)((float)m_pGrid->GetCellWidth()/15.0f)+2;
	rCmb.bottom = rCmb.top + (long)((float)m_pGrid->GetCellHeight()/15.0f);

	// 여기까지 콤보박스 크기 지정
	///////////////////////////////////////////////////////////////////////////

	UINT nFlag = WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_AUTOHSCROLL;

	m_pCmb = new CCmb_GridItem;	
	m_pCmb->m_pGrid = m_pGrid;
	m_pCmb->m_pDM = m_pDM;

	ids = m_pCmb->Create(nFlag, rCmb, m_pGrid, IDC_cmbILL_OPTION);
 	if(FALSE == ids) ERROR_MSG_RETURN_VOID2(-10, strFuncName);

 	m_pCmb->SetHorizontalExtent(10);
 	CFont * pFont = this->GetFont();
 	m_pCmb->SetFont(pFont);

	int nIdx = (int)m_pGrid->GetRow()-1;

	/////////////////////////////////////////////////////////////////////////
	// 여기서부터 콤보박스 데이터 설정
	CString strValueType;
	ids = m_pDM->GetCellData(_T("값형태"), nIdx, strValueType);
 	if(0 > ids) ERROR_MSG_RETURN_VOID2(-11, strFuncName);

	CString strValue;
	ids = m_pDM->GetCellData(_T("설정값"), nIdx, strValue);
 	if(0 > ids) ERROR_MSG_RETURN_VOID2(-12, strFuncName);

	
	if(_T("Y/N") == strValueType)
	{
		m_pCmb->AddString(_T("사용함"));
		m_pCmb->AddString(_T("사용안함"));

		int nIdx = (_T("Y") == strValue || _T("사용함") == strValue) ? 0 : 1;
		m_pCmb->SetCurSel(nIdx);
	}

	// 여기까지 콤보박스 데이터 설정
	/////////////////////////////////////////////////////////////////////////
	m_pCmb->ShowWindow(SW_SHOW);
	m_pCmb->SetFocus();

}

int CMNG_ILL_OPTION_MGR::Refresh()
{
	int ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::Refresh()");

	ids = m_pDM->RefreshDataManager(_T(""));
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	m_pGrid->SetRedraw(FALSE);
	ids = m_pGrid->Display();
	ShowValue_For_Grid();
	m_pGrid->SetRedraw(TRUE);

	return 0;
}

BOOL CMNG_ILL_OPTION_MGR::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	INT ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::OnInitDialog()");
	
	ids = InitESL_Mgr(_T("SM_ILL_OPTION_MGR"));
	if(0 > ids)
	{		
		CANCEL_DIALOG(-1000, _T("ERROR : InitESL_Mgr(\"SM_ILL_OPTION_MGR\")"));
	}
	m_pCM = FindCM(_T("CM_ILL_OPTION_MGR"));

	m_pGrid = (CESL_Grid*)m_pCM->FindControl( _T("OPTION_GRID"));
	if(NULL == m_pGrid) 
	{
		CANCEL_DIALOG(-1010, strFuncName);
	}

	m_pDM = FindDM(_T("DM_ILL_OPTION_MGR"));
	if(NULL == m_pDM) 
	{
		CANCEL_DIALOG(-1020, strFuncName);
	}

	m_pDM->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	ids = Refresh();
//	if(0 > ids) CANCEL_DIALOG(-3, strFuncName);
	
	// 2009.07.07 ADD BY PWR : 책두레운영모드(책두레 사용할때 운영모드 설정(I:통합, D:분산))
	CString strValue;
	ids = GetManageValue(_T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레사용여부"), strValue);
	if(0 > ids) 
	{
		CANCEL_DIALOG(-1030, strFuncName);
	}
	if(strValue.IsEmpty()) 
	{
		m_sBookCooperativeMode = _T("I");
	}
	else
	{
		if(strValue == _T("Y"))
		{ // 사용함
			ids = GetManageValue(_T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레운영모드"), strValue);
			if(0 > ids) 
			{
				CANCEL_DIALOG(-1040, strFuncName);
			}
			if(strValue.IsEmpty()) 
			{
				m_sBookCooperativeMode = _T("I");
			}
			else
			{
				m_sBookCooperativeMode = strValue;
			}
		}
		else
		{ // 사용안함
			m_sBookCooperativeMode = _T("I");
		}
	}
	// 분산시스템 변경적용 툴바 HIDE
	if(_T("D") == m_sBookCooperativeMode)
	{
		CString strButtonAlias = _T("변경적용");
		pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CMNG_ILL_OPTION_MGR, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_ILL_OPTION_MGR)
	ON_EVENT(CMNG_ILL_OPTION_MGR, IDC_gridMNG_ILL_OPTION_MGR, -600 /* Click */, OnClickgridMNGILLOPTIONMGR, VTS_NONE)
	ON_EVENT(CMNG_ILL_OPTION_MGR, IDC_gridMNG_ILL_OPTION_MGR, -601 /* DblClick */, OnDblClickMNGILLOPTIONMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMNG_ILL_OPTION_MGR::OnClickgridMNGILLOPTIONMGR() 
{
	// TODO: Add your control notification handler code here
	FreeComboBox();
	FreeEditBox();

	int nCol = (int)m_pGrid->GetCol();
	if(4 == nCol) 
	{
		CString strValueType;
		m_pDM->GetCellData(_T("값형태"), (int)m_pGrid->GetRow()-1, strValueType);
		if(_T("Y/N") == strValueType) CreateComboBoxInGridSell();
		else if(_T("자료실코드,") == strValueType || _T("등록구분,") == strValueType || _T("별치기호,") == strValueType)
		{
			ViewPermissionListDlg();
		}
		else if(_T("숫자") == strValueType)
		{
			CreateEditBoxInGridSell();
		}
	}
	
}

void CMNG_ILL_OPTION_MGR::OnOptionApply() 
{
	// TODO: Add your control notification handler code here
	int ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::OnOptionApply()");

	BOOL bIsModify = FALSE;
	m_pDM->StartFrame();

	const int ROW_COUNT = m_pDM->GetRowCount();
	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		CString strKey;
		CString strValue;

		ids = m_pDM->GetCellData(_T("변경여부"), nRow, strValue);
 		if(0 > ids) ERROR_MSG_RETURN_VOID2(-2, strFuncName);

		if(_T("") != strValue)
		{
			bIsModify = TRUE;

			ids = m_pDM->GetCellData(_T("REC_KEY"), nRow, strKey);
 			if(0 > ids) ERROR_MSG_RETURN_VOID2(-3, strFuncName);

			ids = m_pDM->GetCellData(_T("설정값"), nRow, strValue);
 			if(0 > ids) ERROR_MSG_RETURN_VOID2(-4, strFuncName);

			if(_T("사용함") == strValue) strValue = _T("Y");
			else if(_T("사용안함") == strValue) strValue = _T("N");

			m_pDM->InitDBFieldData();
			m_pDM->AddDBFieldData(_T("VALUE_1"), _T("STRING"), strValue);
			m_pDM->MakeUpdateFrame(_T("ILL_MANAGE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strKey);
//DEL 			m_pDM->MakeUpdateFrame(_T("ILL.ILL_MANAGE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strKey);
		}
	}
	
	if(TRUE == bIsModify)
	{
		ids = m_pDM->SendFrame();
 		if(0 > ids) ERROR_MSG_RETURN_VOID2(-5, strFuncName);	
	}

	m_pDM->EndFrame();

	AfxMessageBox(_T("적용되었습니다."));
}

BOOL CMNG_ILL_OPTION_MGR::Create(CWnd *pParent)
{
	return CDialog::Create(IDD, pParent);
}

void CMNG_ILL_OPTION_MGR::OnSize(UINT nType, int cx, int cy) 
{
	//2014-05-01 UPDATE BY BJY : 2014년 책두레 기능개선 1차
	//옵션 추가에 따라 그리드 크기 수정
	CDialog::OnSize(nType, cx, cy);
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_ILL_OPTION_MGR);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 40, cx-10, cy-45);		
	//pGrid->MoveWindow(-2, 0, cx, 220);
	pGrid->MoveWindow(-2, 0, cx, 260);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_STATIC1);
//  	pEdt->GetWindowRect(&rect);
// 	pEdt->MoveWindow(-2, 225, cx+2, 60);
//	pEdt->MoveWindow(0, 225, cx, 60);
	pEdt->MoveWindow(0, 265, cx, 60);
	pEdt->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_STATIC2);
// 	pEdt->GetWindowRect(&rect);
// 	pEdt->MoveWindow(-2, 290, cx+2, 50);
//	pEdt->MoveWindow(0, 290, cx, 50);
	pEdt->MoveWindow(0, 330, cx, 50);
	pEdt->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_STATIC3);
// 	pEdt->GetWindowRect(&rect);
// 	pEdt->MoveWindow(-2, 345, cx+2, 130);
//	pEdt->MoveWindow(0, 345, cx, 130);
	pEdt->MoveWindow(0, 385, cx, 130);
	pEdt->ShowWindow(SW_SHOW);


	
	// TODO: Add your message handler code here
	
}

void CMNG_ILL_OPTION_MGR::CreateEditBoxInGridSell()
{
	int ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::CreateEditBoxInGridSell()");

	if(NULL == m_pGrid) return;
	if(NULL == m_pDM) return;

	FreeEditBox();

	///////////////////////////////////////////////////////////////////////////
	// 여기서부터 콤보박스 크기 지정
	CRect rGridClient, rGridWindow;
		
	m_pGrid->GetWindowRect(&rGridWindow);
	ScreenToClient(&rGridWindow);
	m_pGrid->GetClientRect(&rGridClient);

	const WIDTH_OFFSET = (rGridWindow.Width() - rGridClient.Width())/2;
	const HEIGHT_OFFSET = (rGridWindow.Height() - rGridClient.Height())/2;
	
	CRect rCmb;
	rCmb.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - WIDTH_OFFSET;
	rCmb.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - HEIGHT_OFFSET;
	rCmb.right = rCmb.left + (long)((float)m_pGrid->GetCellWidth()/15.0f)-2;
	rCmb.bottom = rCmb.top + (long)((float)m_pGrid->GetCellHeight()/15.0f)-2;

	// 여기까지 콤보박스 크기 지정
	///////////////////////////////////////////////////////////////////////////

	UINT nFlag = WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER;
 
	m_pEdit = new CEdit_GridItem;	
	m_pEdit->m_pGrid = m_pGrid;
	m_pEdit->m_pDM = m_pDM;

	ids = m_pEdit->Create(nFlag, rCmb, m_pGrid, IDC_cmbILL_OPTION);
 	if(FALSE == ids) ERROR_MSG_RETURN_VOID2(-10, strFuncName);

 	CFont * pFont = this->GetFont();
 	m_pEdit->SetFont(pFont);

	int nIdx = (int)m_pGrid->GetRow()-1;

	/////////////////////////////////////////////////////////////////////////
	// 여기서부터 에디트 데이터 설정

	CString strValue;
	ids = m_pDM->GetCellData(_T("설정값"), nIdx, strValue);
 	if(0 > ids) ERROR_MSG_RETURN_VOID2(-12, strFuncName);

	m_pEdit->SetWindowText(strValue);
	m_pEdit->LimitText(3);	// 3자 이하만 설정
	m_pEdit->SetSel(0, -1);
	// 여기까지 에디트 데이터 설정
	/////////////////////////////////////////////////////////////////////////

	m_pEdit->ShowWindow(SW_SHOW);
	m_pEdit->SetFocus();
}

int CMNG_ILL_OPTION_MGR::FreeEditBox()
{
	if(NULL != m_pEdit)
	{
		if(NULL != m_pEdit->GetSafeHwnd())
		{
			m_pEdit->DestroyWindow();
			delete m_pEdit;
		}			
		m_pEdit = NULL;
	}

	return 0;
}

int CMNG_ILL_OPTION_MGR::GetValueToCode(int nRow, CString &strResult)
{
	int ids = -1;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::GetValueToCode");

 	if(NULL == m_pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
 	if(0 > nRow || m_pDM->GetRowCount() <= nRow) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

	CString strValueType;
	ids = m_pDM->GetCellData(_T("값형태"), nRow, strValueType);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	
	CString strValue = m_pGrid->GetTextMatrix(nRow+1, 4);
	
	if(_T("사용함") == strValue || _T("예") == strValue)
	{
		strResult = _T("Y");
		return 0;
	}
	else if(_T("사용안함") == strValue || _T("아니요") == strValue)
	{
		strResult = _T("N");
		return 0;
	}
	else if(_T("숫자") == strValueType)
	{
		strResult.Format(_T("%d"), _ttoi(strValue));

		return 0;
	}
	else return 1;
	
	return 0;
}

int CMNG_ILL_OPTION_MGR::UpdateDM(WPARAM wParam, LPARAM lParam)
{
	CString strGet;
	const int ROW_COUNT = m_pDM->GetRowCount();
	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		m_pDM->GetCellData(_T("변경여부"), nRow, strGet);

		if(_T("Y") == strGet)
		{
			CString strValue;
			if(0 == GetValueToCode(nRow, strValue))
			{
				m_pDM->SetCellData(_T("설정값"), strValue, nRow);
			}

		}
	}

	return 0;
}

int CMNG_ILL_OPTION_MGR::ShowValue_For_Grid()
{
	const int ROW_COUNT = m_pDM->GetRowCount();
	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		CString strValue;
		GetCodeToValue(nRow, strValue);
		m_pGrid->SetTextMatrix(nRow+1, 4, strValue);
	}
	return 0;
}

int CMNG_ILL_OPTION_MGR::GetCodeToValue(int nRow, CString &strResult)
{
	int ids = -1;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::GetCodeToAlias");

 	if(NULL == m_pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
 	if(0 > nRow || m_pDM->GetRowCount() <= nRow) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

	CString strValueType;
	ids = m_pDM->GetCellData(_T("값형태"), nRow, strValueType);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

	CString strValue;
	ids = m_pDM->GetCellData(_T("설정값"), nRow, strValue);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

	CString strValueName;
	ids = m_pDM->GetCellData(_T("기능"), nRow, strValueName);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);

	if(_T("Y/N") == strValueType)
	{
		if(_T("Y") == strValue || _T("y") == strValue || _T("사용함") == strValue)
		{ 
			strResult = _T("사용함"); 
		}
		else if(_T("N") == strValue || _T("n") == strValue || _T("사용안함") == strValue)
		{ 
			strResult = _T("사용안함"); 
		}
 		else 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);
		}

		return 0;
	}
	else if(_T("자료실코드,") == strValueType || _T("등록구분,") == strValueType || _T("별치기호,") == strValueType)
	{
		strResult = _T("설정");

		return 0;
	}
	else if(_T("신청제한되는복귀(만료)횟수") == strValueName)
	{
		strResult.Format(_T("%d회"), _ttoi(strValue));
	}
	else if(_T("복귀(만료)로신청제한시제한일수") == strValueName || _T("책두레대출대기기간") == strValueName)
	{
		strResult.Format(_T("%d일"), _ttoi(strValue));		
	}
	else if(_T("책두레최대신청가능책수") == strValueName)
	{
		strResult.Format(_T("%d권"), _ttoi(strValue));		
	}
	else return 1;
	

	return 0;
}

int CMNG_ILL_OPTION_MGR::FreeComboBox()
{
	if(NULL != m_pCmb)
	{
		if(NULL != m_pCmb->GetSafeHwnd())
		{
			m_pCmb->DestroyWindow();
			delete m_pCmb;
		}			
		m_pCmb = NULL;
	}

	return 0;
}

int CMNG_ILL_OPTION_MGR::ViewPermissionListDlg()
{
	CString strResult;
	m_pDM->GetCellData(_T("값형태"), (int)m_pGrid->GetRow()-1, strResult);

	CString strValue;		
	
 	CMNG_ILL_PERMISSION_LIST_DLG Dlg(this);
 	Dlg.m_pInfo = m_pInfo;
 	Dlg.m_strValueType = strResult;
 	
 	m_pDM->GetCellData(_T("설정값"), (int)m_pGrid->GetRow()-1, strValue);
 	Dlg.m_strValue = strValue;
 	
 	m_pDM->GetCellData(_T("REC_KEY"), (int)m_pGrid->GetRow()-1, strValue);
 	Dlg.m_strRecKey = strValue;
 	
 	int nID = Dlg.DoModal();
 	if(IDOK == nID)
 	{
 		Refresh();
 	}

	return 0;
}

void CMNG_ILL_OPTION_MGR::OnDblClickMNGILLOPTIONMGR()
{
	ShowValue_For_Grid();
}