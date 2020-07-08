// RESEARCH_NEWMAKE_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "RESEARCH_NEWMAKE_DLG.h"
#include "RESEARCH_PREVIEW_DLG.h"


#include <wininet.h>
//#include _T("Afxdlgs.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_NEWMAKE_DLG dialog


CRESEARCH_NEWMAKE_DLG::CRESEARCH_NEWMAKE_DLG(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRESEARCH_NEWMAKE_DLG)
	//}}AFX_DATA_INIT

	m_pResearchInfoDM  = NULL ;
	m_pQuestionInfoDM  = NULL ;
	m_pQuestionDM      = NULL ;
	m_pMetrixInfoDM    = NULL ;
	m_pMetrixDM        = NULL ; 
 	m_nCurrentQuestionIdx = -1 ;
	m_bInsertMode = TRUE ;
	m_bResearchInfoModify = FALSE ;
	m_strResearchKey = _T("-1") ;

	// KOL.UDF.022 시큐어코딩 보완
	m_pEditTitle = NULL;
	m_pComboResultYN = NULL;
	m_pComboOnlineYN = NULL;
	m_pEditEndYY = NULL;
	m_pEditEndMM = NULL;
	m_pEditEndDD = NULL;
	m_pEditResponse = NULL;
	m_pDiscription = NULL;
	m_pEditSubTitle = NULL;
	m_pComboType = NULL;
	m_pComboCount = NULL;
	m_pGridSample = NULL;
	m_pGridQuestion = NULL;
	m_pComboMetrix = NULL;
	m_pStaticMetrix = NULL;
	m_pMetrixGrid = NULL;
	m_nQuestionCount = 0;
}

CRESEARCH_NEWMAKE_DLG::~CRESEARCH_NEWMAKE_DLG()
{
	
}
BOOL CRESEARCH_NEWMAKE_DLG::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CRESEARCH_NEWMAKE_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRESEARCH_NEWMAKE_DLG)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRESEARCH_NEWMAKE_DLG, CDialog)
	//{{AFX_MSG_MAP(CRESEARCH_NEWMAKE_DLG)
	ON_BN_CLICKED(IDC_bPREVIEW, OnbPREVIEW)
	ON_BN_CLICKED(IDC_bResearchInput, OnbResearchInput)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_QCount, OnSelchangeCOMBOQCount)
	ON_BN_CLICKED(IDC_bQuestionInput, OnbQuestionInput)
	ON_BN_CLICKED(IDC_bQuestionDelete, OnbQuestionDelete)
	ON_BN_CLICKED(IDC_bQuestionClear, OnbQuestionClear)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bQuestionModify, OnbQuestionModify)
	ON_BN_CLICKED(IDC_bResearchModify, OnbResearchModify)
	ON_BN_CLICKED(IDC_bOnlineTicket, OnbOnlineTicket)
	ON_CBN_SELCHANGE(IDC_COMBO_METRIC_QCount, OnSelchangeCOMBOMETRICQCount)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_NEWMAKE_DLG message handlers



VOID CRESEARCH_NEWMAKE_DLG::OnbPREVIEW() 
{
	// TODO: Add your control notification handler code here
	
	//++2008.09.04 ADD BY CJY {{++
	if(_T("-1") == m_strResearchKey)
	{
		AfxMessageBox (_T("설문지 정보를 먼저 입력하여 주십시요."));
		return ;
	}
	
 	MakeHtmlFile();
 	CRESEARCH_PREVIEW_DLG dlg(this) ;
 	dlg.SetHTMLFileName (m_strResearchKey ) ;
 	dlg.DoModal () ;
 
	// 바뀐내용 :
	// 기존소스에선 DB에 내용을 파일로 저장한후 읽어들어 보여주는 방식이라
	// DB에 데이터가 저장되지 않은 경우(예: 신규작성시)
	// 미리보기를 하면 에러가 발생하였다.
	// 수정된 소스에서는 현재 내용을 파일로 저장한 후
	// 그 내용을 보여주는 방식으로 변경하였다.
//DEL 	
//DEL 
//DEL 	INT ids = -1;
//DEL 	CFile file;
//DEL 	
//DEL 	INT     nFileSize ;
//DEL 	CString strFileName  ; /* = _T("..\\cfg\\webresearch\\TempPage.htm") ; */
//DEL 	strFileName.Format (_T("..\\cfg\\webresearch\\%s.htm"),m_strResearchKey) ;
//DEL 
//DEL 	nFileSize = _ttoi(m_pResearchInfoDM->GetCellData (_T("FILE_SIZE"),0)) ;
//DEL 
//DEL 
//DEL 	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
//DEL 	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
//DEL 	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
//DEL 	if (pBuffer == NULL)
//DEL 	{
//DEL 		MessageBox(_T("Malloc Memory Fail!"));
//DEL 		return ;
//DEL 	}
//DEL 
//DEL 
//DEL 	if(m_strResearchKey == -1 ) 
//DEL 	{
//DEL 		AfxMessageBox (_T("설문지 정보가 없습니다.")) ;
//DEL 		return ;
//DEL 	}
//DEL 
//DEL 	CESL_DataMgr dm ;
//DEL 	dm.SetCONNECTION_INFO (m_pResearchInfoDM->CONNECTION_INFO ) ;
//DEL 	dm.LOBGet (_T("RESEARCH_INFO_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strResearchKey,_T("FILE_DATA"),pBuffer,&nBufferSize) ;
//DEL 	
//DEL 		// 파일에 저장한다.
//DEL 	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) 
//DEL 	{
//DEL 		AfxMessageBox (_T("설문지를 다운로드 하지 못하였습니다.")) ;
//DEL 		return ;
//DEL 	}
//DEL 	
//DEL 	if( 0 == file.GetLength() )
//DEL 	{			
//DEL 		TCHAR cUni = 0xFEFF;
//DEL 		file.Write( &cUni, sizeof(TCHAR));
//DEL 	}
//DEL 		
//DEL 	file.WriteHuge(pBuffer, nBufferSize);
//DEL 	file.Close();	
//DEL 	free(pBuffer);
//DEL 
//DEL 
//DEL 	CRESEARCH_PREVIEW_DLG dlg(this) ;
//DEL 	dlg.SetHTMLFileName (m_strResearchKey ) ;
//DEL 	dlg.DoModal () ;
//DEL 
	//--2008.09.04 UPDATE BY CJY --}}

 	return;
}

