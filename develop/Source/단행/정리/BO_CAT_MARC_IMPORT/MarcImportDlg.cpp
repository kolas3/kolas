// FileImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcImportDlg.h"
#include "MarcEditDlg.h"
#include "efs.h"

#include "..\..\..\공통\BO_SPECIES_API\BO_SPECIES.h"

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

	m_nOpenMode = 1000;
	m_pThreadImport = NULL;
	m_pThreadDisplay = NULL;

	m_bIsThreadRunning = FALSE;
	m_pCatApi = NULL;

	m_pDM_Index = NULL;
	m_pDM_Species = NULL;
	m_pDM_Book = NULL;
	m_pDM_Main = NULL;

	m_pGrid_OK = NULL;
	m_pGrid_ERR = NULL;
	m_pGrid_SUCCESS = NULL;

	m_pDlgKeyWord = NULL;

	m_lpszSMAlias = _T("SM_정리_마크반입");
	m_lpszCMAlias = _T("CM_정리_마크반입");
	m_lpszCMAlias_Code = _T("CM_정리_마크반입_구분코드");	
	
	// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	m_bIsExcel = TRUE;
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		m_bIsExcel = FALSE;	// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	}	
//*/ END -------------------------------------------------------------------------------------

	// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
	m_bAutoReg = FALSE;
	m_bAutoAcq = FALSE;

	//2020.02.13 JOB.2020.0013 ADD BY KYJ : 비치희망 ISBN 매칭 선택해서 할 수 있도록 기능개선 
	m_bMatchISBN = FALSE;

	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));
}

CMarcImportDlg::~CMarcImportDlg()
{
}

BOOL CMarcImportDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	return FALSE;


	EFS_END
	return FALSE;
}

