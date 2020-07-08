// RETURN_MAIL_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "RETURN_MAIL_MAIN.h"
#include "RETURN_MAIL_DETAIL.h"
#include "RETURN_CODE_DLG.h"
//JOB.2019.0073 : 충돌 License 제거
//#include "..\\POP3_Mgr\\pop3.h"
#include "RETURN_LOGVIEW_DLG.h"
#include "../../단행/열람/장서관리/MN_LOAN_USER_MAIL_INFO/MN_LOAN_USER_MAIL_INFO_DLG.h"
#include "Afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRETURN_MAIL_MAIN dialog


CRETURN_MAIL_MAIN::CRETURN_MAIL_MAIN(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRETURN_MAIL_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGrid = NULL ;
	m_pDM   = NULL ;
	//JOB.2019.0073 : 충돌 License 제거
//	m_pReturnCodeMsg = NULL ;



}
CRETURN_MAIL_MAIN::~CRETURN_MAIL_MAIN()
{
}

BOOL CRETURN_MAIL_MAIN::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CRETURN_MAIL_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRETURN_MAIL_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRETURN_MAIL_MAIN, CDialog)
	//{{AFX_MSG_MAP(CRETURN_MAIL_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRETURN_MAIL_MAIN message handlers

BOOL CRETURN_MAIL_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_CHECK_RETURN_MAIL")) < 0 )
	{
		AfxMessageBox (_T("InitESL_Mgr() Failed")) ;
		return FALSE ;
	}

	m_pDM = FindDM(_T("DM_CHECK_RETURN_MAIL")) ;
	if( NULL == m_pDM )
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return FALSE ;
	}
	m_pDM->FreeData () ;


	m_pGrid = (CESL_Grid*)FindControl (_T("CM_CHECK_RETURN_MAIN"),_T("MainGrid")) ;
	if( NULL == m_pGrid )
	{
		AfxMessageBox (_T("FindControl() Failed")) ;
		return FALSE ;
	}

	GetReturnCodeMsg() ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRETURN_MAIL_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem (IDC_MAIN_GRID) ;
	if(pGrid != NULL)
	{
//MACRODEL		pGrid->MoveWindow (5,5,cx-10,cy-10) ;
		// 사이즈 조절
		// 재용 [2008-05-16]
		pGrid->MoveWindow(0, 0, cx, cy);	
	}
}
/*
INT CRETURN_MAIL_MAIN::GetReturnMail()
{
	
	CString strUser ;
	CString strPassWord ;
	CString strHostName ;
	
	//서버 = mail.eco.co.kr
	//계정 = webmaster@eco.co.kr
	//자명 = 관리자
	//SSWORD = 1234 
	
	
	strHostName =  _T("mail.eco.co.kr") ;
	strUser     =  _T("hjlee") ;
	strPassWord =  _T("dlgkrwnd") ;
	//strUser     =  _T("webmaster") ;
	//strPassWord =  _T("1234") ;
	

	//테스트 후 삭제 ..
	CString strFile = _T("..\\tunning\\pop3.cfg") ;
	CStdioFile file ;
	if(!file.Open (strFile,CFile::modeWrite | CFile::modeCreate))  AfxMessageBox (_T("failed"));
 
	if (!AfxSocketInit())
	{
		TRACE(_T("Failed to initialise the Winsock stack\n"));
		return FALSE;
	}
	
	//try out the POP3 Client class
	CPop3Connection p3;
	
	//you should change the parameters address, name and password 
	//to connect to your own POP3 mailbox
	//if (!p3.Connect(_T("mail.someisp.com"), _T("auser"), _T("apassword")))
	if (!p3.Connect(_T(strHostName), _T(strUser), _T(strPassWord)))
	{
		DWORD dwError = GetLastError();
		CString sError = p3.GetLastCommandResponse();

		//테스트 후 삭제 
		file.WriteString (sError) ;

		return FALSE;
	}
	
	if (!p3.Noop())
	{
		DWORD dwError = GetLastError();
		CString sError = p3.GetLastCommandResponse();

		//테스트 후 삭제 
		file.WriteString (sError) ;
	}
	
	INT nMails;
	INT nSize;
	if (!p3.Statistics(nMails, nSize))
	{
		DWORD dwError = GetLastError();
		CString sError = p3.GetLastCommandResponse();

		//테스트 후 삭제 
		file.WriteString (sError) ;
	}
	
	if (nMails)
	{
		CPop3Message message;
		DWORD dwSize;
		
		
		if (!p3.GetMessageSize(1, dwSize))
		//if (!p3.GetMessageSize(1, dwSize))
		{
			DWORD dwError = GetLastError();
			CString sError = p3.GetLastCommandResponse();

			//테스트 후 삭제 
			file.WriteString (sError) ;
		}
		if (!p3.Retrieve(1, message))
		//if (!p3.Retrieve(2, message))
		{
			DWORD dwError = GetLastError();
			CString sError = p3.GetLastCommandResponse();

			//테스트 후 삭제 
			file.WriteString (sError) ;
		}
		else
		{
			//Same the mail to a file on disk
			CFile f;
			if (f.Open(_T("..\\tunning\\message.dat"), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
			{
				
				LPCTSTR pszMessage = message.GetMessageText();
				
				///////////////////////////////////////////////////////////////////////////////
				//테스트 
				CString strHeader,strSubject,strRawBody,strBody ;

				CString strAction ;
				CString strStatus ;


				strAction = message.GetHeaderItem (_T("Return-Path")) ;

				strStatus = message.GetHeaderItem (_T("status"),1) ;
				file.WriteString (strAction + _T("\r\n")) ;
				file.WriteString (strStatus + _T("\r\n")) ;


				strHeader = message.GetHeader () ;
				strSubject = message.GetSubject () ;
				strRawBody = message.GetRawBody () ;
				strBody = message.GetBody () ;
				

				file.WriteString (strHeader + _T("\r\n")) ;
				file.WriteString (_T("/////////////////////////////////////////////////////////////////")) ;
				file.WriteString (strSubject + _T("\r\n")) ;
				file.WriteString (_T("/////////////////////////////////////////////////////////////////")) ;
				file.WriteString (strRawBody + _T("\r\n")) ;
				file.WriteString (_T("/////////////////////////////////////////////////////////////////")) ;
				file.WriteString (strBody + _T("\r\n")) ;
				///////////////////////////////////////////////////////////////////////////////

file.Write(pszMessage, _tcsclen(pszMessage));
			}
			else
				TRACE(_T("Failed to write message to file\n"));
		}
		
		for (INT i=1; i<=nMails; i++)
		{
			CString sID;
			if (!p3.GetMessageID(i, sID))
			{
				DWORD dwError = GetLastError();
				CString sError = p3.GetLastCommandResponse();

				//테스트 후 삭제 
				file.WriteString (sError) ;
			}
		}
		
		if (!p3.Delete(1))
		{
			DWORD dwError = GetLastError();
			CString sError = p3.GetLastCommandResponse();

							//테스트 후 삭제 
				file.WriteString (sError) ;

		}
		
		if (p3.Delete(nMails+1))
		{    
			DWORD dwError = GetLastError();
			CString sError = p3.GetLastCommandResponse();
			TRACE(_T("Succeeded in deleting a non-existance message ???\n"));

							//테스트 후 삭제 
				file.WriteString (sError) ;

		}
		
		if (!p3.Reset())
		{
			DWORD dwError = GetLastError();
			CString sError = p3.GetLastCommandResponse();

							//테스트 후 삭제 
				file.WriteString (sError) ;

		}
	}
	else
		TRACE(_T("No mails waiting for you at the POP3 server\n"));
	
	if (!p3.Disconnect())
	{
		DWORD dwError = GetLastError();
		CString sError = p3.GetLastCommandResponse();

						//테스트 후 삭제 
				file.WriteString (sError) ;

	}

	//테스트 후 삭제 ..
	file.Close () ;
	
	return 0 ;
}

*/

