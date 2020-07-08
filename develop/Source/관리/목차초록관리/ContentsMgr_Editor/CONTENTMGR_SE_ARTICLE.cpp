// CONTENTMGR_SE_ARTICLE.cpp : implementation file
//

#include "stdafx.h"
#include "CONTENTMGR_SE_ARTICLE.h"
#include "CONTENTMGR_SE_KISA_EDIT.h"
#include "CONTENTMGR_SE_EDIT.h"
#include "..\..\..\공통\K2UP_Common\MakeSearchData.h"

#include "..\ContentsMgr_SE\KisaMarcImport.h"
#include <direct.h>
#include "..\..\..\단행\정리\BO_CAT_PROC_KEYWORD\BO_CAT_PROC_KEYWORD_0000.h"

//마크 고정길이 
//=====================================================
//2009.01.29 UPDATE BY PJW : 고정길이 통합마크로 변경
// #include "..\..\..\공통\CAT_FIXED_FIELD\Cat008FixedField_TX.h"
#include "..\..\..\공통\CAT_UNION_FIXED_FIELD\DLG_008_BK.h"
//=====================================================

// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// 통합마크 개정 적용
/*//NEW------------------------------------------------------------------------*/
#include "..\..\..\공통\CAT_UNION_FIXED_FIELD\DLG_008_BK_1.h"
/*//END------------------------------------------------------------------------*/

#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//======================================================
//2004-07-15 이학중 
class CTemp {
public :
	BOOL m_bCheckUseLimitCode  ;
	INT  m_nCheckLOCFLAG   ;
	CString m_SPECIES_USE_LIMIT_CODE  ;
	CString m_BOOK_USE_LIMIT_CODE ;
};
CTemp _pVirsualClass ;
//======================================================



/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SE_ARTICLE dialog


CCONTENTMGR_SE_ARTICLE::CCONTENTMGR_SE_ARTICLE(INT nDialogType,CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_DlgType = nDialogType ;

    //=========================================================================
	//2004-07-15 이학중 
    
	_pVirsualClass.m_bCheckUseLimitCode = FALSE ;
	_pVirsualClass.m_nCheckLOCFLAG = -1 ;
	_pVirsualClass.m_SPECIES_USE_LIMIT_CODE =_T("");
	_pVirsualClass.m_BOOK_USE_LIMIT_CODE = _T("") ;
	
	m_str008					= _T("");
	m_bFIXED_FIELD				= 0;

	//=========================================================================
}

CCONTENTMGR_SE_ARTICLE::~CCONTENTMGR_SE_ARTICLE()
{
	
}

BOOL CCONTENTMGR_SE_ARTICLE::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}



/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SE_ARTICLE message handlers

VOID CCONTENTMGR_SE_ARTICLE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CREATE_ARTICLE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_SE_ARTICLE, CDialog)
	//{{AFX_MSG_MAP(CSE_CREATE_ARTICLE)
	ON_BN_CLICKED(IDC_bKisaMokcha, OnbKisaMokcha)
	ON_BN_CLICKED(IDC_bMarcImport, OnbMarcImport)
	ON_BN_CLICKED(IDC_bMarcExport, OnbMarcExport)
	ON_BN_CLICKED(IDC_bModify, OnbModify)
	ON_BN_CLICKED(IDC_bKeyword2, OnbKeyword2)
	ON_BN_CLICKED(IDC_bFixedField2, OnbFixedField)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CREATE_ARTICLE message handlers

BOOL CCONTENTMGR_SE_ARTICLE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	INT ids ;
    
    if(InitESL_Mgr(_T("SM_CONTENTMGR_CREATE"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}

	m_CurrentCM = _T("CM_CONTENTMGR_CREATE") ;
    m_CurrentGrid = _T("MainGrid") ;

    m_Se_Article_tbl = _T("DM_CONTENTMGR_SE_ARTICLE") ;
    
	if(m_DlgType == 0) 
	{   
		
        GetDlgItem(IDC_bKisaMokcha)->ShowWindow(FALSE) ;
		GetDlgItem(IDOK)->ShowWindow(TRUE) ;
		ids = GetFieldValue (m_Mokcha) ;
	}
    else 
	{
		GetDlgItem(IDOK)->ShowWindow(FALSE) ;
		GetDlgItem(IDC_bKisaMokcha)->ShowWindow(TRUE) ;
		KisaViewGrid() ;
	}
     
	// Marc Editor SubClassing
	if (!m_marcEditor.SubclassDlgItem(IDC_MarcRichEdit, -1, this))
	{
		AfxMessageBox(_T("Subclass Marc Editor Failed"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return FALSE;
	}

    m_pGrid = (CESL_Grid*)pCM->FindControl (m_CurrentGrid) ;
	if(m_pGrid == NULL) {
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return FALSE;
	}

	//마크 값 초기화
	if (m_pMarc)
		m_pMarc = NULL;
	m_pMarc = new CMarc;

	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);
	m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);

	Display();

	OnClickGridCONTENTSE();

	m_pGrid->SetReverse(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCONTENTMGR_SE_ARTICLE::GetMokcha(CString strTemp)
{
	m_Mokcha = strTemp ;
	return 0 ;
}


INT CCONTENTMGR_SE_ARTICLE::GetFieldValue(CString strTemp)
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
    	SetFieldData(i , ArrayRecord.GetAt (i)) ;
      }

	}
    else
	{
		AfxMessageBox(_T("생성할 기사가 없습니다"));
		return -1 ;
	}

	return 0;
}


INT CCONTENTMGR_SE_ARTICLE::MakeArrayList( CString sLine , CString sDelimiter , CArray <CString,CString>&asArray )
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


INT CCONTENTMGR_SE_ARTICLE::SetFieldData( INT RowIndex , CString sData ) 
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
				/*
				nextIndex = sData.Find (_T("[!--")) ;
				sTitle = sData.Mid (0,nextIndex) ;
				sData = sData.Mid (nextIndex+3,sData.GetLength ()-nextIndex) ;
				sKisaControl_No = sData ;
				*/
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
				/*
				nextIndex = sData.Find (_T("[!--")) ;
				sTitle = sData.Mid (0,nextIndex) ;
				sData = sData.Mid (nextIndex+3,sData.GetLength ()-nextIndex) ;
				sKisaControl_No = sData ;
				*/
			}
			else{
			
			sPage = sData.Mid (0,nthirdIndex) ;
			sPage.TrimLeft () ,sPage.TrimRight () ;

			sKisaControl_No = sData.Mid (nthirdIndex+4,sData.GetLength ()-(nthirdIndex+7)) ;
			sKisaControl_No.TrimLeft () ,sKisaControl_No.TrimRight () ;
           
			}
		}

       /*
		nextIndex = sData.Find (_T("[!--")) ;
        sTitle = sData.Mid (0,nextIndex) ;
		sData = sData.Mid (nextIndex+3,sData.GetLength ()-nextIndex) ;
        sKisaControl_No = sData ;
	   */
	}
    else
	{
		   sTitle = sData.Mid(0,nIndex) ;
		   sTitle.TrimLeft () ; sTitle.TrimRight () ;
		   sData = sData.Mid (nIndex+1,sData.GetLength () - nIndex);
       
		   nextIndex = sData.Find (_T("=")) ;
		   if(nextIndex <0)
		   {	      		
				nthirdIndex = sData.Find(_T("[!--")) ;
				if(nthirdIndex <0 )
				{
					sAuthor = sData ;
					sAuthor.TrimLeft () ,sPage.TrimRight () ;
					/*
					sPage = sData.Mid (0,nextIndex) ;
					sPage.TrimLeft () ,sPage.TrimRight () ;

					sKisaControl_No = sData.Mid (nextIndex+3,sData.GetLength ()-nextIndex) ;
					sKisaControl_No.TrimLeft () ,sKisaControl_No.TrimRight () ;
					*/
				}
				else
				{

					sAuthor = sData.Mid (0,nthirdIndex) ;
					sAuthor.TrimLeft () ,sAuthor.TrimRight () ;

					sKisaControl_No = sData.Mid (nthirdIndex+4,sData.GetLength ()-(nthirdIndex+7)) ;
					sKisaControl_No.TrimLeft () ,sKisaControl_No.TrimRight () ;
				}

		   }
		   else
		   {

			    sAuthor = sData.Mid (0,nextIndex) ;
			    sAuthor.TrimLeft () ; sAuthor.TrimRight () ;
			    sData = sData.Mid(nextIndex+1,sData.GetLength () - nextIndex) ;
				nthirdIndex = sData.Find(_T("[!--")) ;
				if(nthirdIndex <0 )
				{
				sPage = sData ;
				sPage.TrimLeft () ,sPage.TrimRight () ;
				/*
				sPage = sData.Mid (0,nextIndex) ;
				sPage.TrimLeft () ,sPage.TrimRight () ;

				sKisaControl_No = sData.Mid (nextIndex+3,sData.GetLength ()-nextIndex) ;
				sKisaControl_No.TrimLeft () ,sKisaControl_No.TrimRight () ;
				*/
				}
				else
				{

				sPage = sData.Mid (0,nthirdIndex) ;
				sPage.TrimLeft () ,sPage.TrimRight () ;

				sKisaControl_No = sData.Mid (nthirdIndex+4,sData.GetLength ()-(nthirdIndex+7)) ;
				sKisaControl_No.TrimLeft () ,sKisaControl_No.TrimRight () ;
				}
		   } 
		  
	   
	  }

    SetGridData(RowIndex,sTitle,sKisaControl_No,sAuthor,sPage) ;

	return 0 ;
}

