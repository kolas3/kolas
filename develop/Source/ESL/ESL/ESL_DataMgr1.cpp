
#include "stdafx.h"
#include "ESL_DataMgr.h"
#include "ECO_SearchMgrApi.h"

#include "ESL_SearchEngine.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ESLDlg_Error.h"
#include "ESL_ControlMgr.h"
#include "efs.h"
#include "_excelmancontrol.h"
#include "ExcelManager.h"
#include "FWDES.h"

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

CString CESL_DataMgr::GetECOSEARCHReplaceString_WebSearch(CString strField, CString strFieldTable, CString str)
{
	strFieldTable.MakeUpper();
	CString strToken, strTmp, strTmpToken;
	CString strWhere = _T("");
	CString strResult;
	INT nPosPrev=-1, nPosNext=-1;

	while(true) 
	{
		INT nTemp[2] , nFind;
		str.Replace(_T(" | "), _T("|"));
		nTemp[0] = str.Find('|', nPosPrev+1);
		nTemp[1] = str.Find(' ', nPosPrev+1);
		if(-1==nTemp[0] && -1==nTemp[1])
		{
			nFind = 0;
			strTmpToken = str.Mid(nPosPrev+1);
			nPosNext = -1;			
		}
		else if(-1!=nTemp[0] && -1!=nTemp[1])
		{
			if(nTemp[0]<nTemp[1])
			{
				nFind = 1;
				nPosNext = nTemp[0];
			}
			else
			{
				nFind = 2;
				nPosNext = nTemp[1];
			}
			strTmpToken = str.Mid(nPosPrev+1, nPosNext-nPosPrev-1);
		}
		else if(-1!=nTemp[0])
		{
			nFind = 1;
			nPosNext = nTemp[0];
			strTmpToken = str.Mid(nPosPrev+1, nPosNext-nPosPrev-1);
		}
		else if(-1!=nTemp[1])
		{
			nFind = 2;
			nPosNext = nTemp[1];
			strTmpToken = str.Mid(nPosPrev+1, nPosNext-nPosPrev-1);
		}

		if( strTmpToken.Find('*') > -1 ) 
		{
			strTmpToken.Insert( 0, _T('*') );
		}

		strToken = strTmpToken;

		if (strWhere.GetLength() == 0) 
		{
			if(0==nFind) strWhere = strToken;
			else if(1==nFind) strWhere.Format(_T("%s OR"), strToken);
			else if(2==nFind) strWhere.Format(_T("%s AND"), strToken);
		} 
		else 
		{
			strTmpToken = strWhere;
			if(0==nFind) strWhere.Format(_T("%s %s"), strTmpToken, strToken);
			else if(1==nFind) strWhere.Format(_T("%s %s OR"), strTmpToken, strToken);
			else if(2==nFind) strWhere.Format(_T("%s %s AND"), strTmpToken, strToken);
		}
		if (nPosNext == -1) break;
		nPosPrev = nPosNext;
	}

	CString url_table , web_field , searchType;

	if(	CESL_SearchEngine::GetSearchMode() == 0 )
	{
		searchType = _T("_CTX");
	}else{
		searchType = _T("_KO");
	}

	int ids = GetECOSEARCHIndexTableName_WebSearch( strFieldTable , searchType , web_field , url_table );

	if( ids < 0 )
	{
		return _T("");
	}

	strTmp = strWhere;
	strWhere.Format( _T("( %s )") ,  strTmp );

	CString			result_rec_key;
	CStringArray	RecKeys;

	CString		log;

	log.Format( _T(" Send--->>> %s : %s : %s : %s \n"), strField , url_table , web_field , strWhere );
	OutputDebugString( log );

	ids = CESL_SearchEngine::RequestSelectQuery( url_table  , web_field , strWhere , RecKeys );
	if( ids < 0 )
	{
		log.Format( _T("%d") ,  ids);
		AfxMessageBox(_T("CESL_SearchEngine::RequestSelectQuery 에러 : ") + log  );
		return _T("");
	}

	log.Format( _T("<<---- RecKeys : %d \n") , RecKeys.GetSize() );
	OutputDebugString( log );

	CString r_val;

	if( RecKeys.GetSize() > 0 )
	{
		int i=0;
		for( i = 0 ; i < RecKeys.GetSize() ; i++)
		{
			if( i > 0 )result_rec_key += _T(",");
			result_rec_key += RecKeys[i];
		}
	}else{
		result_rec_key.Format( _T("'-1'") );
	}

	if (strField.GetLength() == 0) 
	{
	    if(m_dbkind == 1 || m_dbkind == 6)
		{
			strResult.Format(_T(" ( %s ) ") , result_rec_key );
		}
		else if(m_dbkind == 2)
		{
			strResult.Format(_T(" %s ") , result_rec_key );
		}
	} else {
		strResult.Format(_T("%s IN ( %s )"), strField, result_rec_key );
	}

	log.Format( _T("###### Web Search Result : ") + strResult + _T("\n") );
	OutputDebugString( log );
	return strResult;

}

