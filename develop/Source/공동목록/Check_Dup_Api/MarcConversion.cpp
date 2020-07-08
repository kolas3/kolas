// MarcConversion.cpp: implementation of the CMarcConversion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\include\공동목록\MarcConversion.h"
#include "..\..\include\공동목록\PlusIN049Tag.h"
//#include _T("Eco_FileIO.h")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcConversion::CMarcConversion()
{
	
}

CMarcConversion::~CMarcConversion()
{
	
}

/*=========================================================================
단위도서관 서지 MARC를 국가자료공동목록 서지MASTERMARC로 변환.
IN  : f_pMarcClass( Marc구분값('M': 단행,'N':비도서,'S':연속간행자료)
f_pUBNum( UB번호 )
f_pLibCode( 도서관부호)
f_pLocalMarcData ( LocalMarc )
OUT : f_pISBNNo( 낱권ISBN )
f_pISBNNoCnt( 낱권ISBN 갯수)
f_pSET_ISBNNo( SET ISBN )
f_pVolInfo( 505 FieldData )
f_pSujiMasterMarcData( 서지MASTERMARC )
=========================================================================*/
INT  CMarcConversion::LocalMarcToSujiMasterMarc( TCHAR    f_pMarcClass    ,
												CString   f_pUBNum         ,
												CString   f_pLibCode       ,
												CString   f_pLocalMarcData ,
												CString   **f_pISBNNo      ,
												INT       *f_pISBNNoCnt       ,
												
												CString   &f_pVolInfo      ,
												CString   &f_pSujiMasterMarcData )
{
    INT ids;
	
    switch( f_pMarcClass ){
    case 'M':
        ids = MonoLocalMarcToSujiMasterMarc(f_pUBNum         ,
			f_pLibCode       ,
			f_pLocalMarcData ,
			f_pISBNNo     ,
			f_pISBNNoCnt     ,
			f_pVolInfo       ,
			f_pSujiMasterMarcData );
        if( ids ){
            return -100;
        }
        break;
    case 'N':
        ids = NonBookLocalMarcToSujiMasterMarc(  f_pUBNum              ,
			f_pLibCode            ,
			f_pLocalMarcData      ,
			f_pISBNNo             ,
			f_pISBNNoCnt          ,
			f_pVolInfo            ,
			f_pSujiMasterMarcData ); 
        if( ids ){
            return -100;
        }
        break;
		
    case 'S':
        ids = SerialLocalMarcToSujiMasterMarc(   f_pUBNum              ,
			f_pLibCode            ,
			f_pLocalMarcData      ,
			f_pISBNNo             ,
			f_pISBNNoCnt          ,
			f_pVolInfo            ,
			f_pSujiMasterMarcData ); 
        if( ids ){
            return -100;
        }
        break;
    default:
        AfxMessageBox(_T("MARC 구분값이 올바르지 않습니다."));
        return -1;
    }
    return 0;
}
/*=========================================================================
단위도서관 서지 MARC를 국가자료공동목록 서지MASTERMARC로 변환.(단행자료)
IN  : 
f_pUBNum( UB번호 )
f_pLibCode( 도서관부호)
f_pLocalMarcData ( LocalMarc )
OUT : f_pISBNNo( 낱권ISBN )
f_pISBNNoCnt( 낱권ISBN 갯수)
f_pSET_ISBNNo( SET ISBN )
f_pVolInfo( 505 FieldData )
f_pSujiMasterMarcData( 서지MASTERMARC )
=========================================================================*/
INT CMarcConversion::MonoLocalMarcToSujiMasterMarc( CString   f_pUBNum         ,
												   CString   f_pLibCode       ,
												   CString   f_pLocalMarcData ,
												   CString   **f_pISBNNo      ,
												   INT       *f_pISBNNoCnt     ,
												   CString   &f_pVolInfo       ,
												   CString   &f_pSujiMasterMarcData )
												   
{
    CMarc  s_RawMarcStru,s_ConvertMarcStru;
//    TCHAR          s_FieldData[10000];    
    INT         i,ids,s_505FieldLength;
	
	
	TCHAR s_008FieldData[128];
	s_008FieldData[0] = '\0';
	CString temp_s_008FieldData;
	
	
	f_pVolInfo = _T("");
    *f_pISBNNoCnt = s_505FieldLength = 0;
	
	
	// 1. StreamMarc를 Marc구조체로 변경한다.
	ids = m_pMarcMgr->Decoding(f_pLocalMarcData, &s_RawMarcStru);
	if( ids ){
        return -1;
    }
	
    //s_RawMarcStru.m_listTag.GetCount ()
    // 2.변환규칙에 따라서 단위도서관 서지MARCDATA를 국가자료 공동목록서지MASTER MARC로 변환한다.
	// KOL.UDF.022 시큐어코딩 보완
	INT nTagNo = 0;
//  POSITION pos = s_RawMarcStru.m_listTag.GetTailPosition () ;
	POSITION pos = s_RawMarcStru.m_listTag.GetHeadPosition() ;
    while( pos != NULL ) 
	{   
		CString tmp_Value;
		CString strField ;
		
		//POSITION posOld = pos ;

		//CTag *pTag = (CTag*)s_RawMarcStru.m_listTag.GetPrev (pos) ;
	    CTag *pTag = (CTag*)s_RawMarcStru.m_listTag.GetNext(pos) ;			
		if (pTag == NULL) continue;
        
		nTagNo = _ttoi(pTag->GetTagCode());
		
		switch(nTagNo)
        {
			
        case   1 :
		case   5 :
        //case  20 :
        case  35 :        
        case  49 :
        case  90 :
        case 850 :
        case 900 :
        case 910 :
        case 911 :
        case 930 :
        case 940 :
        case 949 :            
			break;
			
		case 505 :
			
			ids = m_pMarcMgr->GetField  (&s_RawMarcStru,_T("505"),tmp_Value);
			if( ids < 0 ){ return -243 ; }
			s_505FieldLength = tmp_Value.GetLength ();
                  break ;
		case   8 :
			ids = m_pMarcMgr->GetItem (&s_RawMarcStru,_T("008*"),temp_s_008FieldData);
			if( ids < 0 ){ return -243 ; }
			_tcscpy(s_008FieldData,temp_s_008FieldData);			    
			break ;
			
			
			// 권차($n),권차서명($p)를 삭제하는 TAG
        case 110 :
        case 111 :
        case 130 :
        case 240 :
        case 700 :
        case 710 :
        case 711 :
        case 730 :
        case 740 :
			ids = m_pMarcMgr->GetField (&s_RawMarcStru ,pTag->GetTagCode (),strField);
			if( ids < 0 ){ return -243 ; }
			ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode()+strField);
			if( ids < 0 ){ return -243 ; }
			
			// 권차($n)를 삭제.
			ids = m_pMarcMgr->DeleteItem (&s_ConvertMarcStru,pTag->GetTagCode ()+_T("$n"),_T(""));			
			// 권차서명($p)를 삭제.	
			ids = m_pMarcMgr->DeleteItem  (&s_ConvertMarcStru, pTag->GetTagCode() + _T("$p"),_T(""));
			break;
			
        case 765 :
        case 767 :
        case 770 :
        case 772 :
        case 773 :
        case 775 :
        case 776 :
        case 780 :
        case 785 :
        case 787 :
			
            // 2.3.2.제어번호($w)를 삭제.
			
			ids = m_pMarcMgr->GetField(&s_RawMarcStru, pTag->GetTagCode(), strField); 
			if( ids < 0 ){ return -243 ; }
			ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + strField);
			if( ids < 0 ){ return -243 ; }
			
			
			ids = m_pMarcMgr->DeleteItem(&s_ConvertMarcStru, pTag->GetTagCode() + _T("$w") , _T(""));
			
			break;
			
			// 2.4.그대로 이동하는 TAG
        default:
			
            // 2.4.1.제어필드
			if( nTagNo < 10 )
			{
				
				// 2.4.3.필드를 새로운 MARC구조체에 생성한다.	
				ids = m_pMarcMgr->GetItem (&s_RawMarcStru,pTag->GetTagCode()+_T("*"), strField) ;
				if( ids < 0 ){ return -243 ; }
				ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,pTag->GetTagCode()+_T("*"), strField);
                if(ids <0) {return -243 ; }
		
       
            }
			
            // 2.4.2.제어필드이외의 TAG
            else
			{
				// 2.4.3.필드를 새로운 MARC구조체에 생성한다.
				// KOL.UDF.022 시큐어코딩 보완
				INT oldTag = 0;
                
                if(nTagNo == oldTag)
				{
					break ;
				}

				oldTag = nTagNo;
				
				CArray<CString, CString&> array;
				ids = m_pMarcMgr->GetField(&s_RawMarcStru, pTag->GetTagCode(), strField,&array); 
    			if( ids < 0 ){ return -243 ; }
                
				if(array.GetSize () >1)
                {    
                    for(INT i =0 ; i<array.GetSize ();i++) 
					{
						ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + array.GetAt (i));
					    if( ids < 0 ){ return -20030123 ; }
					}
					array.RemoveAll () ;
                }
				else 
				{
					ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + strField);
					if( ids < 0 ){ return -243 ; }
				}
                           

			}
         } //switch
		 
          
	
		 /*========================================================================================*/
     }//while
     
	 /*========================================================================================*/ 
	 // 리더 생성 
	 /*========================================================================================*/
	 TCHAR temp_leader[25] ;
	 temp_leader[24] = '\0';
	 s_RawMarcStru.GetLeader(temp_leader);
	 
	 TCHAR con_leader[25]={_T(" ")} ;
	 _tcscpy(con_leader,temp_leader) ;
	 //con_leader[24] = '\0' ;
	 
	 if(  temp_leader[5] == 'a' || temp_leader[5] == 'c' || temp_leader[5] == 'p' ){	
		 //	s_ConvertMarcStru.MarcLeader.RecStatus = 'n';			
		 con_leader[5] = 'n' ;
		 s_ConvertMarcStru.SetLeader (con_leader);			
	 }
	 else{
		 s_RawMarcStru.GetLeader (temp_leader);
		 s_ConvertMarcStru.SetLeader (con_leader);
	 }				
	 s_RawMarcStru.GetLeader (temp_leader);
	 
	 // 2.5.2.레코드 형태( 시작위치: 6 ,길이 : 1 )
	 con_leader[6] = temp_leader[6];
	 
	 // 2.5.3.서지수준( 시작위치: 7 ,길이 : 1 )
	 con_leader[7] = temp_leader[7];
	 
	 // 2.5.4.입력수준( 시작위치: 17 ,길이 : 1 )
	 con_leader[17] = temp_leader[17];
	 
	 // 2.5.5.목록기술형식( 시작위치: 18 ,길이 : 1 )
	 con_leader[18] = temp_leader[18];
	 
	 // 2.5.6.연관레코드 조건( 시작위치: 19 ,길이 : 1 )
	 con_leader[19] = temp_leader[19];
	 
	 s_ConvertMarcStru.SetLeader (con_leader);
	 
	 
	 /*========================================================================================*/
	 // 001 Tag 생성 
	 /*========================================================================================*/
	 
	 ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("001*"),f_pUBNum);
	 if( ids < 0 ){                
		 return -261;
	 }
	 //005 Tag 생성
	 
	 TCHAR  s_005FieldData[128] ;
	 s_005FieldData[0]='\0' ;
	 CString temp_s_005FieldData ;
	 
	 GetSystemDate(s_005FieldData);
	 GetSystemTime(&s_005FieldData[_tcsclen(s_005FieldData)]);
	 
	 temp_s_005FieldData.Format (_T("%s"),s_005FieldData);	
	 ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("005*"),temp_s_005FieldData);
	 if( ids ){	
		 return -291;
	 }    
	 
	 
	 
	 //008 Tag 생성
	 
	 if( s_008FieldData[0] == '\0' ){
		 wmemset( s_008FieldData , ' ' , 40 );
		 s_008FieldData[40] = '\0';
	 }
	 _tcsnccpy( s_008FieldData , &s_005FieldData[2] , 6 );
	 
	 temp_s_008FieldData.Format (_T("%s"),s_008FieldData);
	 
	 ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("008*"),temp_s_008FieldData);
	 if( ids ){
		 return -291;
	 }
	 
	 // ISBN은 현재의 MARC DATA에서 ISBN번호만을 가지고 중복조사를 한후 중복이 아닌 경우에만 생성.
	 // ISBN을 FIELD DATA를 가져온다.
	 
	 CArray<CString, CString&> array;
	 CString strItemData;
	 
	 array.RemoveAll () ;
     
     //  ids = m_pMarcMgr->GetField (&s_ConvertMarcStru,_T("020"),strItemData,&array);
     
    

	 //ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData,&array);
	 ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData );
	 if ( strItemData == _T("") ) {
		*f_pISBNNoCnt = 0 ;
        f_pISBNNo = NULL ;
	 }
	 else {
		 ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData,&array);
		 *f_pISBNNoCnt = array.GetSize () ;
		f_pISBNNo[0] = new CString[array.GetSize()] ;
        
		for ( i = 0; i < array.GetSize (); i++)
		{
		 f_pISBNNo[0][i] = array.GetAt(i);
		}
	 }
