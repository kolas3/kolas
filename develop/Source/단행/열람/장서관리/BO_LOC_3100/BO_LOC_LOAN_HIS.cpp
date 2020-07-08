// BO_LOC_LOAN_HIS.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_LOAN_HIS.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "BO_LOC_3000Api.h"
#include "PersonInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_LOAN_HIS dialog

CBO_LOC_LOAN_HIS::CBO_LOC_LOAN_HIS(CESL_Mgr* pParent /*=NULL*/,CLoanShareManager* pLoanShareManager, CLonaManageValue* pLona /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_LOAN_HIS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pLoanShareManager = pLoanShareManager;
	m_pLona = pLona;
}

CBO_LOC_LOAN_HIS::~CBO_LOC_LOAN_HIS()
{
	CString strFileName;
	DWORD dwPID = ::GetCurrentProcessId();		
	strFileName.Format(_T("..\\cfg\\TouchPad\\BSCH(%u).dat"), dwPID);
}

VOID CBO_LOC_LOAN_HIS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_LOAN_HIS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_LOAN_HIS, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_LOAN_HIS)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnPRINT_LOAN_HIS, OnbtnPRINT)
	ON_BN_CLICKED(IDC_btnDATE_SEARCH, OnbtnDATE_SEARCH)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FAMILYINFO, OnSelchangeTabFamilyinfo)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnUNDO, OnbtnUNDO)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_LOAN_HIS message handlers

BOOL CBO_LOC_LOAN_HIS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );

	EnableThemeDialogTexture(GetSafeHwnd());

	if( 0 > InitESL_Mgr(_T("BO_LOC_LOAN_HIS")) )
	{
		return -1;
	}

	CTime tFrom = CTime::GetCurrentTime();
	CTime tTo( tFrom.GetYear()-1, tFrom.GetMonth(), tFrom.GetDay(),0,0,0 );

	CDateTimeCtrl *pDateTo	 = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE1 );
	pDateTo->SetTime( &tTo );

	CDateTimeCtrl *pDateFrom = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE2 );
	pDateFrom->SetTime( &tFrom );

	m_pLona->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("가족회원기능사용여부"), m_sIsFamily );
	m_sIsFamily.TrimLeft();	m_sIsFamily.TrimRight();
	
	InitFamilyInfo();
	InitFamilyInfoTab();
	
	ShowLoanHistory();

	CESL_ControlMgr* pCM =  FindCM(_T("CM_BO_LOC_LOAN_HIS"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_LOAN_HIS"),_T("대출이력정보G"));
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("개인별 대출 이력 조회");
	}

	InitControlPos();

	m_bInLS_WORK_T01 = FALSE;
	m_strLibCode     = _T("");
	m_strDecideLSStatus = _T("");
	m_bForceIllReturn   = FALSE;

	return TRUE;
}

VOID CBO_LOC_LOAN_HIS::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);	

	InitControlPos();
}

INT CBO_LOC_LOAN_HIS::InitControlPos()
{
EFS_BEGIN
	
	if( GetSafeHwnd() )
	{
		CRect rcDialog;
		GetWindowRect(rcDialog);

		CWnd* pTabWnd = GetDlgItem(IDC_TAB_FAMILYINFO);
		if( pTabWnd->GetSafeHwnd() )
		{
			pTabWnd->MoveWindow(3, 3, rcDialog.Width()-13, rcDialog.Height()-39);
		}
		
		CWnd* pGridWnd = GetDlgItem(IDC_gridLOAN_HIS);
		if( pGridWnd->GetSafeHwnd() )
		{
			pGridWnd->MoveWindow(10, 57, rcDialog.Width()-30, rcDialog.Height()-100);
		}
	}

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_LOAN_HIS::OnbtnPRINT() 
{	
	Print();
}

INT CBO_LOC_LOAN_HIS::InitFamilyInfo()
{
EFS_BEGIN
	
	INT		ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_LOAN_HIS_FAMILYINFO"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

	pDM->FreeData();
	
	INT	nRowCount = 0;
	CString strWhere;
	if( m_sIsFamily == _T("Y") )
	{
		strWhere.Format(_T("CLU.FID_KEY = (SELECT FID_KEY FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s)"), m_strUserKey);
		
		ids = pDM->RefreshDataManager( strWhere );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

		nRowCount = pDM->GetRowCount();
	}
	else
	{
		nRowCount = 0;
	}

	CString strName;
	CString strUserNo;

	if( 1 < nRowCount )
	{
		for( INT i=0;i<nRowCount;i++ )
		{		
			CString strUserKey;		

			ids = pDM->GetCellData(_T("대출자KEY"), i, strUserKey);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

			if( strUserKey == m_strUserKey )
			{
				ids = pDM->GetCellData(_T("대출자명"), i, strName);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

				ids = pDM->GetCellData(_T("대출자번호"), i, strUserNo);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

				ids = pDM->DeleteRow(i);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

				ids = pDM->InsertRow(0);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

				ids = pDM->SetCellData(_T("대출자KEY"), strUserKey, 0);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

				ids = pDM->SetCellData(_T("대출자명"), strName, 0);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

				ids = pDM->SetCellData(_T("대출자번호"), strUserNo, 0);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

				break;
			}
		}
	}
	else if( 0 == nRowCount )
	{
		strWhere.Format(_T("CLU.REC_KEY = %s"), m_strUserKey);
	
		ids = pDM->RefreshDataManager( strWhere );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfo()") );

	}

	return 0;
EFS_END
return -1;
}

INT CBO_LOC_LOAN_HIS::InitFamilyInfoTab()
{
EFS_BEGIN
	
	INT		ids;
	
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_LOAN_HIS_FAMILYINFO"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfoTab()") );

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_FAMILYINFO);
	if( NULL == pTab )	ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfoTab()") );;
	
	INT	nRowCount = pDM->GetRowCount();

	CString strValue,strTabAlias=_T("대출자명");
	GetManageValue(_T("열람"), _T("공통"), _T("대출자접근권한"), GetUserID(), strValue);
	if( strValue.IsEmpty() || strValue == _T("1") )
		strTabAlias = _T("대출자번호");

	CString strData;
	if( nRowCount > 0 )
	{
		for( INT i=0;i<nRowCount;i++ )
		{
			ids = pDM->GetCellData(strTabAlias, i, strData);
			if( NULL == pTab )	ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfoTab()") );
			pTab->InsertItem(i, strData);
		}
	}
	else if( nRowCount == 0 )
	{
		ids = pDM->GetCellData(strTabAlias, 0, strData);
		if( NULL == pTab )	ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_LOAN_HIS::InitFamilyInfoTab()") );
		pTab->InsertItem(0, strData);
	}
	
	pTab->SetCurSel(0);

	return 0;
EFS_END
return -1;
}

INT CBO_LOC_LOAN_HIS::GetLoanHistory()
{
	return GetLoanHistory(m_strUserKey);
}

