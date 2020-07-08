// Txrx.cpp: implementation of the CTxrx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Txrx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTxrx::CTxrx()
{
	m_nRequestID = (INT)GetSessionRequestID();
}

CTxrx::~CTxrx()
{

}

INT CTxrx::InitEnvVar(INT nMode)
{

	//++2008.09.25 UPDATE BY CJY {{++
	// 설정파일을 제대로 못읽던 현상 수정
	CString strFile = _T("..\\cfg\\BookTrade\\");	

	switch(nMode)
	{
	case ISSUE_ID:
		strFile += _T("ds97_ISSUEID.cfg");
		break;
	case EPC_CHECK_IN:
	case LOAN_HISTORY:
	case MAKE_NEW_INFO:
	case DEL_BOOK_INFO:
		strFile += _T("ds97_BSC.cfg");
		break;
	default:
		return -100;
		break;
	}
		
	CFile cFile;
	int ids = cFile.Open(strFile, CFile::modeRead);


	char* cLine;

	char lpBuffer[4096];
	DWORD dwPos = cFile.GetPosition();
	CString sLine = _T("");

	// cfg의 파일은 총 6줄
	for(int nCnt=0; nCnt < 6; nCnt++)
	{
		memset(lpBuffer, '\0', 4096);
		dwPos = cFile.GetPosition();
		INT nCount = cFile.Read((char*)lpBuffer, 4096);
		if(!nCount)
		{
			break;
		}

		INT nFindPos = 0;
		while(TRUE)
		{
			if('\n' == lpBuffer[nFindPos])
				break;
			else nFindPos++;
		}
		

		cLine = new char[nFindPos];
		memset(cLine, NULL, nFindPos);
		
		for(int nCnt2 = 0; nCnt2 <= nFindPos-2; nCnt2++)
		{			
			cLine[nCnt2] = lpBuffer[nCnt2];
		}
		cLine[nFindPos-1] = '\0';
		
		switch(nCnt)
		{
		case 1:
			strcpy(m_dszIP, cLine);
			break;
		case 3:
			strcpy(m_dszPort, cLine);
			break;
		case 5:
			m_nWaitTime = atoi(cLine);
			break;
		}
		delete []cLine;

		cFile.Seek(dwPos + (DWORD)nFindPos+1, CFile::begin);	
	}
	// 이전소스
//DEL 	FILE *fp;
//DEL 
//DEL 	switch ( nMode ) 
//DEL 	{
//DEL 		case ISSUE_ID:
//DEL 			fp=_tfopen(_T("..\\cfg\\BookTrade\\ds97_ISSUEID.cfg"),_T("rb"));
//DEL 			break;
//DEL 		case EPC_CHECK_IN:
//DEL 		case LOAN_HISTORY:
//DEL 		case MAKE_NEW_INFO:
//DEL 		case DEL_BOOK_INFO:
//DEL 			fp=_tfopen(_T("..\\cfg\\BookTrade\\ds97_BSC.cfg"),_T("rb"));
//DEL 			break;
//DEL 		default:
//DEL 			return -100;
//DEL 			break;
//DEL 	}	
//DEL 	if(fp==NULL) return -1;
//DEL 
//DEL 	CHAR temp[50];
//DEL 	fscanf(fp,"%s",temp);fscanf(fp,"%s",temp);
//DEL 	strcpy(m_dszIP, temp);
//DEL 	fscanf(fp,"%s",temp);fscanf(fp,"%s",temp);
//DEL 	strcpy(m_dszPort, temp);
//DEL 	fscanf(fp,"%s",temp);fscanf(fp,"%s",temp);
//DEL 	m_nWaitTime=atoi(temp);
//DEL 	fclose(fp);		
	//--2008.09.26 UPDATE BY CJY --}}
	return 0;
}