VOID CMarcImportDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcImportDlg)	
	DDX_Text(pDX, IDC_eFILE_PATH, m_strFilePath);
	// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	if(m_bIsExcel != TRUE)
	{
		DDX_Text(pDX, IDC_rEXCEL, CString(_T("HCELL")));	// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	}
//*/ END -------------------------------------------------------------------------------------
	//2020.02.13 JOB.2020.0013 ADD BY KYJ : 비치희망 ISBN 매칭 선택해서 할 수 있도록 기능개선 
	DDX_Check(pDX, IDC_ChkMatchISBN, m_bMatchISBN);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcImportDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcImportDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bFILE_OPEN, OnbFILEOPEN)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bIMPORT, OnbIMPORT)
	ON_BN_CLICKED(IDC_bEXPORT, OnbEXPORT)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMPORT, OnSelchangeTabImport)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bEXPORT_EDITMARC, OnbEXPORTEDITMARC)
	ON_BN_CLICKED(IDC_ChkGiveAutoRegno, OnChkGiveAutoRegno)
	ON_BN_CLICKED(IDC_ChkLastNoUp_REG, OnChkLastNoUp_REG)
	ON_BN_CLICKED(IDC_ChkLastNoUp_Acq, OnChkLastNoUp_Acq)
	ON_CBN_SELCHANGE(IDC_cmbMEDIA_CLASS, OnSelchangecmbMEDIACLASS)
	ON_BN_CLICKED(IDC_bInit, OnbInit)
	ON_BN_CLICKED(IDC_rTXT, OnrTXT)
	ON_BN_CLICKED(IDC_rEXCEL, OnrEXCEL)
	ON_BN_CLICKED(IDC_rMODS, OnrMODS)
	ON_BN_CLICKED(IDC_rCSV, OnrCSV)
	ON_CBN_SELCHANGE(IDC_cmbIMPORTTYPE, OnSelchangecmbIMPORTTYPE)
	ON_BN_CLICKED(IDC_ChkAccessionRec, OnChkAccessionRec)
	ON_BN_CLICKED(IDC_btnAccessionRecNo, OnbtnAccessionRecNo)
	ON_EN_KILLFOCUS(IDC_eAccessionRecNo, OnKillfocuseAccessionRecNo)
	//JOB.2019.0003 ADD BY KYJ : 수입년도변경시 원부번호가 변경안되는 현상 수정 
	ON_EN_KILLFOCUS(IDC_eACQUISIT_YEAR, OnKillfocuseAccessionYear)
	ON_BN_CLICKED(IDC_bNORMALIZATION, OnbNORMALIZATION)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_END_MARC_ADJUST, OnEndMarcAdjust)
	ON_MESSAGE(WM_GRID_DISPLAY, MsgDisplayGrid)
	ON_MESSAGE(WM_SORT_GRID, MsgSortGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 반입 쓰레드
// [Min Spec] 
// 1.Initialization Variable
// 2. 반입 수행 시간 체크
// 3. 선택된 개수만큼 진행바를 초기화한다.
// 4. 선정된 순서대로 마크정리 DM으로 변경한다.
// 5. 자동부여가 체크되어 있을경우 자동부여 DM 생성 및 마크를 수정한다.
// 6. 책DM에 수입구분/등록일/등록번호 적용
// 7. MARC의 049$l값 모두 12자리로 만들기
// 8. 종의 정리상태를 1로 세팅
// 9. 소장반입이면 작업상태를 소장으로 DB에 INSERT한다.
// 10. 정리중 반입이면 작업상태를 소장으로/대표책 배가실 부여/ DB에 INSERT한다.
// 11. 반입이 완료되면 성공탭에 옮기고 메인 그리드에 반영한다.
// 12. 상태바의 개수를 설정한다.
// 13. 오류및 성공 탭으로 데이터를 옮긴다.
// 14. 진행바를 움직여준다.
/////////////////////////////////////////////////////////////////////////////
//===================================================
//2009.03.10 UPDATE BY PJW : 최초
// UINT ThreadFunc_Import(LPVOID pParam)
// {
// 
// 	//=======================================================================
// 	// 1.Initialization Variable
// 	//=======================================================================
// 	CESL_ControlMgr *pCM = NULL;
// 	CESL_Grid *pGrid_OK = NULL;			// 정상인 그리드
// 	CESL_Grid *pGrid_ERR = NULL;		// 에러난 그리드
// 	CESL_Grid *pGrid_SUCCESS = NULL;	// 성공한 그리드
// 	CESL_Grid *pGrid_TO = NULL;			// 옮길 그리드
// 	// 개수증가시킬 ID
// 	UINT nID_TO = -1;
// 	// 개수증가시킬 String
// 	CString strTO;
// 	// DataMgr
// 	CString strDMPK;
// 	
// 	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
// 	if (pDlg == NULL)
// 	{
// 		pDlg->m_bIsThreadRunning = FALSE;
// 		return -1;
// 	}
// 
// 	pDlg->m_bIsThreadRunning = TRUE;
// 	pDlg->ButtonStatus(0);
// 
// 	// 종DM
// 	CESL_DataMgr DM_Species;
// 	DM_Species.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Species.InitDMPKFromFile(pDlg->m_lpszSpeciesDMAlias);
// 	DM_Species.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// 색인DM
// 	CESL_DataMgr DM_Index;
// 	DM_Index.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Index.InitDMPKFromFile(pDlg->m_lpszIndexDMAlias);
// 	DM_Index.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	// 책DM
// 	CESL_DataMgr DM_Book;
// 	DM_Book.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Book.InitDMPKFromFile(pDlg->m_lpszBookDMAlias);
// 	DM_Book.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// 권DM
// 	CESL_DataMgr DM_Volume;
// 	DM_Volume.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Volume.InitDMPKFromFile(pDlg->m_lpszVolumeDMAlias);
// 	DM_Volume.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	INT ids = -1;
// 	INT idxDM = -1;
// 	INT nCount = -1;
// 
// 	INT nColCount = -1;
// 
// 	INT nRowTo = 0;
// 	INT nRowsTo = 0;
// 	
// 	CString strData;
// 	CString strErrorMsg;
//     CString nTotalCnt;
//     CString strTotalMsg;
//     CString nFalseCnt;
// 	CString nTrueCnt;
// 
// 	DWORD start_app, end_app;
// 	CString strMsg;		
// 	INT		nProcCount = 0;
// 
// 	// HSS_040524 헤더세팅
// 	CString strHD_Book, strHD_Species, strHD_Index;
// 	strHD_Species = _T("BS");
// 	strHD_Book = _T("BB");
// 	strHD_Index = _T("IBS");
// 
// 
// 	// 그리드 설정
// 	pGrid_OK = pDlg->m_pGrid_OK;
// 	pGrid_ERR = pDlg->m_pGrid_ERR;	
// 	pGrid_SUCCESS = pDlg->m_pGrid_SUCCESS;
// 	
// 	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
// 		goto ERR;
// 
// 	//=====================================================
// 	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	INT nRegCodeLength;
// 	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
// 	//=====================================================
// 
// 	//=======================================================================
// 	// 2. 반입 수행 시간 체크 (체크를 왜하는지 모르겠음.)
// 	//=======================================================================
// 	FILE *fpTime;
// 	fpTime = _tfopen( _T("..\\tunning\\반입수행시간.txt"), _T("a+b") );	
// 	
// 		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// 		// KOLAS 2 UP UNICODE 수정작업
// 		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
// 		fseek(fpTime, 0, SEEK_END);		
// 		if( 0 == ftell(fpTime) )
// 		{
// 			/*UNCHANGE*/fputc(0xFF, fpTime);
// 			/*UNCHANGE*/fputc(0xFE, fpTime);
// 		}
// 		// ------------------------------------------------------------------------------
// 		
// 	DWORD start_total, end_total;
// 	start_total = GetTickCount();
// 		
// 	//=======================================================================
// 	// 3. 선택된 개수만큼 진행바를 초기화한다.
// 	//=======================================================================
// 	nCount = pGrid_OK->SelectGetCount();
// 	if (nCount <= 0)
// 	{
// 		goto ERR;
// 	}
// 	
// 	pDlg->m_wndProgressBar.SetRange(0, nCount);
// 	pDlg->m_wndProgressBar.SetPos(0);
// 	pDlg->m_wndProgressBar.SetWindowText(_T(""));
// 
// 	nColCount = pGrid_OK->GetCols(0);
// 
// 	// 쓰레드 시작
// 	BOOL	bError;	
// 	while(TRUE)
// 	{
// 		bError = FALSE;
// 		start_app = GetTickCount();
// 		
// 		if (!pDlg->m_bIsThreadRunning) break;
// 		strErrorMsg.Empty();
// 		pGrid_OK->SelectMakeList();
// 		INT idx = pGrid_OK->SelectGetHeadPosition() + 1;
// 		if (idx <= 0) break;
// 
// 		strData = pGrid_OK->GetTextMatrix(idx, 0);
// 		if (strData.IsEmpty()) continue;
// 
// 		idxDM = _ttoi(strData) - 1 ;
// 		
// 		//===================================================================
// 		// 4. 선정된 순서대로 마크정리 DM으로 변경한다.
// 		//===================================================================
// 		ids = pDlg->MakeMarcAdjustDM(pDlg->m_pDM_Species, idxDM, pDlg->m_pDM_Index, idxDM, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// //		CString MngCode = DM_Book.GetCellData(_T("BB_관리구분_코드"), 0);
// 		CMarc marc;
// 		
// 		//===================================================================
// 		// 5. 자동부여가 체크되어 있을경우 자동부여 DM 생성 및 마크를 수정한다.
// 		//===================================================================
// 		CButton * pChkGiveAutoRegno;
// 		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
// 		if ( pChkGiveAutoRegno->GetCheck() == 1 ) 
// 		{	
// 			INT ids = -1;
// 			// 등록구분 가져오기
// 			CString sRegCode;
// 			pDlg->GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Book + _T("_등록구분"), sRegCode );
// 
// 			// 마지막 번호 가져오기
// 			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
// 			CString sLastNum;	
// 			DM_Species.GetOneValueQuery( sQuery , sLastNum );
// 			INT iLastNum = _ttoi ( sLastNum );
// 
// 			//등록구분이 없으면 새로추가
// 			if ( sLastNum == _T("") ) {
// 				iLastNum = 0;
// 
// 				ids = DM_Species.StartFrame();
// 				if ( 0 > ids ) goto ERR;
// 				CString sRecKey;
// 				ids = DM_Species.MakeRecKey( sRecKey );
// 				if ( 0 > ids ) goto ERR;
// 				CString sQuery_reg;
// 				CTime time = CTime::GetCurrentTime();
// 				CString sYear;
// 				sYear.Format( _T("%04d"), time.GetYear() );
// 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
// 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
// //DEL 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER) ")
// //DEL 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s' );") 
// 								  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
// 				ids = DM_Species.AddFrame( sQuery_reg );
// 				if ( 0 > ids ) goto ERR;
// 
// 				ids = DM_Species.SendFrame();			
// 				if (ids < 0) {
// 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
// 					goto ERR;
// 				}
// 				ids = DM_Species.EndFrame();
// 				if ( 0 > ids ) goto ERR;
// 			}
// 
// 			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_마지막번호") );
// 			if (pDmLastNum == NULL) {
// 				AfxMessageBox( _T("DM_마지막번호 확인") );
// 				goto ERR;
// 			}
// 			pDmLastNum->FreeData();
// 			pDmLastNum->InsertRow(-1);
// 			pDmLastNum->SetCellData( strHD_Book + _T("_등록구분_코드"), sRegCode, 0 );
// 			
// 			//수정
// 			CString sMarc = DM_Species.GetCellData( strHD_Species + _T("_MARC"), 0 );
// 			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
// 			CArray<CString, CString&> pArr049l;
// 			CString strItemData;
// 			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
// 
// 
// 			//선정된 자료의 책수만 하도록 수정
// 			//홍수
// 			INT iBookCount = DM_Book.GetRowCount();
// 
// 			for ( INT j = 0 ; j < iBookCount ; j++ ) {
// 				iLastNum++;
// 				CString sRegNo;
// 				//=====================================================
// 				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// // 				sRegNo.Format( _T("%s%010d"), sRegCode, iLastNum );
// // 				sLastNum.Format( _T("%d"), iLastNum );
// 				sLastNum.Format( _T("%d"), iLastNum );
// 				sRegNo = pDlg->m_pInfo->MakeRegNo( sRegCode, sLastNum );
// 				//=====================================================
// 				
// 
// 				
// 				pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, 0 );
// 				
// 				CString sItem;
// 				if ( pArr049l.GetSize() == iBookCount ) {
// 					sItem = pArr049l.GetAt(j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
// 					//=====================================================
// 					//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 					//ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(2), j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j);
// 					//=====================================================
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
// 				}
// 				else {
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
// 					//=====================================================
// 					//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 					//ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(2), j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j);
// 					//=====================================================
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
// 				}
// 				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
// 				DM_Species.SetCellData( strHD_Species + _T("_MARC"), sMarc, 0 );
// 			}
// 		}
// 
// 		//================== 정리중반입, 소장반입 공통적용 ==================
// 		
// 		//===================================================================
// 		// 6. 책DM에 수입구분/등록일/등록번호 적용
// 		//===================================================================
// 		CString sAcqCode;		
// 		pDlg->GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Index + _T("_수입구분_코드"), sAcqCode );
// 		INT iBookCount = DM_Book.GetRowCount();
// 
// 		
// 
// 		for ( INT i = 0 ; i < iBookCount ; i++ ) {
// 			//수입구분 적용
// 			DM_Book.SetCellData( strHD_Book + _T("_수입구분_코드"), sAcqCode , i );
// 			
// 			//등록일 적용
// 			CString result;
// 			CTime t = CTime::GetCurrentTime();
// 			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록일"), result, i);
// 
// 			//등록번호 12자리 맨들기
// 			//=====================================================
// 			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// // 			CString strRegNo;
// // 			ids = DM_Book.GetCellData( strHD_Book + _T("_등록번호"), i, strRegNo);
// // 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// // 				if ( strRegNo.GetLength() > 2 ) {
// // 					CString sRight;
// // 					sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// // 					strRegNo = strRegNo.Left(2) + sRight;
// // 				}
// // 			}
// 			CString strRegNo;
// 			ids = DM_Book.GetCellData( strHD_Book + _T("_등록번호"), i, strRegNo);
// 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 				if ( strRegNo.GetLength() > nRegCodeLength ) {
// 					strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
// 				}
// 			}
// 			//=====================================================
// 			
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록번호"), strRegNo, i);
// 			// 원부키 설정
// 			if ( pDlg->m_strAccessionRecKey != _T("") )
// 			{
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_원부KEY"), pDlg->m_strAccessionRecKey, i);
// 			}
// 						
// 		}
// 
// 		//===================================================================
// 		// 7. MARC의 049$l값 모두 12자리로 만들기
// 		//===================================================================
// 		CArray<CString, CString&> arr049Item;
// 		CString strItemData;
// 		ids = pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arr049Item);
// 		for ( INT n049Cnt = 0 ; n049Cnt < arr049Item.GetSize(); n049Cnt++ )
// 		{
// 			CString str049Item;
// 			str049Item = arr049Item.GetAt( n049Cnt );
// 			//=====================================================
// 			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// // 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) {
// // 				if ( str049Item.GetLength() > 2 ) {
// // 					CString sRight;
// // 					sRight.Format( _T("%010s") , str049Item.Mid(2) );
// // 					str049Item = str049Item.Left(2) + sRight;
// // 				}
// // 			}
// 			
// 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) {
// 				if ( str049Item.GetLength() > nRegCodeLength ) {
// 					str049Item = pDlg->m_pInfo->MakeRegNo(str049Item.Left(nRegCodeLength), str049Item.Mid(nRegCodeLength));
// 				}
// 			}
// 			//=====================================================
// 
// 			
// 
// 			pDlg->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strItemData, arr049Item.GetAt( n049Cnt ) );
// 		}
// 		
// 		//===================================================================
// 		// 8. 종의 정리상태를 1로 세팅
// 		//===================================================================
// 		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) {
// 			DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , i );
// 		}
// 		//		MngCode = DM_Book.GetCellData(_T("BB_관리구분_코드"), 0);
// 		
// 		//===================================================================
// 		// 9. 소장반입이면 작업상태를 소장으로 DB에 INSERT한다. 
// 		//===================================================================
// 		CButton * pChkWorkStatus;	
// 		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
// 		if ( pChkWorkStatus->GetCheck() == 0 ) 
// 		{
// 			for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 			{ 
// 				DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), _T("0") , i ); 
// 			}
// 			for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 			{ 
// 				DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), _T("BOL112N") , i ); 
// 			}
// 			
// 			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// 		}
// 		//===================================================================
// 		// 10. 정리중 반입이면 작업상태를 소장으로/대표책 배가실 부여/ DB에 INSERT한다.
// 		//===================================================================
// 		else 
// 		{
// 			CString strRegNo;
// 			CString strQuery;
// 			CString strResult;			
// 			bError = FALSE;
// 			for ( int i = 0 ; i < DM_Book.GetRowCount() ; i++ ) 
// 			{
// 				// 2008.04.23 ADD BY PDJ
// 				// 정리중 반입일 경우도 등록번호 중복조사
// 				// 등록번호 넣기
// 				DM_Book.GetCellData( _T("BB_등록번호"), i, strRegNo );
// 				
// 				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), DM_Book.TBL_NAME, strRegNo);
// 				ids = DM_Species.GetOneValueQuery(strQuery, strResult);				
// 				if (ids >= 0 && !strResult.IsEmpty())
// 				{
// 					//등록번호 중복조사
// 					strErrorMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
// 					bError = TRUE;
// 					break;					
// 				}
// 				DM_Book.SetCellData( strHD_Book + _T("_자료상태"), "BOC113N" , i );
// 				// 앞서 했기때문에 생략
// /*				CString strRegNo;
// 				ids = DM_Book.GetCellData( strHD_Book + _T("_등록번호"), i, strRegNo);
// 				//등록번호 12자리 만들기
// 				if ( strRegNo != "" && strRegNo.GetLength() < 12 ) {
// 					if ( strRegNo.GetLength() > 2 ) {
// 						CString sRight;
// 						sRight.Format( "%010s" , strRegNo.Mid(2) );
// 						strRegNo = strRegNo.Left(2) + sRight;
// 					}
// 				}
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_등록번호"), strRegNo, i);
// */
// 			}
// 
// 			if ( bError == FALSE )
// 			{			
// 				for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 				{ 
// 					DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), "1" , i ); 
// 				}
// 				for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 				{ 
// 					DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), "BOC113N" , i ); 
// 				}
// 				
// 				
// 				//대표책으로 배가실 부여
// 				INT nIdx;
// 				pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 				
// 				CString sTmpStreamMarc;
// 				CMarc marc;
// 				
// 				DM_Species.GetCellData( strHD_Species + _T("_MARC"), nIdx, sTmpStreamMarc);
// 				pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );
// 				
// 				ids = ((CCatApi_BO*)pDlg->m_pCatApi)->ApplyShelfCodeData(&DM_Book, &marc, FALSE);
// 				
// 				
// 				CStringArray m_arrStrAddFrame;
// 				
// 				// 소정 수정
// 				INT nids = -1;
// 				nids = DM_Species.StartFrame();
// 				if ( 0 > nids ) goto ERR;
// 				
// 				ids = pDlg->InsertSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, &DM_Volume, &m_arrStrAddFrame, FALSE);
// 				
// 				nids = DM_Species.SendFrame();
// 				if (nids < 0) {
// 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
// 					goto ERR;
// 				}
// 				
// 				nids = DM_Species.EndFrame();
// 				if ( 0 > nids ) goto ERR;
// 			}
// 		}		
// 		
// 		if (ids < 0 || bError == TRUE)
// 		{
// 			if ( bError == TRUE )
// 			{
// 				pGrid_TO = pGrid_ERR;
// 				nID_TO = 2;
// 				strTO = _T("오류 : ");				
// 			}
// 			else
// 			{
// 				pGrid_TO = pGrid_ERR;
// 				nID_TO = 2;
// 				strTO = _T("오류 : ");
// 				strErrorMsg = pDlg->GetLastError();
// 			}
// 		}
// 		//===================================================================
// 		// 11. 반입이 완료되면 성공탭에 옮기고 메인 그리드에 반영한다.
// 		//===================================================================
// 		else
// 		{
// 			CString strItemData;
// 			CString strRecKey;
// 
// 			pDlg->m_pDM_Main->InsertRow(-1);
// 			INT nRowIdx = pDlg->m_pDM_Main->GetRowCount() - 1;
// 			
// 			// 색인 정보 카피 
// 			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Main, nRowIdx);
// 			
// 			// 종 정보 카피
// 			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Main, nRowIdx);
// 
// 			// 2005.11.1 ADD BY PDJ
// 			strRecKey = pDlg->m_pDM_Main->GetCellData(_T("BS_종KEY"), nRowIdx );
// 			pDlg->m_pDM_Main->SetCellData(_T("IBS_종KEY"), strRecKey, nRowIdx );			
// 			
// 			// 시작 등록번호, 책수 넣어주기.
// 			INT nIdx = - 1;
// 			ids = pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			if (ids < 0 || nIdx < 0)
// 				nIdx = pDlg->SetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			pDlg->m_pDM_Main->SetCellData(_T("시작등록번호"), DM_Book.GetCellData( strHD_Book + _T("_등록번호"), nIdx), nRowIdx);
// 			
// 			strItemData.Format(_T("%d (%d)"), pDlg->GetBookCount(&DM_Book), pDlg->GetBookCount(&DM_Book));
// 			pDlg->m_pDM_Main->SetCellData(_T("책수"), strItemData, nRowIdx);
// 
// 			strItemData = _T("");
// 
// 			pDlg->m_pDM_Main->GetCellData( strHD_Species + _T("_정리상태"), nRowIdx, strItemData);
// 			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
// 
// 			if (strItemData == _T("1") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_정리상태"), _T("정리됨"), nMainGridRowIdx );
// 				INT ii = pDlg->m_pDM_Main->GetRowCount();
// 			}
// 			else if (strItemData == _T("0") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_정리상태"), _T("정리안됨"), nMainGridRowIdx );
// 			}
// 			
// 			pGrid_TO = pGrid_SUCCESS;
// 			nID_TO = 3;
// 			strTO = _T("성공 : ");
// 
// 			// 누락등록번호에 있는 등록번호라면 갱신해준다.
// 			CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_등록번호"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				//=====================================================
// 				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// // 				strRegCode = strRegNo.Left(2) ;
// // 				iRegNumber = _ttoi(strRegNo.Mid (2)) ;
// 
// 				strRegCode = strRegNo.Left(nRegCodeLength) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
// 				//=====================================================
// 				
// 				
//                 strQuery.Format (_T("delete from CO_DROP_ACCESSION_NO_TBL  where reg_code = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		}
//         
// /*        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	   // 2003년 10월 14일 이학중 수정 
// 	    if(!(ids<0)) 
// 		{ 
// 		    CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_등록번호"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				strRegCode = strRegNo.Left(2) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (2)) ;
// 				
//                 strQuery.Format (_T("delete from CO_DROP_ACCESSION_NO_TBL  where reg_code = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		
// 		}
//        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// */
// 
// 		//===================================================================
// 		// 12. 상태바의 개수를 설정한다.
// 		//===================================================================
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) - 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
// 		
// 	
// 
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) + 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);
// 
// 
// 		//2003.10.15 이학중 수정
//         nTotalCnt =  strTO+strData ; 
// 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 		//===================================================================
// 		// 13. 오류및 성공 탭으로 데이터를 옮긴다.
// 		//===================================================================
// 		nRowsTo = pGrid_TO->GetRows();
// 		if (pGrid_TO->GetTextMatrix(nRowsTo - 1, 0).IsEmpty())
// 		{
// 			nRowTo = nRowsTo - 1;
// 		}
// 		else
// 		{
// 			pGrid_TO->SetRows(nRowsTo + 1);
// 			nRowTo = nRowsTo;
// 		}
// 
// 		for (INT col = 0; col < nColCount; col++)
// 		{
// 			pGrid_TO->SetTextMatrix(nRowTo, col, pGrid_OK->GetTextMatrix(idx, col));
// 		}
// 
// 		// 오류 가 있으면 오류 세팅
// 		if (!strErrorMsg.IsEmpty())
// 		{
// 			pDlg->m_mapGridAlias.Lookup( strHD_Species + _T("_파일반입오류"), col);
// 			pGrid_TO->SetTextMatrix(nRowTo, col, strErrorMsg);
// 		}
// 
// 		//===================================================================
// 		// 14. 진행바를 움직여준다.
// 		//===================================================================
// 		pDlg->m_wndProgressBar.StepIt();
// 
// 		// Remove Row
// 		if (pGrid_OK->GetRows() == 2)
// 		{
// 			pGrid_OK->Clear();
// 			pGrid_OK->SetColumnInfo();
// 			break;
// 		}
// 		else
// 		{
// 			pGrid_OK->RemoveItem(idx);
// 		}
// 
// 		end_app = GetTickCount();
// 		nProcCount++;
// 		strMsg.Format(_T("ROW %d \t %d.%03d\n"), nProcCount, (end_app-start_app)/1000, (end_app-start_app)%1000);	
// 		_ftprintf( fpTime, strMsg );		
// 	}
// 	//=======================================================================
// 	// 전체 반입시간
// 	end_total = GetTickCount();
// 	strMsg.Format(_T("전체반입시간 \t %d.%03d\n======================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
// 	_ftprintf( fpTime, strMsg );		
// 	fclose(fpTime);
// 		
// 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 완료."));
// 
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	//2003.10.20 이학중 수정 
//     nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
//     nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
// 	nTrueCnt  =  pDlg->m_wndStatusBarCtrl.GetText(3) ;
// 
// 	strTotalMsg.Format( _T("반입 완료. \r\n\r\n %s건 \r\n %s건 \r\n %s건"),nTotalCnt,nFalseCnt,nTrueCnt ); 
// 	AfxMessageBox(strTotalMsg);
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	SetEvent(pDlg->m_hThread);
// 
// 	return 0;
// 
// ERR:
// 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 실패"));
// 	AfxMessageBox(_T("반입 실패"));
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	return -1;
// 
// 	return -1;
// }
//===================================================
//===================================================
//2009.03.10 UPDATE BY PJW : TEMP DM 이용 구조
//UINT ThreadFunc_Import(LPVOID pParam)
//{
//	//=======================================================================
// 	// 1.Initialization Variable
// 	//=======================================================================
// 	CESL_ControlMgr *pCM = NULL;
// 	CESL_Grid *pGrid_OK = NULL;			// 정상인 그리드
// 	CESL_Grid *pGrid_ERR = NULL;		// 에러난 그리드
// 	CESL_Grid *pGrid_SUCCESS = NULL;	// 성공한 그리드
// 	CESL_Grid *pGrid_TO = NULL;			// 옮길 그리드
// 	// 개수증가시킬 ID
// 	UINT nID_TO = -1;
// 	// 개수증가시킬 String
// 	CString strTO;
// 	// DataMgr
// 	CString strDMPK;
//
//	INT nStopIdx;
// 	
// 	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
// 	if (pDlg == NULL)
// 	{
// 		pDlg->m_bIsThreadRunning = FALSE;
// 		return -1;
// 	}
// 
// 	pDlg->m_bIsThreadRunning = TRUE;
// 	pDlg->ButtonStatus(0);
// 
// 	// 종DM
// 	CESL_DataMgr DM_Species;
// 	DM_Species.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Species.InitDMPKFromFile(pDlg->m_lpszSpeciesDMAlias);
// 	DM_Species.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// 색인DM
// 	CESL_DataMgr DM_Index;
// 	DM_Index.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Index.InitDMPKFromFile(pDlg->m_lpszIndexDMAlias);
// 	DM_Index.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	// 책DM
// 	CESL_DataMgr DM_Book;
// 	DM_Book.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Book.InitDMPKFromFile(pDlg->m_lpszBookDMAlias);
// 	DM_Book.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// 권DM
// 	CESL_DataMgr DM_Volume;
// 	DM_Volume.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Volume.InitDMPKFromFile(pDlg->m_lpszVolumeDMAlias);
// 	DM_Volume.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	INT ids = -1;
// 	INT idxDM = -1;
// 	INT nCount = -1;
// 
// 	INT nColCount = -1;
// 
// 	INT nRowTo = 0;
// 	INT nRowsTo = 0;
// 	
// 	CString strData;
// 	CString strErrorMsg;
//     CString nTotalCnt;
//     CString strTotalMsg;
//     CString nFalseCnt;
// 	CString nTrueCnt;
// 
// 	DWORD start_app, end_app;
// 	CString strMsg;		
// 	INT		nProcCount = 0;
// 
// 	// HSS_040524 헤더세팅
// 	CString strHD_Book, strHD_Species, strHD_Index;
// 	strHD_Species = _T("BS");
// 	strHD_Book = _T("BB");
// 	strHD_Index = _T("IBS");
// 
// 
// 	// 그리드 설정
// 	pGrid_OK = pDlg->m_pGrid_OK;
// 	pGrid_ERR = pDlg->m_pGrid_ERR;	
// 	pGrid_SUCCESS = pDlg->m_pGrid_SUCCESS;
// 	
// 	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
// 		goto ERR;
// 
// 	//=====================================================
// 	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	INT nRegCodeLength;
// 	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
// 	//=====================================================
// 
// 	//=======================================================================
// 	// 2. 반입 수행 시간 체크 (체크를 왜하는지 모르겠음.)
// 	//=======================================================================
// 	FILE *fpTime;
// 	fpTime = _tfopen( _T("..\\tunning\\반입수행시간.txt"), _T("a+b") );	
// 	
// 		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// 		// KOLAS 2 UP UNICODE 수정작업
// 		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
// 		fseek(fpTime, 0, SEEK_END);		
// 		if( 0 == ftell(fpTime) )
// 		{
// 			/*UNCHANGE*/fputc(0xFF, fpTime);
// 			/*UNCHANGE*/fputc(0xFE, fpTime);
// 		}
// 		// ------------------------------------------------------------------------------
// 		
// 	DWORD start_total, end_total;
// 	start_total = GetTickCount();
// 		
// 	//=======================================================================
// 	// 3. 선택된 개수만큼 진행바를 초기화한다.
// 	//=======================================================================
// 	nCount = pGrid_OK->SelectGetCount();
// 	if (nCount <= 0)
// 	{
// 		goto ERR;
// 	}
// 	
// 	pDlg->m_wndProgressBar.SetRange(0, nCount);
// 	pDlg->m_wndProgressBar.SetPos(0);
// 	pDlg->m_wndProgressBar.SetWindowText(_T(""));
// 
// 	nColCount = pGrid_OK->GetCols(0);
// 
// 	// 쓰레드 시작
// 	BOOL	bError;	
//
//	//=====================================================
//	//2008.12.10 ADD BY PJW : 처음인지 아닌지를 체크한다.
//	BOOL bChkFirstWork;
//	bChkFirstWork = TRUE;
//// 	pGrid_OK->SelectMakeList();
//	//=====================================================
//
// 	while(TRUE)
// 	{
// 		bError = FALSE;
// 		start_app = GetTickCount();
// 		
// 		if (!pDlg->m_bIsThreadRunning) break;
// 		strErrorMsg.Empty();
//		//=====================================================
//		//2008.12.10 UPDATE BY PJW : 반입완료후 선정자료 그리드에서 제거하기 위해 수정
////  		pGrid_OK->SelectMakeList();
////  		INT idx = pGrid_OK->SelectGetHeadPosition() + 1;
//		INT idx;
//		if(bChkFirstWork == TRUE)
//		{
//			idx = pGrid_OK->SelectGetHeadPosition() + 1;
//			bChkFirstWork = FALSE;
//		}
//		else
//		{
//			idx = pGrid_OK->SelectGetNext() + 1;
//		}
//		if (idx <= 0) break;
//		//=====================================================
// 
// 		strData = pGrid_OK->GetTextMatrix(idx, 0);
// 		if (strData.IsEmpty()) continue;
// 
// 		idxDM = _ttoi(strData) - 1 ;
// 		
// 		//===================================================================
// 		// 4. 선정된 순서대로 마크정리 DM으로 변경한다.
// 		//===================================================================
// 		ids = pDlg->MakeMarcAdjustDM(pDlg->m_pDM_Species, idxDM, pDlg->m_pDM_Index, idxDM, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// //		CString MngCode = DM_Book.GetCellData(_T("BB_관리구분_코드"), 0);
// 		CMarc marc;
// 		
// 		//===================================================================
// 		// 5. 자동부여가 체크되어 있을경우 자동부여 DM 생성 및 마크를 수정한다.
// 		//===================================================================
// 		CButton * pChkGiveAutoRegno;
// 		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
// 		if ( pChkGiveAutoRegno->GetCheck() == 1 ) 
// 		{	
// 			INT ids = -1;
// 			// 등록구분 가져오기
// 			CString sRegCode;
// 			pDlg->GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Book + _T("_등록구분"), sRegCode );
// 
// 			// 마지막 번호 가져오기
// 			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
// 			CString sLastNum;	
// 			DM_Species.GetOneValueQuery( sQuery , sLastNum );
// 			INT iLastNum = _ttoi ( sLastNum );
// 
// 			//등록구분이 없으면 새로추가
// 			if ( sLastNum == _T("") ) {
// 				iLastNum = 0;
// 
// 				ids = DM_Species.StartFrame();
// 				if ( 0 > ids ) goto ERR;
// 				CString sRecKey;
// 				ids = DM_Species.MakeRecKey( sRecKey );
// 				if ( 0 > ids ) goto ERR;
// 				CString sQuery_reg;
// 				CTime time = CTime::GetCurrentTime();
// 				CString sYear;
// 				sYear.Format( _T("%04d"), time.GetYear() );
// 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
// 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
// //DEL 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER) ")
// //DEL 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s' );") 
// 								  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
// 				ids = DM_Species.AddFrame( sQuery_reg );
// 				if ( 0 > ids ) goto ERR;
// 
// 				ids = DM_Species.SendFrame();			
// 				if (ids < 0) {
// 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
// 					goto ERR;
// 				}
// 				ids = DM_Species.EndFrame();
// 				if ( 0 > ids ) goto ERR;
// 			}
// 
// 			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_마지막번호") );
// 			if (pDmLastNum == NULL) {
// 				AfxMessageBox( _T("DM_마지막번호 확인") );
// 				goto ERR;
// 			}
// 			pDmLastNum->FreeData();
// 			pDmLastNum->InsertRow(-1);
// 			pDmLastNum->SetCellData( strHD_Book + _T("_등록구분_코드"), sRegCode, 0 );
// 			
// 			//수정
// 			CString sMarc = DM_Species.GetCellData( strHD_Species + _T("_MARC"), 0 );
// 			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
// 			CArray<CString, CString&> pArr049l;
// 			CString strItemData;
// 			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
// 
// 
// 			//선정된 자료의 책수만 하도록 수정
// 			//홍수
// 			INT iBookCount = DM_Book.GetRowCount();
// 
// 			for ( INT j = 0 ; j < iBookCount ; j++ ) {
// 				iLastNum++;
// 				CString sRegNo;
// 				//=====================================================
// 				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// // 				sRegNo.Format( _T("%s%010d"), sRegCode, iLastNum );
// // 				sLastNum.Format( _T("%d"), iLastNum );
// 				sLastNum.Format( _T("%d"), iLastNum );
// 				sRegNo = pDlg->m_pInfo->MakeRegNo( sRegCode, sLastNum );
// 				//=====================================================
// 				
// 
// 				
// 				pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, 0 );
// 				
// 				CString sItem;
// 				if ( pArr049l.GetSize() == iBookCount ) {
// 					sItem = pArr049l.GetAt(j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
// 					//=====================================================
// 					//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 					//ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(2), j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j);
// 					//=====================================================
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
// 				}
// 				else {
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
// 					//=====================================================
// 					//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 					//ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(2), j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j);
// 					//=====================================================
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
// 				}
// 				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
// 				DM_Species.SetCellData( strHD_Species + _T("_MARC"), sMarc, 0 );
// 			}
// 		}
// 
// 		//================== 정리중반입, 소장반입 공통적용 ==================
// 		
// 		//===================================================================
// 		// 6. 책DM에 수입구분/등록일/등록번호 적용
// 		//===================================================================
// 		CString sAcqCode;		
// 		pDlg->GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Index + _T("_수입구분_코드"), sAcqCode );
// 		INT iBookCount = DM_Book.GetRowCount();
// 
// 		
// 
// 		for ( INT i = 0 ; i < iBookCount ; i++ ) {
// 			//수입구분 적용
// 			DM_Book.SetCellData( strHD_Book + _T("_수입구분_코드"), sAcqCode , i );
// 			
// 			//등록일 적용
// 			CString result;
// 			CTime t = CTime::GetCurrentTime();
// 			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록일"), result, i);
// 
// 			//등록번호 12자리 맨들기
// 			//=====================================================
// 			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// // 			CString strRegNo;
// // 			ids = DM_Book.GetCellData( strHD_Book + _T("_등록번호"), i, strRegNo);
// // 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// // 				if ( strRegNo.GetLength() > 2 ) {
// // 					CString sRight;
// // 					sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// // 					strRegNo = strRegNo.Left(2) + sRight;
// // 				}
// // 			}
// 			CString strRegNo;
// 			ids = DM_Book.GetCellData( strHD_Book + _T("_등록번호"), i, strRegNo);
// 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 				if ( strRegNo.GetLength() > nRegCodeLength ) {
// 					strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
// 				}
// 			}
// 			//=====================================================
// 			
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록번호"), strRegNo, i);
// 			// 원부키 설정
// 			if ( pDlg->m_strAccessionRecKey != _T("") )
// 			{
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_원부KEY"), pDlg->m_strAccessionRecKey, i);
// 			}
// 						
// 		}
// 
// 		//===================================================================
// 		// 7. MARC의 049$l값 모두 12자리로 만들기
// 		//===================================================================
// 		CArray<CString, CString&> arr049Item;
// 		CString strItemData;
// 		ids = pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arr049Item);
// 		for ( INT n049Cnt = 0 ; n049Cnt < arr049Item.GetSize(); n049Cnt++ )
// 		{
// 			CString str049Item;
// 			str049Item = arr049Item.GetAt( n049Cnt );
// 			//=====================================================
// 			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// // 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) {
// // 				if ( str049Item.GetLength() > 2 ) {
// // 					CString sRight;
// // 					sRight.Format( _T("%010s") , str049Item.Mid(2) );
// // 					str049Item = str049Item.Left(2) + sRight;
// // 				}
// // 			}
// 			
// 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) {
// 				if ( str049Item.GetLength() > nRegCodeLength ) {
// 					str049Item = pDlg->m_pInfo->MakeRegNo(str049Item.Left(nRegCodeLength), str049Item.Mid(nRegCodeLength));
// 				}
// 			}
// 			//=====================================================
// 
// 			
// 
// 			pDlg->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strItemData, arr049Item.GetAt( n049Cnt ) );
// 		}
// 		
// 		//===================================================================
// 		// 8. 종의 정리상태를 1로 세팅
// 		//===================================================================
// 		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) {
// 			DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , i );
// 		}
// 		//		MngCode = DM_Book.GetCellData(_T("BB_관리구분_코드"), 0);
// 		
// 		//===================================================================
// 		// 9. 소장반입이면 작업상태를 소장으로 DB에 INSERT한다. 
// 		//===================================================================
// 		CButton * pChkWorkStatus;	
// 		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
// 		if ( pChkWorkStatus->GetCheck() == 0 ) 
// 		{
// 			for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 			{ 
// 				DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), _T("0") , i ); 
// 			}
// 			for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 			{ 
// 				DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), _T("BOL112N") , i ); 
// 			}
// 			
// 			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// 		}
// 		//===================================================================
// 		// 10. 정리중 반입이면 작업상태를 소장으로/대표책 배가실 부여/ DB에 INSERT한다.
// 		//===================================================================
// 		else 
// 		{
// 			CString strRegNo;
// 			CString strQuery;
// 			CString strResult;			
// 			bError = FALSE;
// 			for ( int i = 0 ; i < DM_Book.GetRowCount() ; i++ ) 
// 			{
// 				// 2008.04.23 ADD BY PDJ
// 				// 정리중 반입일 경우도 등록번호 중복조사
// 				// 등록번호 넣기
// 				DM_Book.GetCellData( _T("BB_등록번호"), i, strRegNo );
// 				
// 				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), DM_Book.TBL_NAME, strRegNo);
// 				ids = DM_Species.GetOneValueQuery(strQuery, strResult);				
// 				if (ids >= 0 && !strResult.IsEmpty())
// 				{
// 					//등록번호 중복조사
// 					strErrorMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
// 					bError = TRUE;
// 					break;					
// 				}
// 				DM_Book.SetCellData( strHD_Book + _T("_자료상태"), "BOC113N" , i );
// 				// 앞서 했기때문에 생략
// /*				CString strRegNo;
// 				ids = DM_Book.GetCellData( strHD_Book + _T("_등록번호"), i, strRegNo);
// 				//등록번호 12자리 만들기
// 				if ( strRegNo != "" && strRegNo.GetLength() < 12 ) {
// 					if ( strRegNo.GetLength() > 2 ) {
// 						CString sRight;
// 						sRight.Format( "%010s" , strRegNo.Mid(2) );
// 						strRegNo = strRegNo.Left(2) + sRight;
// 					}
// 				}
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_등록번호"), strRegNo, i);
// */
// 			}
// 
// 			if ( bError == FALSE )
// 			{			
// 				for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 				{ 
// 					DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), "1" , i ); 
// 				}
// 				for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 				{ 
// 					DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), "BOC113N" , i ); 
// 				}
// 				
// 				
// 				//대표책으로 배가실 부여
// 				INT nIdx;
// 				pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 				
// 				CString sTmpStreamMarc;
// 				CMarc marc;
// 				
// 				DM_Species.GetCellData( strHD_Species + _T("_MARC"), nIdx, sTmpStreamMarc);
// 				pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );
// 				
// 				ids = ((CCatApi_BO*)pDlg->m_pCatApi)->ApplyShelfCodeData(&DM_Book, &marc, FALSE);
// 				
// 				
// 				CStringArray m_arrStrAddFrame;
// 				
// 				// 소정 수정
// 				INT nids = -1;
// 				nids = DM_Species.StartFrame();
// 				if ( 0 > nids ) goto ERR;
// 				
// 				ids = pDlg->InsertSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, &DM_Volume, &m_arrStrAddFrame, FALSE);
//				//=====================================================
//				//2008.12.29 ADD BY PJW : 홈체이지 IDX_BO_TBL에 추가할 쿼리를 SENDFRAME바로 앞으로 뺀다.
//				CString strSpeciesKey;
//				DM_Species.GetCellData(_T("BS_종KEY"),0,strSpeciesKey);
//			 	pDlg->m_pCatApi->ApplyExtraQuery(&DM_Species, &DM_Index, &DM_Book, strSpeciesKey, &m_arrStrAddFrame);
//				//=====================================================
// 				
// 				nids = DM_Species.SendFrame();
// 				if (nids < 0) {
// 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
// 					goto ERR;
// 				}
// 				
// 				nids = DM_Species.EndFrame();
// 				if ( 0 > nids ) goto ERR;
// 			}
// 		}		
// 		
// 		if (ids < 0 || bError == TRUE)
// 		{
// 			if ( bError == TRUE )
// 			{
// 				pGrid_TO = pGrid_ERR;
// 				nID_TO = 2;
// 				strTO = _T("오류 : ");				
// 			}
// 			else
// 			{
// 				pGrid_TO = pGrid_ERR;
// 				nID_TO = 2;
// 				strTO = _T("오류 : ");
// 				strErrorMsg = pDlg->GetLastError();
// 			}
// 		}
// 		//===================================================================
// 		// 11. 반입이 완료되면 성공탭에 옮기고 메인 그리드에 반영한다.
// 		//===================================================================
// 		else
// 		{
// 			CString strItemData;
// 			CString strRecKey;
// 
// 			pDlg->m_pDM_Main->InsertRow(-1);
// 			INT nRowIdx = pDlg->m_pDM_Main->GetRowCount() - 1;
// 			
// 			// 색인 정보 카피 
// 			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Main, nRowIdx);
// 			
// 			// 종 정보 카피
// 			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Main, nRowIdx);
// 
// 			// 2005.11.1 ADD BY PDJ
// 			strRecKey = pDlg->m_pDM_Main->GetCellData(_T("BS_종KEY"), nRowIdx );
// 			pDlg->m_pDM_Main->SetCellData(_T("IBS_종KEY"), strRecKey, nRowIdx );			
// 			
// 			// 시작 등록번호, 책수 넣어주기.
// 			INT nIdx = - 1;
// 			ids = pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			if (ids < 0 || nIdx < 0)
// 				nIdx = pDlg->SetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			pDlg->m_pDM_Main->SetCellData(_T("시작등록번호"), DM_Book.GetCellData( strHD_Book + _T("_등록번호"), nIdx), nRowIdx);
// 			
// 			strItemData.Format(_T("%d (%d)"), pDlg->GetBookCount(&DM_Book), pDlg->GetBookCount(&DM_Book));
// 			pDlg->m_pDM_Main->SetCellData(_T("책수"), strItemData, nRowIdx);
// 
// 			strItemData = _T("");
// 
// 			pDlg->m_pDM_Main->GetCellData( strHD_Species + _T("_정리상태"), nRowIdx, strItemData);
// 			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
// 
// 			if (strItemData == _T("1") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_정리상태"), _T("정리됨"), nMainGridRowIdx );
// 				INT ii = pDlg->m_pDM_Main->GetRowCount();
// 			}
// 			else if (strItemData == _T("0") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_정리상태"), _T("정리안됨"), nMainGridRowIdx );
// 			}
// 			
// 			pGrid_TO = pGrid_SUCCESS;
// 			nID_TO = 3;
// 			strTO = _T("성공 : ");
// 
// 			// 누락등록번호에 있는 등록번호라면 갱신해준다.
// 			CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_등록번호"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				//=====================================================
// 				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// // 				strRegCode = strRegNo.Left(2) ;
// // 				iRegNumber = _ttoi(strRegNo.Mid (2)) ;
// 
// 				strRegCode = strRegNo.Left(nRegCodeLength) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
// 				//=====================================================
// 				
// 				
//                 strQuery.Format (_T("delete from CO_DROP_ACCESSION_NO_TBL  where reg_code = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		}
//         
// /*        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	   // 2003년 10월 14일 이학중 수정 
// 	    if(!(ids<0)) 
// 		{ 
// 		    CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_등록번호"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				strRegCode = strRegNo.Left(2) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (2)) ;
// 				
//                 strQuery.Format (_T("delete from CO_DROP_ACCESSION_NO_TBL  where reg_code = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		
// 		}
//        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// */
// 
// 		//===================================================================
// 		// 12. 상태바의 개수를 설정한다.
// 		//===================================================================
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) - 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
// 		
// 	
// 
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) + 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);
// 
// 
// 		//2003.10.15 이학중 수정
//         nTotalCnt =  strTO+strData ; 
// 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 		//===================================================================
// 		// 13. 오류및 성공 탭으로 데이터를 옮긴다.
// 		//===================================================================
// 		nRowsTo = pGrid_TO->GetRows();
// 		if (pGrid_TO->GetTextMatrix(nRowsTo - 1, 0).IsEmpty())
// 		{
// 			nRowTo = nRowsTo - 1;
// 		}
// 		else
// 		{
// 			pGrid_TO->SetRows(nRowsTo + 1);
// 			nRowTo = nRowsTo;
// 		}
// 
// 		for (INT col = 0; col < nColCount; col++)
// 		{
// 			pGrid_TO->SetTextMatrix(nRowTo, col, pGrid_OK->GetTextMatrix(idx, col));
// 		}
// 
// 		// 오류 가 있으면 오류 세팅
// 		if (!strErrorMsg.IsEmpty())
// 		{
// 			pDlg->m_mapGridAlias.Lookup( strHD_Species + _T("_파일반입오류"), col);
// 			pGrid_TO->SetTextMatrix(nRowTo, col, strErrorMsg);
// 		}
//		//=====================================================
//		//2008.12.10 ADD BY PJW : 중지하였을 때의 ROWINDEX를 저장한다.
//		nStopIdx = idx;
//		//=====================================================
// 
// 		//===================================================================
// 		// 14. 진행바를 움직여준다.
// 		//===================================================================
// 		pDlg->m_wndProgressBar.StepIt();
// 
//		//=====================================================
//		//2008.12.10 REM BY PJW : 반입완료후 DISPLAY 위해 제거
// 		// Remove Row
////  		if (pGrid_OK->GetRows() == 2)
////  		{
////  			pGrid_OK->Clear();
////  			pGrid_OK->SetColumnInfo();
////  			break;
////  		}
////  		else
////  		{
////  			pGrid_OK->RemoveItem(idx);
////  		}
//		//=====================================================
// 
// 		end_app = GetTickCount();
// 		nProcCount++;
// 		strMsg.Format(_T("ROW %d \t %d.%03d\n"), nProcCount, (end_app-start_app)/1000, (end_app-start_app)%1000);	
// 		_ftprintf( fpTime, strMsg );		
// 	}
// 	//=======================================================================
//	//=====================================================
//	//2008.12.10 ADD BY PJW : 반입완후 선정된 데이터를 제외하고 그리드 DISPLAY한다.
//	{
//		CESL_DataMgr * pGridTempDM;
//		CString strSelectChk;
//		pGridTempDM = NULL;
//		INT nGridRowCnt;
//		INT nColCnt;
//		INT nSelectChkIdx;
//		INT nDmRowIdx;
//		INT nDmRowCnt;
//		INT i ;
//		INT j ;
//		pGridTempDM = pDlg->FindDM( _T("DM_정리_마크반입_GRID_TEMP") );
//		nGridRowCnt = pGrid_OK->GetRows();
//		nColCnt = pGrid_OK->GetColCount();
//		nDmRowIdx = 0;
//		for( i = 0 ; i < nColCnt ; i++ )
//		{
//			if( pGrid_OK->GetTextMatrix( 0 , i) == _T("선정") )
//			{
//				nSelectChkIdx = i;
//				break;
//			}
//		}
//	
//		for( i = 0 ; i < nGridRowCnt-1 ; i++ )
//		{
//			
//			strSelectChk = pGrid_OK->GetTextMatrix( i+1 , nSelectChkIdx);
//			strSelectChk.TrimLeft(); strSelectChk.TrimRight();
//			CString strData;
//			if( i < nStopIdx )
//			{
//				if( strSelectChk == _T("") )
//				{
//					
//					pGridTempDM->InsertRow(-1);				
//					for( j = 0 ; j < nColCnt ; j++ )
//					{
//						strData = pGrid_OK->GetTextMatrix(i+1,j);
//						pGridTempDM->SetCellData(nDmRowIdx,j,strData);
//					}
//					++nDmRowIdx;
//				}
//			}
//			else
//			{
//				pGridTempDM->InsertRow(-1);				
//				for( j = 0 ; j < nColCnt ; j++ )
//				{
//					strData = pGrid_OK->GetTextMatrix(i+1,j);
//					pGridTempDM->SetCellData(nDmRowIdx,j,strData);
//				}
//				++nDmRowIdx;				
//			}
//		}
//
//		nDmRowCnt = pGridTempDM->GetRowCount();
//		pGrid_OK->Clear();
//		pGrid_OK->SetRows(2);
//		pGrid_OK->SetColumnInfo();
//
//		if(nDmRowCnt > 0)
//		{
//			pGrid_OK->SetRows(nDmRowCnt+1);
//			CString strData;
//			for( i = 0 ; i < nDmRowCnt ; i++ )
//			{
//				for( j = 0 ; j < nColCnt ; j++ )
//				{
//					strData = pGridTempDM->GetCellData(i,j);
//					pGrid_OK->SetTextMatrix(i+1,j,strData);
//				}
//			}
//			for( i = 0 ; i < nDmRowCnt ; i++ )
//			{
//				pGridTempDM->FreeData();
//			}
//		}		
//	}
//	//=====================================================
//
//
// 	// 전체 반입시간
// 	end_total = GetTickCount();
// 	strMsg.Format(_T("전체반입시간 \t %d.%03d\n======================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
// 	_ftprintf( fpTime, strMsg );		
// 	fclose(fpTime);
// 		
// 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 완료."));
// 
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	//2003.10.20 이학중 수정 
//     nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
//     nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
// 	nTrueCnt  =  pDlg->m_wndStatusBarCtrl.GetText(3) ;
// 
// 	strTotalMsg.Format( _T("반입 완료. \r\n\r\n %s건 \r\n %s건 \r\n %s건"),nTotalCnt,nFalseCnt,nTrueCnt ); 
// 	AfxMessageBox(strTotalMsg);
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	SetEvent(pDlg->m_hThread);
// 
// 	return 0;
// 
// ERR:
// 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 실패"));
// 	AfxMessageBox(_T("반입 실패"));
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	return -1;
// 
// 	return -1;
// }
//===================================================
//===================================================
//2009.03.10 UPDATE BY PJW : 모든그리드에 DM 적용
//UINT ThreadFunc_Import(LPVOID pParam)
//{
//	//=======================================================================
// 	// 1.Initialization Variable
// 	//=======================================================================
// 	CESL_ControlMgr *pCM = NULL;
// 	CESL_Grid *pGrid_OK = NULL;			// 정상인 그리드
//
//	CESL_DataMgr *pDM_OK		= NULL; // 정상인 DM
//	CESL_DataMgr *pDM_ERR		= NULL; // 에러난 DM
//	CESL_DataMgr *pDM_SUCCESS	= NULL; // 성공한 DM
// 	CESL_DataMgr *pDM_TO		= NULL;	// 옮길	  DM
//
// 	// 개수증가시킬 ID
// 	UINT nID_TO = -1;
// 	// 개수증가시킬 String
// 	CString strTO;
// 	// DataMgr
// 	CString strDMPK;
//
//// 	INT nStopIdx;
// 	
// 	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
// 	if (pDlg == NULL)
// 	{
// 		pDlg->m_bIsThreadRunning = FALSE;
// 		return -1;
// 	}
// 
// 	pDlg->m_bIsThreadRunning = TRUE;
// 	pDlg->ButtonStatus(0);
// 
// 	// 종DM
// 	CESL_DataMgr DM_Species;
// 	DM_Species.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Species.InitDMPKFromFile(pDlg->m_lpszSpeciesDMAlias);
// 	DM_Species.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// 색인DM
// 	CESL_DataMgr DM_Index;
// 	DM_Index.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Index.InitDMPKFromFile(pDlg->m_lpszIndexDMAlias);
// 	DM_Index.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	// 책DM
// 	CESL_DataMgr DM_Book;
// 	DM_Book.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Book.InitDMPKFromFile(pDlg->m_lpszBookDMAlias);
// 	DM_Book.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// 권DM
// 	CESL_DataMgr DM_Volume;
// 	DM_Volume.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Volume.InitDMPKFromFile(pDlg->m_lpszVolumeDMAlias);
// 	DM_Volume.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	INT ids = -1;
// 	INT idxDM = -1;
// 	INT nCount = -1;
// 
// 	INT nColCnt = -1;
// 
//// 	INT nRowTo = 0;
//// 	INT nRowsTo = 0;
// 	
// 	CString strData;
// 	CString strErrorMsg;
//	CString nTotalCnt;
//	CString strTotalMsg;
//	CString nFalseCnt;
// 	CString nTrueCnt;
// 
// 	DWORD start_app, end_app;
// 	CString strMsg;		
// 	INT		nProcCount = 0;
//	INT		nSelectCnt = 0;
// 
// 	// HSS_040524 헤더세팅
// 	CString strHD_Book, strHD_Species, strHD_Index;
// 	strHD_Species = _T("BS");
// 	strHD_Book = _T("BB");
// 	strHD_Index = _T("IBS");
// 
// 
// 	// 그리드 설정
// 	pGrid_OK = pDlg->m_pGrid_OK;
//
//	pDM_OK		= pDlg->m_pDmGridOk;
//	pDM_ERR		= pDlg->m_pDmGridError;
//	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;
// 	
// 	if ( pGrid_OK == NULL )
// 		goto ERR;
// 
// 	//=====================================================
// 	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	INT nRegCodeLength;
// 	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
// 	//=====================================================
// 
// 	//=======================================================================
// 	// 2. 반입 수행 시간 체크 (체크를 왜하는지 모르겠음.)
// 	//=======================================================================
// 	FILE *fpTime;
// 	fpTime = _tfopen( _T("..\\tunning\\반입수행시간.txt"), _T("a+b") );	
// 	
// 	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// 	// KOLAS 2 UP UNICODE 수정작업
// 	// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
// 	fseek(fpTime, 0, SEEK_END);		
// 	if( 0 == ftell(fpTime) )
// 	{
// 		/*UNCHANGE*/fputc(0xFF, fpTime);
// 		/*UNCHANGE*/fputc(0xFE, fpTime);
// 	}
// 	// ------------------------------------------------------------------------------
// 		
// 	DWORD start_total, end_total;
// 	start_total = GetTickCount();
// 		
// 	//=======================================================================
// 	// 3. 선택된 개수만큼 진행바를 초기화한다.
// 	//=======================================================================
//	pGrid_OK->SelectMakeList();
// 	nCount = pGrid_OK->SelectGetCount();
// 	if (nCount <= 0)
// 	{
// 		goto ERR;
// 	}
// 	
// 	pDlg->m_wndProgressBar.SetRange(0, nCount);
// 	pDlg->m_wndProgressBar.SetPos(0);
// 	pDlg->m_wndProgressBar.SetWindowText(_T(""));
// 
// 	nColCnt = pDM_OK->GetRefColumnCount();
// 
// 	// 쓰레드 시작
// 	BOOL	bError;	
//
//	INT idx;
//	idx = pGrid_OK->SelectGetHeadPosition();
//	
//	
// 	while( TRUE )
// 	{
//		if( idx < 0 )
//		{
//			break;
//		}
//
// 		bError = FALSE;
// 		start_app = GetTickCount();
// 		
// 		if ( !pDlg->m_bIsThreadRunning ) 
//		{
//			break;
//		}
// 		strErrorMsg.Empty();
//
//		strData = pDM_OK->GetCellData(_T("BS_파일반입오류"), idx); 
//		if( !strData.IsEmpty() )
//		{
//			idx = pGrid_OK->SelectGetNext();
//			pDlg->m_wndProgressBar.StepIt();
//			continue;
//		}
//  		strData = pDM_OK->GetCellData(_T("순위"), idx); 
//// 		strData = pGrid_OK->GetTextMatrix(idx+1, 0);
//		idxDM = _ttoi(strData) - 1 ;
// 		
// 		//===================================================================
// 		// 4. 선정된 순서대로 마크정리 DM으로 변경한다.
// 		//===================================================================
// 		ids = pDlg->MakeMarcAdjustDM(pDlg->m_pDM_Species, idxDM, pDlg->m_pDM_Index, idxDM, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// 		CMarc marc;
// 		
// 		//===================================================================
// 		// 5. 자동부여가 체크되어 있을경우 자동부여 DM 생성 및 마크를 수정한다.
// 		//===================================================================
// 		CButton * pChkGiveAutoRegno;
// 		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
// 		if ( pChkGiveAutoRegno->GetCheck() == 1 ) 
// 		{	
// 			INT ids = -1;
// 			// 등록구분 가져오기
// 			CString sRegCode;
// 			pDlg->GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Book + _T("_등록구분"), sRegCode );
// 
// 			// 마지막 번호 가져오기
// 			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
// 			CString sLastNum;	
// 			DM_Species.GetOneValueQuery( sQuery , sLastNum );
// 			INT iLastNum = _ttoi ( sLastNum );
// 
// 			//등록구분이 없으면 새로추가
// 			if ( sLastNum == _T("") ) 
//			{
// 				iLastNum = 0;
// 
// 				ids = DM_Species.StartFrame();
// 				if ( 0 > ids ) goto ERR;
// 				CString sRecKey;
// 				ids = DM_Species.MakeRecKey( sRecKey );
// 				if ( 0 > ids ) goto ERR;
// 				CString sQuery_reg;
// 				CTime time = CTime::GetCurrentTime();
// 				CString sYear;
// 				sYear.Format( _T("%04d"), time.GetYear() );
// 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
// 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
// 								  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
// 				ids = DM_Species.AddFrame( sQuery_reg );
// 				if ( 0 > ids ) goto ERR;
// 
// 				ids = DM_Species.SendFrame();			
// 				if (ids < 0) {
// 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
// 					goto ERR;
// 				}
// 				ids = DM_Species.EndFrame();
// 				if ( 0 > ids ) goto ERR;
// 			}
// 
// 			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_마지막번호") );
// 			if (pDmLastNum == NULL) {
// 				AfxMessageBox( _T("DM_마지막번호 확인") );
// 				goto ERR;
// 			}
// 			pDmLastNum->FreeData();
// 			pDmLastNum->InsertRow(-1);
// 			pDmLastNum->SetCellData( strHD_Book + _T("_등록구분_코드"), sRegCode, 0 );
// 			
// 			//수정
// 			CString sMarc = DM_Species.GetCellData( strHD_Species + _T("_MARC"), 0 );
// 			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
// 			CArray<CString, CString&> pArr049l;
// 			CString strItemData;
// 			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
// 
// 
// 			//선정된 자료의 책수만 하도록 수정
// 			//홍수
// 			INT iBookCount = DM_Book.GetRowCount();
// 
// 			for ( INT j = 0 ; j < iBookCount ; j++ ) 
//			{
// 				iLastNum++;
// 				CString sRegNo;
// 				sLastNum.Format( _T("%d"), iLastNum );
// 				sRegNo = pDlg->m_pInfo->MakeRegNo( sRegCode, sLastNum );
//
//				pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, 0 );
// 				
// 				CString sItem;
// 				if ( pArr049l.GetSize() == iBookCount ) 
//				{
// 					sItem = pArr049l.GetAt(j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j); 					
// 					
//					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
// 				}
// 				else 
//				{
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j);
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
// 				}
// 				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
// 				DM_Species.SetCellData( strHD_Species + _T("_MARC"), sMarc, 0 );
// 			}
// 		}
// 
// 		//================== 정리중반입, 소장반입 공통적용 ==================
// 		
// 		//===================================================================
// 		// 6. 책DM에 수입구분/등록일/등록번호 적용
// 		//===================================================================
// 		CString sAcqCode;		
// 		pDlg->GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Index + _T("_수입구분_코드"), sAcqCode );
// 		INT iBookCount = DM_Book.GetRowCount();
// 
// 		for ( INT i = 0 ; i < iBookCount ; i++ ) 
//		{
// 			//수입구분 적용
// 			DM_Book.SetCellData( strHD_Book + _T("_수입구분_코드"), sAcqCode , i );
// 			
// 			//등록일 적용
// 			CString result;
// 			CTime t = CTime::GetCurrentTime();
// 			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록일"), result, i);
// 
// 			//등록번호 12자리 맨들기 
// 			CString strRegNo;
// 			ids = DM_Book.GetCellData( strHD_Book + _T("_등록번호"), i, strRegNo);
// 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 				if ( strRegNo.GetLength() > nRegCodeLength ) {
// 					strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
// 				}
// 			}
//
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록번호"), strRegNo, i);
// 			// 원부키 설정
// 			if ( pDlg->m_strAccessionRecKey != _T("") )
// 			{
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_원부KEY"), pDlg->m_strAccessionRecKey, i);
// 			}
// 						
// 		}
// 
// 		//===================================================================
// 		// 7. MARC의 049$l값 모두 12자리로 만들기
// 		//===================================================================
// 		CArray<CString, CString&> arr049Item;
// 		CString strItemData;
// 		ids = pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arr049Item);
// 		for ( INT n049Cnt = 0 ; n049Cnt < arr049Item.GetSize(); n049Cnt++ )
// 		{
// 			CString str049Item;
// 			str049Item = arr049Item.GetAt( n049Cnt );
//
// 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) 
//			{
// 				if ( str049Item.GetLength() > nRegCodeLength ) {
// 					str049Item = pDlg->m_pInfo->MakeRegNo(str049Item.Left(nRegCodeLength), str049Item.Mid(nRegCodeLength));
// 				}
// 			}
//
// 			pDlg->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strItemData, arr049Item.GetAt( n049Cnt ) );
// 		}
// 		
// 		//===================================================================
// 		// 8. 종의 정리상태를 1로 세팅
// 		//===================================================================
//// 		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
////		{
//// 			DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , i );
//// 		}
// 		
// 		//===================================================================
// 		// 9. 소장반입이면 작업상태를 소장으로 DB에 INSERT한다. 
// 		//===================================================================
// 		CButton * pChkWorkStatus;	
// 		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
// 		if ( pChkWorkStatus->GetCheck() == 0 ) 
// 		{
//			INT nSpeciesRow;
//			INT nIdxRow;
//			nSpeciesRow = DM_Species.GetRowCount();
//			nIdxRow = DM_Index.GetRowCount();
//			if( nSpeciesRow == nIdxRow )
//			{
//				for ( i = 0 ; i < nSpeciesRow ; i++ ) 
// 				{ 
// 					DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), _T("0") , i ); 
//					DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), _T("BOL112N") , i ); 
//					//===================================================
//					//2009.03.12 UPDATE BY PJW : 마크반입은 청구기호를 생성해주지 않음으로 정리상태가 '0'이 들어간다.
//					//							 0 - 정리안됨, 1 - 정리됨
//// 					DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , i );
//					DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("0") , i );
//					//===================================================
//					
// 				}
//			}
//			else
//			{
// 				for ( i = 0 ; i < nSpeciesRow ; i++ ) 
// 				{ 
// 					DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), _T("0") , i ); 
//					//===================================================
//					//2009.03.12 UPDATE BY PJW : 마크반입은 청구기호를 생성해주지 않음으로 정리상태가 '0'이 들어간다.
//					//							 0 - 정리안됨, 1 - 정리됨
//// 					DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , i );
//					DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("0") , i );
//					//===================================================
//					
// 				}
// 				for ( i = 0 ; i < nIdxRow ; i++ ) 
// 				{ 
// 					DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), _T("BOL112N") , i ); 
// 				}
//			}
// 			
// 			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// 		}
// 		//===================================================================
// 		// 10. 정리중 반입이면 작업상태를 소장으로/대표책 배가실 부여/ DB에 INSERT한다.
// 		//===================================================================
// 		else 
// 		{
// 			CString strRegNo;
// 			CString strQuery;
// 			CString strResult;			
// 			bError = FALSE;
// 			for ( int i = 0 ; i < DM_Book.GetRowCount() ; i++ ) 
// 			{
// 				// 2008.04.23 ADD BY PDJ
// 				// 정리중 반입일 경우도 등록번호 중복조사
// 				// 등록번호 넣기
// 				DM_Book.GetCellData( _T("BB_등록번호"), i, strRegNo );
// 				
// 				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), DM_Book.TBL_NAME, strRegNo);
// 				ids = DM_Species.GetOneValueQuery(strQuery, strResult);				
// 				if (ids >= 0 && !strResult.IsEmpty())
// 				{
// 					//등록번호 중복조사
// 					strErrorMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
// 					bError = TRUE;
// 					break;					
// 				}
// 				DM_Book.SetCellData( strHD_Book + _T("_자료상태"), "BOC113N" , i );
// 			}
// 
// 			if ( bError == FALSE )
// 			{		
//				INT nSpeciesRow;
//				INT nIdxRow;
//				nSpeciesRow = DM_Species.GetRowCount();
//				nIdxRow = DM_Index.GetRowCount();
//				if( nSpeciesRow == nIdxRow )
//				{
//					for ( i = 0 ; i < nSpeciesRow ; i++ ) 
// 					{ 
// 						DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), "1" , i );
//						DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), "BOC113N" , i ); 
//						//===================================================
//						//2009.03.12 UPDATE BY PJW : 마크반입은 청구기호를 생성해주지 않음으로 정리상태가 '0'이 들어간다.
//						//							 0 - 정리안됨, 1 - 정리됨
//// 	 					DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , i );
//						DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("0") , i );
//						//===================================================
//						
// 					}
//				}
//				else
//				{
// 					for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 					{ 
// 						DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), "1" , i ); 
//						//===================================================
//						//2009.03.12 UPDATE BY PJW : 마크반입은 청구기호를 생성해주지 않음으로 정리상태가 '0'이 들어간다.
//						//							 0 - 정리안됨, 1 - 정리됨
//// 	 					DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , i );
//						DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("0") , i );
//						//===================================================
// 					}
// 					for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 					{ 
// 						DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), "BOC113N" , i ); 
// 					}
//				}
// 				
// 				
// 				//대표책으로 배가실 부여
// 				INT nIdx;
// 				pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 				
// 				CString sTmpStreamMarc;
// 				CMarc marc;
// 				
// 				DM_Species.GetCellData( strHD_Species + _T("_MARC"), nIdx, sTmpStreamMarc);
// 				pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );
// 				
// 				ids = ((CCatApi_BO*)pDlg->m_pCatApi)->ApplyShelfCodeData(&DM_Book, &marc, FALSE);
// 				
// 				
// 				CStringArray m_arrStrAddFrame;
// 				
// 				// 소정 수정
// 				INT nids = -1;
// 				nids = DM_Species.StartFrame();
// 				if ( 0 > nids ) 
//				{
//					goto ERR;
//				}
// 				
// 				ids = pDlg->InsertSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, &DM_Volume, &m_arrStrAddFrame, FALSE);
//				if (ids < 0 )
// 				{
//					strErrorMsg = pDlg->GetLastError(); 
//					if( strErrorMsg.IsEmpty() )
//					{
//						pDM_OK->SetCellData(_T("BS_파일반입오류"), strErrorMsg, idx);				
//					}
//					else
//					{				
//						pDM_OK->SetCellData(_T("BS_파일반입오류"), _T("마크반입 에러"), idx);
//					}
//
//					nids = DM_Species.EndFrame();
//
// 					if ( 0 > nids ) 
//					{
//						goto ERR;
//					}
//
//					idx = pGrid_OK->SelectGetNext();
//					pDlg->m_wndProgressBar.StepIt();
//					continue;
// 				}
//				//=====================================================
//				//2008.12.29 ADD BY PJW : 홈체이지 IDX_BO_TBL에 추가할 쿼리를 SENDFRAME바로 앞으로 뺀다.
//				CString strSpeciesKey;
//				DM_Species.GetCellData(_T("BS_종KEY"),0,strSpeciesKey);
//			 	pDlg->m_pCatApi->ApplyExtraQuery(&DM_Species, &DM_Index, &DM_Book, strSpeciesKey, &m_arrStrAddFrame);
//				//=====================================================
// 				
// 				nids = DM_Species.SendFrame();
// 				if (nids < 0) {
// 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
// 					goto ERR;
// 				}
// 				
// 				nids = DM_Species.EndFrame();
// 				if ( 0 > nids ) goto ERR;
// 			}
// 		}		
// 		
// 		if (bError == TRUE)
// 		{
//// 			pDM_TO = pDM_ERR;
//// 			nID_TO = 2;
//// 			strTO = _T("오류 : ");				
// 			strErrorMsg = pDlg->GetLastError();
//			if (!strErrorMsg.IsEmpty())
// 			{
// 				pDM_OK->SetCellData( _T("BS_파일반입오류"), strErrorMsg, idx);
// 			}
//			else
//			{				
//				pDM_OK->SetCellData(_T("BS_파일반입오류"), _T("마크반입 에러"), idx);
//			}
//
// 		}
// 		//===================================================================
// 		// 11. 반입이 완료되면 성공탭에 옮기고 메인 그리드에 반영한다.
// 		//===================================================================
// 		else
// 		{
// 			CString strItemData;
// 			CString strRecKey;
// 
// 			pDlg->m_pDM_Main->InsertRow(-1);
// 			INT nRowIdx = pDlg->m_pDM_Main->GetRowCount() - 1;
// 			
// 			// 색인 정보 카피 
// 			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Main, nRowIdx);
// 			
// 			// 종 정보 카피
// 			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Main, nRowIdx);
// 
// 			// 2005.11.1 ADD BY PDJ
// 			strRecKey = pDlg->m_pDM_Main->GetCellData(_T("BS_종KEY"), nRowIdx );
// 			pDlg->m_pDM_Main->SetCellData(_T("IBS_종KEY"), strRecKey, nRowIdx );			
// 			
// 			// 시작 등록번호, 책수 넣어주기.
// 			INT nIdx = - 1;
// 			ids = pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			if (ids < 0 || nIdx < 0)
// 				nIdx = pDlg->SetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			pDlg->m_pDM_Main->SetCellData(_T("시작등록번호"), DM_Book.GetCellData( strHD_Book + _T("_등록번호"), nIdx), nRowIdx);
// 			
// 			strItemData.Format(_T("%d (%d)"), pDlg->GetBookCount(&DM_Book), pDlg->GetBookCount(&DM_Book));
// 			pDlg->m_pDM_Main->SetCellData(_T("책수"), strItemData, nRowIdx);
// 
// 			strItemData = _T("");
// 
// 			pDlg->m_pDM_Main->GetCellData( strHD_Species + _T("_정리상태"), nRowIdx, strItemData);
// 			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
// 
// 			if (strItemData == _T("1") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_정리상태"), _T("정리됨"), nMainGridRowIdx );
// 				INT ii = pDlg->m_pDM_Main->GetRowCount();
// 			}
// 			else if (strItemData == _T("0") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_정리상태"), _T("정리안됨"), nMainGridRowIdx );
// 			}
// 			
//
//// 			pDM_TO = pDM_SUCCESS;
//// 			nID_TO = 3;
//// 			strTO = _T("성공 : ");
//
// 			// 누락등록번호에 있는 등록번호라면 갱신해준다.
// 			CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_등록번호"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				strRegCode = strRegNo.Left(nRegCodeLength) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
// 			
//				strQuery.Format (_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL  WHERE REG_CODE = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		}
// 
// 		//===================================================================
// 		// 12. 상태바의 개수를 설정한다.
// 		//===================================================================
//// 		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
//// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
//// 		strData.Format(_T("%d"), _ttoi(strData) - 1);
//// 		pDlg->m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
//// 		
//// 		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
//// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
//// 		strData.Format(_T("%d"), _ttoi(strData) + 1);
//// 		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);
// 
// 
// 		//2003.10.15 이학중 수정
////          nTotalCnt =  strTO+strData ; 
// 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 		//===================================================================
// 		// 13. 오류및 성공 탭으로 데이터를 옮긴다.
// 		//===================================================================
// 
// 		// 오류 가 있으면 오류 세팅
//// 		if (!strErrorMsg.IsEmpty())
//// 		{
//// 			pDM_OK->SetCellData( _T("BS_파일반입오류"), strErrorMsg, idx);
//// 		}
//
// 		//===================================================================
// 		// 14. 진행바를 움직여준다.
// 		//===================================================================
// 		pDlg->m_wndProgressBar.StepIt();
// 
//
// 		end_app = GetTickCount();
// 		nProcCount++;
// 		strMsg.Format(_T("ROW %d \t %d.%03d\n"), nProcCount, (end_app-start_app)/1000, (end_app-start_app)%1000);	
// 		_ftprintf( fpTime, strMsg );
//		
//		idx = pGrid_OK->SelectGetNext();
// 	}
//
// 	// 전체 반입시간
// 	end_total = GetTickCount();
// 	strMsg.Format(_T("전체반입시간 \t %d.%03d\n======================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
// 	_ftprintf( fpTime, strMsg );		
// 	fclose(fpTime);
// 		
// 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 완료."));
//
//	
//	idx = pGrid_OK->SelectGetTailPosition();
//	while( TRUE )
//	{
//		INT nRowsTo;
//		if( idx < 0 )
//		{
//			break;
//		}
//		
//		strData = pDM_OK->GetCellData( _T("BS_파일반입오류"), idx);
//		if( strData.IsEmpty() )
//		{
//			nRowsTo = pDM_SUCCESS->GetRowCount();
//			pDM_SUCCESS->InsertRow(-1);
// 			for (INT col = 0; col < nColCnt; col++)
// 			{	
//// 				CString strData;
//				strData = pDM_OK->GetCellData( idx ,col);
// 				pDM_SUCCESS->SetCellData( nRowsTo ,col,  strData);
//
// 			}						
//		}
//		else
//		{
//			nRowsTo = pDM_ERR->GetRowCount();
//			pDM_ERR->InsertRow(-1);
// 			for (INT col = 0; col < nColCnt; col++)
// 			{	
//// 				CString strData;
//				strData = pDM_OK->GetCellData( idx ,col);
// 				pDM_ERR->SetCellData( nRowsTo ,col,  strData);
//
// 			}	
//		}
//		pDM_OK->DeleteRow( idx);
//		idx = pGrid_OK->SelectGetPrev();
//	}
//
//	{
//	INT nRowCnt;
//	CString strCount;
//
//	nRowCnt = pDM_OK->GetRowCount();
//	strCount.Format(_T("정상 : %d"), nRowCnt);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);	
//	
//	nRowCnt = pDM_ERR->GetRowCount();
//
//	strCount.Format(_T("오류 : %d"), nRowCnt);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);
//
//	nRowCnt = pDM_SUCCESS->GetRowCount();
//	strCount.Format(_T("성공 : %d"), nRowCnt);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 3, 0);	
//	}
// 
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	//2003.10.20 이학중 수정 
//	nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
//    nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
// 	nTrueCnt  = pDlg->m_wndStatusBarCtrl.GetText(3) ;
// 
// 	strTotalMsg.Format( _T("반입 완료. \r\n\r\n %s건 \r\n %s건 \r\n %s건"),nTotalCnt,nFalseCnt,nTrueCnt ); 
// 	AfxMessageBox(strTotalMsg);
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
//
//	::PostMessage(pDlg->GetSafeHwnd(), WM_SORT_GRID, 0, 0);
//
// 	SetEvent(pDlg->m_hThread);
// 
// 	return 0;
// 
// ERR:
// 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 실패"));
// 	AfxMessageBox(_T("반입 실패"));
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	return -1;
// 
// 	return -1;
// }
UINT ThreadFunc_Import(LPVOID pParam)
{
	//=======================================================================
 	// 1.Initialization Variable
 	//=======================================================================
 	CESL_ControlMgr *pCM = NULL;
 	CESL_Grid *pGrid_OK = NULL;			// 정상인 그리드

	CESL_DataMgr *pDM_OK		= NULL; // 정상인 DM
	CESL_DataMgr *pDM_ERR		= NULL; // 에러난 DM
	CESL_DataMgr *pDM_SUCCESS	= NULL; // 성공한 DM
 	CESL_DataMgr *pDM_TO		= NULL;	// 옮길	  DM

 	// 개수증가시킬 ID
 	UINT nID_TO = -1;
 	// 개수증가시킬 String
 	CString strTO;
 	// DataMgr
 	CString strDMPK;

// 	INT nStopIdx;
 	
	// KOL.UDF.022
 	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;

 	if (&pDlg == NULL)
 	{
 		pDlg->m_bIsThreadRunning = FALSE;
 		return -1;
 	}
 
 	pDlg->m_bIsThreadRunning = TRUE;
 	pDlg->ButtonStatus(0);
 
 	// 종DM
 	CESL_DataMgr DM_Species;
 	DM_Species.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
 	DM_Species.InitDMPKFromFile(pDlg->m_lpszSpeciesDMAlias);
 	DM_Species.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 
 	// 색인DM
 	CESL_DataMgr DM_Index;
 	DM_Index.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
 	DM_Index.InitDMPKFromFile(pDlg->m_lpszIndexDMAlias);
 	DM_Index.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 	
 	// 책DM
 	CESL_DataMgr DM_Book;
 	DM_Book.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
 	DM_Book.InitDMPKFromFile(pDlg->m_lpszBookDMAlias);
 	DM_Book.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 
 	// 권DM
 	CESL_DataMgr DM_Volume;
 	DM_Volume.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
 	DM_Volume.InitDMPKFromFile(pDlg->m_lpszVolumeDMAlias);
 	DM_Volume.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);

	// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
	// 반입된 자료중에 가장 큰 등록번호 저장용 DM
	CESL_DataMgr DM_MaxRegBo;
	DM_MaxRegBo.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
	DM_MaxRegBo.InitDMPKFromFile(pDlg->m_lpszMaxRegNoDMAlias);
	DM_MaxRegBo.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
	DM_MaxRegBo.FreeData();	
	// 가장 큰 수입순번호 저장용DM
	CESL_DataMgr DM_MaxAcqNo;
	DM_MaxAcqNo.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
	DM_MaxAcqNo.InitDMPKFromFile(pDlg->m_lpszMaxAcqNoDMAlias);
	DM_MaxAcqNo.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
	DM_MaxAcqNo.FreeData();		
 	
 	INT ids = -1;
 	INT idxDM = -1;
	INT nStopCnt = 0;
