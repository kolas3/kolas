// MngPreRegNoInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngPreRegNoInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngPreRegNoInputDlg dialog


CMngPreRegNoInputDlg::CMngPreRegNoInputDlg(CESL_Mgr* pParent,INT nMode)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngPreRegNoInputDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMode = nMode;
	m_sSMAlias = _T("SM_MNG_PRE_REG_NO_02");
	m_sParentDMAlias = _T("DM_MNG_PRE_REG_NO");
	m_sParentCMAlias = _T("CM_MNG_PRE_REG_NO");
	m_sParentGridAlias = _T("이전등록번호그리드");

	m_sCMAlias = _T("CM_MNG_PRE_REG_NO_02");
}

CMngPreRegNoInputDlg::~CMngPreRegNoInputDlg()
{	
	
}


VOID CMngPreRegNoInputDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngPreRegNoInputDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMngPreRegNoInputDlg, CDialog)
	//{{AFX_MSG_MAP(CMngPreRegNoInputDlg)
	ON_BN_CLICKED(IDC_btnPREV_REG_NO, OnbtnPREVREGNO)
	ON_BN_CLICKED(IDC_btnNEXT_REG_NO, OnbtnNEXTREGNO)
	ON_BN_CLICKED(IDC_btnCLOSE_REG_NO, OnbtnCLOSEREGNO)
	ON_BN_CLICKED(IDC_btnSAVE_REG_NO, OnbtnSAVEREGNO)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngPreRegNoInputDlg message handlers

BOOL CMngPreRegNoInputDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	INT ids;
	ids = InitESL_Mgr( m_sSMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	
	m_pParentDM = FindDM( m_sParentDMAlias );
	if( m_pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );
	
	m_pParentCM = FindCM( m_sParentCMAlias );
	if( m_pParentCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	m_pParentGrid = (CESL_Grid*)(m_pParentCM->FindControl( m_sParentGridAlias ));
	if( m_pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("OnInitDialog") );

	m_pCM = FindCM( m_sCMAlias );
	if( m_pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("OnInitDialog") );

	InitSetStartPos();

	ViewCurrentWindow();

	EnableThemeDialogTexture(GetSafeHwnd());
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분코드를 6자리 이하로 제한
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtPRE_REG_CODE);
	pEdit->SetLimitText(6);
	//=====================================================

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


INT CMngPreRegNoInputDlg::InitSetStartPos()
{
	EFS_BEGIN

	if( m_nMode == 0 )
	{
		SetWindowText( _T("입력") );
		GetDlgItem(IDC_btnPREV_REG_NO)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_btnNEXT_REG_NO)->ShowWindow( SW_HIDE );		
	}

	if( m_nMode == 1 )
	{
		SetWindowText( _T("수정") );
		m_nCurrentIndex = m_pParentGrid->GetIdx();
		m_nLastIndex = m_pParentGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}

	return 0;

	EFS_END
	return -1;

}


//--------------------------------------------------
INT CMngPreRegNoInputDlg::ViewCurrentWindow()
{
	EFS_BEGIN

	INT ids;

	if( m_nMode == 1 )
	{
		ids = m_pCM->AllControlDisplay(m_nCurrentIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewCurrentWindow") );

		GetDlgItem(IDC_btnPREV_REG_NO)->EnableWindow( TRUE );
		GetDlgItem(IDC_btnNEXT_REG_NO)->EnableWindow( TRUE );

		if( m_nCurrentIndex == m_nFirstIndex )
			GetDlgItem(IDC_btnPREV_REG_NO)->EnableWindow( FALSE );	
		if( m_nCurrentIndex == m_nLastIndex )
			GetDlgItem(IDC_btnNEXT_REG_NO)->EnableWindow( FALSE );
		
		m_pParentGrid->SetReverse( m_nCurrentIndex );
	}
	//===================================================
	//2010.05.04 ADD BY PJW : 입력모드일경우 default로 값이 들어가도록 한다.
	else if( m_nMode == 0)
	{
		CString strManageCode;
		pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("관리구분"),strManageCode);
		m_pCM->SetControlMgrData( _T("관리구분") , strManageCode , -1 , 0  );
	}
	//===================================================
	else
	{
		// 관리구분을 항상 적용안함으로 한다.
		m_pCM->SetControlMgrData( _T("관리구분") , _T("") , -1 , 0  );
	}
	
	return 0;

	EFS_END
	return -1;

}

