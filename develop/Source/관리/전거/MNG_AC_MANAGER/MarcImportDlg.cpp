// FileImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcImportDlg.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

#include "efs.h"

#ifndef OBM_SIZE
#define	OBM_SIZE		32766
// taken from WinresRc.h
// if not used for any reason
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcImportDlg dialog

CMarcImportDlg::CMarcImportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcImportDlg)
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT

	m_pThreadImport		= NULL;
	m_pThreadDisplay	= NULL;
	m_pParent			= pParent;
	m_bIsThreadRunning	= FALSE;

	m_pDM_TOTAL			= NULL;
	m_pDM_OK			= NULL;
	m_pDM_ERR			= NULL;
	m_pDM_SUCCESS		= NULL;

	m_pGrid_OK			= NULL;
	m_pGrid_ERR			= NULL;
	m_pGrid_SUCCESS		= NULL;

	m_lpszSMAlias		= _T("SM_정리_전거마크반입");
	m_lpszCMAlias		= _T("CM_정리_전거마크반입");
	
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));
}

CMarcImportDlg::~CMarcImportDlg()
{
}

BOOL CMarcImportDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CMarcImportDlg::CopyToMain()
{
	INT ids = m_pParent->SendMessage(IDS_MAKEDM,0,0);
	if(ids < 0)
		return;
}
INT CMarcImportDlg::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	EFS_BEGIN

	if (pDM_From == NULL || pDM_To == NULL) return -1;

	INT ids = -1;
	INT nFind = -1;

	INT nRowCount = pDM_From->GetRowCount();
	if (nRowCount - 1 < nFromIdx || nFromIdx < 0) return -1;

	nRowCount = pDM_To->GetRowCount();
	if (nRowCount - 1 < nToIdx ) return -1;

	if (nToIdx < 0)
	{
		ids = pDM_To->InsertRow(-1);
		if (ids < 0) return -1;
		nToIdx = pDM_To->GetRowCount() - 1;
	}	

	BOOL bSwitchHeader = FALSE;
	
	if (!strToAliasHeader.IsEmpty())
		bSwitchHeader = TRUE;
	
	CString strAlias;
	CString strItemData;

	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;

		strAlias = pRef->FIELD_ALIAS;

		// 거부 리스트에 있는지 확인
		// Default Ignore List
//		if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
//		if ( strAlias == _T("IBS_배가위치부호") ) continue;
		// User Ignore List
//		if (pArrIgnoreAlias != NULL)
//			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		
		ids = pDM_From->GetCellData(strAlias, nFromIdx, strItemData);
		if (ids < 0) continue;

		// 헤더 변경
		if (bSwitchHeader)
		{
			nFind = strAlias.Find(_T("_"));
			if (nFind >= 0)
				strAlias = strAlias.Mid(nFind + 1);
			strAlias = strToAliasHeader + _T("_") + strAlias;
		}
		/*
		if ( strAlias == _T("BB_권책기호") || strAlias == _T("BB_편권차") ) continue;
		*/
		if ( strAlias == _T("BB_등록구분_코드") ) continue;
		ids = pDM_To->SetCellData(strAlias, strItemData, nToIdx);
		if (ids < 0) continue;
	}

	return nToIdx;

	EFS_END
	return -1;
}

VOID CMarcImportDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcImportDlg)
	DDX_Text(pDX, IDC_eFILE_PATH, m_strFilePath);
	//}}AFX_DATA_MAP


	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcImportDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcImportDlg)
	ON_BN_CLICKED(IDC_bFILE_OPEN, OnbFILEOPEN)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bIMPORT, OnbIMPORT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMPORT, OnSelchangeTabImport)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bEXPORT_EDITMARC, OnbEXPORTEDITMARC)
	ON_BN_CLICKED(IDC_bPrintRevision, OnbPrintRevision)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_END_MARC_ADJUST, OnEndMarcAdjust)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////
// THREAD
// 홍수
UINT ThreadFunc_Import(LPVOID pParam)
{
	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
	if (pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	pDlg->m_bIsThreadRunning = TRUE;
	pDlg->ButtonStatus(0);

	// Initialization Variable
	CESL_ControlMgr *pCM 	 = NULL;
	CESL_Grid *pGrid_OK		 = NULL;
	CESL_Grid *pGrid_ERR	 = NULL;
	CESL_Grid *pGrid_SUCCESS = NULL;

	// 옮길 그리드
	CESL_Grid *pGrid_TO = NULL;
	// 개수증가시킬 ID
	UINT nID_TO = -1;
	// 개수증가시킬 String
	CString strTO;


	INT ids = -1;
	//INT idxDM = -1;
	INT nCount = -1;
	INT nColCount = -1;

	INT nRowTo = 0;
	INT nRowsTo = 0;

	CString strData;
	CString strErrorMsg;

	//2003.10.20 이학중 수정 +++++++++++++++++++++++++++++++++
    CString nTotalCnt ;
    CString strTotalMsg ;
    CString nFalseCnt ;
	CString nTrueCnt  ;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// 그리드 설정
	pGrid_OK = pDlg->m_pGrid_OK;
	pGrid_ERR = pDlg->m_pGrid_ERR;	
	pGrid_SUCCESS = pDlg->m_pGrid_SUCCESS;
	
	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
		goto ERR;

	// 선택한 자료만 반입한다.
	
	// GRID OK Count
	nCount = pGrid_OK->SelectGetCount();
	if (nCount <= 0)
	{
		goto ERR;
	}

	pDlg->m_wndProgressBar.SetRange(0, nCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));

	nColCount = pGrid_OK->GetCols(0);
	
	while(TRUE)
	{
		if (!pDlg->m_bIsThreadRunning) break;
		strErrorMsg.Empty();
		pGrid_OK->SelectMakeList();
		INT idx = pGrid_OK->SelectGetHeadPosition() + 1;
		if (idx <= 0) break;

		INT iIdxDM = idx -1;
		CString sStreamMarc;
		pDlg->m_pDM_OK->GetCellData(_T("MARC"), iIdxDM, sStreamMarc);

	
		//반입된 MARC로 저장
		ids = pDlg->InsertToDB(sStreamMarc);

		if (ids < 0)
		{
			pGrid_TO = pGrid_ERR;
			nID_TO = 2;
			strTO = _T("오류 : ");
		}
		else
		{
			pGrid_TO = pGrid_SUCCESS;
			nID_TO = 3;
			strTO = _T("성공 : ");

			pDlg->m_pDM_SUCCESS->InsertRow(-1);
			INT nRowIdx = pDlg->m_pDM_SUCCESS->GetRowCount() - 1;			
			pDlg->CopyDM(pDlg->m_pDM_OK, iIdxDM, pDlg->m_pDM_SUCCESS, nRowIdx);
			pDlg->m_pDM_OK->DeleteRow(iIdxDM);
			//메인그리드에 뿌려준다.
			pDlg->CopyToMain();
		}
	
		// 개수를 설정한다.
		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
		strData = strData.Mid(strData.ReverseFind(':') + 1);
		strData.Format(_T("%d"), _ttoi(strData) - 1);
		pDlg->m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
		
	

		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
		strData = strData.Mid(strData.ReverseFind(':') + 1);
		strData.Format(_T("%d"), _ttoi(strData) + 1);
		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);


		//2003.10.15 이학중 수정
        nTotalCnt =  strTO+strData ; 
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// 그리드의 데이터를 옮긴다.
		nRowsTo = pGrid_TO->GetRows();
		if (pGrid_TO->GetTextMatrix(nRowsTo - 1, 0).IsEmpty())
		{
			nRowTo = nRowsTo - 1;
		}
		else
		{
			pGrid_TO->SetRows(nRowsTo + 1);
			nRowTo = nRowsTo;
		}

		for (INT col = 0; col < nColCount; col++)
		{
			pGrid_TO->SetTextMatrix(nRowTo, col, pGrid_OK->GetTextMatrix(idx, col));
		}

		// 오류 가 있으면 오류 세팅
		if (!strErrorMsg.IsEmpty())
		{
			pDlg->m_mapGridAlias.Lookup(_T("BS_파일반입오류"), col);
			pGrid_TO->SetTextMatrix(nRowTo, col, strErrorMsg);
		}

		// 프로그래스바 올려주기...
		pDlg->m_wndProgressBar.StepIt();

		// Remove Row
		if (pGrid_OK->GetRows() == 2)
		{
			pGrid_OK->Clear();
			pGrid_OK->SetColumnInfo();
			break;
		}
		else
		{
			pGrid_OK->RemoveItem(idx);
		}
	}
	
	pDlg->m_wndProgressBar.SetWindowText(_T("반입하였습니다."));
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.10.20 이학중 수정 

    nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
    nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
	nTrueCnt  =  pDlg->m_wndStatusBarCtrl.GetText(3) ;

	strTotalMsg.Format( _T("반입하였습니다. \r\n\r\n %s건 \r\n %s건 \r\n %s건"),nTotalCnt,nFalseCnt,nTrueCnt )  ; 	

	pGrid_OK->DisplayNumber();
	pGrid_ERR->DisplayNumber();
	pGrid_SUCCESS->DisplayNumber();

	//++++++++++++++++++++++++++++++++++++++

	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	SetEvent(pDlg->m_hThread);

	return 0;

