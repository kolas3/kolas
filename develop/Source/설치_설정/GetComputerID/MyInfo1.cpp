// MyInfo1.cpp: implementation of the CMyInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetComputerID.h"
#include "MyInfo1.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


#define REF_FILE_PATH    _T("..\\cfg\\refeditor\\refeditor_mbcs.cfg")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyInfo::CMyInfo()
{

	m_strIP = _T("");
	m_strConnInfo = _T("") ;
	m_strMyNetAddress = _T("") ;
	m_strMyComputerName  = _T("") ;
	m_strLibCode =_T("") ;
	m_strMANAGE_CODE= _T("");
	m_arrMANAGE_CODE.RemoveAll() ;
}

CMyInfo::~CMyInfo()
{

}

CString CMyInfo::GetMyComputerNetID()
{
/*
    NCB ncb;
    UCHAR uRetCode;
    TCHAR NetName[50];

    wmemset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = 0;

    uRetCode = Netbios( &ncb );
    //_tprintf( _T("The NCBRESET return code is: 0x%x \n"), uRetCode );

    wmemset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = 0;

    _tcscpy( (TCHAR*)ncb.ncb_callname,  _T("*               ") );
    ncb.ncb_buffer = (_TUCHAR *) &Adapter;
    ncb.ncb_length = sizeof(Adapter);

    uRetCode = Netbios( &ncb );
    //_tprintf( _T("The NCBASTAT return code is: 0x%x \n"), uRetCode );
    if ( uRetCode == 0 )
    {

        m_strMyNetAddress.Format(_T("%02x%02x%02x%02x%02x%02x"),
                Adapter.adapt.adapter_address[0],
                Adapter.adapt.adapter_address[1],
                Adapter.adapt.adapter_address[2],
                Adapter.adapt.adapter_address[3],
                Adapter.adapt.adapter_address[4],
                Adapter.adapt.adapter_address[5] );
    }

*/

	INT i;

	CString Message;

	NCB			Ncb;
	UCHAR		uRetCode;
	LANA_ENUM	lenum;
	
	TCHAR szBuffer[4];

	wmemset( &Ncb, 0, sizeof(Ncb) );
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (UCHAR *)&lenum;
	Ncb.ncb_length = sizeof(lenum);
	uRetCode = Netbios( &Ncb );
	//_tprintf( _T("The NCBENUM return code is: 0x%x \n"), uRetCode );

	if ( lenum.length > 0)
	{
		for( i = 0; i < lenum.length; i++)
		{
			wmemset( &Ncb, 0, sizeof(Ncb) );
			Ncb.ncb_command = NCBRESET;
			Ncb.ncb_lana_num = lenum.lana[i];

			uRetCode = Netbios( &Ncb );

			//Message.Format(_T("The NCBRESET on LANA %d return code is: 0x%x \n"), lenum.lana[i], uRetCode);
			//MessageBox(Message.GetBuffer(0));

			wmemset( &Ncb, 0, sizeof (Ncb) );
			Ncb.ncb_command = NCBASTAT;
			Ncb.ncb_lana_num = lenum.lana[i];
			
			

			 _tcscpy( (TCHAR*)Ncb.ncb_callname, _T("* ") );
			//WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, (TCHAR*)Ncb.ncb_callname, sizeof(Ncb.ncb_callname), NULL, NULL);

			Ncb.ncb_buffer = (UCHAR *) &Adapter;
			Ncb.ncb_length = sizeof(Adapter);

			uRetCode = Netbios( &Ncb );

			//Message.Format(_T("The NCBASTAT on LANA %d return code is: 0x%x \n"), lenum.lana[i], uRetCode );
			//MessageBox(Message.GetBuffer(0));
					
			if ( uRetCode == 0 )
			{
				//Message.Format(_T("The Ethernet Number on LANA %d is: %02x%02x%02x%02x%02x%02x\n"), 
				m_strMyNetAddress.Format(_T("%02x%02x%02x%02x%02x%02x"),
				Adapter.adapt.adapter_address[0],
				Adapter.adapt.adapter_address[1],
				Adapter.adapt.adapter_address[2],
				Adapter.adapt.adapter_address[3],
				Adapter.adapt.adapter_address[4],
				Adapter.adapt.adapter_address[5] );

				//MessageBox(Message.GetBuffer(0));
			}
			else
			{
				m_strMyNetAddress = _T("") ;
			}
		}
	}
	else
	{
		m_strMyNetAddress = _T("") ;
	}



	return m_strMyNetAddress ;

}