INT CCONTENTMGR_SE_ARTICLE::SetGridData(INT RowIndex , CString strTitle, CString strKisaControl_No ,CString strAuthor ,CString strPage )
{
	
	CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -1;
	}

    m_pGrid = (CESL_Grid*)pCM->FindControl (m_CurrentGrid) ;
	if(m_pGrid == NULL) {
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return -2 ;
	}

    
	m_pGrid->SetAt (RowIndex,1,strKisaControl_No) ;
    m_pGrid->SetAt (RowIndex,2,strTitle) ;
	m_pGrid->SetAt (RowIndex,3,strAuthor) ;
	m_pGrid->SetAt (RowIndex,4,strPage) ;

	///////////////////////////////////////////////////////////
    //기사 CONTROL_NO 번호가 있을경우 _T("V") 체크로 나타나게 한다.
    ///////////////////////////////////////////////////////////
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.11.13 이학중 수정 
	CESL_DataMgr *pArticleDM_Mgr_Temp = FindDM(_T("DM_CONTENTMGR_SE_ARTICLE_TEMP")) ;
    
	CESL_DataMgr *pDM = FindDM(_T("DM_CONTENTMGR_SE_ARTICLE"));
	
	
		

    CString sWhere ;
	CString strCount ;
//
//	pDM->SetCellData(_T("기사명"), strTitle, RowIndex );
//	pDM->SetCellData(_T("저자"), strAuthor, RowIndex );
//	pDM->SetCellData(_T("페이지"), strPage, RowIndex );

	sWhere.Format ( _T(" ARTICLE_CONTROL_NO = '%s'"),strKisaControl_No) ;
	

	if(strKisaControl_No != _T(""))
	{
		pArticleDM_Mgr_Temp->RefreshDataManager (sWhere);
		
		if( pArticleDM_Mgr_Temp->GetRowCount () >0) 
		{
			m_pGrid->SetAt (RowIndex,0,_T("V")) ;
			pDM->InsertRow (-1);
			INT nIdx ;
			INT nRow = pDM->GetRowCount ();
			nIdx = nRow-1 ;
			CString temp ;
			for(INT i =0 ; i <pDM->RefList.GetCount ();i++)
			{
				pArticleDM_Mgr_Temp->GetCellData (0,i,temp);
				pDM->SetCellData (nIdx,i,temp);
			}
//			pDM->SetCellData(_T("기사명"), strTitle, RowIndex );
//			pDM->SetCellData(_T("저자"), strAuthor, RowIndex );
//			pDM->SetCellData(_T("페이지"), strPage, RowIndex );
//			CString strTmp;
//			pArticleDM_Mgr_Temp->GetCellData(_T("ARTICLE"), RowIndex, strTmp);
//			pDM->SetCellData(_T("ARTICLE"), strTmp, RowIndex );
//			
		}
		else
		{
			pDM->InsertRow (-1);
//			INT nRow = pDM->GetRowCount ();
//			nIdx = nRow-1 ;
//			CString temp ;
//			for(INT i =0 ; i <pDM->RefList.GetCount ();i++)
//			{
//				pArticleDM_Mgr_Temp->GetCellData (0,i,temp);
//				pDM->SetCellData (nIdx,i,temp);
//			}

			pDM->SetCellData(_T("기사명"), strTitle, RowIndex );
			pDM->SetCellData(_T("저자"), strAuthor, RowIndex );
			pDM->SetCellData(_T("페이지"), strPage, RowIndex );

		}
		
	}
	else
	{
		    pArticleDM_Mgr_Temp->FreeData () ;
			pDM->InsertRow (-1);
//			INT nRow = pDM->GetRowCount ();
//			nIdx = nRow-1 ;
//			CString temp ;
//			for(INT i =0 ; i <pDM->RefList.GetCount ();i++)
//			{
//				pArticleDM_Mgr_Temp->GetCellData (0,i,temp);
//				pDM->SetCellData (nIdx,i,temp);
//			}
			pDM->SetCellData(_T("기사명"), strTitle, RowIndex );
			pDM->SetCellData(_T("저자"), strAuthor, RowIndex );
			pDM->SetCellData(_T("페이지"), strPage, RowIndex );
	}

	m_pGrid->m_nRealRowCount = RowIndex+1;

	return 0 ;
}

/////////////////////////////////////////////////////////////////////////////
// [FUNC. DESC] 기사 저장
// [RETURN VALUES]
// [PARAMETERS]
// [MINI SPEC]
/////////////////////////////////////////////////////////////////////////////
VOID CCONTENTMGR_SE_ARTICLE::OnOK() 
{
	// TODO: Add extra validation here
    
	CESL_DataMgr *pDM = FindDM(m_Se_Article_tbl) ;
	if(pDM == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

	INT ids ;
	INT nRowIndex, nRowCnt;
	INT nMode;
    
	CString sArticle_Key;   
	CString strKisaControl_No , strTitle , strAuthor , strPage ;
    CString Current_Date ;
	CString strStreamMarc;

	CTime t = CTime::GetCurrentTime () ;
	Current_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;
     
	ids = m_pGrid->SelectMakeList () ;
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SelectMakeList Error!"));
		return;
	}
	nRowCnt = m_pGrid->SelectGetCount () ;

	ids = pDM->StartFrame ();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame Error!"));
		return;
	}

	nRowIndex = m_pGrid->SelectGetHeadPosition() ;
	for(INT i = 0 ; i < nRowCnt ; i++)
	{
// 		nRowIndex = m_pGrid->SelectGetIdx () ;
		// 그리드 데이타 가져 오기 
		m_pGrid->GetAt (nRowIndex,1,strKisaControl_No) ;
	    
/*		if(strKisaControl_No.IsEmpty ()) //입력으로 간주
		{
			strKisaControl_No = MakeKisaControl_No(nRowIndex) ;
			nMode = 1; 
		}
	    else // 수정으로 간주
		{   
            m_pGrid->SelectGetNext (); 
			nMode = -1;
			continue ;
		}
*/
		pDM->GetCellData(_T("기사키"), i, sArticle_Key );
		if ( sArticle_Key.IsEmpty() )
		{
			// 기사키가 없으면 INSERT로 판단
			pDM->MakeRecKey (sArticle_Key) ;
			strKisaControl_No = MakeKisaControl_No(i) ;
			nMode = -1;
		}
		else
		{
			// 기사키가 있으면 UPDATE로 판단.
			pDM->GetCellData(_T("기사제어번호"), i, strKisaControl_No);
			nMode = 1;			
		}

		m_pGrid->GetAt (nRowIndex,2,strTitle) ;
		m_pGrid->GetAt (nRowIndex,3,strAuthor) ;
		m_pGrid->GetAt (nRowIndex,4,strPage) ;
	
		pDM->InitDBFieldData () ;

		if( nMode == -1 )
		{
			pDM->MakeRecKey (sArticle_Key) ;
        	
			pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),sArticle_Key);
			pDM->AddDBFieldData (_T("CONTENTS_KEY"),_T("NUMERIC"),m_Contents_Key) ;
			pDM->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),m_Vol_Key );
			pDM->AddDBFieldData (_T("ARTICLE_CONTROL_NO"),_T("STRING"),strKisaControl_No );
			pDM->AddDBFieldData (_T("ARTICLE_NAME"),_T("STRING"),strTitle);
			pDM->AddDBFieldData (_T("ARTICLE_AUTHOR"),_T("STRING"),strAuthor);
			pDM->AddDBFieldData (_T("START_PAGE"),_T("STRING"),strPage);
			pDM->AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),Current_Date);
			pDM->AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),Current_Date);
			pDM->AddDBFieldData (_T("ARTICLE_APPLY_YN"),_T("STRING"),_T("Y"));
        
			//Marc 생성
			CString strStreamMarc;
			if ( pDM->GetCellData(_T("ARTICLE"), nRowIndex ).IsEmpty() )
			{
				strStreamMarc = MakeStreamMarc( strKisaControl_No, strTitle, strAuthor, strPage, m_Vol_Key, m_Contents_Key );
			}
			else 
			{
				strStreamMarc = MakeStreamMarc_Ex( pDM->GetCellData(_T("ARTICLE"), nRowIndex ), strKisaControl_No );
			}
			
			pDM->AddDBFieldData( _T("ARTICLE"), _T("STRING"), strStreamMarc );

			ids = pDM->MakeInsertFrame (_T("SE_ARTICLE_TBL"));
			if( ids < 0 )
			{
				AfxMessageBox(_T("MakeInserFrame Error!"));
				return;
			}

			ids = Insert_Se_IndexTable(pDM,strStreamMarc,sArticle_Key,strKisaControl_No,strTitle,strAuthor,strPage,m_Vol_Key,m_Contents_Key);
			if(ids == 100)
			{
				AfxMessageBox(_T("Insert_Se_IndexTable() Error~!"));
				pDM->EndFrame () ;
				return ;
			}
		}
		else
		{
			pDM->GetCellData(_T("ARTICLE"), i, strStreamMarc );
			
			m_pInfo->pMarcMgr->Decoding (strStreamMarc , m_pMarc ) ;

			m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$a"), strTitle );
			pDM->AddDBFieldData (_T("ARTICLE_NAME"),_T("STRING"),strTitle);
			
			CString strAuthor;
			m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$d"), strAuthor );			
			pDM->AddDBFieldData (_T("ARTICLE_AUTHOR"),_T("STRING"),strAuthor);
			
			m_pGrid->GetAt (i,4,strPage) ;

			pDM->AddDBFieldData (_T("CONTENTS_KEY"),_T("NUMERIC"),m_Contents_Key) ;
			pDM->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),m_Vol_Key );
			pDM->AddDBFieldData (_T("ARTICLE_CONTROL_NO"),_T("STRING"),strKisaControl_No );
			pDM->AddDBFieldData (_T("START_PAGE"),_T("STRING"),strPage);			
			pDM->AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),Current_Date);
			pDM->AddDBFieldData (_T("ARTICLE_APPLY_YN"),_T("STRING"),_T("Y"));
			pDM->AddDBFieldData (_T("ARTICLE"), _T("STRING"), strStreamMarc );

			ids = pDM->MakeUpdateFrame(_T("SE_ARTICLE_TBL"), _T("REC_KEY"), _T("NUMERIC"), sArticle_Key );
			if( ids < 0 )
			{
				AfxMessageBox(_T("MakeUpdateFrame Error!"));
				return;
			}

			// 색인작업
			ids = Insert_Se_IndexTable( pDM, strStreamMarc, sArticle_Key, strKisaControl_No, strTitle, strAuthor, strPage, m_Vol_Key, m_Contents_Key, 1 );
			if(ids == 100)
			{
				AfxMessageBox(_T("Insert_Se_IndexTable() Error~!"));
				pDM->EndFrame () ;
				return;
			}
		}

