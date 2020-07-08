// EBookUpNDownloadMgr.cpp: implementation of the EBookUpNDownloadMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EBookUpNDownloadMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEBookUpNDownloadMgr::CEBookUpNDownloadMgr()
{
}

CEBookUpNDownloadMgr::~CEBookUpNDownloadMgr()
{

}

INT CEBookUpNDownloadMgr::UploadEBook(CString strFile, CString &strServerFileName)
{
	CFile myFile;
	CFileException fileException;
	if (!IsExist(strFile))
	{
		return -1000;
	}
	
	if(!myFile.Open(strFile.GetBuffer(0), CFile::modeRead | CFile::typeBinary, &fileException))
	{
		return -3000;
	}
	TCHAR cUni;
	myFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		myFile.SeekToBegin();
	}
	
	CString strFilePath, strFileName, strServerFilePath;
	strFilePath = myFile.GetFilePath();
	strFileName = myFile.GetFileName();
	myFile.Close();
	//strFilePath.Replace(strFileName, _T(""));
	strServerFileName = strFileName;
	INT nPos = m_strServerFilePath.ReverseFind('/');
	if(nPos < 0) return -4000;
	strServerFileName = m_strServerFilePath.Mid(nPos+1) + _T("_") + strFileName;
//	INT ids = RS_UpLoadFile(
//		strFileName.GetBuffer(0), strFilePath.GetBuffer(0), strServerFileName.GetBuffer(0), SERVER_EBOOK_POS, SERVER_POS);
//	if(ids < 0) return -2000;
	strServerFileName = strFileName;
	return 0;
}

INT CEBookUpNDownloadMgr::DownloadEBook(CString strKey, CString strFile, CString &strLocalFileName)
{
	if(strKey.IsEmpty()) return -1;
	TCHAR TmpDir[500];
	GetCurrentDirectory(500,TmpDir);
	CString strCurrentPath(TmpDir);
	m_strLocalFilePath = strCurrentPath + LOCAL_EBOOK_POS;
	m_strLocalFilePath.Replace(_T("\\"), _T("/"));
	CreateDirectory(m_strLocalFilePath.GetBuffer(0), NULL);
	m_strLocalFilePath += strKey;
	strLocalFileName = m_strLocalFilePath + _T("/") + strFile;
	
	CString strServerFile = strKey + _T("_") + strFile;
	
	if (!IsExist(m_strLocalFilePath) && !CreateDirectory(m_strLocalFilePath.GetBuffer(0), NULL)) 
	{ 
		return -1000;
	} 

//	INT ids = RS_DownLoadFile(
//		strFile.GetBuffer(0), strLocalFileName.GetBuffer(0), strServerFile.GetBuffer(0), SERVER_EBOOK_POS, SERVER_POS);
//	if(ids < 0) return ids;
	return 0;
}

INT CEBookUpNDownloadMgr::DeleteEBook(CString strFile)
{
	if(strFile.IsEmpty()) return 0;

	CString strServerFile = m_strServerFilePath + _T("_") + strFile;
	//INT ids = RS_RemoveFile(strServerFile.GetBuffer(0), SERVER_EBOOK_POS, SERVER_POS);
//	if(ids < 0 && ids != -6000) return -1000;
	return 0;
}

INT CEBookUpNDownloadMgr::DeleteEBookByKey(CString strKey, CString strConnectionInfo)
{
	if(strKey.IsEmpty() || strConnectionInfo.IsEmpty()) return -1;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(strConnectionInfo);
	CString strQuery;
	strQuery.Format(_T("SELECT IMAGENAME, FILENAME FROM BO_EBOOK_TBL WHERE BOOK_KEY=%s"), strKey);
	tmpDM.RestructDataManager(strQuery);
	if(tmpDM.GetRowCount() == 0) return 0;
	
	m_strServerFilePath = strKey;
	CString strTmpData;
	INT ids = DeleteEBook(tmpDM.GetCellData(0, 0));
	if(ids < 0) return ids;
	ids = DeleteEBook(tmpDM.GetCellData(0, 1));
	if(ids < 0) return ids;
	return 0;
}

INT CEBookUpNDownloadMgr::UploadEBookByDM(CESL_DataMgr * pDM, INT nIdx)
{
	if(pDM == NULL) return -1;
	CString strBookKey = pDM->GetCellData(_T("BB_책KEY"), nIdx);

	m_strServerFilePath = SERVER_EBOOK_POS + strBookKey;
	
	CString strFile, strServerFile;
	strFile = pDM->GetCellData(_T("BB_원본파일"), nIdx);
	INT ids = UploadEBook(strFile, strServerFile);
	if(ids == -2000) return ids;
	else if( ids == 0) pDM->SetCellData(_T("BB_원본파일"), strServerFile, nIdx);
	strFile = pDM->GetCellData(_T("BB_이미지파일"), nIdx);
	ids = UploadEBook(strFile, strServerFile);
	if(ids == -2000) return ids;
	else if( ids == 0) pDM->SetCellData(_T("BB_이미지파일"), strServerFile, nIdx);

	return 0;
}

BOOL CEBookUpNDownloadMgr::IsExist(CString strFile)
{
	CFileFind ff;

    if( ff.FindFile(strFile) )
    {
        return TRUE;
    }
	return FALSE;
}

INT CEBookUpNDownloadMgr::DeleteEBookTempFolder()
{
    SHFILEOPSTRUCT FileOp = {0};
	TCHAR TmpDir[500];
	GetCurrentDirectory(500,TmpDir);
	CString strPath(TmpDir);
	strPath += LOCAL_EBOOK_POS;
	strPath.Replace(_T("/"), _T("\\"));

	INT ids = DeleteFolder(strPath.GetBuffer(0));

	return ids;
}

INT CEBookUpNDownloadMgr::DeleteFolder(LPTSTR lpFolder)
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

INT CEBookUpNDownloadMgr::DeleteEBookByKeyForUpdate(CESL_DataMgr *pDM_EBOOK, INT nIdx)
{
	CString strEBookFile, strImgFile, strKey, strEbookKey;
	BOOL bEBookUpdate, bImgUpdate;
	bEBookUpdate = bImgUpdate = FALSE;
	strEBookFile = pDM_EBOOK->GetCellData(_T("BB_원본파일"), nIdx);
	strImgFile = pDM_EBOOK->GetCellData(_T("BB_이미지파일"), nIdx);
	strKey = pDM_EBOOK->GetCellData(_T("BB_책KEY"), nIdx);
	strEbookKey = pDM_EBOOK->GetCellData(_T("BB_EBOOKKEY"), nIdx);
	if(strEbookKey.IsEmpty()) return 0;
	
	bEBookUpdate = IsExist(strEBookFile);
	bImgUpdate = IsExist(strImgFile);

	if(strKey.IsEmpty()) return -1;

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM_EBOOK->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(_T("SELECT IMAGENAME, FILENAME FROM BO_EBOOK_TBL WHERE BOOK_KEY=%s"), strKey);
	tmpDM.RestructDataManager(strQuery);
	if(tmpDM.GetRowCount() == 0) return 0;
	
	m_strServerFilePath = strKey;
	CString strTmpData;
	INT ids;
	if(bImgUpdate)
	{
		ids = DeleteEBook(tmpDM.GetCellData(0, 0));
		if(ids < 0) return ids;
	}
	if(bEBookUpdate)
	{
		ids = DeleteEBook(tmpDM.GetCellData(0, 1));
		if(ids < 0) return ids;
	}
	return 0;
}
