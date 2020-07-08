// EFSReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EFSReportDlg.h"
//#include _T("CaptureScreen.h")

#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEFSReportDlg dialog


CEFSReportDlg::CEFSReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEFSReportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEFSReportDlg)
	m_strErrMsg = _T("알수 없는 예외가 발생했습니다.");
	m_strDescription = _T("");
	m_strEMail = _T("");
	//}}AFX_DATA_INIT

	// 초기화
	m_bContinue = Init();
	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239));
}
CEFSReportDlg::~CEFSReportDlg()
{
	m_Dlg_brush.DeleteObject();
}

VOID CEFSReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEFSReportDlg)
	DDX_Text(pDX, IDC_eERROR_EFS, m_strErrMsg);
	DDX_Text(pDX, IDC_eEFS_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_eEFS_EMAIL, m_strEMail);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEFSReportDlg, CDialog)
	//{{AFX_MSG_MAP(CEFSReportDlg)
	ON_BN_CLICKED(IDC_bCLOSE_EFS, OnbCLOSEEFS)
	ON_BN_CLICKED(IDC_bEFS_REPORT, OnbEFSREPORT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEFSReportDlg Operations

BOOL CEFSReportDlg::Init()
{
	m_strCfgFileName = _T("..\\cfg\\efs.cfg");

	try
	{
		CStdioFile file;

		if (!file.Open(m_strCfgFileName, CFile::modeRead|CFile::typeBinary)) return FALSE;
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

		CString strKey;
		CString strValue;
		CString strLine;
		INT nFind = -1;
		INT ids = -1;
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();

			if (strLine.IsEmpty()) continue;
			if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

			nFind = strLine.Find('=');
			if (nFind < 0) continue;

			ids = _stscanf(strLine.GetBuffer(0), _T("%s = %s"), strKey.GetBuffer(256), strValue.GetBuffer(256));
			strKey.ReleaseBuffer();
			strValue.ReleaseBuffer();

			if (ids == 0 || ids == _TEOF) continue;

			strKey.TrimLeft();
			strKey.TrimRight();

			strValue.TrimLeft();
			strValue.TrimRight();

			if (strKey.IsEmpty() || strValue.IsEmpty()) continue;

			if (strKey == _T("FEEDBACK_URL"))
				m_strSERVER_URL = strValue;
			else if (strKey == _T("FEEDBACK_OBJECT"))
				m_strSERVER_OBJECT = strValue;
			else if (strKey == _T("PROGRAM_PATH"))
				m_strFileName_PROGRAM_PATH = strValue;
			else if (strKey == _T("USER_INFO"))
				m_strFileName_USER_INFO = strValue;
			else if (strKey == _T("SCREEN"))
				m_strFileName_SCREEN = strValue;
			else if (strKey == _T("DM_DUMP"))
				m_strFileName_DM_DUMP = strValue;
			else if (strKey == _T("MEM_DUMP"))
				m_strFileName_MEM_DUMP = strValue;
		}

		file.Close();

	}
	catch(...)	
	{
		return FALSE;
	}

	return CheckLogFiles();
}