CString CMyInfo::GetMyComputerName()
{

	TCHAR szComputerName[50] = _T("\0") ;
	DWORD dwSize = 51;

	::GetComputerName (szComputerName,&dwSize) ;
	m_strMyComputerName.Format(_T("%s"),szComputerName) ;


	return m_strMyComputerName  ;
}

CString CMyInfo::GetLibCode()
{

	GetRefEditorFile() ;

	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_strConnInfo);
	if(m_strIP.GetLength()  > 2) 
	{
		dm.m_dbkind = 2 ;
	}
       
	CString strQuery ;
	strQuery.Format (_T("select code from cd_code_tbl where class =1 and  description = '%s'"),m_strMANAGE_CODE ) ;
	dm.GetOneValueQuery (strQuery,m_strMANAGE_CODE) ;

	strQuery =_T("") ;


		// 관리구분별 도서관 설정 Y/N 을 읽어온다.
	CString strTmp;
	dm.GetOneValueQuery(_T("SELECT VALUE_NAME ")
						    _T("FROM ESL_MANAGE_TBL ")
						   _T("WHERE GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통' ")
						     _T("AND CLASS_ALIAS='관리구분별도서관설정'"), strTmp);
	strTmp.TrimLeft();		
	strTmp.TrimRight();
	strTmp.MakeUpper();


	// EFS를 위한 USER_INFO를 파일에 저장
	CString strResult[2];

		//LIB_CODE = CA
		//USER_ID = MONAMY
		//IP = 127.0.0.1

	if (strTmp == _T("Y")) 
	{
		if (m_strMANAGE_CODE.GetLength() == 0) 
		{
			dm.GetValuesQuery(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL "
				_T("WHERE MANAGE_CODE IS NULL")), strResult, 2);
		} 
		else 
		{
			strQuery.Format(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL "
				_T("WHERE MANAGE_CODE = '%s'")), m_strMANAGE_CODE);
			dm.GetValuesQuery(strQuery, strResult, 2);
		}
	}
	else 
	{
		dm.GetValuesQuery(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL "
			_T("WHERE MANAGE_CODE IS NULL")), strResult, 2);
	}


	m_strLibCode.Format(_T("%s"), strResult[0]);



	return m_strLibCode ;
}

bool CMyInfo::GetRefEditorFile()
{
	CStdioFile file ;
	if(!file.Open (REF_FILE_PATH,CFile::modeRead | CFile::typeBinary )) return FALSE ;
	
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

	CString strLine = _T("") ;
	
	while(file.ReadString (strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		
		if (strLine.Left(1) == _T("#") || strLine.IsEmpty())
			continue;

		//HOST , ID , SEQ , DB 
		if(strLine.Find (_T("HOST")) > -1 )
		{
			INT nPos = strLine.Find (_T("[")) ;
			INT nEnd = strLine.Find (_T("]")) ;
			m_strIP = strLine.Mid (nPos+1 ,nEnd-nPos-1) ;
			//m_strConnInfo = strLine.Mid (0,nPos) ;	
			m_strConnInfo = strLine ;
			m_strConnInfo.Replace(_T("HOST"),_T(" ")) ;
			m_strConnInfo.TrimLeft() ;
			m_strConnInfo.TrimRight() ;
		}
	
			
	}

	file.Close () ;

	return TRUE ;
}

VOID CMyInfo::GetManageCode(CStringArray &arrData)
{
	GetRefEditorFile() ;

	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_strConnInfo);
	if(m_strIP.GetLength()  > 2) 
	{
		dm.m_dbkind = 2 ;
	}
    
	CString strQuery ;
	//select code,description from cd_code_tbl where class =1 
	strQuery = _T("SELECT CODE,DESCRIPTION FROM CD_CODE_TBL WHERE CLASS = 1");
	dm.RestructDataManager (strQuery) ;


	INT nRowCnt = dm.GetRowCount ();

	for(INT i = 0 ; i < nRowCnt ;i++)
	{
		arrData.Add(dm.GetCellData (i,1)) ;
	}

	return  ;
}
