// CONTENTMGR_INDEX_SEARCH.cpp : implementation file
//

#include "stdafx.h"
//#include _T("contentsmgr_bo.h")
#include "CONTENTMGR_INDEX_SEARCH.h"
#include "..\..\..\공통\K2UP_Common\MakeSearchData.h"
#include "CommonDefine.h"
#include "sbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_INDEX_SEARCH dialog


CCONTENTMGR_INDEX_SEARCH::CCONTENTMGR_INDEX_SEARCH(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCONTENTMGR_INDEX_SEARCH::~CCONTENTMGR_INDEX_SEARCH()
{
	
}
BOOL CCONTENTMGR_INDEX_SEARCH::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}



VOID CCONTENTMGR_INDEX_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_INDEX_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_INDEX_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_INDEX_SEARCH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_INDEX_SEARCH message handlers

BOOL CCONTENTMGR_INDEX_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_CONTENTMGR_INDEX_SEARCH"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}
    
	m_CurrentCM = _T("CM_CONTENTMGR_INDEX_SEARCH") ;
    m_ParentCM  = _T("CM_CONTENTMGR_MAIN") ;
	AllControlDisplay (m_CurrentCM,-1) ;

	AddColorControl(IDC_eCON_INDEX,_T(""),_T(""),'E');
    AddColorControl(IDC_eABS_INDEX,_T(""),_T(""),'E'); 
	
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CCONTENTMGR_INDEX_SEARCH::OnOK() 
{
	// TODO: Add extra validation here
	CMakeSearchData *pMakeData = NULL;
	 pMakeData = new CMakeSearchData(this);


	CString strUseLimit;
	CString strContents;
	CString strAbstracts;
	CString strConCDateF,strConCDateT;
	CString strConUDateF,strConUDateT;
	CString strAbsCDateF,strAbsCDateT;
	CString strAbsUDateF,strAbsUDateT;
	CString strQuery;
	BOOL	bEqualSearch;
	 
    CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) 
	{
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}

	pCM->GetControlMgrData (_T("목차_이용제한구분"),strUseLimit ,-1) ;
	pCM->GetControlMgrData (_T("목차색인어"),strContents ,-1) ;
	pCM->GetControlMgrData (_T("초록색인어"),strAbstracts ,-1) ;
	pCM->GetControlMgrData (_T("목차입력시작"),strConCDateF ,-1) ;
	pCM->GetControlMgrData (_T("목차입력끝"),strConCDateT ,-1) ;
	pCM->GetControlMgrData (_T("목차수정시작"),strConUDateF ,-1) ;
	pCM->GetControlMgrData (_T("목차수정끝"),strConUDateT ,-1) ;
	pCM->GetControlMgrData (_T("초록입력시작"),strAbsCDateF ,-1) ;	
	pCM->GetControlMgrData (_T("초록입력끝"),strAbsCDateT ,-1) ;
	pCM->GetControlMgrData (_T("초록수정시작"),strAbsUDateF ,-1) ;
	pCM->GetControlMgrData (_T("초록수정끝"),strAbsUDateT ,-1) ;


	
	CString pszFields;
	CString pszCollection;


	INT nTotalCnt;
	CESL_DataMgr* pBaseDataMgr ;


	CString m_TOC_INDEX_DM ;
     
	m_TOC_INDEX_DM = _T("DM_CONTENTMGR_INDEX") ;

	pBaseDataMgr = FindDM(m_TOC_INDEX_DM );
	if(pBaseDataMgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}

    
	if(!strUseLimit.IsEmpty ())
	{
		strQuery += _T("TOC_USE_LIMIT_CODE = '") + strUseLimit + _T("'") ;
	}

 
	strContents.TrimLeft () ;
	strContents.TrimRight () ;
    
	CString sWhereTitle ;

	if(!strContents.IsEmpty ())
	{    
	
		INT iCh = pMakeData->ChangeHanjaToHangul( strContents );	//한자 한글 변환
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strContents ) error") );
			return ;
		}			
		
		if ( strContents.Left(1) == _T("\"") && strContents.Right(1) == _T("\"") ) bEqualSearch = TRUE;
		else bEqualSearch = FALSE;		

		iCh = pMakeData->GetKeyWord( strContents );				//공백, 특수문자 제거
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strContents ) error") );
			return ;
		}

		// 2005.12.12  ADD BY PDJ
		// 우절단 검색
		if ( bEqualSearch == FALSE )
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_TOC_KEYWORD,'*%s*',NULL)>0") , strContents );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BOTOC_IDX_TOC_KEYWORD, '%s*')"), strContents );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) 
			{
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return;
			}
		}
		else 
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_TOC_KEYWORD,'%s',NULL)>0") , strContents );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BOTOC_IDX_TOC_KEYWORD, '%s')"), strContents );				
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) 
			{
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return;
			}
		}	

		if(strQuery.IsEmpty ())
		{    
			strQuery = strQuery + _T(" ") + sWhereTitle ;
		}
		else
		{
            strQuery = strQuery + _T(" AND ") + sWhereTitle ;
			//strQuery += _T(" AND IDX_TOC_KEYWORD = '")+strContents+ _T("'") ; 
		}
	}

    if(!strAbstracts.IsEmpty ())
	{    
	
		INT iCh = pMakeData->ChangeHanjaToHangul( strAbstracts );	//한자 한글 변환
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strAbstracts ) error") );
			return ;
		}			
		if ( strAbstracts.Left(1) == _T("\"") && strAbstracts.Right(1) == _T("\"") ) bEqualSearch = TRUE;
		else bEqualSearch = FALSE;		

		iCh = pMakeData->GetKeyWord( strAbstracts );				//공백, 특수문자 제거
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strAbstracts ) error") );
			return ;
		}

		//sWhereTitle.Format( _T("CATSEARCH(IDX_TOC_KEYWORD,'*%s*',NULL)>0") , strContents );
		
		// 2005.12.12  ADD BY PDJ
		// 우절단 검색
		if ( bEqualSearch == FALSE )
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_ABS_KEYWORD,'*%s*',NULL)>0") , strAbstracts );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BOTOC_IDX_ABS_KEYWORD, '%s*')"), strAbstracts );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) 
			{
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return;
			}
		}
		else 
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_ABS_KEYWORD,'%s',NULL)>0") , strAbstracts );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BOTOC_IDX_ABS_KEYWORD, '%s')"), strAbstracts );				
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) 
			{
				AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
				return;
			}
		}		

		//strQry = strQry + _T(" ") + sWhereTitle;

		if(strQuery.IsEmpty ())
		{    
			strQuery = strQuery + _T(" ") + sWhereTitle ;
		}
		else
		{
            strQuery = strQuery + _T(" AND ") + sWhereTitle ;
			//strQuery += _T(" AND IDX_TOC_KEYWORD = '")+strContents+ _T("'") ; 
		}
	}

	//===================================================
	//2009.05.20 UPDATE BY PJW : 나머지 날짜들에대한 처리도 해준다.
