// MainDefaultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDefaultDlg.h"

#include "../../../공통/Duplicate_Data_Search/DUP_KEY_SETTING.h"

#include "..\BO_CAT_DUP_FREE\BO_CAT_DUP_FREE2.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_0202.h"

#include "..\..\..\공통\CAT_PRINT\EquipPrintSaveDlg.h"

#include "..\..\..\공통\CAT_PRINT\EquipPrintDlg.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_0204.h"

#include "..\..\..\공통\CAT_PRINT\RevisionPrintDlg.h"

#include "..\..\..\공통\MarcAdjust\LocRecordDlg.h"

#include "..\..\..\공통\MarcAdjustApi\CatApi.h"

#include "..\..\..\공통\MarcAdjust\SpeciesDeletionDlg.h"

#include "..\..\..\공통\BO_DATA_CHANGE_MANAGER\CatCodeChange.h"

#include "..\..\..\공동목록\Light_BO_CAT_CENTERUPLOAD\BO_CAT_CENTERUPLOAD.h"

#include "..\WEB_HOLD_IMPORT\WebHoldImport.h"

#include "..\DMDP_IMPORT\DMDP_IMPORT1.h"

#include "..\BO_CAT_MARC_EXPORT\MarcExportDlg.h"

#include "..\\..\\..\\공통\\MarcConvertManager\\MarcConvertMgr.h"

#include "../../../공통/REG_NO_CHANGER/BO_REG_NO_CHANGER.h"

#include "../../../공통/CAT_DCM_MANAGER/DCM_MANAGER.h"

#include "ECO_FILE_API.h"
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

    m_Bo_ProgressDlg = NULL ;

}

CMainDefaultDlg::~CMainDefaultDlg()
{

	HWND	RFIDHwnd	=	::FindWindow(NULL,_T("SmartID"));
	if(	NULL	!=	RFIDHwnd)
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
	ON_MESSAGE(DUPLICATE_DATA_DELETE, SelectAgain)
	ON_MESSAGE(WM_USER+1980, OnRFID)
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
	
	m_MarcAdjLaunCher.SetParentInfo(m_pDM, m_pGrid, _T("BS_종KEY"), strBookWhere, m_pGrid->GetRow() - 1);
	m_MarcAdjLaunCher.LaunchMarcAdjust(GetParentFrame(), this);
	
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

	strBookWhere = _T("b.WORKING_STATUS IN ('BOL112N', 'BOL111O', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL116O', 'BOL252N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O', 'BOL611O','BOC111O','BOC112O', 'BOC113N', 'BOC114O', 'BOR111N',' BOR112N','BOR113O') AND BOOK_APPENDIX_FLAG = 'B'")  ;

	INT nGridIdx;
	CString strWorkingStatus;
	m_pGrid->SelectMakeList();
	if( m_pGrid->SelectGetCount() > 0 )
	{
		m_pGrid->SetAt( m_pGrid->GetIdx() , 0 , _T("V") );
		m_pGrid->SelectMakeList();
	}

	nGridIdx = m_pGrid->GetIdx();

	m_pDM->GetCellData(_T("IBS_작업상태INFO"), nGridIdx ,strWorkingStatus);
	if( strWorkingStatus.Find(_T("BOC")) >= 0 || strWorkingStatus == _T("BOL111O") )
	{
		m_bCatHold = FALSE;
	}
	else
	{
		m_bCatHold = TRUE;
	}
	m_MarcAdjLaunCher.SetParentInfo(m_pDM, m_pGrid, _T("BS_종KEY"), strBookWhere, m_pGrid->GetRow() - 1, m_bCatHold);

	LPCTSTR lpszWorkingStatus = NULL;
	if (m_strMenuAlias == _T("단행_정리_분류"))
	{
		lpszWorkingStatus = _T("BOC112O");
	}
    
	m_MarcAdjLaunCher.LaunchMarcAdjust(GetParentFrame(), this, lpszWorkingStatus, MNG_MODIFY);

	EFS_END
}