// 		m_pGrid->SelectGetNext () ;
		nRowIndex = m_pGrid->SelectGetNext() ;
	}
       
    ids = pDM->SendFrame() ;
	if( ids < 0 )
	{
		AfxMessageBox(_T("SendFrame Error!"));
		return;
	}
	
	pDM->EndFrame();
	if( ids < 0 )
	{
		AfxMessageBox(_T("EndFrame Error!"));
		return;
	}

	// 제어번호 있는 ROW 선택 해제시 테이블에서 데이타 삭제 하기.
	INT nTotalRowCnt ;
	nTotalRowCnt = m_pGrid->GetCount () ;
    CArray<CString,CString> arrayTemp ;
    INT arrayCnt =0 ;

	pDM->StartFrame ();
	for(INT j =0 ; j<nTotalRowCnt ;j++)
	{
        if(m_pGrid->GetAt (j,0) == _T(""))
		{
            if(m_pGrid->GetAt (j,1) !=_T("")) 
			{   
				CString sQuery,result;
				sQuery.Format (_T("select rec_key from se_Article_tbl where ARTICLE_CONTROL_NO = '%s'"),m_pGrid->GetAt (j,1));
				pDM->GetOneValueQuery (sQuery,result,TRUE);
				
			//	arrayTemp.Add (m_pGrid->GetAt (j,1)) ;
                
				if(result == _T(""))
				{
					continue ;
				}

				pDM->InitDBFieldData ();
				pDM->MakeDeleteFrame (_T("SE_ARTICLE_TBL"),_T("REC_KEY"),_T("NUMERIC"),result) ;
			

				pDM->InitDBFieldData ();
				pDM->MakeDeleteFrame (_T("IDX_SE_TOC_TBL"),_T("REC_KEY"),_T("NUMERIC"),result) ;
			    
			    ///////////////////////////////////////////////////////////////////////
                // IDX_TMP_SE_TOC_TBL 입력 
				//////////////////////////////////////////////////////////////////////
				pDM->InitDBFieldData ();
                     
				CString strTmp_Key ;
				CString system_Date ;
				CTime t = CTime::GetCurrentTime ();
				system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());


				pDM->MakeRecKey (strTmp_Key) ;
				pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
				pDM->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),result);
				pDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("D"));
				pDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);

				ids = pDM->MakeInsertFrame (_T("IDX_TMP_SE_TOC_TBL"));
				if(ids<0) {
				 AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_SE_TOC_TBL) Error~!"));
				 return ;
				}
				
			}

		}
		else
		{
			continue;
		}
	}
	pDM->SendFrame ();
	pDM->EndFrame ();

	AfxMessageBox ( _T("저장하였습니다.") );
    ReMakeMokcha();
	m_bMakeMokcha = TRUE ;
//	CDialog::OnOK();
}

VOID CCONTENTMGR_SE_ARTICLE::OnCancel() 
{
	// TODO: Add extra cleanup here
    if(m_DlgType == 0 )
	{

		if(m_bMakeMokcha == TRUE) 
		{

			 CESL_ControlMgr *pCM = FindCM(m_CurrentCM );
			if(pCM == NULL) {
				AfxMessageBox(_T("FindCM() Error~!")) ;
				return ;
			}
			CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (m_CurrentGrid );
			if(pGrid == NULL)
			{
				AfxMessageBox(_T("FindControl() Error~!"));
				return ;
			}

			pGrid->SelectMakeList () ;
			pGrid->SelectGetCount () ;

            ((CCONTENTMGR_SE_EDIT *)this->pParentMgr)->GetKisaCount (pGrid->SelectGetCount ()) ; 
			CDialog::OnCancel();
		}
		else
		{
		    CESL_ControlMgr *pCM = FindCM(m_CurrentCM );
			if(pCM == NULL) {
				AfxMessageBox(_T("FindCM() Error~!")) ;
				return ;
			}
			CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (m_CurrentGrid );
			if(pGrid == NULL)
			{
				AfxMessageBox(_T("FindControl() Error~!"));
				return ;
			}

			pGrid->SelectMakeList () ;
			pGrid->SelectGetCount () ;

            ((CCONTENTMGR_SE_EDIT *)this->pParentMgr)->GetKisaCount (pGrid->SelectGetCount ()) ; 
			ReMakeMokcha() ;
			CDialog::OnCancel();
		}
    }
	else
    {
	   	CDialog::OnCancel();
	}

}

INT CCONTENTMGR_SE_ARTICLE::GetKeyValue (CString tmpContents_Key , CString tmpVol_Key ,CString  tmpControl_No) 
{
    m_Contents_Key = tmpContents_Key ;
	m_Vol_Key = tmpVol_Key ;
    m_Control_No = tmpControl_No ;
	return 0;
}

VOID CCONTENTMGR_SE_ARTICLE::OnbKisaMokcha() 
{
	// TODO: Add your control notification handler code here
	CCONTENTMGR_SE_KISA_EDIT  dlg(this);
	dlg.GetDialogType (FALSE); // 메뉴버튼에서 생성
	dlg.Set_Mgr (_T("CM_CONTENTMGR_CREATE"),_T("DM_CONTENTMGR_SE_ARTICLE"),_T("MainGrid"));
	dlg.DoModal () ;
}

CString CCONTENTMGR_SE_ARTICLE::MakeKisaControl_No(INT nIndex)
{
	//제어번호(12) + 권호키 (12) + 일련번호(3) 
	CString sControl_No ;
	CString sVol_Key ;
	CString sKisa_Control_No ;

    //DM_
	
	CESL_DataMgr *pDM = FindDM(m_Se_Article_tbl) ;
	if(pDM == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
	}
    CString strSQL ;
	//CString Max_Control_No ;
	
	if(Max_Control_No.IsEmpty ()) 
    {
		strSQL.Format (_T("select max(article_control_no) from se_Article_tbl where vol_key = %s"),m_Vol_Key) ;
		pDM->GetOneValueQuery (strSQL,Max_Control_No) ;
    } 
    
	INT max ;
	CString Temp ;

	if(Max_Control_No.IsEmpty () )
	{
	   sKisa_Control_No.Format (_T("%s%s%03d"),m_Control_No,m_Vol_Key,1) ;
       m_pGrid->SetAt (nIndex,1,sKisa_Control_No) ;

	   Max_Control_No = sKisa_Control_No ;
	   return sKisa_Control_No ;
	}
    else
	{
       max = _ttoi(Max_Control_No.Right(3)) ;
       
	   Max_Control_No = Max_Control_No.Mid (0,Max_Control_No.GetLength () - 3) ;

       Temp.Format (_T("%s"),Max_Control_No ) ;
	   
	   Max_Control_No.Format (_T("%s%03d"),Temp,max+1) ;
       
	   m_pGrid->SetAt (nIndex,1,Max_Control_No) ;

	   return Max_Control_No ;
	}

}


