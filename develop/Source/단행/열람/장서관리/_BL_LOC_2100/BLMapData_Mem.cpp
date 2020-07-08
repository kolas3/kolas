// BLMapData_Mem.cpp: implementation of the CBLMapData_Mem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BLMapData_Mem.h"

	

void	_BLMapData_Value::SetArrData( CString cstr_arr[] , int c )
{
	int i = 0 ;
	for( i = 0 ; i < c ; i++ )
	{
		arr_str_DMdata.Add( cstr_arr[i] );
	}
}

CString	_BLMapData_Value::GetArrData( int idx )
{
	if( arr_str_DMdata.GetSize() <= idx ) return _T("");
	return arr_str_DMdata[idx];
}

int		_BLMapData_Value::CopyArrData( CString** ppCstr , int c_ppCstr )
{
	int i = 0;
	int c = arr_str_DMdata.GetSize();

	if( c_ppCstr < c )
	{
		c = c_ppCstr;
	}

	for( i = 0 ; i < c ; i++ )
	{
		(*ppCstr)[i] = arr_str_DMdata[i];
	}
	return c;
}


void	_BLMapData_Value::Clear()
{
	arr_str_DMdata.RemoveAll();
}


void	_SortHeader_BLMapData::Clear()
{
	arr_First_Alias.RemoveAll();
	arr_Sort_Alias.RemoveAll();
	arr_Sort_Idx.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBLMapData_Mem::CBLMapData_Mem()
{
	m_pEslMgr_Sort = NULL;
}

CBLMapData_Mem::~CBLMapData_Mem()
{
	Clear();
}

void CBLMapData_Mem::Clear()
{
	m_sortHeader.Clear();

	if( m_Map.GetCount() < 1 ) return;
	GetStartPosition();
	while(true)
	{
		_BLMapData_Value* pD = GetNext();
		if( pD == NULL ) break;
		pD->Clear();
		delete pD;
	}
	m_Map.RemoveAll();
}

_BLMapData_Value*	CBLMapData_Mem::Add(CString _key, CString _value, void*_pData /*, CESL_DataMgr *_pDM, int _idx*/)
{
	_BLMapData_Value*	pDATA = Find( _key );
	if( pDATA == NULL )
	{
		pDATA = new _BLMapData_Value;
	}

	pDATA->str_Key		= _key;
	pDATA->str_DMdata	= _value;
	pDATA->pData		= _pData;
	pDATA->pcBLMapData_Mem = this;

	m_Map[_key] = pDATA;
	return pDATA;
}

_BLMapData_Value*	CBLMapData_Mem::Add( CString _key , CString str_arr[] , int count , int _row_idx)
{
	_BLMapData_Value*	pDATA = Find( _key );
	if( pDATA == NULL ) // 이미 있으면 같은곳에 값만 갱신
	{
		pDATA = new _BLMapData_Value;
	}
	
	pDATA->str_Key		= _key;
	pDATA->row_idx		= _row_idx;
	pDATA->pcBLMapData_Mem = this;
	for( int i = 0 ; i < count ; i++ ) pDATA->arr_str_DMdata.Add( str_arr[i] );
	m_Map[_key] = pDATA;
	return pDATA;
}

//DM 으로부터 데이터 수집
int	CBLMapData_Mem::Add_SelectQuery( CESL_DataMgr*	pDM , CString cstr_Key_Alias  )
{

	int alias_c = SetSort_HeaderAlias( pDM, NULL );
	if( alias_c < 1 ) return -1;
	int key_idx = findSortAlias( cstr_Key_Alias );
	if( key_idx < 0 ) return -1;

	CString* pArData = new CString[alias_c];
	CString* pAlias = NULL;
	GetHeaderAlias_Alloc( &pAlias );

	int i = 0;
	for( i = 0 ; i < pDM->GetRowCount() ; i++)
	{
		CString key_str = pDM->GetCellData( i, key_idx );
		pDM->GetCellData( pAlias , alias_c , pArData , i );
		Add( key_str , pArData , alias_c , i );
	}
	delete [] pAlias;
	delete [] pArData;
	return m_Map.GetCount();
}

// 가지고 있는 데이터를 DM 에 추가
void	CBLMapData_Mem::CopyToDM( CESL_DataMgr*	pDM )
{
	GetStartPosition();
	int alias_c = getHeaderAliasCount();
	CString* pRowData	= new CString[alias_c];

	int o=0;
	int c_arr_data = 0;
	int row_idx = 0;
	while(true)
	{
		_BLMapData_Value*	p =	GetNext();
		if( p == NULL ) 
		{
			break;
		}
		c_arr_data =	p->CopyArrData( &pRowData , alias_c );
		row_idx = pDM->GetRowCount();
		pDM->InsertRow(-1);

		for( o = 0 ; o < c_arr_data ; o++ )
		{
			pDM->SetCellData( row_idx ,o, pRowData[o] );
		}
	}

	delete [] pRowData;

}


int		CBLMapData_Mem::Find( CString _key , CString& _data )
{
	return 0;
}

_BLMapData_Value*	CBLMapData_Mem::Find( CString _key )
{
	_BLMapData_Value*	pDATA = NULL;
	if(	m_Map.Lookup( _key , pDATA ) == FALSE )
	{
		return NULL;
	}
	return pDATA;
}

CArray<CString,CString&>*		CBLMapData_Mem::Find_ArrData( CString _key )
{
	_BLMapData_Value*	pDATA = Find(  _key);
	if(	pDATA == NULL )
	{
		return NULL;
	}
	return &pDATA->arr_str_DMdata;
}

int	CBLMapData_Mem::GetCount()
{
	return m_Map.GetCount();
}

void	CBLMapData_Mem::GetStartPosition()
{
	m_pos = m_Map.GetStartPosition();
}

_BLMapData_Value*	CBLMapData_Mem::GetNext()
{
	CString				key;
	_BLMapData_Value*	pDATA = NULL;

	if( m_pos != NULL )
		m_Map.GetNextAssoc( m_pos , key , pDATA );
	return pDATA;
}

bool	CBLMapData_Mem::GetNextData( CString& cstr_key , CString& cstr_data )
{
	_BLMapData_Value* pD = GetNext();
	if( pD != NULL )
	{
		cstr_key	= pD->str_Key;
		cstr_data	= pD->str_DMdata;
		return true;
	}
	return false;
}

bool	CBLMapData_Mem::GetNextData( CString& cstr_key , CArray<CString,CString&>& arr_cstr )
{
	_BLMapData_Value* pD = GetNext();
	if( pD != NULL )
	{
		cstr_key	= pD->str_Key;
		//arr_cstr	= pD->arr_str_DMdata;
		return true;
	}
	return false;
}

//
// DM 그리드 정렬

void	CBLMapData_Mem::SetSort_HeaderAlias( CString str_arr[] , int count)
{
	m_sortHeader.arr_First_Alias.RemoveAll();
	for( int i = 0 ; i < count ; i++ ) m_sortHeader.arr_First_Alias.Add( str_arr[i] );
}

void	CBLMapData_Mem::SetSort_HeaderAlias( CArray<CString,CString&> &ar_Str )
{
	int c = ar_Str.GetSize();
	if( c < 1 ) return;
	CString* pAr = new CString[c];
	
	int i = 0; 
	for(i=0 ; i < c ; i++)
	{
		pAr[i] = ar_Str[i];
	}
	SetSort_HeaderAlias( pAr , c );
	delete [] pAr;
}

int		CBLMapData_Mem::SetSort_HeaderAlias( CESL_DataMgr*	pDM , CESL_Grid* pGrid )
{
	int c_alias = 0;
	if( pDM != NULL )
	{
		c_alias = pDM->GetColCount();
		if( c_alias < 0 )
		{
			c_alias = pDM->RefList.GetCount();
		}
	}
	else if( pGrid != NULL )
		c_alias = pGrid->GetColCount();

	if( c_alias < 1 )
	{
		return -1;
	}

	CString* alias_str = new CString[c_alias];

	if( pDM != NULL )
	{
		pDM->GetAllAlias(alias_str, c_alias);
	}
	else if( pGrid != NULL )
	{
		POSITION pos;
		int i=0;
		for( pos = pGrid->m_arrColumnInfo.GetHeadPosition() ; pos != NULL ;  )
		{
			CESL_GridColumnInfo* pCol = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetNext(pos);
			alias_str[i] = pCol->strColumnDMFieldAlias;
			i++;
		}

	}
	SetSort_HeaderAlias( alias_str , c_alias );
	delete [] alias_str;

	return c_alias;
}

int		CBLMapData_Mem::getHeaderAliasCount()
{
	return m_sortHeader.arr_First_Alias.GetSize();
}


int 	CBLMapData_Mem::findSortAlias( CString str )
{
	for( int i = 0 ; i < m_sortHeader.arr_First_Alias.GetSize() ; i++ )
	{
		if( str == m_sortHeader.arr_First_Alias[i] ) return i;
	}
	// err
	return -1;
}

int		CBLMapData_Mem::GetHeaderAlias_Alloc( CString** ppAr )
{
	int c = m_sortHeader.arr_First_Alias.GetSize();
	if( c < 1 ) return -1;
	(*ppAr) = new CString[c];
	for( int i = 0 ; i < c ; i++ )
	{
		(*ppAr)[i] = m_sortHeader.arr_First_Alias[i];
	}
	return 1;
}


FP_MultiSort_UserCompear	g_FP_MultiSort_UserCompear = NULL;
int		compear_MapData_Cell( _BLMapData_Value* p1 , _BLMapData_Value* p2 , int idx_alias , int idx_data  )
{
	CString alias;

	CBLMapData_Mem* pMapData = (CBLMapData_Mem*)p1->pcBLMapData_Mem;
	_SortHeader_BLMapData* psh = &pMapData->m_sortHeader;

	alias = psh->arr_First_Alias[idx_alias];

	CString data_1 , data_2;
	CString *p_data_1 = NULL;
	CString *p_data_2 = NULL;

	if( p1->arr_str_DMdata.GetSize() > idx_data )
	{
		data_1 = p1->arr_str_DMdata[idx_data];
		p_data_1 = &data_1;
	}

	if( p2->arr_str_DMdata.GetSize() > idx_data )
	{
		data_2 = p2->arr_str_DMdata[idx_data];
		p_data_2 = &data_2;
	}

	if( g_FP_MultiSort_UserCompear != NULL )
	{
		int r_user = g_FP_MultiSort_UserCompear( alias , p_data_1 , p_data_2 );
		if( r_user == -1 )		return psh->left_sort;
		else if( r_user == 1 )	return psh->right_sort;
		else if( r_user == 0 )	return 0;
	}

	if( p1->arr_str_DMdata.GetSize() <= idx_data ) return psh->right_sort;
	if( p2->arr_str_DMdata.GetSize() <= idx_data ) return psh->left_sort;

	if( p1->arr_str_DMdata[idx_data] == "" ) return psh->right_sort;
	if( p2->arr_str_DMdata[idx_data] == "" ) return psh->left_sort;

	if( p1->arr_str_DMdata[idx_data] < p2->arr_str_DMdata[idx_data] ) 
	{
		return psh->left_sort;
	}
	else if ( p1->arr_str_DMdata[idx_data] > p2->arr_str_DMdata[idx_data] )
	{
		return psh->right_sort;
	}
	return 0;
}



int		qsort_Func_MapData( const void *a1 , const void *a2 )
{
	_BLMapData_Value* p1 = (_BLMapData_Value*)a1;
	_BLMapData_Value* p2 = (_BLMapData_Value*)a2;

	CBLMapData_Mem* pMapData = (CBLMapData_Mem*)p1->pcBLMapData_Mem;
	_SortHeader_BLMapData* psh = &pMapData->m_sortHeader;
	CArray<int,int>	*p_Idx = &psh->arr_Sort_Idx;

	for( int i = 0 ; i < p_Idx->GetSize() ; i++ )
	{
		int r = compear_MapData_Cell( p1 , p2 , i , (*p_Idx)[i] );
		if( r != 0 ) return r;
	}

	return 0;
}

void	CBLMapData_Mem::SetSort_SortAlias( CArray<CString,CString&> &ar_Str , int asc_desc )
{
	int c = ar_Str.GetSize();
	if( c < 1 ) return;
	CString* pAr = new CString[c];
	
	int i = 0; 
	for(i=0 ; i < c ; i++)
	{
		pAr[i] = ar_Str[i];
	}
	SetSort_SortAlias( pAr , c , asc_desc );
	delete [] pAr;	
}

// str_arr :  정렬할 항목 및 순서 
// asc_desc : 오름차순 내림차순
void	CBLMapData_Mem::SetSort_SortAlias( CString str_arr[] , int count , int asc_desc )
{
	m_sortHeader.arr_Sort_Idx.RemoveAll();
	m_sortHeader.arr_Sort_Alias.RemoveAll();
	for( int i = 0 ; i < count ; i++ )
	{
		m_sortHeader.arr_Sort_Alias.Add( str_arr[i] );
		m_sortHeader.arr_Sort_Idx.Add( findSortAlias(str_arr[i]) );
	}

	m_sortHeader.left_sort	= -1;
	m_sortHeader.right_sort =  1;
	if( asc_desc == 1 )
	{
		m_sortHeader.left_sort	=  1;
		m_sortHeader.right_sort = -1;
	}
}

// pDM : 그리드 데이터가 있는 DM
int	CBLMapData_Mem::StartSort( CESL_DataMgr* pDM  , CESL_Grid* pGrid )
{
	int i = 0;
	int o = 0;
	int c_row = 0;

	if( m_sortHeader.arr_First_Alias.GetSize() < 1 || m_sortHeader.arr_Sort_Alias.GetSize() < 1 )
	{
		return -1;
	}

	if( pDM != NULL )
		c_row = pDM->GetRowCount();
	else if( pGrid != NULL )
		c_row = pGrid->GetCount();


	if( c_row < 1 ) 
	{
		return -2;
	}

	_BLMapData_Value *pArr = new _BLMapData_Value[c_row];

	int c_alias = 0;
	if( pDM != NULL )
	{
		c_alias = pDM->GetColCount();
		if( c_alias < 0 )
		{
			c_alias = pDM->RefList.GetCount();
		}
	}
	else if( pGrid != NULL )
		c_alias = pGrid->GetColCount();


	CString* pRowData	= new CString[c_alias];
	CString* alias_str	= new CString[c_alias];

	if( pDM != NULL )
	{
		pDM->GetAllAlias(alias_str, c_alias);
		if( c_alias < 0 )
		{
			c_alias = pDM->RefList.GetCount();
		}
	}
	else if( pGrid != NULL )
	{
		for( i = 0 ; i < c_alias ; i++ )
		{
			pGrid->GetRowInfo( i , alias_str[i] );
		}
	}

	// 데이터 모으기
	for( i = 0 ; i < c_row ; i++ )
	{
		_BLMapData_Value*	pData = &pArr[i];

		if( pDM != NULL )
			pDM->GetCellData( alias_str , c_alias , pRowData , i );
		else if( pGrid != NULL )
		{
			for( o = 0 ; o < c_alias ; o++ )
			{
				pGrid->GetAt( i ,o, pRowData[o] );
			}
		}

		pData->SetArrData( pRowData , c_alias );
		pData->pcBLMapData_Mem = this;
	}
	// 정렬
	qsort( (void*)pArr , c_row , sizeof( _BLMapData_Value ) , qsort_Func_MapData );

	// dm 그리드 채우기

	if( pDM != NULL )
		pDM->FreeData();
	else if( pGrid != NULL )
		pGrid->RemoveAll2();

	for( i = 0 ; i < c_row ; i++ )
	{
		_BLMapData_Value*	pData = &pArr[i];
		for( o = 0 ; o < c_alias ; o++ )
		{
			pRowData[o] = pData->arr_str_DMdata[o];
		}

		if( pDM != NULL )
		{
			pDM->InsertRow(-1);
			for( o = 0 ; o < c_alias ; o++ )
			{
				pDM->SetCellData( i ,o, pRowData[o] );
			}
		}
		else if( pGrid != NULL )
		{
			for( o = 0 ; o < c_alias ; o++ )
			{
				pGrid->SetAt( i ,o, pRowData[o] );
			}
		}
	}

	if( pDM != NULL && m_pEslMgr_Sort != NULL ) 
	{
		m_pEslMgr_Sort->AllControlDisplay( m_strSort_ViewCM );
	}

	for( i = 0 ; i < c_row ; i++ )
	{
		_BLMapData_Value*	pData = &pArr[i];
		pData->Clear();
	}

	delete [] alias_str;
	delete [] pRowData;
	delete [] pArr;

	return c_row;
}

void	CBLMapData_Mem::SetFunc_FP_MultiSort_UserCompear( FP_MultiSort_UserCompear p )
{
	g_FP_MultiSort_UserCompear = p;
}


// 
__declspec(dllexport)
CStdioFile* _OpenFileWrite_UniText( CString path )
{
	CStdioFile* pFile = new CStdioFile();

	if (!pFile->Open(path, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		delete pFile;
		return NULL;
	}
	if( 0 == pFile->GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		pFile->Write( &cUni, sizeof(TCHAR));
	}
	return pFile;
}


__declspec(dllexport)
CStdioFile* _OpenFileRead_UniText( CString path )
{
	CStdioFile* pFile = new CStdioFile();

	if (!pFile->Open(path, CFile::modeRead | CFile::typeBinary))
	{
		if (!pFile->Open(path, CFile::modeRead | CFile::modeCreate | CFile::typeBinary))
		{
			delete pFile;
			return NULL;
		}
		else
		{
			if( 0 == pFile->GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				pFile->Write( &cUni, sizeof(TCHAR));
			}
		}
	}
	TCHAR cUni;
	pFile->Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		pFile->SeekToBegin();
	}

	return pFile;
}