/*

	 if (ids<0) {   
	    *f_pISBNNoCnt = 0 ;
        f_pISBNNo = NULL ;
	 
	 }
	 
     else {
        
		*f_pISBNNoCnt = array.GetSize () ;
		f_pISBNNo[0] = new CString[array.GetSize()] ;
        
		for ( i = 0; i < array.GetSize (); i++)
		{
		 f_pISBNNo[0][i] = array.GetAt(i);
		}
	 }
*/	 
     
	 
	 //012 Tag(국립중앙도서관 제어번호)
	 CString tmp_Data ;
	 tmp_Data.Empty ();
	 if( _tcscmp( f_pLibCode ,_T("011001")) == 0 ) {
		 ids = m_pMarcMgr->GetItem (&s_RawMarcStru,_T("001*"),tmp_Data );
		 
		 ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("012$a"),tmp_Data);
		 
	 }
	 else 
	 {   
		 ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("012"),tmp_Data);
		 if(! tmp_Data.IsEmpty ())
		 {
		 ids = m_pMarcMgr->InsertField (&s_ConvertMarcStru,_T("012")+tmp_Data);
		 }

	}
	 
	 
     /*========================================================================================*/
	 // 035 TAG 생성.( 중앙도서관의 경우 012 TAG생성 )
	 // 중앙도서관 또는 중앙도서관분관이 아닌 경우는 035 TAG에 입력
	 
	    CString tmp_f_pUBNum ;
		tmp_f_pUBNum.Empty ();
		ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("001*"),tmp_f_pUBNum);
        
        if( !tmp_f_pUBNum.IsEmpty ())
		{
			tmp_Data.Format (_T("(%s)%s"),f_pLibCode ,tmp_f_pUBNum);
			ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("035$a"),tmp_Data);
			if( ids < 0 ){                
				return -281;
			}
		}

		else 
		{   
			tmp_Data.Format (_T("(%s)"),f_pLibCode);
			ids = m_pMarcMgr->SetItem(&s_ConvertMarcStru,_T("035$a"),tmp_Data); 
		}


		
		
		// 2.9.040이 없는 경우 도서관부호를 생성.
		CString temp ;
		ids = m_pMarcMgr->GetField (&s_ConvertMarcStru ,_T("040"),temp);
		
		if( ids<0 )
		{			
			ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("040$a"),f_pLibCode);		
			if( ids < 0 ){			
				return -291;
			}
			ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("040$c"),f_pLibCode );
			if(ids<0){return -3000 ;}
		}
	 	
		
		// 2.10.기존의 505가 있는지를 검사하고 있는 경우 사용자가 MARCDAT를 수정할 수 있다.
		
		if( s_505FieldLength != 0 ){
			
			ids = m_pMarcMgr->GetField  (&s_RawMarcStru,_T("505"),f_pVolInfo);
			
		/*	if( ids == 0  )
			{	
				ids = m_pMarcMgr->InsertField  (&s_RawMarcStru,_T("050")+f_pVolInfo);
				if(ids) return -20000;
			}
		*/
		 }
		
	
		
		
		// 2.11.변환된 MARC구조체로 StreamMarc형태의 MarcData를 만든다.
		//ids = WriteMarcStruToStreamMarc( s_ConvertMarcStru , f_pSujiMasterMarcData );
		
		ids = m_pMarcMgr->Encoding (&s_ConvertMarcStru,f_pSujiMasterMarcData);
		if( ids ){
			return -211;
		}
		
       

		return 0;
}


/*=========================================================================
단위도서관 서지 MARC를 국가자료공동목록 서지MASTERMARC로 변환.(비도서자료)
IN  : 
f_pUBNum( UB번호 )
f_pLibCode( 도서관부호)
f_pLocalMarcData ( LocalMarc )
OUT : f_pISBNNo( 낱권ISBN )
f_pISBNNoCnt( 낱권ISBN 갯수)
f_pSET_ISBNNo( SET ISBN )
f_pVolInfo( 505 FieldData )
f_pSujiMasterMarcData( 서지MASTERMARC )
=========================================================================*/
INT CMarcConversion::NonBookLocalMarcToSujiMasterMarc(  CString   f_pUBNum ,
													  CString   f_pLibCode       ,
													  CString   f_pLocalMarcData ,
													  CString   **f_pISBNNo         ,
													  INT       *f_pISBNNoCnt     ,
													  //TCHAR  **f_pSET_ISBNNo    ,
													  CString   &f_pVolInfo       ,
													  CString   &f_pSujiMasterMarcData )
													  
