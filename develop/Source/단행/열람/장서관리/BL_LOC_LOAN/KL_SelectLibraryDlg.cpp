// KL_SelectLibraryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KL_SelectLibraryDlg.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_SelectLibraryDlg dialog


CKL_SelectLibraryDlg::CKL_SelectLibraryDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_SelectLibraryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pGrid = NULL;
	m_bIsSearch = FALSE;
	m_pThread = NULL;
}

CKL_SelectLibraryDlg::~CKL_SelectLibraryDlg()
{
}

void CKL_SelectLibraryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_SelectLibraryDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_SelectLibraryDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_SelectLibraryDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_SelectLibraryDlg message handlers

BOOL CKL_SelectLibraryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());
	
	InitESL_Mgr(_T("SM_KL_SearchLibraryInfo"));

	m_pDM = FindDM(_T("DM_KL_SearchLibraryInfo"));
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_KL_SearchLibraryInfo"), _T("GRID"));

	if(m_strUserNo.IsEmpty())
	{
		CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(pUserDM)
		{
			if(0 < pUserDM->GetRowCount())
			{
				m_strUserNo = pUserDM->GetCellData(_T("대출자번호"), 0);
			}
		}
	}

	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CKL_SelectLibraryDlg::Refresh()
{
	if(m_bIsSearch)
	{
		return 1000;
	}
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		return 1010;
	}

	m_bIsSearch = TRUE;

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K48_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);

	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	Sender.CommMiddleWareDaemon(&Writer, &Reader);

	Reader.makeDM(m_pDM);

	SetTimer(1, 300, NULL);
	return 0;
}

INT CKL_SelectLibraryDlg::DoRefresh()
{
	if(m_bIsSearch)
	{
		return 1000;
	}
	m_pThread = AfxBeginThread(KL_SelectLibraryThread, this);

	return 0;
}

UINT  CKL_SelectLibraryDlg::KL_SelectLibraryThread(LPVOID lParam)
{
	CKL_SelectLibraryDlg* pDlg = (CKL_SelectLibraryDlg*)lParam;
	pDlg->Refresh();

	return 0;
}

void CKL_SelectLibraryDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(1 == nIDEvent)
	{
		KillTimer(1);
		m_pGrid->Display();
		m_bIsSearch = FALSE;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CKL_SelectLibraryDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_astrLibCode.RemoveAll();

	m_pGrid->SelectMakeList();
	if(0 == m_pGrid->SelectGetCount())
	{

	}
	else
	{
		INT nIdx = m_pGrid->SelectGetHeadPosition();
		m_strLibName = m_pDM->GetCellData(_T("도서관명"), nIdx);
		do
		{	
			m_astrLibCode.Add(m_pDM->GetCellData(_T("가입도서관부호"), nIdx));
			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);
	}
	
	CDialog::OnOK();
}

void CKL_SelectLibraryDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_astrLibCode.RemoveAll();

	CDialog::OnCancel();
}

BOOL CKL_SelectLibraryDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}