VOID CMainDefaultDlg::MarcAdjust_Delete()
{
	CSpeciesDeletionDlg dlg(this);
	dlg.SetParentInfo(m_pDM, m_pGrid, _T("BS_종KEY"), _T("IBS_표제사항"));
	if (dlg.DoModal() == IDOK)
		SelectAgain();
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

	CBO_CAT_DUP_FREE dlg(this);
	dlg.SetParentInfo(m_strMainDMAlias, m_strMainCMAlias, m_strMainGridAlias);
	dlg.DoModal();

	CString strFreeFalg = dlg.GetFreeFlag();
	if (!strFreeFalg.Compare(_T("Y"))) return 1;

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::DUP_KeySetting()
{
	EFS_BEGIN

	CDUP_KEY_SETTING dlg(this, _T(".\\duplicate_search_key.txt"));
	dlg.DoModal();

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::PRINT(CString strAlias)
{
	EFS_BEGIN

	INT ids;

	if (strAlias == _T("카드목록"))
	{
		CBO_CAT_PROC_0202 dlg(this);
		dlg.DoModal();
	}
	else if (strAlias == _T("원부류"))
	{
		CBO_CAT_PROC_0204 dlg(this);
		dlg.DoModal();
	}
	else if (strAlias == _T("출력자료보관"))
	{	
		CEquipPrintSaveDlg dlg(this);		
		CStringArray arrStrSpeciesKey;
		dlg.SetParentInfo(m_pDM, m_pGrid, _T("BS_종KEY"), &arrStrSpeciesKey);
		dlg.Init();
	}
	else if (strAlias == _T("보관된자료출력"))
	{
		CEquipPrintDlg dlg(this);		
		dlg.SetParentInfo(m_pDM, m_pGrid, _T("BS_종KEY"), 1);
		dlg.DoModal();		
	}
 	else if (strAlias == _T("장비"))
	{
		CEquipPrintDlg dlg(this);		
		
		dlg.SetParentInfo(m_pDM, m_pGrid, _T("BS_종KEY"), 0);
		dlg.DoModal();
	}
 	else if (strAlias == _T("기관부호출력"))
	{
		CEquipPrintPropertyDlg dlg(this, 2);
		dlg.DoModal();
	}
	else if (strAlias == _T("교열지"))
	{	
		CRevisionPrintDlg dlg(this);
		dlg.SetParentInfo(m_pDM, m_pGrid, _T("BS_종KEY"));
		dlg.DoModal();
	}
	else if (strAlias == _T("열람인계목록"))
	{
		CCatApi_BO CatApi(this);
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		dm.InitDMPKFromFile(_T("DM_정리_출력_열람인계_단행"));
		dm.SetMarcMgr(m_pInfo->pMarcMgr);

		CString strBookWhere;
		CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
		if (pDM_Where)
			strBookWhere = pDM_Where->GetCellData(_T("책조건절"), 0);

		if (strBookWhere.Find(_T("A.")) >= 0)
		{
			dm.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL A ");
		}
		if (strBookWhere.Find(_T("p.")) >= 0)
		{
			dm.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL A ");
		}

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
			strSpeciesKey = m_pDM->GetCellData(_T("BS_종KEY"), idx);
			idx = m_pGrid->SelectGetNext();

			arrStrSpeciesKey.Add(strSpeciesKey);
		}

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

		ids = dm.RefreshDataManager(strOption);
		if (ids < 0) return -1;

		CString strNum;
		nRowCount = dm.GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			strNum.Format(_T("%d"), idx + 1);
			dm.SetCellData(_T("번호"), strNum, idx);
		}

		CSIReportManager SIReportManager(this);
		SIReportManager.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		ids = SIReportManager.GetSIReportFileFromDB(_T("단행_정리_열람인계목록"));
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

INT CMainDefaultDlg::UpdateWorkingStatus(LPCTSTR lpszFromStatus, LPCTSTR lpszToStatus, CString strUpdateFieldName /*= _T("")*/, CString strUpdateFieldData /*= _T("")*/)
{
	EFS_BEGIN

	INT ids = -1;
	CString strQuery;
	CString strSpeciesKey;
	CString strBookKey;

	CCatApi_BO CatApi(this);
	
	CESL_DataMgr DM_Book;
	DM_Book.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	DM_Book.InitDMPKFromFile(_T("DM_단행_정리DM_책정보"));
	DM_Book.SetMarcMgr(m_pInfo->pMarcMgr);

	DM_Book.InitDMPKFromFile(_T("DM_단행_정리DM_책정보"));

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
		strBookWhere = pDM_Where->GetCellData(_T("책조건절"), 0);

	CString strAppendixTblName = DM_Book.TBL_NAME;
	CString strBookTblName = DM_Book.TBL_NAME;

	if (strBookWhere.Find(_T("A.")) >= 0)
	{
		strBookTblName += _T(" , CO_ACCESSION_REC_TBL A ");
	}
	if (strBookWhere.Find(_T("P.")) >= 0)
	{
		strBookTblName += _T(" , BO_PURCHASE_SEQ_NO_TBL P ");
	}

    if (strBookWhere.Find(_T("I.")) >= 0)
	{
		strBookTblName += _T(" , IDX_BO_TBL I ");
	}

	ids = m_pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}


	m_pGrid->SelectMakeList();
	if (m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
            m_Bo_ProgressDlg = NULL ;
			
		}
		return -1;
	}

	if(m_Bo_ProgressDlg != NULL)
	{
		m_Bo_ProgressDlg->m_ProgressCtrl->SetRange (0,m_pGrid->SelectGetCount() ) ;   
    }
	INT nRowCount = 0;
	INT i = -1;
	INT idx = m_pGrid->SelectGetHeadPosition();
	while (idx >= 0)
	{
		strSpeciesKey.Empty();
		
		strSpeciesKey = m_pDM->GetCellData(_T("IBS_종KEY"), idx);
		idx = m_pGrid->SelectGetNext();

		DM_Book.TBL_NAME = strBookTblName;
		strQuery.Format(_T("SPECIES_KEY = %s AND %s"), strSpeciesKey, strBookWhere);
		ids = DM_Book.RefreshDataManager(strQuery);
		if ( 0 > ids ) return -1;
   
		ids = UpdateWorkingStatus(m_pDM, &DM_Book, strUpdateFieldName, strUpdateFieldData, lpszToStatus, lpszFromStatus);
		if ( 0 > ids ) return -1;

        m_pDM->InitDBFieldData ();
		m_pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), lpszToStatus );
		ids = m_pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
		if ( 0 > ids ) return -1;
      
		CString sDate;
		CTime t = CTime::GetCurrentTime();
		sDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());


		if(m_Bo_ProgressDlg != NULL)
		{
			m_Bo_ProgressDlg->m_ProgressCtrl->StepIt () ;
		}
	}

	ids = m_pDM->SendFrame();
	if (ids < 0) return ids;
	ids = m_pDM->EndFrame();
	if (ids < 0) return ids;

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::UpdateWorkingStatus(CESL_DataMgr *pDM_Ouput, CESL_DataMgr *pDM_Book, CString strUpdateFieldName, CString strUpdateFieldData, LPCTSTR lpszToStatus, LPCTSTR lpszFromStatus)
{
	INT ids;
	if (!pDM_Ouput || !pDM_Book) return -1;
	
	CString strBookKey = _T("");
	CString strQuery = _T("");
	CString strPreWorkingStatus = _T("");

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT i = 0; i < nRowCount; i++)
	{
		strBookKey = pDM_Book->GetCellData(_T("BB_책KEY"), i);
		strPreWorkingStatus = pDM_Book->GetCellData(_T("BB_자료상태"), i);
		
		if (!strUpdateFieldName.IsEmpty())
		{
			strUpdateFieldData.MakeUpper();
			if( _T("WORKING_STATUS") == strUpdateFieldName )
			{
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET %s = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
				_T("WHERE REC_KEY = %s AND WORKING_STATUS = '%s';")), 
				strUpdateFieldName, strUpdateFieldData, strPreWorkingStatus, GetWorkLogMsg(_T("정리완료-열람인계-배가기록"),WORK_LOG), strBookKey, lpszFromStatus);
			ids = pDM_Ouput->AddFrame(strQuery);
			}
			else
			{
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET %s = '%s', LAST_WORK='%s' "
				_T("WHERE REC_KEY = %s AND WORKING_STATUS = '%s';")), 
				strUpdateFieldName, strUpdateFieldData, GetWorkLogMsg(_T("정리완료-열람인계-배가기록"),WORK_LOG), strBookKey, lpszFromStatus);
				ids = pDM_Ouput->AddFrame(strQuery);
			}
			
			if ( 0 > ids ) return -1;
		}
		
		if (_tcscmp(lpszFromStatus, _T("")) == 0)
		{
			if( _T("BOR112N") == lpszToStatus )
			{
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s', SHELF_DATE = NULL,  SHELF_WORKER = NULL "
					_T("WHERE REC_KEY = %s;")), 
					lpszToStatus, strPreWorkingStatus, GetWorkLogMsg(_T("정리완료-열람인계-배가기록"),WORK_LOG), strBookKey);
			}
			else
			{
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
					_T("WHERE REC_KEY = %s;")), 
					lpszToStatus, strPreWorkingStatus, GetWorkLogMsg(_T("정리완료-열람인계-배가기록"),WORK_LOG), strBookKey);
			}
		}
		else
		{
			if( _T("BOR112N") == lpszToStatus )
			{
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', SHELF_DATE = NULL,  SHELF_WORKER = NULL, LAST_WORK='%s' "
					_T("WHERE REC_KEY = %s;")), 
					lpszToStatus, strPreWorkingStatus, GetWorkLogMsg(_T("정리완료-열람인계-배가기록"),WORK_LOG), strBookKey);
			}
			else
			{
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
					_T("WHERE REC_KEY = %s;")), 
					lpszToStatus, strPreWorkingStatus, GetWorkLogMsg(_T("정리완료-열람인계-배가기록"),WORK_LOG), strBookKey);
			}	
		}
		
		ids = pDM_Ouput->AddFrame(strQuery);
		if ( 0 > ids ) return -1;
	}

	return 0;
}

