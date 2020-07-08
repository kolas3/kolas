// LibListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LibListDlg.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLibListDlg dialog


CLibListDlg::CLibListDlg(CESL_Mgr* pParent, CBL_LOC_LOAN_PROC *pLoan)
	: CESL_Mgr(CLibListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLibListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strFileName = _T("..\\cfg\\LibListDlg.cfg");
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pLoan = pLoan;
	m_bIsRunningThread = FALSE;
	m_strLibCode = _T("");
	m_nMode = 0;
}
CLibListDlg::~CLibListDlg()
{
	if(NULL != m_pDM)
	{
		m_pDM->FreeData();
	}
}

void CLibListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLibListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLibListDlg, CDialog)
	//{{AFX_MSG_MAP(CLibListDlg)
	ON_BN_CLICKED(IDC_btnLibListSearch, OnbtnLibListSearch)
	ON_BN_CLICKED(IDC_btnLibListSelect, OnbtnLibListSelect)
	ON_BN_CLICKED(IDC_btnLibListClose, OnbtnLibListClose)
	ON_BN_CLICKED(IDC_radLibListArea, OnradLibListArea)
	ON_BN_CLICKED(IDC_radLibListUnity, OnradLibListUnity)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LIB_LIST_GRID_DISPLAY, OnGridDisplay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibListDlg message handlers

BOOL CLibListDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			if(TRUE == m_bIsRunningThread)
			{
				return TRUE;
			}
			OnbtnLibListClose();
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_edtLibListSearch)->m_hWnd == pMsg->hwnd )
			{
				OnbtnLibListSearch();
			}
			if(GetDlgItem(IDC_btnLibListSearch)->m_hWnd == pMsg->hwnd )
			{
				OnbtnLibListSearch();
			}
			if(GetDlgItem(IDC_btnLibListSelect)->m_hWnd == pMsg->hwnd )
			{
				OnbtnLibListSelect();
			}
			if(GetDlgItem(IDC_btnLibListClose)->m_hWnd == pMsg->hwnd )
			{
				OnbtnLibListClose();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CLibListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_LIB_LIST_DLG")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	m_pDM = FindDM(_T("DM_LIB_LIST_DLG"));
	if(NULL == m_pDM)
	{
		AfxMessageBox(_T("DM_LIB_LIST_DLG"));
		return FALSE;
	}
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_LIB_LIST_DLG"), _T("MainGrid"));
	if(NULL == m_pGrid)
	{
		AfxMessageBox(_T("CM_LIB_LIST_DLG-MainGrid"));
		return FALSE;
	}
	
	m_CfgFileApi.SetFile(m_strFileName);
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLibListDlg::InitControl()
{
	CString strFuncName = _T("InitControl");

	CFileFind ff;
	if(!ff.FindFile(m_strFileName))
	{
		FILE *fp = _tfopen(m_strFileName, _T("w+b"));
		fputc(0xFF, fp); 
		fputc(0xFE, fp);
		fclose(fp);
		m_CfgFileApi.SetData(_T("도서관검색모드"), _T("0"));
	}
	CString strValue;
	m_CfgFileApi.GetData(_T("도서관검색모드"), strValue);
	if(_T("0") == strValue)
	{
		OnradLibListArea();
	}
	else if(_T("1") == strValue)
	{
		OnradLibListUnity();
	}
	else
	{
		m_CfgFileApi.SetData(_T("도서관검색모드"), _T("0"));
		OnradLibListArea();
	}

	return 0;
}

void CLibListDlg::OnbtnLibListSearch() 
{
	SetTimer(200, 100, NULL);	
}

void CLibListDlg::OnbtnLibListSelect() 
{
	if(TRUE == m_bIsRunningThread) return;
 	if(NULL == m_pDM) return;
 	if(1 > m_pDM->GetRowCount()) return;
 	if(NULL == m_pGrid) return;
 	INT nIdx = m_pGrid->GetIdx();
	if(0 > nIdx) return;
 	
 	CString strLibcode, strLibName;
 	m_pDM->GetCellData(_T("도서관부호"), nIdx, strLibcode);
	m_pDM->GetCellData(_T("도서관명"), nIdx, strLibName);
 	if(!strLibcode.IsEmpty() && !strLibName.IsEmpty())
	{
		m_strLibCode = strLibcode;
		m_strLibName = strLibName;
		SetTimer(300, 100, NULL);
	}
}

void CLibListDlg::OnbtnLibListClose() 
{
	if(TRUE == m_bIsRunningThread) return;

	OnCancel();
}

BEGIN_EVENTSINK_MAP(CLibListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CLibListDlg)
	ON_EVENT(CLibListDlg, IDC_gridLibListGrid, -601 /* DblClick */, OnDblClickgridLibListGrid, VTS_NONE)
	ON_EVENT(CLibListDlg, IDC_gridLibListGrid, -604 /* KeyUp */, OnKeyUpgridLibListGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CLibListDlg::OnDblClickgridLibListGrid() 
{
	if(1 > m_pDM->GetRowCount()) return;
	if(1 > m_pGrid->GetMouseRow()) return;
	OnbtnLibListSelect();
}

void CLibListDlg::OnKeyUpgridLibListGrid(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
	else if(VK_RETURN == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_RETURN, 0);
	}
}

void CLibListDlg::OnradLibListArea() 
{
	((CButton*)GetDlgItem(IDC_radLibListArea))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radLibListUnity))->SetCheck(FALSE);
	((CEdit*)GetDlgItem(IDC_edtLibListSearch))->EnableWindow(FALSE);
	((CButton*)GetDlgItem (IDC_btnLibListSearch))->EnableWindow(FALSE);
	m_CfgFileApi.SetData(_T("도서관검색모드"), _T("0"));

	SetTimer(100, 100, NULL); 
}

