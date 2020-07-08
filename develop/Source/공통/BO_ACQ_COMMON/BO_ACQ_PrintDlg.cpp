// BO_ACQ_PrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_PrintDlg.h"
#include "..\K2UP_Common\ExcelExporter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PrintDlg dialog


CBO_ACQ_PrintDlg::CBO_ACQ_PrintDlg(SPECIES_TYPE nSpeciesType, ACQ_TYPE nAcqType, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_PrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_PrintDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPrintMgr = NULL;
	m_arrData.RemoveAll();
	m_arrReportFileName.RemoveAll();
	m_arrReportType.RemoveAll();
	m_nSpeciesType = nSpeciesType;
	m_nAcqType = nAcqType;
	m_bCreate = FALSE;
	m_strReportFileName.Empty();
	m_nReportType = -1;
	m_pPrintDM = NULL;
	m_pGroupCollection = NULL;
	m_bSpecies = TRUE;
	m_bSeriesPaper = FALSE;
	m_bExcelExport = FALSE;
	m_bKOLAS2Format = FALSE;
}

CBO_ACQ_PrintDlg::~CBO_ACQ_PrintDlg()
{
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
}

VOID CBO_ACQ_PrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_PrintDlg)
	DDX_Control(pDX, IDC_cmbCPNT_PRINT_LIST, m_ComboBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_PrintDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_PrintDlg)
	ON_CBN_SELCHANGE(IDC_cmbCPNT_PRINT_LIST, OnSelchangecmbPRINTLIST)
	ON_BN_CLICKED(IDC_btnSERIES_PAPER, OnbtnSERIESPAPER)
	ON_BN_CLICKED(IDC_btnEXCEL_EXPORT, OnbtnEXCELEXPORT)
	ON_BN_CLICKED(IDC_btnKOLAS2_FORMAT, OnbtnKOLAS2FORMAT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PrintDlg message handlers

BOOL CBO_ACQ_PrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_pPrintMgr = new CBasicPrintMgr(this);
	m_ComboBox.ResetContent();
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("출력 리스트 선택 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_ACQ_PrintDlg::Init()
{
	if(m_nSpeciesType == SPECIES_TYPE_INVALID) return FALSE;

	if(m_nSpeciesType == BO_BASIC)
	{
		SetWindowText(_T("기초자료 출력물 리스트"));
		m_arrData.Add(_T("기초자료목록"));				m_arrReportFileName.Add(_T("K2UP_기초자료목록"));				m_arrReportType.Add(BO_ACQ_BASIC_DATA);

	}
	else if(m_nSpeciesType == BO_IMPORT)
	{
		SetWindowText(_T("반입자료 출력물 리스트"));
		m_arrData.Add(_T("반입자료목록"));				m_arrReportFileName.Add(_T("K2UP_반입자료목록"));				m_arrReportType.Add(BO_ACQ_IMPORT_DATA);
	}
	else if(m_nSpeciesType == BO_RECYCLE)
	{
		SetWindowText(_T("재활용자료 출력물 리스트"));
		m_arrData.Add(_T("재활용자료목록"));			m_arrReportFileName.Add(_T("K2UP_재활용자료목록"));				m_arrReportType.Add(BO_ACQ_RECYCLE_DATA);
	}
	else if(m_nSpeciesType == BO_REQUEST)
	{
		SetWindowText(_T("비치희망자료 출력물 리스트"));
		m_arrData.Add(_T("비치희망자료목록"));			m_arrReportFileName.Add(_T("K2UP_비치희망자료목록"));			m_arrReportType.Add(BO_ACQ_REQUEST_DATA);
		m_arrData.Add(_T("메일발송목록"));				m_arrReportFileName.Add(_T("K2UP_비치희망메일발송목록"));		m_arrReportType.Add(BO_ACQ_REQUEST_MAIL_SEND_LIST);
	}	
	else if(m_nSpeciesType == BO_PURCHASE)
	{
		SetWindowText(_T("구입대상자료 출력물 리스트"));
		m_arrData.Add(_T("구입대상자료목록"));				m_arrReportFileName.Add(_T("K2UP_구입대상자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_DATA);
		m_arrData.Add(_T("구입대상자료목록(등록구분별)"));	m_arrReportFileName.Add(_T("K2UP_구입대상자료목록_등록구분별"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_REGCODE);
		m_arrData.Add(_T("심의대상자료목록(차수별)"));		m_arrReportFileName.Add(_T("K2UP_심의대상자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO);
		m_arrData.Add(_T("심의대상자료목록(구입처별)"));	m_arrReportFileName.Add(_T("K2UP_심의대상자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER);
		m_arrData.Add(_T("심의대상자료목록(주제구분별)"));	m_arrReportFileName.Add(_T("K2UP_심의대상자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE);
		m_arrData.Add(_T("심의결과자료목록(차수별)"));		m_arrReportFileName.Add(_T("K2UP_심의결과자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO);
		m_arrData.Add(_T("심의결과자료목록(구입처별)"));	m_arrReportFileName.Add(_T("K2UP_심의결과자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER);
		m_arrData.Add(_T("심의결과자료목록(주제구분별)"));	m_arrReportFileName.Add(_T("K2UP_심의결과자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE);
		m_arrData.Add(_T("주문자료목록(차수별)"));			m_arrReportFileName.Add(_T("K2UP_주문자료목록_차수별"));			m_arrReportType.Add(BO_ACQ_ORDER_DATA_DIVNO);
		m_arrData.Add(_T("주문자료목록(구입처별)"));		m_arrReportFileName.Add(_T("K2UP_주문자료목록_구입처별"));			m_arrReportType.Add(BO_ACQ_ORDER_DATA_PROVIDER);
		m_arrData.Add(_T("주문자료목록(주제구분별)"));		m_arrReportFileName.Add(_T("K2UP_주문자료목록_주제구분별"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_SUBJECTCODE);
	}
	else if(m_nSpeciesType == BO_ORDER)
	{
		SetWindowText(_T("검수대상자료 출력물 리스트"));
		m_arrData.Add(_T("검수대상자료목록"));			m_arrReportFileName.Add(_T("K2UP_검수대상자료목록"));			m_arrReportType.Add(BO_ACQ_CHECKINING_DATA);
		m_arrData.Add(_T("주문자료목록(차수별)"));		m_arrReportFileName.Add(_T("K2UP_주문자료목록_차수별"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_DIVNO);
		m_arrData.Add(_T("주문자료목록(구입처별)"));	m_arrReportFileName.Add(_T("K2UP_주문자료목록_구입처별"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_PROVIDER);
		m_arrData.Add(_T("주문자료목록(주제구분별)"));	m_arrReportFileName.Add(_T("K2UP_주문자료목록_주제구분별"));	m_arrReportType.Add(BO_ACQ_ORDER_DATA_SUBJECTCODE);
		m_arrData.Add(_T("검수자료목록"));				m_arrReportFileName.Add(_T("K2UP_검수자료목록"));				m_arrReportType.Add(BO_ACQ_CHECKIN_DATA);
		m_arrData.Add(_T("미검수자료목록"));			m_arrReportFileName.Add(_T("K2UP_미검수자료목록"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_DATA);
		m_arrData.Add(_T("구입신청자료도착목록"));		m_arrReportFileName.Add(_T("K2UP_구입신청자료도착목록"));		m_arrReportType.Add(BO_ACQ_CHECKIN_REQUEST_DATA);
		m_arrData.Add(_T("검수자료통계"));				m_arrReportFileName.Add(_T("K2UP_검수자료통계"));				m_arrReportType.Add(BO_ACQ_CHECKIN_STATISTICS);
		m_arrData.Add(_T("미검수자료통계"));			m_arrReportFileName.Add(_T("K2UP_미검수자료통계"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_STATISTICS);
	}
	else if(m_nSpeciesType == BO_MISSING)
	{
		SetWindowText(_T("미납자료 출력물 리스트"));
		m_arrData.Add(_T("미납자료목록"));				m_arrReportFileName.Add(_T("K2UP_미납자료목록"));				m_arrReportType.Add(BO_ACQ_MISSING_DATA);
		m_arrData.Add(_T("미검수자료통계"));			m_arrReportFileName.Add(_T("K2UP_미검수자료통계"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_STATISTICS);
	}
	else if(m_nSpeciesType == BO_REGOBJ && m_nAcqType == PURCHASE)
	{
		SetWindowText(_T("구입등록인계대상자료 출력물 리스트"));
		m_arrData.Add(_T("구입등록인계대상자료목록"));	m_arrReportFileName.Add(_T("K2UP_구입등록인계대상자료목록"));	m_arrReportType.Add(BO_ACQ_PURCHASE_REGOBJ_DATA);
		m_arrData.Add(_T("검수완료자료목록"));			m_arrReportFileName.Add(_T("K2UP_검수완료자료목록"));			m_arrReportType.Add(BO_ACQ_CHECKIN_COMPLETE_DATA);
		m_arrData.Add(_T("검수자료통계"));				m_arrReportFileName.Add(_T("K2UP_검수자료통계"));				m_arrReportType.Add(BO_ACQ_CHECKIN_STATISTICS);
	}
	else if(m_nSpeciesType == BO_DONATE)
	{
		SetWindowText(_T("기증접수자료 출력물 리스트"));
		m_arrData.Add(_T("기증접수자료목록"));			m_arrReportFileName.Add(_T("K2UP_기증접수자료목록"));			m_arrReportType.Add(BO_ACQ_DONATE_DATA);
	}
	else if(m_nSpeciesType == BO_NONDONATE)
	{
		SetWindowText(_T("기증미등록자료 출력물 리스트"));
		m_arrData.Add(_T("기증미등록자료목록"));		m_arrReportFileName.Add(_T("K2UP_기증미등록자료목록"));			m_arrReportType.Add(BO_ACQ_NONDONATE_DATA);
	}
	else if(m_nSpeciesType == BO_REGOBJ && m_nAcqType == DONATE)
	{
		SetWindowText(_T("기증등록인계대상자료 출력물 리스트"));
		m_arrData.Add(_T("기증등록인계대상자료목록"));	m_arrReportFileName.Add(_T("K2UP_기증등록인계대상자료목록"));	m_arrReportType.Add(BO_ACQ_DONATE_REGOBJ_DATA);
	}
	else if(m_nSpeciesType == BO_PURCHASE_DATA_PRINT)
	{
		SetWindowText(_T("구입자료 출력물 리스트"));
		m_arrData.Add(_T("구입자료목록"));	m_arrReportFileName.Add(_T("K2UP_구입자료목록"));	m_arrReportType.Add(BO_PURCHASE_DATA);
	}
	else if(m_nSpeciesType == BO_DONATE_DATA_PRINT)
	{
		SetWindowText(_T("기증자료 출력물 리스트"));
		m_arrData.Add(_T("기증자료목록"));	m_arrReportFileName.Add(_T("K2UP_기증자료목록"));	m_arrReportType.Add(BO_DONATE_DATA);
		m_arrData.Add(_T("기증자목록"));	m_arrReportFileName.Add(_T("K2UP_기증자목록"));		m_arrReportType.Add(BO_ACQ_DEFAULT);      
	}
	INT cnt = m_arrData.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		m_ComboBox.InsertString(i, (m_arrData.GetAt(i)).GetBuffer(0));
	}
	m_ComboBox.SetCurSel(0);

	OnSelchangecmbPRINTLIST();

	return TRUE;
}

BOOL CBO_ACQ_PrintDlg::Create(CWnd * pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_PrintDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_nReportType < 0) return ;
	INT nExtraInfo;
	if(m_bKOLAS2Format) nExtraInfo = 1000;
	else nExtraInfo = 0;

	::SendMessage(pParentMgr->GetSafeHwnd(), MAKE_PRINT_DM, m_nReportType, nExtraInfo);
}

BOOL CBO_ACQ_PrintDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

INT CBO_ACQ_PrintDlg::Print()
{
	INT ids;
	if(m_bExcelExport)
		ids = PrintExcel();
	else
		ids = PrintPaper();
	m_bExcelExport = FALSE;
	m_bKOLAS2Format = FALSE;
	return ids;
}

VOID CBO_ACQ_PrintDlg::OnSelchangecmbPRINTLIST() 
{
	// TODO: Add your control notification handler code here
	INT cnt = m_ComboBox.GetCount();
	INT nCurSel = m_ComboBox.GetCurSel();
	CString str;
	m_ComboBox.GetWindowText(str);
	m_strReportFileName = m_arrReportFileName.GetAt(nCurSel);
	m_nReportType = m_arrReportType.GetAt(nCurSel);
	if(m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE)
		GetDlgItem(IDC_btnSERIES_PAPER)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_btnSERIES_PAPER)->ShowWindow(SW_HIDE);

	if(m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_ORDER_DATA_DIVNO || m_nReportType == BO_ACQ_ORDER_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_ORDER_DATA_PROVIDER)
		GetDlgItem(IDC_btnKOLAS2_FORMAT)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_btnKOLAS2_FORMAT)->ShowWindow(SW_HIDE);
}

VOID CBO_ACQ_PrintDlg::OnbtnSERIESPAPER() 
{
	// TODO: Add your control notification handler code here
	m_bSeriesPaper = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
}

VOID CBO_ACQ_PrintDlg::OnbtnEXCELEXPORT() 
{
	// TODO: Add your control notification handler code here
	m_bExcelExport = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
}

INT CBO_ACQ_PrintDlg::PrintPaper()
{
	if(m_pPrintDM == NULL) return -1;
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("출력 데이터가 없습니다."));
		return -1;
	}
	CString strAcqYear, strGroupInfo;
	m_pPrintMgr->InitPrintData();
	strAcqYear = _T("적용안함");
	strGroupInfo = _T("적용안함");
	if(m_pGroupCollection != NULL && m_pGroupCollection->GetGroupCount() > 0)
	{
		strAcqYear = m_pGroupCollection->GetAcqYear();
		m_pGroupCollection->GetGroupInfo(strGroupInfo);
		if(strAcqYear.IsEmpty()) strAcqYear = _T("적용안함");
		if(strGroupInfo.IsEmpty()) strGroupInfo = _T("적용안함");
		
		m_pPrintMgr->AddPrintData(_T("수입년도"), strAcqYear);
		if(m_nAcqType == PURCHASE) m_pPrintMgr->AddPrintData(_T("차수번호"), strGroupInfo);
		else if(m_nAcqType == DONATE) m_pPrintMgr->AddPrintData(_T("접수번호"), strGroupInfo);
		else if(m_nAcqType == ACQ_TYPE_INVALID) m_pPrintMgr->AddPrintData(_T("그룹이름"), strGroupInfo);
	}
	m_pPrintMgr->SetPrintDMAlias(m_pPrintDM->DB_MGR_ALIAS);
	if(m_bSeriesPaper)
	{
		if(m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2UP_심의대상자료목록_연속용지");			
		}
		else if(m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2UP_심의결과자료목록_연속용지");	
		}
		m_nReportType = SERIES_PAPER;
		m_pPrintMgr->SetPrintType(m_nReportType);
		m_pPrintMgr->SetSIReportFilename(m_strReportFileName);
	}
	else if(m_bKOLAS2Format)
	{
		if(m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2_심의대상자료목록");			
		}
		else if(m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2_심의결과자료목록");	
		}
		else if(m_nReportType == BO_ACQ_ORDER_DATA_DIVNO || m_nReportType == BO_ACQ_ORDER_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_ORDER_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2_주문자료목록");	
		}
		
		m_pPrintMgr->SetPrintType(m_nReportType);
		m_pPrintMgr->SetSIReportFilename(m_strReportFileName);
	}
	else
	{            
		CString strReportFileName;
		m_pPrintMgr->SetPrintType(m_nReportType);
		if(!m_bSpecies && m_nReportType == BO_ACQ_CHECKINING_DATA) 
			strReportFileName = m_strReportFileName + _T("_권");
		else strReportFileName = m_strReportFileName;
		m_pPrintMgr->SetSIReportFilename(strReportFileName);
	}

	INT ids = 0;
	if(m_nReportType != BO_ACQ_PURCHASE_DATA_REGCODE)
		m_pPrintMgr->Print();
	else
		ids = m_pPrintMgr->Print(m_pPrintDM);
	
	m_bSeriesPaper = FALSE;
	return ids;
}

INT CBO_ACQ_PrintDlg::PrintExcel()
{
	CSIReportManager * pSIReportManager = m_pPrintMgr->GetSIReportManager();
	if(pSIReportManager == NULL) return -1;
	CExcelExporter ExcelExporter(this, pSIReportManager, m_pPrintDM, m_strReportFileName);
	INT ids = ExcelExporter.InitExcelExporter();
	if(ids < 0) return ids;

	ids = ExcelExporter.ExeclExport();
	if(ids < 0) return ids;

	return 0;
}

INT CBO_ACQ_PrintDlg::CreatePrintMgr()
{
	if(m_pPrintMgr) return  0;
	else
	{
		m_pPrintMgr = new CBasicPrintMgr(this);
	}
	return 0;
}

VOID CBO_ACQ_PrintDlg::OnbtnKOLAS2FORMAT() 
{
	// TODO: Add your control notification handler code here
	m_bKOLAS2Format = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);	
}