INT CMainDefaultDlg::Complete_Dup()
{
	EFS_BEGIN

	m_Bo_ProgressDlg = new CBO_CAT_PROGRESS_DLG(this);
	m_Bo_ProgressDlg->Create (this);
    m_Bo_ProgressDlg->CenterWindow () ;
    m_Bo_ProgressDlg->ShowWindow (SW_SHOW);
	m_Bo_ProgressDlg->UpdateWindow() ;
	
	m_Bo_ProgressDlg->SetTitle (_T("정리 인계를 하고 있습니다."));
    m_Bo_ProgressDlg->m_ProgressCtrl->SetStep(1);
    m_Bo_ProgressDlg->m_ProgressCtrl->SetPos(0) ;

	INT ids = -1;
	ids = UpdateWorkingStatus(_T("BOC111O"), _T("BOC113N")) ;
	if (ids < 0)
	{
		return -1;
	}

	if(m_Bo_ProgressDlg != NULL)
	{
		delete m_Bo_ProgressDlg ;
		m_Bo_ProgressDlg = NULL ;
	}

	AfxMessageBox(_T("복본조사 완료 처리하였습니다."));

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Dup2()
{
	EFS_BEGIN

	m_Bo_ProgressDlg = new CBO_CAT_PROGRESS_DLG(this);
	m_Bo_ProgressDlg->Create (this);
    m_Bo_ProgressDlg->CenterWindow () ;
    m_Bo_ProgressDlg->ShowWindow (SW_SHOW);
	m_Bo_ProgressDlg->UpdateWindow () ;
	
	m_Bo_ProgressDlg->SetTitle (_T("정리 분류인계를 수행 하고 있습니다."));
    m_Bo_ProgressDlg->m_ProgressCtrl->SetStep(1);
    m_Bo_ProgressDlg->m_ProgressCtrl->SetPos(0) ;
	INT ids = -1;
	ids = UpdateWorkingStatus(_T("BOC111O"), _T("BOC112O"));
	if (ids < 0)
	{
		return -1;
	}

	if(m_Bo_ProgressDlg != NULL)
	{
		delete m_Bo_ProgressDlg ;
		m_Bo_ProgressDlg = NULL ;
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
	ids = UpdateWorkingStatus(_T("BOC112O"), _T("BOC113N"));
	if (ids < 0)
	{
		return -1;
	}
	AfxMessageBox(_T("분류 완료 처리하였습니다."));
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Cataloging(INT nCatalogMode , BOOL bChangeWorkingStatus /*= TRUE*/)
{
EFS_BEGIN

	if ( nCatalogMode < 0 ) return -1;

	CString sSpeciesKeyList = _T("");
	CCatApi_BO CatApi(this);
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dm.InitDMPKFromFile( _T("DM_단행_정리DM_책정보"));
	dm.SetMarcMgr(m_pInfo->pMarcMgr);

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
		strBookWhere = pDM_Where->GetCellData(_T("책조건절"), 0);

	if (strBookWhere.Find(_T("A.")) >= 0)
	{
		dm.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL A ");
	}
	if (strBookWhere.Find(_T("P.")) >= 0)
	{
		dm.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL P ");
	}

    if (strBookWhere.Find(_T("I.")) >= 0)
	{
		dm.TBL_NAME += _T(" , IDX_BO_TBL I ");
		strBookWhere += _T(" AND I.REC_KEY = B.SPECIES_KEY ");
	} 

	INT ids = -1;
	ids = m_pGrid->SelectMakeList();
	INT nSelectCount = m_pGrid->SelectGetCount();
	if (nSelectCount <= 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}
	
	INT nIdx = m_pGrid->SelectGetHeadPosition();

	CLocRecordDlg dlg(this);
	if ( nCatalogMode == SHELF_DATE_WRITE )
	{
		dlg.m_nModeMONO_SERIAL = MODE_MONO;
		dlg.SetParentInfo(m_pDM, m_pGrid, _T("BS_종KEY"), TRUE );
		if (dlg.DoModal() != IDOK) return -1;
	}
	ids = m_pGrid->SelectMakeList();
	nIdx = m_pGrid->SelectGetHeadPosition();

	m_Bo_ProgressDlg = new CBO_CAT_PROGRESS_DLG(this);
	m_Bo_ProgressDlg->Create (this);
	m_Bo_ProgressDlg->CenterWindow () ;
	m_Bo_ProgressDlg->ShowWindow (SW_SHOW);
	m_Bo_ProgressDlg->UpdateWindow() ;
	
	m_Bo_ProgressDlg->SetTitle (_T("작업중입니다. 잠시만 기다려 주시기 바랍니다."));
	m_Bo_ProgressDlg->m_ProgressCtrl->SetStep(1);
	m_Bo_ProgressDlg->m_ProgressCtrl->SetPos(0) ;

	if(m_Bo_ProgressDlg != NULL)
	{
		m_Bo_ProgressDlg->m_ProgressCtrl->SetRange (0,m_pGrid->SelectGetCount() ) ;   
    }

	while (nIdx >= 0)
	{	
		if(m_Bo_ProgressDlg != NULL)
		{
			m_Bo_ProgressDlg->m_ProgressCtrl->StepIt () ;
		}

		ids = m_pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}
		
		if ( !CheckComplete_Cataloging( &dm, nIdx , strBookWhere ) )
		{
			if(m_Bo_ProgressDlg != NULL) {	delete m_Bo_ProgressDlg ;  
			m_Bo_ProgressDlg = NULL ;}
			m_pDM->EndFrame();
			return -1;
		}

		if ( !SendQueryToDB( &CatApi, nIdx,  bChangeWorkingStatus) ) 
		{
			if(m_Bo_ProgressDlg != NULL) {	delete m_Bo_ProgressDlg ;  
			m_Bo_ProgressDlg = NULL ;}
			m_pDM->EndFrame();
			return -1;
		}
		
		if ( nCatalogMode == LOC_TRANSFER )
		{
			CString strSpeciesKey;
			m_pDM->GetCellData(_T("BS_종KEY"), nIdx, strSpeciesKey);

			ids = LocTransfer(strSpeciesKey, _T("BOC114O"), _T("BOL111O"), _T("LOC_TRANSFER_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));
			if (ids < 0) 
			{
				if(m_Bo_ProgressDlg != NULL) {	delete m_Bo_ProgressDlg ;  
				m_Bo_ProgressDlg = NULL ;}
				m_pDM->EndFrame();
				return -1;
			}
		}
		else if ( nCatalogMode == SHELF_DATE_WRITE )
		{
			CStringArray arrStrSpeciesKey;
			CStringArray arrStrBookKey;
			CString strSpeciesKey;
			CString strBookKey;
			CString strOption;

			m_pDM->GetCellData(_T("BS_종KEY"), nIdx, strSpeciesKey);
			arrStrSpeciesKey.Add( strSpeciesKey );

			if ( strBookWhere != _T("") ) 
				strOption.Format(_T(" SPECIES_KEY = %s AND %s AND B.SHELF_DATE IS NULL "), strSpeciesKey, strBookWhere);
			else 
				strOption.Format(_T(" SPECIES_KEY = %s AND B.SHELF_DATE IS NULL "), strSpeciesKey );

			ids = dm.RefreshDataManager(strOption);
			if (ids < 0) continue;
			
			INT nRowCount = dm.GetRowCount();
			for (INT idx = 0; idx < nRowCount; idx++)
			{
				ids = dm.GetCellData(_T("BB_책KEY"), idx, strBookKey);
				if (ids < 0 || strBookKey.IsEmpty()) continue;
				arrStrBookKey.Add(strBookKey);
			}
			
			CString strShelfDate;
			CTime t = CTime::GetCurrentTime();
			strShelfDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

			ids = CatApi.RecordLocDate(m_pDM, &arrStrSpeciesKey, &arrStrBookKey, NULL, strShelfDate, FALSE);
			if (ids < 0)
			{
				if(m_Bo_ProgressDlg != NULL) {	delete m_Bo_ProgressDlg ;  
				m_Bo_ProgressDlg = NULL ;}
				m_pDM->EndFrame();
				return -1;
			}

			sSpeciesKeyList = sSpeciesKeyList + _T("'") + strSpeciesKey + _T("' , ");
			
		}

		CString strQuery;
 		CString strSpeciesKey;
		CString strControlNo;
		CString strContentsKey;
		CString strData;
		INT nPos;
		INT nPosLast;
		
		m_pDM->GetCellData(_T("BS_종KEY"), nIdx, strSpeciesKey);
		m_pDM->GetCellData(_T("BS_제어번호"), nIdx, strControlNo);
		strQuery.Format(_T("SELECT CONTENTS_YN FROM IDX_BO_TBL WHERE REC_KEY = %s"),strSpeciesKey);
		m_pDM->GetOneValueQuery(strQuery,strData);
		if( _T("Y") == strData )
		{
			strQuery.Format(_T("SELECT CONTENTS_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"),strSpeciesKey);
			m_pDM->GetOneValueQuery(strQuery,strContentsKey);

			strQuery.Format(_T("SELECT CONTENTS FROM BO_CONTENTS_TBL WHERE REC_KEY = %s"),strContentsKey);
			m_pDM->GetOneValueQuery(strQuery,strData);
			if( !strData.IsEmpty() )
			{
				nPos = strData.Find(strControlNo);
				if( 0 > nPos )
				{
					nPos = strData.Find(_T("<id>"));
					nPosLast = strData.Find(_T("<"),4);
					if( 0 <= nPos )
					{
						strData = strData.Left(nPos+4) + strControlNo + strData.Mid(nPosLast);
						strData.Replace(_T("'"),_T("''"));
						m_pDM->InitDBFieldData();
						m_pDM->AddDBFieldData( _T("CONTENTS"), _T("STRING"), strData, 1 );
						m_pDM->MakeUpdateFrame( _T("BO_CONTENTS_TBL"), _T("REC_KEY"), _T("NUMERIC"), strContentsKey );		
					}
				}
			}
		}
		ids = m_pDM->SendFrame();
		if (ids < 0)
		{
			AfxMessageBox(_T("DB와의 작업이 실패했습니다."));
			
			if(m_Bo_ProgressDlg != NULL) {	delete m_Bo_ProgressDlg ;  
			m_Bo_ProgressDlg = NULL ;}
			m_pDM->EndFrame();
			return -1;
		}
		ids = m_pDM->EndFrame();
		if (ids < 0)
		{
			AfxMessageBox(_T("DB와의 작업이 실패했습니다."));

			if(m_Bo_ProgressDlg != NULL) {	delete m_Bo_ProgressDlg ;  
			m_Bo_ProgressDlg = NULL ;}

			return -1;
		}

		nIdx = m_pGrid->SelectGetNext();
	}
	if(m_Bo_ProgressDlg != NULL) {	delete m_Bo_ProgressDlg ;  
	m_Bo_ProgressDlg = NULL ;}

	if ( nCatalogMode == SHELF_DATE_WRITE && dlg.GetCheckCenterUpload() )
	{
		INT nCutLength = sSpeciesKeyList.GetLength();
		nCutLength = nCutLength - 2;

		sSpeciesKeyList = sSpeciesKeyList.Left(nCutLength );

		CString strBaseDM = _T("DM_CENTERUPLOAD_01");		
		CString strDupDM = _T("DM_CENTERUPLOAD_복본자료");
		
		CESL_DataMgr *pDM_Basic = FindDM(strBaseDM);
		if (pDM_Basic == NULL) return -1;
		

		CString strWhere;
		strWhere.Format(_T("I.REC_KEY  IN ( %s )"), sSpeciesKeyList);
		
		if (pDM_Basic->RefreshDataManager(strWhere) < 0) return -1;
		
		CBO_CAT_CENTERUPLOAD dlg(this);
		dlg.SetDMAlias(strBaseDM, strDupDM, _T(""));
		dlg.CenterUpload(FALSE) ;
	}

	return 0;

EFS_END
return -1;
}

INT CMainDefaultDlg::Complete_Loc()
{
EFS_BEGIN

	CCatApi_BO CatApi(this);
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dm.InitDMPKFromFile( _T("DM_단행_정리DM_책정보"));
	dm.SetMarcMgr(m_pInfo->pMarcMgr);

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
	strBookWhere = pDM_Where->GetCellData(_T("책조건절"), 0);

	if (strBookWhere.Find(_T("A.")) >= 0)
	{
		dm.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL A ");
	}
	if (strBookWhere.Find(_T("P.")) >= 0)
	{
		dm.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL P ");
	}

	if (strBookWhere.Find (_T("I.")) >= 0)
	{
		dm.TBL_NAME += _T(" , IDX_BO_TBL I ");
		strBookWhere += _T(" AND I.REC_KEY = B.SPECIES_KEY ");
	} 

	INT ids = -1;
	ids = m_pGrid->SelectMakeList();
	INT nSelectCount = m_pGrid->SelectGetCount();
	if (nSelectCount <= 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}

	INT nIdx = m_pGrid->SelectGetHeadPosition();
	while (nIdx >= 0)
	{	
		ids = m_pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}

		if ( !CheckComplete_Cataloging( &dm, nIdx , strBookWhere ) )
			return -1;
		
		if ( !SendQueryToDB( &CatApi, nIdx ) ) 
			return -1;
		

		CString strSpeciesKey;
		m_pDM->GetCellData(_T("BS_종KEY"), nIdx, strSpeciesKey);

		ids = LocTransfer(strSpeciesKey, _T("BOC114O"), _T("BOL111O"), _T("LOC_TRANSFER_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));
		if (ids < 0) return -1;


		ids = m_pDM->SendFrame();
		if (ids < 0)
		{
			AfxMessageBox(_T("DB와의 작업이 실패했습니다."));
			return -1;
		}
		
		ids = m_pDM->EndFrame();
		if (ids < 0)
		{
			AfxMessageBox(_T("DB와의 작업이 실패했습니다."));
			return -1;
		}
		
		nIdx = m_pGrid->SelectGetNext();
	}

	return 0;

	EFS_END
return -1;
}

INT CMainDefaultDlg::Loc_Record( )
{
	EFS_BEGIN

	INT ids = -1;
	
	CLocRecordDlg dlg(this);
	dlg.m_nModeMONO_SERIAL = MODE_MONO;
	dlg.SetParentInfo(m_pDM, m_pGrid, _T("BS_종KEY"), TRUE );
	if (dlg.DoModal() != IDOK) return -1;

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::SelectAgain()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	INT ids;
	if (pDM == NULL) return -1;
	if (pDM->GetRowCount() <= 0)
	{
		m_pDM->FreeData();
		m_pGrid->Display();
		return -1;
	}

	CString strWhere = pDM->GetCellData(_T("조건절"), 0);
	
	ids = m_pDM->RefreshDataManager(strWhere);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해 주세요"));
		return -1;
	}

	m_pDM->SORT( _T("시작등록번호"));

	CString strMsg;

	INT nRowCount = m_pDM->GetRowCount();

	INT nBookCount = 0;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		nBookCount += _ttoi(m_pDM->GetCellData(_T("책수"), idx));

		CString sCatalogStatus = m_pDM->GetCellData( _T("BS_정리상태"), idx );
		if ( sCatalogStatus == _T("0") ) {
			m_pDM->SetCellData( _T("BS_정리상태"), _T("정리안됨"), idx );
		}
		else if ( sCatalogStatus == _T("1") ) {
			m_pDM->SetCellData( _T("BS_정리상태"), _T("정리됨"), idx );
		}
	}

	strMsg.Format(_T("종수 : %5d\t책수 : %5d"), nRowCount, nBookCount);

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));

	m_pGrid->Display();

	return 0;

	EFS_END
	return -1;
}

VOID CMainDefaultDlg::RecoverySelect()
{	
	INT nSelect;
	CString strTemp;
	INT nCnt;
	nCnt = m_pGrid->SelectGetCount();
	nSelect = m_pGrid->SelectGetHeadPosition();	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		m_pGrid->SetTextMatrix( nSelect+1, 1,_T("V"));
		nSelect =m_pGrid->SelectGetNext();
	}	
}

