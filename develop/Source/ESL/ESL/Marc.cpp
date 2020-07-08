// Marc.cpp: implementation of the CMarc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Marc.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarc::CMarc()
{
	//ZeroMemory(m_szLeader, 25);
	wmemset(m_szLeader, wchar_t(' '), 24);	
	wmemset(m_szLeader + 24, 0, 1);

	wmemcpy(m_szLeader + 10, _T("2"), 1);
	wmemcpy(m_szLeader + 11, _T("2"), 1);
	wmemcpy(m_szLeader + 20, _T("4500"), 4);	
	


	Init();
}

CMarc::~CMarc()
{
	FreeTagList();
}

VOID CMarc::Init()
{
	m_chRecordTerminator = RECORD_TERMINATOR;
	FreeTagList();
}

CMarc *CMarc::Clone()
{
	EFS_BEGIN

	CMarc *pMarc = new CMarc;
	pMarc->SetRecordTerminator(m_chRecordTerminator);
	pMarc->SetLeader(m_szLeader);

	POSITION pos = m_listTag.GetHeadPosition();
	while (pos != NULL)
	{
		CTag *pTag = (CTag*)m_listTag.GetNext(pos);
		if (pTag == NULL) continue;
		
		pMarc->AddTag(pTag->Clone());
	}

	return pMarc;

	EFS_END

	return NULL;
}

CMarc& CMarc::operator =(CMarc& rhs)
{
	if (this == &rhs)
		return *this;

	SetRecordTerminator(rhs.GetRecordTerminator());
	rhs.GetLeader(m_szLeader);

	/// Tag
	FreeTagList();

	POSITION pos = rhs.m_listTag.GetHeadPosition();
	while (pos)
	{
		CTag *pTag = (CTag*)rhs.m_listTag.GetNext(pos);
		if (pTag == NULL) continue;

		AddTag(pTag->Clone());
	}

	return *this;
}

INT CMarc::FreeTagList()
{
	EFS_BEGIN

	INT nListCount = m_listTag.GetCount();
	if (nListCount == 0) return 0;
	CTag *pTag;
	for (INT i = 0; i < nListCount; i++)
	{
		pTag = (CTag*)m_listTag.RemoveHead();
		if (pTag) delete pTag;
	}
	m_listTag.RemoveAll();
	return nListCount;

	EFS_END

	return -1;
}

//////////////////////////////////////////////////////////////////////
// CMarc Operations
INT CMarc::AddTag(CTag *pTag)
{
	EFS_BEGIN

	if (!pTag) 
	{
		m_strLastError = _T("pTag is null");
		return -1;
	}

	m_listTag.AddTail((CObject*)pTag);
	return 0;

	EFS_END

	return -1;
}

CTag *CMarc::FindTag(CString strTagCode)
{
	EFS_BEGIN
	POSITION pos = m_listTag.GetHeadPosition();
	
	while (pos != NULL)
	{
		CTag *pTag = (CTag*)m_listTag.GetNext(pos);
		if (pTag != NULL && pTag->GetTagCode() == strTagCode) 
			return pTag;
	}
	m_strLastError = _T("cannot find tag");
	return NULL;

	EFS_END

	return NULL;
}

CTag *CMarc::FindTag(CString strTagCode, POSITION &pos)
{
	EFS_BEGIN 

	if (pos == NULL)
	{
		pos = m_listTag.GetHeadPosition();
	}

	while (pos != NULL)
	{
		CTag *pTag = (CTag*)m_listTag.GetNext(pos);
		if (pTag != NULL && pTag->GetTagCode() == strTagCode) 
			return pTag;
	}

	m_strLastError = _T("cannot find tag");
	return NULL;

	EFS_END

	return NULL;
}

INT CMarc::GetItemData(CString strTagCode, CString &strItemData, CArray<CString, CString&> *pArray)
{
	EFS_BEGIN 

	if (pArray == NULL)
	{
		CTag* pTag = FindTag(strTagCode);
		if (pTag == NULL) 
		{
			m_strLastError = _T("mached tag does not exist");
			return -1;
		}

		strItemData = pTag->GetItemData();
		return 1;
	}
	else
	{
		POSITION pos = m_listTag.GetHeadPosition();
		while (pos != NULL)
		{
			CTag *pTag = (CTag*)m_listTag.GetNext(pos);
			if (pTag == NULL) continue;

			if (pTag->GetTagCode() == strTagCode)
				pArray->Add(pTag->GetItemData());
		}

		if (pArray->GetSize() > 0)
		{
			strItemData = pArray->GetAt(0);
			return pArray->GetSize();
		}
		else
		{
			m_strLastError = _T("mached tag does not exist");
			return -1;
		}
	}

	return -1;

	EFS_END

	return -1;
}