INT CCONTENTMGR_SE_ARTICLE::ReMakeMokcha()
{
//	CString sLast_Mokcha ;
    CString sDelimiter  = _T("\r\n") ;
	CString delimiter_1 = _T("/") ;
	CString delimiter_2 = _T("=") ;
	CString delimiter_3 = _T("[!--") ;
	CString delimiter_4 = _T("--]") ;
    
	sLast_Mokcha = _T("") ;
     
    INT nRow = m_pGrid->GetCount () ;
   
	CString sControlNo,sTitle,sAuthor,sPage ;
    
    //m_pGrid->SetAt (RowIndex,2,strTitle) ;
	//m_pGrid->SetAt (RowIndex,3,strAuthor) ;
	//m_pGrid->SetAt (RowIndex,4,strPage) ;

	for(INT i = 0 ; i < nRow ; i++)
	{
		
		m_pGrid->GetAt (i,1,sControlNo) ;
        m_pGrid->GetAt (i,2,sTitle) ;
        m_pGrid->GetAt (i,3,sAuthor) ;
		m_pGrid->GetAt (i,4,sPage) ;
       
        
		//sLast_Mokcha =_T("");
        
		if(!sTitle.IsEmpty ()) 
		{
			sLast_Mokcha += sTitle ;
		}
		
		if(!sAuthor.IsEmpty ())
		{
			sLast_Mokcha += _T("/") + sAuthor ;
		}

		if(!sPage.IsEmpty ())
		{
			sLast_Mokcha += _T("=") + sPage ;
		}

		if(!sControlNo.IsEmpty ())
		{
			if(m_pGrid->GetAt (i,0)!=_T(""))
			{
			   sLast_Mokcha += _T("[!--") + sControlNo + _T("--]") ;
			}
		
		}
		
		sLast_Mokcha += _T("\r\n") ;
		
	}
    
    

	return 0 ;
}




INT CCONTENTMGR_SE_ARTICLE::KisaViewGrid()
{
  /*
    m_CurrentCM = _T("CM_CONTENTMGR_CREATE") ;
    m_CurrentGrid = _T("MainGrid") ;
    m_Se_Article_tbl = _T("DM_CONTENTMGR_SE_ARTICLE") ;
  */
	
    CString m_ParentCM ;
	CString m_ParentDM ;
	CString m_ParentGrid ;
   
    m_ParentCM =_T("CM_CONTENTMGR_SE_MAIN");
    m_ParentDM =_T("DM_CONTENTMGR_SE_MAIN");
	m_ParentGrid = _T("MainGrid");

	CESL_ControlMgr * pMainCM = FindCM(m_ParentCM );
	if(pMainCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!"));
		return -1;
    }

	CESL_Grid *pMainGrid = (CESL_Grid*)pMainCM->FindControl (m_ParentGrid );
	if(pMainGrid == NULL) 
	{
		AfxMessageBox(_T("FindControl() Error~!"));
		return -2;
	}
    
	INT ids, nRowIndex ;
	pMainGrid->SelectMakeList ();
	nRowIndex = pMainGrid->SelectGetIdx () ;
 
    CString sVol_Key ;
	ids = GetDataMgrData (m_ParentDM ,_T("권호키"),sVol_Key,nRowIndex);
     

	CString sWhere ;

	sWhere.Format (_T("VOL_KEY = %s"),sVol_Key);

	CESL_DataMgr *pDM = FindDM(m_Se_Article_tbl ) ;
	if(pDM == NULL){
		AfxMessageBox(_T("FindDM() Error~!"));
		return -3 ;
	}

	pDM->RefreshDataManager (sWhere);
    INT RowCnt = pDM->GetRowCount () ;
	ControlDisplay (m_CurrentCM ,_T("MainGrid"),-1) ;


	
	return 0 ;
}


