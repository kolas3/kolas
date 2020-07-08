// MarcRelation.cpp: implementation of the CMarcRelation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlbase.h>
#include "MarcRelation.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////// //////

CMarcRelation::CMarcRelation()
{
	m_strSimpleRelationFileName = _T("marc_simple_relation.txt");
	m_strDeepRelationFileName = _T("marc_deep_relation.txt");
}

CMarcRelation::~CMarcRelation()
{
	FreeSimpleRelationList();
	FreeDeepRelationList();
	FreeInstructionList();
}

INT CMarcRelation::FreeSimpleRelationList()
{
	EFS_BEGIN

	INT nListCount = m_listSimpleRelation.GetCount();
	if (nListCount == 0) return 0;
	CMarcSimpleRelation *pMarcSimpleRelation;
	for (INT i = 0; i < nListCount; i++)
	{
		pMarcSimpleRelation = (CMarcSimpleRelation *)m_listSimpleRelation.RemoveHead();
 		if (pMarcSimpleRelation) delete pMarcSimpleRelation;
	}
	m_listSimpleRelation.RemoveAll();
	return nListCount;

	EFS_END

	return -1;
}

INT CMarcRelation::FreeDeepRelationList()
{
	EFS_BEGIN

	INT nListCount = m_listDeepRelation.GetCount();
	if (nListCount == 0) return 0;
	CMarcInstruction *pMarcInstruction;
	for (INT i = 0; i < nListCount; i++)
	{
		pMarcInstruction = (CMarcInstruction *)m_listDeepRelation.RemoveHead();
		if (pMarcInstruction) delete pMarcInstruction;
	}
	m_listDeepRelation.RemoveAll();
	return nListCount;

	EFS_END

	return -1;
}

INT CMarcRelation::FreeInstructionList()
{
	EFS_BEGIN

	INT nListCount = m_listInstruction.GetCount();
	if (nListCount == 0) return 0;
	CMarcInstruction *pMarcInstruction;
	for (INT i = 0; i < nListCount; i++)
	{
		pMarcInstruction = (CMarcInstruction*)m_listInstruction.RemoveHead();
		if (pMarcInstruction) delete pMarcInstruction;
	}
	m_listInstruction.RemoveAll();
	return nListCount;

	EFS_END

	return -1;
}


//////////////////////////////////////////////////////////////////////
// Make Marc Relation : Fetch Simple relation / Deep relation
//////////////////////////////////////////////////////////////////////

INT CMarcRelation::MakeMarcRelation(CString strMarcGroup)
{	
	EFS_BEGIN

	if (strMarcGroup.IsEmpty()) return -1;

	// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
	// 통합마크 개정 적용
/*//NEW------------------------------------------------------------------------*/
	if (_T("통합마크") == strMarcGroup) {
		CString strGetGroupName = _T("");
		CString strFileName = _T("..\\cfg\\MarcRelation\\통합마크_개정_그룹명.cfg");
		CFileFind find;
		if (TRUE == find.FindFile(strFileName)) {
			CStdioFile file;
			if (TRUE == file.Open(strFileName, CFile::modeRead|CFile::typeBinary)) {
				TCHAR cUni;
				file.Read(&cUni, sizeof(TCHAR));
				if (0xFEFF != cUni) file.SeekToBegin();
				CString strTempData, strGetData;
				while (file.ReadString(strGetData)) {
					strGetData.TrimLeft();
					if (strGetData.IsEmpty()) continue;
					if (_T("#") == strGetData.Left(1)) continue;
					INT nPos = strGetData.Find(_T("="));
					if (0 <= nPos) {
						strTempData = strGetData.Left(nPos);
						strTempData.TrimLeft(); strTempData.TrimRight();
						if (_T("그룹명") == strTempData) {
							strGetGroupName = strGetData.Mid(nPos+1, strGetData.GetLength());
							strGetGroupName.TrimLeft(); strGetGroupName.TrimRight();
							break;
						}
					}
				}
				file.Close ();
			}
			find.Close();
		}
		if (!strGetGroupName.IsEmpty()) {
			strMarcGroup = strGetGroupName;
		}
	}
/*//END------------------------------------------------------------------------*/

	// Free list
	FreeInstructionList();
	FreeDeepRelationList();
	FreeSimpleRelationList();

	// Simple relation parse  --> finally get m_listInstruction
	m_strSimpleRelationFileName.Format(_T("..\\CFG\\MarcRelation\\SimpleRelation_%s.txt"), strMarcGroup);
	if (InstructionList_FromTextFile(m_strSimpleRelationFileName) >= 0)
		MakeSimpleRelation();
	
	// Deep relation paarse  --> finally get m_listInstruction
	FreeInstructionList();
	m_strDeepRelationFileName = _T("..\\CFG\\MarcRelation\\DeepRelation.txt");
	if (InstructionList_FromTextFile(m_strDeepRelationFileName) >= 0)
		MakeDeepRelation();
		
	// Free temp instruction list
	FreeInstructionList();
	return 0;

	EFS_END

	return -1;
}