//  	INT nCount = -1;
 
	INT nSelectRowCnt;
 	INT nColCnt = -1;
 
// 	INT nRowTo = 0;
// 	INT nRowsTo = 0;
 	
 	CString strData;
 	CString strErrorMsg;
	CString nTotalCnt;
	CString strTotalMsg;
	CString nFalseCnt;
 	CString nTrueCnt;
 
 	DWORD start_app, end_app;
 	CString strMsg;		
 	INT		nProcCount = 0;
	INT		nSelectCnt = 0;
 
 	// HSS_040524 헤더세팅
 	CString strHD_Book, strHD_Species, strHD_Index;
 	strHD_Species = _T("BS");
 	strHD_Book = _T("BB");
 	strHD_Index = _T("IBS");
 
 
 	// 그리드 설정
 	pGrid_OK = pDlg->m_pGrid_OK;

	pDM_OK		= pDlg->m_pDmGridOk;
	pDM_ERR		= pDlg->m_pDmGridError;
	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;
 	
 	if ( pGrid_OK == NULL )
 		goto ERR;
 
 	//=====================================================
 	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
 	INT nRegCodeLength;
 	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
 	//=====================================================
 
 	//=======================================================================
 	// 2. 반입 수행 시간 체크 (체크를 왜하는지 모르겠음.)
 	//=======================================================================
 	FILE *fpTime;
 	fpTime = _tfopen( _T("..\\tunning\\반입수행시간.txt"), _T("a+b") );	
 	
 	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
 	// KOLAS 2 UP UNICODE 수정작업
 	// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
 	fseek(fpTime, 0, SEEK_END);		
 	if( 0 == ftell(fpTime) )
 	{
 		/*UNCHANGE*/fputc(0xFF, fpTime);
 		/*UNCHANGE*/fputc(0xFE, fpTime);
 	}
 	// ------------------------------------------------------------------------------
 		
 	DWORD start_total, end_total;
 	start_total = GetTickCount();
 		
 	//=======================================================================
 	// 3. 선택된 개수만큼 진행바를 초기화한다.
 	//=======================================================================
	pGrid_OK->SelectMakeList();
 	nSelectRowCnt = pGrid_OK->SelectGetCount();
 	
 	pDlg->m_wndProgressBar.SetRange(0, nSelectRowCnt);
 	pDlg->m_wndProgressBar.SetPos(0);
 	pDlg->m_wndProgressBar.SetWindowText(_T(""));
 
 	nColCnt = pDM_OK->GetRefColumnCount();
 
 	// 쓰레드 시작
 	BOOL	bError;	

	INT nSelectIdx;
	INT nChkCnt;
	
	
	nSelectIdx = pGrid_OK->SelectGetHeadPosition();

	for( nChkCnt = 0 ; nChkCnt < nSelectRowCnt ; ++nChkCnt )
	{
		bError = FALSE;
 		start_app = GetTickCount();
		if ( !pDlg->m_bIsThreadRunning ) 
		{
			nStopCnt = nChkCnt;
			break;
		}

		strErrorMsg.Empty();

		strData = pDM_OK->GetCellData(_T("BS_파일반입오류"), nSelectIdx); 
		if( !strData.IsEmpty() )
		{
			nSelectIdx = pGrid_OK->SelectGetNext();
			pDlg->m_wndProgressBar.StepIt();
			continue;
		}

		strData = pDM_OK->GetCellData(_T("순위"), nSelectIdx); 
		idxDM = _ttoi(strData) - 1 ;

		//===================================================================
 		// 4. 선정된 순서대로 마크정리 DM으로 변경한다.
 		//===================================================================
 		ids = pDlg->MakeMarcAdjustDM(pDlg->m_pDM_Species, idxDM, pDlg->m_pDM_Index, idxDM, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);
 		CMarc marc;
	
		//===================================================================
 		// 5. 자동부여가 체크되어 있을경우 자동부여 DM 생성 및 마크를 수정한다.
 		//===================================================================
 		CButton * pChkGiveAutoRegno;
 		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
 		if ( pChkGiveAutoRegno->GetCheck() == 1 ) 
 		{	
 			INT ids = -1;
 			// 등록구분 가져오기
 			CString sRegCode;
 			pDlg->GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Book + _T("_등록구분"), sRegCode );
 
 			// 마지막 번호 가져오기
 			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
 			CString sLastNum;	
 			DM_Species.GetOneValueQuery( sQuery , sLastNum );
 			INT iLastNum = _ttoi ( sLastNum );
 
 			//등록구분이 없으면 새로추가
 			if ( sLastNum == _T("") ) 
			{
 				iLastNum = 0;
 
 				ids = DM_Species.StartFrame();
 				if ( 0 > ids ) goto ERR;
 				CString sRecKey;
 				ids = DM_Species.MakeRecKey( sRecKey );
 				if ( 0 > ids ) goto ERR;
 				CString sQuery_reg;
 				CTime time = CTime::GetCurrentTime();
 				CString sYear;
 				sYear.Format( _T("%04d"), time.GetYear() );
 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
 								  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
 				ids = DM_Species.AddFrame( sQuery_reg );
 				if ( 0 > ids ) goto ERR;
 
 				ids = DM_Species.SendFrame();			
 				if (ids < 0) {
 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
 					goto ERR;
 				}
 				ids = DM_Species.EndFrame();
 				if ( 0 > ids ) goto ERR;
 			}
 
 			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_마지막번호") );
 			if (pDmLastNum == NULL) 
			{
 				AfxMessageBox( _T("DM_마지막번호 확인") );
 				goto ERR;
 			}
 			pDmLastNum->FreeData();
 			pDmLastNum->InsertRow(-1);
 			pDmLastNum->SetCellData( strHD_Book + _T("_등록구분_코드"), sRegCode, 0 );
 			
 			//수정
 			CString sMarc = DM_Species.GetCellData( strHD_Species + _T("_MARC"), 0 );
 			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
 			CArray<CString, CString&> pArr049l;
 			CString strItemData;
 			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
 
 
 			//선정된 자료의 책수만 하도록 수정
 			//홍수
 			INT iBookCount = DM_Book.GetRowCount();
 
 			for ( INT j = 0 ; j < iBookCount ; j++ ) 
			{
 				iLastNum++;
 				CString sRegNo;
 				sLastNum.Format( _T("%d"), iLastNum );
 				sRegNo = pDlg->m_pInfo->MakeRegNo( sRegCode, sLastNum );

				pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, 0 );
 				
 				CString sItem;
 				if ( pArr049l.GetSize() == iBookCount ) 
				{
 					sItem = pArr049l.GetAt(j);
 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j); 					
 					
					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
 				}
 				else 
				{
 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j);
 					
 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
 				}
 				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
 				DM_Species.SetCellData( strHD_Species + _T("_MARC"), sMarc, 0 );
 			}
 		}
 
 		//================== 정리중반입, 소장반입 공통적용 ==================
 		
 		//===================================================================
 		// 6. 책DM에 수입구분/등록일/등록번호 적용
 		//===================================================================
 		CString sAcqCode;		
 		pDlg->GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Index + _T("_수입구분_코드"), sAcqCode );
 		INT iBookCount = DM_Book.GetRowCount();
 
 		for ( INT i = 0 ; i < iBookCount ; i++ ) 
		{
 			//수입구분 적용
 			DM_Book.SetCellData( strHD_Book + _T("_수입구분_코드"), sAcqCode , i );
 			
 			//등록일 적용
 			CString result;
 			CTime t = CTime::GetCurrentTime();
 			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록일"), result, i);
 
 			//등록번호 12자리 맨들기 
 			CString strRegNo;
 			ids = DM_Book.GetCellData( strHD_Book + _T("_등록번호"), i, strRegNo);
 			if ( strRegNo.GetLength() > nRegCodeLength && strRegNo.GetLength() < 12 ) 
			{
 				strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
 			}

 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록번호"), strRegNo, i);
 			// 원부키 설정
 			if ( pDlg->m_strAccessionRecKey != _T("") )
 			{
 				ids = DM_Book.SetCellData( strHD_Book + _T("_원부KEY"), pDlg->m_strAccessionRecKey, i);
 			}
 						
 		}
 
 		//===================================================================
 		// 7. MARC의 049$l값 모두 12자리로 만들기
 		//===================================================================
 		CArray<CString, CString&> arr049Item;
 		CString strItemData;
 		ids = pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arr049Item);
 		for ( INT n049Cnt = 0 ; n049Cnt < arr049Item.GetSize(); n049Cnt++ )
 		{
 			CString str049Item;
 			str049Item = arr049Item.GetAt( n049Cnt );

 			if ( str049Item.GetLength() > nRegCodeLength && str049Item.GetLength() < 12 ) 
			{
 				str049Item = pDlg->m_pInfo->MakeRegNo(str049Item.Left(nRegCodeLength), str049Item.Mid(nRegCodeLength));
 			}

 			pDlg->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strItemData, arr049Item.GetAt( n049Cnt ) );
 		}
 		
 		//===================================================================
 		// 8. 종의 정리상태를 1로 세팅
 		//===================================================================
		//===================================================
		//2009.03.17 UPDATE BY PJW : 1. 작업중인 종의 상태만 변경해준다.
//									 2. 마크반입은 청구기호를 생성해주지 않음으로 정리상태가 '0'이 들어간다.
//										0 - 정리안됨, 1 - 정리됨
		//2009.03.25 UPDATE BY PJW : 1. 정리에서의 반입은 정리상태를 1로 셋팅한다. -> 박민경씨 요구사항
// 		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
//		{
// 			DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , i );
// 		}
		DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , 0 );
		//===================================================
 		
 		//===================================================================
 		// 9. 소장반입이면 작업상태를 소장으로 DB에 INSERT한다. 
 		//===================================================================
 		CButton * pChkWorkStatus;	
 		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
 		if ( pChkWorkStatus->GetCheck() == 0 ) 
 		{
			DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), "0" , 0 );
			DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), "BOL112N" , 0 ); 
			
 			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
 		}
 		//===================================================================
 		// 10. 정리중 반입이면 작업상태를 소장으로/대표책 배가실 부여/ DB에 INSERT한다.
 		//===================================================================
 		else 
 		{
 			CString strRegNo;
 			CString strQuery;
 			CString strResult;			
 			bError = FALSE;
			strResult = _T("");
 			for ( INT i = 0 ; i < DM_Book.GetRowCount() ; i++ ) 
 			{
 				// 2008.04.23 ADD BY PDJ
 				// 정리중 반입일 경우도 등록번호 중복조사
 				// 등록번호 넣기
 				DM_Book.GetCellData( _T("BB_등록번호"), i, strRegNo );
 				
 				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), DM_Book.TBL_NAME, strRegNo);
 				ids = DM_Species.GetOneValueQuery(strQuery, strResult);	