int CTxrx::DataToSearch97(CString strHS, CString strXML,CString &strRet)
{	
	INT nNeedByte=-1;
	SOCKET SockFD;
	struct     sockaddr_in    ClientAddr; 

	char *pBodyData=NULL;
	TCHAR *pszTmp=NULL;
	int ids,BodyLen;
	int kind=0;

	//ids=InitEnvVar();
	//if ( ids < 0 ) { return -100; }

	ClientAddr.sin_family      = AF_INET;
    ClientAddr.sin_addr.s_addr = inet_addr(m_dszIP);
    ClientAddr.sin_port        = (unsigned short)htons(atoi(m_dszPort));

	ids=FDKSktTcpInit ('C',&ClientAddr,&SockFD);
	if ( ids < 0 ) { return -200; }

	ids = SendInputFrame(SockFD,strHS);
	if ( ids < 0 ) { close(SockFD);return -300; }

	ids = Worker_Rx2(SockFD,&BodyLen,&pBodyData,m_nWaitTime);
	if ( ids < 0 ) { close(SockFD);return -400; }

	// 정상체크 , 사이즈 전송여부 확인
	ids = CheckMessage1(pBodyData,&kind);
	if ( ids < 0) { free(pBodyData);close(SockFD);return -500; }		
	free(pBodyData);

	// kind 1 : 사이즈 체크 안함
	// kind 2 : 사이즈 체크 함
	if ( kind == 2 )
	{
		ids = SendInputFrame(SockFD,strXML);
		if ( ids < 0 ) { close(SockFD);return -600; }

		ids = Worker_Rx2(SockFD,&BodyLen,&pBodyData,m_nWaitTime);
		if ( ids < 0 ) { close(SockFD);return -700; }

		// 정상 체크
		ids = CheckMessage2(pBodyData);
		if ( ids < 0) { free(pBodyData);close(SockFD);return -800; }		
		free(pBodyData);
	}
	
	ids = SendInputFrame(SockFD,strXML);
	if ( ids < 0 ) { close(SockFD);return -900; }

	ids = Worker_Rx2(SockFD,&BodyLen,&pBodyData,m_nWaitTime);
	if ( ids < 0 ) { close(SockFD);return -1000; }

	nNeedByte = MultiByteToWideChar(CP_UTF8, 0, pBodyData, -1, NULL, NULL );
	if ( 0 == nNeedByte ) {
		pszTmp = NULL;		
		return -1100;
	}

	pszTmp = new TCHAR[nNeedByte];
	nNeedByte = MultiByteToWideChar(CP_UTF8, 0, pBodyData, -1, pszTmp, nNeedByte );
	if ( 0 == nNeedByte ) {
		delete [] pszTmp;
		pszTmp = NULL;		
		return -1200;
	}

	strRet.Format(_T("%s"), pszTmp);

	free(pBodyData);
	delete [] pszTmp;
	close(SockFD);

	return 0;
}


int CTxrx::SendInputFrame(SOCKET f_SockFD,CString strData)
{
	INT nNeedByte=0;
	// 현재 배열로 잡음
	char SendBuffer[30000];
	char *szValue=NULL;	
	int ids,idx;

	idx = 0;	
	cvt.UnicodeToUTF8(strData.GetBuffer(0), &szValue, &nNeedByte);

	idx+=sprintf( &SendBuffer[idx], "%08d", strlen(szValue));   // LEN	
	idx+=sprintf( &SendBuffer[idx], "%s", szValue);   // Data

	delete [] szValue;
	
	// 송신
	ids = CommWorker_Tx2( f_SockFD, idx, SendBuffer );
	if ( ids < 0 ) { return -1;}

	return 0;
}

