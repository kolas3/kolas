// KL_SearchLoanInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KL_SearchLoanInfoDlg.h"
#include "KL_SelectLibraryDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_SearchLoanInfoDlg dialog

#include "FrameManager.h"
CKL_SearchLoanInfoDlg::CKL_SearchLoanInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_SearchLoanInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_bIsSearched = FALSE;

	m_strSMName = _T("SM_KL_SearchLoanInfo");
	m_strDMName = _T("DM_KL_SearchLoanInfo");
	m_strCMName = _T("CM_KL_SearchLoanInfo");
}

CKL_SearchLoanInfoDlg::~CKL_SearchLoanInfoDlg()
{
}

void CKL_SearchLoanInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_SearchLoanInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_SearchLoanInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_SearchLoanInfoDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN2, OnClear)
	ON_BN_CLICKED(IDC_BTN1, OnSearch)
	ON_BN_CLICKED(IDC_BTN3, OnBtnSelectLoanLib)
	ON_BN_CLICKED(IDC_BTN4, OnBtnSelectReturnLib)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_SearchLoanInfoDlg message handlers

BOOL CKL_SearchLoanInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitESL_Mgr(m_strSMName);

	m_pDM = FindDM(m_strDMName);
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_KL_SearchLoanInfo2"), _T("GRID"));
	m_pCM = FindCM(m_strCMName);

	MoveCtrl();

	((CButton*)GetDlgItem(IDC_RADIO_LOAN1))->SetCheck(TRUE);

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(m_strCMName, _T("둥록번호"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);	

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKL_SearchLoanInfoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl();
}

void CKL_SearchLoanInfoDlg::MoveCtrl()
{	
	if(m_pGrid)
	{	
		CRect rCtrl;
		m_pGrid->GetWindowRect(rCtrl);
		ScreenToClient(rCtrl);
		
		CRect rDlg;
		GetWindowRect(rDlg);
		ScreenToClient(rDlg);
		
		rCtrl.right = rDlg.right - 10;
		rCtrl.bottom = rDlg.bottom - 10;
				
		m_pGrid->MoveWindow(rCtrl);
	}
}

void CKL_SearchLoanInfoDlg::OnClear() 
{
	// TODO: Add your control notification handler code here	
	if(m_pCM)
	{
		m_pCM->AllControlClear();
	}

	m_astrLoanLibcode.RemoveAll();
	m_astrReturnLibcode.RemoveAll();
}

void CKL_SearchLoanInfoDlg::OnSearch() 
{
	// TODO: Add your control notification handler code here
// 	DoRefresh();
	Refresh();
}

void CKL_SearchLoanInfoDlg::OnBtnSelectLoanLib() 
{
	// TODO: Add your control notification handler code here
 	CKL_SelectLibraryDlg Dlg(this);
 	UINT uID = Dlg.DoModal();
 	if(IDOK == uID)
 	{
 		m_astrLoanLibcode.RemoveAll();
 
 		const INT nSize = Dlg.m_astrLibCode.GetSize();
 		for(INT nCnt = 0; nCnt < nSize; nCnt++)
 		{
 			m_astrLoanLibcode.Add(Dlg.m_astrLibCode.GetAt(nCnt));
 		}
 
 		CString strText;
 		if(1 < nSize)
 		{
 			strText.Format(_T("%s외 %d개 도서관"), Dlg.m_strLibName, nSize-1);
 		}
 		else
 		{
 			strText = Dlg.m_strLibName;
 		}
 		m_pCM->SetControlMgrData(_T("대출도서관"), strText);
 	}
}

void CKL_SearchLoanInfoDlg::OnBtnSelectReturnLib() 
{
	// TODO: Add your control notification handler code here
 	CKL_SelectLibraryDlg Dlg(this);
 	UINT uID = Dlg.DoModal();	
 	if(IDOK == uID)
 	{
 		m_astrReturnLibcode.RemoveAll();
 
 		const INT nSize = Dlg.m_astrLibCode.GetSize();
 		for(INT nCnt = 0; nCnt < nSize; nCnt++)
 		{
 			m_astrReturnLibcode.Add(Dlg.m_astrLibCode.GetAt(nCnt));
 		}
 
 		CString strText;
 		if(1 < nSize)
 		{
 			strText.Format(_T("%s외 %d개 도서관"), Dlg.m_strLibName, nSize-1);
 		}
 		else
 		{
 			strText = Dlg.m_strLibName;
 		}
 		m_pCM->SetControlMgrData(_T("반납도서관"), strText);
 	}
}