INT CMarc::GetItemData(CString strTagCode, CString strSubfieldCode, CString &strItemData, CArray<CString, CString&> *pArray /*=NULL*/, CArray<CString, CString&> *pIndArray /*= NULL*/)
{
	EFS_BEGIN 

	strSubfieldCode.Format(_T("%c%s"), SUBFIELD_CODE, strSubfieldCode);

	BOOL bSearchInd = FALSE;
	CString strInd;
	INT nFind = strSubfieldCode.Find('!');
	if (nFind > 0)
	{
		bSearchInd = TRUE;

		strInd =  strSubfieldCode.Mid(nFind + 1);
		strSubfieldCode = strSubfieldCode.Mid(0, nFind);
		
		if (strInd.GetLength() == 2)
		{
			
		}	
		else if (strInd.GetLength() == 1)
		{
			strInd += _T(" ");
		}
		else if (strInd.GetLength() == 0)
		{
			strInd += _T("  ");
		}

		strInd.Replace(_T("_"), _T(" "));
	}
	
	if (pArray == NULL)
	{
		POSITION posTag = m_listTag.GetHeadPosition();
		while (posTag != NULL)
		{
			CTag *pTag = (CTag*)m_listTag.GetNext(posTag);
			if (pTag == NULL) continue;
			if (pTag->GetTagCode() != strTagCode) continue;

			// IND
			if (bSearchInd)
				if (strInd != pTag->GetFirstIndicator() + pTag->GetSecondIndicator()) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				if (pSubfield->GetSubfieldCode() != strSubfieldCode) continue;

				strItemData = pSubfield->GetItemData();
				return 1;
			}
		}

		m_strLastError = _T("mached tag does not exist");
		return -1;
	}
	else
	{
		POSITION posTag = m_listTag.GetHeadPosition();
		while (posTag != NULL)
		{
			CTag *pTag = (CTag*)m_listTag.GetNext(posTag);
			if (pTag == NULL) continue;
			if (pTag->GetTagCode() != strTagCode) continue;
			
			// IND
			if (bSearchInd)
				if (strInd != pTag->GetFirstIndicator() + pTag->GetSecondIndicator()) continue;

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				if (pSubfield->GetSubfieldCode() != strSubfieldCode) continue;

				pArray->Add(pSubfield->GetItemData());
				if (pIndArray != NULL)
				{
					pIndArray->Add(pTag->GetFirstIndicator() + pTag->GetSecondIndicator());
				}
			}
		}

		if (pArray->GetSize() > 0)
		{
			strItemData = pArray->GetAt(0);
			return pArray->GetSize();
		}
		else
		{
			m_strLastError = _T("mached tag does not exist");
			return -1;
		}
	}
	
	return -1;

	EFS_END

	return -1;
}

INT CMarc::GetItemData(CString strTagCode, INT nStart, INT nEnd, CString &strItemData, CArray<CString, CString&> *pArray)
{
	EFS_BEGIN 

	if (pArray == NULL)
	{
		CTag* pTag = FindTag(strTagCode);
		if (!pTag)
		{
			m_strLastError = _T("mached tag does not exist");
			return -1;
		}
		
		strItemData = pTag->GetItemData();
		if (strItemData.GetLength() <= nEnd) 
		{
			strItemData.Empty();
			return -1;
		}
		
		strItemData = strItemData.Mid(nStart, nEnd - nStart + 1);
		return 1;
	}
	else
	{
		POSITION pos = m_listTag.GetHeadPosition();
		while (pos != NULL)
		{
			CTag *pTag = (CTag*)m_listTag.GetNext(pos);
			if (pTag == NULL) continue;
			if (pTag->GetTagCode() != strTagCode) continue;

			strItemData = pTag->GetItemData();
			if (strItemData.GetLength() <= nEnd) continue;

			pArray->Add(strItemData.Mid(nStart, nEnd - nStart + 1));
		}
		if (pArray->GetSize() > 0)
		{
			strItemData = pArray->GetAt(0);
			return pArray->GetSize();
		}
		else
		{
			m_strLastError = _T("mached tag does not exist");
			return -1;
		}
	}
	
	return -1;

	EFS_END

	return -1;
}

INT CMarc::SetItemData(CString strTagCode, CString strNewItemData, CString strOldItemData /*=_T("")*/, INT nSetMode /*= ADD_DEFAULT*/)
{
	EFS_BEGIN 

	if (strOldItemData.IsEmpty())
	{
		CTag* pTag = FindTag(strTagCode);
		if (pTag == NULL || nSetMode == ADD_TAG)
		{
			pTag = new CTag;
			if (pTag == NULL) 
			{
				m_strLastError = _T("memory alloc failed");
				return -1;
			}
			
			pTag->SetTagCode(strTagCode);
			
			if (AddTag(pTag) < 0)
			{
				m_strLastError = _T("add tag failed");
				delete pTag;
				return -1;
			}
		}

		pTag->SetItemData(strNewItemData);

		return 0;
	}
	else
	{
		INT n = 0;
		POSITION pos = m_listTag.GetHeadPosition();
		while (pos != NULL)
		{
			CTag *pTag = (CTag*)m_listTag.GetNext(pos);
			if (pTag == NULL) continue;
			if (pTag->GetTagCode() != strTagCode) continue;
			if (pTag->GetItemData() != strOldItemData) continue;

			n++;
			pTag->SetItemData(strNewItemData);		
		}

		if (n == 0) 
			return -1;
		else 
			return n;
	}
	

	return -1;

	EFS_END

	return -1;
}

