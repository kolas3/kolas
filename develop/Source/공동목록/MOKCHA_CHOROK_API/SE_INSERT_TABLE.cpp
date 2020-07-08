// SE_INSERT_TABLE.cpp: implementation of the CSE_INSERT_TABLE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SE_INSERT_TABLE.h"
#include "..\..\공통\K2UP_Common\MakeSearchData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSE_INSERT_TABLE::CSE_INSERT_TABLE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{

     if(InitESL_Mgr(_T("SM_MokCha_Chorok")) <0)
	 {
		 AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		 return ;
	 }
}

CSE_INSERT_TABLE::~CSE_INSERT_TABLE()
{

}

INT CSE_INSERT_TABLE::SetInsertValue (CString sVol_Key,CString sSpecies_Key,CString sControl_No,CString sMokcha)
{
    INT ids ;
	//날짜 시간 //////////////////////////////////////////////////////////
    CString system_Date ;
	CTime t = CTime::GetCurrentTime () ;
    system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;

 
	CESL_DataMgr * pContentsDM_Mgr = FindDM(_T("DM_CONTENTMGR_SE_TBL")) ;
	if(pContentsDM_Mgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}

	CString sContents_RecKey ;
	pContentsDM_Mgr->MakeRecKey (sContents_RecKey);

    ////////////////////////////////////////////////////////////////////////    
    //SE_CONTENTS_TBL에 DATA 입력 
	////////////////////////////////////////////////////////////////////////
    
	pContentsDM_Mgr->StartFrame () ;
	pContentsDM_Mgr->InitDBFieldData () ;
	
	pContentsDM_Mgr->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),sContents_RecKey );
    pContentsDM_Mgr->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),sSpecies_Key );
	pContentsDM_Mgr->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),sVol_Key );
	
	pContentsDM_Mgr->AddDBFieldData (_T("CONTROL_NO"),_T("STRING"),sControl_No  );
	pContentsDM_Mgr->AddDBFieldData (_T("CONTENTS"),_T("STRING"),sMokcha );
//	pContentsDM_Mgr->AddDBFieldData (_T("USE_LIMIT_CODE"),_T("STRING"),sUselimitCode );
    pContentsDM_Mgr->AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),system_Date);
    pContentsDM_Mgr->AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),system_Date);
   
	pContentsDM_Mgr->MakeInsertFrame (_T("SE_CONTENTS_TBL")) ;
     
    pContentsDM_Mgr->InitDBFieldData () ;

	pContentsDM_Mgr->AddDBFieldData (_T("CONTENTS_YN"),_T("STRING"),_T("Y")) ;
    pContentsDM_Mgr->MakeUpdateFrame (_T("SE_VOL_TBL"),_T("REC_KEY"),_T("NUMERIC"),sVol_Key);


	m_Contents_Key = sContents_RecKey ;
	m_Vol_Key = sVol_Key ;


    ids = GetFieldValue(pContentsDM_Mgr,sMokcha) ;
    if(ids <0 )
	{   
		AfxMessageBox(_T("GetFieldValue() Error~!")) ;
		pContentsDM_Mgr->EndFrame () ;
	}

	pContentsDM_Mgr->SendFrame () ;
	pContentsDM_Mgr->EndFrame () ;

    
   
	return 0 ;
}


INT CSE_INSERT_TABLE::GetFieldValue(CESL_DataMgr *pDM,CString strTemp)
{
    CString strBody ;
	strBody = strTemp ;
    
	strBody.TrimLeft () ; strBody.TrimRight () ;

	INT first,end ;
	first = strBody.Find (_T("<body>")) ;
    end   = strBody.Find (_T("</body>"));

    strBody = strBody.Mid (first,end-first) ;
	first = strBody.Find (_T("\r\n")) ;
 
	strBody = strBody.Mid (first,strBody.GetLength ()-(first+2) ) ;

  //  AfxMessageBox(strBody) ;
    
	CArray<CString,CString> ArrayRecord ;

	MakeArrayList(strBody,_T("\r\n"),ArrayRecord) ;


	INT RecCnt ;

	RecCnt = ArrayRecord.GetSize () ;
     
	INT RowIndex = 1 ;
	
	if(RecCnt > 0 ){
       
	  for(INT i = 0 ; i < RecCnt ;i++) 
	  {
    	SetFieldData(pDM,i , ArrayRecord.GetAt (i)) ;
      }

	}
    else
	{
		AfxMessageBox(_T("생성할 기사가 없습니다"));
		return -1 ;
	}

	return 0;
}


INT CSE_INSERT_TABLE::MakeArrayList( CString sLine , CString sDelimiter , CArray <CString,CString>&asArray )
{


	asArray.RemoveAll();

	sLine.TrimLeft(); sLine.TrimRight();

	INT nIndex;
	CString sFieldName;


	bool IsStop = FALSE;
	while( TRUE )
	{
		nIndex = sLine.Find(sDelimiter);

		if( nIndex < 0 )
		{
		asArray.Add( sLine );
		IsStop = TRUE;
		}
		else
		{
		sFieldName = sLine.Mid( 0 , nIndex );
		sLine = sLine.Mid( nIndex+2 , sLine.GetLength()-nIndex );
		sFieldName.TrimLeft(); sFieldName.TrimRight();
		asArray.Add( sFieldName );
		}

		if( IsStop ) break;
	}

	return 0;


//	return -1;

}

