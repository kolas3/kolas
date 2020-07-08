// DupKeyMakerByGrid.cpp: implementation of the CDupKeyMakerByGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMakerByGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDupKeyMakerByGrid::CDupKeyMakerByGrid(CESL_Mgr * pMgr) : CDupKeyMaker(pMgr)
{
	m_pSourceDM = NULL;
	m_pSourceGrid = NULL;
}

CDupKeyMakerByGrid::~CDupKeyMakerByGrid()
{

}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - KEY DM 구축
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::MakeKeyDM()
{
	//================================================================
	// 변수정의
	//================================================================
	INT i, ids, rowCnt, cnt, nPos;	
	CString strMarc, title, author, publisher, pubyear, isbn, strTmpData, regNo;
	CArray<CString, CString&> arrIsbn;
	CMarc marc;

	if( m_pKeyDM		== NULL	|| 
		m_pSourceDM		== NULL || 
		m_pSourceGrid	== NULL	)	return -1;

	//================================================================
	// 1. 그리드 선정 자료들의 렉키를 멤버변수에 저장한다. ( m_ArrRecKey )
	//================================================================
	ids = GetRecKeyList();
	if( ids < 0 )	return -1;

	//================================================================
	// 2. m_pKeyDM을 구축한다.
	//================================================================
	ids = RefreshAndMergeKeyDM();
	if( ids < 0 )	return -1;

	//================================================================
	// 3. 구축된 DM의 마크정보를 이용하여 KEYDM의 내용을 재구축한다.
	//================================================================
	rowCnt	= m_pKeyDM->GetRowCount();
	cnt		= nPos = -1;
	for( i=rowCnt-1; i>=0 ; i-- )
	{
		m_pKeyDM->GetCellData(_T("BS_MARC"), i, strMarc);
		if( strMarc.GetLength() == 0 )	continue;
		ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if( ids < 0 )	continue;

		ids = ExtractKeyByMarc(&marc, i);
		if( ids < 0 )	m_pKeyDM->DeleteRow(i);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 그리드에 선정된 자료들의 렉키를 멤버변수에 저장한다.
// [ 미니스펙 ]
// - 그리드 선정된 자료들의 렉키(I.REC_KEY)를 가져와 멤버변수 렉키리스트에 저장한다.
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::GetRecKeyList()
{
	//================================================================
	// 변수정의
	//================================================================
	INT i, ids, count;
	CString strKey, strInquery;
	count = 0;	
	strKey.Empty();	strInquery.Empty();


	//================================================================
	// 그리드 선정된 자료들의 렉키(I.REC_KEY)를 가져와 멤버변수 렉키리스트에 저장한다.
	//================================================================
	ids = m_pSourceGrid->SelectMakeList();
	if( ids < 0 )	return -1;
	i = m_pSourceGrid->SelectGetHeadPosition();
	if( i < 0 )		return -1;
	
	while( i >= 0 )
	{
		if( count == 600 )
		{
			strInquery = _T("(") + strInquery + _T(")");
			m_ArrRecKey.Add(strInquery);
			strInquery.Empty();
			count = 0;
		}

		// 색인 렉키가 없다면 종렉키를 가져온다.
		m_pSourceDM->GetCellData(_T("IBS_종KEY"), i, strKey);
		if( strKey.IsEmpty() == TRUE )		m_pSourceDM->GetCellData(_T("BS_종KEY"), i, strKey);		

		if( strInquery.IsEmpty() == TRUE )	strInquery = strKey;
		else strInquery += _T(", ") + strKey;
		
		i = m_pSourceGrid->SelectGetNext();
		count++;
		strKey.Empty();
	}
	strInquery = _T("(") + strInquery + _T(")");
	m_ArrRecKey.Add(strInquery);

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 키DM을 구축한다.
// [ 미니스펙 ]
// 1. m_ArrDataMgr(DM저장변수)의 내용을 지운다.
// 2. m_ArrDataMgr를 구축한다.(Key DM 구축)
// 3. 구축된 KeyDM을 모두 합하여 m_pKeyDM으로 구축한다.
// 4. m_ArrDataMgr(DM저장변수)의 내용을 지운다.
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::RefreshAndMergeKeyDM()
{
	//================================================================
	// 변수정의
	//================================================================
	INT i, ids, cnt;
	CString strDMPK, strQuery, strKey;
	CESL_DataMgr* pDM = NULL;
	CESL_DataMgr TmpDM ;

	cnt = m_ArrRecKey.GetSize();
	if( cnt == 0 )	return -1;
	
	//================================================================
	// 1. m_ArrDataMgr(DM저장변수)의 내용을 지운다.
	//	- 600개 단위로 DM을 구축한다.(IN함수)
	//================================================================
	RemoveDataMgrArray();

	//================================================================
	// 2. m_ArrDataMgr를 구축한다.(Key DM 구축)
	//	- 저장된 렉키들을 이용하여 Key DM을 구축하여 멤버변수로 저장한다.
	//================================================================
	for( i=0 ; i<cnt ; i++ )
	{
		// DM 초기화
		pDM = new CESL_DataMgr;
		pDM->SetCONNECTION_INFO(m_pKeyDM->CONNECTION_INFO);
		pDM->InitDMPKFromFile(m_pKeyDM->DB_MGR_ALIAS);
		pDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

		strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN %s"), m_ArrRecKey.GetAt(i));
		
		ids = pDM->RefreshDataManager(strQuery);
		if( ids < 0 )	return -1;
		m_ArrDataMgr.Add(pDM);
	}
	
	//================================================================
	// 3. 구축된 KeyDM을 모두 합하여 m_pKeyDM으로 구축한다.
	//================================================================
	TmpDM.InitDMPKFromFile(m_pKeyDM->DB_MGR_ALIAS);
	TmpDM.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	cnt = m_ArrDataMgr.GetSize();
	m_pKeyDM->FreeData();
	for( i=0 ; i<cnt ; i++ )
	{
		AddToTargetDM(&TmpDM, m_ArrDataMgr.GetAt(i), -1);
	}

	// 2006.03.16 KBS - KeyDM을 그리드의 선정된 순서대로 정렬하여 구축한다.(국방대요구사항)
	//================================================================
	INT col, nCols, nCol, left, right, mid, nKey, nTmpKey;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpKey;
	ids = m_pSourceGrid->SelectMakeList();
	if( ids < 0 )	return -1;
	i = m_pSourceGrid->SelectGetHeadPosition();
	if( i < 0 )		return -1;
	// 종키 이진검색을 위해 정렬한다.
	TmpDM.SORT( _T("IBS_종KEY") );
	while( i >= 0 )
	{
		m_pSourceDM->GetCellData(_T("IBS_종KEY"), i, strKey);
		left	= 0; 
		right	= TmpDM.GetRowCount();
		while( right >= left )
		{
			mid = (right+left) / 2;
			strTmpKey = TmpDM.GetCellData(_T("IBS_종KEY"), mid);
			if( strKey == strTmpKey )
			{
				m_pKeyDM->InsertRow(-1);
				nCols = TmpDM.m_nCols;
				if( nCols < 0 )		nCols = TmpDM.GetRefColumnCount();
				for( col=0; col<nCols; col++ )
				{
					if( TmpDM.GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0 )	continue;

					nCol = m_pKeyDM->FindColumn(FIELD_ALIAS);			
					m_pKeyDM->SetCellData(m_pKeyDM->GetRowCount()-1, nCol, TmpDM.GetCellData(mid, col));
				}
				break;
			}
			else
			{
				nKey	= _ttoi(strKey	);
				nTmpKey = _ttoi(strTmpKey);
				if( nKey > nTmpKey )	left  = mid + 1;
				else					right = mid - 1;
			}
		}
		//  찾지 못했다면 그냥 패스한다. ( 에러처리 )		
		/*
		for( j=0; j<TmpDM.GetRowCount(); j++ )
	{
			if( strKey == TmpDM.GetCellData(_T("IBS_종KEY"), j) )	break;
	}
		if( j > TmpDM.GetRowCount() )		continue;
		*/

		i = m_pSourceGrid->SelectGetNext();
	}	
	//================================================================


	//================================================================
	// 4. m_ArrDataMgr(DM저장변수)의 내용을 지운다.
	//================================================================
	RemoveDataMgrArray();
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - pSourceDM의 내용을  pTargetDM idx번째에 복사한다. 
// [ 미니스펙 ] - 생략
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::AddToTargetDM(CESL_DataMgr *pTargetDM, 
									  CESL_DataMgr *pSourceDM, 
									  INT idx)
{
	if( pTargetDM == NULL || pSourceDM == NULL )	return -1;
	
	//===============================================================
	// 변수정의
	//===============================================================
	INT row, col, nRow, nCurRow, nCol, nCols, FirstRow;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	CMarc marc;
	
	FirstRow = -1;
	
	// 사용할 필드 정보를 구축한다.
	nRow = pSourceDM->GetRowCount();	
	if( nRow == 0 )		return 0;
	
	for( row=0; row<nRow; row++ )
	{
		pTargetDM->InsertRow(idx);
		if( idx >= 0 )	nCurRow = idx;
		else			nCurRow = pTargetDM->GetRowCount() - 1;

		if( FirstRow < 0 )	FirstRow = nCurRow;

		nCols = pSourceDM->m_nCols;
		if( nCols < 0 )		nCols = pSourceDM->GetRefColumnCount();

		for( col=0; col<nCols; col++ )
		{
			if( pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0 )	continue;

			nCol = pTargetDM->FindColumn(FIELD_ALIAS);
			pTargetDM->SetCellData(nCurRow, nCol, pSourceDM->GetCellData(row, col));
		}
	}

	return FirstRow;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - DM이 저장된 멤버변수의 내용을 비운다.
// [ 미니스펙 ] - 생략
//////////////////////////////////////////////////////////////////////
INT CDupKeyMakerByGrid::RemoveDataMgrArray()
{
	INT cnt = m_ArrDataMgr.GetSize();
	for( INT i=0 ; i<cnt ; i++ )
	{
		delete m_ArrDataMgr.GetAt(i);
	}
	m_ArrDataMgr.RemoveAll();

	return 0;
}
