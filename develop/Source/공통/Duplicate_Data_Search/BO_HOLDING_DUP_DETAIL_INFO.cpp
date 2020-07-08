// BO_HOLDING_DUP_DETAIL_INFO.cpp : implementation file
//

#include "stdafx.h"
#include "BO_HOLDING_DUP_DETAIL_INFO.h"
#include "../BO_VOLUME_FORM/BOVolumeDlg.h"
#include "../BO_SPECIES_API/BOFormApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORK_LOG GetWorkLogMsg(_T("복본자료상세보기"), _T(__FILE__), __LINE__)

/////////////////////////////////////////////////////////////////////////////
// CBO_HOLDING_DUP_DETAIL_INFO dialog


CBO_HOLDING_DUP_DETAIL_INFO::CBO_HOLDING_DUP_DETAIL_INFO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_HOLDING_DUP_DETAIL_INFO::IDD, pParent)
{
	AfxInitRichEdit();	
	//{{AFX_DATA_INIT(CBO_HOLDING_DUP_DETAIL_INFO)
	m_SrcFormCode = _T("");
	m_DupFormCode = _T("");
	m_SrcManageNo = _T("");
	m_DupManageNo = _T("");
	m_SrcMediaCode = _T("");
	m_DupMediaCode = _T("");
	m_DupAllData = _T("");
	m_DupSelData = _T("");
	m_SrcAllData = _T("");
	m_SrcSelData = _T("");
	//}}AFX_DATA_INIT
	m_bIsProcDuplicateInsert = FALSE;
	m_bGridSelectYN = FALSE;
	m_nOpenMode = OPERATION_TYPE_INVALID;
	m_pMainDM = NULL;
	m_pMainGrid = NULL;
	m_pSourceBookDM = NULL;
	m_pDuplicateBookDM = NULL;
	m_pBOFormApi = NULL;
	m_pSpeciesApi = NULL;
	m_bSavable = FALSE;
	m_bDupProc = FALSE;
	m_bShowVolumeType = TRUE;
	m_bDupProcType = FALSE;

	// KOL.UDF.022 시큐어코딩 보완
	m_nSrcIdx = -1;
	m_nDupIdx = -1;
	m_nSrcFirstIdx = -1;
	m_nDupFirstIdx = -1;
	m_nSrcLastIdx  = -1;
	m_nDupLastIdx  = -1;
	m_InfoType = -1;	
}

