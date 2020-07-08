// DUPBOVolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DUPBOVolumeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\BO_SPECIES_API\BO_SPECIES.h"

/////////////////////////////////////////////////////////////////////////////
// CDUPBOVolumeDlg dialog


CDUPBOVolumeDlg::CDUPBOVolumeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDUPBOVolumeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	nRegCodes = 0;

	// KOL.UDF.022 시큐어코딩 보완
	m_nRowIdx = -1;
	nRegCodes = -1;
	nRegCodeCount[0] = 0;
}


VOID CDUPBOVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDUPBOVolumeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDUPBOVolumeDlg, CDialog)
	//{{AFX_MSG_MAP(CDUPBOVolumeDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_DUP_bCLOSE, OnDUPbCLOSE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDUPBOVolumeDlg message handlers

BOOL CDUPBOVolumeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Initialize Esl Manger	
	INT ids = InitESL_Mgr(_T("K2UP_중복본조사_권별정보"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("화면을 초기화 하는데 실패하였습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	InitRegCodeArray();

	// start display timer (once at domoal)
	CBO_SPECIES API(this);
	CESL_DataMgr *pDM_BOOK = FindDM(_T("DM_K2UP_중복본조사_권_책정보"));
	CESL_DataMgr *pDM_VOLUME = FindDM(_T("DM_K2UP_중복본조사_권_권정보"));

	if (!pDM_BOOK || !pDM_VOLUME) return -1;
	CString strOption;
	strOption.Format(_T("SPECIES_KEY=%s"), m_strSpecies_key);
	ids = pDM_BOOK->RefreshDataManager(strOption);
	ids = API.CONVERT_BOOK_to_VOLUME(pDM_BOOK, pDM_VOLUME);

	m_nRowIdx = 0;

	SetTimer(0, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDUPBOVolumeDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		Display();
	}
	CDialog::OnTimer(nIDEvent);
}

INT CDUPBOVolumeDlg::Display(VOID)
{
	AllControlDisplay(_T("CM_K2UP_중복본조사_권별정보"), m_nRowIdx);

	CESL_ControlMgr *pCM = FindCM(_T("CM_K2UP_중복본조사_권별정보"));
	if (pCM) {
		CESL_Grid *pGrid = (CESL_Grid *)pCM->FindControl(_T("GRID_VOLUME"));
		if (pGrid) {
			pGrid->SetReverse(m_nRowIdx);
		}
	}

	GetRegCodeStreamAndCount();
	return 0;
}

VOID CDUPBOVolumeDlg::OnDUPbCLOSE() 
{
	CDialog::OnOK();	
}

BEGIN_EVENTSINK_MAP(CDUPBOVolumeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDUPBOVolumeDlg)
	ON_EVENT(CDUPBOVolumeDlg, IDC_DUP_GRID_VOLUME, -600 /* Click */, OnClickDupGridVolume, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDUPBOVolumeDlg::OnClickDupGridVolume() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_K2UP_중복본조사_권별정보"));
	if (!pCM) return;
	CESL_Grid *pGrid = (CESL_Grid *)pCM->FindControl(_T("GRID_VOLUME"));
	if (!pGrid) return;
	INT nRow = pGrid->GetRow();

	if (nRow-1 != m_nRowIdx) {
		m_nRowIdx = nRow-1;

		Display();
	}
}

INT CDUPBOVolumeDlg::GetRegCodeStreamAndCount(VOID)
{
	CString strVOL, strRegCodes;
	INT i, j, nBookCount=0;

	CESL_DataMgr *pDM_BOOK = FindDM(_T("DM_K2UP_중복본조사_권_책정보"));
	CESL_DataMgr *pDM_VOLUME = FindDM(_T("DM_K2UP_중복본조사_권_권정보"));

	if (!pDM_BOOK || !pDM_VOLUME) return -1;
	strVOL = _T("");
	pDM_VOLUME->GetCellData(_T("BB_편권차"), m_nRowIdx, strVOL);
	strVOL.TrimLeft();				strVOL.TrimRight();

	for (j = 0; j < nRegCodes; j++) nRegCodeCount[j] = 0;
	
	CString strTmpVol, strTmpRegCode;
	INT nRowCount = pDM_BOOK->GetRowCount();
	for (i = 0; i < nRowCount; i++) {
		strTmpVol = _T("");
		pDM_BOOK->GetCellData(_T("BB_편권차"), i, strTmpVol);	
		strTmpVol.TrimLeft();			strTmpVol.TrimRight();
		if (strTmpVol != strVOL) continue;

		nBookCount++;
		strTmpRegCode = _T("");
		pDM_BOOK->GetCellData(_T("BB_등록구분_코드"), i, strTmpRegCode);	
		strTmpRegCode.TrimLeft();			strTmpRegCode.TrimRight();			strTmpRegCode.MakeUpper();
		for (j = 0; j < nRegCodes; j++) {
			if (strRegCode[j] == strTmpRegCode) nRegCodeCount[j]++;
		}
	}

	strRegCodes = _T("");
	for (j = 0; j < nRegCodes; j++) {
		if (nRegCodeCount[j] > 0) {
			if (nRegCodeCount[j] == 1) strTmpRegCode.Format(_T("%s"), strRegCode[i]);
				else strTmpRegCode.Format(_T("%s%d"), strRegCode[i], nRegCodeCount[j]);
			strRegCodes += strTmpRegCode;
		}
	}
	GetDlgItem(IDC_DUP_eREG_CLASS_CODE)->SetWindowText(strRegCodes);
	CString strTmp;
	strTmp.Format(_T("%d"), nBookCount);
	GetDlgItem(IDC_DUP_eBOOK_CNT)->SetWindowText(strTmp);
	
	return 0;
}

INT CDUPBOVolumeDlg::InitRegCodeArray(VOID)
{
	nRegCodes = 0;

	// 코드 정보를 읽어온다.
	nRegCodes = m_pInfo->pCodeMgr->GetCodeCount(_T("등록구분"));
	INT startrow = 0;
	CString code[6];
	for (INT i = 0; i < nRegCodes; i++) {
		startrow = m_pInfo->pCodeMgr->GetCode(_T("등록구분"), code, 6, startrow);
		strRegCode[i] = code[2];
		strRegCode[i].TrimLeft();		strRegCode[i].TrimRight();		strRegCode[i].MakeUpper();
		nRegCodeCount[i] = 0;
		startrow++;
	}
	
	return 0;
}
