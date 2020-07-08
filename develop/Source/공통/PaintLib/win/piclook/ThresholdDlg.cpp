// ThresholdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "piclook.h"
#include "ThresholdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThresholdDlg dialog


CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThresholdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThresholdDlg)
	m_threshold_max = 0;
	m_threshold_min = 0;
	//}}AFX_DATA_INIT

	// KOL.UDF.022 시큐어코딩 보완
	m_channel =-1;
}


void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThresholdDlg)
	DDX_Text(pDX, IDC_THRESHOLD_MAX, m_threshold_max);
	DDV_MinMaxInt(pDX, m_threshold_max, 0, 255);
	DDX_Text(pDX, IDC_THRESHOLD_MIN, m_threshold_min);
	DDV_MinMaxInt(pDX, m_threshold_min, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThresholdDlg, CDialog)
	//{{AFX_MSG_MAP(CThresholdDlg)
	ON_BN_CLICKED(IDC_BLUE_RADIO, OnBlueRadio)
	ON_BN_CLICKED(IDC_GREEN_RADIO, OnGreenRadio)
	ON_BN_CLICKED(IDC_RED_RADIO, OnRedRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThresholdDlg message handlers

void CThresholdDlg::OnBlueRadio() 
{
	// TODO: Add your control notification handler code here
	m_channel=0;	
}

void CThresholdDlg::OnGreenRadio() 
{
	// TODO: Add your control notification handler code here
	m_channel=1;
}

void CThresholdDlg::OnRedRadio() 
{
	// TODO: Add your control notification handler code here
	m_channel=2;
}


int CThresholdDlg::GetMinThreshold()
{
	return m_threshold_min;
}

int CThresholdDlg::GetChannel()
{
	return m_channel;
}

void CThresholdDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

int CThresholdDlg::GetMaxThreshold()
{
	return m_threshold_max;

}
