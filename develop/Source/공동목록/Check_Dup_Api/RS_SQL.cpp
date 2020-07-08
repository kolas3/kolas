// RS_SQL.cpp: implementation of the CRS_SQL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RS_SQL.h"

#include "ESL_DataMgr.h"
//#include "..\..\include\공동목록\RS_Acc.h"
#include "ReadCfgFile.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CRS_SQL::CRS_SQL(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)	
{

}

CRS_SQL::~CRS_SQL()
{

}
//도서관 부호 갖고 오기 
INT CRS_SQL::GetLibraryName(CString &str_Lib_Name)
{   
	CESL_DataMgr pDM_2;
     
	CString sPath ;
	CReadCfgFile ReadCfg;	
	ReadCfg.ReadCfgFile(sPath);

	pDM_2.SetCONNECTION_INFO (ReadCfg.m_sConnection) ;
	
	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString sQuery_libcode ;
	sQuery_libcode.Format( _T("SELECT LIB_NAME FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE") ) ;   
     
    CString tmp_Lib_Name ;
	pDM_2.GetOneValueQuery (sQuery_libcode,tmp_Lib_Name) ; 

    str_Lib_Name = tmp_Lib_Name ;


	return 0 ;
}

INT CRS_SQL::GetLibraryInfo(CString &lib_Code) 
{

    CESL_DataMgr pDM;
    CString sPath ;
	CReadCfgFile ReadCfg;	
	ReadCfg.ReadCfgFile(sPath);

	pDM.SetCONNECTION_INFO (ReadCfg.m_sConnection) ;

	// 통합시스템 적용
	CString sQuery_libcode ;
	sQuery_libcode.Format (_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE") );   

    CString tmp_Lib_Code ;
	pDM.GetOneValueQuery (sQuery_libcode,tmp_Lib_Code) ; 

    if(tmp_Lib_Code.IsEmpty ())
	{
		AfxMessageBox (_T("도서관 부호가 설정 되어 있지 않습니다.")) ;
		return 0 ;
	}

	lib_Code = tmp_Lib_Code;
    

	return 0 ;
}

INT CRS_SQL::GetSytemDate (CString &systemdate)
{    
	CString current ;
	
	CTime t = CTime::GetCurrentTime();
	
    current.Format (_T("%4d/%2d/%2d"),t.GetYear() ,t.GetMonth() ,t.GetDay ());
	
    systemdate = current ;
	
	return 0 ;
	
}

