// Query.cpp: implementation of the CQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\include\공동목록\Query.h"

#include "resource.h"
#include "ESL_Mgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define    QUERY_INFO_FILE  _T("..\\ref\\공동목록\\검색\\검색정보.txt")

CQuery::CQuery()
{
    pQueryInfo = NULL;  m_GueryInfoCount = 0;

    m_CtrlAlias [0] = '\0';
    _tcscpy ( m_Collection , _T("dan-gm,dan-limit,dan-etc,dan-rb,dan-ca,dan-dmdp,dan-ha,dan-hr") );
}

CQuery::~CQuery()
{
    if ( pQueryInfo != NULL ) {
        free (pQueryInfo);
        pQueryInfo = NULL;
        m_GueryInfoCount = 0;
    }

	if(m_MakeSearchData)
	{
		delete m_MakeSearchData;
		m_MakeSearchData = NULL;
	}
}

INT  CQuery::LoadRef ( )
{
	FILE         *fp;
	static TCHAR   szBuff[500] , ctmp[128];
	INT           nStep = 0;

	if ( NULL == ( fp = _tfopen( QUERY_INFO_FILE , _T("rb") ) ) ) {
		return -1;
	}	

	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	while ( !feof( fp ) ) {
		_fgetts( szBuff, 499, fp );
		if ( '.' == szBuff[0] || '\n' == szBuff[0] || '\t' == szBuff[0] ) {
			continue;
        }
        pQueryInfo = (QUERY_INFO *)realloc( pQueryInfo , sizeof (QUERY_INFO) * (m_GueryInfoCount+1) );
        if ( pQueryInfo == NULL ) {
            return -2;
        }
    	_stscanf( szBuff, _T("%s%s%s%s%s"), 
                        pQueryInfo[m_GueryInfoCount].szAlias      , 
                        pQueryInfo[m_GueryInfoCount].szIField     , 
                        pQueryInfo[m_GueryInfoCount].szIZoneField , 
                        pQueryInfo[m_GueryInfoCount].szRZoneField , 
                        pQueryInfo[m_GueryInfoCount].szCMDField    );
        m_GueryInfoCount++;
	}
	fclose( fp );

    return 0;
}



VOID CQuery::SetDialogPtr ( CESL_Mgr* pDialogMgr )
{
    m_pDialogMgr = pDialogMgr;
	m_MakeSearchData = new CMakeSearchData(m_pDialogMgr);
}

VOID CQuery::SetQueryType ( UINT QueryType )
{
    m_QueryType = QueryType;
}

VOID CQuery::SetCtrlAlias ( TCHAR * lpCtrlAlias )
{
    _tcscpy ( m_CtrlAlias , lpCtrlAlias );
}

VOID CQuery::GetCollectionType ( INT *lpCollectionType )
{
    TCHAR    szData[32];
    CString temp ;
	CString Data_Temp ;

	temp.Format (_T("%s"),m_CtrlAlias);
    
    m_pDialogMgr->GetControlData (temp,_T("자료"),Data_Temp);

	_tcscpy(szData,Data_Temp);
  	if ( _tcscmp( szData , _T("KOLIS-NET") ) == 0 ) 
	{
        (*lpCollectionType) = m_CollectionType = _DAN_;
    } 
	else 
    if ( _tcscmp( szData , _T("연속") ) == 0 ) {
        (*lpCollectionType) = m_CollectionType = _YON_;
    } 
	else 
    if ( _tcscmp( szData , _T("출판유통진흥원") ) == 0 ) {
        (*lpCollectionType) = m_CollectionType = _CHULPAN_;
	} else 
    if ( _tcscmp( szData , _T("전체") ) == 0 ) {
        (*lpCollectionType) = m_CollectionType = _ALL_;
    } else {
        (*lpCollectionType) = m_CollectionType = -1;
    }
}
VOID CQuery::SetMatCode(CString sMatCode)
{
	m_sMatCode = sMatCode;
}

VOID CQuery::GetQueryRang ( INT *lpQueryRang )
{
    (*lpQueryRang) = m_QueryRang = _KOLASNET_;
}

INT  CQuery::MakeOneFieldQuery ( TCHAR *lpField , TCHAR *lpValue , TCHAR *lpQuery )
{
    _stprintf ( lpQuery , _T("%s = %s") , lpField, lpValue );
    return 0;
}

INT  CQuery::MakeOneZoneQuery  ( TCHAR *lpField , TCHAR *lpValue , TCHAR *lpQuery , BOOL bOne)
{
	TCHAR	sTemp[128];
	TCHAR	sBuffer[128];
	TCHAR  *	tok , * pTmp;
	INT		nCnt , i;
	if ( lpQuery == NULL ) return -1;
	if ( bOne == FALSE )
	{
		SpaceTrim ( lpValue , sTemp );
		SpaceTrim ( lpValue , sBuffer );
		if ( sTemp[0] == ',' )
			_tcscpy ( sBuffer , &sTemp[1] );
		if ( sTemp[_tcsclen ( sTemp )-1] == ',' )
			sBuffer[_tcsclen ( sBuffer ) -1] = '\0';

		_tcscpy ( sTemp , sBuffer );
		if ( _tcsclen ( sTemp ) > 0 )
		{
			nCnt = 1;
		}
		pTmp = _tcschr ( sTemp , ',' );
		if ( pTmp != NULL )
		{
			do 
			{
				nCnt++;
			}while ( ( pTmp = _tcschr (pTmp + 1, ',' ) ) != NULL); 
		}
		TCHAR ** pData;
		pData = ( TCHAR ** ) malloc ( sizeof ( TCHAR* ) * nCnt );
		if ( NULL == pData ) return -2;
		for ( i = 0 ; i < nCnt ; i++ )
		{
			pData[i] = (TCHAR*) malloc ( sizeof ( TCHAR ) * 20 );
			if ( NULL == pData[i] )
			{
				for ( INT j = 0 ; j < i ; j++ )
				{
					free ( pData[j] );
				}
				free ( pData );
				return -3;
			}
			pData[i][0] = '\0';
		}
		tok = _tcstok( sTemp , _T(",") );
		if ( NULL != tok )
		{
			_tcscpy ( pData[0] , tok );
			i = 1;
			while ( (tok = _tcstok (NULL , _T(",")) ) != NULL) 
			{
				SpaceTrim ( tok , tok );
				_tcscpy ( pData[i] , tok ); 
				i++; 
			} 
		}
		if ( nCnt == 1 )
		{
			_stprintf ( lpQuery , _T("( %s <IN> %s )") , pData[0] , lpField );
		}
		for ( i = 0 ; i < nCnt ; i++ )
		{
			if ( _tcsclen ( pData[i] ) > 0 )
			{
				if ( i == 0 )
				{
					_stprintf ( lpQuery , _T("( ( %s <IN> %s ) ") , pData[i] , lpField );
				}
				else
				{
					_stprintf ( lpQuery , _T("%s or ( %s <IN> %s ) ") , lpQuery ,  pData[i] , lpField );
				}
				if ( i == nCnt-1 )
				{
					_stprintf ( lpQuery , _T("%s )") , lpQuery );
				}
			}
		}

		if ( NULL != pData )
		{
			for ( i = 0 ; i < nCnt ; i++ )
			{
				free ( pData[i] );
			}
			free ( pData );
		}
		return 0;
	}

	if ( _tcsicmp( lpField, _T("EST_CODE") ) == 0 ) {

		CString strOutISBN;
		m_MakeSearchData->S97ValueMakeIsbn(lpValue, strOutISBN, 2);
		_tcscpy( lpQuery, strOutISBN );
	} else if ( _tcsicmp( lpField, _T("ZKEYWORD") ) == 0 ) {
		_stprintf ( lpQuery , _T(" %s ") , lpValue );
	} else {
		_stprintf ( lpQuery , _T(" %s <IN> %s") , lpValue, lpField );
	}
    return 0;
}

INT  CQuery::MakeOneCmdQuery  ( TCHAR *lpField , TCHAR *lpValue , TCHAR *lpQuery )
{
    _stprintf ( lpQuery , _T("%s=%s") , lpField, lpValue );
    return 0;
}

