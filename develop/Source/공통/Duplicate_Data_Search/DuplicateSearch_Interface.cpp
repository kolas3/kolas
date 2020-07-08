// DuplicateSearch_Interface.cpp: implementation of the CDuplicateSearch_Interface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DuplicateSearch_Interface.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "_excelmancontrol.h"
#include "ExcelManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDuplicateSearch_Interface::CDuplicateSearch_Interface(CESL_Mgr * pMgr)
{
	m_isDeleteOperation = FALSE;
	m_pPrgCtrl = NULL;
	m_pDataProcessingDlg = NULL;
	m_pThread = NULL;
	m_bRunning = FALSE;
	m_strCMAlias = _T("");
	m_strDMAlias = _T("");
	m_strGridAlias = _T("");
	m_strSearchType = _T("");
	m_nSearchCount = -1;

	m_pParentMgr = pMgr;
	m_pGroupCollection = NULL;
	m_pGroup = NULL;
	m_arrAcqKey.RemoveAll();
	m_strSourceSpeciesClass = _T("");
}

CDuplicateSearch_Interface::~CDuplicateSearch_Interface()
{
	ThreadTerminate();
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
		m_pPrgCtrl = NULL;
	}
}

BEGIN_MESSAGE_MAP(CDuplicateSearch_Interface, CWnd)
	//{{AFX_MSG_MAP(CDuplicateSearch_Interface)
	ON_MESSAGE(JOB_CANCEL_MSG, OnJobCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

VOID CDuplicateSearch_Interface::ThreadTerminate()
{
	if(!m_pThread) return ;
	if(AfxIsValidAddress(m_pThread, sizeof(CWinThread), 0))
	{
		m_pThread->m_bAutoDelete = FALSE;
		m_bRunning = FALSE;
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		m_pThread = NULL;
	}
}

VOID CDuplicateSearch_Interface::OnJobCancel(WPARAM w, LPARAM l)
{
	ThreadTerminate();
}

UINT CDuplicateSearch_Interface::SearchDuplicateData(LPVOID pParam)
{
	CDuplicateSearch_Interface * pObject = (CDuplicateSearch_Interface*)pParam;
	pObject->SearchDuplicateData();
	return 0;
}

VOID CDuplicateSearch_Interface::SearchDuplicateData()
{	
	CDuplicateSearchMgr Mgr(this, m_pParentMgr);
	//Mgr.SetSearchKeyInfo(_T(".\\duplicate_search_key.txt"));
	Mgr.SetKeyFileName(_T(".\\duplicate_search_key.txt"));
	Mgr.InitTempData();
	Mgr.SetTempData(_T("CMAlias"), m_strCMAlias.GetBuffer(0));
	Mgr.SetTempData(_T("SourceDMAlias"), m_strDMAlias.GetBuffer(0));
	Mgr.SetTempData(_T("GridAlias"), m_strGridAlias.GetBuffer(0));
	Mgr.SetTempData(_T("CopyDMAlias"), _T("DM_중복본조사_복사본"));
	Mgr.SetTempData(_T("TargetDMAlias"), _T("DM_중복본조사"));
	Mgr.SetTempData(_T("KeyDMAlias"), _T("DM_중복본조사키"));
	Mgr.SetTempPtr(_T("DataProcessingProgressCtrl"), (DWORD*)m_pPrgCtrl);
	if(m_pGroup != NULL) Mgr.SetTempPtr(_T("GroupInfo"), (DWORD*)m_pGroup);
	if(m_pGroupCollection != NULL) Mgr.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);
	INT searchCount = -1;	
	INT ids = Mgr.SPFExecutePath(m_strSearchType);
	if(!m_bRunning) return ;
	m_pDataProcessingDlg->ShowWindow(SW_HIDE);
	
	if(ids < 0)
	{
		if(ids == -1002)
			ESLAfxMessageBox(_T("선정된자료가 없습니다."));
		m_bRunning = FALSE;
		return ;
	}
	
	Mgr.GetTempData(_T("SearchCount"), searchCount);
	m_nSearchCount = searchCount;
	
	SendShowSearchResultMsg();
	m_bRunning = FALSE;
}

