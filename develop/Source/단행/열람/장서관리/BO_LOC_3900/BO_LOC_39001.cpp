// BO_LOC_39001.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_39001.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define QUERY_UNIT	3

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3900 dialog


CBO_LOC_3900::CBO_LOC_3900(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3900)
	m_nStatus = -1;	
	m_nEditStatus = -1;
	// KOL.UDF.022
	m_pGrid = NULL;
	m_pCM = NULL;
	m_pDM = NULL;
	//}}AFX_DATA_INIT
}
// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3900::~CBO_LOC_3900()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3900::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3900)
	DDX_Control(pDX, IDC_TAB_ILL, m_tabData);
	DDX_Radio(pDX, IDC_RDO_PLAN, m_nStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3900, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3900)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnRESULT_SEARCH, OnbtnSEARCH)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ILL, OnSelchangeTabIll)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3900 message handlers

BOOL CBO_LOC_3900::Create(CWnd* pParentWnd) 
{	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3900::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// SM Initialize
	if( InitESL_Mgr( _T("BO_LOC_3900") ) < 0 )
	{
		ESLAfxMessageBox( _T("InitESL_Mgr Error!") );
		return FALSE;
	}

	// DM Initialize
	m_pDM = FindDM( _T("DM_BO_LOC_3900") );	
	if( m_pDM == NULL )
	{
		ESLAfxMessageBox( _T("DM_BO_LOC_3900 Error!") );
		return FALSE;
	}

	// CM Initialize
	m_pCM = FindCM( _T("CM_BO_LOC_3900") );
	// Grid Initialize
	m_pGrid = (CESL_Grid*) m_pCM->FindControl( _T("MainGrid") );

	if( m_pCM == NULL  || m_pGrid == NULL )
	{
		ESLAfxMessageBox( _T("CM_BO_LOC_3900 Error!") );
		return FALSE;
	}

	// Connection Info Setting
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	// Tab Control Initialize
	m_tabData.InsertItem(0, _T("접수"));
	m_tabData.InsertItem(1, _T("발송"));
	m_tabData.InsertItem(2, _T("완료"));
	m_tabData.SetCurSel(0);

	// Radio Control Initialize
	m_nStatus = 0;

	UpdateData(FALSE);

	// AUTO SEARCH
	OnbtnSEARCH();

	return TRUE;

EFS_END
	return FALSE;
}

VOID CBO_LOC_3900::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);	

	INT nPos;

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_ILL);
	if( pTab->GetSafeHwnd() == NULL )	return;
	pTab->MoveWindow(0, 0, cx+1, cy-0);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(2, 60, cx-6, cy-62);	

	CRect rect, rectTmp;
	CButton* pBtn;
	CEdit* pEdt;
	
	nPos = 10;
	
//	CButton* pBtn = (CButton*)GetDlgItem(IDC_STT_DATE);
//	pBtn->GetWindowRect(&rect);
//	pBtn->MoveWindow(nPos, 35, rect.Width(), rect.Height());
//	pBtn->ShowWindow(SW_SHOW);
	
