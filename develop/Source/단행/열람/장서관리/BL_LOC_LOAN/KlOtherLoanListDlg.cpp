// KlOtherLoanListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KlOtherLoanListDlg.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKlOtherLoanListDlg dialog


CKlOtherLoanListDlg::CKlOtherLoanListDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKlOtherLoanListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKlOtherLoanListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strRegNo = _T("");
	m_nIndex = 0;
}
CKlOtherLoanListDlg::~CKlOtherLoanListDlg()
{
}

void CKlOtherLoanListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKlOtherLoanListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKlOtherLoanListDlg, CDialog)
	//{{AFX_MSG_MAP(CKlOtherLoanListDlg)
	ON_BN_CLICKED(IDC_btnKOLL_Return, OnbtnKOLLReturn)
	ON_BN_CLICKED(IDC_btnKOLL_Close, OnbtnKOLLClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKlOtherLoanListDlg message handlers

BOOL CKlOtherLoanListDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnKOLLClose();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnKOLL_Return)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKOLLReturn();
			}
			else if(GetDlgItem(IDC_btnKOLL_Close)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKOLLClose();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CKlOtherLoanListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if( 0 > InitESL_Mgr(_T("KL_OTHER_LOAN_LIST")) )
	{
		AfxMessageBox( _T("InitESL_Mgr ERROR!!") );
		return -1;
	}
	
	GetDlgItem(IDC_edtKOLL_RegNo)->SetWindowText(m_strRegNo);

	CESL_ControlMgr *pCM = FindCM(_T("CM_KL_OTHER_LOAN_LIST"));
	if(NULL == pCM) return FALSE;
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	pGrid->Display();

	CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_LOAN_VIEW"));
	if(NULL == pDM) return FALSE;

	if(1 == pDM->GetRowCount())
	{
		pGrid->SetAt(0, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKlOtherLoanListDlg::OnbtnKOLLReturn() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_KL_OTHER_LOAN_LIST"));
	if(NULL == pCM) return;
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	pGrid->SelectMakeList();
	int nCnt = pGrid->SelectGetCount();
	if(1 > nCnt)
	{
		MessageBox(_T("선정된 자료가 없습니다."), _T("이용자조회"), MB_OK|MB_ICONINFORMATION);
		return;
	}
	int nIdx = pGrid->SelectGetHeadPosition();
	if(0 > nIdx) return;
	m_nIndex = nIdx;
	OnOK();
}

void CKlOtherLoanListDlg::OnbtnKOLLClose() 
{
	OnCancel();
}

int CKlOtherLoanListDlg::RefreshLoanData()
{
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K47_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("도서관부호"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("등록번호"), m_strRegNo);
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	int ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) return ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_LOAN_VIEW"));
	if(NULL == pDM) return -1;
	ids = Reader.makeDM(pDM);
	if(0 > ids) return ids;
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg,strTemp;
		pDM->GetCellData(0, 0, strTemp);
		strMsg.Format(_T("타관대출현황조회(47) ERROR CODE : %s"), strTemp);
		pDM->QueryLog(strMsg.GetBuffer(0));
		return 100;
	}
	if(1 > pDM->GetRowCount()) return 200;
	return 0;
}

BEGIN_EVENTSINK_MAP(CKlOtherLoanListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CKlOtherLoanListDlg)
	ON_EVENT(CKlOtherLoanListDlg, IDC_gridKOLL_Main, -605 /* MouseDown */, OnMouseDowngridKOLLMain, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CKlOtherLoanListDlg, IDC_gridKOLL_Main, -604 /* KeyUp */, OnKeyUpgridKOLLMain, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKlOtherLoanListDlg::OnMouseDowngridKOLLMain(short Button, short Shift, long x, long y) 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_KL_OTHER_LOAN_LIST"));
	if(NULL == pCM) return;
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(NULL == pGrid) return;
	pGrid->SelectMakeList();
	INT nIdx = pGrid->SelectGetHeadPosition();
	if(pGrid->SelectGetCount())
	{
		do
		{
			pGrid->SetAt(nIdx, pGrid->m_nSelectIdx, _T(""));
			nIdx = pGrid->SelectGetNext();
		}
		while(nIdx >= 0);
	}
	pGrid->SelectRemoveAll();
}

void CKlOtherLoanListDlg::OnKeyUpgridKOLLMain(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
}
