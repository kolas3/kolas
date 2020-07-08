// CallNoPrnMgr.cpp : implementation file
//

#include "stdafx.h"
#include "efs.h"
#include "CallNoPrnMgr.h"
#include "lprn_api.h"
#include "CharConvert.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallNoPrnMgr

CCallNoPrnMgr::CCallNoPrnMgr()
{
	Init();
}

CCallNoPrnMgr::~CCallNoPrnMgr()
{
}


BEGIN_MESSAGE_MAP(CCallNoPrnMgr, CWnd)
	//{{AFX_MSG_MAP(CCallNoPrnMgr)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCallNoPrnMgr message handlers


// **한글, 한자, 영문을 체크하여 Byte수에 맞게 문자열을 자른다.
CString CCallNoPrnMgr::getLimitWord(TCHAR* szToken, INT nCutCnt, BOOL bIsAddSpace)
{
	/// 한글 계산을 위하여 ansi로 변환한 후에
	/// 길이 계산을 하자
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( szToken, &szTmp, &len );
	if( 0 < len )
	{
//		strcpy((char*)szStr,szTmp);
//		delete [] szTmp;
	}
	if(szTmp == NULL)
	{
		CString sTemp = _T("");
		for(INT i = 0; i < nCutCnt; i++)
		{
			sTemp += _T(" ");
		}
		return sTemp;
	}

	BOOL bIsCut = FALSE;

	INT nTokenLen = 0 ;
	INT nCompIndexCnt = 0 ;

	CString sReturnWord = _T("");
	CString sPeriods = _T("…");
	CHAR szPeriods[5];
	sprintf( szPeriods, "%s", "…" );

	CHAR chComp = '\0' ;
	CHAR chExtra = '\0' ;

	CHAR szTmpWord[3] ; 
	CHAR szReturnWord[100] = "" ; 	

	nTokenLen = strlen( szTmp );

	if ( nTokenLen > nCutCnt )
	{
		bIsCut = TRUE;
		nCutCnt -= 3;
	}

	for ( INT i = 0 ; i < nCutCnt ; i++ )
	{
		if ( i > (nTokenLen-1) )
		{
			if ( bIsAddSpace )
			{
				chComp = ' ';
				sprintf ( szTmpWord, "%c", chComp );
				strcat ( szReturnWord, szTmpWord );
				continue;
			}
			else
				break;
		}
		else
		{
			chComp = szTmp[i];
			chExtra = szTmp[i + 1];
		}

		/// 한글과 한자일 때...
		if (chComp & 0x80)
		{
			if ( (i + 2) > nCutCnt )
			{				
				sprintf( szPeriods, "%s", "... " );
				break;
			}

			sprintf ( szTmpWord, "%c%c", chComp, chExtra ) ;			
			strcat ( szReturnWord, szTmpWord ) ;
			i++;
		}
		// 한글, 한자 제외일때
		else
		{
			sprintf ( szTmpWord, "%c", chComp ) ;
			strcat ( szReturnWord, szTmpWord ) ;
		}
	}

	if ( bIsCut )
	{
		sprintf( szTmpWord, "%s", szPeriods );
		strcat( szReturnWord, szTmpWord );
	}
	
	if( 0 < len )
	{
		delete [] szTmp;
	}

	/// 계산이 끝난 글자를
	/// 유니코드로 변환해야 한다
	TCHAR* pszDest = NULL;
	cv.MultiByteToUnicode( szReturnWord, &pszDest, &len );	
	sReturnWord.Format( _T("%s"), pszDest );

	if( 0 < len )
	{
		delete [] pszDest;
	}

	return sReturnWord;
}


