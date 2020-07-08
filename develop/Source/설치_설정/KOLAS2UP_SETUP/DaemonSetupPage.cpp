// DaemonSetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "kolas2up_setup.h"
#include "DaemonSetupPage.h"
#include "..\\..\\..\\include\\공동목록\\CU_ComApi.h"
#include "..\\..\\공동목록\\check_dup_api\\rs_sql.h"
//#include "..\\..\\공동목록\\RS_COMMON\\RS_DM_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaemonSetupPage dialog

IMPLEMENT_DYNCREATE(CDaemonSetupPage, CPropertyPage)

CDaemonSetupPage::CDaemonSetupPage() : CPropertyPage(CDaemonSetupPage::IDD)
{
	//{{AFX_DATA_INIT(CDaemonSetupPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_filePath = _T("..\\cfg\\UserSearch.cfg");
	m_strServerIP = _T("0.0.0.0");
	m_strServerPORT = _T("53200");
	m_strServerTIMEOUT = _T("300");
	m_strServerRETRYCNT = _T("3");	
}

CDaemonSetupPage::~CDaemonSetupPage()
{
}

void CDaemonSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDaemonSetupPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDaemonSetupPage, CDialog)
	//{{AFX_MSG_MAP(CDaemonSetupPage)
	ON_BN_CLICKED(IDC_SET_SERVER_btnTest, OnSETSERVERbtnTest)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_SET_DAEMON_SERVER_IP, OnFieldchangedSetDaemonServerIp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaemonSetupPage message handlers

BOOL CDaemonSetupPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReadRemoteSetting();
	
	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_SET_DAEMON_SERVER_IP);
	if(NULL != pIP)
	{
		BYTE nField0, nField1, nField2, nField3;
		_stscanf(m_strServerIP.GetBuffer(0), _T("%d.%d.%d.%d"), &nField0, &nField1, &nField2, &nField3);
 		m_strServerIP.ReleaseBuffer(0);
		pIP->SetAddress(nField0, nField1, nField2, nField3);
	}
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDaemonSetupPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(IDC_SET_DAEMON_SERVER_IP == GetFocus()->GetDlgCtrlID())
		{
			SetModified();
		}
		if(VK_RETURN == pMsg->wParam)
		{
			return TRUE;
		}
		else if(VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
 	}

	return CDialog::PreTranslateMessage(pMsg);
}


VOID CDaemonSetupPage::Apply()
{
	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_SET_DAEMON_SERVER_IP);
	if(NULL != pIP)
	{
		BYTE nField0, nField1, nField2, nField3;
		pIP->GetAddress(nField0, nField1, nField2, nField3);

		CString strServerIP = _T("");
		strServerIP.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);
		m_strServerIP = strServerIP;
	}

	if(TRUE == WriteRemoteSetting())
	{
		MessageBox(_T("적용되었습니다."), _T("서버설정"), MB_ICONWARNING);
	}
	else
	{
		MessageBox(_T("실패하였습니다."), _T("서버설정"), MB_ICONERROR);
	}
}

BOOL CDaemonSetupPage::ReadRemoteSetting()
{
	TCHAR szRemoteHostAddress[32];
	UINT nRemoteHostPort;
	INT nRemoteTimeOut;
	INT nRemoteRetryCnt;

	FILE *fp;
	static TCHAR szBuff[128], ctmp[128];
	
	fp = _tfopen(m_filePath, _T("rb"));
	if(NULL == fp) 
	{
		return -1;
	}
	FILE* fpUni = fp;

	if(0xFF == fgetc(fpUni)) 
	{
		if(0xFE != fgetc(fpUni))
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}

	while(!feof(fp)) 
	{
		_fgetts( szBuff, 499, fp );
		if('.' == szBuff[0] || '\n' == szBuff[0] || '\t' == szBuff[0]) 
		{
			continue;
        }
        if('[' == szBuff[0]) 
		{
            _stscanf(szBuff, _T("%s"), ctmp);
		    _fgetts(szBuff, 499, fp);
            if(_tcscmp(_T("[USEARCH_SERVER]"), ctmp) == 0) 
			{
		        _stscanf(szBuff, _T("%s%d%d%d"), szRemoteHostAddress, &nRemoteHostPort, &nRemoteTimeOut, &nRemoteRetryCnt);
            }
			else 
			{
            }
        }
	}
	fclose(fp);

	m_strServerIP = (LPTSTR)(LPCTSTR)szRemoteHostAddress;
	m_strServerPORT.Format(_T("%d"), nRemoteHostPort);
	m_strServerTIMEOUT.Format(_T("%d"), nRemoteTimeOut);
	m_strServerRETRYCNT.Format(_T("%d"), nRemoteRetryCnt);	

	return TRUE;	
}

BOOL CDaemonSetupPage::WriteRemoteSetting()
{
	CStdioFile file;
	CString strTemp;

	if (!file.Open(m_filePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		MessageBox(_T("적용실패 : 파일을 열수 없습니다."), _T("서버설정"), MB_ICONERROR);
		return FALSE;
	}
	if(0 == file.GetLength())
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	file.WriteString(_T("[USEARCH_SERVER]\r\n"));
	strTemp.Format(_T("%-20s%-10s%-15s%-10s\r\n"), m_strServerIP, m_strServerPORT, m_strServerTIMEOUT, m_strServerRETRYCNT);
	file.WriteString(strTemp);

	return TRUE;
}

void CDaemonSetupPage::OnSETSERVERbtnTest() 
{
	CString strLibcode;
	BeginWaitCursor();

	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_SET_DAEMON_SERVER_IP);
	if(NULL != pIP)
	{
		BYTE nField0, nField1, nField2, nField3;
		pIP->GetAddress(nField0, nField1, nField2, nField3);

		CString strServerIP = _T("");
		strServerIP.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);
		m_strServerIP = strServerIP;
	}

	if(TRUE == WriteRemoteSetting())
	{
//		CRS_DM_Mgr RS_DM_Mgr ;
//		RS_DM_Mgr.m_dbkind =  6;
//		RS_DM_Mgr.ReLoadConfig();

		
//		INT ids = RS_DM_Mgr.GetOneValueQuery(_T("SELECT MAX(LIB_CODE) FROM MN_LIBINFO2_TBL"), strLibcode);
		INT ids;
		if(0 == ids)
		{
			CWnd *pStc = GetDlgItem(IDC_SET_SERVER_edtTest);
			pStc->SetWindowText(_T(" 접속성공!"));
		}
		else
		{
			CWnd *pStc = GetDlgItem(IDC_SET_SERVER_edtTest);
			pStc->SetWindowText(_T(" 접속실패!"));
		}
	}
	else
	{
		CWnd *pStc = GetDlgItem(IDC_SET_SERVER_edtTest);
		pStc->SetWindowText(_T(" 접속실패!"));
	}

	EndWaitCursor();
}

void CDaemonSetupPage::OnFieldchangedSetDaemonServerIp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetModified();	

	*pResult = 0;
}