BOOL CMainDefaultDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_단행_정리")) < 0)
		goto ERR;

	m_strMainGridAlias = _T("메인기본그리드");
	m_strMainDMAlias = _T("DM_단행_정리_검색결과");
	m_strMainCMAlias = _T("CM_단행_정리");

	m_pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_단행_정리"), _T("메인기본그리드")));
	if (m_pGrid == NULL) goto ERR;
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem(static_cast<CView*>(pMain), this->VIEW_STYLE);

	m_pDM = FindDM(m_strMainDMAlias);
	if (m_pDM == NULL) goto ERR;

	m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
	m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	m_pDuplicateSearchMgr->ShowWindow(SW_HIDE);

	{
		CString strValue;				
		
		m_pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='정리' AND GROUP_3='공통' AND CLASS_ALIAS='저자치환여부' AND MANAGE_CODE = UDF_MANAGE_CODE"), strValue );
		SetAuthorCodeChangeFlag(strValue);
	}
	
	{
		CString strPriorityCatalogYN;
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), strPriorityCatalogYN);
		
		if ( strPriorityCatalogYN != _T("Y") )
		{
			for (int i = 1 ; i < m_pGrid->GetColCount(); i++)
			{
				if( m_pGrid->GetTextMatrix( 0 , i ) == _T("") )
					m_pGrid->SetColWidth(i, 0, 0);
			}
			
		}
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	{
		CString strQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND GROUP_3 = '공통' AND CLASS_ALIAS = '통합도서서비스' AND VALUE_NAME = '통합도서서비스' AND MANAGE_CODE = UDF_MANAGE_CODE");
		
		CESL_DataMgr DM;
		DM.SetCONNECTION_INFO(_T(""));
		
		CString strValue;
		DM.GetOneValueQuery(strQuery, strValue);

		if(_T("Y") != strValue)
		{
			CString strButtonAlias = _T("기관부호출력");
			pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);
		}
	}


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
	
	if (m_pGrid && m_pGrid->GetSafeHwnd())
	{
		m_pGrid->MoveWindow(0,0,cx,cy);
	}

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
VOID CMainDefaultDlg::OnDblClickGridCat() 
{
	EFS_BEGIN

	if (m_pGrid->GetMouseRow() == 0) return;

	if ( m_strMenuAlias == _T("1510") ||
		m_strMenuAlias == _T("단행_정리_분류") )
	{
		MarcAdjust_Modify();	
	}

	EFS_END
}

