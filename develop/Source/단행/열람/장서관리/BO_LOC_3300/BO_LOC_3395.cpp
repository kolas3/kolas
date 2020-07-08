// BO_LOC_3395.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3395.h"
#include "BO_LOC_3000Api.h"
#include "PersonInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3395 dialog


CBO_LOC_3395::CBO_LOC_3395(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3395)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLibInfoDlg = NULL;
	m_pLoanCardFormatSetter = NULL;

	GetManageValue( _T("기타") , _T("공통") , _T("국방대") , _T("") , m_sDefenceFlag );	
	m_sDefenceFlag.TrimLeft();m_sDefenceFlag.TrimRight();

	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService);
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");
	
}

CBO_LOC_3395::~CBO_LOC_3395()
{
	SaveDefaultLoanCardInfo();
	
	if( m_pLibInfoDlg != NULL )
		delete m_pLibInfoDlg;
	m_pLibInfoDlg = NULL;

	if( m_pLoanCardFormatSetter )
	{
		delete m_pLoanCardFormatSetter;
		m_pLoanCardFormatSetter = NULL;
	}
}

VOID CBO_LOC_3395::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3395)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3395, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3395)
	ON_BN_CLICKED(IDC_btnUSER_PRINT, OnbtnUSERPRINT)
	ON_BN_CLICKED(IDC_btnUSER_CLOSE, OnbtnUSERCLOSE)
	ON_BN_CLICKED(IDC_btnORDER_ADD, OnbtnORDERADD)
	ON_BN_CLICKED(IDC_btnORDER_DELTE, OnbtnORDERDELTE)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_spnUPDOWN, OnOutofmemoryspnUPDOWN)
	ON_NOTIFY(UDN_DELTAPOS, IDC_spnUPDOWN, OnDeltaposspnUPDOWN)
	ON_NOTIFY(NM_DBLCLK, IDC_lstDST, OnDblclklstDST)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabPRINT, OnSelchangetabPRINT)
	ON_WM_MOVE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_cLOAN_CARD_TYPE, OnSelchangecLOANCARDTYPE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3395 message handlers