INT CBO_LOC_LOAN_HIS::GetLoanHistory(CString strUserKey)
{
EFS_BEGIN
	
	INT		ids;

	if( strUserKey.IsEmpty() )	
	{
		return -1001;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_LOAN_HIS"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );

	CString sWhere;;
	sWhere.Format(_T("CL.USER_KEY=%s"), strUserKey );

	GetDateControl();
	CString sLoanDateWhere = _T("");
	if( !m_sDateTo.IsEmpty() && !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND (CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" OR CL.RESERVE_CANCEL_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS'))")
								, m_sDateTo, m_sDateFrom, m_sDateTo, m_sDateFrom );
	}
	else if( !m_sDateTo.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND (CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE")
								_T(" OR CL.RESERVE_CANCEL_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE)")
								, m_sDateTo, m_sDateTo );	
	}
	else if( !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND (CL.LOAN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" OR CL.RESERVE_CANCEL_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS'))")
								, m_sDateFrom, m_sDateFrom );
	}

	sWhere += sLoanDateWhere;

	ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );
	
	INT		nRowCount = pDM->GetRowCount();	
	CString strData;

	for( INT i=0;i<nRowCount;i++ )
	{
		ids = pDM->GetCellData( _T("단행연속구분"), i, strData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );

		GetPublishFormCodeToDesc(strData);

		ids = pDM->SetCellData( _T("단행연속구분"), strData, i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );

		ids = pDM->GetCellData( _T("대출상태"), i, strData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );
		GetLoanStatusToDesc(strData);
		ids = pDM->SetCellData( _T("대출상태"),  strData, i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );

		CString sDelayDay, sReturnStatus = _T("");
		ids = pDM->GetCellData( _T("연체일"), i, sDelayDay );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );		

		if(strData == _T("반납") || strData == _T("책두레반납"))
		{
			if(0 < _ttoi(sDelayDay))	
			{
				sReturnStatus = _T("연체");
			}
			else
			{
				sReturnStatus = _T("정상");
			}
		}
		ids = pDM->SetCellData( _T("반납상태"),  sReturnStatus, i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );
				
		strData.Format( _T("%d"), i + 1 );
		ids = pDM->SetCellData( _T("번호"),  strData, i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );		

		ids = pDM->GetCellData( _T("BOOK_MANAGE_CODE"), i, strData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );

		CString strDesc;
		this->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), strData, strDesc );

		ids = pDM->SetCellData( _T("관리구분설명"),  strDesc, i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_LOAN_HIS::GetLoanHistory") );
	}

	return 0;
EFS_END
return -1;
}

INT CBO_LOC_LOAN_HIS::GetPublishFormCodeToDesc(CString &strData, INT nFlag/*=0*/)
{
EFS_BEGIN
	
	const INT nCnt = 5;
	CString strPulishFormCode[nCnt][2] = {
		{_T("MO"), _T("단행")},
		{_T("SE"), _T("연속")},
		{_T("NB"), _T("긴급")}
	};

	for( INT i=0;i<nCnt;i++ )
	{
		if( strData == strPulishFormCode[i][nFlag] )
		{
			strData = strPulishFormCode[i][!nFlag];
			return 0;
		}
	}

	return 0;
EFS_END
return -1;
}

INT CBO_LOC_LOAN_HIS::GetLoanStatusToDesc(CString &strData, INT nFlag/*=0*/)
{
EFS_BEGIN

	const INT nCnt = 11;
	CString strLoanStatus[nCnt][2] = {
		{_T("0"), _T("대출중")},
		{_T("1"), _T("반납")},
		{_T("2"), _T("반납연기")},
		{_T("3"), _T("예약")},
		{_T("4"), _T("예약취소")},
		{_T("5"), _T("무인반납중")},
		{_T("6"), _T("분관반납중")},
		{_T("7"), _T("타관반납중")},
		{_T("L"), _T("책두레대출중")},
		{_T("R"), _T("책두레반납")},
		{_T("D"), _T("책두레반납연기")}
	};

	for( INT i=0;i<nCnt;i++ )
	{
		if( strData == strLoanStatus[i][nFlag] )
		{
			strData = strLoanStatus[i][!nFlag];
			return 0;
		}
	}

	return 0;
EFS_END
return -1;
}

INT CBO_LOC_LOAN_HIS::ShowLoanHistory()
{
EFS_BEGIN
	
	INT		ids;
	
	CString strUserKey;
	ids = GetCurrentUserKey(strUserKey);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::ShowLoanHistory()") );

	if( !strUserKey.IsEmpty() )
		ids = GetLoanHistory( strUserKey );	
	else
		ids = GetLoanHistory();
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_LOAN_HIS::ShowLoanHistory()") );

	ids = AllControlDisplay(_T("CM_BO_LOC_LOAN_HIS"));
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_LOAN_HIS::ShowLoanHistory()") );

	return 0;
EFS_END
return -1;
}

void CBO_LOC_LOAN_HIS::OnSelchangeTabFamilyinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	ShowLoanHistory();
	*pResult = 0;
}

INT CBO_LOC_LOAN_HIS::GetCurrentUserKey(CString &strUserKey)
{
EFS_BEGIN

	INT		ids;

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_FAMILYINFO);
	if( NULL == pTab ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::GetCurrentUserKey") );
	
	INT nIndex = pTab->GetCurSel();
	
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_LOAN_HIS_FAMILYINFO"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_LOAN_HIS::GetCurrentUserKey()") );
	
	ids = pDM->GetCellData( _T("대출자KEY"), nIndex, strUserKey );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_LOAN_HIS::GetCurrentUserKey()") );
	
	return 0;
EFS_END
return -1;
}

INT MakeCopyDM(CESL_DataMgr * pSrcDM, CESL_DataMgr * pDstDM)
{
	INT ids = -1;

	pDstDM->TBL_NAME = pSrcDM->TBL_NAME;
	pDstDM->EXTRA_CONDITION = pSrcDM->EXTRA_CONDITION;
	pDstDM->CONDITION = pSrcDM->CONDITION;
	pDstDM->DB_MGR_ALIAS = pSrcDM->DB_MGR_ALIAS;
	

	pDstDM->InitDataMgrRef( pSrcDM->RefList.GetCount() );
	pDstDM->m_nCols = pSrcDM->RefList.GetCount();

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pSrcDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) return -1;

		ids = pDstDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) return -1;
	}
	return 0;
}