CBO_HOLDING_DUP_DETAIL_INFO::~CBO_HOLDING_DUP_DETAIL_INFO()
{	
	InitDupProcInfo(m_nSrcIdx, -1);
	if(m_pBOFormApi)
	{
		delete m_pBOFormApi;
		m_pBOFormApi = NULL;
	}
	if(m_pSpeciesApi)
	{
		delete m_pSpeciesApi;
		m_pSpeciesApi = NULL;
	}
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_HOLDING_DUP_DETAIL_INFO)
	DDX_Text(pDX, IDC_edtFORM_CLASS, m_SrcFormCode);
	DDX_Text(pDX, IDC_edtFORM_CLASS_SEL, m_DupFormCode);
	DDX_Text(pDX, IDC_edtMANAGEMENT_NO, m_srcStatus);
	DDX_Text(pDX, IDC_edtMANAGEMENT_NO_SEL, m_dupStatus);
	DDX_Text(pDX, IDC_edtMED_CLASS, m_SrcMediaCode);
	DDX_Text(pDX, IDC_edtMED_CLASS_SEL, m_DupMediaCode);
	DDX_Text(pDX, IDC_edtGUN2, m_DupAllData);
	DDX_Text(pDX, IDC_edtJAE2, m_DupSelData);
	DDX_Text(pDX, IDC_edtGUN, m_SrcAllData);
	DDX_Text(pDX, IDC_edtJAE, m_SrcSelData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_HOLDING_DUP_DETAIL_INFO, CDialog)
	//{{AFX_MSG_MAP(CBO_HOLDING_DUP_DETAIL_INFO)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnPRE_SEL, OnbtnPRESEL)
	ON_BN_CLICKED(IDC_btnNEXT_SEL, OnbtnNEXTSEL)
	ON_BN_CLICKED(IDC_btnPREV_SRC, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT_SRC, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnSHOW_SRCVOL, OnbtnSHOWSRCVOL)
	ON_BN_CLICKED(IDC_btnSHOW_DUPVOL, OnbtnSHOWDUPVOL)
	ON_BN_CLICKED(IDC_btnDUPDATA_INSERT, OnbtnDUPDATAINSERT)
	ON_BN_CLICKED(IDC_btnDUPDATA_INSERT_CANCEL, OnbtnDUPDATAINSERTCANCEL)
	ON_BN_CLICKED(IDC_btnCHANGE_SAVE, OnbtnCHANGESAVE)
	ON_MESSAGE(WM_USER + 32015, Create505Tag)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_HOLDING_DUP_DETAIL_INFO message handlers

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_HOLDING_DUP_DETAIL_INFO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_복본자료상세보기")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	m_pSourceBookDM = FindDM(_T("DMUP_원본_책정보"));
	m_pDuplicateBookDM = FindDM(_T("DMUP_복본_책정보"));
	m_marceditor_src.SubclassDlgItem(IDC_richHOLDING_SOURCE, -1, this);
	m_marceditor_dup.SubclassDlgItem(IDC_richHOLDING_DUPLICATE, -1, this);
	
	//pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	Display(0);	
	if(m_InfoType != CAT_DUPLICATE_SEARCH)
	{
		GetDlgItem(IDC_btnDUPDATA_INSERT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDUPDATA_INSERT_CANCEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCHANGE_SAVE)->ShowWindow(SW_HIDE);
	}
	else
	{
		ShowDuplicateDataInsertButtonCtrl();
	}
	
	m_pSpeciesApi = new CBO_SPECIES(this);
	m_pBOFormApi = new CBOFormApi(this);
	m_cCopyCodeMgr.Init(m_pMainDM->CONNECTION_INFO);
	m_pBOFormApi->SetCopyCodeMgr(&m_cCopyCodeMgr);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnbtnPRESEL() 
{
	// TODO: Add your control notification handler code here
	Display(-1000);
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnbtnNEXTSEL() 
{
	// TODO: Add your control notification handler code here
	Display(1000);
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnbtnPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnbtnNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::Display(INT nIdx)
{
	BOOL bSrcChange = FALSE;
	switch(nIdx)
	{
	case 0:
		InitFirstLastIndex();
		GetFirstSelectIndex(m_nSrcIdx, m_nDupIdx);
		ShowSrcNextPrevButton(m_nSrcIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = TRUE;
		break;
	case -1:
		InitDupProcInfo(m_nSrcIdx, m_nDupIdx);
		GetPrevSelectIndex(m_nSrcIdx, m_nDupIdx);
		ShowSrcNextPrevButton(m_nSrcIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = TRUE;
		break;
	case 1:
		InitDupProcInfo(m_nSrcIdx, m_nDupIdx);
		GetNextSelectIndex(m_nSrcIdx, m_nDupIdx);
		ShowSrcNextPrevButton(m_nSrcIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = TRUE;
		break;
	case 1000:
		InitDupProcInfo(m_nSrcIdx, m_nDupIdx);
		GetNextDupIndex(m_nDupIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = FALSE;
		break;
	case -1000:
		InitDupProcInfo(m_nSrcIdx, m_nDupIdx);
		GetPrevDupIndex(m_nDupIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = FALSE;
		break;
	default:
		return ;
	}

	CString srcmarc, dupmarc, strQuery;
	INT ids = m_pMainDM->GetCellData(_T("IBS_종KEY"), m_nSrcIdx, m_SrcManageNo);
	ids = m_pMainDM->GetCellData(_T("IBS_종KEY"), m_nDupIdx, m_DupManageNo);
	ids = m_pMainDM->GetCellData(_T("UDF_자료상태설명"), m_nSrcIdx, m_srcStatus);
	ids = m_pMainDM->GetCellData(_T("UDF_자료상태설명"), m_nDupIdx, m_dupStatus);
	ids = m_pMainDM->GetCellData(_T("IBS_형식구분"), m_nSrcIdx, m_SrcFormCode);
	ids = m_pMainDM->GetCellData(_T("IBS_형식구분"), m_nDupIdx, m_DupFormCode);
	//===================================================
	//2009.03.23 UPDATE BY PJW : INDEX가 잘못들어가 있었음
// 	ids = m_pMainDM->GetCellData(_T("IBS_매체구분"), m_nDupIdx, m_SrcMediaCode);
	ids = m_pMainDM->GetCellData(_T("IBS_매체구분"), m_nSrcIdx, m_SrcMediaCode);
	//===================================================	
	
	ids = m_pMainDM->GetCellData(_T("IBS_매체구분"), m_nDupIdx, m_DupMediaCode);
	ids = m_pMainDM->GetCellData(_T("선택자료"), m_nDupIdx, m_DupAllData);
	ids = m_pMainDM->GetCellData(_T("일련번호"), m_nDupIdx, m_DupSelData);
	ids = m_pMainDM->GetCellData(_T("선택자료"), m_nSrcIdx, m_SrcAllData);
	ids = m_pMainDM->GetCellData(_T("식별번호"), m_nSrcIdx, m_SrcSelData);
	
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pMainDM->CONNECTION_INFO);
	if(bSrcChange)
	{
		strQuery.Format(_T("SELECT MARC, DUP_FLAG FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), m_SrcManageNo);
		tmpDM.RestructDataManager(strQuery);
		srcmarc = tmpDM.GetCellData(0, 0);
		m_pMainDM->SetCellData(_T("BS_MARC"), srcmarc, m_nSrcIdx);
		m_pMainDM->SetCellData(_T("BS_복본플래그"), tmpDM.GetCellData(0, 1), m_nSrcIdx);
	}

	strQuery.Format(_T("SELECT MARC, DUP_FLAG FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), m_DupManageNo);
	tmpDM.RestructDataManager(strQuery);
	dupmarc = tmpDM.GetCellData(0, 0);
	m_pMainDM->SetCellData(_T("BS_MARC"), dupmarc, m_nDupIdx);
	m_pMainDM->SetCellData(_T("BS_복본플래그"), tmpDM.GetCellData(0, 1), m_nDupIdx);

	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(bSrcChange)
	{
		if(m_pInfo->pMarcMgr->Decoding(srcmarc, &m_marc_src))
		{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)
			{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
				UpdateData(FALSE);
			}	
		}
		m_marceditor_src.InitUserAlias();
		m_marceditor_src.Init(m_pInfo->pMarcMgr, &m_marc_src);
		m_marceditor_src.Display();
	}

	if(m_pInfo->pMarcMgr->Decoding(dupmarc, &m_marc_dup))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}
	m_marceditor_dup.InitUserAlias();
	m_marceditor_dup.Init(m_pInfo->pMarcMgr, &m_marc_dup);
	m_marceditor_dup.Display();
		
	this->UpdateData(FALSE);
	m_pMainGrid->SetReverse(m_nDupIdx);
	SetSavableFlag();
	ShowDuplicateDataInsertButtonCtrl();
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnbtnSHOWSRCVOL() 
{
	// TODO: Add your control notification handler code here
	ShowVolumeDlg(TRUE);
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnbtnSHOWDUPVOL() 
{
	// TODO: Add your control notification handler code here
	ShowVolumeDlg(FALSE);
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnbtnDUPDATAINSERT() 
{
	// TODO: Add your control notification handler code here
	InsertToDuplicateData();
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnbtnDUPDATAINSERTCANCEL() 
{
	// TODO: Add your control notification handler code here
	DivideToDuplicateData();
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::SetInfoType(INT infoType)
{
	m_InfoType = infoType;
	if(infoType == HOLDING_DUPLICATE_SEARCH 
		|| infoType == CAT_DUPLICATE_SEARCH 
		|| infoType == HOLDING_DUPLICATE_SEARCH_DIVNO
		|| infoType == HOLDING_DUPLICATE_SEARCH_BASIC_DATA
		|| infoType == HOLDING_DUPLICATE_SEARCH_IMPORT_DATA)
		MainCMAlias = _T("CM_복본조사");
	else if(infoType == TITLE_DUPLICATE_SEARCH)
		MainCMAlias = _T("CM_서명복본조사");
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::ShowVolumeDlg(BOOL isSource)
{
	// TODO: Add your control notification handler code here
	// 공통 권별정보 보기 화면을 생성한다.
	m_bShowVolumeType = isSource;
	INT idx = -1;
	CString strSpeciesKey, strAcqKey, strKey;
	CESL_DataMgr * pDM_BOOK = NULL;
	CMarc * pMarc = NULL;
	CMarcEditor * pMarcEditor = NULL;
	if(isSource)
	{
		idx = m_nSrcIdx;
		strSpeciesKey = m_SrcManageNo;
		pDM_BOOK = m_pSourceBookDM;
		pMarcEditor = &m_marceditor_src;
		pMarc = &m_marc_src;
	}
	else
	{
		idx = m_nDupIdx;
		strSpeciesKey = m_DupManageNo;
		pDM_BOOK = m_pDuplicateBookDM;
		pMarcEditor = &m_marceditor_dup;
		pMarc = &m_marc_dup;
	}

	BOOL bVolDMType = FALSE;
	if(pDM_BOOK == NULL) bVolDMType = FALSE;
	else
	{
		if(pDM_BOOK->GetRowCount() == 0) bVolDMType = FALSE;
		strKey = pDM_BOOK->GetCellData(_T("BB_종KEY"), 0);
		if(strKey == strSpeciesKey) bVolDMType = TRUE;
		else bVolDMType = FALSE;
	}
	
	if(!bVolDMType)
	{
		CBOVolumeDlg dlg(this);
		dlg.SetSpeciesKey(strSpeciesKey);
		dlg.SetOpenMode(8000);
		dlg.DoModal();
	}
	else
	{
		if(pDM_BOOK->GetRowCount() == 0)
		{
			ESLAfxMessageBox(_T("선정한 자료에는 책정보가 없습니다."));
			return ;
		}
		pMarcEditor->ApplyEdit();
		ApplyEditMarcToSourceMarc(pMarc, m_pMainDM, idx);
		CMarcToVolume(pDM_BOOK, pMarc);
		CBOVolumeDlg dlg(this, pDM_BOOK);
		dlg.SetOpenMode(-8000);
		dlg.DoModal();
		VolumeToCMarc(pDM_BOOK, pMarc);
		pMarcEditor->ApplyEdit();
		pMarcEditor->Display();
	}
}

/*
VOID CBO_HOLDING_DUP_DETAIL_INFO::InsertToDuplicateData()
{
	CDuplicateSearchMgr mgr(this);
	CESL_DataMgr * pDM = FindDM(_T("DM_중복본조사"));

	CString tmpquery;
	CBOFormApi api(this);
	if(m_pSourceBookDM->GetRowCount() == 0)
	{
		tmpquery.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.BOOK_APPENDIX_FLAG='B' ORDER BY B.VOL_SORT_NO"), m_SrcManageNo);
		api.MakeBookDM(tmpquery, m_pSourceBookDM);
	}
	if(m_pDuplicateBookDM->GetRowCount() == 0)
	{
		tmpquery.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.BOOK_APPENDIX_FLAG='B' ORDER BY B.VOL_SORT_NO"), m_DupManageNo);
		api.MakeBookDM(tmpquery, m_pDuplicateBookDM);
	}
	
	m_pSourceBookDM->SORT(_T("BB_권일련번호"));
	m_pDuplicateBookDM->SORT(_T("BB_권일련번호"));
	
	//마크정보로 책정보를 재 설정한다.
	m_pSpeciesApi->RestructBookBy049MarcTag(m_pSourceBookDM, &m_marc_src);
	m_pSpeciesApi->RestructBookBy049MarcTag(m_pDuplicateBookDM, &m_marc_dup);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.08.24 이학중 수정 
		//SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '복본기호부여방식'
        //VALUE_1 : 등록구분별,권차별,별치기호별,
		CBOFormApi  *pBOFormApi = new CBOFormApi(this) ;
		CCopyCodeMgr cCopyCodeMgr;
		m_cCopyCodeMgr.Init(m_pSourceBookDM->CONNECTION_INFO);
		m_pBOFormApi->SetCopyCodeMgr(&cCopyCodeMgr);

		//복본자료의 복본기호를 초기화 한다.
		
		for(INT i=0 ; i<m_pDuplicateBookDM->GetRowCount () ; i++)
		{
			m_pDuplicateBookDM->SetCellData(_T("BB_복본기호"), _T(""), i);
		}
		//복본자료 책 정보에 권일련번호, 복본기호를 부여한다.
        INT nLastCopyCode ;
		CString strTmpData ;
		CString strVol,strRegCode,strSeShelfCode;
		for(i=0 ; i<m_pDuplicateBookDM->GetRowCount () ; i++)
		{
			strVol = m_pDuplicateBookDM->GetCellData(_T("BB_편권차"), i);
			strRegCode = m_pDuplicateBookDM->GetCellData(_T("BB_등록구분_코드"), i);
			strSeShelfCode = m_pDuplicateBookDM->GetCellData(_T("BB_별치기호_코드"), i);
			m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
			nLastCopyCode = m_pBOFormApi->GetLastDupCodeNumber(m_pDuplicateBookDM, i);
			if(nLastCopyCode == 0) strTmpData = _T("");
		    else strTmpData.Format(_T("%d"), ++nLastCopyCode);

            if(strRegCode.IsEmpty ()) continue ;

			m_pDuplicateBookDM->SetCellData(_T("BB_복본기호"), strTmpData, i);
		}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	m_pSpeciesApi->CopyDataMerge(m_pSourceBookDM, m_pDuplicateBookDM, &m_marc_dup);
	
	CString strMarc = GetMarcStream(m_pMainDM, m_nDupIdx, &m_marc_dup);
	
	if(strMarc.IsEmpty())
	{
		ESLAfxMessageBox(_T("복본추기할 수 없습니다."));
		return;
	}
	pDM->SetCellData(_T("BS_MARC"), strMarc, m_nDupIdx);
	m_pInfo->pMarcMgr->Encoding(&m_marc_dup, strMarc);
	pDM->SetCellData(_T("BS_복본플래그"), _T("Y"), m_nDupIdx);
	pDM->SetCellData(_T("UDF_원본플래그"), _T("Y"), m_nSrcIdx);
	pDM->SetCellData(_T("UDF_복본자료KEY"), m_DupManageNo, m_nSrcIdx);
	if(m_bDupProcType)
		ESLAfxMessageBox(_T("종통합이 완료되었습니다."));
	else
		ESLAfxMessageBox(_T("복본추기가 완료되었습니다."));
	//마크보여주기
	m_marceditor_dup.InitUserAlias();
	m_marceditor_dup.Init(m_pInfo->pMarcMgr, &m_marc_dup);
	m_marceditor_dup.Display();
	m_bSavable = TRUE;
	m_bDupProc = TRUE;
	ShowDuplicateDataInsertButtonCtrl();
}
*/
VOID CBO_HOLDING_DUP_DETAIL_INFO::InsertToDuplicateData()
{
	CDuplicateSearchMgr mgr(this);
	CESL_DataMgr * pDM = FindDM(_T("DM_중복본조사"));

	CString tmpquery;
	CBOFormApi api(this);
	if(m_pSourceBookDM->GetRowCount() == 0)
	{
		tmpquery.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.BOOK_APPENDIX_FLAG='B' ORDER BY B.VOL_SORT_NO"), m_SrcManageNo);
		api.MakeBookDM(tmpquery, m_pSourceBookDM);
	}
	if(m_pDuplicateBookDM->GetRowCount() == 0)
	{
		tmpquery.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.BOOK_APPENDIX_FLAG='B' ORDER BY B.VOL_SORT_NO"), m_DupManageNo);
		api.MakeBookDM(tmpquery, m_pDuplicateBookDM);
	}
	
	m_pSourceBookDM->SORT(_T("BB_권일련번호"));
	m_pDuplicateBookDM->SORT(_T("BB_권일련번호"));
	
	//마크정보로 책정보를 재 설정한다.
	m_pSpeciesApi->RestructBookBy049MarcTag(m_pSourceBookDM, &m_marc_src);
	m_pSpeciesApi->RestructBookBy049MarcTag(m_pDuplicateBookDM, &m_marc_dup);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.08.24 이학중 수정 
		//SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '복본기호부여방식'
        //VALUE_1 : 등록구분별,권차별,별치기호별,
		CBOFormApi  *pBOFormApi = new CBOFormApi(this) ;
		CCopyCodeMgr cCopyCodeMgr;
		m_cCopyCodeMgr.Init(m_pSourceBookDM->CONNECTION_INFO);
		m_pBOFormApi->SetCopyCodeMgr(&m_cCopyCodeMgr);		

		//복본자료의 복본기호를 초기화 한다.
		
		for(INT i=0 ; i<m_pSourceBookDM->GetRowCount () ; i++)
		{
			m_pSourceBookDM->SetCellData(_T("BB_복본기호"), _T(""), i);
		}
		//복본자료 책 정보에 권일련번호, 복본기호를 부여한다.
        INT nLastCopyCode ;
		CString strTmpData ;
		CString strVol,strRegCode,strSeShelfCode;
		for(i=0 ; i<m_pSourceBookDM->GetRowCount () ; i++)
		{
			strVol = m_pSourceBookDM->GetCellData(_T("BB_편권차"), i);
			strRegCode = m_pSourceBookDM->GetCellData(_T("BB_등록구분_코드"), i);
			strSeShelfCode = m_pSourceBookDM->GetCellData(_T("BB_별치기호_코드"), i);
			m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
			nLastCopyCode = m_pBOFormApi->GetLastDupCodeNumber(m_pSourceBookDM, i);
			if(nLastCopyCode == 0) strTmpData = _T("");
		    else strTmpData.Format(_T("%d"), ++nLastCopyCode);

            if(strRegCode.IsEmpty ()) continue ;

			m_pSourceBookDM->SetCellData(_T("BB_복본기호"), strTmpData, i);
		}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//대상자료의 SET ISBN가져오기
	CArray<CString, CString&> array202;
	CString sItem, strSetISBN;
	m_pInfo->pMarcMgr->GetField( &m_marc_src, _T("020"), sItem, &array202 );
	for ( i = 0 ; i < array202.GetSize() ; i++ ) 
	{
		if ( array202.GetAt(i).Left(1) == _T("1") ) 
			strSetISBN = array202.GetAt(i);
	}
	array202.RemoveAll();

	m_pSpeciesApi->CopyDataMerge(m_pSourceBookDM, m_pDuplicateBookDM, &m_marc_dup);
	
	if( strSetISBN.IsEmpty() == FALSE )
	{
		sItem = _T("");
		m_pInfo->pMarcMgr->GetField( &m_marc_dup, _T("020"), sItem, &array202 );
		m_pInfo->pMarcMgr->DeleteField( &m_marc_dup, _T("020"), ' ', ' ');
		m_pInfo->pMarcMgr->DeleteField( &m_marc_dup, _T("020"), '1', ' ');
		for ( INT i = 0 ; i < array202.GetSize() ; i++ ) 
		{
			if ( array202.GetAt(i).Left(1) == _T(" ") ) 
				m_pInfo->pMarcMgr->InsertField(&m_marc_dup,_T("020")+array202.GetAt(i));
		}
		m_pInfo->pMarcMgr->InsertField(&m_marc_dup,_T("020")+strSetISBN);
		array202.RemoveAll();
	}

	CString strMarc = GetMarcStream(m_pMainDM, m_nDupIdx, &m_marc_dup);
	
	if(strMarc.IsEmpty())
	{
		ESLAfxMessageBox(_T("복본추기할 수 없습니다."));
		return;
	}
	pDM->SetCellData(_T("BS_MARC"), strMarc, m_nDupIdx);
	m_pInfo->pMarcMgr->Encoding(&m_marc_dup, strMarc);
	pDM->SetCellData(_T("BS_복본플래그"), _T("Y"), m_nDupIdx);
	pDM->SetCellData(_T("UDF_원본플래그"), _T("Y"), m_nSrcIdx);
	pDM->SetCellData(_T("UDF_복본자료KEY"), m_DupManageNo, m_nSrcIdx);
	if(m_bDupProcType)
	{
		ESLAfxMessageBox(_T("종통합이 완료되었습니다."));
	}
	else
	{
		ESLAfxMessageBox(_T("복본추기가 완료되었습니다."));
	}
	//마크보여주기
	m_marceditor_dup.InitUserAlias();
	m_marceditor_dup.Init(m_pInfo->pMarcMgr, &m_marc_dup);
	m_marceditor_dup.Display();
	m_bSavable = TRUE;
	m_bDupProc = TRUE;
	ShowDuplicateDataInsertButtonCtrl();
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::DivideToDuplicateData()
{
	CDuplicateSearchMgr mgr(this);
	CESL_DataMgr * pDM = FindDM(_T("DM_중복본조사"));
	
	CString tmpquery;
	CBOFormApi api(this);
	if(m_pDuplicateBookDM->GetRowCount() == 0)
	{
		tmpquery.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.BOOK_APPENDIX_FLAG='B' ORDER BY B.VOL_SORT_NO"), m_DupManageNo);
		api.MakeBookDM(tmpquery, m_pDuplicateBookDM);
	}
	m_pSpeciesApi->CopyDataDivide(m_pSourceBookDM, m_pDuplicateBookDM, &m_marc_dup, m_SrcManageNo);
	CString strMarc = GetMarcStream(m_pMainDM, m_nDupIdx, &m_marc_dup);
	if(strMarc.IsEmpty())
	{
		ESLAfxMessageBox(_T("복본해제할 수 없습니다."));
		return;
	}
	pDM->SetCellData(_T("BS_MARC"), strMarc, m_nDupIdx);
	m_pSpeciesApi->RestructBookBy049MarcTag(m_pSourceBookDM, &m_marc_src);

	pDM->SetCellData(_T("BS_복본플래그"), _T(""), m_nDupIdx);
	pDM->SetCellData(_T("UDF_원본플래그"), _T(""), m_nSrcIdx);
	pDM->SetCellData(_T("UDF_복본자료KEY"), _T(""), m_nSrcIdx);
	if(m_bDupProcType)
		ESLAfxMessageBox(_T("종분리가 완료되었습니다."));
	else
		ESLAfxMessageBox(_T("복본해제가 완료되었습니다."));
	//마크보여주기
	m_marceditor_dup.InitUserAlias();
	m_marceditor_dup.Init(m_pInfo->pMarcMgr, &m_marc_dup);
	m_marceditor_dup.Display();
	m_bSavable = TRUE;
	m_bDupProc = FALSE;
	ShowDuplicateDataInsertButtonCtrl();	
}

INT CBO_HOLDING_DUP_DETAIL_INFO::SaveDupInsertProc()
{
//	m_marceditor_dup.ApplyEdit();
	ApplyEditMarcToSourceMarc(&m_marc_dup, m_pMainDM, m_nDupIdx);
	CMarcToVolume(m_pDuplicateBookDM, &m_marc_dup);
	CArray<CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
	if(m_pDuplicateBookDM == NULL) return -1;
	INT ids ;
	CESL_DataMgr * pDM_OUT = m_pDuplicateBookDM;

	pDM_OUT->StartFrame();
	INT nRowCnt = m_pDuplicateBookDM->GetRowCount();
	CString strKey, strSpeciesKey, strQuery;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strKey = m_pDuplicateBookDM->GetCellData(_T("BB_책KEY"), i);
		ids = m_pDuplicateBookDM->MakeQueryByDM(RemoveAliasList, m_pDuplicateBookDM, m_pDuplicateBookDM->TBL_NAME, i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strKey);
		if(ids < 0) return ids;
		ids = m_pSpeciesApi->MakeEBookInfoAndQueryFrame(m_pDuplicateBookDM, pDM_OUT);
		if(ids < 0) return ids;
	}

	//목차초록 여부 확인해야 한다.
	INT nRow = m_pDuplicateBookDM->GetRowCount();

	CString strCopy, strSrcCnt, strTarCnt, strVSN, strPreSpeciesKey, tmpquery;
		strKey = _T("");

	tmpquery.Format(_T("SELECT COUNT(1) FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = '%s'"),m_SrcManageNo);
	pDM_OUT->GetOneValueQuery(tmpquery,strSrcCnt);
	
	if(strSrcCnt != _T("0") )
	{
		for( i = 0; i< nRow ; i++ )
		{ 
			strPreSpeciesKey = _T("");
			strPreSpeciesKey = m_pDuplicateBookDM->GetCellData(_T("BB_복본종KEY"),i);
			if(strPreSpeciesKey.IsEmpty() == FALSE && strPreSpeciesKey == m_SrcManageNo)
			{
				strVSN = m_pDuplicateBookDM->GetCellData(_T("BB_권일련번호"), i);
				tmpquery.Format(_T("SELECT COUNT(1) FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = '%s' AND VOL_SORT_NO = '%s'"),m_SrcManageNo, strVSN);
				pDM_OUT->GetOneValueQuery(tmpquery,strSrcCnt);
				tmpquery.Format(_T("SELECT COUNT(1) FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = '%s' AND VOL_SORT_NO = '%s'"),m_DupManageNo, strVSN);
				pDM_OUT->GetOneValueQuery(tmpquery,strTarCnt);
				tmpquery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = '%s' AND VOL_SORT_NO = '%s'"),m_SrcManageNo, strVSN);
				pDM_OUT->GetOneValueQuery(tmpquery,strKey);
				if( strSrcCnt == _T("1") && strTarCnt == _T("1"))
				{
					strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = '%s' AND VOL_SORT_NO = '%s';"),m_SrcManageNo, strVSN);
					pDM_OUT->AddFrame(strQuery);
					
					//인덱스도 지워준다. 
					if( !strKey.IsEmpty() )
					{
						strQuery.Format(_T("DELETE FROM IDX_BO_TOC_TBL WHERE REC_KEY = '%s';"), strKey);
						pDM_OUT->AddFrame(strQuery);
					}
					//목차/ 초록 내용도 지워준다. 
					strQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY = '%s';"),strKey);
					pDM_OUT->AddFrame(strQuery);
					strQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = '%s';"),strKey);
					pDM_OUT->AddFrame(strQuery);					
				}
				else if(strSrcCnt == _T("1") && strTarCnt == _T("0"))
				{
					strQuery.Format(_T("UPDATE BO_TOC_COMMON_TBL SET SPECIES_KEY = '%s' WHERE SPECIES_KEY = '%s' AND VOL_SORT_NO = '%s';"),
						m_DupManageNo, m_SrcManageNo, strVSN);
					pDM_OUT->AddFrame(strQuery);
					
					if( !strKey.IsEmpty() )
					{
						strQuery.Format(_T("UPDATE IDX_BO_TOC_TBL SET SPECIES_KEY = '%s' WHERE REC_KEY = '%s';"), m_DupManageNo, strKey);
						pDM_OUT->AddFrame(strQuery);
					}
				}			
			}
		}
	}
	
	CString strMarc;
	ids = m_pInfo->pMarcMgr->Encoding(&m_marc_dup, strMarc);
	if(ids < 0) return ids;
	strMarc.Replace(_T("'"), _T("''"));
	//=====================================================
	//2009.02.24 UPDATE BY PJW : 복본추기시 정리상태 UPDATE부분을 제외한다.
// 	strQuery.Format( 
// 		_T("UPDATE BO_SPECIES_TBL SET DUP_FLAG='Y', CATALOG_STATUS='0', LAST_WORK='%s', MARC='%s' WHERE REC_KEY=%s;"),
// 		WORK_LOG, strMarc, m_DupManageNo);
// 	pDM_OUT->AddFrame(strQuery);
	strQuery.Format( 
		_T("UPDATE BO_SPECIES_TBL SET DUP_FLAG='Y', LAST_WORK='%s', MARC='%s' WHERE REC_KEY=%s;"),
		WORK_LOG, strMarc, m_DupManageNo);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================
	
	
	strQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET SPECIES_KEY=%s WHERE REC_KEY IN ")
		_T("(SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE DUP_SPECIES_KEY=%s);"),
		m_DupManageNo, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);
	strQuery.Format(_T("UPDATE BO_DONINFO_TBL SET SPECIES_KEY=%s WHERE REC_KEY IN ")
		_T("(SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE DUP_SPECIES_KEY=%s);"),
		m_DupManageNo, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);
	//부록자료
	strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SPECIES_KEY=%s, DUP_SPECIES_KEY=%s, LAST_WORK='%s' ")
		_T("WHERE SPECIES_KEY=%s AND BOOK_APPENDIX_FLAG='A';"),
		m_DupManageNo, m_SrcManageNo, WORK_LOG, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);

	strQuery.Format(_T("update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
		_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
		_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
		_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
		_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
		_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
		_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
		_T("DUP_YN='Y', LAST_WORK='%s' where rec_key=%s;"), 
		m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, WORK_LOG, m_DupManageNo);
	pDM_OUT->AddFrame(strQuery);	
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	m_pSpeciesApi->BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), m_DupManageNo, _T("I"));
	//=====================================================
	
	strQuery.Format(_T("UPDATE IDX_BO_TBL SET WORKING_STATUS='BOT212O', LAST_WORK='%s' WHERE REC_KEY=%s;"),
		WORK_LOG, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	m_pSpeciesApi->BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), m_SrcManageNo, _T("I"));
	//=====================================================
	
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), m_DupManageNo);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), m_SrcManageNo);
		pDM_OUT->AddFrame(strQuery);
	}

	ids = pDM_OUT->SendFrame();
	if(ids < 0) return ids;
	ids = pDM_OUT->EndFrame();
	if(ids < 0) return ids;

	m_pMainDM->SetCellData(_T("BS_MARC"), strMarc, m_nDupIdx);
	m_pDuplicateBookDM->FreeData();
	m_pSourceBookDM->FreeData();
	m_bSavable = FALSE;
	ShowDuplicateDataInsertButtonCtrl();
	m_bIsProcDuplicateInsert = TRUE;
	m_marceditor_dup.Display();
	return 0;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::SaveSrcDivideProc()
{
//	m_marceditor_dup.ApplyEdit();
	ApplyEditMarcToSourceMarc(&m_marc_dup, m_pMainDM, m_nDupIdx);
	CMarcToVolume(m_pDuplicateBookDM, &m_marc_dup);	
	
//	m_marceditor_src.ApplyEdit();
	ApplyEditMarcToSourceMarc(&m_marc_src, m_pMainDM, m_nSrcIdx);
	CMarcToVolume(m_pSourceBookDM, &m_marc_src);	

	CESL_DataMgr * pDM_OUT = m_pSourceBookDM;
	INT ids;
	CArray<CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
	if(m_pSourceBookDM == NULL) return -1;
	
	pDM_OUT->StartFrame();

	INT nRowCnt = m_pSourceBookDM->GetRowCount();
	CString strKey, strQuery, strSpeciesKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strKey = m_pSourceBookDM->GetCellData(_T("BB_책KEY"), i);
		ids = m_pSourceBookDM->MakeQueryByDM(RemoveAliasList, m_pSourceBookDM, m_pSourceBookDM->TBL_NAME, i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strKey);
		if(ids < 0) return ids;
		ids = m_pSpeciesApi->MakeEBookInfoAndQueryFrame(m_pSourceBookDM, pDM_OUT);
		if(ids < 0) return ids;
	}
	nRowCnt = m_pDuplicateBookDM->GetRowCount();
	for(i=0 ; i<nRowCnt ; i++)
	{
		strKey = m_pDuplicateBookDM->GetCellData(_T("BB_책KEY"), i);
		m_pDuplicateBookDM->MakeQueryByDM(RemoveAliasList, m_pDuplicateBookDM, m_pDuplicateBookDM->TBL_NAME, i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strKey);
	}

	CString strSrcMarc, strDupMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc_dup, strDupMarc);
	m_pInfo->pMarcMgr->Encoding(&m_marc_src, strSrcMarc);
	strDupMarc.Replace(_T("'"), _T("''"));	strSrcMarc.Replace(_T("'"), _T("''"));

	//=====================================================
	//2009.02.24 UPDATE BY PJW : 복본추기시 정리상태 UPDATE부분을 제외한다.
// 	strQuery.Format(
// 		_T("UPDATE BO_SPECIES_TBL SET CATALOG_STATUS='0', DUP_FLAG=NULL, LAST_WORK='%s', MARC='%s' WHERE REC_KEY = %s;"),
// 		WORK_LOG, strDupMarc, m_DupManageNo);
// 	pDM_OUT->AddFrame(strQuery);
	strQuery.Format(
		_T("UPDATE BO_SPECIES_TBL SET DUP_FLAG=NULL, LAST_WORK='%s', MARC='%s' WHERE REC_KEY = %s;"),
		WORK_LOG, strDupMarc, m_DupManageNo);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================
	
	strQuery.Format(
		_T("UPDATE BO_SPECIES_TBL SET LAST_WORK='%s', MARC='%s' WHERE REC_KEY=%s;"),
		WORK_LOG, strSrcMarc, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);

	strQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET SPECIES_KEY=%s WHERE REC_KEY IN ")
		_T("(SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s);"),
		m_SrcManageNo, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);
	strQuery.Format(_T("UPDATE BO_DONINFO_TBL SET SPECIES_KEY=%s WHERE REC_KEY IN ")
		_T("(SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s);"),
		m_SrcManageNo, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);
	//부록자료
	strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SPECIES_KEY=%s, DUP_SPECIES_KEY=NULL, LAST_WORK='%s' ")
		_T("WHERE SPECIES_KEY=%s AND BOOK_APPENDIX_FLAG='A';"),
		m_SrcManageNo, WORK_LOG, m_DupManageNo);
	pDM_OUT->AddFrame(strQuery);
	
	strQuery.Format(_T("update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
		_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
		_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
		_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
		_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
		_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
		_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
		_T("DUP_YN=NULL, LAST_WORK='%s' where rec_key=%s;"), 
		m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, WORK_LOG, m_DupManageNo);
	pDM_OUT->AddFrame(strQuery);	
	strQuery.Format(
		_T("UPDATE BO_SPECIES_TBL SET CATALOG_STATUS='1' WHERE REC_KEY IN ")
		_T("(SELECT SPECIES_KEY FROM BO_BOOK_TBL ")
		_T(" WHERE WORKING_STATUS>'BOL' AND WORKING_STATUS<'BOT' AND SPECIES_KEY = %s);"),
		m_DupManageNo);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	m_pSpeciesApi->BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), m_DupManageNo, _T("I"));
	//=====================================================
	
	strQuery.Format(_T("UPDATE IDX_BO_TBL SET WORKING_STATUS=testworkstatus(%s), LAST_WORK='%s' WHERE REC_KEY=%s;"),
		m_SrcManageNo, WORK_LOG, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	m_pSpeciesApi->BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), m_SrcManageNo, _T("I"));
	//=====================================================
	
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), m_DupManageNo);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), m_SrcManageNo);
		pDM_OUT->AddFrame(strQuery);
	}

	ids = pDM_OUT->SendFrame();
	if(ids < 0) return ids;
	ids = pDM_OUT->EndFrame();
	if(ids < 0) return ids;

	m_pMainDM->SetCellData(_T("BS_MARC"), strDupMarc, m_nDupIdx);
	m_pMainDM->SetCellData(_T("BS_MARC"), strSrcMarc, m_nSrcIdx);

	m_pDuplicateBookDM->FreeData();
	m_pSourceBookDM->FreeData();
	m_bSavable = FALSE;
	ShowDuplicateDataInsertButtonCtrl();
	m_bIsProcDuplicateInsert = FALSE;
	m_marceditor_dup.Display();
	m_marceditor_src.Display();
	return 0;
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::OnbtnCHANGESAVE() 
{
	// TODO: Add your control notification handler code here
	INT ids = -1;
	if(m_bDupProc)
	{
		if(!m_bDupProcType)
			ids = SaveDupInsertProc();
		else
		{
			ids = SaveDupMergeProc();
			//=====================================================
			//2008.01.05 ADD BY PJW : 변경적용을 하면 버튼들을 비활성화시킨다.
			(CButton*) GetDlgItem(IDC_btnDUPDATA_INSERT_CANCEL)->ShowWindow(SW_HIDE);
			(CButton*) GetDlgItem(IDC_btnSHOW_DUPVOL)->ShowWindow(SW_HIDE);
			(CButton*) GetDlgItem(IDC_btnSHOW_SRCVOL)->ShowWindow(SW_HIDE);
			(CButton*) GetDlgItem(IDC_btnPRE_SEL)->ShowWindow(SW_HIDE);
			(CButton*) GetDlgItem(IDC_btnNEXT_SEL)->ShowWindow(SW_HIDE);
			(CButton*) GetDlgItem(IDC_btnPREV_SRC)->ShowWindow(SW_HIDE);
			(CButton*) GetDlgItem(IDC_btnNEXT_SRC)->ShowWindow(SW_HIDE);
			(CButton*) GetDlgItem(IDC_btnCHANGE_SAVE)->ShowWindow(SW_HIDE);
			(CButton*) GetDlgItem(IDC_btnDUPDATA_INSERT)->ShowWindow(SW_HIDE);
			//=====================================================
		}
	}
	else
		ids = SaveSrcDivideProc();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("변경내용을 저장할 수 없습니다."));
		return ;
	}
	
	ESLAfxMessageBox(_T("변경내용을 저장하였습니다."));	
}