//	nPos += 40 + rectTmp.Width();
	pBtn = (CButton*)GetDlgItem(IDC_RDO_PLAN);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(nPos, 35, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
	
	rectTmp = rect;
	nPos += 10 + rectTmp.Width();
	pBtn = (CButton*)GetDlgItem(IDC_RDO_COMPLETE);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(nPos, 35, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);


	rectTmp = rect;
	nPos += 20 + rectTmp.Width();
	pBtn = (CButton*)GetDlgItem(IDC_CMB_DATETYPE);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(nPos, 32, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
	
	rectTmp = rect;
	nPos += 4 + rectTmp.Width();
	pEdt = (CEdit*)GetDlgItem(IDC_EDT_FROMDATE);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow( nPos, 32, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	rectTmp = rect;
	nPos += 0 + rectTmp.Width();
	pBtn = (CButton*)GetDlgItem(IDC_STT_RANGE);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow( nPos, 35, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
	
	rectTmp = rect;
	nPos += 0 + rectTmp.Width(); 
	pEdt = (CEdit*)GetDlgItem(IDC_EDT_TODATE);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow( nPos, 32, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	
//	rectTmp = rect;
//	nPos += 40+rectTmp.Width();
//	pBtn = (CButton*)GetDlgItem(IDC_STT_STATUS);
//	pBtn->GetWindowRect(&rect);
//	pBtn->MoveWindow( nPos, 35, rect.Width(), rect.Height());
//	pBtn->ShowWindow(SW_SHOW);

	

	rectTmp = rect;
	nPos += 8 + rectTmp.Width();
	pBtn = (CButton*)GetDlgItem(IDC_btnRESULT_SEARCH);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow( nPos, 32, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

EFS_END
}

VOID CBO_LOC_3900::OnbtnSEARCH() 
{
	UpdateData(TRUE);

	m_pDM->RefreshDataManager(MakeSearchQuery());
	
	m_pGrid->Display();	

	m_nEditStatus = m_nStatus;	// 수정 가능할 상태를 저장한다.

	DisplayToolBarButton();

	UpdateData(FALSE);
}

VOID CBO_LOC_3900::OnSelchangeTabIll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// AUTO SEARCH
	OnbtnSEARCH();
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ Desc ] 자료처리
/////////////////////////////////////////////////////////////////////////////
VOID CBO_LOC_3900::CHECKINData()
{
	if( CHECKSelectedData() < 0 )	return;

	if( m_nEditStatus < 0 || m_nEditStatus > 1 )
	{
		ESLAfxMessageBox( _T("검색조건과 결과가 다릅니다.\n재검색 후 처리해 주세요"), MB_ICONINFORMATION );
		return;
	}
	else if( m_nEditStatus != 0 )	return;

	INT nFlag = -1;
	CString strHeadQuery = _T("UPDATE CO_ILL_TBL@KOLAS2ILL SET ");
	
	m_pDM->StartFrame();

	switch( m_tabData.GetCurSel() )
	{
	case 0:	// 접수
		{
			nFlag = 0;

			if( m_nEditStatus == 0 )
			{
				strHeadQuery += _T("ILL_STATUS = '13', RESPOND_DATE = SYSDATE ");
			}
		}
		break;
	case 1:	// 발송
		{
			nFlag = -1;

			if( m_nEditStatus == 0 )
			{
				strHeadQuery += _T("ILL_STATUS = '16', REQUEST_SHIPPED_DATE = SYSDATE ");
			}			
		}
		break;
	case 2:	// 완료
		{
			nFlag = 1;

			if( m_nEditStatus == 0 )
			{
				strHeadQuery += _T("ILL_STATUS = '19', RESPOND_CHECKIN_DATE = SYSDATE ");
			}			
		}
		break;
	default:
		break;
	}

	if( 0 > AddFrameQuery( strHeadQuery, m_pDM, m_pGrid, nFlag ) )
	{
		ESLAfxMessageBox( _T("처리 작업중 오류가 발생했습니다."), MB_ICONWARNING );
	}
	else
	{
		m_pDM->SendFrame();
	}

	m_pDM->EndFrame();

	m_pGrid->Display();
}

/////////////////////////////////////////////////////////////////////////////
// [ Desc ] 취소
/////////////////////////////////////////////////////////////////////////////
VOID CBO_LOC_3900::CANCELData()
{
	if( CHECKSelectedData() < 0 )	return;

	if( m_nEditStatus < 0 || m_nEditStatus > 1 )
	{
		ESLAfxMessageBox( _T("검색조건과 결과가 다릅니다.\n재검색 후 처리해 주세요"), MB_ICONINFORMATION );
		return;
	}
	else if( m_nEditStatus != 1 )	return;

	INT nFlag = -1;
	CString strHeadQuery = _T("UPDATE CO_ILL_TBL@KOLAS2ILL SET ");
	
	m_pDM->StartFrame();

	
	switch( m_tabData.GetCurSel() )
	{
	case 0:	// 접수
		{
			nFlag = 1;

			if ( m_nEditStatus == 1 )
			{
				strHeadQuery += _T("ILL_STATUS = '11', RESPOND_DATE = NULL ");
			}			
		}
		break;
	case 1:	// 발송
		{
			nFlag = -1;

			if ( m_nEditStatus == 1 )
			{
				strHeadQuery += _T("ILL_STATUS = '13', REQUEST_SHIPPED_DATE = NULL ");
			}			
		}
		break;
	case 2:	// 완료
		{
			nFlag = 0;

			if ( m_nEditStatus == 1 )
			{
				strHeadQuery += _T("ILL_STATUS = '18', RESPOND_CHECKIN_DATE = NULL ");
			}			
		}
		break;
	default:
		break;
	}

	if( 0 > AddFrameQuery( strHeadQuery, m_pDM, m_pGrid, nFlag ) )
	{
		ESLAfxMessageBox( _T("처리 작업중 오류가 발생했습니다."), MB_ICONWARNING );
	}
	else
	{
		m_pDM->SendFrame();
	}

	m_pDM->EndFrame();

	m_pGrid->Display();
}

/////////////////////////////////////////////////////////////////////////////
// [ Desc ] 출력
/////////////////////////////////////////////////////////////////////////////
VOID CBO_LOC_3900::PRINTData()
{
	AfxMessageBox( _T("기능 구현중입니다.") , MB_ICONINFORMATION );
}

/////////////////////////////////////////////////////////////////////////////
// [ Desc ] 그리드의 선정데이터 체크
/////////////////////////////////////////////////////////////////////////////
INT CBO_LOC_3900::CHECKSelectedData()
{	
	if( m_pDM->GetRowCount() == 0 )		return -1;
	if( m_pGrid == NULL )				return -1;

	INT ids = m_pGrid->SelectMakeList();
	if( ids < 0 )	return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if( ids < 0 )	return -1;

	return 0;
}

CString CBO_LOC_3900::MakeSearchQuery()
{
	INT nPos;
	CString strLibCode, strTmp, strBuf, strFromDate, strToDate, strDateType, strWhere = _T("");
	CStdioFile file;
		
	// 자관에 신청되는 정보만 검색한다.
	strLibCode = _T("");
	if( file.Open( _T("..\\cfg\\efs\\user_info.log"), CFile::modeRead | CFile::typeBinary) )
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while( file.ReadString( strBuf ) )
		{
			nPos = strBuf.Find( _T("=") );
			if( nPos != -1 )
			{
				strTmp = strBuf.Left( nPos );
				strTmp.TrimLeft();	strTmp.TrimRight();
				if( strTmp == _T("LIB_CODE") )
				{
					strLibCode = strBuf.Mid( nPos+1 );
					strLibCode.TrimLeft();	strLibCode.TrimRight();
				}
			}
		}
		file.Close();
	}

	if( strLibCode.IsEmpty() == FALSE )
	{
		strWhere = _T(" RESPOND_LIB_CODE = '") + strLibCode +_T("' AND ");
	}

	switch( m_tabData.GetCurSel() )
	{
	case 0:	// 접수
		{
			if( m_nStatus == 1 )	strWhere += _T(" CIL.ILL_STATUS = '13' ");
			else					strWhere += _T(" CIL.ILL_STATUS = '11' ");
		}
		break;
	case 1:	// 발송
		{
			if( m_nStatus == 1 )	strWhere += _T(" CIL.ILL_STATUS = '16' ");
			else					strWhere += _T(" CIL.ILL_STATUS = '13' ");
		}
		break;
	case 2:	// 완료
		{
			if( m_nStatus == 1 )	strWhere += _T(" CIL.ILL_STATUS = '19' ");
			else					strWhere += _T(" CIL.ILL_STATUS = '18' ");
		}
		break;
	default:	//strWhere += _T(" CIL.ILL_STATUS IS NULL ");
		break;
	}

	m_pCM->GetControlMgrData( _T("날짜타입") , strDateType );
	m_pCM->GetControlMgrData( _T("시작날짜") , strFromDate );
	m_pCM->GetControlMgrData( _T("끝날짜") , strToDate );
	
	if( strFromDate.IsEmpty() && strToDate.IsEmpty() )
	{
		return strWhere;
	}

	if		( _T("신청일") == strDateType )		strWhere += _T("AND CIL.REQUEST_DATE ");
	else if	( _T("접수일") == strDateType )		strWhere += _T("AND CIL.RESPOND_DATE ");
	else if	( _T("인계일") == strDateType )		strWhere += _T("AND CIL.REQUEST_SHIPPED_DATE ");
	else if	( _T("인수일") == strDateType )		strWhere += _T("AND CIL.REQUEST_INPROCESS_DATE ");
	else if	( _T("복귀일") == strDateType )		strWhere += _T("AND CIL.RESPOND_CHECKIN_DATE ");
	else										return strWhere;

	if		( strFromDate.IsEmpty() == TRUE && strToDate.IsEmpty() == FALSE )
	{
		strWhere += _T("<= TO_DATE('") + strToDate + _T("', 'YYYY/MM/DD HH24:MI:SS')+0.99999");
	}
	else if	( strFromDate.IsEmpty() == FALSE && strToDate.IsEmpty() == TRUE )
	{
		strWhere += _T("BETWEEN TO_DATE('") + strFromDate + _T("',  'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") +strToDate + _T("', 'YYYY/MM/DD HH24:MI:SS')+0.99999");
	}
	else if	( strFromDate.IsEmpty() == FALSE && strToDate.IsEmpty() == FALSE )
	{
		strWhere += _T("BETWEEN TO_DATE('") + strFromDate + _T("',  'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") +strToDate + _T("', 'YYYY/MM/DD HH24:MI:SS')+0.99999");
	}
	else
	{
		strWhere +=  _T("IS NULL");
	}

	return strWhere;
}

INT CBO_LOC_3900::AddFrameQuery( CString strHeadQuery, CESL_DataMgr *pDM, CESL_Grid* pGrid, INT nFlag/*=-1*/)
{
	if( strHeadQuery.IsEmpty() )		return -1;
	if( pDM == NULL || pGrid == NULL )	return -1;

	INT i, ids, nCnt = 0;
	CString strTmp, strRecKey, strWhere = _T(" WHERE REC_KEY IN ( ");
	
	// ++ ADD Query		=================================================================
	INT nMoCnt, nSeCnt;
	CString strMOQuery, strSEQuery, strMOWhere, strSEWhere, strBookKey, strMoSe ;
	nMoCnt = nSeCnt = 0;
	strMOWhere = strSEWhere = strWhere;
	if( nFlag == 0 )		// BOL112N -> BOL211O, SEL212N -> SEL311O
	{
		strSEQuery = _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEL311O' ");
		strMOQuery = _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOL211O' ");
	}
	else if( nFlag == 1 )	// BOL211O -> BOL112N, SEL311O -> SEL212N
	{
		strSEQuery = _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SEL212N' ");
		strMOQuery = _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOL112N' ");
	}	
	// -- END(+)		=================================================================

	
	i = pGrid->SelectGetHeadPosition();
	while( i >= 0 )
	{
		nCnt++;
		// 소장도서관_책키
		ids = pDM->GetCellData(_T("REC_KEY"), i, strRecKey );

		// * 주의 : REC_KEY가 없어서는 안된다.
		if( strRecKey.IsEmpty() == TRUE )	return -1;

		if( nCnt == 1 )		strWhere += strRecKey;
		else				strWhere += _T(", ") + strRecKey;

		// ++ ADD Query(+)	=================================================================
		strBookKey = pDM->GetCellData(_T("소장도서관_책키"), i );
		strMoSe	= pDM->GetCellData(_T("발행구분"), i );
		if( strBookKey.IsEmpty() == FALSE )
		{
			if( nFlag != -1 )		// BOL112N -> BOL211O, SEL212N -> SEL311O
			{
				strMoSe.MakeUpper();
				if( strMoSe == _T("SE") )
				{
					nSeCnt++;
					if( nSeCnt == 1 )	strSEWhere += strBookKey;
					else				strSEWhere += _T(", ") + strBookKey;
					//=====================================================
					//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
					// collection
// 					strTmp.Format(_T("INSERT INTO "
// 									_T("IDX_TMP_SE_TBL( REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 									_T("VALUES( ESL_SEQ.NEXTVAL, (SELECT DISTINCT SPECIES_KEY FROM SE_BOOK_TBL WHERE REC_KEY = %s), 'I', SYSDATE);")), strBookKey);
// 					ids = pDM->AddFrame( strTmp );
// 					if( ids < 0 )	return ids;
					//=====================================================
					
				}
				else
				{
					nMoCnt++;
					if( nMoCnt == 1 )	strMOWhere += strBookKey;
					else				strMOWhere += _T(", ") + strBookKey;
					// collection
					//=====================================================
					//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 					strTmp.Format(_T("INSERT INTO "
// 									_T("IDX_TMP_BO_TBL( REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 									_T("VALUES( ESL_SEQ.NEXTVAL, (SELECT DISTINCT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY = %s), 'I', SYSDATE);")), strBookKey);
// 					ids = pDM->AddFrame( strTmp );
// 					if( ids < 0 )	return ids;
					//=====================================================
					
				}
			}
		}
		// -- END(+)		=================================================================

		/*
		// ++ 제한건수로 쿼리 보내기
		if( nCnt >= QUERY_UNIT )
		{
			strTmp = strHeadQuery + strWhere + _T(" ); ");
			ids = pDM->AddFrame( strTmp );
			if( ids < 0 )	return ids;
			
			// ++ ADD Query(+)	=================================================================
			if( nMoCnt > 0 )
			{		
				// BO_BOOK_TBL
				strMOQuery.Replace(_T("IDX_BO_TBL"), _T("BO_BOOK_TBL"));
				strTmp = strMOQuery + strMOWhere + _T(" ); ");
				ids = pDM->AddFrame( strTmp );
				if( ids < 0 )	return ids;
				// IDX_BO_TBL ADD
				strMOQuery.Replace(_T("BO_BOOK_TBL"), _T("IDX_BO_TBL"));
				strTmp = strMOQuery + _T(" WHERE REC_KEY IN ( SELECT DISTINCT SPECIES_KEY FROM BO_BOOK_TBL ") + strMOWhere + _T(" )); ");
				ids = pDM->AddFrame( strTmp );
				if( ids < 0 )	return ids;
			}
			if( nSeCnt > 0 )
			{
				// SE_BOOK_TBL
				strSEQuery.Replace(_T("IDX_SE_TBL"), _T("SE_BOOK_TBL"));
				strTmp = strSEQuery + strSEWhere += _T(" ); ");
				ids = pDM->AddFrame( strTmp );
				if( ids < 0 )	return ids;
				// IDX_SE_TBL ADD
				strSEQuery.Replace(_T("SE_BOOK_TBL"), _T("IDX_SE_TBL"));
				strTmp = strSEQuery + _T(" WHERE REC_KEY IN ( SELECT DISTINCT SPECIES_KEY FROM SE_BOOK_TBL ") + strSEWhere + _T(" )); ");
				ids = pDM->AddFrame( strTmp );
				if( ids < 0 )	return ids;
			}
			// -- END(+)		=================================================================

			ids = pDM->SendFrame();
			if( ids < 0 )	return ids;

			ids = pDM->FreeFrame();
			if( ids < 0 )	return ids;

			nCnt = nMoCnt = nSeCnt = 0;
			strTmp = strRecKey = strWhere = _T(" WHERE REC_KEY IN ( ");
		}
		// -- 제한건수로 쿼리 보내기
		*/

		i = pGrid->SelectGetNext();
	}

	if( nCnt != 0 )
	{
		strTmp = strHeadQuery + strWhere + _T(" ); ");
		ids = pDM->AddFrame( strTmp );
		if( ids < 0 )	return ids;
		
		// ++ ADD Query(+)	=================================================================
		if( nMoCnt > 0 )
		{	
			// BO_BOOK_TBL
			strMOQuery.Replace(_T("IDX_BO_TBL"), _T("BO_BOOK_TBL"));
			strTmp = strMOQuery + strMOWhere + _T(" ); ");
			ids = pDM->AddFrame( strTmp );
			if( ids < 0 )	return ids;
			// IDX_BO_TBL ADD
			strMOQuery.Replace(_T("BO_BOOK_TBL"), _T("IDX_BO_TBL"));
			strTmp = strMOQuery + _T(" WHERE REC_KEY IN ( SELECT DISTINCT SPECIES_KEY FROM BO_BOOK_TBL ") + strMOWhere + _T(" )); ");
			ids = pDM->AddFrame( strTmp );
			if( ids < 0 )	return ids;
		}
		if( nSeCnt > 0 )
		{
			// SE_BOOK_TBL
			strSEQuery.Replace(_T("IDX_SE_TBL"), _T("SE_BOOK_TBL"));
			strTmp = strSEQuery + strSEWhere += _T(" ); ");
			ids = pDM->AddFrame( strTmp );
			if( ids < 0 )	return ids;
			// IDX_SE_TBL ADD
			strSEQuery.Replace(_T("SE_BOOK_TBL"), _T("IDX_SE_TBL"));
			strTmp = strSEQuery + _T(" WHERE REC_KEY IN ( SELECT DISTINCT SPECIES_KEY FROM SE_BOOK_TBL ") + strSEWhere + _T(" )); ");
			ids = pDM->AddFrame( strTmp );
			if( ids < 0 )	return ids;
		}
		// -- END(+)		=================================================================
	}
	
	// Delete Row
	i = pGrid->SelectGetTailPosition();
	while( i >= 0 )
	{
		ids = pDM->DeleteRow(i);
		if( ids < 0 )	return ids;
		i = pGrid->SelectGetPrev();
	}

	return 0;
}

VOID CBO_LOC_3900::DisplayToolBarButton()
{
	CString strPlanAlias	 = _T("자료처리");
	CString strCompleteAlias = _T("취소");
	if( m_nStatus == 1 )
	{
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strPlanAlias, TRUE);
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strCompleteAlias, FALSE);
	}
	else
	{
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strPlanAlias, FALSE);
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strCompleteAlias, TRUE);
	}
}

HBRUSH CBO_LOC_3900::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