VOID CDuplicateSearch_Interface::SendShowSearchResultMsg()
{
	//::SendMessage(m_pParentMgr->GetSafeHwnd(), SHOW_SEARCH_RESULT, 0, 0);
	m_pParentMgr->PostMessage(SHOW_SEARCH_RESULT);
}

VOID CDuplicateSearch_Interface::ShowHoldDuplicateSearch(CString searchType, CString DMAlias, CString CMAlias, CString GridAlias)
{
	m_strSearchType = searchType;
	m_strDMAlias = DMAlias;
	m_strCMAlias = CMAlias;
	m_strGridAlias = GridAlias;

	ShowDataProcessingDlg();
	CString msg;
	msg = searchType + _T("를 수행하고 있습니다.");
	m_pDataProcessingDlg->SetMessage(msg);
	m_bRunning = TRUE;
	m_pThread = AfxBeginThread(SearchDuplicateData, this);
	
}

INT CDuplicateSearch_Interface::ShowOnlyTitleDuplicateSearch(CString searchType, CString strTitle, CString &strResultCnt, CString species_key)
{
	if(strTitle.GetLength() == 0) return -1000;
	
	CDuplicateSearchMgr Mgr(m_pParentMgr);
	//Mgr.SetSearchKeyInfo(_T(".\\duplicate_search_key.txt"));
	Mgr.SetKeyFileName(_T(".\\duplicate_search_key.txt"));
	Mgr.InitTempData();
	Mgr.SetTempData(_T("TargetDMAlias"), _T("DM_중복본조사"));
	Mgr.SetTempData(_T("CopyDMAlias"), _T("DM_중복본조사_복사본"));
	Mgr.SetTempData(_T("KeyDMAlias"), _T("DM_중복본조사키"));
	Mgr.SetTempData(_T("Title"), strTitle.GetBuffer(0));
	Mgr.SetTempData(_T("SpeciesKey"), species_key.GetBuffer(0));	
	INT ids = Mgr.SPFExecutePath(searchType);
	if(ids < 0) return -1002;
	INT tmpcnt;
	Mgr.GetTempData(_T("SearchCount"), tmpcnt);
	strResultCnt.Format(_T("%d"), tmpcnt);
	
	return 0;
}

INT CDuplicateSearch_Interface::ShowMarcDuplicateSearch(CString searchType, CString strMarc, CString &strResultCnt)
{
	CDuplicateSearchMgr Mgr(m_pParentMgr);
	//Mgr.SetSearchKeyInfo(_T(".\\duplicate_search_key.txt"));
	Mgr.SetKeyFileName(_T(".\\duplicate_search_key.txt"));
	Mgr.InitTempData();
	Mgr.SetTempData(_T("TargetDMAlias"), _T("DM_중복본조사"));
	Mgr.SetTempData(_T("CopyDMAlias"), _T("DM_중복본조사_복사본"));
	Mgr.SetTempData(_T("KeyDMAlias"), _T("DM_중복본조사키"));
	Mgr.SetTempData(_T("Marc"), strMarc.GetBuffer(0));
	Mgr.SetTempData(_T("SpeciesKey"), _T(""));
	INT ids = Mgr.SPFExecutePath(searchType);
	if(ids < 0) return -1002;
	INT tmpcnt;
	Mgr.GetTempData(_T("SearchCount"), tmpcnt);
	strResultCnt.Format(_T("%d"), tmpcnt);
	
	return 0;
}

BOOL CDuplicateSearch_Interface::GetIsDeleteOperation()
{
	return m_isDeleteOperation;
}

INT CDuplicateSearch_Interface::DuplicateDataSearchByMarc(CString strMarc)
{
	CDuplicateSearchMgr Mgr(m_pParentMgr);
	//Mgr.SetSearchKeyInfo(_T(".\\duplicate_search_key.txt"));
	Mgr.SetKeyFileName(_T(".\\duplicate_search_key.txt"));
	Mgr.InitTempData();
	Mgr.SetTempData(_T("TargetDMAlias"), _T("DM_중복본조사"));
	Mgr.SetTempData(_T("CopyDMAlias"), _T("DM_중복본조사_복사본"));
	Mgr.SetTempData(_T("KeyDMAlias"), _T("DM_중복본조사키"));
	Mgr.SetTempData(_T("Marc"), strMarc.GetBuffer(0));
	
	INT ids = Mgr.SPFExecutePath(_T("마크복본조사"));
	if(ids < 0) return -1000;
	return 0;
}

