


#include "stdafx.h"
#include "ILL_SEARCH_USER_LIST.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif








CILL_SEARCH_USER_LIST::CILL_SEARCH_USER_LIST(CESL_Mgr* pParent /*=NULL*/,CString sWhere)
	: CESL_Mgr(IDD, pParent)
{
	
		
	
	m_nCloseState = -1;
	m_nInit = 0;
	m_sWhere = sWhere;
	m_bIsSearhed = FALSE;
	m_bInEnter = FALSE;
	m_pLoanMangeValue = NULL;
}


CILL_SEARCH_USER_LIST::~CILL_SEARCH_USER_LIST()
{
	
}

VOID CILL_SEARCH_USER_LIST::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	
		
	

	EFS_END
}


BEGIN_MESSAGE_MAP(CILL_SEARCH_USER_LIST, CDialog)
	//{{AFX_MSG_MAP(CILL_SEARCH_USER_LIST)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnUSER_SELECT, OnbtnUSERSELECT)
	ON_BN_CLICKED(IDC_btnCLOSE_USER_SELECT, OnbtnCLOSEUSERSELECT)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CILL_SEARCH_USER_LIST::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	


	if(InitESL_Mgr(_T("ILL_SEARCH_USER_LIST")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return -1;
	}
	
	
	{
	int ids = 0;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(
			_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE VALUE_NAME = '책두레최대신청가능책수'"));
	
	CString strValue;
	ids = TmpDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids)
	{
		AfxMessageBox(_T("책두레최대신청가능책수 옵션을 찾을 수 없습니다."));
		PostMessage(WM_CLOSE);
 		return FALSE;
	}
	
	m_strMaxAplCnt = strValue;	
	}
	

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabUSERTITLE);
	pTab->InsertItem( 0 , _T("이용자 목록") );

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_MAINGRID_USER );
	pGrid->SetSort(TRUE);

	SetWhereSql(m_sWhere);

	RunInitProcess();	

	CRect pRect;		 

	 this->GetParent()->GetWindowRect(&pRect); 
	 

	 INT y = pRect.top + (pRect.bottom - pRect.top)/2;
	 INT x = pRect.left + (pRect.right - pRect.left)/2 ;

	SetCursorPos(x, y);

	
	
	
	CString sCMAlias = _T("CM_ILL_SEARCH_USER_LIST");
	CString sGridAlias = _T("그리드_검색된이용자");
	
	CESL_Grid *pUserGrid = (CESL_Grid*) FindControl( sCMAlias, sGridAlias );
	
	
	if( !m_pInfo->m_bUserInfoExportYN )
	{
		pUserGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		
		pUserGrid->m_strPrintInfo = _T("책두레 신청 - 이용자 검색");
	} 


	return TRUE;  
	              

	EFS_END
	return FALSE;

}