//				if (ids >= 0 && !strResult.IsEmpty())
// 				{
// 					//등록번호 중복조사
// 					strErrorMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
// 					bError = TRUE;
// 					break;					
// 				}
				if ( !strResult.IsEmpty() )
 				{
 					//등록번호 중복조사
 					strErrorMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
 					bError = TRUE;
 					break;					
 				}

 				DM_Book.SetCellData( strHD_Book + _T("_자료상태"), "BOC113N" , i );
 			}
 
 			if ( bError == FALSE )
 			{		

				DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), "1" , 0 );
				DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), "BOC113N" , 0 );
 				
 				
 				//대표책으로 배가실 부여
 				INT nIdx;
 				pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
 				
 				CString sTmpStreamMarc;
 				CMarc marc;
 				
 				DM_Species.GetCellData( strHD_Species + _T("_MARC"), nIdx, sTmpStreamMarc);
 				pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );
 				
 				ids = ((CCatApi_BO*)pDlg->m_pCatApi)->ApplyShelfCodeData(&DM_Book, &marc, FALSE);
 				
 				
 				CStringArray m_arrStrAddFrame;
 				
 				// 소정 수정
 				INT nids = -1;
 				nids = DM_Species.StartFrame();
 				if ( 0 > nids ) 
				{
					goto ERR;
				}
 				
 				ids = pDlg->InsertSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, &DM_Volume, &m_arrStrAddFrame, FALSE);

				//=====================================================
				//2008.12.29 ADD BY PJW : 홈체이지 IDX_BO_TBL에 추가할 쿼리를 SENDFRAME바로 앞으로 뺀다.
				CString strSpeciesKey;
				DM_Species.GetCellData(_T("BS_종KEY"),0,strSpeciesKey);
			 	pDlg->m_pCatApi->ApplyExtraQuery(&DM_Species, &DM_Index, &DM_Book, strSpeciesKey, &m_arrStrAddFrame);
				//=====================================================
 				
 				nids = DM_Species.SendFrame();
 				if (nids < 0) {
 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
 					goto ERR;
 				}
 				
 				nids = DM_Species.EndFrame();
 				if ( 0 > nids ) goto ERR;
 			}
 		}		
 		
 		if ( bError == TRUE  || 0 > ids )
 		{
 			strErrorMsg = pDlg->GetLastError();
			if (!strErrorMsg.IsEmpty())
 			{
 				pDM_OK->SetCellData( _T("BS_파일반입오류"), strErrorMsg, nSelectIdx);
 			}
			else
			{				
				pDM_OK->SetCellData(_T("BS_파일반입오류"), _T("마크반입 에러"), nSelectIdx);
			}

 		}
 		//===================================================================
 		// 11. 반입이 완료되면 성공탭에 옮기고 메인 그리드에 반영한다.
 		//===================================================================
 		else
 		{
 			CString strItemData;
 			CString strRecKey;
 
 			pDlg->m_pDM_Main->InsertRow(-1);
 			INT nRowIdx = pDlg->m_pDM_Main->GetRowCount() - 1;
 			
 			// 색인 정보 카피 
 			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Main, nRowIdx);
 			
 			// 종 정보 카피
 			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Main, nRowIdx);
 
 			// 2005.11.1 ADD BY PDJ
 			strRecKey = pDlg->m_pDM_Main->GetCellData(_T("BS_종KEY"), nRowIdx );
 			pDlg->m_pDM_Main->SetCellData(_T("IBS_종KEY"), strRecKey, nRowIdx );			
 			
 			// 시작 등록번호, 책수 넣어주기.
 			INT nIdx = - 1;
 			ids = pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
 
 			if (ids < 0 || nIdx < 0)
			{
 				nIdx = pDlg->SetRepresentationBook(&DM_Book, strHD_Book, nIdx);
			}
 
 			pDlg->m_pDM_Main->SetCellData(_T("시작등록번호"), DM_Book.GetCellData( strHD_Book + _T("_등록번호"), nIdx), nRowIdx);
 			
 			strItemData.Format(_T("%d (%d)"), pDlg->GetBookCount(&DM_Book), pDlg->GetBookCount(&DM_Book));
 			pDlg->m_pDM_Main->SetCellData(_T("책수"), strItemData, nRowIdx);
 
 			strItemData = _T("");
 
 			
 
			//===================================================
			//2009.03.17 UPDATE BY PJW : 청구기호 문제로 마크반입하면 무조건 정리안됨으로 넣는다.
			//2009.03.25 UPDATE BY PJW : 1. 정리에서의 반입은 정리상태를 정리됨으로 셋팅한다. -> 박민경씨 요구사항
//			pDlg->m_pDM_Main->GetCellData( strHD_Species + _T("_정리상태"), nRowIdx, strItemData);
// 			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
//			if (strItemData == _T("1") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_정리상태"), _T("정리됨"), nMainGridRowIdx );
// 				INT ii = pDlg->m_pDM_Main->GetRowCount();
// 			}
// 			else if (strItemData == _T("0") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_정리상태"), _T("정리안됨"), nMainGridRowIdx );
// 			}
			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
			pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_정리상태"), _T("정리됨"), nMainGridRowIdx );
			//===================================================
			//===================================================
			//2009.10.13 ADD BY PJW : 청구기호를 그리드에 반영한다.
			CString strCallNoQuery;
			CString strTempData;
			
			pDlg->m_pDM_Main->GetCellData(_T("IBS_종KEY"), nMainGridRowIdx, strTempData);
			strCallNoQuery.Format( _T("SELECT ECO$F_MAKE_CALLNO$I( SE_SHELF_CODE,  CLASS_NO,  BOOK_CODE,  VOL_CODE) FROM IDX_BO_TBL WHERE REC_KEY = %s"), strTempData );
			pDlg->m_pDM_Main->GetOneValueQuery( strCallNoQuery, strTempData);
			pDlg->m_pDM_Main->SetCellData(_T("종청구기호"), strTempData, nMainGridRowIdx);
			//===================================================
 			
 			

// 			pDM_TO = pDM_SUCCESS;
// 			nID_TO = 3;
// 			strTO = _T("성공 : ");

 			// 누락등록번호에 있는 등록번호라면 갱신해준다.
 			CString strRegNo ;
 			CString strGetAccRegNo ;
 			CString strQuery ;
 			CString strRegCode ;
 			INT iRegNumber ;
 			INT nCount ;
 			CString strCount =_T("") ;
 			
 			nCount = DM_Book.GetRowCount () ;
 			DM_Book.GetCellData ( strHD_Book + _T("_등록번호"),idxDM,strRegNo);
 			if(!strRegNo.IsEmpty ())
 			{   
 				strRegCode = strRegNo.Left(nRegCodeLength) ;
 				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
 			
				strQuery.Format (_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL  WHERE REG_CODE = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
 				DM_Book.StartFrame () ;
 				DM_Book.InitDBFieldData () ;
 				DM_Book.AddFrame (strQuery);
 				DM_Book.SendFrame ();
 				DM_Book.EndFrame () ;
 			}

			// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
			// 완료된 책의 등록번호를 저장.
			ids = pDlg->SaveSuccRegNo(&DM_Book, &DM_MaxRegBo);

			// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
			// 수입순번호 저장
			ids = pDlg->SaveSuccAcqNo(&DM_Species, &DM_MaxAcqNo);			
 		}
 
 		//===================================================================
 		// 12. 상태바의 개수를 설정한다.
 		//===================================================================
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) - 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
// 		
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) + 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);
 
 
 		//2003.10.15 이학중 수정
//          nTotalCnt =  strTO+strData ; 
 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 		//===================================================================
 		// 13. 오류및 성공 탭으로 데이터를 옮긴다.
 		//===================================================================
 
 		// 오류 가 있으면 오류 세팅
