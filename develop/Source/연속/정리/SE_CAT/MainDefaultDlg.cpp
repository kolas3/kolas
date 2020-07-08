// MainDefaultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDefaultDlg.h"

// 복본조사 키세팅
//#include _T("../../../공통/Duplicate_Data_Search/DUP_KEY_SETTING.h")

// 복본 해제
//#include _T("..\BO_CAT_DUP_FREE\BO_CAT_DUP_FREE2.h")

// 카드출력
#include "..\..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0202.h"

// 장비출력
//#include _T("..\..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0203.h")
#include "..\..\..\공통\CAT_PRINT\EquipPrintDlg.h"

// 원부류별 통계 출력
#include "..\..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0204.h"

// 교열지 출력
#include "..\..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0201.h"

// 배가일자 기록
//#include _T("..\..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_1200.h")
#include "..\..\..\공통\MarcAdjust\LocRecordDlg.h"

// 연속수서api
#include "..\..\수서\SE_API\SeApi.h"

// 표지관리
#include "..\..\..\공통\COVERMANAGER\COVER_MAIN.h"

//=====================================================
//2008.12.18 ADD BY PJW : 등록번호 재부여
#include "../../../공통/REG_NO_CHANGER/SE_REG_NO_CHANGER.h"
//=====================================================

//=====================================================
// 2009.04.15 ADD BY PJW : 원문입력
#include "../../../공통/CAT_DCM_MANAGER/DCM_MANAGER.h"
//=====================================================

// 마크반출
#include "..\SE_CAT_MARC_EXPORT\MarcExportDlg_SE.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDefaultDlg dialog


CMainDefaultDlg::CMainDefaultDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDefaultDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pGrid = NULL;
	m_pDuplicateSearchMgr = NULL;

	m_bCatHold = FALSE;
}

CMainDefaultDlg::~CMainDefaultDlg()
{

	//2008.10.15	UPDATE	BY	KSJ
	//RFID	Use	Tool
	HWND	RFIDHwnd	=	::FindWindow(NULL,_T("SmartID"));
	if( NULL !=	RFIDHwnd )
	{
		::PostMessage(RFIDHwnd,WM_MENU_CLOSE,0,0);
	}
}

BOOL CMainDefaultDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CMainDefaultDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDefaultDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMainDefaultDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDefaultDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnDUP_ShowSearchResult)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDefaultDlg Operations

VOID CMainDefaultDlg::MarcAdjust_Insert()
{
	EFS_BEGIN

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
	pDM_Where->GetCellData(_T("책조건절"), 0, strBookWhere);
	
	m_MarcAdjLaunCher.SetParentInfo(m_pDM, m_pGrid, _T("SS_종KEY"), strBookWhere, m_pGrid->GetRow() - 1);
	m_MarcAdjLaunCher.LaunchMarcAdjust(GetParentFrame(), this, _T("SEC111N"), MNG_INSERT, MODE_SERIAL);

	EFS_END
}

VOID CMainDefaultDlg::MarcAdjust_Modify()
{
	EFS_BEGIN

	if (m_pDM->GetRowCount() <= 0) return;

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
		pDM_Where->GetCellData(_T("책조건절"), 0, strBookWhere);
	
	// 050314 
	strBookWhere = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O')");

	m_MarcAdjLaunCher.SetParentInfo(m_pDM, m_pGrid, _T("SS_종KEY"), strBookWhere, m_pGrid->GetRow() - 1, m_bCatHold);
	m_MarcAdjLaunCher.LaunchMarcAdjust(GetParentFrame(), this, _T("SEC111N"), MNG_MODIFY, MODE_SERIAL);

	EFS_END
}

