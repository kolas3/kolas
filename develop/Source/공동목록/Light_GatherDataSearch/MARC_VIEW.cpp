// MARC_VIEW.cpp : implementation file
//

#include "stdafx.h"
#include "UC_MARC_IMPORT.h"
#include "MARC_VIEW.h"
#include "..\..\include\공동목록\RS_Acc.h"

#include "KolisNetService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMARC_VIEW dialog


CMARC_VIEW::CMARC_VIEW(CESL_Mgr* pParent /*=NULL*/)
	:  CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMARC_VIEW)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTocYN = FALSE;
	m_bAbsYN = FALSE;
	m_pCM = NULL;

	m_nCheck = 1;
	
    AfxInitRichEdit(); 
}

CMARC_VIEW::~CMARC_VIEW()
{
}


VOID CMARC_VIEW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMARC_VIEW)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMARC_VIEW, CDialog)
	//{{AFX_MSG_MAP(CMARC_VIEW)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bIMPORT, OnbIMPORT)
	ON_BN_CLICKED(IDC_chkABS, OnchkABS)
	ON_BN_CLICKED(IDC_chkTOC, OnchkTOC)
	ON_BN_CLICKED(IDC_rAll, OnrAll)
	ON_BN_CLICKED(IDC_rALL_505, OnrALL505)
	ON_BN_CLICKED(IDC_rBasic, OnrBasic)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMARC_VIEW message handlers

