// SeFileImport.cpp : implementation file
//

#include "stdafx.h"
#include "SeFileImport.h"
#include "SePurchaseNoDlg.h"
#include "SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeFileImport dialog


CSeFileImport::CSeFileImport(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeFileImport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeFileImport)
	m_nFILE_TYPE = -1;
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT
	
	m_pDM = NULL;
	m_pDM_OK = NULL;
	m_pDM_SUCCESS = NULL;
	m_pDM_ERR = NULL;
	m_pCM = NULL;
	m_pGrid_OK = NULL;
	m_pGrid_SUCCESS = NULL;
	m_pGrid_ERR = NULL;
	m_pThreadDisplay = NULL;
	m_pThreadImport = NULL;

	m_pDM_SEQ_NO = NULL;
	m_pDM_WORK_NO = NULL;

	m_strWORK_NO = _T("");
	m_strSEQ_NO_KEY = _T("");

	// 16/03/29 김혜영 : 한셀 적용하기 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	m_bIsExcel = TRUE;
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		m_bIsExcel = FALSE;	// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	}	
//*/ END ------------------------------------------------------------------------------------
}


CSeFileImport::~CSeFileImport()
{
	if(m_pThreadDisplay != NULL)
	{
		DWORD dwRetCode;
		dwRetCode = ::WaitForSingleObject(m_pThreadDisplay->m_hThread , 2000);
		if (dwRetCode == WAIT_TIMEOUT)
		{
			TerminateThread(m_pThreadDisplay->m_hThread , 0);
		}
	}

	if(m_pThreadImport != NULL)
	{
		DWORD dwRetCode;
		dwRetCode = ::WaitForSingleObject(m_pThreadImport->m_hThread , 2000);
		if (dwRetCode == WAIT_TIMEOUT)
		{
			TerminateThread(m_pThreadImport->m_hThread , 0);
		}
	}
}