VOID CMainDefaultDlg::OnRFID(WPARAM wParam, LPARAM IParam)
{
	TCHAR szSerial[32];
	_stprintf(szSerial, _T("%s\0"), IParam);

	CString sSerialNo;
	sSerialNo.Format( _T("%s"), szSerial );

	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CString sQuery;
	sQuery.Format( _T("SELECT WORKING_STATUS, ACCESSION_REC_KEY, SEQ_NO, SPECIES_KEY FROM BO_BOOK_TBL WHERE WORKNO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sSerialNo );
	DM_BOOK.RestructDataManager( sQuery );
	INT iBookCount = DM_BOOK.GetRowCount();

	CString sBookStatus = DM_BOOK.GetCellData( 0, 0 );
	CString sAccessionRecKey = DM_BOOK.GetCellData( 0, 1 );
	CString sSeqNo = DM_BOOK.GetCellData( 0, 2 );
	CString sSpeciesKey = DM_BOOK.GetCellData( 0, 3 );

	CString strWhere;
	if ( sBookStatus >= _T("BOL112N") && sBookStatus <= _T("BOL215O") ) {
		strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE WORKNO = '%s' ) AND I.REC_KEY = S.REC_KEY"), sSerialNo );
	}
	else {
		if ( sAccessionRecKey != _T("") ) {
			strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY = %s )"), sAccessionRecKey );
		}
		else {
			strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE WORKNO = '%s' )"), sSerialNo );
		}
	}
	if (m_pDM->RefreshDataManager(strWhere) < 0)	return;

	INT iRowCount = m_pDM->GetRowCount();
	
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_단행_정리"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_단행_정리 error") );
		return;
	}
	pCM->AllControlDisplay();
	for ( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ ) {
		if ( sSpeciesKey == m_pDM->GetCellData( _T("BS_종KEY"), i ) ) {
			break;
		}
	}
	SetReverse(_T("CM_단행_정리"), _T("메인기본그리드"), i);
}

