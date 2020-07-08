// LoanUserManage.cpp : implementation file
//

#include "stdafx.h"
#include "LoanUserManage.h"
#include "..\\BL_LOC_USER\\BL_LOC_USER_PROC.h"
#include "..\\BL_LOC_LOAN\\BL_LOC_LOAN_PROC.h"
#include "BO_LOC_33001.h"
#include "..\\BL_LOC_USER\\BO_LOC_3320.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanUserManage dialog


CLoanUserManage::CLoanUserManage(CESL_Mgr* pParent , CLoanShareManager* pLoanShareManager , CLonaManageValue *pManageValue/*=NULL*/,bool IsDoModal )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanUserManage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pLona = pManageValue;
	m_pSearchDlg = NULL;
	m_pAllChangeDlg = NULL;
	m_bIsDoModal = IsDoModal;
}

CLoanUserManage::~CLoanUserManage()
{
	if( m_pSearchDlg != NULL && ::IsWindow(m_pSearchDlg->m_hWnd) )
		delete m_pSearchDlg;
	if( m_pAllChangeDlg != NULL && ::IsWindow(m_pAllChangeDlg->m_hWnd) )
		delete m_pAllChangeDlg;
}

VOID CLoanUserManage::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanUserManage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLoanUserManage, CDialog)
	//{{AFX_MSG_MAP(CLoanUserManage)
	ON_BN_CLICKED(IDC_btnUSERSEARCH, OnbtnUSERSEARCH)
	ON_BN_CLICKED(IDC_btnUSERINSERT, OnbtnUSERINSERT)
	ON_BN_CLICKED(IDC_btnUSEREDIT, OnbtnUSEREDIT)
	ON_BN_CLICKED(IDC_btnUSERDELETE, OnbtnUSERDELETE)
	ON_BN_CLICKED(IDC_btnUSERDETAILVIEW, OnbtnUSERDETAILVIEW)
	ON_BN_CLICKED(IDC_btnUSERDELETECHANGE, OnbtnUSERDELETECHANGE)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnUSERALL_CHANGE, OnbtnUSERALLCHANGE)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnUSERSELECT, OnbtnUSERSELECT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanUserManage message handlers

