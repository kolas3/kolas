#include "stdafx.h"
#include "get_duplicate_key.h"
//#include _T("..\..\공통/K2UP_Common/MakeSearchData.h")





#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif



#define     SPECIAL_CHAR_COUNT 26

typedef struct _SPECIAL_CHAR_STRU {
    TCHAR          Comment[64];
    _TUCHAR s_FirstChar;
    _TUCHAR s_SecondChar;
    _TUCHAR e_FirstChar;
    _TUCHAR e_SecondChar;
} tSPECIAL_CHAR_STRU;

tSPECIAL_CHAR_STRU s_DelCharInfo[SPECIAL_CHAR_COUNT] = {
    { _T("한글")       , 0xB0 , 0xA1 , 0xC8 , 0xFE },
    { _T("한자")       , 0xCA , 0xA1 , 0xFD , 0xFE },
    { _T("일어")       , 0xAA , 0xA1 , 0xAA , 0xF3 },
    { _T("일어")       , 0xAB , 0xA1 , 0xAB , 0xF6 },
    { _T("그리스어")   , 0xA5 , 0xC1 , 0xA5 , 0xD8 },
    { _T("그리스어")   , 0xA5 , 0xE1 , 0xA5 , 0xF8 },
    { _T("라틴어")     , 0xA8 , 0xA1 , 0xA8 , 0xA4 },
    { _T("라틴어")     , 0xA8 , 0xA6 , 0xA8 , 0xA6 },
    { _T("라틴어")     , 0xA8 , 0xA8 , 0xA8 , 0xAF },
    { _T("라틴어")     , 0xA9 , 0xA1 , 0xA9 , 0xAF },  // AA => A1
    { _T("러시아어")   , 0xAC , 0xA1 , 0xAC , 0xC1 },
    { _T("러시아어")   , 0xAC , 0xD1 , 0xAC , 0xF1 },
    { _T("한글자모")   , 0xA4 , 0xA1 , 0xA4 , 0xD3 },
    { _T("한글고어")   , 0xA4 , 0xD5 , 0xA4 , 0xFE },
    { _T("숫자")       , 0xA3 , 0xB0 , 0xA3 , 0xB9 },
    { _T("영문대문자") , 0xA3 , 0xC1 , 0xA3 , 0xDA },
    { _T("영문소문자") , 0xA3 , 0xE1 , 0xA3 , 0xFA },
	{ _T("그림문자")   , 0xA1 , 0xA1 , 0xA1 , 0xFE }, // 그림문자1
    { _T("그림문자")   , 0xA2 , 0xA1 , 0xA2 , 0xE5 }, // 그림문자2
    { _T("그림문자")   , 0xA3 , 0xA1 , 0xA3 , 0xFE }, // 그림문자3
    { _T("그림문자")   , 0xA5 , 0xA1 , 0xA5 , 0xAA }, // 그림문자4 
    { _T("그림문자")   , 0xA5 , 0xB0 , 0xA5 , 0xB9 }, // 그림문자5
    { _T("그림문자")   , 0xA7 , 0xA1 , 0xA7 , 0xFE }, // 그림문자6
    { _T("그림문자")   , 0xA8 , 0xB1 , 0xA8 , 0xFE }, // 그림문자7
    { _T("그림문자")   , 0xA9 , 0xB0 , 0xA9 , 0xFE }, // 그림문자8
    { _T("한글확장")   , 0x81 , 0x20 , 0xC5 , 0xFF }  // 확장한글 => 한글확장    
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGetDuplicateKey::CGetDuplicateKey(CESL_Mgr *pParent /*=NULL*/) 
{
	Trans = (CIndexTool*)pParent->m_pInfo->pIndexTool ;
	//Trans->GetIndex ()->GetIndex ()

}


CGetDuplicateKey::~CGetDuplicateKey() 
{

}

INT CGetDuplicateKey::SetMarcStruct(TCHAR *fi_marc) 
{
	INT ids;

    m_pMarc.Format (_T("%s"),fi_marc);
    
	/*-- MARC 구조체 만들기 --*/
    
    m_pMarcMgr ;
	ids = m_pMarcMgr->Decoding(m_pMarc, &m_marc);
	
	if( ids )
		return -1;

	return 0;
}



INT CGetDuplicateKey::FreeMarcStruct() 
{   
	
//	FreeMarcStru(m_In_MarcStru);

	return 0;
}

// 표제 가져오기
INT CGetDuplicateKey::Get245Title(TCHAR *fo_Title) 
{
	CString		szBuffer;
	TCHAR	    s_SubFieldData[1024];
	TCHAR		*pToken;
	INT			ids;

//245$a 가져오기 
	
	CString temp_s_SubFieldData;
	ids = m_pMarcMgr->GetItem(&m_marc, _T("245$a"), temp_s_SubFieldData);
	if ( ids<0 )
	{
		//AfxMessageBox(_T("m_pMarcMgr->GetItem(245$a) Error!!!"));	
		return -1;
	}
    _tcscpy(s_SubFieldData,temp_s_SubFieldData.GetBuffer(0));
    

	//----------------------------------------------//
	// : 나 ; 의 뒷부분 자료 제거하기..
	//----------------------------------------------//
	szBuffer = s_SubFieldData;

	if ( szBuffer.Find(_T(";")) != -1 )
	{
		pToken = _tcstok(s_SubFieldData, _T(";"));
		if ( pToken != NULL ) {
			_tcscpy(s_SubFieldData, pToken);
			szBuffer = s_SubFieldData;
		}
	}
	
	if ( szBuffer.Find(_T(":")) != -1 )
	{
		pToken = _tcstok(s_SubFieldData, _T(":"));
		if ( pToken != NULL ) {
			_tcscpy(s_SubFieldData, pToken);	
			szBuffer = s_SubFieldData;
		}
	}
    
	//----------------------------------------------//
	// ()안의 내용 제거하기
	//----------------------------------------------//
	if ( szBuffer.Find(_T("(")) != -1 && szBuffer.Find(_T(")")) != -1  )
	{
		szBuffer.TrimLeft();  _tcscpy(s_SubFieldData, szBuffer.GetBuffer(0));
		if ( s_SubFieldData[0] == '(' )
		{
			pToken = _tcstok(s_SubFieldData, _T(")"));
			if ( pToken != NULL ) {
				pToken = _tcstok(NULL, _T(")"));
				if ( pToken != NULL )
					_tcscpy(s_SubFieldData, pToken);	
				else
					_tcscpy(s_SubFieldData, _T(""));	
			}
		}
		else
		{
			pToken = _tcstok(s_SubFieldData, _T("("));
			if ( pToken != NULL ) {
				_tcscpy(s_SubFieldData, pToken);	
			}
		}		
	}
    
	// 공백제거
	
	Get_Keyword(s_SubFieldData, s_SubFieldData,TRUE);
	
	if ( _tcsclen(s_SubFieldData) >= 20 )
		// 10글자로 제한
		s_SubFieldData[20] = '\0';

	// 한자를 한글로 변환 
	
	CString strTemp ;
	strTemp.Format (_T("%s"),s_SubFieldData );
	Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
	_tcscpy(fo_Title,strTemp.GetBuffer (0));
	
	//_tcscpy(fo_Title, s_SubFieldData);

	return 0;
}

// 발행자 가져오기
INT CGetDuplicateKey::Get260Author(TCHAR *fo_Author) 
{
	TCHAR	   s_SubFieldData[1024];
	INT			ids;


	CString temp_s_SubFieldData;
	ids = m_pMarcMgr->GetItem(&m_marc, _T("260$b"), temp_s_SubFieldData);

    if(!temp_s_SubFieldData.IsEmpty ()) 
	{
	   _tcscpy(s_SubFieldData,temp_s_SubFieldData.GetBuffer(0));
	}
	else
	{
       _tcscpy(s_SubFieldData,_T(""));
	}
	// 공백제거
	Get_Keyword(s_SubFieldData, s_SubFieldData,TRUE);
	
	if ( _tcsclen(s_SubFieldData) >= 4 )
		//2글자로 제한
		s_SubFieldData[4] = '\0';


	// 한자를 한글로 변환 
    CString strTemp ;
	strTemp.Format (_T("%s"),s_SubFieldData );
	Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);

	_tcscpy(fo_Author, strTemp.GetBuffer (0));

	return 0;
}


