// MarcSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcSampleDlg.h"
#include "..\MarcToolKit\MarcImpExpMgr.h"
#include "..\..\..\공통\FileImport\FileImportDlg.h"

#include "..\..\KOLAS2UP_MAIN\RCView.h"
#include "..\..\KOLAS2UP_MAIN\KOLAS2UP_MAINView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcSampleDlg dialog


CMarcSampleDlg::CMarcSampleDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcSampleDlg)
	m_nFileType = -1;
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT
}

BOOL CMarcSampleDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMarcSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcSampleDlg)
	DDX_Radio(pDX, IDC_rEXCEL, m_nFileType);
	DDX_Text(pDX, IDC_eFILE_PATH2, m_strFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcSampleDlg)
	ON_BN_CLICKED(IDC_bSTART, OnbSTART)
	ON_BN_CLICKED(IDC_bOPEN_FILE, OnbOPENFILE)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_IMP, OnImp)
	ON_BN_CLICKED(IDC_bHIDE_TOOLBAR, OnbHIDETOOLBAR)
	ON_BN_CLICKED(IDC_bSHOW_TOOLBAR, OnbSHOWTOOLBAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcSampleDlg message handlers

BOOL CMarcSampleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("MARC_SAMPLE2")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_MARC_SAMPLE2"));
	if (pDM)
	{
		pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	}

	CheckDlgButton(IDC_rEXCEL, 1);

	GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);
	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMarcSampleDlg::OnbOPENFILE() 
{
	UpdateData();

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	wmemset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;


	if (m_nFileType == 0)
	{
		OFN.lpstrFilter = _T("XLS파일(*.xls)\0*.xls\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("EXCEL 파일 열기");
	}
	else if(m_nFileType == 1)
	{
		OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("TEXT 파일 열기");
	}


	if (GetOpenFileName(&OFN) != 0)
	{
		m_strFilePath = OFN.lpstrFile;
		GetDlgItem(IDC_bSTART)->EnableWindow(TRUE);
	}

	SetCurrentDirectory(szBuffer);

	UpdateData(FALSE);
}

VOID CMarcSampleDlg::OnbSTART() 
{
	INT ids;

	// 반입 받을 DM 설정
	CESL_DataMgr *pDM = FindDM(_T("DM_MARC_SAMPLE2"));
	if (pDM == NULL) return;

	if (m_strFilePath.IsEmpty()) return;

	GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);

	// CMarcImpExpMgr 선언
	CMarcImpExpMgr marcImpExpMgr;
	// DM, MarcMgr, FileType, FilePath 설정
	// File Type : XLS -- EXCEL 파일, TXT -- TEXT 파일
	marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, m_nFileType, m_strFilePath);
	marcImpExpMgr.InitDMList();
	marcImpExpMgr.AddDMPtr(pDM);
	// 오류 체크 받을 DM Alias 설정
	marcImpExpMgr.SetErrChkAlias(_T("오류코드"));
	// Stream Marc 받을 DM Alias 설정
	marcImpExpMgr.SetMarcStreamAlias(_T("마크"));
	// Import 시작
	ids = marcImpExpMgr.ImportFile();
	if (ids >= 0)
	{
		AllControlDisplay(_T("CM_MARC_SAMPLE2"));
	}


	GetDlgItem(IDC_bSTART)->EnableWindow(TRUE);
}

VOID CMarcSampleDlg::OnButton1() 
{
	CStdioFile file;
	if (!file.Open(_T("marc.txt"), CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	CESL_DataMgr *pDM = FindDM(_T("DM_MARC_SAMPLE2"));
	if (pDM == NULL) return;

	pDM->RestructDataManager(_T("select marc from bo_species_tbl"));
	
	INT nRow = pDM->GetRowCount();
	for (INT i = 0; i < nRow; i++)
	{
		CString strMarc = pDM->GetCellData(i, 0);
		strMarc.TrimLeft();
		strMarc.TrimRight();
		if (strMarc.IsEmpty()) continue;
		
		strMarc += _T("\r\n");

		file.WriteString(strMarc);
	}

	file.Close();
	
}

VOID CMarcSampleDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;
	
	pGrid->MoveWindow(0, 40, cx, cy - 40);
}

VOID CMarcSampleDlg::OnImp() 
{
	CFileImportDlg dlg(this);
	dlg.SetOpenMode(4000);
	dlg.DoModal();
	
}

VOID CMarcSampleDlg::OnbHIDETOOLBAR() 
{
	CString strButtonAlias = _T("숨어라1");

	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
}

VOID CMarcSampleDlg::OnbSHOWTOOLBAR() 
{
	CString strButtonAlias = _T("숨어라1");

	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, FALSE);
	
}