// 		if (!strErrorMsg.IsEmpty())
// 		{
// 			pDM_OK->SetCellData( _T("BS_파일반입오류"), strErrorMsg, idx);
// 		}

 		//===================================================================
 		// 14. 진행바를 움직여준다.
 		//===================================================================
 		pDlg->m_wndProgressBar.StepIt(); 

 		end_app = GetTickCount();
 		nProcCount++;
 		strMsg.Format(_T("ROW %d \t %d.%03d\n"), nProcCount, (end_app-start_app)/1000, (end_app-start_app)%1000);	
 		_ftprintf( fpTime, strMsg );
		
		nSelectIdx = pGrid_OK->SelectGetNext();
 	}

	// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
	// 파일 끝까지 수행했으니 반입된 등록번호로 마지막번호 업데이트
	ids = pDlg->UpdateLastNo(&DM_MaxRegBo);
	if ( ids < 0 ) {
		AfxMessageBox(_T("UpdateLastNo() Error!"));
	}
	// 수입순번호 업데이트
	ids = pDlg->UpdateAcqNo(&DM_MaxAcqNo);
	if ( ids < 0 ) {
		AfxMessageBox(_T("UpdateAcqNo() Error!"));
	}

 	// 전체 반입시간
 	end_total = GetTickCount();
 	strMsg.Format(_T("전체반입시간 \t %d.%03d\n======================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
 	_ftprintf( fpTime, strMsg );		
 	fclose(fpTime);
 		
 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 완료."));

	nSelectIdx = pGrid_OK->SelectGetTailPosition();
	if ( !pDlg->m_bIsThreadRunning ) 
	{
		for( INT i = 0 ; i < nSelectRowCnt - nStopCnt ; i++ )
		{
			nSelectIdx = pGrid_OK->SelectGetPrev();
		}

		nSelectRowCnt = nStopCnt;
	}

	for( nChkCnt = 0 ; nChkCnt < nSelectRowCnt ; ++nChkCnt )
	{
		INT nRowsTo;

		strData = pDM_OK->GetCellData( _T("BS_파일반입오류"), nSelectIdx);

		if( strData.IsEmpty() )
		{
			pDM_TO = pDM_SUCCESS;
		}
		else
		{
			pDM_TO = pDM_ERR;
		}

		nRowsTo = pDM_TO->GetRowCount();
		pDM_TO->InsertRow(-1);
		
		for (INT col = 0; col < nColCnt; col++)
 		{	
			strData = pDM_OK->GetCellData( nSelectIdx ,col);
 			pDM_TO->SetCellData( nRowsTo ,col,  strData);

 		}

		pDM_OK->DeleteRow( nSelectIdx);
		nSelectIdx = pGrid_OK->SelectGetPrev();
	}

	{
	//===================================================================
 	// 상태바의 개수를 설정한다.
 	
	INT nRowCnt;
	CString strCount;

	nRowCnt = pDM_OK->GetRowCount();
	strCount.Format(_T("정상 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);	
	
	nRowCnt = pDM_ERR->GetRowCount();

	strCount.Format(_T("오류 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);

	nRowCnt = pDM_SUCCESS->GetRowCount();
	strCount.Format(_T("성공 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 3, 0);	
	}
	//===================================================================
 
 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 	//2003.10.20 이학중 수정 
	nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
    nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
 	nTrueCnt  = pDlg->m_wndStatusBarCtrl.GetText(3) ;
 
 	strTotalMsg.Format( _T("반입 완료. \r\n\r\n %s건 \r\n %s건 \r\n %s건"),nTotalCnt,nFalseCnt,nTrueCnt ); 
 	AfxMessageBox(strTotalMsg);
 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 	pDlg->m_bIsThreadRunning = FALSE;
 	pDlg->ButtonStatus(1);

	::PostMessage(pDlg->GetSafeHwnd(), WM_SORT_GRID, 0, 0);

 	SetEvent(pDlg->m_hThread);
 
 	return 0;
 
 ERR:
	// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
	// 에러가 발생하기 전까지 반입된 등록번호로 마지막번호 업데이트
	ids = pDlg->UpdateLastNo(&DM_MaxRegBo);
	if ( ids < 0 ) {
		AfxMessageBox(_T("UpdateLastNo() Error!"));
	}
	// 수입순번호 업데이트
	ids = pDlg->UpdateAcqNo(&DM_MaxAcqNo);
	if ( ids < 0 ) {
		AfxMessageBox(_T("UpdateAcqNo() Error!"));
	}

 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 실패"));
 	AfxMessageBox(_T("반입 실패"));
 	pDlg->m_bIsThreadRunning = FALSE;
 	pDlg->ButtonStatus(1);
 	return -1;
 
 	return -1;
 }

 // KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
VOID CMarcImportDlg::OnChkLastNoUp_REG()
{
	CButton * pStatus;	
	pStatus = (CButton *)GetDlgItem(IDC_ChkLastNoUp_REG);
	if ( pStatus->GetCheck() == 1 )  {
		m_bAutoReg = TRUE;
		CString strMsg = _T("");
		strMsg.Format(_T("등록번호가 부여된 마크(자료)를 반입하여 그대로 자관의 등록번호로 사용할 때 적용됩니다.\n")
			          _T("반입자료의 등록구분이 [관리 > 업무환경설정 > 마지막번호관리] 에서 관리 중 이어야 갱신됩니다.")
					  _T("비어있는(누락)번호는 [관리 > 업무환경설정 > 등록번호점검]에서 관리하십시오."));					  
		AfxMessageBox(strMsg);
	}
	else {
		m_bAutoReg = FALSE;
	}
}

// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
VOID CMarcImportDlg::OnChkLastNoUp_Acq()
{
	CButton * pStatus;	
	pStatus = (CButton *)GetDlgItem(IDC_ChkLastNoUp_Acq);
	if ( pStatus->GetCheck() == 1 )  {
		m_bAutoAcq = TRUE;
		CString strMsg = _T("");
		strMsg.Format(_T("수입순 청구기호가 부여된 마크(자료)를 반입 할 때 적용됩니다.\n")
			          _T("090$a , 090$b 의 해당값이 [관리 > 시스템환경설정 > 수입순번호관리] 에서 관리 중 이어야 갱신됩니다.\n")
					  _T("분류기호는 KDC이며, 별치기호는 적용되지 않습니다."));

		AfxMessageBox(strMsg);
	}
	else {
		m_bAutoAcq = FALSE;
	}
}	

// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
// 업로드 성공한 등록번호를 업데이트용도로 저장한다.
INT CMarcImportDlg::SaveSuccRegNo(CESL_DataMgr* pDM_Book, CESL_DataMgr* pDM_MaxRegBo)
{
	if ( m_bAutoReg == FALSE ) return 0;

	INT ids = -1, nRowCnt = 0, i = -1, j = -1, nRegCodeLength = -1, nSerialNo = 0, nSerialLng = 0;
	BOOL bNumber = TRUE;
	CString strRegCode = _T(""), strSerialNo = _T(""), strMsg = _T(""), strRegNo = _T(""), strCmpRegCode = _T("");	

	nRowCnt = pDM_Book->GetRowCount();
	if ( nRowCnt <= 0 ) return 0;
	
	// 등록구분 자리수 확인.
 	nRegCodeLength = this->m_pInfo->nREG_CODE_LENGTH;

	// 건별처리
	for( i = 0; i < nRowCnt; i++ ) {
		// 책인지 체크
		if (!IsBook(pDM_Book, i, _T("BB") ) ) continue;

		// 등록구분 / 일련번호 구하기
		ids = pDM_Book->GetCellData(_T("BB_등록구분_코드"), i, strRegCode);
		if ( ids < 0 ) return -5;
		ids = pDM_Book->GetCellData(_T("BB_등록번호"), i, strRegNo);
		if ( ids < 0 ) return -10;

		if ( strRegNo.GetLength() < 12) {
			strMsg.Format(_T("등록번호가 12자리여야 함 : [%s] 로 처리불가."), strRegNo);
			pDM_Book->QueryLog(strMsg.GetBuffer(0));
			continue;
		}

		strCmpRegCode = strRegNo.Left(nRegCodeLength);
		if ( strRegCode != strCmpRegCode ) {
			strMsg.Format(_T("등록구분과 번호에서 추출한 등록구분 다름 : [%s-%s] 로 처리불가."), strRegCode, strCmpRegCode);
			pDM_Book->QueryLog(strMsg.GetBuffer(0));
			continue;
		}

		// 뽑아낸 일련번호가 모두 숫자인지 체크
		strSerialNo = strRegNo.Mid(nRegCodeLength);		
		nSerialLng = strSerialNo.GetLength();
		for ( j = 0; j < nSerialLng; j++ ) {
			if ( '0' > strSerialNo.GetAt(j) || '9' < strSerialNo.GetAt(j) ) bNumber = FALSE;
		}
		if ( bNumber == FALSE ) {
			strMsg.Format(_T("등록번호에서 추출한 일련번호가 숫자가 아님 : [%s]"), strSerialNo);
			pDM_Book->QueryLog(strMsg.GetBuffer(0));
			continue;
		}
		
		// 등록구분별로 가장 큰 값인지 확인해서 넣는다.
		nSerialNo = _ttoi(strSerialNo);		
		ids = CheckNPutSerialNo(pDM_MaxRegBo, strRegCode, nSerialNo);
		if ( ids < 0 ) return -15;
	}	

	return 0;
}
// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
INT CMarcImportDlg::CheckNPutSerialNo(CESL_DataMgr* pDM_MaxRegBo, CString strRegCode, INT nSerialNo)
{
	INT ids = -1, nRowCnt = 0, i = 0, nCmpNo = 0;
	BOOL bExist = FALSE;
	CString strCmpRegCode = _T(""), strCmpSerialNo = _T(""), strSerialNo = _T("");

	nRowCnt = pDM_MaxRegBo->GetRowCount();
	if ( nRowCnt == 0 ) {
		pDM_MaxRegBo->InsertRow(-1);
		pDM_MaxRegBo->SetCellData(_T("BB_등록구분_코드"), strRegCode, 0);
		strSerialNo.Format(_T("%d"), nSerialNo);
		pDM_MaxRegBo->SetCellData(_T("가장큰일련번호"), strSerialNo, 0);
		return 0;
	}

	// 추가된 등록구분중에 없다면 추가.
	for ( i = 0 ; i < nRowCnt; i++) {
		ids = pDM_MaxRegBo->GetCellData(_T("BB_등록구분_코드"), i, strCmpRegCode);
		if ( strCmpRegCode == strRegCode ) bExist = TRUE;		
	}

	if ( bExist == FALSE ) {
		pDM_MaxRegBo->InsertRow(-1);
		nRowCnt = pDM_MaxRegBo->GetRowCount();
		pDM_MaxRegBo->SetCellData(_T("BB_등록구분_코드"), strRegCode, nRowCnt - 1);
		strSerialNo.Format(_T("%d"), nSerialNo);
		pDM_MaxRegBo->SetCellData(_T("가장큰일련번호"), strSerialNo, nRowCnt - 1);
		return 0;
	}

	// 가장 큰지 확인해서 바꿔치기한다.
	for ( i = 0; i < nRowCnt; i++ ) {
		ids = pDM_MaxRegBo->GetCellData(_T("BB_등록구분_코드"), i, strCmpRegCode);
		if ( strCmpRegCode != strRegCode ) continue;

		ids = pDM_MaxRegBo->GetCellData(_T("가장큰일련번호"), i, strCmpSerialNo);
		nCmpNo = _ttoi(strCmpSerialNo);

		if ( nSerialNo > nCmpNo ) {
			strSerialNo.Format(_T("%d"), nSerialNo);
			ids = pDM_MaxRegBo->SetCellData(_T("가장큰일련번호"), strSerialNo, i );
			break;
		}
	}

	return 0;
}

// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
INT CMarcImportDlg::UpdateLastNo(CESL_DataMgr* pDM_MaxRegBo)
{
	if ( m_bAutoReg == FALSE ) return 0;

	INT ids = -1, nRowCnt = 0, i = -1, nProcCnt = 0;
	CString strQuery = _T(""), strRegCode = _T(""), strSerialNo = _T(""), strReckey = _T(""), strYear = _T(""), strOneMsg = _T(""), strMsg = _T("");

	strMsg = _T("마지막번호가 업데이트 되었습니다.");
	nRowCnt = pDM_MaxRegBo->GetRowCount();
	if (nRowCnt <= 0 ) return 0;	

	// 건별처리
	for( i = 0; i < nRowCnt; i++ ) {
		ids = pDM_MaxRegBo->GetCellData(_T("BB_등록구분_코드"), i, strRegCode);
		if ( ids < 0 ) return -5;
		ids = pDM_MaxRegBo->GetCellData(_T("가장큰일련번호"), i, strSerialNo);
		if ( ids < 0 ) return -10;

		// 업데이트 대상 조회
		strQuery.Format(_T("SELECT REC_KEY FROM CO_LAST_NUMBER_TBL WHERE ")
			            _T("MANAGE_CODE=UDF_MANAGE_CODE AND ")
						_T("KIND_CODE='REG_NO' AND ")						
						_T("PUBLISH_FORM_CODE = 'CO' AND ")
						_T("DIVISION_VALUE='%s' AND ")
						_T("LAST_NUMBER < %s"), strRegCode, strSerialNo);
		ids = pDM_MaxRegBo->GetOneValueQuery( strQuery , strReckey );
		if ( ids < 0 && ids != -4007 ) return -15;

		if ( strReckey.GetLength() <= 0 ) {
			strQuery.Format(_T("결과값 REC_KEY : [NULL]여서 업데이트 대상 없음."));
			pDM_MaxRegBo->QueryLog(strQuery.GetBuffer(0));
			continue;
		}
		
		// 업데이트
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK='%s' WHERE REC_KEY=%s; "), strSerialNo, this->GetWorkLogMsg(_T("마크반입자동업데이트"), __WFILE__, __LINE__), strReckey);
		ids = pDM_MaxRegBo->StartFrame();
		if ( ids < 0 ) return -20;
		pDM_MaxRegBo->InitDBFieldData();		
		ids = pDM_MaxRegBo->AddFrame(strQuery);
		if ( ids < 0 ) return -30;
		ids = pDM_MaxRegBo->SendFrame();
		if ( ids < 0 ) return -35;
		ids = pDM_MaxRegBo->EndFrame();
		if ( ids < 0 ) return -40;

		// 메시지 작성.
		nProcCnt++;
		strOneMsg.Format(_T("\n등록구분 %s : %s"), strRegCode, strSerialNo);
		strMsg += strOneMsg;
	}

	// 완료메시지 출력.
	if ( nProcCnt <= 0 ) return 0;

	strMsg += _T(".");
	AfxMessageBox(strMsg);
	
	return 0;
}

// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
// 업로드 성공한 수입순번호를 업데이트용도로 저장한다.
INT CMarcImportDlg::SaveSuccAcqNo(CESL_DataMgr* pDM_Species, CESL_DataMgr* pDM_MaxAcqNo)
{
	if ( m_bAutoAcq == FALSE ) return 0;

	INT ids = -1, nRowCnt = 0, i = -1, j = -1, nAcqNo = 0, nLength = 0, n090aCnt = 0, n090bCnt = 0;
	BOOL bNumber = TRUE;
	CString strClassNo = _T(""), strAcqNo = _T(""), strMsg = _T(""), strCmpClassNo = _T("");	
	
	CMarc marc;
	CString strMarc = _T(""), strItemData = _T(""), str090a = _T(""), str090b = _T(""), strControlNo = _T("");
	CArray<CString, CString&> pArr090aItem;
	CArray<CString, CString&> pArr090bItem;
	CArray<CString, CString&> pArr001Item;
	
	strMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0 );
	ids = this->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	ids = this->m_pInfo->pMarcMgr->GetItem(&marc, _T("090$a"), strItemData, &pArr090aItem);
	ids = this->m_pInfo->pMarcMgr->GetItem(&marc, _T("090$b"), strItemData, &pArr090bItem);
	ids = this->m_pInfo->pMarcMgr->GetItem(&marc, _T("001*"), strControlNo, &pArr001Item);	

	n090aCnt = pArr090aItem.GetSize();
	n090bCnt = pArr090bItem.GetSize();
	if ( n090aCnt != n090bCnt) {
		strMsg.Format(_T("제어번호 %s 는 090$a와 $b의 갯수가 일치하지 않음. [090$a : %d, 090$b : %d]"), strControlNo, n090aCnt, n090bCnt);
		pDM_MaxAcqNo->QueryLog(strMsg.GetBuffer(0));
		return 0;
	}

	for( i = 0; i < n090aCnt; i++ ) {
		str090a = pArr090aItem.GetAt( i );
		str090b = pArr090bItem.GetAt( i );

		// 분류기호는 숫자 . 만 있어야 하고
		bNumber = TRUE;
		nLength = str090a.GetLength();
		for ( j = 0; j < nLength; j++ ) {
			if ( !(('0' <= str090a.GetAt(j) && '9' >= str090a.GetAt(j) ) || str090a.GetAt(j) == '.' )) bNumber = FALSE;
		}
		if ( bNumber == FALSE ) {
			strMsg.Format(_T("제어번호 %s 는 분류기호 판단 오류. [090$a : %s]"), strControlNo, str090a);
			pDM_MaxAcqNo->QueryLog(strMsg.GetBuffer(0));
			pArr090aItem.RemoveAll();
			pArr090bItem.RemoveAll();
			pArr001Item.RemoveAll();			
			return 0;
		}

		// 수입순번호는 숫자만 있어야 한다.
		bNumber = TRUE;
		nLength = str090b.GetLength();
		for ( j = 0; j < nLength; j++ ) {
			if ( '0' > str090b.GetAt(j) || '9' < str090b.GetAt(j) ) bNumber = FALSE;
		}
		if ( bNumber == FALSE ) {
			strMsg.Format(_T("제어번호 %s 는 도서기호가 숫자가 아님. [090$b : %s]"), strControlNo, str090b);
			pDM_MaxAcqNo->QueryLog(strMsg.GetBuffer(0));
			pArr090aItem.RemoveAll();
			pArr090bItem.RemoveAll();
			pArr001Item.RemoveAll();			
			return 0;
		}

		nAcqNo = _ttoi(str090b);
		ids = CheckNPutAcqNo(pDM_MaxAcqNo, str090a, nAcqNo);
		if ( ids < 0 ) {
			pArr090aItem.RemoveAll();
			pArr090bItem.RemoveAll();
			pArr001Item.RemoveAll();			
			return -15;
		}
	}

	pArr090aItem.RemoveAll();
	pArr090bItem.RemoveAll();
	pArr001Item.RemoveAll();	
	
	return 0;
}

// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
INT CMarcImportDlg::CheckNPutAcqNo(CESL_DataMgr* pDM_MaxAcqNo, CString strClassNo, INT nAcqNo)
{
	INT ids = -1, nRowCnt = 0, i = 0, nCmpNo = 0;
	BOOL bExist = FALSE;
	CString strCmpClassNo = _T(""), strCmpAcqNo = _T(""), strSerialNo = _T("");

	nRowCnt = pDM_MaxAcqNo->GetRowCount();
	if ( nRowCnt == 0 ) {
		pDM_MaxAcqNo->InsertRow(-1);
		pDM_MaxAcqNo->SetCellData(_T("분류기호"), strClassNo, 0);
		strSerialNo.Format(_T("%d"), nAcqNo);
		pDM_MaxAcqNo->SetCellData(_T("가장큰일련번호"), strSerialNo, 0);
		return 0;
	}

	// 추가된 등록구분중에 없다면 추가.
	for ( i = 0 ; i < nRowCnt; i++) {
		ids = pDM_MaxAcqNo->GetCellData(_T("분류기호"), i, strCmpClassNo);
		if ( strCmpClassNo == strClassNo ) bExist = TRUE;	
	}

	if ( bExist == FALSE ) {
		pDM_MaxAcqNo->InsertRow(-1);
		nRowCnt = pDM_MaxAcqNo->GetRowCount();
		pDM_MaxAcqNo->SetCellData(_T("분류기호"), strClassNo, nRowCnt - 1);
		strSerialNo.Format(_T("%d"), nAcqNo);
		pDM_MaxAcqNo->SetCellData(_T("가장큰일련번호"), strSerialNo, nRowCnt - 1);
		return 0;
	}

	// 가장 큰지 확인해서 바꿔치기한다.
	for ( i = 0; i < nRowCnt; i++ ) {
		ids = pDM_MaxAcqNo->GetCellData(_T("분류기호"), i, strCmpClassNo);
		if ( strCmpClassNo != strClassNo ) continue;

		ids = pDM_MaxAcqNo->GetCellData(_T("가장큰일련번호"), i, strCmpAcqNo);
		nCmpNo = _ttoi(strCmpAcqNo);

		if ( nAcqNo > nCmpNo ) {
			strSerialNo.Format(_T("%d"), nAcqNo);
			ids = pDM_MaxAcqNo->SetCellData(_T("가장큰일련번호"), strSerialNo, i );
			break;
		}
	}

	return 0;
}

// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
INT CMarcImportDlg::UpdateAcqNo(CESL_DataMgr* pDM_MaxAcqNo)
{
	if ( m_bAutoAcq == FALSE ) return 0;

	INT ids = -1, nRowCnt = 0, i = -1, nProcCnt = 0;
	CString strQuery = _T(""), strClassNo = _T(""), strSerialNo = _T(""), strReckey = _T(""), strOneMsg = _T(""), strMsg = _T("");

	strMsg = _T("수입순번호가 업데이트 되었습니다.");
	nRowCnt = pDM_MaxAcqNo->GetRowCount();
	if (nRowCnt <= 0 ) return 0;

	// 건별처리
	for( i = 0; i < nRowCnt; i++ ) {
		ids = pDM_MaxAcqNo->GetCellData(_T("분류기호"), i, strClassNo);
		if ( ids < 0 ) return -5;
		ids = pDM_MaxAcqNo->GetCellData(_T("가장큰일련번호"), i, strSerialNo);
		if ( ids < 0 ) return -10;

		// 업데이트 대상 조회
		strQuery.Format(_T("SELECT REC_KEY FROM CO_LAST_BOOK_ACQUISITNO_TBL WHERE ")
			            _T("MANAGE_CODE=UDF_MANAGE_CODE AND ")
						_T("CLASS_NO_TYPE='1' AND ")
						_T("CLASS_NO = '%s' AND ")
						_T("SEPARATE_SHELF_CODE IS NULL AND ")						
						_T("ACQUISIT_NO < %s"), strClassNo, strSerialNo);
		ids = pDM_MaxAcqNo->GetOneValueQuery( strQuery , strReckey );
		if ( ids < 0 && ids != -4007 ) return -15;

		if ( strReckey.GetLength() <= 0 ) {
			strQuery.Format(_T("결과값 REC_KEY : [NULL]여서 업데이트 대상 없음."));
			pDM_MaxAcqNo->QueryLog(strQuery.GetBuffer(0));
			continue;
		}
		
		// 업데이트
		strQuery.Format(_T("UPDATE CO_LAST_BOOK_ACQUISITNO_TBL SET ACQUISIT_NO = %s, LAST_WORK='%s' WHERE REC_KEY=%s; "), strSerialNo, this->GetWorkLogMsg(_T("마크반입자동업데이트"), __WFILE__, __LINE__), strReckey);
		ids = pDM_MaxAcqNo->StartFrame();
		if ( ids < 0 ) return -20;
		pDM_MaxAcqNo->InitDBFieldData();		
		ids = pDM_MaxAcqNo->AddFrame(strQuery);
		if ( ids < 0 ) return -30;
		ids = pDM_MaxAcqNo->SendFrame();
		if ( ids < 0 ) return -35;
		ids = pDM_MaxAcqNo->EndFrame();
		if ( ids < 0 ) return -40;

		// 메시지 작성.
		nProcCnt++;
		strOneMsg.Format(_T("\n분류기호 %s : %s"), strClassNo, strSerialNo);
		strMsg += strOneMsg;
	}

	// 완료메시지 출력.
	if ( nProcCnt <= 0 ) return 0;

	strMsg += _T(".");
	AfxMessageBox(strMsg);
	
	return 0;
}

//===================================================

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 파일패치 쓰레드
// [Min Spec] 
// 1. 상태바/진행바/그리드를 초기화한다.
// 2. 구성된 종DM으로 정상/에러 개수를 카운트 한다.
// 3. 책수를 세팅하고 등록번호및 GRID를 세팅한다.
/////////////////////////////////////////////////////////////////////////////
//===================================================
//2009.03.10 UPDATE BY PJW : 파일반입 그리드에 DM이용하는 구조로 변경
//UINT ThreadFunc_Display(LPVOID pParam)
//{
//	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
//	if (pDlg == NULL)
//	{
//		pDlg->m_bIsThreadRunning = FALSE;
//		return -1;
//	}
//
//	pDlg->m_bIsThreadRunning = TRUE;
//	pDlg->ButtonStatus(0);
//
//	// Initialization Variable
//	CESL_DataMgr *pDM_Species = NULL;
//	CESL_ControlMgr *pCM = NULL;
//
//	CESL_Grid *pGrid_OK			= NULL;
//	CESL_Grid *pGrid_ERR		= NULL;
//	CESL_Grid *pGrid_SUCCESS	= NULL;
//
//	CESL_Grid *pGrid = NULL;
//
//	CString strHD_Species;
//
//	// For Count
//	CString strCount;
//	
//	INT i = 0;
//	INT nTotalCount = -1;
//	INT nErrCount = 0;
//	INT nOKCount = 0;
//
//	// For Fetch
//	CString strGridAlias;
//
//	INT row = 0;
//	INT nGridRow = 0;
//	INT nErrGridRow = 0;
//	INT nOKGridRow = 0;
//	
//	POSITION pos = NULL;
//	INT nGridCol = -1;
//	INT nGridInxCount = 1;
//	INT stepQuota = 10;
//
//	//===================================================================
//	// 1. 상태바/진행바/그리드를 초기화한다.
//	//===================================================================
//
//	// 종정보 DM
//	pDM_Species = pDlg->m_pDM_Species;
//	if (pDM_Species == NULL)
//	{
//		goto ERR;
//	}
//	
//	nTotalCount = pDM_Species->GetRowCount() ;
//	
//	pCM = pDlg->FindCM(pDlg->m_lpszCMAlias);
//	if (pCM == NULL)
//	{
//		goto ERR;
//	}
//
//	// TOTAL_COUNT
//	strCount.Format(_T("전체 : %d"), nTotalCount);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);
//	
//	// 소정수정
//	//if ( nTotalCount/stepQuota == 0 )
//	//	pDlg->m_wndProgressBar.SetRange(0, 1);
//	//else 
//	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
//	
//	pDlg->m_wndProgressBar.SetStep(1);
//	pDlg->m_wndProgressBar.SetPos(0);
//	pDlg->m_wndProgressBar.SetWindowText(_T(""));
//		
//	// HSS_040524 헤더세팅
//	strHD_Species = _T("BS");
//
//	// ERR_COUNT
//	for (i = 0; i < nTotalCount; i++)
//	{
//		if (!pDM_Species->GetCellData( strHD_Species + _T("_파일반입오류"), i).IsEmpty())
//		{
//			nErrCount++;
//		}
//	}
//
//	strCount.Format(_T("오류 : %d"), nErrCount);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);
//		
//	// OK_COUNT
//	nOKCount = nTotalCount - nErrCount;
//	strCount.Format(_T("정상 : %d"), nOKCount);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);
//	
//	// SUCCESS_COUNT
//	pDlg->m_wndStatusBarCtrl.SetText(_T("성공 : 0"), 3, 0);
//
//	pGrid_OK		= pDlg->m_pGrid_OK;
//	pGrid_ERR		= pDlg->m_pGrid_ERR;
//	pGrid_SUCCESS	= pDlg->m_pGrid_SUCCESS;
//	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
//	{
//		goto ERR;
//	}
//
//	// Init Grid
//	pGrid_OK->Clear();
//	pGrid_OK->SetRows(2);
//	pGrid_OK->SetColumnInfo();
//
//	pGrid_ERR->Clear();
//	pGrid_ERR->SetRows(2);
//	pGrid_ERR->SetColumnInfo();
//
//	pGrid_SUCCESS->Clear();
//	pGrid_SUCCESS->SetRows(2);
//	pGrid_SUCCESS->SetColumnInfo();
//
//
//	// Set Grid Rows
//	if (nErrCount > 0)
//		pGrid_ERR->SetRows(nErrCount + 1);
//	if (nOKCount > 0)
//		pGrid_OK->SetRows(nOKCount + 1);
//
//	//========================   Fetch Start  ===========================
//	
//	for (row = 0; row < nTotalCount; row++)
//	{
//		CString sBookCnt;
//
//		//===============================================================
//		// 2. 구성된 종DM으로 정상/에러 개수를 카운트 한다.
//		//===============================================================
//		//if (0==row % stepQuota)
//		pDlg->m_wndProgressBar.StepIt();
//
//		if (pDM_Species->GetCellData( strHD_Species + _T("_파일반입오류"), row).IsEmpty())
//		{
//			pGrid = pGrid_OK;
//			nGridRow = ++nOKGridRow;
//		}
//		else
//		{
//			pGrid = pGrid_ERR;
//			nGridRow = ++nErrGridRow;
//		}
//
//		strCount.Format(_T("%d"), row+1);
//		pGrid->SetTextMatrix(nGridRow, 0, strCount);
//		
//
//		if (!pDlg->m_bIsThreadRunning) 
//			break;
//		
//		//===============================================================
//		// 3. 책수를 세팅하고 등록번호및 GRID를 세팅한다.
//		//===============================================================
//		pos = pDlg->m_mapGridAlias.GetStartPosition();
//
//		//=====================================================
//		//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
//		INT nRegCodeLength;
//		nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
//		//=====================================================
//		
//		while (pos)
//		{
//			pDlg->m_mapGridAlias.GetNextAssoc(pos, strGridAlias, nGridCol);
//			CString sTmp = pDM_Species->GetCellData(strGridAlias, row);
//
//			//자동부여 체크일 경우 그리드에 자동부여로 뿌려주기
//			CButton * pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
//			CButton * pChkMultiBook = (CButton *)pDlg->GetDlgItem(IDC_ChkMULTIBOOK);
//
//			if ( (pChkGiveAutoRegno->GetCheck() == 1) && strGridAlias ==  strHD_Species + _T("_MARC.049$l") ) 
//			{
//				pGrid->SetTextMatrix(nGridRow, nGridCol, _T("자동부여") );
//			
//				if( pChkMultiBook->GetCheck() == 1)
//				{
//					sBookCnt = pDM_Species->GetCellData(_T("UDF_책수"),row);
//					pGrid->SetTextMatrix(nGridRow, nGridCol+1,sBookCnt);
//				}
//				else
//					pGrid->SetTextMatrix(nGridRow, nGridCol+1, _T("1") );
//			}
//			else
//			{
//				//12자리(소정 수정 2004.09.21)+++++++++++++++++++++++++++
//				if ( strGridAlias == _T("BS_MARC.049$l") )
//				{
//					CString stmpRegNo = pDM_Species->GetCellData(strGridAlias, row);
//					if ( stmpRegNo != _T("") && stmpRegNo.GetLength() < 12 ) {
//						//=====================================================
//						//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
//						//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
//// 						if ( stmpRegNo.GetLength() > 2 ) {
//// 							CString sRight;
//// 							sRight.Format( _T("%010s") , stmpRegNo.Mid(2) );
//// 							stmpRegNo = stmpRegNo.Left(2) + sRight;
//// 						}
//						if ( stmpRegNo.GetLength() > nRegCodeLength ) {
//							stmpRegNo =  pDlg->m_pInfo->MakeRegNo( stmpRegNo.Left(nRegCodeLength) , stmpRegNo.Mid(nRegCodeLength) );
//						}
//						//=====================================================
//						
//					}
//					pGrid->SetTextMatrix(nGridRow, nGridCol, stmpRegNo);
//				}
//				// ++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				else pGrid->SetTextMatrix(nGridRow, nGridCol, pDM_Species->GetCellData(strGridAlias, row));
//			}
//		}
//	}
//
//	// 그리드 높이 설정
//	pGrid_ERR->SetRowHeight(-1, pGrid_ERR->m_nHeight * 15);
//	pGrid_OK->SetRowHeight(-1, pGrid_ERR->m_nHeight * 15);
//	pGrid_SUCCESS->SetRowHeight(-1, pGrid_ERR->m_nHeight * 15);
//	
//	//pDlg->m_wndProgressBar.StepIt();
//	pDlg->m_wndProgressBar.SetWindowText(_T("파일패치 완료"));
//	pDlg->m_bIsThreadRunning = FALSE;
//	pDlg->ButtonStatus(1);
//
//	SetEvent(pDlg->m_hThread);
//	
//	return 0;
//ERR:
//	pDlg->m_bIsThreadRunning = FALSE;
//	pDlg->ButtonStatus(1);
//	return -1;
//
//	return -1;
//}
UINT ThreadFunc_Display(LPVOID pParam)
{
	// KOL.UDF.022
	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	pDlg->m_bIsThreadRunning = TRUE;
	pDlg->ButtonStatus(0);

	// Initialization Variable
	CESL_DataMgr *pDM_Species = NULL;
	CESL_ControlMgr *pCM = NULL;

	CESL_DataMgr *pDM_OK		= NULL;
	CESL_DataMgr *pDM_ERR		= NULL;
	CESL_DataMgr *pDM_SUCCESS	= NULL;

	CESL_DataMgr *pTempDM = NULL;

	CString strHD_Species;

	// For Count
	CString strCount;
	
	INT i = 0;
	INT nTotalCount = -1;
	INT nErrCount = 0;
	INT nOKCount = 0;

	INT row = 0;

	CString strAlias;
	CStringArray astrAlias;
	CString strFieldName;
	CString strFieldType;
	CString strDataType;
	CString strData;
	INT nColCnt;

	CString strSortNo;
	strSortNo.Empty();

	//===================================================================
	// 1. 상태바/진행바/그리드를 초기화한다.
	//===================================================================

	// 종정보 DM
	pDM_Species = pDlg->m_pDM_Species;
	if (pDM_Species == NULL)
	{
		goto ERR;
	}
	
	nTotalCount = pDM_Species->GetRowCount() ;
	
	pCM = pDlg->FindCM(pDlg->m_lpszCMAlias);
	if (pCM == NULL)
	{
		goto ERR;
	}

	// TOTAL_COUNT
	strCount.Format(_T("전체 : %d"), nTotalCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);
	
	// 소정수정
	//if ( nTotalCount/stepQuota == 0 )
	//	pDlg->m_wndProgressBar.SetRange(0, 1);
	//else 
	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
	
	pDlg->m_wndProgressBar.SetStep(1);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));
		
	pDM_OK		= pDlg->m_pDmGridOk;
	pDM_ERR		= pDlg->m_pDmGridError;
	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;

	
// 	INT i;
	nColCnt = pDM_OK->GetRefColumnCount();
	for( i = 0 ; i < nColCnt ; i++ )
	{
		pDM_OK->FindColumnFieldName( i , strAlias , strFieldName ,strFieldType,strDataType,0,strData);
		astrAlias.Add(strAlias);
	}


	//========================   Fetch Start  ===========================
	INT nDmRowIdx;
	
	for (row = 0; row < nTotalCount; row++)
	{
		CString sBookCnt;

		//===============================================================
		// 2. 구성된 종DM으로 정상/에러 개수를 카운트 한다.
		//===============================================================
		pDlg->m_wndProgressBar.StepIt();

		if (pDM_Species->GetCellData( _T("BS_파일반입오류"), row).IsEmpty())
		{
			pTempDM = pDM_OK;
		}
		else
		{
			pTempDM = pDM_ERR;
		}
		nDmRowIdx = pTempDM->GetRowCount();
		pTempDM->InsertRow(-1);
		
		if (!pDlg->m_bIsThreadRunning) 
		{
			break;
		}
		
		//===============================================================
		// 3. 책수를 세팅하고 등록번호및 GRID를 세팅한다.
		//===============================================================
		INT nRegCodeLength;
		nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
		for( i = 0 ; i < nColCnt ; i++ )
		{
			strAlias = astrAlias.GetAt(i);
			if( _T("선정") == strAlias ) 
			{
				continue;
			}
			if( _T("순위") == strAlias )
			{
				strData.Format(_T("%d"), row+1);
			}
			else if(  _T("시작등록번호") == strAlias )
			{
				//자동부여 체크일 경우 그리드에 자동부여로 뿌려주기
				CButton * pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
				
				if ( (pChkGiveAutoRegno->GetCheck() == 1) ) 
				{
					strData = _T("자동부여");
				}
				else
				{
					strData = pDM_Species->GetCellData(_T("BS_MARC.049$l"), row);
					if ( strData.GetLength() > nRegCodeLength && strData.GetLength() < 12 ) 
					{
						strData =  pDlg->m_pInfo->MakeRegNo( strData.Left(nRegCodeLength) , strData.Mid(nRegCodeLength) );						
					}
				}
			}
			else if( _T("BS_파일반입책수") == strAlias )
			{
				CButton * pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
				if ( (pChkGiveAutoRegno->GetCheck() == 1) ) 
				{
					CButton * pChkMultiBook = (CButton *)pDlg->GetDlgItem(IDC_ChkMULTIBOOK);
					if( pChkMultiBook->GetCheck() == 1)
					{
						strData = pDM_Species->GetCellData(_T("UDF_책수"),row);
					}
					else
					{
						strData = _T("1");
					}
				}
				else
				{
					strData = pDM_Species->GetCellData(_T("BS_파일반입책수"),row);					
				}

				
			}
			else if( _T("BS_파일반입오류") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_파일반입오류"),row);
			}
			else if( _T("낱권ISBN") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_MARC.020$a"),row);
			}
			else
			{
				pDM_Species->GetCellData(("BS_MARC.") + strAlias, row, strData);
			}

			pTempDM->SetCellData( nDmRowIdx, i, strData);
		}
	}

	INT nRowCnt;

	nRowCnt = pDM_OK->GetRowCount();

	strCount.Format(_T("정상 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);	
	
	nRowCnt = pDM_ERR->GetRowCount();

	strCount.Format(_T("오류 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);

	pDlg->m_wndStatusBarCtrl.SetText(_T("성공 : 0"), 3, 0);

	pDlg->m_wndProgressBar.SetWindowText(_T("파일패치 완료"));

	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);

	SetEvent(pDlg->m_hThread);

	::PostMessage(pDlg->GetSafeHwnd(), WM_GRID_DISPLAY, 0, 0);
	
	return 0;
ERR:
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	return -1;

	return -1;
}
//===================================================

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 초기화
// [Return Values] 정상 1, 에러 -1
// [Mini Spec] 
// 1. API/ API 멤버 초기화
// 2. DM 초기화
// 3. 그리드 Alias 세팅
// 선정/시작등록번호/책수/본표제/저작자/발행자/총서표제/발행년/면장수/ISBN/오류
// 4. 레코드 형태와 형식구분 구성
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::Init()
{
	EFS_BEGIN

	INT ids = -1;

	//=======================================================================
	// 1. API/ API 멤버 초기화
	//=======================================================================
	m_pCatApi = new CCatApi_BO(this);
	if( m_pCatApi == NULL )
	{
		AfxMessageBox(_T("CCatApi_BO 초기화 실패"));
		return -1;
	}

	ids = m_pCatApi->InitMemberData();
	if( ids < 0 )
	{
		AfxMessageBox(_T("InitMemberData() Fail"));
		return -1;
	}

	//=======================================================================
	// 2. DM 초기화
	//=======================================================================
	m_lpszSpeciesDMAlias = _T("DM_단행_정리DM_종정보");
	m_lpszIndexDMAlias = _T("DM_단행_정리DM_색인정보");
	m_lpszVolumeDMAlias = _T("DM_단행_정리DM_권정보");
	m_lpszBookDMAlias = _T("DM_단행_정리DM_책정보");
	// KOL.RED.015 JMC 20160928 마크파일 반입시 마지막번호 자동 업데이트
	m_lpszMaxRegNoDMAlias = _T("DM_단행_정리DM_가장큰등록번호");
	m_lpszMaxAcqNoDMAlias = _T("DM_단행_정리DM_가장큰수입순번호");

	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	SetWindowText(_T("단행 마크 반입"));

	CString strHD_Species;
	strHD_Species = _T("BS");

	
	// 종DM을 구성한다.
	m_pDM_Species = new CESL_DataMgr;
	if (m_pDM_Species == NULL) 
	{
		AfxMessageBox(_T("m_pDM_Species is null"));
		return -1;
	}

	m_pDM_Species->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Species->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pDM_Species->InitDMPKFromFile(m_lpszSpeciesDMAlias);

	// 색인 DM을 구성한다.
	m_pDM_Index = new CESL_DataMgr;
	if (m_pDM_Index == NULL) 
	{
		AfxMessageBox(_T("m_pDM_Index is null"));
		return -1;
	}

	m_pDM_Index->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Index->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pDM_Index->InitDMPKFromFile(m_lpszIndexDMAlias);

	// 책 DM을 구성한다.
	m_pDM_Book = new CESL_DataMgr;
	if (m_pDM_Book == NULL) 
	{
		AfxMessageBox(_T("m_pDM_Book is null"));
		return -1;
	}
	m_pDM_Book->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Book->InitDMPKFromFile(m_lpszBookDMAlias);
	m_pDM_Book->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_Main = FindDM(_T("DM_단행_정리_검색결과"));
	if( m_pDM_Main == NULL )
	{
		AfxMessageBox(_T("m_pDM_Main is null"));
		return -1;
	}

	
	//===================================================
	//2009.03.10 ADD BY PJW : 반입받을 데이터 담을 DM
	// 정상자료 DM
	m_pDmGridOk = FindDM(_T("DM_정리_마크반입_GRID_OK"));
	if( m_pDmGridOk == NULL )
	{
		AfxMessageBox(_T("m_pDmOk is null"));
		return -1;
	}

	// 에러자료 DM
	m_pDmGridError = FindDM(_T("DM_정리_마크반입_GRID_ERROR"));
	if( m_pDmGridError == NULL )
	{
		AfxMessageBox(_T("m_pDmError is null"));
		return -1;
	}

	// 성공자료 DM
	m_pDmGridSuccess = FindDM(_T("DM_정리_마크반입_GRID_SUCCESS"));
	if( m_pDmGridSuccess == NULL )
	{
		AfxMessageBox(_T("m_pDmSuccess is null"));
		return -1;
	}
	//===================================================

	// CM
	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias);
	if (pCM == NULL)
	{
		AfxMessageBox(_T("pCM is null"));
		return -1;
	}

	// Grid
	m_pGrid_OK = (CESL_Grid*)pCM->FindControl(_T("Grid_OK"));
	// ERROR GRID
	m_pGrid_ERR = (CESL_Grid*)pCM->FindControl(_T("Grid_ERR"));
	// SUCCESS GRID
	m_pGrid_SUCCESS = (CESL_Grid*)pCM->FindControl(_T("Grid_SUCCESS"));

	if (m_pGrid_OK == NULL || m_pGrid_ERR == NULL || m_pGrid_SUCCESS == NULL)
	{
		AfxMessageBox(_T("Grids is null"));
		return -1;
	}

	// Move Windows
	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);
	
	m_pGrid_OK->MoveWindow(rcGrid);
	
	m_pGrid_ERR->MoveWindow(rcGrid);
	
	m_pGrid_SUCCESS->MoveWindow(rcGrid);

	
	//=======================================================================
	// 3. 그리드 Alias 세팅
	// 선정/시작등록번호/책수/본표제/저작자/발행자/총서표제/발행년/면장수/ISBN/오류
	//=======================================================================
	m_mapGridAlias.SetAt(_T(""), 1);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.049$l"), 2);
	m_mapGridAlias.SetAt( strHD_Species + _T("_파일반입책수"), 3);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.서명"), 4);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.저자"), 5);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.발행자"), 6);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.총서표제"), 7);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.발행년"), 8);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.면장수"), 9);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.020$a"), 10);
	m_mapGridAlias.SetAt( strHD_Species + _T("_파일반입오류"), 11);

	//=======================================================================
	// 4. 레코드 유형과 서지수준 으로 형식구분,매체구분(대표값)결정
	//=======================================================================
	// BK
	m_mapRecord_FormCodeMatCode.SetAt(_T("aa"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ac"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ad"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("am"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ta"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("tc"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("td"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("tm"), _T("BK=PR|OF"));

	// ER
	m_mapRecord_FormCodeMatCode.SetAt(_T("mi"), _T("ER=OB|OA|OD|OH"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("mm"), _T("ER=OB|OA|OD|OH"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ms"), _T("ER=OB|OA|OD|OH"));

	// MP
	m_mapRecord_FormCodeMatCode.SetAt(_T("ea"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("eb"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ec"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ed"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("em"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("es"), _T("MP=NA"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("fa"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("fc"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("fd"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("fm"), _T("MP=NA"));

	// MU
	m_mapRecord_FormCodeMatCode.SetAt(_T("ca"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cb"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cc"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cd"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cm"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cs"), _T("MU=NA"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("da"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("dc"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("dd"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("dm"), _T("MU=NA"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("ia"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ib"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ic"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("id"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("im"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("is"), _T("MU=SB|SD|SG"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("ja"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jb"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jc"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jd"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jm"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("js"), _T("MU=SB|SD|SG"));

	// CR
	m_mapRecord_FormCodeMatCode.SetAt(_T("ab"), _T("CR=PR"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ai"), _T("CR=PR"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("as"), _T("CR=PR"));	

	// VM
	m_mapRecord_FormCodeMatCode.SetAt(_T("ga"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gb"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gc"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gd"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gi"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gm"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gs"), _T("VM=VD|VT|VU"));		

	m_mapRecord_FormCodeMatCode.SetAt(_T("ka"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("kb"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("kc"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("kd"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ki"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("km"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ks"), _T("VM=LP|LQ|LY|LZ"));	

	m_mapRecord_FormCodeMatCode.SetAt(_T("ra"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rb"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rc"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rd"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ri"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rm"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rs"), _T("VM=VD|VT|VU"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("ya"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yb"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yc"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yd"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yi"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ym"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ys"), _T("VM=VD|VT|VU"));
	
	// RB
	m_mapRecord_FormCodeMatCode.SetAt(_T("oa"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ob"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("oc"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("od"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("oi"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("om"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("os"), _T("RB=PR"));

	// 복합자료
	m_mapRecord_FormCodeMatCode.SetAt(_T("pa"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pb"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pc"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pd"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pi"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pm"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ps"), _T("MP="));
	

	// 키워드 생성 다이얼로그
	CMarc marc;
	m_pDlgKeyWord = new CBO_CAT_PROC_KEYWORD_0000(this, marc);
	m_pDlgKeyWord->Create(this);
	m_pDlgKeyWord->ShowWindow(SW_HIDE);

	// 2005.07.01 ADD BY PDJ
	// USMARC반입시 선택항목 추가
	( (CButton*) GetDlgItem (IDC_rKORMARC) )->SetCheck(TRUE);

	// 반출 Unicode 선택
	( (CButton*) GetDlgItem (IDC_rUNICODE) )->SetCheck(TRUE);

	// 원부 설정
	OnChkAccessionRec();

	

	return 0;

	EFS_END
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 버튼 상태 세팅
// [Return Values] 
// [Mini Spec] 
// Mode : 0 작업진행중 -> 중지
// Mode : 1 작업완료 -> 닫기
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::ButtonStatus(INT nMode)
{
	EFS_BEGIN

	const INT nIDCount = 3;
	INT nArrID[nIDCount] = {
		IDC_bMODIFY,
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

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 파일로부터 마크를 읽어온다.
// [Return Values] 에러면 음
// [Mini Spec] 
// 1. DM알리아스 헤더세팅
// 2. 마크 구분별 마크 그룹 세팅
// 3. IMPORT/EXPORT MGR 초기화
// 4. Display Thread 시작 등록번호 자동부여이면 등록번호 제거
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::ImportMarcFromFile()
{
	EFS_BEGIN

	BeginWaitCursor();
	INT ids;

	//=======================================================================
	// 1. DM알리아스 헤더세팅
	//=======================================================================
	CString strHD_Species, strHD_Index, strHD_Book;
	strHD_Species = _T("BS");
	strHD_Book = _T("BB");
	strHD_Index = _T("IBS");

	if (m_strFilePath.IsEmpty()) return -1;

	//=======================================================================
	// 2. 마크 구분별 마크 그룹 세팅
	//=======================================================================
	CString sFormCode, sMarcGroup;
	GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Index + _T("_형식구분_코드"), sFormCode );
	
	//마크구분
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	//=======================================================================
	// 3. IMPORT/EXPORT MGR 초기화
	//=======================================================================
	// MarcMgr, FileType, FilePath 설정
	// File Type : XLS -- EXCEL 파일, TXT -- TEXT 파일
	CButton * pChkRegNoAutoGive;
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);

	m_marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, m_nFileType, m_strFilePath);	
	// 2007. 08.18 ADD BY PDJ
	// 등록번호 자동부여일 경우 049 제거하고 신택스 체크
	if ( pChkRegNoAutoGive->GetCheck() == 1 )
	{
		m_marcImpExpMgr.bRegNoAutoGrant = TRUE;
	}

	// Call Back Func 설정
	m_marcImpExpMgr.SetCallBackMarcCheck((INT (CWnd::*)(CMarc *pMarc, CString &str))CallBackCheckMarc);

	// DM 설정
	m_marcImpExpMgr.InitDMList();
	m_marcImpExpMgr.AddDMPtr(m_pDM_Species);
	m_marcImpExpMgr.AddDMPtr(m_pDM_Index);

	// 오류 체크 받을 DM Alias 설정
	m_marcImpExpMgr.SetErrChkAlias( strHD_Species + _T("_파일반입오류"));

	// 책수 받을 DM Alias 설정
	m_marcImpExpMgr.SetBookCountAlias( strHD_Species +_T("_파일반입책수"));

	// Stream Marc 받을 DM Alias 설정
	m_marcImpExpMgr.SetMarcStreamAlias( strHD_Species +_T("_MARC"));

	// Import 시작
	// 2005.07.01 ADD BY 
	// USMARC일 경우 파라메터 default 0;
	INT nCheck;
	nCheck = ( (CButton*) GetDlgItem (IDC_rUSMARC) )->GetCheck();		
	ids = m_marcImpExpMgr.ImportFile(_T(""),nCheck);	
	//=======================================================================
	// 4. Display Thread 시작 등록번호 자동부여이면 등록번호 제거
	//=======================================================================
	if (ids >= 0)
	{
		// 소정 수정
		// 등록번호 자동부여 -> 049Tag 삭제		
		if ( pChkRegNoAutoGive->GetCheck() == 1 )
		{
			ids = Delete049TagFromMarc();
			if ( ids < 0 ) return -1;
		}
		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
	}
	else
	{
		// 에러시 초기화
		OnbInit();
		return ids;
	}
	
	EndWaitCursor();

	return ids;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 파일에 마크를 쓴다.
// [Return Values] 에러면 음
// [Mini Spec] 
// 1. 그리드 선정 및 자료 유무검사
// 2. 파일을 열어(생성) stream marc를 반출한다.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::ExportToTxtFile(CString strFilePath)
{
	EFS_BEGIN

	BeginWaitCursor();
	
	// HSS_040524 헤더세팅
	CString strHD_Species;
	strHD_Species = _T("BS");

	// Deterime What Grid is Selected
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL)
	{
		AfxMessageBox(_T("반출 탭컨트롤 이 없습니다."));
		return -1;
	}
	
	//=======================================================================
	// 1. 그리드 선정 및 자료 유무검사
	//=======================================================================
	INT nSel = pTabCtrl->GetCurSel();
	// 	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDM = NULL;

	// OK GRID
	if (nSel == 0)
	{
// 		pGrid = m_pGrid_OK;
		pDM = m_pDmGridOk;
	}
	// ERROR GRID
	else if (nSel == 1)
	{
// 		pGrid = m_pGrid_ERR;
		pDM = m_pDmGridError;
	}
	
	// SUCCESS GRID
	else if (nSel == 2)
	{
// 		pGrid = m_pGrid_SUCCESS;
		pDM = m_pDmGridSuccess;
	}
	
	
// 	if (pGrid == NULL) return;
	if (pDM == NULL) 
	{
		return 0;
	}
	


//	if (pGrid->GetRows() == 2)
//	{
//		if (pGrid->GetTextMatrix(1, 0).IsEmpty())
//		{
//			AfxMessageBox(_T("반출할 자료가 없습니다."));
//			return;
//		}
//	}
	if (pDM->GetRowCount() < 1)
	{
		AfxMessageBox(_T("반출할 자료가 없습니다."));
		return 0;
	}

	//=======================================================================
	// 2. 파일을 열어(생성) stream marc를 반출한다.
	//=======================================================================
	CStdioFile file;

	//20071115 ADD PJW : FILE OPEN 모드 체크
	INT nConvertMode;
	//nConvertMode = m_MarcConvertApi->m_nConvertModeChk;
	nConvertMode = m_MarcConvertApi.m_nConvertModeChk;
	//20071115 UPDATE PJW :모드별 File Open
	if(nConvertMode == _UTF8_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
		if( 0 == file.GetLength() )
		{			
			//TCHAR cUni = 0xEFBB;//
			CHAR  cMul1;
			CHAR  cMul2;
			CHAR  cMul3;
			BYTE byteMul1 = 0xEF;
			BYTE byteMul2 = 0xBB;
			BYTE byteMul3 = 0xBF;
			
			cMul1 = (CHAR)byteMul1;
			cMul2 = (CHAR)byteMul2;
			cMul3 = (CHAR)byteMul3;
			
			file.Write( &cMul1, sizeof(CHAR));
			file.Write( &cMul2, sizeof(CHAR));
			file.Write( &cMul3, sizeof(CHAR));
		}
	}
	else if(nConvertMode == _UNICODE_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
	}
	else if(nConvertMode == _ANSI_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
		return -1;
	}

	CString strErrMarc;
	CMarc pMarc;
	CString strIndex;
	INT nNeedByte;
// 	INT nCount = pGrid->GetCount();
	INT nCount = pDM->GetRowCount();
	for (INT i = 0; i < nCount; i++)
	{
		// 041013_HSS 그리드에 1부터 뿌려주는 경우 다시 되돌려야 함
		//=========================================================
		//2008.09.11 UPDATE BY PJW : 그리드에 컬럼이 추가되어 인덱스가 밀림
// 		// INT nTmp = _ttoi(pGrid->GetTextMatrix(i + 1, 0) -1)
//		INT nTmp = _ttoi(pGrid->GetTextMatrix(i + 1, 0));
//		strErrMarc = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), nTmp-1 );

		strIndex = pDM->GetCellData(_T("순위"), i);
		strErrMarc = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), _ttoi(strIndex) -1 );
		//=========================================================
		
		if (strErrMarc.IsEmpty()) continue;
		
		m_MarcConvertApi.GetMarcStruct(strErrMarc);
		strErrMarc.Format(m_MarcConvertApi.m_strUniConvertMarc);
		if(nConvertMode == _UTF8_FILE)
		{				
			// 2008.11.4 ADD BY PDJ : UTF8 Marc Convert
			m_pInfo->pMarcMgr->Decoding(strErrMarc, &pMarc);
			m_pInfo->pMarcMgr->m_nEncodingCharset = CP_UTF8;
			m_pInfo->pMarcMgr->Encoding(&pMarc, strErrMarc);			

			nNeedByte = m_MarcConvertApi.GetLengthUnicodeToUTF8(strErrMarc);
			m_MarcConvertApi.GetStrUniCodeToUtf8(strErrMarc);	
			CHAR cTemp;
			for( INT i = 0; i < nNeedByte-1 ; i++ )
			{
				cTemp = *(m_MarcConvertApi.m_cpConvertData+i);
				file.Write(&cTemp, sizeof(CHAR));
			}
			CString strTemp;
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
		}
		else if(nConvertMode == _UNICODE_FILE)
		{
			// 2008.11.4 ADD BY PDJ : UTF8 Marc Convert
			m_pInfo->pMarcMgr->Decoding(strErrMarc, &pMarc);
			m_pInfo->pMarcMgr->m_nEncodingCharset = -1;
			m_pInfo->pMarcMgr->Encoding(&pMarc, strErrMarc);
			strErrMarc += _T("\r\n");
			file.WriteString(strErrMarc);			
		}
		else if(nConvertMode == _ANSI_FILE)
		{
			// 2008.11.4 ADD BY PDJ : ANSI Marc Convert			
			m_pInfo->pMarcMgr->Decoding(strErrMarc, &pMarc);
			m_pInfo->pMarcMgr->m_nEncodingCharset = CP_ACP;
			m_pInfo->pMarcMgr->Encoding(&pMarc, strErrMarc);

			nNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(strErrMarc);
			m_MarcConvertApi.GetStrUniCodeToAnsi(strErrMarc);
			CHAR cTemp;
			for( INT i = 0; i < nNeedByte-1 ; i++ )
			{
				cTemp = *(m_MarcConvertApi.m_cpConvertData+i);
				file.Write(&cTemp, sizeof(CHAR));
			}
			CString strTemp;
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
		}
	}

	file.Close();

	m_pInfo->pMarcMgr->m_nEncodingCharset = CP_UTF8;

	EndWaitCursor();

	AfxMessageBox(_T("반출 완료"));

	return 0;

	return -1;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 코드적용
// [Return Values] 
// [Mini Spec] 
// 한개씩 로우별로 코드 적용
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::CodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
		CodeApply(row);
	
	EndWaitCursor();


	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 코드적용
// [Return Values] 
// [Mini Spec] 
// 형식구분 매체구분에 알맞게 코드를 적용한다.
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::CodeApply(INT nIdxDM)
{
	EFS_BEGIN

	CStringArray arrIgnorAlias;
	CString strHD_Species, strHD_Index;
	CMarc marc;
	CString strStreamMarc;
	CString strFormCode;
	CString strMatCode;
	
	// HSS_040524 헤더세팅
	strHD_Species = _T("BS");
	strHD_Index = _T("IBS");

	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
	if (pCM == NULL) return;

	// TODO : 마크 구분별~ 코드 적용
	INT ids = -1;
	
	arrIgnorAlias.Add( strHD_Index + _T("_형식구분_코드"));
	arrIgnorAlias.Add( strHD_Index + _T("_매체구분_코드"));	

	// 형태 구분, 매체구분을 알맞게 넣어준다.
	ids = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), nIdxDM, strStreamMarc);
	if (ids >= 0 && !strStreamMarc.IsEmpty())
	{
		// 형태 구분, 매체구분을 알맞게 넣어준다.
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids >= 0)
		{
			ids = GetFormCodeMatCode(&marc, strFormCode, strMatCode);
			// 2007.06.13 ADD BY PDJ
			if ( strFormCode == _T("") ) strFormCode = _T("BK");
			if ( strMatCode == _T("") ) strMatCode = _T("PR");
			
			m_pDM_Index->SetCellData( strHD_Index + _T("_형식구분_코드"), strFormCode, nIdxDM);
			m_pDM_Index->SetCellData( strHD_Index + _T("_매체구분_코드"), strMatCode, nIdxDM);			
		}
	}
	
	m_pCatApi->MakeDMFromCM(pCM, m_pDM_Species, MNG_INSERT, nIdxDM, strHD_Species, &arrIgnorAlias);
	m_pCatApi->MakeDMFromCM(pCM, m_pDM_Index, MNG_INSERT, nIdxDM, strHD_Index, &arrIgnorAlias);


	//job.2018.0053
	//목록완성에 마크반입시 업무구분이 적용 안되는 현상수정 
	//----------------------------------------------------------------------------------
	CString strWorkCode = "";
	GetControlData(m_lpszCMAlias_Code,_T("BB_업무구분"),strWorkCode,-1,0);
	m_pDM_Index->SetCellData( strHD_Index + _T("_업무구분_코드"), strWorkCode, nIdxDM);	
	//----------------------------------------------------------------------------------
	EFS_END
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 마크로부터 형식구분, 매체구분 구함
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 리더로부터 형식구분을 구한다.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::GetFormCodeMatCode(CMarc *pMarc, CString &strFormCode, CString &strMatCode)
{
	EFS_BEGIN

	if (!pMarc) return -1;
	strFormCode.Empty();
	strMatCode.Empty();

	CString strLeader;
	CString strFormCodeMatCode;
	CString strHD_Index;
	strHD_Index = _T("IBS");

	pMarc->GetLeader(strLeader);
	if (strLeader.GetLength() < 7)
	{
		AfxMessageBox(_T("리더의 길이가 7보다 작습니다."));
		return -1;
	}
	
	//=======================================================================
	// 1. 리더로부터 형식구분을 구한다.
	//=======================================================================
	CString strRecord = strLeader.Mid(6,1) + strLeader.Mid(7,1);
	// 2007.06.12 ADD BY PDJ
	// 엑셀파일 반입시 또는 레코드 형태가 NULL일 경우 Default 'a'
	if ( strRecord == _T("  ") ) strRecord = _T("am");

	BOOL bFind = m_mapRecord_FormCodeMatCode.Lookup(strRecord, strFormCodeMatCode);
	if (bFind < 0)
	{
		AfxMessageBox(_T(""));
		return -1;
	}

	// 형식구분, 매체구분 가져오기
	INT nFind = strFormCodeMatCode.Find(_T("="));
	if (nFind < 0) return -1;

	CString strGetFormCode = strFormCodeMatCode.Mid(0, nFind);
	CString strGetMatCode = strFormCodeMatCode.Mid(nFind + 1);

	strFormCode = strGetFormCode;
	
	// 결정할 수 없는 타입일 경우
	nFind = strGetMatCode.Find(_T("|"));
	if (nFind > 0)
	{
		strGetMatCode = strGetMatCode.Mid(0, nFind);

		// 화면의 설정된 값을 가져온다.
		CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
		if (pCM == NULL) return -1;

		CString strTemp;
		pCM->GetControlMgrData( strHD_Index + _T("_형식구분_코드"), strTemp);
		if (strTemp == strGetFormCode)
		{
			pCM->GetControlMgrData( strHD_Index + _T("_매체구분_코드"), strTemp);
			strMatCode = strTemp;
			return 0;
		}
	}
	
	// 설정된 값도 아니고 결정할수 있는 타입인 경우
	strMatCode = strGetMatCode;
	
	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 마크 콜백 함수 (유효성검사)
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 필수 태그 선택
// 2. 리더검사
// 3. 소장반입이면 별치/가격(950) 유효성을 검사한다. 
// 4. 653 Tag 유무검사 : 없으면 생성
// 5. 필수태그 검사
// 6. 008 유무검사 : 없으면 생성
// 7. 005 최종수정일 갱신
/////////////////////////////////////////////////////////////////////////////
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
	
	CStringArray arrStrEssentialTag;

	CButton * pChkWorkStatus;	
	CButton * pChkRegNoAutoGive;

	strCheckErrMsg.Empty();

	//=======================================================================
	// 1. 필수 태그 선택
	//=======================================================================
	arrStrEssentialTag.Add(_T("245"));

	//자동부여 부여 버튼
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	if ( pChkRegNoAutoGive->GetCheck() == 0 ) {
		arrStrEssentialTag.Add(_T("049"));
	}

	//정리중 반입 버튼
	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);
	if ( pChkWorkStatus->GetCheck() == 0 ) {
		arrStrEssentialTag.Add(_T("008"));

		//arrStrEssentialTag.Add(_T("090"));

		//2004.12.13 반입반을 땐 분류기호 반드시 체크 하지 않아도 됨
		//분류기호는 
		//arrStrEssentialTag.Add(_T("056"));
		arrStrEssentialTag.Add(_T("260"));
		arrStrEssentialTag.Add(_T("300"));
	}

	//=======================================================================
	// 2. 리더검사
	//=======================================================================
	CString strLeader;
	pMarc->GetLeader(strLeader);
	TCHAR ch07 = strLeader.GetAt(7);
	if (ch07 == ' ') 
	{
		strLeader.SetAt(7, 'm');
	}
	else 
	{
		if (!(ch07 == 'a' || ch07 == 'c' || ch07 == 'm'))
		{
			strErrMsg = _T("단행 서지수준이 아닙니다.. 수정해주세요");
			arrStrErrMsg.Add(strErrMsg);
		}
	}

	//=======================================================================
	// 3. 소장반입이면 별치/가격(950) 유효성을 검사한다. 
	//=======================================================================
	if ( pChkWorkStatus->GetCheck() == 0 ) {
		// 별치기호 내용검사
		if ( pChkRegNoAutoGive->GetCheck() == 0 )
		{
			ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$f"), strItemData, &arrItem);
			if (ids >= 0)
			{
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
				if (ids < 0) 
				{
					strErrMsg = _T("별치기호 값이 잘못됬습니다. 수정해주세요");
					arrStrErrMsg.Add(strErrMsg);
				}
			}
		}

		// 950$b 에 화폐구분 있는지 가격 검사 & 화폐구분 내용검사
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strItemData);
		if (ids >= 0)
		{
			// 화폐구분이 있는지 검사
			strItemData = GetPriceCode(strItemData, strTemp);
			if (strItemData.IsEmpty()) 
			{
				strErrMsg = _T("[950$b] 에 화폐구분이 없습니다. 수정해주세요");
				arrStrErrMsg.Add(strErrMsg);
			}

			// 화폐구분이 유효한지 검사
			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("화폐부호관리"), strItemData, strItemData);
			if (ids < 0)
			{
				strErrMsg = _T("[950$b] 의 화폐구분 값이 잘못됬습니다. 수정해주세요");
				arrStrErrMsg.Add(strErrMsg);
			}
		}

		/*-----------------------------------------------------------------------
		// 청구기호 유무 검사
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("090$a"), strItemData);
		if (ids < 0)
		{
			strErrMsg = _T("청구기호가 없습니다. 수정해주세요");
			arrStrErrMsg.Add(strErrMsg);
		}
		*/
	}

	//=======================================================================
	// 4. 653 Tag 유무검사 : 없으면 생성
	//=======================================================================
	CButton * pChk653Tag;
	pChk653Tag = (CButton *)GetDlgItem(IDC_Create653Tag);
	if ( pChk653Tag->GetCheck() == 1 ) {	
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("653$a"), strItemData);	
		if (ids < 0)
		{
			// TODO : CREATE KEY WORD			
			CString strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
			m_pDlgKeyWord->Init(strStreamMarc);
			m_pDlgKeyWord->MakeKeyWord();
			*pMarc = m_pDlgKeyWord->m_marc;		
			
			//arrStrErrMsg.Add(_T("[653] Tag가 없습니다."));	
		}
	}

	//=======================================================================
	// 5. 필수태그 검사
	//=======================================================================
	nCount = arrStrEssentialTag.GetSize();
	for (idx = 0; idx < nCount; idx++)
	{
		strItem = arrStrEssentialTag.GetAt(idx);
		if (pMarc->FindTag(strItem) == NULL)
		{
			strErrMsg.Format(_T("[%s] Tag을 입력해주세요"), strItem);
			arrStrErrMsg.Add(strErrMsg);
		}
	}

	//=======================================================================
	// 6. 008 유무검사 : 없으면 생성
	//=======================================================================
	ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@0-5"), strItemData);
	CIndex::TrimAll(strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		strItemData = CTime::GetCurrentTime().Format(_T("%y%m%d"));
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@0-5"), strItemData);
	}

	//=======================================================================
	// 7. 005 최종수정일 갱신
	//=======================================================================
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

	if (nCount > 0)
		return -1;
	else
		return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcImportDlg message handlers

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	100,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

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

		pTabCtrl->SetCurFocus(1);
		pTabCtrl->SetCurFocus(0);
	}

	//등록구분 비활성화
	GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);

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

	( (CButton*) GetDlgItem (IDC_rTXT) )->SetCheck(TRUE);
	m_nFileType = 1;

	DefaultAccessionRecNo();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	return FALSE;


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

		CDialog::OnOK();
	}
	else if (strText == _T("중지"))
	{
		m_bIsThreadRunning = FALSE;
	}

	AllControlDisplay(_T("CM_단행_정리"));

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
	
	// Resize Grapper
	/*if (::IsWindow(m_wndSizeGrapper.GetSafeHwnd()))
	{
		CRect rcClient;
		m_wndSizeGrapper.GetWindowRect(rcClient);
		ScreenToClient(rcClient);

		rcClient.left = rcDlg.right - rcClient.Width();
		rcClient.right = rcDlg.right;
		rcClient.top = rcDlg.bottom - rcClient.Height();
		rcClient.bottom = rcDlg.bottom;

		m_wndSizeGrapper.MoveWindow(rcClient);
	}
	*/


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

	delete m_pDM_Index;
	m_pDM_Index = NULL;

	delete m_pDM_Species;
	m_pDM_Species = NULL;

	delete m_pDM_Book;
	m_pDM_Book = NULL;

	delete m_pDlgKeyWord;
	m_pDlgKeyWord = NULL;

	// 16/05/30 김혜영 : 목록완성>마크반입 실행한 이후에 메모리릭이 발생하여 수정
//*/ ADD ------------------------------------------------------------------------
	delete m_pCatApi;
	m_pCatApi = NULL;
//*/ END ------------------------------------------------------------------------

	CDialog::OnDestroy();	


	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 파일열기 버튼 이벤트
// [Return Values] 
// [Mini Spec] 
// 1. 파일타입 별로 파일을 열어  파일을 패치한다. 
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::OnbFILEOPEN() 
{
	EFS_BEGIN

	UpdateData();

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

	if (m_nFileType == 0)
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
	else if(m_nFileType == 1)
	{
		OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("TEXT 파일 열기");
	}
	else if( m_nFileType == 5) 
	{
		OFN.lpstrFilter = _T("TXT파일 (탭으로 분리된 텍스트) (*.txt)\0*.txt\0CSV파일 (탭으로 분리된 텍스트) (*.csv)\0*.csv\0모든파일 (탭으로 분리된 텍스트)(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("CSV 파일 열기");
	}

	BOOL bSuccess = GetOpenFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
		UpdateData(FALSE);

		m_wndProgressBar.SetWindowText(_T("파일 패치중..."));
		m_wndProgressBar.UpdateWindow();

		INT ids;
		// 파일 반입 시작
		ids = ImportMarcFromFile();
		if ( ids ) return;
	}
	else
	{
		return;
	}
	
	if ( _T("") != m_strFilePath )
	{
		//반입후 버튼(정리중반입, 파일열기, 등록번호 자동부여, 형식구분)을 Disable
		GetDlgItem(IDC_bFILE_OPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkGiveAutoRegno)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(FALSE);

		GetDlgItem(IDC_bMODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_bEXPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_bIMPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_bEXPORT_EDITMARC)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_ChkAccessionRec)->EnableWindow(FALSE);
		GetDlgItem(IDC_eAccessionRecNo)->EnableWindow(FALSE);
	}	

	CheckAccessionRecNo();
	
EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 마크 수정버튼 이벤트
// [Return Values] 
// [Mini Spec] 
// 1. 코드를 적용한다. 
// 2. 상태별 마크편집창을 띄운다.
/////////////////////////////////////////////////////////////////////////////
//===================================================
//2009.03.11 UPDATE BY PJW : 정리화면이 아닌 마크 수정창을 띄운다.
//VOID CMarcImportDlg::OnbMODIFY() 
//{
//	EFS_BEGIN
//
//	// 마크정리 화면을 띄운다.
//	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
//	if (pTabCtrl == NULL) return;
//	
//	INT nCurSel = pTabCtrl->GetCurSel();
//
//	CESL_Grid *pGrid = NULL;
//	
//	if (nCurSel == 0)
//	{
//		pGrid = m_pGrid_OK;
//	}
//	else if (nCurSel == 1)
//	{
//		pGrid = m_pGrid_ERR;
//	}
//
//	if (pGrid == NULL) return;
//
//	CString strData = pGrid->GetTextMatrix(pGrid->GetRow(), 0) ;
//	if (strData.IsEmpty()) return;
//
//	UpdateData();
//	
//	//===========================================================================
//	// 1. 코드를 적용한다. 
//	//===========================================================================
//	CodeApply();
//
//	m_MarcAdjLaunCher.SetParentInfo(m_pDM_Species, m_pDM_Index, pGrid, pGrid->GetRow() - 1);
//
//	//===================================================
//	//20080121 ADD PJW : 마크정리 화면을 띄울때 부모다이얼로그의 형태구분 콤보박스에 dm의 형태구분을 맞춘다.
//	CString strFormCode;
//	INT nDmRowCnt = 0;
//	GetControlData( _T("CM_정리_마크반입_구분코드"), _T("IBS_형태구분_코드"), strFormCode );
//	nDmRowCnt = m_pDM_Species->GetRowCount();
//	for ( INT i = 0 ; i < nDmRowCnt ; i++)
//	{
//		m_pDM_Species->SetCellData( _T("BS_형태구분_코드"), strFormCode, i);
//	}
//	nDmRowCnt = m_pDM_Index->GetRowCount();
//	for (  i = 0 ; i < nDmRowCnt ; i++)
//	{
//		m_pDM_Index->SetCellData( _T("IBS_형태구분_코드"), strFormCode, i);
//	}
//	//===================================================
//	
//
//	//===========================================================================
//	// 2. 상태별 마크편집창을 띄운다.
//	//===========================================================================
//	CButton * pChkWorkStatus;
//	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);
//	
//	if ( pChkWorkStatus->GetCheck() == 0 ) //소장반입
//	{
//		m_MarcAdjLaunCher.LaunchMarcAdjust(this, this, _T(""), MNG_IMPORT, MODE_MONO);	
//	}
//	else //정리중 반입
//	{	
//		m_MarcAdjLaunCher.LaunchMarcAdjust(this, this, _T(""), MNG_IMPORT_WORKING, MODE_MONO);		
//	}
//
//	//===================================================
//	//2009.03.11 UPDATE BY PJW : 그리드가 변경되면 DM에 추가하고 그리드를 정렬한다.
////	INT nSuccessDmRowCnt;
////	INT nSuccessGridRowCnt;
////	INT nSuccessDmColCnt;
////	INT nChkDm;
////
////	CString strGridData;
////	CString strDmData;
////
////	nSuccessGridRowCnt = m_pGrid_SUCCESS->GetRows();
////	nSuccessDmRowCnt = m_pDmGridSuccess->GetRowCount();
////
////	nSuccessDmColCnt = m_pDmGridSuccess->GetRefColumnCount();
////	
////	nChkDm = nSuccessGridRowCnt - nSuccessDmRowCnt;
////	if( 0 < nChkDm )
////	{
////		for( INT i = 0 ; i < nChkDm ; i++ )
////		{
////			strGridData = m_pGrid_SUCCESS->GetTextMatrix( nSuccessDmRowCnt + i + 1, 0 );
////			
////			for( INT j = 0 ; j < m_pDmGridError->GetRowCount() ; j++ ) 
////			{
////				strDmData = m_pDmGridError->GetCellData(_T("순위"), j);
////				if( strGridData == strDmData )
////				{
////					m_pDmGridSuccess->InsertRow(-1);
////					for( INT n = 0 ; n < nSuccessDmColCnt ; n++ )
////					{
////						strDmData = m_pDmGridError->GetCellData( j, n );
////						m_pDmGridSuccess->SetCellData(  nSuccessDmRowCnt + i , n,  strDmData);
////
////					}
////					m_pDmGridError->DeleteRow(j);
////					
////					break;
////				}
////			}
////		}
////
////	}
//	//===================================================
//
//	return;
//	EFS_END
//}
VOID CMarcImportDlg::OnbMODIFY() 
{
	EFS_BEGIN

	// 마크정리 화면을 띄운다.
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();

	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDm = NULL;
	
	if (nCurSel == 0)
	{
		pGrid = m_pGrid_OK;
		pDm   = m_pDmGridOk;
	}
	else if (nCurSel == 1)
	{
		pGrid = m_pGrid_ERR;
		pDm   = m_pDmGridError;
	}

	if (pGrid == NULL || pDm == NULL)
	{
		return;
	}

	pGrid->SelectMakeList();
	INT nSelectCnt;
	nSelectCnt = pGrid->SelectGetCount();
	if( 1 > nSelectCnt )
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

//	CString strData = pGrid->GetTextMatrix(pGrid->GetRow(), 0) ;
//	if (strData.IsEmpty()) return;

	UpdateData();

	// 마크 편집 화면을 보여준다.
	CMarcEditDlg dlg(this);
	dlg.SetMarcImpExpMgr(&m_marcImpExpMgr);
	INT nFileType = 0;
	INT nChkMultiBook;
	INT nChkGiveAutoRegno;
	INT nChkWorkStatus;

	CButton * pChkGiveAutoRegno = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	nChkGiveAutoRegno = pChkGiveAutoRegno->GetCheck();

	CButton * pChkMultiBook = (CButton *)GetDlgItem(IDC_ChkMULTIBOOK);
	nChkMultiBook = pChkMultiBook->GetCheck();

	CButton * pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkMULTIBOOK);
	nChkWorkStatus = pChkWorkStatus->GetCheck();

	 

	dlg.SetParentInfo(m_pDM_Species, pDm, pGrid, _T("BS_MARC"), _T("BS_파일반입오류")
		              ,0, nChkMultiBook, nChkGiveAutoRegno, nChkWorkStatus);

	pGrid->SetProtectSelectColumn(TRUE);
	dlg.DoModal();
	pGrid->Display();
	INT nIdx;
	CString strData;
	nIdx = pGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		strData = pDm->GetCellData(_T("순위"), nIdx);
		pGrid->SetTextMatrix(nIdx+1, 0,strData);
		nIdx = pGrid->SelectGetNext();		
	}
	pGrid->SetProtectSelectColumn(FALSE);

	return;
	EFS_END
}
//===================================================



/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 정상자료 반입 버튼 이벤트 
// [Return Values] 
// [Mini Spec] 
// 1. 코드를 적용한다. 
// 2. 반입 쓰레드를 돌린다.
/////////////////////////////////////////////////////////////////////////////
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

	//===================================================
	//2009.03.17 REM BY PJW : DM을 이용함으로 필요없어진 코드
//	if (m_pGrid_OK->GetTextMatrix(m_pGrid_OK->SelectGetHeadPosition() + 1, 0).IsEmpty())
//	{   
//		AfxMessageBox(_T("반입할 자료가 없습니다."));
//		return;
//	}
	//===================================================
	
	UpdateData();
	//=======================================================================
	// 1. 코드를 적용한다. 
	//=======================================================================
	CodeApply();

	//=======================================================================
	// 2. 원부 적용
	//=======================================================================
	//=====================================================
	//2009.01.05 UPDATE BY PJW : 체크되어있을경우만 원부를 적용한다.
	//SetAccessionRecNoApply();
	CButton* pBtn = (CButton*) GetDlgItem(IDC_ChkAccessionRec);
	if( pBtn->GetCheck() )
	{
		SetAccessionRecNoApply();
	}
	//=====================================================

	//=======================================================================
	// 3. 반입 쓰레드를 돌린다.
	//=======================================================================
	m_pThreadImport = AfxBeginThread(ThreadFunc_Import, this);

	EFS_END
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 자료반출 이벤트
// [Return Values] 
// [Mini Spec] 
// Stream Marc 를 TXT파일로 반출한다.
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::OnbEXPORT() 
{
	EFS_BEGIN

	INT nExportFileType;
	BOOL bCheck;
	INT ids;
	//20071208 ADD PJW : 저장할 파일 모드 선택
	// =============================================================
	// 1. 마크 반출 타입 선택
	// =============================================================
	// 1.1 ansi 선택
	bCheck = ( (CButton*) GetDlgItem (IDC_rANSI) )->GetCheck();
	if ( bCheck ) nExportFileType = _ANSI_FILE;

	// 1.2 utf-8 선택
	bCheck = ( (CButton*) GetDlgItem (IDC_rUTF8) )->GetCheck();
	if ( bCheck ) nExportFileType = _UTF8_FILE;

	// 1.3 unicode 선택
	bCheck = ( (CButton*) GetDlgItem (IDC_rUNICODE) )->GetCheck();
	if ( bCheck ) nExportFileType = _UNICODE_FILE;
	
	// 1.3 MarcConvert위한 모드 세팅 
	ids = m_MarcConvertApi.SetConvertMode(nExportFileType);

	// Deterime What Grid is Selected
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;

	INT nSel = pTabCtrl->GetCurSel();
	// 	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDM = NULL;

	// OK GRID
	if (nSel == 0)
	{
// 		pGrid = m_pGrid_OK;
		pDM = m_pDmGridOk;
	}
	// ERROR GRID
	else if (nSel == 1)
	{
// 		pGrid = m_pGrid_ERR;
		pDM = m_pDmGridError;
	}
	
	// SUCCESS GRID
	else if (nSel == 2)
	{
// 		pGrid = m_pGrid_SUCCESS;
		pDM = m_pDmGridSuccess;
	}
	
	
// 	if (pGrid == NULL) return;
	if (pDM == NULL) 
	{
		return;
	}
	


//	if (pGrid->GetRows() == 2)
//	{
//		if (pGrid->GetTextMatrix(1, 0).IsEmpty())
//		{
//			AfxMessageBox(_T("반출할 자료가 없습니다."));
//			return;
//		}
//	}
	if (pDM->GetRowCount() < 1)
	{
		AfxMessageBox(_T("반출할 자료가 없습니다."));
			return;
	}

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

	ids = GetSaveFileName(&OFN);
	SetCurrentDirectory(szBuffer);

	if (ids != 0)
	{
		strFilePath= OFN.lpstrFile;
		ExportToTxtFile(strFilePath);
	}

	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 마크에디트반출 이벤트
// [Return Values] 
// [Mini Spec] 
// 마크를 에디트마크 형식으로 반출한다. 
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::OnbEXPORTEDITMARC() 
{
	EFS_BEGIN

	// HSS_040524 헤더세팅
	CString strHD_Species;
	strHD_Species = _T("BS");

	// Marc Editor에 넣었다가 뺀다.
	CMarcEditCtrl MarcEditCtrl;
	MarcEditCtrl.Create(_T(""), _T(""), WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), this, 10202);

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

	// =============================================================
	// 1. 마크 반출 타입 선택
	// =============================================================
	// 1.1 ansi 선택
	BOOL bCheck;
	INT	nExportFileType;
	bCheck = ( (CButton*) GetDlgItem (IDC_rANSI) )->GetCheck();
	if ( bCheck ) nExportFileType = _ANSI_FILE;

	// 1.2 utf-8 선택
	bCheck = ( (CButton*) GetDlgItem (IDC_rUTF8) )->GetCheck();
	if ( bCheck ) nExportFileType = _UTF8_FILE;

	// 1.3 unicode 선택
	bCheck = ( (CButton*) GetDlgItem (IDC_rUNICODE) )->GetCheck();
	if ( bCheck ) nExportFileType = _UNICODE_FILE;
	
	// 1.3 MarcConvert위한 모드 세팅 
	ids = m_MarcConvertApi.SetConvertMode(nExportFileType);

	//20071115 ADD PJW : FILE OPEN 모드 체크
	INT nConvertMode;
	//nConvertMode = m_MarcConvertApi->m_nConvertModeChk;
	nConvertMode = m_MarcConvertApi.m_nConvertModeChk;
	//20071115 UPDATE PJW :모드별 File Open	
	if(nConvertMode == _ANSI_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
		{
			AfxMessageBox(_T("파일 생성 실패"));
			return ;
		}
	}
	else if(nConvertMode == _UTF8_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		{
			AfxMessageBox(_T("파일 생성 실패"));
			return ;
		}
		if( 0 == file.GetLength() )
		{			
			//TCHAR cUni = 0xEFBB;//
			CHAR  cMul1;
			CHAR  cMul2;
			CHAR  cMul3;
			BYTE byteMul1 = 0xEF;
			BYTE byteMul2 = 0xBB;
			BYTE byteMul3 = 0xBF;
			
			cMul1 = (CHAR)byteMul1;
			cMul2 = (CHAR)byteMul2;
			cMul3 = (CHAR)byteMul3;
			
			file.Write( &cMul1, sizeof(CHAR));
			file.Write( &cMul2, sizeof(CHAR));
			file.Write( &cMul3, sizeof(CHAR));
		}
	}
	else //if(nConvertMode == _UNICODE_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		{
			AfxMessageBox(_T("파일 생성 실패"));
			return ;
		}
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
	}
	
		
	CMarc marc;
	CString strStreamMarc;
	CString strEditMarc;
	CString strTmp;
	INT nCount = m_pDM_Species->GetRowCount();
	INT nLen;
	CHAR cTemp;
	for (INT i = 0; i < nCount; i++)
	{
		strStreamMarc = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), i);
		if (strStreamMarc.IsEmpty()) continue;

		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) continue;

		// MARC을 가져와서 Edit Ctrl에 넣은다음 Text를 가져온다.
		MarcEditCtrl.Init(m_pInfo->pMarcMgr, &marc);
		MarcEditCtrl.Display();
		
		MarcEditCtrl.GetWindowText(strEditMarc);

		if(nConvertMode == _UTF8_FILE)
		{
			CHAR	*szpEditMarc;			
			m_cvt.UnicodeToUTF8(strEditMarc.GetBuffer(0), &szpEditMarc, &nLen);			
			file.Write(szpEditMarc, nLen);
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
			free(szpEditMarc);
		}
		else if(nConvertMode == _ANSI_FILE)
		{
			CHAR	*szpEditMarc;			
			m_cvt.UnicodeToMultiByte(strEditMarc.GetBuffer(0), &szpEditMarc, &nLen);			
			file.Write(szpEditMarc, nLen);
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
			free(szpEditMarc);
		}
		else // if(nConvertMode == _UNICODE_FILE)
		{
			strEditMarc += _T("\r\n");
			file.WriteString(strEditMarc);		
		}		
	}

	file.Close();

	EndWaitCursor();

	AfxMessageBox(_T("반출 완료"));

	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 마크정리가 끝났을 때 이벤트 메시지
// [Return Values] 에러면 음수
// [Mini Spec] 
// 성공한 자료들은 성공탭으로 자료를 넘겨준다. 
/////////////////////////////////////////////////////////////////////////////
UINT CMarcImportDlg::OnEndMarcAdjust(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	// HSS_040524 헤더세팅
	CString strHD_Species;
	strHD_Species = _T("BS");

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return -1;
	
	CESL_Grid *pGrid = NULL;

	INT nID_FROM = -1;
	CString strFROM;
	CString strData;

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
	
	INT nRowCount = pGrid->GetCount();
	INT nColCount = pGrid->GetCols(0);

	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		// 소정 수정
		if (m_pDM_Species->GetCellData( strHD_Species + _T("_파일반입성공"), _ttoi(pGrid->GetTextMatrix(idx + 1, 0))-1) == _T("Y"))
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
	ON_EVENT(CMarcImportDlg, IDC_MSHFLEXGRID_ERR, -605 /* MouseDown */, OnMouseDownMshflexgridErr, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMarcImportDlg, IDC_MSHFLEXGRID_OK, -605 /* MouseDown */, OnMouseDownMshflexgridOk, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMarcImportDlg::OnMouseDownMshflexgridOk(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	INT i = m_pGrid_OK->GetMouseRow();
	if (i == 0) return;
	i = m_pGrid_OK->GetFixedRows();
	i = m_pGrid_OK->GetRow();
	//m_pGrid_OK->SetReverse(m_pGrid_OK->GetRow() - 1);


	EFS_END
}

VOID CMarcImportDlg::OnMouseDownMshflexgridErr(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	//m_pGrid_ERR->SetReverse(m_pGrid_ERR->GetRow() - 1);


	EFS_END
}

VOID CMarcImportDlg::OnChkGiveAutoRegno() 
{
	// TODO: Add your control notification handler code here
	CButton * pChkGiveAutoRegno;
	pChkGiveAutoRegno = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	if ( pChkGiveAutoRegno->GetCheck() == 1 ) {
		GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(FALSE);
	}
}

VOID CMarcImportDlg::OnSelchangecmbMEDIACLASS() 
{
	//매체구분 변경했을경우

	CString strHD_Index;
	strHD_Index = _T("IBS");
	

	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
	
	CString sMatCode;
	pCM->GetControlMgrData( strHD_Index + _T("_매체구분_코드") , sMatCode );

	CButton * pChkWorkStatus;	
	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);

	if ( sMatCode == _T("OF") )
	{
		if ( pChkWorkStatus->GetCheck() == 1 )
		{
			AfxMessageBox(_T("매체구분이 OnlineBook(OF)일 경우에는 정리중 반입을 할 수 없습니다.\n소장으로 반입됩니다."));

			CButton * pChkWorkStatus;	
			pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);
			pChkWorkStatus->SetCheck(0);
		}
		GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(TRUE);
	}

}

BOOL CMarcImportDlg::SetConfigAutoRegNo( CESL_DataMgr *DM_Book )
{
EFS_BEGIN
	//자동번호 부여시 설정할 항목 설정
	INT idx = 1;
	if ( idx < 0 ) return FALSE;
	
	// 종dm 가져오기
	// KOL.UDF.022
	CMarcImportDlg *pDlg = (CMarcImportDlg*)this;
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return FALSE;
	}
	CESL_DataMgr *pDM_Species = NULL;
	pDM_Species = pDlg->m_pDM_Species;
	if (pDM_Species == NULL)
	{
		return FALSE;
	}	

	CESL_DataMgr *pDM_Book = NULL;
	pDM_Book = pDlg->m_pDM_Book;
	if (pDM_Book == NULL)
	{
		return FALSE;
	}	

	// HSS_040524 헤더세팅
	CString strHD_Species, strHD_Book;
	strHD_Species = _T("BS");
	strHD_Book = _T("BB");


	INT ids = -1;
	// 등록구분 가져오기
	CString sRegCode;
	GetControlData( _T("CM_정리_마크반입_구분코드"), strHD_Book + _T("_등록구분"), sRegCode );

	// 마지막 번호 가져오기
	CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
	CString sLastNum;	
	pDlg->m_pDM_Species->GetOneValueQuery( sQuery , sLastNum );
	INT iLastNum = _ttoi ( sLastNum );


	//등록구분이 없으면 새로추가
	if ( sLastNum == _T("") ) {
		iLastNum = 0;

		ids = pDM_Species->StartFrame();
		if ( 0 > ids ) return FALSE;
		CString sRecKey;
		ids = pDM_Species->MakeRecKey( sRecKey );
		if ( 0 > ids ) return FALSE;
		CString sQuery_reg;
		CTime time = CTime::GetCurrentTime();
		CString sYear;
		sYear.Format( _T("%04d"), time.GetYear() );
		sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
						  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
//DEL 		sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER) ")
//DEL 						  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s' );") 
						  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
		ids = pDM_Species->AddFrame( sQuery_reg );
		if ( 0 > ids ) return FALSE;

		ids = pDM_Species->SendFrame();			
		if (ids < 0) {
			AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
			return FALSE;
		}
		ids = pDM_Species->EndFrame();
		if ( 0 > ids ) return FALSE;
	}



	CESL_DataMgr *pDmLastNum = FindDM( _T("DM_마지막번호") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_마지막번호 확인") );
		return FALSE;
	}
	pDmLastNum->FreeData();
	pDmLastNum->InsertRow(-1);
	pDmLastNum->SetCellData( strHD_Book + _T("_등록구분_코드"), sRegCode, 0 );


	//수정

	CMarc marc;
	CString sMarc = pDlg->m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), idx );
	m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
	CArray<CString, CString&> pArr049l;
	CString strItemData;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);


	//선정된 자료의 책수만 하도록 수정
	//홍수

	INT iBookCount = pDM_Book->GetRowCount();

	for ( INT j = 0 ; j < iBookCount ; j++ ) {
		iLastNum++;
		CString sRegNo;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		sRegNo.Format( _T("%s%010d"), sRegCode, iLastNum );
		CString strRegNumTemp;
		strRegNumTemp.Format(_T("%d"),iLastNum);
		sRegNo = pDlg->m_pInfo->MakeRegNo(  sRegCode, strRegNumTemp );
		//=====================================================
		

		sLastNum.Format( _T("%d"), iLastNum );
		pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, 0 );
		
		CString sItem;
		if ( pArr049l.GetSize() == iBookCount ) {
			sItem = pArr049l.GetAt(j);
			m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
		}
		else {
			m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
		}
		m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
		pDlg->m_pDM_Species->SetCellData( strHD_Species + _T("_MARC"), sMarc, idx );
	}

	return TRUE;


EFS_END
return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 적용될 DM을 구성해준다. 
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 색인/종 DM을 복사한다.
// 2. 마크로 부터 책정보를 구성한다. 
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::MakeMarcAdjustDM(	CESL_DataMgr *pDM_From_Species, 
										INT nFromSpeciesRow, 
										CESL_DataMgr *pDM_From_Index, 
										INT nFromIndexRow, 
										CESL_DataMgr *pDM_To_Species, 
										CESL_DataMgr *pDM_To_Index, 
										CESL_DataMgr *pDM_To_Book, 
										CESL_DataMgr *pDM_To_Volume)
{
	EFS_BEGIN

	if (!pDM_From_Species ||
		!pDM_From_Index ||
		!pDM_To_Index ||
		!pDM_To_Species ||
		!pDM_To_Book ||
		!pDM_To_Volume ||
		nFromSpeciesRow < 0 ||
		nFromIndexRow < 0) return -1;

	// 데이터 초기화
	pDM_To_Species->FreeData();
	pDM_To_Index->FreeData();
	pDM_To_Book->FreeData();
	pDM_To_Volume->FreeData();

	pDM_To_Species->InsertRow(-1);
	pDM_To_Index->InsertRow(-1);
	pDM_To_Book->InsertRow(-1);
	pDM_To_Volume->InsertRow(-1);

	INT ids = -1;
	
	//=======================================================================
	// 1. 색인/종 DM을 복사한다.
	//=======================================================================
	ids = CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("종DM구성에 실패"));
		return -1;
	}
	
	ids = CopyDM(pDM_From_Index, nFromIndexRow, pDM_To_Index, 0);
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("색인DM구성에 실패"));
		return -1;
	}

	// 마크싱크를 맞추기 전에 049에 해당하는 정보를 책 DM에 넣는다...
	CMarc marc;
	CString strStreamMarc = pDM_To_Species->GetCellData( _T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	CString strRegNo;
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	INT nRowCount = arrItem.GetSize();


	//=======================================================================
	// 2. 마크로 부터 책정보를 구성한다. 
	//=======================================================================
	// 등록번호 자동부여 체크버튼 , 다권반입 체크버튼
	CButton * pChkRegNoAutoGive;
	CButton * pChkMultiBook;
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	pChkMultiBook = (CButton *)GetDlgItem(IDC_ChkMULTIBOOK);

	// 049 Tag이 있으면 다시 만든다.
	if (nRowCount >= 0) 
	{	
		pDM_To_Book->FreeData();
		
		INT nBookCnt = 1;
		// 049 Tag없어도 자동부여이면 1책을 만든다.(소정 수정)
		if ( 0 == nRowCount && pChkRegNoAutoGive->GetCheck() == 1) 
		{
			//nRowCount = 1;
			if( pChkMultiBook->GetCheck() == 1 )
			{
				nBookCnt = _ttoi( pDM_To_Species->GetCellData(_T("UDF_책수"),0));
				if( nBookCnt > 0 )
					nRowCount = nBookCnt;
				else
					nRowCount = 1;
			}
			else
				nRowCount = 1;
		}
	}

	// 049 Tag의 개수만큼 책을 만든다.
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pDM_To_Book->InsertRow(-1);

		// 등록번호 세팅(자동부여 아니면)
		if ( pChkRegNoAutoGive->GetCheck() == 0 )
		{
			strRegNo = arrItem.GetAt(idx);
			pDM_To_Book->SetCellData(_T("BB_등록번호"), strRegNo, idx);

			//등록구분 세팅
			CString sRegCode;
				//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			if ( strRegNo.GetLength() >= 2 ) {
// 				sRegCode = strRegNo.Left(2);
// 				pDM_To_Book->SetCellData(_T("BB_등록구분_코드"), sRegCode, idx);
// 			}

			if ( strRegNo.GetLength() >= nRegCodeLength ) {
				sRegCode = strRegNo.Left(nRegCodeLength);
				pDM_To_Book->SetCellData(_T("BB_등록구분_코드"), sRegCode, idx);
			}
			//=====================================================

			pDM_To_Book->SetCellData(_T("BB_등록구분_코드"), sRegCode, idx);
			
		}

		// 책부록 플래그
		pDM_To_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), idx);

		// DB FLAG 세팅
		pDM_To_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);

		// 종 -> 책
		ids = CopyDM(pDM_To_Species, 0, pDM_To_Book, idx, _T("BB"));
		if( ids < 0 )
		{
			AfxMessageBox(_T("책DM 구성 에러"));
			return -1;
		}
		// 색인 -> 책
		ids = CopyDM(pDM_To_Index, 0, pDM_To_Book, idx, _T("BB"));
		if( ids < 0 )
		{
			AfxMessageBox(_T("책DM 구성 에러"));
			return -1;
		}
	}

	// 대표책 설정
	INT nRepBookIdx = SetRepresentationBook(pDM_To_Book, _T("BB"));

	// 마크와 싱크맞춤
	m_pCatApi->ConvertMarcToCatDM(&marc, pDM_To_Species, pDM_To_Index, pDM_To_Book, pDM_To_Volume);

	return 0;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 반입 소장종 DB 저장
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 등록번호 중복 조사
// 2. 배가위치 적용
// 3. 도서관 부호 가져오기
// 4. 최종수정정보 생성 040
// 5. LOC_FLAG 넣기
// 6. DB에 저장한다
// 7. 001 TAG 생성
// 8. 임의의 배가일자를 기록한다.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::ImportSpecies( CESL_DataMgr *pDM_Species, 
								  CESL_DataMgr *pDM_Index, 
								  CESL_DataMgr *pDM_Book, 
								  CESL_DataMgr *pDM_Volume, 
								  CESL_DataMgr *pDM_Appendix, 
								  CStringArray *pArrStrAddFrame, 
								  LPCTSTR lpszShelfDate)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book ) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strTemp;
	
	//마지막번호 체크 하기 위해
	CESL_DataMgr *DM_LastNumber = NULL;

	//=======================================================================
	// 1. 등록번호 중복 조사
	//=======================================================================
	DM_LastNumber = FindDM( _T("DM_마크반입_마지막번호") );
	
	CString sLastNumberQuery = _T("") ;
	sLastNumberQuery.Format(_T("SELECT DIVISION_VALUE, LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE"));
	DM_LastNumber->RestructDataManager(sLastNumberQuery);

	nRowCount = pDM_Book->GetRowCount();


	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;
		
		ids = pDM_Book->GetCellData( _T("BB_등록번호"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;

		//등록번호 12자리 만들기
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 			if ( strRegNo.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// 				strRegNo = strRegNo.Left(2) + sRight;
// 			}
// 		}

		if ( strRegNo.GetLength() > nRegCodeLength && strRegNo.GetLength() < 12 ) 
		{
			strRegNo = pParentMgr->m_pInfo->MakeRegNo( strRegNo.Left(nRegCodeLength) , strRegNo.Mid(nRegCodeLength) );
		}
		//=====================================================
		

		//등록번호 넣기
		pDM_Book->SetCellData( _T("BB_등록번호"), strRegNo, idx );

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), pDM_Book->TBL_NAME, strRegNo);
		ids = pDM_Species->GetOneValueQuery(strQuery, strResult);

		if (ids >= 0 && !strResult.IsEmpty())
		{
			//등록번호 중복조사
			m_strLastError.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
			return -1;
		}
	}
	
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	//=======================================================================
	// 2. 배가위치 적용
	//=======================================================================
	ids = ((CCatApi_BO*)m_pCatApi)->ApplyShelfCodeData(pDM_Book, &marc, FALSE);
	if (ids < 0) return -1;


	
	//=======================================================================
	// 3. 도서관 부호 가져오기
	//=======================================================================
	CString strLibCode;
	m_pCatApi->GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);
	if ( strLibCode.IsEmpty() )
	{
		m_strLastError.Format(_T("도서관 부호가 없습니다. 수정해주세요"));
		return -1;
	}

	//=======================================================================
	// 4. 최종수정정보 생성 040
	//=======================================================================
	m_pCatApi->ApplyLastModifyData(&marc);

	// 마크 수정된것을 적용한다.
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0);
	ids = pDM_Species->SetCellData( _T("BS_정리된자료"), _T("Y"), 0 );
	
	//=======================================================================
	// 5. LOC_FLAG 넣기
	//=======================================================================
	ids = pDM_Species->SetCellData( _T("BS_작업상태구분"), _T("1"), 0 );

	//=======================================================================
	// 6. DB에 저장한다. 
	//=======================================================================
	ids = InsertSpecies(pDM_Species, pDM_Index, pDM_Book, pDM_Volume, pDM_Appendix, pArrStrAddFrame, FALSE);

	if (ids < 0) return -1;

	//=======================================================================
	// 7. 001 TAG 생성
	//=======================================================================
	ids = m_pCatApi->CompleteCataloging(pDM_Species, pDM_Species, 0, pDM_Book, FALSE);
	if (ids < 0) return -1;

	CString strShelfDate;
	
	//=======================================================================
	// 8. 임의의 배가일자를 기록한다.
	//=======================================================================
	if (lpszShelfDate == NULL)
	{
		// 기존 홍수 수정
		// strShelfDate = _T("1200/01/01");
		strShelfDate = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));
	}
	else
		strShelfDate = lpszShelfDate;		

	CStringArray arrSpeciesKey;
	CStringArray arrBookKey;
	CStringArray arrAppendixKey;

	arrSpeciesKey.Add(pDM_Species->GetCellData( _T("BS_종KEY"), 0));
	
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;

		arrBookKey.Add(pDM_Book->GetCellData( _T("BB_책KEY"), idx));
	}
	if(pDM_Appendix)
	{
		nRowCount = pDM_Appendix->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			arrAppendixKey.Add(pDM_Appendix->GetCellData( _T("BB_책KEY"), idx));
		}
	}

	ids = m_pCatApi->RecordLocDate(pDM_Species, &arrSpeciesKey, &arrBookKey, &arrAppendixKey, strShelfDate, FALSE);
	if (ids < 0) return -1;

	//=====================================================
	//2008.12.29 ADD BY PJW : 홈체이지 IDX_BO_TBL에 추가할 쿼리를 SENDFRAME바로 앞으로 뺀다.
	CString strSpeciesKey;
	pDM_Species->GetCellData(_T("BS_종KEY"),0,strSpeciesKey);
 	m_pCatApi->ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	//=====================================================

	ids = pDM_Species->SendFrame();
	if (ids < 0) 
	{
		m_strLastError = _T("DB에 데이터 저장 실패");
		return ids;
	}

	ids = pDM_Species->EndFrame();
	
	return 0;

	EFS_END
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  DB에 저장
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 색인의 내용을 종에 적용시킨다.
// 2. 기본 정보를 적용시킨다.
// 3. 정리중 반입이면 제어번호 제거
// 4. 색인 쿼리 생성
// 5. 구입/기증정보 쿼리 생성
// 6. 책정보 쿼리 생성
// 7. ebook 관련 저장 하기 
// 8. 나머지쿼리 생성
// 9. 마지막 번호 Update
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::InsertSpecies( CESL_DataMgr *pDM_Species, 
								  CESL_DataMgr *pDM_Index, 
								  CESL_DataMgr *pDM_Book, 
								  CESL_DataMgr *pDM_Volume, 
								  CESL_DataMgr *pDM_Appendix, 
								  CStringArray *pArrStrAddFrame, 
								  BOOL bCommit)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL
		) return -1;

	INT ids = -1;

	// 종키
	CString strSpeciesKey;

	// 쿼리 만들때 무시할 Alias Array
	CArray<CString, CString> arrRemoveAliasList;
	
	//========================================================================
	// 1. 색인의 내용을 종에 적용시킨다.
	//========================================================================
	CopyDM(pDM_Index, 0, pDM_Species, 0,  _T("BS"));

	//========================================================================
	// 2. 기본 정보를 적용시킨다.7
	//========================================================================
	ids = m_pCatApi->ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	// 쿼리 만들기 시작

	// 종 쿼리를 만든다.
	CMarc marc;
	CString strStreamMarc;
	
    ids = pDM_Species->GetCellData( _T("BS_MARC"), 0, strStreamMarc);
	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	//===================================================
	//2010.05.03 ADD BY PJW : 정리중 반입도 040tag 정보 추가하도록 함
	m_pCatApi->ApplyLastModifyData(&marc);
	//===================================================
	//===============================================
	//===============================================
	//2008.07.16 ADD BY PJW : 화폐구분을 마크에서 가져온다.
	CString strCurrencyCode;
	CString strTemp;
	ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strTemp);
	strCurrencyCode = GetPriceCode(strTemp, strCurrencyCode);
	//===============================================
	
	//========================================================================
	// 3. 정리중 반입이면 제어번호 제거
	//========================================================================	
	ids = m_pInfo->pMarcMgr->DeleteField( &marc, _T("001") );
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0 );

	ids = pDM_Species->MakeRecKey(strSpeciesKey);
	if (ids < 0) return ids;
	pDM_Species->SetCellData( _T("BS_종KEY"), strSpeciesKey, 0);
	
	pDM_Species->InitDBFieldData();

	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species);
	m_pCatApi->LogToFile(pDM_Species->m_lastmsg);

	//========================================================================
	// 4. 색인 쿼리 생성
	//========================================================================	
	// 색인에 Insert할 Insert문을 구성한다.
	pDM_Index->SetCellData( _T("IBS_REC_KEY"), strSpeciesKey, 0);
	pDM_Index->SetCellData( _T("IBS_종구분값"), _T("0"), 0);

	// 색인 데이터 적용
	arrRemoveAliasList.RemoveAll();
	ids = m_pCatApi->ApplyIndex(pDM_Species, pDM_Index, pDM_Book);

	// ST_CODE를 설정한다.
	m_pCatApi->ApplyIndex_ST_CODE(pDM_Index, pDM_Book);
	
	pDM_Species->InitDBFieldData();

	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM_Species);
	m_pCatApi->LogToFile(pDM_Species->m_lastmsg);

	//이용제한구분/관리구분/자료실/등록구분 구분값의 구분값 concatenation
	CStringArray Index_classfycodes;
	Index_classfycodes.Add( _T("IBS_이용제한구분_코드"));
	Index_classfycodes.Add( _T("IBS_관리구분_코드"));
	Index_classfycodes.Add( _T("IBS_자료있는곳INFO_코드"));
	Index_classfycodes.Add( _T("IBS_등록구분_코드"));

	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("BB_이용제한구분_코드"));
	Book_classfycodes.Add( _T("BB_관리구분_코드"));
	Book_classfycodes.Add( _T("BB_배가위치부호"));
	Book_classfycodes.Add( _T("BB_등록구분_코드"));

	CString Index_Update_sql;
	m_pCatApi->MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("BB"));

	ids = pDM_Species->AddFrame(Index_Update_sql);
	if (ids < 0) return ids;

	//========================================================================
	// 5. 구입/기증정보 쿼리 생성
	//========================================================================
	CString sAcqCode = pDM_Book->GetCellData( _T("BB_수입구분_코드"), 0 );
	
	CString sAcqKey;
	ids = pDM_Species->MakeRecKey(sAcqKey);
	if ( 0 > ids ) return -1;

	CString sProvider = pDM_Book->GetCellData( _T("BB_구입처"), 0 );
	//===============================================
	//20080630 ADD BY PJW : 입력일자를 추가한다.수입년도를 추가한다.
	CTime t = CTime::GetCurrentTime();
	CString strInputDate;
	CString strAcqYear;
	strInputDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	GetDlgItem( IDC_eACQUISIT_YEAR )->GetWindowText(strAcqYear);
	//===============================================
	//===============================================
	//2008.07.08 ADD BY PJW : 화폐구분, 가격정보를 추가한다.
	CString strPrice; 
	strPrice = pDM_Book->GetCellData( _T("BB_가격"), 0 );
	//===============================================

	if ( sAcqCode == _T("1") ) {
		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
		pDM_Species->AddDBFieldData( _T("PROVIDER_CODE"), _T("STRING"), sProvider);		
		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
		//===============================================
		//20080630 ADD BY PJW : 입력일자를 추가한다.수입년도를 추가한다.
		pDM_Species->AddDBFieldData( _T("INPUT_DATE"), _T("STRING"), strInputDate);
		pDM_Species->AddDBFieldData( _T("ORDER_YEAR"), _T("STRING"), strAcqYear);
		//===============================================
		//2008.07.08 ADD BY PJW : 화폐구분, 가격정보를 추가한다.
		pDM_Species->AddDBFieldData( _T("CURRENCY_CODE"), _T("STRING"), strCurrencyCode);
		strPrice.Replace(_T(","),_T(""));
		pDM_Species->AddDBFieldData( _T("PRICE"), _T("STRING"), strPrice);
		//===============================================
		pDM_Species->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

		pDM_Species->MakeInsertFrame(_T("BO_PURCHASEINFO_TBL"));
	}
	else if ( sAcqCode == _T("2") ) {
		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
		pDM_Species->AddDBFieldData( _T("DONATOR_NAME"), _T("STRING"), sProvider);		
		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
		//===============================================
		//20080630 ADD BY PJW : 입력일자를 추가한다.수입년도를 추가한다.
		pDM_Species->AddDBFieldData( _T("INPUT_DATE"), _T("STRING"), strInputDate);
		pDM_Species->AddDBFieldData( _T("ACQ_YEAR"), _T("STRING"), strAcqYear);
		//===============================================
		//===============================================
		//2008.07.08 ADD BY PJW : 화폐구분, 가격정보를 추가한다.
		pDM_Species->AddDBFieldData( _T("CURRENCY_CODE"), _T("STRING"), strCurrencyCode);
		strPrice.Replace(_T(","),_T(""));
		pDM_Species->AddDBFieldData( _T("PRICE"), _T("STRING"), strPrice);
		//===============================================
		pDM_Species->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

		pDM_Species->MakeInsertFrame(_T("BO_DONINFO_TBL"));
		
	}

	//========================================================================
	// 6. 책정보 쿼리 생성
	//========================================================================

	CString strVolSortNo;
	for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {

		pDM_Book->SetCellData( _T("BB_원부KEY"), m_strAccessionRecKey, i );
		pDM_Book->SetCellData( _T("BB_수정정보KEY"), sAcqKey, i );

		strVolSortNo.Format(_T("%d"), (1)*10);
		pDM_Book->SetCellData( _T("BB_권일련번호"), strVolSortNo, i );
	}

	if ( sAcqKey != _T("") ) {
		ids = ((CCatApi_BO*)m_pCatApi)->MakeBookQueryFrame(pDM_Book, strSpeciesKey, sAcqKey, pDM_Species);
	}
	else {
		ids = ((CCatApi_BO*)m_pCatApi)->MakeBookQueryFrame(pDM_Book, strSpeciesKey, _T("-1"), pDM_Species);
	}

	if (ids < 0)
	{
		if ( m_strLastError == _T("") ) 
		{
			m_strLastError = _T("책정보 쿼리만들기 실패");
		}
		return ids;
	}

	//========================================================================
	// 7. ebook 관련 저장 하기
	//========================================================================
    CESL_DataMgr * pDM_EBook ; 
	pDM_EBook = new CESL_DataMgr;
	pDM_EBook->SetCONNECTION_INFO(pDM_Species->CONNECTION_INFO);
	pDM_EBook->InitDMPKFromFile(_T("DMUP_단행_종_EBOOK"));
	pDM_EBook->SetMarcMgr(m_pInfo->pMarcMgr);

	CBO_SPECIES *pBO_Species = new CBO_SPECIES ( this );

    ids = pBO_Species->MakeEBookInfoAndQueryFrame (pDM_Book,pDM_EBook,pDM_Species) ;
    if (ids < 0) 
	{
		m_strLastError = _T("ebook정보 쿼리만들기 실패");
		return ids;
	}

	//========================================================================
	// 8. 나머지쿼리 생성
	//======================================================================== 
	//=====================================================
	//2008.12.29 REM BY PJW : 홈체이지 IDX_BO_TBL에 추가할 쿼리를 SENDFRAME바로 앞으로 뺀다.