void CLibListDlg::OnradLibListUnity() 
{
	((CButton*)GetDlgItem(IDC_radLibListArea))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radLibListUnity))->SetCheck(TRUE);
	((CEdit*)GetDlgItem(IDC_edtLibListSearch))->EnableWindow(TRUE);
	((CButton*)GetDlgItem (IDC_btnLibListSearch))->EnableWindow(TRUE);
	m_CfgFileApi.SetData(_T("도서관검색모드"), _T("1"));

	SetTimer(500, 100, NULL);
}

HBRUSH CLibListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcLibListString == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

INT CLibListDlg::SearchAreaLib()
{
	INT ids;
	SetMessage(FALSE, _T("지역 도서관을 조회중입니다."));
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K17_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("도서관부호"), m_pInfo->LIB_CODE);
	
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		SetMessage(FALSE, _T("지역 도서관 조회에 실패하였습니다."));
	}
	else
	{
		SetMessage(FALSE, _T("완료중입니다."));
	}
	m_pDM->FreeData();
	Reader.makeDM(m_pDM);
	SendMessage(WM_LIB_LIST_GRID_DISPLAY, 0, 0);
	SetTimer(400, 50, NULL);

	return 0;
}

INT CLibListDlg::SearchLib()
{
	INT ids;
	SetMessage(FALSE, _T("도서관을 조회중입니다."));

	CString strLibInfo;
	GetDlgItemText(IDC_edtLibListSearch, strLibInfo);
	strLibInfo.Replace(_T("#"), _T(""));
	strLibInfo.TrimLeft(); strLibInfo.TrimRight();
	if(strLibInfo.IsEmpty())
	{
		SetMessage(TRUE, _T("검색조건을 입력하십시오."));
		SetTimer(500, 50, NULL);
		SetTimer(600, 1500, NULL);
		return 0;
	}
	INT nMode = 0;
	if(6 == strLibInfo.GetLength())
	{
		if(TRUE == m_pLoan->IsNumericText(strLibInfo))
		{
			nMode = 1;
		}
	}
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K16_LOC_B01_SERVICE"));
	Writer.addRecord();
	if(0 == nMode)
	{
		Writer.addElement(_T("도서관명"), strLibInfo);
	}
	else
	{
		Writer.addElement(_T("도서관부호"), strLibInfo);
	}
	
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		SetMessage(FALSE, _T("도서관 조회에 실패하였습니다."));
	}
	else
	{
		SetMessage(FALSE, _T("완료중입니다."));
	}
	m_pDM->FreeData();
	Reader.makeDM(m_pDM);
	SendMessage(WM_LIB_LIST_GRID_DISPLAY, 0, 0);
	SetTimer(400, 50, NULL);

	return 0;
}