BOOL CMARC_VIEW::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CMARC_VIEW::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids ;

	ids = InitESL_Mgr(_T("SM_마크반입_보기"));
	if( ids < 0 )
		return FALSE;

	m_pCM = FindCM(_T("CM_마크반입_보기"));
	if( m_pCM == NULL )
		return FALSE;

   	m_marcEditor.SubclassDlgItem(IDC_RICHEDIT_MARC_VIEW, -1, this);


    CESL_ControlMgr *pCM = FindCM(_T("CM_찾기")) ;
    
	//CString m_pParentCtrlMgrAlias ;
    
    m_TabSel = ((CUC_MARC_IMPORT*)pParentMgr)->m_TabCtrl.GetCurSel () ;
    
    m_ParentCM  = _T("CM_찾기") ;
    
	if(m_TabSel == 0) 
	{   
		m_CurrentDM = _T("DM_서지마스터_단행");
		m_ParentDM  = _T("DM_단행자료_DB") ;
		m_ParentGrid  = _T("MainBoGrid") ;
	}
	
	else
	{   
		m_CurrentDM = _T("DM_서지마스터_출판유통진흥원") ;
	    m_ParentDM  = _T("DM_출판유통자료_DB");
		m_ParentGrid  = _T("MainChulpanGrid") ;
		( (CButton*) GetDlgItem(IDC_chkTOC) )->ShowWindow(FALSE);
		( (CButton*) GetDlgItem(IDC_chkABS) )->ShowWindow(FALSE);
	}
	pGrid =	(CESL_Grid*)pCM->FindControl(m_ParentGrid);
	if( pGrid->GetCount() == 0 )
	{
		AfxMessageBox(_T("검색된 결과가 없습니다"));
		return FALSE;
	}

	DisplayMarcData(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMARC_VIEW::OnCancel() 
{
	// TODO: Add extra cleanup here
	EndDialog(-1);	
	//CDialog::OnCancel();	
}

INT CMARC_VIEW::DisplayMarcData (INT nDir) 
{
    INT ids ;
	INT nIndex = 0;
	INT nSelectCnt = 0;
	INT nTotalCnt = 0;
	switch(nDir)
	{
	case 0 :
		ids = SelectMakeList(m_ParentCM  ,m_ParentGrid );
		nSelectCnt = SelectGetCount(m_ParentCM  ,m_ParentGrid );
		nTotalCnt = pGrid->GetCount();
		if( nSelectCnt > 0 )
			nIndex = SelectGetHeadPosition(m_ParentCM  ,m_ParentGrid);
		else
			nIndex = pGrid->GetIdx();
		
		break;
		
	case -1 :
	
		nSelectCnt = SelectGetCount(m_ParentCM  ,m_ParentGrid );
		nTotalCnt = pGrid->GetCount();
		if( nSelectCnt <= 0 )
		{
			nIndex = pGrid->GetIdx();
			if( nIndex > 0 )
				nIndex--;
			else
				return -222;
		}
		else
		{
			nIndex = SelectGetPrev (m_ParentCM  ,m_ParentGrid);
			if (nIndex == -1) {
				nIndex = SelectGetHeadPosition(m_ParentCM  ,m_ParentGrid);
				AfxMessageBox(_T("맨 처음 자료입니다."));
				return -223;
			}
		}
	    break;
		
	case 1 :
		nSelectCnt = SelectGetCount(m_ParentCM  ,m_ParentGrid );
		nTotalCnt = pGrid->GetCount();
		if( nSelectCnt <= 0 )
		{
			nIndex = pGrid->GetIdx();
			if( nIndex + 1 != nTotalCnt )
				nIndex++;
			else
				return -224;
		}
		else
		{
			nIndex = SelectGetNext (m_ParentCM  ,m_ParentGrid);
			if (nIndex == -1) {
				nIndex = SelectGetTailPosition(m_ParentCM  ,m_ParentGrid);
				AfxMessageBox(_T("맨 마지막 자료입니다."));
				return -225;
			}
		}
		break;
		
	}
    m_idx = nIndex; 
	SetReverse(m_ParentCM,m_ParentGrid,m_idx);
     
	CString streamMarc ;
    ids = GetDataMgrData (m_ParentDM,_T("MARC"),streamMarc,m_idx) ;
    if(ids <0) AfxMessageBox (_T(" GetDataMgrData Error ")) ;

    CESL_DataMgr * pDataMgr  ;
	CESL_DataMgr * pDataMgr2 ;
	pDataMgr = FindDM(m_ParentDM) ;
    
	CString temp = _T("") ;
    ids = pDataMgr->GetCellData(_T("서지마스터키"),m_idx ,temp ) ;
    if(ids <0) {
		AfxMessageBox(_T("항목을 체크 해 주세요 ~!"));
	    return 0 ;
	}
 
	pDataMgr2 = FindDM(m_CurrentDM ) ;


	if(m_TabSel == 0) 
	{   
		CKolisNetDataMapper Mapper;
		Mapper.AddData(KOLISNET_PARAM_ABSTRACTS_YN, _T("초록유무"));
		Mapper.AddData(KOLISNET_PARAM_CONTENTS_YN, _T("목차유무"));
		Mapper.AddData(KOLISNET_PARAM_CONTROL_NO, _T("제어번호"));
		Mapper.AddData(KOLISNET_PARAM_CREATE_DATE, _T("생성일자"));
		Mapper.AddData(KOLISNET_PARAM_CREATE_LIB_CODE, _T("생성도서관"));
		Mapper.AddData(KOLISNET_PARAM_DDC_CLASS_NO, _T("DDC분류기호"));
		Mapper.AddData(KOLISNET_PARAM_DDC_CLASS_NO_VER, _T("DDC분류기호버전"));
		Mapper.AddData(KOLISNET_PARAM_ETC_CLASS_NO, _T("ETC분류기호"));
		Mapper.AddData(KOLISNET_PARAM_ETC_CLASS_NO_TYPE, _T("ETC분류기호타입"));
		Mapper.AddData(KOLISNET_PARAM_ETC_CLASS_NO_VER, _T("ETC분류기호버전"));
		Mapper.AddData(KOLISNET_PARAM_FORM_CODE, _T("형식구분"));
		Mapper.AddData(KOLISNET_PARAM_KDC_CLASS_NO, _T("KDC분류기호"));
		Mapper.AddData(KOLISNET_PARAM_KDC_CLASS_NO_VER, _T("KDC분류기호버전"));
		Mapper.AddData(KOLISNET_PARAM_BIB_MARC, _T("MARC"));
		Mapper.AddData(KOLISNET_PARAM_MAT_CODE, _T("자료구분"));
		Mapper.AddData(KOLISNET_PARAM_MEDIA_CODE, _T("매체구분"));
		Mapper.AddData(KOLISNET_PARAM_NOTE, _T("메모장"));
		Mapper.AddData(KOLISNET_PARAM_BIB_KEY, _T("서지마스터키"));
		Mapper.AddData(KOLISNET_PARAM_UNDO_FLAG, _T("UNDO_FLAG"));
		Mapper.AddData(KOLISNET_PARAM_UPDATE_DATE, _T("최종수정일자"));
		Mapper.AddData(KOLISNET_PARAM_UPDATE_LIB, _T("최종수정도서관"));
		Mapper.AddData(KOLISNET_PARAM_USE_LIMIT_CODE, _T("이용제한구분"));

		CKolisNetService::GetBibInfo(this, temp, pDataMgr2, &Mapper);
	}
	else
	{
		CKolisNetDataMapper Mapper;
		Mapper.AddData(KOLISNET_PARAM_MARC, _T("MARC"));
		Mapper.AddData(KOLISNET_PARAM_CONTENTS, _T("CONTENTS"));
		Mapper.AddData(KOLISNET_PARAM_CREATE_DATE, _T("생성일자"));
		Mapper.AddData(KOLISNET_PARAM_CONTENTS_YN, _T("목차유무"));
		Mapper.AddData(KOLISNET_PARAM_COVER_YN, _T("표지유무"));
		Mapper.AddData(KOLISNET_PARAM_KDC_CLASS_NO, _T("KDC분류기호"));
		Mapper.AddData(KOLISNET_PARAM_DDC_CLASS_NO, _T("DDC분류기호"));
		Mapper.AddData(KOLISNET_PARAM_UPDATE_DATE, _T("최종수정일자"));
		Mapper.AddData(KOLISNET_PARAM_BS_KEY, _T("서지마스터키"));

		CKolisNetService::GetBtpSpeciesInfo(this, temp, pDataMgr2, &Mapper);
	}

	pDataMgr2->GetCellData(_T("MARC"),0,streamMarc) ;
    CWnd *pWnd = GetDlgItem(IDC_RICHEDIT_MARC_VIEW);   
    pMarcMgr = m_pInfo->pMarcMgr ;	
   
    if(m_TabSel == 0)
	{
		ids = pMarcMgr->Decoding (streamMarc ,&m_marc) ;
		if( ids < 0 )
		{
			AfxMessageBox(_T("마크가 없거나 잘못되었습니다."));
			m_marc.Init();
		}

		m_marcEditor.InitUserAlias();
		m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
		m_marcEditor.Display();
	}
	else
	{    
		pMarcMgr->SetMarcGroup (_T("통합마크"));
    	ids = pMarcMgr->Decoding (streamMarc ,&m_marc) ;
		if( ids < 0 )
		{
			AfxMessageBox(_T("마크가 없거나 잘못되었습니다."));
			m_marc.Init();
		}
		m_marcEditor.InitUserAlias();
		m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
		m_marcEditor.Display();
	}

	CString strLibCode, strLibName;
	CString strTocYN, strAbsYN;
	CString strInputDate, strMatCode, strFormCode;
	CString strPage;
	CString strQuery;
		
	if( nSelectCnt > 0 )
	{
		if( nDir == 0 )
			m_nSelect = 1;
		else if( nDir == -1 )
			m_nSelect -= 1;
		else 
			m_nSelect += 1;

		strPage.Format(_T("%d/%d"),m_nSelect, nSelectCnt);
		GetDlgItem(IDC_sPAGE)->SetWindowText(strPage);
	}
	else
	{
		strPage.Format(_T("%d/%d"),m_idx+1, nTotalCnt);
		GetDlgItem(IDC_sPAGE)->SetWindowText(strPage);
	}

	ids = GetDataMgrData(m_ParentDM,_T("목차유무"),strTocYN,m_idx);
	if( strTocYN == _T("Y") )
	{
		CButton * bToc = (CButton *)GetDlgItem(IDC_chkTOC);
		bToc->EnableWindow(TRUE);
		m_bTocYN = TRUE;
	}
	else
	{
		CButton * bToc = (CButton *)GetDlgItem(IDC_chkTOC);
		bToc->EnableWindow(FALSE);
		m_bTocYN = FALSE;
	}

	ids = GetDataMgrData(m_ParentDM,_T("초록유무"),strAbsYN,m_idx);
	if( strAbsYN == _T("Y") )
	{
		CButton * bAbs = (CButton *)GetDlgItem(IDC_chkABS);
		bAbs->EnableWindow(TRUE);
		m_bAbsYN = TRUE;
	}
	else
	{
		CButton * bAbs = (CButton *)GetDlgItem(IDC_chkABS);
		bAbs->EnableWindow(FALSE);
		m_bAbsYN = FALSE;
	}
	
	CButton * pRCheck = (CButton *)GetDlgItem(IDC_rAll);
	pRCheck->SetCheck(1);

	m_nCheck = 1;

	if ( m_TabSel == 0 )
	{		
		pDataMgr2->GetCellData(_T("생성도서관"),0,strLibCode);

		CKolisNetService::GetLibraryName(this, strLibCode, strLibName);
		CString strLibInfo;
		strLibInfo.Format(_T("%s(%s)"),strLibName, strLibCode);
		GetDlgItem(IDC_eLIB_NAME)->SetWindowText(strLibInfo);
		pDataMgr2->GetCellData(_T("형식구분"),0,strFormCode);
		pDataMgr2->GetCellData(_T("매체구분"),0,strMatCode);
		
		m_pCM->SetControlMgrData(_T("형식구분"),strFormCode,-1);
		m_pCM->SetControlMgrData(_T("매체구분"),strMatCode,-1);		
	}
	else
	{
		GetDlgItem(IDC_eLIB_NAME)->SetWindowText(_T("출판유통진흥원"));
		m_pCM->SetControlMgrData(_T("형식구분"),_T("BK"),-1);
		m_pCM->SetControlMgrData(_T("매체구분"),_T("PR"),-1);	
	}

	pDataMgr2->GetCellData(_T("생성일자"),0,strInputDate);
	GetDlgItem(IDC_eINPUT_DATE)->SetWindowText(strInputDate);

   return 0 ;
}

VOID CMARC_VIEW::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	INT ids ;
	ids = DisplayMarcData(1) ;
    

}

VOID CMARC_VIEW::OnbPREV() 
{
	// TODO: Add your control notification handler code here
	DisplayMarcData(-1) ;
}


VOID CMARC_VIEW::OnbIMPORT() 
{	
	EndDialog(m_idx);	
}

VOID CMARC_VIEW::OnchkABS() 
{
	CButton * bAbs = (CButton *)GetDlgItem(IDC_chkABS);
	if( 1 ==  bAbs->GetCheck() )	m_bTocYN = TRUE;	
	else							m_bTocYN = FALSE;
}

VOID CMARC_VIEW::OnchkTOC() 
{
	CButton * bToc = (CButton *)GetDlgItem(IDC_chkTOC);
	if( 1 == bToc->GetCheck() )		m_bAbsYN = TRUE;
	else							m_bAbsYN = FALSE;
}

VOID CMARC_VIEW::OnrAll() 
{
	m_nCheck = 1;	
}

VOID CMARC_VIEW::OnrALL505() 
{
	m_nCheck = 2;	
}

VOID CMARC_VIEW::OnrBasic() 
{
	m_nCheck = 0;	
}

HBRUSH CMARC_VIEW::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;	
}