BOOL CRESEARCH_NEWMAKE_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_RESEARCH_MAKE"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Failed"));
		return FALSE ;
	}
	
	if(m_bInsertMode == TRUE)
	{
		InitScreen();
	}
	else 
	{
		InitModifyScreen (0) ;
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CRESEARCH_NEWMAKE_DLG::InitScreen()
{


	InitControl () ;


	m_pComboResultYN->SetCurSel (0);
	m_pComboOnlineYN->SetCurSel (1);
	m_pComboType->SetCurSel (0);
	m_pComboCount->SetCurSel (0);


	CButton *pModifyButton = (CButton*)GetDlgItem(IDC_bResearchModify) ;
	pModifyButton->EnableWindow (FALSE) ;


	m_pResearchInfoDM = FindDM(_T("DM_RESEARCH_INFO")) ;
	if(m_pResearchInfoDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed"));
		return -1;
	}
	m_pQuestionInfoDM = FindDM(_T("DM_QUESTION_INFO")) ;
	if(m_pQuestionInfoDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed"));
		return -2;
	}

	m_pQuestionDM = FindDM(_T("DM_QUESTION")) ;
	if(m_pQuestionDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed"));
		return -3;
	}

	
	m_pMetrixInfoDM  = FindDM(_T("DM_METRIX_INFO")) ;
	if(m_pMetrixInfoDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed"));
		return -3;
	}

	
	m_pMetrixDM  = FindDM(_T("DM_METRIX_QUESTION")) ;
	if(m_pMetrixDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed"));
		return -3;
	}




	m_pResearchInfoDM->FreeData () ;
	m_pQuestionInfoDM->FreeData () ;
	m_pQuestionDM ->FreeData () ;

	m_pMetrixInfoDM->FreeData () ;
	m_pMetrixDM->FreeData () ;

	return 0 ;
}

VOID CRESEARCH_NEWMAKE_DLG::OnbResearchInput() 
{
	// TODO: Add your control notification handler code here
	
	CString strReckey =_T("") ;
	CString strTitle=_T("") ;
	CString strResultYN = _T("") ;
	CString strOnlineYN = _T("") ;
	CString strYY,strMM,strDD ;
	CString strEndDate  = _T("");
	CString strResponseTime = _T("");
	CString strDiscription = _T("") ;
	CString strCreateDate ;


	m_pEditTitle->GetWindowText (strTitle);	 ;
	m_pComboResultYN->GetWindowText (strResultYN ) ;
	m_pComboOnlineYN ->GetWindowText (strOnlineYN);
	m_pEditEndYY     ->GetWindowText (strYY);
	m_pEditEndMM     ->GetWindowText (strMM);
	m_pEditEndDD     ->GetWindowText (strDD);
	m_pEditResponse  ->GetWindowText (strResponseTime);
	m_pDiscription   ->GetWindowText (strDiscription );

	if(strYY.IsEmpty() || strMM.IsEmpty() || strDD.IsEmpty())
	{
		AfxMessageBox(_T("종료일을 적확하게 입력 하여 주십시요")) ;
		return ;
	}


	strEndDate.Format (_T("%s/%s/%s"),strYY,strMM,strDD) ;
	
	CTime t = CTime::GetCurrentTime () ;
	strCreateDate.Format (_T("%04d/%02d/%02d"),t.GetYear (),t.GetMonth (),t.GetDay ());


	m_pResearchInfoDM = FindDM(_T("DM_RESEARCH_INFO")) ;
	if(m_pResearchInfoDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed"));
		return ;
	}

	

	if(m_bInsertMode == TRUE)
	{
		if(m_pResearchInfoDM->GetRowCount () != 0)
		{
			m_strResearchKey = m_pResearchInfoDM->GetCellData (_T("REC_KEY"),0) ; 
		}
		m_pResearchInfoDM->FreeData () ;
	}

	m_pResearchInfoDM->InsertRow (-1) ;
	INT nRowIndex = m_pResearchInfoDM->GetRowCount ()-1 ;
	
	//++2008.09.04 UPDATE BY CJY {{++
	if(_T("-1") == m_strResearchKey)
	{
		m_pResearchInfoDM->MakeRecKey (m_strResearchKey) ;
	}	
	//             이전소스
	// if(m_strResearchKey == -1) 
	// m_pResearchInfoDM->MakeRecKey (m_strResearchKey) ;
	//--2008.09.04 UPDATE BY CJY --}}

	m_pResearchInfoDM->SetCellData (_T("REC_KEY"),m_strResearchKey ,nRowIndex );
	m_pResearchInfoDM->SetCellData (_T("TITLE"),strTitle,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("DISCRIPTION"),strDiscription ,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("RESULT_OPEN_YN"),strResultYN ,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("END_DATE"),strEndDate,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("ONLINE_TICKET"),strOnlineYN,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("CREATE_DATE"),strCreateDate,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("RESPONSE_TIME"),strResponseTime,nRowIndex) ;



}

VOID CRESEARCH_NEWMAKE_DLG::OnSelchangeComboType() 
{
	// TODO: Add your control notification handler code here
	CString strType ;
	//m_pComboType->GetWindowText (strType) ;
	INT nRow = m_pComboType->GetCurSel () ;

	if(nRow == 5) // 매크릭스형
	{
		m_pComboMetrix->ShowWindow (SW_SHOW);
		m_pStaticMetrix->ShowWindow (SW_SHOW);
		m_pMetrixGrid->ShowWindow (SW_SHOW) ;
		m_pGridSample->ShowWindow (SW_HIDE);
		m_pComboMetrix->SetCurSel (0) ;

	}
	else
	{
		m_pComboMetrix->ShowWindow (SW_HIDE);
		m_pStaticMetrix->ShowWindow (SW_HIDE);
		m_pMetrixGrid->ShowWindow (SW_HIDE) ;
		m_pGridSample->ShowWindow (SW_SHOW);
		//sample Grid Init
		//InitQuestionSampleGrid();
    }
}

VOID CRESEARCH_NEWMAKE_DLG::OnSelchangeCOMBOQCount() 
{
	// TODO: Add your control notification handler code here
	
	//sample Grid Init
	InitQuestionSampleGrid();
   
	
}

INT CRESEARCH_NEWMAKE_DLG::InitQuestionSampleGrid()
{
	
	CString strCount ;


	INT nRowCount = m_pComboCount->GetCurSel () ;

	m_pGridSample->SetRows (nRowCount+2) ;
	//m_pGridSample->SetRedraw(TRUE);

	m_pMetrixGrid ->SetRows (nRowCount+3) ;


	return 0 ;
}

VOID CRESEARCH_NEWMAKE_DLG::OnbQuestionInput() 
{
	// TODO: Add your control notification handler code here
	
	//1.QUESTION INFO
	
	//REC_KEY
	//RESEARCH_KEY
	//SUBTITLE
	//TYPE
	//CREATE_DATE
	//QUESTION_COUNT


	if(m_strResearchKey.IsEmpty () || m_strResearchKey == -1 )
	{
		AfxMessageBox (_T("설문지 정보를 먼저 입력하여 주십시요."));
		return ;
	}
	
	if(m_bResearchInfoModify == TRUE)
	{
		if(m_nCurrentQuestionIdx == -1)
		{
			AfxMessageBox (_T("질문을 선택하여 주십시요."));
			return ;
		}
		
		//m_pQuestionDM->FreeData () ;
	
	}

	CString strQuestionInfoKey ;
	CString strSubtitle;
	CString strType ;
	CString strCreateDate ;
	CString strQuestionCount;

	
	//질문 정보 control 
	m_pEditSubTitle->GetWindowText (strSubtitle )  ;
	m_pComboType->GetWindowText (strType )     ;
    m_pComboCount->GetWindowText (strQuestionCount )   ;


	if(strSubtitle.IsEmpty ())
	{
		AfxMessageBox (_T("질문 제목을 입력하여 주십시요."));
		return ;
	}

		
	if(strType == _T("서술형"))
	{
		if(_ttoi(strQuestionCount) >1)
		{
			AfxMessageBox(_T("서술형 질문유형의 항목은 1개 이상 선택 할 수 없습니다.")) ;
			return ;
		}
	}
	/*	단일선택형
		복수선택형
		서술형
		우선순위형
		절대점수형
		매트릭스형
    */
	if(strType == _T("단일선택형") || strType == _T("복수선택형") || strType == _T("우선순위형") || strType == _T("절대점수형") )
	{
		if(_ttoi(strQuestionCount) == 1 || _ttoi(strQuestionCount) == 0 )
		{
			AfxMessageBox(_T("서술형 질문유형의 항목은 1개 이상 선택 하셔야 합니다..")) ;
			return ;
		}
	}


	if(strType == _T("매트릭스형"))
	{
		InputMetrixQuestion () ;
		return ;
	}


	CTime t = CTime::GetCurrentTime () ;
	strCreateDate.Format (_T("%04d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay ());
	

	m_pQuestionInfoDM = FindDM(_T("DM_QUESTION_INFO"));
	if(m_pQuestionInfoDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed"));
		return ;
	}


	m_pQuestionInfoDM->InsertRow (-1) ;
	INT nQrowIndex = m_pQuestionInfoDM->GetRowCount () -1;

	m_pQuestionInfoDM->MakeRecKey (strQuestionInfoKey ) ;

	m_pQuestionInfoDM->SetCellData (_T("REC_KEY"),strQuestionInfoKey,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchKey,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("SUBTITLE"),strSubtitle,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("TYPE"),strType,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("CREATE_DATE"),strCreateDate,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("QUESTION_COUNT"),strQuestionCount,nQrowIndex ) ;



	//2.QUESTION
	
	//REC_KEY
	//QUESTION_KEY
	//QUESTION_TYPE
	//SAMPLE_TITLE
	//CREATE_DATE
	
	m_pGridSample ;

	CString strSampleReckey ;
	CString strSampleTitle ;
  
	
	m_pQuestionDM = FindDM(_T("DM_QUESTION"));
	if(m_pQuestionDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed"));
		return ;
	}

   
	for(INT i = 0 ; i< _ttoi(strQuestionCount) ;i++)
	{
		strSampleTitle = m_pGridSample->GetTextMatrix (i+1,2) ;
		
		if(strSampleTitle.IsEmpty ())
		{
			AfxMessageBox (_T("질문 항목의 내용을 입력하여 주십시요."));
			return ;
		}

		m_pQuestionDM->InsertRow (-1) ;
		m_pQuestionDM->MakeRecKey (	strSampleReckey ) ;
		
		INT nRowIndex = m_pQuestionDM->GetRowCount ()-1 ;
		
		m_pQuestionDM->SetCellData (_T("REC_KEY"),strSampleReckey,nRowIndex) ;
		m_pQuestionDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchKey,nRowIndex) ;
		m_pQuestionDM->SetCellData (_T("QUESTION_KEY"),strQuestionInfoKey,nRowIndex) ;
		m_pQuestionDM->SetCellData (_T("QUESTION_TYPE"),strType,nRowIndex) ;
		m_pQuestionDM->SetCellData (_T("SAMPLE_TITLE"),strSampleTitle,nRowIndex) ;
		m_pQuestionDM->SetCellData (_T("CREATE_DATE"),strCreateDate,nRowIndex) ;
		
		CString strTemp ;
		strTemp.Format (_T("%d"),i+1) ;
		m_pQuestionDM->SetCellData (_T("ALIAS"),strTemp,nRowIndex) ;
	}
	


	//질문 control
	m_pGridQuestion->SetRows(m_pQuestionInfoDM->GetRowCount ()+1) ;
	
	for(INT k = 0 ; k < m_pQuestionInfoDM->GetRowCount ();k++)
	{
		m_pGridQuestion->SetTextMatrix (k+1,1,m_pQuestionInfoDM->GetCellData (_T("SUBTITLE"),k)) ;
	}



}

VOID CRESEARCH_NEWMAKE_DLG::OnbQuestionDelete() 
{
	// TODO: Add your control notification handler code here
	
	

	if(m_nCurrentQuestionIdx == -1)
	{
		AfxMessageBox (_T("삭제 할 질문을 선택 하여 주십시요")) ;
		return ;
	}

	m_nCurrentQuestionIdx = m_pGridQuestion->GetIdx () ;

	if(AfxMessageBox (_T("질문을 삭제 하시겠습니까 ?"),IDOK ) != IDOK)
	{
		return ;
	}

	CString strQuestionInfoKey = m_pQuestionInfoDM->GetCellData (_T("REC_KEY"),m_nCurrentQuestionIdx );
	
	CString strType = m_pQuestionInfoDM->GetCellData (_T("TYPE"),m_nCurrentQuestionIdx );
	
	if(strType != _T("매트릭스형")) 
	{
		
		INT nQuestionCnt = m_pQuestionDM->GetRowCount () ;
		INT nRowCnt = nQuestionCnt-1 ;
		
		for( INT i = 0  ; i < nQuestionCnt  ; i++)
		{
			
			CString strTempKey = m_pQuestionDM->GetCellData (_T("QUESTION_KEY"),nRowCnt);
			
			if(strQuestionInfoKey == strTempKey )
			{
				m_pQuestionDM->DeleteRow (nRowCnt) ;
				//m_pQuestionInfoDM->RefreshPositionInfo () ;
			}
			nRowCnt-- ;
		}
		
		m_pQuestionInfoDM->DeleteRow (m_nCurrentQuestionIdx) ;
		m_pQuestionInfoDM->RefreshPositionInfo () ;
		
		//if(m_nCurrentQuestionIdx == 0) 
		//m_pGridQuestion->RemoveAt (1) ;

		if(m_nCurrentQuestionIdx == 0) 
		{
			m_pGridQuestion->SetTextMatrix (1,1,_T("")) ;
		}
		else
		{
			m_pGridQuestion->RemoveAt (m_nCurrentQuestionIdx) ;
		}
		

		OnbQuestionClear();
	}
	
	else if (strType == _T("매트릭스형"))
	{
		CString strQuestionInfoKey = m_pQuestionInfoDM->GetCellData (_T("REC_KEY"),m_nCurrentQuestionIdx );
		
		INT nMetrixInfoCnt = m_pMetrixInfoDM ->GetRowCount () ;
		
		INT nRowCnt = nMetrixInfoCnt-1 ;
		
		for( INT i = 0  ; i < nMetrixInfoCnt  ; i++)
		{
			
			CString strTempKey = m_pMetrixInfoDM->GetCellData (_T("QUESTION_INFO_KEY"),nRowCnt);
			
			if(strQuestionInfoKey == strTempKey )
			{
				
				//METRIX_QUESTION DELETE 
				CString strMetrixInfoKey = m_pMetrixInfoDM->GetCellData (_T("REC_KEY"),nRowCnt);
				INT nMetrixCnt = m_pMetrixDM->GetRowCount () ;
				INT nMetrixRowCnt = nMetrixCnt - 1;
                for (INT j = 0 ;j <nMetrixCnt ;j++)
				{
					CString strFindKey = m_pMetrixDM ->GetCellData (_T("METRIX_INFO_KEY"),nMetrixRowCnt) ;
					if(strMetrixInfoKey == strFindKey )
					{
						m_pMetrixDM->DeleteRow (nMetrixRowCnt) ;
						//m_pQuestionInfoDM->RefreshPositionInfo () ;
					}
					nMetrixRowCnt-- ;
					
				}
				
				m_pMetrixInfoDM->DeleteRow (nRowCnt) ;
				//m_pQuestionInfoDM->RefreshPositionInfo () ;
			}
			nRowCnt-- ;
		}
	
		m_pQuestionInfoDM->DeleteRow (m_nCurrentQuestionIdx) ;
		m_pQuestionInfoDM->RefreshPositionInfo () ;
		
		if(m_nCurrentQuestionIdx == 0) 
		{
			m_pGridQuestion->SetTextMatrix (1,1,_T("")) ;
		}
		else
		{
			m_pGridQuestion->RemoveAt (m_nCurrentQuestionIdx) ;
		}

		OnbQuestionClear();

	}

}

VOID CRESEARCH_NEWMAKE_DLG::OnbQuestionClear() 
{
	// TODO: Add your control notification handler code here
	m_pEditSubTitle->SetWindowText (_T(""));
	m_pComboType->SetCurSel (0);
	m_pComboCount->SetCurSel (0);
	
	m_pGridSample->SetRows (2);
	m_pGridSample->SetTextMatrix (1,2,_T(""));

	m_pComboMetrix ->SetCurSel (0) ;

	m_pMetrixGrid->SetRows (3);
	m_pMetrixGrid->SetCols (0,3) ;

	m_pComboMetrix->ShowWindow (SW_HIDE);
	m_pStaticMetrix->ShowWindow (SW_HIDE);

	//////////////////////////////////////////////////////////////////////////////
	//METRIX GRID INIT 

	m_pMetrixGrid->ShowWindow (SW_HIDE) ;
	m_pGridSample->ShowWindow (SW_SHOW);

}

VOID CRESEARCH_NEWMAKE_DLG::OnbSAVE() 
{
	// TODO: Add your control notification handler code here

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pResearchInfoDM ->CONNECTION_INFO);

	// 발송이후에는 수정을 할 수 없다.
	CString strSendDate;
	m_pResearchInfoDM->GetCellData (_T("SEND_DATE"),0,strSendDate) ;
	if(strSendDate != _T(""))
	{
		AfxMessageBox(_T("발송된 설문지는 수정 할 수 없습니다."));
		return ;
	}

	//*******************************************************************************
	//수정일 경우 모두 삭제 하도록 한다...
	if(m_bResearchInfoModify == TRUE)
	{	
		INT ids ;
		CString strResearchKey = m_pResearchInfoDM->GetCellData (_T("REC_KEY"),0) ;
		dm.StartFrame () ;
		dm.InitDBFieldData () ;
		//QUESTION_TBL
		ids = dm.MakeDeleteFrame (_T("QUESTION_TBL"),_T("RESEARCH_KEY"),_T("NUMERIC"),strResearchKey ) ;
		if(ids <0)
		{
			dm.EndFrame () ;
			return ;
		}
		//RESEARCH_QUESTION_INFO_TBL
		ids = dm.MakeDeleteFrame (_T("RESEARCH_QUESTION_INFO_TBL"),_T("RESEARCH_KEY"),_T("NUMERIC"),strResearchKey ) ;
		if(ids <0)
		{
			dm.EndFrame () ;
			return ;
		}

		//METRIX_QUESTION_INFO_TBL
		ids = dm.MakeDeleteFrame (_T("METRIX_QUESTION_INFO_TBL"),_T("RESEARCH_KEY"),_T("NUMERIC"),strResearchKey ) ;
		if(ids <0)
		{
			dm.EndFrame () ;
			return ;
		}

		//METRIX_TBL 
		ids = dm.MakeDeleteFrame (_T("METRIX_TBL"),_T("RESEARCH_KEY"),_T("NUMERIC"),strResearchKey ) ;
		if(ids <0)
		{
			dm.EndFrame () ;
			return ;
		}

		//RESEARCH_INFO_TBL
	
		dm.SendFrame () ;
		dm.EndFrame () ;

	}
	//*******************************************************************************

	//*******************************************************************************
	//Table insert 하기 
	


	CTime t = CTime::GetCurrentTime () ;
	CString strDate ;
	strDate.Format (_T("%04d/%02d/%02d"),t.GetYear (),t.GetMonth (),t.GetDay ());

	//////////////////////////////////////////
	dm.StartFrame () ;
	
	//*********************************************************************************************
	//RESEARCH_INFO_TBL
	if(m_bResearchInfoModify == FALSE)
	{
		//++2008.09.04 ADD BY CJY {{++
		CString strRecKey = m_pResearchInfoDM->GetCellData (_T("REC_KEY"), 0);
		strRecKey.TrimLeft();
		strRecKey.TrimRight();
		if(_T("") == strRecKey) 
		{
			AfxMessageBox(_T("먼저 설문지 정보를 작성하여 주십시요."));
			return;
		}
		//--2008.09.04 ADD BY CJY --}}

		dm.InitDBFieldData () ;
		dm.AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),m_pResearchInfoDM->GetCellData (_T("REC_KEY"),0));
		dm.AddDBFieldData (_T("TITLE"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("TITLE"),0));
		dm.AddDBFieldData (_T("DISCRIPTION"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("DISCRIPTION"),0));
		dm.AddDBFieldData (_T("RESULT_OPEN_YN"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("RESULT_OPEN_YN"),0));
		dm.AddDBFieldData (_T("END_DATE"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("END_DATE"),0));
		dm.AddDBFieldData (_T("RESPONSE_TIME"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("RESPONSE_TIME"),0));
		dm.AddDBFieldData (_T("ONLINE_TICKET"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("ONLINE_TICKET"),0));
		
		
		dm.AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),strDate);
		dm.AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),strDate);
		//dm.AddDBFieldData (_T("SEND_DATE"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("rec_key"),0));
		
		dm.MakeInsertFrame (_T("RESEARCH_INFO_TBL"));
    }
	else 
	{
		dm.InitDBFieldData () ;
	
		dm.AddDBFieldData (_T("TITLE"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("TITLE"),0));
		dm.AddDBFieldData (_T("DISCRIPTION"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("DISCRIPTION"),0));
		dm.AddDBFieldData (_T("RESULT_OPEN_YN"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("RESULT_OPEN_YN"),0));
		dm.AddDBFieldData (_T("END_DATE"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("END_DATE"),0));
		dm.AddDBFieldData (_T("RESPONSE_TIME"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("RESPONSE_TIME"),0));
		dm.AddDBFieldData (_T("ONLINE_TICKET"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("ONLINE_TICKET"),0));
		
		dm.AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),strDate);
		//dm.AddDBFieldData (_T("SEND_DATE"),_T("STRING"),m_pResearchInfoDM->GetCellData (_T("rec_key"),0));
		
		dm.MakeUpdateFrame  (_T("RESEARCH_INFO_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strResearchKey );
	}

	//*********************************************************************************************

	//*********************************************************************************************
	//QUESTION_INFO_TBL
	for(INT i = 0 ; i < m_pQuestionInfoDM->GetRowCount () ;i++)
	{
		dm.InitDBFieldData () ;
		dm.AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),m_pQuestionInfoDM->GetCellData (_T("REC_KEY"),i));
		dm.AddDBFieldData (_T("RESEARCH_KEY"),_T("NUMERIC"),m_pQuestionInfoDM->GetCellData (_T("RESEARCH_KEY"),i));
		dm.AddDBFieldData (_T("SUBTITLE"),_T("STRING"),m_pQuestionInfoDM->GetCellData (_T("SUBTITLE"),i));
		dm.AddDBFieldData (_T("DISCRIPTION"),_T("STRING"),m_pQuestionInfoDM->GetCellData (_T("DISCRIPTION"),i));
		dm.AddDBFieldData (_T("TYPE"),_T("STRING"),m_pQuestionInfoDM->GetCellData (_T("TYPE"),i));
		dm.AddDBFieldData (_T("QUESTION_COUNT"),_T("NUMERIC"),m_pQuestionInfoDM->GetCellData (_T("QUESTION_COUNT"),i));
		dm.AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),strDate);
		dm.AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),strDate);
		
		dm.MakeInsertFrame (_T("RESEARCH_QUESTION_INFO_TBL"));

	}
	//*********************************************************************************************


	//QUESTION_TBL
	for(INT k = 0 ; k < m_pQuestionDM->GetRowCount () ;k++)
	{
		dm.InitDBFieldData () ;
		dm.AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),m_pQuestionDM->GetCellData (_T("REC_KEY"),k));
		dm.AddDBFieldData (_T("RESEARCH_KEY"),_T("NUMERIC"),m_pQuestionDM->GetCellData (_T("RESEARCH_KEY"),k));
		dm.AddDBFieldData (_T("QUESTION_KEY"),_T("NUMERIC"),m_pQuestionDM->GetCellData (_T("QUESTION_KEY"),k));
		

		dm.AddDBFieldData (_T("SAMPLE_TITLE"),_T("STRING"),m_pQuestionDM->GetCellData (_T("SAMPLE_TITLE"),k));
		dm.AddDBFieldData (_T("QUESTION_TYPE"),_T("STRING"),m_pQuestionDM->GetCellData (_T("QUESTION_TYPE"),k));
		dm.AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),strDate);
		dm.AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),strDate);
		
		dm.AddDBFieldData (_T("ALIAS"),_T("STRING"),m_pQuestionDM->GetCellData (_T("ALIAS"),k));

		dm.MakeInsertFrame (_T("QUESTION_TBL"));
	}
	//////////////////////////////////////////

	//METRIX INFO TBL
	for( k = 0 ; k < m_pMetrixInfoDM->GetRowCount () ;k++)
	{
		dm.InitDBFieldData () ;
		dm.AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),m_pMetrixInfoDM->GetCellData (_T("REC_KEY"),k));
		dm.AddDBFieldData (_T("RESEARCH_KEY"),_T("NUMERIC"),m_pMetrixInfoDM->GetCellData (_T("RESEARCH_KEY"),k));
		dm.AddDBFieldData (_T("QUESTION_INFO_KEY"),_T("NUMERIC"),m_pMetrixInfoDM->GetCellData (_T("QUESTION_INFO_KEY"),k));
		
		dm.AddDBFieldData (_T("SUBTITLE"),_T("STRING"),m_pMetrixInfoDM->GetCellData (_T("SUBTITLE"),k));
		dm.AddDBFieldData (_T("TYPE"),_T("STRING"),m_pMetrixInfoDM->GetCellData (_T("TYPE"),k));
		dm.AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),strDate);
		dm.AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),strDate);
		
		//QUESTION_COUNT
		//METRIX_COUNT
		dm.AddDBFieldData (_T("QUESTION_COUNT"),_T("STRING"),m_pMetrixInfoDM->GetCellData (_T("QUESTION_COUNT"),k));
		dm.AddDBFieldData (_T("METRIX_COUNT"),_T("STRING"),m_pMetrixInfoDM->GetCellData (_T("METRIX_COUNT"),k));
		
		//dm.AddDBFieldData (_T("ALIAS"),_T("STRING"),m_pQuestionDM->GetCellData (_T("ALIAS"),k));

		dm.MakeInsertFrame (_T("METRIX_QUESTION_INFO_TBL"));
	}


	//METRIX TBL 
	//METRIX INFO TBL
	for( k = 0 ; k < m_pMetrixDM->GetRowCount () ;k++)
	{
		dm.InitDBFieldData () ;
		dm.AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),m_pMetrixDM->GetCellData (_T("REC_KEY"),k));
		dm.AddDBFieldData (_T("RESEARCH_KEY"),_T("NUMERIC"),m_pMetrixDM->GetCellData (_T("RESEARCH_KEY"),k));
		dm.AddDBFieldData (_T("QUESTION_INFO_KEY"),_T("NUMERIC"),m_pMetrixDM->GetCellData (_T("QUESTION_INFO_KEY"),k));
		dm.AddDBFieldData (_T("METRIX_INFO_KEY"),_T("NUMERIC"),m_pMetrixDM->GetCellData (_T("METRIX_INFO_KEY"),k));

		dm.AddDBFieldData (_T("SAMPLE_TITLE"),_T("STRING"),m_pMetrixDM->GetCellData (_T("SAMPLE_TITLE"),k));
		dm.AddDBFieldData (_T("QUESTION_TYPE"),_T("STRING"),m_pMetrixDM->GetCellData (_T("QUESTION_TYPE"),k));
		dm.AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),strDate);
		dm.AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),strDate);
		
		dm.AddDBFieldData (_T("ALIAS"),_T("STRING"),m_pQuestionDM->GetCellData (_T("ALIAS"),k));

		dm.MakeInsertFrame (_T("METRIX_TBL"));
	}



	dm.SendFrame ();	
	dm.EndFrame () ;
	
	//*******************************************************************************

	//******************************************************************************* 
	//HTML 파일 만들고 저장 
	
	MakeHtmlFile() ;
	//*******************************************************************************

	//*******************************************************************************
	//FILE UPDATE
	// 파일을 통채로 BLOB으로 DB에 올린다.
	CStdioFile file;
	INT nFileSize = 0;
	CString strFilePath ; //= _T("..\\cfg\\webresearch\\TempPage.htm");

	strFilePath.Format(_T("..\\cfg\\webresearch\\%s.htm"),m_strResearchKey ) ; 

	if (!file.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		MessageBox(_T("파일을 여는데 실패했습니다."));
		return ;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	nFileSize = file.GetLength();
	BYTE *pBuffer = (BYTE*)malloc(sizeof(BYTE) * (nFileSize) + 1024);
	ZeroMemory(pBuffer, nFileSize);
	nFileSize = file.ReadHuge(pBuffer, nFileSize);
	file.Close();

	// 파일

	INT ids = dm.LOBUpdate(_T("RESEARCH_INFO_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_strResearchKey, _T("FILE_DATA"), pBuffer, nFileSize);
	if (ids < 0)
	{
		MessageBox(_T("변경적용 실패"));
		return ;
	}
	
	// 파일길이, 버전정보 업데이트 한다.
	
	ids = dm.StartFrame();
	CString strQuery;
	dm.InitDBFieldData () ;
	strQuery.Format(_T("UPDATE RESEARCH_INFO_TBL SET FILE_SIZE = %d WHERE REC_KEY = %s;") ,nFileSize, m_strResearchKey);
	dm.AddFrame(strQuery);
	ids = dm.SendFrame();
	if (ids < 0) 
	{
		MessageBox(_T("변경적용 실패했습니다."));
	}
	ids = dm.EndFrame();
	if (ids < 0) 
	{
		MessageBox(_T("변경적용 실패했습니다."));
	}
    
	CString strFileSize ;
	strFileSize.Format (_T("%d"),nFileSize) ;
	m_pResearchInfoDM->SetCellData (_T("FILE_SIZE"),strFileSize ,0) ;


	// 메모리 해제
	free(pBuffer);


	//******************************************************************************
	//FTP 에 파일 올리기 ...

    
	if(SendTOFTP() == FALSE) 
	{
		AfxMessageBox (_T("Web Sever 에 파일이 업로드 되지 않았습니다.\r\n관리자에게 문의 하여 주십시요."));
	}


	//******************************************************************************


	//*******************************************************************************
	AfxMessageBox(_T("파일 저장이 성공 하였습니다.")) ;

	CButton * pButton = (CButton*)GetDlgItem (IDC_bSAVE);
	pButton->EnableWindow (FALSE);
	//CDialog::OnOK();
}

INT CRESEARCH_NEWMAKE_DLG::MakeHtmlFile()
{
	
	CString strFileName ;
	
	//strFileName = _T("..\\cfg\\webresearch\\TempPage.htm") ;
	strFileName.Format (_T("..\\cfg\\webresearch\\%s.htm"),m_strResearchKey) ;


	CStdioFile file ;
	if(!file.Open (strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) )
	{
		AfxMessageBox (_T("Make HTML Failed")) ;
		return -1 ;
	}
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	//file.WriteString (_T("<html><body><p><p>")) ;
	file.WriteString (_T("<html><head><style type='text/css'><!-- p,br,body,td,select,input,form,textarea,option {font-size:8.9pt;font-family:굴림;}--></style></head><body><p><p>\r\n")) ;
	/*
		<head><style type=_T("text/css")><!-- p,br,body,td,select,input,form,textarea,option {font-size:8.9pt;font-family:돋움;}--></style></head>
    */
	//file.WriteString(_T("<SCRIPT LANGUAGE = 'JavaScript'> alert('scipt test') </SCRIPT>"));
    
    GetFtpAccountCfg();
	CString strTemp ;
	//strTemp.Format (_T("<FORM METHOD = post NAME = RESEARCH ACTION = http://%s/MailCheck/research.php   onsubmit = 'chk();' ><p><p>\r\n"),m_strgSever ) ;
    strTemp.Format (_T("<FORM METHOD = post NAME = RESEARCH ACTION = http://%s/MailCheck/research.php  ><p><p>\r\n"),m_strgSever ) ;

	//file.WriteString (_T("<FORM METHOD = post NAME = RESEARCH ACTION = http://192.168.1.230/Mailcheck/research.php   onsubmit = 'chk();' ><p><p>")) ;
    file.WriteString (strTemp) ;

	//JAVA Script 생성 
	MakeHTML_ScriptHeader (m_strResearchKey ) ;

	//설문정보 생성 
	MakeHTML_ResearchInfo(file) ;


	//질문 생성 
	MakeHTML_QuestionInfo(file) ;


	file.WriteString (_T("<p><input type = hidden name = output ><p>\r\n")) ;

    //file.WriteString (_T("<p><input type = submit value = 작성완료 ><input type = reset><p>\r\n")) ;
	file.WriteString (_T("<p><input type= button value = '[ 보내기 ]' onClick = 'javascript:chk()' style='border:solid 1 #ababab;'>&nbsp&nbsp;<input type = reset><p>\r\n")) ;
	file.WriteString (_T("</form><p><p>")) ;

	//JAVA Script 생성완료 
	MakeHTML_ScriptEnd () ;

	file.WriteString (m_strJavaScriptStream);

	if(m_pResearchInfoDM->GetCellData (_T("ONLINE_TICKET"),0) == _T("YES"))
	{
		CString strOnlineticketImage ;
		CString strTag ;
		strOnlineticketImage.Format (_T("http://%s/mailcheck/%s.jpg"),m_strgSever ,m_strResearchKey );
		//<IMG height=104 src=_T("http://mailimg.hanmail.net/hm/emo/gol_14.gif") width=254>
		strTag.Format (_T("<img src = %s  width = 500 height = 300 >\r\n"),strOnlineticketImage) ;
		file.WriteString (strTag);
	}
	

	file.WriteString (_T("<p><p></body></html>\r\n")) ;

    
	file.Close () ;

	return 0 ;
}

INT CRESEARCH_NEWMAKE_DLG::MakeHTML_ResearchInfo(CStdioFile &file)
{
	

	INT nRowIndex = 0 ;
    
	file.WriteString(_T("==================================================================="));
	file.WriteString (_T("<br><B>"));
	file.WriteString (_T("<FONT SYTLE = 돋움 SIZE = 2 >")) ;
	//file.WriteString (m_pResearchInfoDM->GetCellData (_T("REC_KEY"),nRowIndex ) );
	//file.WriteString (_T("<br>"));
	file.WriteString (_T("[ 설문 제목 ] : ") + m_pResearchInfoDM->GetCellData (_T("TITLE"),nRowIndex) );
	file.WriteString (_T("<br><br>"));
	file.WriteString (m_pResearchInfoDM->GetCellData (_T("DISCRIPTION"),nRowIndex) );
	file.WriteString (_T("<br><br>"));

	file.WriteString (_T("[ 설문 종료일 ]: ") + m_pResearchInfoDM->GetCellData (_T("END_DATE"),nRowIndex)) ;
	file.WriteString (_T("<br><br>"));

	file.WriteString (m_pResearchInfoDM->GetCellData (_T("RESPONSE_TIME"),nRowIndex) );

	file.WriteString (_T("</FONT>")) ;
	file.WriteString (_T("</B>")) ;
	file.WriteString(_T("==================================================================="));
	file.WriteString (_T("<br>"));

	return 0 ;
}

INT CRESEARCH_NEWMAKE_DLG::MakeHTML_QuestionInfo(CStdioFile &file)
{

	//file.WriteString (_T("라디오 버튼 예제 : <INPUT TYPE = RADIO NAME = Q_R_01 VALUE = 1 > 한국")) ;
	//체크박스  예제 :    <INPUT TYPE = CHECKBOX  NAME = Q_C_02_1 VALUE = 1 > 체크 1 <BR>
	/*
	//선택형 tag 사용 예제 :
	
						  <SELECT NAME = Q_S_03>
						  <OPTION VALUE=사과>사과 </OPTION>
						  <OPTION VALUE=수박>수박 </OPTION>
						  <OPTION VALUE=딸기>딸기 </OPTION>
						  <OPTION VALUE=귤>귤</OPTION>
						  <OPTION VALUE=배>배 </OPTION>
						  </SELECT>년 
	*/


	//서술형   :          <TEXTAREA ROWS = 5 COLS = 50 NAME = Q_T_04 > </TEXTAREA>


	INT nQrowIndex = 0;
	for( INT i = 0 ; i < m_pQuestionInfoDM->GetRowCount () ; i++)
	{
		nQrowIndex = i ;

		file.WriteString (_T("<br>"));
		file.WriteString(_T("===================================================================\r\n"));
		file.WriteString (_T("<br>"));
		file.WriteString (_T("<b>"));
		file.WriteString ( m_pQuestionInfoDM->GetCellData (_T("SUBTITLE"),nQrowIndex )) ;
		file.WriteString (_T("</b>"));
		file.WriteString (_T("<br>"));
	
		CString strRecKey = m_pQuestionInfoDM->GetCellData (_T("REC_KEY"),nQrowIndex);
		CString strType   = m_pQuestionInfoDM->GetCellData (_T("TYPE"),nQrowIndex ) ;

		m_nQuestionCount  = _ttoi(m_pQuestionInfoDM->GetCellData (_T("QUESTION_COUNT"),nQrowIndex) ) ;

		MakeHTML_Question(file,strType,strRecKey) ;

		file.WriteString (_T("<br>"));
		file.WriteString(_T("===================================================================\r\n"));
		file.WriteString (_T("<br>"));

	}





	return 0 ;
}

INT CRESEARCH_NEWMAKE_DLG::MakeHTML_Question(CStdioFile &file,CString strType,CString strQuesioninfoKey)
{

	/*
		단일선택형
		복수선택형
		서술형
		우선순위형
		절대점수형
		매트릭스형
    */


	CString strTagData ;
	if(strType == _T("단일선택형"))
	{

		CString strFind ;
		CString strSample ;
		INT nCount = 0 ;
		for(INT i = 0 ; i < m_pQuestionDM->GetRowCount () ; i++)
		{
			strFind = m_pQuestionDM->GetCellData (_T("QUESTION_KEY"),i) ;
			if(strFind == strQuesioninfoKey )
			{
				strSample = m_pQuestionDM->GetCellData (_T("SAMPLE_TITLE"),i) ;
				//strTagData.Format (_T("<INPUT TYPE = RADIO NAME = %s VALUE = 1 > %s"),strQuesioninfoKey,strSample);
				strTagData.Format (_T("<INPUT TYPE = RADIO NAME = Q_%s VALUE = %d > %s \r\n"),strQuesioninfoKey,i+1,strSample);
				file.WriteString (_T("<br>")) ;
				file.WriteString (strTagData) ;
				nCount++ ;
			}
		}
		
		MakeHTML_ScriptBody (strType,strQuesioninfoKey,nCount) ;

	
	}
	if(strType == _T("복수선택형"))
	{
		//<INPUT TYPE = CHECKBOX  NAME = Q_C_02_1 VALUE = 10 > 체크 1 <BR>
		CString strFind ;
		CString strSample ;
		INT nCount = 0 ;
		for(INT i = 0 ; i < m_pQuestionDM->GetRowCount () ; i++)
		{
			strFind = m_pQuestionDM->GetCellData (_T("QUESTION_KEY"),i) ;
			if(strFind == strQuesioninfoKey )
			{
				strSample = m_pQuestionDM->GetCellData (_T("SAMPLE_TITLE"),i) ;
				//strTagData.Format (_T("<INPUT TYPE = RADIO NAME = %s VALUE = 1 > %s"),strQuesioninfoKey,strSample);
				strTagData.Format (_T("<INPUT TYPE = CHECKBOX NAME = Q_%s VALUE = %d > %s \r\n"),strQuesioninfoKey,i+1,strSample);
				file.WriteString (_T("<br>")) ;
				file.WriteString (strTagData) ;
				nCount++ ;
			}

		}

		MakeHTML_ScriptBody (strType,strQuesioninfoKey,nCount) ;

	}
	if(strType == _T("서술형"))
	{
		//<TEXTAREA ROWS = 5 COLS = 50 NAME = Q_T_04 > </TEXTAREA>
		CString strFind ;
		CString strSample ;
		for(INT i = 0 ; i < m_pQuestionDM->GetRowCount () ; i++)
		{
			strFind = m_pQuestionDM->GetCellData (_T("QUESTION_KEY"),i) ;
			if(strFind == strQuesioninfoKey )
			{
				strSample = m_pQuestionDM->GetCellData (_T("SAMPLE_TITLE"),i) ;
				//strTagData.Format (_T("<INPUT TYPE = RADIO NAME = %s VALUE = 1 > %s"),strQuesioninfoKey,strSample);
				strTagData.Format (_T("<TEXTAREA ROWS = 5 COLS = 50 NAME = Q_%s > </TEXTAREA>\r\n"),strQuesioninfoKey);
				file.WriteString (_T("<br>")) ;
				file.WriteString (strTagData) ;

				MakeHTML_ScriptBody (strType,strQuesioninfoKey,i+1) ;
			}

		}

	}
	if(strType == _T("우선순위형"))
	{
		//QUESTIONINFO KEY를가지고 .. NAME 에는 SAMPLE KEY 를 입력한다...
		CString strRecKey;
		CString strFind ;
		CString strSample ;
		for(INT i = 0 ; i < m_pQuestionDM->GetRowCount () ; i++)
		{
			strFind = m_pQuestionDM->GetCellData (_T("QUESTION_KEY"),i) ;
			if(strFind == strQuesioninfoKey )
			{

				strRecKey = m_pQuestionDM->GetCellData (_T("REC_KEY"),i) ;
				strSample = m_pQuestionDM->GetCellData (_T("SAMPLE_TITLE"),i) ;
				//strTagData.Format (_T("<INPUT TYPE = RADIO NAME = %s VALUE = 1 > %s"),strQuesioninfoKey,strSample);
				//strTagData.Format (_T("<TEXTAREA ROWS = 5 COLS = 50 NAME = %s > </TEXTAREA>"),strQuesioninfoKey);
				CString strOption = _T("");
				CString strTempOption = _T("") ;
								
				for(INT k = 0 ;  k <m_nQuestionCount ; k++)
				{   
					strTempOption.Format (_T("<OPTION VALUE=%d>%d순위</OPTION>\r\n"),k+1,k+1) ;
					strOption += strTempOption ;
				}
				strTagData.Format(_T("<SELECT NAME = Q_%s> %s </SELECT>%s \r\n"),strRecKey,strOption,strSample ) ;
				
				file.WriteString (_T("<br>")) ;
				file.WriteString (strTagData) ;

				MakeHTML_ScriptBody_SelectTag (strType,strQuesioninfoKey,strRecKey,k) ;
			}

		}

	}
	if(strType == _T("매트릭스형"))
	{
		
		CString strFind ;
		CString strSample ;
		INT nCount = 0 ;
	
		file.WriteString (_T("<table width = 261 border = 2 align = left cellpadding= 1  cellspacing= 1>\r\n")) ;
		//file.WriteString (_T("<tr><td>   </td><td>dddd</td><td>sdfsad</td><td>dddd</td></tr>"));
	    
		bool bTitle = FALSE ;

		//매트릭스형 제목 만들기 
		for(INT i = 0 ; i < m_pMetrixInfoDM->GetRowCount () ; i++)
		{

			strFind = m_pMetrixInfoDM->GetCellData (_T("QUESTION_INFO_KEY"),i) ;
			if(strFind == strQuesioninfoKey )
			{
				if(bTitle == true ) break ;

				CString strMetricInfoKey = m_pMetrixInfoDM->GetCellData (_T("REC_KEY") ,i) ;
				CString strMetricInfotitle = m_pMetrixInfoDM->GetCellData (_T("SUBTITLE") ,i) ;
				
				CString strMetrixCnt = m_pMetrixInfoDM->GetCellData (_T("METRIX_COUNT") ,i) ;
				INT nMetrixCnt = _ttoi(strMetrixCnt ) ;
				

				/****************************************************************************/
				//매트릭스형 제목 만들기 
				if(bTitle == FALSE)
				{
					CStringArray arrSampletitle ;
					arrSampletitle.RemoveAll () ;
					for(INT k = 0 ; k < m_pMetrixDM->GetRowCount () ; k++)
					{
						
						CString strFind = m_pMetrixDM->GetCellData (_T("METRIX_INFO_KEY"),k);
						if(strMetricInfoKey == strFind )
						{
							arrSampletitle.Add (m_pMetrixDM->GetCellData (_T("SAMPLE_TITLE"),k)) ;
							nMetrixCnt-- ;
						}
						if(nMetrixCnt == 0) break ;
						
					}
					
					file.WriteString (_T("<TR>")) ;
					file.WriteString(_T("<TD>제목/항목</TD>"));
					for( k = 0 ; k < arrSampletitle.GetSize () ;k++)
					{
						CString strTemp ;
						/*
						if(k == 0)
						{
							file.WriteString (_T("<TD></TD>")) ;
						}
                        */
						strTemp.Format (_T("<TD>%s</TD>"),arrSampletitle.GetAt (k)) ;
						file.WriteString (strTemp) ;
						
					}
					//file.WriteString (_T("</TR>")) ;
					bTitle = TRUE ;
				}
			
			}
		
		}

		for( i = 0 ; i < m_pMetrixInfoDM->GetRowCount () ; i++)
		{

			strFind = m_pMetrixInfoDM->GetCellData (_T("QUESTION_INFO_KEY"),i) ;
			if(strFind == strQuesioninfoKey )
			{
				CString strMetricInfoKey = m_pMetrixInfoDM->GetCellData (_T("REC_KEY") ,i) ;
				CString strMetricInfotitle = m_pMetrixInfoDM->GetCellData (_T("SUBTITLE") ,i) ;
				
				CString strMetrixCnt = m_pMetrixInfoDM->GetCellData (_T("METRIX_COUNT") ,i) ;
				INT nMetrixCnt = _ttoi(strMetrixCnt ) ;
				
				/****************************************************************************/
				//매트릭스형 내용 만들기 
				file.WriteString (_T("<tr>"));
				//제목 
				CString strTemp ;
				strTemp.Format (_T("<td>%s</td>"),strMetricInfotitle) ;
				file.WriteString (strTemp); 

				//내용 
				for(INT i = 0 ; i < _ttoi(strMetrixCnt ) ; i++)
				{
					strTemp.Format (_T("<TD><INPUT TYPE = RADIO NAME = Q_%s VALUE = %d ></TD>\r\n"),strMetricInfoKey,i+1) ;
					file.WriteString (strTemp); 
					nCount++ ;
				}

                file.WriteString (_T("</tr>")); 
				/****************************************************************************/
				MakeHTML_ScriptBody_Metrix(_T("매트릭스형"),strQuesioninfoKey,strMetricInfoKey,_ttoi(strMetrixCnt));

			}
		
		}
		
		file.WriteString (_T("</table>")) ;
	    for(INT k = 0 ; k < nCount+1 ; k++)
		{
			file.WriteString (_T("<br>")) ;
		}

	}
	if(strType == _T("절대점수형"))
	{
		CString strFind ;
		CString strSample ;
		CString strRecKey ;
		INT nCount = 0 ;
		for(INT i = 0 ; i < m_pQuestionDM->GetRowCount () ; i++)
		{
			strRecKey = m_pQuestionDM->GetCellData (_T("REC_KEY"),i) ;
			strFind = m_pQuestionDM->GetCellData (_T("QUESTION_KEY"),i) ;
			if(strFind == strQuesioninfoKey )
			{
				strSample = m_pQuestionDM->GetCellData (_T("SAMPLE_TITLE"),i) ;
				//strTagData.Format (_T("<INPUT TYPE = RADIO NAME = %s VALUE = 1 > %s"),strQuesioninfoKey,strSample);
				strTagData.Format (_T("%s    &nbsp;&nbsp;<INPUT TYPE = TEXT NAME = Q_%s  SIZE = 3 >점 \r\n"),strSample,strRecKey);
				file.WriteString (_T("<br>")) ;
				file.WriteString (strTagData) ;
				MakeHTML_ScriptBody (strType,strQuesioninfoKey,_ttoi(strRecKey)) ;
				nCount++ ;
			}
		}

	}

	return 0 ;
}

bool CRESEARCH_NEWMAKE_DLG::MakeHTML_ScriptHeader(CString strResearchKey)
{
	
	m_strJavaScriptStream = _T("") ;

	//m_strJavaScriptStream.Format (_T("<SCRIPT LANGUAGE = 'JavaScript'> function chk(){ var temp; var result = '%s';  \r\n"),strResearchKey) ;
    m_strJavaScriptStream.Format (_T("<SCRIPT LANGUAGE = 'JavaScript'> function chk(){ var temp; var result = '%s';  var bCheck = false ; \r\n"),strResearchKey) ;

	return TRUE;
}

bool CRESEARCH_NEWMAKE_DLG::MakeHTML_ScriptBody(CString strType, CString strQuestionInfo_Key, INT nCount)
{

	/////////////////////////////////////////////////////////////////////////////
	//RADIO BUTTON
	//[TYPE,QUESTION_INFO_KEY,SELECT]
	if(strType == _T("단일선택형")) 
	{
		CString strRadioScript = _T("");
		strRadioScript.Format (_T(" bCheck = false ; ")
			                   _T(" for(i = 0 ; i < %d ; i++) { ")
							   _T(" if (document.RESEARCH.Q_%s[i].checked) { ")
							   _T(" var select = i+1 ; ")
							   _T(" var strdata = '['+ '%s ;'+ '%s ;' + select  +']' ; ") 
							   _T(" result =  result + strdata ; ")
							   _T(" bCheck = true ; ")
							   _T(" }} \r\n if(bCheck == false ) { alert ('항목이 선택되지 않았습니다.'); return; }")
							   ,nCount,strQuestionInfo_Key,strType,strQuestionInfo_Key) ;
		
		m_strJavaScriptStream += strRadioScript ;
		return TRUE ;

	}
	/////////////////////////////////////////////////////////////////////////////
	//CHECK BOX
	//[TYPE,QUESTION_INFO_KEY,SELECT]
	if(strType == _T("복수선택형"))
	{
		//if (document.RESEARCH.QTEST[i].checked == true ) 
		CString strCheckScript = _T("");
		strCheckScript.Format (_T(" bCheck = false ; ")
			                   _T(" for(i = 0 ; i < %d ; i++) { ")
							   _T(" if (document.RESEARCH.Q_%s[i].checked == true ) { ")
							   _T(" var select = i+1 ; ")
							   _T(" var strdata = '['+ '%s ;' + '%s ;' + select  +']' ; ") 
							   _T(" result =  result + strdata ; ")
							    _T(" bCheck = true ; ")
							   _T(" }} \r\n \r\n if(bCheck == false ) { alert ('항목이 선택되지 않았습니다.'); return; }") 
							   ,nCount,strQuestionInfo_Key,strType,strQuestionInfo_Key) ;

		m_strJavaScriptStream += strCheckScript ;
		return TRUE ;

	}
	/////////////////////////////////////////////////////////////////////////////
	//TEXT BOX
	//[TYPE,QUESTION_INFO_KEY,DISC]
	if(strType == _T("서술형"))
	{
		CString strTextScript = _T("");
		//var result1 = document.RESEARCH.Q63241736.value ;
		strTextScript.Format (_T(" var strdisc = document.RESEARCH.Q_%s.value ; ")
			                  _T(" var strdata = '['+ '%s ;' + '%s ;' +  strdisc  +']' ; ") 
							  _T(" result =  result + strdata ; \r\n")
							  //_T(" alert (result) ; ")
							  , strQuestionInfo_Key,strType,strQuestionInfo_Key) ;

		m_strJavaScriptStream += strTextScript ;
		return TRUE ;

	}
	
	if(strType == _T("절대점수형"))
	{
		CString strTextScript = _T("");
		//var result1 = document.RESEARCH.Q63241736.value ;
		strTextScript.Format (_T(" var strdisc = document.RESEARCH.Q_%d.value ; ")
			                  //_T(" alert (result); ")
			                  _T(" var strdata = '['+ '%s ;' + '%s ;' +  strdisc + ' ; %d ' +']' ; ") 
							  _T(" result =  result + strdata ; \r\n")
							  //_T(" alert (result) ; ")
							  , nCount,strType,strQuestionInfo_Key,nCount) ;

		m_strJavaScriptStream += strTextScript ;
		return TRUE ;
	}


	return TRUE;
}

bool CRESEARCH_NEWMAKE_DLG::MakeHTML_ScriptBody_SelectTag(CString strType, CString strQuestionInfo_Key, CString strSampleKey, INT nCount)
{

	if(strType == _T("우선순위형"))
	{
		//[TYPE,QUESTION_INFO_KEY,SAMPLE_KEY,SELECT]
	
		CString strSelectScript = _T("");
		strSelectScript.Format (_T(" var choice = document.RESEARCH.Q_%s; ") //sample_key
								_T(" var data = choice.options[choice.selectedIndex].value; ")
								_T(" var strdata = '['+ '%s ;'+'%s ;' +'%s ;'+ data +']' ; ") 
								_T(" result =  result + strdata ; \r\n")
								,strSampleKey,strType,strQuestionInfo_Key,strSampleKey) ;
		
		m_strJavaScriptStream += strSelectScript ;
		return TRUE ;
	}


	return TRUE;
}

bool CRESEARCH_NEWMAKE_DLG::MakeHTML_ScriptEnd()
{
	//m_strJavaScriptStream += _T("alert (result) ; document.RESEARCH.output.value = result ; document.RESEARCH.submit() ; } </SCRIPT><BR>") ;
	m_strJavaScriptStream += _T(" document.RESEARCH.output.value = result ; document.RESEARCH.submit() ; } </SCRIPT><BR> \r\n") ;
	return TRUE ;
}

BEGIN_EVENTSINK_MAP(CRESEARCH_NEWMAKE_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CRESEARCH_NEWMAKE_DLG)
	ON_EVENT(CRESEARCH_NEWMAKE_DLG, IDC_QUESTION_GRID, -601 /* DblClick */, OnDblClickQuestionGrid, VTS_NONE)
	ON_EVENT(CRESEARCH_NEWMAKE_DLG, IDC_QUESTION_METRIX_GRID, -601 /* DblClick */, OnDblClickQuestionMetrixGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CRESEARCH_NEWMAKE_DLG::OnDblClickQuestionGrid() 
{
	// TODO: Add your control notification handler code here
	
	m_nCurrentQuestionIdx = m_pGridQuestion->GetIdx () ;
	
	CString strQuestionInfoKey = m_pQuestionInfoDM->GetCellData (_T("REC_KEY"),m_nCurrentQuestionIdx );
	CString strSubTitle = m_pQuestionInfoDM->GetCellData (_T("SUBTITLE"),m_nCurrentQuestionIdx );
    CString strType     = m_pQuestionInfoDM->GetCellData (_T("TYPE"),m_nCurrentQuestionIdx );
	CString strCount     = m_pQuestionInfoDM->GetCellData (_T("QUESTION_COUNT"),m_nCurrentQuestionIdx );

	if(strQuestionInfoKey.IsEmpty ()) return ;


	m_pEditSubTitle->SetWindowText (strSubTitle);
	m_pComboCount->SelectString (0,strCount);
	m_pComboType ->SelectString (0,strType) ;
	
	
	if(strType != _T("매트릭스형"))
	{
		///////////////////////////////////////////////////////////
		m_pMetrixGrid ->ShowWindow (SW_HIDE) ;
		m_pComboMetrix->ShowWindow (SW_HIDE);
	    m_pStaticMetrix->ShowWindow (SW_HIDE);
		m_pGridSample->ShowWindow (SW_SHOW) ;
		
		///////////////////////////////////////////////////////////
		m_pGridSample->SetRows (_ttoi(strCount)+1);

		CString strSampleTitle;
		CString strQuestion_Key ;
		INT nDMCnt = m_pQuestionDM->GetRowCount () ;
		INT nGridRowCnt = 1;
		for(INT i = 0 ; i < nDMCnt ;i++)
		{
			strSampleTitle = _T("") ;
			strQuestion_Key = m_pQuestionDM->GetCellData (_T("QUESTION_KEY"),i);
			if(strQuestionInfoKey == strQuestion_Key)
			{
				strSampleTitle = m_pQuestionDM->GetCellData (_T("SAMPLE_TITLE"),i) ;
				m_pGridSample->SetTextMatrix (nGridRowCnt,2,strSampleTitle);
				nGridRowCnt++ ;
			}

		}
	}
	else if(strType == _T("매트릭스형"))
	{
		m_pMetrixGrid ->ShowWindow (SW_SHOW) ;
		m_pComboMetrix->ShowWindow (SW_SHOW);
	    m_pStaticMetrix->ShowWindow (SW_SHOW);
		m_pGridSample->ShowWindow (SW_HIDE) ;

		CString strSubTitle;
		CString strQuestion_Key ;
		CString strMetrixInfoKey ;

		m_pMetrixGrid->SetRows (_ttoi(strCount)+2) ;
		INT nMetrixInfoDMCnt = m_pMetrixInfoDM->GetRowCount () ;
		INT nGridRowCnt = 2 ;

		INT nSetColCnt = - 1;

		for(INT i = 0 ; i < nMetrixInfoDMCnt ;i++)
		{
			strSubTitle = _T("") ;
			strQuestion_Key = m_pMetrixInfoDM->GetCellData (_T("QUESTION_INFO_KEY"),i);
			if(strQuestionInfoKey == strQuestion_Key)
			{
				strSubTitle = m_pMetrixInfoDM->GetCellData (_T("SUBTITLE"),i) ;
				m_pMetrixGrid->SetTextMatrix (nGridRowCnt,1,strSubTitle);
				nGridRowCnt++ ;
				strMetrixInfoKey = m_pMetrixInfoDM->GetCellData (_T("REC_KEY"),i) ;
				nSetColCnt = _ttoi(m_pMetrixInfoDM->GetCellData (_T("METRIX_COUNT"),i)) ;
			}

		}

		CString strSampleTitle;
		CString strFindKey ;
		INT nMetrixDMCnt = m_pMetrixDM->GetRowCount () ;
		
		m_pMetrixGrid->SetCols (0,nSetColCnt+2) ;

		CString strTemp ;
		strTemp.Format (_T("%d"),nSetColCnt) ;
		m_pComboMetrix->SelectString (0,strTemp ) ;

		INT nGridColCnt = 2 ;
		for(INT j = 0 ; j < nMetrixDMCnt ;j++)
		{
			strSampleTitle = _T("") ;
			strFindKey = m_pMetrixDM->GetCellData (_T("METRIX_INFO_KEY"),j);
			if(strMetrixInfoKey == strFindKey)
			{
				strSampleTitle = m_pMetrixDM->GetCellData (_T("SAMPLE_TITLE"),j) ;
				m_pMetrixGrid->SetTextMatrix (1,nGridColCnt,strSampleTitle);
				nGridColCnt++ ;
			}
		}
	}

}

VOID CRESEARCH_NEWMAKE_DLG::OnbQuestionModify() 
{
	// TODO: Add your control notification handler code here


	if(m_nCurrentQuestionIdx == -1)
	{
		AfxMessageBox (_T("수정 할 질문을 선택 하여 주십시요")) ;
		return ;
	}

	
	m_nCurrentQuestionIdx = m_pGridQuestion->GetIdx () ;
	
	//질문 정보 control 
	CString strSubtitle,strType,strQuestionCount,strUpdateDate ;

	CTime t = CTime::GetCurrentTime ();
	strUpdateDate.Format (_T("%04d/%02d/%02d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;

	m_pEditSubTitle->GetWindowText (strSubtitle )  ;
	m_pComboType->GetWindowText (strType )     ;
    m_pComboCount->GetWindowText (strQuestionCount )   ;

	m_pQuestionInfoDM->SetCellData (_T("SUBTITLE"),strSubtitle,m_nCurrentQuestionIdx ) ;
	m_pQuestionInfoDM->SetCellData (_T("TYPE"),strType,m_nCurrentQuestionIdx ) ;
	m_pQuestionInfoDM->SetCellData (_T("UPDATE_DATE"),strUpdateDate,m_nCurrentQuestionIdx ) ;
	m_pQuestionInfoDM->SetCellData (_T("QUESTION_COUNT"),strQuestionCount,m_nCurrentQuestionIdx ) ;

   

	if(strType != _T("매트릭스형"))
	{	
		//******************************************************************************************
		//기존의 내용을 삭제하고 입력한다.
		CString strQuestionInfoKey = m_pQuestionInfoDM->GetCellData (_T("REC_KEY"),m_nCurrentQuestionIdx );
		
		INT nQuestionCnt = m_pQuestionDM->GetRowCount () ;
		
		INT nRowCnt = nQuestionCnt-1 ;
		
		for( INT i = 0  ; i < nQuestionCnt  ; i++)
		{
			CString strTempKey = m_pQuestionDM->GetCellData (_T("QUESTION_KEY"),nRowCnt);
			
			if(strQuestionInfoKey == strTempKey )
			{
				m_pQuestionDM->DeleteRow (nRowCnt) ;
			}
			nRowCnt-- ;
		}

		//******************************************************************************************

		CString strSampleTitle ;
		CString strSampleReckey ;

		for( i = 0 ; i< _ttoi(strQuestionCount) ;i++)
		{
			strSampleTitle = m_pGridSample->GetTextMatrix (i+1,2) ;
			
			if(strSampleTitle.IsEmpty ())
			{
				AfxMessageBox (_T("질문 항목의 내용을 입력하여 주십시요."));
				return ;
			}
			
			m_pQuestionDM->InsertRow (-1) ;
			m_pQuestionDM->MakeRecKey (	strSampleReckey ) ;
			
			INT nRowIndex = m_pQuestionDM->GetRowCount ()-1 ;
			
			m_pQuestionDM->SetCellData (_T("REC_KEY"),strSampleReckey,nRowIndex) ;
			m_pQuestionDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchKey,nRowIndex) ;
			m_pQuestionDM->SetCellData (_T("QUESTION_KEY"),strQuestionInfoKey,nRowIndex) ;
			m_pQuestionDM->SetCellData (_T("QUESTION_TYPE"),strType,nRowIndex) ;
			m_pQuestionDM->SetCellData (_T("SAMPLE_TITLE"),strSampleTitle,nRowIndex) ;
			m_pQuestionDM->SetCellData (_T("UPDATE_DATE"),strUpdateDate,nRowIndex) ;
			
			CString strTemp ;
			strTemp.Format (_T("%d"),i+1) ;
			m_pQuestionDM->SetCellData (_T("ALIAS"),strTemp,nRowIndex) ;
		}
	}
	else if (strType == _T("매트릭스형"))
	{
	
		//******************************************************************************************
		//기존의 내용을 삭제하고 입력한다.
		CString strQuestionInfoKey = m_pQuestionInfoDM->GetCellData (_T("REC_KEY"),m_nCurrentQuestionIdx );
		INT nMetrixInfoCnt = m_pMetrixInfoDM ->GetRowCount () ;
		INT nRowCnt = nMetrixInfoCnt-1 ;
		
		for( INT i = 0  ; i < nMetrixInfoCnt  ; i++)
		{
			
			CString strTempKey = m_pMetrixInfoDM->GetCellData (_T("QUESTION_INFO_KEY"),nRowCnt);
			
			if(strQuestionInfoKey == strTempKey )
			{

				//METRIX_QUESTION DELETE 
				CString strMetrixInfoKey = m_pMetrixInfoDM->GetCellData (_T("REC_KEY"),nRowCnt);
				INT nMetrixCnt = m_pMetrixDM->GetRowCount () ;
				INT nMetrixRowCnt = nMetrixCnt - 1;
                
				if(nMetrixCnt == 0) continue ;
				for (INT j = 0 ;j <nMetrixCnt ;j++)
				{
					CString strFindKey = m_pMetrixDM ->GetCellData (_T("METRIX_INFO_KEY"),nMetrixRowCnt) ;
					if(strMetrixInfoKey == strFindKey )
					{
						m_pMetrixDM->DeleteRow (nMetrixRowCnt) ;
					}
					nMetrixRowCnt-- ;
				}
				m_pMetrixInfoDM->DeleteRow (nRowCnt) ;
			}
			nRowCnt-- ;
		}
		
		CString strMetrixTitle;
		CString strMetrixInfoReckey;
		CString strMetrixCount;
		CString strMetrixSample ;
		CString strMetrixKey;
		m_pComboMetrix ->GetWindowText (strMetrixCount )   ;

		for( i = 0 ; i < _ttoi(strQuestionCount ) ; i++)
		{
			strMetrixTitle = m_pMetrixGrid ->GetTextMatrix (i+2,1) ;

			m_pMetrixInfoDM->InsertRow (-1) ;
			m_pMetrixInfoDM->MakeRecKey (	strMetrixInfoReckey ) ;
			
			INT nRowIndex = m_pMetrixInfoDM->GetRowCount ()-1 ;
			
			m_pMetrixInfoDM->SetCellData (_T("REC_KEY"),strMetrixInfoReckey,nRowIndex) ;
			m_pMetrixInfoDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchKey,nRowIndex) ;
			m_pMetrixInfoDM->SetCellData (_T("QUESTION_INFO_KEY"),strQuestionInfoKey,nRowIndex) ;
			m_pMetrixInfoDM->SetCellData (_T("TYPE"),strType,nRowIndex) ;
			m_pMetrixInfoDM->SetCellData (_T("SUBTITLE"),strMetrixTitle,nRowIndex) ;
			m_pMetrixInfoDM->SetCellData (_T("UPDATE_DATE"),strUpdateDate,nRowIndex) ;
			m_pMetrixInfoDM->SetCellData (_T("METRIX_COUNT"),strMetrixCount,nRowIndex) ;
			m_pMetrixInfoDM->SetCellData (_T("QUESTION_COUNT"),strQuestionCount,nRowIndex) ;
			
			for(INT j = 0 ; j < _ttoi(strMetrixCount) ;j++)
			{
				strMetrixSample = m_pMetrixGrid ->GetTextMatrix (1,j+2) ;
				
				if(strMetrixSample.IsEmpty ())
				{
					AfxMessageBox (_T("질문 항목의 내용을 입력하여 주십시요."));
					return ;
				}
				
				m_pMetrixDM->InsertRow (-1) ;
				m_pMetrixDM->MakeRecKey (	strMetrixKey ) ;
				
				INT nRowIndex = m_pMetrixDM->GetRowCount ()-1 ;
				
				m_pMetrixDM->SetCellData (_T("REC_KEY"),strMetrixKey,nRowIndex) ;
				m_pMetrixDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchKey,nRowIndex) ;
				m_pMetrixDM->SetCellData (_T("QUESTION_INFO_KEY"),strQuestionInfoKey,nRowIndex) ;
				m_pMetrixDM->SetCellData (_T("METRIX_INFO_KEY"),strMetrixInfoReckey,nRowIndex) ;
				m_pMetrixDM->SetCellData (_T("QUESTION_TYPE"),strType,nRowIndex) ;
				m_pMetrixDM->SetCellData (_T("SAMPLE_TITLE"),strMetrixSample,nRowIndex) ;
				m_pMetrixDM->SetCellData (_T("UPDATE_DATE"),strUpdateDate,nRowIndex) ;
				
				CString strTemp ;
				strTemp.Format (_T("%d"),i+1) ;
				m_pMetrixDM->SetCellData (_T("ALIAS"),strTemp,nRowIndex) ;
			}
			
		}

		//******************************************************************************************
	}
	

}

VOID CRESEARCH_NEWMAKE_DLG::SetScreenMode(CString parentDM,CString parentCM,CString parentGrid,bool bINSERT )
{
	m_bInsertMode = bINSERT ;
	m_strParentDM	  = parentDM ;
	m_strParentCM	  = parentCM ;
	m_strParentGrid  = parentGrid ;
}

INT CRESEARCH_NEWMAKE_DLG::InitModifyScreen(INT ndir)
{

	//*********************************************************************************
	//DM 정보 갖고 오기 

	INT  ids ;
	switch (ndir)
	{
	case 0 :                                                                         
		
		SelectMakeList(m_strParentCM ,m_strParentGrid );
		ids = SelectGetHeadPosition(m_strParentCM ,m_strParentGrid);
		if (ids < 0)
		{
			AfxMessageBox(_T("선택 목록을 구성할 수 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return -222;
		}
		break;
		
	case -1 :
		
        ids = SelectGetPrev (m_strParentCM ,m_strParentGrid);
		if (ids == -1) {
			ids = SelectGetHeadPosition(m_strParentCM ,m_strParentGrid);
			AfxMessageBox(_T(" 처음 자료입니다."));
			return -223;
		}
		break;
		
	case 1 :
		
		ids = SelectGetNext (m_strParentCM ,m_strParentGrid);
		if (ids == -1) {
			ids = SelectGetTailPosition(m_strParentCM ,m_strParentGrid);
			AfxMessageBox(_T(" 마지막 자료입니다."));
			return -224;
		}
		break;
	}
	
	INT m_Idx ;
    m_Idx = ids; 
    SetReverse (m_strParentCM ,m_strParentGrid ,m_Idx) ;
	
	CString strResearchKey ;
	CString strOption ;
	

	if(m_pResearchInfoDM  == NULL )
	{
		m_pResearchInfoDM = FindDM(_T("DM_RESEARCH_INFO"));
	}

	if(m_pQuestionInfoDM  == NULL )
	{
		m_pQuestionInfoDM = FindDM(_T("DM_QUESTION_INFO"));
	}

	if(m_pQuestionDM == NULL )
	{
		m_pQuestionDM = FindDM(_T("DM_QUESTION"));
	}


	if(m_pMetrixInfoDM == NULL )
	{
		m_pMetrixInfoDM = FindDM(_T("DM_METRIX_INFO"));
	}

	if(m_pMetrixDM == NULL )
	{
		m_pMetrixDM = FindDM(_T("DM_METRIX_QUESTION"));
	}


	CESL_DataMgr *pParentDM = FindDM(m_strParentDM);
	strResearchKey = pParentDM->GetCellData (_T("REC_KEY"),m_Idx) ;
	strOption.Format (_T("REC_KEY = %s"),strResearchKey) ;

	m_pResearchInfoDM->RefreshDataManager (strOption);


	strOption.Format (_T("RESEARCH_KEY = %s"),strResearchKey) ;
    m_pQuestionInfoDM->RefreshDataManager (strOption) ;
	m_pQuestionDM->RefreshDataManager (strOption) ;


	m_pMetrixInfoDM->RefreshDataManager (strOption) ;
	m_pMetrixDM->RefreshDataManager (strOption) ;


	CButton *pInsertButton = (CButton*)GetDlgItem(IDC_bResearchInput) ;
	pInsertButton->EnableWindow (FALSE) ;

	
	InitControl () ;
	
	//*********************************************************************************
	//설문 정보 control data 초기화 
	m_pEditTitle ->SetWindowText (m_pResearchInfoDM->GetCellData (_T("TITLE"),0) );    
	m_pComboResultYN ->SelectString (0,m_pResearchInfoDM->GetCellData (_T("RESULT_OPEN_YN"),0) );    
	m_pComboOnlineYN ->SelectString (0,m_pResearchInfoDM->GetCellData (_T("ONLINE_TICKET"),0) );    
    
	CString strEndDate = m_pResearchInfoDM->GetCellData (_T("END_DATE"),0);    

	if(strEndDate.GetLength () >9)
	{
		m_pEditEndYY ->SetWindowText (strEndDate.Mid (0,4));    
		m_pEditEndMM ->SetWindowText (strEndDate.Mid (5,2));
		m_pEditEndDD ->SetWindowText (strEndDate.Mid (8,2));
	}

	m_pEditResponse ->SetWindowText (m_pResearchInfoDM->GetCellData (_T("RESPONSE_TIME"),0));
	m_pDiscription ->SetWindowText (m_pResearchInfoDM->GetCellData (_T("DISCRIPTION"),0));

	//질문 정보 control 
	m_pEditSubTitle->SetWindowText (m_pQuestionInfoDM->GetCellData (_T("SUBTITLE"),0));
	m_pComboType ->SelectString (0,m_pQuestionInfoDM->GetCellData (_T("TYPE"),0) );    
    m_pComboCount->SelectString (0,m_pQuestionInfoDM->GetCellData (_T("QUESTION_COUNT"),0) );    

	m_bResearchInfoModify = TRUE ;

	//질문 control
	
	m_pGridQuestion->SetRows(m_pQuestionInfoDM->GetRowCount ()+1) ;
	
	for(INT k = 0 ; k < m_pQuestionInfoDM->GetRowCount ();k++)
	{
		m_pGridQuestion->SetTextMatrix (k+1,1,m_pQuestionInfoDM->GetCellData (_T("SUBTITLE"),k)) ;
	}

	m_strResearchKey = m_pResearchInfoDM->GetCellData (_T("REC_KEY"),0) ;
	

	return 0 ;
}
VOID CRESEARCH_NEWMAKE_DLG::InitControl()
{
	//*********************************************************************************
	//CONTROL 초기화 

	//설문 정보 control
	m_pEditTitle =     (CEdit*)GetDlgItem(IDC_EDIT_TITLE) ;
	m_pComboResultYN = (CComboBox*)GetDlgItem(IDC_COMBO_RESULTYN) ;
	m_pComboOnlineYN = (CComboBox*)GetDlgItem(IDC_COMBO_ONLINE) ; 
    
	m_pEditEndYY = (CEdit*)GetDlgItem(IDC_eEND_DATE_yy) ;
	m_pEditEndMM = (CEdit*)GetDlgItem(IDC_eEND_DATE_mm) ;
	m_pEditEndDD = (CEdit*)GetDlgItem(IDC_eEND_DATE_dd) ;
	m_pEditResponse = (CEdit*)GetDlgItem(IDC_EDIT_RESPONCE_TIME);

	m_pDiscription = (CEdit*)GetDlgItem(IDC_EDIT9);

	//질문 정보 control 
	m_pEditSubTitle = (CEdit*)GetDlgItem(IDC_EDIT_SUBTITLE);
	m_pComboType = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
    m_pComboCount = (CComboBox*)GetDlgItem(IDC_COMBO_QCount);

	m_pGridSample = (CESL_Grid*)GetDlgItem(IDC_QUESTION_SAMPLE_GRID);

	//질문 control
	m_pGridQuestion = (CESL_Grid*)GetDlgItem(IDC_QUESTION_GRID);


	//////////////////////////////////////////////////////////////////////////////
	//SAMPLE GRID INIT 

	CESL_ControlMgr *pCM = FindCM(_T("CM_RESEARCH_MAKE"));
	if(pCM == NULL)
	{
		AfxMessageBox (_T("FindCM() Failed"));
		return  ;
	}
	m_pGridSample = (CESL_Grid*)pCM->FindControl (_T("SampleGrid"));
	m_pGridSample->m_bGridEdit = TRUE ;
	m_pGridSample->SELEDITOR_RemoveAllColumns () ;
	m_pGridSample->SELEDITOR_AddColumn (1) ;

	//////////////////////////////////////////////////////////////////////////////
	//매트릭스 
	m_pComboMetrix = (CComboBox *)GetDlgItem(IDC_COMBO_METRIC_QCount);
	m_pStaticMetrix = (CStatic *)GetDlgItem (IDC_STATIC_METRIC) ;
    
	m_pComboMetrix->ShowWindow (SW_HIDE);
	m_pStaticMetrix->ShowWindow (SW_HIDE);

	//////////////////////////////////////////////////////////////////////////////
	//METRIX GRID INIT 
	m_pMetrixGrid = (CESL_Grid*)pCM->FindControl (_T("MetrixGrid")) ;
	m_pMetrixGrid->ShowWindow (SW_HIDE) ;

	//////////////////////////////////////////////////////////////////////////////


}


VOID CRESEARCH_NEWMAKE_DLG::OnbResearchModify() 
{
	// TODO: Add your control notification handler code here
	m_bResearchInfoModify = TRUE ;
	
	CString strReckey =_T("") ;
	CString strTitle=_T("") ;
	CString strResultYN = _T("") ;
	CString strOnlineYN = _T("") ;
	CString strYY,strMM,strDD ;
	CString strEndDate  = _T("");
	CString strResponseTime = _T("");
	CString strDiscription = _T("") ;
	CString strUpdateDate ;


	m_pEditTitle->GetWindowText (strTitle);	 ;
	m_pComboResultYN->GetWindowText (strResultYN ) ;
	m_pComboOnlineYN ->GetWindowText (strOnlineYN);
	m_pEditEndYY     ->GetWindowText (strYY);
	m_pEditEndMM     ->GetWindowText (strMM);
	m_pEditEndDD     ->GetWindowText (strDD);
	m_pEditResponse  ->GetWindowText (strResponseTime);
	m_pDiscription   ->GetWindowText (strDiscription );


	strEndDate.Format (_T("%s/%s/%s"),strYY,strMM,strDD) ;
	
	CTime t = CTime::GetCurrentTime () ;
	strUpdateDate.Format (_T("%04d/%02d/%02d"),t.GetYear (),t.GetMonth (),t.GetDay ());

	INT nRowIndex = 0 ;

	m_pResearchInfoDM->SetCellData (_T("REC_KEY"),m_strResearchKey ,nRowIndex );
	m_pResearchInfoDM->SetCellData (_T("TITLE"),strTitle,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("DISCRIPTION"),strDiscription ,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("RESULT_OPEN_YN"),strResultYN ,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("END_DATE"),strEndDate,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("ONLINE_TICKET"),strOnlineYN,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("UPDATE_DATE"),strUpdateDate,nRowIndex) ;
	m_pResearchInfoDM->SetCellData (_T("RESPONSE_TIME"),strResponseTime,nRowIndex) ;



}

bool CRESEARCH_NEWMAKE_DLG::SendTOFTP()
{
	
	//1.CONNECT
	HINTERNET hInternet,hFtp ;
	
	// m_strgSever  = _T("192.168.1.230");
	// m_strgUser    =_T("mailcheck");
	// m_strgPass  =_T("eco#");
	
	GetFtpAccountCfg () ;
 
	hInternet = InternetOpen (_T("mailcheck"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if(hInternet == NULL)
	{
		AfxMessageBox (_T("hInternet Open() Failed"));
		return FALSE ;
	}

	hFtp = InternetConnect (hInternet,m_strgSever,INTERNET_DEFAULT_FTP_PORT,m_strgUser,m_strgPass,INTERNET_SERVICE_FTP,0,0);
	if(hFtp == NULL)
	{
		InternetCloseHandle (hInternet) ;
		AfxMessageBox (_T("hFtp Connect() Failed"));
		return FALSE;
	}

	//2.UPLOAD FILE
	//path 파일을 FTP 현재 디렉토리로 업로드 한다.

	CString strlocalfile ;
	CString strRemotefile ;

	strlocalfile.Format(_T("..\\cfg\\webresearch\\%s.htm"),m_strResearchKey ) ; 
	strRemotefile.Format (_T("%s.htm"),m_strResearchKey) ;
	FtpPutFile(hFtp,strlocalfile.GetBuffer (0),strRemotefile.GetBuffer (0),FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,0) ;

	//3.DISCONNECT
	InternetCloseHandle(hFtp) ;
	InternetCloseHandle(hInternet) ;

	hFtp = NULL ;
	hInternet = NULL ;

		

	return TRUE ;
}

VOID CRESEARCH_NEWMAKE_DLG::OnbOnlineTicket() 
{
	// TODO: Add your control notification handler code here

	TCHAR szOriginPath[1024] = _T("\0"); //현재경로
	GetCurrentDirectory(1024,szOriginPath);

	CString szFileName = _T("");
	CFileDialog dlgOpen(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("All files(*.*)|*.*||"));
	
	if ( IDOK != dlgOpen.DoModal() )    return;
	
	szFileName = dlgOpen.GetPathName();
	SetCurrentDirectory(szOriginPath);


    CEdit *pEdit = (CEdit*)GetDlgItem (IDC_EDIT_ONLINETICKET) ;
	pEdit->SetWindowText (szFileName) ;
	
	if(szFileName != _T(""))
	{
		m_pComboOnlineYN->SelectString (0,_T("YES"));
		HINTERNET hInternet,hFtp ;
			
		// m_strgSever  = _T("192.168.1.230");
		// m_strgUser    =_T("mailcheck");
		// m_strgPass  =_T("eco#");
		GetFtpAccountCfg() ;
		
		hInternet = InternetOpen (_T("mailcheck"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
		if(hInternet == NULL)
		{
			AfxMessageBox (_T("hInternet Open() Failed"));
			return  ;
		}
		
		hFtp = InternetConnect (hInternet,m_strgSever,INTERNET_DEFAULT_FTP_PORT,m_strgUser,m_strgPass,INTERNET_SERVICE_FTP,0,0);
		if(hFtp == NULL)
		{
			InternetCloseHandle (hInternet) ;
			AfxMessageBox (_T("hFtp Connect() Failed"));
			return ;
		}
		
		//2.UPLOAD FILE
		//path 파일을 FTP 현재 디렉토리로 업로드 한다.
		
		CString strlocalfile ;
		CString strRemotefile ;
		
		//strlocalfile.Format(_T("..\\cfg\\webresearch\\%s.htm"),m_strResearchKey ) ; 
		strlocalfile = szFileName ;

		strRemotefile.Format (_T("%s.jpg"),m_strResearchKey) ;
		FtpPutFile(hFtp,strlocalfile.GetBuffer (0),strRemotefile.GetBuffer (0),FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,0) ;
		
		//3.DISCONNECT
		InternetCloseHandle(hFtp) ;
		InternetCloseHandle(hInternet) ;
		
		hFtp = NULL ;
		hInternet = NULL ;
		
	}
	else 
	{
		m_pComboOnlineYN->SelectString (0,_T("NO"));
	}




}

VOID CRESEARCH_NEWMAKE_DLG::OnSelchangeCOMBOMETRICQCount() 
{
	// TODO: Add your control notification handler code here
	CString strCount ;

	INT nColCount = m_pComboMetrix->GetCurSel () ;

	//m_pGridSample->SetRows (nRowCount+2) ;
	//m_pGridSample->SetRedraw(TRUE);
	m_pMetrixGrid ->SetCols (0,nColCount+3) ;

	m_pMetrixGrid->m_bGridEdit = TRUE ;
	m_pMetrixGrid->SELEDITOR_RemoveAllColumns () ;
	
	

	for(INT i = 0 ;i < nColCount+2 ;i++) 
	{
		m_pMetrixGrid->SELEDITOR_AddColumn (i) ;
    }


}

VOID CRESEARCH_NEWMAKE_DLG::OnDblClickQuestionMetrixGrid() 
{
	// TODO: Add your control notification handler code here
	
	INT nRow = m_pMetrixGrid ->GetRow () ;
	INT nCol = m_pMetrixGrid ->GetCol () ;

	if(nRow == 1 && nCol == 1)
	{
		AfxMessageBox (_T("입력 안됨")) ;
		return ;
	}


}

bool CRESEARCH_NEWMAKE_DLG::InputMetrixQuestion()
{
	
	//QUESTIN INFO 입력
	//METRIX INFO 입력
	//METRIX 입력 

	CString strQuestionInfoKey ;
	CString strSubtitle;
	CString strType ;
	CString strCreateDate ;
	CString strQuestionCount;
	CString strMetrixCount ;

	
	//질문 정보 control 
	m_pEditSubTitle->GetWindowText (strSubtitle )  ;
	m_pComboType->GetWindowText (strType )     ;
    m_pComboCount->GetWindowText (strQuestionCount )   ;
	m_pComboMetrix ->GetWindowText (strMetrixCount )   ;

	
	CTime t = CTime::GetCurrentTime () ;
	strCreateDate.Format (_T("%04d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay ());
	

	m_pQuestionInfoDM = FindDM(_T("DM_QUESTION_INFO"));
	if(m_pQuestionInfoDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed"));
		return FALSE;
	}


	m_pQuestionInfoDM->InsertRow (-1) ;
	INT nQrowIndex = m_pQuestionInfoDM->GetRowCount () -1;

	m_pQuestionInfoDM->MakeRecKey (strQuestionInfoKey ) ;

	m_pQuestionInfoDM->SetCellData (_T("REC_KEY"),strQuestionInfoKey,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchKey,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("SUBTITLE"),strSubtitle,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("TYPE"),strType,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("CREATE_DATE"),strCreateDate,nQrowIndex ) ;
	m_pQuestionInfoDM->SetCellData (_T("QUESTION_COUNT"),strQuestionCount,nQrowIndex ) ;


	//2.QUESTION
	
	//REC_KEY
	//QUESTION_KEY
	//QUESTION_TYPE
	//SAMPLE_TITLE
	//CREATE_DATE
	
	m_pMetrixGrid;


	CString strMetrixInfoReckey ;
	CString strMetrixTitle ;
	CString strMetrixSample;
	CString strMetrixKey ;
  

	for(INT i = 0 ; i < _ttoi(strQuestionCount ) ; i++)
	{
		strMetrixTitle = m_pMetrixGrid ->GetTextMatrix (i+2,1) ;
			

		m_pMetrixInfoDM->InsertRow (-1) ;
		m_pMetrixInfoDM->MakeRecKey (	strMetrixInfoReckey ) ;
		
		INT nRowIndex = m_pMetrixInfoDM->GetRowCount ()-1 ;
		
		m_pMetrixInfoDM->SetCellData (_T("REC_KEY"),strMetrixInfoReckey,nRowIndex) ;
		m_pMetrixInfoDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchKey,nRowIndex) ;
		m_pMetrixInfoDM->SetCellData (_T("QUESTION_INFO_KEY"),strQuestionInfoKey,nRowIndex) ;
		m_pMetrixInfoDM->SetCellData (_T("TYPE"),strType,nRowIndex) ;
		m_pMetrixInfoDM->SetCellData (_T("SUBTITLE"),strMetrixTitle,nRowIndex) ;
		m_pMetrixInfoDM->SetCellData (_T("CREATE_DATE"),strCreateDate,nRowIndex) ;
		m_pMetrixInfoDM->SetCellData (_T("METRIX_COUNT"),strMetrixCount,nRowIndex) ;
		m_pMetrixInfoDM->SetCellData (_T("QUESTION_COUNT"),strQuestionCount,nRowIndex) ;


		for(INT j = 0 ; j < _ttoi(strMetrixCount) ;j++)
		{
			strMetrixSample = m_pMetrixGrid ->GetTextMatrix (1,j+2) ;
			
		
			if(strMetrixSample.IsEmpty ())
			{
				AfxMessageBox (_T("질문 항목의 내용을 입력하여 주십시요."));
				return FALSE;
			}

			m_pMetrixDM->InsertRow (-1) ;
			m_pMetrixDM->MakeRecKey (	strMetrixKey ) ;
			
			INT nRowIndex = m_pMetrixDM->GetRowCount ()-1 ;
			
			m_pMetrixDM->SetCellData (_T("REC_KEY"),strMetrixKey,nRowIndex) ;
			m_pMetrixDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchKey,nRowIndex) ;
			m_pMetrixDM->SetCellData (_T("QUESTION_INFO_KEY"),strQuestionInfoKey,nRowIndex) ;
			m_pMetrixDM->SetCellData (_T("METRIX_INFO_KEY"),strMetrixInfoReckey,nRowIndex) ;
			m_pMetrixDM->SetCellData (_T("QUESTION_TYPE"),strType,nRowIndex) ;
			m_pMetrixDM->SetCellData (_T("SAMPLE_TITLE"),strMetrixSample,nRowIndex) ;
			m_pMetrixDM->SetCellData (_T("CREATE_DATE"),strCreateDate,nRowIndex) ;
			
			CString strTemp ;
			strTemp.Format (_T("%d"),i+1) ;
			m_pMetrixDM->SetCellData (_T("ALIAS"),strTemp,nRowIndex) ;
		}
		
	}

	//질문 control
	m_pGridQuestion->SetRows(m_pQuestionInfoDM->GetRowCount ()+1) ;
	
	for(INT k = 0 ; k < m_pQuestionInfoDM->GetRowCount ();k++)
	{
		m_pGridQuestion->SetTextMatrix (k+1,1,m_pQuestionInfoDM->GetCellData (_T("SUBTITLE"),k)) ;
	}



	return true ;
}

VOID CRESEARCH_NEWMAKE_DLG::GetFtpAccountCfg()
{
	/*
	CString m_strgSever ;
	CString m_strgUser ;
	CString m_strgPass ;
    */
	CStdioFile file ;
	CString strFilePath = _T("..\\cfg\\FtpAccount.cfg") ;
	if(!file.Open (strFilePath,CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox (strFilePath + _T(" Open failed")) ;
		return  ;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString strLine ;
	CString strTag;
	CString strData ;
	INT     nPos = -1 ;
	while(file.ReadString (strLine))
	{
		if(strLine.GetLength () < 2) continue ;
		if(strLine.GetAt (0) == '#') continue ;

		nPos = strLine.Find (_T("=")) ;
		strTag = strLine.Mid (0,nPos) ;
		strTag.TrimLeft () ;
		strTag.TrimRight ();
		strData = strLine.Mid (nPos+1) ;
		strData.TrimLeft () ;
		strData.TrimRight ();

		if(strTag == _T("FTPSERVER")) 
		{
			m_strgSever = strData ;
		}
		if(strTag == _T("FTPUSER")) 
		{
			m_strgUser = strData ;
		}
		if(strTag == _T("FTPPASS")) 
		{
			m_strgPass = strData ;
		}
	}

	file.Close () ;
	return ;
}

bool CRESEARCH_NEWMAKE_DLG::MakeHTML_ScriptBody_Metrix(CString strType, CString strQuestionInfo_key, CString strMetrixInfoKey, INT nSelect)
{

	CString strRadioScript = _T("");
	//[TYPE,QUESTION_INFO_KEY,METRIX_INFO_KEY,SELECT]
	strRadioScript.Format (_T(" bCheck = false ; ")
		                   _T(" for(i = 0 ; i < %d ; i++) { ")
						   _T(" if (document.RESEARCH.Q_%s[i].checked) { ")
						   _T(" var select = i+1 ; ")
						   _T(" var strdata = '['+ '%s ;'+ '%s ;' +'%s ;'+ select  +']' ; ") 
						   _T(" bCheck = true ; ")
						   _T(" result =  result + strdata ; ")
						   _T(" }} \r\n if(bCheck == false ) { alert ('항목이 선택되지 않았습니다.'); return; } ")
						   ,nSelect,strMetrixInfoKey,strType,strQuestionInfo_key,strMetrixInfoKey) ;
	
	m_strJavaScriptStream += strRadioScript ;

	return true ;
}

HBRUSH CRESEARCH_NEWMAKE_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
