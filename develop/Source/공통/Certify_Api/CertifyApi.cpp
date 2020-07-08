// CertifyApi.cpp: implementation of the CCertifyApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CertifyApi.h"
#include <Winsock2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCertifyFrameNode::CCertifyFrameNode()
{

}
CCertifyFrameNode::CCertifyFrameNode(const CString& strNodeName)
{
	m_strNodeName = strNodeName;
}

CCertifyFrameNode::~CCertifyFrameNode()
{
	CCertifyFrameNode* pNode = NULL;
	while(m_ChildNodeList.GetCount())
	{
		pNode = m_ChildNodeList.RemoveTail();
		delete pNode;
	}
}

CString CCertifyFrameNode::GetNodeName()
{
	return m_strNodeName;
}

CString CCertifyFrameNode::GetNodeValue()
{
	return m_strNodeValue;
}

CCertifyFrameNode* CCertifyFrameNode::CreateNode(const CString& strNodeName)
{
	CCertifyFrameNode* pNode = new CCertifyFrameNode(strNodeName);

	return pNode;
}
CString CCertifyFrameNode::replaceSpecialChars(const CString& strData, const BOOL bEncode)
{	
	CString strOutput = strData;

	if(bEncode)
	{
		strOutput.Replace(	_T("&"),		_T("&amp;")		);
		strOutput.Replace(	_T("\""),		_T("&quot;")	);
		strOutput.Replace(	_T("''"),		_T("&apos;")	);
		strOutput.Replace(	_T("<"),		_T("&lt;")		);
		strOutput.Replace(	_T(">"),		_T("&gt;")		);		
	}
	else
	{
		strOutput.Replace(	_T("&quot;"),	_T("\"")		);
		strOutput.Replace(	_T("&apos;"),	_T("''")			);
		strOutput.Replace(	_T("&lt;"),		_T("<")			);
		strOutput.Replace(	_T("&gt;"),		_T(">")			);
		strOutput.Replace(	_T("&amp;"),	_T("&")			);
	}

	return strOutput;
}

void CCertifyFrameNode::GetFrame(CString& strFrame, INT nDeapth)
{	
	CString strAlias, strValue, strAttributes, strAttribute;
	POSITION pos = m_Attributes.GetStartPosition();
	while(NULL != pos)
	{
		m_Attributes.GetNextAssoc(pos, strAlias, strValue);		
		strAttribute.Format(_T(" %s=\"%s\""), replaceSpecialChars(strAlias, TRUE), replaceSpecialChars(strValue, FALSE));
		strAttributes += strAttribute;
	}

	CString strTab;	
	for(INT nCnt = 0; nCnt < nDeapth; nCnt++)
	{
		strTab += _T("\t");
	}
// 	if(FALSE == m_strNodeValue.IsEmpty())
	if(0 == m_ChildNodeList.GetCount())
	{
		strFrame.Format(
			_T("%s<%s%s>%s</%s>"), 
			strTab, 
			replaceSpecialChars(m_strNodeName, TRUE), 
			strAttributes, 
			replaceSpecialChars(m_strNodeValue, TRUE), 
			replaceSpecialChars(m_strNodeName, TRUE)
			);
	}
	else
	{
		strFrame.Format(
			_T("%s<%s%s>"),
			strTab,
			replaceSpecialChars(m_strNodeName, TRUE), 
			strAttributes
			);
		
		CString strChildFrame;
		CCertifyFrameNode* pNode = NULL;
		POSITION pos2 = m_ChildNodeList.GetHeadPosition();
		while(NULL != pos2)
		{
			strFrame += _T("\r\n");
			pNode = m_ChildNodeList.GetNext(pos2);
			pNode->GetFrame(strChildFrame, nDeapth+1);
			strFrame += strChildFrame;
		}
		strFrame += _T("\r\n") + strTab + _T("</") + replaceSpecialChars(m_strNodeName, TRUE) + _T(">");
	}

}

BOOL CCertifyFrameNode::parseFrame(CString& strParseFrame)
{
	strParseFrame.TrimLeft();	
	
	// 시작태그를 찾는다.
	INT nPosS1 = strParseFrame.Find(_T("<")); 
	INT nPosS2 = strParseFrame.Find(_T(">"));	
	INT nPosS3 = strParseFrame.Find(_T("</"));
	if(0 != nPosS1 || -1 == nPosS2 || 0 >= nPosS3)
	{
		return FALSE;
	}
	
	// 태그 이름과 속성을 분리시킨다.
	// <Record0 Count="3"> -> NodeName : Record0, Attribute : Count, Value : 3
	CString strTag = strParseFrame.Mid(1, nPosS2-1);
	CString strNodeName, strTmp, strAttribute, strValue;

	AfxExtractSubString(strNodeName, strTag, 0, ' ');
	for(INT nCnt = 1; ; nCnt++)
	{
		if(!AfxExtractSubString(strTmp, strTag, nCnt, ' '))
		{
			break;
		}
		AfxExtractSubString(strAttribute, strTmp, 0, '=');
		AfxExtractSubString(strValue, strTmp, 1, '=');
		strValue.TrimLeft(_T("\""));
		strValue.TrimRight(_T("\""));

		SetAttribute(replaceSpecialChars(strAttribute, FALSE), replaceSpecialChars(strValue, FALSE));
	}
	SetNodeName(replaceSpecialChars(strNodeName, FALSE));
	
	strParseFrame = strParseFrame.Right(strParseFrame.GetLength()-(nPosS2+1));
	strParseFrame.TrimLeft();

	while(TRUE)
	{
		// 새로운 태그가 나오면 자식노드를 추가시키며 
		// 자신의 종료태그가 나올때까지 루프를 돈다.		

		TRACE(_T("%s\r\n"), m_strNodeName);
		// 다음 태그를 찾는다.
		INT nPosN1 = strParseFrame.Find(_T("<"));
		INT nPosN2 = strParseFrame.Find(_T(">"), nPosN1+1);
		INT nPosN3 = strParseFrame.Find(_T("</"));

		if(0 > nPosN1 || 0 > nPosN2 || 0 > nPosN3)
		{
			// 다음 태그를 찾을수 없다.
			return FALSE;
		}

		// 다음태그가 종료태그이다.
		if(nPosN1 == nPosN3)
		{
			// 시작태그와 종료태그가 일치하지 않는다.
			// <태그></태그2>
			CString strNodeName2 = strParseFrame.Mid(nPosN1+2, nPosN2-nPosN1-2);	
			if(strNodeName != strNodeName2)
			{
				return FALSE;
			}

			// <태그>strTagValue</태그>
			CString strTagValue = strParseFrame.Left(nPosN1);
			SetNodeValue(replaceSpecialChars(strTagValue, FALSE));
			strParseFrame = strParseFrame.Right(strParseFrame.GetLength()-(nPosN2+1));
			strParseFrame.TrimLeft();

			return TRUE;
		}
		// 다음태그도 시작태그이다.
		// <태그><태그1>
		// -> 하위노드이다.
		else
		{
			CCertifyFrameNode* pNode = CreateNode(_T(""));
			AddChildNode(pNode);					
			BOOL bIsParse = pNode->parseFrame(strParseFrame);
			if(!bIsParse)
			{
				CCertifyFrameNode* pNode = NULL;
				while(m_ChildNodeList.GetCount())
				{
					pNode = m_ChildNodeList.RemoveTail();
					delete pNode;
				}

				return FALSE;
			}
		}
	}
	

	return TRUE;
}