VOID CDuplicateSearch_Interface::ShowTitleDuplicateSearchDlg(CString strSearchType, CString &Skey)
{
	INT nSearchType = -1;
	if(strSearchType == _T("서명복본조사")) nSearchType = TITLE_DUPLICATE_SEARCH;
	else if(strSearchType == _T("서명자동복본조사")) nSearchType = TITLE_AUTO_DUPLICATE_SEARCH;
	else if(strSearchType == _T("정리서명복본조사")) nSearchType = CAT_TITLE_DUPLICATE_SEARCH;
	else if(strSearchType == _T("비치희망입력화면복본조사")) nSearchType = FURNISH_DATA_DUPLICATE_SEARCH;
	else if(strSearchType == _T("연속서명복본조사")) nSearchType = SERIAL_TITLE_DUPLICATE_SEARCH;
	else if(strSearchType == _T("건별복본조사")) nSearchType = HOLDING_DUPLICATE_SEARCH_INDIVIDUAL_DATA;

	CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, nSearchType);
	Dlg.m_strSourceSpeciesClass = m_strSourceSpeciesClass;
	if(IDOK == Dlg.DoModal())
	{
		Skey = Dlg.GetSpeciesKey();
	}
	else
		Skey = _T("");
	m_isDeleteOperation = Dlg.GetIsDeleteOperation();	
}