INT CILL_SEARCH_USER_LIST::SetWhereSql( CString sWhere )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_ILL_SEARCH_USER_LIST");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CILL_SEARCH_USER_LIST::SetWhereSql( CString &sWhere )") );

	ids = CLocCommonAPI::RefreshDataManager( this , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CILL_SEARCH_USER_LIST::SetWhereSql( CString &sWhere )") );

	if( pDM->GetRowCount() ==  1 )
		m_nCloseState = 0;

	CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8,asDes9,asDes10;

	
	CString strValue1;
	ids = m_pLoanMangeValue->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("SetWhereSql"));
	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();
	CString strUserNo, sStatus;
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		pDM->GetCellData( _T("대출자번호"), i , strUserNo );
		if(_T("Y") != strValue1) CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_pLoanMangeValue->m_sIsUnityLoanService );
		pDM->SetCellData( _T("대출자번호_설명"), strUserNo , i );

		pDM->GetCellData(_T("불량회원구분") , i, sStatus);
		if(_T("0") == sStatus)
			sStatus = _T("정상회원");
		else if(_T("1") == sStatus)
			sStatus = _T("대출정지회원");
		else if(_T("2") == sStatus)
			sStatus = _T("제적회원");
		else if(_T("3") == sStatus)
			sStatus = _T("탈퇴회원");
		else 
			sStatus = _T("미정의된 회원구분");
		pDM->SetCellData(_T("불량회원구분_설명"), sStatus ,i);

		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			
			CString strGetData;
			pDM->GetCellData(_T("자택주소"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("근무지주소"), i, strGetData);
			asDes2.Add(strGetData);
			pDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes3.Add(strGetData);
			pDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes4.Add(strGetData);
			pDM->GetCellData(_T("휴대폰"), i, strGetData);
			asDes5.Add(strGetData);
			pDM->GetCellData(_T("비밀번호"), i, strGetData);
			asDes6.Add(strGetData);
			pDM->GetCellData(_T("자택전화"), i, strGetData);
			asDes7.Add(strGetData);
			pDM->GetCellData(_T("근무지전화"), i, strGetData);
			asDes8.Add(strGetData);
			pDM->GetCellData(_T("IDX_핸드폰"), i, strGetData);
			asDes9.Add(strGetData);
			pDM->GetCellData(_T("회원증비밀번호"), i, strGetData);
			asDes10.Add(strGetData);
		}


	}


	
	ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("대출소속정보") , _T("대출소속정보") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("CILL_SEARCH_USER_LIST::SetWhereSql( CString &sWhere )") );
	
	return 0;

	EFS_END
	return -1;
}


INT CILL_SEARCH_USER_LIST::GetCount()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CILL_SEARCH_USER_LIST::GetCount()") );

	ids = pDM->GetRowCount();
	
	return ids;

	EFS_END
	return -1;
}



INT CILL_SEARCH_USER_LIST::GetSelectUserKey( CString &sUserRecKey )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CILL_SEARCH_USER_LIST::GetSelectUserKey( CString &sUserRecKey )") );

	
	if( m_nCloseState > -1 )
	{

		CESL_DataMgr TmpDM;
		CLocCommonAPI::MakeCopyDM( this , pDM , &TmpDM );
		
		ids = CLocCommonAPI::AddDM( pDM ,&TmpDM , m_nCloseState , -1 , this);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CILL_SEARCH_USER_LIST::GetSelectUserKey( CString &sUserRecKey )") );

		pDM->FreeData();

		ids = CLocCommonAPI::AddDM( &TmpDM , pDM , 0 , -1 , this  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CILL_SEARCH_USER_LIST::GetSelectUserKey( CString &sUserRecKey )") );

		return 0;
	}
	
	else
	{
		pDM->FreeData();
		sUserRecKey.Empty();
		return 1;
	}

	return 0;

	EFS_END
	return -1;
}