// 2007/10/02 ADD BY PDJ
// 발행년 가져오기 260$c 로 우선순위 변경 및  validation 추가
int CGetDuplicateKey::Get008PubYear(TCHAR *fo_PubYear) 
{
	BOOL		bFlag;
	int			ids;
   
	bFlag = TRUE;
	CString strItemData;
    CArray<CString, CString&> array;
	
	ids = m_pMarcMgr->GetItem(&m_marc, _T("260$c"), strItemData,&array);		
	if( !strItemData.IsEmpty () )
	{
		_tcscpy(fo_PubYear, strItemData.GetBuffer (0));
	}
	else
	{
		_tcscpy(fo_PubYear,_T(""));
	}

	// 260 $c의 발행년이 숫자로만 구성되어 있지 않은경우
	for ( INT i = 0 ; i < _tcslen(fo_PubYear) ; i++ ) 
	{
		if( ( fo_PubYear[i] < 48 ) || ( fo_PubYear[i] > 57 ) ) 
		{
			_tcscpy(fo_PubYear,_T(""));			
			break;
		}				
	}
	
	if ( _tcslen(fo_PubYear) <= 0 )
	{
		// 008 Tag 가져오기    
		CString temp_s_FieldData;
		ids = m_pMarcMgr->GetItem(&m_marc, _T("008@7-10"), temp_s_FieldData);
		
		if(!temp_s_FieldData.IsEmpty ())
		{
			_tcscpy(fo_PubYear,temp_s_FieldData) ;
		}
	}

	return 0;
}

