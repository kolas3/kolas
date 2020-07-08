// MngLocMgrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgrDlg dialog


CMngLocMgrDlg::CMngLocMgrDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}
CMngLocMgrDlg::~CMngLocMgrDlg()
{
	delete m_pLoanShareManager;
}

VOID CMngLocMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgrDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMngLocMgrDlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgrDlg)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabMENU, OnSelchangetabMENU)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgrDlg message handlers

BOOL CMngLocMgrDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	INT ids;

	m_pLoanShareManager = new CLoanShareManager(this);
	m_pLoanShareManager->Create(this);


	SettingDefaultValue();

	//TAB 초기화
	InitTabControl();

	// 사용되는 화면들을 모두 크리에이트 한다.
	ids = CreateAllClientWindow();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );


	// SM 적용
	ids = InitESL_Mgr( m_sSMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	// 잡다구리한 정보 가져오기
	ids = GetAllEtcSettingValue();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	// 디폴트로 첫번째화면만 보이게 한다.
	ids = DefaultView();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}
BOOL CMngLocMgrDlg::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CMngLocMgrDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMENU);

	if(pTab->GetSafeHwnd() == NULL)
		return;

	if( m_pMngLocMgr01->GetSafeHwnd() == NULL )
		return;
	if( m_pMngLocMgr02->GetSafeHwnd() == NULL )
		return;
	if( m_pMngLocMgr03->GetSafeHwnd() == NULL )
		return;
	if( m_pMngLocMgr04->GetSafeHwnd() == NULL )
		return;
	if( m_pMngLocMgr05->GetSafeHwnd() == NULL )
		return;
	if( m_pMngLocMgr06->GetSafeHwnd() == NULL )
		return;
	if( m_pMngLocMgr07->GetSafeHwnd() == NULL )
		return;
	if( m_pMngLocMgr08->GetSafeHwnd() == NULL )
		return;
	if( m_pMngLocMgr09->GetSafeHwnd() == NULL )
		return;
	if( m_pMngLocMgr10->GetSafeHwnd() == NULL )
		return;

	pTab->MoveWindow(0, 0, cx, 20);
	m_pMngLocMgr01->MoveWindow(0,20,cx,cy-10);
	m_pMngLocMgr02->MoveWindow(0,20,cx,cy-10);
	m_pMngLocMgr03->MoveWindow(0,20,cx,cy-10);
	m_pMngLocMgr04->MoveWindow(0,20,cx,cy-10);
	m_pMngLocMgr05->MoveWindow(0,20,cx,cy-10);
	m_pMngLocMgr06->MoveWindow(0,20,cx,cy-10);
	m_pMngLocMgr07->MoveWindow(0,20,cx,cy-10);
	m_pMngLocMgr08->MoveWindow(0,20,cx,cy-10);
	m_pMngLocMgr09->MoveWindow(0,20,cx,cy-10);
	m_pMngLocMgr10->MoveWindow(0,20,cx,cy-10);

	EFS_END
}

VOID CMngLocMgrDlg::InitTabControl()
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMENU);

	TC_ITEM item;

	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;

	const INT nCnt = 10;

	TCHAR *sTitle[nCnt] =
	{
		_T(" 대출/반납 형식 && 이용자 ") , 
		_T(" 반납 연기 && 예약 환경 ") , 
		_T(" 연체 자료 ") , 
		_T(" 직급별 환경 ") ,
		_T(" 대출자료 제한 ") ,
		_T(" 메일 관리 "),
		_T(" 통합대출관리 "),
		_T(" 개인정보보호정책 "),
		_T(" 무인대출예약설정 "),
		_T(" 수수료 관리 ")
	};

	INT nLastIndex = nCnt - 1;

	/// 2004/08/16 조병걸 수정 : 메일링 시스템 사용 도서관이라면 보이지 않는다.
	CString sValue, sValue2, sValue3, sValue4;
	INT ids;
	ids = GetManageValue( _T("기타") , _T("공통") , _T("메일링시스템") , _T("사용여부") , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("InitTabControl") );	
	ids = GetManageValue( _T("기타"), _T("공통"), _T("대출공유"), _T("대출반납공유버젼"), sValue2 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("InitTabControl") );	
	ids = GetManageValue( _T("열람"), _T("공통"), _T("대출자료관리"), _T("무인대출예약기능유무"), sValue3 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("InitTabControl") );
	ids = GetManageValue( _T("열람"), _T("공통"), _T("수수료관리"), _T("수수료관리시스템사용유무"), sValue4 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("InitTabControl") );

	if(sValue.IsEmpty()) sValue = _T("N");
	if(sValue2.IsEmpty()) sValue2 = _T("N");
	if(sValue3.IsEmpty()) sValue3 = _T("N");
	if(sValue4.IsEmpty()) sValue4 = _T("N");

	for( INT i = nCnt-1 ; i > -1 ; i-- )
	{		
		if( ( i == 2 ) && (sValue4.Compare(_T("Y")) == 0) ) continue;
		if( ( i == 5 ) && (sValue.Compare(_T("Y")) == 0) ) continue;
		if( ( i == 6 ) && !(sValue2 == _T("1") || sValue2 == _T("2")) ) continue;
		if( ( i == 8 ) && (sValue3.Compare(_T("N")) == 0)) continue;
		if( ( i == 9 ) && (sValue4.Compare(_T("N")) == 0)) continue;
		//if ( i == 7 ) continue;

		item.pszText = sTitle[i];
		pTab->InsertItem(0, &item);
	}

	pTab->SetCurSel(0);


	EFS_END
}