// 	m_pCatApi->ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	//=====================================================
	
	// ECOSEARCH용 쿼리 생성
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strJJSQuery;
		strJJSQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		ids = pDM_Species->AddFrame(strJJSQuery);
		if (ids < 0) return ids;
	}

	//========================================================================
	// 9. 마지막 번호 Update
	//========================================================================
    CESL_DataMgr *pDmLastNum = FindDM( _T("DM_마지막번호") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_마지막번호 확인") );
		return -1;
	}

	INT iBookCount = pDM_Book->GetRowCount();
	pDM_Book->SORT(_T("BB_등록번호"));
	CString sRegno = pDM_Book->GetCellData( _T("BB_등록번호"), iBookCount-1 );
	INT iRegno = 0;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if ( sRegno != _T("") ) {
// 		iRegno = _ttoi(sRegno.Mid(2));
// 	}
	if ( sRegno != _T("") ) {
		iRegno = _ttoi(sRegno.Mid(nRegCodeLength));
	}
	//=====================================================
	
	
	for ( i = 0 ; i < pDmLastNum->GetRowCount() ; i++ ) {
		CString sRegCode = pDmLastNum->GetCellData( _T("BB_등록구분_코드"), i );
		CString sLastNum = pDmLastNum->GetCellData( _T("마지막번호"), i );
		INT iLastNum = _ttoi ( sLastNum );
		if ( iRegno != 0 ) {
			iLastNum = iRegno;
		}
		CString sQuery;
		sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%d', LAST_WORK='%s' WHERE division_value = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;") , iLastNum , _T("자동등록번호부여"), sRegCode );
		ids = pDM_Species->AddFrame( sQuery );
		if ( 0 > ids ) {
			AfxMessageBox( _T("마지막번호 AddFrame 확인") );
			return -1;
		}
	}
	
	// KOL.RED.2018.223 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if ( !sAcqKey.IsEmpty() && m_bMatchISBN == TRUE ) // 수서정보KEY는 반드시 있어야 한다
	{
		//위치이동
		// 예약기능 사용 유무 확인 
		CString strReservationUseYN,strReserveSpecialCond;
		pDM_Species->GetOneValueQuery( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='예약관리' AND VALUE_NAME='예약기능사용여부' AND MANAGE_CODE = UDF_MANAGE_CODE"), strReservationUseYN );

		pDM_Species->GetOneValueQuery(_T("SELECT MAX(VALUE_1) FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='수서' AND GROUP_3='단행' AND VALUE_NAME = '비치희망우선예약특별조건' AND MANAGE_CODE= UDF_MANAGE_CODE"), strReserveSpecialCond); 

		for (i = 0; i < pDM_Book->GetRowCount(); i++ ) 
		{
			// ISBN으로 희망자료 신청정보를 조회한다
			CString strISBN = pDM_Book->GetCellData(_T("BB_낱권ISBN"), i);
			if (!strISBN.IsEmpty())
			{
				//동일 자료가 신청된 경우를 처리하기 위해
				CString strQuery;
				//2020.02-07 JOB.2020.0011 ADD BY KYJ : 자관 비치희망자료만 보도록 쿼리 수정
				//strQuery.Format(_T("SELECT REC_KEY FROM (SELECT REC_KEY FROM BO_FURNISH_TBL WHERE FURNISH_STATUS='1' AND EA_ISBN='%s' ORDER BY APPLICANT_DATE)"), strISBN);
				strQuery.Format(_T("SELECT REC_KEY FROM (SELECT REC_KEY FROM BO_FURNISH_TBL WHERE FURNISH_STATUS='1' AND EA_ISBN='%s' AND MANAGE_CODE = UDF_MANAGE_CODE ORDER BY APPLICANT_DATE)"), strISBN);
				
				CString strRecKey;
				pDM_Species->GetOneValueQuery(strQuery, strRecKey);
				if (!strRecKey.IsEmpty())
				{
					// 자료상태에 따라서 연계처리 내용이 달라진다
					CString strWorkingStatus = pDM_Book->GetCellData(_T("BB_자료상태"), i);
										
					if (!strWorkingStatus.IsEmpty() && strWorkingStatus.CompareNoCase(_T("BOL112N")) == 0)				
					{
						// 소장상태로 반입되었다면
						// 1. 희망자료정보 갱신 (상태, 수서코드, 수서키, 비치일자)
						// 2. 예약처리

						// 자료상태 처리시 컴퓨터 시간으로 처리중 -> ImportSpecies()
						CString strShelfDate = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));

						CString strFurnishQuery;
						strFurnishQuery.Format(_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS='3', ACQ_CODE='%s', ACQ_KEY=%s, FURNISH_DATE='%s' WHERE REC_KEY='%s';"), sAcqCode, sAcqKey, strShelfDate, strRecKey);
						ids = pDM_Species->AddFrame(strFurnishQuery);
						if (0 > ids)
						{
							// 자료반입은 정상처리를 위해서 디버그에서만 출력
							TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -10 : %d"), ids);
						}
						
						//위치이동 
						// 예약기능 사용 유무 확인 
						//CString strReservationUseYN;
						//pDM_Species->GetOneValueQuery( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='예약관리' AND VALUE_NAME='예약기능사용여부' AND MANAGE_CODE = UDF_MANAGE_CODE"), strReservationUseYN );
						
						if (strReservationUseYN.CompareNoCase(_T("Y")) == 0) // 사용하지 않는다면 확인할 필요가 없다
						{
							//위치이동 
							// 희망자료 우선예약유무 확인
							strQuery.Format(_T("SELECT RESERVATION_YN FROM BO_FURNISH_TBL WHERE REC_KEY=%s AND USER_NO IS NOT NULL"), strRecKey);

							CString strReservationYN;
							pDM_Species->GetOneValueQuery(strQuery, strReservationYN);

							if (strReservationYN.CompareNoCase(_T("Y")) == 0)
							{
								// IF문이 많아져서 
								BOOL bFurnishReservation = FALSE;

								// 이용자 DM
								CESL_DataMgr DM_USER;
 								DM_USER.SetCONNECTION_INFO(pDM_Species->CONNECTION_INFO);
 								DM_USER.InitDMPKFromFile(_T("DM_BO_LOC_3300"));

								CString strWhere;
								strWhere.Format(_T("CLU.USER_NO = (SELECT USER_NO FROM BO_FURNISH_TBL WHERE REC_KEY=%s)"), sAcqCode, strRecKey);
								ids = DM_USER.RefreshDataManager(strWhere);
								if (0 > ids)
								{
									// 자료반입은 정상처리를 위해서 디버그에서만 출력
									TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -11 : %d"), ids);
								}

								// 이용자가 검색된 경우에만
								bFurnishReservation = (DM_USER.GetRowCount() > 0);
								
								if (bFurnishReservation)
								{
									// 연체현황 확인
									bFurnishReservation = (_ttoi(DM_USER.GetCellData(_T("연체책수"), 0)) < 1);
								}
																				
								CString strUserClassCode = DM_USER.GetCellData(_T("대출직급정보"), 0);

								//if (bFurnishReservation)
								if (bFurnishReservation && strReserveSpecialCond != _T("Y") )
								{
									// 직급별 예약 가능 권수 확인
									strQuery.Format(_T("SELECT PREENGAGE_BOOK_CNT FROM CD_CODE_TBL WHERE CLASS=31 AND CODE = '%s'"), strUserClassCode); 

									CString strReservationApplicantCount;
									pDM_Species->GetOneValueQuery(strQuery, strReservationApplicantCount);

									CString strReservationCount = DM_USER.GetCellData(_T("총예약책수"), 0);
									bFurnishReservation = (_ttoi(strReservationApplicantCount) > 0 && _ttoi(strReservationApplicantCount) > _ttoi(strReservationCount));
								}

								if (bFurnishReservation)
								{
									// 성인자료
									CString strObjectCode = pDM_Book->GetCellData(_T("BB_이용대상구분_코드"), i);
									if (strObjectCode.CompareNoCase(_T("AD")) == 0)
									{
										strQuery.Format(_T("SELECT USEOBJCODE FROM CD_CODE_TBL WHERE CLASS=31 AND CODE = '%s'"), strUserClassCode); 

										CString strValue;
										pDM_Species->GetOneValueQuery(strQuery, strValue);

										bFurnishReservation = (strValue.IsEmpty() || strValue.CompareNoCase(_T("Y")) == 0);
									}
								}

								if (bFurnishReservation)
								{
									// 등록구분 대출제한
									CString strRegCode = pDM_Book->GetCellData(_T("BB_등록구분_코드"), i);

									strQuery.Format(_T("SELECT LOAN_YN FROM CD_CODE_TBL WHERE CLASS = 4 AND CODE='%s'"), strRegCode); 

									CString strValue;
									pDM_Species->GetOneValueQuery(strQuery, strValue);

									bFurnishReservation = (strValue.CompareNoCase(_T("Y")) == 0);
								}

								if (bFurnishReservation)
								{
									// 별치구분 대출제한
									CString strSeperateCode = pDM_Book->GetCellData(_T("BB_별치기호_코드"), i);
									if (!strSeperateCode.IsEmpty())
									{
										strQuery.Format(_T("SELECT LOAN_YN FROM CD_CODE_TBL WHERE CLASS = 11 AND CODE='%s'"), strSeperateCode); 

										CString strValue;
										pDM_Species->GetOneValueQuery(strQuery, strValue);

										bFurnishReservation = (strValue.CompareNoCase(_T("Y")) == 0);
									}
								}

								if (bFurnishReservation)
								{										
									// 자료실 대출제한
									CString strLocCode = pDM_Book->GetCellData(_T("BB_배가위치부호"), i);

									strQuery.Format(_T("SELECT LOAN_YN FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE='%s'"), strLocCode);
									
									CString strValue;
									pDM_Species->GetOneValueQuery(strQuery, strValue);

									bFurnishReservation = (strValue.CompareNoCase(_T("Y")) == 0);
								}

								if (bFurnishReservation)
								{
									// 예약정보 생성
									CString strParamManageCode = pDM_Book->GetCellData(_T("BB_관리구분_코드"), i);

									// 예약만기일 구성
									strQuery.Format(_T("SELECT RESERVATION_EXPIRE_DAY FROM CD_CODE_TBL WHERE CLASS = 31 and CODE='%s'"), strUserClassCode);	

									CString strValue;
									pDM_Species->GetOneValueQuery(strQuery, strValue);

									CString strParamExpireDate;
									if (strParamManageCode.IsEmpty())
									{
										strParamExpireDate.Format(_T("ECO$F_G_ISADDHOLIDAY$I(TRIM(SYSDATE), %d)+(23/24)+(59/24/60)+(59/24/60/60)"), _ttoi(strValue));
									}
									else
									{
										strParamExpireDate.Format(_T("ECO$F_G_ISADDHOLIDAY$I(TRIM(SYSDATE), %d, '%s')+(23/24)+(59/24/60)+(59/24/60/60)"), _ttoi(strValue), strParamManageCode);
									}

									//청구기호|ECO$F_MAKE_CALLNO$I(B.SEPARATE_SHELF_CODE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE)|str|||
									CString strParamCallNo;
									strParamCallNo.Format(_T("ECO$F_MAKE_CALLNO$I(%s, %s, %s, %s, %s)"), 
											              pDM_Book->GetCellData(_T("BB_별치기호_코드"), i), pDM_Book->GetCellData(_T("BB_분류기호"), i), 
														  pDM_Book->GetCellData(_T("BB_도서기호"), i), pDM_Book->GetCellData(_T("BB_권책기호"), i),
														  pDM_Book->GetCellData(_T("BB_복본기호"), i));

									// 한줄서기라면 권일련번호 
									strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'ONELNERESERV'")); 
									pDM_Species->GetOneValueQuery(strQuery, strValue);

									CString strParamVolInfo;
									if (strValue.CompareNoCase(_T("Y")) == 0)
									{
										strParamVolInfo = pDM_Book->GetCellData(_T("BB_권일련번호"), i);
									}

									CString strReservationQuery;
									strReservationQuery.Format(_T("INSERT INTO LS_WORK_T01(REC_KEY, PUBLISH_FORM_CODE, STATUS, RESERVATION_DATE, ")
																		_T("BOOK_KEY, SPECIES_KEY, USER_KEY, CALL_NO, RESERVATION_EXPIRE_DATE, TITLE, PUBLISHER, AUTHOR, REG_CODE, REG_NO, SHELF_LOC_CODE, CLASS_NO_TYPE, CLASS_NO, MANAGE_CODE, SEPARATE_SHELF_CODE, VOL_INFO, MEMBER_CLASS, USER_POSITION_CODE, USER_CLASS_CODE) ")
											                   _T("VALUES (ESL_SEQ.NEXTVAL, 'MO', '3', SYSDATE, ")
																		_T("%s, %s, %s, %s, %s, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');"),
															   pDM_Book->GetCellData(_T("BB_책KEY"), i),
															   pDM_Book->GetCellData(_T("BB_종KEY"), i),
															   DM_USER.GetCellData(_T("REC_KEY"), 0),
															   strParamCallNo,
															   strParamExpireDate, 
															   pDM_Index->GetCellData(_T("IBS_표제사항"), 0),
															   pDM_Index->GetCellData(_T("IBS_발행자사항"), 0),
															   pDM_Book->GetCellData(_T("BB_저작자"), i),
															   pDM_Book->GetCellData(_T("BB_등록구분_코드"), i),
															   pDM_Book->GetCellData(_T("BB_등록번호"), i),
															   pDM_Book->GetCellData(_T("BB_배가위치부호"), i),
															   pDM_Book->GetCellData(_T("BB_분류기호구분_코드"), i),
															   pDM_Book->GetCellData(_T("BB_분류기호"), i),
															   strParamManageCode, 
															   pDM_Book->GetCellData(_T("BB_별치기호_코드"), i),
															   strParamVolInfo,
															   DM_USER.GetCellData(_T("회원구분"), 0),
															   DM_USER.GetCellData(_T("대출소속정보"), 0),
															   DM_USER.GetCellData(_T("대출직급정보"), 0)																   
															   );

									ids = pDM_Species->AddFrame(strReservationQuery);
									if (0 > ids) {
										// 자료반입은 정상처리를 위해서 디버그에서만 출력
										TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -12 : %d"), ids);
									}
									
									// 이용자 예약권수 수정
									INT nReservationCount = _ttoi(DM_USER.GetCellData(_T("총예약책수"), 0));
									nReservationCount++;

									CString strUserQuery;
									strUserQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET RESERVATION_COUNT=%d WHERE REC_KEY=%s;"), nReservationCount, DM_USER.GetCellData(_T("REC_KEY"), 0));
									ids = pDM_Species->AddFrame(strUserQuery);
									if (0 > ids) {
										// 자료반입은 정상처리를 위해서 디버그에서만 출력
										TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -13 : %d"), ids);
									}										
								}
							}
						}
					}
					else
					{
						// 작업상태로 반입되었다면
						// 1. 희망자료정보 갱신 (상태, 수서코드, 수서키)
						CString strFurnishQuery;
						strFurnishQuery.Format(_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS='2', ACQ_CODE='%s', ACQ_KEY=%s WHERE REC_KEY='%s';"), sAcqCode, sAcqKey, strRecKey);
						ids = pDM_Species->AddFrame(strFurnishQuery);
						if (0 > ids) {
							// 자료반입은 정상처리를 위해서 디버그에서만 출력
							TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -20 : %d"), ids);
						}
					}					
				}
			}
		}
	}
	// KOL.RED.2018.223 ---------------------------------------------------------------------------	

	return 0;

	EFS_END
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  대표책 구한다.
// [Parameters] 책DM/알리아스헤더/참조인덱스
// [Return Values] 성공 0 에러 -1
// [Mini Spec] 
// 대표책을 구한다.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::GetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT &nIdx)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));
	nIdx = -1;
	
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strToAliasHeader)) continue;

		if (pDM_Book->GetCellData( strToAliasHeader + _T("_대표책여부"), idx) == _T("Y")) 
		{
			nIdx = idx;
			return 0;
		}
	}

	return -1;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  대표책 세팅 