INT CCONTENTMGR_SE_ARTICLE::Insert_Se_IndexTable(CESL_DataMgr *pTmpDM,
												 CString strStreamMarc,
												 CString sArticle_Key,
												 CString sArticle_controlno,
												 CString sArticle,
												 CString sAuthor,
												 CString sPage_info,
												 CString sVol_Key,
												 CString sContent_Key,
												 INT nMode)
{
	
	CString nIndexRecKey;
	CString strIndexArticle ,strIndexAuthor;
	
	strIndexArticle = sArticle ;
	strIndexAuthor  = sAuthor ;
	
	CMakeSearchData *pMakeData = NULL; 
	pMakeData = new CMakeSearchData(this);
	nIndexRecKey = sArticle_Key;

	INT nRowIndex = m_pGrid->SelectGetIdx();
	
	//vol_use_limit,vol_key,article_toc_yn,
	//article_info,article_author_info,article_page_info
	//title_info,pub_info,pub_year_info,place_info,se_shelf_code,class_no,book_code,
	//vol_info
	
	CString sMainDMAlias = _T("DM_CONTENTMGR_SE_MAIN") ;
	CESL_DataMgr *pMainDM = FindDM(sMainDMAlias);
	if(pMainDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}
	
	/*
	CESL_ControlMgr *pMainCM = FindCM(_T("CM_CONTENTMGR_SE_MAIN"));
	if(pMainCM == NULL) {
	AfxMessageBox(_T("FindCM() Error~!"));
	return -2 ;
	}
	
	  CESL_Grid *pMainGrid = (CESL_Grid*)pMainCM->FindControl (_T("MainGrid"));
	  if(pMainGrid == NULL){
	  AfxMessageBox(_T("FindControl() Error~!"));
	  return -3;
	  }
	  
		
		  nMainIndex =  pMainGrid->SelectGetIdx () ; 
    */ 
	
	
	INT ids, nMainIndex ;
	nMainIndex = 0 ;
	
	CString sTitle ,sPublisher,sPub_year,sPlace_info,sSe_shelf_code,sClass_no,sBook_code ;
	
	ids = GetDataMgrData (sMainDMAlias,_T("서명"),sTitle,nMainIndex);
	ids = GetDataMgrData (sMainDMAlias,_T("발행자"),sPublisher,nMainIndex);
	ids = GetDataMgrData (sMainDMAlias,_T("발행년"),sPub_year,nMainIndex);
	ids = GetDataMgrData (sMainDMAlias,_T("PLACE_INFO"),sPlace_info,nMainIndex); 
	ids = GetDataMgrData (sMainDMAlias,_T("SE_SHELF_CODE"),sSe_shelf_code,nMainIndex);
	ids = GetDataMgrData (sMainDMAlias,_T("CLASS_NO"),sClass_no,nMainIndex); 
	ids = GetDataMgrData (sMainDMAlias,_T("BOOK_CODE"),sBook_code,nMainIndex); 
	
	//VOL_INFO 데이타 만들기 /////////////////////////////////////////////
	CString sVol_Title , sOther_Vol_Title ,sPub_Date;
	
	ids = GetDataMgrData (sMainDMAlias,_T("권호명"),sVol_Title,nMainIndex); 
	ids = GetDataMgrData (sMainDMAlias,_T("부차적권호명"),sOther_Vol_Title,nMainIndex); 
	ids = GetDataMgrData (sMainDMAlias,_T("발행일"),sPub_Date,nMainIndex); 
	

		 //==================================================================================================

	 //2004-07-09 이학중 
	 //1.USE_LIMIT_CODE = 종테이블.USE_LIMIT_CODE 
	 //2.VOL_USE_LIMIT_CODE = 책테이블.USE_LIMIT_CODE ( 해당종KEY로 검색후 같지 않을 경우 이어서 보이도록 )
     if(_pVirsualClass.m_bCheckUseLimitCode == FALSE) 
	 {
		 CString str_SPECIES_KEY ;
		 ids = GetDataMgrData (sMainDMAlias,_T("종키"),str_SPECIES_KEY,nMainIndex); 
		 
		 
		 //CString SPECIES_USE_LIMIT_CODE;
		 //CString BOOK_USE_LIMIT_CODE;
		 
		 CESL_DataMgr dm ;
		 dm.SetCONNECTION_INFO (m_pInfo->CONNECTION_INFO ) ;
		 CString strQuery ;
		 
		 
		 //1.SELECT USE_LIMIT_CODE FROM SE_SPECIES_TBL WHERE REC_KEY = str_SPECIES_KEY ;
		 strQuery.Format (_T("SELECT USE_LIMIT_CODE FROM SE_SPECIES_TBL WHERE REC_KEY = %s"),str_SPECIES_KEY);
		 dm.GetOneValueQuery (strQuery,_pVirsualClass.m_SPECIES_USE_LIMIT_CODE ) ;
		 
		 //2.SELECT USE_LIMIT_CODE FROM SE_BOOK_TBL WHERE SPECIES_KEY = str_SPECIES_KEY
		 strQuery.Format (_T("SELECT USE_LIMIT_CODE FROM SE_BOOK_TBL WHERE SPECIES_KEY = %s"),str_SPECIES_KEY) ;
		 dm.RestructDataManager (strQuery) ; 
		 
		 CString strTemp =_T("") ;
		 INT nPos ;
		 for(INT i = 0 ; i < dm.GetRowCount ();i++)
		 {
			 dm.GetCellData (i,0,strTemp) ;
			 if(_pVirsualClass.m_BOOK_USE_LIMIT_CODE == _T(""))
			 {
				 _pVirsualClass.m_BOOK_USE_LIMIT_CODE = strTemp ;
			 }
			 else
			 {
				 nPos = _pVirsualClass.m_BOOK_USE_LIMIT_CODE.Find (strTemp.GetBuffer (0)) ;
				 if( nPos == -1) 
				 {
					 _pVirsualClass.m_BOOK_USE_LIMIT_CODE += _T(" ")+strTemp ;
				 }
			 }
		 }

		 _pVirsualClass.m_bCheckUseLimitCode = TRUE ;
		 
	 }
	 //==================================================================================================





	CString s_Vol_Info ;
	
	if(sVol_Title != _T(""))
	{
		s_Vol_Info += sVol_Title ;
	}
	if(sOther_Vol_Title !=_T(""))
	{
		if(s_Vol_Info !=_T(""))
		{
			s_Vol_Info += _T("-(") +sOther_Vol_Title+_T(")") ; 
		}
		else
		{
            s_Vol_Info += _T("(") +sOther_Vol_Title+_T(")") ;
		}
	}
	if(sPub_Date!=_T("")) 
	{
		s_Vol_Info += _T("(") + sPub_Date + _T(")") ;
	}
	
	// 2007.05.28 ADD BY PDJ
	// IDX_ARTICLE 생성시 숫자 부분을 빼고 처리
	TrimTitleNum(strIndexArticle);

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
    //-------------------------------------------------------------------------------------
	//2004-07-19 이학중 
	/*
	iCh = pMakeData->GetKeyWord( strIndexAuthor );				//공백, 특수문자 제거
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexAuthor ) error") );
		return -2;
	}
	*/
	   
	if(strIndexAuthor.Find (_T(";"))!=-1)
	{    
		CString strTemp = _T("");
		CString strToken = _T("") ;
		TCHAR seps[2] ;
		TCHAR *token ;
		seps[0] = ';';
		seps[1] = '\0';
		token = _tcstok( strIndexAuthor.GetBuffer (0), seps );
		while( token != NULL )
		{
			strToken = token ;
			strToken.TrimLeft ();strToken.TrimRight () ;
		    if(strTemp == _T("")) 
			{
				strTemp = strToken ;
			}
			else
			{
				strTemp += _T(" ") + strToken ;
			}
			token = _tcstok( NULL, seps );
		}
       
        strIndexAuthor = strTemp ;
	}

	//--------------------------------------------------------------------------------
	
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding ( strStreamMarc, &marc );

	CIndexTool *pIndexTool;
	pIndexTool = new CIndexTool;
	pIndexTool->Init(m_pInfo->pMarcMgr , &marc);
	CString str653IndexItem;
	pIndexTool->GetIndex( _T("키워드파일") , str653IndexItem);

	pTmpDM->InitDBFieldData () ;
	if ( nMode == 0 )
	{
		pTmpDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),nIndexRecKey) ;
	}
	pTmpDM->AddDBFieldData (_T("IDX_ARTICLE"),_T("STRING"),strIndexArticle) ;
	pTmpDM->AddDBFieldData (_T("IDX_IARTICLE"),_T("STRING"),strIndexArticle) ;

	pTmpDM->AddDBFieldData (_T("IDX_ARTICLE_KEYWORD"),_T("STRING"),str653IndexItem );
	pTmpDM->AddDBFieldData (_T("IDX_ARTICLE_AUTHOR"),_T("STRING"),strIndexAuthor );
     
	pTmpDM->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),sVol_Key );
    CString strAllItem ;
	strAllItem.Format (_T("%s %s %s %s"),strIndexArticle,strIndexArticle,strIndexAuthor,str653IndexItem) ;

    pTmpDM->AddDBFieldData (_T("IDX_ALL_ITEM"),_T("STRING"),strAllItem );

	/*
	pTmpDM->InitDBFieldData () ;
	if( nMode == 0 )
	{
		pTmpDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),nIndexRecKey) ;
	}
	pTmpDM->AddDBFieldData (_T("IDX_ARTICLE"),_T("STRING"),strIndexArticle) ;
	pTmpDM->AddDBFieldData (_T("IDX_IARTICLE"),_T("STRING"),strIndexArticle) ;
	
	pTmpDM->AddDBFieldData (_T("IDX_ARTICLE_KEYWORD"),_T("STRING"),strIndexArticle);
	pTmpDM->AddDBFieldData (_T("IDX_ARTICLE_AUTHOR"),_T("STRING"),strIndexAuthor );
	pTmpDM->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),sVol_Key );
	*/
	
	//sArticle_controlno
	/*

     //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //2003.08.12 
	 //이학중 수정 .

     CString strAllItem ;
	 strAllItem.Format (_T("%s %s %s"),strIndexArticle,strIndexArticle,strIndexAuthor) ;
     pTmpDM->AddDBFieldData (_T("IDX_ALL_ITEM"),_T("STRING"),strAllItem );

     
	 */



	///////////////////////////////////////////////////
	//LOC_FLAG 얻어 오기 
	//  IF ( COUNT >= 1 ) THEN nReturnCnt = 0 소장 
	//  IF ( COUNT == 0 ) THEN nReturnCnt = 1 작업 
	
	if(_pVirsualClass.m_nCheckLOCFLAG == -1) 
	{
		_pVirsualClass.m_nCheckLOCFLAG = GetLocFlag(pTmpDM,sVol_Key);
	}

	if( _pVirsualClass.m_nCheckLOCFLAG == 0 )
	{
		pTmpDM->AddDBFieldData (_T("LOC_FLAG"),_T("STRING"),_T("1") );	
	}
	else if( _pVirsualClass.m_nCheckLOCFLAG >= 1)
	{
		pTmpDM->AddDBFieldData (_T("LOC_FLAG"),_T("STRING"),_T("0") );
	}
	else 
	{
		AfxMessageBox(_T("GetLogFlag() Error~!")) ;
	}
    /////////////////////////////////////////////////////////////////////
	
	pTmpDM->AddDBFieldData (_T("ARTICLE_CONTROL_NO"),_T("STRING"),sArticle_controlno );
	pTmpDM->AddDBFieldData (_T("TITLE_INFO"),_T("STRING"),sTitle );
	pTmpDM->AddDBFieldData (_T("PUB_INFO"),_T("STRING"),sPublisher );
	pTmpDM->AddDBFieldData (_T("PUB_YEAR_INFO"),_T("STRING"),sPub_year );
	pTmpDM->AddDBFieldData (_T("VOL_INFO"),_T("STRING"),s_Vol_Info ); 
	pTmpDM->AddDBFieldData (_T("PLACE_INFO"),_T("STRING"),sPlace_info );
	pTmpDM->AddDBFieldData (_T("SE_SHELF_CODE"),_T("STRING"),sSe_shelf_code );
	pTmpDM->AddDBFieldData (_T("CLASS_NO"),_T("STRING"),sClass_no );
	pTmpDM->AddDBFieldData (_T("BOOK_CODE"),_T("STRING"),sBook_code );
	
	pTmpDM->AddDBFieldData (_T("ARTICLE_INFO"),_T("STRING"),sArticle );
	pTmpDM->AddDBFieldData (_T("ARTICLE_AUTHOR_INFO"),_T("STRING"),sAuthor );
	pTmpDM->AddDBFieldData (_T("ARTICLE_PAGE_INFO"),_T("STRING"),sPage_info );
//	pTmpDM->AddDBFieldData (_T("VOL_USE_LIMIT_CODE"),_T("STRING"),m_BOOK_USE_LIMIT_CODE );	
	
	 //=================================================================================
	 //2004-07-09 이학중 
	 pTmpDM->AddDBFieldData (_T("USE_LIMIT_CODE"),_T("STRING"),_pVirsualClass.m_SPECIES_USE_LIMIT_CODE);
     pTmpDM->AddDBFieldData (_T("VOL_USE_LIMIT_CODE"),_T("STRING"),_pVirsualClass.m_BOOK_USE_LIMIT_CODE );	

	 //=================================================================================



//	ids = pTmpDM->MakeInsertFrame (_T("IDX_SE_TOC_TBL"));
//	if(ids<0) {
//		AfxMessageBox(_T("MakeInsertFrame() Error~!"));
//		return 100 ;
//	}

	if ( nMode == 0 )
	{
		ids = pTmpDM->MakeInsertFrame (_T("IDX_SE_TOC_TBL"));
		if(ids<0) 
		{
		 AfxMessageBox(_T("MakeInsertFrame() Error~!"));
		 return 100 ;
		}
	}
	else
	{
		ids = pTmpDM->MakeUpdateFrame(_T("IDX_SE_TOC_TBL"), _T("REC_KEY"), _T("NUMERIC"), nIndexRecKey, nRowIndex );
		if(ids<0) 
		{
		 AfxMessageBox(_T("MakeUpdateFrame() Error~!"));
		 return 100 ;
		}
	}

	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{   
		CString strData ;
		strData.Format(_T("ECO$P_MAKE_SETOC$I(%s);"), nIndexRecKey);
		pTmpDM->AddFrame(strData);
	}

	//////////////////////////////////////////////////////////////////////////
	// INSERT IDX_TMP_SE_TOC_TBL 
	//////////////////////////////////////////////////////////////////////////

	pTmpDM->InitDBFieldData ();
	
	CString strTmp_Key ;
	CString system_Date ;
	CTime t = CTime::GetCurrentTime ();
	system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
	
	
	pTmpDM->MakeRecKey (strTmp_Key) ;
	pTmpDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
	pTmpDM->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),sArticle_Key);
	pTmpDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
	pTmpDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
	
	ids = pTmpDM->MakeInsertFrame (_T("IDX_TMP_SE_TOC_TBL"));
	if(ids<0) {
		AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_SE_TOC_TBL) Error~!"));
		return 100 ;
	}

	
	delete pMakeData ;
    	
	return 0 ;
}