BOOL CBO_HOLDING_DUP_DETAIL_INFO::IsEnableInsertDuplicateData()
{
	CESL_DataMgr * pDM = this->FindDM(_T("DM_중복본조사"));
	if(!pDM) return FALSE;
	CString tmpquery, tmpstr;

	pDM->GetCellData(_T("BS_복본플래그"), m_nSrcIdx, tmpstr);
	if(tmpstr == _T("Y")) return FALSE;
	pDM->GetCellData(_T("UDF_원본플래그"), m_nSrcIdx, tmpstr);
	if(tmpstr == _T("Y")) return FALSE;
	tmpquery.Format(_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY IN(%s, %s) AND (WORKING_STATUS < 'BOC' OR WORKING_STATUS > 'BOR') AND MANAGE_CODE = UDF_MANAGE_CODE"), m_SrcManageNo, m_DupManageNo);
//DEL 	tmpquery.Format(_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY IN(%s, %s) AND (WORKING_STATUS < 'BOC' OR WORKING_STATUS > 'BOR')"), m_SrcManageNo, m_DupManageNo);
	pDM->GetOneValueQuery(tmpquery, tmpstr);
	tmpstr.TrimLeft();	tmpstr.TrimRight();
	if(_ttoi(tmpstr.GetBuffer(0)) > 0) return FALSE;
	tmpquery.Format(_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s AND ( DUP_SPECIES_KEY IS NULL OR DUP_SPECIES_KEY = 0) AND MANAGE_CODE = UDF_MANAGE_CODE"), m_DupManageNo);
//DEL 	tmpquery.Format(_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s AND ( DUP_SPECIES_KEY IS NULL OR DUP_SPECIES_KEY = 0)"), m_DupManageNo);
	pDM->GetOneValueQuery(tmpquery, tmpstr);
	tmpstr.TrimLeft();	tmpstr.TrimRight();
	if(_ttoi(tmpstr.GetBuffer(0)) > 0) return TRUE;
	
	return FALSE;
}

BOOL CBO_HOLDING_DUP_DETAIL_INFO::IsEnableDivideDuplicateData()
{
	CESL_DataMgr * pDM = this->FindDM(_T("DM_중복본조사"));
	if(!pDM) return FALSE;
	CString tmpquery, tmpstr, tmpstr1, strDupKey, strSrcKey;
	pDM->GetCellData(_T("BS_복본플래그"), m_nDupIdx, tmpstr);
	pDM->GetCellData(_T("UDF_원본플래그"), m_nSrcIdx, tmpstr1);
	pDM->GetCellData(_T("IBS_종KEY"), m_nDupIdx, strDupKey);
	pDM->GetCellData(_T("UDF_복본자료KEY"), m_nSrcIdx, strSrcKey);

	if(tmpstr == _T("Y") && tmpstr == tmpstr1 && strDupKey == strSrcKey)
		return TRUE;
	return FALSE;
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::ShowDuplicateDataInsertButtonCtrl()
{
	CString strBtnName, strBtnName1;
	
	// 2005.10.12  UPDATE BY PDJ
	// 배가자료(소장자료) 로 관리되므로 조건문의 비교가 잘못되었음.
	//if(m_srcStatus == _T("소장자료") && m_dupStatus == _T("소장자료"))
	//{

	//===================================================
	//2009.03.26 UPDATE BY PJW : 소장자료에는 소장이후의 자료상태도 포함한다.
//	if( ( m_srcStatus.Find(_T("소장자료")) > 0 ) && 
//		( m_dupStatus.Find(_T("소장자료")) > 0 ) )
	if( ( m_srcStatus.Find(_T("소장자료")) >= 0 || m_srcStatus.Find(_T("대출")) >= 0 ||
		  m_srcStatus.Find(_T("배달")) >= 0		|| m_srcStatus.Find(_T("수리제본")) >= 0 ||
		  m_srcStatus.Find(_T("파손")) >= 0		|| m_srcStatus.Find(_T("제적")) >= 0 ||
		  m_srcStatus.Find(_T("재정리")) >= 0	|| m_srcStatus.Find(_T("분실")) >= 0 ||
		  // 18.09.27 JMJ KOL.RED.2018.008
		  m_srcStatus.Find(_T("책두레")) >= 0 ) && 
		( m_dupStatus.Find(_T("소장자료")) >= 0 || m_dupStatus.Find(_T("대출")) >= 0 ||
		  m_dupStatus.Find(_T("배달")) >= 0		|| m_dupStatus.Find(_T("수리제본")) >= 0 ||
		  m_dupStatus.Find(_T("파손")) >= 0		|| m_dupStatus.Find(_T("제적")) >= 0 ||
		  m_dupStatus.Find(_T("재정리")) >= 0	|| m_dupStatus.Find(_T("분실")) >= 0 ||
		  // 18.09.27 JMJ KOL.RED.2018.008
		  m_dupStatus.Find(_T("책두레")) >= 0 ) )
	//===================================================	
	{
		m_bDupProcType = TRUE;
		strBtnName = _T("종통합");
		strBtnName1 = _T("종분리");
	}
	else
	{
		m_bDupProcType = FALSE;
		strBtnName = _T("복본추기");
		strBtnName1 = _T("복본해제");
	}

	if(IsEnableDivideDuplicateData())
	{
		GetDlgItem(IDC_btnDUPDATA_INSERT_CANCEL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_btnDUPDATA_INSERT_CANCEL)->EnableWindow(FALSE);
	}
	

	// 2008.04.24 ADD BY PDJ
	// 소장자료만 추기되도록
	//=====================================================
	//2009.02.24 UPDATE BY PJW : 복본조사 상세보기에서는 복본추기만 가능하도록 한다.
// 	if( IsEnableInsertDuplicateData() && 
// 		( m_dupStatus.Find(_T("소장자료")) >= 0 || m_dupStatus.Find(_T("대출")) >= 0 ||
// 		  m_dupStatus.Find(_T("배달")) >= 0 || m_dupStatus.Find(_T("수리제본")) >= 0 ||
// 		  m_dupStatus.Find(_T("파손")) >= 0 || m_dupStatus.Find(_T("제적")) >= 0 ||
// 		  m_dupStatus.Find(_T("재정리")) >= 0 || m_dupStatus.Find(_T("분실")) >= 0 ||
// 		  m_dupStatus.Find(_T("책두레")) >= 0 ) )
// 	{
// 		GetDlgItem(IDC_btnDUPDATA_INSERT)->EnableWindow(TRUE);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_btnDUPDATA_INSERT)->EnableWindow(FALSE);
// 	}
	if( IsEnableInsertDuplicateData() && 
		( m_dupStatus.Find(_T("소장자료")) >= 0 || m_dupStatus.Find(_T("대출")) >= 0 ||
		  m_dupStatus.Find(_T("배달")) >= 0 || m_dupStatus.Find(_T("수리제본")) >= 0 ||
		  m_dupStatus.Find(_T("파손")) >= 0 || m_dupStatus.Find(_T("제적")) >= 0 ||
		  m_dupStatus.Find(_T("재정리")) >= 0 || m_dupStatus.Find(_T("분실")) >= 0 ||
		  // 18.09.27 JMJ KOL.RED.2018.008
		  m_dupStatus.Find(_T("책두레")) >= 0 ) && strBtnName ==  _T("복본추기") )
	{
		GetDlgItem(IDC_btnDUPDATA_INSERT)->EnableWindow(TRUE);
		
	}
	else
	{
		GetDlgItem(IDC_btnDUPDATA_INSERT)->EnableWindow(FALSE);
		strBtnName = _T("복본추기");
		strBtnName1 = _T("복본해제");
		m_bDupProcType = FALSE;
	}
	//=====================================================
	
	GetDlgItem(IDC_btnDUPDATA_INSERT)->SetWindowText(strBtnName);
	GetDlgItem(IDC_btnDUPDATA_INSERT_CANCEL)->SetWindowText(strBtnName1);

	if(m_bSavable)
		GetDlgItem(IDC_btnCHANGE_SAVE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_btnCHANGE_SAVE)->EnableWindow(FALSE);

	if(m_bSavable && m_bDupProc)
	{
		((CEdit*)GetDlgItem(IDC_richHOLDING_DUPLICATE))->SetReadOnly(FALSE);
		((CEdit*)GetDlgItem(IDC_richHOLDING_SOURCE))->SetReadOnly(TRUE);
	}
	else if(m_bSavable && !m_bDupProc)
	{
		((CEdit*)GetDlgItem(IDC_richHOLDING_DUPLICATE))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_richHOLDING_SOURCE))->SetReadOnly(FALSE);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_richHOLDING_DUPLICATE))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_richHOLDING_SOURCE))->SetReadOnly(TRUE);
	}
}