INT  CQuery::MakeOneQuery ( TCHAR *lpFieldAlias , TCHAR *lpValue , INT QueryType , TCHAR *lpQuery  )
{
	BOOL	bStar = FALSE;
	if ( lpValue[_tcslen(lpValue)-1] == '*' ) {
		lpValue[_tcslen(lpValue)-1] = '\0';
		bStar = TRUE;
		_tcscat(lpValue, _T(" "));
	}
	TCHAR	szTmpValue[1024] = {NULL};
    if ( _tcscmp ( lpFieldAlias , _T("본표제") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) 
		{			
			CString strtmpdata;
			m_MakeSearchData->S97ValueMake_Kolisnet(lpValue, strtmpdata, 1);
			_stprintf(lpQuery, _T(" 쪣%s <IN> ZTITLE "), strtmpdata );
			
		} else {
			MakeOneZoneQuery ( _T("ZTITLE") , lpValue , lpQuery );
		}        
    } else
    if ( _tcscmp ( lpFieldAlias , _T("키워드") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) {
			MakeOneZoneQuery ( _T("ZKEYWORD") , lpValue , lpQuery );
        } else {
			if ( bStar ) {
				_stprintf(szTmpValue,_T("뗷 %s 뗷*") , lpValue);
			} else {
				_stprintf(szTmpValue,_T("뗷 %s 뗷") , lpValue);
				_stprintf ( lpQuery , _T("( %s )") , lpValue );
			}
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("저작자") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) {
			MakeOneZoneQuery ( _T("ZAUTHOR") , lpValue , lpQuery );
        } else {
			if ( bStar )
				_stprintf(szTmpValue,_T("뗷 %s 뗷*") , lpValue);
			else 
				_stprintf(szTmpValue,_T("뗷 %s 뗷") , lpValue);
			_stprintf ( lpQuery , _T("( %s <IN> %s )") , szTmpValue , _T("ZAUTHOR") );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("발행자") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) {
			MakeOneZoneQuery ( _T("ZPUBLISHER") , lpValue , lpQuery );
        } else {
            if ( bStar )
				_stprintf(szTmpValue,_T("뗷 %s 뗷*") , lpValue);
			else 
				_stprintf(szTmpValue,_T("뗷 %s 뗷") , lpValue);
			_stprintf(szTmpValue,_T("뗷%s뗷") , lpValue);
			_stprintf ( lpQuery , _T("( %s <IN> %s )") , szTmpValue , _T("ZPUBLISHER") );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("주제") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) {
			MakeOneZoneQuery ( _T("ZSUBJECT") , lpValue , lpQuery );
        } else {
			if ( bStar )
				_stprintf(szTmpValue,_T("뗷 %s 뗷*") , lpValue);
			else 
				_stprintf(szTmpValue,_T("뗷 %s 뗷") , lpValue);
			_stprintf ( lpQuery , _T("( %s <IN> %s )") , szTmpValue , _T("ZSUBJECT") );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("ISBN") ) == 0 ) {
		CString strIsbnArray;
		CString strTemp;
		CString strIsbn;
		CString strIsbnTemp;
		INT nLength;
		INT nPos;

		strIsbnArray.Format(_T("%s"),lpValue);
		strIsbnArray.TrimLeft();	strIsbnArray.TrimRight();
		nPos = strIsbnArray.Find(_T(","));
		BOOL bChk;
		bChk = FALSE;
		while( nPos > 0)
		{
			strTemp = strIsbnArray.Left(nPos);
			strIsbnArray = strIsbnArray.Mid(nPos+1);
			
			nLength = strTemp.GetLength();
			strTemp.TrimLeft(); strTemp.TrimRight(); 
			if( nLength == 13 )
			{
				strTemp = strTemp.Mid(3,9);
			}
			else if( nLength == 10 )
			{
				strTemp = strTemp.Mid(0,9);
			}
		
			strIsbnTemp = strIsbn;
			if( !strIsbnTemp.IsEmpty() )
			{
				strIsbn.Format(_T("%s, %s*, 978%s*, 979%s*"), strIsbnTemp, strTemp, strTemp, strTemp);	
			}
			else
			{
				strIsbn.Format(_T("%s*, 978%s*, 979%s*"), strTemp, strTemp, strTemp);
			}
			nPos = strIsbnArray.Find(_T(","));
		}

		strTemp = strIsbnArray;
		strTemp.TrimLeft(); strTemp.TrimRight(); 
		nLength = strTemp.GetLength();
		if( nLength == 13 )
		{
			strTemp = strTemp.Mid(3,9);
		}
		else if( nLength == 10 )
		{
			strTemp = strTemp.Mid(0,9);
		}
		strIsbnTemp = strIsbn;

		if( !strIsbnTemp.IsEmpty() )
		{
			strIsbn.Format(_T("%s, %s*, 978%s*, 979%s*"), strIsbnTemp, strTemp, strTemp, strTemp);	
		}
		else
		{
			strIsbn.Format(_T("%s*, 978%s*, 979%s*"), strTemp, strTemp, strTemp);
		}
		nPos = strIsbnArray.Find(_T(","));
		strIsbn.TrimRight(); strIsbn.TrimLeft();

		_stprintf( lpQuery, _T(" %s <in> ZST_CODE "), strIsbn );
		
    } else
    if ( _tcscmp ( lpFieldAlias , _T("ISSN") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_ISSN "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("STRN(표준기술보고서번호)") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_STRN "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("RNSTRN(보고서번호)") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_RNSTRN "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("국가서지번호") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_CBM "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("저작권등록번호") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_CAN "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("CODEN") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_CODEN "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("ISRC") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_ISRC "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("UPC") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_UPC "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("ISMN") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_ISMN "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("EAN") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_EAN "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("SICI") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_SICI "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("GPO(자료번호)") ) == 0 ) {
		_stprintf( lpQuery, _T(" %s <in> ZST_GPO "), lpValue );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("국가서지제어번호") ) == 0 ) {
		_stprintf( lpQuery, _T(" CONTROL_NO = %s "), lpValue);
    } else
    if ( _tcscmp ( lpFieldAlias , _T("CIP제어번호") ) == 0 ) {
		_stprintf( lpQuery, _T(" CIP_CONTROL_NO = %s "), lpValue);
    } else
    if ( _tcscmp ( lpFieldAlias , _T("KDC(한국십진분류법)") ) == 0 ) {
		m_MakeSearchData->GetKeyWord( lpValue, lpValue );
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZKDC_CLASS") , lpValue , lpQuery , FALSE);
        } else {
            MakeOneZoneQuery ( _T("ZKDC_CLASS") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("KDCP(한국십진분류표-박봉석편)") ) == 0 ) {
		m_MakeSearchData->GetKeyWord( lpValue, lpValue );		
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZKDCP_CLASS") , lpValue , lpQuery , FALSE);
        } else {
            MakeOneZoneQuery ( _T("ZKDCP_CLASS") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("DDC(듀이십진분류법)") ) == 0 ) {
		m_MakeSearchData->GetKeyWord( lpValue, lpValue );		
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZDDC_CLASS") , lpValue , lpQuery , FALSE);
        } else {
			MakeOneZoneQuery ( _T("ZDDC_CLASS") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("CEC(조선총독부신서부분류표)") ) == 0 ) {
		m_MakeSearchData->GetKeyWord( lpValue, lpValue );		
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZCEC_CLASS") , lpValue , lpQuery , FALSE);
        } else {
            MakeOneZoneQuery ( _T("ZCEC_CLASS") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("CWC(조선총독부 양서부분류표)") ) == 0 ) {
		m_MakeSearchData->GetKeyWord( lpValue, lpValue );		
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZCWC_CLASS") , lpValue , lpQuery , FALSE);
        } else {
            MakeOneZoneQuery ( _T("ZCWC_CLASS") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("COC(조선총독부고서부분류표)") ) == 0 ) {
		m_MakeSearchData->GetKeyWord( lpValue, lpValue );		
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZCOC_CLASS") , lpValue , lpQuery , FALSE);
        } else {
            MakeOneZoneQuery ( _T("ZCOC_CLASS") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("UDC(국제십진분류표)") ) == 0 ) {
		m_MakeSearchData->GetKeyWord( lpValue, lpValue );		
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZUDC_CLASS") , lpValue , lpQuery , FALSE);
        } else {
            MakeOneZoneQuery ( _T("ZUDC_CLASS") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("NDC(일본십진분류표)") ) == 0 ) {
		m_MakeSearchData->GetKeyWord( lpValue, lpValue );		
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZNDC_CLASS") , lpValue , lpQuery , FALSE);
        } else {
            MakeOneZoneQuery ( _T("ZNDC_CLASS") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("LC(미국국회도서관분류표)") ) == 0 ) {
		m_MakeSearchData->GetKeyWord( lpValue, lpValue );
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZLC_CLASS") , lpValue , lpQuery , FALSE);
        } else {
            MakeOneZoneQuery ( _T("ZLC_CLASS") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("한국대학부호") ) == 0 ) {
		MakeOneZoneQuery ( _T("ZUNI_CODE") , lpValue , lpQuery );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("한국정부기관부호") ) == 0 ) {
		MakeOneZoneQuery ( _T("ZGOV_CODE") , lpValue , lpQuery );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("도서관부호") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZLIB_CODE") , lpValue , lpQuery );
        } else {
            MakeOneZoneQuery ( _T("ZLIB_CODE") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("발행국부호") ) == 0 ) {
		MakeOneZoneQuery ( _T("ZPUBLISH_COUNTRY_CODE") , lpValue , lpQuery );
    } else
    if ( _tcscmp ( lpFieldAlias , _T("학위논문의학위수여기관") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) {
			
			MakeOneZoneQuery ( _T("ZDEG_PUBLISHER") , lpValue , lpQuery );
        } else {
            MakeOneZoneQuery ( _T("ZDEG_PUBLISHER") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("학위논문의학과또는전공") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZDEG_CODE") , lpValue , lpQuery );
        } else {
            MakeOneZoneQuery ( _T("ZDEG_CODE") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("학위구분") ) == 0 ) {
		_stprintf(lpQuery, _T("MAT_CODE_%s"), lpValue); 
    } else
    if ( _tcscmp ( lpFieldAlias , _T("본문언어") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZTEXT_LANG") , lpValue , lpQuery );
        } else {
            MakeOneZoneQuery ( _T("ZTEXT_LANG") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("요약문언어") ) == 0 ) {
        if ( QueryType == _RIGHT_ ) {
            MakeOneZoneQuery ( _T("ZSUM_LANG") , lpValue , lpQuery );
        } else {
            MakeOneZoneQuery ( _T("ZSUM_LANG") , lpValue , lpQuery );
        }
    } else
    if ( _tcscmp ( lpFieldAlias , _T("간행빈도") ) == 0 ) {
        MakeOneFieldQuery ( _T("PUB_FREQ") , lpValue , lpQuery );
    } else 
    if ( _tcscmp ( lpFieldAlias , _T("발행년도") ) == 0 ) {
        MakeOneFieldQuery ( _T("IPUB_YEAR") , lpValue , lpQuery );
    } else return -1;

    return 0;
}

INT  CQuery::MakeOneCtrlQuery ( TCHAR *lpCtrlAlias , TCHAR *lpQuery )
{
    return 0;
}

INT  CQuery::MakeOneCtrlQuery ( TCHAR *lpCtrlAlias1 , TCHAR *lpCtrlAlias2 , TCHAR *lpQuery )
{
    return 0;
}

INT  CQuery::MakeQuery ( TCHAR *lpQuery , INT CollectionType , CKolisNetSearchParam *pKolisNetSearchParam )
{
	INT   ids;
    const INT nGubunAliasCnt = 2;
    const INT nIndexAliasCnt = 26;
    const INT nFieldAliasCnt = 5;
    const INT nLibAliasCnt   = 3;

	lpQuery[0]      = '\0';
	m_SortInfo[0]   = '\0';
	m_Collection[0] = '\0';

    CString strGubunAlias[nGubunAliasCnt] = {
        _T("표준부호1"), _T("분류기호1")
    };
	
    CString strGubunData[nGubunAliasCnt];
	for (INT i=0; i<nGubunAliasCnt; i++)
	{
		m_pDialogMgr->GetControlData(m_CtrlAlias, strGubunAlias[i], strGubunData[i]); 
		if (strGubunData[i].CompareNoCase(_T("적용안함")) == 0)
			strGubunData[i].Empty();
	}

    CString strIndexAlias[nIndexAliasCnt] = {
        _T("본표제")            ,_T("저작자")      ,_T("발행자")          ,_T("키워드1")         ,_T("키워드2")     ,
        _T("키워드3")           ,_T("구분자1")     ,_T("구분자2")         ,_T("주제")            ,_T("시작발행년")  ,
        _T("끝발행년")          ,_T("표준부호2")   ,_T("서지제어번호부터"),_T("서지제어번호까지"),_T("분류기호2")   ,
		_T("단위도서관제어번호"),_T("한국대학부호"),_T("정부기관부호")    ,_T("학위논문")        ,_T("발행국부호")  ,
		_T("학위수여기관")      ,_T("본문언어")    ,_T("요약문언어")      ,_T("CIP제어번호")	 , _T("업로드기간")	,
		_T("업로드기간2")
    };

    CString strIndexData[nIndexAliasCnt];
	for (i=0; i<nIndexAliasCnt; i++)
	{
		 m_pDialogMgr->GetControlData(m_CtrlAlias, strIndexAlias[i], strIndexData[i]); 
		 strIndexData[i].TrimLeft();
		 strIndexData[i].TrimRight();
	}
    
    CString strFieldAlias[nFieldAliasCnt] = {
        _T("자료형태"), _T("학위구분"), _T("간행빈도.코드"), _T("매체구분.코드"), _T("비도서매체구분.코드")
    };

    CString strFieldData[nFieldAliasCnt];
	strFieldData[0] = m_sMatCode;
    for (i=1; i<nFieldAliasCnt; i++)
	{
		m_pDialogMgr->GetControlData(m_CtrlAlias, strFieldAlias[i], strFieldData[i]);
		if (strFieldData[i].CompareNoCase(_T("적용안함")) == 0 && i == 1)
			strFieldData[i].Empty();
		strFieldData[i].TrimLeft();
		strFieldData[i].TrimRight();
	}
   	
	CString strLibAlias[nLibAliasCnt] = {
        _T("관종"), _T("지역"), _T("도서관부호")
    };
    
	CString strLibData[nLibAliasCnt] ;
    for (i=0; i<nLibAliasCnt; i++)
	{
		m_pDialogMgr->GetControlData(m_CtrlAlias, strLibAlias[i], strLibData[i]);
		strLibData[i].TrimLeft();
		strLibData[i].TrimRight();
	}

	if (!strGubunData[0].IsEmpty() && !strIndexData[11].IsEmpty())
	{

		if (strGubunData[0].CompareNoCase(_T("ISBN")) == 0)
		{
			CString strIsbn;
			CString strTemp;
			CString strData = strIndexData[11];
	
			do
			{
				INT idx = strData.Find(_T(","));
				if (idx > 0)
				{
					strTemp = strData.Left(idx);
					strData = strData.Mid(idx+1);
				}
				else
				{
					strTemp = strData;
					strData.Empty();
				}

				strTemp.TrimLeft();
				strTemp.TrimRight();

				INT nLength = strTemp.GetLength();
				if		(nLength == 13)
				{
					strTemp = strTemp.Mid(3,9);
				}
				else if (nLength == 10)
				{
					strTemp = strTemp.Mid(0,9);
				}

				if (!strIsbn.IsEmpty())
				{
					strIsbn.Format(_T("%s|%s|978%s|979%s"), strIsbn, strTemp, strTemp, strTemp);
				}
				else
				{
					strIsbn.Format(_T("%s|978%s|979%s"), strTemp, strTemp, strTemp);
				}
			}
			while (!strData.IsEmpty());

			pKolisNetSearchParam->SetISBN(strIsbn);
		}
		else if (strGubunData[0].CompareNoCase(_T("ISSN")) == 0)
		{
			pKolisNetSearchParam->SetISSN(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("STRN(표준기술보고서번호)")) == 0)
		{
			pKolisNetSearchParam->SetSTRN(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("RNSTRN(보고서번호)")) == 0)
		{
			pKolisNetSearchParam->SetRNSTRN(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("국가서지번호")) == 0)
		{
			pKolisNetSearchParam->SetCODEN(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("저작권등록번호")) == 0)
		{
			pKolisNetSearchParam->SetREGNUM(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("CODEN")) == 0)
		{
			pKolisNetSearchParam->SetNBCODE(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("ISRC")) == 0)
		{
			pKolisNetSearchParam->SetIRSC(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("ISRC")) == 0)
		{
			pKolisNetSearchParam->SetUPC(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("ISMN")) == 0)
		{
			pKolisNetSearchParam->SetISMN(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("EAN")) == 0)
		{
			pKolisNetSearchParam->SetEAN(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("SICI")) == 0)
		{
			pKolisNetSearchParam->SetSICI(strIndexData[11]);
		}
		else if (strGubunData[0].CompareNoCase(_T("GPO(자료번호)")) == 0)
		{
			pKolisNetSearchParam->SetGPO_CA(strIndexData[11]);
		}
		return 0;
	}

	if (!strIndexData[12].IsEmpty() || !strIndexData[13].IsEmpty())
	{
		if (!strIndexData[12].IsEmpty() && !strIndexData[13].IsEmpty())
		{
			pKolisNetSearchParam->SetSTART_CONTROL_NO(strIndexData[12]);
			pKolisNetSearchParam->SetEND_CONTROL_NO(strIndexData[13]);
		}
		else if (!strIndexData[12].IsEmpty())
		{
			pKolisNetSearchParam->SetCONTROL_NO(strIndexData[12]);
		}
		else if (!strIndexData[13].IsEmpty())
		{
			pKolisNetSearchParam->SetCONTROL_NO(strIndexData[13]);
		}

		return 0;
	}

	if (!strIndexData[23].IsEmpty())
	{
		pKolisNetSearchParam->SetCIP_CONTROL_NO(strIndexData[23]);
		return 0;
	}

	if (!strIndexData[15].IsEmpty())
	{

	}

	if (!strLibData[2].IsEmpty())
	{
		pKolisNetSearchParam->SetLIB_CODE(strLibData[2]);
	}
	else
	{
		if (strLibData[0].IsEmpty() && !strLibData[1].IsEmpty())
		{
			::MessageBox(m_pDialogMgr->GetSafeHwnd(), _T("관종, 지역을 입력하십시오."), _T("자료찾기"), MB_ICONWARNING);
			return 0;
		}
	}

	if (!strIndexData[0].IsEmpty())
	{
		CString strTiTle = strIndexData[0];
		GetSearchKeyword(strTiTle.GetBuffer(0));
		strTiTle.ReleaseBuffer();

		pKolisNetSearchParam->SetTITLE(strTiTle);

		if (strIndexData[0].GetAt(0) == '"' && strIndexData[0].GetAt(strIndexData[0].GetLength()-1) == '"')
		{
			pKolisNetSearchParam->SetTITLE_TYPE(CKolisNetSearchParam::SEARCH_TYPE::SEARCH_TYPE_EQUAL);
		}
		else
		{
			pKolisNetSearchParam->SetTITLE_TYPE(CKolisNetSearchParam::SEARCH_TYPE::SEARCH_TYPE_RIGHT);
		}
	}

	if (!strIndexData[3].IsEmpty())
	{
		GetSearchKeyword(strIndexData[3].GetBuffer(0));
		strIndexData[3].ReleaseBuffer();

		pKolisNetSearchParam->SetKEYWORD_1(strIndexData[3]);
		pKolisNetSearchParam->SetKEYWORD_TYPE_1(CKolisNetSearchParam::KEYWORD_TYPE::KEYWORD_TYPE_KEYWORD);
	}

	if (!strIndexData[4].IsEmpty())
	{
		GetSearchKeyword(strIndexData[4].GetBuffer(0));
		strIndexData[4].ReleaseBuffer();

		pKolisNetSearchParam->SetKEYWORD_2(strIndexData[4]);
		pKolisNetSearchParam->SetKEYWORD_TYPE_2(CKolisNetSearchParam::KEYWORD_TYPE::KEYWORD_TYPE_KEYWORD);

		if (strIndexData[6].CompareNoCase(_T("AND")) == 0)
		{
			pKolisNetSearchParam->SetKEYWORD_CONDITION_2(CKolisNetSearchParam::KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_AND);
		}
		else
		{
			pKolisNetSearchParam->SetKEYWORD_CONDITION_2(CKolisNetSearchParam::KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_OR);
		}
	}

	if (!strIndexData[5].IsEmpty())
	{
		GetSearchKeyword(strIndexData[5].GetBuffer(0));
		strIndexData[5].ReleaseBuffer();

		pKolisNetSearchParam->SetKEYWORD_2(strIndexData[5]);
		pKolisNetSearchParam->SetKEYWORD_TYPE_2(CKolisNetSearchParam::KEYWORD_TYPE::KEYWORD_TYPE_KEYWORD);

		if (strIndexData[7].CompareNoCase(_T("AND")) == 0)
		{
			pKolisNetSearchParam->SetKEYWORD_CONDITION_2(CKolisNetSearchParam::KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_AND);
		}
		else
		{
			pKolisNetSearchParam->SetKEYWORD_CONDITION_2(CKolisNetSearchParam::KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_OR);
		}
	}

	if (!strIndexData[1].IsEmpty())
	{
		CString strAuthor = strIndexData[1];
		GetSearchKeyword(strAuthor.GetBuffer(0));
		strAuthor.ReleaseBuffer();

		pKolisNetSearchParam->SetAUTHOR(strAuthor);

		if (strIndexData[1].GetAt(0) == '"' && strIndexData[1].GetAt(strIndexData[1].GetLength()-1) == '"')
		{
			pKolisNetSearchParam->SetAUTHOR_TYPE(CKolisNetSearchParam::SEARCH_TYPE::SEARCH_TYPE_EQUAL);
		}
		else
		{
			pKolisNetSearchParam->SetAUTHOR_TYPE(CKolisNetSearchParam::SEARCH_TYPE::SEARCH_TYPE_RIGHT);
		}
	}

	if (!strIndexData[2].IsEmpty())
	{
		CString strPublisher = strIndexData[2];
		GetSearchKeyword(strPublisher.GetBuffer(0));
		strPublisher.ReleaseBuffer();

		pKolisNetSearchParam->SetPUBLISHER(strPublisher);

		if (strIndexData[2].GetAt(0) == '"' && strIndexData[2].GetAt(strIndexData[2].GetLength()-1) == '"')
		{
			pKolisNetSearchParam->SetPUBLISHER_TYPE(CKolisNetSearchParam::SEARCH_TYPE::SEARCH_TYPE_EQUAL);
		}
		else
		{
			pKolisNetSearchParam->SetPUBLISHER_TYPE(CKolisNetSearchParam::SEARCH_TYPE::SEARCH_TYPE_RIGHT);
		}
	}

	if (!strIndexData[8].IsEmpty())
	{
		CString strSubject = strIndexData[8];
		GetSearchKeyword(strSubject.GetBuffer(0));
		strSubject.ReleaseBuffer();

		pKolisNetSearchParam->SetSUBJECT(strSubject);
	}

    if (CollectionType == _DAN_ && (!strIndexData[9].IsEmpty() || !strIndexData[10].IsEmpty()))
	{
		GetSearchKeyword(strIndexData[9].GetBuffer(0));	
		strIndexData[9].ReleaseBuffer(0);
		GetSearchKeyword(strIndexData[10].GetBuffer(0));
		strIndexData[10].ReleaseBuffer(0);

		if (!strIndexData[9].IsEmpty() && !strIndexData[10].IsEmpty())
		{
			pKolisNetSearchParam->SetSTART_IPUB_YEAR(strIndexData[9]);
			pKolisNetSearchParam->SetEND_IPUB_YEAR(strIndexData[10]);
		}
		else if (!strIndexData[9].IsEmpty())
		{
			pKolisNetSearchParam->SetIPUB_YEAR(strIndexData[9]);
		}
		else if (!strIndexData[10].IsEmpty())
		{
			pKolisNetSearchParam->SetIPUB_YEAR(strIndexData[10]);
		} 
	}

	if (!strGubunData[1].IsEmpty())
	{
		if (strIndexData[14].IsEmpty())
		{
			::MessageBox(m_pDialogMgr->GetSafeHwnd(), _T(" 분류기호를 입력하십시요 "), _T("자료찾기"), MB_ICONWARNING);
			return 100;
		}

		m_MakeSearchData->GetKeyWord(strIndexData[14]);

		if		(strGubunData[1].CompareNoCase(_T("KDC(한국십진분류법)")) == 0)
		{
			pKolisNetSearchParam->SetKDC(strIndexData[14]);
		}
		else if (strGubunData[1].CompareNoCase(_T("KDCP(한국십진분류법-박봉석편)")) == 0)
		{
			pKolisNetSearchParam->SetKDCP(strIndexData[14]);
		}
		else if (strGubunData[1].CompareNoCase(_T("DDC(듀이십진분류법)")) == 0)
		{
			pKolisNetSearchParam->SetDDC(strIndexData[14]);
		}
		else if (strGubunData[1].CompareNoCase(_T("CEC(조선총독부신서부분류표)")) == 0)
		{
			pKolisNetSearchParam->SetCEC(strIndexData[14]);
		}
		else if (strGubunData[1].CompareNoCase(_T("CWC(조선총독부 양서부분류표)")) == 0)
		{
			pKolisNetSearchParam->SetCWC(strIndexData[14]);
		}
		else if (strGubunData[1].CompareNoCase(_T("COC(조선총독부고서부분류표)")) == 0)
		{
			pKolisNetSearchParam->SetCOC(strIndexData[14]);
		}
		else if (strGubunData[1].CompareNoCase(_T("UDC(국제십진분류표)")) == 0)
		{
			pKolisNetSearchParam->SetUDC(strIndexData[14]);
		}
		else if (strGubunData[1].CompareNoCase(_T("NDC(일본십진분류표)")) == 0)
		{
			pKolisNetSearchParam->SetNDC(strIndexData[14]);
		}
		else if (strGubunData[1].CompareNoCase(_T("LC(미국국회도서관분류표)")) == 0)
		{
			pKolisNetSearchParam->SetLC(strIndexData[14]);
		}
	}

	if (!strIndexData[24].IsEmpty() || !strIndexData[25].IsEmpty())
	{
		if (!strIndexData[24].IsEmpty() && !strIndexData[25].IsEmpty())
		{
			pKolisNetSearchParam->SetSTART_UPLOAD_TERM(CKolisNetService::GetDateString(strIndexData[24]));
			pKolisNetSearchParam->SetEND_UPLOAD_TERM(CKolisNetService::GetDateString(strIndexData[25], FALSE));
		}
		else if (!strIndexData[24].IsEmpty())
		{
			pKolisNetSearchParam->SetSTART_UPLOAD_TERM(CKolisNetService::GetDateString(strIndexData[24]));
			pKolisNetSearchParam->SetEND_UPLOAD_TERM(CKolisNetService::GetDateString(strIndexData[24], FALSE));
		}
		else if (!strIndexData[25].IsEmpty())
		{
			pKolisNetSearchParam->SetSTART_UPLOAD_TERM(CKolisNetService::GetDateString(strIndexData[25]));
			pKolisNetSearchParam->SetEND_UPLOAD_TERM(CKolisNetService::GetDateString(strIndexData[25], FALSE));
		}
	}

	if (!strIndexData[16].IsEmpty())
	{
		pKolisNetSearchParam->SetUNI_CODE(strIndexData[16]);
	}

	if (!strIndexData[17].IsEmpty())
	{
		pKolisNetSearchParam->SetGOV_CODE(strIndexData[17]);
	}

	if (!strIndexData[19].IsEmpty())
	{
		pKolisNetSearchParam->SetNA_CODE(strIndexData[19]);
	}

	if (!strIndexData[21].IsEmpty())
	{
		pKolisNetSearchParam->SetTEXT_LANG(strIndexData[21]);
	}

	if (!strIndexData[22].IsEmpty())
	{
		pKolisNetSearchParam->SetSUM_LANG(strIndexData[22]);
	}

	CString strMediaCode;
	if		(!strFieldData[4].IsEmpty())
	{
		strMediaCode = strFieldData[4];
	}
	else if (!strFieldData[3].IsEmpty())
	{
		strMediaCode = strFieldData[3];
	}

	CString strSortTitle;
	((CComboBox*)m_pDialogMgr->GetDlgItem(IDC_cSORT_TITLE))->GetLBText(((CComboBox*)m_pDialogMgr->GetDlgItem(IDC_cSORT_TITLE))->GetCurSel(), strSortTitle);
	CString strSortPublisher;
    ((CComboBox*)m_pDialogMgr->GetDlgItem(IDC_cSORT_PUBLISHER))->GetLBText(((CComboBox*)m_pDialogMgr->GetDlgItem(IDC_cSORT_PUBLISHER))->GetCurSel(), strSortPublisher);
	CString strSortPubYear;
	((CComboBox*)m_pDialogMgr->GetDlgItem(IDC_cSORT_PUBYEAR))->GetLBText(((CComboBox*)m_pDialogMgr->GetDlgItem(IDC_cSORT_PUBYEAR))->GetCurSel(), strSortPubYear);
	
	if (strSortTitle.CompareNoCase(_T("정렬안함")) != 0)
	{
		if (strSortTitle.CompareNoCase(_T("오름차순")) == 0)
		{
			pKolisNetSearchParam->SetSORT_TITLE(CKolisNetSearchParam::SORT_TYPE::SORT_TYPE_ASC);
		}
		else
		{
			pKolisNetSearchParam->SetSORT_TITLE(CKolisNetSearchParam::SORT_TYPE::SORT_TYPE_DESC);
		}
	}

	if (strSortPublisher.CompareNoCase(_T("정렬안함")) != 0)
	{
		if (strSortPublisher.CompareNoCase(_T("오름차순")) == 0)
		{
			pKolisNetSearchParam->SetSORT_PUBLISHER(CKolisNetSearchParam::SORT_TYPE::SORT_TYPE_ASC);
		}
		else
		{
			pKolisNetSearchParam->SetSORT_PUBLISHER(CKolisNetSearchParam::SORT_TYPE::SORT_TYPE_DESC);
		}
	}

	if (strSortPubYear.CompareNoCase(_T("정렬안함")) != 0)
	{
		if (strSortPubYear.CompareNoCase(_T("오름차순")) == 0)
		{
			pKolisNetSearchParam->SetSORT_PUB_YEAR(CKolisNetSearchParam::SORT_TYPE::SORT_TYPE_ASC);
		}
		else
		{
			pKolisNetSearchParam->SetSORT_PUB_YEAR(CKolisNetSearchParam::SORT_TYPE::SORT_TYPE_DESC);
		}
	}

	CString strMatCode;
	CString strCollection;
	if (!strFieldData[1].IsEmpty())
	{
		if (strFieldData[1].CompareNoCase(_T("석사")) == 0)
		{
			strMatCode = _T("DM");
		}
		else
		{
			strMatCode = _T("DP");
		}

		strCollection += _T("dan-dmdp");
	}

	if (!strFieldData[0].IsEmpty() && strFieldData[1].IsEmpty() && strFieldData[3].IsEmpty() && strFieldData[4].IsEmpty())
	{		
		CString strFormCode;
		CString strMediaCode;

		INT nIdx = 0;
		do
		{
			nIdx = m_sMatCode.Find(_T(","));

			CString strTemp;
			if (nIdx < 0)
			{
				strTemp = m_sMatCode;
			}
			else
			{
				strTemp    = m_sMatCode.Left(nIdx);
				m_sMatCode = m_sMatCode.Mid(nIdx+1);
			}

			if		(strTemp.CompareNoCase(_T("일반도서")) == 0)
			{
				if (!strCollection.IsEmpty())
				{
					strCollection += _T("|");
				}
				strCollection += _T("dan-gm");
			}
			else if (strTemp.CompareNoCase(_T("학위논문")) == 0)
			{
				if (!strCollection.IsEmpty())
				{
					strCollection += _T("|");
				}
				strCollection += _T("dan-dmdp");
			}
			else if (strTemp.CompareNoCase(_T("교과서")) == 0)
			{
				if (!strCollection.IsEmpty())
				{
					strCollection += _T("|");
				}
				strCollection += _T("dan-ha");
			}
			else if (strTemp.CompareNoCase(_T("학습서")) == 0)
			{
				if (!strCollection.IsEmpty())
				{
					strCollection += _T("|");
				}
				strCollection += _T("dan-hr");
			}
			else if (strTemp.CompareNoCase(_T("만화")) == 0)
			{
				if (!strCollection.IsEmpty())
				{
					strCollection += _T("|");
				}
				strCollection += _T("dan-ca");
			}
			else if (strTemp.CompareNoCase(_T("잡지")) == 0)
			{
				if (!strMatCode.IsEmpty())
				{
					strMatCode += _T("|");
				}
				strMatCode += _T("MA");
			}
			else if (strTemp.CompareNoCase(_T("신문")) == 0)
			{
				if (!strMatCode.IsEmpty())
				{
					strMatCode += _T("|");
				}
				strMatCode += _T("NP");
			}
			else if (strTemp.CompareNoCase(_T("비도서")) == 0)
			{
				strFormCode = _T("CF|MU|VM|MP|TX");

				if (!strMediaCode.IsEmpty())
				{
					strMediaCode += _T("|");
				}
				strMediaCode += _T("XH|XA|XZ");
			}
			else if (strTemp.CompareNoCase(_T("점자자료")) == 0)
			{
				if (!strMediaCode.IsEmpty())
				{
					strMediaCode += _T("|");
				}
				strMediaCode += _T("KA");
			}
		}
		while (nIdx >= 0);
		
		if (!strFormCode.IsEmpty())
		{

		}
		if (!strMediaCode.IsEmpty())
		{
			pKolisNetSearchParam->SetMEDIA_CODE(strMediaCode);
		}
	}

	if (!strMatCode.IsEmpty())
	{
		pKolisNetSearchParam->SetMAT_CODE(strMatCode);
	}

	if (!strIndexData[18].IsEmpty())
	{
		pKolisNetSearchParam->SetDEG_CODE(strIndexData[18]);
	}

	if (!strIndexData[20].IsEmpty())
	{
		pKolisNetSearchParam->SetDEGREE_ORG(strIndexData[20]);
	}

	if (strCollection.IsEmpty())
	{
		strCollection = _T("dan-gm|dan-limit|dan-etc|dan-rb|dan-ca|dan-dmdp|dan-ha|dan-hr");
	}

	pKolisNetSearchParam->SetCOLL_TYPE_1(strCollection);
	pKolisNetSearchParam->SetCOLL_TYPE_2(strCollection);

    return 0;
}

INT  CQuery::MakeIROQuery ( TCHAR *lpQuery )
{
    INT       ids , i;
    const INT LibAliasCnt   = 3;

    TCHAR *LibAlias[LibAliasCnt] = 
    {
        _T("관종"),_T("지역"),_T("도서관부호")
    };
    
    TCHAR **LibData = NULL;
    TCHAR   szQuery[1024];

    lpQuery[0] = '\0';

	CString temp_LibAlias = _T("");
	CString temp_CtrlAlias = _T("") ;
    CString temp_LibData = _T("") ;

	temp_CtrlAlias.Format (_T("%s"),m_CtrlAlias);
	temp_LibAlias.Format (_T("%S"),LibAlias) ;

	m_pDialogMgr->GetControlData (temp_CtrlAlias,temp_LibAlias,temp_LibData,-1,LibAliasCnt);
    
    _tcscpy(*LibAlias,temp_LibAlias);
	_tcscpy(m_CtrlAlias,temp_CtrlAlias);
	
    ids = MakeLibQuery( szQuery );
    if ( ids == -100 ) {
        ::MessageBox ( m_pDialogMgr->GetSafeHwnd() , _T(" 관종, 지역을 입력하십시오. ") , _T("자료찾기"), MB_ICONWARNING );
        return 0;
    }
	if ( _tcsclen(szQuery) > 0 ) {
		_tcscpy ( lpQuery , szQuery );
	}
     
    CString temp_szQuery ;

	temp_szQuery.Format (_T("%s"),szQuery) ;

    m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("검색명령어"),temp_szQuery,-1);
  
    m_Parse.ParseQuery ( szQuery );
    ids = m_Parse.SyntaxCheckQuery();
    if ( ids != 0 ) {
        return ids;
    }

    if ( m_Parse.m_QueryInfoCnt > 0 && _tcsclen(lpQuery) > 0 ) {
        _stprintf ( lpQuery+_tcsclen(lpQuery) , _T(" AND ") );
    }
    for ( i = 0; i < m_Parse.m_QueryInfoCnt; i++ ) {
        switch ( m_Parse.m_pQueryInfo[i].Opertor ) {
        case _BR_ :
            _stprintf ( lpQuery+_tcsclen(lpQuery) , _T("%s") , m_Parse.m_pQueryInfo[i].szValue );
            break;
        case _OP_ :
            if ( m_Parse.m_pQueryInfo[i].szValue[0] == '*' ) {
                _stprintf ( lpQuery+_tcsclen(lpQuery) , _T(" AND ") , m_Parse.m_pQueryInfo[i].szValue );
            } else
            if ( m_Parse.m_pQueryInfo[i].szValue[0] == '+' ) {
                _stprintf ( lpQuery+_tcsclen(lpQuery) , _T(" OR ") , m_Parse.m_pQueryInfo[i].szValue );
            }
            break;
        case _FN_ :
            ids = MakeOneIROQuery ( m_Parse.m_pQueryInfo[i].szValue , m_Parse.m_pQueryInfo[i+1].szValue , m_Parse.m_pQueryInfo[i+2].szValue , szQuery );
            if ( ids ) {
                return -2;
            }
            _stprintf ( lpQuery+_tcsclen(lpQuery) , _T("%s") , szQuery );
            break;
        }
    }

    return 0;
}

INT  CQuery::MakeOneIROQuery ( TCHAR *lpField , TCHAR *lpEqual , TCHAR  *lpValue , TCHAR *lpQuery )
{
    if ( _tcsicmp ( lpField , _T("TI") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("본표제") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("본표제") , lpValue , _RIGHT_ , lpQuery );
        }

    } else 
    if ( _tcsicmp ( lpField , _T("TK") )  == 0 ) {
		MakeOneQuery ( _T("키워드") , lpValue , _SAME_ , lpQuery );
    } else 
    if ( _tcsicmp ( lpField , _T("AU") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("저작자") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("저작자") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("SU") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
		    MakeOneQuery ( _T("주제") , lpValue , _SAME_ , lpQuery );
        } else {
		    MakeOneQuery ( _T("주제") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("PN") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("발행자") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("발행자") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("IB") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("ISBN") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("ISBN") , lpValue , _RIGHT_ , lpQuery );
        }
        
    } else 
    if ( _tcsicmp ( lpField , _T("IS") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("ISSN") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("ISSN") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("STRN") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("STRN(표준기술보고서번호)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("STRN(표준기술보고서번호)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("RN") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("RNSTRN(보고서번호)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("RNSTRN(보고서번호)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("CD") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("CODEN") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("CODEN") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("ISRC") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("ISRC") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("ISRC") , lpValue , _RIGHT_ , lpQuery );
        }
    } else
    if ( _tcsicmp ( lpField , _T("UPC")  )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("UPC") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("UPC") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("ISMN") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("ISMN") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("ISMN") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("EAN") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("EAN") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("EAN") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("SICI") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("SICI") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("SICI") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("GD") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("GPO(자료번호)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("GPO(자료번호)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("KDCP") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("KDCP(한국십진분류표-박봉석편)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("KDCP(한국십진분류표-박봉석편)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("KDC") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("KDC(한국십진분류법)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("KDC(한국십진분류법)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("DDC") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("DDC(듀이십진분류법)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("DDC(듀이십진분류법)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("CEC") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("CEC(조선총독부신서부분류표)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("CEC(조선총독부신서부분류표)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("CWC") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("CWC(조선총독부 양서부분류표)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("CWC(조선총독부 양서부분류표)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("COC") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("COC(조선총독부고서부분류표)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("COC(조선총독부고서부분류표)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("UDC") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("UDC(국제십진분류표)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("UDC(국제십진분류표)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("LCC") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
			MakeOneQuery ( _T("LC(미국국회도서관분류표)") , lpValue , _SAME_ , lpQuery );
        } else {
			MakeOneQuery ( _T("LC(미국국회도서관분류표)") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("UC") )  == 0 ) {
        MakeOneQuery ( _T("한국대학부호") , lpValue , _SAME_ , lpQuery );
    } else 
    if ( _tcsicmp ( lpField , _T("GP") )  == 0 ) {
        MakeOneQuery ( _T("한국정부기관부호") , lpValue , _SAME_ , lpQuery );
    } else 
    if ( _tcsicmp ( lpField , _T("DPLOLI") )  == 0 ) {
        if ( lpValue[0] == '"' && lpValue[_tcsclen(lpValue)-1] == '"' ) {
            MakeOneQuery ( _T("도서관부호") , lpValue , _SAME_ , lpQuery );
        } else {
            MakeOneQuery ( _T("도서관부호") , lpValue , _RIGHT_ , lpQuery );
        }
    } else 
    if ( _tcsicmp ( lpField , _T("TH") )  == 0 ) {
        MakeOneQuery ( _T("학위구분") , lpValue , _SAME_ , lpQuery );
    } else 
    if ( _tcsicmp ( lpField , _T("PY") )  == 0 ) {
        MakeOneQuery ( _T("발행년도") , lpValue , _SAME_ , lpQuery );
    } else 
    if ( _tcsicmp ( lpField , _T("LG") )  == 0 ) {
        TCHAR   szQuery[514];
        MakeOneQuery ( _T("본문언어") , lpValue , _SAME_ , lpQuery );
        MakeOneQuery ( _T("요약문언어") , lpValue , _SAME_ , szQuery );
        _stprintf ( lpQuery + _tcsclen(lpQuery) , _T("%s OR ") , szQuery );
    } else 
    if ( _tcsicmp ( lpField , _T("FR") )  == 0 ) {
        MakeOneQuery ( _T("간행빈도") , lpValue , _SAME_ , lpQuery );
    } else {
    }

    return 0;
}

INT  CQuery::CheckValue ( )
{
    INT    len = 0;
    TCHAR   Data[500];
  
	CString temp_CtrlAlias ;
	CString temp_Data ;

	
    temp_CtrlAlias.Format (_T("%s"),m_CtrlAlias);
	

	m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("본표제"),temp_Data,-1);


    _tcscpy(Data,temp_Data) ;


    str_trimall( Data , Data, ' ' );
    len = _tcsclen( Data );
    if ( 0 < len && len < 1 ) {
        ::MessageBox ( m_pDialogMgr->GetSafeHwnd() ,_T("표제는 1글자 이상 입력해야 합니다.\n표제의 입력된 문자를 확인하십시요 ") , _T("자료찾기"), MB_ICONWARNING );
        return -1;
    }
	
    m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("키워드1"),temp_Data,-1);
   
	_tcscpy(Data,temp_Data) ;
	
	str_trimall( Data , Data, ' ' );
    len = _tcsclen( Data );
    if ( 0 < len && len < 2 ) {
        ::MessageBox ( m_pDialogMgr->GetSafeHwnd() ,_T("키워드는 2글자 이상 입력해야 합니다.\n키워드의 입력된 문자를 확인하십시요 ") , _T("자료찾기"), MB_ICONWARNING );
        return -2;
    }
   
    m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("키워드2"),temp_Data,-1);
 
	_tcscpy(Data,temp_Data) ;

	str_trimall( Data , Data, ' ' );
    len = _tcsclen( Data );
    if ( 0 < len && len < 2 ) {
        ::MessageBox ( m_pDialogMgr->GetSafeHwnd() ,_T("키워드는 2글자 이상 입력해야 합니다.\n키워드의 입력된 문자를 확인하십시요 ") , _T("자료찾기"), MB_ICONWARNING );
        return -2;
    }
    

	m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("키워드3"),temp_Data,-1);
    _tcscpy(Data,temp_Data);
  
	str_trimall( Data , Data, ' ' );
    len = _tcsclen( Data );
    if ( 0 < len && len < 2 ) {
        ::MessageBox ( m_pDialogMgr->GetSafeHwnd() ,_T("키워드는 2글자 이상 입력해야 합니다.\n키워드의 입력된 문자를 확인하십시요 ") , _T("자료찾기"), MB_ICONWARNING );
        return -2;
    }
     

	m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("저작자"),temp_Data,-1);
    _tcscpy(Data,temp_Data);

    str_trimall( Data , Data, ' ' );
    len = _tcsclen( Data );
    if ( 0 < len && len < 2 ) {
        ::MessageBox ( m_pDialogMgr->GetSafeHwnd() ,_T("저작자는 2글자 이상 입력해야 합니다.\n저작자의 입력된 문자를 확인하십시요 ") , _T("자료찾기"), MB_ICONWARNING );
        return -3;
    }
   

    m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("발행자"),temp_Data,-1);
    _tcscpy(Data,temp_Data);

   str_trimall( Data , Data, ' ' );
    len = _tcsclen( Data );
    if ( 0 < len && len < 2 ) {
        ::MessageBox ( m_pDialogMgr->GetSafeHwnd() ,_T("발행자는 2글자 이상 입력해야 합니다.\n발행자의 입력된 문자를 확인하십시요 ") , _T("자료찾기"), MB_ICONWARNING );
        return -4;
    }
   
    
    m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("분류기호2"),temp_Data,-1);
    _tcscpy(Data,temp_Data);

    str_trimall( Data , Data, ' ' );
    len = _tcsclen( Data );
    if ( 0 < len && len < 3 ) {
        ::MessageBox ( m_pDialogMgr->GetSafeHwnd() ,_T(" 분류기호를 3자리 이상 입력하십시요 ") , _T("자료찾기"), MB_ICONWARNING );
        return -5;
    }

    return 0;
}

INT  CQuery::CheckAlways ( )
{
    const INT CheckAliasCnt = 18;

	INT    len = 0;
    TCHAR *Data[CheckAliasCnt];

	CString sTmpData[CheckAliasCnt]; 
  
    CString tmp_Alias[CheckAliasCnt];
    CString temp_CtrlAlias ;
		
    temp_CtrlAlias.Format (_T("%s"),m_CtrlAlias);


    TCHAR   szTempBuffer1[128] , szTempBuffer2[128] , szMsg[256];
    BOOL   bCheckflag = FALSE;

    TCHAR *CheckAlias[CheckAliasCnt] = 
    {
        _T("본표제")      ,
        _T("키워드1")     ,
        _T("키워드2")     ,
        _T("키워드3")     ,
        _T("저작자")      ,
        _T("발행자")      ,
        _T("표준부호2")   ,
        _T("분류기호2")   ,
        _T("한국대학부호"),
        _T("정부기관부호"),
        _T("학위논문")    ,
        _T("주제")        ,
        _T("서지제어번호부터"),
        _T("서지제어번호까지"),
		_T("단위도서관제어번호"),
        _T("학위수여기관"),
        _T("발행국부호"),
        _T("CIP제어번호")

    };

   for(INT i = 0 ; i < CheckAliasCnt ;i++)
   {    
          
		tmp_Alias[i].Format(_T("%s"),CheckAlias[i]);    
		m_pDialogMgr->GetControlData (temp_CtrlAlias,tmp_Alias[i],sTmpData[i]) ;
	    
		Data[i] = (TCHAR*)malloc( (sTmpData[i].GetLength()+sizeof(TCHAR))*sizeof(TCHAR) );
		_tcscpy(Data[i],sTmpData[i].GetBuffer (0));
  
   }
    
    for ( INT j = 0; j < CheckAliasCnt; j++ ) {
      str_trimall( Data[j] , Data[j] , ' ' );
        len = _tcsclen( Data[j] );
        if ( len > 0 )  {
            if ( FALSE == bCheckflag ) bCheckflag = TRUE;
        }       
    }

    if ( _tcsclen(Data[0]) > 0 && ( _tcsclen(Data[1]) > 0 || _tcsclen(Data[2]) > 0 || _tcsclen(Data[3]) > 0 ) ) {
        ::MessageBox ( m_pDialogMgr->GetSafeHwnd() , _T("\n본표제와 키워드는 같이 검색할 수 없습니다.\n\n본표제와 키워드 중 하나만 입력하십시오.") , _T("자료찾기"), MB_ICONWARNING );
        
        return 100;
    }    

	CString tmp_szTempBuffer1;

	m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("표준부호1"),tmp_szTempBuffer1,-1); 
 
	_tcscpy(szTempBuffer1,tmp_szTempBuffer1);
	
	if ( _tcsclen(szTempBuffer1) > 0 ) {
        if ( _tcsclen(Data[6]) <= 0 ) {
			if( tmp_szTempBuffer1 != _T("적용안함") )
			{
				_stprintf ( szMsg ,_T("%s를 입력하십시오.") , szTempBuffer1 ); 
				::MessageBox ( m_pDialogMgr->GetSafeHwnd() , szMsg , _T("자료찾기"), MB_ICONWARNING );
				return 100;
			}
        }
    } else {
        if ( _tcsclen(Data[6]) > 0 ) {
            _stprintf ( szMsg ,_T("표준부호를 선택하십시오.") ); 
            ::MessageBox ( m_pDialogMgr->GetSafeHwnd() , szMsg , _T("자료찾기"), MB_ICONWARNING );
     
            return 100;
        }
    }

    CString tmp_szTempBuffer2;
    m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("분류기호1"),tmp_szTempBuffer2,-1); 
    _tcscpy(szTempBuffer2,tmp_szTempBuffer2);

    if ( _tcsclen(szTempBuffer2) > 0 ) {
        if ( _tcsclen(Data[7]) <= 0 ) {
			if( tmp_szTempBuffer2 != _T("적용안함") )
			{
				_stprintf ( szMsg ,_T("%s를 입력하십시오.") , szTempBuffer2 ); 
				::MessageBox ( m_pDialogMgr->GetSafeHwnd() , szMsg , _T("자료찾기"), MB_ICONWARNING );
   
				return 100;
			}
        }
    } else {
        if ( _tcsclen(Data[7]) > 0 ) {
            _stprintf ( szMsg ,_T("분류기호를 선택하십시오.") ); 
            ::MessageBox ( m_pDialogMgr->GetSafeHwnd() , szMsg , _T("자료찾기"), MB_ICONWARNING );
            
            return 100;
        }
    }
     
	for( i = 0 ; i<CheckAliasCnt ; i++ )
	{
		free(Data[i]) ;
	}

    if ( FALSE == bCheckflag ) return -3;

    return 0;
}