INT	CBO_LOC_LOAN_HIS::Print()
{
EFS_BEGIN

	INT		ids;
	
	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BO_LOC_LOAN_HIS") );
	if( NULL == pPrintDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("Print") );
	
	if( 1 > pPrintDM->GetRowCount() )
	{
		return AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );		
	}	
	
	CSIReportManager* pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if( NULL == pSIReportManager) 
	{
		return AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	ids = pSIReportManager->GetSIReportFileFromDB(_T("개인별대출반납이력목록"));
	if( 0 > ids ) 
	{
		if( pSIReportManager )
			delete pSIReportManager;
		return AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
	}

	ids = GetLoanUserAdditionInfo();
	if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("Print") );

	CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_LOAN_HIS_USER"));
	if( NULL == pUserDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("Print") );

	if(PrivacyPrintLogRecord(pUserDM, _T("개인별 대출이력 출력"), _T("E") ,0 ) < 0 ) return 0;

	if( pUserDM->GetRowCount() )
	{
		CString strData;

		CString strID,strH_Phone,strCellPhone;

		ids = pUserDM->GetCellData(_T("이름"), 0, strData );
		if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("Print") );

		strData = _T("이름:") + strData;
		pSIReportManager->SetTempVariable( _T("대출자이름") , strData );

		ids = pUserDM->GetCellData(_T("사용자ID"), 0, strID );
		if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("Print") );
		strID = _T("ID:") + strID;

		pSIReportManager->SetTempVariable( _T("대출자ID") , strID );

		ids = pUserDM->GetCellData(_T("대출자번호"), 0, strData );
		if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("Print") );

		CLocCommonAPI::ChangeSecuriyUserNo( this , strData , m_pLona->m_sIsUnityLoanService );

		strData = _T("대출자번호:") + strData;
		pSIReportManager->SetTempVariable( _T("대출자번호") , strData );

		ids = pUserDM->GetCellData(_T("대출소속정보"), 0, strData );
		if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_LOAN_HIS::Print()") );

		pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출소속정보"), strData, strData);
		
		strData = _T("소속:") + strData;
		pSIReportManager->SetTempVariable( _T("대출자소속") , strData );

		ids = pUserDM->GetCellData(_T("대출직급정보"), 0, strData );
		if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_LOAN_HIS::Print()") );
		
		pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출직급정보"), strData, strData);

		strData = _T("직급:") + strData;
		pSIReportManager->SetTempVariable( _T("대출자직급") , strData );

		ids = pUserDM->GetCellData(_T("자택전화"), 0, strH_Phone );
		if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_LOAN_HIS::Print()") );

		strH_Phone = _T("집전화:") + strH_Phone;
		pSIReportManager->SetTempVariable( _T("자택전화번호") , strH_Phone );

		ids = pUserDM->GetCellData(_T("휴대폰"), 0, strCellPhone );
		if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_LOAN_HIS::Print()") );

		strCellPhone = _T("핸드폰:") + strCellPhone;
		pSIReportManager->SetTempVariable( _T("휴대폰") , strCellPhone );

		ids = pUserDM->GetCellData(_T("교번"), 0, strData );
		if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_LOAN_HIS::Print()") );

		strData = _T("교번:") + strData;
		pSIReportManager->SetTempVariable( _T("교번") , strData );

		strData.Format(_T("%d"), pPrintDM->GetRowCount());		
		pSIReportManager->SetTempVariable( _T("총계") , strData );
	}	

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	ids = pSIReportManager->Print();
	if(pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_PRINT_LOG") );
	std::vector<CString> list;
	CString strKey;
	ids = pUserDM->GetCellData(_T("REC_KEY"), 0, strKey );
	if( 0 > ids )  ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("Print") );
	list.push_back(strKey);
	CPersonInfo::SetPrintLog(this, pDM, PERINFO_EXCEL, m_pInfo, _T("개인별대출이력보기"), pUserDM->GetRowCount(), &list);
	list.clear();

	return 0;
EFS_END
return -1;
}

INT	CBO_LOC_LOAN_HIS::GetLoanUserAdditionInfo()
{
EFS_BEGIN

	INT		ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_LOAN_HIS_USER"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::GetLoanUserAdditionInfo()") );

	CString strUserKey;
	ids = GetCurrentUserKey(strUserKey);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_LOAN_HIS::GetLoanUserAdditionInfo()") );

	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"), strUserKey );
	ids = pDM->RefreshDataManager(strWhere);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_LOAN_HIS::GetLoanUserAdditionInfo()") );

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray saDes1, saDes2, saDes3, saDes4, saDes5;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("자택주소"), i, strGetData);
			saDes1.Add(strGetData);
			pDM->GetCellData(_T("근무지주소"), i, strGetData);
			saDes2.Add(strGetData);
			pDM->GetCellData(_T("휴대폰"), i, strGetData);
			saDes3.Add(strGetData);
			pDM->GetCellData(_T("자택전화"), i, strGetData);
			saDes4.Add(strGetData);
			pDM->GetCellData(_T("근무지전화"), i, strGetData);
			saDes5.Add(strGetData);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), saDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), saDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), saDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), saDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), saDes5);
					pApi->CloseSocket();	
				}
				for(INT i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("자택주소"), saDes1.GetAt(i), i);
					pDM->SetCellData(_T("근무지주소"), saDes2.GetAt(i), i);
					pDM->SetCellData(_T("휴대폰"), saDes3.GetAt(i), i);
					pDM->SetCellData(_T("자택전화"), saDes4.GetAt(i), i);
					pDM->SetCellData(_T("근무지전화"), saDes5.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		saDes1.RemoveAll(); saDes1.FreeExtra();
		saDes2.RemoveAll(); saDes2.FreeExtra();
		saDes3.RemoveAll(); saDes3.FreeExtra();
		saDes4.RemoveAll(); saDes4.FreeExtra();
		saDes5.RemoveAll(); saDes5.FreeExtra();
	}

	return 0;
EFS_END
return -1;
}

INT CBO_LOC_LOAN_HIS::SIReportEventHandler(CSIReportEventInfo * EventInfo)
{
EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;	
EFS_END
return -1;	
}

HBRUSH CBO_LOC_LOAN_HIS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    return hbr;
}

