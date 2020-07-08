// BO_STATICS_1112.cpp : implementation file
//토
#include "stdafx.h"
#include "BO_STATICS_1112.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_1112 dialog


CBO_STATICS_1112::CBO_STATICS_1112(CESL_Mgr* pParent,CStaticsInfoMgr *pSInfoMgr )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_STATICS_1112)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sSMAlias = _T("BO_STATICS_1112");
	m_pSInfoDM = pSInfoMgr->m_pStaticsMainInfoDM;
	m_pSInfoMgr = pSInfoMgr;
}

CBO_STATICS_1112::~CBO_STATICS_1112()
{
	
}

VOID CBO_STATICS_1112::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATICS_1112)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_STATICS_1112, CDialog)
	//{{AFX_MSG_MAP(CBO_STATICS_1112)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR() 
	ON_CBN_SELCHANGE(IDC_cmbWORKING_STATUS, OnSelchangecmbWORKINGSTATUS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_1112 message handlers

BOOL CBO_STATICS_1112::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CBO_STATICS_1112::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	INT ids;

	ids = InitESL_Mgr( m_sSMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pParentCM = FindCM( _T("CM_BO_STATICS_1100") );
	if( m_pParentCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	m_pSInfoMgr->SetSearchCM(FindCM(_T("CM_BO_STATICS_1112")));
	
	SYSTEMTIME systime;	
	CDateTimeCtrl* pTimeCtrl = NULL;
	
	const INT nCnt = 10;
	INT nIDC[nCnt] =
	{
		IDC_datSHELF_DATE1 , IDC_datSHELF_DATE2 , IDC_datSHELF_DATE3 , IDC_datSHELF_DATE4 ,
			IDC_datSHELF_DATE5 , IDC_datSHELF_DATE6 , IDC_edtLOAN_DATE1 , IDC_edtLOAN_DATE2,
			 IDC_datSHELF_DATE7 , IDC_datSHELF_DATE8
	};
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[i]);
		pTimeCtrl->GetTime(&systime);
		DateTime_SetSystemtime(pTimeCtrl->m_hWnd , GDT_NONE , &systime );
	}


	EnableThemeDialogTexture(GetSafeHwnd()); 

	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_1112MANAGECODE)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmbMANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_STATICS_1112"), _T("등록번호1"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_STATICS_1112"), _T("등록번호구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_STATICS_1112"), _T("등록번호2"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_STATICS_1112"), _T("등록번호구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


INT CBO_STATICS_1112::InitScreen()
{
	EFS_BEGIN

	INT ids;
	CString sStaticsClass;

	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );
	
	ids = EnableDisableControl(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_STATICS_1112::EnableDisableControl( CString sStaticsClass )
{
	EFS_BEGIN

	INT ids;

	CString sTmpStaticsClass;
	CString sSearchCondition;
	CArray < CString , CString > asControlName;

	POSITION pos;

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_STATICS_1112") );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("EnableDisableControl") );
	
	for( INT i = 0 ; i < m_pSInfoDM->GetRowCount() ; i++ )
	{
		ids = m_pSInfoDM->GetCellData( _T("통계종류") , i , sTmpStaticsClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("EnableDisableControl") );

		if( sStaticsClass.Compare( sTmpStaticsClass ) != 0 ) continue;

		ids = m_pSInfoDM->GetCellData( _T("검색조건") , i , sSearchCondition );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("EnableDisableControl") );

		ids = CLocCommonAPI::MakeArrayList( sSearchCondition , _T(",") , asControlName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("EnableDisableControl") );
		
		for( INT k = 0 ; k < pCM->Control_List.GetCount() ; k++ )
		{
			pos = pCM->Control_List.FindIndex(k);
			CESL_Control_Element *pCE = ((CESL_Control_Element*)(pCM->Control_List.GetAt(pos)));
			if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("EnableDisableControl") );

			GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
		}

		for( INT j = 0 ; j < asControlName.GetSize() ; j++ )
		{
			for( INT k = 0 ; k < pCM->Control_List.GetCount() ; k++ )
			{
				pos = pCM->Control_List.FindIndex(k);
				CESL_Control_Element *pCE = ((CESL_Control_Element*)(pCM->Control_List.GetAt(pos)));
				if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("EnableDisableControl") );

				if( pCE->CTRL_ALIAS.Find( asControlName.GetAt(j) ) > -1 )
				{					
					GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
				}
			}
		}
		if(sStaticsClass == _T("자료실별이용통계") || sStaticsClass == _T("신규구입도서이용통계"))
		{
			GetDlgItem(IDC_STATICS0)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_edtLOAN_DATE1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_edtLOAN_DATE2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATICS2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_datSHELF_DATE3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_datSHELF_DATE4)->ShowWindow(SW_HIDE);
		}else
		{
			GetDlgItem(IDC_STATICS0)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_edtLOAN_DATE1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_edtLOAN_DATE2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATICS2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_datSHELF_DATE3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_datSHELF_DATE4)->ShowWindow(SW_SHOW);
		}
	}
	
	return 0;

	EFS_END
	return -1;
}