INT CCallNoPrnMgr::Add(CString strTitle, CString strAuthor, CString strCallNo, CString strShelfLocCode, CString strRegNo)
{
	CString strTotal;

	strTitle.TrimLeft(); strTitle.TrimRight();
	strAuthor.TrimLeft(); strAuthor.TrimRight();
	strCallNo.TrimLeft(); strCallNo.TrimRight();
	strShelfLocCode.TrimLeft(); strShelfLocCode.TrimRight();
	strRegNo.TrimLeft(); strRegNo.TrimRight();

	strTitle = getLimitWord(strTitle.GetBuffer(0), 30, FALSE);
	strAuthor = getLimitWord(strAuthor.GetBuffer(0), 30, FALSE);
	strShelfLocCode = getLimitWord(strShelfLocCode.GetBuffer(0), 30, FALSE);

	strTotal.Format(
		_T("<!FC>------------------------------------------<!LC><!FP>")
		_T("<!FN>서    명 : %s<!LN><!FP>")
		_T("<!FN>청구기호 : %s<!LN><!FP>")
		_T("<!FN>등록번호 : %s<!LN><!FP>")
		_T("<!FN>저    자 : %s<!LN><!FP>")
		_T("<!FN>자 료 실 : %s<!LN><!FP>")
		, strTitle, strCallNo, strRegNo, strAuthor, strShelfLocCode);

	m_strPrintData += strTotal;

	return 0;
}
INT CCallNoPrnMgr::AddSe(CString strTitle, CString strAuthor, CString strCallNo, CString strShelfLocCode)
{
	CString strTotal;

	strTitle.TrimLeft(); strTitle.TrimRight();
	strAuthor.TrimLeft(); strAuthor.TrimRight();
	strCallNo.TrimLeft(); strCallNo.TrimRight();
	strShelfLocCode.TrimLeft(); strShelfLocCode.TrimRight();

	strTitle = getLimitWord(strTitle.GetBuffer(0), 30, FALSE);
	strAuthor = getLimitWord(strAuthor.GetBuffer(0), 30, FALSE);
	strShelfLocCode = getLimitWord(strShelfLocCode.GetBuffer(0), 30, FALSE);

	strTotal.Format(
		_T("<!FC>------------------------------------------<!LC><!FP>")
		_T("<!FN>서    명 : %s<!LN><!FP>")
		_T("<!FN>청구기호 : %s<!LN><!FP>")
		_T("<!FN>저    자 : %s<!LN><!FP>")
		_T("<!FN>자 료 실 : %s<!LN><!FP>")
		, strTitle, strCallNo, strAuthor, strShelfLocCode);

	m_strPrintData += strTotal;

	return 0;
}
INT CCallNoPrnMgr::GetReady()
{
	INT ids;	

	// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
//*/ ADD ---------------------------------------------------------------------------
	CString strPrinter;
	GetReceiptPrinterName(strPrinter);
	if( !strPrinter.IsEmpty() )
	{
		HANDLE hPrinter;
		ids = ::OpenPrinter( strPrinter.GetBuffer(0), &hPrinter, NULL );
		::ClosePrinter(hPrinter);

		if( ids != 0 )	
			return 0;
	}
//*/ END ---------------------------------------------------------------------------

	ids = LP_StartUp();		
	LP_CleanUp();

	return ids;
}
INT CCallNoPrnMgr::Init()
{
	m_strPrintData = _T("<!FC><!FF>[자료위치안내]<!LF><!LC><!FP><!FP>");
	return 0;
}
INT CCallNoPrnMgr::Print()
{
	return printReceipt(m_strPrintData);	
}



