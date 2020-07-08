// MSSQL_MGR.cpp: implementation of the CMSSQL_MGR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSSQL_MGR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMSSQL_MGR::CMSSQL_MGR()
{
	// KOL.UDF.022 시큐어코딩 보완
	m_pCM = NULL;
	m_pDM = NULL;
	m_nCol = -1;
	m_nRow = -1;
}

CMSSQL_MGR::~CMSSQL_MGR()
{

}


INT CMSSQL_MGR::EXE_SELECT (CString strQuery,CESL_DataMgr *pDM, CESL_ControlMgr *pCM) 
{

    PLOGINREC   login;     
    dbinit ();
    login = dblogin ();

	DBSETLUSER (login, (LPSTR)(LPCTSTR)m_UserID );
	DBSETLPWD  (login, (LPSTR)(LPCTSTR)m_PassWord);

	dbproc = dbopen (login, (LPSTR)(LPCTSTR)m_HostIP);

	if ( dbproc == NULL) {
		AfxMessageBox ( _T("국가자료공동목록 주제명 DB접속에 실패하였습니다."));
		dbexit ();
		return -1;
	}
	BOOL bDead = dbdead ( dbproc );
	if (bDead == TRUE) {
		AfxMessageBox ( _T("국가자료공동목록 주제명 DB접속에 실패하였습니다."));
		dbexit ();
		return -1;
	}

	dbuse(dbproc, (LPSTR)(LPCTSTR)m_DbName  );
    m_SqlQuery = strQuery ; 

	dbcmd (dbproc, (LPSTR)(LPCTSTR)m_SqlQuery );
    dbsqlexec (dbproc);   
    CString sTmp, sTmp2; 

	INT i = 0;
    
	_TUCHAR Tmp[15][255];
    const TCHAR *FieldName ;

	INT nRow = 0;
	CString strColName  ;
    CString strColData ;       
	INT nCol ;
	pDM->FreeData();

    if (dbresults (dbproc) == SUCCEED )
    {
		nCol = dbnumcols(dbproc);
 
		INT j = 0;
		
		for ( i = 0 ; i < nCol ; i ++) {
			dbbind (dbproc, i+1, NTBSTRINGBIND, 0, (UCHAR*)Tmp[i]);
		}

        while (dbnextrow (dbproc) != NO_MORE_ROWS)
        {
			pDM->InsertRow(-1);
			for ( i = 0 ; i < nCol ; i ++) 
				pDM->SetCellData ( j, i, Tmp[i]);
			
			nRow++;
			j ++ ; 
        }
        	   
    }

    dbexit ();
	return 0 ;
    
	//::AfxBeginThread (ProcGridThread ,this) ;
	//return 0 ;
}


INT CMSSQL_MGR::LoadDBinfo()
{

	
	CString strFileName ;
	strFileName = _T("..\\CFG\\mssql_db_info.cfg");
    CString strLine ;
   
	CStdioFile fd;
	CString strField,strData ;
    INT nPos ;

	if ( !fd.Open(strFileName, CFile::modeRead | CFile::typeBinary) ) return -3;
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
  while(TRUE)
	{
		fd.ReadString(strLine);
		if ( strLine.IsEmpty ()) break;
		     
		nPos = strLine.Find (_T("="));
		strField = strLine.Mid(0,nPos) ;
    strField.TrimLeft ();
		strField.TrimRight ();
        
		strData = strLine.Mid (nPos+1) ;
		strData.TrimRight ();
		strData.TrimLeft ();

    if(strField == _T("IP"))
		{
			m_HostIP = strData ;
		}
		else if (strField == _T("USERID"))
		{
			m_UserID = strData ;
		}
		else if (strField == _T("PASSWORD"))
		{
			m_PassWord = strData ;
		}
		else if (strField == _T("DB_NAME"))
		{
			m_DbName = strData ;
		}

	}
   fd.Close () ;

	
	return 0 ;
}


INT CMSSQL_MGR::UpdateDBinfo () 
{

	CStdioFile fd ;
	CString strFileName ;
	strFileName = _T("..\..\cfg\\mssql_db_info.cfg");
	if(!fd.Open (strFileName,CFile::modeCreate| CFile::modeWrite | CFile::typeBinary ))
	{
		AfxMessageBox(_T("mssql_db_info.cfg.cfg 파일 열기 실패 "));
		return -1 ;
	}
    
	fd.WriteString (_T("IP = ")+ m_HostIP + _T("\n")) ;
	fd.WriteString (_T("USERID = ") + m_UserID + _T("\n")) ;
    fd.WriteString (_T("PASSWORD = ")+ m_PassWord + _T("\n") ) ;
	fd.WriteString (_T("DB_NAME = ") + m_DbName + _T("\n")) ; 

    fd.Close () ;

	return 0 ;
}

