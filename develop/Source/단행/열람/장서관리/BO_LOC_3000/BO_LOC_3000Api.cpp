// BO_LOC_3000Api.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "BO_LOC_3000Api.h"
#include "FrameCtl.h"
#include "FrameCtlApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3000Api dialog


CBO_LOC_3000Api::CBO_LOC_3000Api(CWnd* pParent /*=NULL*/)
	: CDialog(CBO_LOC_3000Api::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3000Api)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxSocketInit();
	m_pBoLocSoc = NULL;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
}

CBO_LOC_3000Api::~CBO_LOC_3000Api()
{
	if(NULL != m_pArchiveIn)
	{
		delete m_pArchiveIn;
		m_pArchiveIn = NULL;
	}
	if(NULL != m_pArchiveOut)
	{
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
	if(NULL != m_pFile)
	{
		delete m_pFile;
		m_pFile = NULL;
	}
	if(NULL != m_pBoLocSoc)
	{
		delete m_pBoLocSoc;
		m_pBoLocSoc = NULL;
	}
	m_saTable.RemoveAll(); m_saTable.FreeExtra();
	m_saField.RemoveAll(); m_saField.FreeExtra();
}


void CBO_LOC_3000Api::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3000Api)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3000Api, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3000Api)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3000Api message handlers

BOOL CBO_LOC_3000Api::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3000Api::CloseSocket()
{
	if(m_pBoLocSoc != NULL)
	{
		m_pBoLocSoc->ShutDown();
		if(NULL != m_pArchiveIn)
		{
			m_pArchiveIn->Abort();
			delete m_pArchiveIn;
			m_pArchiveIn = NULL;
		}
		if(NULL != m_pArchiveOut)
		{
			m_pArchiveOut->Abort();
			delete m_pArchiveOut;
			m_pArchiveOut = NULL;
		}
		if(NULL != m_pFile)
		{
			delete m_pFile;
			m_pFile = NULL;
		}
		delete m_pBoLocSoc;
		m_pBoLocSoc = NULL;
	}
	
	return TRUE;
}

BOOL CBO_LOC_3000Api::ConnectSocket()
{
	CString strPath;
	GetRegistryInfo(_T("Software\\IVS\\Lib-Secu"), _T("HOME"), strPath);
	if(strPath.IsEmpty())
	{
		TunningLog(_T("¢º GetRegistryInfo(HOME) is empty"));
		return FALSE;
	}
	CString strPortFile = strPath + _T("config\\portinfo.conf");
	CString strPort = _T("52626");
	CFileFind ff;
	if(!ff.FindFile(strPortFile))
	{
		FILE *fp = _tfopen(strPortFile, _T("w+b"));
		_ftprintf(fp, strPort);
		fclose(fp);
	}
	else
	{
		CStdioFile fFile;
		if(TRUE == fFile.Open(strPortFile, CFile::modeReadWrite|CFile::typeBinary))
		{
			CString strLine;
			fFile.ReadString(strLine);
			strLine.TrimLeft(); strLine.TrimRight();
			if(0 < strLine.GetLength())
			{
				strPort = strLine;
			}
			fFile.Close();
		}
	}
	m_nPort = _ttoi(strPort);

	if(NULL != m_pBoLocSoc)
	{
		delete m_pBoLocSoc;
		m_pBoLocSoc = NULL;
	}
	m_pBoLocSoc = new CBoLocSoc();
	if(NULL == m_pBoLocSoc)
	{
		TunningLog(_T("¢º Socket is null"));
		return FALSE;
	}
	if(!m_pBoLocSoc->Create())
	{
		delete m_pBoLocSoc;
		m_pBoLocSoc = NULL;
		TunningLog(_T("¢º Socket Create fail"));
		return FALSE;
	}
	BOOL bConnect = m_pBoLocSoc->Connect(_T("127.0.0.1"), m_nPort);
	if(!bConnect)
	{
		delete m_pBoLocSoc;
		m_pBoLocSoc = NULL;
		TunningLog(_T("¢º Socket Connect fail"));
		return FALSE;
	}
	if(NULL != m_pFile)
	{
		delete m_pFile;
		m_pFile = NULL;
	}
	m_pFile = new CSocketFile(m_pBoLocSoc);
	if(NULL == m_pFile)
	{
		TunningLog(_T("¢º SocketFile is null"));
		return FALSE;
	}
	if(NULL != m_pArchiveIn)
	{
		delete m_pArchiveIn;
		m_pArchiveIn = NULL;
	}
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);
	if(NULL == m_pArchiveIn)
	{
		TunningLog(_T("¢º Archive In is null"));
		return FALSE;
	}
	if(NULL != m_pArchiveOut)
	{
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);
	if(NULL == m_pArchiveOut) 
	{
		TunningLog(_T("¢º Archive Out is null"));
		return FALSE;
	}

	m_saTable.RemoveAll(); m_saTable.FreeExtra();
	m_saField.RemoveAll(); m_saField.FreeExtra();

	CString strConfFile = strPath + _T("config\\targetlist.conf");
	CStdioFile file;
	if(TRUE == file.Open(strConfFile, CFile::modeRead))
	{
		INT nIndex = 0;
		CString strLine, strTable, strField;
		while(file.ReadString(strLine))
		{
			strLine.TrimLeft(); strLine.TrimRight();
			strLine.Replace(_T(" "), _T(""));
			if(1 > strLine.GetLength()) 
			{
				TunningLog(_T("¢º targetlist.conf file is empty"));
				return FALSE;
			}
			nIndex = strLine.Find(_T(":"));
			if(0 > nIndex) 
			{
				TunningLog(_T("¢º targetlist.conf column not found"));
				return FALSE;
			}
			strTable = strLine.Mid(0, nIndex);
			strTable.MakeUpper();
			strField = strLine.Mid(nIndex+1, strLine.GetLength()-nIndex);
			strField.MakeUpper();
			m_saTable.Add(strTable);
			m_saField.Add(strField);
		}
	}
	else
	{
		TunningLog(_T("¢º targetlist.conf file does not exist"));
		return FALSE;
	}

	return TRUE;
}

