// SE_LOC_15001.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_15001.h"
#include "..\\..\\..\\관리\\검색\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"
#include "SE_LOC_1597.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1500 dialog


CSE_LOC_1500::CSE_LOC_1500(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1500)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_arrStrBookKey.RemoveAll();
	m_arrStrVolKey.RemoveAll();
	m_arrStrDeleteBookKey.RemoveAll();
}

CSE_LOC_1500::~CSE_LOC_1500()
{
}

VOID CSE_LOC_1500::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1500)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1500, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1500)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1500 message handlers

BOOL CSE_LOC_1500::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1500")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBL_SE_LOC_15(this);
		
	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("메인그리드") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_SE_LOC_1500"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1500::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);	

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}
	//=====================================================
	//2009.01.29 UPDATE BY PJW : 그리드에 스크롤 나오도록 함
// 	pGrid->MoveWindow(-2, 35 , cx+2, cy );
	pGrid->MoveWindow(-2, 35 , cx+2, cy-35 );
	//=====================================================
	
}

BOOL CSE_LOC_1500::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}



INT CSE_LOC_1500::PrintList( CString title, CString strPrintType)
{
	EFS_BEGIN

	INT ids;

	/// 2004년 7월 27일 조병걸 수정
	/// 실시간 인계 자료 목록은 변경되어야 한다.
	if( title.Compare(_T("실시간 인계자료 목록(2)") ) == 0 )
	{
		PrintShelfList(strPrintType);

		return 0;
	}

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_SE_LOC_1500_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1500") , _T("메인그리드") );

	if ( strPrintType == _T("출력") )
	{
		// pSrcDM -> pPrintDM으로 복사
		ids = MakePrintDM( _T("DM_SE_LOC_1500"), pGrid, pPrintDM);
	}
	else if ( strPrintType == _T("보관자료출력") )
	{
		ids = MakePrintDM(pPrintDM);
	}
	if ( ids != 0 ) return ids;

	// 일반적인 반복물 출력
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속실시간인계자료목록"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);

	// 김용배 추가
	pSIReportManager->SetTempVariable( _T("제목") , title );

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1500::MakePrintDM( CString strSrcDMName, CESL_Grid* pGrid, CESL_DataMgr* pPrintDM )
{
EFS_BEGIN

	if ( strSrcDMName.IsEmpty() || NULL == pGrid || NULL == pPrintDM )
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") );

	CESL_DataMgr *pSrcDM = FindDM( strSrcDMName );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakePrintDM") ); 

	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}

	const INT nCopyCnt=23;
	pPrintDM->FreeData();

	//
	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("수입구분") , _T("수입구분") } , 
		{ _T("서명") , _T("서명") } , 
		{ _T("권호명") , _T("권호명") } , 
		{ _T("발행자") , _T("발행자") } , 
		{ _T("복귀경로") , _T("복귀경로") } , 
		{ _T("실시간인계일자") , _T("실시간인계일자") } ,
		{ _T("간행빈도"), _T("간행빈도") },
		{ _T("관리구분"), _T("관리구분") } , 
		{ _T("이전자료상태"), _T("이전자료상태") } ,
		{ _T("가등록번호"), _T("가등록번호") } ,
		{ _T("배가변경사유설명"), _T("배가변경사유코드") } ,
		{ _T("최초배가일자"), _T("배가변경일자") } ,
		{ _T("자료실"), _T("자료실") } ,
		{ _T("배가담당자"), _T("배가담당자") } ,
		{ _T("이용제한구분"), _T("이용제한구분") } ,
		{ _T("자료상태"), _T("자료상태") } ,
		{ _T("함번호"), _T("함번호") } ,
		{ _T("청구기호"), _T("청구기호") },
		{ _T("간행빈도"), _T("간행빈도") },
		{ _T("부차적권호명"), _T("부차적권호명") },
		{ _T("종정보KEY"), _T("종정보KEY") },
		{ _T("권호KEY"), _T("권호KEY") },
		{ _T("부록여부"), _T("부록여부") }
	};


	CString sData;
	INT ids = -1;
	
	pGrid->SelectMakeList();
	
	INT nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();
	
	// 0 : 모두 출력 , 1 : 선정된 그리드만 출력
	INT nMode = 0;
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pPrintDM->InsertRow(-1);



		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			// Get data
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

			if( j == 0 )
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("수입구분") , sData , sData );

			// Set data to printDM
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}

	return 0;