// 면장수 가져오기
INT CGetDuplicateKey::Get300Page(TCHAR *fo_Page) 
{
	CString		szBuffer;
//	TCHAR		s_SubFieldCode[8];
	TCHAR	    *pToken;
	TCHAR		s_Temp[64];
	INT			ids;

	
	// 300 $a 가져오기
	CString strItemData;
    CArray<CString, CString&> array;
	ids = m_pMarcMgr->GetItem(&m_marc, _T("300$a"), strItemData,&array);
	if ( ids<0 )
	{
		return -2;
	}
	
	if ( array.GetSize () == 0 )
		_tcscpy(fo_Page, _T(""));

	else
	{
		szBuffer = strItemData.GetBuffer (0);
		szBuffer.MakeUpper();
	    _tcscpy(s_Temp, szBuffer.GetBuffer(0)); 
		
		// p 라는 단위가 있을 경우
		if ( szBuffer.Find(_T("P")) != -1 )
		{
			pToken = _tcstok(s_Temp, _T("P"));
			if ( pToken != NULL ) {
				_tcscpy(s_Temp, pToken);
				szBuffer = s_Temp;
			}
		}
		
		// 숫자만 가져오기
		GetInteger(s_Temp, fo_Page);		
	}
	return 0;
}

// 크기 가져오기
INT CGetDuplicateKey::Get300Size(TCHAR *fo_Size) 
{
	CString		szBuffer;
//	TCHAR		s_SubFieldCode[8];
	TCHAR		 *pToken;
	TCHAR		s_Temp[64];
	INT			ids;
    
	CString strItemData;
    CArray<CString, CString&> array;

	// 300 $c 가져오기
	
	
	CString temp_s_SubFieldData;
	ids = m_pMarcMgr->GetItem(&m_marc, _T("300$c"), strItemData,&array);
    if(ids<0)
	{
		_tcscpy(fo_Size, _T(""));
		return -1 ;
	}

	if(array.GetSize () == 0) 
	{
	   _tcscpy(fo_Size, _T(""));
	}
	else
	{
		szBuffer = strItemData.GetBuffer(0);
		szBuffer.MakeUpper();
		_tcscpy(s_Temp, szBuffer.GetBuffer(0));

		if ( szBuffer.Find(_T("C")) != -1 )
		{
			pToken = _tcstok(s_Temp, _T("C"));
			if ( pToken != NULL ) {
				_tcscpy(s_Temp, pToken);
			}
		}

		// 숫자만 가져오기
		GetInteger(s_Temp, fo_Size);
	}

	return 0;
}

// 가격 가져오기
INT CGetDuplicateKey::Get950Price(TCHAR *fo_Price) 
{
	CString		szBuffer;
	TCHAR		s_Temp[64];
	INT			ids;
	INT			i_idx;
    TCHAR       **s_SubFieldData ;
	CString strItemData;
    CArray<CString, CString&> array;

	i_idx = 0;
	
	// 950 $a 가져오기 
	ids = m_pMarcMgr->GetItem(&m_marc, _T("950$a"), strItemData,&array);
    if ( ids<0 )
	{
		return -2;
	}

	if ( array.GetSize () == 0 )
		_tcscpy(fo_Price, _T(""));
	else
	{   
		_tcscpy(s_SubFieldData[0],strItemData.GetBuffer (0));
		for ( INT i = 0 ; i < _tcsclen(s_SubFieldData[0]) ; i++ )
		{
			if (  s_SubFieldData[0][i] >= '0' && s_SubFieldData[0][i] <= '9' )
				s_Temp[i_idx++] = s_SubFieldData[0][i];
			else if ( s_SubFieldData[0][i] == '.' )
				break;
		}
		s_Temp[i_idx] = '\0';
	    _tcscpy(fo_Price, s_Temp);
	}



	return 0;
}