INT CMainDefaultDlg::DUP_Search()
{
	EFS_BEGIN

	m_pDuplicateSearchMgr->ShowWorkDuplicateSearch(_T("정리복본조사"), m_strMainDMAlias, m_strMainCMAlias, m_strMainGridAlias);
	
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::DUP_Free()
{
	EFS_BEGIN

//	CBO_CAT_DUP_FREE dlg(this);
//	dlg.SetParentInfo(m_strMainDMAlias, m_strMainCMAlias, m_strMainGridAlias);
//	dlg.DoModal();

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::DUP_KeySetting()
{
	EFS_BEGIN

//	CDUP_KEY_SETTING dlg(this, _T(".\\duplicate_search_key.txt"));
//	dlg.DoModal();

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::PRINT(CString strAlias)
{
	EFS_BEGIN

	if (strAlias == _T("카드목록"))
	{
		CBO_CAT_PROC_0202 dlg(this);
		dlg.m_nModeMONO_SERIAL = MODE_SERIAL;
		dlg.DoModal();
	}
	else if (strAlias == _T("원부류"))
	{
		CBO_CAT_PROC_0204 dlg(this);
		dlg.DoModal();
	}
	else if (strAlias == _T("장비"))
	{
		CEquipPrintDlg dlg(this);
		dlg.SetParentInfo(m_pDM, m_pGrid, _T("SS_종KEY"), 0 );
		dlg.m_nModeMONO_SERIAL = MODE_SERIAL;
		dlg.DoModal();
	}
	else if (strAlias == _T("교열지"))
	{
		CBO_CAT_PROC_0201 dlg(this);	//교열지 화면
		dlg.DoModal();
	}
	else if (strAlias == _T("열람인계목록"))
	{
		// DM 구성
		CCatApi_SE CatApi(this);
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		dm.InitDMPKFromFile(_T("DM_정리_출력_열람인계_연속"));
		dm.SetMarcMgr(m_pInfo->pMarcMgr);

		CString strBookWhere;
		CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
		if (pDM_Where)
			strBookWhere = pDM_Where->GetCellData(_T("책조건절"), 0);

		// From TBL 을 바꾼다.
		// 조인을 위한 테이블이 있나 검사한다.
		if (strBookWhere.Find(_T("a.")) >= 0)
		{
			// CO_ACCESSION_REC_TBL 가 있음
			dm.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
		}
		if (strBookWhere.Find(_T("p.")) >= 0)
		{
			// BO_PURCHASE_SEQ_NO_TBL 가 있음
			dm.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL p ");
		}

		// 종수를 구한다.
		CStringArray arrStrSpeciesKey;

		m_pGrid->SelectMakeList();
		if (m_pGrid->SelectGetCount() <= 0) 
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));
			return -1;
		}

		CString strSpeciesKey;
		
		INT idx = m_pGrid->SelectGetHeadPosition();
		while (idx >= 0)
		{
			// 선정된 로우의 종키를 가져온다.
			strSpeciesKey = m_pDM->GetCellData(_T("SS_종KEY"), idx);
			idx = m_pGrid->SelectGetNext();

			arrStrSpeciesKey.Add(strSpeciesKey);
		}

		// 각각의 종에대한 책을 가져와서 출력한다.
		CString strOption;
		strOption.Format(_T("b.SPECIES_KEY IN ( "));
		INT nRowCount = arrStrSpeciesKey.GetSize();
		for (idx = 0; idx < nRowCount; idx++)
		{
			strSpeciesKey = arrStrSpeciesKey.GetAt(idx);
			if (strSpeciesKey.IsEmpty()) continue;

			if (idx != nRowCount - 1)
				strOption += strSpeciesKey + _T(", ");
			else
				strOption += strSpeciesKey;
		}

		strOption += _T(") AND ") + strBookWhere;

		INT ids = dm.RefreshDataManager(strOption);
		if (ids < 0) return -1;

		// 돌면서 번호를 넣어준다.
		CString strNum;
		nRowCount = dm.GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			strNum.Format(_T("%d"), idx + 1);
			dm.SetCellData(_T("번호"), strNum, idx);
		}

		CSIReportManager SIReportManager(this);
		SIReportManager.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		ids = SIReportManager.GetSIReportFileFromDB(_T("연속_정리_열람인계목록"));
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return -1;
		}

		CString strCount;
		strCount.Format(_T("%d"), arrStrSpeciesKey.GetSize());
		SIReportManager.SetTempVariable(_T("종수"), strCount);
		strCount.Format(_T("%d"), dm.GetRowCount());
		SIReportManager.SetTempVariable(_T("책수"), strCount);

		SIReportManager.SetDataMgr(0, &dm, 0);
		
		ids = SIReportManager.Print();
	}

	return 0;

	EFS_END
	return -1;
}