VOID CSeFileImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeFileImport)
	DDX_Control(pDX, IDC_tabFILE_IMPORT, m_ctrlTAB);
	DDX_Radio(pDX, IDC_radFILE_TYPE1, m_nFILE_TYPE);
	DDX_Text(pDX, IDC_edtFILE_NAME, m_strFilePath);
	// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	if(m_bIsExcel != TRUE)
	{
		DDX_Text(pDX, IDC_radFILE_TYPE1, CString(_T("HCELL")));	// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	}
//*/ END -------------------------------------------------------------------------------------
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeFileImport, CDialog)
	//{{AFX_MSG_MAP(CSeFileImport)
	ON_BN_CLICKED(IDC_btnFILE_OPEN, OnbtnFILEOPEN)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabFILE_IMPORT, OnSelchangetabFILEIMPORT)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnFILE_ORDER, OnbtnFILEORDER)
	ON_BN_CLICKED(IDC_btnFILE_MARC, OnbtnFILEMARC)
	ON_BN_CLICKED(IDC_btnFILE_EXPORT, OnbtnFILEEXPORT)
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_edtFILE_ORDER_NO, OnKillfocusedtFILEORDERNO)
	ON_MESSAGE(ALLGRIDDISPLAY, OnGridDisplay)
	ON_MESSAGE(COUNT_STATUS_UPDATE, OnCountStatusUpdate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeFileImport message handlers

BOOL CSeFileImport::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_파일반입"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_파일반입") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_연속_수서_파일반입"));
	
	m_pGrid_OK = (CESL_Grid*)m_pCM->FindControl(_T("그리드_정상"));
	m_pGrid_SUCCESS = (CESL_Grid*)m_pCM->FindControl(_T("그리드_성공"));
	m_pGrid_ERR = (CESL_Grid*)m_pCM->FindControl(_T("그리드_에러"));
	
	m_pDM = FindDM(_T("DM_연속_수서_파일반입"));
	m_pDM_OK = FindDM(_T("DM_연속_수서_파일반입_정상"));
	m_pDM_SUCCESS = FindDM(_T("DM_연속_수서_파일반입_성공"));
	m_pDM_ERR = FindDM(_T("DM_연속_수서_파일반입_에러"));
	m_pDM_SEQ_NO = FindDM(_T("DM_연속_수서_파일반입_차수번호"));
	m_pDM_WORK_NO = FindDM(_T("DM_연속_수서_파일반입_작업번호"));

	if(!m_pCM || !m_pGrid_OK ||!m_pGrid_SUCCESS || !m_pGrid_ERR || !m_pDM || !m_pDM_OK || !m_pDM_SUCCESS || !m_pDM_ERR || !m_pDM_SEQ_NO || !m_pDM_WORK_NO) {
		AfxMessageBox( _T("DM,CM,grid Error : 연속_수서_파일반입") );
		return FALSE;
	}	

	GetWindowRect(m_rcInitial);


	m_nFILE_TYPE = 0;

	m_ctrlTAB.InsertItem(0, _T("정상"));
	m_ctrlTAB.InsertItem(1, _T("오류"));
	m_ctrlTAB.InsertItem(2, _T("성공"));

	
	m_pGrid_OK->BringWindowToTop();
	m_pGrid_ERR->ShowWindow(SW_HIDE);
	m_pGrid_SUCCESS->ShowWindow(SW_HIDE);


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

	PostMessage(WM_SIZE);

	UpdateData(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeFileImport::OnbtnFILEOPEN() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

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


	if (m_nFILE_TYPE == 0)
	{
		// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
		// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
		if (m_bIsExcel != TRUE) 
		{
			OFN.lpstrFilter = _T("한셀파일(*.cell)\0*.cell\0모든파일(*.*)\0*.*\0");
			OFN.lpstrTitle = _T("한셀 파일 저장");
		}	
		else
//*/ END -------------------------------------------------------------------------------------
		{
			OFN.lpstrFilter = _T("XLS파일(*.xls)\0*.xls\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("EXCEL 파일 열기");
		}
	}
	else if(m_nFILE_TYPE == 1)
	{
		OFN.lpstrFilter = _T("MARC파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("MARC 파일 열기");
	}


	BOOL bSuccess = GetOpenFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
		UpdateData(FALSE);
		
		
		m_wndProgressBar.SetWindowText(_T("파일 패치중..."));
		m_wndProgressBar.UpdateWindow();
		
		
		//DM free
		m_pDM->FreeData();
		m_pDM_OK->FreeData();
		m_pDM_SUCCESS->FreeData();
		m_pDM_ERR->FreeData();


		// 파일 반입 시작
		ImportMarcFromFile();
		
	}
}


INT CSeFileImport::ImportMarcFromFile()
{
	
	BeginWaitCursor();
	INT ids;

	if (m_strFilePath.IsEmpty()) return -1;

	// MarcMgr, FileType, FilePath 설정
	// File Type : XLS -- EXCEL 파일, TXT -- TEXT 파일
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	//간행빈도 변환을 위한 코드매니져
	m_marcImpExpMgr.m_pCodeMgr = m_pInfo->pCodeMgr;

	m_marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, m_nFILE_TYPE , m_strFilePath);


	// Call Back Func 설정
	m_marcImpExpMgr.SetCallBackMarcCheck((INT (CWnd::*)(CMarc *pMarc, CString &str))CallBackCheckMarc);


	// DM 설정
	m_marcImpExpMgr.InitDMList();
	m_marcImpExpMgr.AddDMPtr(m_pDM);

	// 오류 체크 받을 DM Alias 설정
	m_marcImpExpMgr.SetErrChkAlias(_T("UDF_파일반입오류"));

	// Stream Marc 받을 DM Alias 설정
	m_marcImpExpMgr.SetMarcStreamAlias(_T("MARC"));

	// Import 시작
	ids = m_marcImpExpMgr.ImportFile();
	
	// Display Thread Start
	if (ids >= 0)
		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
	

	EndWaitCursor();

	return ids;
}