// 기타저자 가져오기
INT CGetDuplicateKey::GetEtcAuthor(TCHAR *fo_Author) 
{
	CString		szBuffer, szTemp;
	TCHAR	    s_Author2[1024], s_Author3[128], s_Author4[128];
	INT			ids, i;
	

	szBuffer = _T("");
	szTemp = _T("");
	
	_tcscpy(s_Author2, _T(""));
	_tcscpy(s_Author3, _T(""));
	_tcscpy(s_Author4, _T(""));
		


	// 245 $d 가져오기	 
	CString strItemData;
    CArray<CString, CString&> array;
	 
	ids = m_pMarcMgr->GetItem(&m_marc, _T("245$d"), strItemData,&array);

	if ( array.GetSize () > 0 )
	{
		// 2007/10/02 ADD BY PDJ
		// 역할어 삭제		
		Trans->GetIndex ()->TrimRoleWord(strItemData);

		//2004-03-29[이학중]
		_tcscpy(s_Author3 , strItemData.GetBuffer(0));
	
		// 공백제거
		Get_Keyword(s_Author3, s_Author3,TRUE);
	
		if ( _tcsclen(s_Author3) >= 10 )
			//5글자로 제한
			s_Author3[10] = '\0';

		// 한자를 한글로 변환 
		CString strTemp ;
		strTemp.Format (_T("%s"),s_Author3 );
		Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
        _tcscpy(s_Author3 ,strTemp.GetBuffer (0));

        //pTrans.ConvertChineseTokorean(s_Author3,s_Author3,TRUE);

		if ( szBuffer.IsEmpty() )
				szBuffer = s_Author3;
		else 
		{
			szTemp.Format(_T(" %s"), s_Author3);
			szBuffer += szTemp;
		}
	}


	// 245 $e 가져오기
    ids = m_pMarcMgr->GetItem(&m_marc, _T("245$e"), strItemData,&array);

	if ( array.GetSize () >0 )
	{
		for ( i= 0 ; i < array.GetSize () ; i++ )
		{   
			// 2007/10/02 ADD BY PDJ
			// 역할어 삭제		
			Trans->GetIndex ()->TrimRoleWord(strItemData);

			//2004-03-29[이학중]
			_tcscpy(s_Author2, array.GetAt(i).GetBuffer(0) );
	
			// 공백제거
			Get_Keyword(s_Author2, s_Author2,TRUE);
		
			if ( _tcsclen(s_Author2) >= 10 )
				//5글자로 제한
				s_Author2[10] = '\0';

			// 한자를 한글로 변환 
			//pTrans.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
			CString strTemp ;
			strTemp.Format (_T("%s"),s_Author2 );
			Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
			_tcscpy(s_Author2 ,strTemp.GetBuffer (0));
			
			//pTrans.ConvertChineseTokorean(s_Author2,s_Author2,TRUE);

			if ( szBuffer.IsEmpty() )
				szBuffer = s_Author2;
			else 
			{
				szTemp.Format(_T(" %s"), s_Author2);
				szBuffer += szTemp;
			}			
		}
	}

	szBuffer.TrimLeft();
	_tcscpy(fo_Author, szBuffer.GetBuffer(0));

	return 0;
}




// ISBN 가져오기
INT CGetDuplicateKey::Get020Isbn(TCHAR *fo_Isbn) 
{
	CString		szBuffer;
	CString		strItemData;
	CString		strISBNList;
	CString		strEAISBN;
	CString		strSetISBN;
    CArray<CString, CString&> array;    
	INT			nCount;		
	INT			ids;
	INT			nOffset;
	INT			i;

	array.RemoveAll () ;
	// 020 $a 가져오기
	ids = m_pMarcMgr->GetItem(&m_marc, _T("세트ISBN"), strSetISBN,&array);	
    ids = m_pMarcMgr->GetItem(&m_marc, _T("020$a"), strItemData,&array);

	nCount = array.GetSize();
	strISBNList = "";
	for ( i=0 ; i<nCount ; i++ )
	{
		strItemData = array.GetAt(i);
		// 세트 ISBN은 뺀다.
		if ( strItemData == strSetISBN ) continue;			
		if ( strItemData.Left(1) < _T("0") || strItemData.Left(1) > _T("9") ) continue;

		nOffset = strItemData.Find(_T("("), 0 );
		if ( nOffset > 0 ) 
		{
			strEAISBN = strItemData.Left(nOffset); 
		}
		else
		{
			strEAISBN = strItemData;
		}
		
		strEAISBN.Replace(_T("-"), _T(""));
		strEAISBN.Replace(_T("("), _T(""));
		strEAISBN.Replace(_T(")"), _T(""));		

		// 2017/09/22 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// KOLIS-NET OpenAPI
/*//BEFORE-----------------------------------------------------------------------
		// Check Digit 제외하기
		if ( strEAISBN.GetLength() == 10 ) strEAISBN = strEAISBN.Left(9)+_T("*");
		if ( strEAISBN.GetLength() == 13 ) strEAISBN = strEAISBN.Left(12)+_T("*");

		
		if ( i > 0 ) strISBNList += _T(",");
		strISBNList += strEAISBN;
		
		// 978에 대한 처리
		if ( strEAISBN.Left(3) == _T("978") ) strISBNList += _T(",") + strEAISBN.Mid(3); 
		
		// 89에 대한 처리
		if ( strEAISBN.Left(2) == _T("89") ) strISBNList += _T(",978") + strEAISBN; 
*///AFTER------------------------------------------------------------------------
		if ( i > 0 ) strISBNList += _T("|");
		strISBNList += strEAISBN;
		
		// 978에 대한 처리
		if ( strEAISBN.Left(3) == _T("978") ) strISBNList += _T("|") + strEAISBN.Mid(3); 
		
		// 89에 대한 처리
		if ( strEAISBN.Left(2) == _T("89") ) strISBNList += _T("|978") + strEAISBN; 
/*//END------------------------------------------------------------------------*/	
	}

	if ( strISBNList != _T("") )
	{
		_stprintf(fo_Isbn, _T("(%s)"), strISBNList );
	}
	else
	{
		_stprintf(fo_Isbn, _T("") );
	}	
	
	return 0;
}

