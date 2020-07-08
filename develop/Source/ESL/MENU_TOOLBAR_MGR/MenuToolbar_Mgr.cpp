// MenuToolbar_Mgr.cpp: implementation of the CMenuToolbar_Mgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include _T("kolas2up_main.h")
#include "MenuToolbar_Mgr.h"
#include "PROGRESS_DLG.h"

#define UPLOAD 0 ;
#define DOWNLOAD 1 ;



#define MENU_FILE_PATH _T("..\\cfg\\menutoolbar\\menu.cfg")
#define TOOLBAR_FILE_PATH _T("..\\cfg\\menutoolbar\\toolbar.cfg")
#define TOOLBARGROUP_FILE_PATH _T("..\\cfg\\menutoolbar\\toolbargroup.cfg")
#define VESION_FILE_PATH _T("..\\cfg\\menutoolbar\\version.cfg")

#define TEMP_MENU_FILE    _T("..\\cfg\\temp\\menu.cfg")
#define TEMP_TOOLBAR_FILE _T("..\\cfg\\temp\\toolbar.cfg")
#define TEMP_TOOLBARGROUP_FILE  _T("..\\cfg\\temp\\toolbargroup.cfg")


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuToolbar_Mgr::CMenuToolbar_Mgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	 m_str_USER_NAME    = _T("") ;
	 m_str_FLAG         = _T("") ;
	 m_str_SPF_USE_FLAG = _T("") ;
    
	 m_bMenu = FALSE;
	 m_bToolbar = FALSE;
	 m_bToolbarGroup = FALSE;

	
	 m_DM.SetCONNECTION_INFO (m_strConnectionInfo) ;

	 m_nMenuVersion = 0 ;
     m_nToolbarVersion = 0 ;
	 m_nToolbarGroupVersion =0 ;


}

CMenuToolbar_Mgr::~CMenuToolbar_Mgr()
{

}


INT CMenuToolbar_Mgr::Check_File_Vesion() 
{
	return 0 ;
}

//2.file create 
INT CMenuToolbar_Mgr::Make_File() 
{

	INT ids ;

	DWORD START,END ;
	START = GetTickCount() ;

	if(m_bMenu == FALSE && m_bToolbar == FALSE && m_bToolbarGroup == FALSE)
	{
		return 0 ;
	}

	if(m_bMenu == TRUE)
	{
		Make_Menu_File() ;
	}
	
	if(m_bToolbar == TRUE)
	{
		Make_Toolbar_File() ;
	}
	
	if(m_bToolbarGroup == TRUE)
	{
		Make_ToolbarGroup_File() ;
	}

	
	ids  = Make_Version_File() ;

	
	END = GetTickCount() ;
	TRACE(_T("DWONLOAD TIME = %d ms "),(END-START)) ;



	return 0 ;
}

INT CMenuToolbar_Mgr::Make_Menu_File() 
{
	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_strConnectionInfo);
	
	// 접속정보를 설정하고
	CString strQuery ;
	// 0         1         2    3          4      5       6       7        8                       
	strQuery.Format(_T("SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, TITLE_DESC FROM ESL_MENU_TBL WHERE FLAG = '%s' AND SPF_USE_FLAG ='%s' AND USER_ID ='%s' ORDER BY IDX"),m_str_FLAG ,m_str_SPF_USE_FLAG,m_str_USER_NAME);
	dm.RestructDataManager(strQuery);
	
	CString strLine =_T("");
	CStdioFile file ;
	
	CString strFileName =_T("");
    if(dm.GetRowCount () == 0 )
	{		
		return  0;
	}
	
	
	
	

	for(INT j = 0 ; j < dm.GetRowCount () ; j++)
	{
		
		if(file.Open (TEMP_MENU_FILE,CFile::modeCreate|CFile::modeWrite | CFile::typeBinary))
		{  
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
			
			file.WriteString (_T("//menu.cfg===================================\r\n\r\n\r\n")) ;
			for(INT i = 0 ; i < dm.GetRowCount () ; i++)
			{
				
				file.WriteString (_T("[MENU]\r\n")) ;
				
				dm.GetCellData (i,0,strLine) ;
				file.WriteString (_T("ALIAS = ")+strLine +_T("\r\n")) ;
				dm.GetCellData (i,1,strLine) ;
				file.WriteString (_T("GROUP_ALIAS = ")+strLine+_T("\r\n")) ;
				dm.GetCellData (i,2,strLine) ;
				file.WriteString (_T("IDX = ")+strLine+_T("\r\n")) ;		
				dm.GetCellData (i,3,strLine) ;
				file.WriteString (_T("CAPTION = ")+strLine+_T("\r\n")) ;		
				dm.GetCellData (i,4,strLine) ;
				file.WriteString (_T("VISIBLE = ")+strLine+_T("\r\n")) ;	
				dm.GetCellData (i,5,strLine) ;
				file.WriteString (_T("STYLE = ")+strLine+_T("\r\n")) ;
				dm.GetCellData (i,6,strLine) ;
				file.WriteString (_T("ACTION = ")+strLine+_T("\r\n")) ;
				dm.GetCellData (i,7,strLine) ;
				file.WriteString (_T("TARGET = ")+strLine+_T("\r\n")) ;
				dm.GetCellData (i,8,strLine) ;
				file.WriteString (_T("TITLE_DESC = ")+strLine+_T("\r\n")) ;
				
				file.WriteString (_T("\r\n")) ;
				
			
			}
		}
		else
		{
			return -1;
		}
		
		file.Close () ;
	}

	return 0 ;
}