INT CSeFileImport::DeleteUnnecessaryMarcItem(CMarc* pMarc)
{

	const INT cnt = 5;
	CString strfiled[cnt] = {
		_T("001"),	//0
		_T("049"),	//1	
		_T("090"),	//2
		_T("980"),	//3	
		_T("052")	//4	
	};	
	
	for(INT i=0;i<cnt;i++)
	{
		m_pInfo->pMarcMgr->DeleteField(pMarc,strfiled[i]);
	}

	return 0;
}

INT CSeFileImport::SetCodeInfo(CESL_DataMgr* pDM,INT nRowIdx)
{
	//화면의 콘트롤과 차수 정보, 기본값을 넣어준다
	const INT cnt = 14;
	CString strAlias[cnt] = {
		_T("구입처_코드"),
		_T("관리구분_코드"),
		_T("등록구분_코드"),
		_T("매체구분_코드"),
		_T("보조등록구분_코드"),
		_T("분류기호구분_코드"),
		_T("이용대상구분_코드"),
		_T("이용제한구분_코드"),
		_T("자료구분_코드"),
		_T("제어자료구분_코드"),
		_T("형식구분_코드"),
		_T("차수년도"),
		_T("차수번호"),
		_T("입수책수")
	};
	
	CString strData;

	for(INT i=0;i<cnt;i++)
	{
		strData = _T("");
		m_pCM->GetControlMgrData(strAlias[i],strData);
		pDM->SetCellData(strAlias[i],strData,nRowIdx);
	}

	pDM->SetCellData(_T("입력구분"),_T("2"),nRowIdx); //파일 반입

	//작업번호 및 작업일련번호 처리 
	CSeApi api(this);
	CString strSerialNo = _T("");
	strSerialNo = api.GetSerialWorkNo(pDM,m_strSEQ_NO_KEY,m_strWORK_NO);

	pDM->SetCellData(_T("차수내작업번호"),m_strWORK_NO,nRowIdx); 
	pDM->SetCellData(_T("차수일련번호"),strSerialNo,nRowIdx); 	

	return 0;
}


VOID CSeFileImport::OnGridDisplay(WPARAM w,LPARAM l)
{
	m_pGrid_SUCCESS->Display();
	m_pGrid_ERR->Display();
	m_pGrid_OK->Display();
}

VOID CSeFileImport::OnCountStatusUpdate(WPARAM w,LPARAM l)
{
	CString strCount;

	//ERR COUNT
	strCount.Format(_T("오류 : %d"), m_pDM_ERR->GetRowCount());
	m_wndStatusBarCtrl.SetText(strCount, 2, 0);
	

	//OK COUNT
	strCount.Format(_T("정상 : %d"), m_pDM_OK->GetRowCount());
	m_wndStatusBarCtrl.SetText(strCount, 1, 0);
	

	//SUCCESS COUNT
	strCount.Format(_T("성공 : %d"), m_pDM_SUCCESS->GetRowCount());
	m_wndStatusBarCtrl.SetText(strCount, 3, 0);

}

INT CSeFileImport::CallBackCheckMarc(CMarc *pMarc, CString &strCheckErrMsg)
{
	
	//008 설정
	//입력일 설정
	COleDateTime t = COleDateTime::GetCurrentTime();
	CString tmp,inputdate;
	tmp.Format(_T("%04d%02d%02d"),t.GetYear(),t.GetMonth(),t.GetDay());
	inputdate = tmp.Mid(2);
	m_pInfo->pMarcMgr->DeleteItem(pMarc,_T("008@0-5"));
	m_pInfo->pMarcMgr->SetItem(pMarc,_T("008@0-5"),inputdate);
	

	//길이 설정 (40자리)
	CString strtmp;
	m_pInfo->pMarcMgr->GetItem(pMarc,_T("008*"),strtmp);

	if(strtmp.GetLength()<40)
	{
		m_pInfo->pMarcMgr->SetItem(pMarc,_T("008@39"),_T(" "));
	}

	//간행상태가 없을 경우 간행중(c)로 설정
	strtmp = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc,_T("008@6"),strtmp);
	strtmp.TrimLeft();
	strtmp.TrimRight();

	if(strtmp.IsEmpty())
	{
		m_pInfo->pMarcMgr->SetItem(pMarc,_T("008@6"),_T("c"));
	}

	return 0;
}