// [Parameters] 책DM/알리아스헤더/인덱스
// [Return Values] 성공 0 에러 -1
// [Mini Spec] 
// 대표책을 구한다.
// 대표책이 없으면 첫번째 책이 대표책
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::SetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT nIdx /*= -1*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	// nIdx 가 -1이고 대표책이 없을경우 첫번째 책을 대표책으로 한다.

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	if (nIdx == -1)
	{
		if (GetRepresentationBook(pDM_Book, strToAliasHeader, nIdx) < 0)
		{
			// 대표책이 없을경우
			for (INT idx = 0; idx < nRowCount; idx++)
				if (IsBook(pDM_Book, idx, strToAliasHeader)) break;
			nIdx = idx;
		}		
	}
	else
	{
		// nIdx 의 데이터가 책인지 확인한다.
		if (!IsBook(pDM_Book, nIdx, strToAliasHeader)) return -1;
	}

	// 대표책 IDX만 빼고 나머지는 N으로 세팅해 준다.
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (idx == nIdx)
			pDM_Book->SetCellData( strToAliasHeader + _T("_대표책여부"), _T("Y"), idx);
		else
			pDM_Book->SetCellData( strToAliasHeader + _T("_대표책여부"), _T("N"), idx);
	}
	
	return nIdx;

	EFS_END
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  책수를 구한다. 
// [Parameters] 책DM/알리아스헤더/등록구분/볼륨명
// [Return Values] 성공 0 에러 -1
// [Mini Spec] 
// 책수를 구한다.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::GetBookCount(CESL_DataMgr *pDM_Book, 
						  CString strToAliasHeader /*= _T("")*/, 
						  CString strRegCode /*= _T("")*/, 
						  LPCTSTR lpszVol /*=NULL*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책과 권이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx, strToAliasHeader, lpszVol)) continue;

		// 등록구분 비교
		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData( strToAliasHeader + _T("_등록구분_코드"), idx) != strRegCode) continue;
		
		nReturnCnt++;
	}

	return nReturnCnt;

	EFS_END
	return -1;
}

BOOL CMarcImportDlg::IsBook(CESL_DataMgr *pDM_Book, 
					 INT idx, 
					 CString strToAliasHeader /*= _T("")*/, 
					 LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	// 단행만 있음
	if (pDM_Book->GetCellData( strToAliasHeader + _T("_책부록플래그"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData( strToAliasHeader + _T("_DB_FLAG"), idx) == _T("D")) return FALSE;

	// 편권차가 있으면 체크
	if (lpszVol != NULL)
		if (pDM_Book->GetCellData( strToAliasHeader + _T("_편권차"), idx) != lpszVol) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  DM 복사  
// [Parameters] 소스DM/소스인덱스/타겟DM/타겟인덱스/알리아스헤더/거부리스트
// [Return Values] 성공 0 에러 -1
// [Mini Spec] 
// 거부리스트를 제외한 알리스에 맞춰 DM을 복사한다.
/////////////////////////////////////////////////////////////////////////////
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
		if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		if ( strAlias == _T("IBS_배가위치부호") ) continue;
		// User Ignore List
		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		
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


BOOL CMarcImportDlg::IsStringInArray(CStringArray *pArr, CString str)
{
	EFS_BEGIN

	if (pArr == NULL || str.IsEmpty()) return FALSE;

	INT nRowCount = pArr->GetSize();
	if (nRowCount == 0) return FALSE;

	for (INT i = 0; i < nRowCount; i++)
	{
		if (pArr->GetAt(i) == str) return TRUE;
	}

	return FALSE;

	EFS_END
	return FALSE;
}

CString CMarcImportDlg::GetPriceCode(CString strPrice, 
									CString &strRealPrice)
{
	EFS_BEGIN

	CString strResult;

	INT nStartPrice = -1;

	TCHAR ch = 0;
	INT nLength = strPrice.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		ch = strPrice.GetAt(idx);
		if (_istdigit(ch))
		{
			nStartPrice = idx;
			break;
		}
		
		strResult += ch;
	}

	if (nStartPrice >= 0)
		strRealPrice = strPrice.Mid(nStartPrice);

	return strResult;

	return _T("");

	EFS_END
	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  049태그 제거
// [Parameters] 
// [Return Values] 성공 0 에러 -1
// [Mini Spec] 
// 049태그를 제거한다.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::Delete049TagFromMarc()
{
EFS_BEGIN

	CString strHD_Species = _T("BS");
	CMarc marc;
	CString strStreamMarc;
	CString strEditMarc;
	
	CArray<CString, CString&> pArr049l;
	CString strItemData, strBookCnt;
	
	INT nBookCnt;
	INT ids = -1;

	INT nCount = m_pDM_Species->GetRowCount();
	for (INT i=0; i < nCount; i++ )
	{
		strStreamMarc = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), i);
		if (strStreamMarc.IsEmpty()) return -1;

		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) return -1;
		
		
		m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
		nBookCnt = pArr049l.GetSize();		
		pArr049l.RemoveAll();				
		
		strBookCnt.Format(_T("%d"),nBookCnt);
		m_pDM_Species->SetCellData(_T("UDF_책수"),strBookCnt,i);		

		/*
		// 2007.08.18 ADD BY PDJ
		// 반입시 049를 날리는데 쓰지도 않는 별치를 신택스 체크했음.
		// UDF_책수 세팅후에 날려야 함
		m_pInfo->pMarcMgr->DeleteItem(&marc, _T("049$f") );
		*/

		ids = m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
		if (ids < 0) return -1;

		ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc );
		if (ids < 0) return -1;

		ids = m_pDM_Species->SetCellData( strHD_Species + _T("_MARC"), strStreamMarc, i);
		if (ids < 0) return -1;
		
	}

	return 0;