INT CBO_LOC_3000Api::CheckConnection()
{
	INT nResult = 0;
	nResult = ConnectSocket();
	if(nResult)	
	{
		SendCloseFrame();
		CloseSocket();
	}
	return nResult;
}

void CBO_LOC_3000Api::ProcessPendingRead() 
{
	do
	{
		ReceiveFrame();
	}
	while(!m_pArchiveIn->IsBufferEmpty());
}

BOOL CBO_LOC_3000Api::SendFrame(CString& strText)
{	
	if(m_pArchiveOut != NULL)
	{
		CFrameCtl msg;
		msg.m_strText = strText;
		TRY
		{
			msg.Serialize(*m_pArchiveOut);
			m_pArchiveOut->Flush();
		}
		CATCH(CFileException, e)
		{
			TunningLog(_T("¢º SendFrame CFileException"));
			return FALSE;
		}
		END_CATCH
	}

	return TRUE;
}

BOOL CBO_LOC_3000Api::ReceiveFrame(CString &strFrame)
{	
	BOOL bFlag;

	ReceiveFrame();
	strFrame = m_strReceiveFrame;
	bFlag = CheckFrame(strFrame, _T("AUH"), _T("E"));
	if(bFlag) return FALSE;
	bFlag = CheckFrame(strFrame, _T("AUH"), _T("F"));
	if(bFlag) return FALSE;

	return TRUE;
}

void CBO_LOC_3000Api::ReceiveFrame()
{
	CFrameCtl msg;
	
	TRY
	{	
		msg.Serialize(*m_pArchiveIn);
		m_strReceiveFrame = msg.m_strText;
	}
	CATCH(CFileException, e)
	{
		TunningLog(_T("¢º ReceiveFrame CFileException"));
	}
	END_CATCH
}

