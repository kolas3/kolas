// LOCAL_UPDATE.cpp : implementation file
//

#include "stdafx.h"
#include "LOCAL_UPDATE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLOCAL_UPDATE dialog


CLOCAL_UPDATE::CLOCAL_UPDATE(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewUploadMarc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    
    

}

CLOCAL_UPDATE::~CLOCAL_UPDATE()
{
}

BOOL CLOCAL_UPDATE::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CLOCAL_UPDATE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLOCAL_UPDATE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	AfxInitRichEdit();
}


BEGIN_MESSAGE_MAP(CLOCAL_UPDATE, CDialog)
	//{{AFX_MSG_MAP(CLOCAL_UPDATE)
	ON_BN_CLICKED(IDC_bPrev, OnbPrev)
	ON_BN_CLICKED(IDC_bNext, OnbNext)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLOCAL_UPDATE message handlers

INT CLOCAL_UPDATE::SetDefaultMgr (bool sMode , CString parentDM,CString parentCM,CString parentGrid )
{
    
	m_ParentDM     = parentDM   ;
	m_ParentCM     = parentCM   ;
	m_ParentGrid   = parentGrid ;
	m_CheckMode_YN = sMode ;

	return 0;
}


BOOL CLOCAL_UPDATE::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitESL_Mgr(_T("SM_LOCAL_UPDATE"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}
	
	// TODO: Add extra initialization here
	m_CurrentCM = _T("CM_LOCAL_UPDATE") ;

    Screen_Init (0) ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLOCAL_UPDATE::Screen_Init (INT nDir)
{
    
	 CESL_DataMgr *pDataMgr = FindDM(m_ParentDM );
	 if(pDataMgr == NULL)
	 {
		 AfxMessageBox(_T("FindDM() Error~!")) ;
		 return -1 ;
	 }
	 CESL_ControlMgr *pCM = FindCM(m_ParentCM ) ;
	 if(pCM == NULL)
	 {
		 AfxMessageBox(_T("FindCM() Error~!")) ;
		 return -2 ;
	 }


	 CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (m_ParentGrid );
	 if(pGrid == NULL)
	 {
		 AfxMessageBox(_T("FindControl() Error`!")) ;
		 return - 3;
	 }

     
     CESL_ControlMgr *pCurrentCM = FindCM(_T("CM_LOCAL_UPDATE"));
	 if(pCurrentCM == NULL)
	 {
		 AfxMessageBox(_T("FindCM() Error~!")) ;
		 return -4 ;
	 }



	 INT  ids ;
	 switch (nDir)
	 {
	 case 0 :
		 
		 SelectMakeList(m_ParentCM ,m_ParentGrid );
		 ids = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid );
		 if (ids < 0)
		 {
			 AfxMessageBox(_T("선택 목록을 구성할 수 없습니다."));	// 에러
			 EndDialog(IDCANCEL);
			 return -222;
		 }
		 break;
		 
	 case -1 :
		 
		 ids = SelectGetPrev (m_ParentCM ,m_ParentGrid );
		 if (ids == -1) {
			 ids = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid );
			 AfxMessageBox(_T("처음 자료입니다."));
			 return -223;
		 }
		 break;
		 
	 case 1 :
		 
		 ids = SelectGetNext (m_ParentCM ,m_ParentGrid );
		 if (ids == -1) {
			 ids = SelectGetTailPosition(m_ParentCM ,m_ParentGrid );
			 AfxMessageBox(_T("마지막 자료입니다."));
			 return -224;
		 }
		 break;
	 }
	 
	 
	 m_idx = ids; 
	 SetReverse(m_ParentCM ,m_ParentGrid , m_idx);
	 
	 pDataMgr->GetCellData (_T("REC_KEY"),m_idx,m_Rec_Key) ;

	 if(m_CheckMode_YN == TRUE)
	 {

	 }
	 else
	 {
        CESL_Control_Element *pCE = NULL;
		POSITION pos = pCurrentCM->Control_List.GetHeadPosition();

		INT ids = -1;
		CString strLinkData;
		CString strLinkDM;
		while (pos)
		{
			pCE = (CESL_Control_Element*)pCurrentCM->Control_List.GetNext(pos);
			if (pCE == NULL) continue;

			// Grid인 경우 넘어간다.
			if (pCE->CTRL_TYPE == _T("G")) continue;

			strLinkData = pCE->LINK_DATA;

			strLinkData.Replace(_T("DM_CENTERUPLOAD_01"), _T("DM_SE_CENTERUPLOAD_01"));
			//strLinkData.Replace(_T("IBS"), _T("ISS"));
			//strLinkData.Replace(_T("BS"), _T("SS"));
			//strLinkData.Replace(_T("BB"), _T("SB"));

			pCE->LINK_DATA = strLinkData;
		}	

	 }
     

	AllControlDisplay (_T("CM_LOCAL_UPDATE"),m_idx) ;
    
	MarcEditor_Init () ;

	return 0 ;
}


INT CLOCAL_UPDATE::MarcEditor_Init () 
{
    INT ids ;
   
    m_marcEditor_0.SubclassDlgItem(IDC_rich_LOCAL_MARC, -1, this);
 // ids = m_pBaseDataMgr->SetMarcMgr(m_pInfo->pMarcMgr);
	
	CString tmp_Stream ;
	ids = GetDataMgrData (m_ParentDM ,_T("MARC"),tmp_Stream,m_idx) ;  	
	if(ids <0) 
	{
		AfxMessageBox(_T("MARC 를 가져오지 못했습니다.!!"));
	}

	m_strStreamMarc=tmp_Stream;
	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
    if(m_CheckMode_YN == TRUE)
	{
		m_pInfo->pMarcMgr->SetMarcGroup (_T("통합마크"));	
	}
	else m_pInfo->pMarcMgr->SetMarcGroup (_T("통합마크"));	


	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc_0))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}
    

	m_marcEditor_0.InitUserAlias();
	m_marcEditor_0.Init(m_pInfo->pMarcMgr, &m_marc_0);
	m_marcEditor_0.Display();

	return 0 ;
}

VOID CLOCAL_UPDATE::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CLOCAL_UPDATE::OnbPrev() 
{
	// TODO: Add your control notification handler code here
	Screen_Init (-1) ;
}

VOID CLOCAL_UPDATE::OnbNext() 
{
	// TODO: Add your control notification handler code here
	Screen_Init (1) ;
}


VOID CLOCAL_UPDATE::OnOK() 
{
}

HBRUSH CLOCAL_UPDATE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}	
	