//다권본인지 검사한다.
BOOL CGetDuplicateKey::IsDupVol()
{
	CString		szBuffer;
	INT			ids;


	// 245 $n 	
	CString strItemData;
    CArray<CString, CString&> array;
    
	ids = m_pMarcMgr->GetItem(&m_marc, _T("245$n"), strItemData,&array);	
	if ( !strItemData.IsEmpty () )
	{
		return TRUE;
	}
	


	// 245 $p
    ids = m_pMarcMgr->GetItem(&m_marc, _T("245$p"), strItemData,&array);
   	if ( !strItemData.IsEmpty () )
	{
		return TRUE;
	}
    
  
	// 505 $n
    ids = m_pMarcMgr->GetItem(&m_marc, _T("505$n"),strItemData,&array);
	if ( !strItemData.IsEmpty () )
	{
		return TRUE;
	}


	// 049 $v
    ids = m_pMarcMgr->GetItem(&m_marc, _T("049$v"), strItemData,&array);
   	if ( !strItemData.IsEmpty () )
	{
		return TRUE;
	}

	
	// 300 $a
    ids = m_pMarcMgr->GetItem(&m_marc, _T("300$a"), strItemData,&array);
	if ( !strItemData.IsEmpty ()  )
	{
		szBuffer = strItemData.GetBuffer (0);

	    if ( szBuffer.Find(_T("책")) != -1 )
		{
			return TRUE;
		}
	}
	

	return FALSE;
}


// 비교할 표제 가져오기
INT CGetDuplicateKey::GetCompareAuthor(CString *fo_Author) 
{
	CString		szBuffer, szTemp;
	TCHAR        s_Author[1024];
	INT			ids;

	szBuffer = _T("");
    
	
	CString strItemData;
    CArray<CString, CString&> array;
    
	//245 $a 
	ids = m_pMarcMgr->GetItem(&m_marc, _T("245$a"), strItemData,&array);
	if ( array.GetSize () > 0 )
	{
		_tcscpy(s_Author, strItemData.GetBuffer (0));

		// 공백제거
		// 공백제거
		Get_Keyword(s_Author, s_Author,TRUE);
	
		// 한자를 한글로 변환 
		//pTrans.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
        //pTrans.ConvertChineseTokorean(s_Author,s_Author,TRUE);
		CString strTemp ;
		strTemp.Format (_T("%s"),s_Author );
		Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
		_tcscpy(s_Author,strTemp.GetBuffer (0)); 


		szBuffer.Format (_T("%s"), s_Author);
	}
	
	// 245 $b 
    ids = m_pMarcMgr->GetItem(&m_marc, _T("245$b"), strItemData,&array);
    if (array.GetSize () >0)
	{
	    _tcscpy(s_Author, strItemData.GetBuffer (0));

		// 공백제거
		Get_Keyword(s_Author, s_Author,TRUE);
	
		// 한자를 한글로 변환 
		//pTrans.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
        //pTrans.ConvertChineseTokorean(s_Author,s_Author,TRUE);
		CString strTemp ;
		strTemp.Format (_T("%s"),s_Author );
		Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
		_tcscpy(s_Author,strTemp.GetBuffer (0));


		if ( szBuffer.IsEmpty() )
			szBuffer = s_Author;
		else
		{
			szTemp = s_Author;
			szBuffer += szTemp;
		}
	}
	

	// 245 $c 

    ids = m_pMarcMgr->GetItem(&m_marc, _T("245$c"), strItemData,&array);
	if ( array.GetSize () >0 )
	{
		_tcscpy(s_Author, strItemData.GetBuffer (0));

		// 공백제거
		Get_Keyword(s_Author, s_Author,TRUE);
	
		// 한자를 한글로 변환 
		//pTrans.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
        //pTrans.ConvertChineseTokorean(s_Author,s_Author,TRUE);
		CString strTemp ;
		strTemp.Format (_T("%s"),s_Author );
		Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
		_tcscpy(s_Author,strTemp.GetBuffer (0)); 

		if ( szBuffer.IsEmpty() )
			szBuffer = s_Author;
		else
		{
			szTemp = s_Author;
			szBuffer += szTemp;
		}
	}
	
	*fo_Author = szBuffer;

	return 0;
}

// 정수만 가져오기
INT CGetDuplicateKey::GetInteger(TCHAR *fi_Input, TCHAR* fi_Ouput) 
{
	INT i , i_Idx;

	i_Idx = 0;
	
	
	for ( i = 0; i < _tcsclen(fi_Input); i++ )
	{
		if (fi_Input[i] >= '0' && fi_Input[i] <= '9' )
		{
			fi_Ouput[i_Idx++] = fi_Input[i];
			fi_Ouput[i_Idx] = '\0';
		}
		else 
		{
			fi_Ouput[i_Idx] = '\0';
			if ( i > 0 && i_Idx > 0)
				break;
		}
	}

	return 0;
}