int CTxrx::SendQuitFrame(SOCKET f_SockFD,char *cmd)
{
	char SendBuffer[1000];
	int ids,idx;

	// Quit 프레임을 보낸다.
	// 1. HEAD를 만든다.
	idx = 0;
	SendBuffer[idx] = EPC_STX;  idx += 1;   // EPC_STX
	idx+=sprintf( &SendBuffer[idx], "%5s",cmd);   //CMD
	sprintf( &SendBuffer[idx], "00001");  idx += 5;   // REC CNT
	sprintf( &SendBuffer[idx], "000013");  idx += 6;   // BODY LEN
	
	// 2. BODY를 만든다.
	sprintf( &SendBuffer[idx], "0001");  idx += 4;  // Tag cnt
	sprintf( &SendBuffer[idx], "00004");  idx += 5;  // LEN
	sprintf( &SendBuffer[idx], "QUIT");  idx += 4;   // Data  
	SendBuffer[idx] = EPC_ETX;  idx += 1;	// EPC_ETX
	ids = CommWorker_Tx( f_SockFD, idx, SendBuffer );
	if ( ids < 0 ) { return -1;}

	return 0;
}

int CTxrx::WriteOutput(int RecordCnt,int BodyLen, char *pBodyData,CString &strB_key,CString &strFlag)
{
	tRECORD_STRU_EPC   *s_pRecord;
	int ids;

	if(pBodyData==NULL)
		return 0;

	ids = AnalyzeCommandEPC(RecordCnt,pBodyData,&s_pRecord);
	if ( ids < 0 ) { return -1;}

	// 처음검색된 값을 가져온다
	if( RecordCnt > 1 )
	{
		strB_key=s_pRecord[1].pTag[3].pData;
		strFlag=s_pRecord[1].pTag[4].pData;
	}

	free(s_pRecord);

	return 0;
}

INT	CTxrx::Worker_Rx2 (SOCKET     f_SockFD,
                 INT			 *f_pBodyLen,
				 CHAR  		**f_pBodyData ,
                 INT     	  f_WaitTime)
{

	INT		ids;
#ifdef _aosf40
	INT		StartTime;
#else
	LONG    StartTime;
#endif

	/*=======================================================================*/     
     /*- 1. Frame Read시 최대 TimeOut처리를 위한 최초 시간 저장			  -*/
	/*=======================================================================*/    
     time(&StartTime);

	/*=======================================================================*/     
     /*- 2. Read Frame											  -*/
	/*=======================================================================*/
	ids = ReadFrame2(StartTime, f_WaitTime, f_SockFD, f_pBodyLen, f_pBodyData);
	if (ids < 0) { return TXRX_ERROR-5; }	
	

    return 0;
}

INT	CTxrx::ReadFrame2(INT	    	  f_StartTime,
		       INT	    	  f_TimeOut,
			   SOCKET     f_SockFD,
			   INT			 *f_pBodyLen,
               CHAR  		**f_pBodyData)
{

	CHAR      *pBodyFrame;
	CHAR      HeaderFrame[64], szTmp[1024];
	INT       WillReadCnt, ReadedCnt, BodyLen;
	INT       ids;
	
	memset(HeaderFrame, '\0', sizeof(HeaderFrame));
	memset(szTmp, '\0', sizeof(szTmp));
    
	/*=======================================================================*/     
     /*- 1. Read Frame											  -*/
	/*=======================================================================*/         

   	/*- 1.1 Read HEADER */
	WillReadCnt = 8; 
	ids = ReadBlockDataEPC(f_StartTime, f_TimeOut, f_SockFD, WillReadCnt, HeaderFrame, &ReadedCnt);
	if (ids) return TXRX_ERROR-9;
	HeaderFrame[8+1] = 0;

   	/*- 1.2 Analyze Header										  -*/

	/*- BodyLen을 읽어낸다				  -*/
	memcpy(szTmp  , &HeaderFrame[0], 8); szTmp[9]   = 0; BodyLen 	   = atoi(szTmp);
	*f_pBodyLen = BodyLen;

	/*- 1.3 Read Body											  -*/
	pBodyFrame = (CHAR *)malloc(BodyLen+10);
	if (pBodyFrame == NULL) return TXRX_ERROR-11;
			
	WillReadCnt = BodyLen;

	ids = ReadBlockDataEPC(f_StartTime, f_TimeOut, f_SockFD, WillReadCnt, &pBodyFrame[0], &ReadedCnt);
	if (ids) { free(pBodyFrame); return ids; }
		
	pBodyFrame[ReadedCnt] = 0;
	*f_pBodyData = pBodyFrame;

	return 0;

}