INT CMarc::SetItemData(CString strTagCode, TCHAR *pchInd, CString strSubfieldCode, CString strNewItemData, CString strOldItemData /*=_T("")*/, INT nSetMode /*= ADD_DEFAULT*/)
{
	EFS_BEGIN 

	strSubfieldCode.Format(_T("%c%s"), SUBFIELD_CODE, strSubfieldCode);

	if (strOldItemData.IsEmpty())
	{
		CTag* pTag = FindTag(strTagCode);
		if (pTag == NULL || nSetMode == ADD_TAG)
		{   
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //2003.11.21 이학중 수정 진짜수정부분..
			//if(pTag->GetTagCode ()+pTag->GetFirstIndicator () == _T("0201") && pTag !=NULL)
			/*
			if ( pTag !=NULL )
			{
				if ( pTag->GetTagCode ()+pTag->GetFirstIndicator () == _T("0201") ) {
				    
				}
				
			}
			else if(pTag == NULL || nSetMode == ADD_TAG)
			{
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				pTag = new CTag;
				if (pTag == NULL) 
				{
					m_strLastError = _T("memory alloc failed");
					return -1;
				}

				pTag->SetTagCode(strTagCode);
				pTag->SetFirstIndicator(pchInd[0]);
				pTag->SetSecondIndicator(pchInd[1]);
				
				if (AddTag(pTag) < 0)
				{
					m_strLastError = _T("add tag failed");
					delete pTag;
					return -1;
				}
            }
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++
			*/
			// 기존 소스
			/*
			pTag = new CTag;
			if (pTag == NULL) 
			{
				m_strLastError = _T("memory alloc failed");
				return -1;
			}

			pTag->SetTagCode(strTagCode);
			pTag->SetFirstIndicator(pchInd[0]);
			pTag->SetSecondIndicator(pchInd[1]);
			
			if (AddTag(pTag) < 0)
			{
				m_strLastError = _T("add tag failed");
				delete pTag;
				return -1;
			}
			*/
			// 기존 소스 끝

			// 1222 - 440,020 수정.
			if ( pTag !=NULL ) {
				if ( (pTag->GetTagCode ()+pTag->GetFirstIndicator () == _T("0201") && pTag !=NULL) || strTagCode == _T("440") ) {
				}
				else {
					pTag = new CTag;
					if (pTag == NULL) 
					{
						m_strLastError = _T("memory alloc failed");
						return -1;
					}

					pTag->SetTagCode(strTagCode);
					pTag->SetFirstIndicator(pchInd[0]);
					pTag->SetSecondIndicator(pchInd[1]);
					
					if (AddTag(pTag) < 0)
					{
						m_strLastError = _T("add tag failed");
						delete pTag;
						return -1;
					}
				}
			}
			else {
				pTag = new CTag;
				if (pTag == NULL) 
				{
					m_strLastError = _T("memory alloc failed");
					return -1;
				}

				pTag->SetTagCode(strTagCode);
				pTag->SetFirstIndicator(pchInd[0]);
				pTag->SetSecondIndicator(pchInd[1]);
				
				if (AddTag(pTag) < 0)
				{
					m_strLastError = _T("add tag failed");
					delete pTag;
					return -1;
				}
			}	
			// 1222 - 440,020 수정끝.
		}

		CSubfield* pSubfield = pTag->FindSubfield(strSubfieldCode);
		/*if (pSubfield == NULL || nSetMode == ADD_SUBFIELD)
		{
			pSubfield = new CSubfield;
			if (pSubfield == NULL)
			{
				m_strLastError = _T("memory alloc failed");
				return -1;
			}

			pSubfield->SetSubfieldCode(strSubfieldCode);
			if (pTag->AddSubfield(pSubfield) < 0)
			{
				m_strLastError = _T("add subfield failed");
				delete pSubfield;
				return -1;
			}
		}
		pSubfield->SetItemData(strNewItemData);
		*/
		
		if (pSubfield == NULL)
		{
			pSubfield = new CSubfield;
			if (pSubfield == NULL)
			{
				m_strLastError = _T("memory alloc failed");
				return -1;
			}

			pSubfield->SetSubfieldCode(strSubfieldCode);			
			if (pTag->AddSubfield(pSubfield) < 0)
			{
				m_strLastError = _T("add subfield failed");
				delete pSubfield;
				return -1;
			}
		}
		else if (nSetMode == ADD_SUBFIELD)
		{
			if (!pSubfield->GetItemData().IsEmpty())
			{
				pSubfield = new CSubfield;
				if (pSubfield == NULL)
				{
					m_strLastError = _T("memory alloc failed");
					return -1;
				}

				pSubfield->SetSubfieldCode(strSubfieldCode);				
				if (pTag->AddSubfield(pSubfield) < 0)
				{
					m_strLastError = _T("add subfield failed");
					delete pSubfield;
					return -1;
				}

			}

		}

		pSubfield->SetItemData(strNewItemData);

		return 0;
	}
	else
	{
		INT n = 0;
		POSITION posTag = m_listTag.GetHeadPosition();
		while (posTag != NULL)
		{
			CTag *pTag = (CTag*)m_listTag.GetNext(posTag);
			if (pTag == NULL) continue;
			if (pTag->GetTagCode() != strTagCode) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				if (pSubfield->GetSubfieldCode() != strSubfieldCode) continue;
				if (pSubfield->GetItemData() != strOldItemData) continue;

				n++;
				pSubfield->SetItemData(strNewItemData);
			}
		}

		if (n == 0) 
			return -1;
		else
			return n;
	}

	return -1;

	EFS_END

	return -1;
	
}

INT CMarc::SetItemData(CString strTagCode, INT nStart, INT nEnd, CString strNewItemData, CString strOldItemData /*=_T("")*/, INT nSetMode /*= ADD_DEFAULT*/)
{
	EFS_BEGIN 

	// 크면 자르고 작으면 공백을 뒤에 붙인다.
	INT nItemDataLength = strNewItemData.GetLength();
	if (nItemDataLength < nEnd - nStart + 1) 
	{
		INT nAddCount = (nEnd - nStart + 1) - nItemDataLength;
		for (INT idx = 0; idx < nAddCount; idx++)
			strNewItemData += _T(" ");
	}
	else if (nItemDataLength > nEnd - nStart + 1)
	{
		strNewItemData = strNewItemData.Mid(0, nEnd - nStart + 1);
	}
	
	if (strOldItemData.IsEmpty())
	{
		CTag* pTag = FindTag(strTagCode);
		if (pTag == NULL || nSetMode == ADD_TAG)
		{
			pTag = new CTag;
			if (pTag == NULL)
			{
				m_strLastError = _T("memory alloc failed");
				return -1;
			}
			pTag->SetTagCode(strTagCode);
			
			if (AddTag(pTag) < 0)
			{
				m_strLastError = _T("add tag failed");
				delete pTag;
				return -1;
			}
		}
			
		CString strOriginData = pTag->GetItemData();

		// add space need...
		INT nDiff = nEnd - (strOriginData.GetLength() - 1);
		if (nDiff > 0)
		{
			for (INT i = 0; i < nDiff; i++)
			{
				strOriginData += _T(" ");
			}
		}

		// set data	
		for (INT i = nStart; i <= nEnd; i++)
		{
			strOriginData.SetAt(i, strNewItemData.GetAt(i - nStart));
		}

		pTag->SetItemData(strOriginData);

		return 0;
	}
	else
	{
		INT n = 0;
		POSITION pos = m_listTag.GetHeadPosition();
		while (pos != NULL)
		{
			CTag *pTag = (CTag*)m_listTag.GetNext(pos);
			if (pTag == NULL) continue;
			if (pTag->GetTagCode() != strTagCode) continue;
			CString strOriginData = pTag->GetItemData();
			if (strOriginData.Mid(nStart, nEnd - nStart + 1) != strOldItemData) continue;

			n++;
			for (INT i = nStart; i <= nEnd; i++)
			{
				strOriginData.SetAt(i, strNewItemData.GetAt(i - nStart));
			}

			pTag->SetItemData(strOriginData);
		}

		if (n == 0)
			return -1;
		else
			return n;
	}

	return -1;

	EFS_END

	return -1;
}