VOID CBO_STATICS_1112::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	InitScreen();
	

	EFS_END
}

VOID CBO_STATICS_1112::OnbtnINIT() 
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_STATICS_1112") );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("EnableDisableControl") );

	pCM->AllControlClear();

	EFS_END
}

VOID CBO_STATICS_1112::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_STATICS_1112::OnbtnSAVE() 
{
	EFS_BEGIN

	SaveCondition();	

	EFS_END
}

INT CBO_STATICS_1112::SaveCondition()
{
	EFS_BEGIN

	INT ids;

	CString sSearchCond;
	ids = CMToSql( sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveCondition") );

	if( sSearchCond.IsEmpty() )
	{
		AfxMessageBox( _T("조건을 입력해주십시요") );
		return 1;
	}

	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveCondition") );

	ids = m_pSInfoMgr->SetUserCondition( sStaticsClass , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SaveCondition") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_STATICS_1112::CMToSql( CString &sSearchCond )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_STATICS_1112");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToSql") );
	
	INT nControlCnt = pCM->Control_List.GetCount();
	CString sData;

	CESL_Control_Element *pCE;
	POSITION pos;
	CString sCtrlType;
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		pos = pCM->Control_List.FindIndex(i);
		pCE = (CESL_Control_Element*)pCM->Control_List.GetAt(pos);

		sCtrlType = pCE->CTRL_TYPE;
		sCtrlType.MakeUpper();
		if( sCtrlType == 'S' ) continue;
		
		ids = pCM->GetControlMgrData( pCE->CTRL_ALIAS , sData , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToSql") );
		sData.TrimLeft(); sData.TrimRight();

		if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
			sData = _T("");

		if( sData.IsEmpty() ) continue;

		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;

		sSearchCond += pCE->CTRL_ALIAS + _T("=") + sData;
	}

	UpdateData(FALSE);
	
	const INT nDateCnt = 10;
	CString sDateCtrlAlias[nDateCnt] =
	{
		_T("배가변경일자1") , _T("배가변경일자2") , _T("배가일자1") , _T("배가일자2") ,_T("대출일자1") , _T("대출일자2") ,
			_T("제적일자1") , _T("제적일자2")     , _T("자료상태변경일자1") , _T("자료상태변경일자2")
	};
	
	for( i = 0 ; i < nDateCnt ; i++ )
	{
		pCE = (CESL_Control_Element*)pCM->FindControlElement( sDateCtrlAlias[i] );
		
		sData = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
		sData.TrimLeft(); sData.TrimRight();
		
		if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
			sData = _T("");
		
		if( sData.IsEmpty() ) continue;
		
		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;
		
		sSearchCond += pCE->CTRL_ALIAS + _T("=") + sData;
	}

	if( ((CButton*)GetDlgItem(IDC_chkADDBOOK))->GetCheck() != 1 )
	{
		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;
		sSearchCond += _T("부록자료포함=NO");
	}

	return 0;

	EFS_END
	return -1;

}

CESL_ControlMgr* CBO_STATICS_1112::GetCM()
{
	EFS_BEGIN

	return FindCM(_T("CM_BO_STATICS_1112"));

	EFS_END
	return NULL;

}

BOOL CBO_STATICS_1112::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
			return TRUE;	

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_STATICS_1112::OnSelchangecmbWORKINGSTATUS() 
{
	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_STATICS_1112"));
	if( !pCM ) return;

	CString strData;
	BOOL    bEnable = false;

	if( 0 > pCM->GetControlMgrData( _T("자료상태"), strData ) ) return;

	if( strData == _T("제적자료") )
	{
		bEnable = true;
	}

	CESL_Control_Element *pCE = (CESL_Control_Element*)pCM->FindControlElement( _T("제적일자1") );
	if( pCE )
	{
		GetDlgItem(pCE->CTRL_ID)->EnableWindow( bEnable );
	}
	if( pCE = (CESL_Control_Element*)pCM->FindControlElement( _T("제적일자2") ) )
	{
		GetDlgItem(pCE->CTRL_ID)->EnableWindow( bEnable );
	}

	SYSTEMTIME systime;	
	CDateTimeCtrl* pTimeCtrl = NULL;

	const INT nCnt = 2;
	INT nIDC[nCnt] =
	{
		IDC_datSHELF_DATE7 , IDC_datSHELF_DATE7 
	};
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[i]);
		pTimeCtrl->GetTime(&systime);
		DateTime_SetSystemtime(pTimeCtrl->m_hWnd , GDT_NONE , &systime );
	}	
}


HBRUSH CBO_STATICS_1112::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