{
    CMarc  s_RawMarcStru,s_ConvertMarcStru;
//    TCHAR          s_FieldData[10000];    
    INT         i,ids,s_505FieldLength;
	
	
	TCHAR s_008FieldData[128];
	s_008FieldData[0] = '\0';
	CString temp_s_008FieldData;


     f_pVolInfo = _T("");
    *f_pISBNNoCnt = s_505FieldLength = 0;
	

	// 1. StreamMarc를 Marc구조체로 변경한다.
	ids = m_pMarcMgr->Decoding(f_pLocalMarcData, &s_RawMarcStru);
	if( ids ){
        return -1;
    }
		
    //s_RawMarcStru.m_listTag.GetCount ()
    // 2.변환규칙에 따라서 단위도서관 서지MARCDATA를 국가자료 공동목록서지MASTER MARC로 변환한다.
	// KOL.UDF.022 시큐어코딩 보완
	INT nTagNo = 0;
    POSITION pos = s_RawMarcStru.m_listTag.GetHeadPosition () ;
    while( pos != NULL) 
	{   
		CString tmp_Value;
		CString strField ;
		CTag *pTag = (CTag*)s_RawMarcStru.m_listTag.GetNext (pos) ;
		if (pTag == NULL) continue;
        
		nTagNo = _ttoi(pTag->GetTagCode());
				
		switch(nTagNo)
        {
		
        case   1 :
		case   5 :
        case  20 :
        case  35 :        
        case  49 :
        case  90 :
        case 850 :
        case 900 :
        case 910 :
        case 911 :
        case 930 :
        case 940 :
        case 949 :            
                  break;
		
		case 505 :
		          ids = m_pMarcMgr->GetField  (&s_RawMarcStru,_T("505"),tmp_Value);
				  s_505FieldLength = tmp_Value.GetLength ();
                  break ;
		case   8 :
			      ids = m_pMarcMgr->GetItem (&s_RawMarcStru,_T("008*"),temp_s_008FieldData);
				  _tcscpy(s_008FieldData,temp_s_008FieldData);			    
			      break ;


			// 권차($n),권차서명($p)를 삭제하는 TAG
        case 110 :
        case 111 :
        case 130 :
        case 240 :
        case 700 :
        case 710 :
        case 711 :
        case 730 :
        case 740 :
				ids = m_pMarcMgr->GetField (&s_RawMarcStru ,pTag->GetTagCode (),strField);
				ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode()+strField);
				if( ids < 0 ){ return -243 ; }
				
				// 권차($n)를 삭제.
				ids = m_pMarcMgr->DeleteItem (&s_ConvertMarcStru,pTag->GetTagCode ()+_T("$n"),_T(""));
				
    
				// 권차서명($p)를 삭제.	
				ids = m_pMarcMgr->DeleteItem  (&s_ConvertMarcStru, pTag->GetTagCode() + _T("$p"),_T(""));
			
				break;
		
        case 765 :
        case 767 :
        case 770 :
        case 772 :
        case 773 :
        case 775 :
        case 776 :
        case 780 :
        case 785 :
        case 787 :
					
            // 2.3.2.제어번호($w)를 삭제.
			
				ids = m_pMarcMgr->GetField(&s_RawMarcStru, pTag->GetTagCode(), strField); 
				ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + strField);
				if( ids < 0 ){ return -243 ; }
				
				
				ids = m_pMarcMgr->DeleteItem(&s_ConvertMarcStru, pTag->GetTagCode() + _T("$w") , _T(""));
			
				
				break;
			
			// 2.4.그대로 이동하는 TAG
        default:
				
            // 2.4.1.제어필드
			if( nTagNo < 10 )
			{
            
				// 2.4.3.필드를 새로운 MARC구조체에 생성한다.	
				ids = m_pMarcMgr->GetItem (&s_RawMarcStru,pTag->GetTagCode()+_T("*"), strField) ;
				ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,pTag->GetTagCode()+_T("*"), strField);
                if(ids <0) {return -243 ; }
				
            }
			
            // 2.4.2.제어필드이외의 TAG
            else
			{
				// 2.4.3.필드를 새로운 MARC구조체에 생성한다.
				// KOL.UDF.022 시큐어코딩 보완
				INT oldTag = 0;
                
                if(nTagNo == oldTag)
				{
					break ;
				}

				oldTag = nTagNo;
				
				CArray<CString, CString&> array;
				ids = m_pMarcMgr->GetField(&s_RawMarcStru, pTag->GetTagCode(), strField,&array); 
    			if( ids < 0 ){ return -243 ; }
                
				if(array.GetSize () >1)
                {    
                    for(INT i =0 ; i<array.GetSize ();i++) 
					{
						ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + array.GetAt (i));
					    if( ids < 0 ){ return -20030123 ; }
					}
					array.RemoveAll () ;
                }
				else 
				{
					ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + strField);
					if( ids < 0 ){ return -243 ; }
				}
                           

			}
			
            break;
         }
		
	  
	  /*========================================================================================*/
     }
     
    /*========================================================================================*/ 
    // 리더 생성 
    /*========================================================================================*/
		TCHAR temp_leader[25] ;
		temp_leader[24] = '\0';
		s_RawMarcStru.GetLeader(temp_leader);
		
		TCHAR con_leader[25]={_T(" ")} ;
		_tcscpy(con_leader,temp_leader) ;
		//con_leader[24] = '\0' ;
		
		if(  temp_leader[5] == 'a' || temp_leader[5] == 'c' || temp_leader[5] == 'p' ){	
			//	s_ConvertMarcStru.MarcLeader.RecStatus = 'n';			
			con_leader[5] = 'n' ;
			s_ConvertMarcStru.SetLeader (con_leader);			
		}
		else{
			s_RawMarcStru.GetLeader (temp_leader);
			s_ConvertMarcStru.SetLeader (con_leader);
		}				
		s_RawMarcStru.GetLeader (temp_leader);
		
		// 2.5.2.레코드 형태( 시작위치: 6 ,길이 : 1 )
		con_leader[6] = temp_leader[6];
		
		// 2.5.3.서지수준( 시작위치: 7 ,길이 : 1 )
		con_leader[7] = temp_leader[7];
		
		// 2.5.4.입력수준( 시작위치: 17 ,길이 : 1 )
		con_leader[17] = temp_leader[17];
		
		// 2.5.5.목록기술형식( 시작위치: 18 ,길이 : 1 )
		con_leader[18] = temp_leader[18];
		
		// 2.5.6.연관레코드 조건( 시작위치: 19 ,길이 : 1 )
		con_leader[19] = temp_leader[19];
		
		s_ConvertMarcStru.SetLeader (con_leader);


    /*========================================================================================*/
	// 001 Tag 생성 
    /*========================================================================================*/

    ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("001*"),f_pUBNum);
    if( ids < 0 ){                
				return -261;
			}
    //005 Tag 생성
	
	TCHAR  s_005FieldData[128] ;
	s_005FieldData[0]='\0' ;
	CString temp_s_005FieldData ;
  
	GetSystemDate(s_005FieldData);
	GetSystemTime(&s_005FieldData[_tcsclen(s_005FieldData)]);

	temp_s_005FieldData.Format (_T("%s"),s_005FieldData);	
	ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("005*"),temp_s_005FieldData);
	if( ids ){	
	return -291;
	}    



    //008 Tag 생성

	if( s_008FieldData[0] == '\0' ){
	wmemset( s_008FieldData , ' ' , 40 );
	s_008FieldData[40] = '\0';
	}
	_tcsnccpy( s_008FieldData , &s_005FieldData[2] , 6 );

	temp_s_008FieldData.Format (_T("%s"),s_008FieldData);

	ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("008*"),temp_s_008FieldData);
	if( ids ){
	return -291;
	}

    // ISBN은 현재의 MARC DATA에서 ISBN번호만을 가지고 중복조사를 한후 중복이 아닌 경우에만 생성.
    // ISBN을 FIELD DATA를 가져온다.

	CArray<CString, CString&> array;
	CString strItemData;
    ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData );
	 if ( strItemData == _T("") ) {
		*f_pISBNNoCnt = 0 ;
        f_pISBNNo = NULL ;
	 }
	 else {
		 ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData,&array);
		 *f_pISBNNoCnt = array.GetSize () ;
	     f_pISBNNo[0] = new CString[array.GetSize()] ;
        
		for ( i = 0; i < array.GetSize (); i++)
		{
		 f_pISBNNo[0][i] = array.GetAt(i);
		}
	 }
	 
   
	 
	 //012 Tag(국립중앙도서관 제어번호)
	 CString tmp_Data ;
	 tmp_Data.Empty ();
	 if( _tcscmp( f_pLibCode ,_T("011001")) == 0 ) {
		 ids = m_pMarcMgr->GetItem (&s_RawMarcStru,_T("001*"),tmp_Data );
		 
		 ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("012$a"),tmp_Data);
		 
	 }
	 else 
	 {   
		 ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("012"),tmp_Data);
		 if(! tmp_Data.IsEmpty ())
		 {
		 ids = m_pMarcMgr->InsertField (&s_ConvertMarcStru,_T("012")+tmp_Data);
		 }

	}


     /*========================================================================================*/
	 // 035 TAG 생성.( 중앙도서관의 경우 012 TAG생성 )
	 // 중앙도서관 또는 중앙도서관분관이 아닌 경우는 035 TAG에 입력
		 
	    CString tmp_f_pUBNum ;
		tmp_f_pUBNum.Empty ();
		ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("001*"),tmp_f_pUBNum);
        
        if( !tmp_f_pUBNum.IsEmpty ())
		{
			tmp_Data.Format (_T("(%s)%s"),f_pLibCode ,tmp_f_pUBNum);
			ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("035$a"),tmp_Data);
			if( ids < 0 ){                
				return -281;
			}
		}

		else 
		{   
			tmp_Data.Format (_T("(%s)"),f_pLibCode);
			ids = m_pMarcMgr->SetItem(&s_ConvertMarcStru,_T("035$a"),tmp_Data); 
		}


		
		
		// 2.9.040이 없는 경우 도서관부호를 생성.
		CString temp ;
		ids = m_pMarcMgr->GetField (&s_ConvertMarcStru ,_T("040"),temp);
		
		if( ids<0 )
		{			
			ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("040$a"),f_pLibCode);		
			if( ids < 0 ){			
				return -291;
			}
			ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("040$c"),f_pLibCode );
			if(ids<0){return -3000 ;}
		}
	
		
		
		// 2.10.기존의 505가 있는지를 검사하고 있는 경우 사용자가 MARCDAT를 수정할 수 있다.
		
		if( s_505FieldLength != 0 ){
			
			ids = m_pMarcMgr->GetField  (&s_RawMarcStru,_T("505"),f_pVolInfo);
	
		 }

	// 2.11.변환된 MARC구조체로 StreamMarc형태의 MarcData를 만든다.
	//ids = WriteMarcStruToStreamMarc( s_ConvertMarcStru , f_pSujiMasterMarcData );

	ids = m_pMarcMgr->Encoding (&s_ConvertMarcStru,f_pSujiMasterMarcData);
	if( ids ){
		return -211;
	}
    