VOID CDuplicateSearch_Interface::ShowDuplicateSearchDlg()
{
	//===================================================
	//2008.08.26 UPDATE BY PJW : 복본조사결과가 없을경우에도 복본조사 다이얼로그 띄운다
//	CString tmpstr;
//	m_isDeleteOperation = FALSE;
//	m_arrAcqKey.RemoveAll();
//	
//	if(m_nSearchCount>= 1)
//	{
//		if(m_strSearchType == _T("중복조사"))
//		{
//			CBO_WORK_DUP_SEARCH Dlg(WORK_DUPLICATE_SEARCH, m_pParentMgr);
//			Dlg.SetAcqKeyArray(&m_arrAcqKey);
//			Dlg.DoModal();
//			m_isDeleteOperation = Dlg.GetIsDeleteOperation();
//		}
//		//복본조사시 인자 0:일괄복본조사, 1:서명복본조사, 2:정리일괄복본조사, 3:비치희망복본조사
//		else if(m_strSearchType == _T("복본조사"))
//		{
//			CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH);
//			Dlg.SetAcqKeyArray(&m_arrAcqKey);
//			Dlg.DoModal();
//			m_isDeleteOperation = Dlg.GetIsDeleteOperation();
//		}
//		else if(m_strSearchType == _T("정리복본조사"))
//		{
//			CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, CAT_DUPLICATE_SEARCH);
//			Dlg.DoModal();
//			m_isDeleteOperation = Dlg.GetIsDeleteOperation();
//		}
//		else if(m_strSearchType == _T("차수중복조사"))
//		{
//			CString strGroupInfo;
//			m_pGroupCollection->GetGroupInfo(strGroupInfo);
//			CBO_WORK_DUP_SEARCH Dlg(WORK_DUPLICATE_SEARCH_DIVNO, m_pParentMgr);
//			Dlg.SetAcqKeyArray(&m_arrAcqKey);
//			Dlg.SetAcqYear(m_pGroupCollection->GetAcqYear());
//			Dlg.SetGroupInfo(strGroupInfo);
//			Dlg.DoModal();
//			m_isDeleteOperation = Dlg.GetIsDeleteOperation();
//		}
//		else if(m_strSearchType == _T("차수복본조사"))
//		{
//			CString strGroupInfo;
//			m_pGroupCollection->GetGroupInfo(strGroupInfo);
//			CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH_DIVNO);
//			Dlg.SetAcqKeyArray(&m_arrAcqKey);
//			Dlg.SetAcqYear(m_pGroupCollection->GetAcqYear());
//			Dlg.SetGroupInfo(strGroupInfo);
//			Dlg.DoModal();
//			m_isDeleteOperation = Dlg.GetIsDeleteOperation();
//		}
//		else if(m_strSearchType == _T("기초자료복본조사"))
//		{
//			CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH_BASIC_DATA);
//			Dlg.SetAcqKeyArray(&m_arrAcqKey);
//			Dlg.DoModal();
//			m_isDeleteOperation = Dlg.GetIsDeleteOperation();
//		}
//		else if(m_strSearchType == _T("반입자료복본조사"))
//		{
//			CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH_IMPORT_DATA);
//			Dlg.SetAcqKeyArray(&m_arrAcqKey);
//			Dlg.DoModal();
//			m_isDeleteOperation = Dlg.GetIsDeleteOperation();
//		}
//		else if(m_strSearchType == _T("비치희망자료복본조사"))
//		{
//			CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH_REQUEST_DATA);
//			Dlg.DoModal();
//			m_isDeleteOperation = Dlg.GetIsDeleteOperation();
//		}
//	}
//	else
//	{
//		tmpstr.Format(_T("%s 결과가 없습니다."), m_strSearchType);
//		ESLAfxMessageBox(tmpstr);
//	}
	CString tmpstr;
	m_isDeleteOperation = FALSE;
	m_arrAcqKey.RemoveAll();
	
	if(m_nSearchCount < 1)
	{
		tmpstr.Format(_T("%s 결과가 없습니다."), m_strSearchType);
		ESLAfxMessageBox(tmpstr);
	}
	if(m_strSearchType == _T("중복조사"))
	{
		CBO_WORK_DUP_SEARCH Dlg(WORK_DUPLICATE_SEARCH, m_pParentMgr);
		Dlg.SetAcqKeyArray(&m_arrAcqKey);

		Dlg.m_strParentGridAlias = m_strGridAlias;
		Dlg.m_strParentDMAlias = m_strDMAlias;
		Dlg.m_strParentCMAlias = m_strCMAlias;
		Dlg.m_strSearchType = m_strSearchType;
		Dlg.m_pGroup = m_pGroup;
		Dlg.m_pGroupCollection = m_pGroupCollection;

		Dlg.DoModal();
		m_isDeleteOperation = Dlg.GetIsDeleteOperation();
	}
	//복본조사시 인자 0:일괄복본조사, 1:서명복본조사, 2:정리일괄복본조사, 3:비치희망복본조사
	else if(m_strSearchType == _T("복본조사"))
	{
		CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH);
		Dlg.SetAcqKeyArray(&m_arrAcqKey);

//		Dlg.m_pParentMgr = this->m_pParentMgr;
		Dlg.m_strParentGridAlias = m_strGridAlias;
		Dlg.m_strParentDMAlias = m_strDMAlias;
		Dlg.m_strParentCMAlias = m_strCMAlias;
		Dlg.m_strSearchType = m_strSearchType;
		Dlg.m_pGroup = m_pGroup;
		Dlg.m_pGroupCollection = m_pGroupCollection;
//  		Dlg.m_pDataProcessingDlg = m_pDataProcessingDlg;


		Dlg.DoModal();
		m_isDeleteOperation = Dlg.GetIsDeleteOperation();
	}
	else if(m_strSearchType == _T("정리복본조사"))
	{
		CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, CAT_DUPLICATE_SEARCH);

		Dlg.m_strParentGridAlias = m_strGridAlias;
		Dlg.m_strParentDMAlias = m_strDMAlias;
		Dlg.m_strParentCMAlias = m_strCMAlias;
		Dlg.m_strSearchType = m_strSearchType;
		Dlg.m_pGroup = m_pGroup;
		Dlg.m_pGroupCollection = m_pGroupCollection;

		Dlg.DoModal();
		m_isDeleteOperation = Dlg.GetIsDeleteOperation();
	}
	else if(m_strSearchType == _T("차수중복조사"))
	{
		CString strGroupInfo;
		m_pGroupCollection->GetGroupInfo(strGroupInfo);
		CBO_WORK_DUP_SEARCH Dlg(WORK_DUPLICATE_SEARCH_DIVNO, m_pParentMgr);
		Dlg.SetAcqKeyArray(&m_arrAcqKey);
		Dlg.SetAcqYear(m_pGroupCollection->GetAcqYear());
		Dlg.SetGroupInfo(strGroupInfo);

		Dlg.m_strParentGridAlias = m_strGridAlias;
		Dlg.m_strParentDMAlias = m_strDMAlias;
		Dlg.m_strParentCMAlias = m_strCMAlias;
		Dlg.m_strSearchType = m_strSearchType;
		Dlg.m_pGroup = m_pGroup;
		Dlg.m_pGroupCollection = m_pGroupCollection;

		Dlg.DoModal();
		m_isDeleteOperation = Dlg.GetIsDeleteOperation();
	}
	else if(m_strSearchType == _T("차수복본조사"))
	{
		CString strGroupInfo;
		m_pGroupCollection->GetGroupInfo(strGroupInfo);
		CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH_DIVNO);
		Dlg.SetAcqKeyArray(&m_arrAcqKey);
		Dlg.SetAcqYear(m_pGroupCollection->GetAcqYear());
		Dlg.SetGroupInfo(strGroupInfo);

		Dlg.m_strParentGridAlias = m_strGridAlias;
		Dlg.m_strParentDMAlias = m_strDMAlias;
		Dlg.m_strParentCMAlias = m_strCMAlias;
		Dlg.m_strSearchType = m_strSearchType;
		Dlg.m_pGroup = m_pGroup;
		Dlg.m_pGroupCollection = m_pGroupCollection;

		Dlg.DoModal();
		m_isDeleteOperation = Dlg.GetIsDeleteOperation();
	}
	else if(m_strSearchType == _T("기초자료복본조사"))
	{
		CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH_BASIC_DATA);
		Dlg.SetAcqKeyArray(&m_arrAcqKey);

		Dlg.m_strParentGridAlias = m_strGridAlias;
		Dlg.m_strParentDMAlias = m_strDMAlias;
		Dlg.m_strParentCMAlias = m_strCMAlias;
		Dlg.m_strSearchType = m_strSearchType;
		Dlg.m_pGroup = m_pGroup;
		Dlg.m_pGroupCollection = m_pGroupCollection;

		Dlg.DoModal();
		m_isDeleteOperation = Dlg.GetIsDeleteOperation();
	}
	else if(m_strSearchType == _T("반입자료복본조사"))
	{
		CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH_IMPORT_DATA);
		Dlg.SetAcqKeyArray(&m_arrAcqKey);

		Dlg.m_strParentGridAlias = m_strGridAlias;
		Dlg.m_strParentDMAlias = m_strDMAlias;
		Dlg.m_strParentCMAlias = m_strCMAlias;
		Dlg.m_strSearchType = m_strSearchType;
		Dlg.m_pGroup = m_pGroup;
		Dlg.m_pGroupCollection = m_pGroupCollection;

		Dlg.DoModal();
		m_isDeleteOperation = Dlg.GetIsDeleteOperation();
	}
	else if(m_strSearchType == _T("비치희망자료복본조사"))
	{
		CBO_HOLDING_ALL_DUP_SEARCH Dlg(m_pParentMgr, HOLDING_DUPLICATE_SEARCH_REQUEST_DATA);

		Dlg.m_strParentGridAlias = m_strGridAlias;
		Dlg.m_strParentDMAlias = m_strDMAlias;
		Dlg.m_strParentCMAlias = m_strCMAlias;
		Dlg.m_strSearchType = m_strSearchType;
		Dlg.m_pGroup = m_pGroup;
		Dlg.m_pGroupCollection = m_pGroupCollection;
		Dlg.DoModal();
		m_isDeleteOperation = Dlg.GetIsDeleteOperation();
	}
	//===================================================

	if(m_isDeleteOperation)
		SendRefreshMsg();
	
	return ;		
}