int CESL_DataMgr::GetECOSEARCHIndexTableName_WebSearch( CString strFieldTable, CString &searchType , CString &web_field  , CString &url_table )
{
	CStdioFile* pFile = _OpenFileRead_UniText( "..\\CFG\\웹서치_IDXTABLE_FIELD.cfg" );
	if( pFile == NULL )
	{
		AfxMessageBox(_T("..\\CFG\\웹서치_IDXTABLE_FIELD를 열 수 없습니다."));
		return -1;
	}

	int result = -1;
	CString cstr;
	while( pFile->ReadString(cstr) )
	{
		OutputDebugString( cstr + _T("\n") );

		CString		table;
		AfxExtractSubString(table		, cstr, 0 , ',');
		AfxExtractSubString(web_field	, cstr, 1 , ',');
		AfxExtractSubString(url_table	, cstr, 2 , ',');

		if( table == strFieldTable ) 
		{
			table.TrimRight();
			web_field.TrimRight();
			url_table.TrimRight();

			result = 1;
			break;
		}

		if( table.GetLength() < 1 ) break;
	}

	if( result == 1 )
	{
		if( url_table.GetLength() > 1 )
		{
			if( web_field.Right( 4 ) != _T("_CTX") &&
				web_field.Right( 3 ) != _T("_KO") ) 
			{
				web_field += searchType;
			}
		}else{
			result = -2;
			AfxMessageBox(_T("..\\CFG\\웹서치_IDXTABLE_FIELD 에 연결된 웹필드가 없습니다. : ") + strFieldTable);
		}
	}else{
		result = -3;
		AfxMessageBox(_T("..\\CFG\\웹서치_IDXTABLE_FIELD 에 항목이 없습니다. : ") + strFieldTable );
	}

	pFile->Close();
	delete pFile;
	return result;
}

int		CESL_DataMgr::WebECOSEARCHR_Replace( CString& str )
{
	int ids;
	while( true )
	{
		ids = WebECOSEARCHR_Replace_InWord( str , _T("REC_KEY IN") , _T("INTERSECT") );

		if( ids != 1 )
		{
			break;
		}
	}

	int start_pos = 0;
	while(true)
	{
		ids =	WebECOSEARCHR_Replace_Split( str , _T("REC_KEY IN") , start_pos , 1000 );
		if( ids != 1 )
		{
			break;
		}
	}

	start_pos = 0;
	while(true)
	{
		ids =	RemoveDuplBracket_Prc( str , _T("REC_KEY IN") , start_pos );
		if( ids != 1 )
		{
			break;
		}
	}
	return ids;
}

int	_findLastWord( CString str ,  CString word , int last_pos )
{
	int find_start = 0;
	int find_cur = -1;
	while( true )
	{
		if( find_start >= str.GetLength() )	
			return find_cur;
		
		int f = str.Find( word , find_start );
		if( f >= 0 )
		{
			if( last_pos > -1 && f >= last_pos )
				return find_cur;
				
			find_cur = f;
			find_start = f + 1;
		}else{
			return find_cur;
		}
	}
}

int	_countCharInString( CString str , _TCHAR tch , int start , int end )
{
	CString ts = str.Mid( start , end - start );
	return ts.Remove( tch );
}

int _replaceMid_String( CString& str , CString word_old , CString word_new , int start_pos )
{
	int fs = str.Find( word_old , start_pos );
	if( fs == -1 )return -1;
	int right_start = fs + word_old.GetLength() + 1;
	CString str_left = str.Left( fs );
	CString str_right = str.Right( str.GetLength() - right_start );
	str = str_left + word_new + str_right;
	return 1;
}

