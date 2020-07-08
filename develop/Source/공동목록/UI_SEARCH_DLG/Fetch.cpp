#include "stdafx.h"

#include "TextProgressCtrl.h"
#include "ESL_Mgr.h"

#include "KolisNetService.h"

#include "..\\..\\..\\include\\공동목록\\Search.h"
#include "sbl.h"


VOID QueryWorkerFunc_01 ( LPVOID WorkContext )
{
    TCHAR               szMessage[256];
    INT                 ids  , i , RowCount , ColCount , PageCnt ,PagePerCnt;
    INT                 TotalCnt = 0;

    DB_DATA_ARRAY_TYPE *pTempOuData = NULL;

    ptREQUEST_ELEMENT  pQueryElement =  (ptREQUEST_ELEMENT)WorkContext;
    CSearch *lpThis                  =  (CSearch *)pQueryElement->lpThis;

    lpThis->m_bRequestEndFlag    = FALSE;

    ids = lpThis->WorkerThreadStart( );
    if ( ids ) {
        lpThis->m_bRequestEndFlag    = TRUE;		
        return;
    }

    lpThis->ResultCountDisplay( 0 );

	BEGIN_SBL();

	CString temp_m_szDataMgrAlias;
    temp_m_szDataMgrAlias.Format (_T("%s"), lpThis->m_szDataMgrAlias);

	CESL_DataMgr *pResultDM = lpThis->m_pDialogMgr->FindDM(lpThis->m_szDataMgrAlias);
	if (pResultDM == NULL) return;
	
	if (!CKolisNetService::SearchKolisNet(lpThis->m_pDialogMgr, &lpThis->m_KolisNetSearchParam, pResultDM, &lpThis->m_KolisNetDataMapper))
	{
		lpThis->m_bRequestEndFlag    = TRUE;
        lpThis->WorkerThreadEnd( -1 );
		END_SBL(0);
        ::MessageBox( lpThis->m_pDialogMgr->GetSafeHwnd() , _T("자료찾기가 정상적으로 수행되지 못했습니다.") , _T("자료찾기"),  MB_ICONSTOP );
		return;
	}

	if (lpThis->m_pProgressBar != NULL) 
	{
		lpThis->m_pProgressBar->SetPos(0);
		lpThis->m_pProgressBar->SetRange(0,TotalCnt);
	}

	INT nCount = pResultDM->GetRowCount();

	for (i = 0; i < nCount; i++)
	{
		ids = lpThis->GridDisplayFunc(nCount);

		if (lpThis->m_pProgressBar != NULL)
		{
			_stprintf(szMessage , _T(" %5d건 검색 "), nCount);

			lpThis->m_pProgressBar->SetWindowText(szMessage);
			lpThis->m_pProgressBar->StepIt();
		}
		
		lpThis->ResultCountDisplay(nCount);
	}

	END_SBL(nCount);

    lpThis->WorkerThreadEnd( lpThis->m_ResultCnt );	

    lpThis->m_bRequestEndFlag    = TRUE;

	INT nCollectionType;
	CString strCollection;
	strCollection.Format(_T("%s"), lpThis->m_szCollection);
	if ( strCollection.Left(3) == _T("dan") )
	{
		nCollectionType = 0;
	}
	else if ( strCollection.Left(3) == _T("yon") )
	{
		nCollectionType = 1;
	}
	else
	{
		nCollectionType = 3;
	}

  
	lpThis->m_pDialogMgr->PostMessage( WM_THREAD_END , nCollectionType , 0 );     
}

VOID QueryWorkerFunc_02 ( LPVOID WorkContext )
{

}