INT CCallNoPrnMgr::printReceipt(CString sRctStr)
{
	INT ids = 0;
	INT nStatus = 0;

	TCHAR ErrMsg[1024];
	//UCHAR szStr[2048];
	// UCHAR szStr[2048]; 2048이상이 들어오면 에러라 다음과 같이 처리.
	UCHAR *szStr = new UCHAR[100000];
	if ( szStr == NULL ) return -100;

	//if ( m_strMode == '2' ) return 0;
		
	// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
/*// BEFORE ------------------------------------------------------------------------
	ids = LP_StartUp();	
*/// AFTER -------------------------------------------------------------------------
	DWORD dwWritten;
	HANDLE hPrinter;

	BOOL bIsWTPPrinter = TRUE;	
	CHAR* szTmp2 = NULL;
	INT len2 = 0;
		
	CString strTemp;	
	CString strPrinter;

	GetReceiptPrinterName(strPrinter);
	if( !strPrinter.IsEmpty() )
	{
		bIsWTPPrinter = FALSE;
		ids = ::OpenPrinter( strPrinter.GetBuffer(0), &hPrinter, NULL );
	}
	else
	{
		ids = LP_StartUp();
	}
//*/ END ---------------------------------------------------------------------------

	if( TRUE == bIsWTPPrinter )
	{
		nStatus = LP_GetStatus( );
		MakeRPRNLog( _T("[LOG]LP_GetStatus()"), _T("상태정보 가져오기"), __WFILE__, __LINE__, nStatus );
		if ( 0 > nStatus )
		{
			LP_GetErrorMsg( ErrMsg );
			MakeRPRNLog( _T("[ERROR]LP_GetStatus()"), ErrMsg, __WFILE__, __LINE__, ids );
			AfxMessageBox(_T("출력기의 전원이 꺼져있거나 연결이 되어 있지 않습니다.\n출력기를 점검한뒤 확인버튼을 누르세요.\n출력되지 않은 영수증은 이력에서 재출력이 가능합니다"), MB_OK);
			delete [] szStr;		
			return 100;
		}
		else if ( 0 != nStatus )
		{
			ids = LP_StartUp();
			if( 0 != ids )
			{
				LP_GetErrorMsg( ErrMsg );
				MakeRPRNLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __WFILE__, __LINE__, ids );
				AfxMessageBox(_T("발견된 출력기가 없습니다."));
				delete [] szStr;
				return 200;
			}
			else
				MakeRPRNLog( _T("[LOG]LP_StartUp()"), _T("영수증 출력 준비"), __WFILE__, __LINE__, ids );
		}
	
		ids = LP_StartUp();
		if( 0 != ids )
		{
			LP_GetErrorMsg( ErrMsg );
			MakeRPRNLog( _T("[ERROR]LP_StartUp()"), ErrMsg, __WFILE__, __LINE__, ids );
			AfxMessageBox(_T("발견된 출력기가 없습니다."));
			delete [] szStr;
			return 200;
		}
		else
			MakeRPRNLog( _T("[LOG]LP_StartUp()"), _T("영수증 출력 준비"), __WFILE__, __LINE__, ids );
	}
	// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
//*/ ADD ---------------------------------------------------------------------------
	else
	{
		DWORD dwStatus = 0;
		BOOL bError = IsPrinterError(hPrinter, dwStatus);
		MakeRPRNLog( _T("[LOG]LP_GetStatus()"), _T("상태정보 가져오기"), __WFILE__, __LINE__,  dwStatus);
		if( FALSE != bError )	//Error
		{
			MakeRPRNLog( _T("[ERROR]GetPrinter()"), _T("출력기를 사용할 수 없습니다."), __WFILE__, __LINE__, bError );
			if(PRINTER_STATUS_NOT_AVAILABLE == dwStatus)
			{
				AfxMessageBox(_T("출력기의 전원이 꺼져있거나 연결이 되어 있지 않습니다.\n출력기를 점검한뒤 확인버튼을 누르세요.\n출력되지 않은 영수증은 이력에서 재출력이 가능합니다"), MB_OK);
			}
			else
			{
 				AfxMessageBox(_T("출력기 상태에 문제가 있어서 출력할 수 없습니다.\n출력기를 점검한뒤 확인버튼을 누르세요.\n출력되지 않은 영수증은 이력에서 재출력이 가능합니다"), MB_OK);
			}
			::ClosePrinter(hPrinter);
 			delete [] szStr;
 			return 100;
		}
		
		//출력타이틀 찾기 - [헤더내용]
		INT nStart = sRctStr.Find('[', 0) + 1;
		INT nEnd = sRctStr.Find(']', nStart);
		strTemp = sRctStr.Mid(nStart, nEnd-nStart);

		DOC_INFO_1 di = {0x00};
		di.pDocName = strTemp.GetBuffer(0);
		di.pOutputFile = NULL;
		di.pDatatype = _T("RAW");

		::StartDocPrinter(hPrinter, 1, (LPBYTE)&di);
		::StartPagePrinter(hPrinter);

		MakeRPRNLog( _T("[LOG]LP_StartUp()"), _T("영수증 출력 준비"), __WFILE__, __LINE__, ids );
	}
//*/ END ---------------------------------------------------------------------------

	/// TCHAR 형식을 UCHAR 형식으로 변환해준다.
	//++2008.05.21 UPDATE BY PWR {{++
	// 출력할 데이터가 6000정도가 넘어가면 영수증출력이 이루어지지 않는다.
	// 5000정도를 잘라서 루프를 돌면서 이어출력하도록 수정.	
	INT nOffset = 0;
	INT nAddIndex = 0;
	INT nCutStep = 0;
	INT nLength = 0;
	INT	i = 0;
	INT nIndex = 0;
		