INT CMenuToolbar_Mgr::Make_Toolbar_File() 
{
	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_strConnectionInfo );
	
	// 접속정보를 설정하고
	CString strQuery ;
    
	//                           0           1          2      3       4      5    6        7              8           9              10        11            12      13
	strQuery.Format(_T("SELECT ALIAS, TOOLGROUP_ALIAS, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED , TOOLTIP, MENU_ALIAS, ACCECLERATOR, PK FROM ESL_TOOLBAR_TBL WHERE FLAG = '%s' AND SPF_USE_FLAG ='%s' AND USER_ID ='%s' ORDER BY IDX"),m_str_FLAG,m_str_SPF_USE_FLAG,m_str_USER_NAME);
	dm.RestructDataManager(strQuery);
	
	CString strLine =_T("");
	CStdioFile file ;
	
	

	if(file.Open (TEMP_TOOLBAR_FILE ,CFile::modeCreate|CFile::modeWrite | CFile::typeBinary))
	{  
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		file.WriteString (_T("//toolbar.cfg===================================\r\n\r\n\r\n")) ;
		for(INT i = 0 ; i < dm.GetRowCount () ; i++)
		{
			
			file.WriteString (_T("[TOOLBAR]\r\n")) ;
			
			dm.GetCellData (i,0,strLine) ;
			file.WriteString (_T("ALIAS = ")+strLine +_T("\r\n")) ;
			dm.GetCellData (i,1,strLine) ;
			file.WriteString (_T("TOOLGROUP_ALIAS = ")+strLine+_T("\r\n")) ;
			dm.GetCellData (i,2,strLine) ;
			file.WriteString (_T("IDX = ")+strLine+_T("\r\n")) ;		
			dm.GetCellData (i,3,strLine) ;
			file.WriteString (_T("CAPTION = ")+strLine+_T("\r\n")) ;		
			dm.GetCellData (i,4,strLine) ;
			file.WriteString (_T("STYLE = ")+strLine+_T("\r\n")) ;	
			dm.GetCellData (i,5,strLine) ;
			file.WriteString (_T("TYPE = ")+strLine+_T("\r\n")) ;
			dm.GetCellData (i,6,strLine) ;
			file.WriteString (_T("SM = ")+strLine+_T("\r\n")) ;
			dm.GetCellData (i,7,strLine) ;
			file.WriteString (_T("BITMAP_NORMAL = ")+strLine+_T("\r\n")) ;
			dm.GetCellData (i,8,strLine) ;
			file.WriteString (_T("BITMAP_HOT = ")+strLine+_T("\r\n")) ;
			dm.GetCellData (i,9,strLine) ;
			file.WriteString (_T("BITMAP_DISABLED = ")+strLine+_T("\r\n")) ;
			dm.GetCellData (i,10,strLine) ;
			file.WriteString (_T("TOOLTIP = ")+strLine+_T("\r\n")) ;
            dm.GetCellData (i,11,strLine) ;
			file.WriteString (_T("MENU_ALIAS = ")+strLine+_T("\r\n")) ;
            dm.GetCellData (i,12,strLine) ;
			file.WriteString (_T("ACCECLERATOR = ")+strLine+_T("\r\n")) ;
            dm.GetCellData (i,13,strLine) ;
			file.WriteString (_T("PK = ")+strLine+_T("\r\n")) ;
			file.WriteString (_T("\r\n")) ;
			
		}
	}
	else
	{
		return -1;
	}
	
	file.Close () ;

	
	return 0 ;
}