INT CDuplicateSearch_Interface::ESLAfxMessageBox(CString strMsg, UINT nType /*= MB_ICONEXCLAMATION*/)
{
	return MessageBox(strMsg, NULL, nType);
}

VOID CDuplicateSearch_Interface::SendRefreshMsg()
{
	::SendMessage(m_pParentMgr->GetSafeHwnd(), DUPLICATE_DATA_DELETE, 0, 0);
}

VOID CDuplicateSearch_Interface::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL)
	{
		m_pDataProcessingDlg = new CDataProcessingDlg(this);
		m_pPrgCtrl = m_pDataProcessingDlg->GetProgressCtrl();
	}
	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			m_pPrgCtrl = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//프로그래스 컨트롤 초기화
	m_pPrgCtrl->SetRange(0, 0);
	
	// 2007.06.08 REM BY PDJ
	// 인천중앙 특정 PC에서 오류 발생하는 코드임
	// 상관없는 코드임
	//m_pPrgCtrl->SetPos(0);
}

BOOL CDuplicateSearch_Interface::GetRunning()
{
	return m_bRunning;
}

VOID CDuplicateSearch_Interface::ShowWorkDuplicateSearchByDivno(CString strSearchType, CString strDMAlias, CString strCMAlias, CString strGridAlias, CGroupInfo * pGroup, CGroupCollection * pGroupCollection)
{
	m_strSearchType = strSearchType;
	m_strDMAlias = strDMAlias;
	m_strCMAlias = strCMAlias;
	m_strGridAlias = strGridAlias;
	m_pGroup = pGroup;
	m_pGroupCollection = pGroupCollection;

	ShowDataProcessingDlg();
	CString msg;
	msg = strSearchType + _T("를 수행하고 있습니다.");
	m_pDataProcessingDlg->SetMessage(msg);
	m_bRunning = TRUE;
	m_pThread = AfxBeginThread(SearchDuplicateData, this);
	
}

