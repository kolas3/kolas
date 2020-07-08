// BO_HOLDING_ALL_DUP_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "BO_HOLDING_ALL_DUP_SEARCH.h"
#include "BO_Request_Duplicate_DetailInfo_Dlg.h"
#include "../BO_ACQ_API/BO_ACQ_Business_Api.h"
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "_excelmancontrol.h"
#include "ExcelManager.h"

/////////////////////////////////////////////////////////////////////////////
// CBO_HOLDING_ALL_DUP_SEARCH dialog


CBO_HOLDING_ALL_DUP_SEARCH::CBO_HOLDING_ALL_DUP_SEARCH(CESL_Mgr* pParent, INT DuplicateSearchType)// /*=NULL*/)
	: CESL_Mgr(CBO_HOLDING_ALL_DUP_SEARCH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_HOLDING_ALL_DUP_SEARCH)
	m_chkAuthor = FALSE;
	m_chkISBN = FALSE;
	m_chkMedClass = FALSE;
	m_chkPubYear = FALSE;
	m_chkPublisher = FALSE;
	m_chkRegCode = FALSE;
	m_chkShelfCode = FALSE;
	m_chkTwice = FALSE;
	m_chkVolTitle = FALSE;
	m_chkTitle = FALSE;
	m_chkVol = FALSE;
	m_bIncludeDiscardedBook = FALSE;
	m_radAuthor = -1;
	m_radPublisher = -1;
	m_radTitle = -1;
	m_radVolTitle = -1;
	m_radISBN = -1;
	m_radMedCode = -1;
	m_radPubYear = -1;
	m_radRegCode = -1;
	m_radShelfCode = -1;
	m_radVol = -1;
	m_chkSeriesTitle = FALSE;
	//}}AFX_DATA_INIT
	m_DuplicateSearchType = DuplicateSearchType;
	m_isDeleteOperation = FALSE;
	m_pArrAcqKey = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDupMgr = NULL;
	m_pMenu = NULL;
	m_nSortDirection = ESL_DATAMGR_SORT_ASC;
	//==============================
	//2008.08.06 ADD BY PJW : 설정값 저장파일의 패스를 지정한다.
	m_FileName.Format(_T(".\\duplicate_search_key.txt"));
	m_SearchKey.SetSearchKeys(m_FileName);
	//m_FileName = filename;
	m_bInit = FALSE;
	//==============================

	m_pGroup = NULL;
	m_pGroupCollection = NULL;
}

CBO_HOLDING_ALL_DUP_SEARCH::~CBO_HOLDING_ALL_DUP_SEARCH()
{
	if(m_pDupMgr)
	{
		delete m_pDupMgr;
		m_pDupMgr = NULL;
	}

	FreeMenu();
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_HOLDING_ALL_DUP_SEARCH)
	DDX_Check(pDX, IDC_chkAUTHOR1, m_chkAuthor);
	DDX_Check(pDX, IDC_chkISBN1, m_chkISBN);
	DDX_Check(pDX, IDC_chkMED_CLASS1, m_chkMedClass);
	DDX_Check(pDX, IDC_chkPUBLICATE_YEAR1, m_chkPubYear);
	DDX_Check(pDX, IDC_chkPUBLISHER1, m_chkPublisher);
	DDX_Check(pDX, IDC_chkREG_CODE1, m_chkRegCode);
	DDX_Check(pDX, IDC_chkSEPARATE_SHELF_CODE1, m_chkShelfCode);
	DDX_Check(pDX, IDC_chkTITLE_TWICE_SEARCH1, m_chkTwice);
	DDX_Check(pDX, IDC_chkVOL_TITLE1, m_chkVolTitle);
	DDX_Check(pDX, IDC_chkTITLE1, m_chkTitle);
	DDX_Check(pDX, IDC_chkVOL1, m_chkVol);
	DDX_Check(pDX, IDC_chkDiscardBook1, m_bIncludeDiscardedBook);
	DDX_Radio(pDX, IDC_radAUTHOR_RIGHTTRUNCATION1, m_radAuthor);
	DDX_Radio(pDX, IDC_radPUBLISHER_RIGHTTRUNCATION1, m_radPublisher);
	DDX_Radio(pDX, IDC_radTITLE_RIGHTTRUNCATION1, m_radTitle);
	DDX_Radio(pDX, IDC_radVOL_TITLE_RIGHTTRUNCATION1, m_radVolTitle);
	DDX_Radio(pDX, IDC_radISBN_EQUIVALENCE1, m_radISBN);
	DDX_Radio(pDX, IDC_radMEDIACODE_EQUIVALENCE1, m_radMedCode);
	DDX_Radio(pDX, IDC_radPUBLICATE_YEAR_EQUIVALENCE1, m_radPubYear);
	DDX_Radio(pDX, IDC_radREGCODE_EQUIVALENCE1, m_radRegCode);
	DDX_Radio(pDX, IDC_radSEPARATECODE_EQUIVALENCE1, m_radShelfCode);
	DDX_Radio(pDX, IDC_radVOL_EQUIVALENCE1, m_radVol);
	DDX_Check(pDX, IDC_chkSeriesTitle1, m_chkSeriesTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_HOLDING_ALL_DUP_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CBO_HOLDING_ALL_DUP_SEARCH)
	ON_BN_CLICKED(IDC_btnDETAIL, OnbtnDETAIL)
	ON_BN_CLICKED(IDC_btnPRINT, OnbtnPRINT)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnCOPY_DEL, OnbtnCOPYDEL)
	ON_BN_CLICKED(IDC_btnSELSOURCEDATA, OnbtnSELSOURCEDATA)
	ON_BN_CLICKED(IDC_btnAPPLY, OnbtnAPPLY)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnExportHoldingMarc, OnbtnExportHoldingMarc)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkAUTHOR1, OnchkAUTHOR1)
	ON_BN_CLICKED(IDC_chkDiscardBook1, OnchkDiscardBook1)
	ON_BN_CLICKED(IDC_chkISBN1, OnchkISBN1)
	ON_BN_CLICKED(IDC_chkMED_CLASS1, OnchkMEDCLASS1)
	ON_BN_CLICKED(IDC_chkPUBLICATE_YEAR1, OnchkPUBLICATEYEAR1)
	ON_BN_CLICKED(IDC_chkPUBLISHER1, OnchkPUBLISHER1)
	ON_BN_CLICKED(IDC_chkREG_CODE1, OnchkREGCODE1)
	ON_BN_CLICKED(IDC_chkSEPARATE_SHELF_CODE1, OnchkSEPARATESHELFCODE1)
	ON_BN_CLICKED(IDC_chkTITLE_TWICE_SEARCH1, OnchkTITLETWICESEARCH1)
	ON_BN_CLICKED(IDC_chkTITLE1, OnchkTITLE1)
	ON_BN_CLICKED(IDC_chkVOL_TITLE1, OnchkVOLTITLE1)
	ON_BN_CLICKED(IDC_chkVOL1, OnchkVOL1)
	ON_BN_CLICKED(IDC_radAUTHOR_EQUIVALENCE1, OnradAUTHOREQUIVALENCE1)
	ON_BN_CLICKED(IDC_radAUTHOR_RIGHTTRUNCATION1, OnradAUTHORRIGHTTRUNCATION1)
	ON_BN_CLICKED(IDC_radPUBLISHER_EQUIVALENCE1, OnradPUBLISHEREQUIVALENCE1)
	ON_BN_CLICKED(IDC_radPUBLISHER_RIGHTTRUNCATION1, OnradPUBLISHERRIGHTTRUNCATION1)
	ON_BN_CLICKED(IDC_radTITLE_CUTSEARCH1, OnradTITLECUTSEARCH1)
	ON_BN_CLICKED(IDC_radTITLE_EQUIVALENCE1, OnradTITLEEQUIVALENCE1)
	ON_BN_CLICKED(IDC_radTITLE_RIGHTTRUNCATION1, OnradTITLERIGHTTRUNCATION1)
	ON_BN_CLICKED(IDC_radVOL_TITLE_EQUIVALENCE1, OnradVOLTITLEEQUIVALENCE1)
	ON_BN_CLICKED(IDC_radVOL_TITLE_RIGHTTRUNCATION1, OnradVOLTITLERIGHTTRUNCATION1)
	ON_BN_CLICKED(IDC_btnDUP_FIND_START, OnbtnDUPFINDSTART)
	ON_BN_CLICKED(IDC_radVOL_EQUIVALENCE1, OnradVOLEQUIVALENCE1)
	ON_BN_CLICKED(IDC_radSEPARATECODE_EQUIVALENCE1, OnradSEPARATECODEEQUIVALENCE1)
	ON_BN_CLICKED(IDC_radREGCODE_EQUIVALENCE1, OnradREGCODEEQUIVALENCE1)
	ON_BN_CLICKED(IDC_radPUBLICATE_YEAR_EQUIVALENCE1, OnradPUBLICATEYEAREQUIVALENCE1)
	ON_BN_CLICKED(IDC_radMEDIACODE_EQUIVALENCE1, OnradMEDIACODEEQUIVALENCE1)
	ON_BN_CLICKED(IDC_radISBN_EQUIVALENCE1, OnradISBNEQUIVALENCE1)
	ON_BN_CLICKED(IDC_radALLWORDSEARCH1, OnradALLWORDSEARCH1)
	ON_MESSAGE(SOURCE_DATA_MERGE, SourceDataMerge)
	ON_BN_CLICKED(IDC_chkSeriesTitle1, OnchkSeriesTitle1)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(5000,5003,OnMSGBOKBONSEARCH)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_HOLDING_ALL_DUP_SEARCH message handlers

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnbtnDETAIL() 
{
	// TODO: Add your control notification handler code here
	ShowDetailInfo(SUPDATE);
	return;	
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnbtnPRINT() 
{
	// TODO: Add your control notification handler code here
	CString strTmpData;
	//20070727 ADD BY PJW : 출력목록에 복본조사수식추가한 변수 : strDupFUNC, strTempDupFUNC 
	CString strDupFUNC, strTempDupFUNC;

	CBasicPrintMgr printMgr(this);
	printMgr.SetPrintDM(FindDM(_T("DM_중복본조사")));
	printMgr.SetSIReportFilename(_T("K2UP_복본자료목록"));
	printMgr.InitPrintData();

	//20070727 ADD BY PJW : 출력목록에 복본조사수식추가한 변수 세팅
	GetDlgItem(IDC_dupFUNC1)->GetWindowText(strTempDupFUNC);
	strDupFUNC.Format(_T("= %s"),strTempDupFUNC);

	if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_DIVNO)
	{
		printMgr.AddPrintData(_T("제목"), _T("차수단위복본조사목록"));
		if(!m_strAcqYear.IsEmpty())
		{
			strTmpData.Format(_T("수입년도 : %s"), m_strAcqYear);
			printMgr.AddPrintData(_T("수입년도"), strTmpData);
		}
		if(!m_strGroupInfo.IsEmpty())
		{
			strTmpData.Format(_T("차수번호 : %s"), m_strGroupInfo);
			printMgr.AddPrintData(_T("차수번호"), strTmpData);
		}
		//20070727 ADD BY PJW : 출력에 복본조사식 추가  
		printMgr.AddPrintData(_T("복본조사식"), strDupFUNC);
	}
	else if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH)
	{
		printMgr.AddPrintData(_T("제목"), _T("복본조사목록"));
		
		//20070727 ADD BY PJW : 출력에 복본조사식 추가  
		printMgr.AddPrintData(_T("복본조사식"), strDupFUNC);
	}
	else if(m_DuplicateSearchType == CAT_DUPLICATE_SEARCH)
	{
		printMgr.AddPrintData(_T("제목"), _T("정리복본조사목록"));

		//20070727 ADD BY PJW : 출력에 복본조사식 추가  
		printMgr.AddPrintData(_T("복본조사식"), strDupFUNC);
	}
	else if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_BASIC_DATA)
	{
		printMgr.AddPrintData(_T("제목"), _T("기초자료복본조사목록"));

		//20070727 ADD BY PJW : 출력에 복본조사식 추가  
		printMgr.AddPrintData(_T("복본조사식"), strDupFUNC);
	}
	else if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_IMPORT_DATA)
	{
		printMgr.AddPrintData(_T("제목"), _T("반입자료복본조사목록"));

		//20070727 ADD BY PJW : 출력에 복본조사식 추가  
		printMgr.AddPrintData(_T("복본조사식"), strDupFUNC);
	}
	else if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_REQUEST_DATA)
	{
		printMgr.AddPrintData(_T("제목"), _T("비치희망자료복본조사목록"));

		//20070727 ADD BY PJW : 출력에 복본조사식 추가  
		printMgr.AddPrintData(_T("복본조사식"), strDupFUNC);
	}
	else if(m_DuplicateSearchType == TITLE_DUPLICATE_SEARCH)
		printMgr.AddPrintData(_T("제목"), _T("서명복본조사목록"));
	else if(m_DuplicateSearchType == TITLE_AUTO_DUPLICATE_SEARCH)
		printMgr.AddPrintData(_T("제목"), _T("서명복본조사목록"));
	else if(m_DuplicateSearchType == CAT_TITLE_DUPLICATE_SEARCH)
		printMgr.AddPrintData(_T("제목"), _T("서명복본조사목록"));
	else if(m_DuplicateSearchType == FURNISH_DATA_DUPLICATE_SEARCH)
		printMgr.AddPrintData(_T("제목"), _T("서명복본조사목록"));
	else if(m_DuplicateSearchType == SERIAL_TITLE_DUPLICATE_SEARCH)
		printMgr.AddPrintData(_T("제목"), _T("서명복본조사목록"));	
	
	printMgr.Print();
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnCancel();
}