INT CMenuToolbar_Mgr::Make_ToolbarGroup_File() 
{
    
	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_strConnectionInfo );
	// 접속정보를 설정하고
	CString strQuery ;

	//    0         1             2
	strQuery.Format(_T("SELECT ALIAS, MENU_ALIAS, TOOLGROUP_ALIGN FROM ESL_TOOLGROUP_TBL ") );
	dm.RestructDataManager(strQuery);
	
	CString strLine =_T("");

	CStdioFile file ;
	
	if(file.Open (TEMP_TOOLBARGROUP_FILE,CFile::modeCreate|CFile::modeWrite | CFile::typeBinary))
	{  
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		file.WriteString (_T("//ToolbarGroup.cfg===================================\r\n\r\n\r\n")) ;
		for(INT i = 0 ; i < dm.GetRowCount () ; i++)
		{
			
			file.WriteString (_T("[TOOLBAR_GROUP]\r\n")) ;
			
			dm.GetCellData (i,0,strLine) ;
			file.WriteString (_T("ALIAS = ")+strLine +_T("\r\n")) ;
			dm.GetCellData (i,1,strLine) ;
			file.WriteString (_T("MENU_ALIAS = ")+strLine+_T("\r\n")) ;
			dm.GetCellData (i,2,strLine) ;
			file.WriteString (_T("TOOLGROUP_ALIGN = ")+strLine+_T("\r\n")) ;		
			
			file.WriteString (_T("\r\n")) ;
			
		}
	}
	else
	{
		return -1;
	}
	
	file.Close () ;
	
	
	return 0 ;
}

INT CMenuToolbar_Mgr::Make_Version_File() 
{
	::CreateDirectory(_T("..\\cfg\\menutoolbar"),NULL) ;
	CStdioFile file ;
	CString strLine = _T("") ;
	if(file.Open(VESION_FILE_PATH,CFile::modeCreate|CFile::modeWrite | CFile::typeBinary)) 
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

		file.WriteString (_T("##menutoolbar_version_info.cfg===================================\r\n\r\n\r\n")) ;
		strLine.Format(_T("[USER_NAME = %s ] \r\n"),m_str_USER_NAME) ; 
		file.WriteString (strLine);
	
		strLine.Format(_T("MENU.CFG = %d \r\n"),m_nMenuVersion) ; 
		file.WriteString (strLine);

		strLine.Format(_T("TOOLBAR.CFG = %d \r\n"),m_nMenuVersion) ; 
		file.WriteString (strLine);

		strLine.Format(_T("TOOLBARGROUP.CFG = %d \r\n"),m_nMenuVersion) ; 
		file.WriteString (strLine);
		
	}
	
	file.Close() ;

	return 0 ;
}

