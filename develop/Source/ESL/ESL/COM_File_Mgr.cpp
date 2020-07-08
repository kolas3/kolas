// COM_File_Mgr.cpp: implementation of the CCOM_File_Mgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "COM_File_Mgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCOM_File_Mgr::CCOM_File_Mgr()
{
	m_nDestinationCodePage = -1;
	m_Dlg                  = new CDestinationOption(ISUNICODE);
	m_bFileCopy            = FALSE;
	m_szProcFilePath[0]    = '\0';
}

CCOM_File_Mgr::~CCOM_File_Mgr()
{
	m_nDestinationCodePage = -1;
	delete m_Dlg;
}

INT CCOM_File_Mgr::DestFileOptionDoModal()
{
	m_nDestinationCodePage = -1;

	if ( m_Dlg->DoModal() == IDCANCEL ) return 100;

	m_nDestinationCodePage = m_Dlg->GetDestinationCodePage();	

	return 0;
}


INT CCOM_File_Mgr::DestFileOpen(TCHAR* pszFilePath, TCHAR* pszOpenType)
{
	if ( m_nDestinationCodePage == ISUNICODE ) 
	{
		m_fp = FdkFopen( pszFilePath, pszOpenType );
		if ( m_fp == NULL ) return -5;
	}
	else if ( m_nDestinationCodePage == ISUTF8 ) 
	{
		m_fp = _tfopen( pszFilePath, pszOpenType );
		if ( m_fp == NULL ) return -10;

		BYTE pb[3] = { 0xEF, 0xBB, 0xBF };
		fwrite(pb,1,3,m_fp );
	}
	else  
	{
		m_fp = _tfopen( pszFilePath, pszOpenType );
		if ( m_fp == NULL ) return -15;
	}

	return 0;
}

INT CCOM_File_Mgr::DestFileClose()
{
	if( m_nDestinationCodePage == ISUNICODE )  FdkFclose(m_fp);
	else if( m_nDestinationCodePage == ISUTF8 ) fclose(m_fp);
	else fclose(m_fp);

	return 0;
}

FILE* CCOM_File_Mgr::ReadFileToUTF16FromAllFile(TCHAR* pszOriFilePath, BOOL bFileCopy)
{
	INT ids, nLength=0, nAByte, nUByte, nUB, nIdx=-1;
	CharConvert	Convert;
	nUByte = 50000;
	nAByte = 100000;

	TCHAR * sTemp;
	TCHAR   *sUniBuffer;
	CHAR    *sBuffer;
	CHAR    aszOriFilePath[512];
	CString strFileName, strValue;

	FILE  * get_fp;
	FILE  * put_fp;

	sUniBuffer = new TCHAR[50000*sizeof(TCHAR)];
	sBuffer    = new CHAR [100000*sizeof(CHAR)];	

	m_bFileCopy = bFileCopy;

	if ( m_bFileCopy == TRUE)
	{
		strFileName.Format(_T("%s"), pszOriFilePath);
		nIdx = strFileName.ReverseFind('\\');
		strValue = strFileName.Mid(nIdx+1);
		nIdx =  strValue.ReverseFind('.');
		strValue = strValue.Mid(0, nIdx);
		_stprintf ( m_szProcFilePath , _T("..\\TUNNING\\%s_TmpUni.txt") , strValue.GetBuffer(0) );			

		if( Convert.IsUnicodeFile (pszOriFilePath, &nUByte) == 1000 ) //유니코드 파일일때
		{
			get_fp = FdkFopen ( pszOriFilePath , _T("r") );
			if ( get_fp == NULL )
			{
				delete [] sUniBuffer;
				delete [] sBuffer;
				return NULL;
			}
			put_fp = FdkFopen( m_szProcFilePath, _T("w") );
			if ( put_fp ==NULL )
			{
				delete [] sUniBuffer;
				delete [] sBuffer;
				return NULL;
			}
			while( FdkFgets( sUniBuffer , nUByte , get_fp ) )
			{
				FdkFprintf( put_fp, _T("%s"), sUniBuffer );
			}	
			FdkFclose( get_fp );
			FdkFclose( put_fp );			
		}
		else if( Convert.IsUTF8File (pszOriFilePath, &nUByte) == 3000 )
		{			
			get_fp = _tfopen ( pszOriFilePath , _T("r"));
			if ( get_fp == NULL )
			{
				delete [] sUniBuffer;
				delete [] sBuffer;
				return NULL;
			}
			
			BYTE pb[3];
			fread(pb, 1, 3, get_fp );

			if ((pb[0] == 0xEF) & (pb[1] == 0xBB) & (pb[2] == 0xBF) ) {

			} else {
				fseek( get_fp, 0, SEEK_SET );
			}

			put_fp = FdkFopen( m_szProcFilePath, _T("w") );
			if ( put_fp ==NULL )
			{
				delete [] sUniBuffer;
				delete [] sBuffer;
				return NULL;
			}

			while( fgets( sBuffer , nAByte , get_fp ) )
			{
				ids = cvt.UTF8ToUnicode( sBuffer , &sTemp , &nUB );
				if(ids)
				{
					delete [] sUniBuffer;
					delete [] sBuffer;
					delete [] sTemp;
					return NULL;
				}
				FdkFprintf( put_fp, _T("%s"), sTemp );
				delete [] sTemp;
			}
			fclose( get_fp );
			FdkFclose( put_fp );		
		}
		else 
		{
			WideCharToMultiByte(CP_ACP, 0, pszOriFilePath, _tcslen(pszOriFilePath) + 1, aszOriFilePath, 512, NULL, NULL);
			get_fp = fopen ( aszOriFilePath , "r");
			if ( get_fp == NULL )
			{
				delete [] sUniBuffer;
				delete [] sBuffer;
				return NULL;
			}
			
			put_fp = FdkFopen( m_szProcFilePath, _T("w") );
			if ( put_fp ==NULL )
			{
				delete [] sUniBuffer;
				delete [] sBuffer;
				return NULL;
			}
			
			while( fgets( sBuffer , nAByte , get_fp ) )			
			{
				ids = cvt.MultiByteToUnicode(  sBuffer , & sTemp , &nUB );
				if(ids)
				{
					delete [] sUniBuffer;
					delete [] sBuffer;
					delete [] sTemp;
					return NULL;
				}
				FdkFprintf( put_fp, _T("%s"), sTemp );
				delete [] sTemp;
			}
			
			fclose( get_fp );
			FdkFclose( put_fp );
		}	
	}
	else
	{
		_tcscpy(m_szProcFilePath, pszOriFilePath);		
	}	

	delete [] sUniBuffer;
	delete [] sBuffer;

	return FdkFopen( m_szProcFilePath, _T("r") );
}

INT CCOM_File_Mgr::ColseFileUTF16(FILE* fp)
{	
	CFile Filep;

	FdkFclose(fp);		

	if ( m_bFileCopy == TRUE )
	{
		TRY
		{
			CFile::Remove( m_szProcFilePath );
		}
		CATCH( CFileException, e )
		{
			AfxMessageBox(_T("ColseFileUTF16() Error!!"));
			return -5;
		}
		END_CATCH
	}

	fp = NULL;

	return 0;
}