POSITION CCertifyFrameNode::GetChildNodeHeadPosition()
{
	return m_ChildNodeList.GetHeadPosition();
}

POSITION CCertifyFrameNode::GetChildNodeTailPosition()
{
	return m_ChildNodeList.GetTailPosition();
}

CCertifyFrameNode* CCertifyFrameNode::GetChildNodeByPosition(const POSITION pos)
{	
	return m_ChildNodeList.GetAt(pos);
}

CCertifyFrameNode* CCertifyFrameNode::GetChildNodeByIndex(const INT nIndex)
{
	POSITION pos = m_ChildNodeList.FindIndex(nIndex);
	return GetChildNodeByPosition(pos);
}

CCertifyFrameNode* CCertifyFrameNode::GetChildNode(const CString& strNodeName)
{
	CCertifyFrameNode* pNode = NULL;
	POSITION pos = m_ChildNodeList.GetHeadPosition();
	while(NULL != pos)
	{
		pNode = m_ChildNodeList.GetNext(pos);
		if(strNodeName == pNode->GetNodeName())
		{
			return pNode;
		}
	}
	return NULL;
}

CCertifyFrameNode* CCertifyFrameNode::GetChildNodeNext(POSITION& pos)
{
	return m_ChildNodeList.GetNext(pos);
}

CCertifyFrameNode* CCertifyFrameNode::GetChildNodePrev(POSITION& pos)
{
	return m_ChildNodeList.GetPrev(pos);
}

INT CCertifyFrameNode::GetChildNodeCount()
{
	return m_ChildNodeList.GetCount();
}

CCertifyFrameNode* CCertifyFrameNode::GetLastChildNode()
{
	return m_ChildNodeList.GetTail();
}

BOOL CCertifyFrameNode::AddChildNode(CCertifyFrameNode* pChildNode)
{
	m_ChildNodeList.AddTail(pChildNode);
	return TRUE;
}

void CCertifyFrameNode::SetAttribute(const CString& strAttribute, const CString& strValue)
{
	m_Attributes.SetAt(strAttribute, strValue);
}

void CCertifyFrameNode::GetAttribute(const CString& strAttribute, CString& strValue)
{
	m_Attributes.Lookup(strAttribute, strValue);
}

CString CCertifyFrameNode::GetAttribute(const CString& strAttribute)
{
	CString strValue;
	GetAttribute(strAttribute, strValue);

	return strValue;
}
void CCertifyFrameNode::SetNodeName(const CString& strNodeName)
{
	m_strNodeName = strNodeName;
}

BOOL CCertifyFrameNode::SetNodeValue(const CString& strNodeValue)
{
	if(m_ChildNodeList.GetCount())
	{
		return FALSE;
	}
	m_strNodeValue = strNodeValue;

	return TRUE;
}


CCertifyFrameWriter::CCertifyFrameWriter(CESL_Information* pInfo)
{	
	m_pInfo = pInfo;
	m_pNode = NULL;
	clear();
}

CCertifyFrameWriter::~CCertifyFrameWriter()
{
	delete m_pNode;
}

void CCertifyFrameWriter::setCommand(const CString& command)
{
	CCertifyFrameNode* pNodeHeader = m_pNode->GetChildNode(_T("Header"));
	CCertifyFrameNode* pNodeCommand = pNodeHeader->GetChildNode(_T("Command"));
	CString strCommand = command;
	pNodeCommand->SetNodeValue(strCommand);
}

void CCertifyFrameWriter::setReiteration(BOOL reiteration)
{
	CString strReiteration = (reiteration) ? _T("Y") : _T("N");

	CCertifyFrameNode* pNodeHeader = m_pNode->GetChildNode(_T("Header"));
	CCertifyFrameNode* pNodeReiteration = pNodeHeader->GetChildNode(_T("Reiteration"));	
	pNodeReiteration->SetNodeValue(strReiteration);
}

void CCertifyFrameWriter::setClientIp(const CString& clientIp)
{
	CCertifyFrameNode* pNodeHeader = m_pNode->GetChildNode(_T("Header"));
	CCertifyFrameNode* pNodeClientIP = pNodeHeader->GetChildNode(_T("ClientIp"));
	CString strClientIP = clientIp;
	pNodeClientIP->SetNodeValue(strClientIP);
}

void CCertifyFrameWriter::addRecord()
{
	CCertifyFrameNode* pNodeBody = m_pNode->GetChildNode(_T("Body"));
	CCertifyFrameNode* pNodeBody1 = pNodeBody->GetChildNode(_T("Body1"));	
	if(pNodeBody1)
	{
		INT nRecordCnt = pNodeBody1->GetChildNodeCount();
		CString strNodeName;
		strNodeName.Format(_T("Record%d"), nRecordCnt);
		
		CCertifyFrameNode* pNewRecordNode = pNodeBody1->CreateNode(strNodeName);
		pNewRecordNode->SetAttribute(_T("Count"), _T("0"));
		pNodeBody1->AddChildNode(pNewRecordNode);
	}	
}

void CCertifyFrameWriter::addRecordNew()
{
	CCertifyFrameNode* pNodeBody = m_pNode->GetChildNode(_T("Body"));
	CCertifyFrameNode* pNodeBody2 = pNodeBody->GetChildNode(_T("Body2"));	
	if(!pNodeBody2)
	{
		pNodeBody2 = m_pNode->CreateNode(_T("Body2"));
		pNodeBody->AddChildNode(pNodeBody2);
	}
	
	INT nRecordCnt = pNodeBody2->GetChildNodeCount();
	CString strNodeName;
	strNodeName.Format(_T("Record%d"), nRecordCnt);
	
	CCertifyFrameNode* pNewRecordNode = pNodeBody2->CreateNode(strNodeName);
	pNewRecordNode->SetAttribute(_T("Count"), _T("0"));
	pNodeBody2->AddChildNode(pNewRecordNode);	
}

BOOL CCertifyFrameWriter::addElement(const CString& alias, const CString& data)
{
	CCertifyFrameNode* pNodeBody = m_pNode->GetChildNode(_T("Body"));
	CCertifyFrameNode* pNodeBody1 = pNodeBody->GetChildNode(_T("Body1"));	
	if(pNodeBody1)
	{
		CCertifyFrameNode* pRecord = pNodeBody1->GetLastChildNode();
		if(pRecord)
		{
			CString strAlias, strData;
			strAlias = alias;
			strData = data;
			CCertifyFrameNode* pElement = pRecord->CreateNode(strAlias);
			pElement->SetNodeValue(strData);
			pRecord->AddChildNode(pElement);

			INT nElementCnt = pRecord->GetChildNodeCount();
			strData.Format(_T("%d"), nElementCnt);
			pRecord->SetAttribute(_T("Count"), strData);

			return TRUE;
		}
	}
	
	return FALSE;
}