//3.file upload
INT CMenuToolbar_Mgr::Upload_File() 
{
	
	
	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_strConnectionInfo );


	CString strQuery ;
	strQuery.Format (_T("select rec_key ,user_name,file_size,version,update_date,file_name from MenuToolbar_Info_Tbl WHERE user_name = '%s'"),m_str_USER_NAME) ;
	dm.RestructDataManager  (strQuery);
	
	CMapStringToString map ;
	
	INT nRowCnt = dm.GetRowCount () ;

	map.RemoveAll () ;

	CString strVersion ;
	CString strFileName ;
	CString strRecKey ;

	::CreateDirectory(_T("..\\cfg\\temp"),NULL) ;

	CPROGRESS_DLG *progressDlg  = new CPROGRESS_DLG  ;
    progressDlg->Create (IDD_DIALOG_PROGRESS);
	progressDlg->CenterWindow() ;
	progressDlg->ShowWindow (SW_SHOW) ;

	progressDlg->m_ProgressCtrl.SetRange (0,100) ;
   
	progressDlg->m_ProgressCtrl.SetPos (1) ;
    progressDlg->ShowResult  (_T("[파일업로드]")) ;
 
	if(nRowCnt == 0)
	{
		//========================================================================================
		progressDlg->SetMessage (_T("menu.cfg 파일생성중...")) ;
		Make_Menu_File() ;
        progressDlg->SetMessage  (_T("menu.cfg 파일생성 완료")) ;
		progressDlg->m_ProgressCtrl.SetPos (10) ;
		
		progressDlg->SetMessage (_T("menu.cfg 파일 업로드중...")) ;
		Upload_Menu_File() ;
		progressDlg->SetMessage  (_T("menu.cfg 파일업로드 완료")) ;
		progressDlg->m_ProgressCtrl.SetPos (30) ;
        //========================================================================================

		//========================================================================================
		progressDlg->SetMessage (_T("toolbar.cfg 파일생성중...")) ;
		Make_Toolbar_File() ;
		progressDlg->SetMessage  (_T("toolbar.cfg 파일생성 완료")) ;
		progressDlg->m_ProgressCtrl.SetPos (40) ;
		progressDlg->SetMessage (_T("toolbar.cfg 파일 업로드중...")) ;
		Upload_Toolbar_File() ;
		progressDlg->SetMessage  (_T("toolbar.cfg 파일업로드 완료")) ;
		progressDlg->m_ProgressCtrl.SetPos (60) ;

		//========================================================================================

		//========================================================================================
		progressDlg->SetMessage (_T("toolbargroup.cfg 파일생성중...")) ;
		Make_ToolbarGroup_File() ;
		progressDlg->SetMessage  (_T("toolbargroup.cfg 파일생성 완료")) ;
		progressDlg->m_ProgressCtrl.SetPos (70) ;
		progressDlg->SetMessage (_T("toolbargroup.cfg 파일 업로드중...")) ;
		Upload_ToolbarGroup_File() ;
        progressDlg->SetMessage  (_T("toolbargroup.cfg 파일업로드 완료")) ;
		progressDlg->m_ProgressCtrl.SetPos (100) ;
		
		//========================================================================================

		// KOL.UDF.022
		//if(progressDlg) 
		if(&progressDlg != NULL) 
		{
			delete progressDlg  ;
			progressDlg = NULL ;
		}

		return 0 ;

	}
	for (INT i = 0 ; i < nRowCnt ;i++)
	{
		strRecKey = dm.GetCellData (i,0) ;
		strFileName = dm.GetCellData (i,5) ;

		strVersion = dm.GetCellData (i,3) ;


		strFileName.TrimLeft () ;
		strFileName.TrimRight () ;
		strFileName.MakeUpper () ;
        
		if(m_bMenu == FALSE && m_bToolbar == FALSE && m_bToolbarGroup == FALSE)
		{
			return 0 ;
		}
		
		if(m_bMenu == TRUE && strFileName == _T("MENU.CFG"))
		{
			progressDlg->SetMessage (_T("menu.cfg 파일생성중...")) ;
			
			m_nMenuVersion = _ttoi(strVersion)+ 1 ;
			Make_Menu_File() ;
			
			progressDlg->SetMessage  (_T("menu.cfg 파일생성 완료")) ;
			progressDlg->m_ProgressCtrl.SetPos (10) ;

			strQuery.Format (_T("DELETE FROM MENUTOOLBAR_INFO_TBL WHERE REC_KEY = %s ;"),strRecKey );
			dm.ExecuteQuery (strQuery,true) ;

			progressDlg->SetMessage (_T("menu.cfg 파일 업로드중...")) ;
			
			Upload_Menu_File() ;
			
			progressDlg->SetMessage  (_T("menu.cfg 파일업로드 완료")) ;
			progressDlg->m_ProgressCtrl.SetPos (30) ;
		
		}
		
		if(m_bToolbar == TRUE && strFileName == _T("TOOLBAR.CFG"))
		{
            progressDlg->SetMessage (_T("TOOLBAR.cfg 파일생성중...")) ;
			
			m_nToolbarVersion  = _ttoi(strVersion)+ 1 ;
			Make_Toolbar_File() ;
			
			progressDlg->SetMessage  (_T("TOOLBAR.cfg 파일생성 완료")) ;
			progressDlg->m_ProgressCtrl.SetPos (40) ;
			

			strQuery.Format (_T("DELETE FROM MENUTOOLBAR_INFO_TBL WHERE REC_KEY = %s ;"),strRecKey );
			dm.ExecuteQuery (strQuery,true) ;

			progressDlg->SetMessage (_T("TOOLBAR.cfg 파일 업로드중...")) ;
			
			Upload_Toolbar_File() ;

			progressDlg->SetMessage  (_T("TOOLBAR.cfg 파일업로드 완료")) ;
			progressDlg->m_ProgressCtrl.SetPos (60) ;
		}
		
		if(m_bToolbarGroup == TRUE && strFileName == _T("TOOLBARGROUP.CFG"))
		{
			progressDlg->SetMessage (_T("TOOLBARGROUP.cfg 파일생성중...")) ;

			m_nToolbarGroupVersion  = _ttoi(strVersion)+ 1 ;
			Make_ToolbarGroup_File() ;

			progressDlg->SetMessage  (_T("TOOLBARGROUP.cfg 파일생성 완료")) ;
			progressDlg->m_ProgressCtrl.SetPos (70) ;
			
			
			strQuery.Format (_T("DELETE FROM MENUTOOLBAR_INFO_TBL WHERE REC_KEY = %s ;"),strRecKey );
			dm.ExecuteQuery (strQuery,true) ;

			progressDlg->SetMessage (_T("TOOLBARGROUP.cfg 파일 업로드중...")) ;
			
			Upload_ToolbarGroup_File() ;
			
			progressDlg->SetMessage  (_T("TOOLBARGROUP.cfg 파일업로드 완료")) ;
			progressDlg->m_ProgressCtrl.SetPos (100) ;
		}

	}


	if(progressDlg) 
	{
		delete progressDlg  ;
		progressDlg = NULL ;
	}

	//::RemoveDirectory(_T("..\\cfg\\temp")) ;
    DeleteFolder (_T("..\\cfg\\temp"));
    RemoveDirectory (_T("..\\cfg\\temp"));

	return 0 ;
}