UINT ProcGridThread (LPVOID pParam)
{
    CMSSQL_MGR *pMain = (CMSSQL_MGR*)pParam ;

	PDBPROCESS DbProc = pMain->dbproc  ;
 	CESL_DataMgr *pDM = pMain->m_pDM ;
	CESL_ControlMgr *pCM = pMain->m_pCM ;
    
    CString sTmp, sTmp2; 

	INT i = 0;
    
	_TUCHAR Tmp[15][255];
    const TCHAR *FieldName ;

	INT nRow = 0;
	CString strColName  ;
    CString strColData ;       
	INT nCol ;
	pDM->FreeData();

    if (dbresults (DbProc) == SUCCEED )
    {
		nCol = dbnumcols(DbProc);
 
		INT j = 0;
		
		for ( i = 0 ; i < nCol ; i ++) {
			dbbind (DbProc, i+1, NTBSTRINGBIND, 0, (UCHAR*)Tmp[i]);
		}

        while (dbnextrow (DbProc) != NO_MORE_ROWS)
        {
			pDM->InsertRow(-1);
			for ( i = 0 ; i < nCol ; i ++) 
				pDM->SetCellData ( j, i, Tmp[i]);
			
			nRow++;
			j ++ ; 
        }
        	   
    }
	
	pCM->AllControlDisplay();

    dbexit ();
	return 0 ;
}

INT CMSSQL_MGR::EXE_INSERT ( CString strQuery ) 
{

    PLOGINREC   login;     
    dbinit ();
    login = dblogin ();

	DBSETLUSER (login, (LPSTR)(LPCTSTR)m_UserID );
	DBSETLPWD  (login, (LPSTR)(LPCTSTR)m_PassWord);

	dbproc = dbopen (login, (LPSTR)(LPCTSTR)m_HostIP);

	if ( dbproc == NULL) {
		AfxMessageBox ( _T("국가자료공동목록 주제명 DB접속에 실패하였습니다."));
		dbexit ();
		return -1;
	}
	BOOL bDead = dbdead ( dbproc );
	if (bDead == TRUE) {
		AfxMessageBox ( _T("국가자료공동목록 주제명 DB접속에 실패하였습니다."));
		dbexit ();
		return -1;
	}

	dbuse(dbproc, (LPSTR)(LPCTSTR)m_DbName  );
    m_SqlQuery = strQuery ; 

	dbcmd (dbproc, (LPSTR)(LPCTSTR)m_SqlQuery );
    dbsqlexec (dbproc);   

    dbexit ();
	return 0 ;
}

INT CMSSQL_MGR::EXE_UPDATE ( CString strQuery ) 
{

    PLOGINREC   login;     
    dbinit ();
    login = dblogin ();

	DBSETLUSER (login, (LPSTR)(LPCTSTR)m_UserID );
	DBSETLPWD  (login, (LPSTR)(LPCTSTR)m_PassWord);

	dbproc = dbopen (login, (LPSTR)(LPCTSTR)m_HostIP);

	if ( dbproc == NULL) {
		AfxMessageBox ( _T("국가자료공동목록 주제명 DB접속에 실패하였습니다."));
		dbexit ();
		return -1;
	}
	BOOL bDead = dbdead ( dbproc );
	if (bDead == TRUE) {
		AfxMessageBox ( _T("국가자료공동목록 주제명 DB접속에 실패하였습니다."));
		dbexit ();
		return -1;
	}

	dbuse(dbproc, (LPSTR)(LPCTSTR)m_DbName  );
    m_SqlQuery = strQuery ; 

	dbcmd (dbproc, (LPSTR)(LPCTSTR)m_SqlQuery );
    dbsqlexec (dbproc);   

    dbexit ();
	return 0 ;
}

INT CMSSQL_MGR::EXE_DELETE ( CString strQuery ) 
{

    PLOGINREC   login;     
    dbinit ();
    login = dblogin ();

	DBSETLUSER (login, (LPSTR)(LPCTSTR)m_UserID );
	DBSETLPWD  (login, (LPSTR)(LPCTSTR)m_PassWord);

	dbproc = dbopen (login, (LPSTR)(LPCTSTR)m_HostIP);

	if ( dbproc == NULL) {
		AfxMessageBox ( _T("국가자료공동목록 주제명 DB접속에 실패하였습니다."));
		dbexit ();
		return -1;
	}
	BOOL bDead = dbdead ( dbproc );
	if (bDead == TRUE) {
		AfxMessageBox ( _T("국가자료공동목록 주제명 DB접속에 실패하였습니다."));
		dbexit ();
		return -1;
	}

	dbuse(dbproc, (LPSTR)(LPCTSTR)m_DbName  );
    m_SqlQuery = strQuery ; 

	dbcmd (dbproc, (LPSTR)(LPCTSTR)m_SqlQuery );
    dbsqlexec (dbproc);   

    dbexit ();
	return 0 ;
}