//  IF ( COUNT >= 1 ) THEN nReturnCnt = 0 소장 
//  IF ( COUNT == 0 ) THEN nReturnCnt = 1 작업 

INT CCONTENTMGR_SE_ARTICLE::GetLocFlag(CESL_DataMgr * pDM , CString str_Vol_Key)
{
  
	INT nReturnCnt  ;
	CString strResult ;
    CString strQuery ;
	strQuery.Format (_T("SELECT COUNT(WORKING_STATUS) FROM SE_BOOK_TBL WHERE ")
                     //=====================================================
					 //2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 					 _T("WORKING_STATUS IN('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O') ")
					 // 18.09.27 JMJ KOL.RED.2018.008
					 _T("WORKING_STATUS IN('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O') ")
					 //=====================================================
					 _T("AND VOL_KEY = %s "),str_Vol_Key);
	
	pDM->GetOneValueQuery (strQuery,strResult);

	nReturnCnt = _ttoi(strResult.GetBuffer (0)) ;
    
	return nReturnCnt ;


}
VOID CCONTENTMGR_SE_ARTICLE::OnbMarcImport() 
{
	//기사 마크 반입
	CKisaMarcImport dlg(this);
	if ( dlg.DoModal() == IDOK ) 
	{
		CESL_DataMgr *pDM;
		pDM = FindDM ( m_Se_Article_tbl );
		for ( INT nIdx = 0 ; nIdx < pDM->GetRowCount() ; nIdx++ ) 
		{
			if ( pDM->GetCellData(_T("DB_FLAG"), nIdx ) != _T("N") ) continue;
			
			SaveImportKisa( pDM, nIdx );
		}
		ControlDisplay (m_CurrentCM ,_T("MainGrid"),-1) ;
		for ( nIdx = 0 ; nIdx < pDM->GetRowCount() ; nIdx++ ) 
		{
			m_pGrid->SetAt (nIdx,0,_T("V")) ;
		}
	}
	
}

VOID CCONTENTMGR_SE_ARTICLE::OnbMarcExport() 
{
	//기사 마크 반출
	CESL_DataMgr *pDM = FindDM(m_Se_Article_tbl) ;
	if(pDM == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
	}


	CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_CurrentCM, m_CurrentGrid);
	pGrid->SelectMakeList();
	if ( pGrid->SelectMakeList() < 0 ) 
	{
		AfxMessageBox( _T(" 선정된 기사가 없습니다. "));
		return;
	}
	
	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CFileDialog dlg(FALSE, _T("*.*"), _T("반출.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("파일형식(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return ;

	_tchdir(szCurrentDir);
	
	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return ;
    sFileName = dlg.GetPathName();	

	FILE *fp;
	fp = _tfopen( sFileName , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox(_T("파일을 선택하세요."));
		return ;
	}
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}


	CString strStreamMarc;
	INT nSelGridIdx = pGrid->SelectGetHeadPosition();
	while ( nSelGridIdx > -1 ) 
	{
		strStreamMarc = _T("");
		pDM->GetCellData ( _T("ARTICLE"), nSelGridIdx, strStreamMarc ) ;
		if ( strStreamMarc.IsEmpty() ) continue;

		strStreamMarc += _T("\r\n");
		_ftprintf( fp, strStreamMarc );		
		nSelGridIdx = pGrid->SelectGetNext();
	}

	fclose( fp );

	CString sMessage;
	sMessage.Format( _T("선정된 %d건에 대한 파일작성이 완료되었습니다."), pGrid->SelectGetCount() );
	AfxMessageBox( sMessage );

	return;	
	
}


VOID CCONTENTMGR_SE_ARTICLE::SaveImportKisa( CESL_DataMgr *pDM, INT nRowIndex )
{
	// 반입 받은 넘을 따로 저장하는 루틴 ^^;;;; 지금 시간 새벽 4시.. ㅠㅠ;
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) return ;

	//제어번호만 만들어서 바꿔치기
	CString strKisaControl_No = MakeKisaControl_No( nRowIndex ) ;

	CString strStreamMarc;
	pDM->GetCellData( _T("ARTICLE"), nRowIndex, strStreamMarc);
	
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding ( strStreamMarc, &marc );

	m_pInfo->pMarcMgr->InsertField( &marc, _T("001") + strKisaControl_No );

	//005 최종처리일시
	CTime t = CTime::GetCurrentTime () ;
	CString Current_Date;
	Current_Date.Format (_T("%04d%02d%02d%02d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay (), t.GetHour(), t.GetMinute(), t.GetSecond() ) ;
	Current_Date = _T("005") + Current_Date;
	m_pInfo->pMarcMgr->DeleteField( &marc, _T("005") );
	m_pInfo->pMarcMgr->InsertField( &marc, Current_Date );

	
	m_pInfo->pMarcMgr->Encoding ( &marc , strStreamMarc );

	pDM->SetCellData( _T("ARTICLE"), strStreamMarc, nRowIndex );

	CString sArticle_Key;
	pDM->MakeRecKey (sArticle_Key) ;
	pDM->InitDBFieldData () ;
		
    pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),sArticle_Key);
	pDM->AddDBFieldData (_T("ARTICLE_CONTROL_NO"),_T("STRING"),strKisaControl_No );
	pDM->SetCellData(_T("기사제어번호"), strKisaControl_No , nRowIndex);

	CString strTitle;
	m_pInfo->pMarcMgr->GetItem( &marc, _T("245$a"), strTitle );
	pDM->AddDBFieldData (_T("ARTICLE_NAME"),_T("STRING"),strTitle);

	CString strAuthor;
	m_pInfo->pMarcMgr->GetItem( &marc, _T("245$d"), strAuthor );
	pDM->AddDBFieldData (_T("ARTICLE_AUTHOR"),_T("STRING"),strAuthor);

	t = CTime::GetCurrentTime () ;
	Current_Date.Format (_T("%d/%d/%d"),t.GetYear (), t.GetMonth (), t.GetDay ()) ;

	pDM->AddDBFieldData (_T("CONTENTS_KEY"),_T("NUMERIC"),m_Contents_Key) ;
    pDM->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),m_Vol_Key );

    pDM->AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),Current_Date);
    pDM->AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),Current_Date);
	pDM->AddDBFieldData( _T("ARTICLE"), _T("STRING"), strStreamMarc );
	pDM->AddDBFieldData (_T("ARTICLE_APPLY_YN"),_T("STRING"),_T("Y"));

	pDM->MakeInsertFrame (_T("SE_ARTICLE_TBL"));

	ids = pDM->SendFrame();
	if ( ids < 0 ) return;

	ids = pDM->EndFrame();
	if ( ids < 0 ) return;
}

VOID CCONTENTMGR_SE_ARTICLE::Display()
{
	m_marcEditor.Display();
}