EFS_END
	return -1;
}


INT CSE_LOC_1500::MakePrintDM( CESL_DataMgr *pPrintDM )
{
EFS_BEGIN

	if ( NULL == pPrintDM ) 
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") );


	// 1. 저장된 종키를 가지고 쿼리를 구성한다.
	CString strOption = _T("B.REC_KEY IN (");
	INT nCount = m_arrStrBookKey.GetSize();

	if ( nCount<=0 ) 
	{
		AfxMessageBox(_T("보관된 자료가 없습니다."));
		return 1;
	}

	for (INT idx = 0; idx < nCount; idx++)
	{
		strOption += m_arrStrBookKey.GetAt(idx);
		if (idx != nCount - 1)
			strOption += _T(", ");
	}

	strOption += _T(")");


	// 보관된 자료 출력일 경우
	// 삭제 키가 있으면 
//	nCount = m_arrStrDeleteBookKey.GetSize();
//	if ( nCount > 0 )
//	{
//		CString strTmpOption = _T(" AND B.REC_KEY NOT IN (");
//		
//		for (INT idx = 0; idx < nCount; idx++)
//		{
//			strTmpOption += m_arrStrDeleteBookKey.GetAt(idx);
//			if (idx != nCount - 1)
//				strTmpOption += _T(", ");
//		}
//		strTmpOption += _T(")");
//
//		if (  nCount > 0 ) 
//			strOption += strTmpOption;
//		
//	}

	// 2. 출력할 DM 구성
	INT ids = pPrintDM->RefreshDataManager(strOption);
	if ( ids<0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakePrintDM") );

	// 3. 번호, 수입구분 SET
	CString sData;
	nCount = pPrintDM->GetRowCount();
	for (  INT i=0; i<nCount; i++ )
	{
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakePrintDM") );

		ids = pPrintDM->GetCellData(_T("수입구분"), i, sData);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakePrintDM") );

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("수입구분") , sData , sData );

		// Set data to printDM
		ids = pPrintDM->SetCellData( _T("수입구분") , sData , i );
	}


	return 0;
EFS_END
	return -1;
}

INT CSE_LOC_1500::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}

// 출력자료 보관
INT CSE_LOC_1500::SavePrintList( CString strSpeciesKeyAlias)
{
EFS_BEGIN
	
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SE_LOC_1500") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") ); 

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1500") , _T("메인그리드") );
	if ( NULL == pGrid ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("SavePrintList") );

	pGrid->SelectMakeList();
	
	INT nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();

	CString strKey=_T("");

	INT ids= -1;
	INT nIndex = pGrid->SelectGetIdx();
	for ( INT i=0; i<nSelectCnt; i++ )
	{
		//중복체크 BookKey
		ids = pSrcDM->GetCellData(_T("책정보KEY"), nIndex, strKey );
		if ( ids<0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("SavePrintList") );

		
		if ( CheckPrintListDup(&m_arrStrBookKey, strKey) > 0 ) ;
		else m_arrStrBookKey.Add(strKey);

		//중복체크하자 VolKey
		ids = pSrcDM->GetCellData(_T("권호KEY"), nIndex, strKey );
		if ( ids<0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("SavePrintList") );

		if ( CheckPrintListDup(&m_arrStrVolKey, strKey) > 0 ) ;
		else m_arrStrVolKey.Add(strKey);


		nIndex = pGrid->SelectGetNext();
	}
	
	INT cnt  = m_arrStrBookKey.GetSize();
	CString sMsg;
	sMsg.Format(_T("총 %d건의 자료가 보관되었습니다."), cnt);
	AfxMessageBox(sMsg);

	return 0;