INT CCertifyFrameWriter::setElement(const CString& alias, const CString& data)
{		
	const INT nRecordCnt = getRecordCount();
	if(NULL == m_pNodeBody1)
	{
		return -1000;
	}

	CCertifyFrameNode *pNodeRecord = NULL, *pNodeAlias = NULL;
	CString strRecord;
	for(INT nCnt = 0; nCnt < nRecordCnt; nCnt++)
	{
		strRecord.Format(_T("Record%d"), nCnt);
		pNodeRecord = m_pNodeBody1->GetChildNode(strRecord);
		if(pNodeRecord)
		{
			pNodeAlias = pNodeRecord->GetChildNode(alias);
			if(!pNodeAlias)
			{
				pNodeAlias = pNodeRecord->CreateNode(alias);
				pNodeAlias->SetNodeValue(data);
				pNodeRecord->AddChildNode(pNodeAlias);

				CString strData;
				INT nElementCnt = pNodeRecord->GetChildNodeCount();
				strData.Format(_T("%d"), nElementCnt);
				pNodeRecord->SetAttribute(_T("Count"), strData);
			}
			else
			{
				pNodeAlias->SetNodeValue(data);
			}
		}
	}	

	return 0;
}

BOOL CCertifyFrameWriter::addElementNew(const CString& alias, const CString& data)
{
	CCertifyFrameNode* pNodeBody = m_pNode->GetChildNode(_T("Body"));
	CCertifyFrameNode* pNodeBody2 = pNodeBody->GetChildNode(_T("Body2"));	
	if(pNodeBody2)
	{
		CCertifyFrameNode* pRecord = pNodeBody2->GetLastChildNode();
		if(pRecord)
		{
			CString strAlias, strData;
			strAlias = alias;
			strData = data;
			CCertifyFrameNode* pElement = pRecord->CreateNode(strAlias);
			pElement->SetNodeValue(strData);
			pRecord->AddChildNode(pElement);

			INT nElementCnt = pRecord->GetChildNodeCount();
			strData.Format(_T("%d"), nElementCnt);
			pRecord->SetAttribute(_T("Count"), strData);

			return TRUE;
		}
	}

	return FALSE;
}

INT CCertifyFrameWriter::getRecordCount()
{
	CCertifyFrameNode* pNodeBody = m_pNode->GetChildNode(_T("Body"));
	CCertifyFrameNode* pNodeBody1 = pNodeBody->GetChildNode(_T("Body1"));	
	if(pNodeBody1)
	{
		return pNodeBody1->GetChildNodeCount();
	}

	return 0;
}

INT CCertifyFrameWriter::getRecordCountNew()
{
	CCertifyFrameNode* pNodeBody = m_pNode->GetChildNode(_T("Body"));
	CCertifyFrameNode* pNodeBody2 = pNodeBody->GetChildNode(_T("Body2"));
	if(pNodeBody2)
	{
		return pNodeBody2->GetChildNodeCount();
	}

	return 0;
}

CString CCertifyFrameWriter::getFrame()
{
	CString strFrame;
	m_pNode->GetFrame(strFrame);

	return strFrame;
}

INT CCertifyFrameWriter::getFrame(CHAR** szGet)
{
	// 프레임전송시의 문자열세트인 UTF8로 반환
	CString strFrame = getFrame();
#ifdef _UNICODE
	CHAR* szFrame = WideCharToUTF8(strFrame.GetBuffer(0));
	strFrame.ReleaseBuffer();
#else
	CHAR* szFrame = MultiByteToUTF8(strFrame.GetBuffer(0));
	strFrame.ReleaseBuffer();
#endif
	INT nSize = sizeof(szFrame);
	if(szGet)
	{
		*szGet = szFrame;
	}
	else
	{
		delete szFrame;
	}
	
	return nSize;
}

void CCertifyFrameWriter::clear()
{
	delete m_pNode;
	m_pNode = NULL;

	m_pNode = new CCertifyFrameNode(_T("Frame"));

	CCertifyFrameNode* pNodeHeader			= m_pNode->CreateNode(_T("Header"));
	CCertifyFrameNode* pNodeCommand		= m_pNode->CreateNode(_T("Command"));
	CCertifyFrameNode* pNodeReiteration	= m_pNode->CreateNode(_T("Reiteration"));
	CCertifyFrameNode* pNodeClientIp		= m_pNode->CreateNode(_T("ClientIp"));

	CCertifyFrameNode* pNodeBody			= m_pNode->CreateNode(_T("Body"));
	m_pNodeBody1					= m_pNode->CreateNode(_T("Body1"));
// 	CCertifyFrameNode* pNodeBody2			= m_pNode->CreateNode(_T("Body2"));
	
	m_pNode->AddChildNode(pNodeHeader);

	pNodeHeader->AddChildNode(pNodeCommand);
	pNodeHeader->AddChildNode(pNodeReiteration);
	pNodeHeader->AddChildNode(pNodeClientIp);

	m_pNode->AddChildNode(pNodeBody);

	pNodeBody->AddChildNode(m_pNodeBody1);

	setReiteration(0);

	CHAR szHostName[255];
    CString sAddress;
    PHOSTENT pHostEntry;

	if (gethostname((LPSTR)(LPCTSTR)szHostName, 255) == 0)
	{
		pHostEntry = gethostbyname((LPSTR)(LPCTSTR)szHostName);
		if (pHostEntry)
		{
			sAddress.Format(_T("%d.%d.%d.%d"), 
							(BYTE)pHostEntry->h_addr_list[0][0], 
							(BYTE)pHostEntry->h_addr_list[0][1], 
							(BYTE)pHostEntry->h_addr_list[0][2], 
							(BYTE)pHostEntry->h_addr_list[0][3]);
		}		
	}
	setClientIp(sAddress);
}

CString CCertifyFrameWriter::getCommand()
{
	CCertifyFrameNode* pNodeHeader = m_pNode->GetChildNode(_T("Header"));
	if(pNodeHeader)
	{
		CCertifyFrameNode* pNodeCommand = pNodeHeader->GetChildNode(_T("Command"));
		if(pNodeCommand)
		{			
			return pNodeCommand->GetNodeValue();
		}
	}

	return _T("");
}

CHAR* CCertifyFrameWriter::WideCharToUTF8(const LPWSTR lpwstr)
{
	CHAR* szBuff = NULL;
	INT nSize = ::WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, NULL, NULL, NULL, NULL);
	if(0 < nSize)
	{	
		szBuff = new CHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, szBuff, nSize, NULL, NULL);
	}
	
	return szBuff;
}

CHAR* CCertifyFrameWriter::WideCharToMultiByte(const LPWSTR lpwstr)
{
	CHAR* szBuff = NULL;
	INT nSize = ::WideCharToMultiByte(CP_ACP, 0, lpwstr, -1, NULL, NULL, NULL, NULL);
	if(0 < nSize)
	{	
		szBuff = new CHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::WideCharToMultiByte(CP_ACP, 0, lpwstr, -1, szBuff, nSize, NULL, NULL);
	}
	
	return szBuff;
}


WCHAR* CCertifyFrameWriter::MultiByteToWideChar(const LPSTR lpstr)
{
	WCHAR* szBuff = NULL;
	INT nSize = ::MultiByteToWideChar(CP_ACP, NULL, lpstr, -1, NULL, NULL);	
	if(0 < nSize)
	{	
		szBuff = new WCHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::MultiByteToWideChar(CP_ACP, NULL, lpstr, -1, szBuff, nSize);
	}
	
	return szBuff;
}

