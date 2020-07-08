// BO_REG_EPC_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "BO_REG_EPC_MAIN.h"

#include "BO_REG_EPC_SEARCH.h"
#include "BO_REG_EPC_PROGRESS_DLG.h"
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_MAIN dialog

CBO_REG_EPC_MAIN::CBO_REG_EPC_MAIN(CESL_Mgr* pParent )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_REG_EPC_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg_Process = NULL;
	m_bIsFreeDLL = FALSE;
}

CBO_REG_EPC_MAIN::~CBO_REG_EPC_MAIN()
{
	if(NULL != m_pDlg_Process) delete m_pDlg_Process;
	m_pDlg_Process = NULL;
}

VOID CBO_REG_EPC_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_REG_EPC_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBO_REG_EPC_MAIN, CDialog)
	//{{AFX_MSG_MAP(CBO_REG_EPC_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(JOB_CANCEL_MSG, StopEPCThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_MAIN message handlers

BOOL CBO_REG_EPC_MAIN::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_REG_EPC_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( 0 > InitESL_Mgr(_T("BO_BOOK_TRADE_REG")) )
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pDlg_Process = new CBO_REG_EPC_PROGRESS_DLG(this);
	m_pDlg_Process->Create(this);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

VOID CBO_REG_EPC_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////	
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_MAIN);

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////	
	if( pGrid->GetSafeHwnd() )	
		pGrid->MoveWindow(0, 0, cx, cy);
		//pGrid->MoveWindow( -2, -2 , cx+4, cy+4 );
}

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_MAIN func


/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  검색 화면 호출
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_MAIN::ShowSearchDlg()
{
EFS_BEGIN

	CBO_REG_EPC_SEARCH dlg(this);
	dlg.DoModal();

	return 0;

EFS_END
	return -1;
}