ERR:
	pDlg->m_wndProgressBar.SetWindowText(_T("반입 실패"));
	AfxMessageBox(_T("반입 실패"));
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	return -1;

return -1;
}


/// 마크를 읽어서 그리드에 뿌려주는 쓰레드
UINT ThreadFunc_Display(LPVOID pParam)
{
	EFS_BEGIN

	//메인 다이얼로그 포인터를 받는다. 
	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
	if (pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	//쓰레드 러닝중으로 세팅
	pDlg->m_bIsThreadRunning = TRUE;
	//쓰레드 러닝중인 버튼상태로 세팅
	pDlg->ButtonStatus(0);

	// Initialization Variable
	//CESL_DataMgr *pDM_ACMarc= NULL;
	
	CESL_ControlMgr *pCM = NULL;

	CESL_Grid *pGrid_OK			= NULL;
	CESL_Grid *pGrid_ERR		= NULL;
	CESL_Grid *pGrid_SUCCESS	= NULL;

	// For Count
	CString strCount;
	INT i = 0;
	INT nTotalCount = -1;
	INT nErrCount = 0;
	INT nOKCount = 0;

	// For Fetch
	INT row = 0;
	INT nGridRow = 0;
	INT nErrGridRow = 0;
	INT nOKGridRow = 0;
	CESL_Grid *pGrid = NULL;

	POSITION pos = NULL;
	CString strGridAlias;
	INT nGridCol = -1;

	// Start...

	//파일로부터 읽어 구축된 DM의 로우개수, 전체수
	nTotalCount = pDlg->m_pDM_TOTAL->GetRowCount();
	nOKCount = pDlg->m_pDM_OK->GetRowCount();
	nErrCount = pDlg->m_pDM_ERR->GetRowCount();
	
	pCM = pDlg->FindCM(pDlg->m_lpszCMAlias);
	if (pCM == NULL)
	{
		goto ERR;
	}

	// 전체 반입된 개수
	strCount.Format(_T("전체 : %d"), nTotalCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);

	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));
		