BOOL CEFSReportDlg::CheckLogFiles()
{
	try
	{

		CFileFind fileFind;

		LPCTSTR lpszDir = _T("..\\Cfg\\EFS");

		if (!fileFind.FindFile(lpszDir))
		{
			if (!CreateDirectory(lpszDir, NULL))
				return FALSE;
		}

		CStdioFile file;

		if (!fileFind.FindFile(m_strFileName_PROGRAM_PATH))
		{
			if (!file.Open(m_strFileName_PROGRAM_PATH, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
				return FALSE;
			else
			{			
				if( 0 == file.GetLength() )
				{			
					TCHAR cUni = 0xFEFF;
					file.Write( &cUni, sizeof(TCHAR));
				}			
				file.WriteString(_T("SPF_PATH = NULL\r\n"));
				file.WriteString(_T("SPF_STEP = NULL\r\n"));
				file.WriteString(_T("SM = NULL\r\n"));
				file.WriteString(_T("MENU = NULL\r\n"));
				file.Close();
			}
		}
		
		if (!fileFind.FindFile(m_strFileName_USER_INFO))
		{
			if (!file.Open(m_strFileName_USER_INFO, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
				return FALSE;
			else
			{
				if( 0 == file.GetLength() )
				{			
					TCHAR cUni = 0xFEFF;
					file.Write( &cUni, sizeof(TCHAR));
				}
				file.WriteString(_T("LIB_CODE = NULL\r\n"));
				file.WriteString(_T("USER_ID = NULL\r\n"));
				file.WriteString(_T("IP = NULL\r\n"));
				file.Close();
			}
		}

		if (!fileFind.FindFile(m_strFileName_DM_DUMP))
		{
			if (!file.Open(m_strFileName_DM_DUMP, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
				return FALSE;
			
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
			file.Close();
		}

		if (!fileFind.FindFile(m_strFileName_MEM_DUMP))
		{
			if (!file.Open(m_strFileName_MEM_DUMP, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
				return FALSE;
			
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
			file.Close();
		}

		if (!fileFind.FindFile(m_strFileName_SCREEN))
		{
			if (!file.Open(m_strFileName_SCREEN, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
				return FALSE;
			
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
			file.Close();
		}

	}
	catch (...)
	{
		return FALSE;
	}
	
	return TRUE;
}

INT CEFSReportDlg::SetProgramPath(LPCTSTR lpszItemName, LPCTSTR lpszItemData)
{
	try
	{
		CString strKey;
		CString strValue;

		CStdioFile file;
		if (!file.Open(m_strFileName_PROGRAM_PATH, CFile::modeRead | CFile::typeBinary))
		{
			if (!file.Open(m_strFileName_PROGRAM_PATH, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
				return -1;
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
		}

		CMap<CString, LPCTSTR, CString, LPCTSTR> map;
		INT nFind = -1;
		CString strLine;
		
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();

			if (strLine.IsEmpty()) continue;
			if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

			nFind = strLine.Find('=');
			if (nFind < 0) continue;

			strKey = strLine.Mid(0, nFind);
			strValue = strLine.Mid(nFind + 1);

			strKey.TrimLeft();
			strKey.TrimRight();

			strValue.TrimLeft();
			strValue.TrimRight();

			if (strKey.IsEmpty() || strValue.IsEmpty()) continue;

			map.SetAt(strKey, strValue);
		}

		file.Close();

		map.SetAt(lpszItemName, lpszItemData);

		if (!file.Open(m_strFileName_PROGRAM_PATH, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) return -1;

		POSITION pos = map.GetStartPosition();
		while (pos)
		{
			map.GetNextAssoc(pos, strKey, strValue);

			if (strKey.IsEmpty() || strValue.IsEmpty()) continue;
			
			strLine.Format(_T("%s = %s\r\n"), strKey, strValue);
			
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
			
			file.WriteString(strLine);
		}

		file.Close();

	}
	catch (...)
	{
		return -1;
	}


	return 0;
}

INT CEFSReportDlg::DumpDMException(CEFSDMException *pDMException)
{
	if (!pDMException) return -1;

	try
	{
		INT idx = -1;
		CString strTemp;
		CString strLine;
		CStdioFile file;
		if (!file.Open(m_strFileName_DM_DUMP, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) return -1;

		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

		strLine.Format(_T("<LI> QUERY"));
		file.WriteString(strLine);

		strLine = _T("<TABLE CELLSPACING=1 CELLPADDING=0 BORDER=0 bgcolor = #C8C8C8>\r\n");
		file.WriteString(strLine);

		INT nCount = pDMException->m_QueryArr.GetSize();
		for (idx = 0; idx < nCount; idx++)
		{
			strLine.Format(_T("<TR bgcolor = white><TD> %s </TD></TR>"), pDMException->m_QueryArr.GetAt(idx));
			file.WriteString(strLine);
		}

		strLine = _T("</TABLE>\r\n");
		file.WriteString(strLine);
		
		CESL_DataMgr *pDM = pDMException->m_pDM;
		if (pDM == NULL) return -1;

		INT nRowCount = pDM->GetRowCount();
		INT nColCount = -1;
		

		if (pDM->m_nCols == -1 && pDM->RefList.GetCount() > 0) 
			nColCount = pDM->RefList.GetCount();
		else
			nColCount = pDM->m_nCols;

		strLine.Format(_T("<LI> DM DUMP\r\n"));
		file.WriteString(strLine);

		strLine = _T("<TABLE CELLSPACING=1 CELLPADDING=0 BORDER=0 bgcolor = #C8C8C8>\r\n");
		file.WriteString(strLine);

		strLine = _T("<TR bgcolor = white>\r\n");
		file.WriteString(strLine);

		CESL_DataMgr::reference * pRef = NULL;
		POSITION pos = pDM->RefList.GetHeadPosition();
		while (pos)
		{
			pRef = (CESL_DataMgr::reference*)pDM->RefList.GetNext(pos);
			if (pRef == NULL) continue;

			strLine += _T("<TD>") + pRef->FIELD_ALIAS + _T("</TD>");
		}
		strLine += _T("</TR>\r\n");
		file.WriteString(strLine);
		strLine.Empty();

		DWORD *row = NULL;
		pos = pDM->data.GetHeadPosition();
		while (pos)
		{	
			strLine = _T("<TR bgcolor = white>\r\n");
			file.WriteString(strLine);

			strLine.Empty();

			row = (DWORD*)pDM->data.GetNext(pos);
			if (row == NULL) continue;

			for (idx = 0; idx < nColCount; idx++)
			{
				strTemp.Format(_T("<TD> %s </TD>"), (TCHAR*)row[idx]);
				strLine += strTemp;
			}

			strLine += _T("</TR>\r\n");
			file.WriteString(strLine);
		}

		strLine = _T("</TABLE>\r\n");
		file.WriteString(strLine);

		file.Close();
	}
	catch (...)
	{
		return -1;
	}
	return 0;
}

INT CEFSReportDlg::DumpException(LPCTSTR lpszFileName, INT nLineNumber)
{
	try
	{
		CStdioFile file;
		if (!file.Open(m_strFileName_MEM_DUMP, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) return -1;

		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

		CString strLine;
		
		strLine.Format(_T("<LI>File Name = %s \r\n<LI>Line Number = %d\r\n"), lpszFileName, nLineNumber);
		file.WriteString(strLine);

		strLine.Format(_T("<LI>Error Message : %s\r\n"), m_strErrMsg);
		file.WriteString(strLine);

		file.Close();
	}
	catch (...)
	{
		return -1;
	}
	return 0;
}

INT CEFSReportDlg::DumpScreen()
{
	try
	{

	}
	catch (...)
	{
		return -1;
	}

	return 0;
}

BYTE *CEFSReportDlg::MakeHttpFilePartData(LPCTSTR lpszSeperate, LPCTSTR lpszParam, LPCTSTR lpszFileName, DWORD &dwLen)
{
	BYTE *pBuffer = NULL;
	dwLen = 0;

	CString strPrefix;
	CString strSubfix;

	strPrefix.Format(
		_T("\r\n--%s\r\n")
		_T("Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n")
		_T("Content-Type: application/octet-stream\r\n")
		_T("\r\n"), lpszSeperate, lpszParam, lpszFileName);

	strSubfix.Format( _T("\r\n--%s--\r\n") , lpszSeperate);


	CFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
		return NULL;
		
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	try
	{
		DWORD dwPrefixLen = strPrefix.GetLength();
		DWORD dwSubfixLen = strSubfix.GetLength();
		DWORD dwFileLen = file.GetLength();

		dwLen = dwPrefixLen + dwFileLen + dwSubfixLen;
		
		BYTE *pCurPos = pBuffer = new BYTE[dwLen];

		memcpy(pBuffer, strPrefix.GetBuffer(0), dwPrefixLen);
		pCurPos += dwPrefixLen;

		pCurPos += file.ReadHuge(pCurPos, dwFileLen);

		memcpy(pCurPos, strSubfix.GetBuffer(0), dwSubfixLen);
	}
	catch (...)
	{
		file.Close();
		delete [] pBuffer;
		return NULL;
	}
	
	file.Close();

	return pBuffer;

}

INT CEFSReportDlg::ReportErrorToServer()
{
	UpdateData();

	BOOL bResult = TRUE;

	try
	{
		CInternetSession session;
		CHttpConnection *pConnection = session.GetHttpConnection(m_strSERVER_URL, (INTERNET_PORT)80, NULL, NULL);
		if (pConnection == NULL) return -1;

		CHttpFile *pHttpFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, m_strSERVER_OBJECT);
		if (pHttpFile == NULL) return -1;

		CString strSeperate = _T("---------------------------7d13a23b368");
		CString strRequestHeaders;
		strRequestHeaders.Format(
			_T("Accept: text/*\r\n")
			_T("Content-Type: multipart/form-data; boundary=%s\r\n")
			_T("User-Agent: VCApp\r\n"), strSeperate);

		pHttpFile->AddRequestHeaders(strRequestHeaders);

		DWORD dwPostDataLen = 0;

		CString strPostData;

		strPostData.Format(
			_T("\r\n--%s\r\n")
			_T("Content-Disposition: form-data; name=\"email\"\r\n")
			_T("\r\n%s\r\n")
			_T("\r\n--%s--\r\n")
			_T("\r\n--%s\r\n")
			_T("Content-Disposition: form-data; name=\"description\"\r\n")
			_T("\r\n%s\r\n")
			_T("\r\n--%s--\r\n"), strSeperate, m_strEMail, strSeperate, strSeperate, m_strDescription, strSeperate);		

		dwPostDataLen = strPostData.GetLength();

        const INT nFileCount = 6;

		CString m_strFileName_TUNNING_FILE ;
        
		CTime t = CTime::GetCurrentTime () ;
		CString strTemp ;
		strTemp.Format (_T("%04d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;
		m_strFileName_TUNNING_FILE.Format (_T("..\\tunning\\ESL_DM_%s.txt"),strTemp) ;
	
		//===================================================================

		BYTE *pArrBYTE[nFileCount];
		DWORD dwArrLen[nFileCount];
		
		pArrBYTE[0] = MakeHttpFilePartData(strSeperate, _T("file_program_path"), m_strFileName_PROGRAM_PATH, dwArrLen[0]);
				
		pArrBYTE[1] = MakeHttpFilePartData(strSeperate, _T("file_user_info"), m_strFileName_USER_INFO, dwArrLen[1]);
		
		pArrBYTE[2] = MakeHttpFilePartData(strSeperate, _T("file_dm_dump"), m_strFileName_DM_DUMP, dwArrLen[2]);
		
		pArrBYTE[3] = MakeHttpFilePartData(strSeperate, _T("file_memory_dump"), m_strFileName_MEM_DUMP, dwArrLen[3]);
		
		pArrBYTE[4] = MakeHttpFilePartData(strSeperate, _T("file_screen"), m_strFileName_SCREEN, dwArrLen[4]);
		
        pArrBYTE[5] = MakeHttpFilePartData(strSeperate, _T("file_tunning_file"), m_strFileName_TUNNING_FILE, dwArrLen[5]);


		for (INT i = 0; i < nFileCount; i++)
		{
			dwPostDataLen += dwArrLen[i];
		}
				
		BYTE *pCurPos = NULL;
		BYTE *pPostData = NULL;
		
		pCurPos = pPostData = new BYTE[dwPostDataLen];
		ZeroMemory(pPostData, sizeof(BYTE) * dwPostDataLen);

		memcpy(pCurPos, (LPCTSTR)strPostData, strPostData.GetLength());
		pCurPos += strPostData.GetLength();

		for (i = 0; i < nFileCount; i++)
		{
			if (pArrBYTE[i] == NULL) continue;

			memcpy(pCurPos, pArrBYTE[i], dwArrLen[i]);
			pCurPos += dwArrLen[i];			
			
			delete [] pArrBYTE[i];
		}

		pHttpFile->SendRequest(NULL, 0, (LPVOID)pPostData, dwPostDataLen);
				
		DWORD dwRet;
		pHttpFile->QueryInfoStatusCode(dwRet);
		
		TCHAR szResp[1024];
		while (pHttpFile->ReadString(szResp, 1024))
		{
			TRACE(_T("%s\n"), szResp);
			if (_tcsicmp(szResp, _T("SUCCESS")) == 0)
			{
				MessageBox(_T("에러를 전송하였습니다."));
			}
		}
		delete (LPVOID)pPostData;

		
		pHttpFile->Close();
		delete pHttpFile;
		delete pConnection;
				
		return 0;
	}
	catch (CInternetException &e)
	{
		TRACE(_T("Caught Exception (%d)\n"), e.m_dwError);
	}
	catch (...)
	{
		return -1;
	}

	return 0;

}

/////////////////////////////////////////////////////////////////////////////
// CEFSReportDlg message handlers

BOOL CEFSReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_bContinue) 
	{
		MessageBox(_T("예상하지 못한 에러가 발생해서 프로그램을 실행시킬 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CEFSReportDlg::OnbEFSREPORT() 
{
	OnbCLOSEEFS();
}

VOID CEFSReportDlg::OnbCLOSEEFS() 
{
	EndDialog(IDOK);
}

BOOL CEFSReportDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbEFSREPORT();
		return TRUE;
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CEFSReportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetTextColor(RGB(0,0,0));		
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(239,239,239));		
		
		return m_Dlg_brush;		
	}

	return hbr;
}