CString CBO_HOLDING_DUP_DETAIL_INFO::UpdateDuplicateBookInsertMarc(CESL_DataMgr * pDM_BOOK, CMarc * pMarc)
{
	//mode설명 1:복본추기, -1:복본해제
	if(pDM_BOOK == NULL) return _T("");
	m_pSpeciesApi->Restruct049MarcTag(pDM_BOOK, pMarc);
	m_pSpeciesApi->Restruct020MarcTag(pDM_BOOK, pMarc);
//	api.Restruct505MarcTag(pDM_BOOK, pMarc);

	CString strMarc;
	m_pInfo->pMarcMgr->SortMarc( pMarc );
	m_pInfo->pMarcMgr->Encoding(pMarc, strMarc);
	return strMarc;
}

CString CBO_HOLDING_DUP_DETAIL_INFO::FindVolSortNo(CESL_DataMgr *pDM_TargetBOOK, CString strVol, INT nEndPos/*=-1*/)
{
	CString strTmpData, strVolSortNo;
	
	INT nRowCnt;
	if(nEndPos >= 0) nRowCnt = nEndPos;
	else nRowCnt = pDM_TargetBOOK->GetRowCount(); 
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_TargetBOOK->GetCellData(_T("BB_편권차"), i);
		if(strTmpData == strVol)
		{
			strVolSortNo = pDM_TargetBOOK->GetCellData(_T("BB_권일련번호"), i);
			return strVolSortNo;
		}
	}
	strTmpData = pDM_TargetBOOK->GetCellData(_T("BB_권일련번호"), i-1);
	INT nSortNo = _ttoi(strTmpData.GetBuffer(0));
	nSortNo = ((nSortNo/10)+1)*10;
	strVolSortNo.Format(_T("%d"), nSortNo);