VOID CMainDefaultDlg::OnRfidTest()
{
	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CString sQuery;
	sQuery.Format( _T("SELECT WORKING_STATUS, ACCESSION_REC_KEY, SEQ_NO FROM BO_BOOK_TBL WHERE REG_NO = 'EM0000070006' AND MANAGE_CODE = UDF_MANAGE_CODE") );
	DM_BOOK.RestructDataManager( sQuery );
	INT iBookCount = DM_BOOK.GetRowCount();

	CString sBookStatus = DM_BOOK.GetCellData( 0, 0 );
	CString sAccessionRecKey = DM_BOOK.GetCellData( 0, 1 );
	CString sSeqNo = DM_BOOK.GetCellData( 0, 2 );

	CString strWhere;
	if ( sBookStatus > _T("BOC124N") ) {
		strWhere = _T("I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE reg_no = 'EM0000070006' ) AND I.REC_KEY = S.REC_KEY");
	}
	else {
		if ( sAccessionRecKey != _T("") ) {
			strWhere.Format( _T("I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY = %s )"), sAccessionRecKey );
		}
		else {
			strWhere.Format( _T("I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE SEQ_NO = '%s' )"), sSeqNo );
		}
	}
	if (m_pDM->RefreshDataManager(strWhere) < 0)	return;

	INT iRowCount = m_pDM->GetRowCount();

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_단행_정리"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_단행_정리 error") );
		return;
	}
	pCM->AllControlDisplay();	
}

INT CMainDefaultDlg ::Delete049Tag () 
{
	EFS_BEGIN

	INT ids = -1;

    CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_검색결과"));
	if(pDM == NULL)
	{
		AfxMessageBox(_T("")); 
		return -1 ;
    }
	m_pGrid->SelectMakeList();
	if (m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}

	INT nRowCount = 0;
	INT i = -1;
	INT idx = m_pGrid->SelectGetHeadPosition();

	CMarc m_pMarc ;
	CString strSpeciesKey ;
	CString strStreamMarc ;
	CString strDel049Stream ;
    CString str049Data ;
	CString strQuery ;

    ids = m_pDM->StartFrame () ;	
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	while (idx >= 0)
	{
		strSpeciesKey.Empty();
		strStreamMarc.Empty () ;
		strDel049Stream.Empty () ;
        strQuery.Empty () ;
		
        strStreamMarc = m_pDM->GetCellData (_T("BS_MARC"),idx) ;
		strSpeciesKey = m_pDM->GetCellData (_T("IBS_종KEY"),idx);
		idx = m_pGrid->SelectGetNext();
     	
        	
		m_pInfo->pMarcMgr->Decoding (strStreamMarc,&m_pMarc)  ;	    
		m_pInfo->pMarcMgr->DeleteField (&m_pMarc,_T("049"),_T(""));
         
		m_pInfo->pMarcMgr->Encoding(&m_pMarc ,strDel049Stream) ;
        
		m_pDM->InitDBFieldData();
		m_pDM->AddDBFieldData( _T("MARC"), _T("STRING"), strDel049Stream, 1 );
		m_pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("자료복귀"), WORK_LOG), 1 );	
		INT idsTmp = m_pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey );
	}

    ids = m_pDM->SendFrame ();
	if ( ids < 0 ) {
		AfxMessageBox( _T("DM 저장 확인.") );
		return -1;
	}
	ids = m_pDM->EndFrame () ; 
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return -1;
	}



	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::ModifyCode ()
{
 
    CESL_DataMgr *m_pPrintDM = FindDM(m_strMainDMAlias );
	if(m_pPrintDM == NULL)
	{
		AfxMessageBox(_T("DM을 찾을 수 없습니다. "));
		return -1 ;
	}

	INT ids = m_pGrid->SelectMakeList();
	m_pGrid->SetProtectSelectColumn(TRUE);

	if(ids >= 0) ids = m_pGrid->SelectGetHeadPosition();	
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}
	CCatCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetMainGrid(m_pGrid);
	if(IDOK == dlg.DoModal())
	{
		DisplayGrid();
	}
	else
	{
		m_pGrid->SetProtectSelectColumn(FALSE);
		return -1;
	}
	m_pGrid->SetProtectSelectColumn(FALSE);

	return 0;
}

INT CMainDefaultDlg::Delete_DupFlag_Species()
{
    CESL_DataMgr *m_pMainDM = FindDM(m_strMainDMAlias );
	if(m_pMainDM == NULL)
	{
		AfxMessageBox(_T("DM을 찾을 수 없습니다. "));
		return -1 ;
	}

	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(m_pMainDM->CONNECTION_INFO);
	CString sQuery;

	CString strSpeciesKey ;
	CString strDupSpeciesKey ;
	CString sDup_Flag ;
    CString strDeleteSQL ;
    CString strDupFlagUpdateSql ;
    INT ids ;
	INT nIdx = -1;
	INT nRowCount;
	INT nSelectIndex = m_pGrid->SelectGetHeadPosition();
     
    ids = m_pMainDM->StartFrame ();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}


	while (nSelectIndex >=0)
	{
	    nIdx = nSelectIndex;
		nSelectIndex = m_pGrid->SelectGetNext();

		DM_BOOK.FreeData () ;

		ids = m_pMainDM->GetCellData(_T("BS_종KEY"), nIdx, strSpeciesKey);
		if (ids < 0 || strSpeciesKey.IsEmpty()) continue;

		ids = m_pMainDM->GetCellData(_T("BS_복본여부"), nIdx, sDup_Flag);
        if(sDup_Flag == 'V')
		{
            
			sQuery = _T("select distinct DUP_SPECIES_KEY from bo_book_tbl where species_key = ") +strSpeciesKey + _T(" AND DUP_SPECIES_KEY IS NOT NULL ");    
			DM_BOOK.RestructDataManager (sQuery);
            
			for(INT i = 0 ; i < DM_BOOK.GetRowCount ();i++)
			{
				DM_BOOK.GetCellData (i,0,strDupSpeciesKey);
                strDeleteSQL = _T(" ") ;
				strDeleteSQL.Format (_T("delete bo_species_tbl where rec_key = %s ;"),strDupSpeciesKey);

				ids = m_pMainDM->AddFrame (strDeleteSQL );
				if ( 0 > ids ) return -1;
				strDeleteSQL.Format (_T("delete idx_bo_tbl where rec_key = %s ;"),strDupSpeciesKey);
				ids = m_pMainDM->AddFrame (strDeleteSQL );
				if ( 0 > ids ) return -1;
			}

			
			strDupFlagUpdateSql.Format (_T("update bo_species_tbl set DUP_FLAG ='N', LAST_WORK='%s' where rec_key = %s;"),GetWorkLogMsg(_T("분류판사항관리"),WORK_LOG), strSpeciesKey) ;	
			ids = m_pMainDM->AddFrame (strDupFlagUpdateSql);
			if ( 0 > ids ) return -1;
		}

	}
    
	ids = m_pMainDM->SendFrame () ;
	if ( 0 > ids ) return -1;
	ids = m_pMainDM->EndFrame () ;
	if ( 0 > ids ) return -1;
    
   


	return 0 ;
}

