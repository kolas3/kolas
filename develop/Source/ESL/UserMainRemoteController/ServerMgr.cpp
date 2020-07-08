// ServerMgr.cpp : implementation file
//

#include "stdafx.h"
#include "ServerMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerMgr

CServerMgr::CServerMgr()
{
	m_pServerSocket = NULL;
	// KOL.UDF.022 시큐어코딩 보완
	m_pClientSocket = NULL;
	m_ptrList.RemoveAll();
	m_pHwnd = NULL;
}

CServerMgr::~CServerMgr()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerMgr, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerMgr)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerMgr member functions



/////////////////////////////////////////////////////////////////////////////
//핸들저장 함수
void CServerMgr::SetWnd(HWND hwnd)
{
	m_pHwnd	=	hwnd;
}


/////////////////////////////////////////////////////////////////////////////
//서버 소켓을 생성 함수
INT CServerMgr::CreateServer()
{
	m_pServerSocket = new CServerSocket;
	if( NULL == m_pServerSocket )
	{
		return	-100;
	}

	m_pServerSocket->SetWnd(m_pHwnd);
 	if( !m_pServerSocket->Create(5000,SOCK_STREAM) )
	{
		return	GetLastError();
	}

	if( !m_pServerSocket->Listen() )
	{
		return	GetLastError();
	}
	return	GetLastError();
}

////////////////////////////////////////////////////////////////////////////
//클라이언트의 접속요청을 받는 함수
//클라이언트의 접속 요청을 받고, 통신 할 수 있는 새로운 소켓을 생성
//클라이언트와 소켓을 연결시켜준다
LONG CServerMgr::OnAcceptClient(UINT wParam, LONG lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	INT	nResult	= 1;

	m_pClientSocket	=	new CClientSocket;
	if( NULL	==	m_pServerSocket )
	{
		return	-200;
	}

	m_pClientSocket->SetWnd(m_pHwnd);

	if( !m_pServerSocket->Accept(*m_pClientSocket) )
	{
		return	GetLastError();
	}

	//접속한 Client의 Pointer 변수와, IP, Port를 저장 하는 변수 선언
	CClientData*	clientData	=	new	CClientData;
	CString			strIP		=	"";
	UINT			nPort		=	0;

	if( NULL	==	clientData )
	{
		return	-201;
	}

	if( !m_pClientSocket->GetPeerName(strIP,nPort) )
	{
		return	GetLastError();
	}
	
	
	/////////////////////////////////////////////
	//같은 IP 접속 Check 함수

	//CheckDuplicateIP(strIP);

	
	//접속한 Client의 IP와 Port를 저장
	clientData->m_strIP			= strIP;
	clientData->m_pClientSocket	= m_pClientSocket;


	//접속에 성공했다는 Message를 Client에 보낸다
	nResult	=	ProcessSend(m_pClientSocket,TYPE_CONNECT_SUCCESS);
	if( 1	!=	nResult )
	{
		return	nResult;
	}
	//Object에 저장
	m_ptrList.AddTail(clientData);
	
	return	1;
}


/////////////////////////////
//서버 종료시 처리 하는 함수
LONG CServerMgr::OnCloseServer() 
{
	INT			nResult		=	CloseAllClient();
	if( 1	!=	nResult )
	{
		return nResult;
	}

	nResult		=	ProcessCloseServer();
	if( 1	!=	nResult )
	{
		return nResult;
	}
	/*
	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		POSITION		pos = m_ptrList.FindIndex(i);
		if( NULL == pos )
		{
			nResult = -301;
			return	nResult;
		}

		CClientData*	pClientData;
		pClientData		= (CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pClientSocket = pClientData->m_pClientSocket;		

		nResult			= ProcessSend(pClientSocket,TYPE_CLOSE_SERVER);
		if( 1 != nResult )
		{
			return nResult;
		}

		ProcessCloseClient(pClientData,pos);
		if( GetClientCnt() != nClientCnt  )
		{
			nClientCnt--;
			i--;
		}
	}
	*/
	return 1;
}

//////////////////////////////////////////////
//클라이언트에서 메시지 받았을 경우 처리 함수
LONG CServerMgr::OnReceiveMsg(UINT wParam, LONG lParam) 
{
	INT nResult		= 1;
	INT nClientCnt	= GetClientCnt();
	if( nClientCnt < 1 )
	{
		return	-400;
	}

	CSocketData	Soketdata;

	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		POSITION	pos	= m_ptrList.FindIndex(i);
		if( NULL	==	pos )
		{
			return	-401;
		}

		CClientData*	pClientData;
		pClientData		= (CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pSocket = pClientData->m_pClientSocket;
		if( pSocket	==	(CClientSocket*)wParam )
		{			
			do
			{
				if( !pSocket->Receive( &Soketdata, sizeof(Soketdata) ) )
				{
					return	GetLastError();
				}

				nResult		= ProcessData(&Soketdata, pClientData);
				if( 1	!=	nResult )
				{
					return	nResult;
				}
			}			
			while( pSocket->IsSerializable() );
		}		
		if(nClientCnt	!=	GetClientCnt())
		{
				nClientCnt--;
				i--;
		}
	}
	return	1;
}


