// BO_LOC_3270.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3270.h"
#include "BO_LOC_3000Api.h"
#include "PersonInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3270 dialog


CBO_LOC_3270::CBO_LOC_3270(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3270)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	GetManageValue( _T("기타") , _T("공통") , _T("국방대") , _T("") , m_sDefenceFlag );	
	m_sDefenceFlag.TrimLeft();m_sDefenceFlag.TrimRight();
	GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , m_s64Flag );	
	m_s64Flag.TrimLeft();m_s64Flag.TrimRight();
}

CBO_LOC_3270::~CBO_LOC_3270()
{
}

VOID CBO_LOC_3270::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3270)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3270, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3270)
	ON_BN_CLICKED(IDC_btnUSER_PRINT, OnbtnUSERPRINT)
	ON_BN_CLICKED(IDC_btnUSER_CLOSE, OnbtnUSERCLOSE)
	ON_BN_CLICKED(IDC_btnORDER_ADD, OnbtnORDERADD)
	ON_BN_CLICKED(IDC_btnORDER_DELTE, OnbtnORDERDELTE)
	ON_NOTIFY(UDN_DELTAPOS, IDC_spnUPDOWN, OnDeltaposspnUPDOWN)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabPRINT, OnSelchangetabPRINT)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnUSER_CLOSE2, OnbtnUSERCLOSE2)
	ON_NOTIFY(NM_DBLCLK, IDC_lstDST, OnDblclklstDST)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabINFO, OnSelchangetabINFO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3270 message handlers