BOOL CBO_LOC_3395::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_3395")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	InitControl();

	m_pPrintDM = FindDM(_T("DM_BO_LOC_3395"));
	if( m_pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pOrderFieldMapDM = FindDM( _T("DM_BO_LOC_3395_ORDER_MAP_FIELD") );
	if( m_pOrderFieldMapDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	INT ids = MakeOrderFieldMap();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	m_pLibInfoDlg = new CBO_LOC_3396(this);
	m_pLibInfoDlg->Create(this);
	ShowHideLibInfo();

	m_pLoanCardFormatSetter = new CLoanCardFormatSetter( this );
	m_pLoanCardFormatSetter->Create( this );
	

	MakeLoanCardTypeCombo();
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cLOAN_CARD_TYPE );
	if( m_pLoanCardFormatSetter->m_defaultLoanCardType.Find( _T("NEW") ) > -1 )
		pCombo->SetCurSel( 1 );
	else
		pCombo->SetCurSel( 0 );

	if ( m_sDefenceFlag == _T("Y") )
		InitNDScreen();

	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_staMANAGE_CODE)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmbLOAN_MANAGE_CODE)->ShowWindow(SW_HIDE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


BOOL CBO_LOC_3395::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3395::OnbtnUSERPRINT() 
{
	PrintUserList();
}

VOID CBO_LOC_3395::OnbtnUSERCLOSE() 
{
	EFS_BEGIN

	m_pLibInfoDlg->ShowWindow(SW_HIDE);
	
	OnCancel();	

	EFS_END
}


INT CBO_LOC_3395::PrintUserList()
{
	INT ids;

	CString sWhere;
	INT nMode = GetPrintMode();

	if( nMode == 3 && (((CButton*)GetDlgItem(IDC_chkSELECTGRID))->GetCheck() == 1) )	
	{
		MakePrintDMFromSaveUserDM();
	}
	else
	{
		ids = MakeSql(sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("PrintUserList") );
		if( ids > 0 ) return 0;
		
		ids = MakeOrderBy(sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("PrintUserList") );		

		ids = CLocCommonAPI::RefreshDataManager( this , m_pPrintDM , sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("PrintUserList") );
	}

	if( m_pPrintDM->GetRowCount() < 1 ) 
	{
		AfxMessageBox( _T("목록으로 출력할 자료가 존재하지 않습니다."));
		return 1;
	}

	if(PrivacyPrintLogRecord(m_pPrintDM, _T("이용자 관리 출력"), _T("R") ,0 ) < 0 ) return 1;

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pPrintDM , _T("대출소속정보") , _T("대출소속정보") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pPrintDM , _T("대출직급정보") , _T("대출직급정보") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pPrintDM , _T("자료실구분") , _T("자료실") );

	CString strIsChangeSecuriy;
	GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strIsChangeSecuriy );

	CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8;

	CString sCode;
	CString sOrder;
	for( INT i = 0 ; i < m_pPrintDM->GetRowCount() ; i++ )
	{
		ids = m_pPrintDM->GetCellData( _T("불량회원구분") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("PrintUserList") );

		if( _ttoi(sCode) == 0 )
			sCode = _T("정상회원");
		else if( _ttoi(sCode) == 1 )
			sCode = _T("대출정지");
		else if( _ttoi(sCode) == 2 )
			sCode = _T("제적회원");
		else if( _ttoi(sCode) == 3 )
			sCode = _T("탈퇴회원");

		ids = m_pPrintDM->SetCellData( _T("불량회원구분") , sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("PrintUserList") );

		sOrder.Format(_T("%d") , i+1 );
		ids = m_pPrintDM->SetCellData( _T("순위") , sOrder, i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("PrintUserList") );

		if( _T("Y") != strIsChangeSecuriy )
		{
			ids = m_pPrintDM->GetCellData( _T("대출자번호") , i , sCode );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("PrintUserList") );
			CLocCommonAPI::ChangeSecuriyUserNo( this , sCode , m_sIsUnityLoanService );
			ids = m_pPrintDM->SetCellData( _T("대출자번호") , sCode , i );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("PrintUserList") );
		}

		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CString strGetData;
			m_pPrintDM->GetCellData(_T("자택주소"), i, strGetData);
			asDes1.Add(strGetData);
			m_pPrintDM->GetCellData(_T("근무지주소"), i, strGetData);
			asDes2.Add(strGetData);
			m_pPrintDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes3.Add(strGetData);
			m_pPrintDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes4.Add(strGetData);
			m_pPrintDM->GetCellData(_T("핸드폰"), i, strGetData);
			asDes5.Add(strGetData);
			m_pPrintDM->GetCellData(_T("비밀번호"), i, strGetData);
			asDes6.Add(strGetData);
			m_pPrintDM->GetCellData(_T("자택전화번호"), i, strGetData);
			asDes7.Add(strGetData);
			m_pPrintDM->GetCellData(_T("근무지전화번호"), i, strGetData);
			asDes8.Add(strGetData);
		}
	}

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < m_pPrintDM->GetRowCount())
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes1);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes2);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes3);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes4);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("PASSWORD"), asDes6);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes7);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes8);
					p3000Api->CloseSocket();	
				}
				for(i = 0; i < m_pPrintDM->GetRowCount(); i++)
				{
					m_pPrintDM->SetCellData(_T("자택주소"), asDes1.GetAt(i), i);
					m_pPrintDM->SetCellData(_T("근무지주소"), asDes2.GetAt(i), i);
					m_pPrintDM->SetCellData(_T("주민등록번호"), asDes3.GetAt(i), i);
					m_pPrintDM->SetCellData(_T("E_MAIL"), asDes4.GetAt(i), i);
					m_pPrintDM->SetCellData(_T("핸드폰"), asDes5.GetAt(i), i);
					m_pPrintDM->SetCellData(_T("비밀번호"), asDes6.GetAt(i), i);
					m_pPrintDM->SetCellData(_T("자택전화번호"), asDes7.GetAt(i), i);
					m_pPrintDM->SetCellData(_T("근무지전화번호"), asDes8.GetAt(i), i);
				}
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
		asDes7.RemoveAll(); asDes7.FreeExtra();
		asDes8.RemoveAll(); asDes8.FreeExtra();
	}

	CString sPrintAlias;
	BOOL bIsPrint = FALSE;

	if( nMode == 0 )
		sPrintAlias = _T("이용자목록");
	else if( nMode == 1 )
		sPrintAlias = _T("장기미대출자목록");
	else if( nMode == 2 )
		sPrintAlias = _T("연체회원목록");
	else if( nMode == 3 )
		sPrintAlias = _T("대출증");
	else if( nMode == 4 )
		sPrintAlias = _T("이용자상세정보");
	
	if( nMode==0 || (nMode==1) || (nMode==2) )
	{
		CLocCommonAPI::ChangeSecuriyCivilNo( this, m_pPrintDM, -1, _T("주민등록번호") , m_sIsUnityLoanService );		
		
		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
			return 1;
		}
		pSIReportManager->SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
		ids = pSIReportManager->GetSIReportFileFromDB(sPrintAlias);
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return 1;
		}

		pSIReportManager->SetDataMgr(0, m_pPrintDM, 0);

		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}	
	}
	else if( nMode == 3 )
	{   
		ids = UserCardPrint( sPrintAlias, &bIsPrint );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintUserList") );
	}
	else if( nMode == 4 )
	{   
		ids = UserDetailInfoPrint( sPrintAlias, &bIsPrint  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintUserList") );
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_PRINT_LOG") );

	std::vector<CString> list;
	CString strKey;
	for(i=0; i<m_pPrintDM->GetRowCount(); i++)
	{
		ids = m_pPrintDM->GetCellData(_T("대출자KEY"), i, strKey );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3395::PrintUserList()") );
		list.push_back(strKey);
	}

	if( nMode < 3)
		CPersonInfo::SetPrintLog(this, pDM, PERINFO_EXCEL, m_pInfo, sPrintAlias, m_pPrintDM->GetRowCount(), &list);
	else if(bIsPrint)
		CPersonInfo::SetPrintLog(this, pDM, PERINFO_PRINT, m_pInfo, sPrintAlias, m_pPrintDM->GetRowCount(), &list);

	list.clear();
	
	return 0;

}

INT CBO_LOC_3395::UserCardPrint( CString sPrintAlias, BOOL* pIsPrint )
{
	INT ids = 0;

	if( GetLoanCardType() )
	{
		CString sTmpRegDate = _T("");
		CString sTmpCivilNo = _T("");

		for ( INT f= 0 ; f < m_pPrintDM->GetRowCount( ) ; f++ )
		{
			ids = m_pPrintDM->GetCellData( _T("등록일"), f, sTmpRegDate );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UserCardPrint") );
			ids = m_pPrintDM->GetCellData( _T("주민등록번호"), f, sTmpCivilNo );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UserCardPrint") );

			ids = MakeUserCardInputValue( sTmpRegDate, sTmpCivilNo );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UserCardPrint") );

			ids = m_pPrintDM->SetCellData( _T("등록일"), sTmpRegDate, f );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UserCardPrint") );
			ids = m_pPrintDM->SetCellData( _T("주민등록번호"), sTmpCivilNo, f );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UserCardPrint") );
		}

		m_pLoanCardFormatSetter->PrintLoanCard( m_pPrintDM );
		return 0;
	}
	bool IsPrint = false;
	const INT nTmpAliasCnt = 4;

	CString sLibName , sHomePage , sPhoneNumber;
	if( m_pLibInfoDlg->GetLibInfo( sHomePage , sLibName , sPhoneNumber ) > 0 )
		return 1;
	INT nStartPos;
	if( m_pLibInfoDlg->GetStartPos( nStartPos ) > 0 )
		return 1;
	nStartPos--;

	CString sUserName[4];
	CString sRegDate[4];
	CString sUserNo[4];
	CString sCivilNo[4];

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(sPrintAlias);
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 2;
	}
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;
	ids = pSIReportManager->DeleteTempPageFiles();

	CSIReportWorkSheet *pSheet = pSIReportManager->GetSheet(0);
	if (pSheet == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UserCardPrint") );


	
	pSIReportManager->SetTempVariable( _T("도서관명") , sLibName );
	pSIReportManager->SetTempVariable( _T("홈페이지") , sHomePage  );
	pSIReportManager->SetTempVariable( _T("전화번호") , sPhoneNumber );


	CString sTmpAlias[nTmpAliasCnt] =
	{
		_T("대출자이름") , _T("등록일") , _T("대출자번호") , _T("주민등록번호")
	};
	
	INT nIndex;
	for( INT i = nStartPos ; i < m_pPrintDM->GetRowCount() + nStartPos ; i++ )
	{
		nIndex = i - nStartPos;

		IsPrint = FALSE;

		if( i == m_pPrintDM->GetRowCount()-1 ) IsPrint = TRUE;
		if( (i+1)%4==0 && (i!=0) ) IsPrint = TRUE;

		ids = m_pPrintDM->GetCellData( sTmpAlias[0] , nIndex , sUserName[(i)%4] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );
		ids = m_pPrintDM->GetCellData( sTmpAlias[1] , nIndex , sRegDate[(i)%4] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("") );
		ids = m_pPrintDM->GetCellData( sTmpAlias[2] , nIndex , sUserNo[(i)%4] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("") );
		ids = m_pPrintDM->GetCellData( sTmpAlias[3] , nIndex , sCivilNo[(i)%4] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("") );
		
		ids = MakeUserCardInputValue( sRegDate[(i)%4] ,sCivilNo[(i)%4] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UserCardPrint") );

		if( !IsPrint ) continue;

		CString sVariableName;
		for( INT j = 0 ; j < nTmpAliasCnt ; j++ )
		{
			sVariableName.Format( _T("%s%d") , sTmpAlias[0] , j+1 );
			pSIReportManager->SetTempVariable( sVariableName , sUserName[j] );
			sVariableName.Format( _T("%s%d") , sTmpAlias[1] , j+1 );
			pSIReportManager->SetTempVariable( sVariableName , sRegDate[j] );
			sVariableName.Format( _T("%s%d") , sTmpAlias[3] , j+1 );
			pSIReportManager->SetTempVariable( sVariableName , sCivilNo[j] );
			sVariableName.Format( _T("%s%d") , sTmpAlias[2] , j+1 );
			pSIReportManager->SetTempVariable( sVariableName , sUserNo[j] );

			sVariableName.Format( _T("%s%d") , sTmpAlias[2] , j+1 );
			CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pSIReportManager->FindObject(pSheet, sVariableName );
			if (pBarcode == NULL)  ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UserCardPrint") );
			pBarcode->SetCaption(sUserNo[j]);

		}

		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("UserCardPrint") );

		for( j = 0 ; j < nTmpAliasCnt ; j++ )
		{
			sUserName[j].Empty();
			sRegDate[j].Empty();
			sUserNo[j].Empty();
			sCivilNo[j].Empty();
		}

	}

	ids = pSIReportManager->Print2(pIsPrint);
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	return 0;

}

INT CBO_LOC_3395::MakeSql( CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	ids = IsInputCond(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSql") );
	if( ids > 0 ) return ids;

	const INT nCnt = 14;

	CString sGetData[nCnt];
	CString sCMAlias = _T("CM_BO_LOC_3395");

	TCHAR *sDBFieldName[nCnt] =
	{
		_T("CLU.USER_NO")                 , _T("CLU.USER_NO")                 , _T("CLU.NAME")           , _T("CLU.NAME") , 
		_T("CLU.REG_DATE")                , _T("CLU.REG_DATE")                , _T("CLU.LAST_LOAN_DATE") , _T("CLU.LAST_LOAN_DATE") , 
		_T("CLU.USER_POSITION_CODE") , _T("CLU.USER_POSITION_CODE") , _T("CLU.SHELF_LOC_CODE") , _T("CLU.CLASS_NO") ,
		_T("CLU.CLASS_NO"),			_T("CLU.MANAGE_CODE")
	};

	INT nDBFieldType[nCnt] = 
	{
		TWOSTRING , ETC , TWOSTRING , ETC , 
		TWODATE   , ETC , TWODATE   , ETC ,
		TWOSTRING , ETC , ONESTRING , TWOSTRING ,
		ETC, ONESTRING
	};

	TCHAR *sControlName[nCnt] =
	{
		_T("대출자번호1")   , _T("대출자번호2")   , _T("대출자명1")   , _T("대출자명2") , 
		_T("등록일1")       , _T("등록일2")       , _T("최종대출일1") , _T("최종대출일2") ,
		_T("대출소속정보1") , _T("대출소속정보2") , _T("자료실")      , _T("교번1") ,
		_T("교번2")			, _T("관리구분")
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sControlName[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSql") );
	}

	if(3 == GetPrintMode())
	{
		CString strValue;
		GetManageValue(_T("열람"), _T("공통"), _T("통합대출자"), _T("통합대출권수사용여부"), strValue);
		if(_T("Y") == strValue)
		{
			ids = CLocCommonAPI::MakeSql( sGetData , sDBFieldName , nDBFieldType , nCnt-1 , sWhere , this);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSql") );
		}
		else
		{
			ids = CLocCommonAPI::MakeSql( sGetData , sDBFieldName , nDBFieldType , nCnt , sWhere , this);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSql") );
		}
	}
	else
	{
		ids = CLocCommonAPI::MakeSql( sGetData , sDBFieldName , nDBFieldType , nCnt , sWhere , this);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSql") );
	}

	if( sWhere.IsEmpty() )
	{
		AfxMessageBox( _T("조건을 입력해주십시요") );
		return 1;
	}

	sWhere += _T(" AND (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1') ");

	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3395::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3395::InitControl()
{
	EFS_BEGIN

	const INT nMenuCnt = 5;
	TCHAR *sMenuName[nMenuCnt] = 
	{
		_T("회원가입명단") , _T("장기미대출자목록") , _T("연체회원목록") , _T("대출증") , _T("이용자상세정보")
	};

	for( INT i = 0 ; i < nMenuCnt ; i++ )
	{
		((CTabCtrl*)GetDlgItem(IDC_tabPRINT))->InsertItem(i,sMenuName[i]);
	}

	const INT nOrderCnt=5;
	TCHAR *sOrderList[nOrderCnt] = 
	{
		_T("대출자번호") , _T("대출자이름") , _T("대출소속정보") , _T("등록일") , _T("최종대출일") 
	};

	LVCOLUMNW lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = 130;
	lvcolumn.pszText = _T("정렬조건");
	
	CListCtrl *pList;
	pList = ((CListCtrl*)GetDlgItem(IDC_lstSRC));
	pList->InsertColumn(0,&lvcolumn);
	for( i = 0 ; i < nOrderCnt ; i++ )
	{		
		pList->InsertItem( i , sOrderList[i] );
	}

	lvcolumn.pszText = _T("조건");
	lvcolumn.cx = 90;
	pList = ((CListCtrl*)GetDlgItem(IDC_lstDST));
	pList->SetExtendedStyle(pList->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	
	
	pList->InsertColumn(0,&lvcolumn);
	
	lvcolumn.pszText = _T("차순");
	lvcolumn.cx = 40;
	pList->InsertColumn(1,&lvcolumn);
	
	return 0;

	EFS_END
	return -1;

}


VOID CBO_LOC_3395::OnbtnORDERADD() 
{
	EFS_BEGIN

	CListCtrl *pSrcList,*pDstList;
	pSrcList = ((CListCtrl*)GetDlgItem(IDC_lstSRC));
	pDstList = ((CListCtrl*)GetDlgItem(IDC_lstDST));

	MoveOrderList(pSrcList,pDstList);


	EFS_END
}


INT CBO_LOC_3395::MoveOrderList(CListCtrl *pSrcList,CListCtrl *pDstList)
{
	EFS_BEGIN

	INT nSelectCount = pSrcList->GetSelectedCount();

	INT nMode;
	CListCtrl* pModeList = (CListCtrl*)GetDlgItem(IDC_lstSRC);
	
	if( pModeList == pSrcList )
		nMode = 0;
	else
		nMode = 1;

	POSITION pos;
	pos = pSrcList->GetFirstSelectedItemPosition();
	CString sData;
	CStringArray saSelectIdx;

	INT nIndex;
	INT nItemRowIndex;
	for( INT i = 0 ; i < nSelectCount ; i++ )
	{
		nIndex = pSrcList->GetNextSelectedItem(pos);

		sData.Format( _T("%d") , nIndex );
		saSelectIdx.Add(sData);
		sData = pSrcList->GetItemText( nIndex , 0 );

		nItemRowIndex = pDstList->GetItemCount();
		pDstList->InsertItem( nItemRowIndex , sData );

		if( nMode == 0 )
			pDstList->SetItemText(nItemRowIndex,1,_T("오름"));
		
		
	}
	
	for( i = saSelectIdx.GetSize()-1 ; i > -1 ; i-- )
	{
		pSrcList->DeleteItem( _ttoi(saSelectIdx.GetAt(i)) );
	}

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3395::OnbtnORDERDELTE() 
{
	EFS_BEGIN

	CListCtrl *pSrcList,*pDstList;
	pSrcList = ((CListCtrl*)GetDlgItem(IDC_lstSRC));
	pDstList = ((CListCtrl*)GetDlgItem(IDC_lstDST));

	MoveOrderList(pDstList,pSrcList);
	

	EFS_END
}

INT CBO_LOC_3395::MakeOrderBy( CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	CString sOrderBy;
	CString sData,sData2;

	CListCtrl *pDstList;
	pDstList = ((CListCtrl*)GetDlgItem(IDC_lstDST));

	for( INT i = 0 ; i < pDstList->GetItemCount() ; i++ )
	{
		sData = pDstList->GetItemText( i , 0 );
		sData2 = pDstList->GetItemText( i , 1 );

		ids = GetCellData( m_pOrderFieldMapDM , _T("조건종류") , sData , _T("필드명") , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderBy") );
		
		if( !sOrderBy.IsEmpty() )
			sOrderBy += _T(" , ");
		sOrderBy += sData;
		if( sData2.Compare(_T("오름")) == 0 )
			sOrderBy += _T(" ASC ");
		else
			sOrderBy += _T(" DESC ");
	}

	if( !sOrderBy.IsEmpty() )
		sWhere += _T(" ORDER BY ") + sOrderBy;

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3395::GetCellData( CESL_DataMgr *pDM , CString sConditionAlias , CString sConditionData , CString sGetAlias , CString &sGetData )
{
	EFS_BEGIN

	INT ids;
	ids = CLocCommonAPI::GetCellData( this , pDM , sConditionAlias , sConditionData  , sGetAlias , sGetData );
	return ids;

	EFS_END
	return -1;

}

INT CBO_LOC_3395::MakeOrderFieldMap()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pOrderFieldMapDM;

	CString sFilePath = _T("..\\cfg\\출력물_이용자관리\\정렬조건생성맵.txt");

	CStdioFile fFile;
	ids = fFile.Open( sFilePath , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( ids==0 )
	{
		AfxMessageBox( _T("파일명이 잘못되었습니다.\n올바른 경로인지 확인해 주십시오.") );
		return 1;
    }
		TCHAR cUni;
		fFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fFile.SeekToBegin();
		}
	
	if( fFile.GetLength() == 0 ) return 0;

	CString sLine;
	CArray < CString , CString > asColumnList;

	fFile.ReadString( sLine );

	ids = CLocCommonAPI::MakeArrayList( sLine , _T("|") , asColumnList );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeStaticsMainInfoDM") );

	CArray < CString , CString > asDataList;

    while ( fFile.ReadString( sLine ) ) 
	{
        sLine.TrimRight(); sLine.TrimLeft();
		if( sLine.IsEmpty() ) continue;

		pDM->InsertRow(-1);

		ids = CLocCommonAPI::MakeArrayList( sLine , _T("|") , asDataList );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeStaticsMainInfoDM") );

		for( INT i = 0 ; i < asDataList.GetSize() ; i++ )
		{
			ids = pDM->SetCellData( asColumnList.GetAt(i) , asDataList.GetAt(i) , pDM->GetRowCount()-1 );
			if( ids < 0 ) 
			{
				CString sMsg;
				sMsg.Format( _T("sLine : %s , pDM->SetCellData(%s,%s,%d) "), sLine ,  asColumnList.GetAt(i) , asDataList.GetAt(i) , pDM->GetRowCount()-1 );
				AfxMessageBox( sMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeStaticsMainInfoDM") );
		}

	}

	fFile.Close();
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3395::IsInputCond( CString &sWhere )
{
	EFS_BEGIN

	INT ids;
	INT nMode = GetPrintMode();


	CString sCMAlias = _T("CM_BO_LOC_3395");
	const INT nCnt = 3;
	TCHAR *sControlAlias[nCnt] =
	{
		_T("미대출기간1") , _T("미대출기간2") , _T("연체수") 
	};

	CString sData[nCnt];

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sControlAlias[i] , sData[i] ,  -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );
		sData[i].TrimLeft(); sData[i].TrimRight();
	}
	
	if( nMode == 1 )
	{
		if( sData[0].IsEmpty() )
		{
			AfxMessageBox( _T("미대출기간을 입력하십시요") );
			GetDlgItem(IDC_edtPRINT_NON_LOAN_DATE1)->SetFocus();
			return 1;
		}
	}

	if( nMode == 2 )
	{
		if( sData[2].IsEmpty() )
		{
			AfxMessageBox( _T("연체수를 입력하십시요") );
			GetDlgItem(IDC_edtPRINT_DELAY_CNT)->SetFocus();
			return 1;
		}
	}

	CString sTmpWhere,sTmpWhere2;

	if( !sData[0].IsEmpty() )
	{
		sTmpWhere.Format( _T(" (SELECT COUNT(*) FROM LH_STORAGE_V01 CL ")
							_T("WHERE CL.USER_KEY = CLU.REC_KEY AND to_char(CL.LOAN_DATE,'YYYY/MM/DD') ") );
		if( !sData[1].IsEmpty() )
			sTmpWhere2.Format(	_T(" BETWEEN '%s' AND '%s' )=0") , sData[0] , sData[1] );
		else 
			sTmpWhere2.Format(	_T(" = '%s')=0 ") , sData[0] );

		sTmpWhere += sTmpWhere2;
	}
	if( !sWhere.IsEmpty() && !sTmpWhere.IsEmpty() )
		sWhere += _T(" AND ");;

	sWhere += sTmpWhere;

	sTmpWhere.Empty();
	if( !sData[2].IsEmpty() )
	{
		sTmpWhere.Format( _T(" (SELECT COUNT(*) FROM LS_WORK_T01 CL ")
							_T("WHERE CL.USER_KEY = CLU.REC_KEY AND CL.STATUS IN ('0','2')) >= %s ") , sData[2] );
	}
	if( !sWhere.IsEmpty() && !sTmpWhere.IsEmpty() )
		sWhere += _T(" AND ");;

	sWhere += sTmpWhere;

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3395::GetPrintMode()
{
	EFS_BEGIN

	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabPRINT);
	
	return pTab->GetCurSel();

	EFS_END
	return -1;

}

VOID CBO_LOC_3395::OnOutofmemoryspnUPDOWN(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	EFS_BEGIN

	*pResult = 0;

	EFS_END
}

INT CBO_LOC_3395::MoveListRecord( INT nMode , CListCtrl *pList )
{
	EFS_BEGIN

	POSITION pos;

	pos = pList->GetFirstSelectedItemPosition();

	CString sData,sData2;
	CStringArray saSelectIdx;

	INT nIndex = pList->GetNextSelectedItem(pos);

	if( nMode == 0 && (nIndex == 0) )
		return 0;
	if( nMode == 1 && (nIndex == pList->GetItemCount()-1 ) )
		return 0;
	
	sData = pList->GetItemText( nIndex , 0 );
	sData2 = pList->GetItemText( nIndex , 1 );
	
	if( nMode == 0 )
	{
		pList->InsertItem( nIndex-1 , sData );		
		pList->SetItemText( nIndex-1 , 1 , sData2 );		
		pList->DeleteItem( nIndex+1 );
		pList->SetItem( nIndex-1, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED,
			LVIS_SELECTED, 0);
	}
	if( nMode == 1 )
	{
		pList->InsertItem( nIndex+2 , sData );		
		pList->SetItemText( nIndex+2 , 1 , sData2 );		
		pList->DeleteItem( nIndex );
		pList->SetItem( nIndex+1, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED,
			LVIS_SELECTED, 0);
	}


	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3395::OnDeltaposspnUPDOWN(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	CListCtrl *pList = ((CListCtrl*)GetDlgItem(IDC_lstDST));	

	if( pList->GetItemCount() < 1 )
	{
		*pResult = 0;
		return ;
	}
	if( pList->GetSelectedCount() < 1 )
	{
		*pResult = 0;
		return ;
	}
	if( pList->GetSelectedCount() > 1 )
	{
		AfxMessageBox( _T("하나만 선택해주십시요") );
		*pResult = 0;
		return ;
	}
    
    NMUPDOWN* pNMUpDown = (NMUPDOWN*)pNMHDR;
    
    if(0 > pNMUpDown->iDelta)
    {
		MoveListRecord( 0 , pList );
    }
    else
    {
		MoveListRecord( 1 , pList );
    }
    
	
	*pResult = 0;

	EFS_END
}

INT CBO_LOC_3395::MakeUserCardInputValue( CString &sRegDate , CString &sCivilNo )
{
	EFS_BEGIN

	DateFormatConvert( sRegDate );

	CLocCommonAPI::ChangeSecuriyCivilNo(this, sCivilNo, m_sIsUnityLoanService);

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3395::DateFormatConvert( CString &sDate )
{
	EFS_BEGIN

	CStringArray saDate;

	sDate.TrimLeft(); sDate.TrimRight();

	if( sDate.IsEmpty() )
		return 0;

	CLocCommonAPI::MakeArrayList( sDate , _T("/") , saDate );

	if( saDate.GetSize() != 3 ) return 0;

	sDate.Format( _T("%s년%s월%s일") , saDate.GetAt(0) , saDate.GetAt(1) , saDate.GetAt(2));

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3395::CivilNoBlindProc( CString &sCivilNo )
{
	EFS_BEGIN

	INT nIndex = 0;

	CString sQuery = _T("");
	CString sCivilOption = _T("");
	sQuery.Format( 
		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
		_T("WHERE GROUP_1='L' AND GROUP_2='열람' AND GROUP_3='공통' AND CLASS_ALIAS='대출자주민등록번호보여주기'")
		_T("AND VALUE_NAME='대출자주민번호' AND MANAGE_CODE=UDF_MANAGE_CODE") );
	m_pPrintDM->GetOneValueQuery( sQuery, sCivilOption );
	if ( _T("N")==sCivilOption ) return 0;

	nIndex = sCivilNo.Find( _T("-") );
	if( nIndex < 0 ) return 0;

	CString sShow = sCivilNo.Mid(0,nIndex);

	sCivilNo.Format( _T("%s*******") , sShow );

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3395::OnDblclklstDST(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	ChangeOrderASCDSC();	
	
	*pResult = 0;
}

INT CBO_LOC_3395::ChangeOrderASCDSC()
{
	EFS_BEGIN
		
	CListCtrl* pList = (CListCtrl*)(GetDlgItem(IDC_lstDST));
	
	POSITION pos;
	pos = pList->GetFirstSelectedItemPosition();
	
	INT nIndex = pList->GetNextSelectedItem(pos);
	
	if( nIndex < 0 ) return 0;
	
	CString sOrderAscDsc;
	
	sOrderAscDsc = pList->GetItemText( nIndex , 1 );
	
	if( sOrderAscDsc.Compare(_T("오름")) == 0 )
		sOrderAscDsc = _T("내림");
	else
		sOrderAscDsc = _T("오름");
	
	pList->SetItemText( nIndex , 1 , sOrderAscDsc );
	
	return 0;
	
	EFS_END
		
	return -1;
}

INT CBO_LOC_3395::UserDetailInfoPrint( CString sPrintAlias , BOOL* pIsPrint)
{
	EFS_BEGIN

	INT ids;

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return NULL;
	}
	pSIReportManager->SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	
	ids = pSIReportManager->GetSIReportFileFromDB(sPrintAlias);
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return NULL;
	}
	
	ids = pSIReportManager->DeleteTempPageFiles();

	pSIReportManager->SetDataMgr(0, m_pPrintDM, 0);

	for( INT i = 0 ; i < m_pPrintDM->GetRowCount() ; i++ )
	{
		ids = SetUserDetailInfoCondition( pSIReportManager , m_pPrintDM , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("UserDetailInfoPrint") );
		
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintUserList") );
	}
	
	ids = pSIReportManager->Print2(pIsPrint);
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_3395::SetUserDetailInfoCondition(  CSIReportManager* pSIReportManager , CESL_DataMgr* pPrintDM , INT nIndex )
{
	EFS_BEGIN
	
	const INT nCnt = 20;

	CString sInputTmpAlias[nCnt] =
	{
		_T("이용자번호") , _T("가족ID") , _T("이름") , _T("주민등록번호") , _T("생일") , 
		_T("휴대폰") , _T("E_MAIL") , _T("소속") , _T("직급") , _T("자료실") , 
		_T("자택_우편번호") , _T("자택_주소") , _T("자택_전화") , _T("근무지_우편번호") , _T("근무지_전화") , 
		_T("근무지_주소") , _T("학교명") , _T("학년") , _T("반") , _T("비고")
	};

	const INT nGetFieldCnt = 20;
	CString sValue[nGetFieldCnt];

	CString sGetDMFieldAlias[nGetFieldCnt] =
	{
		_T("대출자번호") , _T("가족ID") , _T("대출자이름") , _T("주민등록번호") , _T("생일") , 
		_T("핸드폰") , _T("E_MAIL") , _T("대출소속정보") , _T("대출직급정보") , _T("자료실") ,
		_T("자택우편번호") , _T("자택주소") , _T("자택전화번호") , _T("근무지우편번호") , _T("근무지전화번호") ,
		_T("근무지주소") , _T("학교") , _T("학년") , _T("반") , _T("비고")
	};

	m_pPrintDM->GetCellData( sGetDMFieldAlias , nGetFieldCnt , sValue , nIndex );

	if( TRUE != sValue[3].IsEmpty() )
	{
		CLocCommonAPI::ChangeSecuriyCivilNo( this , sValue[3] , m_sIsUnityLoanService );
	}
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		sValue[i].TrimLeft();
		sValue[i] = _T(" ") + sValue[i];
		pSIReportManager->SetTempVariable( sInputTmpAlias[i] , sValue[i] );
	}
	

	return 0;


	EFS_END

	return -1;
}

INT CBO_LOC_3395::ShowHideLibInfo()
{
	EFS_BEGIN

	if( !m_pLoanCardFormatSetter || !m_pLoanCardFormatSetter->GetSafeHwnd() )
		return 0;

	INT nMode = GetPrintMode();
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cLOAN_CARD_TYPE );	
	if( nMode == 3 )
	{		
		pCombo->ShowWindow( true );
		
		ViewLoanCardInfo();
	}
	else
	{
		pCombo->ShowWindow( false );
		
		GetDlgItem(IDC_chkSELECTGRID)->ShowWindow(SW_HIDE);
		m_pLoanCardFormatSetter->ShowWindow( SW_HIDE );
	}

	return 0;

	EFS_END

	return -1;

}


VOID CBO_LOC_3395::OnSelchangetabPRINT(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowHideLibInfo();	
	*pResult = 0;
}

VOID CBO_LOC_3395::OnMove(INT x, INT y) 
{
	CDialog::OnMove(x, y);
	
	if( GetSafeHwnd() == NULL )return;
	ShowHideLibInfo();
}

VOID CBO_LOC_3395::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	ShowHideLibInfo();
}

INT CBO_LOC_3395::MakePrintDMFromSaveUserDM()
{
	EFS_BEGIN

	INT ids;
	
	m_pPrintDM->FreeData();

	CESL_DataMgr* pSaveDM = FindDM( _T("DM_BO_LOC_3300_SAVE_USER") );
	if( pSaveDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDMFromSaveUserDM") );

	CESL_DataMgr TmpDM;
	CLocCommonAPI::MakeCopyDM( this , m_pPrintDM , &TmpDM );

	INT nMaxCnt = 50;
	CString sUserKey;
	CString sUserKeyList;
	CString sWhere;
	for( INT i = 0 ; i < pSaveDM->GetRowCount() ; i++ )
	{
		ids = pSaveDM->GetCellData( _T("이용자KEY") , i , sUserKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakePrintDMFromSaveUserDM") );

		if( !sUserKeyList.IsEmpty() ) sUserKeyList += _T(",");
		sUserKeyList += sUserKey;

		if( i == pSaveDM->GetRowCount()-1 || ((i+1)%nMaxCnt == 0 ) )
		{
			sWhere.Format( _T(" CLU.REC_KEY IN (%s) ") , sUserKeyList );
			ids = CLocCommonAPI::RefreshDataManager( this , &TmpDM , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakePrintDMFromSaveUserDM") );

			CLocCommonAPI::AddDM( &TmpDM , m_pPrintDM , this);
			TmpDM.FreeData();
			sUserKeyList.Empty();
		}
	}

	CString sData,sData2;
	
	CListCtrl *pDstList;
	pDstList = ((CListCtrl*)GetDlgItem(IDC_lstDST));

	INT nCondColumns = pDstList->GetItemCount();
	if( nCondColumns == 0 ) return 0;

	CString* strAlias = new CString[nCondColumns];
	INT* nCond = new INT[nCondColumns];
	
	for( i = 0 ; i < nCondColumns ; i++ )
	{
		sData = pDstList->GetItemText( i , 0 );
		sData2 = pDstList->GetItemText( i , 1 );
		
		strAlias[i] = sData;
		
		if( sData2.Compare(_T("오름")) == 0 )
			nCond[i] = ESL_DATAMGR_SORT_ASC;
		else
			nCond[i] = ESL_DATAMGR_SORT_DESC;
	}

	m_pPrintDM->SORT( nCondColumns , strAlias , nCond , ESL_DATAMGR_SORT_ASC );
	
	delete []strAlias;
	delete []nCond;

	return 0;

	EFS_END

	return -1;
}

VOID CBO_LOC_3395::OnClose() 
{
	m_pLibInfoDlg->ShowWindow(SW_HIDE);
	m_pLoanCardFormatSetter->ShowWindow( SW_HIDE );
	
	CDialog::OnClose();
}

VOID CBO_LOC_3395::MakeLoanCardTypeCombo()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cLOAN_CARD_TYPE );
	pCombo->InsertString( 0, _T("이전 형식") );
	pCombo->InsertString( 1, _T("새로운 형식") );
	pCombo->SetCurSel( 0 );	
}

INT CBO_LOC_3395::GetLoanCardType()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cLOAN_CARD_TYPE );
	return pCombo->GetCurSel();
}

VOID CBO_LOC_3395::OnSelchangecLOANCARDTYPE() 
{
	// TODO: Add your control notification handler code here
	
	ViewLoanCardInfo();
}

VOID CBO_LOC_3395::ViewLoanCardInfo()
{
	GetDlgItem( IDC_chkSELECTGRID )->ShowWindow( SW_SHOW );
	CRect WndPos, WndPos2;
	GetWindowRect( WndPos );
		
	if( GetLoanCardType() )
	{
		m_pLoanCardFormatSetter->GetWindowRect( WndPos2 );
		m_pLoanCardFormatSetter->MoveWindow( WndPos.right , WndPos.top , WndPos2.Width() , WndPos.Height() );
		m_pLoanCardFormatSetter->ShowWindow( SW_SHOW );
		m_pLibInfoDlg->ShowWindow( false );

		m_defaultLoanCardType = _T("NEW");
	}
	else
	{
		m_pLibInfoDlg->GetWindowRect( WndPos2 );
		m_pLibInfoDlg->MoveWindow( WndPos.right , WndPos.top , WndPos2.Width() , WndPos2.Height() );
		m_pLibInfoDlg->ShowWindow( SW_SHOW );
		m_pLoanCardFormatSetter->ShowWindow( false );

		m_defaultLoanCardType = _T("OLD");
	}
}

CString CBO_LOC_3395::GetDefaultLoanCardType()
{
	CString type;
	switch( GetLoanCardType() )
	{
		case 0 :
			type = _T("OLD");
			break;
		case 1 :
			type = _T("NEW");
			break;
		default :
			break;
	}

	return type;
}

INT CBO_LOC_3395::SaveDefaultLoanCardInfo()
{
	FILE *fp = _tfopen( _T("..\\cfg\\DefaultLoanCardInfo.cfg"), _T("w+b") );
	if( NULL == fp )
		return -1;

		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
		
	_ftprintf( fp, _T("DEFAULT_PATTERN:%s\r\n"), m_pLoanCardFormatSetter->m_defaultPtrn.GetBuffer( 0 ) );
	_ftprintf( fp, _T("LOAN_CARD_TYPE:%s\r\n"), m_defaultLoanCardType.GetBuffer( 0 ) );

	fclose( fp );
	
	return 0;
}

INT CBO_LOC_3395::InitNDScreen()
{
	CWnd* pWnd;
	CString sErrMsg;

	pWnd = GetDlgItem(IDC_edtPRINT_USER_NO1);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3395::InitNDScreen -1 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pWnd->ShowWindow(SW_HIDE);
	
	pWnd = GetDlgItem(IDC_edtPRINT_USER_NO2);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3395::InitNDScreen -2 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pWnd->ShowWindow(SW_HIDE);

	pWnd = GetDlgItem(IDC_edtCLASS_NO1);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3395::InitNDScreen -3 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pWnd->ShowWindow(SW_SHOW);

	pWnd = GetDlgItem(IDC_edtCLASS_NO2);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3395::InitNDScreen -4 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pWnd->ShowWindow(SW_SHOW);

	pWnd = GetDlgItem(IDC_stCHANGE_ALIAS1);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3395::InitNDScreen -5 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	pWnd->SetWindowText( _T("교      번") );

    return 0;
}

HBRUSH CBO_LOC_3395::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