CString CCONTENTMGR_SE_ARTICLE::MakeStreamMarc(CString sArticle_controlno,
										   CString strTitle,
										   CString sAuthor ,
										   CString sPage_info,
										   CString sVol_Key,
										   CString sContent_Key
										   )
{
	CString strStreamMarc;
	CString strItemData;
	strStreamMarc.Empty();
	
	m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("245"), _T("") );
	m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("653"), _T("") );
	m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("001"), _T("") );

	//001 제어번호
	m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("001"), sArticle_controlno );

	//005 최종처리일시
	CTime t = CTime::GetCurrentTime () ;
	CString Current_Date;
	Current_Date.Format (_T("%04d%02d%02d%02d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay (), t.GetHour(), t.GetMinute(), t.GetSecond() ) ;
	Current_Date = _T("005") + Current_Date;
	m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("005") );
	m_pInfo->pMarcMgr->InsertField( m_pMarc, Current_Date );

	//040 정보 
	CString strLibCode;
	GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("040$a"), strItemData);
	if (strItemData.IsEmpty())
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("040$a"), strLibCode);
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("040$c"), strLibCode);
	}
	// 도서관 부호가 다른경우 040$d로 넣어준다. 반복 안되게..
	else if (strItemData != strLibCode)
	{
		// 삭제하고 넣는다.
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("040$d"), strLibCode);
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("040$d"), strLibCode);
	}

	//008 입력
	Current_Date = CTime::GetCurrentTime().Format(_T("%y%m%d")) ;
	for ( INT i = Current_Date.GetLength() ; i < 39 ; i++ )
	{
		Current_Date = Current_Date + _T(" ");
	}
	m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("008@0-39"), Current_Date );


	//245 Tag
	m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("245$a"), strTitle );
	m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("245$d"), sAuthor );
	

	//041(언어부호) Tag
	strItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("008$35@37"), strItemData );
	if ( !strItemData.IsEmpty() )
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("041$a"), strItemData );


	// 연속 종의 Marc를 가져와서 입력
	CESL_DataMgr *pSeDM = FindDM ( _T("DM_CONTENTMGR_SE_TBL") );
	CString strSpeciesKey;
	pSeDM->GetCellData(_T("종키"), 0, strSpeciesKey);

	CString strQuery = _T("SELECT MARC FROM SE_SPECIES_TBL WHERE REC_KEY = '") + strSpeciesKey + _T("'");
	CString strSeTBLMarc;
	strSeTBLMarc.Empty();
	pSeDM->GetOneValueQuery( strQuery, strSeTBLMarc );

	CMarc *pSpeciesMarc = NULL;
	pSpeciesMarc = new CMarc;
	m_pInfo->pMarcMgr->Decoding( strSeTBLMarc , pSpeciesMarc );

	CString str052Field;
	m_pInfo->pMarcMgr->GetField( pSpeciesMarc, _T("052"), str052Field );
	if ( !str052Field.IsEmpty() )
	{
		m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("052") );
		str052Field = _T("052") + str052Field;
		m_pInfo->pMarcMgr->InsertField( m_pMarc, str052Field );
	}

	CString str260Field;
	m_pInfo->pMarcMgr->GetField( pSpeciesMarc, _T("260"), str260Field );
	if ( !str260Field.IsEmpty() )
	{
		m_pInfo->pMarcMgr->DeleteField( m_pMarc, _T("260") );
		str260Field = _T("260") + str260Field;
		m_pInfo->pMarcMgr->InsertField( m_pMarc, str260Field );
	}

	//773$t (잡지명 - 해당권호의 잡지명)
	strItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pSpeciesMarc, _T("245$a"), strItemData );
	if ( !strItemData.IsEmpty() )
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("773$t"), strItemData );
	
	CString strVolQuery, strVolTitle;
	strVolQuery.Format(_T("SELECT VOL_TITLE FROM SE_VOL_TBL WHERE REC_KEY = %s "), sVol_Key );
	pSeDM->GetOneValueQuery(strVolQuery, strVolTitle);
				
	if ( !strVolTitle.IsEmpty() )
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("773$g"), strVolTitle );


	//773$w ( 제어번호 - 해당 연속간행물종의 제어번호)
	strItemData = _T("");
	/*
	m_pInfo->pMarcMgr->GetItem( pSpeciesMarc, _T("245$c"), strItemData );
	if ( !strItemData.IsEmpty() )
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("773$w"), strItemData );
	*/
	pSeDM->GetCellData(_T("제어번호"), 0, strItemData );
	if ( !strItemData.IsEmpty() )
	{
		CString strTmpData;
		strTmpData.Format(_T("(%s)%s"), strLibCode , strItemData );
		m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("773$w"), strTmpData );
	}


	//653Tag 생성
	CBO_CAT_PROC_KEYWORD_0000 *pDlg = NULL;
	pDlg = new CBO_CAT_PROC_KEYWORD_0000(this, *m_pMarc);
	pDlg->pMain = this->pMain;
	pDlg->pParentMgr = (CESL_Mgr*)this;
	pDlg->m_pInfo = this->m_pInfo;
	pDlg->Create(this);
	pDlg->UpdateWindow();
	pDlg->CenterWindow();
	pDlg->MakeKeyWord();
	strStreamMarc = pDlg->GetStreamMarc();
	
	m_pInfo->pMarcMgr->Decoding (strStreamMarc , m_pMarc ) ;
	//*m_pMarc = dlg.m_marc;

	m_pInfo->pMarcMgr->DeleteEmptyField( m_pMarc );
	m_pInfo->pMarcMgr->DeleteEmptySubfield( m_pMarc );
	m_pInfo->pMarcMgr->Encoding( m_pMarc, strStreamMarc );

	Display();
	return strStreamMarc;
}

BEGIN_EVENTSINK_MAP(CCONTENTMGR_SE_ARTICLE,CDialog )
    //{{AFX_EVENTSINK_MAP(CSE_CREATE_ARTICLE)
	ON_EVENT(CCONTENTMGR_SE_ARTICLE, IDC_Grid_CONTENT_SE, -600 /* Click */, OnClickGridCONTENTSE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCONTENTMGR_SE_ARTICLE::OnClickGridCONTENTSE() 
{
	CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return ;
	}

    m_pGrid = (CESL_Grid*)pCM->FindControl (m_CurrentGrid) ;
	if(m_pGrid == NULL) {
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return ;
	}
	
	INT nGirdIdx = m_pGrid->GetRow();
	CESL_DataMgr *pDM = FindDM(m_Se_Article_tbl ) ;
	if(pDM == NULL){
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}

	CString strKisaControl_No;
	INT nRowIndex;
	nRowIndex = m_pGrid->GetRow();
	m_pGrid->GetAt (nRowIndex-1,1,strKisaControl_No) ;
	if ( strKisaControl_No.IsEmpty() && pDM->GetCellData( _T("ARTICLE"), nGirdIdx-1 ).IsEmpty() ) 
	{
		//기본적인 마크 뿌려주기
		MakeDefaultMarc( pDM, nRowIndex );
	}


	CString strStreamMarc;
	strStreamMarc.Empty();
	pDM->GetCellData( _T("ARTICLE"), nGirdIdx-1, strStreamMarc);

	m_pMarc->Init();
	m_pInfo->pMarcMgr->Decoding( strStreamMarc, m_pMarc );

	CString str008;
	m_pInfo->pMarcMgr->GetItem(m_pMarc,_T("008*"),str008);

	if( m_bFIXED_FIELD != 0 && m_str008.IsEmpty() == FALSE && str008.IsEmpty() == TRUE )
		m_pInfo->pMarcMgr->InsertField(m_pMarc, _T("008") + m_str008 );

	m_marcEditor.Display();

	return;
	
}

CString CCONTENTMGR_SE_ARTICLE::MakeStreamMarc_Ex(CString strStream,
											  CString sArticle_controlno										   
												)
{
	CString strStreamMarc;
	CMarc *pMarc;
	pMarc = new CMarc;
	m_pInfo->pMarcMgr->Decoding( strStream , pMarc );
	m_pInfo->pMarcMgr->InsertField( pMarc , _T("001") + sArticle_controlno );

	//040 정보 
	CString strLibCode;
	GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);

	CString strItemData;
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("040$a"), strItemData);
	if (strItemData.IsEmpty())
	{
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("040$a"), strLibCode);
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("040$c"), strLibCode);
	}
	// 도서관 부호가 다른경우 040$d로 넣어준다. 반복 안되게..
	else if (strItemData != strLibCode)
	{
		// 삭제하고 넣는다.
		m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("040$d"), strLibCode);
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("040$d"), strLibCode);
	}

	//008 입력
	CString Current_Date = CTime::GetCurrentTime().Format(_T("%y%m%d")) ;
	for ( INT i = Current_Date.GetLength() ; i < 39 ; i++ )
	{
		Current_Date = Current_Date + _T(" ");
	}
	m_pInfo->pMarcMgr->SetItem( pMarc, _T("008@0-39"), Current_Date );

	m_pInfo->pMarcMgr->Encoding( pMarc, strStreamMarc );

	return strStreamMarc;

}


