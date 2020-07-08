// ArrearViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArrearViewDlg.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrearViewDlg dialog


CArrearViewDlg::CArrearViewDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CArrearViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrearViewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
}
CArrearViewDlg::~CArrearViewDlg()
{
	m_Brush.DeleteObject();
}

void CArrearViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrearViewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrearViewDlg, CDialog)
	//{{AFX_MSG_MAP(CArrearViewDlg)
	ON_BN_CLICKED(IDC_btnAV_CLOSE, OnbtnAVCLOSE)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrearViewDlg message handlers

BEGIN_EVENTSINK_MAP(CArrearViewDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CArrearViewDlg)
	ON_EVENT(CArrearViewDlg, IDC_gridAV_MAIN, -604 /* KeyUp */, OnKeyUpgridAVMAIN, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CArrearViewDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnAVCLOSE();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnAV_CLOSE)->m_hWnd == pMsg->hwnd )
			{
				OnbtnAVCLOSE();
			}
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CArrearViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	m_Brush.CreateSolidBrush(RGB(255,0,0));

	if(InitESL_Mgr( _T("SM_ARREAR_VIEW") ) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	if(NULL == m_pDM) return FALSE;
	if(1 > m_pDM->GetRowCount()) return FALSE;

	SetTimer(1, 100, NULL);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CArrearViewDlg::OnKeyUpgridAVMAIN(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

void CArrearViewDlg::OnbtnAVCLOSE() 
{
	CDialog::OnCancel();
}

HBRUSH CArrearViewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if(IDC_stcAV_MSG == pWnd->GetDlgCtrlID())
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkColor(RGB(255,0,0));
		return (HBRUSH) m_Brush;
	}

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CArrearViewDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		InitData();
	}
	
	CDialog::OnTimer(nIDEvent);
}

INT CArrearViewDlg::InitData()
{
	CString strFuncName = _T("InitData");

	CString strWhere = _T("");
	CString strRecKeyList = _T("");
	if(1 == m_pDM->GetRowCount())
	{
		m_pDM->GetCellData(0, 0, strRecKeyList);
		strWhere.Format(_T("REC_KEY=%s"), strRecKeyList);
	}
	else
	{
		strWhere = _T("REC_KEY IN (");
		for(INT i = 0; i < m_pDM->GetRowCount(); i++)
		{
			if(!strRecKeyList.IsEmpty()) strRecKeyList += _T(",");
			CString strValue;
			m_pDM->GetCellData(i, 0, strValue);
			strRecKeyList += strValue;
		}
		strWhere += strRecKeyList + _T(")");
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_VIEW"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	pDM->RefreshDataManager(strWhere);
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		CString strUserKey;
		pDM->GetCellData(_T("USER_KEY"), i, strUserKey);
		for(INT j = 0; j < m_pDM->GetRowCount(); j++)
		{
			CString strUserKey2;
			m_pDM->GetCellData(_T("USER_KEY"), j, strUserKey2);
			if(strUserKey == strUserKey2)
			{
				CString strArrear;
				m_pDM->GetCellData(_T("연체료"), j, strArrear);
				pDM->SetCellData(_T("연체료"), strArrear, i);
				break;
			}
		}
	}
	ControlDisplay(_T("CM_ARREAR_VIEW"), _T("MainGrid"));

	return 0;
}