INT CQuery::DelMaxLengthStr( INT f_MaxStrLen , TCHAR *f_pRawStr )
{

    TCHAR s_Buffer[1024];
    INT i;

    s_Buffer[0] = '\0';

    if( f_MaxStrLen <= 0 || f_pRawStr == NULL ) return -1;

    if( f_MaxStrLen <= (INT)_tcsclen(f_pRawStr) ){
        for( i = 0; i < f_MaxStrLen; i++){
            if( (_TUCHAR)f_pRawStr[i] >= 0x80 ){
                _tcsnccpy( &s_Buffer[i] , &f_pRawStr[i] , 2 );
                i++;
                if( i >= f_MaxStrLen ){
                    s_Buffer[i-1] = '\0';
                    break;
                }
                else{
                    s_Buffer[i+1] = '\0';
                }
            }
            else{
                s_Buffer[i] = f_pRawStr[i];
                if( i == f_MaxStrLen ){
                    s_Buffer[i] = '\0';
                    break;
                }
                else{
                    s_Buffer[i+1] = '\0';
                }
            }
        }
    }
    else{
        return 0;
    }

    _tcscpy( f_pRawStr,s_Buffer );
    return 0;

}

INT  CQuery::GetQueryInfoIndex ( TCHAR *lpFieldAlias )
{
    for ( INT i = 0; i < m_GueryInfoCount; i++ ) {
        if ( _tcscmp( lpFieldAlias , pQueryInfo[i].szAlias ) ==  0 ) {
            return i;
        }
    }
    return -1;
}