INT CMarc::SetItemDataWithFunc(CString strTagCode, TCHAR *pchInd, CString strSubfieldCode, CString strNewItemData, CString strPunc, CString strOldItemData /*=_T("")*/, INT nSetMode /*= ADD_DEFAULT*/)
{
	EFS_BEGIN 

	strSubfieldCode.Format(_T("%c%s"), SUBFIELD_CODE, strSubfieldCode);

	if (strOldItemData.IsEmpty())
	{
		CTag* pTag = FindTag(strTagCode);
		if (pTag == NULL || nSetMode == ADD_TAG)
		{   
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //2003.11.21 이학중 수정 진짜수정부분..
			//if(pTag->GetTagCode ()+pTag->GetFirstIndicator () == _T("0201") && pTag !=NULL)
			/*
			if ( pTag !=NULL )
			{
				if ( pTag->GetTagCode ()+pTag->GetFirstIndicator () == _T("0201") ) {
				    
				}
				
			}
			else if(pTag == NULL || nSetMode == ADD_TAG)
			{
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				pTag = new CTag;
				if (pTag == NULL) 
				{
					m_strLastError = _T("memory alloc failed");
					return -1;
				}

				pTag->SetTagCode(strTagCode);
				pTag->SetFirstIndicator(pchInd[0]);
				pTag->SetSecondIndicator(pchInd[1]);
				
				if (AddTag(pTag) < 0)
				{
					m_strLastError = _T("add tag failed");
					delete pTag;
					return -1;
				}
            }
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++
			*/
			// 기존 소스
			/*
			pTag = new CTag;
			if (pTag == NULL) 
			{
				m_strLastError = _T("memory alloc failed");
				return -1;
			}

			pTag->SetTagCode(strTagCode);
			pTag->SetFirstIndicator(pchInd[0]);
			pTag->SetSecondIndicator(pchInd[1]);
			
			if (AddTag(pTag) < 0)
			{
				m_strLastError = _T("add tag failed");
				delete pTag;
				return -1;
			}
			*/
			// 기존 소스 끝

			// 1222 - 440,020 수정.
			if ( pTag !=NULL ) {
				if ( (pTag->GetTagCode ()+pTag->GetFirstIndicator () == _T("0201") && pTag !=NULL) || strTagCode == _T("440") ) {
				}
				else {
					pTag = new CTag;
					if (pTag == NULL) 
					{
						m_strLastError = _T("memory alloc failed");
						return -1;
					}

					pTag->SetTagCode(strTagCode);
					pTag->SetFirstIndicator(pchInd[0]);
					pTag->SetSecondIndicator(pchInd[1]);
					
					if (AddTag(pTag) < 0)
					{
						m_strLastError = _T("add tag failed");
						delete pTag;
						return -1;
					}
				}
			}
			else {
				pTag = new CTag;
				if (pTag == NULL) 
				{
					m_strLastError = _T("memory alloc failed");
					return -1;
				}

				pTag->SetTagCode(strTagCode);
				pTag->SetFirstIndicator(pchInd[0]);
				pTag->SetSecondIndicator(pchInd[1]);
				
				if (AddTag(pTag) < 0)
				{
					m_strLastError = _T("add tag failed");
					delete pTag;
					return -1;
				}
			}	
			// 1222 - 440,020 수정끝.
		}

		CSubfield* pSubfield = pTag->FindSubfield(strSubfieldCode);
		/*if (pSubfield == NULL || nSetMode == ADD_SUBFIELD)
		{
			pSubfield = new CSubfield;
			if (pSubfield == NULL)
			{
				m_strLastError = _T("memory alloc failed");
				return -1;
			}

			pSubfield->SetSubfieldCode(strSubfieldCode);
			if (pTag->AddSubfield(pSubfield) < 0)
			{
				m_strLastError = _T("add subfield failed");
				delete pSubfield;
				return -1;
			}
		}
		pSubfield->SetItemData(strNewItemData);
		*/
		
		if (pSubfield == NULL)
		{
			pSubfield = new CSubfield;
			if (pSubfield == NULL)
			{
				m_strLastError = _T("memory alloc failed");
				return -1;
			}

			pSubfield->SetSubfieldCode(strSubfieldCode);			
			if (pTag->AddSubfield(pSubfield) < 0)
			{
				m_strLastError = _T("add subfield failed");
				delete pSubfield;
				return -1;
			}
		}
		else if (nSetMode == ADD_SUBFIELD)
		{
			if (!pSubfield->GetItemData().IsEmpty())
			{
				pSubfield = new CSubfield;
				if (pSubfield == NULL)
				{
					m_strLastError = _T("memory alloc failed");
					return -1;
				}

				pSubfield->SetSubfieldCode(strSubfieldCode);				
				if (pTag->AddSubfield(pSubfield) < 0)
				{
					m_strLastError = _T("add subfield failed");
					delete pSubfield;
					return -1;
				}

			}

		}

		pSubfield->SetItemData(strNewItemData);
		pSubfield->SetPunctuationMark(strPunc);

		return 0;
	}
	else
	{
		INT n = 0;
		POSITION posTag = m_listTag.GetHeadPosition();
		while (posTag != NULL)
		{
			CTag *pTag = (CTag*)m_listTag.GetNext(posTag);
			if (pTag == NULL) continue;
			if (pTag->GetTagCode() != strTagCode) continue;
			
			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				if (pSubfield->GetSubfieldCode() != strSubfieldCode) continue;
				if (pSubfield->GetItemData() != strOldItemData) continue;

				n++;
				pSubfield->SetItemData(strNewItemData);
			}
		}

		if (n == 0) 
			return -1;
		else
			return n;
	}

	return -1;

	EFS_END

	return -1;
	
}