BOOL CBO_HOLDING_ALL_DUP_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UP_중복본조사")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	GetWindowRect(&m_rcInitial);
	if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH || 
		m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_DIVNO)
	{
		GetDlgItem(IDC_btnAPPLY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridINPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridSE_INPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		m_strCMAlias = _T("CM_복본조사");
		m_strDMAlias = _T("DM_중복본조사");
	}
	else if(m_DuplicateSearchType == TITLE_DUPLICATE_SEARCH || 
		m_DuplicateSearchType == CAT_TITLE_DUPLICATE_SEARCH ||
		m_DuplicateSearchType == TITLE_AUTO_DUPLICATE_SEARCH ||
		m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_INDIVIDUAL_DATA)
	{
		if(m_DuplicateSearchType == CAT_TITLE_DUPLICATE_SEARCH)
			GetDlgItem(IDC_btnAPPLY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSELSOURCEDATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCOPY_DEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridBOKBON_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridSE_INPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		//2007.07.26 ADD PJW : 서명복본조사 시 복존조사수식 제외 
		//GetDlgItem(IDC_dupFUNC1)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_dupFUNC1)->SetWindowText(dupFUNC1);

		m_strCMAlias = _T("CM_서명복본조사");
		m_strDMAlias = _T("DM_중복본조사");
	}
	else if(m_DuplicateSearchType == CAT_DUPLICATE_SEARCH)
	{
		GetDlgItem(IDC_btnAPPLY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCOPY_DEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridINPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridSE_INPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		m_strCMAlias = _T("CM_복본조사");
		m_strDMAlias = _T("DM_중복본조사");
	}
	else if(m_DuplicateSearchType == FURNISH_DATA_DUPLICATE_SEARCH)
	{
		GetDlgItem(IDC_btnAPPLY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSELSOURCEDATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCOPY_DEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridBOKBON_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridSE_INPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		//2007.07.26 ADD PJW : 서명복본조사 시 복존조사수식 제외
		//GetDlgItem(IDC_dupFUNC1)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_dupFUNC1)->SetWindowText(dupFUNC1);

		m_strCMAlias = _T("CM_서명복본조사");
		m_strDMAlias = _T("DM_중복본조사");
	}                               
	else if(m_DuplicateSearchType == SERIAL_TITLE_DUPLICATE_SEARCH)
	{
		GetDlgItem(IDC_btnSELSOURCEDATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCOPY_DEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridINPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridBOKBON_SEARCH)->ShowWindow(SW_HIDE);
		m_strCMAlias = _T("CM_연속서명복본조사");
		m_strDMAlias = _T("DM_연속중복본조사");
	}
	else if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_BASIC_DATA 
		||	m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_IMPORT_DATA)
	{
		GetDlgItem(IDC_btnAPPLY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridINPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridSE_INPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_btnSELSOURCEDATA)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_btnCOPY_DEL)->ShowWindow(SW_HIDE);		
		m_strCMAlias = _T("CM_복본조사");
		m_strDMAlias = _T("DM_중복본조사");
	}
	else if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_REQUEST_DATA)
	{
		GetDlgItem(IDC_btnAPPLY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridINPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridSE_INPUT_BOKBON_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSELSOURCEDATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCOPY_DEL)->ShowWindow(SW_HIDE);		
		m_strCMAlias = _T("CM_복본조사");
		m_strDMAlias = _T("DM_중복본조사");
	}

	m_pCM = FindCM(m_strCMAlias);
	m_pDM = FindDM(m_strDMAlias);
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid2"));
	
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid->SetSort(FALSE);
	m_pGrid->m_bFroceReverse = FALSE;
	m_pDupMgr = new CDuplicateSearchMgr(this);

	// 출판유통 반출 버튼
	if ( GetBookTradeInfo() == TRUE )
	{
		( (CButton*) GetDlgItem(IDC_btnExportHoldingMarc) )->ShowWindow(SW_SHOW);
	}

	// 0.1 초후에 Grid display 시작
	SetTimer(0, 100, NULL);		

	//2007.06.27 ADD PJW : 복본조사식 VIEW 
	CString dupFUNC;
	CString tempDupKEY;
	CString dupKEY;
	CString tempDupFUNC;
	CStdioFile file;		
	CString strLine;
	CString filename = _T(".\\duplicate_search_key.txt");
	CString filename_1 = _T(".\\duplicate_search_key2.txt");
	INT firstIdx, lastIdx;
	//서명복본조사를 위한 변수
/*	CString breakFlag;
	CString dupFUNC1;
	CString tempDupFUNC1;
	INT breakCNT = 0;
*/


	dupFUNC.Empty();
	tempDupKEY.Empty();
	dupKEY.Empty();
	tempDupFUNC.Empty();
/*	breakFlag.Empty();
	dupFUNC1.Empty();
	tempDupFUNC1.Empty();
*/
	if ( !file.Open(filename, CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("복본조사 설정키가 완전하지 않습니다."));
		return FALSE;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while( file.ReadString(strLine) )
	{
		lastIdx = 0;
		firstIdx = strLine.Find('[');
		lastIdx = strLine.Find(']');

		if(lastIdx > 1)
		{
			tempDupKEY = strLine.Mid(firstIdx +1, lastIdx-1);
			dupKEY.Format(_T("%s :"),tempDupKEY);
			//dupKEY.Format(_T("%s"),tempDupKEY);
			//breakFlag.Format(_T("%s"),tempDupKEY);
		}

		/*if( breakFlag != _T("본표제") && breakCNT == 0)
		{
			tempDupFUNC1.Format(_T("%s"), tempDupFUNC);
			breakCNT++;
		}*/

		if( dupKEY == _T("본표제 :") )
		{
			dupKEY.Format(_T("서명 :"));
		}
		//===================================================
		//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
		else if( dupKEY == _T("총서명 :") )
		{
			dupKEY.Format(_T("총서명 :"));
		}
		//===================================================
		

		if(dupKEY == _T("낱권ISBN :"))
		{
			dupKEY.Format(_T("ISBN :"));
		}

		if(dupKEY == _T("편권차 :"))
		{
			dupKEY.Format(_T("편/권차 :"));
		}
		
		//======================================
		//20080630 UPDATE BY PJW : ==를 FIND로 바꾼다.
		//if(strLine == _T("SEARCH_TYPE=0"))
		if( 0 <= strLine.Find( _T("SEARCH_TYPE=0") ) )
		//======================================	
		{
			tempDupKEY = dupKEY;
			dupKEY.Format(_T("%s 우절단검색"), tempDupKEY);				
		}
		//======================================
		//20080630 UPDATE BY PJW : ==를 FIND로 바꾼다.
		//else if(strLine == _T("SEARCH_TYPE=1"))
		else if( 0 <= strLine.Find( _T("SEARCH_TYPE=1") ) )
		//======================================	
		{
			tempDupKEY = dupKEY;
			dupKEY.Format(_T("%s 일치검색"), tempDupKEY);
					
		}
		//======================================
		//20080630 UPDATE BY PJW : ==를 FIND로 바꾼다.
		//else if(strLine == _T("SEARCH_TYPE=2"))
		else if( 0 <= strLine.Find( _T("SEARCH_TYPE=2") ) )
		//======================================	
		{
			tempDupKEY = dupKEY;
			dupKEY.Format(_T("%s, 절취검색"), tempDupKEY);					
		}
		//======================================
		//20080630 UPDATE BY PJW : ==를 FIND로 바꾼다.
		//if(strLine == _T("ARTICLE_SEARCH_YN=1"))	
		if( 0 <= strLine.Find( _T("ARTICLE_SEARCH_YN=1") ) )
		//======================================
		{
			tempDupKEY = dupKEY;
			dupKEY.Format(_T("%s, 관칭사용"), tempDupKEY);				
		}

		//======================================
		//20080630 UPDATE BY PJW : ==를 FIND로 바꾼다.
		//if(strLine == _T("USE_YN=1"))
		if( 0 <= strLine.Find( _T("USE_YN=1") ) )
		//======================================
		{
			if(tempDupFUNC == _T(""))
			{
				dupFUNC.Format(_T("(%s)"), dupKEY);						
			}else
			{
				dupFUNC.Format(_T("%s AND (%s)"), tempDupFUNC, dupKEY);		
			}
			tempDupFUNC.Format(_T("%s"), dupFUNC);				
		}
	}
	file.Close();

	//2007.07.05 ADD PJW : 제적자료 제외 여부추가
	if ( !file.Open (filename_1, CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("복본조사 설정키가 완전하지 않습니다."));
		return FALSE;
	}
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while( file.ReadString(strLine) )
	{
		lastIdx = 0;
		firstIdx = strLine.Find('[');
		lastIdx = strLine.Find(']');

		if (lastIdx > 1)
		{
			dupKEY = strLine.Mid(firstIdx +1, lastIdx-1);
		}

		if (dupKEY == _T("제적자료포함여부") && strLine == _T("USE_YN=1"))
		{
			if (tempDupFUNC == _T(""))
			{
				dupFUNC.Format(_T("(제적자료제외)"), dupKEY);
			}
			else
			{
				dupFUNC.Format(_T("%s AND (제적자료제외)"), tempDupFUNC);
			}
			
			tempDupFUNC.Format(_T("%s"), dupFUNC);	

			/*if (tempDupFUNC1 == _T(""))
			{
				dupFUNC1.Format(_T("(제적자료제외)"), dupKEY);
			}
			else
			{
				dupFUNC1.Format(_T("%s AND (제적자료제외)"), tempDupFUNC1);
			}

			tempDupFUNC1.Format(_T("%s"), dupFUNC1);	
			*/
		}
		dupFUNC.Format(_T("%s"),tempDupFUNC);
		GetDlgItem(IDC_dupFUNC1)->SetWindowText(dupFUNC);
	}
	file.Close();

	dupFUNC.Format(_T("%s"), tempDupFUNC);

	//=====================================================
	//2008.11.18 ADD BY PJW : 서명복본조사일경우 조사실행버튼을 숨긴다.
	CButton* pBtn = (CButton*) GetDlgItem(IDC_btnDUP_FIND_START);
	//=====================================================

//	dupFUNC1.Format(_T("%s"),tempDupFUNC1);
	if( m_DuplicateSearchType == TITLE_DUPLICATE_SEARCH || 
		m_DuplicateSearchType == CAT_TITLE_DUPLICATE_SEARCH ||
		m_DuplicateSearchType == TITLE_AUTO_DUPLICATE_SEARCH ||
		m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_INDIVIDUAL_DATA ||
		m_DuplicateSearchType == FURNISH_DATA_DUPLICATE_SEARCH )
	{
		//GetDlgItem(IDC_dupFUNC1)->SetWindowText(dupFUNC1);
		GetDlgItem(IDC_dupFUNC1)->SetWindowText(_T("(서명 : 우절단검색)"));

		//=====================================================
		//2008.11.18 ADD BY PJW : 서명복본조사일경우 조사실행버튼을 숨긴다.
		pBtn->ShowWindow(SW_HIDE);
		//=====================================================
	}
	else
	{
		GetDlgItem(IDC_dupFUNC1)->SetWindowText(dupFUNC);	
		//=====================================================
		//2008.11.18 ADD BY PJW : 서명복본조사일경우 조사실행버튼을 숨긴다.
		pBtn->ShowWindow(SW_SHOW);
		//=====================================================
		
	}

	//========================================================================
	//2008.08.26 ADD BY PJW : 복본조사 설정값을 초기화 한다.
	InitDupKeySetting();
	//========================================================================
	
	// 2007.09.03 ADD BY PDJ
	// 서명복본조사 일경우 소장종이면 못하도록 한다.
	if ( m_DuplicateSearchType == TITLE_DUPLICATE_SEARCH )
	{		
		if ( m_strSourceSpeciesClass == _T("1") ) 
		{
			( (CButton*) GetDlgItem (IDC_btnAPPLY) )->ShowWindow(SW_HIDE);
		}
	}
	//===================================
	//20080603 ADD BY PJW : 반입자료관리에서는 수입구분체크박스를 선정할수 없도록 한다.
	CheckApplyBtnShow(_T("DM_반입자료관리"));
	CheckApplyBtnShow(_T("DM_구입자료관리"));
	CheckApplyBtnShow(_T("DM_주문자료관리"));
	CheckApplyBtnShow(_T("DM_기증접수자료관리"));
	CheckApplyBtnShow(_T("DM_단행_정리_등록_메인"));
	//===================================

	PostMessage(WM_SIZE);	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_HOLDING_ALL_DUP_SEARCH::RefreshDMandShowList(VOID)
{
	//정렬하자
	//20070830 DELETE PJW : 처음 복본 디스플래이 할때 정렬하지 않는다. SortDuplicateData(NULL); 제외
    //SortDuplicateData(NULL);
	DisplayGrid();
	
	return 0;
}

INT CBO_HOLDING_ALL_DUP_SEARCH::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData, strCallNumber, strRegNo, strAcqInfo;

	//관리구분 설정
	strTmpData = m_pDM->GetCellData(_T("IBS_관리구분"), nRow);
	strTmpData.TrimLeft();	strTmpData.TrimRight();
	INT nPos = strTmpData.Find(_T(" "), 0);
	if(nPos >= 0)
	{
		strTmpData = strTmpData.Left(nPos);
		m_pDM->SetCellData(_T("IBS_관리구분"), strTmpData, nRow);
	}
	
	if(strDMFieldAlias == _T("관리번호"))
	{
		strTmpData = m_pDM->GetCellData(_T("UDF_자료상태설명"), nRow);
		GetManageNumber(nRow, strCallNumber, strRegNo, strAcqInfo);
		//===================================================
		//2010.01.05 ADD BY PJW : 
		//2008.02.20 UPDATE BY PJW : 관외대출자료도 청구기호를 보여주도록 수정
		//2008.05.06 UPDATE BY PJW : 수리제본자료도 청구기호를 보여주도록 수정
		//2010.01.14 UPDATE BY PJW : 열람인계자료, 분실자료, 제적자료 추가함
		//if(strTmpData.Find(_T("소장자료")) >= 0 )		
		if(	strTmpData.Find(_T("소장자료")) >= 0		|| 
			strTmpData.Find(_T("관외대출자료")) >= 0	|| 
			strTmpData.Find(_T("수리제본")) >= 0		|| 
			strTmpData.Find(_T("열람인계자료")) >= 0	|| 
			strTmpData.Find(_T("제적자료")) >= 0	|| 
			strTmpData.Find(_T("분실자료")) >= 0		||
			strTmpData.Find(_T("특별대출자료")) >= 0	||
			// 18.09.27 JMJ KOL.RED.2018.008
			strTmpData == _T("단행책두레업무진행자료")
			)
		//===================================================
		{
			if(!strCallNumber.IsEmpty()) str = strCallNumber;
			else if(!strRegNo.IsEmpty()) str = strRegNo;
			else str = strAcqInfo;
		}
		else if(strTmpData == _T("정리자료"))
		{
			if(!strRegNo.IsEmpty()) str = strRegNo;
			else str = strAcqInfo;
		}
		else str = strAcqInfo;
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
		strTmpData = m_pDM->GetCellData(_T("IBS_업무구분"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("업무구분"), strTmpData, strTmpData);
		m_pDM->SetCellData(_T("UDF_업무구분"), strTmpData, nRow);
	}	
	else if(strDMFieldAlias == _T("식별번호"))
	{
		if(str != _T("**"))
		{
			SetGridRowBackColor(m_pGrid, nRow);
		}
	}
	return 0;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnTimer(UINT nIDEvent) 
{
	INT ids;
	if (nIDEvent == 0) {
		KillTimer(0);
		ids = RefreshDMandShowList();
	}
	CDialog::OnTimer(nIDEvent);
}

INT CBO_HOLDING_ALL_DUP_SEARCH::CheckSelectDataValid()
{
	CESL_DataMgr * pDM = FindDM(m_strDMAlias);
	CESL_Grid * pGrid = (CESL_Grid*)FindControl(m_strCMAlias, _T("MainGrid2"));

	if(pDM->GetRowCount() == 0) return -1;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnbtnCOPYDEL() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	CStringArray * pAcqKeyArray = NULL;
	CStringArray * pSpeciesKeyArray = NULL;
	CESL_DataMgr * pDM = FindDM(m_strDMAlias);
	CESL_Grid * pGrid = (CESL_Grid*)FindControl(m_strCMAlias, _T("MainGrid2"));
	SPECIES_TYPE nSpeciesType = SPECIES_TYPE_INVALID;
	ACQ_TYPE nAcqType = ACQ_TYPE_INVALID;
	if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_BASIC_DATA)
	{
		nSpeciesType = BO_BASIC;
		pSpeciesKeyArray = m_pArrAcqKey;
	}
	else if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_IMPORT_DATA)
	{
		nSpeciesType = BO_IMPORT;
		pSpeciesKeyArray = m_pArrAcqKey;
	}
	else
	{
		nSpeciesType = BO_PURCHASE;
		nAcqType = PURCHASE;
		pSpeciesKeyArray = m_pArrAcqKey;
		//pAcqKeyArray = m_pArrAcqKey;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)NULL);
	api.SetTempPtr(_T("Grid"), (DWORD*)pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)nSpeciesType);
	api.SetTempData(_T("ACQ_TYPE"), (INT)nAcqType);
	api.SetTempData(_T("삭제타입"), (INT)SPECIES_DELETE);
	api.SetTempData(_T("책수알리아스"), _T("UDF_자료상태설명"));
	api.SetTempData(_T("가격알리아스"), _T("관리번호"));
	api.SetTempData(_T("삭제종류"), _T("중복자료삭제"));
	api.SetTempPtr(_T("종키리스트"), (DWORD*)pSpeciesKeyArray);
	api.SetTempPtr(_T("수서정보키리스트"), (DWORD*)pAcqKeyArray);
	api.SetTempPtr(_T("권일련번호리스트"), NULL);
	ids = api.SPFExecutePath(_T("삭제"));
	if(ids == 0)
	{
		m_isDeleteOperation = TRUE;
		DisplayGrid();
	}	
}

BEGIN_EVENTSINK_MAP(CBO_HOLDING_ALL_DUP_SEARCH, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_HOLDING_ALL_DUP_SEARCH)
	ON_EVENT(CBO_HOLDING_ALL_DUP_SEARCH, IDC_gridBOKBON_SEARCH, -600 /* Click */, OnClickgridBOKBONSEARCH, VTS_NONE)
	ON_EVENT(CBO_HOLDING_ALL_DUP_SEARCH, IDC_gridINPUT_BOKBON_SEARCH, -600 /* Click */, OnClickgridINPUTBOKBONSEARCH, VTS_NONE)
	ON_EVENT(CBO_HOLDING_ALL_DUP_SEARCH, IDC_gridSE_INPUT_BOKBON_SEARCH, -600 /* Click */, OnClickgridSEINPUTBOKBONSEARCH, VTS_NONE)
	ON_EVENT(CBO_HOLDING_ALL_DUP_SEARCH, IDC_gridINPUT_BOKBON_SEARCH, -601 /* DblClick */, OnDlbClickgridSEINPUTBOKBONSEARCH, VTS_NONE)
	ON_EVENT(CBO_HOLDING_ALL_DUP_SEARCH, IDC_gridBOKBON_SEARCH, -605 /* MouseDown */, OnMouseDowngridBOKBONSEARCH, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBO_HOLDING_ALL_DUP_SEARCH, IDC_gridBOKBON_SEARCH, -601 /* DblClick */, OnDblClickgridBOKBONSEARCH, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnDlbClickgridSEINPUTBOKBONSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	if(m_pGrid->GetMouseRow() == 0) 
	{
		SortDuplicateData(m_pGrid);
		return ;
	}
	ShowDetailInfo(GUPDATE);
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnClickgridBOKBONSEARCH() 
{
/*
	// TODO: Add your control notification handler code here
	m_pDupMgr->InitTempData();
	m_pDupMgr->SetTempData(_T("DMAlias"), m_strDMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("GridAlias"), _T("MainGrid2"));
	m_pDupMgr->SetTempData(_T("CMAlias"), m_strCMAlias.GetBuffer(0));
//	if(m_bDuplicateSearchType)
		m_pDupMgr->SetTempData(_T("선정문자"), _T("MainGrid"));
//	else
//		Mgr.SetTempData(_T("선정문자"), _T("OneSelect"));
	m_pDupMgr->SetTempData(_T("Index"), -1);
	INT ids = m_pDupMgr->SPFExecutePath(_T("그리드선정처리"));

	if(ids < 0)
	{
		//에러처리
		return;
	}
*/
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnbtnSELSOURCEDATA() 
{
	// TODO: Add your control notification handler code here
	m_pDupMgr->InitTempData();
	m_pDupMgr->SetTempData(_T("CMAlias"), m_strCMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("DMAlias"), m_strDMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("GridAlias"), _T("MainGrid2"));
	m_pDupMgr->SetTempData(_T("선정문자"), _T("All"));
	m_pDupMgr->SetTempData(_T("Index"), -1);
	INT ids = m_pDupMgr->SPFExecutePath(_T("그리드선정처리"));
	if(ids < 0)
	{
		//에러처리
		return;
	}
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnbtnAPPLY() 
{
	// TODO: Add your control notification handler code here
	m_pDupMgr->InitTempData();
	m_pDupMgr->SetTempData(_T("CMAlias"), m_strCMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("DMAlias"), m_strDMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("GridAlias"), _T("MainGrid2"));	


	INT ids;
	if(m_DuplicateSearchType <= 5) 
	{
		//===================================================
		//2009.12.01 ADD BY PJW :  복본추기시 선정된 갯수에 따라서 메세지 보여준다.
		m_pGrid->SelectMakeList();
		INT nGridSelect;
		nGridSelect = m_pGrid->SelectGetCount();
		if( nGridSelect <= 0 )
		{
			ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
			return;
		}
		else if( nGridSelect > 1 )
		{
			ESLAfxMessageBox(_T("복본추기시 하나의 자료만 선정해 주세요."));
			return;
		}
		//===================================================
		ids = m_pDupMgr->SPFExecutePath(_T("복본추기"));
	}
	else ids = m_pDupMgr->SPFExecutePath(_T("연속복본자료적용"));
	if(ids < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	else if(ids >0)
	{
		ESLAfxMessageBox(_T("수서자료상태의 자료는 추가할 수 없습니다."));
		return;
	}
		
	m_pDupMgr->GetTempData(_T("SpeciesKey"), m_strSKey);
	CDialog::OnOK();
}

CString CBO_HOLDING_ALL_DUP_SEARCH::GetSpeciesKey()
{
	return m_strSKey;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnClickgridINPUTBOKBONSEARCH() 
{
	/********************************************************
	 ********************************************************/
	//20080515 ADD BY PJW : 서명복본조사도 하나만 선택하도록 수정		
//	m_pDupMgr->InitTempData();
//	m_pDupMgr->SetTempData(_T("CMAlias"), m_strCMAlias.GetBuffer(0));
//	m_pDupMgr->SetTempData(_T("DMAlias"), m_strDMAlias.GetBuffer(0));
//	m_pDupMgr->SetTempData(_T("GridAlias"), _T("MainGrid2"));
//	m_pDupMgr->SetTempData(_T("선정문자"), _T("OneSelect"));
//	m_pDupMgr->SetTempData(_T("Index"), -1);
//	INT ids = m_pDupMgr->SPFExecutePath(_T("그리드선정처리"));
	
	
	INT nSelect;
	CString strStatus;
	m_pGrid->SelectMakeList();
	nSelect = m_pGrid->SelectGetHeadPosition();
	m_pDM->GetCellData(_T("UDF_자료상태설명"),nSelect,strStatus);
	if( ( strStatus.Find(_T("소장자료")) >= 0 || strStatus.Find(_T("대출")) >= 0 ||
		  strStatus.Find(_T("배달")) >= 0	 || strStatus.Find(_T("수리제본")) >= 0 ||
		  strStatus.Find(_T("파손")) >= 0	 || strStatus.Find(_T("제적")) >= 0 ||
		  strStatus.Find(_T("재정리")) >= 0	 || strStatus.Find(_T("분실")) >= 0 ||
		  // 18.09.27 JMJ KOL.RED.2018.008
		  strStatus.Find(_T("책두레")) >= 0  || strStatus.Find(_T("상호대차대출자료")) >= 0 ||
		  // 17/07/20 김혜영 : 책나래대출자료 상태 추가
//*/ ADD ---------------------------------------------------------------------------
		  strStatus.Find(_T("책나래대출자료")) >= 0 	
//*/ END ---------------------------------------------------------------------------	
	 ) )
	{
		GetDlgItem(IDC_btnAPPLY)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_btnAPPLY)->EnableWindow(FALSE);
	}

	/********************************************************
	 ********************************************************/
	return ;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

BOOL CBO_HOLDING_ALL_DUP_SEARCH::GetIsDeleteOperation()
{
	return m_isDeleteOperation;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnClickgridSEINPUTBOKBONSEARCH() 
{
	// TODO: Add your control notification handler code here
	m_pDupMgr->InitTempData();
	m_pDupMgr->SetTempData(_T("DMAlias"), m_strDMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("GridAlias"), _T("MainGrid2"));
	m_pDupMgr->SetTempData(_T("CMAlias"), m_strCMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("선정문자"), _T("OneSelect"));
	m_pDupMgr->SetTempData(_T("Index"), -1);
	INT ids = m_pDupMgr->SPFExecutePath(_T("그리드선정처리"));

	if(ids < 0)
	{
		//에러처리
		return;
	}		
}

INT CBO_HOLDING_ALL_DUP_SEARCH::GetDonateGroupInfo(CString strSrc, CString &strAcqYear, CString &strGroupNo)
{
	CStringArray arrData;
	m_pDupMgr->ParsingString(_T("-"), strSrc, &arrData, FALSE);
	if(arrData.GetSize() > 0)
		strAcqYear = arrData.GetAt(0);
	if(arrData.GetSize() > 1)
		strGroupNo = arrData.GetAt(1);
	return 0;
}

INT CBO_HOLDING_ALL_DUP_SEARCH::GetPurchaseGroupInfo(CString strSrc, CString &strAcqYear, CString &strGroupNo, CString &strWorkNo, CString &strSerialNo)
{
	CStringArray arrData;
	m_pDupMgr->ParsingString(_T("-"), strSrc, &arrData, FALSE);
	if(arrData.GetSize() > 0)
		strAcqYear = arrData.GetAt(0);
	if(arrData.GetSize() > 1)
		strGroupNo = arrData.GetAt(1);
	if(arrData.GetSize() > 2)
		strWorkNo = arrData.GetAt(2);
	if(arrData.GetSize() > 3)
		strSerialNo = arrData.GetAt(3);
	return 0;	
}

CString CBO_HOLDING_ALL_DUP_SEARCH::MakeCallNumber(CESL_DataMgr *pDM, INT nIdx)
{
	CString strCallNo;
	CString strItem;
	pDM->GetCellData(_T("IBS_별치기호"), nIdx, strItem);
	if (!strItem.IsEmpty())
	{
		// 별치기호는 설명으로 바꾼다.
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItem, strItem);
	}

	strCallNo = strItem;

	pDM->GetCellData(_T("IBS_분류기호"), nIdx, strItem);
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T(" ") + strItem;
	}

	pDM->GetCellData(_T("IBS_도서기호"), nIdx, strItem);
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T("-") + strItem;
	}

	pDM->GetCellData(_T("IBS_권책기호"), nIdx, strItem);	
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T("-") + strItem;
	}

	// 청구기호
	pDM->SetCellData(_T("관리번호"), strCallNo, nIdx);
	return strCallNo;
}

CString CBO_HOLDING_ALL_DUP_SEARCH::MakeAcqInfo(CESL_DataMgr * pDM, INT nIdx)
{
	CString strTmpData, strAcqInfo, strAcqYear, strGroupNo, strWorkNo, strSerialNo, strWorkCode;
	INT nGroupNo, nWorkNo, nSerialNo;
	strTmpData = pDM->GetCellData(_T("BP_차수정보"), nIdx);
	if(!strTmpData.IsEmpty())
	{
		GetPurchaseGroupInfo(strTmpData, strAcqYear, strGroupNo, strWorkNo, strSerialNo);
		strWorkCode = pDM->GetCellData(_T("IBS_업무구분"), nIdx);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		strAcqInfo.Format(_T("구입:%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);
	}
	else 
	{
		strTmpData = pDM->GetCellData(_T("BP_접수번호정보"), nIdx);
		if(!strTmpData.IsEmpty())
		{
			GetDonateGroupInfo(strTmpData, strAcqYear, strGroupNo);
			nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
			strAcqInfo.Format(_T("기증:%s-%04d"), strAcqYear, nGroupNo);
		}
		else 
			strAcqInfo = _T("적용안함");
	}
	pDM->SetCellData(_T("관리번호"), strAcqInfo, nIdx);
	return strAcqInfo;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::CheckActiveValue( INT row, INT option )
{
	if( _T("V") == m_pGrid->GetText() )
	{
		m_pGrid->SetTextMatrix( row, 1, _T("") );
	}
	else
	{
		if(m_pDM->GetCellData(_T("식별번호"), row-1) != _T("**"))
			m_pGrid->SetTextMatrix( row, 1, _T("V") );
	}
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnMSGBOKBONSEARCH( UINT  nIDC )
{
	INT nRows = m_pGrid->GetRows();
	INT row = m_pGrid->GetRow();
	INT col = 1;

	INT i;

	INT top, bottom;
	top = m_pGrid->GetRow();
	bottom = m_pGrid->GetRowSel();

	INT tmp;
	if ( top < bottom ) {
		tmp = bottom;
		bottom = top;
		top = tmp;
	}

	// 선정
	if (nIDC == 5000) {
		for ( i = bottom ; i <= top ; i++ ) 
		{
			if(m_pDM->GetCellData(_T("식별번호"), i-1) != _T("**"))
				m_pGrid->SetTextMatrix(i, col, _T("V"));
		}
	}
	// 비선정
	if (nIDC == 5001) {
		for ( i = bottom ; i <= top ; i++ ) 
			m_pGrid->SetTextMatrix(i, col, _T(""));
	}
	// 전체선택
	if (nIDC == 5002) {
		for (i = 1; i < nRows; i++)
		{
			if(m_pDM->GetCellData(_T("식별번호"), i-1) != _T("**"))
				m_pGrid->SetTextMatrix(i, col, _T("V"));
		}
	}
	// 전체해제
	if (nIDC == 5003) {
		for (i = 1; i < nRows; i++) m_pGrid->SetTextMatrix(i, col, _T(""));
	}

	return;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::FreeMenu(VOID)
{
	if (m_pMenu)
	{
		m_pMenu->DestroyMenu();
		delete m_pMenu;
		m_pMenu = NULL;
	}

	return;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnMouseDowngridBOKBONSEARCH(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	INT row, col;
	row = m_pGrid->GetRow();
	col = m_pGrid->GetCol();

	// click된 컬럼이 2 일때 디폴트 값을 체크한다. 
	if (Button == 1) {
		switch( col )
		{
			case 1 :
				CheckActiveValue( row, 1 );
				break;
			default :
				break;
		}		
	}
	if (Button == 2) {	// 오른쪽 버튼일때
		if (col == 1) {
			FreeMenu();
			m_pMenu = new CMenu;
			if (m_pMenu == NULL) return;
			m_pMenu->CreatePopupMenu();

			m_pMenu->AppendMenu( MF_STRING , 5000 , _T("선정") );
			m_pMenu->AppendMenu( MF_STRING , 5001 , _T("비선정") );
			m_pMenu->AppendMenu( MF_STRING , 5002 , _T("전체선정") );
			m_pMenu->AppendMenu( MF_STRING , 5003 , _T("전체비선정") );
			CPoint pos;
			GetCursorPos(&pos);
			SetForegroundWindow();
			m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
		}
	}	
}

INT CBO_HOLDING_ALL_DUP_SEARCH::SetGridRowBackColor(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return -1;

	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(red);
			pGrid->SetCellForeColor(black);
		}
	}
	
	pGrid->SetCol(nCol);

	return 0;
}

INT CBO_HOLDING_ALL_DUP_SEARCH::SortDuplicateData(CESL_Grid *pGrid)
{
	CString strSortAlias;
	if(pGrid)
	{
		INT nCol = pGrid->GetCol();
		POSITION pos = pGrid->m_arrColumnInfo.FindIndex(nCol-1);
		CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetAt(pos);
		if(pColumnInfo == NULL) return -1;
		strSortAlias = (CString)pColumnInfo->strColumnDMFieldAlias;
		if(m_pDupMgr == NULL) return 0;
	}
	else
	{
		strSortAlias = _T("IBS_자료상태");
		m_nSortDirection = ESL_DATAMGR_SORT_ASC;
	}
	
	m_pDupMgr->InitTempData();
	m_pDupMgr->InitTempPtr();
	m_pDupMgr->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pDupMgr->SetTempData(_T("SORT_FIELD_ALIAS"), strSortAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("SORT_DIRECTION"), m_nSortDirection);
	INT ids = m_pDupMgr->SPFExecutePath(_T("자료정렬"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("자료를 정렬할 수 없습니다."));
		return -1;
	}
	if(m_nSortDirection == ESL_DATAMGR_SORT_ASC) m_nSortDirection = ESL_DATAMGR_SORT_DESC;
	else m_nSortDirection = ESL_DATAMGR_SORT_ASC;
	DisplayGrid();
	return 0;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnDblClickgridBOKBONSEARCH() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0)
	{
		SortDuplicateData(m_pGrid);
		return;
	}
	ShowDetailInfo(GUPDATE);
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

INT CBO_HOLDING_ALL_DUP_SEARCH::ShowDetailInfo(OPERATION_TYPE nOpType)
{
	if(m_pDM->GetRowCount() == 0) return 0;
	
	if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_REQUEST_DATA)
	{
		CBO_Request_Duplicate_DetailInfo_Dlg dlg(this);
		dlg.SetMainDM(m_pDM);
		dlg.SetMainGrid(m_pGrid);
		dlg.SetOpenMode(nOpType);
		dlg.DoModal();
		if(dlg.GetFurnishDataChange()) m_isDeleteOperation = TRUE;
	}
	else if(m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH 
		|| m_DuplicateSearchType == CAT_DUPLICATE_SEARCH 
		|| m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_DIVNO
		|| m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_BASIC_DATA
		|| m_DuplicateSearchType == HOLDING_DUPLICATE_SEARCH_IMPORT_DATA)
	{
		CBO_HOLDING_DUP_DETAIL_INFO dlg(this);
		dlg.SetMainDM(m_pDM);
		dlg.SetMainGrid(m_pGrid);
		dlg.SetOpenMode(nOpType);
		dlg.SetInfoType(m_DuplicateSearchType);
		dlg.DoModal();
		if(dlg.m_bIsProcDuplicateInsert) m_isDeleteOperation = TRUE;
	}
	else if(m_DuplicateSearchType == TITLE_DUPLICATE_SEARCH 
		|| m_DuplicateSearchType == TITLE_AUTO_DUPLICATE_SEARCH 
		|| m_DuplicateSearchType == CAT_TITLE_DUPLICATE_SEARCH
		|| m_DuplicateSearchType == SERIAL_TITLE_DUPLICATE_SEARCH
		|| m_DuplicateSearchType == FURNISH_DATA_DUPLICATE_SEARCH)
	{
		CBO_INPUT_DUP_DETAIL_INFO dlg(this);
		dlg.SetParentGrid(m_pGrid);
		dlg.SetParentDM(m_pDM);
		dlg.SetOpenMode(SUPDATE);
		dlg.DoModal();
	}
	return 0;
}

//2007.06.28 ADD PJW : 복본/중복조건 설정 키의 정보를 보여준다
INT CBO_HOLDING_ALL_DUP_SEARCH::ShowDupKeyInfo()
{
	
	return 0;
} 

INT CBO_HOLDING_ALL_DUP_SEARCH::GetManageNumber(INT nIdx, CString &strCallNumber, CString &strRegNo, CString &strAcqInfo)
{
	CMarc marc;
	CString strMarc;
	strMarc = m_pDM->GetCellData(_T("BS_MARC"), nIdx);
	if(!strMarc.IsEmpty())
	{
		m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strRegNo);
	}
	strCallNumber = MakeCallNumber(m_pDM, nIdx);
	strAcqInfo = MakeAcqInfo(m_pDM, nIdx);
	return 0;
}

BOOL CBO_HOLDING_ALL_DUP_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if ( pMsg->message == WM_KEYDOWN ) 
	{
		if ( pMsg->wParam == VK_F12 ) 
		{
			return TRUE;
		}
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
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
	
	CRect rect;
	GetClientRect(rect);

	//rect.top += 60;

	rect.top += 190;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);		
}

UINT CBO_HOLDING_ALL_DUP_SEARCH::SourceDataMerge(LPARAM lParam, WPARAM wParam)
{
	CString strKey;
	CBO_ACQ_Business_Api api(this);
	INT ids = api.DeleteDuplicateRow(m_pDM, lParam);
	if(ids < 0) return -1;
	m_pDM->DeleteRow(lParam);
	strKey.Format(_T("%ul"), wParam);
	ids = api.DeleteRow(m_pDM, strKey, _T("IBS_종KEY"));
	if(ids < 0) return -1;

	m_isDeleteOperation = TRUE;
	DisplayGrid();

	return 0;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::OnbtnExportHoldingMarc() 
{
	INT ids;

	if ( AfxMessageBox(_T("복본으로 나온 자료중 소장마크일 경우만 1건씩 반출합니다.\n진행하시겠습니까?"), MB_YESNO ) == IDYES )
	{
		ids = DoHoldingMarcExport();
		if ( ids < 0 )
		{
			AfxMessageBox(_T("반출 실패."));			
			return;
		}
		else if ( ids > 0 )
		{
			CString strTmp;

			strTmp.Format(_T("%d 건을 반출하였습니다."), ids);
			AfxMessageBox(strTmp);
			return;			
		}
		else
		{
			AfxMessageBox(_T("반출된 자료가 없습니다."));
		}		
	}
	
}

INT CBO_HOLDING_ALL_DUP_SEARCH::DoHoldingMarcExport()
{
	CString strWorkingStatus;
	CString strStreamMarc;
	CString	strCheckNo;
	CString strRecKey;
	CString strWhere;
	CString strHoldingStatus;
	BOOL	bStartFlag;
	BOOL	bOkFlag;
	INT nCount;
	INT nExportCount;
	INT	i;

	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CFileDialog dlg(FALSE, _T("txt"), _T("소장반출.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("파일형식(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return -1;

	_tchdir(szCurrentDir);

	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return -1;
    sFileName = dlg.GetPathName();		
	
	bStartFlag = FALSE;
	nExportCount = 0;
	nCount = m_pDM->GetRowCount();
	if ( nCount == 0 ) return 0;

	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 	strHoldingStatus = _T("BOT212O BOL112N BOL113O BOL114O BOL115O BOL116O BOL252N BOL211O BOL212O BOL213O BOL214O BOL215O BOL217O");
	// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
	// 18.09.27 JMJ KOL.RED.2018.008
	strHoldingStatus = _T("BOT212O BOL112N BOL113O BOL114O BOL115O BOL116O BOL252N BOL211O BOL212O BOL213O BOL214O BOL215O BOL217O BOL411O BOL511O BOL611O");
	//strHoldingStatus = _T("BOT212O BOL112N BOL113O BOL114O BOL115O BOL116O BOL252N BOL211O BOL212O BOL213O BOL214O BOL215O BOL217O BOL511O");
	//=====================================================
	

	CStdioFile file;
	if ( file.Open(sFileName,::CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)==NULL ) return -1 ;
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	for ( i=0 ; i<nCount ; i++ )
	{
		m_pDM->GetCellData(_T("식별번호"), i, strCheckNo);
		if ( strCheckNo != _T("**") ) 
		{			
			bOkFlag = FALSE;
			continue;
		}		
		
		// 복본자료중 소장자료를 찾았으면 다음자료가 나올때 까지 skip한다.
		if ( bOkFlag == TRUE ) 
		{
			continue;
		}
		
		m_pDM->GetCellData(_T("IBS_자료상태"), i, strWorkingStatus);
		if ( strHoldingStatus.Find(strWorkingStatus) >= 0  )
		{
			m_pDM->GetCellData(_T("IBS_종KEY"), i, strRecKey);
			strWhere.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), strRecKey);
			m_pDM->GetOneValueQuery(strWhere, strStreamMarc);
			file.WriteString(strStreamMarc+_T("\r\n"));
			nExportCount++;
			bOkFlag = TRUE;			
		}		
	}
	file.Close();	

	return nExportCount;
}

BOOL CBO_HOLDING_ALL_DUP_SEARCH::GetBookTradeInfo()
{
	CString		sFileName;
	CStdioFile	file;

	sFileName = _T("..\\CFG\\출판유통.CFG");

	if ( file.Open(sFileName,::CFile::modeRead | CFile::typeBinary)==NULL ) return FALSE;
	
	return TRUE;
}
//2007.06.28 ADD PJW : 
VOID CBO_HOLDING_ALL_DUP_SEARCH::OnbtnDupKeyINFO() 
{
	// TODO: Add your control notification handler code here
	ShowDupKeyInfo();
	return;	
	// TODO: Add your control notification handler code here
}

//===============================================================
//20080617 ADD BY PJW : 수서 파트에서는 복본추기버튼을 숨긴다.
INT CBO_HOLDING_ALL_DUP_SEARCH::CheckApplyBtnShow( CString strMainDmAlias ) 
{
	CESL_DataMgr* pTempDM = FindDM( strMainDmAlias );

	if ( pTempDM != NULL )
	{
		GetDlgItem(IDC_btnAPPLY)->ShowWindow(SW_HIDE);
	}
	return 0;	
}
//===============================================================

HBRUSH CBO_HOLDING_ALL_DUP_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
//========================================================================
//2008.08.26 ADD BY PJW : 복본조사설정을 복본조사 다이얼로그에서 가능하도록 수정
INT CBO_HOLDING_ALL_DUP_SEARCH::InitDupKeySetting()
{
	this->SetDisplayData();
	m_bInit = TRUE;
	OnchkAUTHOR1();
	OnchkISBN1();
	OnchkMEDCLASS1();
	OnchkPUBLICATEYEAR1();
	OnchkPUBLISHER1();
	OnchkREGCODE1();
	OnchkSEPARATESHELFCODE1();
	OnchkTITLE1();
	OnchkTITLETWICESEARCH1();
	OnchkVOL1();
	OnchkVOLTITLE1();
	m_bInit = FALSE;

	// 2007.06.01 ADD BY PDJ
	// 복본조사 제적자료 포함여부 조건 추가함
	// 기존 방식과 형식이 달라 별도의 파일로 추가함.
	// 또한 패치시 기존 데이터 overwrite 문제로 별도로 나가야 함.
	LoadAppendCondition();
	return 0;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::SetDisplayData()
{
	INT titleStype;
	CButton * pRadTitleAll = (CButton *)GetDlgItem(IDC_radALLWORDSEARCH1);
	CButton * pRadRight = (CButton *)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION1);
	CButton * pRadEqual = (CButton *)GetDlgItem(IDC_radTITLE_EQUIVALENCE1);
	CButton * pRadTitleEng = (CButton *)GetDlgItem(IDC_radTITLE_CUTSEARCH1);
	CButton * pChkTwice = (CButton *)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH1);
	INT ids = m_SearchKey.GetFieldKeySettingInfo(_T("본표제"), &m_chkTitle, &titleStype, &m_chkTwice);
	if(ids < 0) 
	{
		m_SearchKey.SetDefaultSearchKeys();
	}

	if(titleStype == 2)
	{
		pRadTitleEng->SetCheck(1);
		this->m_radTitle = -1;
		pChkTwice->EnableWindow(FALSE);
	}
	else
	{
		pRadTitleAll->SetCheck(1);
		if(titleStype < 0)
		{
			m_SearchKey.GetFieldKeySettingInfo(_T("본표제"), &m_chkTitle, &titleStype, &m_chkTwice);
		}
		this->m_radTitle = titleStype;		
	}

	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("총서명"), &m_chkSeriesTitle, &titleStype, &m_chkTwice);
	if(ids < 0) 
	{
		m_SearchKey.SetDefaultSearchKeys();
	}
	
	if(titleStype == 2)
	{
		pRadTitleEng->SetCheck(1);
		this->m_radTitle = -1;
		pChkTwice->EnableWindow(FALSE);
	}
	else
	{
		pRadTitleAll->SetCheck(1);
		if(titleStype < 0)
		{
			m_SearchKey.GetFieldKeySettingInfo(_T("총서명"), &m_chkSeriesTitle, &titleStype, &m_chkTwice);
		}
		this->m_radTitle = titleStype;		
	}
	//===================================================
	

//	this->m_chkTwice = 0;

	ids = m_SearchKey.GetFieldKeySettingInfo(_T("저작자"), &this->m_chkAuthor, &this->m_radAuthor, NULL);
	if(ids < 0) return;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("발행자"), &this->m_chkPublisher, &this->m_radPublisher, NULL);
	if(ids < 0) return ;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("발행년"), &this->m_chkPubYear, &this->m_radPubYear, NULL);
	if(ids < 0) return;
	this->m_radPubYear = this->m_radPubYear - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("낱권ISBN"), &this->m_chkISBN, &this->m_radISBN, NULL);
	if(ids < 0) return;
	this->m_radISBN = this->m_radISBN - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("매체구분"), &this->m_chkMedClass, &this->m_radMedCode, NULL);
	if(ids < 0) return;
	this->m_radMedCode = this->m_radMedCode - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("편권차"), &this->m_chkVol, &this->m_radVol, NULL);
	if(ids < 0) return;
	this->m_radVol = this->m_radVol - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("권서명"), &this->m_chkVolTitle, &this->m_radVolTitle, NULL);
	if(ids < 0) return;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("별치기호"), &this->m_chkShelfCode, &this->m_radShelfCode, NULL);
	if(ids < 0) return;
	this->m_radShelfCode = this->m_radShelfCode - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("등록구분"), &this->m_chkRegCode, &this->m_radRegCode, NULL);
	if(ids < 0) return;
	this->m_radRegCode = this->m_radRegCode -1;	

	this->UpdateData(FALSE);

}
VOID CBO_HOLDING_ALL_DUP_SEARCH::GetDisplayData()
{
	this->UpdateData(TRUE);

	INT ids;
	CButton * pRadTitleAll = (CButton *)GetDlgItem(IDC_radALLWORDSEARCH1);
	if(pRadTitleAll->GetCheck() == 1)
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("본표제"), this->m_chkTitle, this->m_radTitle, this->m_chkTwice);
	}
	else
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("본표제"), this->m_chkTitle, 2, this->m_chkTwice);
	}
	if(ids < 0) return;

	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
	if(pRadTitleAll->GetCheck() == 1)
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("총서명"), this->m_chkSeriesTitle, this->m_radTitle, this->m_chkTwice);
	}
	else
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("총서명"), this->m_chkSeriesTitle, 2, this->m_chkTwice);
	}
	if(ids < 0) return;
	//===================================================	
	
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("저작자"), this->m_chkAuthor, this->m_radAuthor, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("발행자"), this->m_chkPublisher, this->m_radPublisher, FALSE);
	if(ids < 0) return ;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("발행년"), this->m_chkPubYear, this->m_radPubYear+1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("낱권ISBN"), this->m_chkISBN, this->m_radISBN + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("매체구분"), this->m_chkMedClass, this->m_radMedCode + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("편권차"), this->m_chkVol, this->m_radVol + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("권서명"), this->m_chkVolTitle, this->m_radVolTitle, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("별치기호"), this->m_chkShelfCode, this->m_radShelfCode + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("등록구분"), this->m_chkRegCode, this->m_radRegCode + 1, FALSE);
	if(ids < 0) return;	
}