//	nLength = sRctStr.GetLength( );

	//++2007.11.1 UNICODE WORK BY PDW {{++
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( sRctStr.GetBuffer(0), &szTmp, &len );
	if( 0 < len )
	{
//		strcpy((char*)szStr,szTmp);
//		delete [] szTmp;
	}

	szStr[len]= '\0';
	nLength = len;
//	ids = LP_PrintData( szStr );
	//--2007.11.1 UNICODE WORK BY PDW --}}

	while( i < nLength )
	{
		nAddIndex = 0;
		nCutStep = 0;
		nIndex = 0;		
		// 잘라진 부분부터 다시 시작
		for( i = nOffset ; i < nLength ; i++ )
		{
			szStr[nIndex] = (UCHAR)szTmp[i];
			// 데이터가 5000이 되면
			if ( (i > 0) && (i % 2500 == 0) )
			{				
				// 책의 반납형태까지 출력하고 잘른다.
				// 자르는부분 = 잘른부분(5000)부터 반납형태까지 찾은부분에서 자르기전부분
				/*
				nAddIndex = (sRctStr.Find( _T("<!LN><!FP><!FN>  "), i) + 15 )-i;				
				if ( sRctStr.Mid(i+nAddIndex, 1 ) == _T(" ") )
				{
					// 자르는 부분이 등록번호에서 끝나게 되면 한줄더 내려간다.
					nAddIndex = (sRctStr.Find( _T("<!LN><!FP><!FN>  "), i+nAddIndex) + 15 )-i;					
				}
				*/			
				
				CHAR* szFindDest = strstr(szTmp+i, "<!LN><!FP><!FN>  ") + 15;				
				CHAR* szFindBuf = szTmp + i; // 테스트용

				nAddIndex = szFindDest - (szTmp+i) + 1;
				
				if( 0 < nAddIndex &&  ' ' == szTmp[i+nAddIndex] )
				{
					szFindDest = strstr(szTmp+i+nAddIndex, "<!LN><!FP><!FN>  ") + 15;
					nAddIndex = szFindDest - (szTmp+i) + 1;
				}
	
			}
			
			if ( nAddIndex > 0 ) nCutStep ++;
			nOffset++;

			if ( nAddIndex > 0 && nCutStep == nAddIndex ) 
			{
				break;
			}
			nIndex++;
		}
		
		szStr[nIndex]= '\0';
		
		// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
/*// BEFORE ------------------------------------------------------------------------
		ids = LP_PrintData( szStr );	
*/// AFTER -------------------------------------------------------------------------
		if( TRUE == bIsWTPPrinter )
		{
			ids = LP_PrintData( szStr );		
		}
		else
		{				
			TCHAR* pszDest = NULL;
			cv.MultiByteToUnicode( (char*)szStr, &pszDest, &len2 );	
			strTemp.Format( _T("%s"), pszDest );
			if( 0 < len2 )
			{
				delete [] pszDest;
				pszDest = NULL;
			}
			
			RemoveTag(strTemp);

			cv.UnicodeToMultiByte( strTemp.GetBuffer(0), &szTmp2, &len2 );
			::WritePrinter(hPrinter, szTmp2, len2, &dwWritten);
			if( 0 < len2 )
			{
				delete [] szTmp2;
				szTmp2 = NULL;
			}
		}
//*/ END ---------------------------------------------------------------------------
	}

	if( 0 < len )
	{
		delete [] szTmp;
		delete [] szStr;
	}
		
	// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
/*// BEFORE ------------------------------------------------------------------------
	LP_CleanUp();
*/// AFTER -------------------------------------------------------------------------
	if( TRUE == bIsWTPPrinter )
	{
		LP_CleanUp();
	}
	else
	{
		//페이지 끝 처리
		strTemp.Format(_T("\r\n\r\n\r\n\r\n\x1b\x69")); // \r\n(위치보정), \x1b\x69(print_cut)
		cv.UnicodeToMultiByte( strTemp.GetBuffer(0), &szTmp2, &len2 );
		::WritePrinter(hPrinter, szTmp2, len2, &dwWritten);
		if( 0 < len2 )
		{
			delete [] szTmp2;
			szTmp2 = NULL;
		}
		
		::EndPagePrinter(hPrinter);
		::EndDocPrinter(hPrinter);
		::ClosePrinter(hPrinter);
	}	