INT  CQuery::MakePublicateYearQuery( TCHAR *lpFieldData1, TCHAR *lpFieldData2  , TCHAR *lpQuery )
{
    TCHAR    szQuery[128];

    if ( _tcsclen( lpFieldData1 ) > 0 && _tcsclen( lpFieldData2 ) > 0  ) {
        _stprintf( szQuery, _T("( IPUB_YEAR >= %s AND IPUB_YEAR <= %s )"), lpFieldData1, lpFieldData2 );
    } else
    if ( _tcsclen( lpFieldData1 ) > 0 ) {
        _stprintf( szQuery, _T("( %s <IN> ZPUB_YEAR )"), lpFieldData1 );
    } else 
    if ( _tcsclen( lpFieldData2 ) > 0 ) {
        _stprintf( szQuery, _T("( %s <IN> ZPUB_YEAR )"), lpFieldData2 );
    } else return -1;

    if ( _tcsclen(lpQuery) > 0 ) {
        _stprintf ( lpQuery+_tcsclen(lpQuery) , _T(" AND %s") , szQuery );
    } else {
        _stprintf ( lpQuery , _T("%s") , szQuery );
    }

    return 0;
}

INT  CQuery::MakeCreateDateQuery( TCHAR *lpFieldData1, TCHAR *lpFieldData2  , TCHAR *lpQuery )
{
    TCHAR    szQuery[128];
	CString strDate1;
	CString strDate2;

	strDate1.Format(_T("%s"), lpFieldData1 );
	strDate2.Format(_T("%s"), lpFieldData2 );

    if ( _tcsclen( lpFieldData1 ) > 0 && _tcsclen( lpFieldData2 ) > 0  ) {
        _stprintf( szQuery, _T("( CREATE_DATE >= %s AND CREATE_DATE <= %s )"), strDate1, strDate2 );
    } else
    if ( _tcsclen( lpFieldData1 ) > 0 ) {
		_stprintf( szQuery, _T("( CREATE_DATE >= %s AND CREATE_DATE <= %s )"), strDate1, strDate1 );        
    } else 
    if ( _tcsclen( lpFieldData2 ) > 0 ) {
		_stprintf( szQuery, _T("( CREATE_DATE >= %s AND CREATE_DATE <= %s )"), strDate2, strDate2 );                
    } else return -1;

    if ( _tcsclen(lpQuery) > 0 ) {
        _stprintf ( lpQuery+_tcsclen(lpQuery) , _T(" AND %s") , szQuery );
    } else {
        _stprintf ( lpQuery , _T("%s") , szQuery );
    }

    return 0;
}