////////////////////////////////////////
//클라이언트 한테 받은 메시지 처리 함수
//-------------------------------------------------------------------------------//
INT CServerMgr::ProcessData(CSocketData *pSocketData, CClientData *pClientData)
{
	::SendMessage(m_pHwnd,WM_TYPE_INFO,(WPARAM)this,pSocketData->m_nType);

	INT nResult		= -501;
	INT nClientCnt	= GetClientCnt();
	if( nClientCnt < 1 )
	{
		nResult = -500;
	}

	CClientData		*pData;
	POSITION		pos;

	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		pos		=	m_ptrList.FindIndex(i);	
		if( NULL	==	pos )
		{
			return	-502;
		}

		pData	=	(CClientData*)m_ptrList.GetAt(pos);
		if( pData	==	pClientData )
		{					
			break;				
		}
	}
	///////////////////////////////////////////
	//받은 메시지의 타입에 따라 처리 하는 과정
	if(		 TYPE_CONNECT		==	pSocketData->m_nType )
	{	
		ProcessConnectClient(pSocketData,pClientData,pos);
		return	1;
	}

//********************************************************************************************************
	else if( TYPE_CONNECT_CHECK	==	pSocketData->m_nType )
	{
		::SendMessage(m_pHwnd,WM_RECEIVE_MSG
					,(WPARAM)this,TYPE_CONNECT_CHECK);
		return	1;
	}
	else if( TYPE_CLOSE_CLIENT	==	pSocketData->m_nType )
	{
		/*
		if( 1 == ProcessSend(pClientData->m_pClientSocket,TYPE_CLOSE_CLIENT) )
		{
			ProcessCloseClient(pClientData,pos);

			::SendMessage(m_pHwnd,WM_DELETE_CLOSE_CLIENT_LIST
						,(WPARAM)this,(LPARAM)(LPCTSTR)pClientData->m_strIP);
			return	1;
		}
		nResult = -503;
		return	nResult;
		*/
		nResult = ProcessSend(pClientData->m_pClientSocket,TYPE_CLOSE_CLIENT);
		if( 1 != nResult )
		{
			return	nResult;
		}
		ProcessCloseClient(pClientData,pos);

		::SendMessage(m_pHwnd,WM_DELETE_CLOSE_CLIENT_LIST
					,(WPARAM)this,(LPARAM)(LPCTSTR)pClientData->m_strIP);
	}
	else if( TYPE_CLOSE_SERVER	==	pSocketData->m_nType )
	{
		::SendMessage(m_pHwnd,WM_CLOSE_SERVER
					,(WPARAM)this,(LPARAM)(LPCTSTR)pClientData->m_strIP);
		return 1;
	}
	return	nResult;
}

//////////////////////////////
//클라이언트가 접속 했을 경우
void CServerMgr::ProcessConnectClient(CSocketData *Socketdata, CClientData *pClient, POSITION pos)
{
	CString		strMsg;
	strMsg.Format("%s:%s",Socketdata->m_cName,pClient->m_strIP);

	::SendMessage(m_pHwnd,WM_PRINT_CLIENT_LIST
					,0,(LPARAM)(LPCTSTR)strMsg);
}

////////////////////////////
//클라이언트 종료 처리 함수
void CServerMgr::ProcessCloseClient(CClientData *pClientData,POSITION pos)
{
	if( NULL != pClientData )
	{			
		m_ptrList.RemoveAt( pos );
	}
}

/////////////////////
//서버 종료 함수
INT CServerMgr::ProcessCloseServer()
{
	if( NULL != m_pServerSocket )
	{
		m_pServerSocket->ShutDown();

		delete			m_pServerSocket;
		m_pServerSocket	= NULL;
	}
	else
	{
		return	-1001;
	}
	return	1;
}

///////////////////////////////////////
//클라이언트 전체에 메시지 보내는 함수
//-------------------------------------------------------------------------------//
INT CServerMgr::SendMsg(INT nType)
{
	INT nResult		=	1;

	INT	nClientCnt = GetClientCnt();
	//각 클라이언트로 메시지 전송
	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		POSITION	pos	=	m_ptrList.FindIndex(i);
		if( NULL	==	pos )
		{
			return		-701;
		}
		CClientData*	pClientData;
		pClientData		=	(CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pClientSocket	=	pClientData->m_pClientSocket;		

		nResult			=	ProcessSend(pClientSocket,nType);
		if( 1 != nResult )
		{
			return	nResult;
		}
	}
	return	1;
}

