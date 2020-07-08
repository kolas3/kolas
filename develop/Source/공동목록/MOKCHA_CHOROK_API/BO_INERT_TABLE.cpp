// BO_INERT_TABLE.cpp: implementation of the CBO_INERT_TABLE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_INERT_TABLE.h"
#include "..\..\공통\K2UP_Common\MakeSearchData.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_INERT_TABLE::CBO_INERT_TABLE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	 m_BO_TOC_COMMON_TBL = _T("BO_TOC_COMMON_TBL");
	 m_CONTENTS_TBL =_T("BO_CONTENTS_TBL");
	 m_ABSTRACTS_TBL=_T("BO_ABSTRACTS_TBL");
	 m_IDX_BO_TOC_TBL=_T("IDX_BO_TOC_TBL") ;

	 if(InitESL_Mgr(_T("SM_MokCha_Chorok")) <0)
	 {
		 AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		 return ;
	 }

}

CBO_INERT_TABLE::~CBO_INERT_TABLE()
{

}


INT CBO_INERT_TABLE::SetInsertValue (CString sSpecies_Key,CString Control_No,CString sMokcha,CString sChorok)
{
	INT ids ;
	CESL_DataMgr *pDM = FindDM(_T("DM_목차초록")) ;
	if(pDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1;
	}
    

	CESL_DataMgr *pSpecies_DM = FindDM(_T("DM_BO_CONTENTMGR_SPECIES"));
	if(pSpecies_DM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!"));
		return -2 ;
	}
   
    m_Mokcha = sMokcha;
	m_Chorok = sChorok;


	CString sWhere ;
	sWhere.Format (_T("REC_KEY = %s"),sSpecies_Key);
    pSpecies_DM->RefreshDataManager (sWhere);


    CString sRec_Key ;
    CString system_date ;
    CString sCho_Rec_Key ;
    CString sMok_Rec_Key ;

	CTime t = CTime::GetCurrentTime ();
	system_date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;
	pDM->MakeRecKey (sRec_Key);
    
	pDM->StartFrame () ;
	
	//단행자료-목차 공통 입력 (BO_TOC_COMMON_TBL)
    pDM->InitDBFieldData () ;
    pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),sRec_Key) ;
	pDM->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),sSpecies_Key);
    pDM->AddDBFieldData (_T("CONTROL_NO"),_T("STRING"),Control_No);
    pDM->AddDBFieldData (_T("SPECIES_CLASS"),_T("STRING"),_T("1"));   
    ids = pDM->MakeInsertFrame (_T("BO_TOC_COMMON_TBL")) ; 
    
    //목차/초록 각각의 테이블에 정보를 넣는다.
    if(!sMokcha.IsEmpty ())
	{
	
		pDM->MakeRecKey (sMok_Rec_Key);

		pDM->InitDBFieldData ();
        pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),sMok_Rec_Key);
		pDM->AddDBFieldData (_T("TOC_COMMON_KEY"),_T("NUMERIC"),sRec_Key);
		pDM->AddDBFieldData (_T("CONTENTS"),_T("STRING"),sMokcha);
        pDM->AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),system_date);
        pDM->MakeInsertFrame(_T("BO_CONTENTS_TBL")) ;

		pDM->InitDBFieldData () ;
		pDM->AddDBFieldData(_T("CONTENTS_KEY"),_T("NUMERIC"),sMok_Rec_Key);
		ids  = pDM->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),sRec_Key);

	}
	
	if(!sChorok.IsEmpty ())
	{
		
		pDM->MakeRecKey (sCho_Rec_Key);
		pDM->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),sCho_Rec_Key);
		pDM->AddDBFieldData(_T("CREATE_DATE"),_T("STRING"),system_date);
        pDM->AddDBFieldData (_T("TOC_COMMON_KEY"),_T("NUMERIC"),sRec_Key);
		pDM->AddDBFieldData(_T("ABSTRACTS"),_T("STRING"),sChorok);
        ids = pDM->MakeInsertFrame (_T("BO_ABSTRACTS_TBL")) ;

		pDM->InitDBFieldData () ;
		pDM->AddDBFieldData(_T("ABSTRACTS_KEY"),_T("NUMERIC"),sCho_Rec_Key);
		ids  = pDM->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),sRec_Key);
	}

    InsertIndexTable(pDM,sSpecies_Key,sMok_Rec_Key,sCho_Rec_Key) ;

    
	pDM->SendFrame ();
    pDM->EndFrame () ;

    

	return 0 ;
}