int		_find_BracketPair( CString str , int start_pos , int& first , int& last )
{
	first = last = -1;
	int depth=0;
	int in=-1 , out=-1;
	while( true )
	{
		if( in>-1 || out>-1 )
			if( depth <= 0 )
				break;

		int _in	= str.Find( _T('(') , start_pos );
		int	_out = str.Find( _T(')') , start_pos );

		if( in == -1 && _in > -1 )
		{
			first = _in;
		}
		last = _out;

		in = _in;
		out = _out;

		if( in == -1 && out == -1 ) break;

		if( in != -1 && out == -1 )
		{
			depth++;
			start_pos=in+1;
			continue;
		}

		if( in == -1 && out != -1 )
		{
			depth--;
			start_pos=out+1;
			continue;
		}

		if( in < out )
		{
			depth++;
			start_pos=in+1;
			continue;
		}

		if( out < in )
		{
			depth--;
			start_pos=out+1;
			continue;
		}
	}

	if( depth == 0 )
	{
		return 1;
	}

	return -1;
}

int		CESL_DataMgr::WebECOSEARCHR_Replace_InWord( CString& str , CString mark , CString in_word )
{
	int		startFind = 0;
	CArray<int,int&>	arr_FindPos;
	CArray<int,int&>	arr_Depth;

	while(true)
	{
		int count_in=0 , count_out=0 , depth=0;
		if( WebECOSEARCHR_Replace_FindTwoChar( str , mark , in_word , startFind , count_in , count_out , depth ) == -1 )
		{
			break;
		}
		if( count_in >= 0 )
		{
			if( (count_in - count_out) == 1 )
			{
				arr_FindPos.Add( startFind );
				arr_Depth.Add( depth );
			}
		}
		startFind++;
	}

	if( arr_FindPos.GetSize() < 1 ) return 0;

	int find_pos = -1;
	int find_depth = -1;

	int i=0;
	for( i=0 ; i<arr_Depth.GetSize() ; i++ )
	{
		if( find_depth < arr_Depth[i] )
		{
			find_pos = arr_FindPos[i];
			find_depth = arr_Depth[i];
		}
	}

	int ids = 0;
	ids = _replaceMid_String( str , in_word , _T(",") , find_pos );

	if( ids < 0 ) return -1;

	find_pos = _findLastWord( str , mark , find_pos );
	int Bracket_first, Bracket_last;
	ids = _find_BracketPair( str , find_pos , Bracket_first , Bracket_last );
	if( ids < 0 ) return -1;

	if( Bracket_first == -1 && Bracket_last == -1 ) 
	{
		return 1;
	}

	Bracket_first += 1;
	Bracket_last -=1;

	CString str_left  = str.Left( Bracket_first );

	CString str_right = str.Right( str.GetLength() - Bracket_last );

	CString str_mid = str.Mid( Bracket_first , Bracket_last - Bracket_first );

	str_mid.Replace( _T("(") , _T("") );
	str_mid.Replace( _T(")") , _T("") );
	str = str_left + str_mid + str_right;
	return 1;
}

int		CESL_DataMgr::WebECOSEARCHR_Replace_FindTwoChar( CString str , CString mark , CString in_word , int& start_pos , int& count_in , int& count_out , int& depth )
{

	int f1 = str.Find( in_word , start_pos );
	if( f1 == -1 ) return -1;
	
	int f2 = _findLastWord( str , mark , start_pos - 1 );
	if( f2 == -1 ) return -1;	

	count_in	= _countCharInString( str , _T('(') , f2 , f1 );
	count_out	= _countCharInString( str , _T(')') , f2 , f1 );

	int in_all	= _countCharInString( str , _T('(') , 0 , f1 );
	int out_all	= _countCharInString( str , _T(')') , 0 , f1 );

	depth = in_all - out_all;
	start_pos = f1;
	return 1;
}