void CLibListDlg::OnTimer(UINT nIDEvent) 
{
	if(100 == nIDEvent)
	{
		KillTimer(100);
		AfxBeginThread(SearchAreaLibThread, this);
	}
	else if(200 == nIDEvent)
	{
		KillTimer(200);
		AfxBeginThread(SearchLibThread, this);
	}
	else if(300 == nIDEvent)
	{
		KillTimer(300);
		if(TRUE == m_bIsRunningThread) return;
		OnOK();
	}
	else if(400 == nIDEvent)
	{
		KillTimer(400);
		if(0 == m_nMode)
		{
			SetMessage(TRUE, _T("반납하는 도서의 소장 도서관을 선택하여 주십시오."));
		}
		else if(1 == m_nMode)
		{
			SetMessage(TRUE, _T("도서관을 선택하여 주십시오."));
		}
		else
		{
			SetMessage(TRUE, _T("이용자를 조회할 도서관을 선택하여 주십시오."));
		}
		m_pGrid->SetFocus();
	}
	else if(500 == nIDEvent)
	{
		KillTimer(500);
		CEdit* edt = (CEdit*)GetDlgItem( IDC_edtLibListSearch );
		edt->SetFocus();
		INT nLen = edt->GetWindowTextLength();
		edt->SetSel(0, nLen);
	}
	else if(600 == nIDEvent)
	{
		if(0 == m_nMode)
		{
			SetMessage(TRUE, _T("반납하는 도서의 소장 도서관을 선택하여 주십시오."));
		}
		else if(1 == m_nMode)
		{
			SetMessage(TRUE, _T("도서관을 선택하여 주십시오."));
		}
		else
		{
			SetMessage(TRUE, _T("이용자를 조회할 도서관을 선택하여 주십시오."));
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

UINT CLibListDlg::SearchAreaLibThread(LPVOID pParam)
{
	CLibListDlg* pDlg = (CLibListDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;
	pDlg->m_bIsRunningThread = TRUE;
	pDlg->SearchAreaLib();
	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

UINT CLibListDlg::SearchLibThread(LPVOID pParam)
{
	CLibListDlg* pDlg = (CLibListDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;
	pDlg->m_bIsRunningThread = TRUE;
	pDlg->SearchLib();
	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

LRESULT CLibListDlg::OnGridDisplay(WPARAM wParam, LPARAM lParam)
{
	if(NULL != m_pGrid)
	{
		m_pGrid->m_nHeight = 27;
		m_pGrid->Display();
		m_pGrid->SetReverse(0);
	}

	return 0;
}

VOID CLibListDlg::SetMessage(BOOL bEnable, CString strMsg)
{
	SetDlgItemText(IDC_stcLibListString, strMsg);
	GetDlgItem(IDC_radLibListArea)->EnableWindow(bEnable);
	GetDlgItem(IDC_radLibListUnity)->EnableWindow(bEnable);
	GetDlgItem(IDC_btnLibListSearch)->EnableWindow(bEnable);
	GetDlgItem(IDC_btnLibListSelect)->EnableWindow(bEnable);
	GetDlgItem(IDC_btnLibListClose)->EnableWindow(bEnable);
}

void CLibListDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	
	CDialog::OnClose();
}

INT CLibListDlg::SetMode(INT nMode/*=0*/)
{
	m_nMode = nMode;

	return 0;
}