BOOL CBO_HOLDING_ALL_DUP_SEARCH::CheckDupSearchElements()
{
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
//	if(!m_chkVolTitle && !m_chkTitle && !m_chkAuthor && !m_chkPublisher && !m_chkPubYear 
//		&& !m_chkISBN && !this->m_chkMedClass && !this->m_chkRegCode && !this->m_chkShelfCode
//		&& !this->m_chkVol )
	if(!m_chkVolTitle && !m_chkTitle && !m_chkAuthor && !m_chkPublisher && !m_chkPubYear 
		&& !m_chkISBN && !this->m_chkMedClass && !this->m_chkRegCode && !this->m_chkShelfCode
		&& !this->m_chkVol && !this->m_chkSeriesTitle)
	//===================================================
	{
		ESLAfxMessageBox(_T("적어도 한 개 이상의 조사항목을 선정하셔야 합니다."));
		return FALSE;
	}

	// 17/05/10 김혜영 : 복본검색 개선 ('서명검색', 'ISBN검색'으로 선택)
/*//BEFORE----------------------------------------------------------------------------------------
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
	//if(!m_chkTitle && !m_chkAuthor && !m_chkPublisher )
	if(!m_chkTitle && !m_chkAuthor && !m_chkPublisher && !m_chkSeriesTitle )
	//===================================================
	
	{
		ESLAfxMessageBox(_T("[서명]을 조사항목으로 설정하지 않을 경우\n[저작자]나 [발행자] 항목이 조사항목으로 선정되어야 합니다."));
		PostMessage(WM_COMMAND, IDC_chkAUTHOR1, BN_CLICKED);
		PostMessage(WM_COMMAND, IDC_chkPUBLISHER1, BN_CLICKED);
		return FALSE;
	}
*///AFTER-----------------------------------------------------------------------------------------
	if(!m_chkTitle && !m_chkSeriesTitle && !m_chkISBN && !m_chkAuthor && !m_chkPublisher )
	{
		ESLAfxMessageBox(_T("[서명] 또는 [ISBN]을 조사항목으로 설정하지 않을 경우\n[저작자]나 [발행자] 항목이 조사항목으로 선정되어야 합니다."));
		PostMessage(WM_COMMAND, IDC_chkAUTHOR1, BN_CLICKED);
		PostMessage(WM_COMMAND, IDC_chkPUBLISHER1, BN_CLICKED);
		return FALSE;
	}
/*//END-------------------------------------------------------------------------------------------*/

	return TRUE;
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::LoadAppendCondition()
{	
	CStdioFile file;		
	CString strLine;
	CString filename = _T("duplicate_search_key2.txt");	

	if ( !file.Open(filename, CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("복본조사 설정키가 완전하지 않습니다."));
		return;
	}	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while( file.ReadString(strLine) )
	{			
		if ( strLine.Find(_T("[제적자료포함여부]") ) >= 0 )
		{
			file.ReadString(strLine);
			if ( strLine.Find(_T("USE_YN=1") )  >= 0 )
			{
				m_bIncludeDiscardedBook = TRUE; 
			}
			else 
			{
				m_bIncludeDiscardedBook = FALSE;				
			}
		}		
	}

	file.Close();

	UpdateData(FALSE);	
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::SaveAppendCondition()
{
	UpdateData(TRUE);		
	
	CStdioFile file;		
	CString strLine;
	CString filename = _T("duplicate_search_key2.txt");	

	m_bIncludeDiscardedBook = ( (CButton*) GetDlgItem(IDC_chkDiscardBook1) )->GetCheck();

	if ( !file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("복본조사 설정키가 완전하지 않습니다."));
		return;
	}
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	if ( m_bIncludeDiscardedBook == TRUE) 
	{
		strLine = _T("[제적자료포함여부]\nUSE_YN=1\n");
	}
	else
	{
		strLine = _T("[제적자료포함여부]\nUSE_YN=0\n");
	}
	file.WriteString(strLine);
	file.Close();	
}

VOID CBO_HOLDING_ALL_DUP_SEARCH::DupKeySave() 
{
	// TODO: Add extra validation here
	if(!CheckDupSearchElements()) return;
	GetDisplayData();
	m_SearchKey.GetSearchKeys(m_FileName);
	SaveAppendCondition();
// 	ESLAfxMessageBox(_T("설정하신 중복/복본조사 키값을 저장하였습니다."));
}
//========================================================================

//========================================================================
//2008.08.26 ADD BY PJW : 복본조사를 복본조사 다이얼로그에서 가능하도록 수정
VOID CBO_HOLDING_ALL_DUP_SEARCH::SearchDuplicateData()
{
	INT searchCount = -1;	
	m_pDupMgr->SetKeyFileName(_T(".\\duplicate_search_key.txt"));
	m_pDupMgr->InitTempData();
	m_pDupMgr->SetTempData(_T("CMAlias"), m_strParentCMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("SourceDMAlias"), m_strParentDMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("GridAlias"), m_strParentGridAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("CopyDMAlias"), _T("DM_중복본조사_복사본"));
	m_pDupMgr->SetTempData(_T("TargetDMAlias"), _T("DM_중복본조사"));
	m_pDupMgr->SetTempData(_T("KeyDMAlias"), _T("DM_중복본조사키"));
//	m_pDupMgr->SetTempPtr(_T("DataProcessingProgressCtrl"), (DWORD*)m_pPrgCtrl);
//	if(m_pGroup != NULL) m_pDupMgr->SetTempPtr(_T("GroupInfo"), (DWORD*)m_pGroup);
//	if(m_pGroupCollection != NULL) m_pDupMgr->SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);

	INT ids = m_pDupMgr->SPFExecutePath(m_strSearchType);
	m_pDupMgr->GetTempData(_T("SearchCount"), searchCount);
	m_nSearchCount = searchCount;
	
	if( searchCount < 1 )
	{
		ESLAfxMessageBox(_T("복본자료가 없습니다."));

	}
}
//========================================================================

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkAUTHOR1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkAuthor = !this->m_chkAuthor;
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radAUTHOR_RIGHTTRUNCATION1);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radAUTHOR_EQUIVALENCE1);
	if(this->m_chkAuthor)
	{
		if(this->m_radAuthor == -1)
			this->m_radAuthor = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radAuthor = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
	}
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkDiscardBook1() 
{
	this->UpdateData(TRUE);	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkISBN1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkISBN = !this->m_chkISBN;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radISBN_EQUIVALENCE1);
	if(this->m_chkISBN)
	{
		this->m_radISBN = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radISBN = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkMEDCLASS1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkMedClass = !this->m_chkMedClass;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radMEDIACODE_EQUIVALENCE1);
	if(this->m_chkMedClass)
	{
		this->m_radMedCode = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radMedCode = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkPUBLICATEYEAR1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkPubYear = !this->m_chkPubYear;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radPUBLICATE_YEAR_EQUIVALENCE1);
	if(this->m_chkPubYear)
	{
		this->m_radPubYear = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radPubYear = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkPUBLISHER1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkPublisher = !this->m_chkPublisher;
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radPUBLISHER_RIGHTTRUNCATION1);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radPUBLISHER_EQUIVALENCE1);
	if(this->m_chkPublisher)
	{
		if(this->m_radPublisher == -1)
			this->m_radPublisher = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radPublisher = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkREGCODE1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkRegCode = !this->m_chkRegCode;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radREGCODE_EQUIVALENCE1);
	if(this->m_chkRegCode)
	{
		this->m_radRegCode = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radRegCode = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkSEPARATESHELFCODE1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkShelfCode = !this->m_chkShelfCode;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radSEPARATECODE_EQUIVALENCE1);
	if(this->m_chkShelfCode)
	{
		this->m_radShelfCode = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radShelfCode = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkTITLE1() 
{
	// TODO: Add your control notification handler code here
	//===================================================
	//2010.11.29 UPDATE BY PJW : 필수조건으로 총서명 추가함
//	if(!m_bInit) this->m_chkTitle = !this->m_chkTitle;
	if(!m_bInit)
	{
		this->m_chkTitle = !this->m_chkTitle;
		if( TRUE == m_chkTitle )
		{
			this->m_chkSeriesTitle = FALSE;
		}
	}
	//===================================================
	
	CButton * prad1 = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH1);
	CButton * prad2 = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION1);
	CButton * prad3 = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE1);
	CButton * prad4 = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH1);
	CButton * pchk = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH1);
	//===================================================
	//2010.11.29 UPDATE BY PJW : 필수조건으로 총서명 추가함