//	AfxMessageBox(f_pSujiMasterMarcData);

	return 0;
}

/*=========================================================================
단위도서관 서지 MARC를 국가자료공동목록 서지MASTERMARC로 변환.(연속간행자료)
IN  : 
f_pUBNum( UB번호 )
f_pLibCode( 도서관부호)
f_pLocalMarcData ( LocalMarc )
OUT : f_pISBNNo( 낱권ISBN )
f_pISBNNoCnt( 낱권ISBN 갯수)
f_pSET_ISBNNo( SET ISBN )
f_pVolInfo( 505 FieldData )
f_pSujiMasterMarcData( 서지MASTERMARC )
=========================================================================*/

INT CMarcConversion::SerialLocalMarcToSujiMasterMarc(  CString   f_pUBNum         ,
													 CString   f_pLibCode       ,
													 CString   f_pLocalMarcData ,
													 CString   **f_pISBNNo        ,
													 INT       *f_pISBNNoCnt     ,
													 //TCHAR  **f_pSET_ISBNNo    ,
													 CString   &f_pVolInfo       ,
													 CString   &f_pSujiMasterMarcData )
{
    CMarc  s_RawMarcStru,s_ConvertMarcStru;
//    TCHAR          s_FieldData[10000];    
    INT         i,ids,s_505FieldLength;
	
	
	TCHAR s_008FieldData[128];
	s_008FieldData[0] = '\0';
	CString temp_s_008FieldData;


     f_pVolInfo = _T("");
    *f_pISBNNoCnt = s_505FieldLength = 0;
	

	// 1. StreamMarc를 Marc구조체로 변경한다.
	ids = m_pMarcMgr->Decoding(f_pLocalMarcData, &s_RawMarcStru);
	if( ids ){
        return -1;
    }
		
    //s_RawMarcStru.m_listTag.GetCount ()
    // 2.변환규칙에 따라서 단위도서관 서지MARCDATA를 국가자료 공동목록서지MASTER MARC로 변환한다.
	// KOL.UDF.022 시큐어코딩 보완
	INT nTagNo = 0;
    POSITION pos = s_RawMarcStru.m_listTag.GetHeadPosition () ;
    while( pos != NULL) 
	{   
		CString tmp_Value;
		CString strField ;
		CTag *pTag = (CTag*)s_RawMarcStru.m_listTag.GetNext (pos) ;
		if (pTag == NULL) continue;
        
		nTagNo = _ttoi(pTag->GetTagCode());
				
		switch(nTagNo)
        {
		
		case   5 :
        case  20 :
        case  35 :        
        case  49 :
        case  90 :
        case 850 :
        case 900 :
        case 910 :
        case 911 :
        case 930 :
        case 940 :
        case 949 :  
		case 980 :
                  break;
		
		case 505 :
		          ids = m_pMarcMgr->GetField  (&s_RawMarcStru,_T("505"),tmp_Value);
				  s_505FieldLength = tmp_Value.GetLength ();
                  break ;
		case   8 :
			      ids = m_pMarcMgr->GetItem (&s_RawMarcStru,_T("008*"),temp_s_008FieldData);
				  _tcscpy(s_008FieldData,temp_s_008FieldData);			    
			      break ;


			// 권차($n),권차서명($p)를 삭제하는 TAG
        case 110 :
        case 111 :
        case 130 :
        case 240 :
        case 700 :
		case 210 :
        case 710 :
        case 711 :
        case 730 :
        case 740 :
		case 222 :
				ids = m_pMarcMgr->GetField (&s_RawMarcStru ,pTag->GetTagCode (),strField);
				ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode()+strField);
				if( ids < 0 ){ return -243 ; }
				
				// 권차($n)를 삭제.
				ids = m_pMarcMgr->DeleteItem (&s_ConvertMarcStru,pTag->GetTagCode ()+_T("$n"),_T(""));
			
				// 권차서명($p)를 삭제.	
				ids = m_pMarcMgr->DeleteItem  (&s_ConvertMarcStru, pTag->GetTagCode() + _T("$p"),_T(""));
			
				break;
		
        case 765 :
        case 767 :
        case 770 :
        case 772 :
        case 773 :
        case 775 :
        case 776 :
        case 780 :
        case 785 :
        case 787 :
		case 760 :
		case 762 :
		case 777 :
					
            // 2.3.2.제어번호($w)를 삭제.
			
				ids = m_pMarcMgr->GetField(&s_RawMarcStru, pTag->GetTagCode(), strField); 
				ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + strField);
				if( ids < 0 ){ return -243 ; }
				
				
				ids = m_pMarcMgr->DeleteItem(&s_ConvertMarcStru, pTag->GetTagCode() + _T("$w") , _T(""));
			
				
				break;
			
			// 2.4.그대로 이동하는 TAG
        default:
				
            // 2.4.1.제어필드
			if( nTagNo < 10 )
			{
            
				// 2.4.3.필드를 새로운 MARC구조체에 생성한다.	
				ids = m_pMarcMgr->GetItem (&s_RawMarcStru,pTag->GetTagCode()+_T("*"), strField) ;
				ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,pTag->GetTagCode()+_T("*"), strField);
                if(ids <0) {return -243 ; }
				
            }
			
            // 2.4.2.제어필드이외의 TAG
            else
			{
				// 2.4.3.필드를 새로운 MARC구조체에 생성한다.
				// KOL.UDF.022 시큐어코딩 보완
				INT oldTag = 0;
                
                if(nTagNo == oldTag)
				{
					break ;
				}

				oldTag = nTagNo;
				
				CArray<CString, CString&> array;
				ids = m_pMarcMgr->GetField(&s_RawMarcStru, pTag->GetTagCode(), strField,&array); 
    			if( ids < 0 ){ return -243 ; }
                
				if(array.GetSize () >1)
                {    
                    for(INT i =0 ; i<array.GetSize ();i++) 
					{
						ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + array.GetAt (i));
					    if( ids < 0 ){ return -20030123 ; }
					}
					array.RemoveAll () ;
                }
				else 
				{
					ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + strField);
					if( ids < 0 ){ return -243 ; }
				}
                           

			}
			
            break;
         }
	  
	  /*========================================================================================*/
     }
     
    /*========================================================================================*/ 
    // 리더 생성 
    /*========================================================================================*/
		TCHAR temp_leader[25] ;
		temp_leader[24] = '\0';
		s_RawMarcStru.GetLeader(temp_leader);
		
		TCHAR con_leader[25]={_T(" ")} ;
		_tcscpy(con_leader,temp_leader) ;
		//con_leader[24] = '\0' ;
		
		if(  temp_leader[5] == 'a' || temp_leader[5] == 'c' || temp_leader[5] == 'p' ){	
			//	s_ConvertMarcStru.MarcLeader.RecStatus = 'n';			
			con_leader[5] = 'n' ;
			s_ConvertMarcStru.SetLeader (con_leader);			
		}
		else{
			s_RawMarcStru.GetLeader (temp_leader);
			s_ConvertMarcStru.SetLeader (con_leader);
		}				
		s_RawMarcStru.GetLeader (temp_leader);
		
		// 2.5.2.레코드 형태( 시작위치: 6 ,길이 : 1 )
		con_leader[6] = temp_leader[6];
		
		// 2.5.3.서지수준( 시작위치: 7 ,길이 : 1 )
		con_leader[7] = temp_leader[7];
		
		// 2.5.4.입력수준( 시작위치: 17 ,길이 : 1 )
		con_leader[17] = temp_leader[17];
		
		// 2.5.5.목록기술형식( 시작위치: 18 ,길이 : 1 )
		con_leader[18] = temp_leader[18];
		
		// 2.5.6.연관레코드 조건( 시작위치: 19 ,길이 : 1 )
		con_leader[19] = temp_leader[19];
		
		s_ConvertMarcStru.SetLeader (con_leader);


    /*========================================================================================*/
	// 001 Tag 생성 
    /*========================================================================================*/

    ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("001*"),f_pUBNum);
    if( ids < 0 ){                
				return -261;
			}
    //005 Tag 생성
	
	TCHAR  s_005FieldData[128] ;
	s_005FieldData[0]='\0' ;
	CString temp_s_005FieldData ;
  
	GetSystemDate(s_005FieldData);
	GetSystemTime(&s_005FieldData[_tcsclen(s_005FieldData)]);

	temp_s_005FieldData.Format (_T("%s"),s_005FieldData);	
	ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("005*"),temp_s_005FieldData);
	if( ids ){	
	return -291;
	}    



    //008 Tag 생성

	if( s_008FieldData[0] == '\0' ){
	wmemset( s_008FieldData , ' ' , 40 );
	s_008FieldData[40] = '\0';
	}
	_tcsnccpy( s_008FieldData , &s_005FieldData[2] , 6 );

	temp_s_008FieldData.Format (_T("%s"),s_008FieldData);

	ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("008*"),temp_s_008FieldData);
	if( ids ){
	return -291;
	}

    // ISBN은 현재의 MARC DATA에서 ISBN번호만을 가지고 중복조사를 한후 중복이 아닌 경우에만 생성.
    // ISBN을 FIELD DATA를 가져온다.

	CArray<CString, CString&> array;
	CString strItemData;
    ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData );
	 if ( strItemData == _T("") ) {
		*f_pISBNNoCnt = 0 ;
        f_pISBNNo = NULL ;
	 }
	 else {
		 ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData,&array);
		 *f_pISBNNoCnt = array.GetSize () ;
		f_pISBNNo[0] = new CString[array.GetSize()] ;
        
		for ( i = 0; i < array.GetSize (); i++)
		{
		 f_pISBNNo[0][i] = array.GetAt(i);
		}
	 }
	 
   
	 
	 //012 Tag(국립중앙도서관 제어번호)
	 CString tmp_Data ;
	 tmp_Data.Empty ();
	 if( _tcscmp( f_pLibCode ,_T("011001")) == 0 ) {
		 ids = m_pMarcMgr->GetItem (&s_RawMarcStru,_T("001*"),tmp_Data );
		 
		 ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("012$a"),tmp_Data);
		 
	 }
	 else 
	 {   
		 ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("012"),tmp_Data);
		 if(! tmp_Data.IsEmpty ())
		 {
		 ids = m_pMarcMgr->InsertField (&s_ConvertMarcStru,_T("012")+tmp_Data);
		 }

	}


     /*========================================================================================*/
	 // 035 TAG 생성.( 중앙도서관의 경우 012 TAG생성 )
	 // 중앙도서관 또는 중앙도서관분관이 아닌 경우는 035 TAG에 입력
		
	 
	    CString tmp_f_pUBNum ;
		tmp_f_pUBNum.Empty ();
		ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("001*"),tmp_f_pUBNum);
        
        if( !tmp_f_pUBNum.IsEmpty ())
		{
			tmp_Data.Format (_T("(%s)%s"),f_pLibCode ,tmp_f_pUBNum);
			ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("035$a"),tmp_Data);
			if( ids < 0 ){                
				return -281;
			}
		}

		else 
		{   
			tmp_Data.Format (_T("(%s)"),f_pLibCode);
			ids = m_pMarcMgr->SetItem(&s_ConvertMarcStru,_T("035$a"),tmp_Data); 
		}


		
		
		// 2.9.040이 없는 경우 도서관부호를 생성.
		CString temp ;
		ids = m_pMarcMgr->GetField (&s_ConvertMarcStru ,_T("040"),temp);
		
		if( ids<0 )
		{			
			ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("040$a"),f_pLibCode);		
			if( ids < 0 ){			
				return -291;
			}
			ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("040$c"),f_pLibCode );
			if(ids<0){return -3000 ;}
		}
	
		
		
		// 2.10.기존의 505가 있는지를 검사하고 있는 경우 사용자가 MARCDAT를 수정할 수 있다.
		
		if( s_505FieldLength != 0 ){
			
			ids = m_pMarcMgr->GetField  (&s_RawMarcStru,_T("505"),f_pVolInfo);
			
		 }
	// 2.11.변환된 MARC구조체로 StreamMarc형태의 MarcData를 만든다.
	//ids = WriteMarcStruToStreamMarc( s_ConvertMarcStru , f_pSujiMasterMarcData );

	ids = m_pMarcMgr->Encoding (&s_ConvertMarcStru,f_pSujiMasterMarcData);
	if( ids ){
		return -211;
	}

	return 0;
}
	
	/*=========================================================================
	단위도서관 서지 MARC를 소장MARC로 변환.(도서,비도서,연속)
	IN  : f_pUHNum(UH 번호)
	f_pUBNum( UB번호 )
	f_pInputDate(입력일자)
	f_pLocalMarcData ( LocalMarc )
	OUT : f_pSujiMasterMarcData( 서지MASTERMARC )            
	=========================================================================*/