CHAR* CCertifyFrameWriter::MultiByteToUTF8(const LPSTR lpstr)
{
	WCHAR* szBuff = MultiByteToWideChar(lpstr);
	CHAR* szResult = WideCharToUTF8(szBuff);
	delete []szBuff;
	return szResult;
}

WCHAR* CCertifyFrameWriter::UTF8ToWideChar(const LPSTR lpstr)
{
	WCHAR* szBuff = NULL;
	INT nSize = ::MultiByteToWideChar(CP_UTF8, NULL, lpstr, -1, NULL, NULL);	
	if(0 < nSize)
	{	
		szBuff = new WCHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::MultiByteToWideChar(CP_UTF8, NULL, lpstr, -1, szBuff, nSize);
	}
	
	return szBuff;
}

CHAR* CCertifyFrameWriter::UTF8ToMultiByte(const LPSTR lpstr)
{
	WCHAR* szBuff = UTF8ToWideChar(lpstr);
	CHAR* szResult = WideCharToMultiByte(szBuff);
	delete []szBuff;
	return szResult;
}

CCertifyFrameReader::CCertifyFrameReader()
{
	m_bParseFrame = FALSE;
	m_pNode = NULL;

	m_pNode = NULL;
	m_pNodeBody1 = NULL;
	m_pNodeBody2 = NULL;
	m_pNodeCommand = NULL;
	m_pNodeReiteration = NULL;
	m_pNodeClientIP = NULL;

	m_nRecordCount = 0;
	m_nElementCount = 0;
	m_pPos = NULL;

	clear();
}

CCertifyFrameReader::~CCertifyFrameReader()
{
	delete m_pNode;
	delete []m_pPos;
}