//	if(this->m_chkTitle)
	if(this->m_chkTitle || this->m_chkSeriesTitle)
	//===================================================	
	{
		prad1->EnableWindow(TRUE);
		prad2->EnableWindow(TRUE);
		prad3->EnableWindow(TRUE);
		prad4->EnableWindow(TRUE);
		pchk->EnableWindow(TRUE);
	}
	else
	{
		if(!m_bInit)
			ESLAfxMessageBox(_T("서명조건을 해제할 경우 복본/중복조사 결과가 많아질 수 있습니다.\n이 사항은 복본/중복조사 속도를 저하 시킬수 있습니다."));
		prad1->EnableWindow(FALSE);
		prad2->EnableWindow(FALSE);
		prad3->EnableWindow(FALSE);
		prad4->EnableWindow(FALSE);
		pchk->EnableWindow(FALSE);
	}
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkTITLETWICESEARCH1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkTwice = !this->m_chkTwice;
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH1);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH1);
	if(pradCut->GetCheck() == 1)
	{
		pchkTwice->EnableWindow(FALSE);
	}
	else
	{
		pchkTwice->EnableWindow(TRUE);
	}

	// 17/05/10 김혜영 : 복본검색 개선 ('서명검색', 'ISBN검색'으로 선택)
//*/ ADD ---------------------------------------------------------------------------
	if ( m_chkTitle == 0 && m_chkSeriesTitle == 0)
	{
		pchkTwice->EnableWindow(FALSE);
	}