INT CMarcConversion::LocalMarcToHoldingMarc(  CString f_pUHNum          ,
	CString f_pUBNum          ,
	CString f_pLibCode        ,
	CString C_pLocalMarcData  ,
	CString f_pInputDate      ,
	CString &C_pSojangMarcData )
{
		
		CMarc       s_RawMarcStru,s_ConvertMarcStru;
		INT         ids ;
		TCHAR        s_tmpMarcData[10000];
		CString     strTemp;
	
		CString      First_ind ;
		CString      strItemData;
		BOOL        s_SingleBookFlag = TRUE;
		CArray<CString, CString&> array;
		
			
		ids = m_pMarcMgr->Decoding(C_pLocalMarcData, &s_RawMarcStru);
		if( ids ){
			return -1;
		}
	
		_tcscpy(s_tmpMarcData,C_pSojangMarcData.GetBuffer (0)) ;


	    CString s_CtrlNo ;	
	    s_CtrlNo.Empty () ;
		INT nTagNo;
		POSITION pos = s_RawMarcStru.m_listTag.GetHeadPosition () ;
		while( pos != NULL) 
		{   
			
			
			CString strField ;
			CTag *pTag = (CTag*)s_RawMarcStru.m_listTag.GetNext (pos) ;
			if (pTag == NULL) continue;
			
			nTagNo = _ttoi(pTag->GetTagCode());

			switch(nTagNo)
			{
			
			case 1 :	
				 if(nTagNo == 1) 
				 {
					 m_pMarcMgr->GetItem (&s_RawMarcStru,_T("001*"),s_CtrlNo) ;
				 }

		         break ;
			
				//--------------------------------------------------------------------------------//
				//  그대로 이동   
				//--------------------------------------------------------------------------------//
				
			case   7:
			case  12:
			case  22:
			// 2008.05.26 REM BY PDJ
			// 소장마크가 아니라 서지마크에 넣어야 되는 부분 
			//20080312 ADD BY PJW : CIP자료일경우 023tag추가해준다.
			//case  23:
			case  24:
			case  27:
			case  30:
			case  35:
			// 2008.05.26 REM BY PDJ
			// 소장마크가 아니라 서지마크에 넣어야 되는 부분 			
			//20080312 ADD BY PJW : 형태사항추가 300tsg
			//case 300:
			case 583:
				 
					if( nTagNo < 10 )
					{
						
						ids = m_pMarcMgr->GetItem (&s_RawMarcStru,pTag->GetTagCode()+_T("*"), strField) ;
						ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,pTag->GetTagCode()+_T("*"), strField);
						if(ids <0) {return -243 ; }
					}
					
					else
					{
						ids = m_pMarcMgr->GetField(&s_RawMarcStru, pTag->GetTagCode(), strField); 
						// 2008.05.26 REM BY PDJ
						// 소장마크가 아니라 서지마크에 넣어야 되는 부분 			
						/*
						//20080312 ADD BY PJW : 023,300 field에 데이터가 없을경우 넘어간다.
						if( nTagNo == 23 && strField.IsEmpty())
						{
							break;							
						}
						if( nTagNo == 300 && strField.IsEmpty())
						{
							break;							
						}
						*/
						ids = m_pMarcMgr->InsertField(&s_ConvertMarcStru,  pTag->GetTagCode() + strField);
						if( ids < 0 ){ return -243 ; }
					}
								
					break;     
           
			case 980 :
				
				ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("980$a"),strTemp) ;
				
				ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru ,_T("866$a"),strTemp);
				
				break ;

			default :
				
					
     		        break ;
					
				} //switch
				
	} //while
	


    INT   *s_pSortTemp;
	TCHAR        tmp_Leader[25];
	tmp_Leader[24] = '\0';

	TCHAR        s_005FieldData[128] ;
	s_005FieldData[0]='\0' ;

	CString     tmp_005 ;
	tmp_005.Format (_T("%s"),s_005FieldData); 
    CString      strField ;
	CString      tmp_008 ;
	//--------------------------------------------------------------------------------//
	//  1. Leader 생성 
	//--------------------------------------------------------------------------------//


	s_RawMarcStru.GetLeader (tmp_Leader) ;
	if(s_tmpMarcData[0] == '\0' ){
	tmp_Leader[5] = 'n' ;
	}
	if(tmp_Leader[7] == 'a' ){
		if(m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("049$v"),strTemp) == TRUE )
		{
		tmp_Leader[6] = 'v';
		}
		else 
		tmp_Leader[6] = 'x';

		}
	else if(tmp_Leader[7] == 'm')
	{
		if(m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("049$v"),strTemp) == TRUE )
		{
		tmp_Leader[6] = 'v' ;
		}
		else
		tmp_Leader[6] = 'x' ;

	}
	else if(tmp_Leader[7] =='c'){
		if(m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("049$v"),strTemp) == TRUE )
		{
		tmp_Leader[6] = 'v';
		}
		else 
		tmp_Leader[6] ='x';

	}
	else if(tmp_Leader[7] =='b'){
		if(m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("980*"),strTemp) == FALSE )
		{
		tmp_Leader[6] = 'x';
		}
		else 
		tmp_Leader[6] ='s';
	}

	else if( tmp_Leader[7] == 's'){
		tmp_Leader[6] = 's' ;
		}

		if( tmp_Leader[7] == 'c'|| tmp_Leader[7] == 'a' || tmp_Leader[7] == 'm' ){
		tmp_Leader[17] = '1';
	}
	else if( tmp_Leader[7] == 'b'|| tmp_Leader[7] == 's'  ){
	    tmp_Leader[17] = '5';
	}

	s_ConvertMarcStru.SetLeader (tmp_Leader);


	// 001 생성 
	ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("001*"),f_pUHNum);
	if(ids){
		return -3 ; }
	
	//004 생성
	ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("004*"),f_pUBNum);
	if(ids)
	{
        return -4 ;}
    
    //005생성
	GetSystemDate(s_005FieldData);
	GetSystemTime(&s_005FieldData[_tcsclen(s_005FieldData)]);
	tmp_005.Format (_T("%s"),s_005FieldData);
	ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("005*"),tmp_005); 
	if(ids){
	return -5 ;
	}

    //008생성 
