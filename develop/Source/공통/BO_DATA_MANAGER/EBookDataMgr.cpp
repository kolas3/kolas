// EBookDataMgr.cpp: implementation of the CEBookDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EBookDataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("단행종API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEBookDataMgr::CEBookDataMgr(CESL_Mgr* pParent /*=NULL*/)
{
	m_pParentMgr = pParent;
	m_pEBookDM = NULL;
	m_bDMCreate = FALSE;
}

CEBookDataMgr::~CEBookDataMgr()
{
	if (m_pEBookDM && m_bDMCreate) 
	{
		delete m_pEBookDM;
		m_pEBookDM = NULL;
	}
}

INT CEBookDataMgr::SetDataMgr(CESL_DataMgr *pDM)
{
	if(m_pParentMgr == NULL) return -1;
	if(pDM == NULL)
	{
		m_pEBookDM = new CESL_DataMgr;
		if(m_pEBookDM == NULL) return -1;
		m_pEBookDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		m_pEBookDM->InitDMPKFromFile(_T("DMUP_단행_종_색인정보"));
		m_pEBookDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		m_bDMCreate = TRUE;
	}
	else
	{
		m_pEBookDM = pDM;
	}
	return 0;
}

INT CEBookDataMgr::MakeEBookInfoAndQueryFrame(CESL_DataMgr *pDM_BOOK, CESL_DataMgr * pDM_OUT)
{
	if(pDM_BOOK == NULL || m_pEBookDM == NULL) return -1;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	INT nCurRow, ids;
	CString strMediaCode, strDBFlag, strKey;
//	CString strImgFile, strEBookFile, strExeFile, strFileSize, strQuery;	
	m_pEBookDM->FreeData();
//	CFile file;
//	CFileException fileException;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pEBookDM->InsertRow(-1);
		nCurRow = m_pEBookDM->GetRowCount()-1;
		CopyDMRowToDMByAlias(pDM_BOOK, i, m_pEBookDM, nCurRow);
		
		//EBOOK DB FLAG설정
		pDM_BOOK->GetCellData(_T("BB_매체구분_코드"), i, strMediaCode);
		pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag);
		if(strMediaCode != _T("EB")) m_pEBookDM->SetCellData(_T("BB_DB_FLAG"), _T("D"), nCurRow);
		else
		{
			if(strDBFlag == _T("D")) m_pEBookDM->SetCellData(_T("BB_DB_FLAG"), _T("D"), nCurRow);
			else
			{
				m_pEBookDM->GetCellData(_T("BB_EBOOKKEY"), nCurRow, strKey);
				if(strKey.IsEmpty()) m_pEBookDM->SetCellData(_T("BB_DB_FLAG"), _T("I"), nCurRow);
				else m_pEBookDM->SetCellData(_T("BB_DB_FLAG"), _T("U"), nCurRow);
			}

		}
		ids = SetEBookInfo(pDM_BOOK, nCurRow);
		if(ids < 0) return -1;
	}
	return MakeEBookQueryFrame(pDM_OUT);
}