INT CMarc::DeleteItemData(CString strTagCode, CString strItemData)
{
	EFS_BEGIN 

	INT n = 0;
	POSITION pos = m_listTag.GetTailPosition();
	while (pos != NULL)
	{
		POSITION posOld = pos;
		CTag *pTag = (CTag*)m_listTag.GetPrev(pos);
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() != strTagCode) continue;

		if (!strItemData.IsEmpty())
		{
			if (strItemData != pTag->GetItemData()) continue;
		}
		
		delete pTag;
		pTag = NULL;
		m_listTag.RemoveAt(posOld);
		
		n++;
	}

	if (n == 0)
	{
		m_strLastError = _T("matched tagcode is not exist");
		return -1;
	}
	else
		return n;

	EFS_END

	return -1;
}

INT CMarc::DeleteItemData(CString strTagCode, CString strSubfieldCode, CString strItemData)
{
	EFS_BEGIN 

	INT n = 0;
	strSubfieldCode.Format(_T("%c%s"), SUBFIELD_CODE, strSubfieldCode);

	BOOL bSearchInd = FALSE;
	CString strInd;
	INT nFind = strSubfieldCode.Find('!');
	if (nFind > 0)
	{
		strSubfieldCode = strSubfieldCode.Mid(0, nFind);
		/*
		bSearchInd = TRUE;

		strSubfieldCode = strSubfieldCode.Mid(0, nFind);
		strInd= strSubfieldCode.Mid(nFind + 1);
		if (strInd.GetLength() == 2)
		{
			
		}	
		else if (strInd.GetLength() == 1)
		{
			strInd += _T(" ");
		}
		else if (strInd.GetLength() == 0)
		{
			strInd += _T("  ");
		}
		*/
	}
	
	POSITION pos = m_listTag.GetTailPosition();
	while (pos != NULL)
	{
		CTag *pTag = (CTag*)m_listTag.GetPrev(pos);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() != strTagCode) continue;
		// IND
		if (bSearchInd)
			if (strInd != pTag->GetFirstIndicator() + pTag->GetSecondIndicator()) continue;

		POSITION posSubfield = pTag->m_listSubfield.GetTailPosition();
		while (posSubfield != NULL)
		{
			POSITION posOld = posSubfield;
			CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetPrev(posSubfield);
			if (pSubfield == NULL) continue;

			if (pSubfield->GetSubfieldCode() != strSubfieldCode) continue;

			if (!strItemData.IsEmpty())
			{
				if (strItemData != pSubfield->GetItemData()) continue;
			}

			delete pSubfield;
			pSubfield = NULL;
			pTag->m_listSubfield.RemoveAt(posOld);
			n++;
		}
	}
	
	if (n == 0)
	{
		m_strLastError = _T("matched tagcode and subfield code is not exist");
		return -1;
	}
	else
		return n;

	EFS_END

	return -1;
}

INT CMarc::DeleteItemData(CString strTagCode, INT nStart, INT nEnd, CString strItemData)
{
	EFS_BEGIN 

	if (!strItemData.IsEmpty() && strItemData.GetLength() != nEnd - nStart + 1) 
	{
		m_strLastError = _T("data length is invalid");
		return -1;
	}

	INT ids = 0;
	INT n = 0;
	CString strOriginData;
	CArray<CString, CString&> arrayItem;
	POSITION pos = m_listTag.GetHeadPosition();
	while (pos != NULL)
	{
		arrayItem.RemoveAll();
		CTag *pTag = (CTag*)m_listTag.GetNext(pos);
		if (pTag == NULL) continue;
		if (pTag->GetTagCode() != strTagCode) continue;

		ids = GetItemData(strTagCode, nStart, nEnd, strOriginData, &arrayItem);
		if (ids < 0) continue;

		if (!strItemData.IsEmpty())
		{
			if (ids == 1)
			{
				if (strItemData != strOriginData) continue;
			}
			else
			{
				BOOL bIsFind = FALSE;
				for (INT i = 0; i < ids; i++)
				{
					if (strItemData == strOriginData)
					{
						bIsFind = TRUE;
						break;
					}
				}

				if (!bIsFind) continue;
			}
		}		

		INT nCount = strOriginData.GetLength();
		for (INT i = 0; i < nCount; i++)
		{
			strOriginData.SetAt(i, ' ');
		}
		
		ids = SetItemData(strTagCode, nStart, nEnd, strOriginData, strItemData);
		if (ids < 0) continue;

		n++;
	}

	if (n == 0)
	{
		m_strLastError = _T("matched tagcode is not exist");
		return -1;
	}
	else
		return n;

	EFS_END

	return -1;
}