BOOL CBO_LOC_LOAN_HIS::PreTranslateMessage(MSG* pMsg) 
{
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

VOID CBO_LOC_LOAN_HIS::OnbtnDATE_SEARCH()
{
	ShowLoanHistory();
}

VOID CBO_LOC_LOAN_HIS::GetDateControl()
{
	SYSTEMTIME	tTime;
	DWORD		dwResult;

	CDateTimeCtrl *pDateTo	 = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE1 );
	dwResult = pDateTo->GetTime( &tTime );
	if (dwResult != GDT_VALID )
	{
		m_sDateTo = _T("");
	}
	else
	{
		m_sDateTo.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
	
	CDateTimeCtrl *pDateFrom = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE2 );
	dwResult = pDateFrom->GetTime( &tTime );
	if (dwResult != GDT_VALID ) 
	{
		m_sDateFrom = _T("");
	}
	else
	{
		m_sDateFrom.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
}

VOID CBO_LOC_LOAN_HIS::OnbtnUNDO() 
{
	INT ids, i, nRowCnt=0, nSelectedRow=-1;
	CString strSelect, sMsg;
	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_LOAN_HIS") );
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_LOAN_HIS"));
	CESL_DataMgr* pDM_User = FindDM(_T("DM_BO_LOC_LOAN_HIS_USER"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_LOAN_HIS") , _T("대출이력정보G") );

	nRowCnt = pDM->GetRowCount();
	if( nRowCnt <= 0)
	{
		AfxMessageBox(_T("이력을 복원할 자료가 없습니다."));
		return;
	}

	for ( i=0; i<nRowCnt; i++ ) {
		pGrid->GetAt(i, 0, strSelect);
		if ( strSelect == _T("V") ) nSelectedRow = i;
	}
	if ( nSelectedRow < 0 ) {
		AfxMessageBox(_T("복원하실 자료를 선정하십시오."));
		return;
	}	

	m_strLibCode = m_pInfo->LIB_CODE;
	m_strDecideLSStatus = _T("");

	ids = IsRightMat(pDM, nSelectedRow);
	if (ids == 100 ) return;

	ids = DecideReturnCase(pDM, nSelectedRow);
	if ( ids ) ERROR_MSG_RETURN_VOID2( -250 , _T("OnbtnUNDO()") );
	
	ids = ProcReplace(pDM, nSelectedRow);
	if ( ids ) ERROR_MSG_RETURN_VOID2( -200 , _T("OnbtnUNDO()") );

	ShowLoanHistory();

	CESL_Mgr* pMgr = m_pParent;
	while(TRUE)
	{
		if(_T("BO_LOC_3100") == pMgr->SM_ALIAS)
		{
			CESL_DataMgr* pReturnBasket = FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));	
			if( pReturnBasket == NULL ) ERROR_MSG_RETURN_VOID2( -3 , _T("DeleteReturnedBook") );
			for(INT i = 0; i < pReturnBasket->GetRowCount(); i++)
			{
				CString sValue, strLoanKey;
				strLoanKey = pDM->GetCellData( _T("대출/반납KEY"), nSelectedRow );		//LS_WORK_T01 - REC_KEY
				pReturnBasket->GetCellData(_T("대출/반납정보KEY"), i, sValue);
				if(nSelectedRow == sValue)
				{
					pReturnBasket->DeleteRow(i);
					break;
				}
			}
			break;
		}
		if(NULL == pMgr->pParentMgr) break;
		pMgr = pMgr->pParentMgr;
		}
	m_pParent->SendMessage(WM_USER+150,0,0);

	AfxMessageBox(_T("복원처리 되었습니다."));
	
	
	return;
}

INT CBO_LOC_LOAN_HIS::IsRightMat(CESL_DataMgr* pDM, INT nSelectedRow)
{	
	CString strStatus, strReturnLibCode, strBookKey, strQuery, strDelayDay, strLoanFlag, strEndCnt;

	strStatus     = pDM->GetCellData( _T("상태")    , nSelectedRow );

	strReturnLibCode = pDM->GetCellData( _T("ILL_RETURN_LIBCODE"), nSelectedRow );	
	if ( strReturnLibCode.GetLength() == 0 ) {
		strReturnLibCode = pDM->GetCellData( _T("BOOK_LIBCODE"), nSelectedRow );
	}
	if ( strReturnLibCode != m_pInfo->LIB_CODE ) {
		AfxMessageBox(_T("우리관에서 반납한 이력만 복원가능합니다."));
		return 100;
	}
	
	if( strStatus == _T("4")) {
		AfxMessageBox(_T("예약취소 자료는 복원이 불가능합니다."));
		return 100;
	}
	
	if( _T("1") != strStatus && _T("R") != strStatus && _T("6") != strStatus && _T("7") != strStatus ) {
		AfxMessageBox(_T("복원 가능한 자료가 아닙니다."));
		return 100;
	}
	
	strBookKey    = pDM->GetCellData( _T("책KEY")   , nSelectedRow );			
	strQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE STATUS IN ('0','2','3','L') AND BOOK_KEY=%s"), strBookKey);
	pDM->GetOneValueQuery(strQuery, strLoanFlag);
	if(strLoanFlag != _T("0"))
	{
		AfxMessageBox(_T("선택하신 자료는 대출중이거나 예약중이어서 복원할 수 없습니다."));		
		return 100;
	}		
	
	strDelayDay   = pDM->GetCellData( _T("연체일")  , nSelectedRow);	
	if(strDelayDay.IsEmpty()) {
		strDelayDay = _T("0");
	}
	
	if(0 < _ttoi(strDelayDay)) {
		AfxMessageBox(_T("연체 자료는 반납 복원할 수 없습니다."));		
		return 100;
	}

	CString strttt;
	if ( strStatus == _T("R")) {
		strQuery.Format( _T("SELECT COUNT(1) FROM ILL_TRANS_TBL IT, LS_WORK_T01 LW, BO_BOOK_TBL BB ")
			_T("WHERE ( (BB.WORKING_STATUS = 'BOL112N' AND IT.STATE = '0009' AND IT.GIVE_LIBCODE = IT.RETURN_LIBCODE AND LW.STATUS = '1') OR ")
			_T("        (BB.WORKING_STATUS = 'BOL411O' AND IT.STATE = '0014' AND IT.GIVE_LIBCODE != IT.RETURN_LIBCODE AND LW.STATUS = 'R') OR ")
			_T("        (BB.WORKING_STATUS = 'BOL112N' AND IT.STATE = '0009' AND IT.GIVE_LIBCODE = IT.RETURN_LIBCODE )) AND ")
			_T("IT.BOOK_KEY = BB.REC_KEY AND IT.BOOK_KEY = LW.BOOK_KEY AND IT.BOOK_KEY = %s"), strBookKey);
		
		pDM->GetOneValueQuery(strQuery,strEndCnt);

		if(_ttoi(strEndCnt) <= 0) {
			strQuery.Format( _T("SELECT COUNT(1) FROM ILL_TRANS_TBL IT, CO_LOAN_TBL LW, BO_BOOK_TBL BB ")
				_T("WHERE ( (BB.WORKING_STATUS = 'BOL112N' AND IT.STATE = '0009' AND IT.GIVE_LIBCODE = IT.RETURN_LIBCODE AND LW.STATUS = '1') OR ")
				_T("        (BB.WORKING_STATUS = 'BOL411O' AND IT.STATE = '0014' AND IT.GIVE_LIBCODE != IT.RETURN_LIBCODE AND LW.STATUS = 'R') OR ")
				_T("        (BB.WORKING_STATUS = 'BOL112N' AND IT.STATE = '0009' AND IT.GIVE_LIBCODE = IT.RETURN_LIBCODE )) AND ")
				_T("IT.BOOK_KEY = BB.REC_KEY AND IT.BOOK_KEY = LW.BOOK_KEY AND IT.BOOK_KEY = %s"), strBookKey);
			
			pDM->GetOneValueQuery(strQuery,strEndCnt);
		}
		
		if(_ttoi(strEndCnt) <= 0) {
			AfxMessageBox(_T("해당 자료는 복원이 불가능한 자료입니다."));
			return 100;
		}
	}	

	CString strBookLibCode, strBookAppendixFlag;
	CString strUnityLoanCnt, strLoanCnt, strAlertMsg;

	strBookLibCode = pDM->GetCellData( _T("BOOK_LIBCODE"), nSelectedRow );	
	strBookAppendixFlag = pDM->GetCellData( _T("BOOK_APPENDIX_FLAG"), nSelectedRow );	

	GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출가능책수"), strLoanCnt, 0);
	if(m_pLona->m_sUnityLoanCntYN == _T("Y")){
		GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합대출가능책수"), strUnityLoanCnt, 0);
	}else{
		GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("타관대출가능책수"), strUnityLoanCnt, 0);
	}

	strAlertMsg.Empty();

	if(  strBookAppendixFlag == _T("B") || (strBookAppendixFlag == _T("A") && m_pLona->m_sIsIncludeLoanCnt == _T("Y")) ){
		if(_ttoi(strUnityLoanCnt) <= 0){
			if(m_pLona->m_sUnityLoanCntYN == _T("Y")){
				strAlertMsg.Format(_T("해당 대출이력을 복원할 경우\r\n통합대출가능책수가 초과됩니다.\r\n복원하시겠습니까?"));
			}else{
				strAlertMsg.Format(_T("해당 대출이력을 복원할 경우\r\n타관대출가능책수가 초과됩니다.\r\n복원하시겠습니까?"));
			}
		}else if(_ttoi(strLoanCnt) <= 0 && strBookLibCode == m_pInfo->LIB_CODE){
			strAlertMsg.Format(_T("해당 대출이력을 복원할 경우\r\n대출가능책수가 초과됩니다.\r\n복원하시겠습니까?"));
		}
	}
	if(strAlertMsg.IsEmpty() == FALSE){
		if(IDYES == (AfxMessageBox(strAlertMsg, MB_ICONQUESTION | MB_YESNO))){
			return 0;
		}else{
			return 100;
		}
	}

	strAlertMsg.Format(_T("\r\n선정된 반납 자료를 복원 하시겠습니까?\r\n"));
	if(IDYES == (AfxMessageBox(strAlertMsg, MB_ICONQUESTION | MB_YESNO)))
		return 0;
	else
		return 100;
}