BOOL CBO_LOC_3000Api::CheckFrame(CString sFrame, CString strCheckCMD, CString strCheckResult)
{
	INT nBodyLength;
	if(sFrame.GetLength() >= 17)
	{
		if(sFrame.Left(1) == CHAR(2) && sFrame.Right(1) == CHAR(3))
		{
			if(!(sFrame.Mid(2,3) == strCheckCMD && sFrame.Mid(5,1) == strCheckResult))			
			{
				return FALSE;
			}
		}
		nBodyLength = _ttoi(sFrame.Mid(6,10));
		if(nBodyLength+1 != sFrame.Mid(17).GetLength()) 
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CBO_LOC_3000Api::MakeEncrytion(CString strDatatype, CStringArray &pArray, BOOL bNextData)
{
	if(1 > pArray.GetSize()) return FALSE;

	INT ids = 0;
	BOOL bFlag;
	CString strNextDataFlag;
	CString sFrame;
	CString strTempDataType;
	CString strTempCMD;
	if(bNextData) strNextDataFlag=_T("1");
	else strNextDataFlag=_T("0");
	ids = MakeFrame(_T("3"), _T("ENC"), strDatatype, pArray, sFrame, strNextDataFlag);
	if(ids) return FALSE;
	SendFrame(sFrame);
	bFlag = ReceiveFrame(sFrame);
	if(FALSE == bFlag)
	{
		CloseSocket();
		return FALSE;
	}
	bFlag = CheckFrame(sFrame, _T("ENC"), strDatatype);
	if(bFlag)
	{
		ids = AnalyzeFrame(sFrame, strTempDataType, strTempCMD, strDatatype, strNextDataFlag, pArray);
		if(ids) return FALSE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CBO_LOC_3000Api::MakeDecrytion(CString strDatatype, CStringArray &pArray, BOOL bNextData)
{
	if(1 > pArray.GetSize()) return FALSE;

	INT ids = 0;
	BOOL bFlag;
	CString strNextDataFlag;
	CString sFrame;
	CString strTempDataType;
	CString strTempCMD;
	if(bNextData) strNextDataFlag=_T("1");
	else strNextDataFlag=_T("0");
	ids = MakeFrame(_T("3"), _T("DEC"), strDatatype, pArray, sFrame, strNextDataFlag);
	if(ids) return FALSE;
	SendFrame(sFrame);
	bFlag = ReceiveFrame(sFrame);
	if(FALSE == bFlag)
	{
		CloseSocket();
		return FALSE;
	}
	bFlag = CheckFrame(sFrame, _T("DEC"), strDatatype);
	if(bFlag)
	{
		ids = AnalyzeFrame(sFrame, strTempDataType, strTempCMD, strDatatype, strNextDataFlag, pArray);
		if(ids) return FALSE;
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CBO_LOC_3000Api::GetEncKeyword(CStringArray &pArray)
{
	if(1 > pArray.GetSize()) return FALSE;
	
	INT	ids = 0; 		
	BOOL bFlag;
	CString	strTempNextDataFlag;
	CString	strTempDataType;
	CString	strTempCMD;
	CString	strResultArray;
	CString	sFrame;
	ids = MakeFrame(_T("3"), _T("SRH"), _T("K"), pArray, sFrame, _T("0"));
	if(ids) return FALSE;

	SendFrame(sFrame);
	bFlag = ReceiveFrame(sFrame);
	if(FALSE == bFlag) 
	{
		CloseSocket();
		return FALSE;
	}
	bFlag = CheckFrame(sFrame, _T("SRH"), _T("K"));
	if(bFlag)
	{
		ids = AnalyzeFrame(sFrame, strTempDataType, strTempCMD, strTempDataType, strTempNextDataFlag, pArray);
		if(ids) return FALSE;
	}	

	return TRUE;
}

BOOL CBO_LOC_3000Api::GetDecKeyword(CStringArray &pArray)
{
	if(1 > pArray.GetSize()) return FALSE;
	
	INT	ids = 0;		
	BOOL bFlag;
	CString	strTempNextDataFlag;
	CString	strTempDataType;
	CString	strTempCMD;
	CString	strResultArray;
	CString	sFrame;
	ids = MakeFrame(_T("3"), _T("SRH"), _T("N"), pArray, sFrame, _T("0"));
	if(ids) return FALSE;

	SendFrame(sFrame);
	bFlag = ReceiveFrame(sFrame);
	if(FALSE == bFlag)
	{
		CloseSocket();
		return FALSE;
	}
	bFlag = CheckFrame(sFrame, _T("SRH"), _T("N"));
	if(bFlag)
	{
		ids = AnalyzeFrame(sFrame, strTempDataType, strTempCMD, strTempDataType, strTempNextDataFlag, pArray);
		if(ids) return FALSE;
	}

	return TRUE;
}

BOOL CBO_LOC_3000Api::UserInfoEncrytion(CString strTable, CString strField, CStringArray &pArray)
{
	if(strTable.IsEmpty() || strField.IsEmpty())
	{
		SendCloseFrame();
		return FALSE;
	}

	if(FALSE == CheckTarget(strTable, strField))
	{
		SendCloseFrame();
		return FALSE;
	}

	if(1 > pArray.GetSize())
	{
		SendCloseFrame();
		return FALSE;
	}

	return GetEncKeyword(pArray);
}

BOOL CBO_LOC_3000Api::UserInfoDecrytion(CString strTable, CString strField, CStringArray &pArray)
{
	if(strTable.IsEmpty() || strField.IsEmpty())
	{
		SendCloseFrame();
		return FALSE;
	}

	if(FALSE == CheckTarget(strTable, strField)) 
	{
		SendCloseFrame();
		return FALSE;
	}

	if(1 > pArray.GetSize())
	{
		SendCloseFrame();
		return FALSE;
	}

	return GetDecKeyword(pArray);
}

BOOL CBO_LOC_3000Api::CheckTarget(CString strTable, CString strField)
{
	strTable.MakeUpper();
	strField.MakeUpper();
	strField.Replace(_T(";"), _T(""));
	CString strObject = _T("%") + strField + _T("%");
	for(INT i = 0; i < m_saTable.GetSize(); i++)
	{
		if(m_saTable.GetAt(i) == strTable)
		{
			if(-1 < m_saField.GetAt(i).Find(strObject))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CBO_LOC_3000Api::SendCloseFrame()
{	
	CStringArray pArray;
	CString sFrame;
	MakeFrame("3", "OUT", "C", pArray, sFrame, "1");		
	SendFrame(sFrame);
}

BOOL CBO_LOC_3000Api::OnEraseBkgnd(CDC* pDC) 
{
	return CDialog::OnEraseBkgnd(pDC);
}