INT CMarcRelation::MakeSimpleRelation()
{
	EFS_BEGIN

	CString strGroupName;
	while (GetGroupNameFromInstructionList(strGroupName) >= 0)
	{
		CMarcSimpleRelation *pMarcSimpleRelation = new CMarcSimpleRelation;
		if (pMarcSimpleRelation == NULL) continue;

		POSITION pos = m_listInstruction.GetTailPosition();
		while (pos != NULL)
		{
			POSITION posOld = pos;
			CMarcInstruction* pMarcInstruction = (CMarcInstruction*)m_listInstruction.GetPrev(pos);
			if (pMarcInstruction == NULL) continue;
			
			//if (pMarcInstruction->m_strGroupName != strGroupName) continue;
			if (wmemcmp(pMarcInstruction->m_pszGroupName, strGroupName.GetBuffer(0), _tcslen(pMarcInstruction->m_pszGroupName)) != 0) continue;

			m_listInstruction.RemoveAt(posOld);

			// Group name
			pMarcSimpleRelation->m_strGroupName = strGroupName;		
			// TAG + EXAMPLE
			//pMarcSimpleRelation->m_mapTag.SetAt(pMarcInstruction->m_strParam, *pMarcInstruction);
			// OPERATION TAG
			
			if (wmemcmp(pMarcInstruction->m_pszOperator, _T("TAG"), 3) == 0)
				pMarcSimpleRelation->m_mapTag.SetAt(pMarcInstruction->m_pszBasicParam, *pMarcInstruction);
			// OPERATION REPEAT TAG
			else if (wmemcmp(pMarcInstruction->m_pszOperator, _T("REPEAT_TAG"), 10) == 0)
				pMarcSimpleRelation->m_mapRepeatTag.SetAt(pMarcInstruction->m_pszBasicParam, *pMarcInstruction);
			// OPERATION SUBFILED_CODE
			else if (wmemcmp(pMarcInstruction->m_pszOperator, _T("SUBFILED_CODE"), 12) == 0)
				pMarcSimpleRelation->m_mapSubfiled.SetAt(pMarcInstruction->m_pszBasicParam, *pMarcInstruction);
			// OPERATION INDICATOR
			else if (wmemcmp(pMarcInstruction->m_pszOperator, _T("INDICATOR"), 9) == 0)
				pMarcSimpleRelation->m_mapIndicator.SetAt(pMarcInstruction->m_pszBasicParam, *pMarcInstruction);
			// OPERATION DEFAULT INDICATOR
			else if (wmemcmp(pMarcInstruction->m_pszOperator, _T("DEFAULT_INDICATOR"), 17) == 0)
				pMarcSimpleRelation->m_mapDefaultIndicator.SetAt(pMarcInstruction->m_pszBasicParam, *pMarcInstruction);
			// OPERATION REPEAT_SUBFIELD_CODE
			else if (wmemcmp(pMarcInstruction->m_pszOperator, _T("REPEAT_SUBFIELD_CODE"), 20) == 0)
				pMarcSimpleRelation->m_mapRepeatSubfield.SetAt(pMarcInstruction->m_pszBasicParam, *pMarcInstruction);
			// OPERATION PUNCTUATION_MARK
			else if (wmemcmp(pMarcInstruction->m_pszOperator, _T("PUNCTUATION_MARK"), 16) == 0)
				pMarcSimpleRelation->m_mapPunctuationMark.SetAt(pMarcInstruction->m_pszBasicParam, *pMarcInstruction);
		
			delete pMarcInstruction;

		}
		m_listSimpleRelation.AddTail((CObject*)pMarcSimpleRelation);
	}
	
	return 0;

	EFS_END

	return -1;
}