INT CMenuToolbar_Mgr::Upload_Menu_File() 
{
	CESL_DataMgr dm ;
    dm.SetCONNECTION_INFO (m_strConnectionInfo );
	CTime t = CTime::GetCurrentTime();
	CString cur_date;
	cur_date.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	
	CFile fd;
	fd.Open(TEMP_MENU_FILE, CFile::modeRead | CFile::typeBinary);
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString query;
	CString REC_KEY;

	dm.MakeRecKey(REC_KEY, _T("esl_seq"));
	
	query.Format (_T("INSERT INTO MENUTOOLBAR_INFO_TBL (REC_KEY,USER_NAME,FILE_SIZE,VERSION,UPDATE_DATE,FILE_NAME) VALUES ")
		          _T("( %s ,'%s', %d , %d ,'%s','menu.cfg') ;"),
				  REC_KEY ,m_str_USER_NAME ,fd.GetLength () ,m_nMenuVersion ,cur_date ) ;

	dm.ExecuteQuery(query, true);
	
	BYTE *pBuffer = (BYTE *)malloc(sizeof(BYTE) * fd.GetLength() + 64);
	if (pBuffer == NULL) {
		AfxMessageBox(_T("메모리 할당 에러"));
		return -1;
	}
	INT BufferSize = sizeof(BYTE) * fd.GetLength() + 64;
	fd.ReadHuge(pBuffer, fd.GetLength());
	
	

	INT ids = dm.LOBUpdate(_T("MENUTOOLBAR_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), REC_KEY, _T("FILEDATA"), pBuffer, fd.GetLength());
	if (ids < 0) {
		AfxMessageBox(_T("파일을 DB에 올리던중 에러가 발생하였습니다."));
	} else {
		//AfxMessageBox(_T("파일을 DB에 성공적으로 Upload 하였습니다."));
	}
	if (pBuffer) {
		fd.Close () ;
		free(pBuffer);
		pBuffer = NULL;
	}


	return 0 ;
}

INT CMenuToolbar_Mgr::Upload_Toolbar_File() 
{


	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_strConnectionInfo );
	CTime t = CTime::GetCurrentTime();
	CString cur_date;
	cur_date.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	
	CFile fd;
	fd.Open(TEMP_TOOLBAR_FILE , CFile::modeRead | CFile::typeBinary);
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString query;
	CString REC_KEY;
	
	dm.MakeRecKey(REC_KEY, _T("esl_seq"));
	
	
	query.Format (_T("INSERT INTO MENUTOOLBAR_INFO_TBL (REC_KEY,USER_NAME,FILE_SIZE,VERSION,UPDATE_DATE,FILE_NAME) VALUES ")
		      _T("( %s ,'%s', %d , %d ,'%s','toolbar.cfg') ;"),
			  REC_KEY ,m_str_USER_NAME ,fd.GetLength () ,m_nToolbarVersion   ,cur_date ) ;
	
	
	dm.ExecuteQuery(query, true);
	
	BYTE *pBuffer = (BYTE *)malloc(sizeof(BYTE) * fd.GetLength() + 64);
	if (pBuffer == NULL) {
		AfxMessageBox(_T("메모리 할당 에러"));
		return -1;
	}
	INT BufferSize = sizeof(BYTE) * fd.GetLength() + 64;
	fd.ReadHuge(pBuffer, fd.GetLength());
	
	
	INT ids = dm.LOBUpdate(_T("MENUTOOLBAR_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), REC_KEY, _T("FILEDATA"), pBuffer, fd.GetLength());
	if (ids < 0) {
		AfxMessageBox(_T("파일을 DB에 올리던중 에러가 발생하였습니다."));
	} else {
	//	AfxMessageBox(_T("파일을 DB에 성공적으로 Upload 하였습니다."));
	}
	if (pBuffer) {
		fd.Close () ;
		free(pBuffer);
		pBuffer = NULL;
	}
	
	

return 0 ;
}

INT CMenuToolbar_Mgr::Upload_ToolbarGroup_File() 
{
	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_strConnectionInfo );
	CTime t = CTime::GetCurrentTime();
	CString cur_date;
	cur_date.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	
	CFile fd;
	fd.Open(TEMP_TOOLBARGROUP_FILE , CFile::modeRead | CFile::typeBinary);
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString query;
	CString REC_KEY;
	
	dm.MakeRecKey(REC_KEY, _T("esl_seq"));
	
	
	query.Format (_T("INSERT INTO MENUTOOLBAR_INFO_TBL (REC_KEY,USER_NAME,FILE_SIZE,VERSION,UPDATE_DATE,FILE_NAME) VALUES ")
		      _T("( %s ,'%s', %d , %d ,'%s','toolbargroup.cfg') ;"),
			  REC_KEY ,m_str_USER_NAME ,fd.GetLength () ,m_nToolbarGroupVersion  ,cur_date ) ;
	
	
	dm.ExecuteQuery(query, true);
	
	BYTE *pBuffer = (BYTE *)malloc(sizeof(BYTE) * fd.GetLength() + 64);
	if (pBuffer == NULL) {
		AfxMessageBox(_T("메모리 할당 에러"));
		return -1;
	}
	INT BufferSize = sizeof(BYTE) * fd.GetLength() + 64;
	fd.ReadHuge(pBuffer, fd.GetLength());
	
	INT ids = dm.LOBUpdate(_T("MENUTOOLBAR_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), REC_KEY, _T("FILEDATA"), pBuffer, fd.GetLength());
	if (ids < 0) {
		AfxMessageBox(_T("파일을 DB에 올리던중 에러가 발생하였습니다."));
	} else {
	//	AfxMessageBox(_T("파일을 DB에 성공적으로 Upload 하였습니다."));
	}
	if (pBuffer) {
	    fd.Close () ;
		free(pBuffer);
		pBuffer = NULL;
	}
	
	
	return 0 ;
}

//4.file download
INT CMenuToolbar_Mgr::Download_File() 
{

	// 패치한다.
	CStdioFile file;
	CString strLine;
	if (!file.Open(VESION_FILE_PATH, CFile::modeRead | CFile::typeBinary))
	{
		return -1;
	}
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	CMap<CString, LPCTSTR, INT, INT> map;
	CString strUserName ;

	INT nFind = -1;

	CString strTempAlias ;
	INT     nTempVersion ;

	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty()) continue;
		if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;
        
		
		nFind = strLine.Find('=');
		if (nFind < 0) continue;

		if (strLine.Left(1) == '[')
		{
			strUserName = strLine.Mid(nFind+1) ;
			strUserName.Replace (_T("]"),_T(""));
			continue ;
		}
        strTempAlias = strLine.Mid(0, nFind) ;
		strTempAlias.TrimLeft () ;strTempAlias.TrimRight () ;
		strTempAlias.MakeUpper () ;
		nTempVersion =  _ttoi(strLine.Mid(nFind + 1)) ;


		map.SetAt(strTempAlias,nTempVersion);
	}

	file.Close();

	// 테이블과 버전파일과의 싱크를 맞춘다.
	                         //      0        1           2         3        4
	CString strQuery ; 
	strQuery.Format (_T("SELECT REC_KEY, USER_NAME , FILE_NAME,VERSION , FILE_SIZE FROM MENUTOOLBAR_INFO_TBL where user_name ='%s'"),m_str_USER_NAME);
	m_DM.SetCONNECTION_INFO (m_strConnectionInfo );
	if (m_DM.RestructDataManager(strQuery) < 0) return -1;

	CString strFileName;
	CString strAlias;
	CString strVersion;
	CString strFileSize ;
	CString strRecKey ;

	INT nVersion;
	INT nRowCount = m_DM.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strAlias = m_DM.GetCellData(idx, 2);
		strVersion = m_DM.GetCellData(idx, 3);

		strAlias.TrimLeft () ;strAlias.TrimRight () ;
		strAlias.MakeUpper () ;
		

		if (strAlias.IsEmpty() || strVersion.IsEmpty()) continue;

		if (!map.Lookup(strAlias, nVersion))
		{
			map.SetAt(strAlias, -1);
			nVersion = -1;
		}

		/// 버전 비교
		if (nVersion < _ttoi(strVersion) || m_str_USER_NAME != strUserName )
		{
			strFileName.Format(_T("%s"), m_DM.GetCellData(idx, 2));
		    
			strFileName.TrimLeft () ;strFileName.TrimRight () ;
			strFileName.MakeUpper () ;
           
			strFileSize = m_DM.GetCellData(idx, 4) ;
			strRecKey = m_DM.GetCellData(idx,0) ;
			//	if (DownloadManagementData(m_DM.GetCellData(idx, 0), strFileName, _ttoi(m_DM.GetCellData(idx, 4)))) return -1;
			if(strFileName == _T("MENU.CFG"))
			{
                if (Download_Menu_File (strRecKey ,MENU_FILE_PATH,_ttoi(strFileSize)) ) return -1 ;
			}
			if(strFileName == _T("TOOLBAR.CFG"))
			{
				if (Download_Toolbar_File  (strRecKey ,TOOLBAR_FILE_PATH ,_ttoi(strFileSize)) ) return -1 ;
				
			}
			if(strFileName == _T("TOOLBARGROUP.CFG"))
			{
                if (Download_ToolbarGroup_File  (strRecKey ,TOOLBARGROUP_FILE_PATH,_ttoi(strFileSize)) ) return -1 ;
			}
			
			// 맵의 버전 정보를 업데이트 한다.
			map.SetAt(strAlias, _ttoi(strVersion));
		}
	}
	

	
	// 맵에 저장되어있는 버전 정보를 파일로 쓴다.
	if (!file.Open(VESION_FILE_PATH, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) return -1;
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	
		
	file.WriteString(_T("##menuversion_info.cfg#################################### \r\n"));
	CString strTemp ;
	strTemp.Format (_T("[USER_NAME = %s] \r\n"),m_str_USER_NAME);
	file.WriteString(strTemp);
	
	POSITION pos = map.GetStartPosition();
	while (pos)
	{
		map.GetNextAssoc(pos, strAlias, nVersion);
		strLine.Format(_T("%s=%d\r\n"), strAlias, nVersion);
		file.WriteString(strLine);
	}
	file.Close();

	return 0 ;
}


INT CMenuToolbar_Mgr::Download_Menu_File(CString strRecKey, CString strFileName, INT nFileSize) 
{
	if (strRecKey.IsEmpty()) return -1;

	INT ids = -1;
	CFile file;

	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	// KOL.UDF.022
	//if (pBuffer == NULL)
	if (&pBuffer == NULL)
	{
	AfxMessageBox(_T("Malloc Memory Fail!"));
	return -1;
	}

	m_DM.SetCONNECTION_INFO (m_strConnectionInfo );
	ids = m_DM.LOBGet(_T("MENUTOOLBAR_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILEDATA"), pBuffer, &nBufferSize);
	if (ids < 0) goto ERR;

	//if (nBufferSize <= 0) goto END;

	// 파일에 저장한다.
	if (!file.Open(MENU_FILE_PATH, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary )) goto ERR;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	file.WriteHuge(pBuffer, nBufferSize);
	file.Close();	
	goto END;


	END:
	free(pBuffer);
	return 0;

	ERR:
	CString strMsg;
	strMsg.Format(_T("[%s] 다운로드에 실패했습니다."), strFileName);
	AfxMessageBox(strMsg);
	free(pBuffer);
	return -1;	


}

INT CMenuToolbar_Mgr::Download_Toolbar_File(CString strRecKey, CString strFileName, INT nFileSize)
{

	if (strRecKey.IsEmpty()) return -1;
	
	INT ids = -1;
	CFile file;
	
	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	// KOL.UDF.022
	//if (pBuffer == NULL)
	if (&pBuffer == NULL)
	{
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return -1;
	}
		m_DM.SetCONNECTION_INFO (m_strConnectionInfo );
	ids = m_DM.LOBGet(_T("MENUTOOLBAR_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILEDATA"), pBuffer, &nBufferSize);
	if (ids < 0) goto ERR;
	
	//if (nBufferSize <= 0) goto END;
	
	// 파일에 저장한다.
	if (!file.Open(TOOLBAR_FILE_PATH, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) goto ERR;
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	file.WriteHuge(pBuffer, nBufferSize);
	file.Close();	
	goto END;
	
	
END:
	free(pBuffer);
	return 0;
	
ERR:
	CString strMsg;
	strMsg.Format(_T("[%s] 다운로드에 실패했습니다."), strFileName);
	AfxMessageBox(strMsg);
	free(pBuffer);
	return -1;	
	
}

INT CMenuToolbar_Mgr::Download_ToolbarGroup_File(CString strRecKey, CString strFileName, INT nFileSize)
{

	if (strRecKey.IsEmpty()) return -1;
	
	INT ids = -1;
	CFile file;
	
	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	// KOL.UDF.022
	//if (pBuffer == NULL)
	if (&pBuffer == NULL)
	{
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return -1;
	}
		m_DM.SetCONNECTION_INFO (m_strConnectionInfo );
	ids = m_DM.LOBGet(_T("MENUTOOLBAR_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILEDATA"), pBuffer, &nBufferSize);
	if (ids < 0) goto ERR;
	
	//if (nBufferSize <= 0) goto END;
	
	// 파일에 저장한다.
	if (!file.Open(TOOLBARGROUP_FILE_PATH, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) goto ERR;
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	file.WriteHuge(pBuffer, nBufferSize);
	file.Close();	
	goto END;
	
	
END:
	free(pBuffer);
	return 0;
	
ERR:
	CString strMsg;
	strMsg.Format(_T("[%s] 다운로드에 실패했습니다."), strFileName);
	AfxMessageBox(strMsg);
	free(pBuffer);
	return -1;	
	
}

INT CMenuToolbar_Mgr::SetUploadValue(bool bMenu, bool bToolbar, bool bToolbarGroup)
{
	m_bMenu = bMenu ;
	m_bToolbar = bToolbar ;
	m_bToolbarGroup = bToolbarGroup ;

	return 0 ;
}

INT CMenuToolbar_Mgr::SetDefalutValue(CString username, CString flag, CString spf_use_flag)
{
	m_str_USER_NAME = username ;
	m_str_FLAG = flag ;
	m_str_SPF_USE_FLAG = spf_use_flag ;

	return 0 ;
}

INT CMenuToolbar_Mgr::SetConnectionInfo(CString strConnectioninfo)
{

	m_strConnectionInfo = strConnectioninfo ;

	return 0 ;
}


INT CMenuToolbar_Mgr::DeleteFolder(LPTSTR lpFolder)
{
    WIN32_FIND_DATA  FindData;
    HANDLE    FindHandle;
    TCHAR temp1[MAX_PATH];
    INT lpFolder_Len;
	
    if(lpFolder==NULL)
    {
        return 0;
    }
	
    if(*lpFolder=='\0')
    {
        return 0;
    }
	
    lpFolder_Len=_tcsclen(lpFolder);
	
    if(lpFolder[lpFolder_Len-1]=='\\')
    {
        _stprintf(temp1, _T("%s*.*"), lpFolder);
    }
    else
    {
        _stprintf(temp1, _T("%s\\*.*"), lpFolder);
    }
	
    FindHandle=FindFirstFile(temp1, &FindData);
	
    if(INVALID_HANDLE_VALUE!=FindHandle)
    {
        while(1)
        {
            if(lpFolder[lpFolder_Len-1]=='\\')
            {
				_stprintf(temp1, _T("%s%s"), lpFolder, FindData.cFileName);
            }
            else
            {
                _stprintf(temp1, _T("%s\\%s"), lpFolder, FindData.cFileName);
            }
            //Dir	
            if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if(_tcscmp(FindData.cFileName, _T(".")) != 0 && _tcscmp(FindData.cFileName, _T("..")) != 0)
                {
                    DeleteFolder(temp1);
                    if((FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)|| (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
                    {
                        SetFileAttributes(temp1, FILE_ATTRIBUTE_NORMAL);
                    }
                    RemoveDirectory(temp1);
                }
            }
            //File
            else
            {
                if((FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) || (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
                {
                    SetFileAttributes(temp1, FILE_ATTRIBUTE_NORMAL);
                }
                DeleteFile(temp1);
            }
			
            if(!FindNextFile(FindHandle, &FindData)) break;
        }
    }
    FindClose(FindHandle);
    SetFileAttributes(lpFolder, FILE_ATTRIBUTE_NORMAL);
    //RemoveDirectory(lpFolder);
	return 0;
}