INT  CQuery::MakeControlNoQuery( TCHAR *lpFieldData1, TCHAR *lpFieldData2  , TCHAR *lpQuery )
{
    TCHAR    szQuery[1024];
	TCHAR	tmpQuery[1024] = {NULL};
	INT		nCnt = 0 , i;
	TCHAR  * szTmp;
	TCHAR ** szCondition = NULL;
	TCHAR  * tmp;

    if ( _tcsclen( lpFieldData1 ) > 0 && _tcsclen( lpFieldData2 ) > 0  )
	{
		if ( lpFieldData1[_tcsclen(lpFieldData1)-1] == ',' )
		{
			lpFieldData1[_tcsclen(lpFieldData1)-1] = '\0';
		}
		if ( lpFieldData1[0] == ',' )
		{
			_stprintf ( lpFieldData1 , &lpFieldData1[1] );
		}
		szTmp = _tcschr (lpFieldData1 , ',');
		if ( szTmp != NULL ) 
		{
			nCnt++;
			do
			{
				nCnt++;
			}while ( ( szTmp = _tcschr (szTmp + 1, ',') ) != NULL);

			szCondition = ( TCHAR ** ) malloc ( sizeof ( TCHAR * ) * nCnt );
			for ( i = 0 ; i < nCnt ; i++ )
			{
				szCondition[i] = (TCHAR*) malloc ( sizeof ( TCHAR ) * 64 );
				szCondition[i][0] = '\0';
			}
			tmp = _tcstok ( lpFieldData1,_T(",") );
			_tcscpy ( szCondition[0] , tmp ); 
			i = 1 ;
			while ( (tmp = _tcstok(NULL , _T(","))) != NULL)
			{
				_tcscpy ( szCondition[i] , tmp );
				i++;
			}
			for ( i = 0 ; i < nCnt-1 ; i++ )
			{
				if ( _tcsclen(szCondition[i]) > 0  )
					_stprintf ( tmpQuery , _T("%s (CONTROL_NO = %s ) OR ") , tmpQuery , szCondition[i] );
			}
			_stprintf ( szQuery , _T("(%s(CONTROL_NO = %s) )") , tmpQuery , szCondition[i]);
		}
		else
		{
			_stprintf( szQuery, _T("( CONTROL_NO >= %s AND CONTROL_NO <= %s )"), lpFieldData1, lpFieldData2 );
		}
    }
	else if ( _tcsclen( lpFieldData1 ) > 0 ) 
	{
		if ( lpFieldData1[_tcsclen(lpFieldData1)-1] == ',' )
		{
			lpFieldData1[_tcsclen(lpFieldData1)-1] = '\0';
		}
		if ( lpFieldData1[0] == ',' )
		{
			_stprintf ( lpFieldData1 , &lpFieldData1[1] );
		}
		szTmp = _tcschr (lpFieldData1 , ',');
		if ( szTmp != NULL ) 
		{
			nCnt++;
			do
			{
				nCnt++;
			}while ( ( szTmp = _tcschr (szTmp + 1, ',') ) != NULL);

			szCondition = ( TCHAR ** ) malloc ( sizeof ( TCHAR * ) * nCnt );
			for ( i = 0 ; i < nCnt ; i++ )
			{
				szCondition[i] = (TCHAR*) malloc ( sizeof ( TCHAR ) * 64 );
				szCondition[i][0] = '\0';
			}
			
			tmp = _tcstok ( lpFieldData1,_T(",") );
			_tcscpy( szCondition[0], tmp );
			i = 1 ;
			while ( (tmp = _tcstok(NULL , _T(","))) != NULL)
			{
				_tcscpy( szCondition[i], tmp );
				i++;
			}
			for ( i = 0 ; i < nCnt-1 ; i++ )
			{
				if ( _tcsclen(szCondition[i]) > 0  )
					_stprintf ( tmpQuery , _T("%s (CONTROL_NO = %s ) OR ") , tmpQuery , szCondition[i] );
			}
			_stprintf ( szQuery , _T("(%s(CONTROL_NO = %s) )") , tmpQuery , szCondition[i]);
		}
		else
		{
			_stprintf( szQuery, _T("( CONTROL_NO =  %s )"), lpFieldData1 );
		}
    }
	else if ( _tcsclen( lpFieldData2 ) > 0 )
	{
        _stprintf( szQuery, _T("( CONTROL_NO =  %s )"), lpFieldData2 );
    }

	else return -1;

	if ( NULL != szCondition )
	{
		for ( i = 0 ; i < nCnt ; i++ )
		{
			if ( NULL != szCondition[i] )
			{
				free ( szCondition[i] );
				szCondition[i] = NULL;
			}
		}
		free ( szCondition );
		szCondition = NULL;
	}

    if ( _tcsclen(lpQuery) > 0 )
	{
        _stprintf ( lpQuery+_tcsclen(lpQuery) , _T(" AND %s") , szQuery );
    }
	else
	{
        _stprintf ( lpQuery , _T("%s") , szQuery );
    }

    return 0;
}