//	if(!strConCDateF.IsEmpty ())
//	{
//		if(strConCDateT .IsEmpty ())
//		{
//			strQuery+= _T(" AND TOC_CREATE_DATE >= to_date('")+strConCDateF +_T("','yyyy/mm/dd')") ;
//		}
//		else
//		{
//			if(!strQuery.IsEmpty ())
//			{
//				strQuery +=_T(" AND (TOC_CREATE_DATE between to_date( '")+strConCDateF +_T("', 'yyyy/mm/dd' ) and to_date( '")+strConCDateT +_T("', 'yyyy/mm/dd' ) )");
//			}
//			else
//			{
//	            strQuery +=_T(" (TOC_CREATE_DATE between to_date( '")+strConCDateF +_T("', 'yyyy/mm/dd' ) and to_date( '")+strConCDateT +_T("', 'yyyy/mm/dd' ) )");
//			}
//		
//		}
//	}
	if(!strConCDateF.IsEmpty ())
	{
		if(strConCDateT .IsEmpty ())
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery+= _T(" AND TOC_CREATE_DATE >= TO_DATE('")+strConCDateF +_T("','YYYY/MM/DD')") ;
			}
			else
			{
				strQuery+= _T(" TOC_CREATE_DATE >= TO_DATE('")+strConCDateF +_T("','YYYY/MM/DD')") ;
			}
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (TOC_CREATE_DATE BETWEEN TO_DATE( '")+strConCDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strConCDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
			else
			{
	            strQuery +=_T(" (TOC_CREATE_DATE BETWEEN TO_DATE( '")+strConCDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strConCDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}		
		}
	}

	if(!strConUDateF.IsEmpty ())
	{
		if(strConUDateT .IsEmpty ())
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery+= _T(" AND TOC_UPDATE_DATE >= TO_DATE('")+strConUDateF +_T("','YYYY/MM/DD')") ;
			}
			else
			{
				strQuery+= _T(" TOC_UPDATE_DATE >= TO_DATE('")+strConUDateF +_T("','YYYY/MM/DD')") ;
			}			
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (TOC_UPDATE_DATE BETWEEN TO_DATE( '")+strConUDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strConUDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
			else
			{
	            strQuery +=_T(" (TOC_UPDATE_DATE BETWEEN TO_DATE( '")+strConUDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strConUDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
		
		}
	}

	if(!strAbsCDateF.IsEmpty ())
	{
		if(strAbsCDateT .IsEmpty ())
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery+= _T(" AND ABS_CREATE_DATE >= TO_DATE('")+strAbsCDateF +_T("','YYYY/MM/DD')") ;
			}
			else
			{
				strQuery+= _T(" ABS_CREATE_DATE >= TO_DATE('")+strAbsCDateF +_T("','YYYY/MM/DD')") ;
			}
			
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (ABS_CREATE_DATE BETWEEN TO_DATE( '")+strAbsCDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strAbsCDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
			else
			{
	            strQuery +=_T(" (ABS_CREATE_DATE BETWEEN TO_DATE( '")+strAbsCDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strAbsCDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
		
		}
	}

	if(!strAbsUDateF.IsEmpty ())
	{
		if(strAbsUDateT .IsEmpty ())
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery+= _T(" AND ABS_UPDATE_DATE >= TO_DATE('")+strAbsUDateF +_T("','YYYY/MM/DD')") ;
			}
			else
			{
				strQuery+= _T(" ABS_UPDATE_DATE >= TO_DATE('")+strAbsUDateF +_T("','YYYY/MM/DD')") ;
			}
			
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (ABS_UPDATE_DATE BETWEEN TO_DATE( '")+strAbsUDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strAbsUDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
			else
			{
	            strQuery +=_T(" (ABS_UPDATE_DATE BETWEEN TO_DATE( '")+strAbsUDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strAbsUDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
		
		}
	}
	//===================================================


	BEGIN_SBL();
	pBaseDataMgr->RefreshDataManager (strQuery) ;
    nTotalCnt = pBaseDataMgr->GetRowCount () ;
	END_SBL(nTotalCnt);
	
	if( nTotalCnt > 2000 ) 
	{
		CString strMsg ;
		strMsg.Format (_T("검색 결과가 너무 많습니다. ( 총 %d 건 )"),nTotalCnt);
		AfxMessageBox(strMsg) ;
		return ;
	}
	INT ids ;
    
	if( nTotalCnt > 0 )
	{     
		 CESL_DataMgr *pMainDM = FindDM(_T("DM_BO_CONTENTMGR_SPECIES")) ;
		 if(pMainDM == NULL) {
			 AfxMessageBox(_T("FindDM() Error~!")) ;
			 return ;
		 }
         
         ids = FromIndexToMainDM(pBaseDataMgr, pMainDM) ;
         if(ids <0) 
		 {
			 AfxMessageBox(_T("FromIndexToMainDM() Error~!"));
			 return ;
		 }

 
	}
	else
	{
		AfxMessageBox(_T("검색된 내용이 없습니다."));
		return ;
	}

    AllControlDisplay (m_ParentCM,-1) ;


	CString str ;//= _T("여기에 이렇게 메시지를 보여줄 수 있습니다.");
	str.Format (_T("%d 건 검색 되었습니다."),nTotalCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));
     
	delete pMakeData ;

	return;
//ERR_RETURN:
//	AfxMessageBox(_T("검색에 실패하였습니다."));
//	CDialog::OnOK();
}



INT CCONTENTMGR_INDEX_SEARCH::FromIndexToMainDM(CESL_DataMgr *pIndexDM, CESL_DataMgr *pMainDM)
{
	INT nRowCnt ;
    nRowCnt = pIndexDM->GetRowCount () ;
    
	CString sSpecies_Key ;
	CString sTemp ;
    CString sWhere ;
	//===================================================
	//2010.01.06 UPDATE BY PJW : 1000개 이상일경우는 나누어서 한다.
//  for(INT i =0 ; i < nRowCnt ;i++) 
//	{
//	   pIndexDM->GetCellData (_T("SPECIES_KEY"),i,sSpecies_Key ) ;
//	   if ( sSpecies_Key == _T("") ) continue;
//
//	   if(!sTemp.IsEmpty ())
//	   {
//			sTemp+= _T(",") + sSpecies_Key ; 
//
//	   }
//	   else
//	   {
//			sTemp += sSpecies_Key ;
//	   }
//       
//	   sSpecies_Key.Empty () ;
//
//    }
//
//	sWhere = _T("I.REC_KEY IN( ") + sTemp + _T(")") ;

	sWhere = _T("");
	for(INT i =0 ; i < nRowCnt ;i++) 
	{
	   pIndexDM->GetCellData ( _T("SPECIES_KEY"), i, sSpecies_Key ) ;
	   if ( sSpecies_Key == _T("") ) continue;

	   if(!sTemp.IsEmpty ())
	   {
		   if( i%1000 == 999)
		   {
			   if( sWhere.IsEmpty() )
			   {
				   sWhere = _T("I.REC_KEY IN( ") + sTemp + _T(",") + sSpecies_Key + _T(" )");
			   }
			   else
			   {
					sWhere += _T(" OR I.REC_KEY IN( ") + sTemp + _T(",") + sSpecies_Key + _T(" )");
			   }
			   sTemp = _T("");
		   }
		   else
		   {
			   sTemp+= _T(",") + sSpecies_Key ; 
		   }
	   }
	   else
	   {
			sTemp = sSpecies_Key ;
	   }
       
	   sSpecies_Key.Empty () ;
    }

	if ( !sTemp.IsEmpty() )
	{
		if( sWhere.IsEmpty() )
		{
			sWhere = _T("I.REC_KEY IN( ") + sTemp + _T(" )");
		}
		else
		{
			sWhere += _T(" OR I.REC_KEY IN( ") + sTemp + _T(" )");
		}	
	}

	sWhere = _T("( ") + sWhere + _T(" )");
	//===================================================
	

    pMainDM->RefreshDataManager (sWhere) ;
    
	return 0 ;
}
//===================================================
//2009.12.14 ADD BY PJW : 
//	CString strTemp;
// 	CString strRecKey;
// 	CString strBookKeyTempList;
// 	CString strQueryWhere;
// 	INT nCnt;
// 	INT nLocation;
// 	INT nStartLocation;
//	INT i ; 
//	strBookKeyTempList = strRecKeyList;
//	strBookKeyTempList.Replace(_T("'"), _T(""));
//	nCnt = 0;
//	nLocation = 0;
//	strQueryWhere.Format(_T(""));
//	while( 1 )
//	{
//		nLocation = strBookKeyTempList.Find(_T(","), nLocation+1);
//		++nCnt;
//		if( 0 > nLocation )
//		{
//			break;			
//		}
//	}
//	if ( (nCnt % 1000) == 0 )
//	{
//		nCnt = (nCnt/1000);
//	}
//	else
//	{
//		nCnt = (nCnt/1000)+1;
//	}
//	nStartLocation = strBookKeyTempList.GetLength()/nCnt;
//
//	for( i = 0 ; i < nCnt ; i++)
//	{
//		if( i == (nCnt -1) )
//		{
//			strRecKey = strBookKeyTempList;
//		}
//		else
//		{
//			nLocation = strBookKeyTempList.Find(_T(","), nStartLocation);
//			strRecKey = strBookKeyTempList.Mid(0, nLocation );
//			strTemp.Format(_T("%s,"), strRecKey);
//			strBookKeyTempList.Replace( strTemp, _T("") );
//		}
//	
//		if( strQueryWhere.IsEmpty() )
//		{
//			strQueryWhere.Format(_T("%s IN (%s)"), strKeyName, strRecKey);
//		}
//		else
//		{
//			strTemp.Format(_T("%s OR %s IN( %s )"), strQueryWhere,strKeyName, strRecKey );
//			strQueryWhere = strTemp;
//		}
//	}
//	strRecKeyList.Format(_T("SELECT %s FROM %s WHERE %s"), strKeyName, strTableName,strQueryWhere);
//
//	return 0;
//===================================================

BOOL CCONTENTMGR_INDEX_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) 
	{
		OnOK() ;
		return TRUE ;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CCONTENTMGR_INDEX_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