INT CMainDefaultDlg::CheckReturnRegist ()
{
 
	INT nIdx = -1;

	m_pGrid->SelectMakeList() ;
	INT nSelectIndex = m_pGrid->SelectGetHeadPosition();
   
	CString strSerial_NO ;
	CString strDupFlag ;
	CString strSpeciesKey ;

	INT nSerialCnt = 0 ;
	INT nDupCnt = 0 ;

	CString strMsg ;
    CString strQuery ;
    CString nCount ;
    while (nSelectIndex >=0)
	{
	    nIdx = nSelectIndex;
		nSelectIndex = m_pGrid->SelectGetNext();
        
		m_pDM->GetCellData (_T("BS_종KEY"),nIdx,strSpeciesKey);
        strQuery.Format (_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s AND SERIAL_NO = -1"),strSpeciesKey );
        m_pDM->GetOneValueQuery (strQuery ,nCount) ;
        if(_ttoi(nCount)>0) strSerial_NO = _T("-1") ;

		m_pDM->GetCellData (_T("BS_복본여부"),nIdx,strDupFlag);
        
		if(strSerial_NO == _T("-1"))
		{
			m_pGrid->SetAt (nIdx,0,_T(""));
			nSerialCnt++;
			continue ;
        }
		if(!strDupFlag.IsEmpty())
        {
			m_pGrid->SetAt (nIdx,0,_T(""));
			nDupCnt++;
			continue ;
		}
	}

	if(nDupCnt > 0 || nSerialCnt>0 )
	{
		strMsg.Format(_T("복본 추기된 자료와 정리에서 입력된 자료는 자료복귀를 할 수 없습니다.\r\n 복본추기된자료 : %d 건 ,정리에서 입력된 자료 : %d 건 "),nDupCnt,nSerialCnt );
	    AfxMessageBox(strMsg);
		return 100;
	}
   
	return 0;
}

INT CMainDefaultDlg::LocTransfer(CString strSpeciesKey, 
								 LPCTSTR lpszFromStatus, 
								 LPCTSTR lpszToStatus, 
								 CString strUpdateFieldName /*= _T("")*/, 
								 CString strUpdateFieldData /*= _T("")*/)
{

	CString sQuery;
	INT ids = -1 ;

	sQuery.Format(_T("UPDATE BO_BOOK_TBL SET %s = '%s', WORKING_STATUS = '%s', "
		_T("PREV_WORKING_STATUS = '%s' , LAST_WORK = '%s' ")
		_T("WHERE SPECIES_KEY IN( %s ) AND WORKING_STATUS = '%s';")), 
		strUpdateFieldName, strUpdateFieldData, lpszToStatus,
		lpszFromStatus, GetWorkLogMsg(_T("열람인계"),WORK_LOG), 
		strSpeciesKey, lpszFromStatus );
	ids = m_pDM->AddFrame( sQuery );
	if ( 0 > ids ) return -1;	
	
	sQuery.Format(_T("UPDATE IDX_BO_TBL SET WORKING_STATUS = '%s' "
		_T("WHERE REC_KEY IN ( %s ) AND WORKING_STATUS = '%s';")),
		lpszToStatus, strSpeciesKey, lpszFromStatus );	
	ids = m_pDM->AddFrame( sQuery );	
	if ( 0 > ids ) return -1;

	sQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = '%s' "
		_T("WHERE SPECIES_KEY IN ( %s ) AND WORKING_STATUS = '%s' AND BOOK_APPENDIX_FLAG = 'A';")),
		lpszToStatus, strSpeciesKey, lpszFromStatus);	
	ids = m_pDM->AddFrame( sQuery );	
	if ( 0 > ids ) return -1;

	
	CString sDate;
	CTime t = CTime::GetCurrentTime();
	sDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	
	return 0;
}


BOOL CMainDefaultDlg::CheckComplete_Cataloging( CESL_DataMgr *pDM, INT nIdx, CString strBookWhere )
{
EFS_BEGIN

	if ( nIdx < 0 ) return FALSE;

	CString strSpeciesKey = _T("") ;
	CString strQuery = _T("") ;
	CString strItem = _T("") ;
	INT ids = -1;

	ids = m_pDM->GetCellData(_T("BS_종KEY"), nIdx, strSpeciesKey);
	if (ids < 0 || strSpeciesKey.IsEmpty()) 
	{
		AfxMessageBox(_T("종KEY 가 없습니다."));
		return FALSE;
	}

	
	ids = m_pDM->GetCellData(_T("BS_정리상태"), nIdx, strItem);
	if ( strItem == _T("정리안됨") ) 
	{
		AfxMessageBox(_T("정리안됨 자료는 처리할 수 없습니다."));
		m_pGrid->SetReverse(nIdx);
		return FALSE;
	}
	
	ids = m_pDM->GetCellData(_T("BS_제어자료구분_코드"), nIdx, strItem);
	if (ids < 0 || strItem.IsEmpty())
	{
		AfxMessageBox(_T("제어자료구분 코드가 없습니다."));
		return FALSE;
	}
	
	strQuery.Format(_T(" ( REG_NO IS NULL OR SHELF_LOC_CODE IS NULL ) AND SPECIES_KEY in (%s) AND B.BOOK_APPENDIX_FLAG = 'B'"), strSpeciesKey );
	
	if ( strBookWhere != _T("")) strQuery = strQuery + _T(" AND ") + strBookWhere;
		
	ids = pDM->RefreshDataManager(strQuery);

	INT nRowCount = pDM->GetRowCount();
	if ( nRowCount > 0 ) {

		strItem = _T("");
		CString sTmpMessage = _T("");
		m_pDM->GetCellData(_T("IBS_표제사항"), nIdx, strItem);

		sTmpMessage.Format(_T("'%s' 에 등록번호나 배가실 코드가 부여되지 않은 책이 있습니다. "), strItem );
		AfxMessageBox( sTmpMessage );
		return FALSE;
	}
	

	return TRUE;
	
EFS_END
return FALSE;
}

