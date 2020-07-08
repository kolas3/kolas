// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ArrayConditionQuery.cpp: implementation of the CArrayConditionQuery class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ArrayConditionQuery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString			CArrayConditionQuery::m_cstr_Where = _T("");
CESL_DataMgr*	CArrayConditionQuery::m_pDM_Recent_Select = NULL;

CArrayConditionQuery::CArrayConditionQuery()
{

}

CArrayConditionQuery::~CArrayConditionQuery()
{

}

VOID CArrayConditionQuery::SetCONNECTION_INFO(CString info)
{
	m_cDM.SetCONNECTION_INFO( info );
}

void CArrayConditionQuery::SetManageCode( CString strManageCode )
{
	m_cDM.SetManageCode( strManageCode );
}

/*
void CArrayConditionQuery::SetDefault()
{
	// 가장 최근에 했던 쿼리의 Table 을 세팅
	if( m_pDM_Recent_Select == NULL ) return;
	m_sqlTable = m_pDM_Recent_Select->TBL_NAME;
	m_cDM.SetCONNECTION_INFO( m_pDM_Recent_Select->CONNECTION_INFO );
}



void CArrayConditionQuery::ClearWhere()
{
	m_arWhere_X.RemoveAll();
	m_arWhere_Y.RemoveAll();
}


CStringArray*	CArrayConditionQuery::getAxisWhere( int type )
{
	if( type == 0  )		return &m_arWhere_X;
	else if( type == 1 )	return &m_arWhere_Y;
	return NULL;
}

int	CArrayConditionQuery::AddWhere( int type , CString cstr )
{
	CStringArray* whereArr = getAxisWhere( type );
	CString str;
	str = _T("");
	if( cstr.IsEmpty() == FALSE )
	{
		str.Format( _T(" AND %s ") , cstr );
	}
	return	whereArr->Add( str );
}

int	CArrayConditionQuery::AddWhere( int type , CString cstr_Column , CString cstr_Val )
{
	CString str;
	str = _T("");
	if( cstr_Column.IsEmpty() == FALSE && cstr_Val.IsEmpty() == FALSE )
	{
		str.Format( _T(" %s = '%s'") , cstr_Column , cstr_Val );
	}
	return AddWhere( type , str );
}
*/


void CArrayConditionQuery::Remove_Query_GROUP_BY()
{
	int find =	m_cstr_Where.Find( _T("GROUP BY") );
	if( find >= 0 )
	{
		m_cstr_Where = m_cstr_Where.Left( find );
	}
}

int	CArrayConditionQuery::findLastWord( CString str ,  CString word )
{
	int find_start = 0;
	int find_cur = -1;
	while( true )
	{
		if( find_start >= str.GetLength() )	
		{
			return find_cur;
		}

		int f = str.Find( word , find_start );
		if( f >= 0 )
		{
			find_cur = f;
			find_start = f + 1;
		}else{
			return find_cur;
		}
	}
}

void CArrayConditionQuery::Remove_AND_Last( CString& str  )
{
	//int find =	m_cstr_Where.ReverseFind( _T("AND") );
	int find =	findLastWord(m_cstr_Where , _T("AND") );
	if( find >= 0 )
	{
		m_cstr_Where = m_cstr_Where.Left( find );
	}
}

// 가장 뒤쪽에서 찾은 word 의 문자열과 가장 가까운 앞의 "AND" 를 삭제
// 지금은 임시로 찾으면 그냥 맨뒤 삭제
void CArrayConditionQuery::Remove_AND_NearLastWord( CString& str , CString word )
{
	while(true)
	{
		if( findLastWord( str , word ) >= 0 )
		{
			Remove_AND_Last( str );
		}else{
			break;
		}
	}
}

/*
int	CArrayConditionQuery::MakeQuery(int x , int y )
{
	if( m_arWhere_X.GetSize() <= x || m_arWhere_Y.GetSize() <= y )
	{
		//err
		return -1;
	}
	m_sqlRun.Format( _T("SELECT %s FROM %s WHERE %s %s %s") , m_sqlSelectColumn , m_sqlTable , m_cstr_Where , m_arWhere_X[x] , m_arWhere_Y[y] );
	return 1;
}

int	CArrayConditionQuery::RunQuery( int custom_x , int custom_y )
{
	CString cstr = m_sqlRun;
	if( custom_x >= 0 && custom_y >= 0 )
	{
		cstr = m_cDM_CustomSQL.GetCellData( custom_y , custom_x );
	}
	int ids = m_cDM.RestructDataManager( cstr );
	return ids;
}
*/

int	CArrayConditionQuery::RunQuery( CString cstr )
{
	int ids = m_cDM.RestructDataManager( cstr );
	return ids;
}

int	CArrayConditionQuery::FillGrid( CESL_Grid* pGrid )
{
	int c_alias = 0;
	c_alias = m_cDM.GetColCount();
	if( c_alias < 0 )
	{
		c_alias = m_cDM.RefList.GetCount();
	}

	if( pGrid->GetColCount() < c_alias )
	{
		c_alias = pGrid->GetColCount();
	}
	
	CString* pRowData	= new CString[c_alias];
	CString* alias_str	= new CString[c_alias];

	m_cDM.GetAllAlias(alias_str, c_alias);

	int i=0;
	int o=0;
	int	c_row = m_cDM.GetRowCount();
	pGrid->RemoveAll2();


	for( i = 0 ; i < c_row ; i++ )
	{
		pRowData[0] =	m_cDM.GetCellData( i , 0 );
		pRowData[1] =	m_cDM.GetCellData( i , 1 );

		for( o = 0 ; o < c_alias ; o++ )
		{
			pGrid->SetAt( i ,o, pRowData[o] );
		}
	}
	
	pGrid->DisplayNumber();

	delete [] pRowData;
	delete [] alias_str;

	return 1;
}

/*
int	CArrayConditionQuery::CodeMgr_FillAxis( CESL_Mgr* pMgr , int type , CString codeField , CString codeDesc )
{
	CESL_CodeMgr *pCodeMgr = pMgr->m_pInfo->pCodeMgr;

	int i;
	INT cnt = pCodeMgr->GetCodeCount(codeDesc);
	INT nStartIndex = 0;

	for( i = 0 ; i < cnt ; i++ )
	{
		CString strCode;
		nStartIndex = pCodeMgr->GetCode( codeDesc , _T("코드"), strCode, nStartIndex);
		AddWhere( type , codeField , strCode );
	}
	return cnt;
}

void CArrayConditionQuery::ClearCustomSQL()
{
	m_cDM_CustomSQL.FreeData();
}

void CArrayConditionQuery::SetCustomSQL( int x , int y , CString cstr_custom )
{
	// 컬럼 x , Row y
	m_cDM_CustomSQL.SetCellData( y , x , cstr_custom );
}
*/

// KOL.RED.2018.211 ---------------------------------------------------------------------------