//*/ END ---------------------------------------------------------------------------	

	return 0;
}

// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
//*/ ADD ---------------------------------------------------------------------------
//설정파일의 프린터명 얻어오기
VOID CCallNoPrnMgr::GetReceiptPrinterName(CString &strValue)
{
	// 인증코드 
	CStdioFile file ;		
	CString strTemp;		
	CString strLine;
	INT		ids;
	
	strTemp = _T("..\\cfg\\ReceiptPrintName.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary) ) 
	{	
		return;
	}

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	ids = file.ReadString (strLine);	
	strValue = strLine;
	file.Close();
}

//프린터 태그 제거
VOID CCallNoPrnMgr::RemoveTag(CString &strValue)
{
	strValue.Replace(_T("<!FH>"),_T("\x1b\x21\x51\x1b\x45\r"));		//header/footer 시작- \x1b\x21\x51(enlarge), \x1b\x45\r(bold)
	strValue.Replace(_T("<!FF>"),_T("\x1b\x21\x38"));				//\x1b\x21x38(bold_width_height) 
	strValue.Replace(_T("<!LF>"),_T("\x1b\x21\x02"));				//\x1b\x21\x02(fontstyle_normal)
	strValue.Replace(_T("<!FC>"),_T("\x1b\x61\x31"));				//\x1b\x61\x31(align_center)
	strValue.Replace(_T("<!LC>"),_T("\x1b\x61\x30"));				//\x1b\x61\x30(align_left)
	strValue.Replace(_T("<!FN>"),_T("\x1b\x61\x30\x1b\x21\x02"));	//body 시작 - \x1b\x61\x30(align_left), \x1b\x21\x02(fontstyle_normal)
	strValue.Replace(_T("<!LN>"),_T(""));
	strValue.Replace(_T("<!FR>"),_T("\x1b\x61\x32"));				//\x1b\x61\x32(align_right)	
	strValue.Replace(_T("<!LR>"),_T("\x1b\x61\x30"));				//\x1b\x61\x30(align_left)
	strValue.Replace(_T("<!LH>"),_T("\x1b\x21\x02"));				//\x1b\x21\x02(fontstyle_normal)
	strValue.Replace(_T("<!FP>"),_T("\r\n"));
}

BOOL CCallNoPrnMgr::GetJobs(HANDLE hPrinter,        /* Handle to the printer. */ 
                JOB_INFO_2 **ppJobInfo, /* Pointer to be filled.  */ 
                INT *pcJobs,            /* Count of jobs filled.  */ 
                DWORD *pStatus)         /* Print Queue status.    */ 

{

	DWORD               cByteNeeded,
						nReturned,
						cByteUsed;
	JOB_INFO_2          *pJobStorage = NULL;
	PRINTER_INFO_2       *pPrinterInfo = NULL;

	/* Get the buffer size needed. */ 
   if (!GetPrinter(hPrinter, 2, NULL, 0, &cByteNeeded))
   {
       if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
           return FALSE;
   }

   pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);
   if (!(pPrinterInfo))
       /* Failure to allocate memory. */ 
       return FALSE;

   /* Get the printer information. */ 
   if (!GetPrinter(hPrinter,
           2,
           (unsigned char*)pPrinterInfo,
           cByteNeeded,
           &cByteUsed))
   {
       /* Failure to access the printer. */ 
       free(pPrinterInfo);
       pPrinterInfo = NULL;
       return FALSE;
   }

   /* Get job storage space. */ 
   if (!EnumJobs(hPrinter,
           0,
           pPrinterInfo->cJobs,
           2,
           NULL,
           0,
           (LPDWORD)&cByteNeeded,
           (LPDWORD)&nReturned))
   {
       if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
       {
           free(pPrinterInfo);
           pPrinterInfo = NULL;
           return FALSE;
       }
   }

   pJobStorage = (JOB_INFO_2 *)malloc(cByteNeeded);
   if (!pJobStorage)
   {
       /* Failure to allocate Job storage space. */ 
       free(pPrinterInfo);
       pPrinterInfo = NULL;
       return FALSE;
   }

   ZeroMemory(pJobStorage, cByteNeeded);

   /* Get the list of jobs. */ 
   if (!EnumJobs(hPrinter,
           0,
           pPrinterInfo->cJobs,
           2,
           (LPBYTE)pJobStorage,
           cByteNeeded,
           (LPDWORD)&cByteUsed,
           (LPDWORD)&nReturned))
   {
       free(pPrinterInfo);
       free(pJobStorage);
       pJobStorage = NULL;
       pPrinterInfo = NULL;
       return FALSE;
   }

   /*
    *  Return the information.
    */ 
   *pcJobs = nReturned;
   *pStatus = pPrinterInfo->Status;
   *ppJobInfo = pJobStorage;
   free(pPrinterInfo);

   return TRUE;

}