//*/ END ---------------------------------------------------------------------------

	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkVOL1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkVol = !this->m_chkVol;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radVOL_EQUIVALENCE1);
	if(this->m_chkVol)
	{
		this->m_radVol = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radVol = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnchkVOLTITLE1() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkVolTitle = !this->m_chkVolTitle;
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radVOL_TITLE_RIGHTTRUNCATION1);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radVOL_TITLE_EQUIVALENCE1);
	if(this->m_chkVolTitle)
	{
		if(this->m_radVolTitle == -1)
			this->m_radVolTitle = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radPublisher = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradAUTHOREQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkAuthor = TRUE;
	this->m_radAuthor = 1;
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradAUTHORRIGHTTRUNCATION1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkAuthor = TRUE;
	this->m_radAuthor = 0;
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradMEDIACODEEQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkMedClass = TRUE;
	this->m_radMedCode = 0;
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradPUBLICATEYEAREQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkPubYear = TRUE;
	this->m_radPubYear = 0;
	this->UpdateData(FALSE);	
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradPUBLISHEREQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkPublisher = TRUE;
	this->m_radPublisher = 1;
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradPUBLISHERRIGHTTRUNCATION1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkPublisher = TRUE;
	this->m_radPublisher = 0;
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradREGCODEEQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkRegCode = TRUE;
	this->m_radRegCode = 0;
	this->UpdateData(FALSE);	
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradSEPARATECODEEQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkShelfCode = TRUE;
	this->m_radShelfCode = 0;
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradTITLECUTSEARCH1() 
{
	// TODO: Add your control notification handler code here
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH1);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH1);
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION1);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE1);
	if(pradCut->GetCheck() == 1)
	{
		//this->m_radTitle = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
		pchkTwice->EnableWindow(FALSE);
	}
	else
	{
		//this->m_radTitle = -1;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
		pchkTwice->EnableWindow(TRUE);
	}
	this->UpdateData(FALSE);	
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradTITLEEQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH1);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH1);
	CButton * pradAllWord = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH1);
	if(pradCut->GetCheck() == 1)
		this->m_radTitle = -1;
	else
	{
		this->m_radTitle = 1;
		pradAllWord->SetCheck(1);
	}
	
	this->UpdateData(FALSE);	
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradTITLERIGHTTRUNCATION1() 
{
	// TODO: Add your control notification handler code here
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH1);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH1);
	CButton * pradAllWord = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH1);
	if(pradCut->GetCheck() == 1)
		this->m_radTitle = -1;
	else
	{
		this->m_radTitle = 0;
		pradAllWord->SetCheck(1);
	}
	
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradVOLEQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkVol = TRUE;
	this->m_radVol = 0;
	this->UpdateData(FALSE);	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradVOLTITLEEQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkVolTitle = TRUE;
	this->m_radVolTitle = 1;
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradISBNEQUIVALENCE1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkISBN = TRUE;
	this->m_radISBN = 0;
	this->UpdateData(FALSE);
	
}

