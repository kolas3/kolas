// SanFileAPI.cpp: implementation of the SanFileAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SanFileAPI.h"
#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "AVLBaum.h"
#include "CharConvert.h"

CharConvert m_cvt;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SanFileAPI::SanFileAPI(CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	
}

SanFileAPI::~SanFileAPI()
{

}

INT SanFileAPI::SanFileLoad( CESL_DataMgr *pDM , CESL_Mgr* pEslMgr , CString sFileName /*= _T("")*/ )
{
	EFS_BEGIN

	INT  ids;
	INT  nAccessionNoCount = 0;

	const INT nAccessionNoLength = 12;

	TCHAR    szCurrentDir[512];

	CString sPath;
	CString sAccessionNoStream;
	CString sAccessionNo;

	if( sFileName.IsEmpty() )
	{
		_tgetcwd(szCurrentDir, 512);

		CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
		if (fd.DoModal() != IDOK) return 100;

		_tchdir(szCurrentDir);

		sPath = fd.GetPathName();
	}
	else
	{
		sPath = sFileName;
	}

	ids = m_cvt.FileCodeChange(sPath.GetBuffer(0), sPath.GetBuffer(0));
	if ( ids == 100 ) 
	{
		AfxMessageBox(_T("text 파일 형식이 아닙니다."));
		return 0;
	}

	CFile File(sPath, CFile::modeRead | CFile::shareDenyWrite);

	if( File.GetLength() == 0 ) return 0;

	CArchive ar(&File, CArchive::load);

	ar >> nAccessionNoCount;

	if( (nAccessionNoCount < 1) || (nAccessionNoCount > 1000000) )
	{
		ar.Close();
		return 101;
	}
	
	ar >> sAccessionNoStream;
	
	ar.Close();

	CPreRegCodeMgr PreRegCodeMgr(pEslMgr);
	
	CString title;
	for (INT i = 0; i < nAccessionNoCount; i++)
	{

		sAccessionNo = sAccessionNoStream.Mid(i * nAccessionNoLength, nAccessionNoLength);

		if( sAccessionNo.GetLength() != nAccessionNoLength )
			return 0;
		if( !( ( sAccessionNo.GetAt(0) > 64 && sAccessionNo.GetAt(0) < 91 ) || 
			( sAccessionNo.GetAt(0) > 96 && sAccessionNo.GetAt(0) < 123 ) ) )
		{
			File.Close();
			return 101;
		}

		pDM->InsertRow(-1);

		CLocCommonAPI::MakeRegNoSpaceToZero( pEslMgr , sAccessionNo , &PreRegCodeMgr );

		ids = pDM->SetCellData( _T("등록번호") ,sAccessionNo , pDM->GetRowCount()-1 );
		if ( ids < 0 ) 
		{
			File.Close();
			return -2;
		}
	}

	File.Close();

	return 0;

	EFS_END
	return -1;

}