/*	pDM_TargetBOOK->InsertRow(-1);
	i = pDM_TargetBOOK->GetRowCount()-1;
	pDM_TargetBOOK->SetCellData(_T("BB_편권차"), strVol, i);
	pDM_TargetBOOK->SetCellData(_T("BB_권일련번호"), strVolSortNo, i);
*/	return strVolSortNo;
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::InitFirstLastIndex()
{
	CString strTmpData;
	INT ids = m_pMainGrid->SelectMakeList();
	ids = m_pMainGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pMainDM->GetRowCount();
		m_nSrcFirstIdx = 0;
		for(INT i=cnt-1 ; i>=0 ; i--)
		{
			strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
			if(strTmpData != _T("**"))
				break;
		}
		m_nSrcLastIdx = i;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_nSrcLastIdx = m_pMainGrid->SelectGetTailPosition();
		m_nSrcFirstIdx = m_pMainGrid->SelectGetHeadPosition();
	}
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::ShowSrcNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnPREV_SRC)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT_SRC)->EnableWindow(TRUE);

	if(m_nSrcFirstIdx >= idx || idx < 0)
		GetDlgItem(IDC_btnPREV_SRC)->EnableWindow(FALSE);
	if(m_nSrcLastIdx <= idx || idx < 0)
		GetDlgItem(IDC_btnNEXT_SRC)->EnableWindow(FALSE);
}