INT  CQuery::MakeLocalControlNoQuery( TCHAR *lpControlNo, TCHAR *lpQuery )
{
    INT     RowCount , ids ,i , offset;

	TCHAR    szSpeciesKey[128];
	CString sWhere;

    CESL_DataMgr    *pDataMgr;
    
	pDataMgr = m_pDialogMgr->FindDM( _T("DB_제어번호") );
	if( pDataMgr == NULL ) return -1;
    
    ids = MakeControlNoWhere ( _T("CONTROL_NO") , lpControlNo , sWhere , 20 );
	if ( ids != 0  ) {
		if (ids == -100 ) {
			sWhere.Format(_T("단위도서관제어번호는 최대%d개 까지 검색할 수 있습니다.") , 20 );
			::MessageBox( m_pDialogMgr->GetSafeHwnd() , (TCHAR *)(LPCTSTR)sWhere , _T("업무용자료찾기") , MB_ICONINFORMATION );
		}
		return ids;
	}

	pDataMgr->RefreshDataManager( sWhere );	
    RowCount = pDataMgr->GetRowCount();

    if ( RowCount < 0 ) return -3;

    if ( RowCount == 0 ) {
        lpQuery[0] = 0;
        return 100;
    }
	offset = 0;

	for ( i = 0; i < RowCount; i ++ ) {

		CString tmp_szSpeciesKey ;

        tmp_szSpeciesKey.Format (_T("%s"),szSpeciesKey);
        
        m_pDialogMgr->GetDataMgrData (_T("DB_제어번호"),_T("서지_KEY"),tmp_szSpeciesKey,i);
		   
        _tcscpy(szSpeciesKey,tmp_szSpeciesKey);
	     
	    if ( offset > 0 ) {
			offset += _stprintf ( &lpQuery[offset] , _T(" OR "));
		}
		offset += _stprintf ( &lpQuery[offset] , _T("VDKVGWKEY = %s") , szSpeciesKey );
	}
    return 0;
}