INT CSeFileImport::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

UINT ThreadFunc_Display(LPVOID pParam)
{
	CSeFileImport *pDlg = (CSeFileImport*)pParam;
	// KOL.UDF.022
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	pDlg->m_bIsThreadRunning = TRUE;
	
	CString strCount;
	INT nTotalCount = -1;
	INT nErrCount = 0;
	INT nOKCount = 0;
	
	//Total Count
	nTotalCount = pDlg->m_pDM->GetRowCount();
	strCount.Format(_T("전체 : %d"), nTotalCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);
	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));

	CMarc Marc;

	//Error Count
	CString strErr;
	INT nRowIdx;
	for (INT i=0;i<nTotalCount;i++)
	{
		
		strErr = _T("");
		strErr = pDlg->m_pDM->GetCellData(_T("UDF_파일반입오류"), i);
		
		if(!strErr.IsEmpty())
		{
			pDlg->m_pDM_ERR->InsertRow(-1);
			nRowIdx = pDlg->m_pDM_ERR->GetRowCount()-1;
			
			pDlg->CopyDMToDMByAlias(pDlg->m_pDM,i,pDlg->m_pDM_ERR,nRowIdx);
			nErrCount++;	

		}
		else
		{
			pDlg->m_pDM_OK->InsertRow(-1);
			nRowIdx = pDlg->m_pDM_OK->GetRowCount()-1;
			
			pDlg->CopyDMToDMByAlias(pDlg->m_pDM,i,pDlg->m_pDM_OK,nRowIdx);
		}
		
		pDlg->m_wndProgressBar.SetPos(i+1);
	}

	strCount.Format(_T("오류 : %d"), nErrCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);
	

	//OK COUNT
	nOKCount = nTotalCount - nErrCount;
	strCount.Format(_T("정상 : %d"), nOKCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);
	

	//SUCCESS COUNT
	pDlg->m_wndStatusBarCtrl.SetText(_T("성공 : 0"), 3, 0);

	//Grid Display(post message)
	::PostMessage(pDlg->GetSafeHwnd(),ALLGRIDDISPLAY,0,0);

	pDlg->m_bIsThreadRunning = FALSE;
	

	return 0;
}

