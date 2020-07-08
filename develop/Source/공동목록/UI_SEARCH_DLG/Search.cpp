// Search.cpp: implementation of the CSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\..\\..\\include\\공동목록\\Search.h"
#include "ESL_Mgr.h"    
#include "except.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUm_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSearch::CSearch()
{
    m_ResultCnt          = 0;
    m_bRequestEndFlag    = TRUE;
    m_bThreadActiveFlag  = FALSE;

    m_bShowFlag          = FALSE;
    m_bSortAsc           = TRUE ;
    m_bSortedFlag        = FALSE;
    m_bAppendDisplayFlag = FALSE;


     m_PagePerCnt         = 200;
    m_MaxFetchCnt        = 2000;

    m_pDialogMgr         = NULL;
    m_pGrid              = NULL;
    m_pProgressBar       = NULL;
    m_pDataMgr           = NULL;

    _tcscpy ( m_szSortInfo , _T("SORT_TITLE ASC") );

}

CSearch::~CSearch()
{

}

INT CSearch::Initialize     (
                                 CESL_Mgr				*lpDialogMgr		,
                                 CTextProgressCtrl		*lpProgressBar		,
                                 TCHAR					*lpDispAlias		,
                                 TCHAR					*lpDataMgrAlias		,
                                 TCHAR					*lpCtrlMgrAlias		,
                                 TCHAR					*lpGridAlias		,
                                 TCHAR					*lpCollection		,
                                 TCHAR					*lpQuery			,
                                 CWnd					*lpResutWnd			, 
                                 INT					PagePerCnt			,
                                 INT					MaxFetchCnt			,
                                 BOOL					AppendDisplay		,
								 CKolisNetSearchParam	*pKolisNetSearchParam ,
								 CKolisNetDataMapper	*pKolisNetDataMapper
                             )
{
    INT   ids;

    SetDialogMgrPtr		(lpDialogMgr);
    SetProgressBarPtr	(lpProgressBar);
    SetDispAlias		(lpDispAlias);
    SetDataMgrAlias		(lpDataMgrAlias);
    SetCtrlMgrAlias		(lpCtrlMgrAlias);
    SetGridAlias		(lpGridAlias);
    SetResultPtr		(lpResutWnd);
    SetPagePerCnt		(PagePerCnt);
    SetMaxFetchCnt		(MaxFetchCnt);
    SetAppendDisplay	(AppendDisplay);
    SetQuery			(lpQuery);

	if (pKolisNetSearchParam->GetPUB_FORM_TYPE() == 0)
	{
		CString strCollection = pKolisNetSearchParam->GetCOLL_TYPE_1();
		if(strCollection.GetLength()  > 0){			
			SetCollection		(strCollection.GetBuffer(0));			
		}else {
			SetCollection		(_T("dandok"));	
		}
		strCollection.ReleaseBuffer();
	}
	else
	{
		pKolisNetSearchParam->SetCOLL_TYPE_1(_T(""));
		pKolisNetSearchParam->SetCOLL_TYPE_2(_T(""));
		SetCollection		(lpCollection);
	}

	pKolisNetSearchParam->SetPAGESIZE(PagePerCnt);
    
	CString temp_lpDataMgrAlias;
    temp_lpDataMgrAlias.Format (_T("%s"),lpDataMgrAlias);
	
	m_pDataMgr = lpDialogMgr->FindDM (temp_lpDataMgrAlias);
	
 	if( m_pDataMgr == NULL ) return -1;

    CString temp_m_szCtrlMgrAlias ;
	CString temp_m_szGridAlias ;

    temp_m_szCtrlMgrAlias.Format (_T("%s"),m_szCtrlMgrAlias);
    temp_m_szGridAlias.Format (_T("%s"),m_szGridAlias);

	CESL_ControlMgr *pCM = lpDialogMgr->FindCM(temp_m_szCtrlMgrAlias );
	
	m_pGrid = (CESL_Grid*)(pCM->FindControl( temp_m_szGridAlias));

    if ( AppendDisplay ) {
        ids = m_ShelfDecision.Init( _T("..\\ref\\공동목록\\DecideDataSearchCode\\자료찾기구분값결정REF.txt") );
	    if ( ids != 0 ) {
            return ids;
	    }
    }

    m_ThreadParam.lpThis = this  ;

    return 0;
}

CWinThread *CSearch::SearchDiaplay( INT ThreadType )
{
    if ( ThreadType == _ST_ ) {
        return AfxBeginThread ( (AFX_THREADPROC) QueryWorkerFunc_01, (LPVOID)(&m_ThreadParam) , THREAD_PRIORITY_NORMAL  );
    } else
    if ( ThreadType == _MT_ ) {
        return AfxBeginThread ( (AFX_THREADPROC) QueryWorkerFunc_02, (LPVOID)(&m_ThreadParam) , THREAD_PRIORITY_NORMAL  );
    } else {
        return NULL;
    }

}

