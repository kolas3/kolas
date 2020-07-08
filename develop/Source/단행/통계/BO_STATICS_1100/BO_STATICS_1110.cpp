// BO_STATICS_1110.cpp : implementation file
//

#include "stdafx.h"
#include "BO_STATICS_1110.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_1110 dialog


CBO_STATICS_1110::CBO_STATICS_1110(CESL_Mgr* pParent,CStaticsInfoMgr *pSInfoMgr )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_STATICS_1110)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sSMAlias = _T("BO_STATICS_1110");
	m_pSInfoDM = pSInfoMgr->m_pStaticsMainInfoDM;
	m_pSInfoMgr = pSInfoMgr;
}

CBO_STATICS_1110::~CBO_STATICS_1110()
{
}



VOID CBO_STATICS_1110::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATICS_1110)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_STATICS_1110, CDialog)
	//{{AFX_MSG_MAP(CBO_STATICS_1110)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_BN_CLICKED(IDC_btnMOVE_SHELF_LOC_CODE, OnbtnMOVESHELFLOCCODE)
	ON_BN_CLICKED(IDC_btnMOVE_SHELF_LOC_CODE2, OnbtnMOVESHELFLOCCODE2)
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_1110 message handlers
BOOL CBO_STATICS_1110::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CBO_STATICS_1110::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	INT ids;

	ids = InitESL_Mgr( m_sSMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pParentCM = FindCM( _T("CM_BO_STATICS_1100") );
	if( m_pParentCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	m_pSInfoMgr->SetSearchCM(FindCM(_T("CM_BO_STATICS_1110")));

	CString strValue;
	GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , strValue );
	if ( strValue == _T("Y") )
	{
		((CComboBox*)GetDlgItem(IDC_cmbMANAGE_CODE2))->AddString( _T("장기") );
	}

	((CComboBox*)GetDlgItem(IDC_cmbMANAGE_CODE2))->SetCurSel(0);

	EnableThemeDialogTexture(GetSafeHwnd()); 

	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_1110MANAGECODE)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmbMANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	if(NULL != m_pInfo && TRUE == m_pInfo->m_bKLUse)
	{
		GetDlgItem(IDC_sta1110_KLMEMBER)->ShowWindow(SW_SHOW);
 		GetDlgItem(IDC_cmb1110_KLMEMBER)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_sta1110_KLMEMBER)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmb1110_KLMEMBER)->ShowWindow(SW_HIDE);
	}
	((CComboBox*)GetDlgItem(IDC_cmb1110_KLMEMBER))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE


	EFS_END
	return FALSE;

}


INT CBO_STATICS_1110::InitScreen()
{
	EFS_BEGIN

	INT ids;
	CString sStaticsClass;

	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	ids = EnableDisableControl(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );
	
	if( _T("대출자 등록통계")==sStaticsClass )
	{
		SetDlgItemText(IDC_staRegDate, _T("대출자등록일"));
	}

	else if( _T("회원증발급통계")==sStaticsClass )
	{
		SetDlgItemText(IDC_staRegDate, _T("발 급 일"));
	}
	else
	{
		SetDlgItemText(IDC_staRegDate, _T("등 록 일"));
	}

	return 0;

	EFS_END
	return -1;

}

INT CBO_STATICS_1110::EnableDisableControl( CString sStaticsClass )
{
	EFS_BEGIN

	INT ids;

	CString sTmpStaticsClass;
	CString sSearchCondition;	
	CArray < CString , CString > asControlName;
	CArray < CString , CString > asExtraDateControlName;

	POSITION pos;

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_STATICS_1110") );
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
		
		ids = m_pSInfoDM->GetCellData( _T("추가날짜조건") , i , sSearchCondition );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("EnableDisableControl") );

		if (!sSearchCondition.IsEmpty())
		{
			ids = CLocCommonAPI::MakeArrayList( sSearchCondition , _T(",") , asExtraDateControlName );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("EnableDisableControl") );
		}
		
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

					if (pCE->CTRL_TYPE.CompareNoCase(_T("S")) == 0)
					{
						HWND hWnd = GetDlgItem(pCE->CTRL_ID)->GetSafeHwnd();
						if (hWnd == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-6 , _T("EnableDisableControl"));						

						SYSTEMTIME time;
						memset(&time, 0x00, sizeof(SYSTEMTIME));

						GetLocalTime(&time);						
						DateTime_SetSystemtime(hWnd, GDT_VALID, &time);

						for (INT l=0; l<asExtraDateControlName.GetSize(); l++)
						{
							if (pCE->CTRL_ALIAS.Find(asExtraDateControlName.GetAt(l)) > -1)
							{
								DateTime_SetSystemtime(hWnd, GDT_NONE, NULL);
							}
						}						
					}
				}
			}
		}
		
	}

	GetDlgItem(IDC_btnMOVE_SHELF_LOC_CODE)->EnableWindow(GetDlgItem(IDC_cmbSHELF_LOC_CODE)->IsWindowEnabled());

	return 0;

	EFS_END
	return -1;

}

VOID CBO_STATICS_1110::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	InitScreen();
	

	EFS_END
}

VOID CBO_STATICS_1110::OnbtnINIT() 
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_STATICS_1110") );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("EnableDisableControl") );

	pCM->AllControlClear();

	EFS_END
}