INT CSE_INSERT_TABLE::SetFieldData(CESL_DataMgr *pDM, INT RowIndex , CString sData ) 
{

    INT nIndex , nextIndex ,nthirdIndex ;
    

	CString sTitle = _T("");
    CString sKisaControl_No =_T("");
    CString sAuthor =_T("");
    CString sPage =_T(""); 

	sData.TrimLeft () ; sData.TrimRight () ;
	nIndex  = sData.Find (_T("/")) ;
    
	if(nIndex <0) 
	{
        
        nextIndex = sData.Find (_T("=")) ;
		if(nextIndex<0) 
		{
			nthirdIndex = sData.Find (_T("[!--")) ;
			if(nthirdIndex <0 )
			{
				sTitle = sData ;
				sTitle.TrimLeft () ,sPage.TrimRight () ;
			
			}
			else{

				sTitle = sData.Mid (0,nthirdIndex) ;
				sTitle.TrimLeft () ;sTitle.TrimRight () ;
				sData = sData.Mid (nthirdIndex+4,sData.GetLength ()-(nthirdIndex+7)) ;
				sKisaControl_No = sData ;
				sKisaControl_No.TrimLeft ();sKisaControl_No.TrimRight () ;
			}
		}
		else 
		{
			sTitle = sData.Mid (0,nextIndex) ;
			sTitle.TrimLeft () ; sTitle.TrimRight () ;
			sData = sData.Mid (nextIndex+1,sData.GetLength () - nextIndex) ;

			nthirdIndex = sData.Find(_T("[!--")) ;
			if(nthirdIndex <0 )
			{ 
				sPage = sData ;
		        sPage.TrimLeft () ,sPage.TrimRight () ;
			
			}
			else{
			
			sPage = sData.Mid (0,nthirdIndex) ;
			sPage.TrimLeft () ,sPage.TrimRight () ;

			sKisaControl_No = sData.Mid (nthirdIndex+4,sData.GetLength ()-(nthirdIndex+7)) ;
			sKisaControl_No.TrimLeft () ,sKisaControl_No.TrimRight () ;
           
			}
		}

      
	}
    else
	{
       sTitle = sData.Mid(0,nIndex) ;
	   sTitle.TrimLeft () ; sTitle.TrimRight () ;
	   sData = sData.Mid (nIndex+1,sData.GetLength () - nIndex);
       
	   nextIndex = sData.Find (_T("=")) ;
	   if(nextIndex <0) {
		      			
	   }
	   else{

		   sAuthor = sData.Mid (0,nextIndex) ;
		   sAuthor.TrimLeft () ; sAuthor.TrimRight () ;
		   sData = sData.Mid(nextIndex+1,sData.GetLength () - nextIndex) ;
	   } 
       nthirdIndex = sData.Find(_T("[!--")) ;
	   if(nthirdIndex <0 )
	   {
		   sPage = sData ;
		   sPage.TrimLeft () ,sPage.TrimRight () ;
	
	   }
	   else{

       sPage = sData.Mid (0,nthirdIndex) ;
	   sPage.TrimLeft () ,sPage.TrimRight () ;

	   sKisaControl_No = sData.Mid (nthirdIndex+4,sData.GetLength ()-(nthirdIndex+7)) ;
       sKisaControl_No.TrimLeft () ,sKisaControl_No.TrimRight () ;
	   }
	   
	  }
     
	if(!sKisaControl_No.IsEmpty ())
	{
       
		SetDataKisa(pDM,sTitle,sKisaControl_No,sAuthor,sPage) ;
	}
	return 0 ;
}

INT CSE_INSERT_TABLE::SetDataKisa(CESL_DataMgr *pDM, CString sTtitle, CString sKisaControl_No, CString sAuthor, CString sPage)
{
    INT ids ;	    
	CString sArticle_Key ;  
	     
	pDM->MakeRecKey (sArticle_Key) ;
	

	CString Current_Date ;
	CTime t = CTime::GetCurrentTime () ;
	Current_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());


	pDM->InitDBFieldData () ;
	
	pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),sArticle_Key);
	pDM->AddDBFieldData (_T("CONTENTS_KEY"),_T("NUMERIC"),m_Contents_Key) ;
	pDM->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),m_Vol_Key );
	pDM->AddDBFieldData (_T("ARTICLE_CONTROL_NO"),_T("STRING"),sKisaControl_No );
	pDM->AddDBFieldData (_T("ARTICLE_NAME"),_T("STRING"),sTtitle);
	pDM->AddDBFieldData (_T("AUTHOR"),_T("STRING"),sAuthor);
	pDM->AddDBFieldData (_T("START_PAGE"),_T("STRING"),sPage);
	pDM->AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),Current_Date);
	pDM->AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),Current_Date);
	pDM->MakeInsertFrame (_T("SE_ARTICLE_TBL"));
	
	ids = Insert_Se_IndexTable(pDM,sTtitle,sAuthor,sPage,m_Vol_Key,m_Contents_Key);
	if(ids == 100)
	{
		AfxMessageBox(_T("Insert_Se_IndexTable() Error~!"));
		pDM->EndFrame () ;
		return -1;
	}


	return 0 ;
}