INT SanFileAPI::PortableScannerFileLoad( CESL_DataMgr *pDM , CESL_Mgr* pEslMgr , CString sFileName /*= _T("")*/, bool checkRegNo/* = false */  )
{
	EFS_BEGIN

	INT  ids;

	CString sPath;

    CStdioFile file;
	TCHAR    szCurrentDir[512];

    CString str, line;

	if( sFileName.IsEmpty() )
	{
		_tgetcwd(szCurrentDir, 512);

		CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
		if (fd.DoModal() != IDOK) return 100;

		_tchdir(szCurrentDir);

		sPath = fd.GetPathName();
	}
	else
	{
		sPath = sFileName;
	}

	CPreRegCodeMgr PreRegCodeMgr(pEslMgr);
	FILE *fp = _tfopen( TEMPORARY_FILE, _T("w+b") );
	CString regNo;
	INT rowCnt = pDM->GetRowCount();
	
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);

	if( _T("DM_단행_정리_검색_등록번호") != pDM->GETDMAlias() )
	{
 		for( INT i = 0; i < rowCnt; i++ )
 		{
 			regNo = pDM->GetCellData( _T("등록번호"), i );
 			_ftprintf( fp, _T("%s\r\n"), regNo );
 		}
	}

	CStdioFile newFile;
	newFile.Open( sPath, CFile::modeRead | CFile::typeBinary );
	TCHAR cUni;
	newFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		newFile.SeekToBegin();
	}
	while( newFile.ReadString( regNo ) )
	{
		CLocCommonAPI::MakeRegNoSpaceToZero( pEslMgr , regNo , &PreRegCodeMgr );
		_ftprintf( fp, _T("%s\r\n"), regNo );
		
	}
	fclose( fp );	
	
	switch( MakeSortedFile( file, TEMPORARY_FILE ) )	
	{
		case -1 :
			AfxMessageBox( _T("파일명이 잘못되었습니다.\n올바른 경로인지 확인해 주십시오.") );
			return -1;
			break;
		case -2 :
			AfxMessageBox( _T("등록번호 중복검사에 실패하였습니다.") );
			return -1;
			break;
	}
	
	if( file.GetLength() == 0 ) return 0;

	INT idx = 0;
	CString oldTitle, numericProgress;
	pEslMgr->GetWindowText( oldTitle );
	
	pDM->FreeData();
	INT row = pDM->GetRowCount();

	DWORD START,FINISH;

	START = GetTickCount();
	
	DWORD *pRow ;

	INT nColIndex = pDM->FindColumn(_T("등록번호"));
	
	CWrongRegNoListDlg wrongRegNoListDlg;
	
	INT currentPos = 0;
	WPARAM length = ( WPARAM )file.GetLength();
	SendMessage( pEslMgr->m_hWnd, WM_USER + 807, ( WPARAM )length, NULL );

	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while ( file.ReadString( line ) ) 
	{
		currentPos += line.GetLength();
		SendMessage( pEslMgr->m_hWnd, WM_USER + 808, currentPos, NULL );
		
		numericProgress.Format( _T("%10d 번째 줄 읽는중... "), ++idx );
		pEslMgr->SetWindowText( numericProgress );

		line.TrimRight(); line.TrimLeft();
		if( line.IsEmpty() ) continue;

		if( checkRegNo )
		{
			ids = 0;
			if( !wrongRegNoListDlg.CheckRegNo( line ) )
			{
				pRow = ( DWORD * )pDM->InsertRow( -2 );
				ids = pDM->SetCellData( pRow , nColIndex , line  );
			}
		}
		else
		{
			pRow = ( DWORD * )pDM->InsertRow( -2 );
			ids = pDM->SetCellData( pRow , nColIndex , line  );
		}
	
		if ( ids < 0 ) 
		{
			file.Close();
			return -2;
		}
		
	}
	SendMessage( pEslMgr->m_hWnd, WM_USER + 808, length, NULL );
	
	pDM->RefreshPositionInfo();

	file.Close();

	pEslMgr->SetWindowText( oldTitle );

	FINISH = GetTickCount();

	CString strMsg;
	strMsg.Format(_T("(Finish) - 등록번호 파일 구성시간 :  [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	TRACE(strMsg.GetBuffer(0));
	pDM->QueryLog(strMsg.GetBuffer(0));
	
	if( checkRegNo && wrongRegNoListDlg.m_nWrongRegNoCnt )
	{
		wrongRegNoListDlg.DoModal();
	}

	return 0;

	EFS_END
	return -1;

}

INT SanFileAPI::SanFileSave( CESL_DataMgr *pDM , CString sFileName/*=_T("")*/ )
{
	EFS_BEGIN

	INT  ids;
		
	INT  nRowCount = pDM->GetRowCount();

	if( sFileName.IsEmpty() )
	{
		if( nRowCount < 1 )
		{
			AfxMessageBox( _T("파일로 생성할 등록번호가 존재하지 않습니다. ") );
			return 1;
		}
	}
	

	CString sAccessionNoStream;
	CString sRegNo;

	for (INT i = 0; i < nRowCount; i++) 
	{
		ids = pDM->GetCellData( _T("등록번호") , i , sRegNo );
		if (ids) return -2;

		sAccessionNoStream += sRegNo;

	}

	TCHAR szCurrentDir[512];

	CString sPath;


	if( sFileName.IsEmpty() )
	{
		_tgetcwd(szCurrentDir, 512);
	
		CFileDialog fd(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
		if (fd.DoModal() != IDOK) return 100;
		_tchdir(szCurrentDir);

		sPath = fd.GetPathName();
	}
	else
	{
		sPath = sFileName;
	}

	CFile File(sPath, CFile::modeCreate | CFile::modeWrite);

	CArchive ar(&File, CArchive::store);

	ar << nRowCount;
	ar << sAccessionNoStream;

	ar.Close();
	File.Close();

	return 0;

	EFS_END
	return -1;

}

INT SanFileAPI::PortableScannerFileSave( CESL_DataMgr *pDM , CString sFileName/*=_T("")*/)
{
	EFS_BEGIN

	INT  ids;
		
	INT  nRowCount = pDM->GetRowCount();

	if( sFileName.IsEmpty() )
	{
		if( nRowCount < 1 )
		{
			AfxMessageBox( _T("파일로 생성할 등록번호가 존재하지 않습니다. ") );
			return 1;
		}
	}
	
	CString sPath;

    CStdioFile file;
    TCHAR msg[512];
	TCHAR szCurrentDir[512];

    CString str, line;

	if( sFileName.IsEmpty() )
	{
		_tgetcwd(szCurrentDir, 512);

		CFileDialog fd(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
		if (fd.DoModal() != IDOK) return 100;

		_tchdir(szCurrentDir);

		sPath = fd.GetPathName();
	}
	else
	{
		sPath = sFileName;
	}


	if ( !file.Open( sPath , CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) ) {
        _stprintf( msg, _T("파일명이 잘못되었습니다.\n올바른 경로인지 확인해 주십시오.") );
		AfxMessageBox( msg );
		return -1;
    }
 		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

	CString sRegNo;

	for (INT i = 0; i < nRowCount; i++) 
	{
		ids = pDM->GetCellData( _T("등록번호") , i , sRegNo );
		if (ids) return -2;
		sRegNo.TrimLeft(); sRegNo.TrimRight();

		if( sRegNo.IsEmpty() ) continue;

		file.WriteString( sRegNo );
		file.WriteString( _T("\r\n") );
	}

	file.Close();

	return 0;

	EFS_END
	return -1;

}

INT SanFileAPI::RegNoFileLoad( CESL_DataMgr *pDM , CESL_Mgr* pEslMgr , CString sFileName/*=_T("")*/ )
{
	EFS_BEGIN

	if( sFileName.IsEmpty() )
	{
		TCHAR szCurrentDir[512];
		_tgetcwd(szCurrentDir, 512);
		CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
		if (fd.DoModal() != IDOK) return 100;
		_tchdir(szCurrentDir);
		
		sFileName = fd.GetPathName();
	}
	INT ids;

	CComboBox *pCombo = ( CComboBox * )pEslMgr->FindControl( _T("CM_BO_LOC_2110_INFO"), _T("등록번호검사") );
	bool flag = false;
	if( pCombo->GetSafeHwnd() )
		if( !pCombo->GetCurSel() )
			flag = true;
	
	ids = SanFileLoad( pDM , pEslMgr , sFileName );
	if( ids == 101 )
	{
		ids = PortableScannerFileLoad( pDM , pEslMgr , sFileName, flag );
		return CHECK_DUPLICATEREGNO_IS_DONE;
	}

	return 0;

	EFS_END
	return -1;

}

INT SanFileAPI::RegNoFileSave( CESL_DataMgr *pDM , CString sFileName/*=_T("")*/ )
{
	EFS_BEGIN

	if( sFileName.IsEmpty() )
	{
		TCHAR szCurrentDir[512];
		_tgetcwd(szCurrentDir, 512);
		CFileDialog fd(FALSE, _T(".txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("파일형식(*.txt)|*.txt|") );
		if (fd.DoModal() != IDOK) return 100;
		_tchdir(szCurrentDir);
		
		sFileName = fd.GetPathName();
	}
	else
	{
		TCHAR szCurrentDir[512];
		_tgetcwd(szCurrentDir, 512);
		
		CFileDialog fd(FALSE, _T(".txt"), sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("파일형식(*.txt)|*.txt|") );
		if (fd.DoModal() != IDOK) return 100;
		_tchdir(szCurrentDir);
		
		sFileName = fd.GetPathName();

	}
	
	INT ids;

	ids = PortableScannerFileSave ( pDM , sFileName );
	return ids;

	EFS_END
	return -1;

}

VOID SanFileAPI::SortFile( const TCHAR *ifname, const TCHAR *ofname )
{
	CStdioFile fInputFile;
	fInputFile.Open( ifname , CFile::modeRead | CFile::typeBinary );
		TCHAR cUni;
		fInputFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fInputFile.SeekToBegin();
		}

	CAVLTree< CString > tree;

	CString sLine;
	CStdioFile fOutputFile;
	fOutputFile.Open( ofname , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary );

	while( fInputFile.ReadString(sLine) )
	{
		if( 0 == fOutputFile.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			fOutputFile.Write( &cUni, sizeof(TCHAR));
		}
		fOutputFile.WriteString( sLine );
		fOutputFile.WriteString(_T("\r\n"));
	}

	fInputFile.Close();
	fOutputFile.Close();

}

INT SanFileAPI::MakeSortedFile( CStdioFile &file, CString path )
{
	if( !file.Open( path , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) )
		return -1;
	else
		file.Close();

	CString newFile = TEMPORARY_REGNO_FILE;
	SortFile( path.GetBuffer( 0 ), newFile.GetBuffer( 0 ) );

	if( !file.Open( newFile , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) )
		return -2;
	else
		return 0;
}


INT SanFileAPI::RemoveDuplicateRcd( CESL_DataMgr *pDM )
{
	
	
	
	return 0;
}