INT CMarc::GetField(CString strTagCode, CString &strField, CArray<CString, CString&> *pArray /*= NULL*/)
{
	EFS_BEGIN 

	CTag *pTag = NULL;
	if (pArray == NULL)
	{
		pTag = FindTag(strTagCode);
		if (pTag == NULL) return -1;

		POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
		
		strField += pTag->GetFirstIndicator();
		strField += pTag->GetSecondIndicator();

		while (posSubfield)
		{
			CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
			if (pSubfield == NULL) continue;

			strField += pSubfield->GetPunctuationMark();
			strField += pSubfield->GetSubfieldCode();
			strField += pSubfield->GetItemData();
		}

		strField += pTag->GetFieldTerminator();

		if (strField.IsEmpty()) 
			return -1;
		else 
			return 0;
	}
	else
	{
		CString strTempField;
		POSITION posTag = m_listTag.GetHeadPosition();
		while (posTag)
		{
			strTempField.Empty();
			CTag *pTag = (CTag*)m_listTag.GetNext(posTag);
			if (pTag == NULL) continue;
			if (pTag->GetTagCode() != strTagCode) continue;

			strTempField += pTag->GetFirstIndicator();
			strTempField += pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;
				
				strTempField += pSubfield->GetPunctuationMark();
				strTempField += pSubfield->GetSubfieldCode();
				strTempField += pSubfield->GetItemData();
			}

			strTempField += pTag->GetFieldTerminator();

			pArray->Add(strTempField);
		}

		if (pArray->GetSize() == 0) return -1;
		strField = pArray->GetAt(0);
		if (strField.IsEmpty())
			return -1;
		else
			return 0;
	}

	return -1;

	EFS_END

	return -1;

}


INT CMarc::DeleteField(CString strTagCode, CString strTagData /*= _T("")*/)
{
	EFS_BEGIN 

	if (strTagData.IsEmpty())
	{
		POSITION pos = m_listTag.GetTailPosition();
		while (pos != NULL)
		{
			POSITION posOld = pos;
			CTag *pTag = (CTag*)m_listTag.GetPrev(pos);
			if (pTag == NULL) continue;

			if (pTag->GetTagCode() == strTagCode)
			{
				delete pTag;
				pTag = NULL;

				m_listTag.RemoveAt(posOld);
			}
		}
	}
	else
	{
		CString strOriginTagData;
		POSITION pos = m_listTag.GetTailPosition();
		while (pos != NULL)
		{
			POSITION posOld = pos;
			strOriginTagData.Empty();
			CTag *pTag = (CTag*)m_listTag.GetPrev(pos);
			if (pTag == NULL) continue;
			if (pTag->GetTagCode() != strTagCode) continue;
			
			// check data
			strOriginTagData += pTag->GetFirstIndicator();
			strOriginTagData += pTag->GetSecondIndicator();

			POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield != NULL)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (pSubfield == NULL) continue;

				strOriginTagData += pSubfield->GetPunctuationMark();
				strOriginTagData += pSubfield->GetSubfieldCode();
				strOriginTagData += pSubfield->GetItemData();
			}

			strOriginTagData += pTag->GetFieldTerminator();

			if (strTagData == strOriginTagData)
			{
				delete pTag;
				pTag = NULL;

				m_listTag.RemoveAt(posOld);
			}
		}
	}

	return 0;

	EFS_END

	return -1;
}

INT CMarc::DeleteField(CString strTagCode, TCHAR chFirstIndicator, TCHAR chSecondIndicator)
{
	EFS_BEGIN 

	if (strTagCode.GetLength() < 3) return -1;

	POSITION pos = m_listTag.GetTailPosition();
	while (pos != NULL)
	{
		POSITION posOld = pos;
		CTag *pTag = (CTag*)m_listTag.GetPrev(pos);
		if (pTag == NULL) continue;

		if (pTag->GetTagCode() != strTagCode) continue;
		if (pTag->GetFirstIndicator() != chFirstIndicator) continue;
		if (pTag->GetSecondIndicator() != chSecondIndicator) continue;

		delete pTag;
		pTag = NULL;

		m_listTag.RemoveAt(posOld);
	}

	return 0;

	EFS_END

	return -1;
}

INT CMarc::DeleteEmptyField()
{
	EFS_BEGIN 

	POSITION pos = m_listTag.GetTailPosition();
	while (pos != NULL)
	{
		POSITION posOld = pos;
		CTag *pTag = (CTag*)m_listTag.GetPrev(pos);
		if (pTag == NULL) continue;

		if (pTag->GetItemData().IsEmpty() && pTag->m_listSubfield.GetCount() == 0)
		{
			delete pTag;
			pTag = NULL;

			m_listTag.RemoveAt(posOld);
		}
	}

	return 0;

	EFS_END

	return -1;
}