int CTxrx::CheckMessage1(CHAR *pBodyData, int* kind)
{
	*kind=1;
	return 0;
}

int CTxrx::CheckMessage2(CHAR *pBodyData)
{
	return 0;
}

INT CTxrx::FDKSktTcpInit( CHAR                f_ServiceType,
                       struct sockaddr_in *f_pSockAddr,
                       SOCKET             *f_pSockFD )
{

	INT     ids;
	INT	  OptVal;
    
#ifdef WINDOWSNT
	WSADATA     	WSAData;
	if ( (status = WSAStartup(MAKEWORD(2,0), &WSAData)) != 0 ) { printf( "\nfdksocket.c := socket Initialize error ..." ); return -1; }

#endif    
    switch ( f_ServiceType ) {
        case    c_SERVER :    /*- 프로세스가 SERVER로 동작하기 위한 작업 -*/
            *f_pSockFD = socket( AF_INET, SOCK_STREAM, 0 );

		  OptVal = 1;
		  /*ids  =FDKSktSetOption( *f_pSockFD,
                                    SOL_SOCKET,
                           		 SO_REUSEADDR,
                           		 (VOID*)&OptVal,
                           		 sizeof (INT) );
            if ( ids ) return ids;*/
            
            ids = bind( *f_pSockFD, (struct sockaddr *)f_pSockAddr, sizeof(struct sockaddr) );
            if ( ids  == -1 )  { printf("\nfdksocket.c:= Bind() Error[%d]\n", ids ); return ids; }

            ids = listen( *f_pSockFD, 10 );
            if ( ids  == -1 ) { printf("\nfdksocket.c := listen() Error !!!"); return ids; }

            break;

        case    c_CLIENT :    /*- 프로세스가 CLIENT로 동작하기 위한 작업 -*/
            *f_pSockFD = socket( AF_INET, SOCK_STREAM, 0 );
            ids = connect( *f_pSockFD, (struct sockaddr *)f_pSockAddr, sizeof(struct sockaddr) );
            if ( ids == -1 ) return ids;
            break;

        default : return -10000;
    }

    return 0;
}

INT  CTxrx::CommWorker_Tx2(SOCKET  	 f_SockFD, 
				 	INT		 	 f_FrameSize, 
				 	CHAR			*f_pFrame)
{

	INT	ids;
	
	ids = FDKSktTcpSendL(f_SockFD, f_pFrame, f_FrameSize, 0);
	if (ids < 0) { return TXRX_ERROR-2; }	
	
	return 0;

}

INT  CTxrx::CommWorker_Tx(SOCKET  	 f_SockFD, 
				 	INT		 	 f_FrameSize, 
				 	CHAR			*f_pFrame)
{

	INT	RetryCnt, ids;
	
	RetryCnt = 1;
	while (1) {		
		ids = FDKSktTcpSendL(f_SockFD, f_pFrame, f_FrameSize, 0);
		if (ids < 0) { return TXRX_ERROR-2; }	
	
		ids = ReadCWAckNakEPC(f_SockFD);
		if (ids < 0) { return TXRX_ERROR-3; }
		else if (ids == ACK) break;
		else if (ids == NAK && RetryCnt >= 3) { return TXRX_ERROR-4; }	
		RetryCnt++;
	}

	return 0;

}