BOOL CCertifyFrameReader::parseFrame(const LPSTR frame)
{
	BOOL bResult = FALSE;
#ifdef _UNICODE
	WCHAR *szResult = UTF8ToWideChar(frame);
	if(szResult)
	{
		bResult = parseFrame(szResult);
		delete []szResult;
	}
#else
	CString strResult;
	CHAR *szResult = UTF8ToMultiByte(frame);
	strResult.Format(_T("%s"), szResult);
	delete []szResult;
	bResult = parseFrame(strResult);
#endif

	return bResult;
}
BOOL CCertifyFrameReader::makeDM(CESL_DataMgr* pDM)
{
	if(NULL == pDM)
	{
		return FALSE;
	}

	pDM->FreeData();

	if(!m_bParseFrame)
	{
		return FALSE;
	}

	POSITION posRecord = m_pNodeBody1->GetChildNodeHeadPosition();
	if(!posRecord)
	{
		return FALSE;
	}

	CCertifyFrameNode* pRecord = m_pNodeBody1->GetChildNodeNext(posRecord);
	if(!pRecord)
	{
		return FALSE;
	}

	INT nColumnCnt = pRecord->GetChildNodeCount();
	if(0 >= nColumnCnt)
	{
		return FALSE;
	}


	// 알리아스 설정
	CCertifyFrameNode* pNodeCol = NULL;
	CESL_DataMgr::reference* pRef = NULL;
	POSITION posColumn = pRecord->GetChildNodeHeadPosition();

	if(0 == pDM->RefList.GetCount())
	{
		while(posColumn)
		{
			pRef = new CESL_DataMgr::reference;
			pNodeCol = pRecord->GetChildNodeNext(posColumn);
			pRef->FIELD_ALIAS = pNodeCol->GetNodeName();
			pDM->RefList.AddTail((CObject*)pRef);
		}
		pDM->m_nCols = pDM->RefList.GetCount();
	}

	// 데이터 설정
	INT nRow = -1;
	posRecord = m_pNodeBody1->GetChildNodeHeadPosition();
	while(posRecord)
	{
		nRow++;
		pDM->InsertRow(-2);
		pRecord = m_pNodeBody1->GetChildNodeNext(posRecord);	
	}
	pDM->RefreshPositionInfo();

	nRow = -1;
	posRecord = m_pNodeBody1->GetChildNodeHeadPosition();
	while(posRecord)
	{
		nRow++;
		pRecord = m_pNodeBody1->GetChildNodeNext(posRecord);
		posColumn = pRecord->GetChildNodeHeadPosition();
		while(posColumn)
		{
			pNodeCol = pRecord->GetChildNodeNext(posColumn);
 			pDM->SetCellData(pNodeCol->GetNodeName(), pNodeCol->GetNodeValue(), nRow);
		}		
	}

	return TRUE;
}
BOOL CCertifyFrameReader::parseFrame(const CString& frame)
{	
	clear();
	m_bParseFrame = FALSE;
	m_pNode = new CCertifyFrameNode;

	CString strFrame = frame;
	BOOL bIsParse = m_pNode->parseFrame(strFrame);

	if(bIsParse)
	{
		CCertifyFrameNode* pNodeBody = m_pNode->GetChildNode(_T("Body"));
		if(NULL != pNodeBody)
		{
			m_pNodeBody1 = pNodeBody->GetChildNode(_T("Body1"));
// 			m_pNodeBody2 = pNodeBody->GetChildNode(_T("Body2"));
		}

		CCertifyFrameNode* pNodeHeader = m_pNode->GetChildNode(_T("Header"));
		if(NULL != pNodeHeader)
		{
			m_pNodeCommand = pNodeHeader->GetChildNode(_T("Command"));
			m_pNodeReiteration = pNodeHeader->GetChildNode(_T("Reiteration"));
			m_pNodeClientIP = pNodeHeader->GetChildNode(_T("ClientIp"));
		}

		if(
			NULL == m_pNodeBody1 || 
// 			NULL == m_pNodeBody2 ||
			NULL == m_pNodeCommand ||
			NULL == m_pNodeReiteration ||
			NULL == m_pNodeClientIP
			)
		{
			delete m_pNode;
			m_pNode = NULL;
			return FALSE;
		}


		m_nRecordCount = m_pNodeBody1->GetChildNodeCount();
		CCertifyFrameNode* pNodeRecord0 = m_pNodeBody1->GetChildNode(_T("Record0"));
		if(NULL == pNodeRecord0)
		{
			m_nElementCount = 0;
		}
		else
		{
	 		m_nElementCount = _ttoi(pNodeRecord0->GetAttribute(_T("Count")));
		}

		INT nMaxRecord = getRecordCount();
		INT nMaxElement = getColumnCount();
		if(0 >= nMaxRecord || 0 >= nMaxElement)
		{
// 			bIsParse = FALSE;
		}
		else
		{
			m_pPos = new POSITION[nMaxRecord * nMaxElement];
			memset(m_pPos, 0x00, sizeof(POSITION) * nMaxRecord * nMaxElement);

			INT nRow = -1, nCol = -1;
			CCertifyFrameNode* pNodeRecord = NULL;
			POSITION posRecord = m_pNodeBody1->GetChildNodeHeadPosition();
			POSITION posElement = NULL;
			while(posRecord)
			{
				nRow++;
				nCol = -1;

				if(nRow >= nMaxRecord)
				{
					break;
				}

				pNodeRecord = m_pNodeBody1->GetChildNodeNext(posRecord);
				posElement = pNodeRecord->GetChildNodeHeadPosition();
				while(posElement)
				{
					nCol++;
					if(nCol < nMaxElement)
					{
						m_pPos[(nRow * nMaxElement) + nCol] = posElement;
						pNodeRecord->GetChildNodeNext(posElement);
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	m_bParseFrame = bIsParse;

	return bIsParse;
}

CString CCertifyFrameReader::getCommand()
{
	if(!m_bParseFrame)
	{
		return _T("");
	}

	CString strCommand = m_pNodeCommand->GetNodeValue();
	strCommand = strCommand;
	
	return strCommand;
}
INT CCertifyFrameReader::getRecordCount()
{
	return m_nRecordCount;
}
INT CCertifyFrameReader::getRecordCountNew()
{
	if(!m_bParseFrame)
	{
		return -1;
	}

	if(!m_pNodeBody2)
	{
		return -2;
	}
	return m_pNodeBody2->GetChildNodeCount();

	return 0;
}
INT CCertifyFrameReader::getReiteration()
{
	if(!m_bParseFrame)
	{
		return -1;
	}

	CString strReiteration = m_pNodeReiteration->GetNodeValue();
	strReiteration = strReiteration;
	
	return (_T("Y") == strReiteration) ? TRUE : FALSE;
}

CString CCertifyFrameReader::getClientIp()
{
	if(!m_bParseFrame)
	{
		return _T("");
	}

	CString strClientIP = m_pNodeClientIP->GetNodeValue();
	strClientIP = strClientIP;

	return strClientIP;
}

INT CCertifyFrameReader::getColumnCount()
{
	return m_nElementCount;
}

INT CCertifyFrameReader::getColumnCountNew()
{
	if(!m_bParseFrame)
	{
		return -1;
	}

	if(!m_pNodeBody2)
	{
		return -2;
	}

	CCertifyFrameNode* pNodeRecord0 = m_pNodeBody2->GetChildNode(_T("Record0"));
	CString strCnt = pNodeRecord0->GetAttribute(_T("Count"));
	if(strCnt.IsEmpty())
	{
		return -1;
	}
	else
	{
		return _ttoi(strCnt);
	}
}

CString* CCertifyFrameReader::getColumnNameList()
{
	CCertifyFrameNode* pNodeRecord0 = m_pNodeBody1->GetChildNode(_T("Record0"));
	
	INT nNodeCnt = pNodeRecord0->GetChildNodeCount();
	CString* pastrColumnName = new CString[nNodeCnt];

	INT nCnt = 0;
	POSITION pos = pNodeRecord0->GetChildNodeHeadPosition();
	while(NULL != pos )
	{
		CCertifyFrameNode* pNode = pNodeRecord0->GetChildNodeNext(pos);
		if(pNode)
		{
			pastrColumnName[nCnt] = pNode->GetNodeName();
		}
		nCnt++;
	}	
	
	return pastrColumnName;
}

CString* CCertifyFrameReader::getColumnNameListNew()
{
	if(!m_pNodeBody2)
	{
		return NULL;
	}

	CCertifyFrameNode* pNodeRecord0 = m_pNodeBody2->GetChildNode(_T("Record0"));
	
	INT nNodeCnt = pNodeRecord0->GetChildNodeCount();
	CString* pastrColumnName = new CString[nNodeCnt];

	INT nCnt = 0;
	POSITION pos = pNodeRecord0->GetChildNodeHeadPosition();
	while(NULL != pos )
	{
		CCertifyFrameNode* pNode = pNodeRecord0->GetChildNodeNext(pos);
		if(pNode)
		{
			pastrColumnName[nCnt] = pNode->GetNodeName();
		}
		nCnt++;
	}	
	
	return pastrColumnName;
}

CString CCertifyFrameReader::getColumnName(const INT index)
{
	if(!m_bParseFrame)
	{
		return _T("");
	}

	CCertifyFrameNode* pNodeRecord0 = m_pNodeBody1->GetChildNode(_T("Record0"));
	if(pNodeRecord0)
	{
		CCertifyFrameNode* pColumn = pNodeRecord0->GetChildNodeByIndex(index);
		if(pColumn)
		{
			return pColumn->GetNodeName();
		}
	}

	return _T("");
}

CString CCertifyFrameReader::getColumnNameNew(const INT index)
{
	if(!m_bParseFrame)
	{
		return _T("");
	}

	if(!m_pNodeBody2)
	{
		return _T("");
	}

	CCertifyFrameNode* pNodeRecord0 = m_pNodeBody2->GetChildNode(_T("Record0"));
	
	if(pNodeRecord0)
	{
		CCertifyFrameNode* pNodeColumn = pNodeRecord0->GetChildNodeByIndex(index);
		if(pNodeColumn)
		{
			return pNodeColumn->GetNodeName();
		}
	}

	return _T("");
}

CString CCertifyFrameReader::getElement(const INT rowIndex, const CString& alias)
{
	if(!m_bParseFrame)
	{
		return _T("");
	}

	CString strRecordName;
	strRecordName.Format(_T("Record%d"), rowIndex);

	CCertifyFrameNode* pNodeRecord = m_pNodeBody1->GetChildNode(strRecordName);
	if(pNodeRecord)
	{
		CCertifyFrameNode* pNodeColumn = pNodeRecord->GetChildNode(alias);	
		if(pNodeColumn)
		{
			return pNodeColumn->GetNodeValue();
		}
	}

	return _T("");
}

CString CCertifyFrameReader::getElementNew(const INT rowIndex, const CString& alias)
{
	if(!m_bParseFrame)
	{
		return _T("");
	}
	if(!m_pNodeBody2)
	{
		return _T("");
	}

	CString strRecordName;
	strRecordName.Format(_T("Record%d"), rowIndex);

	CCertifyFrameNode* pNodeRecord = m_pNodeBody2->GetChildNode(strRecordName);
	if(pNodeRecord)
	{
		CCertifyFrameNode* pNodeColumn = pNodeRecord->GetChildNode(alias);	
		if(pNodeColumn)
		{
			return pNodeColumn->GetNodeValue();
		}
	}	
	return _T("");
}

CString CCertifyFrameReader::getElement(const INT rowIndex, const INT columnIndex)
{
	if(
		!m_bParseFrame ||
		rowIndex >= m_nRecordCount || 
		columnIndex >= m_nElementCount || 
		rowIndex < 0 ||
		columnIndex < 0
		)
	{
		return _T("");
	}

	CString strRecordName;
	strRecordName.Format(_T("Record%d"), rowIndex);

	CCertifyFrameNode* pNodeRecord = m_pNodeBody1->GetChildNode(strRecordName);
	if(pNodeRecord)
	{
		POSITION pos = m_pPos[(rowIndex * m_nElementCount) + columnIndex];
		if(pos)
		{
			CCertifyFrameNode* pNodeColumn = pNodeRecord->GetChildNodeByPosition(pos);
	// 		CCertifyFrameNode* pNodeColumn = pNodeRecord->GetChildNodeByIndex(columnIndex);	
			if(pNodeColumn)
			{
				return pNodeColumn->GetNodeValue();
			}
		}
	}

	return _T("");
}

CString CCertifyFrameReader::getElementNew(const INT rowIndex, const INT columnIndex)
{
	if(!m_bParseFrame)
	{
		return _T("");
	}
	if(!m_pNodeBody2)
	{
		return _T("");
	}

	CString strRecordName;
	strRecordName.Format(_T("Record%d"), rowIndex);

	CCertifyFrameNode* pNodeRecord = m_pNodeBody2->GetChildNode(strRecordName);
	if(pNodeRecord)
	{
		CCertifyFrameNode* pNodeColumn = pNodeRecord->GetChildNodeByIndex(columnIndex);	
		if(pNodeColumn)
		{
			return pNodeColumn->GetNodeValue();
		}
	}

	return _T("");
}

void CCertifyFrameReader::clear()
{
	delete m_pNode;
	delete []m_pPos;
	
	m_bParseFrame = FALSE;
	m_pNode = NULL;

	m_pNode = NULL;
	m_pNodeBody1 = NULL;
	m_pNodeBody2 = NULL;
	m_pNodeCommand = NULL;
	m_pNodeReiteration = NULL;
	m_pNodeClientIP = NULL;

	m_nRecordCount = 0;
	m_nElementCount = 0;
	m_pPos = NULL;
}

CHAR* CCertifyFrameReader::WideCharToUTF8(const LPWSTR lpwstr)
{
	CHAR* szBuff = NULL;
	INT nSize = ::WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, NULL, NULL, NULL, NULL);
	if(0 < nSize)
	{	
		szBuff = new CHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, szBuff, nSize, NULL, NULL);
	}
	
	return szBuff;
}

CHAR* CCertifyFrameReader::WideCharToMultiByte(const LPWSTR lpwstr)
{
	CHAR* szBuff = NULL;
	INT nSize = ::WideCharToMultiByte(CP_ACP, 0, lpwstr, -1, NULL, NULL, NULL, NULL);
	if(0 < nSize)
	{	
		szBuff = new CHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::WideCharToMultiByte(CP_ACP, 0, lpwstr, -1, szBuff, nSize, NULL, NULL);
	}
	
	return szBuff;
}


WCHAR* CCertifyFrameReader::MultiByteToWideChar(const LPSTR lpstr)
{
	WCHAR* szBuff = NULL;
	INT nSize = ::MultiByteToWideChar(CP_ACP, NULL, lpstr, -1, NULL, NULL);	
	if(0 < nSize)
	{	
		szBuff = new WCHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::MultiByteToWideChar(CP_ACP, NULL, lpstr, -1, szBuff, nSize);
	}
	
	return szBuff;
}

CHAR* CCertifyFrameReader::MultiByteToUTF8(const LPSTR lpstr)
{
	WCHAR* szBuff = MultiByteToWideChar(lpstr);
	CHAR* szResult = WideCharToUTF8(szBuff);
	delete []szBuff;
	return szResult;
}

WCHAR* CCertifyFrameReader::UTF8ToWideChar(const LPSTR lpstr)
{
	WCHAR* szBuff = NULL;
	INT nSize = ::MultiByteToWideChar(CP_UTF8, NULL, lpstr, -1, NULL, NULL);	
	if(0 < nSize)
	{	
		szBuff = new WCHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::MultiByteToWideChar(CP_UTF8, NULL, lpstr, -1, szBuff, nSize);
	}
	
	return szBuff;
}

CHAR* CCertifyFrameReader::UTF8ToMultiByte(const LPSTR lpstr)
{
	WCHAR* szBuff = UTF8ToWideChar(lpstr);
	CHAR* szResult = WideCharToMultiByte(szBuff);
	delete []szBuff;
	return szResult;
}


extern "C" __declspec(dllexport) int CommMiddleWareDaemon(char* szIP, int nPort, int nWaitTime, char* szInput, char** szOutput);

CCertifyFrameSender::CCertifyFrameSender(CESL_Information* pInfo)
{
	m_pInfo = pInfo;
	m_nPort = 33205;
	m_nWaitTime = 30;
	m_bIsTestMode = FALSE;

	CString strFileName, strLine;
	CFileFind FF;
	strFileName.Format(_T("..\\CFG\\EFS\\DefaultCertifyServiceInfo.log"));
	BOOL bFind = FF.FindFile(strFileName);
	BOOL bIsDefault = FALSE;

	if(bFind)
	{	
		CStdioFile fd;
		if (fd.Open(strFileName, CFile::modeRead|CFile::typeBinary)) 
		{
#ifdef _UNICODE
			TCHAR cUni;
			fd.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				fd.SeekToBegin();
			}		
#endif
			fd.ReadString(strLine);
			strLine.TrimLeft();
			strLine.TrimRight();
			fd.Close();		

			if(!strLine.IsEmpty())
			{
				bIsDefault = TRUE;
			}
		}
	}

	if(!bIsDefault)
	{
		DWORD dwPID = ::GetCurrentProcessId();	
  		strFileName.Format(_T("..\\CFG\\EFS\\CertifyServiceInfo(%u).log"), dwPID);
	}

	CStdioFile fd;
	if (fd.Open(strFileName, CFile::modeRead|CFile::typeBinary)) 
	{
#ifdef _UNICODE
		TCHAR cUni;
		fd.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fd.SeekToBegin();
		}		
#endif
		fd.ReadString(strLine);
		strLine.TrimLeft();
		strLine.TrimRight();
		fd.Close();		

		BOOL bResult[3] = {FALSE, FALSE, FALSE};
		CString strIP, strPort, strWaitTime;
		bResult[0] = AfxExtractSubString(strIP, strLine, 0, ',');
		bResult[1] = AfxExtractSubString(strPort, strLine, 1, ',');
		bResult[2] = AfxExtractSubString(strWaitTime, strLine, 2, ',');
		
		SetIP(strIP);
		m_nPort = _ttoi(strPort);
		m_nWaitTime = _ttoi(strWaitTime);
	}
	else
	{
		SetIP(_T("127.0.0.1"));
	}
}

CCertifyFrameSender::~CCertifyFrameSender()
{
	m_pInfo = NULL;
}

BOOL CCertifyFrameSender::SetIP(CString strIP)
{	
#ifdef _UNICODE
	CHAR* szBuff = WideCharToMultiByte(strIP.GetBuffer(0));
	strIP.ReleaseBuffer();
	memset(m_szIP, 0x00,  MAX_PATH);
	strcpy(m_szIP, szBuff);

	delete []szBuff;
#else
	memset(m_szIP, 0x00,  MAX_PATH);
	strcpy(m_szIP, strIP);
#endif

	return TRUE;
}

void CCertifyFrameSender::SetPort(INT nPort)
{
	m_nPort = nPort;
}
void CCertifyFrameSender::SetWaitTime(INT nWaitTime)
{
	m_nWaitTime = nWaitTime;
}

void CCertifyFrameSender::SetTestMode(BOOL bIsTest)
{
	m_bIsTestMode = bIsTest;
}

INT CCertifyFrameSender::CommMiddleWareDaemon(CHAR* szIP, INT nPort, INT nWaitTime, CHAR* szInput, CHAR* &szOutput)
{
	if(m_pInfo)
	{
		if(10000 == m_pInfo->MODE || 30000 == m_pInfo->MODE)
		{
			return 1000;
		}
	}

	CFileFind FF;
	BOOL bLog = FF.FindFile(_T("..\\cfg\\_CERTIFYLOG"));

	SYSTEMTIME t;
	GetLocalTime(&t);
	CString filename, msg;
	filename.Format(_T("..\\tunning\\CertifySender_%04d%02d%02d.log"), t.wYear, t.wMonth, t.wDay);

	if(bLog)
	{
		CFileFind filefind;
		UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
		if(TRUE == filefind.FindFile(filename))
		{
			nOpenFlags ^= CFile::modeCreate;
		}
		
		CFile file;
		BOOL bIsOpen = file.Open(filename, nOpenFlags);
		if(bIsOpen)
		{
			if(0 == file.GetLength())
			{			
				//	UTF8 봄문자
				BYTE BOM[3] = { 0xEF, 0xBB, 0xBF };
				file.Write(&BOM, sizeof(BOM));
			}
			file.SeekToEnd();

			CString strIP;
#ifdef _UNICODE
			WCHAR* szwIP = MultiByteToWideChar(szIP);
			strIP.Format(_T("%s"), szwIP);
			delete []szwIP;
			szwIP = NULL;
#else
			strIP.Format(_T("%s"), szIP);
#endif
			CString strIds;
			strIds.Format(_T("\r\n[%02d:%02d:%02d][%s:%d,%d] Query : \r\n"), t.wHour, t.wMinute, t.wSecond, strIP, nPort, nWaitTime);
#ifdef _UNICODE
			CHAR* szIds = WideCharToUTF8(strIds.GetBuffer(0));
			strIds.ReleaseBuffer();
			file.Write(szIds, strlen(szIds));
			delete []szIds;
			file.Write(szInput, strlen(szInput));
#else
			file.Write(strIds.GetBuffer(0), strlen(strIds));
			strIds.ReleaseBuffer();
			file.Write(szInput, strlen(szInput));
#endif
			file.Close();
		}
	}

	// API 이용	: INPUT과 OUTPUT 모두 UTF8형식이다.
	szOutput = NULL;
 	INT ids = ::CommMiddleWareDaemon(szIP, nPort, nWaitTime, szInput, &szOutput);

	if(bLog)
	{
		GetLocalTime(&t);

		CFileFind filefind;
		UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
		if(TRUE == filefind.FindFile(filename))
		{
			nOpenFlags ^= CFile::modeCreate;
		}
		
		CFile file;
		BOOL bIsOpen = file.Open(filename, nOpenFlags);
		if(bIsOpen)
		{
			if(0 == file.GetLength())
			{			
				//	UTF8 봄문자
				BYTE BOM[3] = { 0xEF, 0xBB, 0xBF };
				file.Write(&BOM, sizeof(BOM));
			}
			file.SeekToEnd();
			CString strIds;
			strIds.Format(_T("\r\n[%02d:%02d:%02d] Result : %d\r\n"), t.wHour, t.wMinute, t.wSecond, ids);
#ifdef _UNICODE
			CHAR* szIds = WideCharToUTF8(strIds.GetBuffer(0));
			strIds.ReleaseBuffer();
			file.Write(szIds, strlen(szIds));
			delete []szIds;
#else
			file.Write(strIds.GetBuffer(0), strlen(strIds));
			strIds.ReleaseBuffer();
#endif
			if(szOutput)
			{				
				file.Write(szOutput, strlen(szOutput));		
			}
			file.Close();
		}
	}

	return ids;
}

INT CCertifyFrameSender::CommMiddleWareDaemon(CString& szInput, CString& szOutput)
{	
	if(m_pInfo)
	{
		if(10000 == m_pInfo->MODE || 30000 == m_pInfo->MODE)
		{
			szOutput.Empty();
			return 1000;
		}
	}
#ifdef _UNICODE
	CHAR* szData = WideCharToUTF8(szInput.GetBuffer(0));
	szInput.ReleaseBuffer();
	szOutput.Empty();

	CHAR* szGet = NULL;
	INT ids = CommMiddleWareDaemon(m_szIP, m_nPort, m_nWaitTime, szData, szGet);

	delete []szData;
#else
	CHAR* szData = MultiByteToUTF8(szInput.GetBuffer(0));
	szInput.ReleaseBuffer();
	szOutput.Empty();


	CHAR* szGet = NULL;
	INT ids = CommMiddleWareDaemon(m_szIP, m_nPort, m_nWaitTime, szData, szGet);
	szInput.ReleaseBuffer();

	delete []szData;
#endif
	if(szGet)
	{
#ifdef _UNICODE
		// 결과값은 UTF8로 오기 때문에 변환시킨다.
		WCHAR* szResult = UTF8ToWideChar(szGet);
		szOutput.Format(_T("%s"), szResult);
		delete []szResult;
#else
		CHAR* szResult = UTF8ToMultiByte(szGet);
		szOutput.Format(_T("%s"), szResult);
		delete []szResult;
#endif
		delete []szGet;
	}

	return ids;
}

INT CCertifyFrameSender::CommMiddleWareDaemon(CCertifyFrameWriter* pWriter, CCertifyFrameReader* pReader)
{
	if(NULL == pWriter)
	{
		return -1000;
	}

	if(m_pInfo)
	{
		if(10000 == m_pInfo->MODE || 30000 == m_pInfo->MODE)
		{
			pReader->clear();
			return 1000;
		}
	}

	CHAR *szIn = NULL, *szOut = NULL;
	pWriter->getFrame(&szIn);

	INT ids = CommMiddleWareDaemon(m_szIP, m_nPort, m_nWaitTime, szIn, szOut);
	delete []szIn;
	if(0 > ids)
	{
		return ids;
	}

	if(NULL != pReader && szOut)
	{
		ids = pReader->parseFrame(szOut);
	}
	delete []szOut;

	return ids;
}
CHAR* CCertifyFrameSender::WideCharToUTF8(const LPWSTR lpwstr)
{
	CHAR* szBuff = NULL;
	INT nSize = ::WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, NULL, NULL, NULL, NULL);
	if(0 < nSize)
	{	
		szBuff = new CHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, szBuff, nSize, NULL, NULL);
	}
	
	return szBuff;
}