// -------------------------------------------------
// nMode 0 -> PREV
//		 1 -> NEXT
INT CMngPreRegNoInputDlg::MoveIndex( INT nMode )
{	
	EFS_BEGIN

	INT ids;
	ids = InitSetStartPos();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MoveIndex( INT nMode )") );

	if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
	else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;

	ViewCurrentWindow();

	return 0;

	EFS_END
	return -1;

}

VOID CMngPreRegNoInputDlg::OnbtnPREVREGNO() 
{
	EFS_BEGIN

	MoveIndex(0);	

	EFS_END
}

VOID CMngPreRegNoInputDlg::OnbtnNEXTREGNO() 
{
	EFS_BEGIN

	MoveIndex(1);	

	EFS_END
}

VOID CMngPreRegNoInputDlg::OnbtnCLOSEREGNO() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}


VOID CMngPreRegNoInputDlg::OnbtnSAVEREGNO() 
{
	EFS_BEGIN

	INT ids;

	if( !IsSave() )
		return ;
	
	if( m_nMode == 0 )
		ids = InputDB();
	else
		ids = UpdateDB();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnSAVEREGNO") );
	if( ids > 0 ) return;
	AfxMessageBox(_T("저장되었습니다."));

	EFS_END
}

BOOL CMngPreRegNoInputDlg::IsSave()
{
	EFS_BEGIN

	INT ids;

	const INT nCnt = 3;
	TCHAR *sDMFieldAlias[nCnt] = 
	{
		_T("이전등록구분") , _T("자리수") , _T("현재등록구분")
	};

	INT nIDC[nCnt] = 
	{
		IDC_edtPRE_REG_CODE , IDC_edtREG_CNT , IDC_cmbREG_CODE 
	};

	CString sData;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pCM->GetControlMgrData( sDMFieldAlias[i] , sData , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("InputDB") );
		sData.TrimLeft(); sData.TrimRight();

		if( i == 0 ) continue;

		if( sData.IsEmpty() )
		{
			CString sMsg;
			sMsg.Format( _T("'%s' 먼저 입력해주십시오") , sDMFieldAlias[i] );
			AfxMessageBox( sMsg );
			GetDlgItem(nIDC[i])->SetFocus();
			return FALSE;
		}
	}
	return TRUE;

	EFS_END
	return FALSE;

}

BOOL CMngPreRegNoInputDlg::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

INT CMngPreRegNoInputDlg::InputDB()
{
	EFS_BEGIN

	INT ids;

	CString sRecKey;

	// Mobile Api 적용
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , m_pParentDM );

//	ids = m_pParentDM->MakeRecKey( sRecKey );
	ids = pMobileApi->MakeRecKey( sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("InputDB") );

	ids = m_pParentDM->MakeControlMgrFiledData( (CObject*)m_pCM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("InputDB") );

	CString sRegNoLength;
	ids = m_pCM->GetControlMgrData( _T("자리수") , sRegNoLength );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("InputDB") );
	sRegNoLength.TrimLeft(); sRegNoLength.TrimRight();

	if( sRegNoLength.IsEmpty() )
	{
		AfxMessageBox( _T("자리수는 반드시 입력해야 합니다.") );
		GetDlgItem(IDC_edtREG_CNT)->SetFocus();
		return 1;
	}

	pMobileApi->StartFrame();

//	m_pParentDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );

//	ids = m_pParentDM->MakeControlMgrInsertFrame();
	ids = pMobileApi->MakeInsertFrame( _T("CO_PREV_REG_NO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("InputDB") );

//	ids = m_pParentDM->SendFrame();
	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("InputDB") );
	ids = m_pParentDM->EndFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -5, _T("InputDB") );

	// Mobile Api delete
	CLocCommonAPI::MobileApiEndLog( pMobileApi );


	// DM에 추가
	m_pParentDM->InsertRow(-1);
	INT nInsertIndex = m_pParentDM->GetRowCount() -1;

	const INT nCnt = 4;
	TCHAR *sDMFieldAlias[nCnt] = 
	{
		_T("이전등록구분") , _T("자리수") , _T("현재등록구분") , _T("관리구분")
	};

	CString sData;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pCM->GetControlMgrData( sDMFieldAlias[i] , sData , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("InputDB") );

		ids = m_pParentDM->SetCellData( sDMFieldAlias[i] , sData , nInsertIndex );
//		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("InsertNonBookDBProc") );
		if( ids < 0 ){
			AfxMessageBox(sDMFieldAlias[i]);
			return -1;
		}
	}
	// REC_KEY
	ids = m_pParentDM->SetCellData( _T("REC_KEY") , sRecKey , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("InputDB") );

	// 화면에 뿌려주기
	ids = CLocCommonAPI::InsertGridRow( this , m_pParentGrid , nInsertIndex , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -8 , _T("InputDB") );

	return 0;

	EFS_END
	return -1;

}

INT CMngPreRegNoInputDlg::UpdateDB()
{
	EFS_BEGIN

	INT ids;

	CString sRecKey;

	ids = m_pParentDM->GetCellData( _T("REC_KEY") , m_nCurrentIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UpdateDB") );
	
	// Mobile Api 적용
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , m_pParentDM );

	m_pParentDM->StartFrame();
	m_pParentDM->InitDBFieldData();

	ids = m_pParentDM->MakeControlMgrFiledData( (CObject*)m_pCM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("UpdateDB") );

	CString sRegNoLength;
	ids = m_pCM->GetControlMgrData( _T("자리수") , sRegNoLength );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("UpdateDB") );
	sRegNoLength.TrimLeft(); sRegNoLength.TrimRight();

	if( sRegNoLength.IsEmpty() )
	{
		AfxMessageBox( _T("자리수는 반드시 입력해야 합니다.") );
		GetDlgItem(IDC_edtREG_CNT)->SetFocus();
		return 1;
	}

//	ids = m_pParentDM->MakeControlMgrUpdateFrame(_T("REC_KEY") , _T("NUMERIC") , sRecKey , m_nCurrentIndex );
	ids = pMobileApi->MakeUpdateFrame( _T("CO_PREV_REG_NO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("UpdateDB") );

	ids = m_pParentDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("UpdateDB") );
	ids = m_pParentDM->EndFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UpdateDB") );

	const INT nCnt = 4;
	TCHAR *sDMFieldAlias[nCnt] = 
	{
		_T("이전등록구분") , _T("자리수") , _T("현재등록구분") , _T("관리구분")
	};
	
	CString sData;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = m_pCM->GetControlMgrData( sDMFieldAlias[i] , sData , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("UpdateDM") );
		
		ids = m_pParentDM->SetCellData( sDMFieldAlias[i] , sData , m_nCurrentIndex );
		//		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -15 , _T("InsertNonBookDBProc") );
		if( ids < 0 ){
			AfxMessageBox(sDMFieldAlias[i]);
			return -1;
		}
	}
	

	// 화면에 뿌려주기
	ids = CLocCommonAPI::InsertGridRow( this , m_pParentGrid , m_nCurrentIndex , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("UpdateDB") );

	return 0;

	EFS_END
	return -1;

}


HBRUSH CMngPreRegNoInputDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