INT CEBookDataMgr::MakeEBookQueryFrame(CESL_DataMgr * pDM_OUT)
{
	if(m_pEBookDM == NULL || pDM_OUT == NULL) return -1;
	// 설정된 화일을 서버에 올린다.
	INT ids = UploadEBookFiles();
	if(ids < 0) return ids;
	CString strDBFlag, strKey, strQuery;
	INT nRowCnt = m_pEBookDM->GetRowCount();
	CArray<CString, CString> RemoveAliasList;
	
	for(INT i = 0; i < nRowCnt; i++) 
	{
		RemoveAliasList.RemoveAll();
		strDBFlag.Empty();
		ids = m_pEBookDM->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag);
		if (strDBFlag == _T("I")) 
		{
			pDM_OUT->MakeRecKey(strKey);
			m_pEBookDM->SetCellData(_T("BB_EBOOKKEY"), strKey, i);
			m_pEBookDM->SetCellData(_T("BB_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			m_pEBookDM->SetCellData(_T("BB_최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pEBookDM, _T("BO_EBOOK_TBL"), i, pDM_OUT);
		}
		if (strDBFlag == _T("U")) {
			RemoveAliasList.Add(_T("BB_EBOOKKEY"));
			ids = m_pEBookDM->GetCellData(_T("BB_EBOOKKEY"), i, strKey);
			m_pEBookDM->SetCellData(_T("BB_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			if(!strKey.IsEmpty())
				pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pEBookDM, _T("BO_EBOOK_TBL"), i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strKey);
		}
		if (strDBFlag == _T("D")) 
		{
			ids = m_pEBookDM->GetCellData(_T("BB_EBOOKKEY"), i, strKey);
			if(!strKey.IsEmpty())
			{
				strQuery.Format(_T("DELETE FROM BO_EBOOK_TBL WHERE REC_KEY=%s;"), strKey);
				pDM_OUT->AddFrame(strQuery);
			}
		}
	}	
	return 0;
}

INT CEBookDataMgr::SetEBookInfo(CESL_DataMgr * pDM_BOOK, INT nCurRow)
{
	CString strImgFile, strEBookFile, strExeFile, strFileSize, strDBFlag, strBookKey, strTmpFile;
	CEBookUpNDownloadMgr mgr;

	strDBFlag = m_pEBookDM->GetCellData(_T("BB_DB_FLAG"), nCurRow);
	if(strDBFlag == _T("D")) return 0;
	INT ids;

	if(strDBFlag == _T("I"))
	{
		m_pEBookDM->SetCellData(_T("BB_최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
		m_pEBookDM->SetCellData(_T("BB_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	}
	else if(strDBFlag == _T("U"))
	{
		m_pEBookDM->SetCellData(_T("BB_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCurRow);
	}
	
	strImgFile = m_pEBookDM->GetCellData(_T("BB_이미지파일"), nCurRow);		
	if(!strImgFile.IsEmpty())
	{
		ids = SetEBookFileSize(nCurRow, strImgFile, _T("BB_이미지파일_크기"));
		if(ids < 0 && strDBFlag == _T("I"))
		{
			strBookKey = m_pEBookDM->GetCellData(_T("BB_책KEY"), nCurRow);
			ids = FindSameVolumeEBookInfo(pDM_BOOK, strBookKey, strImgFile, strTmpFile);
			if(ids < 0) return -1;
			ids = SetEBookFileSize(nCurRow, strTmpFile, _T("BB_이미지파일_크기"));
			if(ids < 0) return -1;
			m_pEBookDM->SetCellData(_T("BB_이미지파일"), strTmpFile, nCurRow);
		}			
	}
	strEBookFile = m_pEBookDM->GetCellData(_T("BB_원본파일"), nCurRow);
	if(!strEBookFile.IsEmpty())
	{
		ids = SetEBookFileSize(nCurRow, strEBookFile, _T("BB_원본파일_크기"));
		if(ids < 0 && strDBFlag == _T("I"))
		{
			strBookKey = m_pEBookDM->GetCellData(_T("BB_책KEY"), nCurRow);
			ids = FindSameVolumeEBookInfo(pDM_BOOK, strBookKey, strEBookFile, strTmpFile);
			if(ids < 0) return -1;
			ids = SetEBookFileSize(nCurRow, strTmpFile, _T("BB_원본파일_크기"));
			if(ids < 0) return -1;
			m_pEBookDM->SetCellData(_T("BB_원본파일"), strTmpFile, nCurRow);
		}	
	}
	strExeFile = m_pEBookDM->GetCellData(_T("BB_실행파일"), nCurRow);
	if(!strExeFile.IsEmpty())
	{
		ids = SetEBookExecutorName(nCurRow, strExeFile, _T("BB_실행파일"));
	}
	return 0;
}
INT CEBookDataMgr::CopyDMRowToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	return 0;
}
INT CEBookDataMgr::UploadEBookFiles()
{
	if(m_pEBookDM == NULL) return -1;
	INT nRowCnt = m_pEBookDM->GetRowCount();
	INT ids;
	CEBookUpNDownloadMgr mgr;
	CString strDBFLAG, strFileName, strServerFileName;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strDBFLAG = m_pEBookDM->GetCellData(_T("BB_DB_FLAG"), i);
		if(strDBFLAG == _T("I"))
		{
			ids = mgr.UploadEBookByDM(m_pEBookDM, i);
			if(ids == -2000) 
			{
				m_pParentMgr->ESLAfxMessageBox(_T("E-BOOK 파일을 서버에 업로드 할 수 없습니다."));
				return ids;
			}
		}
		else if(strDBFLAG == _T("U"))
		{
			ids = mgr.DeleteEBookByKeyForUpdate(m_pEBookDM, i);
			if(ids < 0) 
			{
				m_pParentMgr->ESLAfxMessageBox(_T("E-BOOK 파일을 삭제할 수 없습니다."));
				return ids;
			}
			ids = mgr.UploadEBookByDM(m_pEBookDM, i);
			if(ids == -2000) 
			{
				m_pParentMgr->ESLAfxMessageBox(_T("E-BOOK 파일을 서버에 업로드 할 수 없습니다."));
				return ids;
			}
		}
		if(strDBFLAG == _T("D"))
		{
			if(!m_pEBookDM->GetCellData(_T("BB_EBOOKKEY"), i).IsEmpty())
			{
				ids = mgr.DeleteEBookByKey(m_pEBookDM->GetCellData(_T("BB_책KEY"), i), m_pEBookDM->CONNECTION_INFO);
				if(ids == -1000)
				{
					m_pParentMgr->ESLAfxMessageBox(_T("E-BOOK 파일을 서버에서 삭제 할 수 없습니다."));
					return ids;
				}
			}
		}
	}
	return 0;
}
INT CEBookDataMgr::SetEBookFileSize(INT nIdx, CString strFile, CString strAlias)
{
	CFile file;
	CFileException fileException;
	CString strFileSize;
	if (file.Open( strFile.GetBuffer(0), CFile::modeRead | CFile::typeBinary, &fileException ))
	{		
		strFileSize.Format(_T("%d"), file.GetLength());
		m_pEBookDM->SetCellData(strAlias, strFileSize, nIdx);
		file.Close();
		return 0;
	}
	return -1;
}

INT CEBookDataMgr::SetEBookExecutorName(INT nIdx, CString strFile, CString strAlias)
{
	CFile file;
	CFileException fileException;

	if (file.Open( strFile.GetBuffer(0), CFile::modeRead | CFile::typeBinary, &fileException ))
	{				
		m_pEBookDM->SetCellData(strAlias, file.GetFileName(), nIdx);
		file.Close();
		return 0;
	}
	return -1;
}

INT CEBookDataMgr::FindSameVolumeEBookInfo(CESL_DataMgr *pDM_BOOK, CString strBookKey, CString strFile, CString &strTmpFile)
{
	CString strVolSortNo, strTmpKey, strTmpVolSortNo;
	CEBookUpNDownloadMgr mgr;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	INT ids;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpKey = pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
		if(strTmpKey == strBookKey)
			strVolSortNo = pDM_BOOK->GetCellData(_T("BB_권일련번호"), i);
	}
	if(strVolSortNo.IsEmpty()) return -1;

	for(i=0 ; i<nRowCnt ; i++)
	{
		strTmpVolSortNo = pDM_BOOK->GetCellData(_T("BB_권일련번호"), i);
		if(strVolSortNo == strTmpVolSortNo)
		{
			strTmpKey = pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
			if(strTmpKey == strBookKey) continue;
			ids = mgr.DownloadEBook(strTmpKey, strFile, strTmpFile);
			if(ids < 0) continue;
			else return 0;
		}		
	}
	
	return ids;
}