VOID CSearch::SetQuery      ( TCHAR *lpQuery )
{
    _tcscpy ( m_szQuery , lpQuery );
}
VOID CSearch::SetPagePerCnt( INT PagePerCnt )
{
    m_PagePerCnt = PagePerCnt;
}
VOID CSearch::SetMaxFetchCnt( INT MaxFetchCnt )
{
    m_MaxFetchCnt = MaxFetchCnt;
}
VOID CSearch::SetAppendDisplay( BOOL AppendDisplay )
{
    m_bAppendDisplayFlag  = AppendDisplay;
}
VOID CSearch::SetDialogMgrPtr( CESL_Mgr *lpDialogMgr )
{
    m_pDialogMgr = lpDialogMgr;
}
VOID CSearch::SetGridPtr( CESL_Grid *lpGrid )
{
    m_pGrid = lpGrid;
}
VOID CSearch::SetProgressBarPtr( CTextProgressCtrl *lpProgressBar )
{
    m_pProgressBar = lpProgressBar;
}
VOID CSearch::SetDataMgrPtr( CESL_DataMgr *lpDataMgr )
{
    m_pDataMgr = lpDataMgr;
}
VOID CSearch::SetCollection( TCHAR *lpCollection )
{
    _tcscpy ( m_szCollection , lpCollection );
}
VOID CSearch::SetDispAlias( TCHAR *lpDispAlias )
{
    _tcscpy ( m_szDispAlias , lpDispAlias );
}

VOID CSearch::SetDataMgrAlias( TCHAR *lpDataMgrAlias )
{
    _tcscpy ( m_szDataMgrAlias , lpDataMgrAlias );
}

VOID CSearch::SetCtrlMgrAlias( TCHAR *lpCtrlMgrAlias )
{
    _tcscpy ( m_szCtrlMgrAlias , lpCtrlMgrAlias );
}

VOID CSearch::SetGridAlias( TCHAR *lpGridAlias )
{
    _tcscpy ( m_szGridAlias , lpGridAlias );
}

VOID CSearch::SetResultPtr( CWnd *lpResutWnd )
{
    m_pResutWnd = lpResutWnd;
}

VOID CSearch::SetSortInfo( TCHAR *lpSortInfo )
{
    _tcscpy ( m_szSortInfo , lpSortInfo );
}

VOID CSearch::Destroy( CWinThread *lpThread) 
{
    if ( NULL != lpThread ) {
        TerminateThread( lpThread->m_hThread  , -1 );
    }
}

INT  CSearch::WorkerThreadStart( )
{
    m_pGrid    = m_pGrid;
    if ( m_pProgressBar != NULL ) {
        m_pProgressBar->ShowWindow( TRUE );
        m_pProgressBar->SetShowText( TRUE );
    }

    return 0;

}

INT  CSearch::InitDiaplay( )
{

    TCHAR    szMsg[128];

    m_ResultCnt          = 0;
    m_bRequestEndFlag    = FALSE;
    m_bThreadActiveFlag  = FALSE;

    if ( m_pDataMgr->GetRowCount() > 0 ) {
        
		m_pDataMgr->FreeData();
        
        if ( m_pProgressBar != NULL ) {
            _stprintf ( szMsg , _T("검색중 입니다    "));
            m_pProgressBar->SetWindowText(szMsg);
            m_pProgressBar->SetShowText( TRUE );
            m_pProgressBar->SetPos(0);
            m_pProgressBar->SetRange(0,m_MaxFetchCnt);
            m_pProgressBar->ShowWindow( TRUE );
        }
        ResultCountDisplay( 0 );
    }

    return 0;

}

INT  CSearch::WorkerThreadEnd  ( INT FetchCount )
{
    if ( m_pProgressBar != NULL ) {
        m_pProgressBar->ShowWindow( FALSE );
    }
  
    if ( FetchCount <= 0 ) {
    } else {
    }

    return 0;
}



VOID CSearch::ResultCountDisplay( INT TotalCnt ) 
{


    TCHAR    szCount[128];

    _stprintf ( szCount , _T("%d / %d") , m_ResultCnt , TotalCnt );
    if ( m_pResutWnd != NULL ) {

    }

}