EFS_END
	return -1;	
}

// 출력자료 보관된 키 중복체크
INT CSE_LOC_1500::CheckPrintListDup(CStringArray *arrStrKey, CString strKey)
{
EFS_BEGIN
	
	INT nCnt = arrStrKey->GetSize();
	
	CString strTmp;
	for ( INT i=0; i<nCnt; i++)
	{
		strTmp = arrStrKey->GetAt(i);

		if ( strTmp == strKey ) return 1;
	}
	

	return 0;
EFS_END
	return -1;
}

INT CSE_LOC_1500::DetailView()
{
	EFS_BEGIN
		
	CString sParentCMAlias = _T("CM_SE_LOC_1500");
	CString sParentDMAlias = _T("DM_SE_LOC_1500");
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("DetailView") );
	
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("DetailView") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("메인그리드") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("DetailView") );
	
	if( pParentDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("자료를 검색해주십시오") );
		return 0;
	}
	
	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;
	
	INT nMode = 0;
	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	INT nStartPos = 0;
	INT nTmpStartPos = pGrid->GetIdx();
	if( nSelectCnt > 1 )
	{
		nMode = 1;
		pGrid->SelectGetHeadPosition();
	}
	else
	{
		nSelectCnt = pParentDM->GetRowCount();
		nStartPos = pGrid->GetIdx();
	}
	
	INT nIndex;
	
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 1 )
			nIndex = pGrid->SelectGetIdx();
		else
			nIndex = i;
		
		pParentDM->GetCellData( _T("종정보KEY") , nIndex, sSpeciesKey );
		sSpeciesKeyList.AddTail( sSpeciesKey );
		asGridRow.Add( nIndex );

		if( nIndex == nTmpStartPos )
			nStartPos = i;
		
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
	
	CSEBookDetailViewerDlg Dlg(this);
	
	Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartPos );
	Dlg.DoModal();
	
	return 0;
	
	EFS_END
	return -1;
	
}

BEGIN_EVENTSINK_MAP(CSE_LOC_1500, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_LOC_1500)
	ON_EVENT(CSE_LOC_1500, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_LOC_1500::OnDblClickgridMAIN() 
{
	// 상세 정보 보여주기	
	CString sCMAlias = _T("CM_SE_LOC_1500");
	CString sGridAlias = _T("메인그리드");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnClickgridMAIN") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnClickgridMAIN") );
	
	if( pGrid->GetMouseRow() < 1 ) return;
	
	pGrid->SelectMakeList();
	if( pGrid->SelectGetCount() > 0 )
		pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );

	
	DetailView();
}

INT CSE_LOC_1500::PrintShelfList(CString strPrintType)
{
	EFS_BEGIN

	INT ids;

	// 인계일 가져오기
	CString sShelfDate , sShelfLoc , sTitle , sBookCnt;

	CSE_LOC_1597 ShelfDate(this);
	ShelfDate.DoModal();

	if( !ShelfDate.m_bPrint ) return 0;

	sShelfDate = ShelfDate.m_sShelfDate;

	// 자료실별 > 종별 > 권호별 로 정렬이 되어야 한다.

//	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SE_LOC_1500") );
//	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintShelfList") ); 

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_SE_LOC_1500_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("PrintShelfList") );

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1500") , _T("메인그리드") );

