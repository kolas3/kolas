// MngLocMgr06Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr06Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr06Dlg dialog


CMngLocMgr06Dlg::CMngLocMgr06Dlg(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr06Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;
}
CMngLocMgr06Dlg::~CMngLocMgr06Dlg()
{
	
}

VOID CMngLocMgr06Dlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr06Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMngLocMgr06Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr06Dlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr06Dlg message handlers

BOOL CMngLocMgr06Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;

	ids = InitESL_Mgr(_T("SM_MNG_LOC_MGR_06_DLG"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIL);
	pGrid->SetRowHeight(0, 400);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMngLocMgr06Dlg::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}


INT CMngLocMgr06Dlg::ViewScreen()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_MNG_LOC_MGR_06");
	CString sCMAlias = _T("CM_MNG_LOC_MGR_06");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewScreen") );

	CString sWhere;
//	pDM->RefreshDataManager(sWhere);
	CLocCommonAPI::RefreshDataManager( this , pDM , sWhere );

	pDM->SORT( _T("메일종류") );

	INT nRowCnt = pDM->GetRowCount();

	/*
	for( INT i = nRowCnt-1 ; i > 6 ; i-- )
	{
		pDM->DeleteRow(i);
	}
	*/

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = CreateCondition( i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewScreen") );
	}

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ViewScreen") );

	pCM->AllControlDisplay();

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIL);
	pGrid->SetRowHeight(0, 400);


	const INT nCount = 2;
	TCHAR *sMenuAlias[nCount] = 
	{
		_T("반납시창보여주기여부") , _T("예약상태시창보여주기여부") 
	};

	INT nIDC[nCount] =
	{
		IDC_cmbYN1 , IDC_cmbYN2
	};

	CString strData = _T("");

	for( i = 0 ; i < nCount ; i++ )
	{
		ids = this->GetManageValue(_T("열람"),_T("공통"),_T("메일관리"),sMenuAlias[i],strData);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ViewScreen") );
		strData.TrimLeft(); strData.TrimRight();

		if(_T("Y") == strData) strData = _T("0");
		else  strData = _T("1");

//		((CComboBox*)GetDlgItem(nIDC[i]))->SetWindowText(strData);
		((CComboBox*)GetDlgItem(nIDC[i]))->SetCurSel(_ttoi(strData));
	}


	return 0;

	EFS_END
	return -1;

}


INT CMngLocMgr06Dlg::SaveScreen()
{
	EFS_BEGIN


	INT ids;

	CString sDMAlias = _T("DM_MNG_LOC_MGR_06");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveScreen") );


	const INT nCnt = 4;

	TCHAR *sDBField[nCnt][3] =
	{
		{ _T("기능사용여부") , _T("USE_YN") , _T("STRING") }, 
		{ _T("일괄보내기기능사용여부") , _T("BATCH_PROCESSING_YN") , _T("STRING") }, 
		{ _T("재발송허용여부") , _T("BATCH_CONDITION_RE_SEND_YN") , _T("STRING") }, 
		{ _T("조건값") , _T("BATCH_CONDITION_VALUE") , _T("STRING") }, 
	};

	CString sValue;
	CString sRecKey;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("메일정보KEY") , i , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveScreen") );

		

		// Mobile API 적용
		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , pDM );

		pDM->StartFrame();
		pDM->InitDBFieldData();
		
		for( INT j = 0 ; j < nCnt ; j++ )
		{
			ids = pDM->GetCellData( sDBField[j][0] , i , sValue );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveScreen") );
			pDM->AddDBFieldData( sDBField[j][1] , sDBField[j][2] , sValue );
		}