//DEL /////////////////////////////////////////////////////////////////////////////
//DEL // [Func Desc]
//DEL //  EPC 일괄부여
//DEL // [Mini Spec]
//DEL //  1. 검색유무 확인
//DEL //  2. 선정자료 확인
//DEL //  3. 화면DM을 임시DM에 복사
//DEL //  4. 임시DM을 ISBN으로 정렬
//DEL //  5. ISBN별로 EPC 요청
//DEL //  6. 결과 DB Update
//DEL //  7. 임시DM 결과 화면DM에 적용
//DEL //  8. 결과 화면 출력
//DEL /////////////////////////////////////////////////////////////////////////////
//DEL INT CBO_REG_EPC_MAIN::CreateEPC()
//DEL {
//DEL EFS_BEGIN
//DEL 	
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	// Declare Variable
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	INT		ids;
//DEL 	INT		nMode = 0;
//DEL 	INT		nGridSelectCount = 0;
//DEL 	CBO_REG_EPC_PROGRESS_DLG* pProgressDlg = NULL;
//DEL 
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	// 1. 검색유무 확인
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	CESL_DataMgr* pGridDM = FindDM( _T("DM_BO_BOOK_TRADE_MAIN") );
//DEL 	if( !pGridDM )
//DEL 	{
//DEL 		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -1 !!!") );
//DEL 		return -1;
//DEL 	}
//DEL 
//DEL 	if( !(pGridDM->GetRowCount()) )
//DEL 	{
//DEL 		AfxMessageBox( _T("검색된 자료가 없습니다.") );
//DEL 		return 0;
//DEL 	}
//DEL 	
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	// 2. 선정자료 확인
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_BOOK_TRADE_MAIN") , _T("MAIN_GRID") );
//DEL 	if( !pGrid )
//DEL 	{
//DEL 		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -2 !!!") );
//DEL 		return -1;
//DEL 	}
//DEL 	
//DEL 	pGrid->SelectMakeList();
//DEL 
//DEL 	if( !(nGridSelectCount = pGrid->SelectGetCount()) )
//DEL 	{
//DEL 		if( IDNO == AfxMessageBox( _T("전체 자료를 일괄부여 하시겠습니까?"), MB_YESNO|MB_ICONQUESTION ) )
//DEL 		{
//DEL 			return 0;
//DEL 		}
//DEL 
//DEL 		nGridSelectCount = pGridDM->GetRowCount();		
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		pGrid->SelectMakeList();
//DEL 		pGrid->SelectGetHeadPosition();
//DEL 		nMode = 1;
//DEL 	}
//DEL 	
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	// 3. 화면DM을 임시DM에 복사
//DEL 	//    모든 항목을 복사할 필요는 없다.
//DEL 	//    book_key/isbn/epc
//DEL 	//    isbn이 NULL인 자료는 제외한다.
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 
//DEL 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DEL 	pProgressDlg = new CBO_REG_EPC_PROGRESS_DLG(this);
//DEL 	pProgressDlg->Create (this);
//DEL     pProgressDlg->CenterWindow () ;
//DEL     pProgressDlg->ShowWindow (SW_SHOW);
//DEL 	pProgressDlg->UpdateWindow() ;
//DEL 	
//DEL 	pProgressDlg->SetMessage(_T("EPC 부여 대상 목록을 구축중입니다."));
//DEL 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DEL 
//DEL 	CESL_DataMgr TmpDM;
//DEL 
//DEL  	const INT    nColumn = 7;
//DEL  	CString      strColumn[nColumn] = {	_T("ISBN")		,
//DEL  						_T("BOOK_KEY")	,										
//DEL  						_T("EPC")		,
//DEL  						_T("서명")		,
//DEL  						_T("저자")		,
//DEL  						_T("발행자")	,
//DEL  						_T("발행년")
//DEL  					};
//DEL 
//DEL 	CString		 strResult;
//DEL 	INT			 nIndex;
//DEL 	
//DEL 	INT          nExistEPCancle   = 0;
//DEL 	INT          nISBNErrorCancle = 0;
//DEL 	INT          nSuccessCount    = 0;
//DEL 
//DEL 	TmpDM.SetCONNECTION_INFO( pGridDM->CONNECTION_INFO );
//DEL 	TmpDM.InitDataMgrRef( nColumn );
//DEL 
//DEL 	for( INT i=0;i<nGridSelectCount;i++ )
//DEL 	{
//DEL 		if( nMode )
//DEL 		{
//DEL 			nIndex = pGrid->SelectGetIdx();
//DEL 			
//DEL 			pGrid->SelectGetNext();
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			nIndex = i;
//DEL 		}
//DEL 
//DEL 		TmpDM.InsertRow(-1);
//DEL 
//DEL 		for( INT j=0;j<nColumn;j++ )
//DEL 		{
//DEL 			ids = pGridDM->GetCellData( strColumn[j], nIndex, strResult );
//DEL 			if( 0 > ids )
//DEL 			{
//DEL 				AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -3 !!!") );
//DEL 				if( pProgressDlg )	delete pProgressDlg;
//DEL 				
//DEL 				return -1;
//DEL 			}
//DEL 
//DEL 			/// isbn 유무검사
//DEL 			if( 0 == j )
//DEL 			{
//DEL 				if( 0 == strResult.GetLength() )
//DEL 				{
//DEL 					nISBNErrorCancle++;
//DEL 
//DEL 					ids = TmpDM.DeleteRow(TmpDM.GetRowCount()-1);
//DEL 					if( 0 > ids )
//DEL 					{
//DEL 						AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -4 !!!") );
//DEL 						if( pProgressDlg )	delete pProgressDlg;
//DEL 						return -1;
//DEL 					}
//DEL 					break;
//DEL 				}
//DEL 			}
//DEL 			
//DEL 			/// EPC가 이미 부여된 자료는 제외
//DEL 			if( 2 == j )
//DEL 			{
//DEL 				if( 0 < strResult.GetLength() )
//DEL 				{
//DEL 					nExistEPCancle++;
//DEL 
//DEL 					ids = TmpDM.DeleteRow(TmpDM.GetRowCount()-1);
//DEL 					if( 0 > ids )
//DEL 					{
//DEL 						AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -4 !!!") );
//DEL 						if( pProgressDlg )	delete pProgressDlg;
//DEL 						return -1;
//DEL 					}
//DEL 					break;
//DEL 				}
//DEL 			}
//DEL 
//DEL 			ids = TmpDM.SetCellData( TmpDM.GetRowCount()-1, j, strResult );
//DEL 			if( 0 > ids )
//DEL 			{
//DEL 				AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -5 !!!") );
//DEL 				if( pProgressDlg )	delete pProgressDlg;
//DEL 				return -1;
//DEL 			}
//DEL 		}	
//DEL 	}
//DEL 
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	// 4. 임시DM을 ISBN으로 정렬
//DEL 	//    MTB통신 과정에서 ISBN하나에 복수개의 EPC를 가져올수 있다.
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	ids = TmpDM.SORT_QUICKSORT( 0, ESL_DATAMGR_TYPE_STR, 0, TmpDM.GetRowCount()-1, FALSE );
//DEL 	if( 0 > ids )
//DEL 	{
//DEL 		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -6 !!!") );
//DEL 		if( pProgressDlg )	delete pProgressDlg;
//DEL 		return -1;
//DEL 	}
//DEL 
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	// 5. ISBN별로 EPC 요청
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	CString			   strCurIdxISBN;
//DEL 	CString			   strBeforeIdxISBN;
//DEL 	INT				   nStartIdx = 0;
//DEL 	
//DEL 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DEL 	pProgressDlg->SetMessage(_T("EPC 부여 작업을 진행중입니다."));
//DEL 	pProgressDlg->SetProgressRange( 0, TmpDM.GetRowCount() );
//DEL 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DEL 
//DEL 	for( i=0;i<TmpDM.GetRowCount();i++ )
//DEL 	{
//DEL 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DEL 		pProgressDlg->SetProgressStepIt();
//DEL 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DEL 
//DEL 		/// 이전 인덱스의 ISBN 저장
//DEL 		if( strCurIdxISBN.GetLength() )
//DEL 		{
//DEL 			strBeforeIdxISBN = strCurIdxISBN;
//DEL 		}
//DEL 
//DEL 		ids = TmpDM.GetCellData( i, 0, strCurIdxISBN );
//DEL 		if( 0 > ids )
//DEL 		{
//DEL 			AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -6 !!!") );
//DEL 			if( pProgressDlg )	delete pProgressDlg;
//DEL 			return -1;
//DEL 		}
//DEL 
//DEL 		if( !strCurIdxISBN.GetLength() )
//DEL 		{
//DEL 			nStartIdx = i + 1;
//DEL 			continue;
//DEL 		}
//DEL 
//DEL 		/// 최초 작업시 이전에 기억된 ISBN은 없다.
//DEL 		if( !strBeforeIdxISBN.GetLength() )
//DEL 		{
//DEL 			strBeforeIdxISBN = strCurIdxISBN;
//DEL 			continue;
//DEL 		}
//DEL 
//DEL 		/// 동일하다면 pass~
//DEL 		if( strBeforeIdxISBN == strCurIdxISBN )
//DEL 		{
//DEL 			continue;
//DEL 		}
//DEL 
//DEL 		/// 해당 ISBN에 대한 EPC를 받아야 한다.
//DEL 		/// 지정된 범위의 DB 정보를 Update 한다.
//DEL 		ids = UpdateProc( strBeforeIdxISBN, nStartIdx, i-nStartIdx, &TmpDM );
//DEL 		if( 0 > ids && -4001 != ids )
//DEL 		{
//DEL 			//AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -7 !!!") );
//DEL 			if( pProgressDlg )	delete pProgressDlg;
//DEL 			return -1;
//DEL 		}
//DEL 		if( -4001 == ids )
//DEL 		{
//DEL 			nISBNErrorCancle += (i-nStartIdx);
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			nSuccessCount += (i-nStartIdx);
//DEL 		}
//DEL 
//DEL 		nStartIdx = i;
//DEL 	}
//DEL 
//DEL 	/// 미처리 자료의 잔여잔업
//DEL 	if( 0 < (i-nStartIdx) )
//DEL 	{
//DEL 		ids = UpdateProc( strCurIdxISBN, nStartIdx, i-nStartIdx, &TmpDM );
//DEL 		if( 0 > ids && -4001 != ids )
//DEL 		{
//DEL 			//AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -7 !!!") );
//DEL 			if( pProgressDlg )	delete pProgressDlg;
//DEL 			return -1;
//DEL 		}
//DEL 		if( -4001 == ids )
//DEL 		{
//DEL 			nISBNErrorCancle += (i-nStartIdx);
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			nSuccessCount += (i-nStartIdx);
//DEL 		}
//DEL 	}
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	// 7. 임시DM 결과 화면DM에 적용
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	/// 임시DM은 ISBN으로 정렬되어 있다.
//DEL 	/// 화면DM은 정렬되어 있지 않다.
//DEL 	
//DEL 	CString strBookKey;
//DEL 	CString strTmpBookKey;
//DEL 	
//DEL 	pGrid->SelectGetHeadPosition();
//DEL 
//DEL 	for( i=0;i<nGridSelectCount;i++ )
//DEL 	{
//DEL 		if( nMode )
//DEL 		{
//DEL 			nIndex = pGrid->SelectGetIdx();
//DEL 			
//DEL 			pGrid->SelectGetNext();
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			nIndex = i;
//DEL 		}
//DEL 
//DEL 		ids = pGridDM->GetCellData( _T("BOOK_KEY"), nIndex, strBookKey );
//DEL 		if( 0 > ids )
//DEL 		{
//DEL 			AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -8 !!!") );
//DEL 			if( pProgressDlg )	delete pProgressDlg;
//DEL 			return -1;
//DEL 		}
//DEL 
//DEL 		for( INT j=0;j<TmpDM.GetRowCount();j++ )
//DEL 		{
//DEL 			ids = TmpDM.GetCellData( j, 1, strTmpBookKey );
//DEL 			if( 0 > ids )
//DEL 			{
//DEL 				AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -9 !!!") );
//DEL 				if( pProgressDlg )	delete pProgressDlg;
//DEL 				return -1;
//DEL 			}
//DEL 
//DEL 			if( strBookKey == strTmpBookKey )
//DEL 			{
//DEL 				ids = TmpDM.GetCellData( j, 2, strResult );
//DEL 				if( 0 > ids )
//DEL 				{
//DEL 					AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -10 !!!") );
//DEL 					if( pProgressDlg )	delete pProgressDlg;
//DEL 					return -1;
//DEL 				}
//DEL 				ids = pGridDM->SetCellData( _T("EPC"), strResult, nIndex );
//DEL 				if( 0 > ids )
//DEL 				{
//DEL 					AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -11 !!!") );
//DEL 					if( pProgressDlg )	delete pProgressDlg;
//DEL 					return -1;
//DEL 				}
//DEL 
//DEL 				break;
//DEL 			}
//DEL 
//DEL 			/// 못찾은 자료 처리는???
//DEL 		}
//DEL 	}
//DEL 
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	// 8. 결과 화면 출력
//DEL 	/////////////////////////////////////////////////////////////////////////
//DEL 	this->AllControlDisplay( _T("CM_BO_BOOK_TRADE_MAIN") );
//DEL 
//DEL 	CString strResultMsg;
//DEL 
//DEL 	if( TmpDM.GetRowCount() == nSuccessCount )
//DEL 	{
//DEL 		strResultMsg.Format( _T("[ %d ]의 자료에 성공적으로  EPC가 부여되었습니다"), nSuccessCount );
//DEL 	}
//DEL 	else if( TmpDM.GetRowCount() < nGridSelectCount )
//DEL 	{
//DEL 		strResultMsg.Format( _T("선정된 자료건수                      : [ %d ] \r\n")
//DEL 			                 _T("EPC가 부여된 자료건수              : [ %d ] \r\n")							 
//DEL 							 _T("ISBN오류로 취소된 자료건수       : [ %d ] \r\n")
//DEL 							 _T("EPC가 존재하여 취소된 자료건수 : [ %d ] "), 
//DEL 							 nGridSelectCount, nSuccessCount, nISBNErrorCancle ,nExistEPCancle
//DEL 							 );
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		strResultMsg = _T("CBO_REG_EPC_SEARCH::CreateEPC() ERROR -12 !!!");
//DEL 	}
//DEL 	
//DEL 	AfxMessageBox( strResultMsg );
//DEL 
//DEL 	if( pProgressDlg )	delete pProgressDlg;
//DEL 
//DEL 	return 0;
//DEL 
//DEL EFS_END
//DEL 	return -1;
//DEL }

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  EPC를 생성하여 DB에 Update한다.
// [Mini Spec]
//  1. EPC 생성
//  2. DB Update
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_MAIN::UpdateProc(CString strISBN, INT nStartIdx, INT nCount, CESL_DataMgr *pDM, CString &strErrorMsg)
{
EFS_BEGIN

	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT				    ids;
	CStringArray        arrEPC;
	CString             strBookKey,strTitle, strPublisher, strPubYear, strAuthor, strClassNo, strClassNoType;
	BOOL				bNEW = FALSE;
	BOOL				bMAKE = TRUE;

	CEPCTradeAPI EPCAPI;
	EPCAPI.SetCONNECTION_INFO( pDM->CONNECTION_INFO );
	EPCAPI.m_pInfomation = this->m_pInfo;
	EPCAPI.LoadAllXML();			
	EPCAPI.SetConServer(ISSUE_ID);

	//++2008.08.22 ADD BY CJY {{++
	// 목록구축시의 인자로 넘어가지 않는 값들을 주기위해
	// API에 DM에 북키를 넘겨준다.
	EPCAPI.m_DM.FreeData();
	EPCAPI.m_DM.FreeDMRef();
	EPCAPI.m_DM.InitDataMgrRef(1);
	EPCAPI.m_DM.InsertRow(-1);	
	//--2008.08.22 ADD BY CJY --}}

	/////////////////////////////////////////////////////////////////////////
	// 1. EPC 생성
	/////////////////////////////////////////////////////////////////////////
	ids = EPCAPI.GetEPCToServer( strISBN, nCount, arrEPC );
//DEL	if( 0 > ids && -3 < ids )
//DEL	{
//DEL		/// ISBN 에러 별도 처리
//DEL		return -4001;
//DEL	}
//DEL	if( -2 > ids )
//DEL	{
//DEL		CString strIds;
//DEL		strIds.Format( _T("%d"), ids );
//DEL		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::UpdateProc() ERROR -1 !!!") );
//DEL		return -1;
//DEL	}
	if(0 > ids) 
	{	
		if(-7 == ids) 
		{
			strErrorMsg = arrEPC.GetAt(0);
		}
		if(-7 != ids && TRUE == strErrorMsg.IsEmpty())
		{
			GetErrorMsg(_T("ISSUEID"), ids, strErrorMsg);
		}
		return ids;
	}

	if( 1001 == ids )
	{
		bNEW = TRUE;
	}
	//// 도서관 정보 생성해야한다.
	CString strLibCode;
	EPCAPI.GetLibCode( this->m_pInfo, strLibCode );
	/////////////////////////////////////////////////////////////////////////
	// 2. DB Update
	/////////////////////////////////////////////////////////////////////////
	pDM->StartFrame();

	for( INT i=0;i<nCount;i++ )
	{	
		ids = pDM->GetCellData(_T("BOOK_KEY"), nStartIdx+i, strBookKey );	// _T("BOOK_KEY")
		ids = pDM->GetCellData(_T("서명"), nStartIdx+i, strTitle ); // _T("서명")
		ids = pDM->GetCellData(_T("저자"), nStartIdx+i, strAuthor ); // _T("저자")
		ids = pDM->GetCellData(_T("발행자"), nStartIdx+i, strPublisher ); // _T("발행자")
		ids = pDM->GetCellData(_T("발행년"), nStartIdx+i, strPubYear );	// _T("발행년")
		ids = pDM->GetCellData(_T("CLASS_NO"), nStartIdx+i, strClassNo );
		ids = pDM->GetCellData(_T("CLASS_NO_TYPE"), nStartIdx+i, strClassNoType );	// _T("발행년")

//DEL 		ids = pDM->GetCellData(nStartIdx+i, 1, strBookKey );	// _T("BOOK_KEY")
//DEL 		ids = pDM->GetCellData(nStartIdx+i, 3, strTitle); // _T("서명")
//DEL 		ids = pDM->GetCellData(nStartIdx+i, 4, strAuthor); // _T("저자")
//DEL 		ids = pDM->GetCellData(nStartIdx+i, 5, strPublisher); // _T("발행자")
//DEL 		ids = pDM->GetCellData(nStartIdx+i, 6, strPubYear);	// _T("발행년")
		
		/////////////////////////////////////////////////////////////////////
		//// 목록 구축 .... 긴급추가 구조가 이상해졌다 ㅡㅡㅋ
		if( bNEW && i==0 )
		{			
			//++2008.08.22 ADD BY CJY {{++
			// 목록구축시의 인자로 넘어가지 않는 값들을 주기위해
			// API에 DM에 북키를 넘겨준다.
			EPCAPI.m_DM.SetCellData(0, 0, strBookKey);
			//--2008.08.22 ADD BY CJY --}}

			STRU_EPC_SEND_NEW_ISBN struNewISBN;			
			
			struNewISBN.strAuthor = strAuthor;
			struNewISBN.strClassNo = strClassNo;
			struNewISBN.strClassNoType = strClassNoType;
			struNewISBN.strISBN = strISBN;
			struNewISBN.strLibCode = m_pInfo->LIB_CODE;
			struNewISBN.strPublisher = strPublisher;
			struNewISBN.strPubYear = strPubYear;
			struNewISBN.strTitle = strTitle;			

// 			ids = EPCAPI.SendNewISBN(strISBN, strTitle, strPublisher, strPubYear, strAuthor, strLibCode.GetBuffer(0));
			ids = EPCAPI.SendNewISBN(struNewISBN, strErrorMsg);
			
			//++2009.01.23 UPDATE BY CJY {{++
			if(0 > ids)  
			{	
				if(FALSE == strErrorMsg.IsEmpty())
				{
					return ids;
				}
				GetErrorMsg(_T("MAKE_NEW_INFO"), ids, strErrorMsg);
				return ids;
			}
//DEL		이전소스
//DEL 			if( -2 > ids && -500 != ids )
//DEL 			{
//DEL 				pDM->EndFrame();
//DEL 				AfxMessageBox( _T("CBO_REG_EPC_SEARCH::UpdateProc() ERROR -2 !!!") );
//DEL 				return -2;
//DEL 			}
//DEL 			if( -500 == ids )
//DEL 			{
//DEL 				// 땜빵이다.
//DEL //				ids = pDM->GetCellData( nStartIdx+i, 1, strBookKey );
//DEL 				bMAKE = FALSE;
//DEL 			}
			//--2009.01.23 UPDATE BY CJY --}}
		}
		
		if( bMAKE )
		{
			//++2008.08.25 DEL BY CJY {{++
//DEL 			ids = EPCAPI.SendCheckInEPC(strLibCode, arrEPC.GetAt(i) ); 
//DEL 			if ( ids ) return ids;
			//--2008.08.25 DEL BY CJY --}}
		}

		ids = pDM->SetCellData(_T("EPC"), arrEPC.GetAt(i), nStartIdx+i);
//DEL 	ids = pDM->SetCellData( nStartIdx+i, 2, arrEPC.GetAt(i) );
		pDM->InitDBFieldData();

		ids = pDM->AddDBFieldData( _T("EPC"), _T("STRING"), arrEPC.GetAt(i) );

		ids = pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("등록"), __WFILE__, __LINE__ ) );

		ids = pDM->MakeUpdateFrame( _T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), strBookKey );
	}

	ids = pDM->SendFrame();

	pDM->EndFrame();

	return 0;

EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  EPC가 부여된 자료를 취소한다.
// [Mini Spec]
//  1. 검색유무 확인
//  2. 선정자료 확인
//  3. EPC 부여 여부 확인
//  4. EPC 취소
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_MAIN::CancelEPC()
{
EFS_BEGIN
	
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;	
	INT		nGridSelectCount = 0;
	
	/////////////////////////////////////////////////////////////////////////
	// 1. 검색유무 확인
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pGridDM = FindDM( _T("DM_BO_BOOK_TRADE_MAIN") );
	if( !pGridDM )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CancelEPC() ERROR -1 !!!") );
		return -1;
	}

	if( !(pGridDM->GetRowCount()) )
	{
		AfxMessageBox( _T("검색된 자료가 없습니다.") );
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// 2. 선정자료 확인
	/////////////////////////////////////////////////////////////////////////
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_BOOK_TRADE_MAIN") , _T("MAIN_GRID") );
	if( !pGrid )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CancelEPC() ERROR -2 !!!") );
		return -1;
	}
	
	pGrid->SelectMakeList();

	if( !(nGridSelectCount = pGrid->SelectGetCount()) )
	{
		AfxMessageBox( _T("선정된 자료가 없습니다.") );
		return 0;		
	}
	
	/////////////////////////////////////////////////////////////////////////
	//  3. EPC 부여 여부 확인
	/////////////////////////////////////////////////////////////////////////
	INT		nIndex;
	CString strEPC;
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	for( INT i=0;i<nGridSelectCount;i++ )
	{
		nIndex = pGrid->SelectGetIdx();
			
		pGrid->SelectGetNext();

		ids = pGridDM->GetCellData( _T("EPC"), nIndex, strEPC );
		if( 0 > ids )
		{
			AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CancelEPC() ERROR -8 !!!") );
			return -1;
		}

		if( 0 == strEPC.GetLength() )
		{
			AfxMessageBox( _T("EPC가 부여되지 않은 자료가 포함되어 작업을 취소합니다.") );
			return 0;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// 4. EPC 취소
	/////////////////////////////////////////////////////////////////////////
	CString strBookKey;
	CString strQuery;

	pGrid->SelectGetHeadPosition();

	pGridDM->InitDBFieldData();
	pGridDM->StartFrame();

	for( i=0;i<nGridSelectCount;i++ )
	{
		nIndex = pGrid->SelectGetIdx();
			
		pGrid->SelectGetNext();

		ids = pGridDM->GetCellData( _T("BOOK_KEY"), nIndex, strBookKey );
		if( 0 > ids )
		{
			AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CancelEPC() ERROR -8 !!!") );
			return -1;
		}

		if( 0 == strBookKey.GetLength() )
		{
			AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CancelEPC() ERROR -9 !!!") );
			return -1;
		}

		strQuery.Format( _T("UPDATE BO_BOOK_TBL SET EPC=NULL WHERE REC_KEY=%s;"), strBookKey );
		pGridDM->AddFrame( strQuery );
	}

	ids = pGridDM->SendFrame();
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CancelEPC() ERROR -10 !!!") );
		return -1;
	}

	pGridDM->EndFrame();

	/////////////////////////////////////////////////////////////////////////
	// 5. Display
	/////////////////////////////////////////////////////////////////////////
	pGrid->SelectGetHeadPosition();

	for( i=0;i<nGridSelectCount;i++ )
	{
		nIndex = pGrid->SelectGetIdx();
			
		pGrid->SelectGetNext();

		ids = pGridDM->SetCellData( _T("EPC"), _T(""), nIndex);
		if( 0 > ids )
		{
			AfxMessageBox( _T("CBO_REG_EPC_SEARCH::CancelEPC() ERROR -11 !!!") );
			return -1;
		}
	}

	this->AllControlDisplay( _T("CM_BO_BOOK_TRADE_MAIN") );

	AfxMessageBox( _T("취소작업을 완료하였습니다.") );
	
	return 0;

EFS_END
	return -1;
}