//	if( pSrcDM->GetRowCount() < 1 )
//	{
//		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
//		return 1;
//	}


	if ( strPrintType == _T("출력") )
	{
		// pSrcDM -> pPrintDM으로 복사
		ids = MakePrintDM( _T("DM_SE_LOC_1500"), pGrid, pPrintDM);
	}
	else if ( strPrintType == _T("보관자료출력") )
	{
		ids = MakePrintDM(pPrintDM);
	}
	if ( ids != 0 ) return ids;

	// pSrcDM -> pPrintDM으로 복사
	const INT nCopyCnt=22;
	//pPrintDM->FreeData();

	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("수입구분") , _T("수입구분") } , 
		{ _T("서명") , _T("서명") } , 
		{ _T("권호명") , _T("권호명") } , 
		{ _T("발행자") , _T("발행자") } , 
		{ _T("복귀경로") , _T("복귀경로") } , 
		{ _T("실시간인계일자") , _T("실시간인계일자") } ,
		{ _T("간행빈도"), _T("간행빈도") },
		{ _T("관리구분"), _T("관리구분") } , 
		{ _T("이전자료상태"), _T("이전자료상태") } ,
		{ _T("가등록번호"), _T("가등록번호") } ,
		{ _T("배가변경사유설명"), _T("배가변경사유코드") } ,
		{ _T("최초배가일자"), _T("배가변경일자") } ,
		{ _T("자료실"), _T("자료실") } ,
		{ _T("배가담당자"), _T("배가담당자") } ,
		{ _T("이용제한구분"), _T("이용제한구분") } ,
		{ _T("자료상태"), _T("자료상태") } ,
		{ _T("함번호"), _T("함번호") } ,
		{ _T("청구기호"), _T("청구기호") },
		{ _T("간행빈도"), _T("간행빈도") },
		{ _T("부차적권호명"), _T("부차적권호명") },
		{ _T("종정보KEY"), _T("종정보KEY") },
		{ _T("권호KEY"), _T("권호KEY") }
	};


	CString sData;
	
/*	pGrid->SelectMakeList();
	
	INT nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();
	
	// 0 : 모두 출력 , 1 : 선정된 그리드만 출력
	INT nMode = 0;
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pPrintDM->InsertRow(-1);



		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintShelfList") );

		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintShelfList") );

			if( j == 0 )
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("수입구분") , sData , sData );

			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintShelfList") );
		}
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
*/

	const INT nSortCnt = 3;
	CString sSortAlias[nSortCnt] =
	{
		_T("자료실") , _T("종정보KEY") , _T("권호KEY") 
	};

	INT nSortCond[nSortCnt] =
	{
		TRUE, TRUE, TRUE
	};

	ids = pPrintDM->SORT( nSortCnt , sSortAlias , nSortCond , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("PrintShelfList") );

	/// 종수 , 권호수를 뽑아낸다.
	

	// 일반적인 반복물 출력
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속실시간인계자료목록(갑지추가)"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}


	INT nRowCnt = pPrintDM->GetRowCount();
	CESL_DataMgr TmpPrintDM;
	CLocCommonAPI::MakeCopyDM( this , pPrintDM , &TmpPrintDM );

	pSIReportManager->SetDataMgr(0, &TmpPrintDM , 0);

	pSIReportManager->DeleteTempPageFiles();
	
	// 인계 일자 , 자료실 , 서명 , 책수
	BOOL bFirst = TRUE;
	CString sNextShelfLoc;
	CString sTotalCnt;
	INT nSpeciesCnt = 0;
	INT nVolCnt = 0;
	INT nInterIndex = 0;
	BOOL bPrint = FALSE;
	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		if( bFirst )
		{
			ids = pPrintDM->GetCellData( _T("자료실") , i , sShelfLoc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("PrintShelfList") );
			ids = pPrintDM->GetCellData( _T("서명") , i , sTitle );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("PrintShelfList") );

			bFirst = FALSE;
		}
		
		if( i == (nRowCnt-1) )
			bPrint = TRUE;
		else
		{
			ids = pPrintDM->GetCellData( _T("자료실") , i+1 , sNextShelfLoc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("PrintShelfList") );

			if( sShelfLoc.Compare( sNextShelfLoc ) != 0 )
				bPrint = TRUE;
		}

		TmpPrintDM.InsertRow(-1);

		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pPrintDM->GetCellData( sField[j][0] , i , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintShelfList") );

			ids = TmpPrintDM.SetCellData( sField[j][1] , sData , nInterIndex );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nInterIndex , sField[j][1] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintShelfList") );
		}
		CString sIndex;
		sIndex.Format( _T("%d") , nInterIndex+1 );
		ids = TmpPrintDM.SetCellData( _T("번호") , sIndex  , nInterIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("PrintShelfList") );

		nInterIndex++;

		if( !bPrint ) 
			continue;

		ids = GetSpeciesVolCnt( nSpeciesCnt , nVolCnt , &TmpPrintDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("PrintShelfList") );

		if( sTitle.GetLength() > 30 )
			CutString( sTitle , 30 );

		sTitle = sTitle + _T(" 등");
		sBookCnt.Format( _T("%d") , TmpPrintDM.GetRowCount() );
		sTotalCnt.Format( _T("%d 종 %d 권 %s 책") , nSpeciesCnt , nVolCnt , sBookCnt );
		pSIReportManager->SetTempVariable( _T("자료실") , sShelfLoc );
		pSIReportManager->SetTempVariable( _T("서명") , sTitle );
		pSIReportManager->SetTempVariable( _T("책수") , sBookCnt );
		pSIReportManager->SetTempVariable( _T("인계일자") , sShelfDate );
		pSIReportManager->SetTempVariable( _T("합계") , sTotalCnt );



		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("PrintShelfList") );

		bFirst = TRUE;
		bPrint = FALSE;
		nInterIndex = 0;
		TmpPrintDM.FreeData();
	}

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	



	return 0;


	EFS_END

	return -1;

}