INT  CQuery::MakeControlNoWhere ( TCHAR *lpFieldName , TCHAR *lpValue , CString & sWhere , INT MaxArrayCount )
{
    TCHAR   *TokenStr = NULL;
	TCHAR    szTmp[128] , szControlNo[128];

	INT     DummyCount = 0 , i = 0 , ControlNo = 0;
	INT     AccessionCount = 0;

	TCHAR   *mValue = (TCHAR *)malloc( sizeof(TCHAR) * _tcsclen(lpValue) +sizeof(TCHAR) );

	_tcscpy ( mValue , lpValue );
	
    TokenStr = _tcstok( mValue, _T("+") );
	if ( _tcscmp ( TokenStr , lpValue ) !=0 ) {
        _tcscpy ( szControlNo , TokenStr );
		TokenStr = _tcstok( NULL, _T("+") );
		if ( TokenStr != NULL ) {
			DummyCount = _ttoi(TokenStr);
			if ( DummyCount > MaxArrayCount && MaxArrayCount > 0 ) {
				free ( mValue );
				return -100;
			}
		}
		if ( szControlNo[0] >= 97 && szControlNo[0] < 123 ) szControlNo[0] -= 32;
		if ( szControlNo[1] >= 97 && szControlNo[1] < 123 ) szControlNo[1] -= 32;

		szTmp[0] = szControlNo[0];
		szTmp[1] = szControlNo[1];
		wmemcpy ( &szTmp[2] , &szControlNo[2] , 4 );
		_stprintf( &szTmp[6], _T("%07d"), _ttoi( &szControlNo[6] ) );
		ControlNo = _ttoi( &szControlNo[6] );

		sWhere.Format( _T(" %s in (") , lpFieldName );
		sWhere += _T("'");
		sWhere +=  szTmp;
		sWhere += _T("'");

		for ( i = 0; i < DummyCount; i ++ ) {
			_stprintf( &szTmp[6], _T("%07d"), ControlNo +i+1  );
			sWhere += _T(",");
			sWhere += _T("'");
			sWhere +=  szTmp;
			sWhere += _T("'");
		}
		sWhere += _T(")");
		free ( mValue );
		return 0;
	}

	TokenStr = NULL;
    TokenStr = _tcstok( mValue, _T("-") );
	if ( _tcscmp ( TokenStr , lpValue ) !=0 ) {
        _tcscpy ( szControlNo , TokenStr );
		TokenStr = _tcstok( NULL, _T("-") );
		if ( TokenStr != NULL ) {
			DummyCount = _ttoi(TokenStr);
			if ( DummyCount > MaxArrayCount && MaxArrayCount > 0 ) {
				free ( mValue );
				return -100;
			}
		}
		if ( szControlNo[0] >= 97 && szControlNo[0] < 123 ) szControlNo[0] -= 32;
		if ( szControlNo[1] >= 97 && szControlNo[1] < 123 ) szControlNo[1] -= 32;

		szTmp[0] = szControlNo[0];
		szTmp[1] = szControlNo[1];
		wmemcpy ( &szTmp[2] , &szControlNo[2] , 4 );
		_stprintf( &szTmp[6], _T("%07d"), _ttoi( &szControlNo[6] ) );
		ControlNo = _ttoi( &szControlNo[6] );

		sWhere.Format( _T(" %s in (") , lpFieldName );
		sWhere += _T("'");
		sWhere +=  szTmp;
		sWhere += _T("'");

		for ( i = 0; i < DummyCount; i ++ ) {
			if ( ControlNo - i <= 0 ) break;
			_stprintf( &szTmp[6], _T("%07d"), ControlNo - i -1 );
			sWhere += _T(",");
			sWhere += _T("'");
			sWhere +=  szTmp;
			sWhere += _T("'");
		}
		sWhere += _T(")");
		free ( mValue );
		return 0;
	}

	TokenStr = NULL;
    TokenStr = _tcstok( mValue, _T(" ") );
	if ( _tcscmp ( TokenStr , lpValue ) !=0 ) {
		if ( TokenStr[0] >= 97 && TokenStr[0] < 123 ) TokenStr[0] -= 32;
		if ( TokenStr[1] >= 97 && TokenStr[1] < 123 ) TokenStr[1] -= 32;

		szTmp[0] = TokenStr[0];
		szTmp[1] = TokenStr[1];

		wmemcpy ( &szTmp[2] , &TokenStr[2] , 4 );
		_stprintf( &szTmp[6], _T("%07d"), _ttoi( &TokenStr[6] ) );
		szTmp[13] = 0;
		sWhere.Format( _T(" %s in (") , lpFieldName );
		sWhere += _T("'");
		sWhere +=  szTmp;
		sWhere += _T("'");
		AccessionCount++;

		while ( 1 ) {
			TokenStr = _tcstok( NULL , _T(" ") );
			if ( TokenStr == NULL ) break;
			if ( AccessionCount > MaxArrayCount && MaxArrayCount > 0 ) {
				free ( mValue );
				return -100;
			}
			if ( NULL != TokenStr ) {
				if ( TokenStr[0] >= 97 && TokenStr[0] < 123 ) TokenStr[0] -= 32;
				if ( TokenStr[1] >= 97 && TokenStr[1] < 123 ) TokenStr[1] -= 32;

				szTmp[0] = TokenStr[0];
				szTmp[1] = TokenStr[1];

				wmemcpy ( &szTmp[2] , &TokenStr[2] , 4 );
				_stprintf( &szTmp[6], _T("%07d"), _ttoi( &TokenStr[6] ) );
				sWhere += _T(",");
				sWhere += _T("'");
				sWhere +=  szTmp;
				sWhere += _T("'");
				AccessionCount++;
			}
		}
		if ( AccessionCount > 0 ) {
			sWhere += _T(")");
		}
		free ( mValue );
		return 0;
	}
	if ( lpValue[0] >= 97 && lpValue[0] < 123 ) lpValue[0] -= 32;
	if ( lpValue[1] >= 97 && lpValue[1] < 123 ) lpValue[1] -= 32;

	szTmp[0] = lpValue[0];
	szTmp[1] = lpValue[1];
	wmemcpy ( &szTmp[2] , &lpValue[2] , 4 );
	_stprintf( &szTmp[6], _T("%07d"), _ttoi( &lpValue[6] ) );

	sWhere.Format( _T(" %s in ( '%s' )") , lpFieldName , szTmp );
	free ( mValue );
	return 0;
}