void CBO_HOLDING_ALL_DUP_SEARCH::OnradVOLTITLERIGHTTRUNCATION1() 
{
	// TODO: Add your control notification handler code here
	this->m_chkVolTitle = TRUE;
	this->m_radVolTitle = 0;
	this->UpdateData(FALSE);	
	
}


void CBO_HOLDING_ALL_DUP_SEARCH::OnbtnDUPFINDSTART() 
{
	DupKeySave();
	SearchDuplicateData();	
	DisplayGrid();
	
}




void CBO_HOLDING_ALL_DUP_SEARCH::OnradALLWORDSEARCH1() 
{
	// TODO: Add your control notification handler code here
	CButton * pradAllWord = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH1);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH1);
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION1);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE1);
	if(pradAllWord->GetCheck() == 1)
	{
		if(this->m_radTitle == -1)
			this->m_radTitle = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
		pchkTwice->EnableWindow(TRUE);
	}
	
	this->UpdateData(FALSE);
	
}



//===================================================
//2010.11.29 ADD BY PJW : 필수조건으로 총서명 추가함
void CBO_HOLDING_ALL_DUP_SEARCH::OnchkSeriesTitle1() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//===================================================
	//2010.11.29 UPDATE BY PJW : 필수조건으로 총서명 추가함