INT CMarcRelation::MakeDeepRelation()
{
	EFS_BEGIN

	INT nCount = m_listInstruction.GetCount();
	if (nCount == 0) return 0;

	for (INT i = 0; i < nCount; i++)
	{
		CMarcInstruction *pMarcInstruction = (CMarcInstruction*)m_listInstruction.RemoveHead();
		if (pMarcInstruction == NULL) continue;

		m_listDeepRelation.AddTail((CObject*)pMarcInstruction);
	}

	return 0;

	EFS_END

	return -1;
}

INT CMarcRelation::GetGroupNameFromInstructionList(CString &strGroupName)
{
	EFS_BEGIN

	if (m_listInstruction.GetCount() == 0) 
	{
		m_strLastError = _T("Instruction list is empty");
		return -1;
	}

	CMarcInstruction *pMarcInstruction = (CMarcInstruction*)m_listInstruction.GetTail();


	//CMarcInstruction *pMarcInstruction = (CMarcInstruction*)m_listInstruction.GetAt(m_listInstruction.FindIndex(0));
	if (pMarcInstruction == NULL) 
	{
		m_strLastError = _T("marc instruction is null");
		return -1;
	}

	//strGroupName = pMarcInstruction->m_strGroupName;
	strGroupName.Format(_T("%s"), pMarcInstruction->m_pszGroupName);
	return 0;

	EFS_END

	return -1;
}

CMarcSimpleRelation *CMarcRelation::GetSimpleMarcRelation(CString strGroupName)
{
	EFS_BEGIN

	CMarcSimpleRelation *pMarcSimpleRelation;
	POSITION pos = m_listSimpleRelation.GetHeadPosition();
	while (pos)
	{
		pMarcSimpleRelation = (CMarcSimpleRelation*)m_listSimpleRelation.GetNext(pos);
		if (pMarcSimpleRelation->m_strGroupName == strGroupName)
			return pMarcSimpleRelation;
	}
	
	m_strLastError = _T("Not exist marc group name");
	return NULL;

	EFS_END

	return NULL;
}

CMarcInstruction *CMarcRelation::GetDeepMarcInstruction(CString strGroupName, CString strBasicParam)
{
	EFS_BEGIN

	CMarcInstruction *pMarcInstruction;
	POSITION pos = m_listDeepRelation.GetHeadPosition();

	while (pos)
	{
		pMarcInstruction = (CMarcInstruction*)m_listDeepRelation.GetNext(pos);
		if ((_tcscmp(pMarcInstruction->m_pszGroupName, strGroupName.GetBuffer(0)) == 0) && (_tcscpy(pMarcInstruction->m_pszBasicParam ,strBasicParam.GetBuffer(0)) == 0))
		{
			return pMarcInstruction;
		}
	}

	m_strLastError = _T("Not exist marc group name");
	return NULL;

	EFS_END

	return NULL;
}