INT CSE_LOC_1500::GetSpeciesVolCnt( INT &nSpeciesCnt , INT &nVolCnt , CESL_DataMgr* pDM )
{
	EFS_BEGIN

	INT ids;

	nSpeciesCnt = 0;
	nVolCnt = 0;

	CString sPreSpeciesKey , sPreVolKey;
	CString sSpeciesKey , sVolKey;

	INT nRowCnt = pDM->GetRowCount();

	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("종정보KEY") , i , sSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSpeciesVolCnt") );
		ids = pDM->GetCellData( _T("권호KEY") , i , sVolKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetSpeciesVolCnt") );

		if( i != 0 )
		{
			ids = pDM->GetCellData( _T("종정보KEY") , i-1 , sPreSpeciesKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetSpeciesVolCnt") );
			ids = pDM->GetCellData( _T("권호KEY") , i-1 , sPreVolKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetSpeciesVolCnt") );
		}

		if( sSpeciesKey.Compare( sPreSpeciesKey ) != 0 ) nSpeciesCnt++;
		if( sVolKey.Compare( sPreVolKey ) != 0 ) nVolCnt++;

	}


	return 0;

	EFS_END

	return -1;
}

INT CSE_LOC_1500::CutString( CString &sStr , INT nMaxLength )
{
	EFS_BEGIN

	BOOL bPreHanguel = FALSE;
	for( INT i = 0 ; i < nMaxLength ; i++ )
	{
		if( sStr.GetAt(i) < 0 )
			bPreHanguel = TRUE;
		if( sStr.GetAt(i) >= 0 )
			bPreHanguel = FALSE;
	}
	
	if( sStr.GetAt(nMaxLength-1) < 0 && bPreHanguel )
	{
		sStr = sStr.Mid( 0 , nMaxLength-1 );
	}
	else
		sStr = sStr.Mid( 0 , nMaxLength );


	sStr += _T(".. ");



	return 0;


	EFS_END

	return -1;
}

HBRUSH CSE_LOC_1500::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