VOID CBO_HOLDING_DUP_DETAIL_INFO::ShowDupNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnPRE_SEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT_SEL)->EnableWindow(TRUE);

	if(m_nDupFirstIdx >= idx || idx < 0)
		GetDlgItem(IDC_btnPRE_SEL)->EnableWindow(FALSE);
	if(m_nDupLastIdx <= idx || idx < 0)
		GetDlgItem(IDC_btnNEXT_SEL)->EnableWindow(FALSE);
}

INT CBO_HOLDING_DUP_DETAIL_INFO::GetFirstSelectIndex(INT &nSrcIdx, INT &nDupIdx)
{
	INT nFirstSelIdx = -1;
	if(m_nOpenMode == GUPDATE)
	{
		nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_nSrcFirstIdx;
		else
			nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	CString strTmpData;
	strTmpData = m_pMainDM->GetCellData(_T("식별번호"), nFirstSelIdx);
	if(strTmpData != _T("**"))
	{
		nSrcIdx = nFirstSelIdx;
		nDupIdx = nFirstSelIdx+1;
	}
	else
	{
		for(INT i=nFirstSelIdx ; i>=0 ; i--)
		{
			strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
			if(strTmpData != _T("**"))
				break;
		}
		nSrcIdx = i;
		nDupIdx = nFirstSelIdx;
	}
	//복본자료의 처음 마지막 인덱스를 설정한다.
	SetDupIndexInfo(nSrcIdx);
	return nFirstSelIdx;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::GetPrevSelectIndex(INT &nSrcIdx, INT &nDupIdx)
{
	INT nPrevSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nPrevSelIndex = m_pMainGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pMainGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nSrcIdx != m_nSrcFirstIdx)
		{
			CString strTmpData;
			for(INT i=m_nSrcIdx-1 ; i>=0 ; i--)
			{
				strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
				if(strTmpData != _T("**")) break;
			}
			nPrevSelIndex = i;
		}
	}
	nSrcIdx = nPrevSelIndex;
	nDupIdx = nPrevSelIndex + 1;
	//복본자료의 처음 마지막 인덱스를 설정한다.
	SetDupIndexInfo(nSrcIdx);
	return nPrevSelIndex;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::GetNextDupIndex(INT &nDupIdx)
{
	INT nNextSelIndex = -1;
	if(m_nDupIdx != m_nDupLastIdx)
	{
		nNextSelIndex = m_nDupIdx + 1;
	}
	nDupIdx = nNextSelIndex;
	return nNextSelIndex;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::GetPrevDupIndex(INT &nDupIdx)
{
	INT nPrevSelIndex = -1;
	if(m_nDupIdx != m_nDupFirstIdx)
	{
		nPrevSelIndex = m_nDupIdx - 1;
	}
	nDupIdx = nPrevSelIndex;
	return nPrevSelIndex;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::GetNextSelectIndex(INT &nSrcIdx, INT &nDupIdx)
{
	INT nNextSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nNextSelIndex = m_pMainGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pMainGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nSrcIdx != m_nSrcLastIdx)
		{
			CString strTmpData;
			for(INT i=m_nSrcIdx+1 ; i<=m_nSrcLastIdx ; i++)
			{
				strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
				if(strTmpData != _T("**")) break;
			}
			nNextSelIndex = i;			
		}
	}
	nSrcIdx = nNextSelIndex;
	nDupIdx = nNextSelIndex + 1;
	//복본자료의 처음 마지막 인덱스를 설정한다.
	SetDupIndexInfo(nSrcIdx);
	return nNextSelIndex;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::SetDupIndexInfo(INT nSrcIdx)
{
	CString strTmpData;
	m_nDupFirstIdx = nSrcIdx + 1;
	for(INT i=m_nDupFirstIdx ; i<m_pMainDM->GetRowCount() ; i++)
	{
		strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
		if(strTmpData != _T("**")) break;
	}
	m_nDupLastIdx = i - 1;
	return 0;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::InitDupProcInfo(INT nSrcIdx, INT nDupIdx)
{
	CESL_DataMgr tmpDM;
	CString strQuery, strKey;
	tmpDM.SetCONNECTION_INFO(m_pMainDM->CONNECTION_INFO);
	if(m_bSavable && m_pDuplicateBookDM && m_pDuplicateBookDM->GetRowCount() > 0)
	{
		if(nDupIdx < 0)
		{
			nDupIdx = FindIdx(m_pDuplicateBookDM->GetCellData(_T("BB_종KEY"), 0), m_pMainDM);
			if(nDupIdx < 0) return -1;
		}
		strKey = m_pMainDM->GetCellData(_T("IBS_종KEY"), nDupIdx);
		m_pDuplicateBookDM->FreeData();
		if(!strKey.IsEmpty())
		{
			strQuery.Format(_T("SELECT MARC, DUP_FLAG FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), strKey);
			tmpDM.RestructDataManager(strQuery);
			m_pMainDM->SetCellData(_T("BS_MARC"), tmpDM.GetCellData(0, 0), nDupIdx);
			m_pMainDM->SetCellData(_T("BS_복본플래그"), tmpDM.GetCellData(0, 1), nDupIdx);
		}
	}
	if(m_bSavable && m_pSourceBookDM)// && m_pSourceBookDM->GetRowCount() > 0)
	{
		if(nSrcIdx < 0)
		{
			nSrcIdx = FindIdx(m_pSourceBookDM->GetCellData(_T("BB_종KEY"), 0), m_pMainDM);
			if(nSrcIdx < 0) return -1;
		}
		strKey = m_pMainDM->GetCellData(_T("IBS_종KEY"), nSrcIdx);
		m_pSourceBookDM->FreeData();
		if(!strKey.IsEmpty())
		{
			strQuery.Format(_T("SELECT MARC, DUP_FLAG FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), strKey);
			tmpDM.RestructDataManager(strQuery);
			m_pMainDM->SetCellData(_T("BS_MARC"), tmpDM.GetCellData(0, 0), nSrcIdx);
			m_pMainDM->SetCellData(_T("BS_복본플래그"), tmpDM.GetCellData(0, 1), nSrcIdx);
			m_pMainDM->SetCellData(_T("UDF_원본플래그"), _T(""), nSrcIdx);
			m_pMainDM->SetCellData(_T("UDF_복본자료KEY"), _T(""), nSrcIdx);
		}
	}
	return 0;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::FindIdx(CString strKey, CESL_DataMgr *pDM)
{
	INT nRowCnt = pDM->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("IBS_종KEY"), i);
		if(strKey == strTmpData) return i;
	}
	return -1;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::SetSavableFlag()
{
	if(!m_pDuplicateBookDM || !m_pSourceBookDM || 
		(m_pSourceBookDM->GetRowCount() == 0 && m_pDuplicateBookDM->GetRowCount() == 0))
	{
		m_bSavable = FALSE;
		return 0;
	}
	CString strSrcKey, strDupKey;
	if(m_pSourceBookDM->GetRowCount() > 0)
		strSrcKey = m_pSourceBookDM->GetCellData(_T("BB_종KEY"), 0);
	if(m_pDuplicateBookDM->GetRowCount() > 0)
		strDupKey = m_pDuplicateBookDM->GetCellData(_T("BB_종KEY"), 0);
	if((strDupKey.IsEmpty() || m_DupManageNo == strDupKey) && (strSrcKey.IsEmpty() || m_SrcManageNo == strSrcKey)) m_bSavable = TRUE;
	else m_bSavable = FALSE;

	return 0;
}	

VOID CBO_HOLDING_DUP_DETAIL_INFO::Create505Tag(WPARAM wParam, LPARAM lParam)
{
	//권->마크로 505tag를 생성한다.
	if(m_bShowVolumeType)
	{
		m_pSpeciesApi->Restruct505MarcTag(m_pSourceBookDM, &m_marc_src);
		m_marceditor_src.Display();
	}
	else
	{
		m_pSpeciesApi->Restruct505MarcTag(m_pDuplicateBookDM, &m_marc_dup);
		m_marceditor_dup.Display();
	}

	return;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::CMarcToVolume(CESL_DataMgr *pDM_BOOK, CMarc *pMarc)
{
	//마크->권정보로 049만 간다.
	m_pSpeciesApi->RestructBookBy049MarcTag(pDM_BOOK, pMarc);
//
	//권일련번호 재부여
	CString strVol, strVolSortNo;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	for(INT i=0 ; i< nRowCnt ; i++)
	{
		strVol = pDM_BOOK->GetCellData(_T("BB_편권차"), i);
		strVolSortNo = FindVolSortNo(pDM_BOOK, strVol);
		pDM_BOOK->SetCellData(_T("BB_권일련번호"), strVolSortNo, i);
	}

	return 0;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::VolumeToCMarc(CESL_DataMgr *pDM_BOOK, CMarc *pMarc)
{
	//권정보->마크로 049, 020만 간다.
	m_pSpeciesApi->Restruct049MarcTag(pDM_BOOK, pMarc);
	m_pSpeciesApi->Restruct020MarcTag(pDM_BOOK, pMarc);
	return 0;
}

CString CBO_HOLDING_DUP_DETAIL_INFO::GetMarcStream(CESL_DataMgr *pDM, INT nIdx, CMarc *pMarc)
{
	CString strMarc, str049, str020, str505;
	CArray<CString, CString&> arr020;	arr020.RemoveAll();
	CMarc Marc;
	strMarc = pDM->GetCellData(_T("BS_MARC"), nIdx);
	m_pInfo->pMarcMgr->Decoding(strMarc, &Marc);
	//049/020/505 가져온다.
	m_pInfo->pMarcMgr->GetField(pMarc, _T("049"), str049);
	m_pInfo->pMarcMgr->GetField(pMarc, _T("020"), str020, &arr020);
	m_pInfo->pMarcMgr->GetField(pMarc, _T("505"), str505);
	//원본 마크에서 049/020/505지운다.
	m_pInfo->pMarcMgr->DeleteField(&Marc, _T("049"));
	m_pInfo->pMarcMgr->DeleteField(&Marc, _T("020"));
	m_pInfo->pMarcMgr->DeleteField(&Marc, _T("505"));
	//원본마크에 추출한 049/020/505를 넣어준다.
	m_pInfo->pMarcMgr->InsertField(&Marc, _T("049") + str049);
	INT cnt = arr020.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		str020 = arr020.GetAt(i);
		m_pInfo->pMarcMgr->InsertField(&Marc, _T("020") + str020);
	}
	m_pInfo->pMarcMgr->InsertField(&Marc, _T("505") + str505);
	//마크를 추출한다.
	m_pInfo->pMarcMgr->DeleteEmptySubfield(&Marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(&Marc);
	m_pInfo->pMarcMgr->Encoding(&Marc, strMarc);
	return strMarc;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::ApplyEditMarcToSourceMarc(CMarc *pEditMarc, CESL_DataMgr *pMainDM, INT nidx)
{
	CString strMarc = GetMarcStream(pMainDM, nidx, pEditMarc);
	m_pInfo->pMarcMgr->Decoding(strMarc, pEditMarc);
	return 0;
}

INT CBO_HOLDING_DUP_DETAIL_INFO::SaveDupMergeProc()
{
	
	//	m_marceditor_dup.ApplyEdit();
	ApplyEditMarcToSourceMarc(&m_marc_dup, m_pMainDM, m_nDupIdx);
	CMarcToVolume(m_pDuplicateBookDM, &m_marc_dup);
	CArray<CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
	if(m_pDuplicateBookDM == NULL) return -1;
	INT ids ;
	CESL_DataMgr * pDM_OUT = m_pDuplicateBookDM;
	
	pDM_OUT->StartFrame();
	INT nRowCnt = m_pDuplicateBookDM->GetRowCount();
	CString strKey, strSpeciesKey, strQuery, strValue;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strKey = m_pDuplicateBookDM->GetCellData(_T("BB_책KEY"), i);
		ids = m_pDuplicateBookDM->MakeQueryByDM(RemoveAliasList, m_pDuplicateBookDM, m_pDuplicateBookDM->TBL_NAME, i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strKey);
		if(ids < 0) return ids;
		ids = m_pSpeciesApi->MakeEBookInfoAndQueryFrame(m_pDuplicateBookDM, pDM_OUT);
		if(ids < 0) return ids;
	}
	
	CString strMarc;
	ids = m_pInfo->pMarcMgr->Encoding(&m_marc_dup, strMarc);
	if(ids < 0) return ids;
	strMarc.Replace(_T("'"), _T("''"));
	
	strQuery.Format( 
		_T("UPDATE BO_SPECIES_TBL SET LAST_WORK='%s', MARC='%s' WHERE REC_KEY=%s;"),
		WORK_LOG, strMarc, m_DupManageNo);
	pDM_OUT->AddFrame(strQuery);
	
	pDM_OUT->MakeDeleteFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_SrcManageNo);
	pDM_OUT->MakeDeleteFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_SrcManageNo);
	
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_DELETE_BO$I(%s);"), m_SrcManageNo);
		pDM_OUT->AddFrame(strQuery);
	}
	
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	strQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (rec_key, species_key, idx_work_flag, input_date) ")
// 		_T(" values (ESL_SEQ.NEXTVAL, %s, 'D', SYSDATE);"), m_SrcManageNo);
// 	pDM_OUT->AddFrame(strQuery);
	//=====================================================
	
	strQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET SPECIES_KEY=%s WHERE SPECIES_KEY=%s;"), m_DupManageNo, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);
	strQuery.Format(_T("UPDATE BO_DONINFO_TBL SET SPECIES_KEY=%s WHERE SPECIES_KEY=%s;"), m_DupManageNo, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);
	strQuery.Format(_T("UPDATE BO_EBOOK_TBL SET SPECIES_KEY=%s WHERE SPECIES_KEY=%s;"), m_DupManageNo, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);

	strQuery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"), m_SrcManageNo);
	pDM_OUT->GetOneValueQuery(strQuery, strValue);
	if(!strValue.IsEmpty())
	{
		strQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY = %s;"), m_SrcManageNo);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
		strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = %s;"), strValue);
		pDM_OUT->AddFrame(strQuery);
	}
	//부록자료
	strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SPECIES_KEY=%s, LAST_WORK='%s' ")
		_T("WHERE SPECIES_KEY=%s AND BOOK_APPENDIX_FLAG='A';"),
		m_DupManageNo, WORK_LOG, m_SrcManageNo);
	pDM_OUT->AddFrame(strQuery);

	strQuery.Format(_T("update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE'), ")
											_T("MANAGE_CODE=GET_BO_BOOK_INFO(%s,'MANAGE_CODE'), ")
											_T("REG_CODE=GET_BO_BOOK_INFO(%s,'REG_CODE'), ")
											_T("IDX_VOL_TITLE=GET_BO_BOOK_INFO(%s,'VOL_TITLE_INDEX'), ")
											_T("PLACE_INFO=GET_BO_BOOK_INFO(%s,'SHELF_LOC_CODE'), ")
											_T("IDX_VOL=GET_BO_BOOK_INFO(%s,'VOL_INDEX'), ")
											_T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE'), ")
											_T("LAST_WORK='%s' where rec_key=%s;"), 
		m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, m_DupManageNo, WORK_LOG, m_DupManageNo);
	pDM_OUT->AddFrame(strQuery);	
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	m_pSpeciesApi->BO_INDEX_TEMP_INSERT(pDM_OUT, _T("IDX_TMP_BO_TBL"), m_DupManageNo, _T("I"));
	//=====================================================
	
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		strQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), m_DupManageNo);
		pDM_OUT->AddFrame(strQuery);
	}

	ids = pDM_OUT->SendFrame();
	if(ids < 0) return ids;
	ids = pDM_OUT->EndFrame();
	if(ids < 0) return ids;
	
	m_pMainDM->SetCellData(_T("BS_MARC"), strMarc, m_nDupIdx);

	::PostMessage(pParentMgr->GetSafeHwnd(), SOURCE_DATA_MERGE, m_nSrcIdx, _ttol(m_SrcManageNo.GetBuffer(0)));	
	
	return 0;
}

HBRUSH CBO_HOLDING_DUP_DETAIL_INFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