VOID CBO_STATICS_1110::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_STATICS_1110::OnbtnSAVE() 
{
	EFS_BEGIN

	SaveCondition();	

	EFS_END
}

INT CBO_STATICS_1110::SaveCondition()
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

INT CBO_STATICS_1110::CMToSql( CString &sSearchCond )
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_STATICS_1110");

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

		if( pCE->CTRL_ALIAS.Compare(_T("자료실")) == 0 )
			continue;
		if( pCE->CTRL_ALIAS.Compare(_T("자료실_조건")) == 0 )
			continue;
			

		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;

		sSearchCond += pCE->CTRL_ALIAS + _T("=") + sData;
	}

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("자료실_조건")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CMToSql") );

	CString sShelfLoc,sShelfLocCode;
	INT nCnt = pCMCB->GetRowCount();
	for( i = 0 ; i < nCnt ; i++ )
	{
		pCMCB->GetData( sShelfLocCode , i , 0 );

		if( !sShelfLoc.IsEmpty() ) sShelfLoc += _T("|");
		sShelfLoc += sShelfLocCode;
	}

	if( !sShelfLoc.IsEmpty() )
	{
		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;
		sSearchCond += _T("자료실_조건=") + sShelfLoc;
	}

	UpdateData(FALSE);

	const INT nDateCnt = 13;
	CString sDateCtrlAlias[nDateCnt] =
	{
		_T("등록일1") , _T("등록일2") , _T("대출일1") , _T("대출일2") , _T("반납일1") , 
		_T("반납일2") , _T("반납예정일1") , _T("반납예정일2") , _T("예약일1") , _T("예약일2") ,
		_T("예약취소일1") , _T("예약취소일2"), _T("통합회원여부")
	};

	for( i = 0 ; i < nDateCnt ; i++ )
	{
		pCE = (CESL_Control_Element*)pCM->FindControlElement( sDateCtrlAlias[i] );
				
		sData = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
		sData.TrimLeft(); sData.TrimRight();
		
		if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled())
			sData = _T("");
		
		if( sData.IsEmpty() ) continue;
		
		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;
		
		sSearchCond += pCE->CTRL_ALIAS + _T("=") + sData;
	}
	
	return 0;

EFS_END
	return -1;
}



CESL_ControlMgr* CBO_STATICS_1110::GetCM()
{
	EFS_BEGIN

	return FindCM(_T("CM_BO_STATICS_1110"));

	EFS_END
	return NULL;
}
VOID CBO_STATICS_1110::OnbtnMOVESHELFLOCCODE() 
{
	MoveShelfLoc();
}

VOID CBO_STATICS_1110::OnbtnMOVESHELFLOCCODE2() 
{
	DeleteShelfLoc();	
}

INT CBO_STATICS_1110::MoveShelfLoc()
{
	EFS_BEGIN

	INT ids;
	CString sInsertCode[2];
	
	CESL_ControlMgr* pCM = GetCM();
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MoveShelfLoc") );

	ids = pCM->GetControlMgrData( _T("자료실") , sInsertCode[0] , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MoveShelfLoc") );
	ids = pCM->GetControlMgrData( _T("자료실") , sInsertCode[1] , -1 , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MoveShelfLoc") );
	
	sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
	
	if( sInsertCode[0].IsEmpty() ) return 0;

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("자료실_조건")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MoveShelfLoc") );

	INT nRowCount = pCMCB->GetCount();

	if( nRowCount < 1 )
		pCMCB->InitMultiComboBox(2);

	CString sPreCode;

	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = pCMCB->GetData( sPreCode , i , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MoveShelfLoc") );
		sPreCode.TrimLeft(); sPreCode.TrimRight();

		if( sPreCode.Compare( sInsertCode[0] ) == 0 )
			return 0;
	}

	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MoveShelfLoc") );

	pCMCB->SetCurSel(nRowCount);

	return 0;

	EFS_END

	return -1;
}

INT CBO_STATICS_1110::DeleteShelfLoc()
{
	EFS_BEGIN

	CESL_ControlMgr* pCM = GetCM();
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnbtnMOVESHELFLOCCODE2") );
	
	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("자료실_조건")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("OnbtnMOVESHELFLOCCODE2") );
	
	INT nCount = pCMCB->GetRowCount();
	
	if( nCount == 0 )
		return 0;
	
	INT nCur = pCMCB->GetCurSel();

	CString** sItem;
	sItem = NULL;
	sItem = new CString*[nCount];
	for( INT i = 0 ; i < nCount ; i++ )
	{
		sItem[i] = new CString[2];
	}
	
	for( i = 0 ; i < nCount ; i++ )
	{
		pCMCB->GetData( sItem[i][0] , i , 0 );
		pCMCB->GetData( sItem[i][1] , i , 1 );
	}

	pCMCB->ResetContent();
	pCMCB->FreeComboData();

	pCMCB->InitMultiComboBox(2);

	for( i = 0 ; i < nCount ; i++ )
	{
		if( i == nCur ) continue;
		
		pCMCB->SetData( sItem[i] , 2 );
	}

	for( i = 0 ; i < nCount ; i++ )
		delete []sItem[i];

	delete []sItem;
			
	pCMCB->SetCurSel(0);
	
	return 0;

	EFS_END

	return -1;
}

BOOL CBO_STATICS_1110::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
			return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CBO_STATICS_1110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