INT CRETURN_MAIL_MAIN::GetReturnMail()
{
EFS_BEGIN

	CString strUser ;
	CString strPassWord ;
	CString strHostName ;

	/*
	strHostName =  _T("mail.eco.co.kr") ;
	strUser     =  _T("hjlee") ;
	strPassWord =  _T("dlgkrwnd") ;
    */
	
	/*
	메일서버 = mail.eco.co.kr
	메일계정 = speedmax@eco.co.kr
	발송자명 = 김덕환
	패스워드 = 1111
    */

	CStdioFile file ;
	CString strFilePath = _T("..\\cfg\\MailAccount.cfg") ;
	if(!file.Open (strFilePath,CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox ( strFilePath + _T(" Open failed") ) ;
		return -1010;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString strLine ;
	CString strTag;
	CString strData ;
	INT     nPos = -1 ;
	while(file.ReadString (strLine))
	{
		if(strLine.GetLength () < 2) continue ;
		if(strLine.GetAt (0) == '#') continue ;

		nPos = strLine.Find (_T("=")) ;
		strTag = strLine.Mid (0,nPos) ;
		strTag.TrimLeft () ;
		strTag.TrimRight ();
		strData = strLine.Mid (nPos+1) ;
		strData.TrimLeft () ;
		strData.TrimRight ();

		if(strTag == _T("수신서버")) 
		{
			strHostName = strData ;
		}
		if(strTag == _T("메일계정")) 
		{
			nPos = strData.Find (_T("@"));
			if(nPos != -1)
			{
				strUser = strData.Mid (0,nPos) ;
			}
			else
			{
				strUser = strData;
			}
		}
		
		if(_T("패스워드") == strTag)		
		{
			strPassWord = strData ;
		}
	}
	file.Close () ;

	//테스트 후 삭제 ..
    /*
	CString strFile = _T("..\\tunning\\pop3.cfg") ;
	CStdioFile file ;
	if(!file.Open (strFile,CFile::modeWrite | CFile::modeCreate))  AfxMessageBox (_T("failed"));
    */

	if (!AfxSocketInit())
	{
//DEL 		TRACE(_T("Failed to initialise the Winsock stack\n"));
//DEL 		return FALSE;
		AfxMessageBox( (_T("Failed to initialise the Winsock stack")) );
		return -1020;
	}
	//JOB.2019.0073 : 충돌 License 제거
	/*
	//try out the POP3 Client class
//	CPop3Connection p3;
	
	//you should change the parameters address, name and password 
	//to connect to your own POP3 mailbox
	//if (!p3.Connect(_T("mail.someisp.com"), _T("auser"), _T("apassword")))
	if (!p3.Connect(strHostName, strUser, strPassWord))
	{
		DWORD dwError = GetLastError();
		CString sError = p3.GetLastCommandResponse();
		//return -1;
		AfxMessageBox( (_T("Failed to Connect the POP3 mailbox")) );
		return -1030;
	}
	
	if (!p3.Noop())
	{
		DWORD dwError = GetLastError();
		CString sError = p3.GetLastCommandResponse();
	}
	
	INT nMails;
	INT nSize;
	if (!p3.Statistics(nMails, nSize))
	{
		DWORD dwError = GetLastError();
		CString sError = p3.GetLastCommandResponse();
	}
	
	if (nMails)
	{
		CPop3Message message;
		DWORD dwSize;
		INT   nIdx ;
		

		for(INT i = 0 ; i < nMails ; i++)
		{
			nIdx = i+1 ;
 			if (!p3.GetMessageSize(nIdx, dwSize))
			{
				DWORD dwError = GetLastError();
				CString sError = p3.GetLastCommandResponse();
			}
			
			if (!p3.Retrieve(nIdx, message))
			{
				DWORD dwError = GetLastError();
				CString sError = p3.GetLastCommandResponse();
			}
			else
			{
				//Same the mail to a file on disk
				
				LPCTSTR pszMessage = (LPTSTR)(LPCTSTR)message.GetMessageText();
				
				///////////////////////////////////////////////////////////////////////////////
				//테스트 
				//header
				CString strReturnDate ;
				CString strReturnSubject ;
				
				//body
				CString strBodyReason ;
				CString strBodyAction ;
				CString strBodyStatus ;
				CString strBodyFrom ;
				CString strBodyTo ;
				CString strBody ;
				CString strBodySubject ;
				CString strTranscript ;
				
				strReturnDate = message.GetHeaderItem (_T("Date")) ;
				strReturnSubject = message.GetDecode (message.GetHeaderItem (_T("Subject"))) ;
				
				if(strReturnSubject.Find (_T("Returned mail:")) != -1)
				{
					
					strBodyAction = message.GetBodyAction () ;
					strBodyStatus = message.GetBodyStatus () ;
					strBodyReason = message.GetBodyReason  () ;
					strBodyFrom   =  message.GetDecode(message.GetBodyFrom ()) ;
					strBodyTo     =  message.GetDecode(message.GetBodyTo ()) ;
					strBodySubject =  message.GetDecode(message.GetBodySubect ()) ;
					strTranscript  =  message.GetDecode(message.GetTranscript ()) ;
					
					m_pDM->InsertRow (-1) ;
					INT nRowIndex = m_pDM->GetRowCount ()-1 ;
					m_pDM->SetCellData (_T("DATE"),strReturnDate,nRowIndex) ;
					m_pDM->SetCellData (_T("TITLE"),strReturnSubject,nRowIndex) ;
					m_pDM->SetCellData (_T("BODYACTION"),strBodyAction,nRowIndex) ;
					m_pDM->SetCellData (_T("BODYSTATUS"),strBodyStatus,nRowIndex) ;
					m_pDM->SetCellData (_T("BODYREASON"),strBodyReason,nRowIndex) ;
					m_pDM->SetCellData (_T("BODYFROM"),strBodyFrom,nRowIndex) ;
					m_pDM->SetCellData (_T("BODYTO"),strBodyTo,nRowIndex) ;
					m_pDM->SetCellData (_T("BODYSUBJECT"),strBodySubject,nRowIndex) ;
					m_pDM->SetCellData (_T("TRANSCRIPT"),strTranscript ,nRowIndex) ;

					CString strGroup=_T("") ;
					CString strTagCode=_T("") ;
					GetCodeGroup(strTranscript,strGroup,strTagCode) ;

					m_pDM->SetCellData (_T("GROUP"),strGroup,nRowIndex) ;
					m_pDM->SetCellData (_T("TAG_CODE"),strTagCode ,nRowIndex) ;



					
						//테스트 후 삭제 ..
    
					CString strFile = _T("..\\tunning\\pop3.cfg") ;
					CStdioFile file ;
					if(!file.Open (strFile,CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))  AfxMessageBox (_T("failed"));
		
					if( 0 == file.GetLength() )
					{			
						TCHAR cUni = 0xFEFF;
						file.Write( &cUni, sizeof(TCHAR));
					}
		
					file.WriteString (strTranscript) ;
					file.WriteString (_T("#####################################################################")) ;
					file.WriteString (message.GetBody ()) ;

					file.Close () ;

				}

// 				file.WriteString (_T("===============================================================\r\n")) ;
// 				file.WriteString (_T("Date : ")+strReturnDate+_T("\r\n")) ;
// 				
// 				strReturnSubject = message.GetDecode (strReturnSubject ) ;
// 				file.WriteString (_T("Title : ")+strReturnSubject+_T("\r\n")) ;
// 				
// 				strBodySubject = message.GetDecode (strBodySubject) ;
// 				
// 				
// 				file.WriteString (_T("Action : ")+strBodyAction+_T("\r\n")) ;
// 				file.WriteString (_T("Status : ")+strBodyStatus+_T("\r\n")) ;
// 				file.WriteString (_T("Reason : ")+strBodyReason+_T("\r\n")) ;
// 				file.WriteString (_T("From : ")+strBodyFrom+_T("\r\n")) ;
// 				file.WriteString (_T("To : ") +strBodyTo+_T("\r\n")) ;
// 				file.WriteString (_T("Subject : ")+strBodySubject+_T("\r\n")) ;
// 				
// 				file.WriteString (_T("===============================================================\r\n")) ;

			}
		}

		INT		nRowCnt		=	m_pDM->GetRowCount();
		if( 0 == nRowCnt )
		{
			//2009.03.18 DELETE BY KSJ : 메세지 중복으로 인하여 함수 호출한 부분에서 메세지 처리
//DEL 			AfxMessageBox(_T("확인 할 내용이 없습니다.")) ;
			return 1010;
		}
		else
		{
			m_pDM->SORT (_T("GROUP")) ;
		}

	}
	else
	{
		//2009.03.18 DELETE BY KSJ : 메세지 중복으로 인하여 함수 호출한 부분에서 메세지 처리
		//TRACE(_T("No mails waiting for you at the POP3 server\n"));
//DEL 		AfxMessageBox(_T("확인 할 내용이 없습니다.")) ;
		return 1010;
	}
	
	if (!p3.Disconnect())
	{
		DWORD dwError = GetLastError();
		CString sError = p3.GetLastCommandResponse();
	}

	//테스트 후 삭제 ..
	//file.Close () ;
	*///JOB.2019.0073 : 충돌 License 제거
	return 0 ;
	
EFS_END
return -1;
}

INT CRETURN_MAIL_MAIN::DisplayRetrunMail()
{

	INT ids ;

	m_pDM->FreeData () ;

	ids = GetReturnMail () ;
	if( 0 == ids )
	{
		AfxMessageBox( _T("검색되었습니다.") , MB_ICONINFORMATION );
	}
	else if( 1010 == ids )
	{
		AfxMessageBox( _T("확인 할 내용이 없습니다.") , MB_ICONINFORMATION ) ;
	}
	else
	{
		return	ids;
	}

	m_pGrid->Display (-1) ;

	return	ids ;

}

INT CRETURN_MAIL_MAIN::ShowDetailReturnMail()
{
	CRETURN_MAIL_DETAIL dlg(this);
	dlg.SetManger (_T("DM_CHECK_RETURN_MAIL"),_T("CM_CHECK_RETURN_MAIN"),_T("MainGrid")) ;
	//JOB.2019.0073 : 충돌 License 제거
	//dlg.SetReturnCodeMgr (m_pReturnCodeMsg ) ;
	dlg.DoModal () ;


	return 0 ;
}

bool CRETURN_MAIL_MAIN::GetReturnCodeMsg()
{
	//JOB.2019.0073 : 충돌 License 제거
//	m_pReturnCodeMsg = new CReturnCodeMgr ;
//	m_pReturnCodeMsg->ReadCodeCfg() ;



	CESL_DataMgr *pDM = FindDM(_T("DM_CHECK_RETURN_CODE")) ;
	if(pDM == NULL) 
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return FALSE ;
	}

	pDM->FreeData () ;
	//JOB.2019.0073 : 충돌 License 제거
	/*
	INT nRowCnt = m_pReturnCodeMsg->m_CodeList.GetCount () ;
	INT nRowIndex = 0 ;

	CRETURN_CODE *pERRORCODE = NULL ;

	POSITION pos = m_pReturnCodeMsg->m_CodeList.GetHeadPosition () ;
	while(pos)
	{
		pERRORCODE = (CRETURN_CODE*)m_pReturnCodeMsg->m_CodeList.GetNext (pos) ;
		if(pERRORCODE)
		{

			pDM->InsertRow (-1);
			pDM->SetCellData (_T("GROUP"),pERRORCODE->GROUP ,nRowIndex) ;
			pDM->SetCellData (_T("TAG"),pERRORCODE->TAG ,nRowIndex) ;
			pDM->SetCellData (_T("TAG_CODE"),pERRORCODE->TAG_CODE ,nRowIndex) ;
			pDM->SetCellData (_T("TAG_DISP"),pERRORCODE->TAG_DISP ,nRowIndex) ;
			pDM->SetCellData (_T("TAG_CONVERT"),pERRORCODE->TAG_CONVERT ,nRowIndex) ;
			CString strDispConvert = pERRORCODE->TAG_DISP + _T("\r\n\r\n") + pERRORCODE->TAG_CONVERT ;
			pDM->SetCellData (_T("설명"),strDispConvert,nRowIndex) ;

			nRowIndex++ ;
			pERRORCODE = NULL ;
		}
	}

	pDM->SORT (_T("TAG_CODE")) ;
	*/
	return TRUE ;
}

VOID CRETURN_MAIL_MAIN::ShowReturnCodeMsg()
{
	CRETURN_CODE_DLG dlg(this) ;
	//JOB.2019.0073 : 충돌 License 제거
// 	if(m_pReturnCodeMsg == NULL) return ;
// 	dlg.SetReturnCodeMgr (m_pReturnCodeMsg ) ;
	dlg.DoModal () ;

}

INT CRETURN_MAIL_MAIN::GetCodeGroup(CString INstrData ,CString &OUTstrGroup, CString &OUTstrTagCode)
{
	INstrData ;

	CESL_DataMgr *pDM = FindDM(_T("DM_CHECK_RETURN_CODE")) ;
	if(pDM == NULL) 
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return FALSE ;
	}

	CString strFind = _T("") ;
	for(INT i = 0; i < pDM->GetRowCount () ; i++)
	{
		pDM->GetCellData (_T("TAG_CODE"),i,strFind) ;

		if(INstrData.Find (strFind) != -1)
		{
			pDM->GetCellData (_T("GROUP"),i,OUTstrGroup) ;	
			OUTstrTagCode = strFind ;

			//break ;
		}
	
	}



	return 0 ;
}

