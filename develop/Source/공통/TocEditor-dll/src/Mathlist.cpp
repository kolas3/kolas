// Mathlist.cpp : implementation file
//

#include "stdafx.h"
#include "Mathlist.h"

CMathlist::CMathlist()
{
	m_nMaxMathCnt		= 0;
//	m_mathList = (_MATHLIST *)malloc( sizeof(_MATHLIST) );
//	m_mathList[0].m_bitmap = new CBitmap();
//	m_mathList[0].m_strFilename = new CString();
//	m_mathList[0].nIndex = -1;
	Init();

	// KOL.UDF.022 시큐어코딩 보완
	m_mathList = NULL;
	m_nLastMathCnt = 0;
}

CMathlist::~CMathlist()
{
}

/*
CBitmap* CMathlist::AddMath(INT m_nMath)
{
	INT i = 0;
//	m_mathList = (_MATHLIST *)realloc( m_mathList, sizeof(_MATHLIST) * (m_nMaxMathCnt + 1) );
//	m_mathList[m_nMaxMathCnt].m_bitmap = new CBitmap();
	// 삽입시의 예외처리..
//	if(GetMath(m_nMath)) {
//	}

	while(m_mathList[i].nIndex >= 0) {
		i++;
//		if(i >



		if(m_nMaxMathCnt < i + 1) {
			m_mathList = (_MATHLIST *)realloc( m_mathList, sizeof(_MATHLIST) * (m_nMaxMathCnt + 1) );
			m_mathList[i].m_bitmap = new CBitmap();
			m_mathList[i].m_strFilename = new CString();
			m_mathList[i].nIndex = -1;
		}
		
	}
	
	m_mathList[i].nIndex = m_nMath;
//	m_mathList[i].nIndex = m_nMaxMathCnt;
	m_nMaxMathCnt++;
	return m_mathList[i].m_bitmap;
}

INT CMathlist::DeleteMath(INT m_nMath)
{
	INT i=0;
	while(m_mathList[i].nIndex == m_nMath) {
		i++;
		if(i > m_nMath) break;
	}
		
	delete m_mathList[i].m_bitmap;
	delete m_mathList[i].m_strFilename;
	m_mathList[i].nIndex = -1;

	i=0;
	while(m_mathList[i].nIndex > m_nMath) {
		m_mathList[i].nIndex--;
		i++;
		if(i > m_nMath) break;
	}

	if(m_nMath >= m_nMaxMathCnt)
		m_nMaxMathCnt--;
	m_mathList = (_MATHLIST *)realloc( m_mathList, sizeof(_MATHLIST) * (m_nMaxMathCnt + 1) );
	return 0;
}
*/
CBitmap* CMathlist::GetMath(INT m_nMath)
{
	_MATHLIST *s;
	s = findMath(m_nMath);
	if(IsTrue(s)) return s->m_bitmap;
	else return NULL;
}

BOOL CMathlist::IsTrue(_MATHLIST *p)
{
	if(p->nIndex > 0) return TRUE;
	else return FALSE;

	return TRUE;
}
/*
INT CMathlist::GetMaxCnt() {
	REOBJECT reobj;
	ZeroMemory(&reobj, sizeof(REOBJECT));
	reobj.cbStruct = sizeof(REOBJECT);

	UINT i=0;
	while(!FAILED(m_pRichEditOle->GetObject( i, &reobj, REO_GETOBJ_POLEOBJ)) )
		i++;
	
	return i;
}
*/

VOID CMathlist::Init(VOID)
{
	head = (_MATHLIST*)malloc(sizeof(_MATHLIST));
	tail = (_MATHLIST*)malloc(sizeof(_MATHLIST));
	head->next = tail;
	head->prev = head;
	tail->next = tail;
	tail->prev = head;
//	nHeight = nIndex = 0;
}

/*
_MATHLIST *CMathlist::InsertMath(INT m_nMath, _MATHLIST* pMath)
{
	_MATHLIST *pMathIns;
	if(pMath == head)
		return NULL;
	pMathIns = (_MATHLIST*)malloc(sizeof(_MATHLIST));

	pMathIns->nIndex = m_nMath;
	pMath->prev->next = pMathIns;
	pMathIns->prev = pMath->prev;
	pMath->prev = pMathIns;
	pMathIns->next = pMath;
	return pMathIns;
}

INT CMathlist::deleteMath(_MATHLIST* p)
{
	if(p == head || p == tail) return 0;
	p->prev->next = p->next;
	p->next->prev = p->prev;
	free(p);
	return 1;
}
*/
INT CMathlist::GetMathIndex(INT m_nIndex)
{
	_MATHLIST *p;
	p = findMath(m_nIndex);
	return p->nIndex;
}

_MATHLIST *CMathlist::findMath(INT k)
{
	_MATHLIST *s;
	s = head->next;
	while(s->nIndex != k && s!=tail)
		s=s->next;
	return s;
}

CBitmap *CMathlist::InsertMath(INT k, INT nHeight)
//CBitmap *CMathlist::InsertMath(INT k, INT nHeight, INT nWidth)
{
	_MATHLIST *s;
	_MATHLIST *i = NULL;

	s = findMath(k);
//	if(s != tail) {
	i=(_MATHLIST*)(malloc(sizeof(_MATHLIST)));

	//node에 값 넣기..
	i->nIndex = k;
	i->nHeight = nHeight;
//	i->nWidth = nWidth;
	i->m_bitmap = new CBitmap();
	i->m_strFilename = new CString();

	i->prev = s->prev;
	s->prev->next = i;
	i->next = s;
	s->prev = i;

//	s = s->next;
	s=i;
	while(s!=tail) {
		s=s->next;
		if(s->nIndex >= k) s->nIndex++;
//		s=s->next;
	}
	return i->m_bitmap;

}
/*
CBitmap *CMathlist::InsertMath(INT k, INT t)
{
	_MATHLIST *s;
	_MATHLIST *i = NULL;

	s = findMath(t);
	if(s != tail) {
		i=(_MATHLIST*)(malloc(sizeof(_MATHLIST)));

		//node에 값 넣기..
		i->nIndex = k;
		i->m_bitmap = new CBitmap();
		i->m_strFilename = new CString();

		s->prev->next = i;
		i->prev = s->prev;
		s->prev = i;
		i->next = s;
//		return i->m_bitmap;
	}
	else {
		i=(_MATHLIST*)(malloc(sizeof(_MATHLIST)));

		//node에 값 넣기..
		i->nIndex = k;
		i->m_bitmap = new CBitmap();
		i->m_strFilename = new CString();

		s->prev->next = i;
		i->prev = s->prev;
		s->prev = i;
		i->next = s;
	}
	return i->m_bitmap;
//	return NULL;
}
*/

INT CMathlist::deleteMath(INT k)
{
	_MATHLIST *s, *pbak;
	s = findMath(k);
	if( s != tail ) {
		s->prev->next = s->next;
		s->next->prev = s->prev;

		// node 지우기..
		delete s->m_bitmap;
		delete s->m_strFilename;
			
		pbak = s;  s = s->next;
		while(s!=tail) {
			if(s->nIndex > k) s->nIndex--;
			s=s->next;
		}
		free(pbak);
		return 1;
	}
	return 0;
}

VOID CMathlist::deleteAll()
{
	for(INT i=m_nLastMathCnt;i>0;i--)
		deleteMath(i);
}