INT CMarc::DeleteEmptySubfield()
{
	EFS_BEGIN 

	POSITION posTag = NULL;
	POSITION posSubfield = NULL;
	POSITION posOld = NULL;

	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	

	posTag = m_listTag.GetTailPosition();
	while (posTag != NULL)
	{
		pTag = (CTag*)m_listTag.GetPrev(posTag);
		if (pTag == NULL) continue;

		posSubfield = pTag->m_listSubfield.GetTailPosition();
		while (posSubfield != NULL)
		{
			posOld = posSubfield;
			pSubfield = (CSubfield*)pTag->m_listSubfield.GetPrev(posSubfield);
			if (pSubfield == NULL) continue;

			if (pSubfield->GetItemData().IsEmpty())
			{
				delete pSubfield;
				pSubfield = NULL;
				pTag->m_listSubfield.RemoveAt(posOld);
			}
		}
	}

	return 0;

	EFS_END

	return -1;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2003.09.09 수정전 
//INT CMarc::SortMarc(INT nOrder /*= SORT_ASC*/, CMapStringToOb *pMap /*= NULL*/)
//{
//	return SortMarc_Bubble(nOrder, pMap);
//}
// 수정후 
INT CMarc::SortMarc(INT nOrder /*= SORT_ASC*/, CMapStringToOb *pMap /*= NULL*/,BOOL bNewSort /*= FALSE*/)
{
 	return SortMarc_Bubble(nOrder, pMap ,bNewSort);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//2003.09.09 수정전 

//INT CMarc::SortMarc_Bubble(INT nOrder /*=SORT_ASC*/, CMapStringToOb *pMap /*= NULL*/)
/*
{
	EFS_BEGIN 

	// 먼저 Subfield를 정렬한다.
	CStringArray *pArr = NULL;
	CTag *pTag = NULL;
	POSITION pos = m_listTag.GetHeadPosition();
	while (pos)
	{
		pTag = (CTag*)m_listTag.GetNext(pos);
		if (pTag == NULL) continue;

		// 특수정렬에 속해 있으면 특수정렬을 한다.
		if (pMap != NULL && pMap->Lookup(pTag->GetTagCode(), (CObject*&)pArr))
			pTag->SortSubfield_Special(pMap);
		// 049와 505는 Subfield를 정렬하지 않는다.
		else if (pTag->GetTagCode() != _T("049") && pTag->GetTagCode() != _T("505") && pTag->GetTagCode() != _T("440") && pTag->GetTagCode() != _T("245"))
			pTag->SortSubfield(nOrder);
	}

	INT nCount = m_listTag.GetCount();
	INT nCount2 = nCount;
	for (INT i = 0; i < nCount; i++)
	{
		for (INT j = 0; j < nCount2 - 1; j++)
		{
			CTag *pTagMaster = (CTag*)m_listTag.GetAt(m_listTag.FindIndex(j));
			if (pTagMaster == NULL) continue;

			CTag *pTagSlave = (CTag*)m_listTag.GetAt(m_listTag.FindIndex(j + 1));
			if (pTagSlave == NULL) continue;

			// asc --> 049 tag is biggest
			if (nOrder == SORT_ASC)
			{
				if (pTagSlave->GetTagCode() != _T("049")) 
				{
					if (pTagMaster->GetTagCode() == _T("049") || 
						pTagMaster->GetTagCode() + pTagMaster->GetFirstIndicator() + pTagMaster->GetSecondIndicator() > 
						pTagSlave->GetTagCode() + pTagSlave->GetFirstIndicator() + pTagSlave->GetSecondIndicator())
					{
						// Swap
						m_listTag.SetAt(m_listTag.FindIndex(j), (CObject*)pTagSlave);
						m_listTag.SetAt(m_listTag.FindIndex(j + 1), (CObject*)pTagMaster);

					}
				}
			}
			// desc
			else if (nOrder == SORT_DESC)
			{
				if (pTagMaster->GetTagCode() < pTagSlave->GetTagCode())
				{
					m_listTag.SetAt(m_listTag.FindIndex(i), (CObject*)pTagSlave);
					m_listTag.SetAt(m_listTag.FindIndex(j + 1), (CObject*)pTagMaster);
				}
			}
		}

		nCount2 --;
	}

	return 0;

	EFS_END

	return -1;
}
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
INT CMarc::SortMarc_Bubble(INT nOrder /*=SORT_ASC*/, CMapStringToOb *pMap /*= NULL*/,BOOL bNewSort /*= FALSE*/)
{
	EFS_BEGIN 

	// 먼저 Subfield를 정렬한다.
	CStringArray *pArr = NULL;
	CTag *pTag = NULL;
	POSITION pos = m_listTag.GetHeadPosition();
	while (pos)
	{
		pTag = (CTag*)m_listTag.GetNext(pos);
		if (pTag == NULL) continue;

		// 특수정렬에 속해 있으면 특수정렬을 한다.
		if (pMap != NULL && pMap->Lookup(pTag->GetTagCode(), (CObject*&)pArr))
			pTag->SortSubfield_Special(pMap);
		// 049와 505는 Subfield를 정렬하지 않는다.
		// 17/08/11 김혜영 : 목록완성의 권별정보가 실행되면 260$b태그가 뒤로 이동되는 현상 수정
		// 260태그의 정렬을 막으면 권별정보 보기 뿐 아니라 희망자료관리에서 신규 입력된 정보도 정렬이 되지 않으며 문제가 발생함
		// 발행자가 여러개 입력될 수 있어서 delete하고 다시 insert하면서 위치가 변경되는 것이고 정렬을 막음으로써 b식별기호가 뒤쪽으로 이동되는 현상
		// 위의 경우 외에 관련된 부분을 모두 고려하여야 하므로 17/06/07에 작업한 260태그에 정렬을 막은 작업은 원복하고 추후에 다시 검토하여 진행하도록 하겠음
/*//BEFORE1 -----------------------------------------------------------------------------
		else if (pTag->GetTagCode() != _T("049") && pTag->GetTagCode() != _T("505") && pTag->GetTagCode() != _T("440") && pTag->GetTagCode() != _T("245"))
////BEFORE2 -----------------------------------------------------------------------------
		// 17/06/07 김혜영 : 목록완성에서 260태그 입력시 식별기호가 정렬되는 현상 수정
		else if (pTag->GetTagCode() != _T("049") && pTag->GetTagCode() != _T("505") && pTag->GetTagCode() != _T("440") && pTag->GetTagCode() != _T("245") && pTag->GetTagCode() != _T("260"))
*///AFTER----------------------------------------------------------------------------------	
		else if (pTag->GetTagCode() != _T("049") && pTag->GetTagCode() != _T("505") && pTag->GetTagCode() != _T("440") && pTag->GetTagCode() != _T("245"))
/*//END-----------------------------------------------------------------------------------*/
			pTag->SortSubfield(nOrder);
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.09.08 이학중 수정
        /*
		if(bNewSort == FALSE)
		{
		    if (pTag->GetTagCode() != _T("049") && pTag->GetTagCode() != _T("505") && pTag->GetTagCode() != _T("440") && pTag->GetTagCode() != _T("245"))
			pTag->SortSubfield(nOrder);
         
		}
		else 
		{
            if ( pTag->GetTagCode() != _T("505") && pTag->GetTagCode() != _T("440") && pTag->GetTagCode() != _T("245"))
			pTag->SortSubfield(nOrder);
		}
		//++++++++++++++++++++++++++++++++++
		*/
	}

	INT nCount = m_listTag.GetCount();
	INT nCount2 = nCount;
	for (INT i = 0; i < nCount; i++)
	{
		for (INT j = 0; j < nCount2 - 1; j++)
		{
			CTag *pTagMaster = (CTag*)m_listTag.GetAt(m_listTag.FindIndex(j));
			if (pTagMaster == NULL) continue;

			CTag *pTagSlave = (CTag*)m_listTag.GetAt(m_listTag.FindIndex(j + 1));
			if (pTagSlave == NULL) continue;

			// asc --> 049 tag is biggest
			if (nOrder == SORT_ASC)
			{
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//2003.09.08 이학중 수정
				if(bNewSort == FALSE)
				{
					if (pTagSlave->GetTagCode() != _T("049")) 
					{
						if (pTagMaster->GetTagCode() == _T("049") || 
							pTagMaster->GetTagCode() + pTagMaster->GetFirstIndicator() + pTagMaster->GetSecondIndicator() > 
							pTagSlave->GetTagCode() + pTagSlave->GetFirstIndicator() + pTagSlave->GetSecondIndicator())
						//if (pTagMaster->GetTagCode() + pTagMaster->GetFirstIndicator() + pTagMaster->GetSecondIndicator() > 
						//	pTagSlave->GetTagCode() + pTagSlave->GetFirstIndicator() + pTagSlave->GetSecondIndicator())
						{
							// Swap
							m_listTag.SetAt(m_listTag.FindIndex(j), (CObject*)pTagSlave);
							m_listTag.SetAt(m_listTag.FindIndex(j + 1), (CObject*)pTagMaster);

						}
					}
                } 
				else
				{
                        if (pTagMaster->GetTagCode() + pTagMaster->GetFirstIndicator() + pTagMaster->GetSecondIndicator() > 
							pTagSlave->GetTagCode() + pTagSlave->GetFirstIndicator() + pTagSlave->GetSecondIndicator())
						{
							// Swap
							m_listTag.SetAt(m_listTag.FindIndex(j), (CObject*)pTagSlave);
							m_listTag.SetAt(m_listTag.FindIndex(j + 1), (CObject*)pTagMaster);

						}

				}
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			}
			// desc
			else if (nOrder == SORT_DESC)
			{
				if (pTagMaster->GetTagCode() < pTagSlave->GetTagCode())
				{
					m_listTag.SetAt(m_listTag.FindIndex(i), (CObject*)pTagSlave);
					m_listTag.SetAt(m_listTag.FindIndex(j + 1), (CObject*)pTagMaster);
				}
			}
		}

		nCount2 --;
	}

	return 0;

	EFS_END

	return -1;
}



INT CMarc::SortMarc_Selection(INT nOrder /*= SORT_ASC*/)
{
	EFS_BEGIN 

	INT nCount = m_listTag.GetCount();
	for (INT i = 0; i < nCount - 1; i++)
	{
		CTag *pTagMaster = (CTag*)m_listTag.GetAt(m_listTag.FindIndex(i));
		if (pTagMaster == NULL) continue;

		// 049 field does not subfield sorting
		if (pTagMaster->GetTagCode() != _T("049") || pTagMaster->GetTagCode() != _T("260"))
			pTagMaster->SortSubfield(nOrder);

		for (INT j = i + 1; j < nCount; j++)
		{
			CTag *pTagSlave = (CTag*)m_listTag.GetAt(m_listTag.FindIndex(j));
			if (pTagSlave == NULL) continue;
			
			// asc --> 049 tag is biggest
			if (nOrder == SORT_ASC)
			{
				if (pTagSlave->GetTagCode() != _T("049"))
				{
					if (pTagMaster->GetTagCode() == _T("049") || pTagMaster->GetTagCode() > pTagSlave->GetTagCode())
					{
						m_listTag.SetAt(m_listTag.FindIndex(i), (CObject*)pTagSlave);
						m_listTag.SetAt(m_listTag.FindIndex(j), (CObject*)pTagMaster);

						pTagMaster = pTagSlave;
					}
				}
			}
			// desc
			else if (nOrder == SORT_DESC)
			{
				if (pTagMaster->GetTagCode() < pTagSlave->GetTagCode())
				{
					m_listTag.SetAt(m_listTag.FindIndex(i), (CObject*)pTagSlave);
					m_listTag.SetAt(m_listTag.FindIndex(j), (CObject*)pTagMaster);

					pTagMaster = pTagSlave;
				}
			}
		}
	}

	return 0;

	EFS_END

	return -1;
}

VOID CMarc::GetLeader(CString &strLeader) 	
{
	strLeader.Format(_T("%s"), m_szLeader);
}
VOID CMarc::GetLeader(TCHAR *szLeader)			
{
	_tcscpy(szLeader, m_szLeader);
}
VOID CMarc::SetLeader(TCHAR *szLeader)				
{
	_tcscpy(m_szLeader, szLeader);
	m_szLeader[25] = 0;
}
	