void CKL_SearchLoanInfoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(1 == nIDEvent)
	{
		KillTimer(1);
		m_pGrid->Display();
		GetDlgItem(IDC_BTN1)->EnableWindow(TRUE);
		m_bIsSearched = FALSE;

	}
	CDialog::OnTimer(nIDEvent);
}

INT CKL_SearchLoanInfoDlg::DoRefresh()
{
	if(m_bIsSearched)
	{
		return 1000;
	}
	GetDlgItem(IDC_BTN1)->EnableWindow(FALSE);

	AfxBeginThread(SearchLoanInfoThread, this);

	return 0;
}

UINT CKL_SearchLoanInfoDlg::SearchLoanInfoThread(LPVOID lParam)
{
	CKL_SearchLoanInfoDlg* pDlg = (CKL_SearchLoanInfoDlg*)lParam;
	pDlg->Refresh();

	return 0;
}

INT CKL_SearchLoanInfoDlg::Refresh()
{	
	if(m_bIsSearched)
	{
		return 1000;
	}

	if(!m_pInfo->IsConnectUnityLoanService())
	{
		return 1010;
	}


	m_bIsSearched = TRUE;
	CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(!pUserDM)
	{
		SetTimer(1, 300, NULL);
		return -1000;
	}

	m_pDM->FreeData();

	CString strUserNo = pUserDM->GetCellData(_T("대출자번호"), 0);
	CString strData[16];

	if(!m_pCM)
	{
		SetTimer(1, 300, NULL);
		return -1010;
	}

	m_pCM->GetControlMgrData(_T("대출일1"),		strData[0]);
	m_pCM->GetControlMgrData(_T("대출일2"),		strData[1]);
	m_pCM->GetControlMgrData(_T("반납예정일1"),	strData[2]);
	m_pCM->GetControlMgrData(_T("반납예정일2"),	strData[3]);
	m_pCM->GetControlMgrData(_T("반납일1"),		strData[4]);
	m_pCM->GetControlMgrData(_T("반납일2"),		strData[5]);
	m_pCM->GetControlMgrData(_T("예약일1"),		strData[6]);
	m_pCM->GetControlMgrData(_T("예약일2"),		strData[7]);
	m_pCM->GetControlMgrData(_T("예약만기일1"),	strData[8]);
	m_pCM->GetControlMgrData(_T("예약만기일2"),	strData[9]);
	m_pCM->GetControlMgrData(_T("서명"),			strData[10]);
	m_pCM->GetControlMgrData(_T("발행자"),		strData[11]);
	m_pCM->GetControlMgrData(_T("저자"),			strData[12]);
	m_pCM->GetControlMgrData(_T("둥록번호"),		strData[13]);
	m_pCM->GetControlMgrData(_T("대출도서관"),	strData[14]);
	m_pCM->GetControlMgrData(_T("반납도서관"),	strData[15]);

	CFrameWriter Writer(m_pInfo);

	if( ((CButton*)GetDlgItem(IDC_RADIO_LOAN1))->GetCheck() )
	{
		Writer.setCommand(_T("IL_K14_LOC_B01_SERVICE"));
	}
	else
	{
		Writer.setCommand(_T("IL_K15_LOC_B01_SERVICE"));
	}
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), strUserNo);


	const INT nLoanLibCnt = m_astrLoanLibcode.GetSize();
	const INT nReturnLibCnt = m_astrReturnLibcode.GetSize();

	if(0 < nLoanLibCnt)
	{
		CString strLib;
		if(1 < nLoanLibCnt)
		{
			for(INT nCnt = 0; nCnt < nLoanLibCnt; nCnt++)
			{
				strLib = strLib + _T(",") + m_astrLoanLibcode.GetAt(nCnt);
			}
 			strLib.TrimLeft(_T(","));
		}
		else
		{
			strLib = m_astrLoanLibcode.GetAt(0);
		}
		Writer.addElement(_T("대출도서관"), strLib);

	}
	if(0 < nReturnLibCnt)
	{
		CString strLib;
		if(1 < nReturnLibCnt)
		{
			for(INT nCnt = 0; nCnt < nReturnLibCnt; nCnt++)
			{
				strLib = strLib + _T(",") + m_astrReturnLibcode.GetAt(nCnt);
			}
			strLib.TrimLeft(_T(","));
		}
		else
		{
			strLib = m_astrReturnLibcode.GetAt(0);
		}
		Writer.addElement(_T("반납도서관"), strLib);

	}

	if(!strData[0].IsEmpty())
	{
		strData[1] = (strData[1].IsEmpty()) ? strData[0] : strData[1];
		Writer.addElement(_T("대출일부터"), strData[0]);
		Writer.addElement(_T("대출일까지"), strData[1]);
	}
	if(!strData[2].IsEmpty())
	{
		strData[3] = (strData[3].IsEmpty()) ? strData[2] : strData[3];
		Writer.addElement(_T("반납예정일부터"), strData[2]);
		Writer.addElement(_T("반납예정일까지"), strData[3]);
	}
	if(!strData[4].IsEmpty())
	{
		strData[5] = (strData[5].IsEmpty()) ? strData[4] : strData[5];
		Writer.addElement(_T("반납일부터"), strData[4]);
		Writer.addElement(_T("반납일까지"), strData[5]);
	}
	if(!strData[6].IsEmpty())
	{
		strData[7] = (strData[7].IsEmpty()) ? strData[6] : strData[7];
		Writer.addElement(_T("예약일부터"), strData[6]);
		Writer.addElement(_T("예약일까지"), strData[7]);
	}
	if(!strData[8].IsEmpty())
	{
		strData[9] = (strData[9].IsEmpty()) ? strData[8] : strData[9];
		Writer.addElement(_T("예약만기일부터"), strData[7]);
		Writer.addElement(_T("예약만기일"), strData[7]);
	}

	if(
		strData[0].IsEmpty() && 
		strData[2].IsEmpty() && 
		strData[4].IsEmpty() && 
		strData[6].IsEmpty() &&
		strData[8].IsEmpty()
		)
	{
		AfxMessageBox(_T("검색조건이 부족합니다.\r\n검색할 날자범위를 입력하여 주십시요."));
		SetTimer(1, 300, NULL);
		return 1000;
	}

	if(!strData[13].IsEmpty())
	{
		CLocCommonAPI::MakeRegNoSpaceToZero(strData[13]);
		Writer.addElement(_T("등록번호"), strData[13]);
	}
	if(!strData[10].IsEmpty())
	{
		Writer.addElement(_T("서명"), strData[10]);
	}
	if(!strData[12].IsEmpty())
	{
		Writer.addElement(_T("저자"), strData[12]);
	}
	if(!strData[11].IsEmpty())
	{
		Writer.addElement(_T("발행자"), strData[11]);
	}
	
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
 	Sender.CommMiddleWareDaemon(&Writer, &Reader);

	Reader.makeDM(m_pDM);
	SetDescDM();

	SetTimer(1, 300, NULL);

	return 0;
}