//		pDM->MakeUpdateFrame( _T("MN_MAIL_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T(""), __WFILE__, __LINE__ ), TRUE );
		pMobileApi->MakeUpdateFrame( _T("MN_MAIL_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
//		ids = pDM->SendFrame();
		pMobileApi->SendFrame();
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SaveScreen") );

		pDM->EndFrame();

		// Mobile API delete
		CLocCommonAPI::MobileApiEndLog( pMobileApi );
	}

	const INT nCount = 2;
	TCHAR *sMenuAlias[nCount] = 
	{
		_T("반납시창보여주기여부") , _T("예약상태시창보여주기여부") 
	};

	INT nIDC[nCount] =
	{
		IDC_cmbYN1 , IDC_cmbYN2
	};

	CString strData = _T("");
	INT nCurSel;

	for( i = 0 ; i < nCount ; i++ )
	{
		nCurSel = ((CComboBox*)GetDlgItem(nIDC[i]))->GetCurSel();

		((CComboBox*)GetDlgItem(nIDC[i]))->GetLBText( nCurSel , strData );

		ids = this->SetManageValue(_T("열람"),_T("공통"),_T("메일관리"),sMenuAlias[i],strData);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SaveScreen") );

	}

	return 0;

	EFS_END
	return -1;

}

INT CMngLocMgr06Dlg::CreateCondition(INT nIndex)
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_MNG_LOC_MGR_06");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CreateCondition") );

	CString sHeadDesc;
	CString sTailDesc;
	CString sValue;

	ids = pDM->GetCellData( _T("조건앞설명") , nIndex , sHeadDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CreateCondition") );
	sHeadDesc.TrimLeft(); sHeadDesc.TrimRight();

	ids = pDM->GetCellData( _T("조건뒤설명") , nIndex , sTailDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CreateCondition") );
	sTailDesc.TrimLeft(); sTailDesc.TrimRight();

	ids = pDM->GetCellData( _T("조건값") , nIndex , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CreateCondition") );
	sValue.TrimLeft(); sValue.TrimRight();

	CString sValueDesc = sHeadDesc + _T(" ") + sValue + sTailDesc ;

	ids = pDM->SetCellData( _T("조건") , sValueDesc , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CreateCondition") );

	return 0;

	EFS_END
	return -1;

}

INT CMngLocMgr06Dlg::ViewGrid( INT nIndex )
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_MNG_LOC_MGR_06");
	CString sGridAlias = _T("일괄처리기능관리");

	CLocCommonAPI::InsertGridRow( this , sCMAlias , sGridAlias , nIndex , nIndex );

	return 0;

	EFS_END
	return -1;

}

VOID CMngLocMgr06Dlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIL);

	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	CRect GridRect;
	pGrid->GetWindowRect( &GridRect );

	pGrid->MoveWindow(5, 40, cx-8, cy - 60 );

	

	EFS_END
}

BEGIN_EVENTSINK_MAP(CMngLocMgr06Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr06Dlg)
	ON_EVENT(CMngLocMgr06Dlg, IDC_gridMAIL, -601 /* DblClick */, OnDblClickgridMAIL, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMngLocMgr06Dlg::OnDblClickgridMAIL() 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_MNG_LOC_MGR_06");
	CString sGridAlias = _T("일괄처리기능관리");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("ViewScreen") );

	CESL_Grid * pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));

	if( pGrid->GetMouseRow() < 1 ) return;

	InputDataToGrid();


	EFS_END
}



VOID CMngLocMgr06Dlg::InputDataToGrid()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_MNG_LOC_MGR_06");
	CString sGridAlias = _T("일괄처리기능관리");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("ViewScreen") );

	CESL_Grid * pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));

	CString sValue = pGrid->GetText();

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	switch( nCol )
	{
		case 2 :		// 사용여부
		case 3 :		// 기능 사용여부
		case 4 :		// 재발송 여부
			SetYNCheck( sValue, nRow, nCol );
			break;
		default :
			SetEditBox( nRow );
			break;
	}	

	EFS_END
}