VOID CCONTENTMGR_SE_ARTICLE::MakeDefaultMarc( CESL_DataMgr *pDM, INT nGridRow )
{
	//선정된 자료의 기본적인 마크 뿌려주기
	CMarc *pTmpMarc;
	pTmpMarc = new CMarc;

	//고정길이 유지체크되있고, 저장된 008이 있으면 넣어준다.
	if( m_bFIXED_FIELD == 1 && m_str008.IsEmpty == FALSE )
		m_pInfo->pMarcMgr->InsertField(pTmpMarc, _T("008") + m_str008);

	CString strTitle, strAuthor, strPage;

	m_pGrid->GetAt (nGridRow-1,2,strTitle) ;
	m_pGrid->GetAt (nGridRow-1,3,strAuthor) ;
	m_pGrid->GetAt (nGridRow-1,4,strPage) ;


	m_pInfo->pMarcMgr->SetItem( pTmpMarc, _T("245$a"), strTitle );
	m_pInfo->pMarcMgr->SetItem( pTmpMarc, _T("245$d"), strAuthor );

	CESL_DataMgr *pSeDM = FindDM ( _T("DM_CONTENTMGR_SE_TBL") );
	CString strSpeciesKey;
	pSeDM->GetCellData(_T("종키"), 0, strSpeciesKey);

	CString strQuery = _T("SELECT MARC FROM SE_SPECIES_TBL WHERE REC_KEY = '") + strSpeciesKey + _T("'");
	CString strSeTBLMarc;
	strSeTBLMarc.Empty();
	pSeDM->GetOneValueQuery( strQuery, strSeTBLMarc );

	CMarc *pMarc = NULL;
	pMarc = new CMarc;
	m_pInfo->pMarcMgr->Decoding( strSeTBLMarc , pMarc );


	//005 최종처리일시
	CTime t = CTime::GetCurrentTime () ;
	CString Current_Date;
	Current_Date.Format (_T("%04d%02d%02d%02d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay (), t.GetHour(), t.GetMinute(), t.GetSecond() ) ;
	Current_Date = _T("005") + Current_Date;
	m_pInfo->pMarcMgr->DeleteField( pTmpMarc, _T("005") );
	m_pInfo->pMarcMgr->InsertField( pTmpMarc, Current_Date );


	CString str052Field;
	m_pInfo->pMarcMgr->GetField( pMarc, _T("052"), str052Field );
	if ( !str052Field.IsEmpty() )
	{
		m_pInfo->pMarcMgr->DeleteField( pTmpMarc, _T("052") );
		str052Field = _T("052") + str052Field;
		m_pInfo->pMarcMgr->InsertField( pTmpMarc, str052Field );
	}

	CString str260Field;
	m_pInfo->pMarcMgr->GetField( pMarc, _T("260"), str260Field );
	if ( !str260Field.IsEmpty() )
	{
		m_pInfo->pMarcMgr->DeleteField( pTmpMarc, _T("260") );
		str260Field = _T("260") + str260Field;
		m_pInfo->pMarcMgr->InsertField( pTmpMarc, str260Field );
	}

	//773$t (잡지명 - 해당권호의 잡지명)
	CString strItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$a"), strItemData );
		if ( !strItemData.IsEmpty() )
			m_pInfo->pMarcMgr->SetItem( pTmpMarc, _T("773$t"), strItemData );

	CString sVol_Key ;
	//GetDataMgrData (_T("DM_CONTENTMGR_SE_ARTICLE") ,_T("권호키"), sVol_Key , 0 );
	GetDataMgrData (_T("DM_CONTENTMGR_SE_MAIN") ,_T("권호키"),sVol_Key,0);

	CString strVolQuery, strVolTitle;
	strVolQuery.Format(_T("SELECT VOL_TITLE FROM SE_VOL_TBL WHERE REC_KEY = %s "), sVol_Key );
	pSeDM->GetOneValueQuery(strVolQuery, strVolTitle);
				
	if ( !strVolTitle.IsEmpty() )
	{
		CString strTmpData;
		strTmpData.Format(_T("%s, %s"), strVolTitle, strPage);
		m_pInfo->pMarcMgr->SetItem( pTmpMarc, _T("773$g"), strTmpData );
	}


	//773$w ( 제어번호 - 해당 연속간행물종의 제어번호)
	strItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$c"), strItemData );
	if ( !strItemData.IsEmpty() )
		m_pInfo->pMarcMgr->SetItem( pTmpMarc, _T("773$w"), strItemData );


	//653Tag 생성
	CString strStreamMarc;
	CBO_CAT_PROC_KEYWORD_0000 *pDlg = NULL;
	pDlg = new CBO_CAT_PROC_KEYWORD_0000(this, *pTmpMarc);
	pDlg->pMain = this->pMain;
	pDlg->pParentMgr = (CESL_Mgr*)this;
	pDlg->m_pInfo = this->m_pInfo;
	pDlg->Create(this);
	pDlg->UpdateWindow();
	pDlg->CenterWindow();
	pDlg->MakeKeyWord();
	strStreamMarc = pDlg->GetStreamMarc();
	
	pDM->SetCellData(_T("ARTICLE"), strStreamMarc, nGridRow-1 );

}


INT CCONTENTMGR_SE_ARTICLE::GetLibInfo(CString ArrFieldAttr[], 
						CString ArrResult[], 
						INT nFieldCount)
{
	if (!ArrFieldAttr || !ArrResult || nFieldCount < 1) return -1;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString strFieldAttr;
	for (INT idx = 0; idx < nFieldCount; idx++)
	{
		if (ArrFieldAttr[idx].IsEmpty()) continue;

		strFieldAttr += ArrFieldAttr[idx];
		if (idx != nFieldCount - 1)
			strFieldAttr += _T(", ");
	}
	CString strQuery;
	strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE"), strFieldAttr);
	INT ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
	if (ids < 0) return -1;
//	// 설정 값 가져오기
//	CString strManageValue;
//	CString strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
//			           _T("WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND CLASS_ALIAS = '관리구분별도서관설정'");
//
//	INT ids = dm.GetOneValueQuery(strQuery, strManageValue);
//	if (ids < 0) return -1;
//
//	CString strFieldAttr;
//	for (INT idx = 0; idx < nFieldCount; idx++)
//	{
//		if (ArrFieldAttr[idx].IsEmpty()) continue;
//
//		strFieldAttr += ArrFieldAttr[idx];
//		if (idx != nFieldCount - 1)
//			strFieldAttr += _T(", ");
//	}
//
//	CString strManageCode;
//	if (strManageValue == _T("Y"))
//	{
//		strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE REC_KEY = %s"), m_pInfo->USER_PK);
//		ids = dm.GetOneValueQuery(strQuery, strManageCode);
//		if (ids >= 0)
//		{
//			if (strManageCode.IsEmpty())
//				strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"), strFieldAttr, strManageCode);
//			else
//				strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), strFieldAttr, strManageCode);
//
//			ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
//		}
//	}
//	else
//	{
//		strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"), strFieldAttr);
//		INT ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
//	}
	//--2008.10.15 UPDATE BY PWR --}}

	return 0;
}


VOID CCONTENTMGR_SE_ARTICLE::OnbModify() 
{
	CMarc *pMarc;
	pMarc = new CMarc;
	CString strTitle, strAuthor, strPage;

	m_marcEditor.GetEditCtrl()->GetMarc( pMarc );

	CESL_DataMgr *pDM = FindDM(m_Se_Article_tbl) ;
	if(pDM == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
	}

	INT nRowIndex = m_pGrid->GetRow();

	CString strStreamMarc;
	m_pInfo->pMarcMgr->Encoding( pMarc, strStreamMarc );

	pDM->SetCellData(_T("ARTICLE"), strStreamMarc, nRowIndex-1 );

	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$a"), strTitle );
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$d"), strAuthor );
	
	pDM->SetCellData(_T("기사명"), strTitle, nRowIndex-1 );
	pDM->SetCellData(_T("저자"), strAuthor, nRowIndex-1 );	
	m_pGrid->GetAt (nRowIndex-1,4,strPage) ;
	pDM->SetCellData(_T("페이지"), strPage, nRowIndex-1 );
//	pDM->SetCellData(_T("DB_FLAG"), _T("U"), nRowIndex-1 );

	//ControlDisplay (m_CurrentCM ,_T("MainGrid"),-1) ;
	m_pGrid->SetTextMatrix(nRowIndex, 3, strTitle );
	m_pGrid->SetTextMatrix(nRowIndex, 4, strAuthor );
	m_pGrid->SetTextMatrix(nRowIndex, 5, strPage );

	m_pGrid->SetReverse(nRowIndex-1);	

	AfxMessageBox ( _T("수정되었습니다.") );
}

VOID CCONTENTMGR_SE_ARTICLE::TrimTitleNum(CString &str)
{
	CString strChar;
	BOOL	bNumFlag;
	INT		i;
	INT		nLength;
	nLength = str.GetLength();

	for ( i=0 ; i<nLength ; i++ )
	{
		strChar = str.Mid(i, 1);
		if ( strChar >= '0' && strChar <= '9' )
		{
			bNumFlag = TRUE;
			continue;
		}
		else
		{
			if ( bNumFlag == TRUE )
			{
				if ( strChar == _T(".") || strChar == _T(" ") )
				{
					str = str.Mid(i+1);
					break;
				}
				else
				{
					bNumFlag = FALSE;
					break;
					
				}
			}
			else
			{
				bNumFlag = FALSE;
				break;
			}				
		}
	}
}

VOID CCONTENTMGR_SE_ARTICLE::OnbKeyword2() 
{
	CString strStreamMarc;
	INT		nIdx;

	CESL_DataMgr *pDM = FindDM(_T("DM_CONTENTMGR_SE_ARTICLE"));
	if(pDM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error"));
	}

	nIdx = m_pGrid->GetRow();
	m_marcEditor.GetEditCtrl()->GetMarc( m_pMarc );

	CBO_CAT_PROC_KEYWORD_0000 dlg(this, *m_pMarc);
	if (IDOK != dlg.DoModal()) return ;

	*m_pMarc = dlg.m_marc;

	m_pInfo->pMarcMgr->Encoding( m_pMarc, strStreamMarc );
	pDM->SetCellData(_T("ARTICLE"), strStreamMarc, nIdx-1 );

	Display();	
}

VOID CCONTENTMGR_SE_ARTICLE::OnbFixedField() 
{
	// 고정길이
	CMarc *pMarc;
	pMarc = new CMarc;
	m_marcEditor.GetEditCtrl()->GetMarc( m_pMarc );

	// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
	// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
	//=====================================================
	//2009.01.29 UPDATE BY PJW : 고정길이 통합마크로 변경
// 	CCat008FixedField_TX dlg(this, m_pMarc);
	CDLG_008_BK dlg(this, m_pMarc);
	//=====================================================
	dlg.DoModal();
*///AFTER------------------------------------------------------------------------
	CString strGetGroupName = _T("");
	CString strFileName = _T("..\\cfg\\MarcRelation\\통합마크_개정_그룹명.cfg");
	CFileFind find;
	if (TRUE == find.FindFile(strFileName)) {
		CDLG_008_BK_1 dlg(this, m_pMarc);
		dlg.DoModal();
	} else {
		CDLG_008_BK dlg(this, m_pMarc);
		dlg.DoModal();
	}
	find.Close();
/*//END------------------------------------------------------------------------*/
	m_marcEditor.Display();

	// 008을 저장해둔다. 
	CString str008;
	str008.Empty();
	m_pInfo->pMarcMgr->GetItem(m_pMarc,_T("008*"), str008);
	if(!str008.IsEmpty())
	m_str008 = str008;

	return;
}

VOID CCONTENTMGR_SE_ARTICLE::OnCheck1() 
{
	m_bFIXED_FIELD = !m_bFIXED_FIELD;	
}

HBRUSH CCONTENTMGR_SE_ARTICLE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}