EFS_END
	return -1;
}


VOID CMarcImportDlg::OnbInit() 
{	
	//반입후 버튼(정리중반입, 파일열기, 등록번호 자동부여, 형식구분)을 Disable
	GetDlgItem(IDC_bFILE_OPEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(TRUE);
	GetDlgItem(IDC_ChkGiveAutoRegno)->EnableWindow(TRUE);
	GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(TRUE);
	GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ChkGiveAutoRegno))->SetCheck(FALSE);
	/***********************************************************
	 ***********************************************************/
	//20080515 ADD BY PJW : 정리중반입, 파일열기, 등록번호 자동부여, 형태구분 Default값 셋팅되도록 수정
	((CButton*)GetDlgItem(IDC_ChkWorkStatus))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_ChkMULTIBOOK))->SetCheck(FALSE);
	GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(TRUE);
	CString strFormCode;
	pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"), strFormCode);
	CESL_ControlMultiComboBox * pCMB = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbFORM_CLASS);
	pCMB->SelectString(strFormCode);
	/***********************************************************
	 ***********************************************************/
	GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_bMODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_bEXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_bIMPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_bEXPORT_EDITMARC)->EnableWindow(FALSE);
	
	( (CButton*) GetDlgItem(IDC_ChkAccessionRec) )->SetCheck(FALSE);
	//GetDlgItem(IDC_ChkAccessionRec)->EnableWindow(FALSE);
	GetDlgItem(IDC_ChkAccessionRec)->EnableWindow(TRUE);
	GetDlgItem(IDC_eAccessionRecNo)->EnableWindow(TRUE);
	
	//2020.02.13 JOB.2020.0013 ADD BY KYJ : 비치희망 ISBN 매칭 선택해서 할 수 있도록 기능개선
	((CButton*)GetDlgItem(IDC_ChkMatchISBN))->SetCheck(FALSE);

	//===================================================
	//2009.03.10 ADD BY PJW : 그리드DM을 초기화한다.
	m_pDmGridError->FreeData();
	m_pDmGridOk->FreeData();
	m_pDmGridSuccess->FreeData();
	//===================================================
	

	AllControlDisplay(_T("CM_정리_마크반입"));

	DefaultAccessionRecNo();

	m_wndStatusBarCtrl.SetText(_T(""), 1, 0);
	m_wndStatusBarCtrl.SetText(_T(""), 2, 0);
	m_wndStatusBarCtrl.SetText(_T(""), 3, 0);
}

void CMarcImportDlg::OnrTXT() 
{
	m_nFileType = 1;
	
}

void CMarcImportDlg::OnrEXCEL() 
{
	m_nFileType = 0;
	
}

void CMarcImportDlg::OnrMODS() 
{
	m_nFileType = 3;	
}

void CMarcImportDlg::OnrCSV() 
{
	m_nFileType = 5;	
}

INT CMarcImportDlg::GetAcqType()
{
	CString sAcqCode;
	GetControlData( _T("CM_정리_마크반입_구분코드"), _T("IBS_수입구분_코드"), sAcqCode );

	return _ttoi(sAcqCode);
}

VOID CMarcImportDlg::SetCtrlByAcqCode()
{
	
}

void CMarcImportDlg::OnSelchangecmbIMPORTTYPE() 
{
	SetCtrlByAcqCode();	
}

void CMarcImportDlg::OnChkAccessionRec() 
{
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck )
	{
		if ( CheckAccessionRecNo() == FALSE ) 
		{
			( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->SetCheck(FALSE);
			return;
		}

		//JOB.2019.0003 ADD BY KYJ : 수입년도변경시 원부번호가 변경안되는 현상 수정 
		//=================================================================================		
		//DefaultAccessionRecNo();
		//==================================================================================

		( (CEdit*)	GetDlgItem	(IDC_eAccessionRecNo	) )->EnableWindow(TRUE);
		( (CButton*) GetDlgItem	(IDC_btnAccessionRecNo	) )->EnableWindow(TRUE);		
		
	}
	else
	{
		( (CEdit*) GetDlgItem	(IDC_eAccessionRecNo	) )->EnableWindow(FALSE);
		( (CButton*) GetDlgItem	(IDC_btnAccessionRecNo	) )->EnableWindow(FALSE);
	}
}

void CMarcImportDlg::OnbtnAccessionRecNo() 
{
	// 원부관리
	
}

void CMarcImportDlg::DefaultAccessionRecNo()
{
	CString strQuery;
	CString strAcqYear;
	CString strAccessionRecNo;
	INT		nAccNum;

	GetControlData( _T("CM_정리_마크반입_구분코드"), _T("BS_수입년도"), strAcqYear );

	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'ACC_NO' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear );
	CString sResult;
	m_pDM_Main->GetOneValueQuery( strQuery , sResult );

	nAccNum = _ttoi(sResult.GetBuffer(0));
	m_nAccNum = ++nAccNum;	
	strAccessionRecNo.Format( _T("%d") , nAccNum );
	SetControlData( _T("CM_정리_마크반입_구분코드") , _T("BB_원부번호") , strAccessionRecNo);

	( (CEdit*) GetDlgItem (IDC_eAccessionRecNo) )->EnableWindow(FALSE);


}

INT CMarcImportDlg::SetAccessionRecNoApply()
{
	// 0. 원부 적용유무 체크
	//    원부 마지막 번호 체크. 마지막 번호 보다 크면 사용할 수 없다.
	// 1. 선정된 자료중 등록번호가 부여되지 않은 자료가 있는지 확인한다.
	// 2. 부여되지 않으 자료가 있으면 원부적용을 자동 해제 하면 메시지로 안내한다.
	//  => 등록번호가 부여되지 않은 자료가 있어 원부번호를 적용할 수 없습니다.
	// 3. 사용되고 있는 원부있지 알려준다.
	// 4. 원부가 있으면 키만 따오고
	// 5. 원부가 없으면 생성한다.	
	EFS_BEGIN
	CString strAccessionRecNo;
	m_strAccessionRecKey = _T("");

	CString strAcqCode;
	CString strAcqYear;
	CString sREGISTRANT;
	CString sREG_DATE;
	CString sQuery;
	
	GetControlData(_T("CM_정리_마크반입_구분코드"), _T("IBS_수입구분_코드")	, strAcqCode);
	GetControlData(_T("CM_정리_마크반입_구분코드"), _T("BS_수입년도")		, strAcqYear);
	GetControlData(_T("CM_정리_마크반입_구분코드"), _T("BB_원부번호")		, strAccessionRecNo);

	//원부번호 체크		
	sQuery.Format(_T("SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR='%s' AND ACCESSION_REC_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear, strAccessionRecNo );
	CString sResult;	//시작 등록번호
	m_pDM_Species->GetOneValueQuery( sQuery , sResult );
	if ( sResult != _T("") ) {
		m_strAccessionRecKey = sResult;		
		return 0;
	}


	sREGISTRANT = this->GetUserID();

	CTime t = CTime::GetCurrentTime();
	sREG_DATE.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	
	//rec_key 만들기
	CString sRecKey;	
	INT ids = m_pDM_Species->MakeRecKey( sRecKey );
	if ( 0 > ids ) return ids;
	ids = m_pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return ids;
	}

	m_strAccessionRecKey = sRecKey;
	m_pDM_Species->InitDBFieldData();
	m_pDM_Species->AddDBFieldData(_T("REC_KEY")					, _T("STRING"), sRecKey);
	m_pDM_Species->AddDBFieldData(_T("ACCESSION_REC_MAKE_YEAR")	, _T("STRING"), strAcqYear);
	m_pDM_Species->AddDBFieldData(_T("ACCESSION_REC_NO")		, _T("STRING"), strAccessionRecNo);
	m_pDM_Species->AddDBFieldData(_T("PUBLISH_FORM_CODE")		, _T("STRING"), _T("MO"));
	m_pDM_Species->AddDBFieldData(_T("ACQUISIT_CODE")			, _T("STRING"), strAcqCode);
	m_pDM_Species->AddDBFieldData(_T("ACCESSION_REC_MAKER")		, _T("STRING"), sREGISTRANT);
	m_pDM_Species->AddDBFieldData(_T("REG_DATE")				, _T("STRING"), sREG_DATE);
	m_pDM_Species->AddDBFieldData(_T("MANAGE_CODE")				, _T("STRING"), _T("UDF_MANAGE_CODE") );

	ids = m_pDM_Species->MakeInsertFrame(_T("CO_ACCESSION_REC_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
		return ids;
	}
	
	// 마지막번호 +1 을 그대로 사용하면 UPDATE
	// 마지막 번호+1보다 작을 경우 존재한다고 본다.
	if (m_nAccNum == _ttoi(strAccessionRecNo) )
	{		
		sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = 'ACC_NO' AND MANAGE_CODE = UDF_MANAGE_CODE;") , strAccessionRecNo, strAcqYear );
		INT iCh = m_pDM_Species->AddFrame( sQuery );
		if ( 0 > iCh ) return -100;
	}

	ids = m_pDM_Species->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -100;
	}
	ids = m_pDM_Species->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return -100;
	}	

	return 0;
	EFS_END
	return -1;
}

BOOL CMarcImportDlg::CheckAccessionRecNo()
{
	CString strAccessionRecNo;
	INT	nAccNum;
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck == FALSE ) return FALSE;

	GetControlData( _T("CM_정리_마크반입_구분코드") , _T("BB_원부번호") , strAccessionRecNo);
	nAccNum = _ttoi(strAccessionRecNo.GetBuffer(0));
	if ( nAccNum > m_nAccNum )
	{		
		AfxMessageBox(_T("마지막 원부번호 +1 보다 큰 원부번호는 사용할 수 없습니다."));		
		return FALSE;
	} 
	else if ( nAccNum <= 0 ) 
	{	
		AfxMessageBox(_T("사용할 수 없는 원부번호 입니다."));		
		return FALSE;
	}	

	return TRUE;
}



void CMarcImportDlg::OnKillfocuseAccessionRecNo() 
{
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck == FALSE ) return;

	if ( CheckAccessionRecNo() == FALSE )
	{
		( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->SetCheck(FALSE);		
	}
}

//JOB.2019.0003 ADD BY KYJ : 수입년도변경시 원부번호가 변경안되는 현상 수정 
//====================================================================================================================================================
VOID CMarcImportDlg::OnKillfocuseAccessionYear()
{
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck == FALSE ) return;
	
	CString strQuery;
	CString strAcqYear;
	CString strAccessionRecNo;
	INT		nAccNum;
	
	GetControlData( _T("CM_정리_마크반입_구분코드"), _T("BS_수입년도"), strAcqYear );
	
	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'ACC_NO' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear );
	CString sResult;
	m_pDM_Main->GetOneValueQuery( strQuery , sResult );
	
	nAccNum = _ttoi(sResult.GetBuffer(0));
	m_nAccNum = ++nAccNum;	
	strAccessionRecNo.Format( _T("%d") , nAccNum );
	SetControlData( _T("CM_정리_마크반입_구분코드") , _T("BB_원부번호") , strAccessionRecNo);
	
}
//====================================================================================================================================================

HBRUSH CMarcImportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}


//===================================================
//2009.03.10 ADD BY PJW : 그리드를 display한후 순위를 셋팅한다.
INT CMarcImportDlg::DisplayGrid()
{
	m_pGrid_OK->Display();
	m_pGrid_ERR->Display();	
	m_pGrid_SUCCESS->Display();	

	SetGridRank();
	
	return 0 ;
}
//===================================================

//===================================================
//2009.03.10 ADD BY PJW : 그리드에 순위를 셋팅한다.
INT CMarcImportDlg::SetGridRank()
{
	INT nRowCnt;
	INT i;
	CString strData;

 	nRowCnt = m_pDmGridOk->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridOk->GetCellData(_T("순위"),i);
		m_pGrid_OK->SetTextMatrix( i+1, 0, strData);
	}

	nRowCnt = m_pDmGridError->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridError->GetCellData(_T("순위"),i);
		m_pGrid_ERR->SetTextMatrix( i+1, 0, strData);
	}

	nRowCnt = m_pDmGridSuccess->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridSuccess->GetCellData(_T("순위"),i);
		m_pGrid_SUCCESS->SetTextMatrix( i+1, 0, strData);
	}

	return 0 ;
}

//===================================================

//===================================================
//2009.03.17 UPDATE BY PJW : 그리드를 순위에 따라 정렬한다.
INT CMarcImportDlg::SortGrid()
{
//	INT nRowCnt;
//	CString strData;
//
// 	nRowCnt = m_pDmGridOk->GetRowCount();

	m_pDmGridOk->SORT(_T("순위"));
	m_pDmGridError->SORT(_T("순위"));
	m_pDmGridSuccess->SORT(_T("순위"));
//	SortDm( m_pDmGridOk );
//	SortDm( m_pDmGridError );
//	SortDm( m_pDmGridSuccess );

	return 0 ;
}
//===================================================

//===================================================
//2009.03.17 UPDATE BY PJW : 그리드를 순위에 따라 정렬한다.
INT CMarcImportDlg::SortDm( CESL_DataMgr * pSortDm )
{
	BOOL bChkSwap;

	INT nRowCnt;
	INT i;

	CString strData1;
	CString strData2;
	
	nRowCnt = pSortDm->GetRowCount();
	while(1)
	{
		bChkSwap = FALSE;
		for( i = 0 ; i < nRowCnt-1 ; i++ )
		{
			strData1 = pSortDm->GetCellData( _T("순위"), i );		
			strData2 = pSortDm->GetCellData( _T("순위"), i+1 );
			if( _ttoi(strData1) > _ttoi(strData2) )
			{
				pSortDm->SORT_SwapRow( i , i + 1 );
				bChkSwap = TRUE;
			}
		}

		if( bChkSwap == FALSE )
		{
			break;
		}
	}
	
	return 0 ;
}
//===================================================



//===================================================
//2009.03.10 ADD BY PJW : 

INT CMarcImportDlg::MsgSortGrid()
{
	SortGrid();
	DisplayGrid();

	return 0 ;
}

INT CMarcImportDlg::MsgDisplayGrid()
{
	DisplayGrid();
	
	return 0 ;
}

//===================================================

void CMarcImportDlg::OnbNORMALIZATION() 
{
	EFS_BEGIN

	INT nDmColCount		= 0;
	INT nIdx			= 0;
	INT nSelectCnt		= 0;
	INT nOkDmRowCnt		= 0;
	INT nErrDmRowCnt	= 0;
	INT nRecovers		= 0;

	CString strMsg;
	CString strData;

 	BeginWaitCursor();

	nDmColCount = m_pDmGridError->GetRefColumnCount();
	nErrDmRowCnt = m_pDmGridError->GetRowCount();
	m_pGrid_ERR->SelectMakeList();
	nSelectCnt = m_pGrid_ERR->SelectGetCount();
	
	if( 0 < nSelectCnt )
	{
		nIdx = m_pGrid_ERR->SelectGetTailPosition();

		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			m_pDmGridError->GetCellData(_T("BS_파일반입오류"), nIdx, strData);

			if( strData.IsEmpty() )
			{
				nOkDmRowCnt = m_pDmGridOk->GetRowCount();
				m_pDmGridOk->InsertRow(-1);
				
				for( INT j = 0 ; j < nDmColCount ; j++ )
				{
					strData = m_pDmGridError->GetCellData( nIdx, j );
					m_pDmGridOk->SetCellData( nOkDmRowCnt, j, strData );					
				}

				m_pDmGridError->DeleteRow( nIdx );

				// 성공 개수 설정
//				strData = m_wndStatusBarCtrl.GetText(1);
//				strData = strData.Mid(strData.ReverseFind(':') + 1);
//				strData.Format(_T("%d"), _ttoi(strData) + 1);
//				m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
//				
//				strData = m_wndStatusBarCtrl.GetText(2);
//				strData = strData.Mid(strData.ReverseFind(':') + 1);
//				strData.Format(_T("%d"), _ttoi(strData) - 1);
//				m_wndStatusBarCtrl.SetText(_T("오류 : ") + strData, 2, 0);

				nRecovers++;
			}

			nIdx = m_pGrid_ERR->SelectGetPrev();
		}
	}
	else
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

 	EndWaitCursor();

	if( nRecovers > 0 )
	{
		INT nRowCnt;
		nRowCnt = m_pDmGridOk->GetRowCount();
		strData.Format( _T("%d"), nRowCnt );
		m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
				
		nRowCnt = m_pDmGridError->GetRowCount();
		strData.Format( _T("%d"), nRowCnt );
		m_wndStatusBarCtrl.SetText(_T("오류 : ") + strData, 2, 0);

		strMsg.Format(_T("[ %d ]건 중 [ %d ] 건이 복귀했습니다."), nErrDmRowCnt, nRecovers);
		AfxMessageBox(strMsg);
// 		m_pDmGridOk->SORT(_T("순위"));
		SortDm(m_pDmGridOk);
		DisplayGrid();
	}
	else
	{
		strMsg.Format(_T("오류 수정된 자료가 없습니다."));
		AfxMessageBox(strMsg);	
	}

	return ;
	EFS_END
	
}