INT CRETURN_MAIL_MAIN::SaveLogToTable()
{

	m_pGrid->SelectMakeList () ;

	INT nSelectCnt = m_pGrid->SelectGetCount () ;
	
	if(nSelectCnt == 0 )
	{
		AfxMessageBox (_T("선택 항목이 없습니다.")) ;
		return 0 ;
	}
	
	
	m_pGrid->SelectGetHeadPosition () ;

	INT nRowIndex ;

	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_pDM->CONNECTION_INFO ) ;

	CString strLogRecKey ;

	dm.StartFrame () ;
	for(INT i = 0 ; i < nSelectCnt ; i++)
	{
		nRowIndex = m_pGrid->SelectGetIdx () ;
        dm.InitDBFieldData () ;

		dm.MakeRecKey (strLogRecKey) ;
		
		dm.AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strLogRecKey) ;
		dm.AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),m_pDM->GetCellData (_T("DATE"),nRowIndex));
        /*
	
	
		m_pDM->SetCellData (_T("BODYSTATUS"),strBodyStatus,nRowIndex) ;
		m_pDM->SetCellData (_T("BODYREASON"),strBodyReason,nRowIndex) ;
		m_pDM->SetCellData (_T("BODYFROM"),strBodyFrom,nRowIndex) ;
		m_pDM->SetCellData (_T("BODYTO"),strBodyTo,nRowIndex) ;
		m_pDM->SetCellData (_T("BODYSUBJECT"),strBodySubject,nRowIndex) ;
		m_pDM->SetCellData (_T("TRANSCRIPT"),strTranscript ,nRowIndex) ;
        */ 
        dm.AddDBFieldData (_T("TITLE"),_T("STRING"),m_pDM->GetCellData (_T("TITLE"),nRowIndex));
		dm.AddDBFieldData (_T("BODYACTION"),_T("STRING"),m_pDM->GetCellData (_T("BODYACTION"),nRowIndex));
		dm.AddDBFieldData (_T("BODYSTATUS"),_T("STRING"),m_pDM->GetCellData (_T("BODYSTATUS"),nRowIndex));
		dm.AddDBFieldData (_T("BODYREASON"),_T("STRING"),m_pDM->GetCellData (_T("BODYREASON"),nRowIndex));
		dm.AddDBFieldData (_T("BODYFROM"),_T("STRING"),m_pDM->GetCellData (_T("BODYFROM"),nRowIndex));
		dm.AddDBFieldData (_T("BODYTO"),_T("STRING"),m_pDM->GetCellData (_T("BODYTO"),nRowIndex));
		dm.AddDBFieldData (_T("BODYSUBJECT"),_T("STRING"),m_pDM->GetCellData (_T("BODYSUBJECT"),nRowIndex));
		dm.AddDBFieldData (_T("TRANSCRIPT"),_T("STRING"),m_pDM->GetCellData (_T("TRANSCRIPT"),nRowIndex));
		dm.AddDBFieldData (_T("TAG_GROUP"),_T("STRING"),m_pDM->GetCellData (_T("GROUP"),nRowIndex));
		dm.AddDBFieldData (_T("TAG_CODE"),_T("STRING"),m_pDM->GetCellData (_T("TAG_CODE"),nRowIndex));
		
		dm.MakeInsertFrame (_T("RETURN_MAIL_LOG"));

		m_pGrid->SelectGetNext () ;
	}

	dm.SendFrame () ;
	dm.EndFrame () ;

	AfxMessageBox (_T("로그 저장이 완료 되었습니다.")) ;

	return 0 ;
}

VOID CRETURN_MAIL_MAIN::ShowLogTable()
{
	CRETURN_LOGVIEW_DLG dlg(this);
	dlg.DoModal();
}
INT CRETURN_MAIL_MAIN::EMailEdit()
{

	CMN_LOAN_USER_MAIL_INFO_DLG Dlg(this,_T("BODYTO"));

	Dlg.SetDlgType( __ECO_MODIFY_TOOLBAR_MODE , _T("CM_CHECK_RETURN_MAIN") , _T("MainGrid") , _T("DM_CHECK_RETURN_MAIL") );

	Dlg.DoModal();

	return 0;
}

HBRUSH CRETURN_MAIL_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