VOID CMngLocMgrDlg::SettingDefaultValue()
{
	EFS_BEGIN

	m_sSMAlias = _T("MNG_LOC_MGR_DLG");

	m_pMngLocMgr01 = new CMngLocMgr01Dlg(this,m_pLoanShareManager);
	m_pMngLocMgr02 = new CMngLocMgr02Dlg(this,m_pLoanShareManager);
	m_pMngLocMgr03 = new CMngLocMgr03Dlg(this,m_pLoanShareManager);
	m_pMngLocMgr04 = new CMngLocMgr04Dlg(this,m_pLoanShareManager);
	m_pMngLocMgr05 = new CMngLocMgr05Dlg(this,m_pLoanShareManager);
	m_pMngLocMgr06 = new CMngLocMgr06Dlg(this,m_pLoanShareManager);
	m_pMngLocMgr07 = new CMngLocMgr07Dlg(this,m_pLoanShareManager);
	m_pMngLocMgr08 = new CMngLocMgr08Dlg(this,m_pLoanShareManager);
	m_pMngLocMgr09 = new CMngLocMgr09Dlg(this,m_pLoanShareManager);
	m_pMngLocMgr10 = new CMngLocMgr10Dlg(this,m_pLoanShareManager);

	EFS_END
}

INT CMngLocMgrDlg::CreateAllClientWindow()
{
	EFS_BEGIN

	INT ids;	

	const INT nCnt = 10;

	CESL_Mgr **ppDlg;
	ppDlg = new CESL_Mgr*[nCnt];

	ppDlg[0] = (CESL_Mgr*)m_pMngLocMgr01;
	ppDlg[1] = (CESL_Mgr*)m_pMngLocMgr02;
	ppDlg[2] = (CESL_Mgr*)m_pMngLocMgr03;
	ppDlg[3] = (CESL_Mgr*)m_pMngLocMgr04;
	ppDlg[4] = (CESL_Mgr*)m_pMngLocMgr05;
	ppDlg[5] = (CESL_Mgr*)m_pMngLocMgr06;
	ppDlg[6] = (CESL_Mgr*)m_pMngLocMgr07;
	ppDlg[7] = (CESL_Mgr*)m_pMngLocMgr08;
	ppDlg[8] = (CESL_Mgr*)m_pMngLocMgr09;
	ppDlg[9] = (CESL_Mgr*)m_pMngLocMgr10;

	for( INT i=0 ; i < nCnt ; i++ )
	{
		ppDlg[i]->pMain = this->pMain;
		ppDlg[i]->pParentMgr = (CESL_Mgr*)this;
		ppDlg[i]->SM_STYLE = 'Z';
		ppDlg[i]->m_pInfo = this->m_pInfo;

		if( i==0 ) ids = ((CMngLocMgr01Dlg*)ppDlg[i])->Create(this);
		if( i==1 ) ids = ((CMngLocMgr02Dlg*)ppDlg[i])->Create(this);
		if( i==2 ) ids = ((CMngLocMgr03Dlg*)ppDlg[i])->Create(this);
		if( i==3 ) ids = ((CMngLocMgr04Dlg*)ppDlg[i])->Create(this);
		if( i==4 ) ids = ((CMngLocMgr05Dlg*)ppDlg[i])->Create(this);
		if( i==5 ) ids = ((CMngLocMgr06Dlg*)ppDlg[i])->Create(this);
		if( i==6 ) ids = ((CMngLocMgr07Dlg*)ppDlg[i])->Create(this);
		if( i==7 ) ids = ((CMngLocMgr08Dlg*)ppDlg[i])->Create(this);
		if( i==8 ) ids = ((CMngLocMgr09Dlg*)ppDlg[i])->Create(this);
		if( i==9 ) ids = ((CMngLocMgr10Dlg*)ppDlg[i])->Create(this);

		if( ids )
		{
			ppDlg[i]->ShowWindow(SW_SHOW);
			ppDlg[i]->UpdateWindow();
			ppDlg[i]->CenterWindow();
		}
	}

	delete []ppDlg;
	return 0;

	EFS_END
	return -1;

}