/***************************************************************************************************
Return values
0		:		S_OK
-2		:		can not create output file
****************************************************************************************************/
INT CMarcRelation::InstructionList_ToTextFile(CString strFilename)
{
	EFS_BEGIN

DWORD START, FINISH;
START = GetTickCount();
	INT nCount = m_listInstruction.GetCount();
	CMarcInstruction *pMarcInstruction;

	TCHAR szbuf[16384];
	FILE *fd = _tfopen(strFilename, _T("wb"));
	if (fd == NULL) return -2;		// output file를 작성할 수 없음
	
		/*UNCHANGE*/fputc(0xFF, fd);
		/*UNCHANGE*/fputc(0xFE, fd);
	
	POSITION pos;
	pos = m_listInstruction.GetHeadPosition();
	for(INT i = 0; i < nCount; i++) {
		pMarcInstruction = NULL;
		pMarcInstruction = (CMarcInstruction*)m_listInstruction.GetNext(pos);
		if (!pMarcInstruction) continue;
		szbuf[0] = '\0';
		InstructionList_InstructionEncodeFrame(szbuf, pMarcInstruction);
		_ftprintf(fd, _T("%s\r\n"), szbuf);
	}
	fclose(fd);
FINISH = GetTickCount();
TRACE(_T("=======> TO TEXT FILE : %d.%d sec\r\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	return 0;

	EFS_END

	return -1;
}

/***************************************************************************************************
Return values
0		:		S_OK
bigger than 0	: frame size
!! Warnning : buffer overflow!
****************************************************************************************************/
INT CMarcRelation::InstructionList_InstructionEncodeFrame(TCHAR *pBuf, CMarcInstruction *pMarcInstruction)
{
	EFS_BEGIN

	if (!pMarcInstruction) return -1;
	INT nFrameSize = 0;
	INT nSubSize;
	INT nPos = 0;

	TCHAR szSizeBuf[10];
	nPos = 9;
	// GroupName
	nSubSize = 0;
	
	//nSubSize = pMarcInstruction->m_strGroupName.GetLength();
	nSubSize = _tcslen(pMarcInstruction->m_pszGroupName);
	_stprintf(pBuf+nPos, _T("%09d"), nSubSize);
	nPos += 9;
	//_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_strGroupName.GetBuffer(0));
	_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_pszGroupName);
	nPos += nSubSize;

	// Operator
	nSubSize = 0;
	//nSubSize = pMarcInstruction->m_strOperator.GetLength();
	nSubSize = _tcslen(pMarcInstruction->m_pszOperator);
	_stprintf(pBuf+nPos, _T("%09d"), nSubSize);
	nPos += 9;
	//_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_strOperator.GetBuffer(0));
	_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_pszOperator);
	nPos += nSubSize;

	// Basic Param
	nSubSize = 0;
	//nSubSize = pMarcInstruction->m_strBasicParam.GetLength();
	nSubSize = _tcslen(pMarcInstruction->m_pszBasicParam);
	_stprintf(pBuf+nPos, _T("%09d"), nSubSize);
	nPos += 9;
	//_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_strBasicParam.GetBuffer(0));
	_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_pszBasicParam);
	nPos += nSubSize;

	// Param
	nSubSize = 0;
	//nSubSize = pMarcInstruction->m_strParam.GetLength();
	nSubSize = _tcslen(pMarcInstruction->m_pszParam);
	_stprintf(pBuf+nPos, _T("%09d"), nSubSize);
	nPos += 9;
	//_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_strParam.GetBuffer(0));
	_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_pszParam);
	nPos += nSubSize;

	// Desc
	nSubSize = 0;
	//nSubSize = pMarcInstruction->m_strDesc.GetLength();
	nSubSize = _tcslen(pMarcInstruction->m_pszDesc);

	_stprintf(pBuf+nPos, _T("%09d"), nSubSize);
	nPos += 9;
	//_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_strDesc.GetBuffer(0));
	_stprintf(pBuf+nPos, _T("%s"), pMarcInstruction->m_pszDesc);
	nPos += nSubSize;

	// Total Frame Size
	nFrameSize = nPos;
	_stprintf(szSizeBuf, _T("%09d"), nFrameSize);
	wmemcpy(pBuf, szSizeBuf, 9);
	pBuf[nFrameSize] = '\0';

	return 0;

	EFS_END

	return -1;
}