// 총서표제 가져오기
INT CGetDuplicateKey::SeriesTitle(TCHAR *fo_Title) 
{
	CString		szBuffer;
	TCHAR	    s_SubFieldData[1024];
	TCHAR		*pToken;
	INT			ids;

    // 440 $a 가져오기	
	
	CString temp_s_SubFieldData;
    ids = m_pMarcMgr->GetItem(&m_marc, _T("440$a"), temp_s_SubFieldData);
	if ( ids<0 )
	{
		temp_s_SubFieldData.Empty ();
	}
	
	_tcscpy(s_SubFieldData,temp_s_SubFieldData.GetBuffer (0));    
	if ( _tcsclen(s_SubFieldData) <= 0 )
	{
		// 490 $a 가져오기	
		
		CString temp_s_SubFieldData;
        ids = m_pMarcMgr->GetItem(&m_marc, _T("490$a"), temp_s_SubFieldData);
     	if ( ids<0 )
		{
			//AfxMessageBox(_T("GetItem(490$a) Error!!!"));
			//return -1;
			temp_s_SubFieldData.Empty ();
		}
		
		_tcscpy(s_SubFieldData,temp_s_SubFieldData.GetBuffer (0));
	
	}

	// 총서 표제가 존재하지 않으면
	if ( _tcsclen(s_SubFieldData) <= 0 )
	{
	    _tcscpy(fo_Title, _T(""));
		return 0;
	}

	//----------------------------------------------//
	// : 나 ; 의 뒷부분 자료 제거하기..
	//----------------------------------------------//
	szBuffer = s_SubFieldData;

	if ( szBuffer.Find(_T(";")) != -1 )
	{
		pToken = _tcstok(s_SubFieldData, _T(";"));
		if ( pToken != NULL ) {
			_tcscpy(s_SubFieldData, pToken);
			szBuffer = s_SubFieldData;
		}
	}
	
	if ( szBuffer.Find(_T(":")) != -1 )
	{
		pToken = _tcstok(s_SubFieldData, _T(":"));
		if ( pToken != NULL ) {
			_tcscpy(s_SubFieldData, pToken);	
			szBuffer = s_SubFieldData;
		}
	}
    
	//----------------------------------------------//
	// ()안의 내용 제거하기
	//----------------------------------------------//
	if ( szBuffer.Find(_T("(")) != -1 && szBuffer.Find(_T(")")) != -1  )
	{
		szBuffer.TrimLeft();  _tcscpy(s_SubFieldData, szBuffer.GetBuffer(0));
		if ( s_SubFieldData[0] == '(' )
		{
			pToken = _tcstok(s_SubFieldData, _T(")"));
			if ( pToken != NULL ) {
				pToken = _tcstok(NULL, _T(")"));
				_tcscpy(s_SubFieldData, pToken);	
			}
		}
		else
		{
			pToken = _tcstok(s_SubFieldData, _T("("));
			if ( pToken != NULL ) {
			    _tcscpy(s_SubFieldData, pToken);	
			}
		}		
	}
    
	// 공백제거
	Get_Keyword(s_SubFieldData, s_SubFieldData,TRUE);
	
	if ( _tcsclen(s_SubFieldData) >= 20 )
		// 10글자로 제한
		s_SubFieldData[20] = '\0';

	// 한자를 한글로 변환 
    //pTrans.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
    //pTrans.ConvertChineseTokorean(s_SubFieldData,s_SubFieldData,TRUE);
    CString strTemp ;
	strTemp.Format (_T("%s"),s_SubFieldData );
	Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
	_tcscpy(fo_Title,strTemp.GetBuffer (0));


	//_tcscpy(fo_Title, s_SubFieldData);

	return 0;
}