BOOL CLoanUserManage::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if(InitESL_Mgr(_T("BO_LOC_3300")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );
	
	m_pUserDM = FindDM( _T("DM_BO_LOC_3300_LIST") );
	if( m_pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	SetInitControl();

	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3300"),_T("MainGrid"));
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("대출자 관리 엑셀");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

BOOL CLoanUserManage::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CLoanUserManage::OnbtnUSERSEARCH() 
{
	EFS_BEGIN

	if( m_pSearchDlg == NULL || !::IsWindow(m_pSearchDlg->m_hWnd) )
	{
		m_pSearchDlg = new CBO_LOC_3310(this,m_pLoanShareManager);
		m_pSearchDlg->pMain = this->pMain;
		m_pSearchDlg->pParentMgr = (CESL_Mgr*)this;
		m_pSearchDlg->m_pInfo = this->m_pInfo;
		m_pSearchDlg->Create(this);
	}

	if( !m_pSearchDlg->IsWindowVisible() )
	{
		m_pSearchDlg->ShowWindow(SW_SHOW);
		m_pSearchDlg->UpdateWindow();
		m_pSearchDlg->CenterWindow();
	}
	else
	{
		m_pSearchDlg->SetFocus();
	}

	EFS_END
}

VOID CLoanUserManage::OnbtnUSERINSERT() 
{
	EFS_BEGIN

	UserInsert();

	EFS_END
}

INT	CLoanUserManage::UserInsert()
{
EFS_BEGIN

	CBO_LOC_3320 Dlg( this, m_pLoanShareManager, m_pLona );
	Dlg.SetMode(0,TRUE);
	Dlg.DoModal();	

	return 0;
EFS_END
return -1;
}

INT	CLoanUserManage::UserInsert(CString& strLastInsertUserNo)
{
EFS_BEGIN

	CBO_LOC_3320 Dlg( this, m_pLoanShareManager, m_pLona );
	Dlg.SetMode(0,TRUE);
	Dlg.DoModal();	
	strLastInsertUserNo = Dlg.m_strLastWorkUserNo;

	return 0;
EFS_END
return -1;
}

CESL_Mgr* CLoanUserManage::UserInsert(CESL_Mgr* pParent)
{
	CBO_LOC_3320* pDlg = new CBO_LOC_3320(pParent, m_pLoanShareManager, m_pLona);
	pDlg->SetMode(0,TRUE);

	return pDlg;
}

VOID CLoanUserManage::OnbtnUSEREDIT() 
{
	EFS_BEGIN
	CBO_LOC_3320 Dlg( this, m_pLoanShareManager, m_pLona );
	Dlg.SetMode(1,TRUE);
	if( Dlg.CheckParentDMCount() < 1 )
	{
		return ;
	}
	Dlg.DoModal();	
	EFS_END
}

VOID CLoanUserManage::OnbtnUSERDELETE() 
{
	EFS_BEGIN

	BL_LOC_USER_PROC SpfWork( this, m_pLoanShareManager );
	SpfWork.SPFExcutePath( _T("이용자_삭제") );

	EFS_END
}

VOID CLoanUserManage::OnbtnUSERDETAILVIEW() 
{
	EFS_BEGIN

	CBO_LOC_3320 Dlg( this, m_pLoanShareManager, m_pLona );
	Dlg.SetMode(2,TRUE);
	if( Dlg.CheckParentDMCount() < 1 )
	{
		return ;
	}
	Dlg.DoModal();	

	EFS_END
}

VOID CLoanUserManage::OnbtnUSERDELETECHANGE() 
{	
	EFS_BEGIN

	BL_LOC_USER_PROC SpfWork( this , m_pLoanShareManager );
	SpfWork.SPFExcutePath( _T("이용자_제적처리") );

	EFS_END
}


VOID CLoanUserManage::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	ResizeControl( cx , cy );	

	EFS_END
}

VOID CLoanUserManage::ResizeControl( INT cx , INT cy )
{
	EFS_BEGIN

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}
	CRect TopRect;
	CRect BottomRect;
	GetDlgItem(IDC_TOP_POS)->GetWindowRect(&TopRect);
	GetDlgItem(IDC_BOTTOM_POS)->GetWindowRect(&BottomRect);

	pGrid->MoveWindow( 0 , TopRect.Height() , cx, cy-BottomRect.Height()-TopRect.Height());


	EFS_END
}

BEGIN_EVENTSINK_MAP(CLoanUserManage, CDialog)
    //{{AFX_EVENTSINK_MAP(CLoanUserManage)
	ON_EVENT(CLoanUserManage, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	ON_EVENT(CLoanUserManage, IDC_gridMAIN, -600 /* Click */, OnClickgridMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INT CLoanUserManage::GetSelectUserKey()
{
	EFS_BEGIN

	INT ids;

	if( m_pUserDM->GetRowCount() < 1 )
	{
		m_sSelectRecKey.Empty();
		return 0;
	}

	CESL_Grid *pGrid = GetGridCtrl();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSelectUserKey") );

	INT nIndex = pGrid->GetIdx();

	CString strInferiorClass;
	ids = m_pUserDM->GetCellData(_T("불량회원구분") , nIndex , strInferiorClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("GetSelectUserKey") );
	
	strInferiorClass.TrimLeft();strInferiorClass.TrimRight();
	if( _T("3") == strInferiorClass || _T("탈퇴") == strInferiorClass || _T("탈퇴회원") == strInferiorClass )
	{
		AfxMessageBox( _T("탈퇴회원은 선택할 수 없습니다."), MB_ICONSTOP );
		m_sSelectRecKey.Empty();
		return 0;
	}

	ids = m_pUserDM->GetCellData(_T("REC_KEY") , nIndex , m_sSelectRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("GetSelectUserKey") );

	return 0;

	EFS_END
	return -1;

}

VOID CLoanUserManage::OnDblClickgridMAIN() 
{
	EFS_BEGIN

	INT ids;
	
	CESL_Grid *pGrid = GetGridCtrl();
	if( pGrid == NULL ) return;

	
	if( pGrid->GetMouseRow() < 1 ) return;
	

	ids = GetSelectUserKey();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-1,_T("OnClickgridMAIN") );

	if( m_sSelectRecKey.IsEmpty() )
		return;
	
	ids = ChangeUser();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-1,_T("OnDblClickgridMAIN") );

	EFS_END
}