BOOL CMainDefaultDlg::SendQueryToDB(CCatApi *CatApi, INT nIdx, BOOL bChangeWorkingStatus)
{
EFS_BEGIN

	if ( nIdx < 0 ) return FALSE;

	INT ids = -1;
	CString strSpeciesKey = _T("");
	CString sQuery = _T("");

	ids = CatApi->CompleteCataloging(m_pDM, m_pDM, nIdx, NULL, FALSE);
	if ( 0 > ids ) return FALSE;

	ids = m_pDM->GetCellData(_T("BS_종KEY"), nIdx, strSpeciesKey);

	if (bChangeWorkingStatus) {
		ids = LocTransfer(strSpeciesKey, _T("BOC113N"), _T("BOC114O"), _T("CAT_COMPLETE_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));
	}
	else {
		ids = LocTransfer(strSpeciesKey, _T("BOC113N"), _T("BOC113N"), _T("CAT_COMPLETE_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));
	}

	if (ids < 0) return FALSE;

	return TRUE;
		
EFS_END
return FALSE;
}

VOID CMainDefaultDlg::MarcExport()
{
EFS_BEGIN

	
	m_pGrid->SelectMakeList();

	if (m_pGrid->SelectGetCount() > 0  ) 
	{	
		if( AfxMessageBox(_T("선정된 자료가 있습니다. \r\n선정된 자료를 반출하시겠습니까? "), IDOK, IDCANCEL ) == IDOK )
		{
			MakeFileSelected( _T("CM_단행_정리"), _T("메인기본그리드") );
		}
	}
	else
	{
		CMarcExportDlg *pDlg = new CMarcExportDlg(this);
		pDlg->DoModal();

		if(pDlg != NULL)
		{
			delete pDlg ;
			pDlg = NULL ;
		}
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
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;		
	}

	CMarcConvertMgr MarcConvertMgr(this);
	if ( MarcConvertMgr.DoModal() != IDOK ) return 0;		

	CString sFileName;
	INT		nEncodingType;
    sFileName = MarcConvertMgr.GetPath();
	nEncodingType = MarcConvertMgr.GetEncodingType();

	// 
	CESL_DataMgr * pDmRegMain;
	pDmRegMain = FindDM( _T("DM_단행_정리_검색결과") );
	if ( pDmRegMain == NULL ) {
		AfxMessageBox( _T("DM_단행_정리_등록_메인 확인") );
		return -1;
	}

	CESL_DataMgr * pDM;
	pDM = FindDM( _T("DM_BO_CAT_PROC_1100") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1100 error") );
		return -1;
	}

	FILE *file;
	if ( nEncodingType == UTF16_MARC )
	{
		file = FdkFopen( sFileName, _T("w") );
		if ( file ==NULL )
		{
			AfxMessageBox(_T("반출화일 신규작성에 실패하였습니다"));
			fclose(file);
			return -1;
		}
	}
	else if ( nEncodingType == UTF8_MARC )
	{
		file = _tfopen ( sFileName , _T("w"));
		if ( file == NULL )
		{
			AfxMessageBox(_T("반출화일 신규작성에 실패하였습니다"));
			fclose(file);
			return -1;			
		}
		BYTE pb[3] = { 0xEF, 0xBB, 0xBF };
		fwrite(pb,1,3,file );
	}
	else 
	{
		file = _tfopen ( sFileName , _T("w"));
		if ( file == NULL )
		{
			AfxMessageBox(_T("반출화일 신규작성에 실패하였습니다"));
			fclose(file);
			return -1;			
		}		
	}
			
	CString sBookKeyList;
	CStringArray aBookKeyList;
	while ( idx >= 0 ) {
		CString sBookKey = pDmRegMain->GetCellData( _T("IBS_종KEY") , idx );
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
	for ( INT j = 0 ; j < aBookKeyList.GetSize() ; j++ ) 
	{
		sBookKeyList = aBookKeyList.GetAt(j).Mid(2);

		CString strWhere;
		strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" ) ORDER BY INPUT_DATE");
		if (pDM->RefreshDataManager(strWhere) < 0)	return -1;
		CMarc pMarc;
		iRowCount = pDM->GetRowCount();
		iTotCount = iTotCount + iRowCount;
		for ( INT i = 1 ; i <= iRowCount ; i++ ) 
		{
			CString sMarc;
			sMarc = pDM->GetCellData( _T("BS_MARC"), i-1 );						

			if ( ANSI_MARC == nEncodingType )
			{
				m_pInfo->pMarcMgr->Decoding( sMarc, &pMarc );			
				m_pInfo->pMarcMgr->m_nEncodingCharset = CP_ACP;
				m_pInfo->pMarcMgr->Encoding( &pMarc, sMarc, TRUE );

				CHAR *szpMarc;				
				szpMarc = NULL;
				MarcConvertMgr.UnicodeToAnsi(sMarc, &szpMarc);
				fprintf( file, "%s\n", szpMarc );
				free(szpMarc);
			}
			else if ( UTF8_MARC == nEncodingType )
			{
				m_pInfo->pMarcMgr->Decoding( sMarc, &pMarc );			
				m_pInfo->pMarcMgr->m_nEncodingCharset = CP_UTF8;
				m_pInfo->pMarcMgr->Encoding( &pMarc, sMarc, TRUE );

				CHAR *szpMarc;
				szpMarc = NULL;
				MarcConvertMgr.UnicodeToUTF8(sMarc, &szpMarc);

				fprintf( file, "%s\n", szpMarc );
				free(szpMarc);
			}			
			else 
			{
				m_pInfo->pMarcMgr->Decoding( sMarc, &pMarc );			
				m_pInfo->pMarcMgr->m_nEncodingCharset = -1;
				m_pInfo->pMarcMgr->Encoding( &pMarc, sMarc, TRUE );
				FdkFprintf( file, _T("%s\r\n"), sMarc );
			}
		}
	}

	if ( nEncodingType == UTF16_MARC )
	{
		FdkFclose( file );
	}
	else
	{
		fclose( file );
	}

	m_pInfo->pMarcMgr->m_nEncodingCharset = CP_UTF8;

	

	CString sMessage;
	sMessage.Format( _T("선정된 %d건에 대한 파일작성이 완료되었습니다."), iTotCount );
	AfxMessageBox( sMessage );

	return 0;
	EFS_END
	return -1;
}

VOID CMainDefaultDlg::WebImport()
{
	CWebHoldImport dlg(this);
	dlg.DoModal();
	return ;
}

VOID CMainDefaultDlg::DMDPImport()
{
	CDMDP_IMPORT dlg(this);
	dlg.DoModal();
	return ;
}

INT CMainDefaultDlg::RegNoChang()
{
	EFS_BEGIN
 	this->m_pGrid->SelectMakeList();
 	this->m_pGrid->SetProtectSelectColumn(TRUE);
	INT nSelectCnt;
	nSelectCnt = 0;
	nSelectCnt = this->m_pGrid->SelectGetCount();
	if( nSelectCnt > 0 )
	{
		CBO_REG_NO_CHANGER dlg(this);
		dlg.SetParentDM(this->m_pDM);
		dlg.SetParentGRID(this->m_pGrid);
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

INT CMainDefaultDlg::InputDcm()
{
	EFS_BEGIN

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
		dlg.SetDbFlag(_T("M"));
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

INT CMainDefaultDlg::SetAuthorCodeChangeFlag(CString strValue)
{
	CStdioFile	file;
	CString		strFileName;
	CString		strLine;
	BOOL		bCheck;

	bCheck = TRUE;
	strFileName = _T("..\\CFG\\저자치환여부.cfg");
	if(!file.Open (strFileName,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ) ) 
	{		
		return -1;
	}
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
	
	file.WriteString(strValue);
	file.Close();
		
	return 0;
}

HBRUSH CMainDefaultDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

VOID CMainDefaultDlg::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetRow();
	INT nTopRow = m_pGrid->GetTopRow();
	if( nCurSel < 0 )						bSetTopRow = FALSE;
	if( m_pDM->GetRowCount() == 0 )			bSetTopRow = FALSE;
	if( nCurSel > m_pDM->GetRowCount() )	bSetTopRow = FALSE;

	m_pGrid->Display();
	if( bSetTopRow == TRUE )	
	{
		m_pGrid->SetTopRow(nTopRow);
		m_pGrid->SetReverse(nCurSel-1);
	}
}