INT  CTxrx::AnalyzeCommandEPC(
					INT            f_RecordCnt,
				   	CHAR*          f_pBodyData,
				    tRECORD_STRU_EPC** f_pRecord 
				   )
{
 
	INT		   	  i, j;
	INT           nIdx = 0;
	INT           nBodyLen;

	TCHAR		  szTmp[1024];

	tRECORD_STRU_EPC* s_pRecord = NULL;
		
	memset(szTmp, '\0', sizeof(szTmp));
	
	s_pRecord = (tRECORD_STRU_EPC*) malloc(sizeof(tRECORD_STRU_EPC) * f_RecordCnt);
	if (s_pRecord == NULL) return ANALYZECMD_ERROR - 1;

	nBodyLen = strlen(f_pBodyData);
	for(i = 0; i < f_RecordCnt; i++) 
	{
		if (nBodyLen < (nIdx + TAG_COUNT_BYTE)) return ANALYZECMD_ERROR - 2;
     	memcpy(szTmp, &f_pBodyData[nIdx], TAG_COUNT_BYTE); 
		szTmp[TAG_COUNT_BYTE] = '\0'; 
		nIdx += TAG_COUNT_BYTE;

     	s_pRecord[i].TagCnt = _ttoi(szTmp);
     	s_pRecord[i].pTag   = (tTAG_STRU_EPC*) malloc(sizeof(tTAG_STRU_EPC) * s_pRecord[i].TagCnt);
     	if (s_pRecord[i].pTag == NULL) return ANALYZECMD_ERROR - 3;
	     	
     	for (j = 0; j < s_pRecord[i].TagCnt; j++) 
		{
			if (nBodyLen < (nIdx + TAG_LEN_BYTE)) return ANALYZECMD_ERROR - 4;
     		memcpy(szTmp, &f_pBodyData[nIdx], TAG_LEN_BYTE); 
			szTmp[TAG_LEN_BYTE] = 0; 
			nIdx += TAG_LEN_BYTE;

     		s_pRecord[i].pTag[j].DataLen = _ttoi(szTmp);
     		s_pRecord[i].pTag[j].pData   = (CHAR*) malloc(s_pRecord[i].pTag[j].DataLen + 1);
			if (s_pRecord[i].pTag[j].pData == NULL) return ANALYZECMD_ERROR - 5;
			     		
     		if (nBodyLen < (nIdx + s_pRecord[i].pTag[j].DataLen)) return ANALYZECMD_ERROR - 6;
     		memcpy(s_pRecord[i].pTag[j].pData, &f_pBodyData[nIdx], s_pRecord[i].pTag[j].DataLen);
     		s_pRecord[i].pTag[j].pData[s_pRecord[i].pTag[j].DataLen] = '\0';
     		nIdx += s_pRecord[i].pTag[j].DataLen;
		}
	 }
     
     *f_pRecord = s_pRecord;
     
     return 0;
 
}

INT  CTxrx::ReadBlockDataEPC(INT        	 f_StartTime,
				    INT		 	 f_TimeOut,
                    SOCKET	 f_SockFD,
                    INT        	 f_WillReadCnt, 
                    CHAR      	*f_pReadFrame, 
                    INT       	*f_pReadedCnt)
{

	INT		ids, idx, compCount;
	fd_set	rset;
	struct	timeval tv;

	compCount = f_WillReadCnt;
	tv.tv_sec = f_TimeOut;
	tv.tv_usec = 0;		
	FD_ZERO(&rset);
	FD_SET(f_SockFD, &rset);
	
	ids = select (f_SockFD + 1, &rset, NULL, NULL, &tv);
	if (ids < 0) return TXRX_ERROR-12;
	if (ids == 0) {
		printf("receive time out !!!\n");
		return TXRX_ERROR-13;
	}
	idx = 0;
#define SOCKET_SIZE		1431
	while(1) {
/*		if (f_WillReadCnt > SOCKET_SIZE) ids = FDKSktTcpRead(f_SockFD, SOCKET_SIZE, &f_pReadFrame[idx], 0);
		else 
*/		ids = FDKSktTcpRead(f_SockFD, f_WillReadCnt, &f_pReadFrame[idx], 0);
		if (ids <= 0) { 
			if (ids == 0) printf("connection closed !!!\n");
			else printf("connection error !!![%d:%d]\n",f_WillReadCnt,ids);
			return TXRX_ERROR-14;
		}
		idx += ids;
		if (idx >= compCount) break;
		f_WillReadCnt -= ids;
	}
	*f_pReadedCnt = idx;

	return    0;


}