/*	ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("008*"),strField);
	if(ids<0){
	return -6;
	}
*/


	TCHAR s_008FieldData[33];
	s_008FieldData[32] = '\0';
    TCHAR             s_CurentDate[128];
    GetSystemDate(s_CurentDate);

//	_tcscpy(s_008FieldData,strField.GetBuffer(0));
    wmemset ( s_008FieldData , ' ' , 32 );
	//입력 일자 
	_tcsnccpy(s_008FieldData,&s_CurentDate[2],6);
	//수서 상태 
	s_008FieldData[6] ='0' ;
	//수서 방법 
	s_008FieldData[7] ='u' ;
	//취소 일자 
	_tcsnccpy( &s_008FieldData[8] , _T("    "), 4 );
	//일반보유정책 
	s_008FieldData[12] = '0';
	//특수보유정책 
	_tcsnccpy(&s_008FieldData[13],_T("   "),3);
	//완전성 
	s_008FieldData[16] = '0';
	
	//복본수
	//_tcsnccpy(&s_008FieldData[17],_T("000"),3);
	CString str_Temp;
    INT     j, s_temp ;
	
	if(m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("049$c"),str_Temp) == FALSE )
	{
		_tcsnccpy( &s_008FieldData[17] , _T("001"), 3 );
	}
	
	else
	{
		if(m_pMarcMgr->GetItem (&s_RawMarcStru,_T("049$v"),str_Temp) == TRUE)
		{    
			array.RemoveAll ();
		  	m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("049$c"),strItemData, &array) ;

			s_pSortTemp = (INT*)malloc( sizeof(INT) * array.GetSize () );
            for(INT i = 0; i < array.GetSize (); i++ ){
					s_pSortTemp[i] = _ttoi( array.GetAt (i) );
				}
             
			// 쉘정렬로 오름 차순으로 정렬
			for( i = 1; i < array.GetSize (); i++ ){
				s_temp = s_pSortTemp[i];
				j = i - 1;
				while( j >= 0 && s_pSortTemp[j] > s_temp ){
					s_pSortTemp[j+1] = s_pSortTemp[j];
				}
				j--;
				s_pSortTemp[j+1] = s_temp;
			}


			_stprintf( &s_008FieldData[17] , _T("%03d"), s_pSortTemp[array.GetSize()-1] );
			free(s_pSortTemp);
		}
       
		else 

		{   array.RemoveAll ();
			m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("049$c"),strItemData, &array) ;
			_stprintf( &s_008FieldData[17] , _T("%03d"), _ttoi(strItemData.GetBuffer (0)) );
		}
        

	}
        
	//대출정책 
	s_008FieldData[20] = 'u';
	//복제정책 
	s_008FieldData[21] = 'u' ;
	//언어 
	_tcsnccpy(&s_008FieldData[22],_T("und"),3);
	//복본기술방법 
	s_008FieldData[25] = '1' ;
	//소장정책갱신일자 
	_tcsnccpy(&s_008FieldData[26],&s_005FieldData[2],6);


	tmp_008.Format (_T("%s"),s_008FieldData);

	ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("008*"),tmp_008); 
	if(ids) {
		return -20030111;
	}



	//014생성
		ids =m_pMarcMgr->GetItem  (&s_RawMarcStru ,_T("001*"),strField);
		if(!strField.IsEmpty () ){
		ids = m_pMarcMgr->SetItem  (&s_ConvertMarcStru,_T("014$a"),strField);
		m_pMarcMgr->SetTagIndicator (&s_ConvertMarcStru,_T("014"),_T("0"),_T("0"));
	
		}
	
	
	//020 생성 
//	INT start,end ;
	CString temp ;
	temp.Empty ();
    
	array.RemoveAll ();
	strItemData.Empty () ;
	//ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData,&array);
    ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData) ;
	
	CString tmp_020 ;
     
	if( !strItemData.IsEmpty ())
	{   
		
		ids = m_pMarcMgr->GetField (&s_RawMarcStru ,_T("020"),strItemData,&array);
		if(array.GetSize () > 1)
		{
			for ( INT i = 0; i < array.GetSize(); i++)
			{
			   if( array.GetAt (i).Left (1) == _T("1"))
			   {   /*
				   start = array.GetAt (i).Find (_T("$a"))+2 ;
				   end   = array.GetAt (i).Find (_T("$"),start);
                   if(end <0) 
				   {
					   end = array.GetAt(i).GetLength ();
				   }

				   tmp_020 = array.GetAt (i).Mid (start,end - start);
                   */
					tmp_020 = array.GetAt (i).GetBuffer (0);
					ids = m_pMarcMgr->InsertField (&s_ConvertMarcStru,_T("020") + tmp_020);
					if(tmp_020.Find (_T("$g")) == 0) 
					{
					ids = m_pMarcMgr->DeleteItem (&s_ConvertMarcStru,_T("020$g"),_T(""));
					}
				 
			   }
			}       
		}
	
		else  
		{   
			if(array.GetSize() == 1)
			{
			 ids = m_pMarcMgr->InsertField (&s_ConvertMarcStru,_T("020")+strItemData);

			 CString temp ;
			 temp.Format (_T("%cg"),SUBFIELD_CODE);
			 
             if(strItemData.Find (temp) > -1) 
			 {
			     ids = m_pMarcMgr->DeleteItem (&s_ConvertMarcStru,_T("020$g"),_T(""));
			 }


			}

		}
    
	}



    //852 생성 
   
	ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("049$f"),strTemp );
    if(ids == TRUE){
	   ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru ,_T("852$k"),strTemp);
	}

	strTemp.Empty () ;
    
	ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("090$a"),strTemp);
	if(ids == TRUE) {
       ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru ,_T("852$h"), strTemp, _T(""), ADD_SUBFIELD);
	   }
	ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("090$b"),strTemp);
	if(ids == TRUE) {
       ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru ,_T("852$i"),strTemp, _T(""), ADD_SUBFIELD);
	   }
    ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("090$c"),strTemp);
	if(ids == TRUE) {
       ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru ,_T("852$m"),strTemp, _T(""), ADD_SUBFIELD);
	   }
    
	if(!s_CtrlNo.IsEmpty ())
	{
	   ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("001*"),strTemp );
	   ids = m_pMarcMgr->SetItem (&s_ConvertMarcStru,_T("852$6") , strTemp, _T(""), ADD_SUBFIELD) ;
    }
    



    // 테스트 후 삭제 할 것 
    //ids = m_pMarcMgr->DeleteField (&s_ConvertMarcStru,_T("852*"));
	//ids = m_pMarcMgr->DeleteField (&s_ConvertMarcStru,_T("014*"));
    //ids = m_pMarcMgr->DeleteField (&s_ConvertMarcStru,_T("008*"));
    //ids = m_pMarcMgr->DeleteField (&s_ConvertMarcStru,_T("020*"));
    //ids = m_pMarcMgr->DeleteField  (&s_ConvertMarcStru ,_T("980$a"));
    
	ids = m_pMarcMgr->Encoding (&s_ConvertMarcStru,C_pSojangMarcData );
	if(ids){
		return -100 ;
	} 
	
   // AfxMessageBox(C_pSojangMarcData);

	return 0 ;
	
}





/*=========================================================================
국가자료공동목록 서지MASTERMARC를 단위도서관 서지 MARC로 변환.
IN  : f_pSujiMasterMarcData( 서지MASTERMARC )            
OUT : f_pLocalMarcData ( LocalMarc )
=========================================================================*/

INT CMarcConversion::SujiMasterMarcTOLocalMarc( CString f_pSujiMasterMarcData , CString &f_pLocalMarcData )
{
    
    return 0;
}


//===============================================================================
//  1. 등록번호 중복 검사를 위해 사용된다.
//  2. 등록번호, 권책기호, 복본기호의 값을 구한다.
//  인자 fo_pBookInfoCode ( marc 에서 값을 추출할 때만 사용한다. )
//       : 등록번호char(17)권책기호char(17)복본기호
//===============================================================================