INT CBO_LOC_LOAN_HIS::DecideReturnCase(CESL_DataMgr* pDM, INT nSelectedRow)
{
	CString strResult, strQuery, strLoanKey, strDelayCnt, strStatus, strBookKey, strAplLib;
	
	strLoanKey = pDM->GetCellData( _T("대출/반납KEY"), nSelectedRow );
	strStatus  = pDM->GetCellData( _T("상태")    , nSelectedRow );
	strBookKey = pDM->GetCellData( _T("책KEY")   , nSelectedRow );			
	
	strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE REC_KEY=%s"), strLoanKey);
	pDM->GetOneValueQuery(strQuery,strResult);
	
	m_bInLS_WORK_T01 = FALSE;
	
	if(_ttoi(strResult) != 0) {
		m_bInLS_WORK_T01 = TRUE;
	}
		
	if(m_bInLS_WORK_T01) {
		strQuery.Format(_T("SELECT DELAY_CNT FROM LS_WORK_T01 WHERE REC_KEY=%s"), strLoanKey);
	}
	else {
		strQuery.Format(_T("SELECT DELAY_CNT FROM CO_LOAN_TBL WHERE REC_KEY=%s"), strLoanKey);
	}
	pDM->GetOneValueQuery(strQuery, strDelayCnt);
	if(strDelayCnt.IsEmpty()) {
		strDelayCnt = _T("0");
	}	

	m_bForceIllReturn = FALSE;
	strAplLib.Empty();
	if ( strStatus == _T("R") ) {
		strQuery.Format(_T("SELECT APL_LIBCODE FROM ILL_TRANS_TBL WHERE LOAN_KEY=%s"), strLoanKey);
		pDM->GetOneValueQuery(strQuery, strAplLib);
		if(strAplLib.IsEmpty()) {
			m_bForceIllReturn = TRUE;
		}		

		if ( m_bForceIllReturn ) {
			if ( _ttoi(strDelayCnt) > 0 ) m_strDecideLSStatus = _T("2");
			else m_strDecideLSStatus = _T("0");
		}
		else { 
			if ( _ttoi(strDelayCnt) > 0 ) m_strDecideLSStatus = _T("D");
			else m_strDecideLSStatus = _T("L");
		}
	}
	else {
		if ( _ttoi(strDelayCnt) > 0 ) m_strDecideLSStatus = _T("2");
		else m_strDecideLSStatus = _T("0");	
	}
	
	return 0;
}