//////////////////////////////////
//클라이언트에 메시지 보내는 함수
INT CServerMgr::ProcessSend(CClientSocket *pSocket, INT nType)
{
	INT				nReturn		= -600;
	CSocketData*	pSocketData	= new CSocketData;
	if( NULL	==	pSocketData )
	{
		return		-601;
	}

	if( NULL	!=	pSocketData )
	{
		pSocketData->m_nType	=	nType;
	
		if( !pSocket->Send( pSocketData , sizeof(pSocketData) ) )
		{
			return	GetLastError();
		}
		delete	pSocketData;
		return	1;
	}
	return	nReturn;
}

/////////////////////////////////////////
//선택한 클라이언트에 메시지 보내는 함수
INT CServerMgr::SendToSelectClient(INT nType, CString strIp)
{
	// 클라이언트에 메시지 전송
	INT nResult		= -801;

	INT nClientCnt	= GetClientCnt();
	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		POSITION		pos = m_ptrList.FindIndex(i);
		if( NULL	==	pos )
		{
			return		-802;
		}
		CClientData*	pClientData;
		pClientData	=	(CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pSocket = pClientData->m_pClientSocket;


		//IP로 Client Check
		if( strIp	== pClientData->m_strIP )
		{
			nResult	=	ProcessSend(pSocket,nType);
			if( 1	!= nResult )
			{
				return	nResult;
			}
			break;
		}
		
		/*
		//Socket으로 Client Check
		if( strIp	== pClientData->m_pClientSocket )
		{
			nResult	=	ProcessSend(pSocket,nType);
			if( 1	!= nResult )
			{
				return	nResult;
			}
			break;
		}
		*/
		if(nClientCnt != GetClientCnt())
		{
			nClientCnt--;
			i--;
		}
	}
	return	nResult;
}



//모든 클라이언트 종료
INT CServerMgr::CloseAllClient()
{	
	INT nResult		= 1;

	INT nClientCnt	= GetClientCnt();
	//클라이언트의 갯수 만큼 반복을 하여
	//각 클라이언트 종료
	for( INT nIndex = nClientCnt-1 ; nIndex >= 0 ; nIndex-- )
	{
		POSITION pos	= m_ptrList.FindIndex(nIndex);
		if( NULL == nResult )
		{
			return		-902;
		}

		CClientData*	pClientData;
		pClientData		= (CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pClientSocket = pClientData->m_pClientSocket;		

		ProcessCloseClient(pClientData,pos);
		/*
		if( GetClientCnt() != nClientCnt  )
		{
			nClientCnt--;
			nIndex--;
		}
		*/
	}
	return	1;
}



BOOL CServerMgr::CloseSelectClient(INT nIndex)
{
	//클라이언트 종료
	POSITION pos	= m_ptrList.FindIndex(nIndex);
	if( NULL == pos )
	{
		return	FALSE;
	}

	CClientData*	pClientData;
	pClientData		= (CClientData*)m_ptrList.GetAt(pos);
	CClientSocket*	pClientSocket = pClientData->m_pClientSocket;		
	ProcessCloseClient(pClientData,pos);

	return	TRUE;
}


////////////////////////////////////////////////
//접속 되어 있는 클라이언트의 갯수를 구하는 함수
INT CServerMgr::GetClientCnt()
{
	return m_ptrList.GetCount();
}

void CClientData::Close()
{
	m_strIP		= "";
	m_strName	= "";
	if( m_pClientSocket )
	{
		m_pClientSocket->ShutDown(2);
		m_pClientSocket->Close();
		if(NULL != m_pClientSocket)
		{
			delete		m_pClientSocket;
		}
		m_pClientSocket	= NULL;
	}
}

INT CServerMgr::CheckDuplicateIP(CString strIP)
{
	INT nResult;

	INT nClientCnt	= GetClientCnt();	
	for( int i = 0 ; i < nClientCnt ; i++ )
	{
		POSITION	pos	=	m_ptrList.FindIndex(i);
		if( NULL	==	pos )
		{
			return	-1101;
		}

		CClientData*	pClientData;
		pClientData		=	(CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pSocket	=	pClientData->m_pClientSocket;
		if( strIP	==	pClientData->m_strIP )
		{

			nResult		=	ProcessSend(m_pClientSocket,TYPE_CONNECT_FAILED);
			if( 1	!=	nResult )
			{
				return	nResult;
			}
			return	-1102;
		}		
	}
	return 1;
}