INT  CQuery::MakeInfomationTypeQuery( TCHAR *lpFieldData , TCHAR *lpQuery , INT CollectionType )
{
    TCHAR    szTmpQuery[1024];
    TCHAR    szQuery   [1024];

    _tcscpy ( szTmpQuery , lpQuery );
    szQuery[0]    = 0;

	if ( CollectionType == _DAN_ ) {
		if ( _tcscmp( lpFieldData , _T("일반도서") ) == 0 ) {
			_tcscpy ( m_Collection , _T("dan-gm") );
		} 
		else if ( _tcscmp( lpFieldData , _T("학위논문") ) == 0 ) {
			_tcscpy ( m_Collection , _T("dan-dmdp") );
		} 
		else if ( _tcscmp( lpFieldData , _T("교과서") ) == 0 ) {
			_tcscpy ( m_Collection , _T("dan-ha") );
		} 
		else if ( _tcscmp( lpFieldData , _T("학습서") ) == 0 ) {
			_tcscpy ( m_Collection , _T("dan-hr") );
		} 
		else if ( _tcscmp( lpFieldData , _T("만화") ) == 0 ) {
			_tcscpy ( m_Collection , _T("dan-ca") );
		} 
		else if ( _tcscmp( lpFieldData , _T("잡지") ) == 0 ) 
		{
			if ( _tcsclen(lpQuery) > 0 ) {
				_stprintf( szQuery , _T(" ( MAT_CODE_MA ) AND ") );
			} else {
				_stprintf ( szQuery , _T("( MAT_CODE_MA )") );
			}
		} 
		else if ( _tcscmp( lpFieldData , _T("신문") ) == 0 ) 
		{
			if ( _tcsclen(lpQuery) > 0 ) {
				_stprintf( szQuery , _T(" ( MAT_CODE_NP ) AND ") );
			} else {
				_stprintf ( szQuery , _T("( MAT_CODE_NP )") );
			}
		} 
		else if ( _tcscmp( lpFieldData , _T("비도서") ) == 0 ) 
		{
			if ( _tcsclen(lpQuery) > 0 ) {
				_stprintf( szQuery , _T(" ( ( FORM_CODE_CF OR FORM_CODE_MU OR FORM_CODE_VM OR FORM_CODE_MP ) OR ( FORM_CODE_TX  AND ( MEDIA_CODE_XH OR MEDIA_CODE_XA OR MEDIA_CODE_XZ )) ) AND ") );
			} else {
				_stprintf ( szQuery , _T("( ( FORM_CODE_CF OR FORM_CODE_MU OR FORM_CODE_VM OR FORM_CODE_MP ) OR ( FORM_CODE_TX  AND ( MEDIA_CODE_XH OR MEDIA_CODE_XA OR MEDIA_CODE_XZ )) )") );
			}
		} else
		if ( _tcscmp( lpFieldData , _T("점자자료") ) == 0 ) {
			if ( _tcsclen(lpQuery) > 0 ) {
				_stprintf( szQuery , _T(" ( MEDIA_CODE_KA ) AND ") );
			} else {
				_stprintf ( szQuery , _T("( MEDIA_CODE_KA )") );
			}
		}
	} else {
		if ( _tcscmp( lpFieldData , _T("일반도서") ) == 0 ) {
	        if ( _tcsclen(lpQuery) > 0 ) {
	            _stprintf( szQuery , _T(" ( MAT_CODE_GM ) AND ") );
	        } else {
	            _stprintf ( szQuery , _T("( MAT_CODE_GM )") );
	        }
		} else
		if ( _tcscmp( lpFieldData , _T("학위논문") ) == 0 ) {
	        if ( _tcsclen(lpQuery) > 0 ) {
	            _stprintf( szQuery , _T(" ( MAT_CODE_DM OR MAT_CODE_DP ) AND ") );
	        } else {
	            _stprintf ( szQuery , _T("( MAT_CODE_DM OR MAT_CODE_DP )") );
	        }
		} else
		if ( _tcscmp( lpFieldData , _T("교과서") ) == 0 ) {
	        if ( _tcsclen(lpQuery) > 0 ) {
	            _stprintf( szQuery , _T(" ( MAT_CODE_HA ) AND ") );
	        } else {
	            _stprintf ( szQuery , _T("( MAT_CODE_HA )") );
	        }
		} else
		if ( _tcscmp( lpFieldData , _T("학습서") ) == 0 ) {
	        if ( _tcsclen(lpQuery) > 0 ) {
	            _stprintf( szQuery , _T(" ( MAT_CODE_HR ) AND ") );
	        } else {
	            _stprintf ( szQuery , _T("( MAT_CODE_HR )") );
	        }
		} else
		if ( _tcscmp( lpFieldData , _T("만화") ) == 0 ) {
	        if ( _tcsclen(lpQuery) > 0 ) {
	            _stprintf( szQuery , _T(" ( MAT_CODE_CA ) AND ") );
	        } else {
	            _stprintf ( szQuery , _T("( MAT_CODE_CA )") );
	        }
		} else
		if ( _tcscmp( lpFieldData , _T("잡지") ) == 0 ) {
			if ( _tcsclen(lpQuery) > 0 ) {
				_stprintf( szQuery , _T(" ( MAT_CODE_MA ) AND ") );
			} else {
				_stprintf ( szQuery , _T("( MAT_CODE_MA )") );
			}
		} else
		if ( _tcscmp( lpFieldData , _T("신문") ) == 0 ) {
			if ( _tcsclen(lpQuery) > 0 ) {
				_stprintf( szQuery , _T(" ( MAT_CODE_NP ) AND ") );
			} else {
				_stprintf ( szQuery , _T("( MAT_CODE_NP )") );
			}
		} else
		if ( _tcscmp( lpFieldData , _T("비도서") ) == 0 ) {
			if ( _tcsclen(lpQuery) > 0 ) {
				_stprintf( szQuery , _T(" ( ( FORM_CODE_CF OR FORM_CODE_MU OR FORM_CODE_VM OR FORM_CODE_MP ) OR ( FORM_CODE_TX  AND ( MEDIA_CODE_XH OR MEDIA_CODE_XA OR MEDIA_CODE_XZ )) ) AND ") );
			} else {
				_stprintf ( szQuery , _T("( ( FORM_CODE_CF OR FORM_CODE_MU OR FORM_CODE_VM OR FORM_CODE_MP ) OR ( FORM_CODE_TX  AND ( MEDIA_CODE_XH OR MEDIA_CODE_XA OR MEDIA_CODE_XZ )) )") );
			}
		} else
		if ( _tcscmp( lpFieldData , _T("점자자료") ) == 0 ) {
			if ( _tcsclen(lpQuery) > 0 ) {
				_stprintf( szQuery , _T(" ( MEDIA_CODE_KA ) AND ") );
			} else {
				_stprintf ( szQuery , _T("( MEDIA_CODE_KA )") );
			}
		}
	}
    _stprintf ( lpQuery , _T("%s%s ") , szQuery , szTmpQuery );

    return 0;

}

INT  CQuery::MakeLibQuery( TCHAR *lpQuery )
{
    INT      LibCount = 0 , offset = 0;
    TCHAR    *TokenStr = NULL;
    TCHAR    szLibType[32] , szRegion[32] , szLibCode[128] ; 

    lpQuery[0] = 0;
    
    CString tmp_szLibType;
	CString tmp_szRegion;
	CString tmp_szLibCode;
    
	CString temp_CtrlAlias;

	temp_CtrlAlias.Format(_T("%s"),m_CtrlAlias);

 
    m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("관종"),tmp_szLibType,-1);
	m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("지역"),tmp_szRegion,-1);
	m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("도서관부호"),tmp_szLibCode,-1);
 
    _tcscpy(szLibType,tmp_szLibType);
    _tcscpy(szRegion,tmp_szRegion);
	_tcscpy(szLibCode,tmp_szLibCode);

    str_trimall( szLibType  , szLibType , ' ' );
    str_trimall( szRegion   , szRegion  , ' ' );
    str_ltrim  ( szLibCode , szLibCode, ' ' );
    str_rtrim  ( szLibCode , szLibCode, ' ' );

	if ( _tcsclen(szLibCode) > 0 ) {
        MakeOneQuery ( _T("도서관부호") , szLibCode , _SAME_ , lpQuery );
		return 0;
	} else 
	if ( _tcsclen(szLibType) > 0 ) {
        _stprintf( szLibCode , _T("%s%s*") , szLibType,szRegion );
        _stprintf( lpQuery , _T("%s <IN> ZLIB_CODE") , szLibCode );
	} else 
	if ( _tcsclen(szLibType) <= 0 && _tcsclen(szRegion) > 0 ) {
        return -100;
	}

    return 0;
}

VOID  CQuery::MakeSortInfo( TCHAR *lpSortInfo )
{
    INT     offset = 0;
    TCHAR    szSortTitle[32] , szSortPublisher[32] ,  szTitle[1024] , szPublisher[1024]; 
	TCHAR	szSortPubYear[32], szPubYear[1024];
    lpSortInfo[0] = 0;

    CString tmp_szTitle ;
	CString tmp_szPublisher ;
	CString tmp_szPubYear;
	CString temp_CtrlAlias ;

	temp_CtrlAlias.Format (_T("%s"),m_CtrlAlias);


    m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("본표제"),tmp_szTitle,-1);
    m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("발행자"),tmp_szPublisher,-1);
	m_pDialogMgr->GetControlData (temp_CtrlAlias,_T("발행년"),tmp_szPubYear,-1);

    _tcscpy(szTitle,tmp_szTitle);
	_tcscpy(szPublisher,tmp_szPublisher);
	_tcscpy(szPubYear,tmp_szPubYear);

    str_trimall( szTitle  , szTitle , ' ' );
    str_trimall( szPublisher   , szPublisher  , ' ' );
	str_trimall( szPubYear   , szPubYear  , ' ' );

    ((CComboBox *)m_pDialogMgr->GetDlgItem(IDC_cSORT_TITLE))->GetLBText( ((CComboBox *)m_pDialogMgr->GetDlgItem(IDC_cSORT_TITLE))->GetCurSel() , szSortTitle );
    ((CComboBox *)m_pDialogMgr->GetDlgItem(IDC_cSORT_PUBLISHER))->GetLBText( ((CComboBox *)m_pDialogMgr->GetDlgItem(IDC_cSORT_PUBLISHER))->GetCurSel() , szSortPublisher );
	((CComboBox *)m_pDialogMgr->GetDlgItem(IDC_cSORT_PUBYEAR))->GetLBText( ((CComboBox *)m_pDialogMgr->GetDlgItem(IDC_cSORT_PUBYEAR))->GetCurSel() , szSortPubYear );

    if ( _tcscmp( szSortTitle , _T("정렬안함") ) != 0  ) {
        if ( _tcsclen(szTitle) > 0 ) {
            if ( _tcscmp( szSortTitle , _T("오름차순") ) == 0  ) {
                offset += _stprintf( lpSortInfo , _T("SORT_TITLE ASC") ); 
            } else {
                offset += _stprintf( lpSortInfo , _T("SORT_TITLE DESC") ); 
            }
        }
    }
    if ( _tcscmp( szSortPublisher , _T("정렬안함") ) != 0  ) {
        if ( _tcsclen(szTitle) > 0 ) {
            if ( _tcscmp( szSortPublisher , _T("오름차순") ) == 0  ) {
                offset += _stprintf( lpSortInfo+offset , _T(" SORT_PUBLISHER ASC") ); 
            } else {
                offset += _stprintf( lpSortInfo+offset , _T(" SORT_PUBLISHER DESC") ); 
            }
        }
    }
	if ( _tcscmp( szSortPubYear , _T("정렬안함") ) != 0  ) {
            if ( _tcscmp( szSortPubYear , _T("오름차순") ) == 0  ) {
                offset += _stprintf( lpSortInfo+offset , _T(" IPUB_YEAR ASC") ); 
            } else {
                offset += _stprintf( lpSortInfo+offset , _T(" IPUB_YEAR DESC") ); 
            }
    }
}

VOID  CQuery::GetSortInfo( TCHAR *lpSortInfo )
{
    _tcscpy ( lpSortInfo , m_SortInfo );
}
VOID  CQuery::GetCollection( TCHAR *lpCollection )
{
    _tcscpy ( lpCollection , m_Collection );
}


VOID  CQuery::str_trimall(TCHAR   *strln , TCHAR   *strline1, TCHAR    var )
{  
  	INT     i = 0, j = 0;

    for ( i = 0; strln[i] != '\0' && strln[i] != '\n'; i++ ) {
        if ( strln[i] == var );
        else { strline1[j] = strln[i]; j++; }
    }
    strline1[j] ='\0';
 

}



VOID    CQuery::str_ltrim	(	TCHAR   *strln   , TCHAR   *strline1, TCHAR    var )
{

	INT i = 0;

    for ( i = 0; strln[i] != '\0' && strln[i] != '\n'; i++ ){
        if(strln[i] != var) {
            _tcscpy( strline1, &strln[i] );
            return;
        }
    }
    strline1[0] = '\0';

}

VOID    CQuery::str_rtrim	(	TCHAR   *strln   ,  TCHAR   *strline1,   TCHAR    var	)
{

	INT i = 0;

    _tcscpy( strline1, strln );
    for ( i = _tcsclen(strln)-1; i >= 0; i-- ){
        if(strln[i] != var) {
            strline1[i+1] = '\0';
            return;
        }
    }
    strline1[0] = '\0';

}

VOID CQuery::GetSearchKeyword(TCHAR* pWord)
{
	CString strTmp;

	strTmp.Format(_T("%s"), pWord);			
	INT iCh = m_MakeSearchData->ChangeHanjaToHangul(strTmp);
	if ( 0 != iCh ) return;
	
	iCh = m_MakeSearchData->GetKeyWord( strTmp );
	if ( 0 != iCh ) return;	

	_stprintf(pWord, _T("%s"), strTmp );
}

void CQuery::SpaceTrim(TCHAR *in, TCHAR *out)
{
	CString str;
	str.Format(_T("%s") , in );
	str.TrimLeft();
	str.TrimRight();
	str.TrimLeft(_T("\t"));
	str.TrimRight(_T("\t"));
	str.TrimLeft(_T("\n"));
	str.TrimRight(_T("\n"));
	_tcscpy ( out , str.GetBuffer(0));
}