INT CBO_LOC_LOAN_HIS::ProcReplace(CESL_DataMgr* pDM, INT nSelectedRow)
{
	INT ids;
	CString strLoanKey, strStatus, strBookKey, strPublishFormCode, strSpeciesKey, strQuery; 
	CString strTblName, strWorkingStatus, strPrevWorkingStatus, strReturnPlanDate;
	

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	strLoanKey = pDM->GetCellData( _T("대출/반납KEY"), nSelectedRow );
	strStatus  = pDM->GetCellData( _T("상태")    , nSelectedRow );
	strBookKey = pDM->GetCellData( _T("책KEY")   , nSelectedRow );
	strPublishFormCode = pDM->GetCellData(_T("단행연속구분"), nSelectedRow);
	strSpeciesKey = pDM->GetCellData(_T("종정보키"), nSelectedRow);

	TmpDM.FreeData();
	TmpDM.StartFrame();
	
	if(m_bInLS_WORK_T01 == FALSE){
		strQuery.Format(
			_T("INSERT INTO LS_WORK_T01")
			_T("(")
			_T("STATUS,RETURN_DATE,R_DEVICE,R_WORKER,RETURN_TYPE_CODE,LAST_WORK,")
			_T("REC_KEY,PUBLISH_FORM_CODE,BOOK_KEY,USER_KEY,LOAN_KEY,LOAN_TYPE_CODE,LOAN_DATE,RETURN_PLAN_DATE,RESERVATION_DATE,RESERVATION_EXPIRE_DATE,SHELF_LOC_CODE,ARREAR,REPARATION,CLASS_NO_TYPE,CLASS_NO,MANAGE_CODE,REG_CODE,MEMBER_CLASS,SPECIES_KEY,SEPARATE_SHELF_CODE,REG_NO,TITLE,AUTHOR,PUBLISHER,CALL_NO,DELAY_CNT,RESERVE_CANCEL_DATE,DELAY_RETURN_DATE,PRE_RETURN_PLAN_DATE,SELF_RETURN_STATION,SELF_RETURN_DOOR,SELF_LOAN_STATION,LOAN_MANAGE_CODE,RETURN_MANAGE_CODE,VOL_INFO,RECEIPT_LOAN_KEY,RECEIPT_RETURN_KEY,RECEIPT_PRINT_LOAN_YN,RECEIPT_PRINT_RETURN_YN,NIGHT_RESERVATION_LOAN,UNMANNED_RESERVATION_LOAN,RESERVATION_CANCEL_TYPE,PREENGAGE_CANCEL_LIMIT_COUNT,PREENGAGE_CANCEL_LIMIT_PERIOD,USER_CLASS_CODE,USER_POSITION_CODE,REQUEST_LIB_CODE,FIRST_WORK,L_DEVICE,L_WORKER")
			_T(")")
			_T("SELECT ")
			_T("'%s',NULL,NULL,NULL,NULL,'%s',")
			_T("REC_KEY,PUBLISH_FORM_CODE,BOOK_KEY,USER_KEY,LOAN_KEY,LOAN_TYPE_CODE,LOAN_DATE,RETURN_PLAN_DATE,RESERVATION_DATE,RESERVATION_EXPIRE_DATE,SHELF_LOC_CODE,ARREAR,REPARATION,CLASS_NO_TYPE,CLASS_NO,MANAGE_CODE,REG_CODE,MEMBER_CLASS,SPECIES_KEY,SEPARATE_SHELF_CODE,REG_NO,TITLE,AUTHOR,PUBLISHER,CALL_NO,DELAY_CNT,RESERVE_CANCEL_DATE,DELAY_RETURN_DATE,PRE_RETURN_PLAN_DATE,SELF_RETURN_STATION,SELF_RETURN_DOOR,SELF_LOAN_STATION,LOAN_MANAGE_CODE,RETURN_MANAGE_CODE,VOL_INFO,RECEIPT_LOAN_KEY,RECEIPT_RETURN_KEY,RECEIPT_PRINT_LOAN_YN,RECEIPT_PRINT_RETURN_YN,NIGHT_RESERVATION_LOAN,UNMANNED_RESERVATION_LOAN,RESERVATION_CANCEL_TYPE,PREENGAGE_CANCEL_LIMIT_COUNT,PREENGAGE_CANCEL_LIMIT_PERIOD,USER_CLASS_CODE,USER_POSITION_CODE,REQUEST_LIB_CODE,FIRST_WORK,L_DEVICE,L_WORKER ")
			_T("FROM CO_LOAN_TBL ")
			_T("WHERE REC_KEY=%s;")
			,m_strDecideLSStatus, m_pParent->GetWorkLogMsg(_T("반납(UNDO)"), __WFILE__, __LINE__), strLoanKey);
		TmpDM.AddFrame(strQuery);

		strQuery.Format( _T("DELETE FROM CO_LOAN_TBL WHERE REC_KEY=%s;"), strLoanKey);
		TmpDM.AddFrame(strQuery);

	}
	else {
		strQuery.Format(_T("UPDATE LS_WORK_T01 ")
			            _T("SET RETURN_TYPE_CODE = NULL, RETURN_DATE = NULL, STATUS = '%s', ")
						_T("R_DEVICE=NULL, R_WORKER = NULL, LAST_WORK='%s' ")
						_T("WHERE BOOK_KEY = %s AND REC_KEY = %s;")
						, m_strDecideLSStatus, m_pParent->GetWorkLogMsg(_T("반납(UNDO)"), __WFILE__, __LINE__), strBookKey, strLoanKey);
		TmpDM.AddFrame(strQuery);
	}

	if(_T("MO") == strPublishFormCode || _T("단행") == strPublishFormCode) {
		strTblName = _T("BO_BOOK_TBL");
		if ( m_strDecideLSStatus == _T("0") || m_strDecideLSStatus == _T("2") ) {
			strWorkingStatus      = _T("BOL211O");
			strPrevWorkingStatus  = _T("BOL112N");
		}
		else {
			strWorkingStatus      = _T("BOL411O");
			strPrevWorkingStatus  = _T("BOL112N");				
		}
	}
	else if(_T("SE") == strPublishFormCode || _T("연속") == strPublishFormCode) {
		strTblName = _T("SE_BOOK_TBL");
		if ( m_strDecideLSStatus == _T("0") || m_strDecideLSStatus == _T("2") ) {
			strWorkingStatus      = _T("SEL311O");
			strPrevWorkingStatus  = _T("SEL212N");
		}
		else {
			strWorkingStatus      = _T("SEL411O");
			strPrevWorkingStatus  = _T("SEL212N");				
		}
	}
	else if(_T("NB") == strPublishFormCode || _T("긴급") == strPublishFormCode)
	{
		strTblName = _T("CO_NON_DB_BOOK_TBL");
		if ( m_strDecideLSStatus == _T("0") || m_strDecideLSStatus == _T("2") ) {
			strWorkingStatus      = _T("BOL211O");
			strPrevWorkingStatus  = _T("BOL112N");
		}
		else {
			strWorkingStatus      = _T("BOL411O");
			strPrevWorkingStatus  = _T("BOL112N");				
		}
	}

	strQuery.Format(_T("UPDATE %s ")
		            _T("SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK = '%s' ")
					_T("WHERE REC_KEY = %s;"), strTblName, strWorkingStatus, strPrevWorkingStatus, m_pParent->GetWorkLogMsg( _T("반납(UNDO)"), __WFILE__, __LINE__ ), strBookKey);
	TmpDM.AddFrame(strQuery);


	if ( strStatus == _T("R") ) {
		if ( m_bForceIllReturn == TRUE ) {
			strQuery.Format(_T("DELETE FROM ILL_TRANS_TBL WHERE BOOK_KEY = %s AND LOAN_KEY = %s;"),strBookKey, strLoanKey);
			TmpDM.AddFrame(strQuery);
		}
		else {
			strQuery.Format(_T("SELECT RETURN_PLAN_DATE FROM LS_WORK_T01 WHERE BOOK_KEY = '%s' AND STATUS = 'R' AND REC_KEY = %s"), strBookKey, strLoanKey);
			pDM->GetOneValueQuery(strQuery, strReturnPlanDate);

			strQuery.Format(_T("UPDATE ILL_TRANS_TBL ")
				            _T("SET RETURN_LIBCODE = NULL, RETURN_PLAN_DATE = '%s', LS_RETURN_DATE = NULL, LAST_WORK = '%s' ")
							_T(", STATE='0014' ")
							_T("WHERE BOOK_KEY = %s AND LOAN_KEY = %s;")
							, strReturnPlanDate, m_pParent->GetWorkLogMsg( _T("반납(UNDO)"), __WFILE__, __LINE__ ), strBookKey, strLoanKey);
			TmpDM.AddFrame(strQuery);
		}
	}


	ids = TmpDM.SendFrame();
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::ProcReplace()") );	
	TmpDM.EndFrame();	

	CStringArray asFrames;
	if(_T("Y") == m_pLona->m_sIsUnityLoanService) {
		CString strGetData;
		GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strGetData, 0);
		if(_T("Y") == strGetData)
		{
			CString strLoanLibCode;
			
			strLoanLibCode = pDM->GetCellData( _T("BOOK_LIBCODE"), nSelectedRow );			

			if(strLoanLibCode == m_pInfo->LIB_CODE)
			{
				CString strUserNo;
				GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strUserNo, 0);
				
				CFrameWriter Writer(m_pInfo);
				Writer.setCommand(_T("IL_K22_LOC_B01_SERVICE"));
				Writer.addRecord();
				Writer.addElement(_T("대출도서관부호"), strLoanLibCode);
				CString strGpinHash;
				GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("G-PINHASH인증코드"), strGpinHash, 0);
				Writer.addElement(_T("G-PINHASH인증코드"), strGpinHash);
				Writer.addElement(_T("통합대출자번호"), strUserNo);
				Writer.addElement(_T("상태"), m_strDecideLSStatus);				
				Writer.addElement(_T("로컬KEY"), strBookKey);
				CString strFrame = Writer.getFrame();
				asFrames.Add(strFrame);
			}
		}

		if(0 < asFrames.GetSize()) {
			CFrameSender Sender(m_pInfo);
			Sender.SendTempTbl(asFrames, m_pParent->GetWorkLogMsg(_T("반납(UNDO)"), __WFILE__, __LINE__ ));
		}	
	}
	
	return 0;
}


