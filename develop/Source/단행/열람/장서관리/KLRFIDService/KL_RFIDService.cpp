// KL_RFIDService.cpp: implementation of the CKL_RFIDService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KL_RFIDService.h"
#include "StdioFileEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKL_RFIDService::CKL_RFIDService() : m_Writer(NULL)
{		
	m_bIsBusy = FALSE;
	m_pServerSocket = NULL;
	m_pClientSocket = NULL;
	m_bIsRegisterWnd = FALSE;
	m_bIsWritableTags = FALSE;
	PutCommand(KL_MEMBER_CARD_CLOSE);

	RegisterWnd();
	BOOL bResult = CWnd::CreateEx(WS_EX_TOOLWINDOW, _T("KL_RFIDService"), NULL, WS_POPUP, 0, 0, 10, 10, NULL, NULL);
}

CKL_RFIDService::~CKL_RFIDService()
{
	if(m_pServerSocket)
	{
		delete m_pServerSocket;
		m_pServerSocket = NULL;
	}
	if(m_pClientSocket)
	{
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}
void CKL_RFIDService::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_RFIDService)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKL_RFIDService, CWnd)
	//{{AFX_MSG_MAP(CPopupImage)	
	ON_MESSAGE(WM_MSG_RECEIVE, OnReceiveMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

INT CKL_RFIDService::StartService(UINT nPort)
{
	BOOL bResult = FALSE;
	CloseService();

	m_pServerSocket = new CKLServerSocket;
	m_pServerSocket->SetParentHandle(GetSafeHwnd());
	bResult = m_pServerSocket->Create(nPort);
	if(!bResult)
	{
		return GetLastError();
	}
	bResult = m_pServerSocket->Listen();
	if(!bResult)
	{
		return GetLastError();
	}

	return 0;
}

INT CKL_RFIDService::CloseService()
{
	if(m_pServerSocket)
	{
		m_pServerSocket->Close();
		delete m_pServerSocket;
		m_pServerSocket = NULL;
	}

	if(m_pClientSocket)
	{
		m_pClientSocket->Close();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}

	return 0;
}

void CKL_RFIDService::RegisterWnd()
{
	if (m_bIsRegisterWnd)
	{
		return;
	}

    HINSTANCE hInst = AfxGetInstanceHandle( );

    // initialise the window class information
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
	wndcls.lpszClassName = _T("KL_RFIDService");

    // register the window class
    if (!AfxRegisterClass(&wndcls))
		AfxThrowResourceException();

	m_bIsRegisterWnd = TRUE;
}


LRESULT CKL_RFIDService::OnReceiveMsg(WPARAM wParam, LPARAM lParam)
{ // 0
	INT nIds = 0;
	INT nErrorCode = 0;
	if(m_bIsBusy)
	{
		MSG msg;
		DWORD dwStart;
		dwStart = GetTickCount();
		
		while (GetTickCount() - dwStart < 1000)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	BOOL bReady = FALSE;


	m_bIsBusy = TRUE;
	CHAR* szChar = (CHAR*)lParam;	
	CFrameReader Reader;
	BOOL bParse = Reader.parseFrame(szChar);
	if(!bParse)
	{		
		SendError(-20000);
	}
	CString strCommand = Reader.getCommand();
	CString strResult;
	if(_T("KL_MEMBER_CARD_OPEN") == strCommand)
	{
		nIds = Open(strResult);
		if(0 == nIds)
		{
			CFrameWriter Writer(NULL);
			Writer.setCommand(_T("KL_MEMBER_CARD_OPEN"));
			Writer.addRecord();
			Writer.addElement(_T("결과"), strResult);
			Send(&Writer);
		}
	}
	else if(_T("KL_MEMBER_STATUS") == strCommand)
	{
		BOOL bIsRegister = FALSE;
		CString strUserNo = Reader.getElement(0, _T("통합대출자번호"));
		CString strUID = Reader.getElement(0, _T("UID"));
		nIds = IsRegsisterUID(strUserNo, strUID, strResult);
		if(0 == nIds)
		{
			CFrameWriter Writer(NULL);
			Writer.setCommand(_T("KL_MEMBER_STATUS"));
			Writer.addRecord();
			Writer.addElement(_T("결과"), strResult);
			Send(&Writer);
		}
	}
	else if(_T("KL_TAG_TYPE_SET") == strCommand)
	{
		CString strTagType = Reader.getElement(0, _T("TYPE"));
		nIds = SetTagType(strTagType, strResult);
		if(0 == nIds)
		{
			CFrameWriter Writer(NULL);
			Writer.setCommand(_T("KL_TAG_TYPE_SET"));
			Writer.addRecord();
			Writer.addElement(_T("결과"), strResult);
			Send(&Writer);
		}
	}
	else if(_T("KL_LIBRARY") == strCommand)
	{
		CString strLibCode;
		nIds = GetLibCode(strLibCode);
		if(0 == nIds)
		{
			CFrameWriter Writer(NULL);
			Writer.setCommand(_T("KL_LIBRARY"));
			Writer.addRecord();
			Writer.addElement(_T("결과"), strLibCode);
			Send(&Writer);
		}
	}
	else if(_T("KL_MEMBER_NO") == strCommand)
	{
		CString strUserNo;
		nIds = GetUserNo(strUserNo);
		if(0 == nIds)
		{
			CFrameWriter Writer(NULL);
			Writer.setCommand(_T("KL_MEMBER_NO"));
			Writer.addRecord();
			Writer.addElement(_T("결과"), strUserNo);
			Send(&Writer);
		}
	}
	else if(_T("KL_MEMBER_WRTIE_RFID_UID") == strCommand)
	{
		CString strUserNo = Reader.getElement(0, _T("통합대출자번호"));
		CString strUID = Reader.getElement(0, _T("UID"));
		nIds = RegsisterUID(strUserNo, strUID, strResult);
		if(0 == nIds)
		{
			CFrameWriter Writer(NULL);
			Writer.setCommand(_T("KL_MEMBER_WRTIE_RFID_UID"));
			Writer.addRecord();
			Writer.addElement(_T("결과"), strResult);
			Send(&Writer);
		}
	}
	else if(_T("KL_MEMBER_CARD_CLOSE") == strCommand)
	{
		nIds = Close(strResult);
		if(0 == nIds)
		{
			CFrameWriter Writer(NULL);
			Writer.setCommand(_T("KL_MEMBER_CARD_CLOSE"));
			Writer.addRecord();
			Writer.addElement(_T("결과"), strResult);
			Send(&Writer);
		}
	}
 	if(0 != nIds)
	{
		SendError(nIds);

		m_bIsBusy = FALSE;
		return nIds;
	}

// 	CString strData;	
// 	CStdioFile File;
// 	if(File.Open(_T("..\\cfg\\KL\\KLRFID"), CFile::modeRead))
// 	{
// 		bReady = TRUE;
// 		File.ReadString(strData);
// 		File.Close();
// 	}
// 	HWND hWnd = HWND(_ttoi(strData));
// 	BOOL bWnd = ::IsWindow(hWnd);
// 	if(bWnd)
// 	{
// 		::SendMessage(hWnd, WM_USER+1030, NULL, NULL);
// 	}
// 	else
// 	{
// 		nIds = -20001;
// 	}

	m_bIsBusy = FALSE;

	return nIds;
}

INT CKL_RFIDService::Open(CString& strResult)
{// 1 : KL_MEMBER_CARD_OPEN
	INT nErrorCode = -20100;
// 	if(7 != m_nPrevCommand)
// 	{
// 		return nErrorCode;
// 	}

	CString strData = _T("KL_MEMBER_CARD_OPEN");
	INT nResult = SendK3Data(strData);
	if(0 == nResult)
	{
		nResult = ReadK3Result(_T("KL_MEMBER_CARD_OPEN"), strResult);
	}
	PutCommand(KL_MEMBER_CARD_OPEN);
	return nResult;
}

INT CKL_RFIDService::IsRegsisterUID(CString& strUserNo, CString& strUID, CString& strResult)
{  // 2 : KL_MEMBER_STATUS
	INT nErrorCode = -20200;

	strResult.Empty();
// 	if(1 != m_nPrevCommand)
// 	{
// 		return nErrorCode;
// 	}
// 	if(strUserNo.IsEmpty())
// 	{
// 		return -20201;
// 	}
	if(strUID.IsEmpty())
	{
		return -20202;
	}

	CString strData = _T("KL_MEMBER_STATUS|") + strUserNo + _T("|") + strUID;
	INT nResult = SendK3Data(strData);
	if(0 == nResult)
	{
		nResult = ReadK3Result(_T("KL_MEMBER_STATUS"), strResult);
// 		m_strUserNo = strUserNo;
	}	

	PutCommand(KL_MEMBER_STATUS);
	return nResult;
}

INT CKL_RFIDService::GetLibCode(CString& strLibCode)
{ // 3 : KL_LIBRARY
	INT nErrorCode = -20300;

// 	if(2 != m_nPrevCommand)
// 	{
// 		return nErrorCode;
// 	}
	// 기록가능한 태그가 아닌데, 도서관부호를 요청한 경우
	if(!m_bIsWritableTags)
	{
		return -20301;
	}
	CString strData = _T("KL_LIBRARY");
	INT nResult = SendK3Data(strData);
	if(0 == nResult)
	{
		nResult = ReadK3Result(_T("KL_LIBRARY"), strLibCode);
	}

	PutCommand(KL_LIBRARY);
	return nResult;
}

INT CKL_RFIDService::GetUserNo(CString& strUserNo)
{ // 4 : KL_MEMBER_NO
	INT nErrorCode = -20400;

// 	if(3 != m_nPrevCommand)
// 	{
// 		return nErrorCode;
// 	}
	// 기록가능한 태그가 아닌데, 이용자번호를 요청한 경우
	if(!m_bIsWritableTags)
	{
		return -20401;
	}

	CString strData = _T("KL_MEMBER_NO");
	INT nResult = SendK3Data(strData);
	if(0 == nResult)
	{
		nResult = ReadK3Result(_T("KL_MEMBER_NO"), strUserNo);
		m_strUserNo = strUserNo;
	}

	PutCommand(KL_MEMBER_NO);
	return nResult;
}

INT CKL_RFIDService::RegsisterUID(CString& strUserNo, CString& strUID, CString& strResult)
{ // 5 : KL_MEMBER_WRTIE_RFID_UID
	INT nErrorCode = -20500;

	if(strUID.IsEmpty())
	{
		return -20502;
	}

	// 태그가 기록가능하냐에 따라 함수호출순서와 인자값을 체크한다.
	if(m_bIsWritableTags)
	{
		if(strUserNo.IsEmpty())
		{
			return -20501;
		}
		if(strUserNo != m_strUserNo)
		{
			return -20503;
		}

		if(
			KL_MEMBER_CARD_OPEN != m_nPrevCommand[0] 
			|| KL_MEMBER_STATUS != m_nPrevCommand[1] 
			|| KL_TAG_TYPE_SET != m_nPrevCommand[2] 
			|| KL_LIBRARY != m_nPrevCommand[3]
			|| KL_MEMBER_NO != m_nPrevCommand[4]
			)
		{
			return -20504;
		}
	}
	else
	{	// 기록이 불가능한 태그의 경우엔 대출자번호나 도서관부호 요청없이 바로 태깅이 들어가야 한다.
		if(
			KL_MEMBER_CARD_OPEN != m_nPrevCommand[2] 
			|| KL_MEMBER_STATUS != m_nPrevCommand[3] 
			|| KL_TAG_TYPE_SET != m_nPrevCommand[4] 
			)
		{
			return -20505;
		}
	}

	CString strData = _T("KL_MEMBER_WRTIE_RFID_UID|") + strUserNo + _T("|") + strUID;
	INT nResult = SendK3Data(strData);
	if(0 == nResult)
	{
		nResult = ReadK3Result(_T("KL_MEMBER_WRTIE_RFID_UID"), strResult);
	}

	PutCommand(KL_MEMBER_WRTIE_RFID_UID);
	return nResult;
}

INT CKL_RFIDService::Close(CString& strResult)
{ // 7 : KL_MEMBER_CARD_CLOSE
	INT nErrorCode = -20700;

// 	if(7 == m_nPrevCommand)
// 	{
// 		return nErrorCode;
// 	}
	CString strData = _T("KL_MEMBER_CARD_CLOSE");
	INT nResult = SendK3Data(strData);
	if(0 == nResult)
	{
		nResult = ReadK3Result(_T("KL_MEMBER_CARD_CLOSE"), strResult);
	}

	PutCommand(KL_MEMBER_CARD_CLOSE);
	return nResult;
}

INT CKL_RFIDService::Send(CFrameWriter* pWriter)
{
	if(pWriter)
	{
		CHAR* szFrame = NULL;
		pWriter->getFrame(&szFrame);
		m_pServerSocket->Send(szFrame, strlen(szFrame));
		delete []szFrame;
	}
	return 0;
}

INT CKL_RFIDService::SendError(INT nErrorCode)
{
	CString strErrorCode;
	CFrameWriter Writer(NULL);
	Writer.setCommand(_T("ERROR"));
	Writer.addRecord();
	strErrorCode.Format(_T("%d"), nErrorCode);
	Writer.addElement(_T("결과"), strErrorCode);

	return Send(&Writer);
}

INT CKL_RFIDService::SendK3Data(CString& strData)
{
	HWND hWnd = NULL;
	CStdioFileEx File;
	if(!File.Open(_T("..\\cfg\\KL\\KLRFID"), CFile::modeRead|CFile::typeText))
	{
		return -20002;
	}
	CString strData1;	
	File.ReadString(strData1);
	File.Close();
	
	hWnd = HWND(_ttoi(strData1));
	if(!::IsWindow(hWnd))
	{
		return -20003;
	}
	
	if(!File.Open(_T("..\\cfg\\KL\\KLRFIDS"), CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode))
	{
		return -20004;
	}		
	File.WriteString(strData);
	File.Close();

	INT nResult = ::SendMessage(hWnd, WM_USER+1030, NULL, NULL);	
	return nResult;
}

INT CKL_RFIDService::ReadK3Result(CString strCommand, CString& strData)
{
	CStdioFileEx File;
	if(!File.Open(_T("..\\cfg\\KL\\KLRFIDR"), CFile::modeRead|CFile::typeText))
	{
		return -20002;
	}
	CString strLine, strData1, strData2;
	File.ReadString(strLine);
	File.Close();

	AfxExtractSubString(strData1, strLine, 0, '|');
	AfxExtractSubString(strData2, strLine, 1, '|');

	if(strData1 != strCommand)
	{
		return -20003;
	}
	else
	{
		strData = strData2;
	}

	return 0;
}

void CKL_RFIDService::PutCommand(INT nCommand)
{
	m_nPrevCommand[0] = m_nPrevCommand[1];
	m_nPrevCommand[1] = m_nPrevCommand[2];
	m_nPrevCommand[2] = m_nPrevCommand[3];
	m_nPrevCommand[3] = m_nPrevCommand[4];
	m_nPrevCommand[4] = nCommand;

	return;
}

INT CKL_RFIDService::SetTagType(CString& strTagType, CString& strResult)
{ // KL_TAG_TYPE_SET
	INT nErrorCode = -20600;

	if(_T("ISO14443A") == strTagType)
	{
		strResult = _T("성공");
		m_bIsWritableTags = FALSE;
	}
	else if(_T("ISO15693") == strTagType)
	{
		strResult = _T("성공");
		m_bIsWritableTags = TRUE;
	}
	else
	{
		strResult = _T("실패");	
		m_bIsWritableTags = FALSE;
	}
	PutCommand(KL_TAG_TYPE_SET);

	return 0;
}