// 관칭, 부서명, 잡제 가져오기
INT CGetDuplicateKey::GetDetailCompare(CString *fo_Prefix, CString *fo_SubTitle, CString *fo_EtcTitle) 
{
	CString		szBuffer, szTemp;
//	TCHAR		s_SubFieldCode[8];
	TCHAR		**s_SubFieldData;
	TCHAR	    *sPrefix, *sSubTitle, *sEtcTitle;
	TCHAR		*pToken, *sTemp;
	INT			ids;


	sPrefix = sSubTitle = sEtcTitle = NULL;
	s_SubFieldData = NULL;
	szBuffer = _T("");
	*fo_Prefix = *fo_SubTitle = *fo_EtcTitle = _T("");


	// 관칭 가져오기. 245$ a에서 
 
	CString strItemData;
    CArray<CString, CString&> array;

    ids = m_pMarcMgr->GetItem(&m_marc, _T("245$a"), strItemData,&array);
	if(array.GetSize ()>0)
	{
		szBuffer = strItemData.GetBuffer (0);
		szBuffer.TrimLeft();
      
		sPrefix = (TCHAR*)malloc(sizeof(TCHAR)*(strItemData.GetLength ()+8) );
		sTemp = (TCHAR*)malloc(sizeof(TCHAR)*(strItemData.GetLength ()+8) );
	    _tcscpy(sPrefix, szBuffer.GetBuffer(0));

		// 관칭이 존재할 경우 
		if ( sPrefix[0] == '(' )
		{
			pToken = _tcstok(sPrefix, _T(")"));

			if ( pToken != NULL )
			{
				_tcscpy(sTemp, &pToken[1]); 
				_tcscpy(sPrefix, sTemp);
				// 공백제거
				Get_Keyword(sPrefix, sPrefix,TRUE);
				// 한자를 한글로 변환 
				//pTrans.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
                //pTrans.ConvertChineseTokorean(sPrefix,sPrefix,TRUE);
				CString strTemp ;
				strTemp.Format (_T("%s"),sPrefix );
				Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
				_tcscpy(sPrefix,strTemp.GetBuffer (0));
			}			
		}
        else 
        {
            free(sPrefix);
            sPrefix = NULL;
        }
		free(sTemp);
	}

	// 245 $b 부표제 가져오기
    ids = m_pMarcMgr->GetItem(&m_marc, _T("245$b"), strItemData,&array);	
	if ( array.GetSize () > 0 )
	{    
		
		sSubTitle = (TCHAR*)malloc(sizeof(TCHAR)*(strItemData.GetLength ()+(sizeof(TCHAR)*8)));
		_tcscpy(sSubTitle, strItemData.GetBuffer (0));
		// 공백제거
		Get_Keyword(sSubTitle, sSubTitle,TRUE);
		// 한자를 한글로 변환 
		
		//pTrans.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
        //pTrans.ConvertChineseTokorean(sSubTitle,sSubTitle,TRUE);
		CString strTemp ;
		strTemp.Format (_T("%s"),sSubTitle );
		Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
		_tcscpy(sSubTitle,strTemp.GetBuffer (0));

	}


	// 245 $c 잡표제 가져오기
    ids = m_pMarcMgr->GetItem(&m_marc, _T("245$c"),strItemData,&array);
	if ( array.GetSize () > 0 )
	{   
		//===============================================================
        //2004-02-16 이학중 
        //_tcscpy(s_SubFieldData[0],strItemData.GetBuffer (0));
		//===============================================================
		sEtcTitle = (TCHAR*)malloc(sizeof(TCHAR)*(strItemData.GetLength ()+(sizeof(TCHAR)*8)));
		_tcscpy(sEtcTitle,strItemData.GetBuffer(0));
		// 공백제거
		Get_Keyword(sEtcTitle, sEtcTitle,TRUE);
		// 한자를 한글로 변환 
		
		//pTrans.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
        //pTrans.ConvertChineseTokorean(sEtcTitle,sEtcTitle,TRUE);
		CString strTemp ;
		strTemp.Format (_T("%s"),sEtcTitle );
		Trans->GetIndex ()->ConvertHanjaToHangul (strTemp);
		_tcscpy(sEtcTitle,strTemp.GetBuffer (0));	
	}

	if ( sPrefix != NULL )
	{
		*fo_Prefix = sPrefix;
		free(sPrefix);
	}

	if ( sSubTitle != NULL )
	{
		*fo_SubTitle = sSubTitle;
		free(sSubTitle);
	}

	if ( sEtcTitle != NULL )
	{
		*fo_EtcTitle = sEtcTitle;
		free(sEtcTitle);
	}

	return 0;
}



VOID CGetDuplicateKey::Get_Keyword(TCHAR *data, TCHAR *keyword , BOOL f_KeywordFlag )
{	
	Trans->GetIndex ()->MakeIndex(data, keyword );	
	CString strKeyword;
	strKeyword.Format(_T("%s"), keyword );
	strKeyword.Replace(_T(" "), _T(""));
	_tcscpy(keyword, strKeyword.GetBuffer(0));
			
	
	return;

	/*    
    TCHAR     s_tmpbuf[1024];
    _TUCHAR ch ;
    TCHAR *ptr,*s_pStrptr ;
    INT      s_Strlen;

    _tcscpy( s_tmpbuf , data );
    if( f_KeywordFlag == FALSE ){
        ptr = _tcschr( data , '.' );
        if( ptr != NULL ){
            s_pStrptr = _tcschr( data , '.' );
            if( s_pStrptr != NULL ){
                s_Strlen = _tcsclen(data) - _tcsclen(s_pStrptr);
                _tcsnccpy( s_tmpbuf , data , s_Strlen );
                s_tmpbuf[s_Strlen] = '\0';
            }
            str_trimall( &ptr[1], &ptr[1] , '.' );
            _stprintf( &s_tmpbuf[_tcsclen(s_tmpbuf)] , _T(".%s"), &ptr[1] );
        }
        _tcscpy( data , s_tmpbuf );

        str_trimall( data, keyword, ' ' );
        str_trimall( keyword, keyword, '-' );
        return;
    }

    ptr = keyword ;

    for (; ch = *data; data++)
    {
		if( (_TUCHAR) ch & 0x80 )
		{
		    *keyword++ = ch;
			*keyword++ = *(++data);

		}else if ((ch >= 'A' && ch <= 'Z') ||      // 대문자 영문 
			      (ch >= 'a' && ch <= 'z') ||      // 소문자 영문
			      (ch >= '0' && ch <= '9') ){
			if ( ch >= 'a' && ch <= 'z' )
				ch -= 32;
    		*keyword++ = ch;
		}
    }
    *keyword = '\0';
    DelSpecialChar( ptr, ptr , f_KeywordFlag );
	*/
    
}