INT CMarcConversion::Extract_BookInfo ( CMarc & fi_tpMarcInfo    ,
								        TCHAR       * fo_sErrorMsg     ,
										TCHAR       * fo_pBookInfoCode ,
                                        TCHAR       **f_pAccessionNo   ,
                                        TCHAR       **f_pVolCode       ,
                                        TCHAR       **f_pCopyCode      ,
										TCHAR       **f_pShelfCode     )
{
	CPlusIN049Tag cPlusIN049Tag ;
	BO_MARCIMPORT_TOOL s_MarcTool;
    TCHAR FieldData  [3060] ;
    TCHAR AccessionNo[24000] , VolCode[10240] , CopyCode[10240] , ShelfCode[10240];
    TCHAR OneBookInfo[512] , NextBookInfo[1024]  , tmp[64] ;	
    TCHAR *preptr , *strptr  ;
    TCHAR tokenstr[16];
    INT  rtn ;
    BOOL Plus_Exist ;

    //===============================================================================
    // 049 Tag Read
    //===============================================================================
	
//    INT ids  ;
    CString tmp_FieldData ;
	tmp_FieldData.Empty ();

	rtn = m_pMarcMgr->GetItem (&fi_tpMarcInfo ,_T("049*"),tmp_FieldData);
    
	_tcscpy(FieldData,tmp_FieldData.GetBuffer (0));

	
    if (rtn && fo_pBookInfoCode )
    {
		s_MarcTool.Save_ErrorMsg ( fo_sErrorMsg , _T("049 TAG 를 구할 때 오류 발생") );
		return -1;
    }

    //===============================================================================
    // 등록번호 별로 정보 저장
    //===============================================================================

    AccessionNo[0] = VolCode[0] = CopyCode[0] = ShelfCode[0] = '\0';
    Plus_Exist = FALSE ;

    _stprintf ( tokenstr , _T("%c%c") , GT , 'l' );

    strptr = FieldData;
    preptr = _tcsstr( strptr, tokenstr );
	if ( preptr == NULL )
		return -2;
    strptr = preptr + 2;

    while ( strptr = _tcsstr( strptr , tokenstr ) )
    {
        _stprintf ( OneBookInfo , _T("%.*s") , strptr - preptr , preptr );

        Plus_Exist = cPlusIN049Tag.CheckPlusExist ( OneBookInfo );
        if ( Plus_Exist == TRUE )
        { 
            preptr = strptr ;
            strptr += 2     ;
            strptr = _tcsstr( strptr , tokenstr );
            if ( strptr != NULL )  
                Plus_Exist = FALSE ;
            //------------------------------------------------------------- 
            // 등록번호에 '+' 가 있을 경우
            //  strptr 는 다음의 $l 를 가리키고 있어야 한다.
            //------------------------------------------------------------- 
			_stprintf ( tmp , _T("%.*s") , _tcsclen( OneBookInfo ) -1 , OneBookInfo );
			if ( strptr == NULL ) 
				_stprintf ( NextBookInfo , _T("%s") , preptr );
			else
                _stprintf ( NextBookInfo , _T("%.*s") , strptr - preptr , preptr );

			//-------------------------------------------------------------
			//  '+' 기호가 있는 등록번호 처리 
			//-------------------------------------------------------------
			if ( cPlusIN049Tag.Save_Plus_BookInfo ( tmp , NextBookInfo , 
				                                    AccessionNo , VolCode , CopyCode ,ShelfCode ) )
			{
				if ( fo_pBookInfoCode )
					s_MarcTool.Save_ErrorMsg( fo_sErrorMsg , _T(" '+' 가 있는 등록번호 처리 중 오류 ") ); 
				return -3;
			}

			if ( strptr == NULL )
				break;
        }
        else
        {
            //------------------------------------------------------------- 
            // 등록번호에 '+' 가 없을 경우
            //------------------------------------------------------------- 
            cPlusIN049Tag.Save_BookInfo  ( OneBookInfo, AccessionNo , VolCode , CopyCode ,ShelfCode );
        }

        preptr = strptr ;
        strptr += 2 ;        
    }

    if ( Plus_Exist == FALSE )
    {
        _stprintf ( OneBookInfo , _T("%s") , preptr );
        cPlusIN049Tag.Save_BookInfo  ( OneBookInfo, AccessionNo , VolCode , CopyCode ,ShelfCode );
    } 

    //===============================================================================
	// 등록번호의 중복을 확인한다.
    //===============================================================================
	/*if ( cPlusIN049Tag.Dup_AccessionNo ( AccessionNo , sDupAccessionNoList ) )
	{
		TCHAR temp[1024];
		_stprintf ( temp , _T("등록번호 중복 : %s") , sDupAccessionNoList );
		s_MarcTool.Save_ErrorMsg( fo_sErrorMsg , temp );

		return -4;
	}	*/

	/**f_pAccessionNo  = (TCHAR*)malloc(sizeof(TCHAR) * (_tcsclen(AccessionNo) + 1 ) );
    if( *f_pAccessionNo == NULL ){
        return -5;
    }
    _tcscpy( *f_pAccessionNo , AccessionNo );*/

    *f_pVolCode  = (TCHAR*)malloc(sizeof(TCHAR) * (_tcsclen(VolCode) + sizeof(TCHAR) ) );
    if( *f_pVolCode == NULL ){
        free(*f_pAccessionNo);
        return -6;
    }
    _tcscpy( *f_pVolCode , VolCode );

    *f_pCopyCode  = (TCHAR*)malloc(sizeof(TCHAR) * (_tcsclen(CopyCode) + sizeof(TCHAR) ) );
    if( *f_pCopyCode == NULL ){
        free(*f_pAccessionNo);
        free(*f_pVolCode);
        return -7;
    }
    _tcscpy( *f_pCopyCode , CopyCode );

    *f_pShelfCode  = (TCHAR*)malloc(sizeof(TCHAR) * (_tcsclen(ShelfCode) + sizeof(TCHAR) ) );
    if( *f_pShelfCode == NULL ){
        free(*f_pAccessionNo);
        free(*f_pVolCode);
        free(*f_pCopyCode);
        return -8;
    }
    _tcscpy( *f_pShelfCode , ShelfCode );


    return 0;
}


INT CMarcConversion::InsertISBNToMARC(CString f_pISBNInfo[], INT f_ISBNCnt, CString &f_pMarcData)
{
	CMarc    s_RawMarcStru;
    CString  s_ISBNNoData,s_tmpISBNNoData,s_SubFieldData,s_SubFieldCode;
    CString  *s_pISBNFieldData ;
    INT      i,j,ids,s_ISBNFieldCnt;

    // 1. StreamMarc를 Marc구조체로 변경한다. 
	ids = m_pMarcMgr->Decoding (f_pMarcData,&s_RawMarcStru);

    if( ids ){
        return -101;
    }
    
    
    CArray<CString, CString&> array;
	CString strItemData;
    CString tmp_subFieldData ;
    ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("020$a"),strItemData);
  //ids = m_pMarcMgr->GetField (&s_RawMarcStru,_T("020*"),strItemData);
	if( ids <0 ){
        s_ISBNFieldCnt = 0 ;
        s_pISBNFieldData = NULL ;
		return 0 ;
    }
    //s_pISBNFieldData = (CString**)malloc( sizeof(CString*) );
    
	else{
		ids = m_pMarcMgr->GetItem (&s_RawMarcStru ,_T("020$a"),strItemData,&array);
		s_ISBNFieldCnt = array.GetSize () ;
		s_pISBNFieldData = new CString[s_ISBNFieldCnt] ; 

		for(i = 0 ; i < s_ISBNFieldCnt ; i++)
		{
		if( !array.GetAt(i).IsEmpty() )
			s_pISBNFieldData[i] = array.GetAt (i);
		}
    }
    

    CTag *pTag ;
    CString First_ids ;
    pTag = s_RawMarcStru.FindTag (_T("020"));       
    First_ids = pTag->GetFirstIndicator () ;
     

/*
    if( s_ISBNFieldCnt == 0 )
	{
        for( i = 0; i < f_ISBNCnt; i++ )
		{
			ids = m_pMarcMgr->SetItem (&s_RawMarcStru ,_T("020*"),f_pISBNInfo[i]);
            if( ids < 0 )
			{
                return -202;
            }
        }
    }
*/
     INT start , end ;
  
        // SET ISBN 입력
     if ( array.GetSize () >0)
	 {
		for( i = 0; i < f_ISBNCnt; i++)
		{
			if(f_pISBNInfo[i].Left(1) != _T("1") )				
			continue; 
			for( j = 0; j < s_ISBNFieldCnt; j++ )
			{
				if( First_ids != _T("1")) continue;
			    
				start = f_pISBNInfo[i].Find (_T("$a")) + 2 ;
				end   = f_pISBNInfo[i].Find (_T("$"),start);
				s_SubFieldData = f_pISBNInfo[i].Mid (start,end-start) ;
			    
				s_ISBNNoData = s_SubFieldData.GetBuffer (0) ;            
				
				start = s_pISBNFieldData[j].Find (_T("$a")) + 2 ;
				end   = s_pISBNFieldData[j].Find (_T("$"),start);
				
				s_tmpISBNNoData = s_pISBNFieldData[j].Mid (start,end-start);

				if(  (s_ISBNNoData == s_tmpISBNNoData ) == 0 ) continue;
				
				ids = m_pMarcMgr->DeleteField (&s_RawMarcStru,_T("020"));
				ids = m_pMarcMgr->InsertField (&s_RawMarcStru ,_T("020") + f_pISBNInfo[i]);
				if( ids < 0 ){
				return -205;
				}
			}

		}
        // 낱권 ISBN 입력
        for( i = 0; i < f_ISBNCnt; i++)
		{
            if( f_pISBNInfo[i].Left(1) != ' ') continue;
            
			for( j = 0; j < s_ISBNFieldCnt; j++ )
			{
				if( First_ids != ' ') continue;          
				
				start = f_pISBNInfo[i].Find (_T("$a")) + 2 ;
				end   = f_pISBNInfo[i].Find (_T("$"),start);
				s_SubFieldData = f_pISBNInfo[i].Mid (start,end-start) ;
				s_ISBNNoData = s_SubFieldData.GetBuffer (0) ;           
			
				start = s_pISBNFieldData[j].Find (_T("$a")) + 2 ;
				end   = s_pISBNFieldData[j].Find (_T("$"),start);
				
				s_tmpISBNNoData = s_pISBNFieldData[j].Mid (start,end-start);
				if( _tcscmp( s_ISBNNoData, s_tmpISBNNoData ) == 0 ) continue;

				ids = m_pMarcMgr->InsertField  (&s_RawMarcStru ,_T("020")+f_pISBNInfo[i]);
				if( ids < 0 ){
				return -208;
				}
            }
        }

	 }

    ids = m_pMarcMgr->Encoding ( &s_RawMarcStru , f_pMarcData );
    if( ids ){       
        return -209;
    }
	
    delete []s_pISBNFieldData ;

    return 0;
}



