// DupKeyMaker.cpp: implementation of the CDupKeyMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMaker.h"
#include "../IndexToolKit/Index.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자
CDupKeyMaker::CDupKeyMaker(CESL_Mgr * pMgr)
{
	m_pKeyDM = NULL;
	m_pParentMgr = pMgr;
}
// 소멸자
CDupKeyMaker::~CDupKeyMaker()
{

}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
//
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CDupKeyMaker::RemoveRoleWord(CString &strAuthor)
{
	//================================================================
	// 변수정의
	//================================================================
	INT nFind;
	CString str1, str2;
	CESL_CharSetMgr CharSetMgr_UnlessWord;


	//================================================================
	// 1. 역할어에대한 문자셋을 구한다.
	//================================================================
	CharSetMgr_UnlessWord.InitData(_T("역할어"), _T("역할어"));
	
	//================================================================
	// 2. 인자값을 공백문자로 두 단어로 분리한다.
	// 3. 두단어에대한 값이 역할어에 대한 문자셋으로 있다면 공백뒤를 지운다.
	//================================================================
	// 2.
	strAuthor.TrimLeft();	strAuthor.TrimRight();
	nFind = strAuthor.ReverseFind(' ');
	if( nFind > 0 )
	{
		str1 = strAuthor.Mid(nFind);
		str2 = str1;
		str1.TrimLeft();	str1.TrimRight();
		
		// 3.
		if( CharSetMgr_UnlessWord.Find(str1, str2) == 0 )
		{
			strAuthor.Delete(nFind, strAuthor.GetLength() - nFind);
		}
		strAuthor.TrimLeft();	strAuthor.TrimRight();
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 마크에서 키값을 추출하여 DM에 저장한다.
//
// [ 미니스펙 ]
// 1. 마크에서 해당값들을 추출한다.
//		- 245$a : title		(본서명)
//		- 245$d : author	(첫번째 저자)
//		- 260$b : publisher	(발행자,배포자)
//		- 260$c : pubyear	(발행년,배포년)
//		- 020$a : isbn		(ISBN)
//		- 049$l : regno		(등록번호)
// 2. 저자에서 역할어를 제거한다.
// 3. 245$d에 저자가 없을경우 100$a 태크에서 저자를 가져온다.
// 4. 본서명, 저자, 발행자가 없다면 올바른 마크가 아니므로 에러 리턴한다.
// 5. 멤버 DM의 nIdx번째 로우에 위에서 추출한 값들을 저장한다.
//		- IBS_본표제
//		- IBS_저작자
//		- IBS_발행자
//		- IBS_발행년
//		- BB_등록번호
//		- BB_낱권_ISBN
//////////////////////////////////////////////////////////////////////
INT CDupKeyMaker::ExtractKeyByMarc(CMarc *pMarc, INT nIdx/*=-1*/)
{
	//================================================================
	// 변수정의
	//================================================================
	INT nPos, cnt, i;
	CString strMarc, title, author, publisher, pubyear, isbn, strTmpData, regNo;
	//===================================================
	//2010.11.29 ADD BY PJW : 총서명 조건추가
	CString strSeriesTitle;
	//===================================================
	CArray<CString, CString&> arrIsbn;
	
	if( pMarc == NULL || m_pKeyDM == NULL )		return -1;	
	
	//================================================================
	// 1. 마크에서 해당값들을 추출한다.
	//		- 245$a : title		(본서명)
	//		- 245$d : author	(첫번째 저자)
	//		- 260$b : publisher	(발행자,배포자)
	//		- 260$c : pubyear	(발행년,배포년)
	//		- 020$a : isbn		(ISBN)
	//		- 049$l : regno		(등록번호)
	//================================================================
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$a"), title			);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$d"), author			);	
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("260$b"), publisher		);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("260$c"), pubyear		);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("020$a"), isbn, &arrIsbn	);
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$l"), regNo			);
	//===================================================
	//2010.11.29 ADD BY PJW : 총서명 조건추가
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("440$a"), strSeriesTitle	);
	if( strSeriesTitle.IsEmpty() )
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("490$a"), strSeriesTitle	);
	}
	//===================================================


	//================================================================
	// 2. 저자에서 역할어를 제거한다.
	//================================================================
	RemoveRoleWord(author);
	
	//================================================================
	// 3. 245$d에 저자가 없을경우 100$a 태크에서 저자를 가져온다.
	//================================================================
	if( author.IsEmpty() == TRUE ) 
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), author);
	}
	
	//================================================================
	// 4. 본서명, 저자, 발행자가 없다면 올바른 마크가 아니므로 에러 리턴한다.
	//	2005.11.01  isbn항목 추가 (육사요구사항)
	//================================================================
	if( title.IsEmpty() == TRUE && author.IsEmpty() == TRUE && publisher.IsEmpty() == TRUE && isbn.IsEmpty() == TRUE )	return -1;
	

	//================================================================
	// 5. 멤버 DM의 nIdx번째 로우에 위에서 추출한 값들을 저장한다.
	//		- IBS_본표제
	//		- IBS_저작자
	//		- IBS_발행자
	//		- IBS_발행년
	//		- BB_등록번호
	//		- BB_낱권_ISBN
	//================================================================
	// -1이라면 당연히 로우를 추가하여 저장한다.
	if( nIdx < 0 )
	{
		m_pKeyDM->InsertRow(-1);
		nIdx = m_pKeyDM->GetRowCount()-1;
	}
	m_pKeyDM->SetCellData(_T("IBS_본표제"), title, nIdx);
	m_pKeyDM->SetCellData(_T("IBS_저작자"), author, nIdx);
	m_pKeyDM->SetCellData(_T("IBS_발행자"), publisher, nIdx);
	m_pKeyDM->SetCellData(_T("IBS_발행년"), pubyear, nIdx);
	m_pKeyDM->SetCellData(_T("BB_등록번호"), regNo, nIdx);
	//===================================================
	//2010.11.29 ADD BY PJW : 총서명 조건추가
	m_pKeyDM->SetCellData(_T("UDF_총서명"), strSeriesTitle, nIdx);
	//===================================================
	
	
	cnt = arrIsbn.GetSize();
	if		( cnt == 0 ) 
	{
		m_pKeyDM->SetCellData(_T("BB_낱권_ISBN"), _T(""), nIdx);
	}
	else if	( cnt == 1 )
	{
		nPos = isbn.Find(_T("("), 0);
		if( nPos >= 0 ) 
		{
			isbn = isbn.Mid(0, nPos);
		}

		m_pKeyDM->SetCellData(_T("BB_낱권_ISBN"), isbn, nIdx);
	}
	else
	{
		for( i=0 ; i<cnt ; i++)
		{
			strTmpData = arrIsbn.GetAt(i);
			nPos = strTmpData.Find(_T("("), 0);
			if( nPos >= 0 )		
			{
				strTmpData = strTmpData.Mid(0, nPos);
			}
			if( isbn.IsEmpty() == TRUE )	isbn = strTmpData;
			else isbn += _T(" ") + strTmpData;
		}
		m_pKeyDM->SetCellData(_T("BB_낱권_ISBN"), isbn, nIdx);
	}

	title = author = publisher = pubyear = isbn = regNo = _T("");

	return nIdx;
}