INT CSE_INSERT_TABLE::Insert_Se_IndexTable(CESL_DataMgr *pTmpDM,CString sArticle,CString sAuthor,CString sPage_info ,CString sVol_Key,CString sContent_Key)
{
	 INT ids ;
     CString nIndexRecKey;
     CString strIndexArticle ,strIndexAuthor;

	 strIndexArticle = sArticle ;
	 strIndexAuthor  = sAuthor ;

	 CMakeSearchData *pMakeData = NULL; 

	 pTmpDM->MakeRecKey (nIndexRecKey);
  
	 CString sMainDMAlias = _T("DM_CONTENTMGR_SE_MAIN") ;
     CESL_DataMgr *pMainDM = FindDM(sMainDMAlias);
	 if(pMainDM == NULL) {
		 AfxMessageBox(_T("FindDM() Error~!"));
		 return -1 ;
	 }


      
	 CString sTitle ,sPublisher,sPub_year,sPlace_info,sSe_shelf_code,sClass_no,sBook_code ;

	 pMarcMgr = m_pInfo->pMarcMgr  ;
	 pMarcMgr->SetMarcGroup (_T("통합마크")) ;
	 ids = pMarcMgr->Decoding (m_strMarc,&pMarc) ;

     pMarcMgr->GetItem (&pMarc,_T("090$a"),sClass_no);
     pMarcMgr->GetItem (&pMarc,_T("090$b"),sBook_code);
	 pMarcMgr->GetItem (&pMarc,_T("245$a"),sTitle);
	 pMarcMgr->GetItem (&pMarc,_T("260$b"),sPublisher);
	 pMarcMgr->GetItem (&pMarc,_T("260$c"),sPub_year);
     pMarcMgr->GetItem (&pMarc,_T("049$f"),sSe_shelf_code);

	 
	INT iCh = pMakeData->ChangeHanjaToHangul( strIndexArticle );	//한자 한글 변환
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexArticle ) error") );
		return -1;
	}
	iCh = pMakeData->GetKeyWord( strIndexArticle );				//공백, 특수문자 제거
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexArticle ) error") );
		return -2;
	}

	iCh = pMakeData->ChangeHanjaToHangul( strIndexAuthor );	//한자 한글 변환
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexAuthor ) error") );
		return -1;
	}
	iCh = pMakeData->GetKeyWord( strIndexAuthor );				//공백, 특수문자 제거
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexAuthor ) error") );
		return -2;
	}


	 pTmpDM->InitDBFieldData () ;
	 pTmpDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),nIndexRecKey) ;
	 pTmpDM->AddDBFieldData (_T("IDX_ARTICLE"),_T("STRING"),strIndexArticle) ;
	 pTmpDM->AddDBFieldData (_T("IDX_IARTICLE"),_T("STRING"),strIndexArticle) ;
     
	 pTmpDM->AddDBFieldData (_T("IDX_ARTICLE_KEYWORD"),_T("STRING"),strIndexArticle);
	 pTmpDM->AddDBFieldData (_T("IDX_ARTICLE_AUTHOR"),_T("STRING"),strIndexAuthor );
     
	 pTmpDM->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),sVol_Key );
     
	 pTmpDM->AddDBFieldData (_T("TITLE_INFO"),_T("STRING"),sTitle );
     pTmpDM->AddDBFieldData (_T("PUB_INFO"),_T("STRING"),sPublisher );
	 pTmpDM->AddDBFieldData (_T("PUB_YEAR_INFO"),_T("STRING"),sPub_year );
     pTmpDM->AddDBFieldData (_T("PLACE_INFO"),_T("STRING"),sPlace_info );
	 pTmpDM->AddDBFieldData (_T("SE_SHELF_CODE"),_T("STRING"),sSe_shelf_code );
	 pTmpDM->AddDBFieldData (_T("CLASS_NO"),_T("STRING"),sClass_no );
	 pTmpDM->AddDBFieldData (_T("BOOK_CODE"),_T("STRING"),sBook_code );
     
	 pTmpDM->AddDBFieldData (_T("ARTICLE_INFO"),_T("STRING"),sArticle );
     pTmpDM->AddDBFieldData (_T("ARTICLE_AUTHOR_INFO"),_T("STRING"),sAuthor );
     pTmpDM->AddDBFieldData (_T("ARTICLE_PAGE_INFO"),_T("STRING"),sPage_info );


	 ids = pTmpDM->MakeInsertFrame (_T("IDX_SE_TOC_TBL"));
     if(ids<0) {
		 AfxMessageBox(_T("MakeInsertFrame() Error~!"));
		 return 100 ;
     }

	 return 0 ;
}