CHAR* CCertifyFrameSender::WideCharToMultiByte(const LPWSTR lpwstr)
{
	CHAR* szBuff = NULL;
	INT nSize = ::WideCharToMultiByte(CP_ACP, 0, lpwstr, -1, NULL, NULL, NULL, NULL);
	if(0 < nSize)
	{	
		szBuff = new CHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::WideCharToMultiByte(CP_ACP, 0, lpwstr, -1, szBuff, nSize, NULL, NULL);
	}
	
	return szBuff;
}

WCHAR* CCertifyFrameSender::MultiByteToWideChar(const LPSTR lpstr)
{
	WCHAR* szBuff = NULL;
	INT nSize = ::MultiByteToWideChar(CP_ACP, NULL, lpstr, -1, NULL, NULL);	
	if(0 < nSize)
	{	
		szBuff = new WCHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::MultiByteToWideChar(CP_ACP, NULL, lpstr, -1, szBuff, nSize);
	}
	
	return szBuff;
}

CHAR* CCertifyFrameSender::MultiByteToUTF8(const LPSTR lpstr)
{
	WCHAR* szBuff = MultiByteToWideChar(lpstr);
	CHAR* szResult = WideCharToUTF8(szBuff);
	delete []szBuff;
	return szResult;
}

WCHAR* CCertifyFrameSender::UTF8ToWideChar(const LPSTR lpstr)
{
	WCHAR* szBuff = NULL;
	INT nSize = ::MultiByteToWideChar(CP_UTF8, NULL, lpstr, -1, NULL, NULL);	
	if(0 < nSize)
	{	
		szBuff = new WCHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::MultiByteToWideChar(CP_UTF8, NULL, lpstr, -1, szBuff, nSize);
	}
	
	return szBuff;
}