HBRUSH CBO_REG_EPC_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

int CBO_REG_EPC_MAIN::CreateEPC(CString &strMessage)
{
	int ids = -1;
	CString strFuncName = _T("CreateEPC()");
	
	CESL_DataMgr* pMainDM = FindDM(_T("DM_BO_BOOK_TRADE_MAIN"));
	CESL_Grid* pMainGrid = (CESL_Grid*)FindControl(_T("CM_BO_BOOK_TRADE_MAIN"), _T("MAIN_GRID"));
	
	if(NULL == pMainDM) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	if(NULL == pMainGrid) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

	/////////////////////////////////////////////////////////////////////////
	// EPC를 부여대상 확인
	/////////////////////////////////////////////////////////////////////////
	pMainGrid->SelectMakeList();
	if(0 == pMainGrid->SelectGetCount())
	{
		int nResult = AfxMessageBox(_T("검색된 전체 자료를 일괄부여 하시겠습니까?"), MB_YESNO|MB_ICONQUESTION);
		if(IDYES == nResult)
		{			
			int ROW_COUNT = pMainDM->GetRowCount();		
			for (int i = 0; i < ROW_COUNT; i++)
			{
				pMainGrid->m_arraySelect.AddTail(i);
			}			
		}
		else return 1;
	}
	int nTotalCount = pMainGrid->SelectGetCount();

	m_pDlg_Process->ShowHideCancelButton(TRUE);
	m_pDlg_Process->TogleOKCancelBtn(IDCANCEL);
	m_pDlg_Process->GetProgressCtrl()->SetRange(0, 0);
	m_pDlg_Process->GetProgressCtrl()->SetPos(0);
	m_pDlg_Process->CenterWindow();
	m_pDlg_Process->ResetGrid();
	m_pDlg_Process->ShowWindow(SW_SHOW);
	m_pDlg_Process->UpdateWindow();	


	/////////////////////////////////////////////////////////////////////////
	// EPC를 가져오기 전에 DM을 ISBN으로 정렬시킨다.
	/////////////////////////////////////////////////////////////////////////
	m_pDlg_Process->SetMessage(_T("EPC 부여가 가능한 리스트를 추출하고 있습니다."));

	CESL_DataMgr EPC_DM;
	int nExistEPCancle = 0;
	int nISBNErrorCancle = 0;
	
	ids = MakeSortISBNDM(&EPC_DM, nExistEPCancle, nISBNErrorCancle);
	if(0 > ids) return ids;

	const int RETRY_COUNT = 2;
	const int ISBN_COUNT = EPC_DM.GetRowCount();

	m_pDlg_Process->SetMessage(_T("EPC 부여 작업을 진행중입니다."));
	m_pDlg_Process->GetProgressCtrl()->SetRange(0, ISBN_COUNT);
	
	int nSuccessCount = 0;
	int nEPCErrorCount = 0;
	for(int nRow = 0; nRow < ISBN_COUNT; )
	{
		int nCount = 0;

		if(TRUE == m_bIsCancelThread)
		{
			m_pDlg_Process->SetMessage(_T("취소중입니다."));
			break;
		}
		
		CString strErrorMsg;
		// 실패한경우 한번 더 시도해본다.
		for(int nTry = 0; nTry < RETRY_COUNT; nTry++)
		{			
			ids = CreateEPC(&EPC_DM, nRow, nCount, strErrorMsg);
			if(0 == ids) break;
		}
//		if(0 > ids) StopEPCThread();
		
		if(0 > ids) 
		{
			int nErrorCode = ids;

			CString strTitle;
			ids = EPC_DM.GetCellData(_T("서명"), nRow, strTitle);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

			nEPCErrorCount = nEPCErrorCount + nCount;
			m_pDlg_Process->AddErrorList(nErrorCode, strTitle, strErrorMsg);
		}
		else nSuccessCount = nSuccessCount + nCount;
		
		nRow = nRow + nCount;				

		m_pDlg_Process->GetProgressCtrl()->SetPos(nRow);
	}

	/////////////////////////////////////////////////////////////////////////
	// EPC를 MainDM에 업데이트 시킨다.
	// 화면을 갱신시킨다.
	/////////////////////////////////////////////////////////////////////////
	if(FALSE == m_bIsCancelThread)
	{
		ids = UpdateEPCToMainDM(&EPC_DM);
	}	

	strMessage.Format(
		_T("선정된 자료건수\t\t\t : %d건 \r\n")
		_T("EPC가 존재하여 취소된 자료건수\t : %d건 \r\n")
		_T("ISBN이 존재하지 않아 취소된 자료건수\t : %d건 \r\n")
		_T("실패한 자료건수\t\t\t : %d건 \r\n")
		_T("성공한 자료건수\t\t\t : %d건 \r\n")
		_T("이용자취소된 자료건수\t\t : %d건"),
		nTotalCount,
		nExistEPCancle,
		nISBNErrorCancle,
		nEPCErrorCount,
		nSuccessCount,
		nTotalCount - nExistEPCancle - nISBNErrorCancle - nEPCErrorCount - nSuccessCount
	);

	if(FALSE == m_bIsFreeDLL) m_pDlg_Process->MessageBox(strMessage, _T("EPC부여결과"), MB_OK);
	

	m_pDlg_Process->TogleOKCancelBtn(IDOK);

	return ids;
}
// EPC코드를 얻어온다.
//DEL	PARAM			pSortDM			[IN]	ISBN을 기준으로 정렬된 DM
//DEL					nStartRow		[IN]	같은 ISBN을 가진 열중의 첫번째 열
//DEL					nEpcCount			[OUT]	얻어온 EPC의 갯수
int CBO_REG_EPC_MAIN::CreateEPC(CESL_DataMgr *pSortDM, int nStartRow, int &nEpcCount, CString &strErrorMsg)
{
	int ids = -1;
	CString strFuncName = _T("CreateEPC(CESL_DataMgr *pSortDM, int nStartRow, int &nEndRow)");
	if(NULL == pSortDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	const int ROW_COUNT = pSortDM->GetRowCount();
	if(0 > nStartRow || nStartRow >= ROW_COUNT) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

	// 첫번째 열의 ISBN을 얻어온다.
	CString strISBN;
	ids = pSortDM->GetCellData(_T("ISBN"), nStartRow, strISBN);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

	// 첫째 열과 같은 ISBN을 가진 열수를 계산한다.
	for(int nCnt = nStartRow+1; nCnt < ROW_COUNT; nCnt++)
	{
		CString strGetISBN;
		ids = pSortDM->GetCellData(_T("ISBN"), nCnt, strGetISBN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

		if(strISBN != strGetISBN) break;
	}
	const int GET_EPC_COUNT = nEpcCount = nCnt - nStartRow;

	// EPC를 받아온다.	
	ids = UpdateProc(strISBN, nStartRow, GET_EPC_COUNT, pSortDM, strErrorMsg);
	return ids;

	return 0;
}

// 메인DM을 ISBN을 기준으로 정렬하여 pDstDM으로 얻어온다.
//DEL					pDstDM			[OUT]	정렬된 DM
int CBO_REG_EPC_MAIN::MakeSortISBNDM(CESL_DataMgr *pDstDM, int &nExistEPCancle, int &nISBNErrorCancle)
{
	int ids = -1;
	CString strFuncName = _T("MakeSortISBNDM(CESL_DataMgr *pDstDM)");
	
	CESL_DataMgr* pSrcDM = FindDM(_T("DM_BO_BOOK_TRADE_MAIN"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_BOOK_TRADE_MAIN"), _T("MAIN_GRID"));

	if(NULL == pDstDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
	if(NULL == pSrcDM) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

	pDstDM->SetCONNECTION_INFO(pSrcDM->CONNECTION_INFO);

	////////////////////////////////////////////////////////////////////////////////////
	// 여기서부터 DM복사

	const int ROW_COUNT = pSrcDM->GetRowCount();
	if(0 == ROW_COUNT) return 0;

 	const int COLUMN_CNT = 7;
 	CString strColumn[COLUMN_CNT] = 
	{
		_T("ISBN"), _T("BOOK_KEY"), _T("EPC"), _T("서명"), _T("저자"), _T("발행자"), _T("발행년")
	};
	const enum ALIAS
	{
		ISBN, BOOK_KEY, EPC, TITLE, AUTHOR, PUBLISHER, PUB_YEAR
	};

	INT          nSuccessCount    = 0;

	pDstDM->FreeData();
	pDstDM->FreeDBFieldData();
	pDstDM->FreeDMRef();
	pDstDM->FreeFrame();
	pDstDM->InitDMPKFromFile(_T("DM_BO_BOOK_TRADE_MAIN"));
	

	const int SELECT_ROW_COUNT = pGrid->SelectGetCount();
	if(0 >= SELECT_ROW_COUNT) return 0;
	int idx = pGrid->SelectGetHeadPosition();
	// 열복사
	do
	{
		if(TRUE == m_bIsCancelThread)
		{
			m_pDlg_Process->SetMessage(_T("취소중입니다."));
			break;
		}

		ids = pDstDM->InsertRow(-1);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

		// 행복사
		BOOL bContinue = FALSE;
		for(int nColumnCnt = 0; nColumnCnt < COLUMN_CNT; nColumnCnt++)
		{
			CString strGet;
			ids = pSrcDM->GetCellData(strColumn[nColumnCnt], idx, strGet);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);

			// ISBN이 존재하지 않는 자료나 EPC코드가 이미 부여된 자료는 스킵한다.			
			if(ISBN == nColumnCnt && _T("") == strGet)
			{
				bContinue = TRUE;
				nISBNErrorCancle++;
			}
			else if(EPC == nColumnCnt && _T("") != strGet)
			{
				bContinue = TRUE;
				nExistEPCancle++;
			}			
			if(TRUE == bContinue) break;

			ids = pDstDM->SetCellData(strColumn[nColumnCnt], strGet, pDstDM->GetRowCount()-1);		
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-7, strFuncName);
		}
		if(TRUE == bContinue)
		{
			ids = pDstDM->DeleteRow(pDstDM->GetRowCount()-1);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
		}

		idx = pGrid->SelectGetNext();
	}while(0 <= idx);

	// 여기까지 DM복사
	////////////////////////////////////////////////////////////////////////////////////

	// ISBN으로 정렬시킨다.
	ids = pDstDM->SORT_QUICKSORT(ISBN, ESL_DATAMGR_TYPE_STR, 0, pDstDM->GetRowCount()-1, FALSE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-8, strFuncName);

	return 0;
}

// EPC를 가진 DM의 정보를 이용하여 메인DM의 EPC를 업데이트시킨다.
//DEL					pEpcDM			[OUT]	EPC값을 가지고 있는 DM
int CBO_REG_EPC_MAIN::UpdateEPCToMainDM(CESL_DataMgr *pEpcDM)
{
	int ids = -1;
	CString strFuncName = _T("UpdateEPCToMainDM(CESL_DataMgr *pEpcDM)");
	
	CESL_DataMgr* pMainDM = FindDM(_T("DM_BO_BOOK_TRADE_MAIN"));
	CESL_Grid* pMainGrid = (CESL_Grid*)FindControl(_T("CM_BO_BOOK_TRADE_MAIN"), _T("MAIN_GRID"));

	if(NULL == pEpcDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
	if(NULL == pMainDM) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	if(NULL == pMainGrid) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
	
	const int EPC_DM_COUNT = pEpcDM->GetRowCount();
	const int SELECT_ROW_COUNT = pMainGrid->SelectGetCount();
	if(0 >= SELECT_ROW_COUNT) return 0;
	int idx = pMainGrid->SelectGetHeadPosition();

	
	do
	{
		CString strMainDMBookKey;
		ids = pMainDM->GetCellData(_T("BOOK_KEY"), idx, strMainDMBookKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

		for(int nEpcDMRow = 0; nEpcDMRow < EPC_DM_COUNT; nEpcDMRow++)
		{
			CString strEPCDMBookKey;
			ids = pEpcDM->GetCellData(_T("BOOK_KEY"), nEpcDMRow, strEPCDMBookKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);

			if(strEPCDMBookKey == strMainDMBookKey)
			{
				CString strEPC;
				ids = pEpcDM->GetCellData(_T("EPC"), nEpcDMRow, strEPC);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);

				ids = pMainDM->SetCellData(_T("EPC"), strEPC, idx);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-7, strFuncName);

				break;
			}
		}

		idx = pMainGrid->SelectGetNext();
	}while(0 <= idx);

	return 0;
}


int CBO_REG_EPC_MAIN::RunEPCThread()
{
	if(TRUE == m_bIsRunningSearchThread) return -1;

	m_bIsRunningSearchThread = TRUE;
	m_bIsCancelThread = FALSE;

	CWinThread * Thread;
	Thread = AfxBeginThread(EPCThread, this);	
	
	return 0;
}

int CBO_REG_EPC_MAIN::StopEPCThread()
{
	if(FALSE == m_bIsRunningSearchThread) return -1;
	m_bIsCancelThread = TRUE;

	return 0;
}

UINT CBO_REG_EPC_MAIN::EPCThread(LPVOID pParam)
{
	CBO_REG_EPC_MAIN* pDlg = (CBO_REG_EPC_MAIN*)pParam;

	CString strMessage;
	int ids = pDlg->CreateEPC(strMessage);
	if(0 > ids)
	{		
		pDlg->SetTimer(IDT_FAIL_SEARCHTHREAD, 100, NULL);
		return ids;
	}

	pDlg->SetTimer(IDT_END_SEARCHTHREAD, 100, NULL);
		
	return 0;
}

void CBO_REG_EPC_MAIN::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	// 쓰레드가 종료된 경우
	// 정상적으로 종료된 경우든 실패한 경우든 지금까지 검색된 그리드를 표시시킨다.
	// DM과 GRID의 내용이 일치하지 않으면 이후의 처리에 문제발생 가능성이 있기 때문
	if(IDT_END_SEARCHTHREAD == nIDEvent || IDT_FAIL_SEARCHTHREAD == nIDEvent)
	{	
		KillTimer(nIDEvent);
		
		m_bIsRunningSearchThread = FALSE;
		m_bIsCancelThread = FALSE;
		
//DEL 		if(FALSE == m_bIsFreeDLL) m_pDlg_Process->ShowWindow(SW_HIDE);
 		if(FALSE == m_bIsFreeDLL) AllControlDisplay(_T("CM_BO_BOOK_TRADE_MAIN"));
	}
//DEL  	// 쓰레드가 동작중이라 다이얼로그 닫기가 연기된 경우
//DEL  	else if(IDT_DELAY_CLOSE_DIALOG == nIDEvent)
//DEL  	{
//DEL  		KillTimer(IDT_DELAY_CLOSE_DIALOG);
//DEL  		OnClose();
//DEL  	}	

	CDialog::OnTimer(nIDEvent);
}

//DEL void CBO_REG_EPC_MAIN::OnClose() 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	// TODO: Add extra cleanup here
//DEL 	// 쓰레드가 실행중일 경우 쓰레드를 멈추고, 다이얼로그 닫기를 연기시킨다.
//DEL 	if(TRUE == m_bIsRunningSearchThread)
//DEL 	{
//DEL 		StopEPCThread();
//DEL 		SetTimer(IDT_DELAY_CLOSE_DIALOG, 1000, NULL);
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	CDialog::OnClose();
//DEL }

int CBO_REG_EPC_MAIN::GetErrorMsg(CString strCodeAlias, int nErrorCode, CString &strMessage)
{
	if(_T("ISSUEID") == strCodeAlias)
	{
		switch(nErrorCode)
		{
		case -1:
			strMessage = _T("EPC부여 : ISBN의 자리수가 올바르지 않습니다.");
			break;
		case -2:
			strMessage = _T("EPC부여 : 국가코드가 89가 아닙니다. 혹은 13자리 ISBN변환에 실패하였습니다.");
			break;
		case -3:
			strMessage = _T("EPC부여 : 국가코드가 89가 아닙니다. 혹은 ISBN변환에 실패하였습니다.");
			break;
		case -4:
			strMessage = _T("EPC부여 : 요청파일을 불러오는 데, 실패하였습니다.");
			break;
		case -5:
			strMessage = _T("EPC부여 : 서버와 연결을 할 수 없습니다.");
			break;
		case -6:
			strMessage = _T("EPC부여 : Result XML파일을 로드할 수 없었습니다.");
			break;
		case -7:
			strMessage = _T("EPC부여 : EPC 코드를 받아오지 못했습니다.");
			break;
		default:
			strMessage = _T("IssueID 작업중 오류가 발생하였습니다.");
			break;
		}
	}
	else if(_T("MAKE_NEW_INFO") == strCodeAlias)
	{
		switch(nErrorCode)
		{
		case -4:
			strMessage = _T("목록구축 : 요청파일을 불러오는 데, 실패하였습니다.");
			break;
		case -5:
			strMessage = _T("목록구축 : 서버와 연결을 할 수 없습니다.");
			break;
		case -6:
			strMessage = _T("목록구축 : 결과파일을 불러오는 데, 실패하였습니다.");
			break;
		default:
			strMessage = _T("목록구축 작업중 오류가 발생하였습니다.");
			break;
		}
	}
	else if(_T("EPC_CHECK_IN") == strCodeAlias)
	{
		switch(nErrorCode)
		{
		case -1:
			strMessage = _T("ItemReceipt : 도서관부호를 찾지 못하였습니다.");
			break;
		case -2:
			strMessage = _T("ItemReceipt : EPC코드를 찾지 못하였습니다.");
			break;
		case -3:
			strMessage = _T("ItemReceipt : ISBN 변환에 실패하였습니다.");
			break;
		case -4:
			strMessage = _T("ItemReceipt : Query XML파일을 로드할 수 없었습니다.");
			break;
		case -5:
			strMessage = _T("ItemReceipt : 서버와 연결을 할 수 없었습니다.");
			break;
		case -6:
			strMessage = _T("ItemReceipt : Result XML파일을 로드할 수 없었습니다.");
			break;
		default:
			strMessage = _T("ItemReceipt 작업중 에러가 발생하였습니다.");
			break;
		}
	}

	return 0;
}

BOOL CBO_REG_EPC_MAIN::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
//DEL 		if(VK_F5 == pMsg->wParam)
//DEL 		{
//DEL 			CreateEPC();
//DEL 			return TRUE;
//DEL 		}
//DEL 		else if(VK_F6 == pMsg->wParam)
//DEL 		{
//DEL 			RunEPCThread();
//DEL 			return TRUE;
//DEL 		}
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);
}