VOID CDuplicateSearch_Interface::ShowWorkDuplicateSearch(CString strSearchType, CString strDMAlias, CString strCMAlias, CString strGridAlias, CGroupCollection * pGroupCollection/*=NULL*/)
{
	m_strSearchType = strSearchType;
	m_strDMAlias = strDMAlias;
	m_strCMAlias = strCMAlias;
	m_strGridAlias = strGridAlias;
	m_pGroupCollection = pGroupCollection;

	ShowDataProcessingDlg();
	CString msg;
	msg = strSearchType + _T("를 수행하고 있습니다.");
	m_pDataProcessingDlg->SetMessage(msg);
	m_bRunning = TRUE;
	m_pThread = AfxBeginThread(SearchDuplicateData, this);	
}

INT CDuplicateSearch_Interface::SearchDuplicateDataByIndividually(CString strSearchType, 
																  CString strMarc, 
																  CString strMediaCode, 
																  CString strVol, 
																  CString strVolTItle, 
																  CString strShelfCode, 
																  CString strRegCode, 
																  CString strSearchKey,
																  CString &strResultCnt)
{
	if(strSearchType.GetLength() == 0) return -1000;
	if(strMarc.IsEmpty()) return -1000;
	
	CDuplicateSearchMgr Mgr(m_pParentMgr);
	//Mgr.SetSearchKeyInfo(_T(".\\duplicate_search_key.txt"));
	Mgr.SetKeyFileName(_T(".\\duplicate_search_key.txt"));
	Mgr.InitTempData();	Mgr.InitTempPtr();
	Mgr.SetTempData(_T("TargetDMAlias"), _T("DM_중복본조사"));
	Mgr.SetTempData(_T("CopyDMAlias"), _T("DM_중복본조사_복사본"));
	Mgr.SetTempData(_T("KeyDMAlias"), _T("DM_중복본조사키"));
	Mgr.SetTempData(_T("Marc"), strMarc.GetBuffer(0));
	Mgr.SetTempData(_T("MediaCodeInfo"), strMediaCode.GetBuffer(0));
	Mgr.SetTempData(_T("VolInfo"), strVol.GetBuffer(0));
	Mgr.SetTempData(_T("VolTitleInfo"), strVolTItle.GetBuffer(0));
	Mgr.SetTempData(_T("ShelfCodeInfo"), strShelfCode.GetBuffer(0));
	Mgr.SetTempData(_T("RegCodeInfo"), strRegCode.GetBuffer(0));
	Mgr.SetTempData(_T("SpeciesKey"), strSearchKey.GetBuffer(0));
	INT ids = Mgr.SPFExecutePath(strSearchType);
	if(ids < 0) return -1002;
	INT tmpcnt;
	Mgr.GetTempData(_T("SearchCount"), tmpcnt);
	strResultCnt.Format(_T("%d"), tmpcnt);
	
	return 0;
}