VOID  CGetDuplicateKey::str_trimall(TCHAR   *strln , TCHAR   *strline1, TCHAR    var )
{  
  	INT     i = 0, j = 0;

    for ( i = 0; strln[i] != '\0' && strln[i] != '\n'; i++ ) {
        if ( strln[i] == var );
        else { strline1[j] = strln[i]; j++; }
    }
    strline1[j] ='\0';
 

}


/*================================================================================
  IN  : TCHAR      * f_pInStr
  OUT : TCHAR      * f_pOutStr
================================================================================*/

VOID CGetDuplicateKey::DelSpecialChar( TCHAR *f_pRawStr , TCHAR *f_pWorkStr , BOOL f_Flag )
{
   

    INT           i,j,idx; 
    TCHAR          s_tmpbeffer[10000];
    _TUCHAR ch1,ch2;
    // KOL.UDF.022 시큐어코딩 보완
	CString sComment = _T("");

    
    idx = 0;
    if( f_pRawStr == NULL ) 
	{
		f_pWorkStr = NULL  ;
		return;
	}

    _tcscpy( s_tmpbeffer , f_pRawStr );
	f_pWorkStr[0] = '\0';

	idx = 0;

    for( i = 0; i < (INT)_tcsclen(s_tmpbeffer); i++ ){
        // 1. 2byte문자인 경우는 2byte를 복사.
        if( (_TUCHAR)s_tmpbeffer[i] >= 0x80 ){
            
            if( i == (INT)_tcsclen(s_tmpbeffer) - 1 ){
                f_pWorkStr[idx] = '\0';
                break;
            }

            ch1 = (_TUCHAR)s_tmpbeffer[i];
            ch2 = (_TUCHAR)s_tmpbeffer[i+1];
            
            // KOL.UDF.022 시큐어코딩 보완
            //for( j = 0; j <= SPECIAL_CHAR_COUNT ;j++) {
			for( j = 0; j < SPECIAL_CHAR_COUNT ;j++) {
                // KOL.UDF.022
				sComment.Format(_T("%s"),s_DelCharInfo[j].Comment);
                
				// KOL.UDF.022
				//if( _tcscmp( s_DelCharInfo[j].Comment , _T("그림문자"))  == 0 && f_Flag == TRUE ){ 
				if( sComment.Compare(_T("그림문자"))  == 0 && f_Flag == TRUE ){ 
                    continue;
                }

                if( ch1 >= s_DelCharInfo[j].s_FirstChar && ch2 >= s_DelCharInfo[j].s_SecondChar &&
                    ch1 <= s_DelCharInfo[j].e_FirstChar && ch2 <= s_DelCharInfo[j].e_SecondChar ){
					
					if( sComment.Compare(_T("숫자"))  == 0 ){
                        f_pWorkStr[idx++] = ch2 - 0x80;
                    }else
                    if( sComment.Compare(_T("영문대문자"))  == 0 ){
                        f_pWorkStr[idx++] = ch2 - 0x80;
                    }else
                    if( sComment.Compare(_T("영문소문자"))  == 0 ){
                        f_pWorkStr[idx++] = ch2 - 0xA0;
                    }else
						if( sComment.Compare(_T("한글확장"))  == 0 ){
                        if( ch2 <= 0x7F ){
                            if( ( ch2 >= 0x41 && ch2 <= 0x5A ) || ( ch2 >= 0x61 && ch2 <= 0x7A ) ){
                                f_pWorkStr[idx++] = ch1;
                                f_pWorkStr[idx++] = ch2;
                            }
                        }
                        else{
                            if( ( ch1 <= 0xA0 ) && ( ch2 >= 0x81 && ch2 <= 0xFE ) ){
                                f_pWorkStr[idx++] = ch1;
                                f_pWorkStr[idx++] = ch2;
                            }else
                            if( ( ch1 >= 0xA1 ) && ( ch2 >= 0x81 && ch2 <= 0xA0 ) ){
                                f_pWorkStr[idx++] = ch1;
                                f_pWorkStr[idx++] = ch2;
                            }
                        }
                    }
                    else{
                        f_pWorkStr[idx++] = ch1;
                        f_pWorkStr[idx++] = ch2;
                    }
					break ;
                }
            }
            i++;
       }
       else{
            f_pWorkStr[idx++] = s_tmpbeffer[i];
       }
    }
	f_pWorkStr[idx] = '\0';


}