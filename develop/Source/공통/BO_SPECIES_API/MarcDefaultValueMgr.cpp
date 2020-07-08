// MarcDefaultValueMgr.cpp: implementation of the CMarcDefaultValueMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcDefaultValueMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcDefaultValueMgr::CMarcDefaultValueMgr(CESL_Mgr * pMgr)
{
	m_pParentMgr = pMgr;
	m_strFileName.Empty();
}

CMarcDefaultValueMgr::~CMarcDefaultValueMgr()
{

}

INT CMarcDefaultValueMgr::LoadDefaultValue(CString &strMarc)
{
	if(m_strFileName.IsEmpty()) return -1;

	CStdioFile file;
	if (!file.Open(m_strFileName, CFile::modeRead | CFile::typeBinary))
		return -1;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	file.ReadString(strMarc);
	
	file.Close();
	return 0;
}

INT CMarcDefaultValueMgr::SaveDefaultValue(CString strMarc)
{
	if(m_strFileName.IsEmpty()) return -1;

	CStdioFile file;
	if (!file.Open(m_strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
	
	file.WriteString(strMarc);
	
	file.Close();
	return 0;
}

INT CMarcDefaultValueMgr::ApplyBookCollationByDefaultValue(CString strFormCode, CESL_DataMgr *pDM_IDX, CESL_DataMgr *pDM_BOOK)
{
	if(m_pParentMgr == NULL || pDM_BOOK == NULL || pDM_IDX == NULL) return -1;
	if(pDM_BOOK->GetRowCount() == 0) return 0;

	CString strData = strFormCode + _T(".cfg");
	m_strFileName = _T("..\\cfg\\species_form.cfg");
	m_strFileName.Replace(_T(".cfg"), strData);
	
	CString strMarc, strTmpData, strRepresentBookYN;
	INT ids = LoadDefaultValue(strMarc);
	if(ids < 0) return -1;

	if(strMarc.IsEmpty()) return 0;
	CMarc marc;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	if(ids < 0) return -1;

	for(INT i=0 ; i<pDM_BOOK->GetRowCount() ; i++)
	{
		strRepresentBookYN = pDM_BOOK->GetCellData(_T("BB_대표책여부"), i);
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("300$c"), strTmpData);
		pDM_BOOK->SetCellData(_T("BB_크기"), strTmpData, i);		
		if(strRepresentBookYN == _T("Y")) pDM_BOOK->SetCellData(_T("IBS_크기"), strTmpData, 0);		
		strTmpData.Empty();
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("300$a"), strTmpData);
		pDM_BOOK->SetCellData(_T("BB_면장수"), strTmpData, i);		
		if(strRepresentBookYN == _T("Y")) pDM_BOOK->SetCellData(_T("IBS_면장수"), strTmpData, 0);
		strTmpData.Empty();
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("300$e"), strTmpData);
		pDM_BOOK->SetCellData(_T("BB_딸림자료"), strTmpData, i);	
		if(strRepresentBookYN == _T("Y")) pDM_BOOK->SetCellData(_T("IBS_딸림자료"), strTmpData, 0);	
		strTmpData.Empty();
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(&marc, _T("300$b"), strTmpData);
		pDM_BOOK->SetCellData(_T("BB_물리적특성"), strTmpData, i);	
		if(strRepresentBookYN == _T("Y")) pDM_BOOK->SetCellData(_T("IBS_물리적특성"), strTmpData, 0);	
		strTmpData.Empty();
	}
	return 0;
}