INT CMarcConversion::InsertISBNToMARC(CString f_pISBNInfo[], INT f_ISBNCnt, CMarc &f_pMarcData)
{
    CString          s_ISBNNoData,s_tmpISBNNoData,s_SubFieldData,s_SubFieldCode;
    CString          *s_pISBNFieldData = NULL;
    INT           i,j,ids,s_ISBNFieldCnt;
	

    CArray<CString, CString&> array;
	CString strItemData;
    CString tmp_subFieldData ;
    ids = m_pMarcMgr->GetField  (&f_pMarcData ,_T("020"),strItemData,&array);
	if( ids ){
        return -201;
    }	
	
    s_ISBNFieldCnt = array.GetSize () ;
    s_pISBNFieldData = new CString[array.GetSize ()] ;
    for ( i = 0 ; i < array.GetSize () ; i ++) 
	{
        s_pISBNFieldData[i] = array.GetAt(i) ;
	}
 

    if( s_ISBNFieldCnt == 0 ){
        for( i = 0; i < f_ISBNCnt; i++ ){
            
			ids = m_pMarcMgr->InsertField  (&f_pMarcData,_T("020")+f_pISBNInfo[i]) ;
            if( ids < 0 ){
                return -202;

            }
        }
    }
    else{
        // SET ISBN 입력
        for( i = 0; i < f_ISBNCnt; i++){
            for( j = 0; j < s_ISBNFieldCnt; j++ ){
               
				s_SubFieldData = f_pISBNInfo[i].Mid(5,10) ;
                //Get_Keyword( s_SubFieldData , s_SubFieldData );
                s_ISBNNoData = s_SubFieldData ;                  
                s_tmpISBNNoData = s_pISBNFieldData[j] ;
                //Get_Keyword( s_SubFieldData , s_SubFieldData );
                      
                if( ( s_ISBNNoData == s_tmpISBNNoData ) == 0 ) break;
            }
			if( j >= s_ISBNFieldCnt ){
				
				ids = m_pMarcMgr->InsertField  (&f_pMarcData ,_T("020")+f_pISBNInfo[i]);
                if( ids < 0 ){          
                    return -205;
                }
			}
        }
    }

  
    return 0;
}







// 서지 MASTER를 통합한다.
INT CMarcConversion::MergeSujiMasterMarc(CString f_pSourceMarcData, CString  &f_pMergeMarcData)
{
	CMarc      s_SourceMarcStru,s_MergeMarcStru;
	CString    s_SubFieldCode1,s_SubFieldCode2,s_FieldData;
	CString    *s_pISBNFieldData;
	INT        i,ids,s_ISBNFieldCnt;
	BOOL       s_RepeateFlag = FALSE;
    CString    TempData ;
    
	// 1. StreamMarc를 Marc구조체로 변경한다.
	ids = m_pMarcMgr->Decoding (f_pSourceMarcData,&s_SourceMarcStru);    
	if( ids ){
	return -101;
	}

	// 2. StreamMarc를 Marc구조체로 변경한다.
	ids = m_pMarcMgr->Decoding (f_pMergeMarcData,&s_MergeMarcStru);
	if( ids ){
	return -201;
	}

	//-------------------------------------------------------------//
    
	CArray<CString, CString&> array;
    CString strItemData;

	INT nTagNo;
	POSITION pos = s_MergeMarcStru.m_listTag.GetHeadPosition () ;
	while( pos != NULL) 
	{   

		CString strField ;
		CTag *pTag = (CTag*)s_MergeMarcStru.m_listTag.GetNext (pos) ;
		if (pTag == NULL) continue;

		nTagNo = _ttoi(pTag->GetTagCode());
        // 3.MARC를 통합.
		switch(nTagNo) 
		{
        // 3.1. 001,505의 DATA는 통합대상 마크의 자료는 제외한다.
	    case   1:
        case   5:
			    break ;

        // 3.2.ISBN을 중복체크하여 복사.
		case  20:
				if( s_RepeateFlag == FALSE )
				{
				    
				    ids = m_pMarcMgr->GetItem (&s_MergeMarcStru,_T("020*"),strItemData,&array);
                    if( ids ){
					         return -321;
					}

					s_ISBNFieldCnt = array.GetSize() ;
					s_pISBNFieldData = new CString[array.GetSize()] ;

					for( i = 0 ; i < s_ISBNFieldCnt ; i++)
					{
						s_pISBNFieldData[i] = array.GetAt (i) ;
					}
					ids = InsertISBNToMARC( s_pISBNFieldData , s_ISBNFieldCnt , s_SourceMarcStru );
					if( ids){
					return 322;
					}

                    for( i = 0 ; i < s_ISBNFieldCnt ; i++)
					{
						delete [] s_pISBNFieldData ;
					}
				}
				s_RepeateFlag = TRUE;

				break;

        // 3.3.1XX,250,255,260,300 TAG의 DATA은 통합주MARC에 없으면 INSERT 있으면 제외
        case 100:
        case 110:
        case 111:
        case 130:
        case 250:
        case 255:
        case 260:
        case 300:  
                 
			    if( s_SourceMarcStru.FindTag(pTag->GetTagCode()) == FALSE )
				{
                    ids = m_pMarcMgr->GetItem (&s_MergeMarcStru,pTag->GetTagCode()+_T("*") ,s_FieldData) ;
                    if(ids){
						return 400 ;
					}

					ids = m_pMarcMgr->SetItem (&s_SourceMarcStru,pTag->GetTagCode()+_T("*") ,s_FieldData) ;
					if( ids < 0 ){
					return -331;
					}

				}
				
				break;

        // 3.4.----------------------------------------------------------------------------------//
        //     3.4.1.$n,$p가 있고 505가 없으면 $n,$p 무시
        //     3.4.2.$n,$p가 있고 505가 있으면 $n,$p 무시
        //     3.4.1.$n이 있고 505가 있으면 통합주 MARC에 505 삽입한다.(통합주MARC의 505를 삭제)
        //---------------------------------------------------------------------------------------//
		case 245 :     
				ids = m_pMarcMgr->GetItem (&s_MergeMarcStru,_T("245$n"),TempData);
			    ids = m_pMarcMgr->GetItem (&s_MergeMarcStru,_T("245$p"),TempData);	
			    /*

				if( SearchSubFieldCodeFromMarcStru( 245 , s_SubFieldCode1, s_MergeMarcStru ) == TRUE &&
				SearchSubFieldCodeFromMarcStru( 245 , s_SubFieldCode2, s_MergeMarcStru ) == FALSE )
				{

					s_pFieldData = NULL;
					s_FieldDataCnt = 0;

					ids = GetMultiFieldDataFromMarcStru( 505 , &s_MergeMarcStru , &s_FieldDataCnt , &s_pFieldData );
					if( ids ){			
					return -342;
					}

					for( j = 0; j < s_FieldDataCnt; j++ ){

					ids = InsertFieldDataToMarcStru( 505              ,
											 s_pFieldData[j] ,
											 &s_SourceMarcStru );
					if( ids < 0 ){		
					return -343;
					}
					}
				}

                */
				break;

    

         
		}//switch 
    
	}//while
      
     
    return 0;
}





// 소장 MARC를 통합한다.
INT CMarcConversion::MergeHoldMarc( CString f_pSourceMarcData , CString &_pMergeMarcData )
{
	
	
    return 0;
}




INT  CMarcConversion::GetSystemDate( TCHAR * pDate )
{
    SYSTEMTIME t;
    GetLocalTime(&t);
    _stprintf( pDate ,_T("%4d%02d%02d") ,t.wYear , t.wMonth , t.wDay);
    return 0;
}


INT  CMarcConversion::GetSystemTime( TCHAR * pTime )
{
    SYSTEMTIME t;
    GetLocalTime(&t);
    _stprintf( pTime ,_T("%02d%02d%02d") ,t.wHour , t.wMinute , t.wSecond );
    return 0;
}


VOID CMarcConversion::str_rtrim ( TCHAR *str_in , TCHAR *str_out , TCHAR var )
{
    INT i = 0 ;
	
    _tcscpy( str_out, str_in );
    for ( i = _tcsclen(str_in)-1; i >= 0; i-- ){
        if(str_in[i] != var) {
            str_out[i+1] = '\0';
            return;
        }
    }
    str_out[0] = '\0';
}

VOID CMarcConversion::Get_Keyword(TCHAR *data, TCHAR *keyword , BOOL f_KeywordFlag )
{
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

VOID  CMarcConversion::str_trimall(TCHAR   *strln , TCHAR   *strline1, TCHAR    var )
{  
  	INT     i = 0, j = 0;

    for ( i = 0; strln[i] != '\0' && strln[i] != '\n'; i++ ) {
        if ( strln[i] == var );
        else { strline1[j] = strln[i]; j++; }
    }
    strline1[j] ='\0';

}