UINT ThreadFunc_Import(LPVOID pParam)
{
	
	CSeFileImport *pDlg = (CSeFileImport*)pParam;
	// KOL.UDF.022
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	CSeApi api(pDlg);
	pDlg->m_bIsThreadRunning = TRUE;
	
	pDlg->m_pGrid_OK->SelectMakeList();
	INT nCount = pDlg->m_pGrid_OK->SelectGetCount();
	INT idx = pDlg->m_pGrid_OK->SelectGetTailPosition();
	pDlg->m_wndProgressBar.SetRange(0, nCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));
	INT nProcess = 0;
	INT nFailCount = 0;
	CMarc Marc;
	CString strStreamMarc;
	INT ids;
	INT nSuccessIdx;
	CString strTitle;
	CString msg;

	while(idx>=0)
	{
		
		//불필요한 마크를 제거한후 
		strStreamMarc = _T("");		
		strStreamMarc = pDlg->m_pDM_OK->GetCellData(_T("MARC"),idx);
		pDlg->m_pInfo->pMarcMgr->Decoding(strStreamMarc,&Marc);
		pDlg->DeleteUnnecessaryMarcItem(&Marc);
		strTitle = _T("");
		pDlg->m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTitle);
		msg.Format(_T("[ %s ] 반입중..."),strTitle);
		pDlg->m_wndProgressBar.SetWindowText(msg);


		//코드,차수, 기본값 값을 설정해주고 
		pDlg->SetCodeInfo(pDlg->m_pDM_OK,idx);
		
		
		//연속 마크 Leader 설정
		api.SetSerialMarcReader(&Marc);
		

		//API 함수를 부른다
		ids = api.SPECIES_PUR_OBJ_FILE_IMPORT(pDlg->m_pDM_OK,
											  &Marc,
	         								  idx);
		
		if(ids<0) {
			nFailCount++;
		}
		else 
		{
			pDlg->m_pDM_SUCCESS->InsertRow(-1);
			nSuccessIdx = pDlg->m_pDM_SUCCESS->GetRowCount()-1;
			pDlg->CopyDMToDMByAlias(pDlg->m_pDM_OK,idx,pDlg->m_pDM_SUCCESS,nSuccessIdx);
			pDlg->m_pDM_OK->DeleteRow(idx);
		}

		nProcess++;
		pDlg->m_wndProgressBar.SetPos(nProcess);
		idx = pDlg->m_pGrid_OK->SelectGetPrev();
	}
	
	pDlg->m_wndProgressBar.SetWindowText(_T("반입완료"));


	if(0 == nFailCount)
	{
		msg.Format(_T(" 총 [ %d ] 건의 자료를 반입 성공 하였습니다!"),nProcess);
	}
	else
	{
		msg.Format(_T(" 총 [ %d ] 건의 자료중 [ %d ] 건의 자료  반입에 실패 하였습니다!"),nProcess,nFailCount);
	}

	AfxMessageBox(msg);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));	

	//Grid Display(post message)
	::PostMessage(pDlg->GetSafeHwnd(),ALLGRIDDISPLAY,0,0);

	//Count Status Update(post message)
	::PostMessage(pDlg->GetSafeHwnd(),COUNT_STATUS_UPDATE,0,0);

	pDlg->m_bIsThreadRunning = FALSE;
	

	return 0;
}

VOID CSeFileImport::OnSelchangetabFILEIMPORT(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	INT nCurSel = m_ctrlTAB.GetCurSel();
	
	if(0 == nCurSel)
	{
		m_pGrid_OK->BringWindowToTop();
		m_pGrid_OK->ShowWindow(SW_SHOW);
		m_pGrid_ERR->ShowWindow(SW_HIDE);
		m_pGrid_SUCCESS->ShowWindow(SW_HIDE);
	}
	else if(1 == nCurSel)
	{
		m_pGrid_ERR->BringWindowToTop();
		m_pGrid_OK->ShowWindow(SW_HIDE);
		m_pGrid_ERR->ShowWindow(SW_SHOW);
		m_pGrid_SUCCESS->ShowWindow(SW_HIDE);

	}
	else if(2 == nCurSel)
	{

		m_pGrid_SUCCESS->BringWindowToTop();
		m_pGrid_OK->ShowWindow(SW_HIDE);
		m_pGrid_ERR->ShowWindow(SW_HIDE);
		m_pGrid_SUCCESS->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}

VOID CSeFileImport::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
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
	pWnd = GetDlgItem(IDC_tabFILE_IMPORT);
	if (pWnd)
	{
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);

		rcClient.right = rcDlg.right - 10;
		rcClient.bottom = rcDlg.bottom - 24;

		pWnd->MoveWindow(rcClient);
	}
	
	// Grids
	rcClient.top += 20;
	
	const INT nResizeControlCnt = 3;
	INT nArrayResizeControl[nResizeControlCnt] = {
		IDC_gridFILE_OK, IDC_gridFILE_ERR, 
		IDC_gridFILE_SUCCESS, 
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
}

VOID CSeFileImport::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