int		CESL_DataMgr::WebECOSEARCHR_Replace_Split( CString& str , CString mark , int& start_pos , int count )
{
	int ids , left_start_str , right_start_str;

	int f1 = str.Find( mark , start_pos );
	if( f1 == -1 ) return 0;

	start_pos += f1 + mark.GetLength() + 1;

	int	f2 = _findLastWord( str , _T(" ") , f1 );
	if( f2 == -1 ) return -1;

	left_start_str = f2;

	CString full_filed = str.Mid( f2 , f1-f2 + mark.GetLength() );

	int Bracket_first, Bracket_last;
	ids = _find_BracketPair( str , f1 , Bracket_first , Bracket_last );
	if( ids < 0 ) return -1;

	if( Bracket_first == -1 && Bracket_last == -1 ) 
		return 1;
	
	right_start_str = Bracket_last + 1;

	Bracket_first += 1;
	Bracket_last -=1;

	CString		key_str = str.Mid( Bracket_first , Bracket_last - Bracket_first );

	key_str.TrimLeft();
	key_str.TrimRight();


	CStringArray	all_key;
	WebSearch_ParseCommaList( key_str , all_key );

	if( all_key.GetSize() <= count )
	{
		return 1;
	}

	int		cur=0;
	bool	fst = true;
	CString		mid_new;

	mid_new = full_filed + _T(" ("); 
	while(true)
	{
		if( fst == false )
		{
			mid_new += _T(",");
		}

		mid_new += all_key[cur];
		cur++;
		if( cur >= all_key.GetSize() )
			break;

		if( cur % count == 0 )
		{
			fst = true;
			mid_new += _T(") OR ") + full_filed + _T(" (");
		}else{
			fst = false;
		}
	}
	mid_new += _T(") ");

	CString str_left  = str.Left( left_start_str );
	CString str_right = str.Right( str.GetLength() - right_start_str );
	str = str_left + mid_new + str_right;

	start_pos = str_left.GetLength() + mid_new.GetLength();
	return 1;
}

int		CESL_DataMgr::WebSearch_ParseCommaList( CString key_str , CStringArray& arr )
{
	CString			cur_key;
	int find_c;
	int n;
	while(true)
	{
		AfxExtractSubString(cur_key	, key_str, 0 , ',');
		if( cur_key.GetLength() == 0 )break;
		arr.Add( cur_key );
		find_c = key_str.Find( _T(',') );
		if( find_c > -1 )
		{
			n =	key_str.Delete( 0 , find_c+1 );
		}else{
			break;
		}
	}
	return arr.GetSize();
}

int	CESL_DataMgr::RemoveDuplBracket_Prc( CString& strQuery , CString strMark , int& startIdx )
{
	int f = strQuery.Find( strMark , startIdx );
	if( f == -1 ) return 0;
	startIdx = f + 1;

	int f1 = strQuery.Find( _T("(") , f );
	if( f1 == -1 ) return 0;
	startIdx = f1 + 1;

	int f2 = strQuery.Find( _T("(") , f1 + 1 );
	if( f2 == -1 ) return 0;
	startIdx = f2 + 1;

	CString tmp = strQuery.Mid( f1 , f2 - f1 );
	tmp.TrimLeft();
	tmp.TrimRight();
	if( tmp.GetLength() > 1 )
	{
		return 1;
	}

	int Bracket_first, Bracket_last;
	_find_BracketPair( strQuery , f , Bracket_first , Bracket_last );

	strQuery.SetAt( f2 , _T(' ') );

	int last_out_pos =	_findLastWord( strQuery ,  _T(")") , Bracket_last );
	strQuery.SetAt( last_out_pos , _T(' ') );
	
	startIdx = Bracket_last + 1;

	return 1;
}

CString	CESL_DataMgr::DeltaUpdate_Find_Core( CString str )
{
	CString r_str = _T("");
	if( str.Find( _T("IDX_BO_TBL") )	> -1 )
	{
		m_webSearch_deltaUpdateCore[0] = r_str = _T("IDX_BO_TBL");
	}
	if( str.Find( _T("IDX_BO_TOC_TBL")) > -1 )	
	{
		m_webSearch_deltaUpdateCore[1] = r_str = _T("IDX_BO_TOC_TBL");
	}
	if( str.Find( _T("IDX_SE_TBL") )	> -1 )
	{
		m_webSearch_deltaUpdateCore[2] = r_str = _T("IDX_SE_TBL");
	}
	if( str.Find( _T("IDX_SE_TOC_TBL")) > -1 )
	{
		m_webSearch_deltaUpdateCore[3]  = r_str= _T("IDX_SE_TOC_TBL");
	}
	if( r_str.GetLength() > 0 )  
	{
		OutputDebugString( _T("DeltaUpdate_Find_Core : ") + r_str + _T("\n") );
	}
	return r_str;
}

int		CESL_DataMgr::DeltaUpdate_Send()
{
	int i;
	INT ids;
	CString strImportResult;
	for( i = 0 ; i < 4 ; i++ )
	{
		CString core = m_webSearch_deltaUpdateCore[i];
		if( core != _T("") )
		{
			ids = CESL_SearchEngine::RequestDeltaImport(core, strImportResult );
			OutputDebugString( _T("DeltaUpdate_Send : ") + core + _T(" : ") + strImportResult + _T("\n") );
		}
	}

	for( i = 0 ; i < m_webSearch_DeleteQuery.GetSize() ; i++ )
	{
		WebSearch_SendParseDeleteQuery( m_webSearch_DeleteQuery[i] );
	}

	return 1;
}