BOOL CCallNoPrnMgr::IsPrinterError(HANDLE hPrinter, DWORD &dwStatus)
{
   JOB_INFO_2  *pJobs;
   INT         cJobs,
               i;
   DWORD       dwPrinterStatus;

   /*
    *  Get the state information for the Printer Queue and
    *  the jobs in the Printer Queue.
    */ 
   if (!GetJobs(hPrinter, &pJobs, &cJobs, &dwPrinterStatus))
		return FALSE;

   dwStatus = dwPrinterStatus;

   /*
    *  If the Printer reports an error, believe it.
    */ 
   if (dwPrinterStatus &
       (PRINTER_STATUS_ERROR |
       PRINTER_STATUS_PAPER_JAM |
       PRINTER_STATUS_PAPER_OUT |
       PRINTER_STATUS_PAPER_PROBLEM |
       PRINTER_STATUS_OUTPUT_BIN_FULL |
       PRINTER_STATUS_NOT_AVAILABLE |
       PRINTER_STATUS_NO_TONER |
       PRINTER_STATUS_OUT_OF_MEMORY |
       PRINTER_STATUS_OFFLINE |
       PRINTER_STATUS_DOOR_OPEN))
   {
       free( pJobs );
       return TRUE;
   }

   /*
    *  Find the Job in the Queue that is printing.
    */ 
   for (i=0; i < cJobs; i++)
   {
       if (pJobs[i].Status & JOB_STATUS_PRINTING)
       {
           /*
            *  If the job is in an error state,
            *  report an error for the printer.
            *  Code could be inserted here to
            *  attempt an interpretation of the
            *  pStatus member as well.
            */ 
           if (pJobs[i].Status &
               (JOB_STATUS_ERROR |
               JOB_STATUS_OFFLINE |
               JOB_STATUS_PAPEROUT |
               JOB_STATUS_BLOCKED_DEVQ))
           {
               free( pJobs );
               return TRUE;
           }
       }
   }

   /*
    *  No error condition.
    */ 
   free( pJobs );
   return FALSE;

}
//*/ END ---------------------------------------------------------------------------

INT CCallNoPrnMgr::MakeRPRNLog( CString funcInfo, CString workInfo, CString fileName, INT line, INT ids, CString expInfo )
{		
	CTime time = CTime::GetCurrentTime();
	CString output;
	
	output.Format( 
		_T("[ %04d-%02d-%02d . %02d:%02d:%02d ]\r\n")
		_T("RESULT\t : %d\r\nFUNC\t : %s\r\nWORK\t : %s\r\nFILE\t : %s - LINE : %d\r\n\r\n"),
		time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(),
		ids, funcInfo, workInfo, fileName, line );

	CString logFileName;
	logFileName.Format( _T("..\\tunning\\RPRN_LOG-%04d-%02d-%02d.txt"), time.GetYear(), time.GetMonth(), time.GetDay() );

	FILE *fp;
	fp = _tfopen( logFileName.GetBuffer( 0 ), _T("a+b") );
	if( !fp )
	{
		AfxMessageBox( _T("Receipt Printer Log 기록에 실패하였습니다.") );
		return -1;
	}
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
	// KOLAS 2 UP UNICODE 수정작업
	// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	// ------------------------------------------------------------------------------
		
	_ftprintf( fp, _T("%s"), output.GetBuffer( 0 )  );
	fclose( fp );
	
	return 0;	
}