VOID CSeFileImport::OnOK() 
{
	// TODO: Add extra validation here
	
	//정상 탭에 대해서만 반입(선택자료에 대해), 성공 자료는 성공 탭으로 보낸다 Thread 사용 
	//차수 정보와 코드 정보를 넣어 주고 불필요한 태그는 삭제 한다.
	
	// Display Thread Start
	INT nCurSel = m_ctrlTAB.GetCurSel();
	if(nCurSel != 0) 
	{
		AfxMessageBox(_T("반입은 정상자료에 대해서만 할 수 있습니다!"));
		return;
	}
	
	CString strSEQ_NO;
	m_pCM->GetControlMgrData(_T("차수번호"),strSEQ_NO);

	if(	strSEQ_NO.IsEmpty())
	{	
		AfxMessageBox(_T("차수번호가 입력되지 않았습니다!"));
		return;
	}

	CString strACQ_BOOK_CNT;
	m_pCM->GetControlMgrData(_T("입수책수"),strACQ_BOOK_CNT);
	if(strACQ_BOOK_CNT.IsEmpty())
	{
		AfxMessageBox(_T("입수책수가 입력되지 않았습니다!"));
		return;
	}


	m_pGrid_OK->SelectMakeList();
	INT idx = m_pGrid_OK->SelectGetHeadPosition();
	if(idx<0 || 0 == m_pDM_OK->GetRowCount())
	{
		AfxMessageBox(_T("선정된 항목이 없습니다!"));
		return;
	}
	

	//작업 번호를 확인해 새번호일 경우 먼저 해당 차수에 새 작업번호를 추가하고 멤버 변수로 보관한다. 
	if(0 == m_pDM_SEQ_NO->GetRowCount()) 
	{
		AfxMessageBox(_T("차수 정보를 찾을 수 없습니다!"));
		return;
	}
	
	m_strSEQ_NO_KEY = m_pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);
	if(m_strSEQ_NO_KEY.IsEmpty())
	{
		AfxMessageBox(_T("차수KEY를 찾을 수 없습니다!"));
		return;

	}
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbFILE_WORK_NO);
	if(!pCombo) return;
	CString	tmp	= _T("");
	pCombo->GetWindowText(tmp);	
	
	if(_T("새번호") == tmp)
	{
		
		CSeApi api(this);

		CString strREC_KEY;
		CString strWorkLog;
		strWorkLog = GetWorkLogMsg(WORK_LOG_SEFILEIMPORT);
		m_pDM_WORK_NO->MakeRecKey(strREC_KEY);
		
		m_strWORK_NO = api.GetLastWorkNo(m_pDM_SEQ_NO,m_strSEQ_NO_KEY);
	
		m_pDM_WORK_NO->FreeData();
		m_pDM_WORK_NO->InsertRow(-1);
		m_pDM_WORK_NO->SetCellData(_T("REC_KEY"),strREC_KEY,0);
		m_pDM_WORK_NO->SetCellData(_T("차수관리KEY"),m_strSEQ_NO_KEY,0);
		m_pDM_WORK_NO->SetCellData(_T("작업번호"),m_strWORK_NO,0);
		m_pDM_WORK_NO->SetCellData(_T("마지막일련번호"),_T("0"),0);		
		m_pDM_WORK_NO->SetCellData(_T("생성일"),api.GetTodayDate(),0);
		m_pDM_WORK_NO->SetCellData(_T("생성자"),m_pInfo->USER_ID,0);
		m_pDM_WORK_NO->SetCellData(_T("최초작업"),strWorkLog,0);		
		m_pDM_WORK_NO->SetCellData(_T("마지막작업"),strWorkLog,0);	

		
		CArray<CString,CString> RemoveAliasList;

		m_pDM_WORK_NO->StartFrame();
		RemoveAliasList.RemoveAll();
		m_pDM_WORK_NO->InitDBFieldData();
		m_pDM_WORK_NO->MakeQueryByDM(RemoveAliasList, m_pDM_WORK_NO, _T("SE_PURCHASE_WORK_NO_TBL"), 0 , m_pDM_WORK_NO);
		INT ids = m_pDM_WORK_NO->SendFrame();
		m_pDM_WORK_NO->EndFrame();


		if(ids<0) 
		{
			AfxMessageBox(_T("새 작업번호 입력에 실패 했습니다!"));
			return;
		}


	}
	else if(_T("사용안함") == tmp)
	{
		m_strWORK_NO = _T("0");
	}
	else
	{
		m_strWORK_NO = tmp;
	}
	

	//파일반입 Thread
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pThreadImport = AfxBeginThread(ThreadFunc_Import, this);


	//CDialog::OnOK();
}