/***************************************************************************************************
Return values
0		:		S_OK
-3		:		can not open output file
****************************************************************************************************/
INT CMarcRelation::InstructionList_FromTextFile(CString strFilename)
{
	EFS_BEGIN

	TCHAR szbuf[16384];
DWORD START, FINISH;
START = GetTickCount();

	CStdioFile fd;
	if (fd.Open(strFilename, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return -3;

	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	//INT iTmp = fd.Open(strFilename, CStdioFile::modeRead | CStdioFile::shareDenyWrite);

	INT ids;
	while(TRUE) {
		if (fd.ReadString(szbuf, 16383) == NULL) break;
		CMarcInstruction *pMarcInstruction = new CMarcInstruction;
		if (pMarcInstruction == NULL) continue;
		ids = InstructionList_InstructionDecodeFrame(szbuf, pMarcInstruction);
		if (ids == 0) {
			m_listInstruction.AddTail((CObject*)pMarcInstruction);
		} else {
			if (pMarcInstruction) delete pMarcInstruction;
			pMarcInstruction = NULL;
		}
	}
	fd.Close();
FINISH = GetTickCount();
TRACE(_T("=======> FROM TEXT FILE : %d.%d sec\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	return 0;

	EFS_END

	return -1;
}

/***************************************************************************************************
Return values
0		:		S_OK
bigger than 0	: frame size
!! Warnning : buffer overflow!
****************************************************************************************************/
INT CMarcRelation::InstructionList_InstructionDecodeFrame(TCHAR *pBuf, CMarcInstruction *pMarcInstruction)
{
	EFS_BEGIN

	if (!pMarcInstruction) return -1;
	INT nFrameSize = 0;
	INT nSubSize;
	INT nPos = 0;

//	TCHAR szDataBuf[8192];

	nPos = 9;
	// GroupName
	wmemcpy(szSizeBuf, pBuf+nPos, 9);
	szSizeBuf[9] = '\0';
	nSubSize = _ttoi(szSizeBuf);
	nPos += 9;
	
	//pMarcInstruction->SetGroupName(pBuf + nPos, nSubSize);
	wmemcpy(pMarcInstruction->m_pszGroupName, pBuf+nPos, nSubSize);
	pMarcInstruction->m_pszGroupName[nSubSize] = '\0';
	//szDataBuf[nSubSize] = '\0';
	//pMarcInstruction->m_strGroupName.Format(_T("%s"), szDataBuf);
	
	nPos += nSubSize;

	// Operator
	wmemcpy(szSizeBuf, pBuf+nPos, 9);
	szSizeBuf[9] = '\0';
	nSubSize = _ttoi(szSizeBuf);
	nPos += 9;

	//pMarcInstruction->SetOperator(pBuf + nPos, nSubSize);
	wmemcpy(pMarcInstruction->m_pszOperator, pBuf+nPos, nSubSize);
	pMarcInstruction->m_pszOperator[nSubSize] = '\0';

	//wmemcpy(szDataBuf, pBuf+nPos, nSubSize);
	//szDataBuf[nSubSize] = '\0';
	//pMarcInstruction->m_strOperator.Format(_T("%s"), szDataBuf);
	
	nPos += nSubSize;

	// Basic Param
	wmemcpy(szSizeBuf, pBuf+nPos, 9);
	szSizeBuf[9] = '\0';
	nSubSize = _ttoi(szSizeBuf);
	nPos += 9;

	//pMarcInstruction->SetBasicParam(pBuf + nPos, nSubSize);
	wmemcpy(pMarcInstruction->m_pszBasicParam, pBuf+nPos, nSubSize);
	pMarcInstruction->m_pszBasicParam[nSubSize] = '\0';

	//wmemcpy(szDataBuf, pBuf+nPos, nSubSize);
	//szDataBuf[nSubSize] = '\0';
	//pMarcInstruction->m_strBasicParam.Format(_T("%s"), szDataBuf);
	
	nPos += nSubSize;

	// Param
	wmemcpy(szSizeBuf, pBuf+nPos, 9);
	szSizeBuf[9] = '\0';
	nSubSize = _ttoi(szSizeBuf);
	nPos += 9;
	
	//pMarcInstruction->SetParam(pBuf + nPos, nSubSize);
	wmemcpy(pMarcInstruction->m_pszParam, pBuf+nPos, nSubSize);
	pMarcInstruction->m_pszParam[nSubSize] = '\0';

	//wmemcpy(szDataBuf, pBuf+nPos, nSubSize);
	//szDataBuf[nSubSize] = '\0';
	//pMarcInstruction->m_strParam.Format(_T("%s"), szDataBuf);
	
	nPos += nSubSize;

	// Desc
	wmemcpy(szSizeBuf, pBuf+nPos, 9);
	szSizeBuf[9] = '\0';
	nSubSize = _ttoi(szSizeBuf);
	nPos += 9;

	//pMarcInstruction->SetDesc(pBuf + nPos, nSubSize);
	wmemcpy(pMarcInstruction->m_pszDesc, pBuf+nPos, nSubSize);
	pMarcInstruction->m_pszDesc[nSubSize] = '\0';
	//pMarcInstruction->m_pszDesc[0] = '\0';

	//wmemcpy(szDataBuf, pBuf+nPos, nSubSize);
	//szDataBuf[nSubSize] = '\0';
	//pMarcInstruction->m_strDesc.Format(_T("%s"), szDataBuf);
	
	nPos += nSubSize;

	// Total Frame Size
	/*
	nFrameSize = nPos;
	wmemcpy(szSizeBuf, pBuf, 9);
	if (_ttoi(szSizeBuf) == nFrameSize) return 0;
		else return -1;
	*/
	return 0;

	EFS_END

	return -1;
}

/***************************************************************************************************
Return values
0		:		S_OK
****************************************************************************************************/
INT CMarcRelation::InstructionList_AddInstruction(CString strGroupName, CString strOperator, CString strBasicParam, CString strParam, CString strDesc)
{
	EFS_BEGIN

	CMarcInstruction *pMarcInstruction = new CMarcInstruction;
	if (!pMarcInstruction) return -1;		// construct new _T("MarcInstruction") object fail.

	pMarcInstruction->SetGroupName(strGroupName.GetBuffer(0), strGroupName.GetLength());
	pMarcInstruction->SetOperator(strOperator.GetBuffer(0), strOperator.GetLength());
	pMarcInstruction->SetBasicParam(strBasicParam.GetBuffer(0), strBasicParam.GetLength());
	pMarcInstruction->SetParam(strParam.GetBuffer(0), strParam.GetLength());
	pMarcInstruction->SetDesc(strDesc.GetBuffer(0), strDesc.GetLength());

	//pMarcInstruction->m_strGroupName = strGroupName;
	//pMarcInstruction->m_strOperator = strOperator;
	//pMarcInstruction->m_strBasicParam = strBasicParam;
	//pMarcInstruction->m_strParam = strParam;
	//pMarcInstruction->m_strDesc = strDesc;

	m_listInstruction.AddTail((CObject*)pMarcInstruction);
	return 0;

	EFS_END

	return -1;
}

/***************************************************************************************************
Return values
0		:		S_OK
****************************************************************************************************/
INT CMarcRelation::InstructionList_AddInstruction(TCHAR *pszstrGroupName, TCHAR *pszstrOperator, TCHAR *pszstrBasicParam, TCHAR *pszstrParam, TCHAR *pszstrDesc)
{
	EFS_BEGIN

	CMarcInstruction *pMarcInstruction = new CMarcInstruction;
	if (!pMarcInstruction) return -1;		// construct new _T("MarcInstruction") object fail.

	pMarcInstruction->SetGroupName(pszstrGroupName, _tcslen(pszstrGroupName));
	pMarcInstruction->SetOperator(pszstrOperator, _tcslen(pszstrOperator));
	pMarcInstruction->SetBasicParam(pszstrBasicParam, _tcslen(pszstrBasicParam));
	pMarcInstruction->SetParam(pszstrParam, _tcslen(pszstrParam));
	pMarcInstruction->SetDesc(pszstrDesc, _tcslen(pszstrDesc));
	
	//pMarcInstruction->m_strGroupName.Format(_T("%s"), pszstrGroupName);
	//pMarcInstruction->m_strOperator.Format(_T("%s"), pszstrOperator);
	//pMarcInstruction->m_strBasicParam.Format(_T("%s"), pszstrBasicParam);
	//pMarcInstruction->m_strParam.Format(_T("%s"), pszstrParam);
	//pMarcInstruction->m_strDesc.Format(_T("%s"), pszstrDesc);

	m_listInstruction.AddTail((CObject*)pMarcInstruction);
	return 0;

	EFS_END

	return -1;
}