//	if(!m_bInit) this->m_chkSeriesTitle = !this->m_chkSeriesTitle;
	if(!m_bInit)
	{
		this->m_chkSeriesTitle = !this->m_chkSeriesTitle;
		if( TRUE == m_chkSeriesTitle )
		{
			this->m_chkTitle = FALSE;
		}
	}
	//===================================================
	
	CButton * prad1 = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH1);
	CButton * prad2 = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION1);
	CButton * prad3 = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE1);
	CButton * prad4 = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH1);
	CButton * pchk = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH1);
	if(this->m_chkTitle || this->m_chkSeriesTitle)	
	{
		prad1->EnableWindow(TRUE);
		prad2->EnableWindow(TRUE);
		prad3->EnableWindow(TRUE);
		prad4->EnableWindow(TRUE);
		pchk->EnableWindow(TRUE);
	}
	else
	{
		if(!m_bInit)
			ESLAfxMessageBox(_T("서명조건을 해제할 경우 복본/중복조사 결과가 많아질 수 있습니다.\n이 사항은 복본/중복조사 속도를 저하 시킬수 있습니다."));
		prad1->EnableWindow(FALSE);
		prad2->EnableWindow(FALSE);
		prad3->EnableWindow(FALSE);
		prad4->EnableWindow(FALSE);
		pchk->EnableWindow(FALSE);
	}
	this->UpdateData(FALSE);
}
//===================================================