// 040401_HSS 부록자료의 상태도 바꾸어 주어야 한다.
INT CMainDefaultDlg::UpdateWorkingStatus(LPCTSTR lpszFromStatus, LPCTSTR lpszToStatus, CString strUpdateFieldName /*= _T("")*/, CString strUpdateFieldData /*= _T("")*/)
{
	EFS_BEGIN

	INT ids = -1;
	CString strQuery;
	CString strSpeciesKey;
	CString strBookKey;
	CCatApi_SE CatApi(this);

	m_pDM->StartFrame();

	m_pGrid->SelectMakeList();
	if (m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}

	// 책 DM 구성
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dm.InitDMPKFromFile(_T("DM_연속_정리DM_책정보"));
	dm.SetMarcMgr(m_pInfo->pMarcMgr);

	// 040401_HSS 부록 DM 구성
	CESL_DataMgr dmApp;
	dmApp.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dmApp.InitDMPKFromFile(_T("DM_연속_정리DM_부록정보"));
	dmApp.SetMarcMgr(m_pInfo->pMarcMgr);

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
		strBookWhere = pDM_Where->GetCellData(_T("책조건절"), 0);

	//050314
	strBookWhere = _T(" b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O')");

	// From TBL 을 바꾼다.
	// 조인을 위한 테이블이 있나 검사한다.
	if (strBookWhere.Find(_T("a.")) >= 0)
	{
		// CO_ACCESSION_REC_TBL 가 있음
		dm.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
		dmApp.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
	}
	if (strBookWhere.Find(_T("p.")) >= 0)
	{
		// BO_PURCHASE_SEQ_NO_TBL 가 있음
		dm.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL p ");
		dmApp.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL p ");
	}
	
	INT nRowCount = 0;
	INT i = -1;
	INT idx = m_pGrid->SelectGetHeadPosition();
	while (idx >= 0)
	{
		strSpeciesKey.Empty();
		
		// 종키를 가져온다.
		strSpeciesKey = m_pDM->GetCellData(_T("ISS_종KEY"), idx);
		//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
		//KOL.RED.019-005 JY 연속간행물 기능 안정화
		//ADD=================================================================
		if (strSpeciesKey.IsEmpty())
		{	
			strSpeciesKey = m_pDM->GetCellData(_T("SS_종KEY"), idx);
		}
		if(strSpeciesKey.IsEmpty()) continue;
		//====================================================================
		idx = m_pGrid->SelectGetNext();

		// 종과 Book Where절에 해당하는 책 키들을 가져온다.
		strQuery.Format(_T("SPECIES_KEY = %s AND %s"), strSpeciesKey, strBookWhere);
		ids = dm.RefreshDataManager(strQuery);
		if (ids < 0) continue;
		ids = dmApp.RefreshDataManager(strQuery);
		if (ids < 0) continue;


		// 책을 돌면서 working status를 Update한다.
		nRowCount = dm.GetRowCount();
		for (i = 0; i < nRowCount; i++)
		{
			strBookKey = dm.GetCellData(_T("SB_책KEY"), i);
			if (!strUpdateFieldName.IsEmpty())
				strQuery.Format(_T("UPDATE SE_BOOK_TBL SET %s = '%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
								   _T("WHERE (REC_KEY = %s OR BINDING_BOOK_KEY = %s) AND WORKING_STATUS = '%s';")), 
							       strUpdateFieldName, strUpdateFieldData, lpszToStatus, lpszFromStatus, 
								   GetWorkLogMsg(_T("연속상태변경"),WORK_LOG), strBookKey, strBookKey, lpszFromStatus);
			else 
				strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
								   _T("WHERE (REC_KEY = %s OR BINDING_BOOK_KEY = %s) AND WORKING_STATUS = '%s';")), 
								   lpszToStatus, lpszFromStatus, GetWorkLogMsg(_T("연속상태변경"),WORK_LOG), strBookKey, strBookKey, lpszFromStatus);
			m_pDM->AddFrame(strQuery);
		}
		// 040401_HSS 부록을 돌면서 working status를 Update한다.
		nRowCount = dmApp.GetRowCount();
		for (i = 0; i < nRowCount; i++)
		{
			strBookKey = dmApp.GetCellData(_T("SB_책KEY"), i);
			if (!strUpdateFieldName.IsEmpty())
				strQuery.Format(_T("UPDATE SE_BOOK_TBL SET %s = '%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
								   _T("WHERE (REC_KEY = %s OR BINDING_BOOK_KEY = %s) AND WORKING_STATUS = '%s';")), 
							       strUpdateFieldName, strUpdateFieldData, lpszToStatus, lpszFromStatus, 
								   GetWorkLogMsg(_T("연속상태변경"),WORK_LOG), strBookKey, strBookKey, lpszFromStatus);
			else 
				strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
								   _T("WHERE (REC_KEY = %s OR BINDING_BOOK_KEY = %s) AND WORKING_STATUS = '%s';")), 
								   lpszToStatus, lpszFromStatus, GetWorkLogMsg(_T("연속상태변경"),WORK_LOG), strBookKey, strBookKey, lpszFromStatus);
			m_pDM->AddFrame(strQuery);
		}
	}

	// SendFrame
	ids = m_pDM->SendFrame();
	if (ids < 0) return ids;
	ids = m_pDM->EndFrame();
	if (ids < 0) return ids;

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Dup()
{
	EFS_BEGIN

	INT ids = -1;
	ids = UpdateWorkingStatus(_T("BOC1110"), _T("BOC113N")) ;
	if (ids < 0)
	{
		//AfxMessageBox(_T("실패하였습니다."));
		return -1;
	}
	AfxMessageBox(_T("복본조사 완료 처리하였습니다."));

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Dup2()
{
	EFS_BEGIN

	INT ids = -1;
	ids = UpdateWorkingStatus(_T("BOC111O"), _T("BOC112O"));
	if (ids < 0)
	{
		//AfxMessageBox(_T("실패하였습니다."));
		return -1;
	}
	AfxMessageBox(_T("복본조사 완료 처리하였습니다."));
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Classification()
{
	EFS_BEGIN

	INT ids = -1;
	ids = UpdateWorkingStatus(_T("BOC1120"), _T("BOC113N"));
	if (ids < 0)
	{
		//AfxMessageBox(_T("실패하였습니다."));
		return -1;
	}
	AfxMessageBox(_T("분류 완료 처리하였습니다."));
	return 0;

	EFS_END
	return -1;
}

// 040401_HSS 부록자료도 포함한다.
INT CMainDefaultDlg::Complete_Cataloging(BOOL bChangeWorkingStatus /*= TRUE*/)
{
	EFS_BEGIN

	CCatApi_SE CatApi(this);
	CatApi.InitMemberData();

	// DM 구성
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dm.InitDMPKFromFile(_T("DM_연속_정리DM_책정보"));
	dm.SetMarcMgr(m_pInfo->pMarcMgr);

	// 040401_HSS 부록 DM 구성
	CESL_DataMgr dmApp;
	dmApp.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dmApp.InitDMPKFromFile(_T("DM_연속_정리DM_부록정보"));
	dmApp.SetMarcMgr(m_pInfo->pMarcMgr);

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
		strBookWhere = pDM_Where->GetCellData(_T("책조건절"), 0);
	
	// 050311
	strBookWhere = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O')");


	// From TBL 을 바꾼다.
	// 조인을 위한 테이블이 있나 검사한다.
	if (strBookWhere.Find(_T("a.")) >= 0)
	{
		// CO_ACCESSION_REC_TBL 가 있음
		dm.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
		dmApp.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
	}
	
	// 선정된 자료 체크
	INT ids = -1;
	ids = m_pGrid->SelectMakeList();
	INT nSelectCount = m_pGrid->SelectGetCount();
	if (nSelectCount <= 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}

	// 선정된 종을 돌면서 체크를 한다.
	CString strSpeciesKey;
	CString strQuery;
	CString strItem;

	INT nIdx = -1;
	INT nRowCount, nRowCount_App;
	INT nSelectIndex = m_pGrid->SelectGetHeadPosition();
	while (nSelectIndex >= 0)
	{
		nIdx = nSelectIndex;
		nSelectIndex = m_pGrid->SelectGetNext();
		ids = m_pDM->GetCellData(_T("SS_종KEY"), nIdx, strSpeciesKey);
		if (ids < 0 || strSpeciesKey.IsEmpty()) continue;

		// 메인 그리드 - 제어구분 코드 확인
		ids = m_pDM->GetCellData(_T("SS_제어자료구분_코드"), nIdx, strItem);
		if (ids < 0 || strItem.IsEmpty())
		{
			AfxMessageBox(_T("제어자료구분 코드가 없습니다."));
			return -1;
		}
		
		// 책 Select
		strQuery.Format(_T("SPECIES_KEY = %s AND %s"), strSpeciesKey, strBookWhere);
		ids = dm.RefreshDataManager(strQuery);
		if (ids < 0) 
		{
			AfxMessageBox(_T("대상 책 검색 에러!"));
			return -1;
		}
		// 040401_HSS 부록 Select
		strQuery.Format(_T("SPECIES_KEY = %s AND %s"), strSpeciesKey, strBookWhere);
		ids = dmApp.RefreshDataManager(strQuery);
		if (ids < 0) 
		{
			AfxMessageBox(_T("대상 부록 검색 에러!"));
			return -1;
		}

		// 자료 수 확인
		nRowCount = dm.GetRowCount();
		nRowCount_App = dmApp.GetRowCount();
		if (nRowCount <= 0 && nRowCount_App <= 0)
		{
			AfxMessageBox(_T("대상이 되는 자료가 없습니다."));
			return -1;
		}	
		
		// 정리완료 대상 자료인지 체크 - 책
		INT idx;
		for (idx = 0; idx < nRowCount; idx++)
		{
			// 1. 대상책들에게 등록번호가 부여했는지 체크
			ids = dm.GetCellData(_T("SB_등록번호"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty()) 
			{
				AfxMessageBox(_T("등록번호가 부여되지 않은 책이 있습니다."));
				return -1;
			}			

			// 2. 배가실 코드 확인
			ids = dm.GetCellData(_T("SB_배가위치부호"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty())
			{
				AfxMessageBox(_T("배가실 코드가 부여되지 않은 책이 있습니다."));
				return -1;
			}
		}

		// 040401_HSS 정리완료 대상 자료인지 체크 - 부록
		for (idx = 0; idx < nRowCount_App; idx++)
		{
			// 1. 대상부록들에게 등록번호가 부여됬는지 체크
			ids = dmApp.GetCellData(_T("SB_등록번호"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty()) 
			{
				AfxMessageBox(_T("등록번호가 부여되지 않은 책이 있습니다."));
				return -1;
			}			

			// 2. 배가실 코드 확인
			ids = dmApp.GetCellData(_T("SB_배가위치부호"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty())
			{
				AfxMessageBox(_T("배가실 코드가 부여되지 않은 책이 있습니다."));
				return -1;
			}
		}

	}

	m_pDM->StartFrame();

	// 정리완료 작업을 시작한다.
	nSelectIndex = m_pGrid->SelectGetHeadPosition();
	while (nSelectIndex >= 0)
	{
		nIdx = nSelectIndex;
		nSelectIndex = m_pGrid->SelectGetNext();

		// 정리 완료를 부른다.
		ids = CatApi.CompleteCataloging(m_pDM, m_pDM, nIdx, NULL, FALSE);
	}

	ids = m_pDM->SendFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("DB와의 작업이 실패했습니다."));
		return -1;
	}
	ids = m_pDM->EndFrame();
	
	if (bChangeWorkingStatus)
		ids = UpdateWorkingStatus(_T("SEC111N"), _T("SEC112O"), _T("CAT_COMPLETE_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));
	else
		ids = UpdateWorkingStatus(_T("SEC111N"), _T("SEC111N"), _T("CAT_COMPLETE_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));

	
	if (ids < 0)
		return -1;
	
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Loc()
{
	EFS_BEGIN

	INT ids = -1;
	ids = UpdateWorkingStatus(_T("SEC112O"), _T("SEL211O"), _T("LOC_TRANSFER_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));
	if (ids < 0)
		return -1;
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Loc2()
{
	EFS_BEGIN

	INT ids = -1;
	/*// 열람인계자료로 변경
	ids = UpdateWorkingStatus(_T("SEL211O"), _T("SEL212N"));
	if (ids < 0)
	{
		//AfxMessageBox(_T("실패하였습니다."));
		return -1;
	}
	*/

	// 배가일자 기록 & 배가자료로 변경
	ids = Loc_Record();
	if (ids < 0)
	{
		return -1;
	}

	//AfxMessageBox(_T("열람인계 처리하였습니다."));
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Loc_Record()
{
	EFS_BEGIN

	/*
	INT ids = -1;
	CBO_CAT_PROC_1200 dlg(this);
	dlg.SetParentInfo(m_strMainDMAlias, m_strMainCMAlias, m_strMainGridAlias);
	if (dlg.DoModal() != IDOK) return -1;
	
	// 배가자료로 변경
	ids = UpdateWorkingStatus(_T("SEL211O"), _T("SEL212N"));
	*/

	// 배가일자 기록하는것 변경됨
	CLocRecordDlg dlg(this);
	dlg.m_nModeMONO_SERIAL = MODE_SERIAL;
	dlg.SetParentInfo(m_pDM, m_pGrid, _T("SS_종KEY"));
	if (dlg.DoModal() != IDOK) return -1;

	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMainDefaultDlg message handlers

BOOL CMainDefaultDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_연속_정리")) < 0)
		goto ERR;

	m_strMainGridAlias = _T("메인기본그리드");
	m_strMainDMAlias = _T("DM_연속_정리_검색결과");
	m_strMainCMAlias = _T("CM_연속_정리");

	m_pGrid = static_cast<CESL_Grid*>(FindControl(m_strMainCMAlias, m_strMainGridAlias));
	if (m_pGrid == NULL) goto ERR;
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem(static_cast<CView*>(pMain), this->VIEW_STYLE);

	m_pDM = FindDM(m_strMainDMAlias);
	if (m_pDM == NULL) goto ERR;

	// 복본조사 메니저
	m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
	m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	m_pDuplicateSearchMgr->ShowWindow(SW_HIDE);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	MessageBox(_T("다이얼로그 초기화 실패"));
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CMainDefaultDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;

	CRect rcClient;
	GetClientRect(rcClient);
	
	if (m_pGrid && m_pGrid->GetSafeHwnd())
		m_pGrid->MoveWindow(rcClient);	//수정안함
	

	EFS_END
}

VOID CMainDefaultDlg::OnDUP_ShowSearchResult(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();

	EFS_END
}

VOID CMainDefaultDlg::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();
	
	if (m_pDuplicateSearchMgr)
	{
		delete m_pDuplicateSearchMgr;
		m_pDuplicateSearchMgr = NULL;
	}

	EFS_END
}

BEGIN_EVENTSINK_MAP(CMainDefaultDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMainDefaultDlg)
	ON_EVENT(CMainDefaultDlg, IDC_GRID_CAT, -601 /* DblClick */, OnDblClickGridCat, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INT CMainDefaultDlg::SelectAgain()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM == NULL) return -1;
	if (pDM->GetRowCount() <= 0) return -1;

	CString strWhere = pDM->GetCellData(_T("조건절"), 0);
	

	// Select
	m_pDM->RefreshDataManager(strWhere);
	//시작 등록번호 소트
	m_pDM->SORT( _T("시작등록번호"));


	CString strMsg;

	INT nRowCount = m_pDM->GetRowCount();

	INT nBookCount = 0;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		nBookCount += _ttoi(m_pDM->GetCellData(_T("책수"), idx));
	}

	strMsg.Format(_T("종수 : %5d\t책수 : %5d"), nRowCount, nBookCount);

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));


	m_pGrid->Display();

	return 0;

	EFS_END
	return -1;
}

VOID CMainDefaultDlg::OnDblClickGridCat() 
{
	EFS_BEGIN

	if (m_pGrid->GetMouseRow() == 0) return;

	//===================================================
	//2009.05.25 UPDATE BY PJW : 메뉴 아이디와 alias모두 체크한다.
//	if (m_strMenuAlias == _T("연속_정리_목록완성"))
//		MarcAdjust_Modify();
	if (m_strMenuAlias == _T("2510") || m_strMenuAlias == _T("연속_정리_목록완성"))
	{
		MarcAdjust_Modify();
	}
	//===================================================
	

	EFS_END
}

// 041319_HSS 종삭제 추가
VOID CMainDefaultDlg::DeleteSpecies()
{
	m_pGrid->SelectMakeList();

	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
	    return;
	}
	
	CString msg;
	INT nCount = m_pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] 건의 자료를 삭제 하시겠습니까?\n삭제시 검색된 책 뿐 아니라 해당 종의 모든 자료가 삭제됩니다."),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 
	
	BeginWaitCursor();

	CSeApi api(this);
	CString strSPECIES_KEY, strVOL_KEY;
	CESL_DataMgr* pDM_SPECIES = FindDM(_T("DM_연속_서지정보관리_종"));
	CESL_DataMgr* pDM_SPECIES_DELETE = FindDM(_T("DM_연속_서지정보관리_삭제종"));
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_연속_서지정보관리_권"));


	CString strTITLE;
	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	while(idx>=0)
	{
		strSPECIES_KEY = m_pDM->GetCellData(_T("ISS_종KEY"),idx);
		
		ids = api.SPECIES_CAT_DELETE(strSPECIES_KEY,
							     pDM_SPECIES,
							     pDM_SPECIES_DELETE,
							     pDM_VOL);

		if(ids<0)
		{
			EndWaitCursor();
			strTITLE = m_pDM->GetCellData(_T("ISS_표제사항"),idx);

			if(-1001 == ids) //cancel
			{
				msg.Format(_T("[ %s ] 자료의 삭제를 취소하셨습니다!"),strTITLE);
				AfxMessageBox(msg);
			}
			else if(-1002 == ids)
			{
				// 특정한 이유로 삭제 못함
			}
			else
			{
				msg.Format(_T("[ %s ] 자료를 삭제하던 중 문제가 발생해 작업을 중단합니다!"),strTITLE);
				AfxMessageBox(msg);
				break;
			}
		}
		else 
		{
			arrayDeletedRow.Add(idx);
		}
		
		// 표지삭제 041130_소정추가
		for ( INT i=0; i< pDM_VOL->GetRowCount(); i++)
		{
			strVOL_KEY = pDM_VOL->GetCellData(_T("SV_권KEY"), i);
			
			if ( !strVOL_KEY.IsEmpty() )
			{
				CCOVER_MAIN dlg(this);
				dlg.DeleteCoverInfo ( strVOL_KEY.GetBuffer(0) ) ;
			}
		}

		idx = m_pGrid->SelectGetNext();
	}
	
	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		m_pDM->DeleteRow(nDeletedRow);
	}
	
	m_pGrid->Display();
	EndWaitCursor();

	msg.Format(_T("[ %d ] 건의 자료를 삭제하였습니다!"),nSize);
	AfxMessageBox(msg);

	return; 	

}

// 050103_redphil 바크반출
VOID CMainDefaultDlg::MarcExport()
{
EFS_BEGIN

	
	m_pGrid->SelectMakeList();
	if (m_pGrid->SelectGetCount() > 0 && AfxMessageBox(_T("선정된 자료가 있습니다. \r\n선정된 자료를 반출하시겠습니까? "), IDOK, IDCANCEL ) == IDOK ) 
	{	
		//선정된 데이터가 있을경우 선정된 데이터로 반출 가능하도록 수정
		MakeFileSelected( _T("CM_연속_정리"), _T("메인기본그리드") );
	}
	else
	{
		//선정된 데이터가 없을경우는 걍 반출 화면 띄운다
		CMarcExportDlg_SE *pDlg = new CMarcExportDlg_SE(this);
		pDlg->DoModal();
	}

	return;

EFS_END
return;
}

INT CMainDefaultDlg::MakeFileSelected(CString sCMAlias, CString sGridAlias)
{
	EFS_BEGIN

	INT idx;
	SelectMakeList(sCMAlias, sGridAlias);
	idx = SelectGetHeadPosition(sCMAlias, sGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return -1;		
	}

	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CFileDialog dlg(FALSE, _T("*.*"), _T("반출.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("파일형식(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return -1;

	_tchdir(szCurrentDir);

	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return -1;
    sFileName = dlg.GetPathName();	

	CESL_DataMgr * pDmRegMain;
	pDmRegMain = FindDM( _T("DM_연속_정리_검색결과") );
	if ( pDmRegMain == NULL ) {
		AfxMessageBox( _T("DM_연속_정리_검색결과 확인") );
		return -1;
	}

	CESL_DataMgr * pDM;
	pDM = FindDM( _T("DM_SE_CAT_PROC_1100") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_SE_CAT_PROC_1100 error") );
		return -1;
	}

	FILE *fp;
	fp = _tfopen( sFileName , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox(_T("파일을 선택하세요."));
		return -1;
	}

		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
		
	CString sBookKeyList;
	CStringArray aBookKeyList;
	while ( idx >= 0 ) {
		CString sBookKey = pDmRegMain->GetCellData( _T("ISS_종KEY") , idx );
		sBookKeyList = sBookKeyList + _T(", ") + sBookKey;

		if ( ((idx+1) % 999 ) == 0 ) {
			aBookKeyList.Add( sBookKeyList );
			sBookKeyList = _T("");
		}

		idx = SelectGetNext(sCMAlias, sGridAlias);
	}
	aBookKeyList.Add( sBookKeyList );

	INT iRowCount;
	INT iTotCount = 0;
	for ( INT j = 0 ; j < aBookKeyList.GetSize() ; j++ ) {
		sBookKeyList = aBookKeyList.GetAt(j).Mid(2);

		CString strWhere;
		strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" ) ORDER BY INPUT_DATE");
		if (pDM->RefreshDataManager(strWhere) < 0)	return -1;

		iRowCount = pDM->GetRowCount();
		iTotCount = iTotCount + iRowCount;

		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			CString sMarc;
			sMarc = pDM->GetCellData( _T("SS_MARC"), i-1 );
			CMarc marc;
			m_pInfo->pMarcMgr->Decoding( sMarc, &marc );
			m_pInfo->pMarcMgr->Encoding( &marc, sMarc, TRUE );

			sMarc += _T("\r\n");
			_ftprintf( fp, sMarc );
		}
	}

	fclose( fp );

	CString sMessage;
	sMessage.Format( _T("선정된 %d건에 대한 파일작성이 완료되었습니다."), iTotCount );
	AfxMessageBox( sMessage );

	return 0;
	EFS_END
	return -1;
}

//=====================================================
//2008.12.18 ADD BY PJW : 등록번호 재부여
INT CMainDefaultDlg::RegNoChang()
{
	EFS_BEGIN
	m_pGrid->SelectMakeList();
	m_pGrid->SetProtectSelectColumn(TRUE);
	INT nSelectCnt;
	nSelectCnt = 0;
	nSelectCnt = m_pGrid->SelectGetCount();
	if( nSelectCnt > 0 )
	{
		CSE_REG_NO_CHANGER dlg(this);
		dlg.SetParentDM(m_pDM);
		dlg.SetParentGRID(m_pGrid);
		dlg.DoModal();
	}
	else
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
	}
	this->m_pGrid->Display();
	this->m_pGrid->SetProtectSelectColumn(FALSE);
	
	return 0;

	EFS_END
	return -1;
 	
}
//=====================================================

//=====================================================
// 2009.04.15 ADD BY PJW : 원문입력
INT CMainDefaultDlg::InputDcm()
{
	EFS_BEGIN

//		CDCM_MANAGER dlg(this);
//		dlg.SetParentDM(this->m_pDM);
//		dlg.SetParentGRID(this->m_pGrid);
//		dlg.DoModal();
	
  	this->m_pGrid->SelectMakeList();
 	this->m_pGrid->SetProtectSelectColumn(TRUE);
	INT nSelectCnt;
	nSelectCnt = 0;
	nSelectCnt = this->m_pGrid->SelectGetCount();
	if( nSelectCnt > 0 )
	{
		CDCM_MANAGER dlg(this);
		dlg.SetParentDM(this->m_pDM);
		dlg.SetParentGRID(this->m_pGrid);
		dlg.SetDbFlag(_T("Y"));
		dlg.DoModal();
	}
	else
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
	}
 	this->m_pGrid->Display();
 	this->m_pGrid->SetProtectSelectColumn(FALSE);
	
	return 0;

	EFS_END
	return -1;
 	
}
//=====================================================

HBRUSH CMainDefaultDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