INT CLoanUserManage::GetSelectRecKey( CString &sRecKey )
{
	EFS_BEGIN

	sRecKey = m_sSelectRecKey;
	return 0;

	EFS_END
	return -1;

}

CESL_Grid* CLoanUserManage::GetGridCtrl()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3300");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}


INT CLoanUserManage::ChangeUser()
{
	EFS_BEGIN

	if( m_bIsDoModal )
		OnCancel();
	else
	{
		CBL_LOC_LOAN_PROC SpfWork(this,m_pLoanShareManager,m_pLona);
		SpfWork.SetChangeMemberKey( m_sSelectRecKey );
		SpfWork.SPFExcutePath( _T("대출자관리_회원변경") );

		if (pParentMgr)
			pParentMgr->SendMessage( WM_USER_LOAD );
	}

	return 0;

	EFS_END
	return -1;

}

VOID CLoanUserManage::OnClickgridMAIN() 
{
	EFS_BEGIN

	if( m_pUserDM->GetRowCount() < 1 )
	{
		m_sSelectRecKey.Empty();
		return ;
	}

	EFS_END
}

VOID CLoanUserManage::SetInitControl()
{
	INT nMode = _ttoi(m_pLona->m_sLoanUserAccessMode);

	if( nMode == 0 )
	{
	}
	else if( nMode == 1 )
	{
		const INT nCnt = 5;
		INT nUnEnableIDC[nCnt] =
		{
			IDC_btnUSERINSERT , IDC_btnUSEREDIT , IDC_btnUSERDELETE , IDC_btnUSERDELETECHANGE , IDC_btnUSERALL_CHANGE
		};
		for( INT i = 0 ; i < nCnt ; i++ )
			GetDlgItem( nUnEnableIDC[i] )->EnableWindow(FALSE);
	}
	else if(2 == nMode)
	{
		CWnd* pWnd = GetDlgItem(IDC_btnUSERDELETE);
		if(NULL != pWnd)
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	CString bCivilNoDisplayCheck;
	CString strColName;
	INT		i;
	INT		nColCnt;

	GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("주민등록번호사용유무"), bCivilNoDisplayCheck );
	if ( bCivilNoDisplayCheck != _T("Y") )
	{
		CESL_Grid *pGrid = GetGridCtrl();
		if( pGrid == NULL ) return;	
		nColCnt = pGrid->GetColCount();

		for ( i = 0 ; i < nColCnt ; i++ )
		{
			strColName = pGrid->GetTextMatrix(0,i);
			if ( strColName==_T("주민등록번호") )
			{
				pGrid->SetColWidth(i,0,0);
			}
		}
	}

	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
}

VOID CLoanUserManage::OnbtnUSERALLCHANGE() 
{	
	EFS_BEGIN

	if( m_pAllChangeDlg == NULL || !::IsWindow(m_pAllChangeDlg->m_hWnd) )
	{
		m_pAllChangeDlg = new CBO_LOC_3370(this,m_pLoanShareManager);
		m_pAllChangeDlg->pMain = this->pMain;
		m_pAllChangeDlg->pParentMgr = (CESL_Mgr*)this;
		m_pAllChangeDlg->m_pInfo = this->m_pInfo;
		m_pAllChangeDlg->Create(this);
	}

	if( !m_pAllChangeDlg->IsWindowVisible() )
	{
		m_pAllChangeDlg->ShowWindow(SW_SHOW);
		m_pAllChangeDlg->UpdateWindow();
		m_pAllChangeDlg->CenterWindow();
	}
	else
	{
		m_pAllChangeDlg->SetFocus();
	}

	EFS_END
}

VOID CLoanUserManage::OnClose() 
{
	EFS_BEGIN

	if( m_bIsDoModal )
		CDialog::OnCancel();
	else
		DestroyWindow();

	EFS_END
}

INT CLoanUserManage::SearchUser( CString sUserRecKey )
{
	EFS_BEGIN


	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3300_LIST");

	CESL_DataMgr* pDM = FindDM(sDMAlias);
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SearchUser") );

	CString sWhere;
	sWhere.Format( _T("CLU.REC_KEY = %s") , sUserRecKey);

	ids = pDM->RefreshDataManager( sWhere );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SearchUser") );

	ids = UserCodeToDesc( sDMAlias );
	if( 0 > ids ) return -1;

	ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1 , _T("주민등록번호") , m_sIsUnityLoanService);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SearchUser") );

	ids = AllControlDisplay( _T("CM_BO_LOC_3300") , -1);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SearchUser") );

	return 0;

	EFS_END

	return -1;
}