INT CBO_INERT_TABLE::InsertIndexTable( CESL_DataMgr *pTempMgr,CString v_SPECIES_KEY,CString m_strConKey, CString m_strAbsKey) 
{
	INT ids ;
	  
	CString sBody;   

	CString v_CLASS_NO, v_BOOK_CODE, v_VOL_CODE_DISP, v_TITLE_INFO, v_AUTHOR_INFO, v_PUB_INFO, v_PUB_YEAR_INFO ;

	CIndexTool *pIndex;
	pIndex = new CIndexTool();
	
    pMarcMgr = m_pInfo->pMarcMgr  ;
	ids = pMarcMgr->Decoding (m_strMarc,&pMarc) ;
    
	pMarcMgr->GetItem (&pMarc,_T("090$a"),v_CLASS_NO);
    pMarcMgr->GetItem (&pMarc,_T("090$b"),v_BOOK_CODE);
	pMarcMgr->GetItem (&pMarc,_T("090$c"),v_VOL_CODE_DISP);
	pMarcMgr->GetItem (&pMarc,_T("245$a"),v_TITLE_INFO);
	pMarcMgr->GetItem (&pMarc,_T("245$d"),v_AUTHOR_INFO);
	pMarcMgr->GetItem (&pMarc,_T("260$b"),v_PUB_INFO);
	pMarcMgr->GetItem (&pMarc,_T("260$c"),v_PUB_YEAR_INFO);

	CString v_TOC_CREATE_DATE,v_ABS_CREATE_DATE ;
	CTime t = CTime::GetCurrentTime ();

	v_TOC_CREATE_DATE.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;
	v_ABS_CREATE_DATE.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ; 


    if(!m_Mokcha.IsEmpty ())
	{
		sBody = m_Mokcha ;		

		MakeIndexKeyWord(sBody) ;		

		INT iCh = pIndex->GetIndex()->ConvertHanjaToHangul( sBody );	//한자 한글 변환
		if ( 0 != iCh ) 
		{			
			AfxMessageBox ( _T("ChangeHanjaToHangul( sBody ) error") );
			delete pIndex;
			return -1;
		}

		iCh = pIndex->GetIndex()->RemoveSpecialChar( sBody );	//한자 한글 변환
		if ( 0 != iCh ) 
		{			
			AfxMessageBox ( _T("RemoveSpecialChar( sBody ) error") );
			delete pIndex;
			return -1;
		}

		//2004-03-11[이학중 수정]
		/*
		INT iCh = pMakeData->ChangeHanjaToHangul( sBody );	//한자 한글 변환
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			return -1;
		}
		
		INT iCh = pMakeData->GetKeyWord( sBody );				//공백, 특수문자 제거
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			return -2;
		}*/		
		
		CString v_RecKey;
		pTempMgr->MakeRecKey (v_RecKey) ;
		
		
		pTempMgr->InitDBFieldData () ;
		pTempMgr->AddDBFieldData (_T("REC_KEY") ,_T("NUMERIC"),v_RecKey) ;
		//=====================================================
		//2009.01.13 UPDATE BY PJW : UTF8변환 LENGTH가 4000이상일경우는 저장하지 않는다.
// 		pTempMgr->AddDBFieldData (_T("IDX_TOC_KEYWORD") ,_T("STRING"),sBody) ;
		INT nGetByte;
		CString strTempData;
		strTempData = sBody;
		nGetByte = WideCharToMultiByte( CP_UTF8	, 
								0				,
								strTempData.GetBuffer(0), 
								-1				,
								NULL			, 
								0				,
								NULL			, 
								NULL      );
		
		// 목차일때			
		if( nGetByte > 4000 )
		{
			AfxMessageBox(_T("목차 KEYWORD의 길이가 너무 깁니다."));
		}
		else
		{
			pTempMgr->AddDBFieldData (_T("IDX_TOC_KEYWORD") ,_T("STRING"),sBody) ;
		}
		//=====================================================
		pTempMgr->AddDBFieldData (_T("TOC_CREATE_DATE") ,_T("STRING"),v_TOC_CREATE_DATE) ;
		pTempMgr->AddDBFieldData (_T("TOC_YN"),_T("STRING"),_T("Y"));
		//pTempMgr->AddDBFieldData (_T("TOC_USE_LIMIT_CODE") ,_T("STRING"),v_TOC_USE_LIMIT_CODE) ;
		pTempMgr->AddDBFieldData (_T("CONTENTS_KEY") ,_T("NUMERIC"),m_strConKey ) ;
		
		ids =   pTempMgr->AddDBFieldData (_T("SPECIES_KEY") ,_T("NUMERIC"),v_SPECIES_KEY  ) ;
		//	ids =   pTempMgr->AddDBFieldData (_T("SE_SHELF_CODE") ,_T("STRING"),v_SE_SHELF_CODE) ;
		ids =	pTempMgr->AddDBFieldData (_T("CLASS_NO") ,_T("STRING"),v_CLASS_NO) ;
		ids =	pTempMgr->AddDBFieldData (_T("BOOK_CODE") ,_T("STRING"),v_BOOK_CODE) ;
		ids =	pTempMgr->AddDBFieldData (_T("VOL_CODE_DISP") ,_T("STRING"),v_VOL_CODE_DISP) ;
		ids =	pTempMgr->AddDBFieldData (_T("TITLE_INFO") ,_T("STRING"),v_TITLE_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("PUB_INFO") ,_T("STRING"),v_PUB_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("PUB_YEAR_INFO") ,_T("STRING"),v_PUB_YEAR_INFO) ;
		//	ids =	pTempMgr->AddDBFieldData (_T("PLACE_INFO") ,_T("STRING"),v_PLACE_INFO) ;
		//	ids =	pTempMgr->AddDBFieldData (_T("SPECIES_CLASS") ,_T("STRING"),v_SPECIES_CLASS) ;
		
		pTempMgr->MakeInsertFrame (_T("IDX_BO_TOC_TBL"));
	}

    if(!m_Chorok.IsEmpty ())
	{

		sBody = m_Chorok ;

		MakeIndexKeyWord(sBody) ;		
		
		INT iCh = pIndex->GetIndex()->ConvertHanjaToHangul( sBody );	//한자 한글 변환
		if ( 0 != iCh ) 
		{			
			AfxMessageBox ( _T("ChangeHanjaToHangul( sBody ) error") );
			delete pIndex;
			return -1;
		}

		iCh = pIndex->GetIndex()->RemoveSpecialChar( sBody );	//한자 한글 변환
		if ( 0 != iCh ) 
		{			
			AfxMessageBox ( _T("RemoveSpecialChar( sBody ) error") );
			delete pIndex;
			return -1;
		}

		pTempMgr->InitDBFieldData () ;

		INT nGetByte;
		CString strTempData;
		strTempData = sBody;
		nGetByte = WideCharToMultiByte( CP_UTF8	, 
								0				,
								strTempData.GetBuffer(0), 
								-1				,
								NULL			, 
								0				,
								NULL			, 
								NULL      );
		
		// 목차일때			
		if( nGetByte > 4000 )
		{
			AfxMessageBox(_T("초록 KEYWORD의 길이가 너무 깁니다."));
		}
		else
		{
			pTempMgr->AddDBFieldData (_T("IDX_ABS_KEYWORD") ,_T("STRING"),sBody) ;
		}
		//=====================================================
		
		pTempMgr->AddDBFieldData (_T("ABS_CREATE_DATE") ,_T("STRING"),v_ABS_CREATE_DATE) ;   
		pTempMgr->AddDBFieldData (_T("ABS_YN"),_T("STRING"),_T("Y"));
		//pTempMgr->AddDBFieldData (_T("ABS_USE_LIMIT_CODE") ,_T("STRING"),v_ABS_USE_LIMIT_CODE) ;
		pTempMgr->AddDBFieldData (_T("ABSTRACTS_KEY") ,_T("NUMERIC"),m_strAbsKey  ) ;

		pTempMgr->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),v_SPECIES_KEY) ;

	}


	delete pIndex;
	return 0 ;
}

INT CBO_INERT_TABLE::MakeIndexKeyWord(CString &tmpValue)
{	
	TCHAR	*sData;

	sData = new TCHAR[200000*sizeof(TCHAR)];
	sData[0] = '\0';
    
	tmpValue.TrimLeft () ; tmpValue.TrimRight () ;

	INT first,end ;
	first = tmpValue.Find (_T("<body>")) ;
    end   = tmpValue.Find (_T("</body>"));

    tmpValue = tmpValue.Mid (first,end-first) ;
	first = tmpValue.Find (_T("\r\n")) ;
 
	tmpValue = tmpValue.Mid (first,tmpValue.GetLength ()-(first+2) ) ;

	_stprintf(sData, _T("%s"), tmpValue.GetBuffer(0));

	if (_tcsclen(sData) > 3900)
	{
		if( (_TUCHAR)sData[3900] >= 0x80 )
		{
			if( (_TUCHAR)sData[3901] >= 0x80 )
			{
				sData[3902] = '\0';
			}
			else
			{
				sData[3901] = '\0';
			}
		}
		else
		{
			sData[3900] = '\0';
		}
		tmpValue.Format(_T("%s"), sData);
	}


	delete [] sData;
	return 0 ;
}