INT  CSearch::GridDisplayFunc( INT WorkIndex )
{


    INT     ids;
    TCHAR    tmpData[32], szBuffer[128];
    
    szBuffer[0]= 0;

    if ( m_bAppendDisplayFlag == TRUE ) {


		CString temp_m_szDataMgrAlias;
		temp_m_szDataMgrAlias.Format (_T("%s"),m_szDataMgrAlias);

		m_pDataMgr = m_pDialogMgr->FindDM (temp_m_szDataMgrAlias);
		if( m_pDataMgr == NULL ) return -1;

		CString temp_tmpData ;
		temp_tmpData.Format (_T("%s"),tmpData);

		m_pDataMgr->GetCellData(_T("형식구분"),WorkIndex,temp_tmpData);
		_tcscpy(tmpData,temp_tmpData);


		m_ShelfDecision.AddSearchValue( _T("형식구분")    , tmpData );


		m_pDataMgr->GetCellData(_T("발행형태구분"),WorkIndex,temp_tmpData);
		_tcscpy(tmpData,temp_tmpData);


		if ( tmpData[0] == '\0' ) {
		_tcscpy ( tmpData , _T("MO") );
		}
		m_ShelfDecision.AddSearchValue( _T("발행형태구분"), tmpData );

		m_pDataMgr->GetCellData(_T("자료구분"),WorkIndex,temp_tmpData);
		_tcscpy(tmpData,temp_tmpData);	


		m_ShelfDecision.AddSearchValue( _T("자료구분") , tmpData );

		m_ShelfDecision.AddSearchValue( _T("이용대상구분"), _T("") );

		m_pDataMgr->GetCellData(_T("매체구분"),WorkIndex,temp_tmpData);
		_tcscpy(tmpData,temp_tmpData);		

		m_ShelfDecision.AddSearchValue( _T("매체구분"), tmpData );

		ids = m_ShelfDecision.DecideCode( szBuffer );
		if ( ids < 0 ) {
		if ( ids == -100 ) szBuffer[0] = 0;
		else return -5;
		}

		CString temp_szBuffer;
		temp_szBuffer.Format (_T("%s"),szBuffer);

		m_pDataMgr->SetCellData(_T("구분"),temp_szBuffer,WorkIndex);
		_tcscpy(tmpData,temp_szBuffer);	

    }

    
	CString temp_m_szCtrlMgrAlias;
	temp_m_szCtrlMgrAlias.Format (_T("%s"),m_szCtrlMgrAlias);
    
   	
   CESL_ControlMgr *pCM = m_pDialogMgr->FindCM(m_szCtrlMgrAlias );
    
   CString temp_m_szGridAlias ;
   temp_m_szGridAlias.Format (_T("%s"),m_szGridAlias);
   m_pGrid = (CESL_Grid*)(pCM->FindControl( temp_m_szGridAlias));

    return 0;

}


INT  CSearch::MakeQueryInfo( TCHAR *lpFields )
{
  
    INT    i , offset;

    if ( lpFields == NULL ) return -2;

    offset = 0;
	
    for ( i = 0; i < m_pDataMgr->RefList.GetCount(); i ++ ) {
        
		reference *tmp  = (reference*) m_pDataMgr->RefList.GetAt(m_pDataMgr->RefList.FindIndex(i));
        
		if ( tmp->DATA_TYPE == _T("UDF") || tmp->DATA_TYPE == _T("udf") ) continue;

        if ( offset == 0 ) 
            offset += _stprintf ( &lpFields[offset] , _T("%s") , tmp->FIELD_NAME );
        else 
            offset += _stprintf ( &lpFields[offset] , _T(",%s") , tmp->FIELD_NAME );
    }
   
    return 0;

}

INT  CSearch::DisplayGird()
{
	CESL_ControlMgr *pCM = m_pDialogMgr->FindCM(m_szCtrlMgrAlias );
	CString temp_m_szGridAlias ;
	temp_m_szGridAlias.Format (_T("%s"),m_szGridAlias);
	m_pGrid = (CESL_Grid*)(pCM->FindControl( temp_m_szGridAlias));
	m_pGrid->Display();

	return 0;
}

VOID CSearch::insert_data(CJsonReader* pSrcReader, INT RowCount)
{
EFS_BEGIN
	
    CString temp_m_szDataMgrAlias;
    temp_m_szDataMgrAlias.Format (_T("%s"),m_szDataMgrAlias);

	CESL_DataMgr *pDM = m_pDialogMgr->FindDM(temp_m_szDataMgrAlias);
	if (pDM == NULL) return;

	pDM->FreeData();

	INT nMapCount = m_KolisNetDataMapper.GetSize();	
	for (INT i=0; i<RowCount; i++)
	{
		pDM->InsertRow(-1);

		for (INT j=0; j<nMapCount; j++)
		{
			CString strTagName;
			CString strColName;

			m_KolisNetDataMapper.GetAt(j, strTagName, strColName);

			CString strData;
			pSrcReader->get_fdata_Value(strTagName, i, strData);

			pDM->SetCellData(strColName, strData, i);
		}		
	}

	return;

EFS_END
}