INT CLoanUserManage::UserCodeToDesc( CString sDMAlias )
{
	EFS_BEGIN
		
		INT ids;
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("BL_LOC_USER_PROC::UserCodeToDesc( CString sDMAlias )") );
	
	const INT nCodeCnt = 2;
	TCHAR *sCodeAlias[nCodeCnt] =
	{
		_T("대출소속정보") , _T("대출직급정보") 
	};
	TCHAR *sFieldAlias[nCodeCnt] =
	{
		_T("대출소속정보") , _T("대출직급정보") 
	};
	
	for( INT i = 0 ; i < nCodeCnt ; i++ )
	{
		ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , sCodeAlias[i] , sFieldAlias[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("BL_LOC_USER_PROC::UserCodeToDesc( CString sDMAlias )") );
	}
	
	CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6;

	CString sCodeData;
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("불량회원구분") , i , sCodeData );
		if( ids < 0 && ids != -4012 ) ERROR_MSG_RETURN_ERROR_CODE2(-3 , _T("BL_LOC_USER_PROC::UserCodeToDesc( CString sDMAlias )") );
		if( sCodeData.IsEmpty() || ids== -4012 ) continue;
		
		if( (sCodeData.Compare(_T("0"))==0) || (sCodeData.Compare(_T("정상"))==0) || (sCodeData.Compare(_T("정상회원"))==0))
			sCodeData = _T("정상회원");
		else if( (sCodeData.Compare(_T("1"))==0) || (sCodeData.Compare(_T("대출정지"))==0))
			sCodeData = _T("대출정지");
		else if( (sCodeData.Compare(_T("2"))==0) || (sCodeData.Compare(_T("제적"))==0) || (sCodeData.Compare(_T("제적회원"))==0))
			sCodeData = _T("제적회원");		
		else if( (sCodeData.Compare(_T("3"))==0) || (sCodeData.Compare(_T("탈퇴"))==0) || (sCodeData.Compare(_T("탈퇴회원"))==0))
			sCodeData = _T("탈퇴회원");

		ids = pDM->SetCellData( _T("불량회원구분") , sCodeData , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("BL_LOC_USER_PROC::UserCodeToDesc( CString sDMAlias )") );

		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CString strGetData;
			pDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes2.Add(strGetData);
			pDM->GetCellData(_T("자택전화"), i, strGetData);
			asDes3.Add(strGetData);
			pDM->GetCellData(_T("집주소"), i, strGetData);
			asDes4.Add(strGetData);
			pDM->GetCellData(_T("핸드폰"), i, strGetData);
			asDes5.Add(strGetData);
			pDM->GetCellData(_T("제2연락처"), i, strGetData);
			asDes6.Add(strGetData);
		}	
	}

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("SECOND_PHONE"), asDes6);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("E_MAIL"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("자택전화"), asDes3.GetAt(i), i);
					pDM->SetCellData(_T("집주소"), asDes4.GetAt(i), i);
					pDM->SetCellData(_T("핸드폰"), asDes5.GetAt(i), i);
					pDM->SetCellData(_T("제2연락처"), asDes6.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
	}
	
	return 0;
	
	EFS_END
		return -1;
	
}
VOID CLoanUserManage::OnbtnUSERSELECT() 
{
	
EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetGridCtrl();
	if( pGrid == NULL ) return;	

	pGrid->SelectMakeList();
	INT nSelGridCnt = pGrid->SelectGetCount();
	if ( nSelGridCnt > 1 ) 
	{
		AfxMessageBox( _T("대출자 정보는 반드시 한개의 값만 선정되어야 합니다."));
		return;
	}
	

	ids = GetSelectUserKey();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-1,_T("OnClickgridMAIN") );

	if( m_sSelectRecKey.IsEmpty() )
		return;
	
	ids = ChangeUser();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-1,_T("OnDblClickgridMAIN") );

EFS_END

}

HBRUSH CLoanUserManage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