CHAR* CCertifyFrameSender::UTF8ToMultiByte(const LPSTR lpstr)
{
	WCHAR* szBuff = UTF8ToWideChar(lpstr);
	CHAR* szResult = WideCharToMultiByte(szBuff);
	delete []szBuff;
	return szResult;
}

INT CCertifyFrameSender::Send(CCertifyFrameWriter *pWriter, CCertifyFrameReader *pReader)
{
	if(!pWriter)
	{
		return -1000;
	}

	return CommMiddleWareDaemon(pWriter, pReader);
}

CString CCertifyFrameSender::getError(INT nCode)
{
	CString strResult;
	strResult.Format(_T("자관서버와 연결이 되지 않습니다.[%d]"), nCode);
	return strResult;
}

INT CCertifyFrameWriter::getWorkLog(CString& strWorkLog)
{
	CString strLibCode;
	CString strUserID;
	CString strIP;
	CString strCommand = getCommand();

	if(m_pInfo)
	{
		strLibCode = m_pInfo->LIB_CODE;
		strUserID = m_pInfo->USER_ID;
		strIP = m_pInfo->LOCAL_IP;
	}
#ifdef _UNICODE
	CString sSourcePath = __WFILE__;
#else
	CString sSourcePath = __FILE__;
#endif
	INT nBackSlashPos = sSourcePath.ReverseFind(_T('\\'));

	CString sSourceFile;
	sSourceFile = sSourcePath.Mid(nBackSlashPos+1);
	if (22 < sSourceFile.GetLength()) sSourceFile = sSourceFile.Left(22);

	CTime t = CTime::GetCurrentTime();

	if (NULL==m_pInfo) 
	{
		strWorkLog.Format(_T("%04d%02d%02d %02d%02d %s>%s#%d"), 
					  t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), 
					  strCommand, sSourceFile, __LINE__);
	}
	else
	{
		strWorkLog.Format(_T("%04d%02d%02d %02d%02d [%s]%s@%s %s>%s#%d"), 
					  t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), 
					  strLibCode, strUserID, strIP, strCommand, sSourceFile, __LINE__);
	}

	if (99 < strWorkLog.GetLength())
	{
		strWorkLog = strWorkLog.Left(99);
	}

	return 0;
}