void	CESL_DataMgr::WebSearch_UpdateDelete_Start()
{
	for(int i = 0 ; i < 4 ; i++) m_webSearch_deltaUpdateCore[i] = _T("");
	m_webSearch_DeleteQuery.RemoveAll();
}


int	CESL_DataMgr::WebSearch_AddDeleteQuery( CString str )
{
	if(  CESL_SearchEngine::GetUsed() == 0 )
		return 0;

	if( str.Find( _T("DELETE FROM ") ) == -1 ) return 0;

	CString core = DeltaUpdate_Find_Core( str );
	if( core == _T("") )return 0;

	m_webSearch_DeleteQuery.Add( str );

	return 1;
}

int	CESL_DataMgr::WebSearch_SendParseDeleteQuery( CString str )
{
	if(  CESL_SearchEngine::GetUsed() == 0 )
		return 0;

	int				i;
	CESL_DataMgr	cDM;
	CStringArray	all_key;

	int f1 = str.Find( _T("REC_KEY IN") );

	CString log;
	if( f1 > -1 )
	{
		int Bracket_first, Bracket_last;
		_find_BracketPair( str , f1 , Bracket_first , Bracket_last );

		if( Bracket_first == -1 || Bracket_last == -1 ) 
		{
			//err
			log.Format( _T("Error!! : WebSearch_SendParseDeleteQuery : TYPE : 1. REC_KEY IN  안의 셀렉트 : 괄호 에러 : %s \n") , str );
			OutputDebugString(log);
			AfxMessageBox(log);
			return -1;
		}

		Bracket_first++;
		Bracket_last--;

		CString sub_Bracket = str.Mid( Bracket_first , Bracket_last - Bracket_first );

		if(	sub_Bracket.Find( _T("SELECT") ) > -1 )
		{
			cDM.SetCONNECTION_INFO(CONNECTION_INFO);
			cDM.RestructDataManager( sub_Bracket );

			int count = cDM.GetRowCount();
			for( i = 0 ; i < count ; i++ )
			{
				all_key.Add( cDM.GetCellData( _T("REC_KEY"), i ) );
			}
		}else{
			WebSearch_ParseCommaList( sub_Bracket , all_key );
		}

	}else{
		int f2 = str.Find( _T("REC_KEY") );
		if( f2 == -1 )
		{
			//err
			log.Format( _T("Warning!! : WebSearch_SendParseDeleteQuery : TYPE : 3. REC_KEY= 또는 REC_KEY = 의 직접 값 : REC_KEY 단어 없음 : %s \n") , str );
			OutputDebugString(log);
			AfxMessageBox(log);
			return 0;
		}

		int f3 = str.Find( _T("=") , f2 );
		if( f3 == -1 )
		{
			//err
			log.Format( _T("Error!! : WebSearch_SendParseDeleteQuery : TYPE : 3. REC_KEY= 또는 REC_KEY = 의 직접 값 : REC_KEY 단어 있는데  =  기호 없음 : %s \n") , str );
			OutputDebugString(log);
			AfxMessageBox(log);
			return -1;
		}

		f3++;
		CString sub_key = str.Mid( f3 , str.GetLength() - f3 );
		sub_key.Replace( _T(";") , _T("") );
		all_key.Add( sub_key );
	}

	log.Format( _T("Proc : WebSearch_SendParseDeleteQuery : %s \n ") , str );
	OutputDebugString(log);


	CString strCore = DeltaUpdate_Find_Core( str );
	CString strDeleteResult;
	CESL_SearchEngine::RequestDeleteQuery( strCore , all_key, strDeleteResult);

	log.Format( _T("CESL_SearchEngine::RequestDeleteQuery : %s : %d : %s \n") , strCore , all_key.GetSize() , strDeleteResult );
	OutputDebugString( log );
}

int		CESL_DataMgr::WebSearch_Delete( CString strCore, CString RecKeys, INT nOpt )
{
	if(  CESL_SearchEngine::GetUsed() == 0 )
		return 0;

	CStringArray	keyarr;
	CString			strDeleteResult;
	keyarr.Add(RecKeys);
	CESL_SearchEngine::RequestDeleteQuery( strCore , keyarr, strDeleteResult);
	OutputDebugString( _T("CESL_SearchEngine::RequestDeleteQuery : ") + strCore + _T(" : ") + RecKeys + _T(" : ") + strDeleteResult + _T("\n") );

	return 1;
}