INT     CTxrx::FDKSktTcpSendL( SOCKET     f_SockFD,
                        CHAR          *f_pSendMsg,
                        INT            f_Len,
                        INT            f_Flags )
{

    CHAR	  ClientIP[64];
    INT     ids, PortNo;
//#ifdef WINDOWSNT
    ULONG   argp;

    argp = 0;
    ids = ioctlsocket( f_SockFD, FIONBIO, &argp );
    if ( ids == SOCKET_ERROR ) return -1;
//#else 
    //ids = fcntl( f_SockFD, F_SETFL, O_SYNC );
    //if ( ids == -1 ) return	-1;
//#endif

    FDKSktPeerInfo( f_SockFD, ClientIP, &PortNo );
    if ( PortNo == 0 ) { return -1; }

    ids = send( f_SockFD, f_pSendMsg, f_Len, f_Flags );

    return ids;

}

INT  CTxrx::ReadCWAckNakEPC(SOCKET f_SockFD)
{

     CHAR      AckNakFrame[64];
     INT       WillReadCnt, ReadedCnt;
     INT       ids, i;
#ifdef _aosf40
	 INT		StartTime;
#else
	 LONG   	StartTime;
#endif
     
     memset(AckNakFrame, '\0', sizeof(AckNakFrame));

     time(&StartTime);
     while(1) {
		WillReadCnt = 5;
		ids = ReadBlockDataEPC(StartTime, 30, f_SockFD, WillReadCnt, AckNakFrame, &ReadedCnt);
		if (ids) return ids;
		
		if (AckNakFrame[0] != EPC_STX) {
			for (i=0; i < ReadedCnt; i++) if (AckNakFrame[i] == EPC_STX) break;
			if (i >= ReadedCnt) continue;
			ReadedCnt = ReadedCnt-i;
			memcpy(&AckNakFrame[0], &AckNakFrame[i], ReadedCnt);
		}
		
		if (ReadedCnt < WillReadCnt) {
			ids = ReadBlockDataEPC(StartTime, 30, f_SockFD, WillReadCnt-ReadedCnt, &AckNakFrame[ReadedCnt], &ReadedCnt);
			if (ids) return ids;
		}          
		if (AckNakFrame[4] != EPC_ETX) continue;     	
		break;
     } /*- end while -*/

     if (AckNakFrame[3] == ACK) return ACK;
     else if (AckNakFrame[3] == NAK) return NAK;
     else return TXRX_ERROR-18;

}

INT     CTxrx::FDKSktTcpRead( SOCKET         f_SockFD,
                       INT            f_ReadCnt,
                       CHAR          *f_pReadMsg,
                       INT            f_Flags
                     )
{

    INT     ids;

    ids = recv( f_SockFD, f_pReadMsg, f_ReadCnt, f_Flags );
   
    return ids;
    

}

INT      CTxrx::FDKSktPeerInfo( SOCKET    f_SockFD,
                         CHAR     *f_pClientIP,
                         INT      *f_pPort )
{

     struct    sockaddr      s_SockAddr;
     struct    sockaddr_in  *s_pSockAddr;
     INT       ids, Len;

     Len = sizeof( s_SockAddr );
	 ids = getpeername( f_SockFD, &s_SockAddr, &Len );
     if ( ids ) {
     	sprintf(f_pClientIP, "000.000.000.000");
     	*f_pPort = 0;
     } else {    
     	s_pSockAddr = (struct sockaddr_in *)&s_SockAddr;
     	*f_pPort = ntohs( s_pSockAddr->sin_port );
     	FDKSktIP( s_pSockAddr->sin_addr, f_pClientIP );
	}
	

    return    0;

}

INT      CTxrx::FDKSktIP( struct in_addr f_InAddr , CHAR     *f_pIP )
{

     CHAR      *s_pPtr;

     s_pPtr = inet_ntoa( f_InAddr );
     strcpy( f_pIP, s_pPtr );
     
     return 0;

}