BOOL CBO_LOC_3270::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("BO_LOC_3270")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	InitControl();

	m_pPrintDM = FindDM(_T("DM_BO_LOC_3270"));
	if( m_pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pPrintMainInfoDM = FindDM( _T("DM_BO_LOC_3270_PRINT_MAIN_INFO") );
	if( m_pPrintMainInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	m_pOrderFieldMapDM = FindDM( _T("DM_BO_LOC_3270_ORDER_MAP_FIELD") );
	if( m_pOrderFieldMapDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	MakeMainInfo();

	INT ids = MakeOrderFieldMap();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("OnInitDialog") );

	InitScreen();

	InitLibInfo();

	SetUserNoViewMode();
	
	if ( m_sDefenceFlag == _T("Y") )
		InitNDScreen();		
	if ( m_s64Flag == _T("Y") )
	{
		GetDlgItem(IDC_stLoanType)->ShowWindow(SW_SHOW);		
		GetDlgItem(IDC_cmbLoanType)->ShowWindow(SW_SHOW);	
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_3270"), _T("등록번호1"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_LOC_3270"), _T("등록번호구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_3270"), _T("등록번호2"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_LOC_3270"), _T("등록번호구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}


BOOL CBO_LOC_3270::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_3270::OnbtnUSERPRINT() 
{
	EFS_BEGIN

	PrintUserList();

	EFS_END
}

VOID CBO_LOC_3270::OnbtnUSERCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}


INT CBO_LOC_3270::PrintUserList()
{
	EFS_BEGIN

	INT ids;

	INT nMode = GetPrintMode();

	CString sWhere;

	ids = DefaultWhere(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintUserList") );
	if( ids > 0 ) return 0;

	ids = MakeSql(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintUserList") );
	if( ids > 0 ) return 0;

	if( nMode == 7 )
		ids = MakeDMFromDB2( sWhere );
	else
		ids = MakeDMFromDB( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("PrintUserList") );

	CESL_DataMgr *pPrintDM;
	if( nMode == 7 )
		pPrintDM = FindPrintDM(7);
	else
		pPrintDM = m_pPrintDM;

	if( pPrintDM->GetRowCount() < 1 ) 
	{
		AfxMessageBox( _T("목록으로 출력할 자료가 존재하지 않습니다."));
		return 1;
	}

	if( PrivacyPrintLogRecord(pPrintDM, _T("대출자료관리 출력"), _T("E") ,0 ) < 0 ) return 1; 

	ids = CodeToDesc();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintUserList") );

	if( nMode == 2 || (nMode == 3) || (nMode == 7) )
	{
		ids = SortFromOrder();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintUserList") );

		ids = PrintGeneral(nMode);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintUserList") );
	}

	else if( nMode == 6 )
	{
		ids = ReStructDM(nMode);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintUserList") );	
	}
	else
	{
		ids = DivPrint(nMode);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintUserList") );	
	}



	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::MakeSql( CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	CString sWhereFromControl;

	const INT nCnt = 28;

	CString sGetData[nCnt];
	CString sCMAlias = _T("CM_BO_LOC_3270");

	TCHAR *sDBFieldName[nCnt] =
	{
		_T("CLU.USER_NO")                 , _T("CLU.USER_NO")                 , _T("CLU.NAME")              , _T("CLU.NAME") , 
		_T("CLU.USER_POSITION_CODE") , _T("CLU.USER_POSITION_CODE") , _T("CL.LOAN_DATE")          , _T("CL.LOAN_DATE") ,
		_T("CL.RETURN_PLAN_DATE")         , _T("CL.RETURN_PLAN_DATE")         , _T("B.MANAGE_CODE")        , _T("B.MANAGE_CODE") ,
		_T("B.SHELF_LOC_CODE")            , _T("B.SHELF_LOC_CODE")            , _T("B.REG_CODE")            , _T("B.REG_NO") , 
		_T("B.REG_NO")                    , _T("B.SEPARATE_SHELF_CODE")       , _T("B.SEPARATE_SHELF_CODE") , _T("B.CLASS_NO") , 
		_T("B.CLASS_NO")                  , _T("CLMS.MAIL_SEND_DATE")         , _T("CLMS.MAIL_SEND_DATE")   , _T("CL.RETURN_DATE") ,
		_T("CL.RETURN_DATE")              , _T("CLU.USER_CLASS_CODE")    , _T("CLU.CLASS_NO")          , _T("CLU.CLASS_NO")

	};

	INT nDBFieldType[nCnt] = 
	{
		TWOSTRING , ETC , TWOSTRING , ETC , 
		TWOSTRING , ETC , TWODATE , ETC ,
		TWODATE , ETC , TWOSTRING , ETC , 
		TWOSTRING , ETC , THREESTRING , ETC , 
		ETC , ONESTRING , ETC , TWOSTRING , 
		ETC , TWODATE , ETC , TWODATE , 
		ETC , ONESTRING , TWOSTRING , ETC
	};

	TCHAR *sControlName[nCnt] =
	{
		_T("대출자번호1")   , _T("대출자번호2")   , _T("대출자명1")    , _T("대출자명2") , 
		_T("대출소속정보1") , _T("대출소속정보2") , _T("대출일1")      , _T("대출일2") , 
		_T("반납예정일1")   , _T("반납예정일2")   , _T("관리구분1")    , _T("관리구분2") , 
		_T("자료실명1")     , _T("자료실명2")     , _T("등록번호구분") , _T("등록번호1") , 
		_T("등록번호2")     , _T("별치기호1")     , _T("별치기호2")    , _T("분류기호1") , 
		_T("분류기호2")     , _T("메일발송일1")   , _T("메일발송일2")  , _T("반납일1") ,
		_T("반납일2")       , _T("대출직급정보")  , _T("교번1")        , _T("교번2")
	};

	CESL_Control_Element *pCE;
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSql") );

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( i == 5 ) continue;

		ids = GetControlData( sCMAlias , sControlName[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) 
		{
			CString sEMsg;
			sEMsg.Format( _T(" GetControlData( %s , %s , %s , -1 , 0 ) ") , sCMAlias , sControlName[i] , sGetData[i] );
			AfxMessageBox( sEMsg );
		}	
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSql") );
		
		pCE = (CESL_Control_Element*)pCM->FindControlElement(sControlName[i]);
		if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSql") );

		if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
			sGetData[i] = _T("");
		
	}

	ids = CLocCommonAPI::MakeSql( sGetData , sDBFieldName , nDBFieldType , nCnt , sWhereFromControl , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSql") );

	CString sZipCode , sAddress;
	ids = pCM->GetControlMgrData( _T("우편번호") , sZipCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -201 , _T("GetSqlBO_LOC_3210") );
	sZipCode.TrimLeft(); sZipCode.TrimRight();
	
	ids = pCM->GetControlMgrData( _T("주소") , sAddress , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -201 , _T("GetSqlBO_LOC_3210") );
	sAddress.TrimLeft(); sAddress.TrimRight();

	pCE = (CESL_Control_Element*)pCM->FindControlElement(_T("우편번호"));
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSql") );

	if( !sZipCode.IsEmpty() && GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
	{
		if( sWhereFromControl.GetLength() > 1 )
			sWhereFromControl += _T(" AND ");
		
		sWhereFromControl += _T("CLU.H_ZIPCODE LIKE ('")+sZipCode+_T("%')");
	}
	
	pCE = (CESL_Control_Element*)pCM->FindControlElement(_T("주소"));
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSql") );
	
	if( !sAddress.IsEmpty() && GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
	{
		if( sWhereFromControl.GetLength() > 1 )
			sWhereFromControl += _T(" AND ");
		
		sWhereFromControl += _T("CLU.H_ADDR1 LIKE ('%")+sAddress+_T("%')");
	}
	
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbMAIL_CLASS);
	if( pCombo->IsWindowEnabled() )
	{
		CString sMailClass;
		INT nMailClass = pCombo->GetCurSel();
		sMailClass.Format( _T("%d") , nMailClass );
		if( !sWhereFromControl.IsEmpty() )
			sWhereFromControl += _T(" AND ");
		sWhereFromControl += _T(" CLMS.MAIL_CLASS = '") + sMailClass + _T("' ");
	}

	if ( m_sDefenceFlag == _T("Y") )
	{
		CButton* pBT = (CButton*)GetDlgItem(IDC_chkReservation);
		INT nIsReservation = pBT->GetCheck();
		if( pBT->IsWindowEnabled() && nIsReservation == 1 )
		{
			CString sChkReservation;
			sChkReservation.Format( _T("( SELECT COUNT(*) FROM LS_WORK_T01 CL2 ")
									  _T("WHERE CL2.STATUS='3'")
									  _T("AND CL2.BOOK_KEY=CL.BOOK_KEY ) > 0   ") );
			if( !sWhereFromControl.IsEmpty() )
				sWhereFromControl += _T(" AND ");
			sWhereFromControl += sChkReservation;
		}
	}

	if ( m_s64Flag == _T("Y") )
	{
		CComboBox *pcmbLoanType = (CComboBox*)GetDlgItem(IDC_cmbLoanType);

		if ( pcmbLoanType->GetCurSel() )
		{
			INT nLoanType = pcmbLoanType->GetCurSel() - 1;
			if ( nLoanType == 3 ) nLoanType = 4;

			CString sLoanTypeQuery;
			sLoanTypeQuery.Format( _T(" CL.LOAN_TYPE_CODE = '%d' ") , nLoanType );
			
			if( !sWhereFromControl.IsEmpty() )
				sWhereFromControl += _T(" AND ");
			sWhereFromControl += sLoanTypeQuery;
		}
	}

	if( !sWhereFromControl.IsEmpty() )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		sWhere += sWhereFromControl;
	}
	else
	{
		AfxMessageBox( _T("조건을 입력해주십시요") );
		return 1;
	}

	if( sWhere.IsEmpty() )
	{
		AfxMessageBox( _T("조건을 입력해주십시요") );
		return 1;
	}

	return 0;

	EFS_END
	return -1;


}


INT CBO_LOC_3270::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;

	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3270::InitControl()
{
	EFS_BEGIN

	const INT nMenuCnt = 8;
	TCHAR *sMenuName[nMenuCnt] = 
	{
		_T("개인별대출현황목록") , _T("개인별대출이력목록") , _T("대출자료목록") , _T("반납대상자료목록") , 
		_T("반납독촉장") , _T("반납독촉장카드형식") , _T("대출자료반납요청서주소레이블") , _T("E-MAIL발송현황목록")
	};

	m_sPrintModeAlias = new CString[nMenuCnt];

	for( INT i = 0 ; i < nMenuCnt ; i++ )
	{
		((CTabCtrl*)GetDlgItem(IDC_tabPRINT))->InsertItem(i,sMenuName[i]);
		m_sPrintModeAlias[i] = sMenuName[i];
	}

	CString sInfoTabTitle[2] =
	{
		_T("정렬조건") , _T("반납요청서정보")
	};
	((CTabCtrl*)GetDlgItem(IDC_tabINFO))->InsertItem(i,sInfoTabTitle[0]);
	((CTabCtrl*)GetDlgItem(IDC_tabINFO))->InsertItem(i,sInfoTabTitle[1]);

	LVCOLUMNW lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = 130;
	lvcolumn.pszText = _T("정렬조건");
	
	CListCtrl *pList;
	pList = ((CListCtrl*)GetDlgItem(IDC_lstSRC));
	pList->SetExtendedStyle(pList->GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	pList->InsertColumn(0,&lvcolumn);

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


VOID CBO_LOC_3270::OnbtnORDERADD() 
{
	EFS_BEGIN

	CListCtrl *pSrcList,*pDstList;
	pSrcList = ((CListCtrl*)GetDlgItem(IDC_lstSRC));
	pDstList = ((CListCtrl*)GetDlgItem(IDC_lstDST));

	MoveOrderList(pSrcList,pDstList);


	EFS_END
}


INT CBO_LOC_3270::MoveOrderList(CListCtrl *pSrcList,CListCtrl *pDstList)
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

VOID CBO_LOC_3270::OnbtnORDERDELTE() 
{
	EFS_BEGIN

	CListCtrl *pSrcList,*pDstList;
	pSrcList = ((CListCtrl*)GetDlgItem(IDC_lstSRC));
	pDstList = ((CListCtrl*)GetDlgItem(IDC_lstDST));

	MoveOrderList(pDstList,pSrcList);
	

	EFS_END
}

INT CBO_LOC_3270::MakeOrderBy( CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	CString sOrderBy;
	CString sData;

	CListCtrl *pDstList;
	pDstList = ((CListCtrl*)GetDlgItem(IDC_lstDST));

	for( INT i = 0 ; i < pDstList->GetItemCount() ; i++ )
	{
		sData = pDstList->GetItemText( i , 0 );

		ids = GetCellData( m_pOrderFieldMapDM , _T("조건종류") , sData , _T("필드명") , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderBy") );
		
		if( !sOrderBy.IsEmpty() )
			sOrderBy += _T(" , ");
		sOrderBy += sData;		
	}

	if( !sOrderBy.IsEmpty() )
		sWhere += _T(" ORDER BY ") + sOrderBy;

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::GetCellData( CESL_DataMgr *pDM , CString sConditionAlias , CString sConditionData , CString sGetAlias , CString &sGetData )
{
	EFS_BEGIN

	INT ids;
	ids = CLocCommonAPI::GetCellData( this , pDM , sConditionAlias , sConditionData  , sGetAlias , sGetData );
	return ids;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::MakeOrderFieldMap()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pOrderFieldMapDM;

	CString sFilePath = _T("..\\cfg\\출력물_대출자료관리\\정렬조건생성맵.txt");

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

INT CBO_LOC_3270::GetPrintMode()
{
	EFS_BEGIN

	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabPRINT);
	
	return pTab->GetCurSel();

	EFS_END
	return -1;

}

INT CBO_LOC_3270::MoveListRecord( INT nMode , CListCtrl *pList )
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

VOID CBO_LOC_3270::OnDeltaposspnUPDOWN(NMHDR* pNMHDR, LRESULT* pResult) 
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





VOID CBO_LOC_3270::OnSelchangetabPRINT(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	InitScreen();
	
	*pResult = 0;

	EFS_END
}

INT CBO_LOC_3270::InitScreen()
{
	EFS_BEGIN

	INT ids;

	INT nMode = GetPrintMode();

	ids = InitSearchCond(nMode);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	ids = InitOrderCond(nMode);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );

	ids = InitEtcInputCond(nMode);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::InitSearchCond( INT nMode )
{
	EFS_BEGIN

	INT ids;

	CStringArray saSearchField;
	CString sSearchField;

	ids = GetCellData( m_pPrintMainInfoDM , _T("출력물종류") , m_sPrintModeAlias[nMode] , _T("검색조건") , sSearchField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitSearchCond") );

	ids = CLocCommonAPI::MakeArrayList( sSearchField , _T(",") ,saSearchField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitSearchCond") );

	CString sCMAlias = _T("CM_BO_LOC_3270");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitSearchCond") );

	INT nControlCnt = pCM->Control_List.GetCount();

	CString sControlAlias;
	CESL_Control_Element *pCE;
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetAt((pCM->Control_List.FindIndex(i)));
		sControlAlias = pCE->CTRL_ALIAS;

		if ( sControlAlias == _T("예약자료검색") )
		{
			CButton* pBT = (CButton*)GetDlgItem( pCE->CTRL_ID );
			if ( pBT == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitSearchCond") );
			pBT->SetCheck( 0 );
		}

		GetDlgItem( pCE->CTRL_ID )->EnableWindow(FALSE);

		for( INT j = 0 ; j < saSearchField.GetSize() ; j++ )
		{
			if( sControlAlias.Find(saSearchField.GetAt(j)) > -1 )
				GetDlgItem( pCE->CTRL_ID )->EnableWindow(TRUE);
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::InitOrderCond( INT nMode )
{
	EFS_BEGIN

	INT ids;
	CStringArray saOrderField;
	CString sOrderField;

	ids = GetCellData( m_pPrintMainInfoDM , _T("출력물종류") , m_sPrintModeAlias[nMode] , _T("정렬조건") , sOrderField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitSearchCond") );

	ids = CLocCommonAPI::MakeArrayList( sOrderField , _T(",") ,saOrderField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitSearchCond") );

	const INT nCnt = 6;

	INT nIDC[nCnt] =
	{
		IDC_staORDER_TITLE , IDC_lstSRC , IDC_btnORDER_ADD , 
		IDC_btnORDER_DELTE , IDC_lstDST , IDC_spnUPDOWN
	};

	InitOrderList(nMode,saOrderField);

	ShowHideOrdernputCond( nIDC , nCnt );


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::InitOrderList( INT nMode , CStringArray &saOrderField )
{
	EFS_BEGIN

	CListCtrl *pSrcList = (CListCtrl*)(GetDlgItem(IDC_lstSRC));
	CListCtrl *pDstList = (CListCtrl*)(GetDlgItem(IDC_lstDST));

	pDstList->DeleteAllItems();
	pSrcList->DeleteAllItems();
	
	for( INT i = 0 ; i < saOrderField.GetSize() ; i++ )
	{		
		pSrcList->InsertItem( i , saOrderField.GetAt(i) );
	}

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::InitEtcInputCond( INT nMode )
{
	EFS_BEGIN

	

	INT ids;
	CString sEtcInputYN;

	ids = GetCellData( m_pPrintMainInfoDM , _T("출력물종류") , m_sPrintModeAlias[nMode] , _T("기타입력조건가능여부") , sEtcInputYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitSearchCond") );


	CRect WndRect;
	GetWindowRect(&WndRect);

	const INT nCnt = 14;

	INT nIDC[nCnt] =
	{
		IDC_staORDER_TITLE2 , IDC_staMANAGER , IDC_edtPRINT_MANAGER , IDC_edtPRINT_PHONE , 
		IDC_staLIB , IDC_edtPRINT_LIB_NAME , IDC_staRETURNEXPIRE , IDC_edtPRINT_RETURN_EXPIRE , 
		IDC_chkIS_IN_USER_NO , IDC_staPHONE ,  IDC_staMANAGER2 , IDC_staMANAGER3 , 
		IDC_edtPRINT_MANAGER2 , IDC_edtPRINT_MANAGER3
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sEtcInputYN.Compare(_T("Y")) == 0 )
		{
			GetDlgItem( nIDC[i] )->EnableWindow(TRUE);
		}
		else
			GetDlgItem( nIDC[i] )->EnableWindow(FALSE);
	}

	ShowHideEtcInputCond( nIDC , nCnt );
	
	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3270::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	EFS_END
}

INT CBO_LOC_3270::MakeMainInfo()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pPrintMainInfoDM;

	CString sFilePath = _T("..\\cfg\\출력물_대출자료관리\\기본정보.txt");

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

INT CBO_LOC_3270::MakeDMFromDB( CString sWhere )
{
	EFS_BEGIN

	INT ids;

	const INT nDMCnt = 3;
	CString sDMAlias[nDMCnt] =
	{
		_T("DM_BO_LOC_3270") , _T("DM_BO_LOC_3270_NON_DB_BOOK") , _T("DM_BO_LOC_3270_SE_BOOK")
	};

	CESL_DataMgr *pDM[nDMCnt];
	for( INT i = 0 ; i < nDMCnt ; i++ )
	{
		pDM[i] = FindDM( sDMAlias[i] );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeDMFromDB") );

		ids = CLocCommonAPI::RefreshDataManager( this , pDM[i] , sWhere , (CProgressCtrl*)GetDlgItem(IDC_prgPRINT) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeDMFromDB") );

		if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM[i]->GetRowCount())
		{
			CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7;
			for(INT j = 0; j < pDM[i]->GetRowCount(); j++)
			{
				CString strGetData;
				pDM[i]->GetCellData(_T("자택주소"), j, strGetData);
				asDes1.Add(strGetData);
				pDM[i]->GetCellData(_T("근무지주소"), j, strGetData);
				asDes2.Add(strGetData);
				pDM[i]->GetCellData(_T("주민등록번호"), j, strGetData);
				asDes3.Add(strGetData);
				pDM[i]->GetCellData(_T("E_MAIL"), j, strGetData);
				asDes4.Add(strGetData);
				pDM[i]->GetCellData(_T("핸드폰"), j, strGetData);
				asDes5.Add(strGetData);
				pDM[i]->GetCellData(_T("자택전화번호"), j, strGetData);
				asDes6.Add(strGetData);
				pDM[i]->GetCellData(_T("근무지전화번호"), j, strGetData);
				asDes7.Add(strGetData);
			}
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes1);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes2);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes3);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes4);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes6);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes7);
						pApi->CloseSocket();	
					}
					for(j = 0; j < pDM[i]->GetRowCount(); j++)
					{
						pDM[i]->SetCellData(_T("자택주소"), asDes1.GetAt(j), j);
						pDM[i]->SetCellData(_T("근무지주소"), asDes2.GetAt(j), j);
						pDM[i]->SetCellData(_T("주민등록번호"), asDes3.GetAt(j), j);
						pDM[i]->SetCellData(_T("E_MAIL"), asDes4.GetAt(j), j);
						pDM[i]->SetCellData(_T("핸드폰"), asDes5.GetAt(j), j);
						pDM[i]->SetCellData(_T("자택전화번호"), asDes6.GetAt(j), j);
						pDM[i]->SetCellData(_T("근무지전화번호"), asDes7.GetAt(j), j);
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
			asDes7.RemoveAll(); asDes7.FreeExtra();
		}
	}

	( (CProgressCtrl*)GetDlgItem(IDC_prgPRINT) )->SetPos(100);

	ids = CLocCommonAPI::AddDM( pDM[1] , pDM[0] , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeDMFromDB") );

	ids = CLocCommonAPI::AddDM( pDM[2] , pDM[0] , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeDMFromDB") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::MakeDMFromDB2( CString sWhere )
{
	EFS_BEGIN

	INT ids;

	const INT nDMCnt = 3;
	CString sDMAlias[nDMCnt] =
	{
		_T("DM_BO_LOC_3270_MAIL_PRINT") , _T("DM_BO_LOC_3270_MAIL_NON_DB_BOOK") , _T("DM_BO_LOC_3270_MAIL_SE_BOOK")
	};

	CESL_DataMgr *pDM[nDMCnt];
	for( INT i = 0 ; i < nDMCnt ; i++ )
	{
		pDM[i] = FindDM( sDMAlias[i] );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeDMFromDB") );

		ids = CLocCommonAPI::RefreshDataManager( this , pDM[i] , sWhere , (CProgressCtrl*)GetDlgItem(IDC_prgPRINT) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeDMFromDB") );

		if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM[i]->GetRowCount())
		{
			CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7;
			for(INT j = 0; j < pDM[i]->GetRowCount(); j++)
			{
				CString strGetData;
				pDM[i]->GetCellData(_T("자택주소"), j, strGetData);
				asDes1.Add(strGetData);
				pDM[i]->GetCellData(_T("근무지주소"), j, strGetData);
				asDes2.Add(strGetData);
				pDM[i]->GetCellData(_T("주민등록번호"), j, strGetData);
				asDes3.Add(strGetData);
				pDM[i]->GetCellData(_T("E_MAIL"), j, strGetData);
				asDes4.Add(strGetData);
				pDM[i]->GetCellData(_T("핸드폰"), j, strGetData);
				asDes5.Add(strGetData);
				pDM[i]->GetCellData(_T("자택전화번호"), j, strGetData);
				asDes6.Add(strGetData);
				pDM[i]->GetCellData(_T("근무지전화번호"), j, strGetData);
				asDes7.Add(strGetData);
			}
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes1);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes2);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes3);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes4);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes6);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes7);
						pApi->CloseSocket();
					}
					for(j = 0; j < pDM[i]->GetRowCount(); j++)
					{
						pDM[i]->SetCellData(_T("자택주소"), asDes1.GetAt(j), j);
						pDM[i]->SetCellData(_T("근무지주소"), asDes2.GetAt(j), j);
						pDM[i]->SetCellData(_T("주민등록번호"), asDes3.GetAt(j), j);
						pDM[i]->SetCellData(_T("E_MAIL"), asDes4.GetAt(j), j);
						pDM[i]->SetCellData(_T("핸드폰"), asDes5.GetAt(j), j);
						pDM[i]->SetCellData(_T("자택전화번호"), asDes6.GetAt(j), j);
						pDM[i]->SetCellData(_T("근무지전화번호"), asDes7.GetAt(j), j);
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
			asDes7.RemoveAll(); asDes7.FreeExtra();
		}
	}

	( (CProgressCtrl*)GetDlgItem(IDC_prgPRINT) )->SetPos(100);

	ids = CLocCommonAPI::AddDM( pDM[1] , pDM[0] , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeDMFromDB") );

	ids = CLocCommonAPI::AddDM( pDM[2] , pDM[0] , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeDMFromDB") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::CodeToDesc()
{
EFS_BEGIN

	INT ids;
	
	CESL_DataMgr* pPrintDM;
	if( GetPrintMode() == 7 )
		pPrintDM = FindPrintDM(7);
	else
		pPrintDM = m_pPrintDM;
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pPrintDM , _T("대출소속정보") , _T("대출소속정보") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pPrintDM , _T("대출직급정보") , _T("대출직급정보") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pPrintDM , _T("자료실구분") , _T("자료실") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pPrintDM , _T("관리구분") , _T("관리구분") );

	ids = CLocCommonAPI::MakeShelfCode( this , pPrintDM->DB_MGR_ALIAS );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CodeToDesc") );

	CString sCode;
	CString sOrder;
	CString sCivilNo;
	CString sUserNo;
	CString strBuf;
	for( INT i = 0 ; i < pPrintDM->GetRowCount() ; i++ )
	{
		ids = pPrintDM->GetCellData( _T("불량회원구분") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CodeToDesc") );

		if( _ttoi(sCode) == 0 )
			sCode = _T("정상회원");
		else if( _ttoi(sCode) == 1 )
			sCode = _T("대출정지");
		else if( _ttoi(sCode) == 2 )
			sCode = _T("제적회원");
		
		ids = pPrintDM->SetCellData( _T("불량회원구분") , sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CodeToDesc") );

		ids = pPrintDM->GetCellData( _T("대출상태") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CodeToDesc") );

		if( _ttoi(sCode) == 0 )
			sCode = _T("대출");
		else if( _ttoi(sCode) == 1 )
			sCode = _T("반납");
		else if( _ttoi(sCode) == 2 )
			sCode = _T("반납연기");
		else if( _ttoi(sCode) == 3 )
			sCode = _T("예약");
		else if( _ttoi(sCode) == 4 )
			sCode = _T("예약취소");
		else if( _ttoi(sCode) == 5 )
			sCode = _T("무인반납");
		else if( _ttoi(sCode) == 6 )
			sCode = _T("분관반납");

		ids = pPrintDM->SetCellData( _T("대출상태") , sCode , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CodeToDesc") );

		sOrder.Format(_T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sOrder, i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CodeToDesc") );
		ids = pPrintDM->GetCellData( _T("대출자번호") , i , sUserNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11241 , _T("CodeToDesc") );
	
		ids = pPrintDM->GetCellData( _T("주민등록번호") , i , sCivilNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11242 , _T("CodeToDesc") );

		if(!m_bUserNoViewMode)
		{
			CLocCommonAPI::ChangeSecuriyUserNo( this , sUserNo , m_sIsUnityLoanService );
			ids = pPrintDM->SetCellData( _T("대출자번호") , sUserNo , i );
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -11243 , _T("CodeToDesc") );
		}

		ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , sCivilNo , m_sIsUnityLoanService);
		ids = pPrintDM->SetCellData( _T("주민등록번호") , sCivilNo , i );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -11243 , _T("CodeToDesc") );
	}

	return 0;

EFS_END
	return -1;
}

VOID CBO_LOC_3270::OnClose() 
{
	EFS_BEGIN

	if( m_sPrintModeAlias != NULL ) 
	{
		delete []m_sPrintModeAlias;
		m_sPrintModeAlias = NULL;
	}
	CDialog::OnClose();

	EFS_END
}


INT CBO_LOC_3270::SortFromOrder( CESL_DataMgr* pPrintDM/* = NULL */)
{
	EFS_BEGIN

	INT ids;

	CArray < CString, CString > saOrder;

	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_lstDST);

	if( pList->GetItemCount() == 0 ) return 0;

	CString sCond;
	INT* nCond = new INT[pList->GetItemCount()];

	for( INT i = 0 ; i < pList->GetItemCount() ; i++ )
	{
		saOrder.Add(pList->GetItemText( i , 0 ));
		sCond = pList->GetItemText( i , 1 );

		if( sCond.Compare(_T("오름")) == 0 )
			nCond[i] = ESL_DATAMGR_SORT_ASC;		
		else
			nCond[i] = ESL_DATAMGR_SORT_DESC;
		
	}

	if( pPrintDM == NULL )
	{
		if( GetPrintMode() == 7 )
			pPrintDM = FindPrintDM(7);
		else
			pPrintDM = m_pPrintDM;
	}

	ids = CLocCommonAPI::SortDM( this , pPrintDM , saOrder , nCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SortFromOrder") );


	delete []nCond;

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::SortFromOrder( CESL_DataMgr* pPrintDM , CString sGroupAlias )
{
	EFS_BEGIN
		
	INT ids;
	
	CArray < CString, CString > saOrder;
	
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_lstDST);
	
	if( pList->GetItemCount() == 0 ) return 0;
	
	CString sCond;
	INT* nCond = new INT[pList->GetItemCount()];
	
	for( INT i = 0 ; i < pList->GetItemCount() ; i++ )
	{
		saOrder.Add(pList->GetItemText( i , 0 ));
		sCond = pList->GetItemText( i , 1 );
		
		if( sCond.Compare(_T("오름")) == 0 )
			nCond[i] = ESL_DATAMGR_SORT_ASC;		
		else
			nCond[i] = ESL_DATAMGR_SORT_DESC;
		
	}

	CESL_DataMgr TmpDM;
	
	ids = CLocCommonAPI::MakeCopyDM( this , pPrintDM , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SortFromOrder") );

	CESL_DataMgr TmpDM2;
	
	ids = CLocCommonAPI::MakeCopyDM( this , pPrintDM , &TmpDM2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SortFromOrder") );

	bool bIsPrint;
	CString sRecKey;
	CString sNextRecKey;
	for( i = 0 ; i < pPrintDM->GetRowCount() ; i++ )
	{
		bIsPrint = FALSE;
					
		if( (i == pPrintDM->GetRowCount()-1) )
			bIsPrint = TRUE;
		else
		{
			ids = pPrintDM->GetCellData( sGroupAlias , i , sRecKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SortFromOrder") );
			
			ids = pPrintDM->GetCellData( sGroupAlias , i+1 , sNextRecKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SortFromOrder") );
			
			if( sRecKey.Compare(sNextRecKey) != 0 )
				bIsPrint = TRUE;
		}
		
		CLocCommonAPI::AddDM( pPrintDM , &TmpDM , i , -1 , this );
		
		if( bIsPrint ) continue;

		TmpDM.DeleteRow(TmpDM.GetRowCount()-1);
	}


	ids = CLocCommonAPI::SortDM( this , &TmpDM , saOrder , nCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SortFromOrder") );
	delete []nCond;
	
	CLocCommonAPI::AddDM( pPrintDM , &TmpDM2 , this );

	pPrintDM->FreeData();

	for( i = 0 ; i < TmpDM.GetRowCount() ; i++ )
	{
		ids = TmpDM.GetCellData( sGroupAlias , i , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SortFromOrder") );
		for( INT j = 0 ; j < TmpDM2.GetRowCount() ; j++ )
		{
			ids = TmpDM2.GetCellData( sGroupAlias , j , sNextRecKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SortFromOrder") );

			if( sRecKey.Compare(sNextRecKey) == 0 )
			{
				ids = CLocCommonAPI::AddDM( &TmpDM2 , pPrintDM , j , -1 , this );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SortFromOrder") );
			}		
		}
	}
		
	return 0;
	
	EFS_END
	
	return -1;
	
}


INT CBO_LOC_3270::DefaultWhere( CString &sWhere )
{
	EFS_BEGIN

	INT nMode = GetPrintMode();

	if( nMode == 0 )
		sWhere = _T(" CL.STATUS IN ( '0' , '2' , '3' , 'L', 'D' ) ");
	else if( nMode == 1 )		
		sWhere = _T(" CL.STATUS IN ( '0' , '1' , '2' , '3' , '4' , '5' , '6' , 'L', 'R', 'D' ) ");
	else if( nMode == 2 )
		sWhere = _T(" CL.STATUS IN ( '0' , '1' , '2' , 'L', 'R', 'D' ) ");
	else if( nMode == 3 )
		sWhere = _T(" CL.STATUS IN ( '0' , '2' , 'L', 'D' ) ");
	else if( nMode == 4 )
	{
		sWhere.Format(_T(" CL.STATUS IN ( '0' , '2' , 'L', 'D' ) AND CL.RETURN_PLAN_DATE < SYSDATE AND B.MANAGE_CODE = '%s' "), m_pInfo->MANAGE_CODE);
	}
	else if( nMode == 5 )
	{
		sWhere = _T(" CL.STATUS IN ( '0' , '2' , 'L', 'D') AND CL.RETURN_PLAN_DATE < SYSDATE ");
	}
	else if( nMode == 6 )
	{
		sWhere = _T(" CL.STATUS IN ( '0' , '2' , 'L', 'D' ) AND CL.RETURN_PLAN_DATE < SYSDATE ");
	}
	else if( nMode == 7 )
		sWhere = _T(" CL.STATUS IN ( '0' , '1' , '2' , '3' , '4' , '5' , '6' , 'L', 'R', 'D') ");


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::SetCondition(CSIReportManager *pSIReportManager)
{
	EFS_BEGIN

	INT ids;

	ids = SetTowCondValue(pSIReportManager);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetCondition") );

	ids = SetOneCondValue(pSIReportManager);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetCondition") );


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::SetTowCondValue(CSIReportManager *pSIReportManager)
{
	EFS_BEGIN

	
	INT ids;
	CString sValue;


	const INT nCondCnt = 3;
	CString sCMAlias = _T("CM_BO_LOC_3270");

	TCHAR *sCondAlias[nCondCnt] =
	{
		_T("자료실조건") , _T("대출일조건") , _T("소속조건")
	};

	CString sData[2];
	CString sControlAlias[nCondCnt][2] =
	{
		{ _T("자료실명1") , _T("자료실명2") } , 
		{ _T("대출일1") , _T("대출일2") } ,
		{ _T("대출소속정보1") , _T("") }
	};

	for( INT i = 0 ; i < nCondCnt; i++ )
	{
		for( INT j = 0 ; j < 2 ; j++ )
		{
			if( sControlAlias[i][j].IsEmpty() ) continue;

			ids = GetControlData( sCMAlias , sControlAlias[i][j] , sData[j] , -1 , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetCondition") );
		}

		if( sData[0].IsEmpty() || (sData[0].Compare(_T("적용안함"))==0) )
			sValue = _T("적용안함");
		else
		{
			if( sData[1].IsEmpty() || (sData[1].Compare(_T("적용안함"))==0) )
				sValue.Format( _T("%s") , sData[0] );
			else
				sValue.Format( _T("%s ~ %s ") , sData[0] , sData[1] );
		}

		pSIReportManager->SetTempVariable( sCondAlias[i] , sValue );
	}
	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3270::SetOneCondValue(CSIReportManager *pSIReportManager)
{
	EFS_BEGIN

	
	INT ids;

	const INT nCondCnt = 6;
	CString sCMAlias = _T("CM_BO_LOC_3270_ETC_INPUT");

	CString sControlAlias[nCondCnt] =
	{
		_T("담당자") , _T("문의전화") , _T("도서관명") , _T("반납한도기일") , _T("도서관주소") , _T("도서관우편번호")
	};

	CString sValue[nCondCnt];
	for( INT i = 0 ; i < nCondCnt; i++ )
	{
		ids = GetControlData( sCMAlias , sControlAlias[i] , sValue[i] , -1 , -1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetCondition") );

		pSIReportManager->SetTempVariable( sControlAlias[i] , sValue[i] );
	}

	DateFormatConvert( sValue[3] );

	pSIReportManager->SetTempVariable( sControlAlias[3] , sValue[3] );

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbMAIL_CLASS);
	CString sMailClass;
	pCombo->GetLBText( pCombo->GetCurSel() , sMailClass );
	pSIReportManager->SetTempVariable( _T("메일종류") , sMailClass );



	return 0;

	EFS_END
	return -1;

}

CESL_DataMgr* CBO_LOC_3270::FindPrintDM( INT nMode )
{
	EFS_BEGIN

	if( nMode == 6 ) 
		return FindDM( _T("DM_BO_LOC_3270_ADDRESS_LABLE") );
	if( nMode == 7 ) 
		return FindDM( _T("DM_BO_LOC_3270_MAIL_PRINT") );

	return m_pPrintDM;

	EFS_END
	return NULL;

}

INT CBO_LOC_3270::ReStructDM( INT nMode )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pPrintDM = FindPrintDM(nMode);
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ReStructDM") );

	pPrintDM->FreeData();

	int nRowCnt = 0;
	std::vector<CString> list;
	CString strKey;

	m_pPrintDM->SORT( _T("대출자KEY") );
	
	SortFromOrder( m_pPrintDM , _T("대출자KEY") );

	CSIReportManager *pSIReportManager = NULL;
	pSIReportManager = InitPrint( nMode );
	if( pSIReportManager == NULL ) return 0;
	
	INT nColIndex = 0;
	TCHAR *sColumnName[2] =
	{
		_T("왼쪽") , _T("오른쪽")
	};

	CString sData;
	CString sOrder;
	CString sRecKey;
	CString sNextRecKey;
	CString sAddress;
	CString sTestAddress;
	bool IsInsert;
	for( INT i = 0 ; i < m_pPrintDM->GetRowCount() ; i++ )
	{
		IsInsert = FALSE;
		
		if( (i == m_pPrintDM->GetRowCount()-1) )
				IsInsert = TRUE;
		else
		{
			ids = m_pPrintDM->GetCellData( _T("대출자KEY") , i , sRecKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReStructDM") );

			ids = m_pPrintDM->GetCellData( _T("대출자KEY") , i+1 , sNextRecKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReStructDM") );

			if( sRecKey.Compare(sNextRecKey) != 0 )
				IsInsert = TRUE;
		}

		if( !IsInsert ) continue;
		
		sAddress.Empty();
		ids = MakeAddress( m_pPrintDM , i , sAddress );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ReStructDM") );

		if( i == 0 )
			sTestAddress = sAddress;

		if( nColIndex == 0 )
			pPrintDM->InsertRow(-1);
		ids = pPrintDM->SetCellData( sColumnName[nColIndex] , sAddress , pPrintDM->GetRowCount()-1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReStructDM") );

		ids = pPrintDM->SetCellData( sColumnName[nColIndex] , sAddress , pPrintDM->GetRowCount()-1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReStructDM") );

		if( nColIndex == 0 )
			nColIndex = 1;
		else
			nColIndex = 0;

		ids = m_pPrintDM->GetCellData(_T("대출자KEY"), i, strKey );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3270::ReStructDM()") );
		list.push_back(strKey);
		nRowCnt++;

	}

	pSIReportManager->m_strNoMoreStatement = _T("");
	pSIReportManager->SetDataMgr(0, pPrintDM, 0);

	BOOL bPrint = FALSE;
	ids = pSIReportManager->Print(&bPrint);
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	WritePrintLog(nMode, bPrint, nRowCnt, &list);
	list.clear();
		
	return 0;


	EFS_END
	return -1;

}

VOID CBO_LOC_3270::WritePrintLog(INT nMode, BOOL bPrint, INT nRowCnt, std::vector<CString>* pList)
{
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_PRINT_LOG") );

	CString sPrintAlias = _T("");

	if(nMode == 0)
	{
		sPrintAlias = _T("개인별대출현황목록");
	}
	else if(nMode == 1)
	{
		sPrintAlias = _T("개인별대출이력목록");
	}
	else if(nMode == 2)
	{
		sPrintAlias = _T("대출자료목록");
	}
	else if(nMode == 3)
	{
		sPrintAlias = _T("반납대상자료목록");
	}
	else if(nMode == 4)
	{
		sPrintAlias = _T("반납독촉장");
	}
	else if(nMode == 5)
	{
		sPrintAlias = _T("반납독촉장카드형식");
	}
	else if(nMode == 6)
	{
		sPrintAlias = _T("대출자료반납요청서주소레이블");
	}
	else if(nMode == 7)
	{
		sPrintAlias = _T("E-MAIL발송현황목록");
	}
		
	if(bPrint)
	{
		if(nMode == 0 || nMode == 1 || nMode == 4 || nMode == 5|| nMode == 6|| nMode == 7)
			CPersonInfo::SetPrintLog(this, pDM, PERINFO_PRINT, m_pInfo, sPrintAlias, nRowCnt, pList);
		else
			CPersonInfo::SetPrintLog(this, pDM, PERINFO_EXCEL, m_pInfo, sPrintAlias, nRowCnt, pList);		
	}	
}

INT CBO_LOC_3270::PrintGeneral( INT nMode )
{
	EFS_BEGIN

	INT ids;


	CSIReportManager *pSIReportManager = new CSIReportManager(this);

	pSIReportManager = InitPrint(nMode);
	if( pSIReportManager == NULL ) return 0;

	ids = SetCondition( pSIReportManager );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintUserList") );

	CESL_DataMgr *pPrintDM = FindPrintDM( nMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintUserList") );

	CString sOrder;
	for( INT i = 0 ; i < pPrintDM->GetRowCount() ; i++ )
	{
		sOrder.Format( _T("%d") , i+1 );
		pPrintDM->SetCellData( _T("번호") , sOrder , i );
	}

	pSIReportManager->SetDataMgr(0, pPrintDM, 0);

	BOOL bPrint = FALSE;
	ids = pSIReportManager->Print(&bPrint);
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	if(nMode == 2 || nMode == 3 )
		bPrint = TRUE;

	std::vector<CString> list;
	CString strKey;
	for(i=0; i<pPrintDM->GetRowCount(); i++)
	{
		ids = pPrintDM->GetCellData(_T("대출자KEY"), i, strKey );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("PrintUserList") );
		list.push_back(strKey);
	}

	WritePrintLog(nMode, bPrint, pPrintDM->GetRowCount(), &list);

	list.clear();

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::DivPrint( INT nMode )
{
	EFS_BEGIN

	INT ids;

	CSIReportManager *pSIReportManager = NULL;
	pSIReportManager = InitPrint( nMode );
	if( pSIReportManager == NULL ) return 0;

	SortAllCondition( m_pPrintDM );
	
	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BO_LOC_3270_TEMP_DIV") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintUserList") );

	pSIReportManager->SetDataMgr(0, pPrintDM, 0);

	bool bIsPrint;
	int nRowCnt = 0;
	std::vector<CString> list;
	CString strKey;

	POSITION pos;
	CESL_DataMgr::reference *Ref;
	CString sData;
	CString sOrder;
	CString sRecKey;
	CString sNextRecKey;
	for( INT i = 0 ; i < m_pPrintDM->GetRowCount() ; i++ )
	{
		bIsPrint = FALSE;

		pPrintDM->InsertRow(-1);
		
		for( INT j = 0 ; j < m_pPrintDM->RefList.GetCount() ; j++ )
		{
			pos = m_pPrintDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)m_pPrintDM->RefList.GetAt(pos);

			ids = m_pPrintDM->GetCellData( Ref->FIELD_ALIAS , i , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DivPrint") );

			ids = pPrintDM->SetCellData( Ref->FIELD_ALIAS , sData , pPrintDM->GetRowCount()-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DivPrint") );
		}

		if( (i == m_pPrintDM->GetRowCount()-1) )
			bIsPrint = TRUE;
		else
		{
			ids = m_pPrintDM->GetCellData( _T("대출자KEY") , i , sRecKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DivPrint") );

			ids = m_pPrintDM->GetCellData( _T("대출자KEY") , i+1 , sNextRecKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DivPrint") );

			if( sRecKey.Compare(sNextRecKey) != 0 )
				bIsPrint = TRUE;
		}


		if( !bIsPrint ) continue;

		for( INT k = 0 ; k < pPrintDM->GetRowCount() ; k++ )
		{
			sOrder.Format( _T("%d") , k+1 );
			ids = pPrintDM->SetCellData( _T("번호") , sOrder , k );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DivPrint") );			
		}

		ids = SetCondition( pSIReportManager , pPrintDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintUserList") );
	
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintUserList") );

		nRowCnt++;
		pPrintDM->FreeData();

		ids = m_pPrintDM->GetCellData(_T("대출자KEY"), i, strKey );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("PrintUserList") );
		list.push_back(strKey);

	}

	BOOL bPrint = FALSE;

	ids = pSIReportManager->Print2(&bPrint);
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	WritePrintLog(nMode, bPrint, nRowCnt, &list);

	list.clear();
	
	return 0;

	EFS_END
	return -1;

}

CSIReportManager *CBO_LOC_3270::InitPrint( INT nMode)
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

	pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;

	ids = pSIReportManager->GetSIReportFileFromDB(m_sPrintModeAlias[nMode]);
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return NULL;
	}
	
	ids = pSIReportManager->DeleteTempPageFiles();

	return pSIReportManager;

	EFS_END
	return NULL;

}

INT CBO_LOC_3270::SetCondition( CSIReportManager *pSIReportManager , CESL_DataMgr *pPrintDM )
{
	EFS_BEGIN

	INT ids;

	const INT nCnt = 12;

	TCHAR *sFieldAlias[nCnt] =
	{
		_T("대출자이름")     , _T("대출자번호") , _T("대출직급정보")   , _T("대출소속정보") ,
		_T("자택전화번호")   , _T("자택주소")   , _T("자택우편번호")   , _T("우편발송지역") , 
		_T("근무지전화번호") , _T("근무지주소") , _T("근무지우편번호") , _T("핸드폰")
	};

	CString sValue[nCnt];

	for( INT i = 0 ; i < nCnt; i++ )
	{
		ids = pPrintDM->GetCellData( sFieldAlias[i] , 0 , sValue[i] );
		if( ids < 0 ) 
		{
			CString sEMsg;
			sEMsg.Format( _T("pPrintDM->GetCellData( %s , 0 , %s )") , sFieldAlias[i] , sValue[i] );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetCondition") );

		pSIReportManager->SetTempVariable( sFieldAlias[i] , sValue[i] );
	}

	CString sOneValue;
	sOneValue.Format( _T("%d") , pPrintDM->GetRowCount() );
	pSIReportManager->SetTempVariable( _T("총계") , sOneValue );

	sOneValue.Format( _T("%s") , sValue[0] );
	pSIReportManager->SetTempVariable( sFieldAlias[0] , sOneValue );

	sOneValue.Format( _T("%s") , sValue[1] );
	pSIReportManager->SetTempVariable( sFieldAlias[1] , sOneValue );

	sOneValue.Format( _T("%s(%s)") , sValue[2] , sValue[3] );
	pSIReportManager->SetTempVariable( sFieldAlias[2] , sOneValue );

	if( sValue[7].Compare(_T("H")) == 0 )
	{
		pSIReportManager->SetTempVariable( _T("대출자주소") , sValue[5] );
		pSIReportManager->SetTempVariable( _T("대출자우편번호") , sValue[6] );
	}
	else
	{
		pSIReportManager->SetTempVariable( _T("대출자주소") , sValue[9] );
		pSIReportManager->SetTempVariable( _T("대출자우편번호") , sValue[10] );		
	}
	pSIReportManager->SetTempVariable( _T("우편발송용대출자이름") , sValue[0] );
	

	SetOneCondValue( pSIReportManager );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3270::DateFormatConvert( CString &sDate )
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

INT CBO_LOC_3270::MakeAddress( CESL_DataMgr *pSrcDM , INT nRowIndex , CString &sAddress )
{
	EFS_BEGIN

	INT ids;

	const INT nCnt = 7;
	TCHAR *sAddressColumnAlias[nCnt] =
	{
		_T("우편발송지역") , _T("대출자이름") , _T("대출자번호") , _T("자택주소") , _T("자택우편번호") , 
		_T("근무지주소") , _T("근무지우편번호") 
	};

	CString sData[nCnt];

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = pSrcDM->GetCellData( sAddressColumnAlias[i] , nRowIndex , sData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeAddress") );

		sData[i].TrimLeft(); sData[i].TrimRight();
	}

	CString sUserName;
	CString sSendAddress;
	CString sZipCode;

	if( ((CButton*)GetDlgItem(IDC_chkIS_IN_USER_NO))->GetCheck() != 1 )
		sUserName.Format( _T("%s 귀하") , sData[1] );
	else
		sUserName.Format( _T("%s(%s) 귀하") , sData[1] , sData[2] );

	if( sData[0].Compare( _T("W") ) == 0 )
	{
		sSendAddress = sData[5];
		sZipCode = sData[6];
	}
	else
	{
		sSendAddress = sData[3];
		sZipCode = sData[4];
	}
	
	INT nMaxLineChar = 40;
	INT nCharSize = sZipCode.GetLength();
	
	for( i = 0 ; i < nMaxLineChar-nCharSize ; i++ )
		sZipCode = _T(" ") + sZipCode;

	sAddress +=  _T("\r\n\r\n\r\n\r\n") + sSendAddress + _T("\r\n") + sUserName + _T("\r\n") + sZipCode;


	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3270::OnbtnUSERCLOSE2() 
{
	EFS_BEGIN

	ClearControl();

	EFS_END
}


VOID CBO_LOC_3270::ClearControl()
{
	EFS_BEGIN

	const INT nCnt = 2;
	TCHAR *sCMAlias[nCnt] =
	{
		_T("CM_BO_LOC_3270") , _T("CM_BO_LOC_3270_ETC_INPUT")
	};

	CESL_ControlMgr *pCM;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pCM = FindCM( sCMAlias[i] );
		if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("ClearControl") );

		pCM->AllControlClear();
	}

	CComboBox *pcmbLoanType = (CComboBox*)GetDlgItem(IDC_cmbLoanType);
	if ( !pcmbLoanType ) ERROR_MSG_RETURN_VOID2( -2 , _T("ClearControl") );
	pcmbLoanType->SetCurSel(0);

	EFS_END
}

VOID CBO_LOC_3270::OnDblclklstDST(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ChangeOrderASCDSC();
	
	*pResult = 0;
}

INT CBO_LOC_3270::ChangeOrderASCDSC()
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



INT CBO_LOC_3270::ShowHideEtcInputCond( INT* nIDC , INT nCnt )
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabINFO);

	INT nMode = pTab->GetCurSel();

	INT bView;
	if( nMode == 0 )
		bView = SW_HIDE;
	else
		bView = SW_SHOW;

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		GetDlgItem(nIDC[i])->ShowWindow(bView);
	}

	return 0;

	EFS_END

	return -1;
}


INT CBO_LOC_3270::ShowHideOrdernputCond( INT* nIDC , INT nCnt )
{
	EFS_BEGIN
		
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabINFO);
	
	INT nMode = pTab->GetCurSel();
	
	INT bView;
	if( nMode == 1 )
		bView = SW_HIDE;
	else
		bView = SW_SHOW;
		
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		GetDlgItem(nIDC[i])->ShowWindow(bView);
	}
	
	return 0;
	
	EFS_END
		
	return -1;
}

VOID CBO_LOC_3270::OnSelchangetabINFO(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowHideEtcInfo();
	
	*pResult = 0;
}


INT CBO_LOC_3270::ShowHideEtcInfo()
{
	EFS_BEGIN

	const INT nOrderCnt = 6;
	
	INT nOrderIDC[nOrderCnt] =
	{
		IDC_staORDER_TITLE , IDC_lstSRC , IDC_btnORDER_ADD , 
		IDC_btnORDER_DELTE , IDC_lstDST , IDC_spnUPDOWN
	};

	ShowHideOrdernputCond( nOrderIDC , nOrderCnt );
	
	const INT nEtcCnt = 14;
	
	INT nEtcIDC[nEtcCnt] =
	{
		IDC_staORDER_TITLE2 , IDC_staMANAGER , IDC_edtPRINT_MANAGER , IDC_edtPRINT_PHONE , 
		IDC_staLIB , IDC_edtPRINT_LIB_NAME , IDC_staRETURNEXPIRE , IDC_edtPRINT_RETURN_EXPIRE , 
		IDC_chkIS_IN_USER_NO , IDC_staPHONE , IDC_staMANAGER2 , IDC_staMANAGER3 , 
		IDC_edtPRINT_MANAGER2 , IDC_edtPRINT_MANAGER3
	};

	ShowHideEtcInputCond( nEtcIDC , nEtcCnt );

	return 0;
	
	EFS_END

	return -1;
}

INT CBO_LOC_3270::InitLibInfo()
{
	EFS_BEGIN

	const INT nLibInfoCnt = 4;

	CString sLibInfo[nLibInfoCnt] =
	{
		_T("LIB_NAME") , _T("LIB_ADDRESS") , _T("LIB_TEL") , _T("LIB_ZIP_CODE")
	};

	CLocCommonAPI::GetLibInfo( sLibInfo , sLibInfo , nLibInfoCnt , this );

	CString sCMAlias = _T("CM_BO_LOC_3270_ETC_INPUT");
	
	CString sControlAlias[nLibInfoCnt] =
	{
		_T("도서관명") , _T("도서관주소") , _T("문의전화") , _T("도서관우편번호")
	};
	
	for( INT i = 0 ; i < nLibInfoCnt ; i++ )
		SetControlData( sCMAlias , sControlAlias[i] , sLibInfo[i] );

	SetControlData( sCMAlias , _T("담당자") , m_pInfo->USER_NAME );
	

	return 0;

	EFS_END
	
	return -1;

}

INT CBO_LOC_3270::SortAllCondition( CESL_DataMgr *pDM )
{
	EFS_BEGIN
	
	CListCtrl *pListCtrl = ( CListCtrl * )GetDlgItem( IDC_lstDST );
	INT listCnt = pListCtrl->GetItemCount() + 1;

	CString *strAlias = new CString[ listCnt ];
	INT *nCond = ( INT * )calloc( listCnt, sizeof( INT ) );
	
	strAlias[ 0 ].Format( _T("대출자KEY") );
	nCond[ 0 ] = true;
	
	for( INT i = 0; i < listCnt - 1; i++ )
	{
		if( _T("오름") == pListCtrl->GetItemText( i, 1 ) )
			nCond[ i + 1 ] = true;
		else
			nCond[ i + 1 ] = false;

		strAlias[ i + 1 ].Format( _T("%s"), pListCtrl->GetItemText( i, 0 ) );		
	}
	
	CString* strBuf = new CString[ listCnt ];
	INT* nCondBuf = ( INT * )calloc( listCnt, sizeof( INT ) );

	INT nDefPosition = 0;
	
	for( i = 1;i < listCnt;i++ )
	{
		if ( strAlias[i] == _T("대출자이름") || strAlias[i] == _T("대출자번호"))
		{
			strBuf[ nDefPosition ] = strAlias[i];
			nCondBuf[ nDefPosition ] = nCond[i];
			nDefPosition++;
		}			
	}

	for( i = 0;i < listCnt;i++ )
	{
		if ( strAlias[i] == _T("대출자이름") || strAlias[i] == _T("대출자번호")) continue;

		strBuf[ nDefPosition ] = strAlias[i];
		nCondBuf[ nDefPosition ] = nCond[i];
		nDefPosition++;
	}
		
	pDM->SORT( listCnt, strBuf, nCondBuf, ESL_DATAMGR_SORT_ASC );
	
	delete [] strBuf;
	free( nCondBuf );

	delete [] strAlias;
	free( nCond );

	return 0;
	
	EFS_END

	return -1;
}

VOID CBO_LOC_3270::SetCivilNumViewMode()
{
	EFS_BEGIN

	CString result;
	GetManageValue( _T("열람"), _T("공통"), _T("대출자주민등록번호보여주기"), _T("대출자주민번호"), result );
 
	if( _T("N") == result )
		m_civilNumViewMode = false;
	else
		m_civilNumViewMode = true;


	EFS_END

}

VOID CBO_LOC_3270::SetUserNoViewMode()
{
EFS_BEGIN

	CString result;
	GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), result );
 
	if( _T("Y") == result ) 
		m_bUserNoViewMode = TRUE;
	else
		m_bUserNoViewMode = FALSE;

EFS_END
}

INT CBO_LOC_3270::InitNDScreen()
{
	CWnd* pWnd;
	CString sErrMsg;

	pWnd = GetDlgItem(IDC_edtPRINT_USER_NO1);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3270::InitNDScreen -1 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pWnd->ShowWindow(SW_HIDE);
	
	pWnd = GetDlgItem(IDC_edtPRINT_USER_NO2);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3270::InitNDScreen -2 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pWnd->ShowWindow(SW_HIDE);

	pWnd = GetDlgItem(IDC_edtCLASSNO1);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3270::InitNDScreen -3 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pWnd->ShowWindow(SW_SHOW);

	pWnd = GetDlgItem(IDC_edtCLASSNO2);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3270::InitNDScreen -4 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pWnd->ShowWindow(SW_SHOW);

	pWnd = GetDlgItem(IDC_stChangeNoSTATIC);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3270::InitNDScreen -5 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	pWnd->SetWindowText( _T("교        번") );

	pWnd = GetDlgItem(IDC_chkReservation);
	if ( pWnd == NULL )
	{
		sErrMsg.Format( _T(" CBO_LOC_3270::InitNDScreen -6 ERROR! ") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	pWnd->ShowWindow(SW_SHOW);
	return 0;
}


HBRUSH CBO_LOC_3270::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