INT CMngLocMgrDlg::DefaultView()
{
	EFS_BEGIN

	INT ids;

	ids = ViewClientDlg(0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DefaultView") );

	return 0;

	EFS_END
	return -1;

}


INT CMngLocMgrDlg::ViewClientDlg(INT nIndex)
{
	EFS_BEGIN


	m_pMngLocMgr01->ShowWindow(SW_HIDE);
	m_pMngLocMgr02->ShowWindow(SW_HIDE);
	m_pMngLocMgr03->ShowWindow(SW_HIDE);
	m_pMngLocMgr04->ShowWindow(SW_HIDE);
	m_pMngLocMgr05->ShowWindow(SW_HIDE);
	m_pMngLocMgr06->ShowWindow(SW_HIDE);
	m_pMngLocMgr07->ShowWindow(SW_HIDE);
	m_pMngLocMgr08->ShowWindow(SW_HIDE);
	m_pMngLocMgr09->ShowWindow(SW_HIDE);
	m_pMngLocMgr10->ShowWindow(SW_HIDE);

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMENU);
	if( nIndex == -1 )
	{	
		nIndex = pTab->GetCurSel();
	}

	TCITEM pTabCtrlItem;
	TCHAR szBuff[64]; 
	pTabCtrlItem.mask = TCIF_TEXT; 
	pTabCtrlItem.pszText = szBuff; 
	pTabCtrlItem.cchTextMax = sizeof(szBuff);	
	pTab->GetItem(nIndex, &pTabCtrlItem);
	if ( _tcscmp(szBuff, _T(" 대출/반납 형식 && 이용자 ") ) == 0 ) nIndex = 0;
	if ( _tcscmp(szBuff, _T(" 반납 연기 && 예약 환경 ") ) == 0 ) nIndex = 1;
	if ( _tcscmp(szBuff, _T(" 연체 자료 ") ) == 0 ) nIndex = 2;
	if ( _tcscmp(szBuff, _T(" 직급별 환경 ") ) == 0 ) nIndex = 3;
	if ( _tcscmp(szBuff, _T(" 대출자료 제한 ") ) == 0 ) nIndex = 4;
	if ( _tcscmp(szBuff, _T(" 메일 관리 ") ) == 0 ) nIndex = 5;
	if ( _tcscmp(szBuff, _T(" 통합대출관리 ") ) == 0 ) nIndex = 6;
	if ( _tcscmp(szBuff, _T(" 개인정보보호정책 ") ) == 0 ) nIndex = 7;
	if ( _tcscmp(szBuff, _T(" 무인대출예약설정 ") ) == 0 ) nIndex = 8;
	if ( _tcscmp(szBuff, _T(" 수수료 관리 ") ) == 0 ) nIndex = 9;

	if( nIndex == 0 )
	{
		m_pMngLocMgr01->ShowWindow(SW_SHOW);
		m_pMngLocMgr01->ViewScreen();
	}
	else if( nIndex == 1 )
	{
		m_pMngLocMgr02->ShowWindow(SW_SHOW);
		m_pMngLocMgr02->ViewScreen();
	}
	else if( nIndex == 2 )
	{
		m_pMngLocMgr03->ShowWindow(SW_SHOW);
		m_pMngLocMgr03->ViewScreen();
	}
	else if( nIndex == 3 )
	{
		m_pMngLocMgr04->ShowWindow(SW_SHOW);
		m_pMngLocMgr04->ViewScreen();
	}
	else if( nIndex == 4 )
	{
		m_pMngLocMgr05->ShowWindow(SW_SHOW);
		m_pMngLocMgr05->ViewScreen();
	}
	else if( nIndex == 5 )
	{
		m_pMngLocMgr06->ShowWindow(SW_SHOW);
		m_pMngLocMgr06->ViewScreen();
	}
	else if( nIndex == 6 )
	{
		m_pMngLocMgr07->ShowWindow(SW_SHOW);
		m_pMngLocMgr07->ViewScreen();
	}
	else if( nIndex == 7 )
	{
		m_pMngLocMgr08->ShowWindow(SW_SHOW);
		m_pMngLocMgr08->ViewScreen();
	}
	else if( nIndex == 8 )
	{
		m_pMngLocMgr09->ShowWindow(SW_SHOW);
		m_pMngLocMgr09->ViewScreen();
	}
	else if( nIndex == 9 )
	{
		m_pMngLocMgr10->ShowWindow(SW_SHOW);
		m_pMngLocMgr10->ViewScreen();
	}


	return 0;

	EFS_END
	return -1;

}

