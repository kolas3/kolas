// Service.cpp: implementation of the CService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Service.h"
#include "StdioFileEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CService::CService()
{
	m_bIsBusy = FALSE;
	m_pServerSocket = NULL;
	m_pClientSocket = NULL;
	m_bIsRegisterWnd = FALSE;

	RegisterWnd();
	BOOL bResult = CWnd::CreateEx(WS_EX_TOOLWINDOW, _T("Service"), NULL, WS_POPUP, 0, 0, 10, 10, NULL, NULL);
}

CService::~CService()
{
	if(m_pServerSocket) {
		delete m_pServerSocket;
		m_pServerSocket = NULL;
	}
	if(m_pClientSocket) {
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}
void CService::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CService)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CService, CWnd)
	//{{AFX_MSG_MAP(CPopupImage)	
	ON_MESSAGE(WM_MSG_RECEIVE, OnReceiveMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

INT CService::StartService(UINT nPort)
{
	BOOL bResult = FALSE;
	CloseService();

	m_pServerSocket = new CServerSocket;
	m_pServerSocket->SetParentHandle(GetSafeHwnd());
	bResult = m_pServerSocket->Create(nPort);
	if(!bResult) {
		return GetLastError();
	}
	bResult = m_pServerSocket->Listen();
	if(!bResult) {
		return GetLastError();
	}

	return 0;
}

INT CService::CloseService()
{
	if(m_pServerSocket) {
		m_pServerSocket->Close();
		delete m_pServerSocket;
		m_pServerSocket = NULL;
	}

	if(m_pClientSocket) {
		m_pClientSocket->Close();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}

	return 0;
}

void CService::RegisterWnd() 
{
	if (m_bIsRegisterWnd) {
		return;
	}

    HINSTANCE hInst = AfxGetInstanceHandle( );

	WNDCLASS wndcls;
	wndcls.style         = CS_SAVEBITS | CS_DBLCLKS;
    wndcls.lpfnWndProc   = ::DefWindowProc;
	wndcls.cbClsExtra    = 0;
    wndcls.cbWndExtra    = 0;
	wndcls.hInstance     = hInst;
	wndcls.hIcon         = NULL;
    wndcls.hCursor       = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground = NULL;
	wndcls.lpszMenuName  = NULL;
	wndcls.lpszClassName = _T("Service");

    if (!AfxRegisterClass(&wndcls)) {
		AfxThrowResourceException();
	}

	m_bIsRegisterWnd = TRUE;
}

LRESULT CService::OnReceiveMsg(WPARAM wParam, LPARAM lParam) 
{
	INT nIds = 0;
	INT nErrorCode = 0;
	if(m_bIsBusy) {
		MSG msg;
		DWORD dwStart = GetTickCount();
		while (GetTickCount() - dwStart < 1000) {
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	
	m_bIsBusy = TRUE;

	CHAR* szChar = (CHAR*)lParam;	
	CFrameReader Reader;
	BOOL bParse = Reader.parseFrame(szChar);
	if(!bParse) {
		return SendError(-1000);
	}
	CString strCommand = Reader.getCommand();
	CString strResult;

	if (_T("LRC_001_SERVICE") == strCommand) {
		CString strUserNo = Reader.getElement(0, _T("대출자번호"));
		nIds = UserSearch(strCommand, strUserNo, strResult);
	} 
	else if (_T("LRC_002_SERVICE") == strCommand) {
		CString strUserNo = Reader.getElement(0, _T("대출자번호"));
		CString strRegNo = Reader.getElement(0, _T("등록번호"));
		nIds = Loan(strCommand, strUserNo, strRegNo, strResult);
	} 
	else if (_T("LRC_003_SERVICE") == strCommand) {
		CString strUserNo = Reader.getElement(0, _T("대출자번호"));
		CString strRegNo = Reader.getElement(0, _T("등록번호"));
		nIds = Return(strCommand, strUserNo, strRegNo, strResult);
	}
	else if (_T("LRC_004_SERVICE") == strCommand) {
		CString strValue = Reader.getElement(0, _T("기능"));
		nIds = Extra(strCommand, strValue, strResult);
	}
	if (0 == nIds) {
		CFrameWriter Writer(NULL);
		Writer.setCommand(strCommand);
		Writer.addRecord();
		Writer.addElement(_T("결과"), strResult);
		Send(&Writer);
	} else {
		SendError(nIds);
	}

	m_bIsBusy = FALSE;
	return nIds;
}

INT CService::Send(CFrameWriter* pWriter)
{
	if(pWriter) {
		CHAR* szFrame = NULL;
		pWriter->getFrame(&szFrame);
		m_pServerSocket->Send(szFrame, strlen(szFrame));
		delete []szFrame;
	}
	return 0;
}

INT CService::SendError(INT nErrorCode)
{
	CString strErrorCode;
	CFrameWriter Writer(NULL);
	Writer.setCommand(_T("ERROR"));
	Writer.addRecord();
	strErrorCode.Format(_T("%d"), nErrorCode);
	Writer.addElement(_T("결과"), strErrorCode);

	return Send(&Writer);
}

INT CService::SendK3Data(CString& strData) {
	HWND hWnd = NULL;
	CStdioFileEx File;
	if (!File.Open(_T("..\\cfg\\3100HWND"), CFile::modeRead|CFile::typeText)) {
		return -1001;
	}
	CString strData1;
	File.ReadString(strData1);
	File.Close();
	
	hWnd = HWND(_ttoi(strData1));
	if (!::IsWindow(hWnd)) {
		return -1002;
	}
	
	if (!File.Open(_T("..\\cfg\\3100LRCtoKOLAS"), CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode)) {
		return -1003;
	}		
	File.WriteString(strData);
	File.Close();

	INT nResult = ::SendMessage(hWnd, WM_USER+3100, NULL, NULL);
	return nResult;
}

INT CService::ReadK3Result(CString strCommand, CString& strData)
{
	CStdioFileEx File;
	if(!File.Open(_T("..\\cfg\\3100KOLAStoLRC"), CFile::modeRead|CFile::typeText)) {
		return -1004;
	}
	CString strLine, strData1, strData2;
	File.ReadString(strLine);
	File.Close();

	AfxExtractSubString(strData1, strLine, 0, '|');
	AfxExtractSubString(strData2, strLine, 1, '|');

	if(strData1 != strCommand) {
		return -1005;
	} else {
		strData = strData2;
	}

	return 0;
}

INT CService::UserSearch(CString strCommand, CString strUserNo, CString& strResult)
{
	if (TRUE == strUserNo.IsEmpty()) {
		return -1006;
	}

	CString strData = strCommand + _T("|") + strUserNo;
	INT nResult = SendK3Data(strData);
	if(0 == nResult) {
		nResult = ReadK3Result(strCommand, strResult);
	}
	return nResult;
}

INT CService::Loan(CString strCommand, CString strUserNo, CString strRegNo, CString& strResult)
{
	if (TRUE == strRegNo.IsEmpty()) {
		return -1008;
	}

	CString strData = strCommand + _T("|") + strUserNo + _T("|") + strRegNo;
	INT nResult = SendK3Data(strData);
	if(0 == nResult) {
		nResult = ReadK3Result(strCommand, strResult);
	}
	return nResult;
}

INT CService::Return(CString strCommand, CString strUserNo, CString strRegNo, CString& strResult)
{
	if (TRUE == strUserNo.IsEmpty()) {
		return -1009;
	}
	if (TRUE == strRegNo.IsEmpty()) {
		return -1010;
	}
	
	CString strData = strCommand + _T("|") + strUserNo + _T("|") + strRegNo;
	INT nResult = SendK3Data(strData);
	if(0 == nResult) {
		nResult = ReadK3Result(strCommand, strResult);
	}
	return nResult;
}

INT CService::Extra(CString strCommand, CString strValue, CString& strResult)
{
	if (TRUE == strValue.IsEmpty()) {
		return -1011;
	}
	
	CString strData = strCommand + _T("|") + strValue;
	INT nResult = SendK3Data(strData);
	if(0 == nResult) {
		nResult = ReadK3Result(strCommand, strResult);
	}
	return nResult;
}