INT CCertifyFrameSender::getWorkLog(CCertifyFrameWriter* pWriter, CString& strWorkLog)
{
	if(!pWriter)
	{
		return -1000;
	}

	CString strLibCode;
	CString strUserID;
	CString strIP;
	CString strCommand = pWriter->getCommand();

	if(m_pInfo)
	{
		strLibCode = m_pInfo->LIB_CODE;
		strUserID = m_pInfo->USER_ID;
		strIP = m_pInfo->LOCAL_IP;
	}

#ifdef _UNICODE
	CString sSourcePath = __WFILE__;
#else
	CString sSourcePath = __FILE__;
#endif
	INT nBackSlashPos = sSourcePath.ReverseFind(_T('\\'));

	CString sSourceFile;
	sSourceFile = sSourcePath.Mid(nBackSlashPos+1);
	if (22 < sSourceFile.GetLength()) sSourceFile = sSourceFile.Left(22);

	CTime t = CTime::GetCurrentTime();

	if (NULL==m_pInfo) 
	{
		strWorkLog.Format(_T("%04d%02d%02d %02d%02d %s>%s#%d"), 
					  t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), 
					  strCommand, sSourceFile, __LINE__);
	}
	else
	{
		strWorkLog.Format(_T("%04d%02d%02d %02d%02d [%s]%s@%s %s>%s#%d"), 
					  t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), 
					  strLibCode, strUserID, strIP, strCommand, sSourceFile, __LINE__);
	}

	if (99 < strWorkLog.GetLength())
	{
		strWorkLog = strWorkLog.Left(99);
	}

	return 0;
}

BOOL CCertifyFrameSender::IsCommandList(const CString &strCommand, const CString &strFileName)
{	
	BOOL bIsFind = FALSE;
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0)
	{
		return FALSE;
	}
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}

	CString strLine;
	while(fd.ReadString(strLine))
	{
		strLine.TrimRight();
		strLine.TrimLeft();
		if(strLine == strCommand)
		{
			bIsFind = TRUE;
			break;
		}
	}
	fd.Close();
	
	return bIsFind;
}


CString CCertifyFrameReader::getFrame()
{
	CString strFrame;
	if(m_pNode)
	{
		m_pNode->GetFrame(strFrame);
	}

	return strFrame;
}

BOOL CCertifyFrameWriter::IsCommandList(const CString &strCommand, const CString &strFileName)
{	
	BOOL bIsFind = FALSE;
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0)
	{
		return FALSE;
	}
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}

	CString strLine;
	while(fd.ReadString(strLine))
	{
		strLine.TrimRight();
		strLine.TrimLeft();
		if(strLine == strCommand)
		{
			bIsFind = TRUE;
			break;
		}
	}
	fd.Close();
	
	return bIsFind;
}

INT CCertifyFrameReader::getError(CString& strCode, CString& strDesc)
{
	strCode.Empty();
	strDesc.Empty();

	CString strCommand = getCommand();
	if(_T("ERROR") != strCommand)
	{
		return 1000;
	}
	strCode = getElement(0, 0);

	if(14 != strCode.GetLength())
	{
		return -1000;
	}

	// 자리수	크기	설명
	// 0		1		미들웨어 : M
	// 1		1		시스템정의코드 : K
	// 2		2		서비스 그룹 번호 : 01, 02, ..., n
	// 4		3		지역/통합센터 : LCT(지역센터), TCT(통합센터)
	// 7		1		서비스방향 : B(하위->상위), R(상위->하위)
	// 8		2		그룹내 서비스번호 : 01, 02, ..., n
	// 10		2		에러코드 : SY(프레임), DB(DB), DT(결과없음), MW(미들웨어통신), ET(Exception)
	// 12		2		에러코드별 번호 : 01, 02, ..., n	

	// 14/07/26 박대우 : 마이핀 인증 기능 추가로 인해서 용어 변경 (실명인증->본인인증)
/*// BEFORE ---------------------------------------------------------------------------------
	strDesc.Format(_T("미들웨어 실명인증오류.[%s]"), strCode);
*/// AFTER ----------------------------------------------------------------------------------
	strDesc.Format(_T("미들웨어 본인인증오류.[%s]"), strCode);
//*/ END ------------------------------------------------------------------------------------
	return 0;
}