INT CMngLocMgrDlg::SaveClientDlg()
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMENU);
	
	INT nIndex = pTab->GetCurSel();

	INT ids;

	TCITEM pTabCtrlItem;
	TCHAR szBuff[64]; 
	pTabCtrlItem.mask = TCIF_TEXT; 
	pTabCtrlItem.pszText = szBuff; 
	pTabCtrlItem.cchTextMax = sizeof(szBuff);	
	pTab->GetItem(nIndex, &pTabCtrlItem);
	if ( _tcscmp(szBuff, _T(" 대출/반납 형식 && 이용자 ") ) == 0 ) nIndex = 0;
	if ( _tcscmp(szBuff, _T(" 반납 연기 && 예약 환경 ") ) == 0 ) nIndex = 1;
	if ( _tcscmp(szBuff, _T(" 연체 자료 ") ) == 0 ) nIndex = 2;
	if ( _tcscmp(szBuff, _T(" 직급별 환경 ") ) == 0 ) nIndex = 3;
	if ( _tcscmp(szBuff, _T(" 대출자료 제한 ") ) == 0 ) nIndex = 4;
	if ( _tcscmp(szBuff, _T(" 메일 관리 ") ) == 0 ) nIndex = 5;
	if ( _tcscmp(szBuff, _T(" 통합대출관리 ") ) == 0 ) nIndex = 6;
	if ( _tcscmp(szBuff, _T(" 개인정보보호정책 ") ) == 0 ) nIndex = 7;
	if ( _tcscmp(szBuff, _T(" 무인대출예약설정 ") ) == 0 ) nIndex = 8;
	if ( _tcscmp(szBuff, _T(" 수수료 관리 ") ) == 0 ) nIndex = 9;

	if( nIndex == 0 )
	{
		ids = m_pMngLocMgr01->SaveScreen();
		//++2008.06.16 ADD BY PWR {{++
		// 재대출제한일 0에서 100으로 제한 오류 수정
		if( ids == 1000 )
			return 0;
		//--2008.06.16 ADD BY PWR --}}
	}
	if( nIndex == 1 )
		ids = m_pMngLocMgr02->SaveScreen();
	if( nIndex == 2 )
		ids = m_pMngLocMgr03->SaveScreen();
	if( nIndex == 3 )
		ids = m_pMngLocMgr04->SaveScreen();
	if( nIndex == 4 )
		ids = m_pMngLocMgr05->SaveScreen();
	if( nIndex == 5 )
		ids = m_pMngLocMgr06->SaveScreen();
	if( nIndex == 6 )
		ids = m_pMngLocMgr07->SaveScreen();
	if( nIndex == 7 )
		ids = m_pMngLocMgr08->SaveScreen();
	if( nIndex == 8 )
		ids = m_pMngLocMgr09->SaveScreen();
	if( nIndex == 9 )
		ids = m_pMngLocMgr10->SaveScreen();

	if( ids < 0 ) AfxMessageBox(_T("실패했습니다."));
	else
		AfxMessageBox(_T("수정되었습니다."));

	return 0;

	EFS_END
	return -1;

}

VOID CMngLocMgrDlg::OnSelchangetabMENU(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMENU);

	ViewClientDlg( pTab->GetCurSel() );
	
	*pResult = 0;

	EFS_END
}


INT CMngLocMgrDlg::GetAllEtcSettingValue()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_잡다구리설정값2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetAllEtcSettingValue") );

	ids = CLocCommonAPI::RefreshDataManager( this , pDM , _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetAllEtcSettingValue") );

	return 0;

	EFS_END

	return -1;
}

HBRUSH CMngLocMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
