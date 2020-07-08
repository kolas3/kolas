// Request_DataExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Request_DataExportDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequest_DataExportDlg dialog


CRequest_DataExportDlg::CRequest_DataExportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequest_DataExportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;

}

CRequest_DataExportDlg::~CRequest_DataExportDlg()
{
}

VOID CRequest_DataExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_DataExportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequest_DataExportDlg, CDialog)
	//{{AFX_MSG_MAP(CRequest_DataExportDlg)
	ON_BN_CLICKED(IDC_bDEFAULT_CONFIG, OnbDEFAULTCONFIG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequest_DataExportDlg message handlers

BOOL CRequest_DataExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_nCheckKeyCount = 0;
	// 체크버튼 알리아스									// 체크버튼 리소스												
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("서명");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkTITLE;				m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("저자");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkAUTHOR;				m_nCheckKeyCount++;			
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("발행자");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkPUBLISHER;			m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("발행년");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkPUBLISH_YEAR;		m_nCheckKeyCount++;			
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("가격");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkPRICE;				m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("추천의견");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkRECOM_OPINION;		m_nCheckKeyCount++;
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("비고");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkCANCLE_REASON;		m_nCheckKeyCount++;	
//	m_strChkButtonAlias[m_nCheckKeyCount] = _T("SMS수신여부");	m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkSMS_YN;				m_nCheckKeyCount++;		
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("비치상태");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkFURNISH_STATUS;		m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("신청일");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkAPPLICANT_DATE;		m_nCheckKeyCount++;		
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("신청자");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkAPPLICANT_NAME;		m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("E-MAIL");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkEMAIL;				m_nCheckKeyCount++;
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("주민등록번호");	m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkCIVIL_NO;			m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("우편번호");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkZIP_CODE;			m_nCheckKeyCount++;		
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("주소");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkADDRESS;			m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("전화번호");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkTEL;				m_nCheckKeyCount++;		
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("핸드폰");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkHANDPHONE;			m_nCheckKeyCount++;	
//	m_strChkButtonAlias[m_nCheckKeyCount] = _T("메일발송일");	m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkMAIL_SEND_DATE;		m_nCheckKeyCount++;

	// 설정값들을 저장해놓은 파일을 읽어와서 체크 표시한다.
	SetDefaultCheckedCode();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequest_DataExportDlg::OnOK() 
{
	// TODO: Add extra validation here

	// 다이얼로그에서 체크된 항목만 리스트에 추가한다.
	INT nCheck = 0;
	CButton * pBtn;
	
	
	
	for (INT i=0; i < m_nCheckKeyCount; i++)
	{
		pBtn = (CButton*)GetDlgItem(m_nChkButtonResource[i]);
		nCheck = pBtn->GetCheck();
		if(nCheck) 
			m_strCheckArray.Add(m_strChkButtonAlias[i]);
	}
	
	
	CDialog::OnOK();
}

VOID CRequest_DataExportDlg::SetDefaultCheckedCode()
{

	CStdioFile fp;
	CButton * pBtn;

	// 모든 체크버튼을 초기화한다.
	for( INT i=0; i < m_nCheckKeyCount; i++)
	{
		pBtn = (CButton*)GetDlgItem(m_nChkButtonResource[i]);
		pBtn->SetCheck(0);
	}

	INT ids;
	if(!fp.Open(_T("..\\Cfg\\RequestExportData.txt"), CFile::modeRead | CFile::typeBinary))
	{
		// 파일을 찾지 못한 경우에는 필수항목(서명, 비치상태)에 체크표시 한다.
		pBtn = (CButton*)GetDlgItem(IDC_chkTITLE);
		pBtn->SetCheck(1);
		// 2005-01-16 
		// 비치상태는 필수항목으로 추가하고, 
		// 신청일과 추천의견은 필수항목은 아니지만 체크표시는 항상 하도록 한다.
		pBtn = (CButton*)GetDlgItem(IDC_chkFURNISH_STATUS);
		pBtn->SetCheck(1);
		pBtn = (CButton*)GetDlgItem(IDC_chkRECOM_OPINION);
		pBtn->SetCheck(1);
		pBtn = (CButton*)GetDlgItem(IDC_chkAPPLICANT_DATE);
		pBtn->SetCheck(1);
	}
	else
	{
		// 파일에서 셋팅값 읽어서 체크표시 한다.
		CString strLine;
		CString strAlias;
		
		TCHAR cUni;
		fp.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}
		
		while (fp.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			
			strAlias.Empty();

			if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;
			
			ids = _stscanf(strLine, _T("%[^\r\n]"), strAlias.GetBuffer(256));
			
			if (ids == _TEOF) continue;

			strAlias.ReleaseBuffer();

			for( INT i=0; i < m_nCheckKeyCount; i++)
			{
				if(!_tcscmp(m_strChkButtonAlias[i], strAlias))
				{
					pBtn = (CButton*)GetDlgItem(m_nChkButtonResource[i]);
					pBtn->SetCheck(1);
				}
			}
		}
	}

}

BOOL CRequest_DataExportDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CRequest_DataExportDlg::OnbDEFAULTCONFIG() 
{
	// TODO: Add your control notification handler code here
	// 체크된 항목을 기본값설정 파일에 저장한다.
	CString strTmp;
	CStdioFile fp;
	CButton * pBtn;

	// 파일을 열지 못하는 경우 새로 생성한다.
	fp.Open(_T("..\\Cfg\\RequestExportData.txt"), CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
	if ( fp == NULL ) {
		AfxMessageBox(_T("파일 열기 실패."));
		return ;
	}
	
	if( 0 == fp.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fp.Write( &cUni, sizeof(TCHAR));
	}

	for( INT i=0; i < m_nCheckKeyCount; i++)
	{
		pBtn = (CButton*)GetDlgItem(m_nChkButtonResource[i]);
		if(pBtn->GetCheck())
		{
			strTmp = m_strChkButtonAlias[i] + _T("\r\n");
			fp.WriteString(strTmp);
			strTmp.Empty();
		}
	}

	fp.Close();

	AfxMessageBox(_T("기본값을 저장하였습니다."));

}

HBRUSH CRequest_DataExportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}