VOID CSeFileImport::OnbtnFILEORDER() 
{
	// TODO: Add your control notification handler code here
	CSePurchaseNoDlg dlg(this);
	if(dlg.DoModal() != IDOK) return;
		
	m_pCM->SetControlMgrData(_T("차수년도"),dlg.m_strYear);
	m_pCM->SetControlMgrData(_T("차수번호"),dlg.m_strPurchaseNo);

	SetPurchaseNo();

}

VOID CSeFileImport::OnbtnFILEMARC() 
{
	// TODO: Add your control notification handler code here
	INT nCurSel = m_ctrlTAB.GetCurSel();
	INT nType;

	if(0 == nCurSel) 
	{
		nType = 1;
	}
	else if(1 == nCurSel) 
	{

		nType = 2;
	}
	else
	{
		AfxMessageBox(_T("반입 성공한 자료는 수정할 수 없습니다!"));
		return;
	}

	CSeFileImportMarcEdit dlg(this);
	dlg.m_nType = nType;
	dlg.m_pDM_ERR = m_pDM_ERR;
	dlg.m_pDM_OK = m_pDM_OK;
	dlg.m_pGrid_ERR = m_pGrid_ERR;
	dlg.m_pGrid_OK = m_pGrid_OK;
	

	dlg.DoModal();

}

VOID CSeFileImport::OnbtnFILEEXPORT() 
{
	// TODO: Add your control notification handler code here
	
	
	INT nCurSel = m_ctrlTAB.GetCurSel();
	// KOL.UDF.022 시큐어코딩 보완
	INT nRowCnt = 0;
	//INT nRowCnt;
	
	if (nCurSel == 0) 
		nRowCnt = m_pDM_OK->GetRowCount();
	else if (nCurSel == 1) 
		nRowCnt = m_pDM_ERR->GetRowCount();
	else if (nCurSel == 2)
		nRowCnt = m_pDM_SUCCESS->GetRowCount();


	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("반출할 자료가 없습니다."));
		return;
	}
	
	CString msg;
	msg.Format(_T("[ %d ] 건의 자료를 반출 하시겠습니까?"),nRowCnt);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;


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

	if (GetSaveFileName(&OFN) != 0)
	{
		strFilePath= OFN.lpstrFile;
		ExportToTxtFile(strFilePath);
	}

	SetCurrentDirectory(szBuffer);

}

INT CSeFileImport::ExportToTxtFile(CString strFilePath)
{
	
	BeginWaitCursor();

	// Determine What Grid is Selected
	CESL_DataMgr* pDM = NULL;
	INT nCurSel = m_ctrlTAB.GetCurSel();

	if (nCurSel == 0) 
		pDM = m_pDM_OK;
	else if (nCurSel == 1) 
		pDM = m_pDM_ERR;
	else if (nCurSel == 2)
		pDM = m_pDM_SUCCESS;


	// File
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	CString strMarc;

	for (INT i=0;i<pDM->GetRowCount();i++)
	{
		strMarc = _T("");
		strMarc = pDM->GetCellData(_T("MARC"),i);
		if (strMarc.IsEmpty()) continue;

		strMarc += _T("\r\n");
		file.WriteString(strMarc);
	}

	file.Close();
	EndWaitCursor();
	AfxMessageBox(_T("반출하였습니다!"));

	return 0;
}