BEGIN_EVENTSINK_MAP(CILL_SEARCH_USER_LIST, CDialog)
    
	ON_EVENT(CILL_SEARCH_USER_LIST, IDC_MAINGRID_USER, 69 /* SelChange */, OnSelChangeMaingridUser, VTS_NONE)
	ON_EVENT(CILL_SEARCH_USER_LIST, IDC_MAINGRID_USER, -601 /* DblClick */, OnDblClickMaingridUser, VTS_NONE)
	ON_EVENT(CILL_SEARCH_USER_LIST, IDC_MAINGRID_USER, -605 /* MouseDown */, OnMouseDownMaingridUser, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	
END_EVENTSINK_MAP()

BOOL CILL_SEARCH_USER_LIST::PreTranslateMessage(MSG* pMsg) 
{	
	if ( VK_ESCAPE==pMsg->wParam )
		PostMessage( WM_CLOSE, NULL, NULL );
	else if ( (WM_KEYDOWN==pMsg->message) && (VK_RETURN==pMsg->wParam) )
		OnbtnUSERSELECT( );
	else if ( pMsg->message == WM_LBUTTONDBLCLK )
	{
	
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

VOID CILL_SEARCH_USER_LIST::OnSelChangeMaingridUser() 
{
	EFS_BEGIN

	INT ids = 0;
	INT nCloseState = -1;

	CString sCMAlias = _T("CM_ILL_SEARCH_USER_LIST");
	CString sGridAlias = _T("그리드_검색된이용자");

	CESL_Grid *pGrid = (CESL_Grid*) FindControl( sCMAlias, sGridAlias );
	if( pGrid == NULL ) return ;

	nCloseState = pGrid->GetIdx( );
	if ( (nCloseState<0) || (m_nCloseState==nCloseState) )
		return;

	ids = SetReverse(pGrid, nCloseState);
	if ( ids < 0 ) return;

	EFS_END
}

VOID CILL_SEARCH_USER_LIST::OnDblClickMaingridUser() 
{
	EFS_BEGIN

	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_ILL_SEARCH_USER_LIST") , _T("그리드_검색된이용자") );
	if( pGrid_MainGrid_ALL == NULL ) return;
	
	INT i = pGrid_MainGrid_ALL->GetRowSel();
	INT j = pGrid_MainGrid_ALL->GetMouseRow();
	CPoint k = pGrid_MainGrid_ALL->GetCaretPos();
	INT l = pGrid_MainGrid_ALL->GetRow();
	INT m = pGrid_MainGrid_ALL->GridGetRow();
	INT n = pGrid_MainGrid_ALL->GetMousePointer();

	if( j < 1 ) return;
		
	OnbtnUSERSELECT( );

	EFS_END
}

INT CILL_SEARCH_USER_LIST::SetReverse(CESL_Grid *pGrid, INT nIndex)
{
	EFS_BEGIN

	m_nCloseState = nIndex;

	return pGrid->SetReverse( m_nCloseState );

	EFS_END
	return -1;
}

INT CILL_SEARCH_USER_LIST::ViewGrid()
{	

	INT ids = 0;
	CString strFuncName = _T("CILL_SEARCH_USER_LIST::ViewGrid()");


	CString sCMAlias = _T("CM_ILL_SEARCH_USER_LIST");
	CString sGridAlias = _T("그리드_검색된이용자");

	CESL_Grid* pGrid = ( CESL_Grid* ) FindControl( sCMAlias, sGridAlias );
	if ( NULL==pGrid ) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CESL_DataMgr* pDM = FindDM(_T("DM_ILL_SEARCH_USER_LIST"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

	BOOL bError = FALSE;
	const int ROW_COUNT = pDM->GetRowCount();
	for(int nRow = 0 ; nRow < ROW_COUNT ; nRow++ )
	{
		CString sCode;
		ids = pDM->GetCellData(_T("대출직급정보_코드"), nRow, sCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

		CString strDesc;
		ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출직급정보"), sCode, strDesc);
		if(0 > ids)
		{
			bError = TRUE;
			strDesc = sCode;
		}
		
		ids = pDM->SetCellData(_T("대출직급정보_설명"), strDesc, nRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);
	}

	if(TRUE == bError)
	{
		AfxMessageBox(_T("한 명이상의 이용자가 존재하지 않는 직급코드를 가지고 있습니다."));
	}








	
	
	
	CString strValue1;
	
	
	
	

	
	
		ids = this->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("CILL_SEARCH_USER_LIST::ViewGrid()") );

		strValue1.TrimLeft();strValue1.TrimRight();
		strValue1.MakeUpper();

























	
	

 	ids = AllControlDisplay( sCMAlias );
 	if(ids > 0) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CILL_SEARCH_USER_LIST::ViewGrid()") );
	
	ids = SetReverse(pGrid, 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CILL_SEARCH_USER_LIST::ViewGrid()") );
	return 0;
}

VOID CILL_SEARCH_USER_LIST::OnClose() 
{	
	EFS_BEGIN

	OnbtnCLOSEUSERSELECT();

	EFS_END
}

VOID CILL_SEARCH_USER_LIST::OnbtnUSERSELECT() 
{

	int ids = 0;
	CString strFuncName = _T("CILL_SEARCH_USER_LIST::OnbtnUSERSELECT()");
	if ( m_nCloseState < 0 )
	{
		AfxMessageBox( _T("대출자를 먼저 선택해주십시오-OnbtnUSERSELECT") );
		return ;
	}
	else
	{
		CESL_DataMgr* pDM = FindDM(_T("DM_ILL_SEARCH_USER_LIST"));
		if(NULL == pDM) ERROR_MSG_RETURN_VOID2(-1, strFuncName);
		
		
		
		CString strGet;
		ids = pDM->GetCellData(_T("불량회원구분"), m_nCloseState, strGet);
		if(0 > ids) ERROR_MSG_RETURN_VOID2(-3, strFuncName);
		
		CString sCodeData;
		if( (strGet.Compare(_T("0"))==0) || (strGet.Compare(_T("정상"))==0) || (sCodeData.Compare(_T("정상회원"))==0))
			sCodeData = _T("정상회원");
		else if( (strGet.Compare(_T("1"))==0) || (strGet.Compare(_T("대출정지"))==0))
			sCodeData = _T("대출정지");
		else if( (strGet.Compare(_T("2"))==0) || (strGet.Compare(_T("제적"))==0) || (sCodeData.Compare(_T("제적회원"))==0))
			sCodeData = _T("제적회원");
		else if( (strGet.Compare(_T("3"))==0) || (strGet.Compare(_T("탈퇴"))==0) || (sCodeData.Compare(_T("탈퇴회원"))==0))
			sCodeData = _T("탈퇴회원");
		else
			sCodeData = _T("미정의된 회원구분");
		if(_T("정상회원") != sCodeData)
		{
			CString strMessage;
			strMessage.Format(
				_T("선택된 회원은 [%s]이기 때문에 신청할 수 없습니다."),
				sCodeData);
			AfxMessageBox(strMessage);
			
			return ;
		}

		CString strMemberClass;
		ids = pDM->GetCellData(_T("회원구분"), m_nCloseState, strMemberClass);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_VOID2(-1000, strFuncName);
		}
		if(0 != _ttoi(strMemberClass))
		{
			AfxMessageBox(_T("선택된 회원은 정회원이 아니기 때문에 신청할 수 없습니다."));
			return;
		}
	}
	

	CESL_DataMgr* pDM = FindDM(_T("DM_ILL_SEARCH_USER_LIST"));
	if(NULL == pDM) ERROR_MSG_RETURN_VOID2(-4, strFuncName);
	
	CString strUserKey;
	pDM->GetCellData(_T("REC_KEY"), m_nCloseState, strUserKey);
	if(NULL == pDM) ERROR_MSG_RETURN_VOID2(-5, strFuncName);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	CString strQuery;

	
	
	CString strAplStopDate;
	strQuery.Format(
		_T("SELECT TO_CHAR(ILL_APL_STOP_DATE, 'YYYY/MM/DD') ")
		_T("FROM   CO_LOAN_USER_TBL ")
		_T("WHERE  REC_KEY = %s AND ")
			_T("TO_CHAR(ILL_APL_STOP_DATE, 'YYYY/MM/DD') >= TO_CHAR(SYSDATE, 'YYYY/MM/DD')")
		, strUserKey);
	ids = TmpDM.RestructDataManager(strQuery);	
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-6, strFuncName);

	if(0 < TmpDM.GetRowCount())	
	{
		ids = TmpDM.GetCellData(0, 0, strAplStopDate);
		if(0 > ids) ERROR_MSG_RETURN_VOID2(-7, strFuncName);

		CString strMsg;
		strMsg.Format(
			_T("책두레 신청제한이 걸린 이용자입니다.\r\n책두레신청제한일[%s]"),
			strAplStopDate);
		AfxMessageBox(strMsg);

		return;
	}
	
	

	
	if(_T("Y") == m_strUnityLoanCntYN)
	{
		
		
		
		
		
		
		
	}
	
	else
	{
		CString strOtherLoanCnt;
		
		strQuery.Format(
		 
			_T("SELECT COUNT(1) AS CNT ")
			_T("FROM LS_WORK_T01 ")
			_T("WHERE USER_KEY = %s AND STATUS IN ('0', '2') AND MANAGE_CODE <> ")
			_T("( SELECT MANAGE_CODE FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s) ")
			, strUserKey, strUserKey);
		ids = TmpDM.GetOneValueQuery(strQuery, strOtherLoanCnt);
		if(0 > ids) ERROR_MSG_RETURN_VOID2(-8, strFuncName);

		CString strILLCnt;
		strQuery.Format(
			
		_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE ")
		_T("USER_NO = (SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s) ")
		_T("AND END_DIV IS NULL AND RETURN_LIBCODE IS NULL")
		, strUserKey);
		ids = TmpDM.GetOneValueQuery(strQuery, strILLCnt);
		if(0 > ids) ERROR_MSG_RETURN_VOID2(-9, strFuncName);

		CString strUserClass;
		pDM->GetCellData(_T("대출직급정보_코드"), m_nCloseState, strUserClass);
		if(0 > ids) ERROR_MSG_RETURN_VOID2(-10, strFuncName);

		
		CString strMaxOtherLoanCnt;
		strQuery.Format(
		_T("SELECT OTHER_LOAN_BOOK_CNT ")
		_T("FROM CD_CODE_TBL ")
		_T("WHERE CLASS = 31 AND CODE = '%s'"), strUserClass);

		ids = TmpDM.GetOneValueQuery(strQuery, strMaxOtherLoanCnt);
		if(0 > ids)
		{
			AfxMessageBox(
				_T("해당 이용자의 타관대출가능책수를 구해오지 못했습니다.\r\n")
				_T("이용자의 직급이 올바르지 않거나 해당 직급에 대해 타관대출가능책수가 설정되어 있지 않습니다.")
				);
			return;

		}

		
		CString strMaxAplCnt = m_strMaxAplCnt;

		
		strMaxAplCnt = (_ttoi(strMaxOtherLoanCnt)>_ttoi(strMaxAplCnt))? strMaxAplCnt : strMaxOtherLoanCnt;

		BOOL bIsApl = TRUE;

		
		int nIsAplCnt = _ttoi(strMaxAplCnt) - _ttoi(strILLCnt);
		if(0 >= nIsAplCnt) { bIsApl = FALSE; }

	
	
	
	
	
	
	
		
		if(FALSE == bIsApl)
		{
			AfxMessageBox(_T("이용자의 신청한도가 넘어 신청할 수 없습니다."));
			return;
		}
	}
	OnCancel();
}

VOID CILL_SEARCH_USER_LIST::OnbtnCLOSEUSERSELECT() 
{
	EFS_BEGIN

	m_nCloseState = -1;
	OnCancel();

	EFS_END
}

VOID CILL_SEARCH_USER_LIST::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN

	if( nIDEvent == 0 )
	{
		KillTimer(0);
		if( GetSafeHwnd() == NULL ) 
		{
			SetTimer(0,1,NULL);
			return;
		}
		RunInitProcess();
		return;
	}
	
	CDialog::OnTimer(nIDEvent);

	EFS_END
}


INT CILL_SEARCH_USER_LIST::RunInitProcess()
{
	EFS_BEGIN

	INT ids = ViewGrid();
	if( ids ){
		AfxMessageBox( _T("RunInitProcess ") );
		return -1;
	}

	m_bIsSearhed = TRUE;

	return 0;

	EFS_END
	return -1;
}


INT CILL_SEARCH_USER_LIST::SortGridData(INT nColIndex)
{
	EFS_BEGIN


	return 0;

	EFS_END
	return -1;
}

VOID CILL_SEARCH_USER_LIST::OnMouseDownMaingridUser(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	

	EFS_END	
}

HBRUSH CILL_SEARCH_USER_LIST::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}



VOID CILL_SEARCH_USER_LIST::OnClickedCaptionButton(BOOL KOLAS1004_SECURITY_STATE)
{
EFS_BEGIN

EFS_END
}