INT CKL_SearchLoanInfoDlg::SetDescDM()
{
	CString strGetData, strSetData;
	const INT ROW_CNT = m_pDM->GetRowCount();
	for(INT nRow = 0; nRow < ROW_CNT; nRow++)
	{
		strGetData = m_pDM->GetCellData(_T("상태"), nRow);
		if(_T("0") == strGetData)
		{
			strSetData = _T("대출");
		}
		else if(_T("1") == strGetData)
		{
			strSetData = _T("반납");
		}
		else if(_T("2") == strGetData)
		{
			strSetData = _T("반납연기");
		}
		else if(_T("3") == strGetData)
		{
			strSetData = _T("예약");
		}
		else if(_T("4") == strGetData)
		{
			strSetData = _T("예약취소");
		}
		else if(_T("L") == strGetData)
		{
			strSetData = _T("책두레대출");
		}
		else if(_T("R") == strGetData)
		{
			strSetData = _T("책두레반납");
		}
		else if(_T("D") == strGetData)
		{
			strSetData = _T("책두레반납연기");
		}
		else if(_T("O") == strGetData)
		{
			strSetData = _T("책두레예약");
		}
		else
		{
			strSetData = strGetData;
		}
		
		m_pDM->SetCellData(_T("상태설명"), strSetData, nRow);

		strSetData = strGetData = m_pDM->GetCellData(_T("대출도서관"), nRow);
		 m_pDM->SetCellData(_T("대출도서관이름"), strSetData, nRow);
	}
	return 0;
}

BOOL CKL_SearchLoanInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			CWnd * pWnd = GetDlgItem(IDC_BTN1);
			if(pWnd && pWnd->IsWindowEnabled())
			{
				OnSearch();
			}
			return TRUE;
		}
		else if(VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CKL_SearchLoanInfoDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	if(SC_CLOSE == nID)
	{
		CDialog::OnCancel();
		return;
	}
	CDialog::OnSysCommand(nID, lParam);
}

void CKL_SearchLoanInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