VOID CSeFileImport::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	

	CDialog::OnClose();
}

VOID CSeFileImport::OnKillfocusedtFILEORDERNO() 
{
	// TODO: Add your control notification handler code here
	
	// 차수 번호 kill focus
	SetPurchaseNo();

}

VOID CSeFileImport::SetPurchaseNo()
{

	CString strSEQ_NO = _T("");
	m_pCM->GetControlMgrData(_T("차수번호"),strSEQ_NO);
	if(strSEQ_NO.IsEmpty()) return;

	CString strYear;
	m_pCM->GetControlMgrData(_T("차수년도"),strYear);
	if(strYear.IsEmpty()) return;

	CString strQuery;
	strQuery.Format(_T("ACQ_YEAR = '%s' AND SEQ_NO = %s"),strYear,strSEQ_NO);
	m_pDM_SEQ_NO->RefreshDataManager(strQuery);
	
	INT nRowCnt = m_pDM_SEQ_NO->GetRowCount();
	if(0 == nRowCnt) {
		AfxMessageBox(_T("해당 차수년도에 존재하지 않는 차수번호입니다!"));
		GetDlgItem(IDC_edtFILE_ORDER_NO)->SetFocus();
		return;
	}
	
	//구입대상에 해당하는 차수만 입력 할 수 있게 한다
	CString strWorking_status;
	strWorking_status = m_pDM_SEQ_NO->GetCellData(_T("작업상태"),0);
	if(_T("0") != strWorking_status) //구입대상이 아니면 
	{
		AfxMessageBox(_T("해당 차수는 주문중입니다. 구입대상 차수를 선택해 주세요!"));
		GetDlgItem(IDC_edtFILE_ORDER_NO)->SetFocus();
		return;
	}

	CString strWorkNo = _T("");
	strWorkNo = m_pDM_SEQ_NO->GetCellData(_T("마지막작업번호"),0);
	
	CString strREC_KEY = _T("");
	strREC_KEY = m_pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);


	if(_T("0") == strWorkNo) //작업번호를 사용하지 않는 경우 
	{
		SetWorkNo(_T(""),_T(""),FALSE);
	}
	else //사용하는 경우
	{
		SetWorkNo(strREC_KEY,strWorkNo); //마지막 번호로 setting 
	}

}


VOID CSeFileImport::SetWorkNo(CString strSEQ_NO_KEY, CString strCurrentWorkNo , BOOL bUseWorkNo)
{
	//작업번호 설정 함수 
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbFILE_WORK_NO);
	if(!pCombo) return;
	
	if(!bUseWorkNo) //사용안함
	{
		pCombo->ResetContent();
		pCombo->InsertString( 0, _T("사용안함") );
		pCombo->SetCurSel( 0 );
		return;
	}

	if(strSEQ_NO_KEY.IsEmpty()) return;
	
	CString strQuery;
	strQuery.Format(_T("PURCHASE_SEQ_NO_KEY = %s"),strSEQ_NO_KEY);
	m_pDM_WORK_NO->RefreshDataManager(strQuery);
	
	pCombo->ResetContent();
	INT nRowCnt = m_pDM_WORK_NO->GetRowCount();
	CString strWorkNo;

	for(INT i=0;i<nRowCnt;i++)
	{
		strWorkNo = _T("");
		strWorkNo = m_pDM_WORK_NO->GetCellData(_T("작업번호"),i);
		pCombo->InsertString( i, strWorkNo);
	}
	
	pCombo->InsertString( nRowCnt , _T("새번호") );
		
	
	if(strCurrentWorkNo.IsEmpty())
	{
		pCombo->SetCurSel( 0 );
	}
	else
	{
		INT nIndex = pCombo->FindStringExact(0,strCurrentWorkNo);
	
		if(nIndex>=0) pCombo->SetCurSel( nIndex );
		else pCombo->SetCurSel( 0 );

	}

}


HBRUSH CSeFileImport::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