INT CMngLocMgr06Dlg::SetEditBox( INT nRow )
{
	EFS_BEGIN

	INT ids;

	CString sColumnAlias = _T("조건값");

	CString sDMAlias = _T("DM_MNG_LOC_MGR_06");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetYNCheck") );

	CString sValue;
	ids = pDM->GetCellData( sColumnAlias , nRow-1 , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetEditBox") );

	CEditBoxDlg EditDlg(this);
	EditDlg.SetValue( sValue );
	EditDlg.DoModal();

	if( EditDlg.IsEdit() == 0 )
		EditDlg.GetValue( sValue );

	ids = pDM->SetCellData( sColumnAlias  , sValue , nRow-1);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetEditBox") );

	CreateCondition( nRow-1 );
	ViewGrid( nRow - 1 );

	return 0;

	EFS_END
	return -1;

}

INT CMngLocMgr06Dlg::SetYNCheck( CString sValue , INT nRow , INT nCol )
{
	EFS_BEGIN

	if( sValue.Compare(_T("Y"))==0 )
		sValue = _T("N");
	else 
		sValue = _T("Y");
	
	INT ids;

	CString sColumnAlias;
	if( nCol == 2 )
		sColumnAlias = _T("기능사용여부");
	else if( nCol == 3 )
		sColumnAlias = _T("일괄보내기기능사용여부");
	else if( nCol == 4 )
		sColumnAlias = _T("재발송허용여부");

	CString sDMAlias = _T("DM_MNG_LOC_MGR_06");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetYNCheck") );

	ids = pDM->SetCellData( sColumnAlias , sValue ,  nRow-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetYNCheck") );

	
	ViewGrid( nRow-1 );

	return 0;

	EFS_END
	return -1;

}



/// 잡다구리 DM에서 VALUE를 뽑아내는 함수
INT CMngLocMgr06Dlg::GetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_잡다구리설정값2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	ids = CLocCommonAPI::GetManageValue( this , pDM , sGroup2 , sGroup3 , sAlias , sValueName , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	return 0;

	EFS_END

	return -1;
}

/// 잡다구리 DM에서 VALUE를 뽑아내는 함수
INT CMngLocMgr06Dlg::SetManageValue( CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_잡다구리설정값2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetManageValue") );

	INT nIndex;

	ids = CLocCommonAPI::FindManageDMIndex( this , pDM , sGroup2 , sGroup3 , sAlias , sValueName , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetManageValue") );
	if( ids > 0 )
	{
		/// 존재하지 않는다면 새로 만든다.
		pDM->StartFrame();
		
		CString sRecKey;
		
		ids = pDM->MakeRecKey( sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SetManageValue") );
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("GROUP_2") , _T("STRING") , sGroup2 , TRUE );
		pDM->AddDBFieldData( _T("GROUP_3") , _T("STRING") , sGroup3 , TRUE );
		pDM->AddDBFieldData( _T("CLASS_ALIAS") , _T("STRING") , sAlias, TRUE );
		pDM->AddDBFieldData( _T("VALUE_NAME") , _T("STRING") , sValueName , TRUE );
		pDM->AddDBFieldData( _T("VALUE_1") , _T("STRING") , sValue , TRUE );
		pDM->AddDBFieldData( _T("SYS_SETUP_YN") , _T("STRING") , _T("Y") , TRUE );
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") , TRUE ); // 2008.10.22 ADD BY PWR (통합시스템 적용)
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("메일설정"), __WFILE__, __LINE__ ), TRUE );
		
		pDM->MakeInsertFrame( _T("ESL_MANAGE_TBL") );
		
		ids = pDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetManageValue") );
		
		pDM->InsertRow(-1);
		INT nInsertRow = pDM->GetRowCount() -1 ;
		ids = pDM->SetCellData( _T("REC_KEY") , sRecKey , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("GROUP_2") , sGroup2 , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("GROUP_3") , sGroup3 , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("CLASS_ALIAS") , sAlias , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("VALUE_NAME") , sValueName , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("SetManageValue") );
		ids = pDM->SetCellData( _T("VALUE_1") , sValue , nInsertRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("SetManageValue") );
		
		return 0;
	}
	
	ids = pDM->SetCellData( _T("VALUE_1") , sValue , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetManageValue") );

	ids = CESL_Mgr::SetManageValue( sGroup2 , sGroup3 , sAlias , sValueName , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetManageValue") );

	return 0;

	EFS_END

	return -1;
}

HBRUSH CMngLocMgr06Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