INT CBO_LOC_LOAN_HIS::AddIdxQueryFrame( CESL_DataMgr *pDM, INT type, CString sSpeciesKey )
{
	EFS_BEGIN

	if( (m_pParent->m_pInfo->MODE == 10000) || (m_pParent->m_pInfo->MODE == 30000) )
		return 0;

	CString sIDXTblName, sProcName;
	switch( type )
	{
		case 1 :
			sIDXTblName = _T("IDX_BO_TBL");			
			sProcName = _T("TESTWORKSTATUS");
			break;
		case 2 :
			sIDXTblName = _T("IDX_SE_TBL");			
			sProcName = _T("TESTSEWORKSTATUS");
			break;
		default :
			return -1;
			break;
	}
	

	CString sQuery;
	sQuery.Format( _T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;") ,sIDXTblName, sProcName , sSpeciesKey , sSpeciesKey);	
	pDM->AddFrame( sQuery );

	return 0;

	EFS_END

	return -1;
}

void CBO_LOC_LOAN_HIS::End(UINT uResult)
{
	EndDialog(uResult);
}


void CBO_LOC_LOAN_HIS::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		EndDialog(IDCANCEL);		
	}
	CDialog::OnTimer(nIDEvent);
}


INT CBO_LOC_LOAN_HIS::ILL_RESTORE(CString sCLRecKey, CString strBookKey, CString strState, CString& strDecideLSStatus)
{
	INT ids = 0;
	CString strManageCode = m_pInfo->MANAGE_CODE;
	CString strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	strQuery.Format(_T("SELECT (SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = BB.MANAGE_CODE), IT.APL_LIBCODE, IT.RETURN_LIBCODE FROM ILL_TRANS_TBL IT, BO_BOOK_TBL BB WHERE IT.BOOK_KEY = BB.REC_KEY AND ((IT.STATE='0014' AND BB.WORKING_STATUS = 'BOL411O') OR (IT.STATE='0009' AND BB.WORKING_STATUS = 'BOL112N')) AND BB.REC_KEY = %s AND IT.LOAN_KEY = %s"),strBookKey,sCLRecKey);
	TmpDM.RestructDataManager(strQuery);
	
	
	CString strValue[3] = 
	{
		_T(""), _T(""), _T("")
	};

	for(int i=0;i<TmpDM.GetColCount();i++)
	{
		ids = TmpDM.GetCellData(0,i,strValue[i]);
		if(ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::ILL_RESTORE()") );
	}
	
	TmpDM.FreeData();
	TmpDM.StartFrame();
	if(strState.Compare(_T("R"))==0 && (strValue[0].Compare(strValue[1])==0 || strValue[1] == _T("") ) && strValue[0].Compare(strValue[2])!=0)
	{
		strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_TYPE_CODE = NULL, RETURN_DATE = NULL, STATUS = '0', R_DEVICE=NULL, R_WORKER = NULL WHERE BOOK_KEY = %s AND STATUS = 'R' AND REC_KEY = %s;"),strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);

		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOL211O', PREV_WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;"),strBookKey);
		TmpDM.AddFrame(strQuery);

		strQuery.Format(_T("DELETE FROM ILL_TRANS_TBL WHERE BOOK_KEY = %s AND STATE = '0014' AND LOAN_KEY = %s;"),strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);

		strDecideLSStatus = _T("0");
	}
	else if(strState.Compare(_T("R"))==0 && strValue[0].Compare(strValue[1])!=0 && strValue[0].Compare(strValue[2])!=0 )
	{
		strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_TYPE_CODE = NULL, RETURN_DATE = NULL, STATUS = 'L', R_DEVICE=NULL, R_WORKER = NULL WHERE BOOK_KEY = %s AND STATUS = 'R' AND REC_KEY = %s;"),strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);

		CString strReturnPlanDate;
		strQuery.Format(_T("SELECT RETURN_PLAN_DATE FROM LS_WORK_T01 WHERE BOOK_KEY = '%s' AND STATUS = 'R' AND REC_KEY = %s"),strBookKey,sCLRecKey);
		TmpDM.GetOneValueQuery(strQuery,strReturnPlanDate);
		strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET RETURN_LIBCODE = NULL, RETURN_PLAN_DATE = '%s', LS_RETURN_DATE = NULL, LAST_WORK = '%s' WHERE BOOK_KEY = %s AND STATE = '0014' AND LOAN_KEY = %s;"),strReturnPlanDate,m_pParent->GetWorkLogMsg( _T("책두레자료 복원(UNDO)"), __WFILE__, __LINE__ ),strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);

		strDecideLSStatus = _T("L");		
	}

	else if( (strState.Compare(_T("R"))==0 || strState.Compare(_T("R"))==0)&& strValue[0].Compare(strValue[1])!=0 && strValue[0].Compare(strValue[2])==0)
	{
		strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_TYPE_CODE = NULL, RETURN_DATE = NULL, STATUS = 'L', R_DEVICE=NULL, R_WORKER = NULL WHERE BOOK_KEY = %s AND STATUS = 'R' AND REC_KEY = %s;"),strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);
		
		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOL411O', PREV_WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;"),strBookKey);
		TmpDM.AddFrame(strQuery);

		CString strReturnPlanDate;
		strQuery.Format(_T("SELECT RETURN_PLAN_DATE FROM LS_WORK_T01 WHERE BOOK_KEY = '%s' AND STATUS = 'R' AND REC_KEY = %s"),strBookKey,sCLRecKey);
		TmpDM.GetOneValueQuery(strQuery,strReturnPlanDate);
		
		strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET STATE = '0014', RETURN_LIBCODE = NULL, END_DATE = NULL, END_USERID = NULL, END_DIV = NULL, LAST_WORK = '%s', LS_RETURN_DATE = NULL, RETURN_PLAN_DATE = '%s' WHERE BOOK_KEY = %s AND STATE = '0009' AND LOAN_KEY = %s;"),m_pParent->GetWorkLogMsg( _T("책두레자료 복원(UNDO)"), __WFILE__, __LINE__ ),strReturnPlanDate,strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);

		strDecideLSStatus = _T("L");
	}
	else
	{
		AfxMessageBox(_T("해당 자료는 복귀 대상 자료가 아닙니다."));
		return -1;			
	}

	ids = TmpDM.SendFrame();
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::ILL_RESTORE()") );

	TmpDM.EndFrame();

	ShowLoanHistory();
	m_pParent->SendMessage(WM_USER+150,0,0);
	

	return ids;
}

INT CBO_LOC_LOAN_HIS::ILL_RESTORE_DELAY(CString sCLRecKey, CString strBookKey, CString strState, CString& strDecideLSStatus)
{

	INT ids = 0;

	CString strQuery;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	strQuery.Format(_T("SELECT (SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = BB.MANAGE_CODE), IT.APL_LIBCODE, IT.RETURN_LIBCODE FROM ILL_TRANS_TBL IT, BO_BOOK_TBL BB WHERE IT.BOOK_KEY = BB.REC_KEY AND ((IT.STATE='0014' AND BB.WORKING_STATUS = 'BOL411O') OR (IT.STATE='0009' AND BB.WORKING_STATUS = 'BOL112N')) AND BB.REC_KEY = %s AND IT.LOAN_KEY = '%s'"),strBookKey,sCLRecKey);
	TmpDM.RestructDataManager(strQuery);
	
	
	CString strValue[3] = 
	{
		_T(""), _T(""), _T("")	
	};
	
	for(int i=0;i<TmpDM.GetColCount();i++)
	{
		ids = TmpDM.GetCellData(0,i,strValue[i]);
		if(ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::ILL_RESTORE()") );
	}

	TmpDM.StartFrame();
	if(strState.Compare(_T("R"))==0 && (strValue[0].Compare(strValue[1])==0 || strValue[1] == _T("") ) && strValue[0].Compare(strValue[2])!=0)
	{
		strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_TYPE_CODE = NULL, RETURN_DATE = NULL, STATUS = '2', R_DEVICE=NULL, R_WORKER = NULL WHERE BOOK_KEY = %s AND STATUS = 'R' AND REC_KEY = %s;"),strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);

		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOL211O', PREV_WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;"),strBookKey);
		TmpDM.AddFrame(strQuery);

		strQuery.Format(_T("DELETE FROM ILL_TRANS_TBL WHERE BOOK_KEY = %s AND STATE = '0014' AND LOAN_KEY = %s;"),strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);

		strDecideLSStatus = _T("2");
	}
	else if(strState.Compare(_T("R"))==0 && strValue[0].Compare(strValue[1])!=0 && strValue[0].Compare(strValue[2])!=0 )
	{
		strQuery.Format(_T("UPDATE LS_WORK_T01 SET STATUS='D', RETURN_DATE=NULL, R_WORKER=NULL, R_DEVICE=NULL, RETURN_TYPE_CODE=NULL WHERE BOOK_KEY = '%s' AND STATUS='R' AND USER_KEY = (SELECT USER_KEY FROM ILL_TRANS_TBL WHERE BOOK_KEY = '%s' AND STATE = '0014' AND RETURN_LIBCODE IS NOT NULL) AND REC_KEY = %s;"),strBookKey,strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);

		CString strReturnPlanDate;
		strQuery.Format(_T("SELECT RETURN_PLAN_DATE FROM LS_WORK_T01 WHERE BOOK_KEY = '%s' AND STATUS = 'R' AND REC_KEY = %s"),strBookKey,sCLRecKey);
		TmpDM.GetOneValueQuery(strQuery,strReturnPlanDate);

		strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET RETURN_LIBCODE=NULL, LS_RETURN_DATE=NULL, RETURN_PLAN_DATE = '%s', LAST_WORK = '%s' WHERE BOOK_KEY = '%s' AND STATE = '0014' AND LOAN_KEY = %s;"),strReturnPlanDate,m_pParent->GetWorkLogMsg( _T("책두레자료 복원(UNDO)"), __WFILE__, __LINE__ ),strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);

		strDecideLSStatus = _T("D");
	}
	else if( (strState.Compare(_T("R"))==0 || strState.Compare(_T("R"))==0)&& strValue[0].Compare(strValue[1])!=0 && strValue[0].Compare(strValue[2])==0)
	{
		strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_TYPE_CODE = NULL, RETURN_DATE = NULL, STATUS = 'D', R_DEVICE=NULL, R_WORKER = NULL WHERE BOOK_KEY = %s AND STATUS = 'R' AND USER_KEY = (SELECT USER_KEY FROM ILL_TRANS_TBL WHERE BOOK_KEY = '%s' AND STATE = '0009' AND RETURN_LIBCODE IS NOT NULL) AND REC_KEY = %s;"),strBookKey,strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);
		
		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOL411O', PREV_WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;"),strBookKey);
		TmpDM.AddFrame(strQuery);

		CString strReturnPlanDate;
		strQuery.Format(_T("SELECT RETURN_PLAN_DATE FROM LS_WORK_T01 WHERE BOOK_KEY = '%s' AND STATUS = 'R' AND REC_KEY = %s"),strBookKey,sCLRecKey);
		TmpDM.GetOneValueQuery(strQuery,strReturnPlanDate);

		strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET STATE = '0014', RETURN_PLAN_DATE = '%s', RETURN_LIBCODE = NULL, END_DATE = NULL, END_USERID = NULL, END_DIV = NULL, LAST_WORK = '%s', LS_RETURN_DATE = NULL WHERE BOOK_KEY = '%s' AND STATE = '0009' AND LOAN_KEY = %s;"),strReturnPlanDate,m_pParent->GetWorkLogMsg( _T("책두레자료 복원(UNDO)"), __WFILE__, __LINE__ ),strBookKey,sCLRecKey);
		TmpDM.AddFrame(strQuery);


		strDecideLSStatus = _T("D");
	}
	else
	{
		AfxMessageBox(_T("해당 자료는 복귀 대상 자료가 아닙니다."));
		return -1;
	}
	

	ids = TmpDM.SendFrame();
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_LOAN_HIS::ILL_RESTORE_DELAY()") );

	TmpDM.EndFrame();

	ShowLoanHistory();
	m_pParent->SendMessage(WM_USER+150,0,0);
	
	return ids;
}