/*	
	for (i = 0; i < nTotalCount; i++)
	{
		// ERR_COUNT
		INT iCheckSyntax = -1;
		CString sTmpStreamMarc;
		CMarc *pMarc = NULL ;
		pMarc = new CMarc;
		
		pDlg->m_pDM_TOTAL->GetCellData(_T("MARC"), i , sTmpStreamMarc);	

		pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, pMarc);

		//마크 문법검사
		iCheckSyntax = pDlg->m_pInfo->pMarcMgr->CheckSimpleSyntax(pMarc);
		if ( iCheckSyntax < 0 ) nErrCount++;

		delete pMarc;		
	}
*/
	strCount.Format(_T("오류 : %d"), nErrCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);
		
	// OK_COUNT
	nOKCount = nTotalCount - nErrCount;
	strCount.Format(_T("정상 : %d"), nOKCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);
	
	// SUCCESS_COUNT
	pDlg->m_wndStatusBarCtrl.SetText(_T("성공 : 0"), 3, 0);


	pGrid_OK		= pDlg->m_pGrid_OK;
	pGrid_ERR		= pDlg->m_pGrid_ERR;
	pGrid_SUCCESS	= pDlg->m_pGrid_SUCCESS;
	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
	{
		goto ERR;
	}

	// 그리드를 기본값으로 세팅한다. 
	pGrid_OK->Clear();
	pGrid_OK->SetRows(2);
	pGrid_OK->SetColumnInfo();

	pGrid_ERR->Clear();
	pGrid_ERR->SetRows(2);
	pGrid_ERR->SetColumnInfo();

	pGrid_SUCCESS->Clear();
	pGrid_SUCCESS->SetRows(2);
	pGrid_SUCCESS->SetColumnInfo();


	// 정상/오류그리드의 로우수를 세팅한다. 
	if (nErrCount > 0)
		pGrid_ERR->SetRows(nErrCount + 1);
	if (nOKCount > 0)
		pGrid_OK->SetRows(nOKCount + 1);

	// OK 그리드의 값을 세팅한다. 
	for (row = 0; row < nOKCount; row++)
	{
		pDlg->m_wndProgressBar.StepIt();

		pGrid = pGrid_OK;
		
		strCount.Format(_T("%d"), row);
	
		CString sTmpData = _T("") ;
		pDlg->m_pDM_OK->GetCellData(_T("전거표목"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 1, sTmpData );

		sTmpData = _T("") ;
		pDlg->m_pDM_OK->GetCellData(_T("생몰년"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 2, sTmpData );

		sTmpData = _T("") ;
		pDlg->m_pDM_OK->GetCellData(_T("주기"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 3, sTmpData );	

		sTmpData = _T("") ;
		pDlg->m_pDM_OK->GetCellData(_T("복본개수"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 4, sTmpData );	

		nGridRow = ++nOKGridRow;
		
		if (!pDlg->m_bIsThreadRunning) break;
	}

	// ERROR 그리드의 값을 세팅한다. 
	for (row = 0; row < nErrCount; row++)
	{
		pDlg->m_wndProgressBar.StepIt();

		pGrid = pGrid_ERR;
				
		CString sTmpData = _T("") ;
		pDlg->m_pDM_ERR->GetCellData(_T("전거표목"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 1, sTmpData );

		sTmpData = _T("") ;
		pDlg->m_pDM_ERR->GetCellData(_T("생몰년"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 2, sTmpData );

		sTmpData = _T("") ;
		pDlg->m_pDM_ERR->GetCellData(_T("주기"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 3, sTmpData );	

		sTmpData = _T("") ;
		pDlg->m_pDM_ERR->GetCellData(_T("복본개수"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 4, sTmpData );	

		nGridRow = ++nErrGridRow;
		
		if (!pDlg->m_bIsThreadRunning) break;
	}

	pDlg->m_wndProgressBar.SetWindowText(_T("파일패치 완료"));

	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);

	SetEvent(pDlg->m_hThread);
	
	return 0;

ERR:
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);

	return -1;
	
	EFS_END
	return -1;
}


INT CMarcImportDlg::DataDisplay()
{
	EFS_BEGIN

	ButtonStatus(0);

	// For Count
	CString strCount;
	INT i = 0;
	INT nTotalCount = -1;
	INT nErrCount = 0;
	INT nOKCount = 0;

	// For Fetch
	INT row = 0;
	INT nGridRow = 0;
	INT nErrGridRow = 0;
	INT nOKGridRow = 0;
	CESL_Grid *pGrid = NULL;

	POSITION pos = NULL;
	CString strGridAlias;
	INT nGridCol = -1;

	// Start...

	//파일로부터 읽어 구축된 DM의 로우개수, 전체수
	nTotalCount = m_pDM_TOTAL->GetRowCount();
	nOKCount = m_pDM_OK->GetRowCount();
	nErrCount = m_pDM_ERR->GetRowCount();
	
	// 전체 반입된 개수
	strCount.Format(_T("전체 : %d"), nTotalCount);
	m_wndStatusBarCtrl.SetText(strCount, 0, 0);

	m_wndProgressBar.SetRange(0, nTotalCount);
	m_wndProgressBar.SetPos(0);
	m_wndProgressBar.SetWindowText(_T(""));
		
	strCount.Format(_T("오류 : %d"), nErrCount);
	m_wndStatusBarCtrl.SetText(strCount, 2, 0);
		
	// OK_COUNT
	nOKCount = nTotalCount - nErrCount;
	strCount.Format(_T("정상 : %d"), nOKCount);
	m_wndStatusBarCtrl.SetText(strCount, 1, 0);
	
	// SUCCESS_COUNT
	m_wndStatusBarCtrl.SetText(_T("성공 : 0"), 3, 0);


	// 그리드를 기본값으로 세팅한다. 
	m_pGrid_OK->Clear();
	m_pGrid_OK->SetRows(2);
	m_pGrid_OK->SetColumnInfo();

	m_pGrid_ERR->Clear();
	m_pGrid_ERR->SetRows(2);
	m_pGrid_ERR->SetColumnInfo();

	m_pGrid_SUCCESS->Clear();
	m_pGrid_SUCCESS->SetRows(2);
	m_pGrid_SUCCESS->SetColumnInfo();


	// 정상/오류그리드의 로우수를 세팅한다. 
	if (nErrCount > 0)
		m_pGrid_ERR->SetRows(nErrCount + 1);
	if (nOKCount > 0)
		m_pGrid_OK->SetRows(nOKCount + 1);

	// OK 그리드의 값을 세팅한다. 
	for (row = 0; row < nOKCount; row++)
	{
		m_wndProgressBar.StepIt();
		
		strCount.Format(_T("%d"), row);
		
		CString sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("전거표목"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 1, sTmpData );

		sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("생몰년"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 2, sTmpData );

		sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("주기"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 3, sTmpData );	

		sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("필수태그"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 4, sTmpData );

		sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("복본개수"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 5, sTmpData );	

		nGridRow = ++nOKGridRow;
	}

	// ERROR 그리드의 값을 세팅한다. 
	for (row = 0; row < nErrCount; row++)
	{
		m_wndProgressBar.StepIt();
		
		CString sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("전거표목"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 1, sTmpData );

		sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("생몰년"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 2, sTmpData );

		sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("주기"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 3, sTmpData );	

		sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("필수태그"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 4, sTmpData );

		sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("복본개수"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 5, sTmpData );	

		nGridRow = ++nErrGridRow;
	}

	m_wndProgressBar.SetWindowText(_T("파일패치 완료"));

	ButtonStatus(1);

	m_pCM->AllControlDisplay();

	return 0;
	EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcImportDlg Operations

INT CMarcImportDlg::Init()
{
	EFS_BEGIN

	// 전거 MASTER DM Alias
	m_lpszACDMAlias = _T("DM_정리_전거마크반입");
	m_pInfo->pMarcMgr->SetMarcGroup(_T("전거"));
	SetWindowText(_T("전거 마크 반입"));

/*	m_pDM_TOTAL = new CESL_DataMgr;
	if (m_pDM_TOTAL == NULL) return -1;

	m_pDM_TOTAL->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_TOTAL->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_TOTAL->InitDMPKFromFile(m_lpszACDMAlias);
*/

	m_pDM_TOTAL = FindDM(_T("DM_정리_전거마크반입") );
	if( m_pDM_TOTAL == NULL )
	{
		AfxMessageBox(_T("DM_정리_전거마크반입 not found!!") );
		return -1;
	}

	m_pDM_OK = new CESL_DataMgr;
	if (m_pDM_OK == NULL) return -1;

	m_pDM_OK->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_OK->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_ERR = new CESL_DataMgr;
	if (m_pDM_ERR == NULL) return -1;

	m_pDM_ERR->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_ERR->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_SUCCESS = new CESL_DataMgr;
	if (m_pDM_SUCCESS == NULL) return -1;

	m_pDM_SUCCESS->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_SUCCESS->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_OK->CopyDM(m_pDM_TOTAL);
	m_pDM_ERR->CopyDM(m_pDM_TOTAL);
	m_pDM_SUCCESS->CopyDM(m_pDM_TOTAL);

	InitDMAlias(m_pDM_OK);
	InitDMAlias(m_pDM_ERR);

	// CM
	m_pCM = FindCM(m_lpszCMAlias);
	if (m_pCM == NULL)
	{
		AfxMessageBox(_T("CM_정리_전거마크반입 not found!!") );
		return -1;
	}

	// Grid
	m_pGrid_OK = (CESL_Grid*)m_pCM->FindControl(_T("Grid_OK"));
	// ERROR GRID
	m_pGrid_ERR = (CESL_Grid*)m_pCM->FindControl(_T("Grid_ERR"));
	// SUCCESS GRID
	m_pGrid_SUCCESS = (CESL_Grid*)m_pCM->FindControl(_T("Grid_SUCCESS"));

	if (m_pGrid_OK == NULL || m_pGrid_ERR == NULL || m_pGrid_SUCCESS == NULL) return -1;


	// Move Windows
	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);
	
	m_pGrid_OK->MoveWindow(rcGrid);
	m_pGrid_ERR->MoveWindow(rcGrid);
	m_pGrid_SUCCESS->MoveWindow(rcGrid);

	return 0;


EFS_END
return -1;
}

INT CMarcImportDlg::InitDMAlias(CESL_DataMgr * pDM)
{
	pDM->InitDataMgrRef(17);
	pDM->SetDataMgrRef(0,	_T("AC_CONTROL_NO"),	_T("AC_CONTROL_NO"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(1,	_T("MARC"),				_T("MARC"),				_T("INT"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(2,	_T("REC_KEY"),	_T("REC_KEY"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(3,	_T("AC_TYPE"),	_T("AC_TYPE"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(4,	_T("AC_CLASS"),	_T("AC_CLASS"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(5,	_T("SEE_ALSO_REFERENCE_YN"),	_T("SEE_ALSO_REFERENCE_YN"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(6,	_T("INPUT_DATE"),	_T("INPUT_DATE"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(7,	_T("UPDATE_DATE"),	_T("UPDATE_DATE"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(8,	_T("FIRST_WORKER"),	_T("FIRST_WORKER"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(9,	_T("LAST_WORKER"),	_T("LAST_WORKER"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(10,	_T("FIRST_WORK"),	_T("FIRST_WORK"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(11,	_T("LAST_WORK"),	_T("LAST_WORK"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(12,	_T("전거표목"),		_T("전거표목"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(13,	_T("생몰년"),		_T("생몰년"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(14,	_T("주기"),			_T("주기"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(15,	_T("필수태그"),		_T("필수태그"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(16,	_T("복본개수"),		_T("복본개수"),	_T("str"),	_T(""),	_T(""));

	return 0;
}

// Mode : 0 작업진행중
// Mode : 1 작업완료
VOID CMarcImportDlg::ButtonStatus(INT nMode)
{
	EFS_BEGIN

	const INT nIDCount = 4;
	INT nArrID[nIDCount] = {
		IDC_bFILE_OPEN, IDC_bEXPORT,
		IDC_bIMPORT, IDC_bEXPORT
	};

	for (INT i = 0; i < nIDCount; i++)
	{
		if (nMode == 0)
		{
			GetDlgItem(nArrID[i])->EnableWindow(FALSE);
		}
		else if (nMode == 1)
		{
			GetDlgItem(nArrID[i])->EnableWindow(TRUE);
		}
	}

	if (nMode == 0)
	{
		GetDlgItem(IDC_bCLOSE)->SetWindowText(_T("중지"));
	}
	else if (nMode == 1)
	{
		GetDlgItem(IDC_bCLOSE)->SetWindowText(_T("닫기"));
	}


	EFS_END
}

//파일로부터 마크 반입을 시작한다. 
//
INT CMarcImportDlg::ImportMarcFromFile()
{
	EFS_BEGIN

	BeginWaitCursor();
	INT ids;
	m_pDM_TOTAL->FreeData();


	if (m_strFilePath.IsEmpty()) return -1;

	ids = ImportFile();

	// Display Thread Start
	if (ids >= 0)		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
//	ids = DataDisplay();

	EndWaitCursor();
	return ids;

EFS_END
return -1;
}

INT CMarcImportDlg::ExportToTxtFile(CString strFilePath)
{
	EFS_BEGIN

	BeginWaitCursor();

	// Deterime What Grid is Selected
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return -1;

	INT nSel = pTabCtrl->GetCurSel();
	CESL_Grid *pGrid = NULL;

	// OK GRID
	if (nSel == 0)
		pGrid = m_pGrid_OK;
	// ERROR GRID
	else if (nSel == 1)
		pGrid = m_pGrid_ERR;
	// SUCCESS GRID
	else if (nSel == 2)
		pGrid = m_pGrid_SUCCESS;
	
	if (pGrid == NULL) return -1;


	if (pGrid->GetRows() == 2)
	{
		if (pGrid->GetTextMatrix(1, 0).IsEmpty())
		{
			AfxMessageBox(_T("반출할 자료가 없습니다."));
			return -1;
		}
	}

	// File
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	

	CString strErrMarc;
	INT nCount = pGrid->GetCount();
	for (INT i = 0; i < nCount; i++)
	{
		strErrMarc = m_pDM_TOTAL->GetCellData(_T("MARC"), _ttoi(pGrid->GetTextMatrix(i + 1, 0)));
		if (strErrMarc.IsEmpty()) continue;

		strErrMarc += _T("\n");

		file.WriteString(strErrMarc);
	}

	file.Close();

	EndWaitCursor();

	AfxMessageBox(_T("반출 완료"));

	return 0;

	return -1;

	EFS_END
	return -1;
}


INT CMarcImportDlg::CallBackCheckMarc(CMarc *pMarc, CString &strCheckErrMsg)
{
	EFS_BEGIN

	if (pMarc == NULL) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nCount = -1;
	CString strItem;
	CString strTemp;
	CString strItemData;
	CString strErrMsg;
	CStringArray arrStrErrMsg;
	CArray<CString, CString&> arrItem;

	strCheckErrMsg.Empty();

	CStringArray arrStrEssentialTag;
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 008 입력일자가 없으면 넣어준다.
	ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@0-5"), strItemData);
	CIndex::TrimAll(strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		strItemData = CTime::GetCurrentTime().Format(_T("%y%m%d"));
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@0-5"), strItemData);
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//전거에서 있어야 함
	// 최종수정일자 생성 005 무조건 갱신..
	CTime time = CTime::GetCurrentTime();	
	strItem.Format(_T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	m_pInfo->pMarcMgr->DeleteField(pMarc, _T("005"));
	m_pInfo->pMarcMgr->InsertField(pMarc, _T("005") + strItem);
	
	nCount = arrStrErrMsg.GetSize();
	for (idx = 0; idx < nCount; idx ++)
	{
		strCheckErrMsg += arrStrErrMsg.GetAt(idx) + _T("\n");
	}

	strCheckErrMsg.TrimLeft();
	strCheckErrMsg.TrimRight();
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if (nCount > 0)
		return -1;
	else
		return 0;

	EFS_END
	return -1;
}

BOOL CMarcImportDlg::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();


	if (InitESL_Mgr(m_lpszSMAlias) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if (Init() < 0)
	{
		AfxMessageBox(_T("Initialization is Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	GetWindowRect(m_rcInitial);
	
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl)
	{
		pTabCtrl->InsertItem(0, _T("정상"));
		pTabCtrl->InsertItem(1, _T("오류"));
		pTabCtrl->InsertItem(2, _T("성공"));

		pTabCtrl->SetCurFocus(0);
	}

	// Status Bar
	m_wndStatusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);
	m_wndStatusBarCtrl.SetMinHeight(22);
	
	if (!m_wndProgressBar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(0,0,100,100), &m_wndStatusBarCtrl, 17298))
		return -1;

	m_wndProgressBar.SetFont(GetFont());

	const INT nParts = 5;
	CRect rect;
	m_wndStatusBarCtrl.GetClientRect(&rect);
	INT widths[nParts] = { 100, 200, 300, 400, -1 };
	m_wndStatusBarCtrl.SetParts(nParts, widths);

	m_pGrid_ERR->ShowWindow(SW_HIDE);
	m_pGrid_SUCCESS->ShowWindow(SW_HIDE);
	m_pGrid_OK->ShowWindow(SW_SHOW);

	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

VOID CMarcImportDlg::OnbCLOSE() 
{
	EFS_BEGIN

	CString strText;

	GetDlgItem(IDC_bCLOSE)->GetWindowText(strText);

	if (strText == _T("닫기"))
	{

		// Thread Check
		if (m_bIsThreadRunning)
		{
			m_bIsThreadRunning = FALSE;

			MSG msg;
			while (TRUE)
			{
				if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
					AfxGetApp()->PumpMessage();

				if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
					break;
			}
		}
		CDialog::OnCancel();
	}
	else if (strText == _T("중지"))
	{
		m_bIsThreadRunning = FALSE;
	}

	EFS_END
}



VOID CMarcImportDlg::OnSelchangeTabImport(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();

	CMSHFlexGrid *pGridNormal = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_OK);
	CMSHFlexGrid *pGridError  = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_ERR);
	CMSHFlexGrid *pGridSuccess  = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_SUCCESS);

	if (pGridNormal == NULL || pGridError == NULL || pGridSuccess == NULL) return;
	
	if (nCurSel == 0)
	{
		pGridNormal->ShowWindow(SW_SHOW);
		pGridError->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 1)
	{
		pGridError->ShowWindow(SW_SHOW);
		pGridNormal->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 2)
	{
		pGridError->ShowWindow(SW_HIDE);
		pGridNormal->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_SHOW);
	}
		
	*pResult = 0;

	EFS_END
}

VOID CMarcImportDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL) return;

	CRect rcWnd;
	GetWindowRect(rcWnd);

	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	

		MoveWindow(rcWnd);
		return;
	}

	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			

		MoveWindow(rcWnd);
		return;
	}

	INT i = 0;
	CRect rcClient;
	CWnd *pWnd = NULL;

	CRect rcDlg;
	GetClientRect(rcDlg);

	// Tab Ctrl
	pWnd = GetDlgItem(IDC_TAB_IMPORT);
	if (pWnd)
	{
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);

		rcClient.right = rcDlg.right - 10;
		rcClient.bottom = rcDlg.bottom - 24;

		pWnd->MoveWindow(rcClient);
	}

	// Picture Ctrl
	pWnd = GetDlgItem(IDC_STATIC_GRID);
	if (pWnd)
	{
		rcClient.DeflateRect(5, 5);
		rcClient.top += 20;
		pWnd->MoveWindow(rcClient);
	}

		
	// Grids
	const INT nResizeControlCnt = 3;
	INT nArrayResizeControl[nResizeControlCnt] = {
		IDC_MSHFLEXGRID_OK, IDC_MSHFLEXGRID_ERR, 
		IDC_MSHFLEXGRID_SUCCESS, 
	};	
	
	for (i = 0; i < nResizeControlCnt; i++)
	{
		pWnd = GetDlgItem(nArrayResizeControl[i]);
		if (pWnd != NULL)
		{
			pWnd->MoveWindow(rcClient);
		}
	}

	// StatusBar Ctrl
	if (m_wndStatusBarCtrl.GetSafeHwnd() != NULL)
		m_wndStatusBarCtrl.SendMessage(WM_SIZE, 0, 0);

	// ProgressBar
	if(!::IsWindow(m_wndProgressBar.m_hWnd))
        return;

	CRect   rc;
    m_wndStatusBarCtrl.GetRect(4,rc);
	rc.top -=1;
	rc.bottom +=5;
    
    m_wndProgressBar.SetWindowPos(NULL,rc.left,
                                 rc.top-1,
                                 rc.Width(),
                                 rc.Height()+2,SWP_NOZORDER | SWP_NOACTIVATE);

	m_wndProgressBar.GetClientRect(rc);
	HRGN hrgn = CreateRectRgn(rc.left+2, rc.top+4, rc.right-2, rc.bottom-6);
	m_wndProgressBar.SetWindowRgn(hrgn, TRUE);
	DeleteObject(hrgn);
	
	EFS_END
}

BOOL CMarcImportDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);

	return FALSE;

	EFS_END
	return FALSE;
}

VOID CMarcImportDlg::OnDestroy() 
{
	EFS_BEGIN
	
	CDialog::OnDestroy();

	EFS_END
}

VOID CMarcImportDlg::OnbFILEOPEN() 
{
	EFS_BEGIN

	//UpdateData();

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;

	OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT 파일 열기");

	BOOL bSuccess = GetOpenFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
		UpdateData(FALSE);

		m_wndProgressBar.SetWindowText(_T("파일 패치중..."));
		m_wndProgressBar.UpdateWindow();

		// 파일 반입 시작
		ImportMarcFromFile();		
	}


EFS_END
}


VOID CMarcImportDlg::OnbIMPORT() 
{
	EFS_BEGIN

	m_pGrid_OK->SelectMakeList();

	INT nCount = m_pGrid_OK->SelectGetCount();
	if (nCount < 1) 
	{
		AfxMessageBox(_T("반입할 자료를 선정해 주세요"));
		return;
	}

	UpdateData();

	m_pThreadImport = AfxBeginThread(ThreadFunc_Import, this);

	EFS_END
}

VOID CMarcImportDlg::OnbEXPORTEDITMARC() 
{
	EFS_BEGIN

	// Marc Editor에 넣었다가 뺀다.
	CMarcEditCtrl MarcEditCtrl;
	MarcEditCtrl.Create(_T("RichEdit20W"), NULL , WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), this, 10202);

	// File Select Dialog
	CString strFilePath;
	
	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lpstrDefExt = _T("TXT");

	OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT 파일 저장");

	INT ids = GetSaveFileName(&OFN);
	SetCurrentDirectory(szBuffer);

	if (ids == 0) return;

	strFilePath= OFN.lpstrFile;

	BeginWaitCursor();
	// File
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return;
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	
		
	CMarc marc;
	CString strStreamMarc;
	CString strEditMarc;
	INT nCount = m_pDM_TOTAL->GetRowCount();
	for (INT i = 0; i < nCount; i++)
	{
		strStreamMarc = m_pDM_TOTAL->GetCellData(_T("BS_MARC"), i);
		if (strStreamMarc.IsEmpty()) continue;

		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) continue;

		// MARC을 가져와서 Edit Ctrl에 넣은다음 Text를 가져온다.
		MarcEditCtrl.Init(m_pInfo->pMarcMgr, &marc);
		MarcEditCtrl.Display();

		MarcEditCtrl.GetWindowText(strEditMarc);

		strEditMarc += _T("\r\n\r\n+++++++++++++++++++++++++++++++++++++++++++++++\r\n\r\n");

		file.WriteString(strEditMarc);
	}

	file.Close();

	EndWaitCursor();

	AfxMessageBox(_T("반출 완료"));

	EFS_END
}

UINT CMarcImportDlg::OnEndMarcAdjust(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	// 마크 정리가 완료된 자료는 성공자료로 간다.
	
	// Deterime What Grid is Selected
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return -1;
	
	CESL_Grid *pGrid = NULL;

	INT nID_FROM = -1;
	CString strFROM;

	INT nSel = pTabCtrl->GetCurSel();
	// OK GRID
	if (nSel == 0)
	{
		pGrid = m_pGrid_OK;
		nID_FROM = 1;
		strFROM = _T("정상 : ");
	}
	// ERROR GRID
	else if (nSel == 1)
	{
		pGrid = m_pGrid_ERR;
		nID_FROM = 2;
		strFROM = _T("오류 : ");
	}
	else
		return -1;
	
	INT nRowsCUR = -1;
	INT nRowsOK = -1;

	INT nRowOK = -1;
	CString strData;

	INT nRowCount = pGrid->GetCount();
	INT nColCount = pGrid->GetCols(0);

	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		if (m_pDM_TOTAL->GetCellData(_T("BS_파일반입성공"), _ttoi(pGrid->GetTextMatrix(idx + 1, 0))) == _T("Y"))
		{
			// 성공 Grid로 옮긴다.
			nRowsOK = m_pGrid_SUCCESS->GetRows();
			strData = m_pGrid_SUCCESS->GetTextMatrix(nRowsOK - 1, 0);
			if (strData.IsEmpty())
			{
				nRowOK = nRowsOK - 1;
			}
			else
			{
				m_pGrid_SUCCESS->SetRows(nRowsOK + 1);
				nRowOK = nRowsOK;
			}

			for (INT col = 0; col < nColCount; col++)
			{
				m_pGrid_SUCCESS->SetTextMatrix(nRowOK, col, pGrid->GetTextMatrix(idx + 1, col));
			}

			// RemoveItem
			INT nRowsCUR = pGrid->GetRows();
			if (nRowsCUR != 2)
			{
				pGrid->RemoveItem(idx + 1);
			}
			else if (nRowsCUR == 2)
			{
				pGrid->Clear();
				pGrid->SetColumnInfo();
			}

			// 개수를 설정한다.
			strData = m_wndStatusBarCtrl.GetText(nID_FROM);
			strData = strData.Mid(strData.ReverseFind(':') + 1);
			strData.Format(_T("%d"), _ttoi(strData) - 1);
			m_wndStatusBarCtrl.SetText(strFROM + strData, nID_FROM, 0);		

			strData = m_wndStatusBarCtrl.GetText(3);
			strData = strData.Mid(strData.ReverseFind(':') + 1);
			strData.Format(_T("%d"), _ttoi(strData) + 1);
			m_wndStatusBarCtrl.SetText(_T("성공 : ") + strData, 3, 0);
		}
	}

	return 0;

	EFS_END
	return -1;
}

BEGIN_EVENTSINK_MAP(CMarcImportDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMarcImportDlg)
//	ON_EVENT(CMarcImportDlg, IDC_MSHFLEXGRID_ERR, -605 /* MouseDown */, OnMouseDownMshflexgridErr, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
//	ON_EVENT(CMarcImportDlg, IDC_MSHFLEXGRID_OK, -605 /* MouseDown */, OnMouseDownMshflexgridOk, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
/*
VOID CMarcImportDlg::OnMouseDownMshflexgridOk(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	INT i = m_pGrid_OK->GetMouseRow();
	if (i == 0) return;
	i = m_pGrid_OK->GetFixedRows();
	i = m_pGrid_OK->GetRow();

	EFS_END
}

VOID CMarcImportDlg::OnMouseDownMshflexgridErr(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	//m_pGrid_ERR->SetReverse(m_pGrid_ERR->GetRow() - 1);

	EFS_END
}
*/

VOID CMarcImportDlg::OnbPrintRevision() 
{
EFS_BEGIN

	m_pGrid_OK->SelectMakeList();

	INT nCount = m_pGrid_OK->SelectGetCount();
	if (nCount < 1) 
	{
		AfxMessageBox(_T("교열지 출력할 자료를 선정해 주세요"));
		return;
	}

	//교열지 출력
	PrintRevision();

EFS_END	
}

INT CMarcImportDlg::ImportFile()
{
EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox( _T("File Open Failed!!") ) ;
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	//INT ids;
	INT idxDMMarc = 0 ;
	CString strLine;
	CMarc marc;

	file.ReadString(strLine);

	m_pDM_TOTAL->FreeData();
	m_pDM_OK->FreeData();
	m_pDM_ERR->FreeData();

	do 
	{
		marc.Init();
		strLine.TrimLeft();
		strLine.TrimRight();
		if (strLine.IsEmpty()) break;
		
		// 스트림 마크를 패치 한다.
		while (TRUE)
		{
			CString strStreamMarc = _T("");
			
			INT nFind = strLine.Find(RECORD_TERMINATOR);
			if (nFind < 0)
				break;
			else
			{
				if (nFind == strLine.GetLength() - 1)
				{
					strStreamMarc = strLine;
					strLine.Empty();
				}
				else
				{
					strStreamMarc = strLine.Mid(0, nFind + 1);
					strLine = strLine.Mid(nFind + 1);
				}
			}
			
			//ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
			//if (ids < 0) continue;
			
			MakeDM(idxDMMarc, strStreamMarc);
			idxDMMarc++;
		}
	} while (file.ReadString(strLine));

	//INT iLen = strLine.GetLength();
	file.Close();

	return 0;

EFS_END
return -1;
}

//마크를 통해 DM을 만든다.
VOID CMarcImportDlg::MakeDM(INT idxDMMarc, CString strStreamMarc )
{
EFS_BEGIN

	if (strStreamMarc == _T("") ) return;

	//INT ids;
	CString sTmpMarcData = _T("");

	m_pDM_TOTAL->InsertRow(-1);

	//Stream Marc 저장
	m_pDM_TOTAL->SetCellData(_T("MARC"), strStreamMarc, idxDMMarc );
	
	//Marc Decoding
	CMarc *pMarc = NULL;
	pMarc = new CMarc;

	m_pInfo->pMarcMgr->Decoding( strStreamMarc, pMarc );
	
	INT ids = m_pInfo->pMarcMgr->CheckSimpleSyntax(pMarc);
	ids = 0;
	CESL_DataMgr * pDM;

	if(ids < 0) // 구문오류로 판단
	{
		m_pDM_ERR->InsertRow(-1);
		pDM = (CESL_DataMgr *)m_pDM_ERR;
	}
	else // 정상
	{
		m_pDM_OK->InsertRow(-1);
		pDM = (CESL_DataMgr *)m_pDM_OK;
	}


	pDM->SetCellData(_T("MARC"), strStreamMarc, idxDMMarc );

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("전거표목"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("0") , idxDMMarc );

		//생몰년
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$d"), sTmpMarcData);
		if ( sTmpMarcData != _T(""))
			pDM->SetCellData(_T("생몰년"), sTmpMarcData , idxDMMarc );
	}

	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("전거표목"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("1") , idxDMMarc );

		//생몰년
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$d"), sTmpMarcData);
		if ( sTmpMarcData != _T(""))
			pDM->SetCellData(_T("생몰년"), sTmpMarcData , idxDMMarc );
	}

	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("전거표목"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("2") , idxDMMarc );

		//생몰년
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$d"), sTmpMarcData);
		if ( sTmpMarcData != _T(""))
			pDM->SetCellData(_T("생몰년"), sTmpMarcData , idxDMMarc );
	}

	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("130$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("전거표목"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("3") , idxDMMarc );
	}

	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("150$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("전거표목"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("4") , idxDMMarc );
	}
	
	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("678$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("주기"), sTmpMarcData , idxDMMarc );
	}
	
	INT nIdx = pDM->GetRowCount()-1;

	// 복본개수를 얻는다. 
	CString strType		= _T("");
	CString strRefItem  = _T("");
	CString strCopy		= _T("");
	CString strQuery	= _T("");
	
	pDM->GetCellData(_T("AC_TYPE"),nIdx,strType);
	if(strType.IsEmpty() == FALSE)
	{
		strRefItem = pDM->GetCellData(_T("전거표목"),nIdx);
		if(strRefItem.IsEmpty() == FALSE)
		{
			if(strType == _T("0") || strType == _T("1") || strType == _T("2") )
			{
				strQuery.Format(_T("SELECT COUNT(*) FROM AC_INDEX_AUTHOR_TBL WHERE KEYWORD = '%s' "), strRefItem);
				m_pDM_TOTAL->GetOneValueQuery(strQuery, strCopy);
				pDM->SetCellData(_T("복본개수"),strCopy,nIdx);
			}
			else if(strType == _T("3"))
			{
				strQuery.Format(_T("SELECT COUNT(*) FROM AC_INDEX_TITLE_TBL WHERE KEYWORD = '%s' "), strRefItem);
				m_pDM_TOTAL->GetOneValueQuery(strQuery,strCopy);
				pDM->SetCellData(_T("복본개수"),strCopy,nIdx);
			}
			else
			{
				strQuery.Format(_T("SELECT COUNT(*) FROM AC_INDEX_SUBJECT_TBL WHERE KEYWORD = '%s' "), strRefItem);
				m_pDM_TOTAL->GetOneValueQuery(strQuery, strCopy);
				pDM->SetCellData(_T("복본개수"),strCopy,nIdx);
			}
		}
	}

	if ( pMarc != NULL ) delete pMarc;

	return;

EFS_END
}

INT CMarcImportDlg::PrintRevision()
{
EFS_BEGIN

	// Grid 선정된 종 Key를 가져온다.
	INT nCount = m_pGrid_OK->SelectGetCount();
	if (nCount <= 0)
	{
		AfxMessageBox( _T("선정된 리스트가 없습니다.") );
		return -1;
	}

	// Marc Editor에 넣었다가 뺀다.
	CString strEditMarc;
	CMarcEditCtrl MarcEditCtrl;
	MarcEditCtrl.Create(_T("RichEdit20W"), NULL, WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), this, 10202);

	BeginWaitCursor();
	// DM 구성
	CESL_DataMgr DM_ACMarc;
	DM_ACMarc.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	
	// Where 절을 구성한다.
	CString strOption;
	CString strQuery;
	CStringArray arrStrSpeciesKey;

	
	CESL_DataMgr *pACPrintDM = NULL;
	pACPrintDM = FindDM(_T("DM_정리_전거교열지"));

	pACPrintDM->FreeData();

	INT iCount = 0;
	INT iRec = 0;
	CString sSpeciesKey;
	CString strStreamMarc;

	CMarc *pMarc = NULL ;
	pMarc = new CMarc;

	INT nIdx = m_pGrid_OK->SelectGetHeadPosition();

	for( INT i = 0 ; i < nCount ; i++ ) {

		m_pDM_OK->GetCellData(_T("MARC"), nIdx, strStreamMarc);
		
		if (m_pInfo->pMarcMgr->Decoding( strStreamMarc, pMarc ) < 0) continue;

		MarcEditCtrl.Init(m_pInfo->pMarcMgr, pMarc);
		MarcEditCtrl.Display();
			
		MarcEditCtrl.GetWindowText( strEditMarc );

		CString str;
		str.Format(_T("%c"), SUBFIELD_CODE);
		strEditMarc.Replace(str, _T("▼"));

		str.Format(_T("%c"), FIELD_TERMINATOR);
		strEditMarc.Replace(str, _T("▲"));

		CString sLeader;
		pMarc->GetLeader(sLeader);
		strEditMarc = sLeader + _T("▲") + strEditMarc;
		
		//마크 짜르기
		INT iStart = 0;

		while ( TRUE ) {
			INT iPos = strEditMarc.Find(_T("▲"));
			if ( iPos == 0 ) {
				iPos = strEditMarc.Find(_T("\r\n"));
			}
			CString sData = strEditMarc.Mid( iStart, iPos+2 );			
			strEditMarc = strEditMarc.Mid( iPos+2 );
			strEditMarc.TrimLeft();
			INT iLineWordCount = 1000;

			while ( sData.GetLength() > iLineWordCount ) {
				CString sDataSub = sData.Left( iLineWordCount );
				BOOL bCh = IsCanCut( sDataSub, --iLineWordCount );
				if ( bCh == FALSE ) {
					iLineWordCount;
					sDataSub = sData.Left( iLineWordCount );
				}
				pACPrintDM->InsertRow(-1);
				pACPrintDM->SetCellData( _T("MarcLine"), sDataSub, iCount );
				iCount++;
				sData = sData.Mid(iLineWordCount);
			}
			iStart = 0;
			pACPrintDM->InsertRow(-1);
			pACPrintDM->SetCellData( _T("MarcLine"), sData, iCount );
			iCount++;

			if ( iPos == -1 ) {
				break;
			}
		}

		pACPrintDM->InsertRow(-1);
		pACPrintDM->SetCellData( _T("MarcLine"), _T(""), iCount );
		iCount++;

		nIdx = m_pGrid_OK->SelectGetNext();
	}	

	CSIReportManager SIReportManager(this);
	SIReportManager.SetCONNECTION_INFO(pACPrintDM->CONNECTION_INFO);
	INT ids = SIReportManager.GetSIReportFileFromDB(_T("입력교열지"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return -1;
	}
	
	SIReportManager.SetDataMgr(0, pACPrintDM, 0);
	
	EndWaitCursor();
	
	ids = SIReportManager.Print();
	return 0;

EFS_END
return -1;
}

BOOL CMarcImportDlg::IsCanCut(CString sStr, INT nPos)
{
	EFS_BEGIN
		
		INT i, nLastPos = -1;
	//글자수 초과면 FALSE
	if(nPos > sStr.GetLength())
		return FALSE;
	TCHAR ch = sStr.GetAt(nPos);
	//마지막 글자 || 한글이 아니면 TRUE
	if(nPos + 1 == sStr.GetLength() || (ch & (0x80)) != 128)
		return TRUE;
	//첫글자가 1byte문자면 TRUE 아니면 FALSE
	if(nPos == 0)
		if((ch & (0x80)) != 128)
			return TRUE;
		else
			return FALSE;
		
		for(i = 0; i < nPos; i++) {
			ch = sStr.GetAt(i);
			//자르는 부분에서 가장 가까운(앞쪽으로)영문의 위치
			if((ch & (0x80)) != 128)
				nLastPos = i;
		}
		if (nLastPos == -1 && nPos % 2 == 1) return FALSE;
		//짝수개 차이나면 TRUE
		return !((nPos - nLastPos) % 2);
		
		EFS_END
			return FALSE;
		
}

INT CMarcImportDlg::InsertToDB(CString sStreamMarc)
{
EFS_BEGIN

	CString sACMasterReckey;
	m_pDM_TOTAL->MakeRecKey(sACMasterReckey);

	CMarc *pMarc = NULL;
	pMarc = new CMarc;

	m_pInfo->pMarcMgr->Decoding(sStreamMarc, pMarc );

	if ( InsertACMASTERTBL(sACMasterReckey, pMarc) < 0 ) return -1;
	
	if ( InsertACTBL(sACMasterReckey, pMarc) < 0 ) return -1;

	delete pMarc;

	return 0;

EFS_END
return -1;
}

INT CMarcImportDlg::InsertACMASTERTBL(CString sACMasterKey, CMarc *pMarc)
{
EFS_BEGIN

	INT ids = -1;

	m_pDM_TOTAL->FreeData();

//	CESL_DataMgr * pDM = new CESL_DataMgr;
//	pDM->SetCONNECTION_INFO(m_pDM_TOTAL->CONNECTION_INFO);

	ids = m_pDM_TOTAL->StartFrame();
	if ( ids < 0 ) return -1;

	m_pDM_TOTAL->InsertRow(-1);
	m_pDM_TOTAL->SetCellData(_T("REC_KEY"), sACMasterKey , 0);

	//전거 제어번호 구하기 
	CString sACConNo = GetAcControlNo(m_pDM_TOTAL);

	if ( sACConNo == _T("") ) return -1;
	m_pDM_TOTAL->SetCellData(_T("AC_CONTROL_NO"), sACConNo , 0);

	//MARC에 제어번호 입력
	m_pInfo->pMarcMgr->DeleteField(pMarc, _T("001"));
	m_pInfo->pMarcMgr->InsertField(pMarc, _T("001") + sACConNo);

	// STREAM MARC 입력
	CString sStreamMarc;
	m_pInfo->pMarcMgr->Encoding(pMarc, sStreamMarc );
	m_pDM_TOTAL->SetCellData(_T("MARC"), sStreamMarc , 0);

	//전거형식 구하기
	CString ACType = GetACType(pMarc);
	if ( ACType == _T("") ) return -1;

	m_pDM_TOTAL->SetCellData(_T("AC_TYPE"), ACType , 0);
	m_pDM_TOTAL->SetCellData(_T("AC_CLASS"), _T("") , 0);
	m_pDM_TOTAL->SetCellData(_T("SEE_ALSO_REFERENCE_YN"), _T("N") , 0);


	CTime t = CTime::GetCurrentTime();
	CString strMsg;
	strMsg.Format(_T("%04d%02d%02d %02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute());


	m_pDM_TOTAL->SetCellData(_T("INPUT_DATE"), strMsg , 0);
	m_pDM_TOTAL->SetCellData(_T("UPDATE_DATE"), strMsg , 0);

	m_pDM_TOTAL->SetCellData(_T("FIRST_WORKER"), m_pInfo->USER_ID , 0);
	m_pDM_TOTAL->SetCellData(_T("LAST_WORKER"), m_pInfo->USER_ID , 0);

	m_pDM_TOTAL->SetCellData(_T("FIRST_WORK"), GetWorkLogMsg(_T("전거마크반입"),WORK_LOG) , 0);
	m_pDM_TOTAL->SetCellData(_T("LAST_WORK"), GetWorkLogMsg(_T("전거마크반입"),WORK_LOG) , 0);

	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	ids = m_pDM_TOTAL->MakeQueryByDM(RemoveAliasList, m_pDM_TOTAL, m_pDM_TOTAL->TBL_NAME, 0, m_pDM_TOTAL);
	if ( ids < 0) return -1;

	ids = m_pDM_TOTAL->SendFrame();
	if ( ids < 0) return -1;

	ids = m_pDM_TOTAL->EndFrame();
	if ( ids < 0 ) return -1;

	return 0;


EFS_END
return -1;
}

INT CMarcImportDlg::InsertACTBL(CString sACMasterKey, CMarc *pMarc)
{
EFS_BEGIN

	INT ids = -1;
	CESL_DataMgr *pDM = NULL;
	INT iACMode = -1;

	iACMode = GetACMode(pMarc);

	if ( iACMode == 0 )
		pDM = FindDM( _T("DM_정리_전거마크반입_AUTHOR") );
	else if ( iACMode == 1 )
		pDM = FindDM( _T("DM_정리_전거마크반입_TITLE") );
	else if ( iACMode == 2 )
		pDM = FindDM( _T("DM_정리_전거마크반입_SUBJECT") );

	if ( pDM == NULL ) return -1;

	ids = pDM->StartFrame();
	if ( ids < 0 ) return -1;
	
	CString sACReckey;
	
	// 전거인덱스 테이블에 입력
	// 채택표목은 마스터와 인덱스에 참조표목은 인덱스에 넣는다.
	// 참조표목을 구한다.

	CArray <CString, CString &> sArrayRef;
	sArrayRef.RemoveAll();
	
	ids = GetRefItem(pMarc,GetACType(pMarc),&sArrayRef);
	if( ids < 0 ) return -1;

	INT nCount = sArrayRef.GetSize();
	if(nCount < 1)
		return 0;

	CString sACKeyWord = _T("");
	for( INT i = 0; i < nCount+1 ; i++ )
	{
		pDM->MakeRecKey(sACReckey);
		pDM->InsertRow(-1);
		pDM->SetCellData(_T("REC_KEY"), sACReckey , i);
		pDM->SetCellData(_T("AC_MASTER_MAT_KEY"), sACMasterKey , i);

		if(i == nCount)
		{
			sACKeyWord = GetACKeyword(pMarc);
			pDM->SetCellData(_T("KEYWORD"), sACKeyWord , i);
			pDM->SetCellData(_T("SELECT_YN"), _T("Y") , i);
		}
		else
		{
			pDM->SetCellData(_T("KEYWORD"), sArrayRef.GetAt(i) , i);
			pDM->SetCellData(_T("SELECT_YN"), _T("N") , i);
		}

		pDM->SetCellData(_T("FIRST_WORK"), GetWorkLogMsg(_T("전거마크반입"),WORK_LOG) , i);
		pDM->SetCellData(_T("LAST_WORK"), GetWorkLogMsg(_T("전거마크반입"),WORK_LOG) , i);

		CArray<CString, CString> RemoveAliasList;
		RemoveAliasList.RemoveAll();

		ids = pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, i, pDM);
		if ( ids < 0) return -1;
	}

	ids = pDM->SendFrame();
	if ( ids < 0) return -1;

	ids = pDM->EndFrame();
	if ( ids < 0 ) return -1;

	return 0;

EFS_END
return -1;
}

CString CMarcImportDlg::GetAcControlNo(CESL_DataMgr *pDM)
{
	EFS_BEGIN
		
	CTime t = CTime::GetCurrentTime();
	
	CString sYear;
	sYear.Format(_T("%04d"), t.GetYear());
	
	CString strQuery;
	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE MAKE_YEAR = '%s' AND DIVISION_VALUE = 'KAC' AND KIND_CODE = 'CON_NO'"), sYear );
	
	//제어번호 검색
	CString sACConNo = _T("");
	pDM->GetOneValueQuery(strQuery, sACConNo);	
	
	if ( sACConNo == _T("") )
	{
		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR, DIVISION_VALUE, LAST_NUMBER, CREATE_DATE, CREATE_WORKER, FIRST_WORK, LAST_WORK) VALUES ( ESL_SEQ.NEXTVAL, 'CON_NO', 'CO', '%s', 'KAC', '1', '','','','');"), sYear );
		pDM->AddFrame(strQuery);
		sACConNo = _T("1");
	}
	else {
		INT iTmp = _ttoi(sACConNo);
		iTmp++;
		sACConNo.Format(_T("%d"), iTmp);
		
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s' WHERE KIND_CODE = 'CON_NO' AND  PUBLISH_FORM_CODE = 'CO' AND MAKE_YEAR = '%s' AND DIVISION_VALUE = 'KAC' ;"), sACConNo, sYear );
		pDM->AddFrame(strQuery);
	}
	
	CString sTmp0 = _T("");
	for ( INT i = sACConNo.GetLength() ; i < 9 ; i ++)
	{
		sTmp0 += _T("0");
	}
	sACConNo = _T("KAC") + sTmp0 + sACConNo;
	
	return sACConNo;
	
	EFS_END
		return _T("");
}

CString CMarcImportDlg::GetACType(CMarc *pMarc)
{
	EFS_BEGIN
	if ( pMarc == NULL ) return _T("");
		
	CString sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("0");
	
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("1");
	
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("2");
	
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("130$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("3");
	
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("150$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("4");
	
	
EFS_END
return _T("");
}

INT CMarcImportDlg::GetACMode(CMarc *pMarc)
{
EFS_BEGIN

	CString sTmpItem = _T("");
	
	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 0;

	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 0;

	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 0;

	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("130$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 1;

	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("150$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 2;


EFS_END
return -1;
}

CString CMarcImportDlg::GetACKeyword(CMarc *pMarc)
{
EFS_BEGIN

	if ( pMarc == NULL ) return _T("");

	CString sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("130$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("150$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

EFS_END
return _T("");
}

INT CMarcImportDlg::GetRefItem(CMarc * pMarc, CString sType, CArray <CString, CString&>* sArray)
{
	//CString <CString , CString &> sArray;
	CString str;
	if( sType == _T("0") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("400$a"), str, sArray);
	}
	else if( sType == _T("1") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("410$a"), str, sArray);
	}
	else if( sType == _T("2") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("411$a"), str, sArray);
	}
	else if( sType == _T("3") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("430$a"), str, sArray);
	}
	else if( sType == _T("4") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("450$a"), str, sArray);
	}
	else 
	{
		AfxMessageBox(_T("ACType Undefined!!"));
		return -1;
	}
	return 0;
}

HBRUSH CMarcImportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
