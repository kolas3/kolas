// CONTENTMGR_SE_EDIT.cpp : implementation file
//

#include "stdafx.h"
#include "CONTENTMGR_SE_EDIT.h"
#include "CONTENTMGR_SE_ARTICLE.h"

#include "CommonDefine.h"
#include "..\..\..\°øÅë\K2UP_Common\MakeSearchData.h"
//=====================================================
// 2009.04.15 ADD BY PJW : ¿ø¹®ÀÔ·Â
// #include "../../../°øÅë/CAT_DCM_MANAGER/DCM_MANAGER.h"
#include "..\..\..\°øÅë\CAT_DCM_MANAGER/DCM_MANAGER.h"
//=====================================================

//#include _T("..\ContentsMgr_SE\SE_CREATE_ARTICLE1.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SE_EDIT dialog


CCONTENTMGR_SE_EDIT::CCONTENTMGR_SE_EDIT(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_KisaCount = 0;
	AfxInitRichEdit();
}

CCONTENTMGR_SE_EDIT::~CCONTENTMGR_SE_EDIT()
{
	
}
BOOL CCONTENTMGR_SE_EDIT::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}



VOID CCONTENTMGR_SE_EDIT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_SE_EDIT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_SE_EDIT, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_SE_EDIT)
	ON_BN_CLICKED(IDC_bCreateArticle, OnbCreateArticle)
	ON_BN_CLICKED(IDC_bNEXT, OnbNext)
	ON_BN_CLICKED(IDC_bPREV, OnbPrev)
	ON_BN_CLICKED(IDC_bFile, OnbFile)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_BN_CLICKED(IDC_bDCMInput, OnbDCMInput)
	ON_WM_CTLCOLOR()  
	ON_BN_CLICKED(IDC_btnAddDcm, OnbtnAddDcm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SE_EDIT message handlers

BOOL CCONTENTMGR_SE_EDIT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	m_TocEditdlg.MyCreate(this,CRect(12,160,10+650,45+410),_YON_);
  	//m_ChorokDlg.MyCreate(this,&m_TocEditdlg,CRect(0,10,11,11));

    
	if(InitESL_Mgr(_T("SM_CONTENTS_MANAGER_SE"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}
   
	
/*
	m_ParentCM  = _T("CM_CONTENTMGR_SE_MAIN") ;
	m_MainDM    = _T("") ;   
	m_ParentGrid = _T("MainGrid") ;
    m_RECKEY_Alias = _T("") ;
*/  

    m_CurrentDM = _T("DM_CONTENTMGR_SE_TBL")  ;
    m_CurrentCM = _T("CM_CONTENTMGR_SE_EDIT") ;
	m_ParentDM  = _T("DM_CONTENTMGR_SE_MAIN") ;
   
    INT ids ;

    m_ViewerControl2.InitViewer(IDC_VIEW_Grid, this); 
	m_ViewerControl2.SetViewerWidth(20, 75);    

	ids = SetScreen(0) ;

    (CButton*)GetDlgItem(IDC_bNEXT)->ShowWindow(FALSE) ;
    (CButton*)GetDlgItem(IDC_bPREV)->ShowWindow(FALSE) ;
/*
    CString sMokcha ;
	sMokcha= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>Ã¢°£È£\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>¸ñÂ÷\r\nÏéÔéåë / ÛÜäºÓí = 2[!--17806227--]\r\nóÜÊÊŞö / ÑÑÙ¥ãÕ = 4[!--17806228--]\r\nãæßæüÀãæÑºêªÀÇğ¯ìéÒ´ğ¯ìéìíÀÇ³ªÀÇá¶ØĞ / Ë©Øä = 10[!--17806229--]\r\nÜşìÑÙığ¹ÀÇìéÍÅóÌ : í»ë¦ŞÖßÌ°úúçÙ½åĞô£ñ«ëù / ãôìíéÊ = 14[!--17806230--]\r\nñìßæÇÏ¶ó / ÑÑêªÛı = 19[!--17806231--]\r\nãæßæüÀ÷«ßæ¿¡ÓßÇÏ¾ß / ì°ã¯ñç = 22[!--17806232--]\r\nßæüÀÀÇİÕäÌ / ì°àø÷Á = 24[!--17806233--]\r\nÚÅñëïñãêÀÇìéÍÅóÌ / ï÷ÛÛ = 29[!--17806234--]\r\nŞäüåê¡ÔÑÀÇà»ÏÌíºÀÇõóÕÎ¸¦õµÇÏ³ë¶ó / ãéÛ×éë = 34[!--17806235--]\r\nÊÆá¥ÇÏ¶óãæŞÖßÌÀ» / úÔäàßæ = 39[!--17806236--]\r\nÜşìÑÙığ¹ğúğú / ĞÇê® = 42[!--17806237--]\r\nùÓãÌ = 46[!--17806238--]\r\nŞäüåí÷Àºù¼? / ÑÑÙ¥ãÕ = 47[!--17806239--]\r\nïáëùÀÇñÂÛô / îåáÅßæ = 55[!--17806240--]\r\nĞİË§ß£ë´ÑÀ / õğê® = 57[!--17806241--]\r\nãæßæüÀÀÇ×âßÌ / ÍëÚÅ = 64[!--17806242--]\r\nö¬ò©ßöĞàğÚòÄ[!--17806243--]\r\nøºòşÀ»¸ÀÄ¡°í[!--17806244--]\r\n</body>\r\n</vol>\r\n");
    //sMokcha= _T("<id>KSE000011632\r\n<vol>2È£(II)\r\n<body>¸ñÂ÷\r\nÏéÔéåë / ÛÜäºÓí = 2[!--10000000--]\r\nÀÌÀç¿µ ¼ö½Ä¿¹Á¦$$wow~~good~~$$ \r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\<body> Wow!!\r\n</body>\r\n<body>¸ñÂ÷\r\nÏéÔéåë / ÛÜäºÓí = 2[!--10000000--]\r\nóÜÊÊŞö ¼ö½Ä¿¹Á¦ $$\\sum { aa }$$** !$$!!$$!ÑÑÙ¥ãÕ\r\n!$$%%%$$= 10[!--20000000--]\r\nÜşìÑÙığ¹ÀÇìéÍÅóÌ : í»ë¦ŞÖßÌ°úúçÙ½åĞô£ñ«ëù / ãôìíéÊ = 14[!--30000000--]\r\n</body>\r\n") ;
    m_TocEditdlg.SetTocFromString(sMokcha) ;
*/
	//===================================================
	//2009.05.11 ADD BY PJW : ºÎÀå´Ô Áö½Ã»çÇ×À¸·Î DCM¹öÆ° ¼û±è
	(CButton*)GetDlgItem(IDC_btnAddDcm)->ShowWindow(FALSE) ;
	//===================================================

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCONTENTMGR_SE_EDIT::Set_Mgr( CString Parent_CMAlias, 
								   CString Parent_DMAlias , 
								   CString Parent_GridAlias,
								   CString RECKEY_Alias ,
								   INT nRowIndex 
								   ) 
{
	m_ParentCM  =  Parent_CMAlias;
	m_MainDM  =    Parent_DMAlias ;
	m_ParentGrid = Parent_GridAlias ;
	m_RECKEY_Alias = RECKEY_Alias ;
	m_RowIndex = nRowIndex ;
	return ;
}


VOID CCONTENTMGR_SE_EDIT::OnbCreateArticle() 
{
	// TODO: Add your control notification handler code here

	CString sMokcha ;
	//sMokcha= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>Ã¢°£È£\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>¸ñÂ÷\r\nÏéÔéåë / ÛÜäºÓí = 2[!--17806227--]\r\nóÜÊÊŞö / ÑÑÙ¥ãÕ = 4[!--17806228--]\r\nãæßæüÀãæÑºêªÀÇğ¯ìéÒ´ğ¯ìéìíÀÇ³ªÀÇá¶ØĞ / Ë©Øä = 10[!--17806229--]\r\nÜşìÑÙığ¹ÀÇìéÍÅóÌ : í»ë¦ŞÖßÌ°úúçÙ½åĞô£ñ«ëù / ãôìíéÊ = 14[!--17806230--]\r\nñìßæÇÏ¶ó / ÑÑêªÛı = 19[!--17806231--]\r\nãæßæüÀ÷«ßæ¿¡ÓßÇÏ¾ß / ì°ã¯ñç = 22[!--17806232--]\r\nßæüÀÀÇİÕäÌ / ì°àø÷Á = 24[!--17806233--]\r\nÚÅñëïñãêÀÇìéÍÅóÌ / ï÷ÛÛ = 29[!--17806234--]\r\nŞäüåê¡ÔÑÀÇà»ÏÌíºÀÇõóÕÎ¸¦õµÇÏ³ë¶ó / ãéÛ×éë = 34[!--17806235--]\r\nÊÆá¥ÇÏ¶óãæŞÖßÌÀ» / úÔäàßæ = 39[!--17806236--]\r\nÜşìÑÙığ¹ğúğú / ĞÇê® = 42[!--17806237--]\r\nùÓãÌ = 46[!--17806238--]\r\nŞäüåí÷Àºù¼? / ÑÑÙ¥ãÕ = 47[!--17806239--]\r\nïáëùÀÇñÂÛô / îåáÅßæ = 55[!--17806240--]\r\nĞİË§ß£ë´ÑÀ / õğê® = 57[!--17806241--]\r\nãæßæüÀÀÇ×âßÌ / ÍëÚÅ = 64[!--17806242--]\r\nö¬ò©ßöĞàğÚòÄ[!--17806243--]\r\nøºòşÀ»¸ÀÄ¡°í[!--17806244--]\r\n</body>\r\n</vol>\r\n");
	
	//m_TocEditdlg.SetTocFromString(sMokcha) ;
    sMokcha=m_TocEditdlg.GetFinalToc();
	if(m_TocEditdlg.CheckSyntax())return;

	CCONTENTMGR_SE_ARTICLE dlg(0,this) ;
	dlg.GetMokcha (sMokcha) ;
	dlg.GetKeyValue (m_Contents_key,m_sVol_Key,m_Control_Key) ;
	
	/*
	CSE_CREATE_ARTICLE dlg(0,this) ;
	dlg.GetTabCurSel(0) ;
	dlg.GetMokcha (sMokcha) ;
	dlg.GetKeyValue (m_Contents_key,m_sVol_Key,m_Control_Key) ;
	*/

	if(dlg.DoModal() == IDCANCEL)
	{
		INT first,end ;
		CString strBody ;
		strBody = sMokcha ;
		first = strBody.Find (_T("<body>")) ;
		end   = strBody.Find (_T("</body>"));

		strBody = strBody.Mid (first,end-first) ;
		first = strBody.Find (_T("\r\n")) ;

		strBody = strBody.Mid (first+2,strBody.GetLength () ) ;

        sMokcha.Replace (strBody,dlg.sLast_Mokcha ) ;
		
		//sMokcha = sMokcha.Mid (0,sMokcha.GetLength()-2) ; 

		//AfxMessageBox(sMokcha) ;

        CESL_DataMgr *pDM = FindDM(m_CurrentDM ) ;
		if(pDM == NULL) 
		{
			AfxMessageBox(_T("FindDM() Error~!")) ;
			return ;
		}

		pDM->StartFrame () ;
		pDM->InitDBFieldData () ;
		  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.10.24 ÀÌÇĞÁß ¼öÁ¤ 
		//' 'ÀÎ¿ëºÎÈ£ ÀÖÀ»°æ¿ì PARAMETER :true Ãß°¡ 
		pDM->AddDBFieldData (_T("CONTENTS"),_T("STRING"),sMokcha,true) ;
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		pDM->MakeUpdateFrame (_T("SE_CONTENTS_TBL"),_T("VOL_KEY"),_T("NUMERIC"),m_sVol_Key );
		pDM->SendFrame ();
		pDM->EndFrame () ;

		SetScreen(0) ;
	}
}
 
INT CCONTENTMGR_SE_EDIT::SetScreen(INT ndir)
{
	INT ids ;
	

    if(!m_ParentCM.IsEmpty ())
    {
		CESL_ControlMgr *pCM = FindCM(m_ParentCM) ;
		if(pCM == NULL) AfxMessageBox(_T("FindCM() Error~!")) ;
		
		CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (m_ParentGrid) ;
		
		switch(ndir)
		{
		case 0 :
			
			SelectMakeList(m_ParentCM  ,m_ParentGrid);
			ids = SelectGetHeadPosition(m_ParentCM  ,m_ParentGrid);
			if (ids < 0)
			{
				//2004-07-09 ÀÌÇĞÁß 
				//AfxMessageBox(_T("¼±ÅÃ ¸ñ·ÏÀ» ±¸¼ºÇÒ ¼ö ¾ø½À´Ï´Ù."));	// ¿¡·¯
				AfxMessageBox(_T("¼±Á¤ Ç×¸ñÀÌ ¾ø½À´Ï´Ù."));	// ¿¡·¯
				EndDialog(IDCANCEL);
				return -222;
			}
			break;
			
		case -1 :
			
			ids = SelectGetPrev (m_ParentCM  ,m_ParentGrid);
			if (ids == -1) {
				ids = SelectGetHeadPosition(m_ParentCM  ,m_ParentGrid);
				AfxMessageBox(_T("Ã³À½ ÀÚ·áÀÔ´Ï´Ù."));
				return -223;
			}
			break;
			
		case 1 :
			
			ids = SelectGetNext (m_ParentCM  ,m_ParentGrid);
			if (ids == -1) {
				ids = SelectGetTailPosition(m_ParentCM  ,m_ParentGrid);
				AfxMessageBox(_T("¸¶Áö¸· ÀÚ·áÀÔ´Ï´Ù."));
				return -224;
			}
			break;
			
		}
		m_idx = ids; 
		SetReverse(m_ParentCM  ,m_ParentGrid, m_idx);
		
	}
	else
	{
		m_idx = m_RowIndex ;

	}
	
    CESL_DataMgr *pDM = FindDM(m_ParentDM ) ;
    if(pDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -2;
	}

	CESL_DataMgr *pContentDM = FindDM(m_CurrentDM) ;
	if(pContentDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -3;
	}



	CESL_DataMgr *pCurrentDM = FindDM(m_MainDM);
	if(pCurrentDM == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!"));
		return -998 ;
	}
    CString tmpRec_Key ;
	pCurrentDM->GetCellData (m_RECKEY_Alias,m_idx,tmpRec_Key) ;
    
	CString sWhere;
	sWhere.Format (_T("V.REC_KEY = %s"),tmpRec_Key) ;
	pDM->RefreshDataManager (sWhere);
	m_idx = 0 ;

   

    
	CString strSQL ;

	CString sContentYN ;
   // CString sSpecies_Key ;

    CString sControl_no,sTitle,sVol_title,sPublish_date,sPublisher,sPub_Freq,sSub_Vol_Title ;

    pDM->GetCellData (_T("Á¾Å°"),m_idx,m_sSpecies_Key ) ;
	pDM->GetCellData (_T("¸ñÂ÷À¯¹«"),m_idx,sContentYN) ;
	pDM->GetCellData (_T("Á¦¾î¹øÈ£"),m_idx,sControl_no);
    
	pDM->GetCellData (_T("¼­¸í"),m_idx,sTitle);
	pDM->GetCellData (_T("±ÇÈ£¸í"),m_idx,sVol_title);
	pDM->GetCellData (_T("¹ßÇàÀÏ"),m_idx,sPublish_date);
  //  pDM->GetCellData (_T("±ÇÈ£Å°")m_idx,sVol_Key);
	pDM->GetCellData (_T("±ÇÈ£Å°"),m_idx,m_sVol_Key ) ;
	
	pDM->GetCellData (_T("¹ßÇàÀÚ"),m_idx,sPublisher ) ;
    pDM->GetCellData (_T("°£Çàºóµµ"),m_idx,sPub_Freq ) ;
    pDM->GetCellData (_T("ºÎÂ÷Àû±ÇÈ£¸í"),m_idx,sSub_Vol_Title ) ;

    m_Control_Key = sControl_no ; 

	if(sContentYN == _T("Y") || sContentYN == _T("y"))
	{
		pDM->GetCellData (_T("±ÇÈ£Å°"),m_idx,m_sVol_Key ) ;
        
		strSQL.Format (_T(" VOL_KEY = %s"),m_sVol_Key ) ;

        pContentDM->RefreshDataManager (strSQL) ;
		
        pContentDM->GetCellData (_T("¸ñÂ÷"),0,m_Mokcha) ;
       
        basic_Mokcha = m_Mokcha ;

		pContentDM->GetCellData (_T("¿¬¼Ó¸ñÂ÷Å°"),0,m_Contents_key);
        
      
		if(pContentDM->GetRowCount ()>0) 
		{
			(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(TRUE) ;
			(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(TRUE) ;
		    CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
            pIDOK->SetWindowText (_T("ÀúÀå"));
			m_SaveCheck = FALSE ;
			ContentYN = TRUE ;
		}	
		else
		{
		(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(FALSE) ;
		(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(FALSE) ;
	     CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
         pIDOK->SetWindowText (_T("ÀúÀå"));
		 m_SaveCheck = TRUE ;
		 ContentYN = FALSE ;
		 m_Mokcha = _T("") ;
		}

	}
	else
    {
		(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(FALSE) ;
		(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(FALSE) ;
	     CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
         pIDOK->SetWindowText (_T("ÀúÀå"));
		 m_SaveCheck = TRUE ;
		 ContentYN = FALSE ;
		 m_Mokcha = _T("") ;
	}


/*
	CESL_ControlMgr *pCurrentCM_Mgr = FindCM(m_CurrentCM );
	if(pCurrentCM_Mgr == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -22 ;
	}

	pCurrentCM_Mgr->SetControlMgrData (_T("Á¦¾î¹øÈ£"),sControl_no ,-1) ;
	pCurrentCM_Mgr->SetControlMgrData (_T("¼­¸í"),sTitle ,-1) ;
	pCurrentCM_Mgr->SetControlMgrData (_T("±ÇÈ£¸í"),sVol_title,-1) ;
*/    

	//m_Mokcha= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>Ã¢°£È£\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>¸ñÂ÷\r\nÏéÔéåë / ÛÜäºÓí = 2[!--17806227--]\r\nóÜÊÊŞö / ÑÑÙ¥ãÕ = 4[!--17806228--]\r\nãæßæüÀãæÑºêªÀÇğ¯ìéÒ´ğ¯ìéìíÀÇ³ªÀÇá¶ØĞ / Ë©Øä = 10[!--17806229--]\r\nÜşìÑÙığ¹ÀÇìéÍÅóÌ : í»ë¦ŞÖßÌ°úúçÙ½åĞô£ñ«ëù / ãôìíéÊ = 14[!--17806230--]\r\nñìßæÇÏ¶ó / ÑÑêªÛı = 19[!--17806231--]\r\nãæßæüÀ÷«ßæ¿¡ÓßÇÏ¾ß / ì°ã¯ñç = 22[!--17806232--]\r\nßæüÀÀÇİÕäÌ / ì°àø÷Á = 24[!--17806233--]\r\nÚÅñëïñãêÀÇìéÍÅóÌ / ï÷ÛÛ = 29[!--17806234--]\r\nŞäüåê¡ÔÑÀÇà»ÏÌíºÀÇõóÕÎ¸¦õµÇÏ³ë¶ó / ãéÛ×éë = 34[!--17806235--]\r\nÊÆá¥ÇÏ¶óãæŞÖßÌÀ» / úÔäàßæ = 39[!--17806236--]\r\nÜşìÑÙığ¹ğúğú / ĞÇê® = 42[!--17806237--]\r\nùÓãÌ = 46[!--17806238--]\r\nŞäüåí÷Àºù¼? / ÑÑÙ¥ãÕ = 47[!--17806239--]\r\nïáëùÀÇñÂÛô / îåáÅßæ = 55[!--17806240--]\r\nĞİË§ß£ë´ÑÀ / õğê® = 57[!--17806241--]\r\nãæßæüÀÀÇ×âßÌ / ÍëÚÅ = 64[!--17806242--]\r\nö¬ò©ßöĞàğÚòÄ[!--17806243--]\r\nøºòşÀ»¸ÀÄ¡°í[!--17806244--]\r\n</body>\r\n</vol>\r\n");

    m_TocEditdlg.SetTocFromString(m_Mokcha) ;
	
    //m_TocEditdlg.SetYonKeyFunc( (INT (CWnd::*)(TCHAR*))MakePrimaryKey ) ;
   
	//AllControlDisplay (m_currentCM ,m_idx) ;
/*
 == ¿¬¼Ó ======

 Á¦¾î¹øÈ£    <id>
 ±ÇÈ£ key    <key>
 ÃÑ¼­¸í
 ¼­¸í
 ±ÇÈ£¸í
 ¹ßÇàÀÏ
 Ã»±¸±âÈ£
 ÃÑÆäÁö 
*/
	CString strDefualt[8];
    strDefualt[0] = sControl_no ;
    strDefualt[1] = m_sVol_Key ;
	strDefualt[2] = _T("") ;
	strDefualt[3] = sTitle ;
	strDefualt[4] = sVol_title ;
	strDefualt[5] = sPublish_date ;
	strDefualt[6] = _T("") ;
    strDefualt[7] = _T("") ;

	m_TocEditdlg.SetTocDefault (strDefualt) ;


	m_arrBringDefault[0] = _T("<id>")+strDefualt[0]+_T("\r\n");
	m_arrBringDefault[1] = _T("<call>")+strDefualt[1]+_T("\r\n"); 
	m_arrBringDefault[2] = _T("<count>")+strDefualt[2]+_T("\r\n") ;
	m_arrBringDefault[3] = _T("<date>")+strDefualt[3]+_T("\r\n");    
	m_arrBringDefault[4] = _T("<pub>")+strDefualt[4]+_T("\r\n") ;
	m_arrBringDefault[5] = _T("<sertitle>")+strDefualt[5]+_T("\r\n") ;
	m_arrBringDefault[6] = _T("<title>")+strDefualt[6]+_T("\r\n") ;
    //m_arrBringDefault[7] = _T("<body>")+strDefualt[7]+_T("\r\n</body>") ;


	//sPublisher,sPub_Freq,sSub_Vol_Title
    //m_ViewerControl2.SetControlData(_T("Á¦¾î¹øÈ£"),sControl_no);
    m_ViewerControl2.SetControlData(_T("¼­    ¸í"),sTitle);
    m_ViewerControl2.SetControlData(_T("¹ß Çà ÀÚ"),sPublisher);
	m_ViewerControl2.SetControlData(_T("°£Çàºóµµ"),sPub_Freq);
	m_ViewerControl2.SetControlData(_T("±Ç È£ ¸í"),sVol_title);
    m_ViewerControl2.SetControlData(_T("ºÎÂ÷Àû±ÇÈ£¸í"),sSub_Vol_Title);
	m_ViewerControl2.SetControlData(_T("¹ß Çà ÀÏ"),sPublish_date);
    
    m_ViewerControl2.Display();
    
	return 0 ;
}

VOID CCONTENTMGR_SE_EDIT::OnbNext() 
{
	// TODO: Add your control notification handler code here
	SetScreen (1) ;
}

VOID CCONTENTMGR_SE_EDIT::OnbPrev() 
{
	// TODO: Add your control notification handler code here
	SetScreen (-1) ;
}

VOID CCONTENTMGR_SE_EDIT::OnOK() 
{
	// TODO: Add extra validation here
	
	CESL_DataMgr *pContentsDM_Mgr = FindDM(m_CurrentDM);
	if(pContentsDM_Mgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

    CString sButtonText ;
    CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
    pIDOK->GetWindowText (sButtonText) ;
     
     
	CString sContents_RecKey ;

    CESL_DataMgr *pParentDM_Mgr = FindDM(m_ParentDM );
	if(pParentDM_Mgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}
    // ·¹ÄÚµå_KEY,Á¾Á¤º¸_KEY,Á¦¾î¹øÈ£,±â»ç °³¼ö,¸ñÂ÷,ÀÌ¿ëÁ¦ÇÑ±¸ºĞ,ÀÔ·ÂÀÏ,¼öÁ¤ÀÏ 
    CString sSpecies_Key ,sControl_No ,sUselimitCode ,sVol_Key;

    pParentDM_Mgr->GetCellData (_T("Á¾Å°"),m_idx,sSpecies_Key) ;
    pParentDM_Mgr->GetCellData (_T("Á¦¾î¹øÈ£"),m_idx,sControl_No) ;
    pParentDM_Mgr->GetCellData (_T("ÀÌ¿ëÁ¦ÇÑ±¸ºĞ"),m_idx,sUselimitCode) ;
    pParentDM_Mgr->GetCellData (_T("±ÇÈ£Å°"),m_idx,sVol_Key) ;
    

	//TocEditor ¿¡¼­ ¹Ş¾Æ ¿Ã°Í.........../////////////////////////////
 	CString sMokcha ;
   	sMokcha=m_TocEditdlg.GetFinalToc();
	if(m_TocEditdlg.CheckSyntax())return;

	//¼öÁ¤¹öÆ° ´©¸¥ÈÄ ¸ñÂ÷ .....
    last_Mokcha = sMokcha ;

	//³¯Â¥ ½Ã°£ //////////////////////////////////////////////////////////
    CString system_Date ;
	CTime t = CTime::GetCurrentTime () ;
    system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;

    //¸ñÂ÷À¯¹« UPDATE ////////////////////////////////////////////////////
//	CString sContentsYN ;
//	sContentsYN = _T("Y") ;

    CString sMokcha_Key;



    // »õ·Î ÀÔ·Â ÇÒ °æ¿ì¸¸ ÀúÀå //////////////////////////////////////////////////
	if(m_SaveCheck == TRUE )
	{
	   	pContentsDM_Mgr->MakeRecKey (sContents_RecKey);
        
        pContentsDM_Mgr->StartFrame () ;
		pContentsDM_Mgr->InitDBFieldData () ;
		
		pContentsDM_Mgr->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),sContents_RecKey );
        pContentsDM_Mgr->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),sSpecies_Key );
		pContentsDM_Mgr->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),sVol_Key );
		
		///////////////////////////////////////////////////
		//LOC_FLAG ¾ò¾î ¿À±â 
		//  IF ( COUNT >= 1 ) THEN nReturnCnt = 0 ¼ÒÀå 
		//  IF ( COUNT == 0 ) THEN nReturnCnt = 1 ÀÛ¾÷ 

		INT ids = GetLocFlag(pContentsDM_Mgr,sVol_Key);
		if( ids == 0 )
		{
		pContentsDM_Mgr->AddDBFieldData (_T("LOC_FLAG"),_T("STRING"),_T("1") );	
		}
		else if( ids >= 1)
		{
		pContentsDM_Mgr->AddDBFieldData (_T("LOC_FLAG"),_T("STRING"),_T("0") );
		}
		else 
		{
		AfxMessageBox(_T("GetLogFlag() Error~!")) ;
		}

        /////////////////////////////////////////////////////////////////////

		pContentsDM_Mgr->AddDBFieldData (_T("CONTROL_NO"),_T("STRING"),sControl_No  );
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.10.24 ÀÌÇĞÁß ¼öÁ¤ 
		//' 'ÀÎ¿ëºÎÈ£ ÀÖÀ»°æ¿ì PARAMETER :true Ãß°¡ 
		pContentsDM_Mgr->AddDBFieldData (_T("CONTENTS"),_T("STRING"),sMokcha,true );

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		pContentsDM_Mgr->AddDBFieldData (_T("USE_LIMIT_CODE"),_T("STRING"),sUselimitCode );
        pContentsDM_Mgr->AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),system_Date);
        pContentsDM_Mgr->AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),system_Date);
       
		pContentsDM_Mgr->MakeInsertFrame (_T("SE_CONTENTS_TBL")) ;
         
        pContentsDM_Mgr->InitDBFieldData () ;

		pContentsDM_Mgr->AddDBFieldData (_T("CONTENTS_YN"),_T("STRING"),_T("Y")) ;
        pContentsDM_Mgr->MakeUpdateFrame (_T("SE_VOL_TBL"),_T("REC_KEY"),_T("NUMERIC"),sVol_Key);


		pContentsDM_Mgr->SendFrame () ;
		pContentsDM_Mgr->EndFrame () ;
         
		
        pParentDM_Mgr->SetCellData (_T("¸ñÂ÷À¯¹«"),_T("Y"),m_idx) ;
        


		//2003³â 6¿ù 14ÀÏ ¼öÁ¤ 
        AllControlDisplay (m_ParentCM ,-1) ;
		
        //AfxMessageBox (_T("¸ñÂ÷ ÀúÀåÀÌ ¿Ï·á µÇ¾ú½À´Ï´Ù.")) ;

	}
	// ±âÁ¸ÀÇ ¸ñÂ÷°¡ ÀÖÀ»°æ¿ì ¼öÁ¤ //////////////////////////////////////////////////
    else
	{
        //2004-07-21 ÀÌÇĞÁß 
		if(CheckControlNo() == false)
		{
			AfxMessageBox(_T("Á¦¾î¹øÈ£´Â ¼öÁ¤ÇÏ½Ç¼ö ¾ø½À´Ï´Ù.")) ;
		    SetScreen(0) ;

			return ;
		}

	
        pContentsDM_Mgr->GetCellData (_T("¿¬¼Ó¸ñÂ÷Å°"),0,sMokcha_Key) ;
       
		
		pContentsDM_Mgr->StartFrame () ;
		pContentsDM_Mgr->InitDBFieldData () ;
        pContentsDM_Mgr->AddDBFieldData (_T("CONTENTS"),_T("STRING"),sMokcha) ;
        pContentsDM_Mgr->AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),system_Date ) ;
      
        pContentsDM_Mgr->MakeUpdateFrame (_T("SE_CONTENTS_TBL"),_T("REC_KEY"),_T("NUMERIC"),sMokcha_Key ) ;

		
		if(basic_Mokcha.Compare (last_Mokcha) != 0 ) 
		{
			MakeUpdateQuery(pContentsDM_Mgr) ;
		}

		pContentsDM_Mgr->SendFrame () ;
		pContentsDM_Mgr->EndFrame () ;

        Update_Check() ;

	

	}

	SetScreen(0) ;
    
  //    m_TocEditdlg.SetTocFromString(sMokcha) ;
  //	(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(TRUE) ;
  //	CDialog::OnOK();
}

VOID CCONTENTMGR_SE_EDIT::OnbFile() 
{
	// TODO: Add your control notification handler code here
		if(!BringIn(0))AfxMessageBox(_T("¸ñÂ÷¹İÀÔ½ÇÆĞ"));
}
BOOL CCONTENTMGR_SE_EDIT::BringIn(INT nType)
{
	// ÇöÀç °æ·Î¸¦ ÀúÀåÇÑ´Ù.
	INT nCodePage = 0;// UNICODE : 0, UTF-8 : 1, ANSI : 2
	TCHAR szOriginPath[1024] =_T("\0"); //ÇöÀç°æ·Î
	GetCurrentDirectory(1024,szOriginPath);
	// Å½»ö ½ÃÀÛ
	CFileDialog filedlg(true,_T(""),_T(""),OFN_HIDEREADONLY |OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_LONGNAMES|OFN_PATHMUSTEXIST,_T("¸ñÂ÷/ÃÊ·Ï (*.toc) |*.toc|ÅØ½ºÆ® (*.txt) |*.txt|"),this);
POPUPDLG:
	if(_tcscmp(m_szCurrentPath,_T(""))!=0)
		SetCurrentDirectory(m_szCurrentPath);
	if(filedlg.DoModal()==IDOK){
		GetCurrentDirectory(1024,m_szCurrentPath);
		SetCurrentDirectory(szOriginPath);
		CFile file;
		if(!file.Open(filedlg.GetPathName(),CFile::modeRead | CFile::typeBinary)){
			AfxMessageBox(_T("È­ÀÏÀ» ¿­ ¼ö ¾ø½À´Ï´Ù."));
			goto ERR_RETURN;
		}
		else{
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				//===================================================
				//2010.05.24 ADD BY PJW : ÆÄÀÏÀÇ CODE PAGE¿¡ µû¶ó¼­ º¯È¯ÇØ ÁØ´Ù.
//				file.SeekToBegin();
				file.SeekToBegin();
				file.Read( &cUni, 3 );
				if(  0xBBEF == cUni || 0xAAEB == cUni )
				{/// UTFÀÏ °æ¿ì
					//fFile.SetCodePage(CP_UTF8);
					nCodePage = 1;
				}
//				else if( 0xFFFE == cUni )
//				{
//				}
				else
				{/// ANSIÀÏ °æ¿ì
					//fFile.SetCodePage(CP_ACP);
					file.SeekToBegin();
					nCodePage = 2;
				}
				//===================================================						
			}
			// 1. ÆíÁı±âÀÇ ³»¿ë¹é¾÷
			CString strBackup;
			if(nType==0)
				strBackup=m_TocEditdlg.GetFinalToc();
			else
				strBackup=m_ChorokDlg.GetChorokText();

			// 2. ¿ÀÇÂµÈ ½ºÆ®¸² ¿¡¼­ ÆíÁı±â¿¡ ³Ö¾îÁØ´Ù.
			CString strFileStream;
			//===================================================
			//2010.05.24 ADD BY PJW : ÆÄÀÏÀÇ CODE PAGE¿¡ µû¶ó¼­ º¯È¯ÇØ ÁØ´Ù.
//			if(file.ReadHuge( strFileStream.GetBuffer(file.GetLength()),file.GetLength())<=0){
//				strFileStream.ReleaseBuffer();
//				goto ERR_RETURN;
//			}
//			strFileStream.ReleaseBuffer();
			if( nCodePage == 0 )
			{
				if(file.ReadHuge( strFileStream.GetBuffer(file.GetLength()),file.GetLength())<=0){
					strFileStream.ReleaseBuffer();
					goto ERR_RETURN;
				}
				strFileStream.ReleaseBuffer();
			}
			else
			{
				CHAR * cpData;
				DWORD dSize = file.GetLength();

				cpData = (CHAR *)malloc(sizeof(CHAR) * (dSize ));
				file.ReadHuge(cpData, dSize);
				cpData[dSize] = '\0';
				file.Close();

				CharConvert cvt;
				TCHAR *lpBuffer;
				INT nNeedByte;
				if( nCodePage == 2)
				{
					nNeedByte = MultiByteToWideChar( CP_ACP, 0, cpData, -1, NULL, 0 );
					lpBuffer = (TCHAR *)malloc( sizeof( TCHAR )*nNeedByte+sizeof( TCHAR ) );
					cvt.MultiByteToUnicode(cpData, &lpBuffer, &nNeedByte );	
				}
				else
				{
					INT nLen;
					nLen = strlen(cpData);
					nNeedByte = MultiByteToWideChar( CP_UTF8, 0, cpData, nLen, NULL, 0 );
					lpBuffer = (TCHAR *)malloc( sizeof( TCHAR )*nNeedByte+sizeof( TCHAR ) );
					lpBuffer[nNeedByte] = '\0';
					nNeedByte = MultiByteToWideChar(	CP_UTF8, 0, cpData, nLen, lpBuffer, nNeedByte);
				}
//				free(cpData);
//				free(lpBuffer);

				strFileStream.Replace(_T("\r\n"), _T("\n"));
				strFileStream.Replace(_T("\r"), _T("\n"));
				strFileStream.Replace(_T("\n"), _T("\r\n"));
				strFileStream.Format(_T("%s"), lpBuffer);
			}
			//===================================================

			//=====================================================
			//2010.01.18 ADD BY PJW : ¾²·¹±â°ª Á¦°Å ÄÚµå
 			TCHAR cData = 52685;

			for( INT i = strFileStream.GetLength() -1 ; i >= 0 ; i-- )
			{
				if ( cData != strFileStream.GetAt(i) )
				{
					strFileStream = strFileStream.Left(i+1);
					break;
				}
			}
			//=====================================================

			if(nType==0)
			{
				CString strTempStream ;
				strTempStream = ReMake_BringInStream(nType,strFileStream) ;
				m_TocEditdlg.SetTocFromString( strTempStream );
			}
			else
				m_ChorokDlg.SetChorok( strFileStream );

			// 3. ±×´ë·Î ÁøÇàÇÒ°ÍÀÎÁö »ç¿ëÀÚ¿¡°Ô ¹¯´Â´Ù.
			if(AfxMessageBox(_T("°è¼ÓÇÏ½Ã°Ú½À´Ï±î?"),MB_YESNO)==IDNO){
				if(nType==0)m_TocEditdlg.SetTocFromString( strBackup );
				else m_ChorokDlg.SetChorok( strBackup  );
				goto POPUPDLG;
			}
		}
	}
	SetCurrentDirectory(szOriginPath);
	return true;
ERR_RETURN:
	SetCurrentDirectory(szOriginPath);
	return false;
}
VOID CCONTENTMGR_SE_EDIT::OnbDelete() 
{
	// TODO: Add your control notification handler code here
	CString sContents_RecKey ;

	CESL_DataMgr *pContentsDM_Mgr = FindDM(m_CurrentDM);
	if(pContentsDM_Mgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

    CESL_DataMgr *pParentDM_Mgr = FindDM(m_ParentDM );
	if(pParentDM_Mgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}
	 // ·¹ÄÚµå_KEY,Á¾Á¤º¸_KEY,Á¦¾î¹øÈ£,±â»ç °³¼ö,¸ñÂ÷,ÀÌ¿ëÁ¦ÇÑ±¸ºĞ,ÀÔ·ÂÀÏ,¼öÁ¤ÀÏ 
    CString sSpecies_Key ,sControl_No ,sUselimitCode ,sVol_Key;

    pParentDM_Mgr->GetCellData (_T("Á¾Å°"),m_idx,sSpecies_Key) ;
    pParentDM_Mgr->GetCellData (_T("±ÇÈ£Å°"),m_idx,sVol_Key) ;
    
	

    pContentsDM_Mgr->GetCellData (_T("¿¬¼Ó¸ñÂ÷Å°"),0,sContents_RecKey);
   
	// ¸ñÂ÷À¯¹« _T("N") À¸·Î UPDATE 
    pParentDM_Mgr->StartFrame () ;
 
	pParentDM_Mgr->InitDBFieldData () ;
	pParentDM_Mgr->MakeDeleteFrame (_T("SE_CONTENTS_TBL"),_T("VOL_KEY"),_T("NUMERIC"),sVol_Key);

    pParentDM_Mgr->InitDBFieldData () ;    
	pParentDM_Mgr->MakeDeleteFrame (_T("SE_ARTICLE_TBL"),_T("VOL_KEY"),_T("NUMERIC"),sVol_Key);
    
    pParentDM_Mgr->InitDBFieldData () ;
	pParentDM_Mgr->AddDBFieldData (_T("CONTENTS_YN"),_T("STRING"),_T("N")) ;
	pParentDM_Mgr->MakeUpdateFrame (_T("SE_VOL_TBL"),_T("REC_KEY"),_T("NUMERIC"),sVol_Key) ;


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2004-07-27 leehakjoong

	// IDX_TMP_SE_TOC_TBL

	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_pInfo->CONNECTION_INFO ) ;
	CString strQuery ;         //0
	strQuery.Format (_T("select rec_key from idx_se_toc_Tbl where vol_key = %s"),sVol_Key) ;
	dm.RestructDataManager (strQuery) ;

	//	
    CString strArticleKey =_T("") ;
	for(INT i = 0 ; i < dm.GetRowCount () ;i++)
	{
		strArticleKey = dm.GetCellData (i,0) ;

		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{   
			CString strData ;
			strData.Format(_T("ECO$P_DELETE_SETOC$I(%s);"), strArticleKey);
			pParentDM_Mgr->AddFrame(strData);
		}
		
		pParentDM_Mgr->InitDBFieldData ();
		
		CString strTmp_Key ;
		CString system_Date ;
		CTime t = CTime::GetCurrentTime ();
		system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
		
		
		pParentDM_Mgr->MakeRecKey (strTmp_Key) ;
		pParentDM_Mgr->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
		pParentDM_Mgr->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),strArticleKey);
		pParentDM_Mgr->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("D"));
		pParentDM_Mgr->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
		
		INT ids ;
		ids = pParentDM_Mgr->MakeInsertFrame (_T("IDX_TMP_SE_TOC_TBL"));
		if(ids<0) 
		{
			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_SE_TOC_TBL) Error~!"));
			return ;
		}
		
    }

	// IDX_SE_TOC_TBL »èÁ¦ ÇÏ±â
    pParentDM_Mgr->InitDBFieldData () ;    
	pParentDM_Mgr->MakeDeleteFrame (_T("IDX_SE_TOC_TBL"),_T("VOL_KEY"),_T("NUMERIC"),sVol_Key);

 
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




	pParentDM_Mgr->SendFrame () ;
	pParentDM_Mgr ->EndFrame () ;

    pParentDM_Mgr->SetCellData (_T("¸ñÂ÷À¯¹«"),_T("N"),m_idx) ;

	AllControlDisplay (m_ParentCM ,-1) ;
    
	AfxMessageBox (_T("¸ñÂ÷°¡ »èÁ¦ µÇ¾ú½À´Ï´Ù."));

	OnCancel() ;
  
}

VOID CCONTENTMGR_SE_EDIT::OnCancel() 
{
	// TODO: Add extra cleanup here
//	AfxMessageBox(m_KisaCount) ;
     
	SetKisaCount () ;
	CDialog::OnCancel();
}
INT CCONTENTMGR_SE_EDIT::Update_Check()
{
    //¼öÁ¤¹öÆ° ´©¸£±âÀü ¸ñÂ÷  
    basic_Mokcha ;
   
	//¼öÁ¤¹öÆ° ´©¸¥ÈÄ ¸ñÂ÷ 
    last_Mokcha ;

    if(basic_Mokcha.Compare (last_Mokcha) == 0 ) 
	{
		 AfxMessageBox (_T("¸ñÂ÷ ¼öÁ¤»çÇ× ¾ø½À´Ï´Ù.")) ;
	}
    else
	{
       	 AfxMessageBox (_T("¸ñÂ÷°¡ ¼öÁ¤ µÇ¾ú½À´Ï´Ù.±â»ç ¸ñÂ÷¸¦ ¼öÁ¤ÇÕ´Ï´Ù.")) ;
	}
 

	
	return 0 ;
}

CString CCONTENTMGR_SE_EDIT::ReMake_BringInStream(INT nType,CString strStream)
{
	CString str_BringStream ;

	if(nType == 0)
	{  
         m_arrBringDefault[7] = _T("<body>¸ñÂ÷\r\n")+strStream+_T("\r\n</body>") ;
		 for(INT i=0 ;i<8;i++)
		 {
			str_BringStream += m_arrBringDefault[i] ;
		 }
         //Å×½ºÆ®ÈÄ »èÁ¦ ÇÒ°Í .
		 //AfxMessageBox (str_BringStream) ;
		 
	}
	else
	{


	}

    
	return str_BringStream ;
	
}



//  IF ( COUNT >= 1 ) THEN nReturnCnt = 0 ¼ÒÀå 
//  IF ( COUNT == 0 ) THEN nReturnCnt = 1 ÀÛ¾÷ 

INT CCONTENTMGR_SE_EDIT::GetLocFlag(CESL_DataMgr * pDM , CString str_Vol_Key)
{
   
	INT nReturnCnt  ;
	CString strResult ;
    CString strQuery ;
	strQuery.Format (_T("SELECT COUNT(WORKING_STATUS) FROM SE_BOOK_TBL WHERE ")
                     //=====================================================
					 //2008.12.01 UPDATE BY PJW : ´ÜÇà »óÈ£´ëÂ÷ ÀÚ·á»óÅÂ Ãß°¡
// 					 _T("WORKING_STATUS IN('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O') ")
					 // 18.09.27 JMJ KOL.RED.2018.008
					 _T("WORKING_STATUS IN('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O') ")
					 //=====================================================
					 _T("AND VOL_KEY = %s "),str_Vol_Key);
	
	pDM->GetOneValueQuery (strQuery,strResult);

	nReturnCnt = _ttoi(strResult.GetBuffer (0)) ;
    
	return nReturnCnt ;
     
}



INT CCONTENTMGR_SE_EDIT::SetKisaCount ()
{
	//m_sVol_Key ;
    //select count(*) from se_article_tbl where vol_key = m_sVol_Key ;
    CESL_DataMgr *pDM = FindDM(m_ParentDM );
	if(pDM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}
	CString strQuery ;
	strQuery.Format (_T("select count(*) from se_article_tbl where vol_key = %s"),m_sVol_Key) ;
    CString nCount ;
	pDM->GetOneValueQuery (strQuery,nCount) ;

	m_KisaCount = _ttoi(nCount.GetBuffer (0)) ;

	return m_KisaCount ;
}

VOID CCONTENTMGR_SE_EDIT::OnbDCMInput() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	
	CString strDBFLAG ;
	strDBFLAG = _T("M");

	CString sWhere = _T("");
	CString strTempContents = _T(""); 
    CString strBackup = _T("");

	CESL_DataMgr *pDM = FindDM(m_ParentDM );
	if(pDM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!"));
        return ;
	}
     
	//Å×½ºÆ® ÈÄ »èÁ¦ ÇÒ °Í ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//m_strSpeciesKey.Format (_T("%d"),60212958) ;

	sWhere.Format (_T("SELECT CONTENTS FROM CO_DCM_TOC_TBL WHERE SPECIES_KEY = %s and VOL_KEY = %s") ,m_sSpecies_Key ,m_sVol_Key) ;

	pDM->GetOneValueQuery (sWhere,strTempContents) ;
	
	if(strTempContents.IsEmpty ())
	{   
		AfxMessageBox(_T("°ü·Ã ÀÚ·á°¡ ¾ø½À´Ï´Ù."));
		return ;
	}
    else
	{   
		
		strBackup=m_TocEditdlg.GetFinalToc();
        
        CString strDCMStream = _T("");
        //strDCMStream = strTempContents ;
        INT nStart ,nEnd ;
        nStart = strTempContents.Find (_T("<body>"));
        nEnd   = strTempContents.Find (_T("</body>"));
		if(nStart <0 ) return ;

		strDCMStream = strTempContents.Mid (nStart+6, nEnd-nStart-6) ; 
        strDCMStream.TrimRight ();
		strDCMStream.TrimLeft ();

        strTempContents = ReMake_BringInStream(0,strDCMStream) ;
        strTempContents.TrimLeft ();
		strTempContents.TrimRight ();
		m_TocEditdlg.SetTocFromString(strTempContents);

	}
    
    
    // 3. ±×´ë·Î ÁøÇàÇÒ°ÍÀÎÁö »ç¿ëÀÚ¿¡°Ô ¹¯´Â´Ù.
	if(AfxMessageBox(_T("°è¼ÓÇÏ½Ã°Ú½À´Ï±î?"),MB_YESNO)==IDNO)
	{
		m_TocEditdlg.SetTocFromString( strBackup );
		return ;
	}

}


bool CCONTENTMGR_SE_EDIT::CheckControlNo()
{

	
	CString strBasicBody ;
	CString strLastBody ;
	
	strBasicBody = m_Mokcha ;
	strLastBody  = m_TocEditdlg.GetFinalToc();  ;
	
	//AfxMessageBox (strBasicBody);
	//AfxMessageBox (strLastBody) ;


	map_Basic_Mokcha.RemoveAll () ;
	map_Last_Mokcha.RemoveAll () ;
	
	INT first,end ;
	
	first = strBasicBody.Find (_T("<body>")) ;
	end   = strBasicBody.Find (_T("</body>"));
	
	strBasicBody = strBasicBody.Mid (first,end-first) ;
	first = strBasicBody.Find (_T("\r\n")) ;
	
	strBasicBody = strBasicBody.Mid (first,strBasicBody.GetLength ()-(first+2) ) ;
	
	
	
	first = strLastBody.Find (_T("<body>")) ;
	end   = strLastBody.Find (_T("</body>"));
	
	strLastBody = strLastBody.Mid (first,end-first) ;
	first = strLastBody.Find (_T("\r\n")) ;
	
	strLastBody = strLastBody.Mid (first,strLastBody.GetLength ()-(first+2) ) ;
	
	//AfxMessageBox (strBasicBody +_T("\r\n\r\n")+strLastBody) ;
	
	INT nPos ;
	INT nIndex;
	CString strLine ;
	CString strFeildData ;
	CString strControlNo ;
	CString sDelimiter ;
	sDelimiter = _T("\r\n") ;
	bool IsStop ;
	
	for(INT i = 0 ;i < 2; i++)
	{
		IsStop = FALSE;

		if(i == 0 )strLine = strBasicBody;
		else strLine = strLastBody ;
		
		while( TRUE )
		{
			nIndex = strLine.Find(sDelimiter);
			
			if( nIndex < 0 )
			{
				if(strLine.Find (_T("[!--")) == -1)
				{
					IsStop = TRUE;
					//ontinue ;
				}
				else
				{
					nPos = strLine.Find (_T("[!--")) ;
					strFeildData = strLine.Mid (0,nPos) ;
					strControlNo = strLine.Mid (nPos+4,23) ;
					
					if(i == 0)map_Basic_Mokcha.SetAt (strControlNo,strFeildData) ;
					else map_Last_Mokcha.SetAt (strControlNo,strFeildData) ;
					
				}
				IsStop = TRUE;
			}
			else
			{
				if(strLine.Find (_T("[!--")) == -1)
				{
					//continue ;
				}
				else
				{
					nPos = strLine.Find (_T("[!--")) ;
					strFeildData = strLine.Mid (0,nPos) ;
					strControlNo = strLine.Mid (nPos+4,23) ;
			
					strFeildData.TrimLeft () ; strFeildData.TrimRight () ;
					
					if(i== 0)map_Basic_Mokcha.SetAt (strControlNo,strFeildData) ;
					else map_Last_Mokcha.SetAt (strControlNo,strFeildData) ;
					
				}

				strLine = strLine.Mid( nIndex+2 , strLine.GetLength()-nIndex );
				strLine.TrimLeft () ;strLine.TrimRight () ;
			}
			
			if( IsStop ) break;
		}
		
	}
	
	//Á¦¾î¹øÈ£ Ã¼Å© ÇÏ±â 
    INT ids ;
	CString strCompareLine ;
	CString strUpdateLine ;
	CString strCompareControlNo ;
	CString strUpdateControlNo ;
	CStringArray arrUpdateData ;

	POSITION pos ;
	for(pos = map_Basic_Mokcha.GetStartPosition () ; pos != NULL ;)
	{
	
		map_Basic_Mokcha.GetNextAssoc (pos,strCompareControlNo,strCompareLine ) ;
		
		ids = map_Last_Mokcha.Lookup (strCompareControlNo,strUpdateLine);
		if(ids == 0 )
		{
			return FALSE ;
		}
		
	}

	return TRUE ;
}
INT CCONTENTMGR_SE_EDIT::MakeUpdateQuery (CESL_DataMgr *pDM) 
{
	
//	CMapStringToString map_Basic_Mokcha;
//	CMapStringToString map_Last_Mokcha; 
	
	CString strBasicBody ;
	CString strLastBody ;
	
	strBasicBody = basic_Mokcha ;
	strLastBody  = last_Mokcha  ;
	
//	AfxMessageBox (strBasicBody) ;
//	AfxMessageBox (strLastBody) ;

	map_Basic_Mokcha.RemoveAll () ;
	map_Last_Mokcha.RemoveAll () ;
	
	INT first,end ;
	
	first = strBasicBody.Find (_T("<body>")) ;
	end   = strBasicBody.Find (_T("</body>"));
	
	strBasicBody = strBasicBody.Mid (first,end-first) ;
	first = strBasicBody.Find (_T("\r\n")) ;
	
	strBasicBody = strBasicBody.Mid (first,strBasicBody.GetLength ()-(first+2) ) ;
	
	
	
	first = strLastBody.Find (_T("<body>")) ;
	end   = strLastBody.Find (_T("</body>"));
	
	strLastBody = strLastBody.Mid (first,end-first) ;
	first = strLastBody.Find (_T("\r\n")) ;
	
	strLastBody = strLastBody.Mid (first,strLastBody.GetLength ()-(first+2) ) ;
	
	//AfxMessageBox (strBasicBody +_T("\r\n\r\n")+strLastBody) ;
	
	INT nPos ;
	INT nIndex;
	CString strLine ;
	CString strFeildData ;
	CString strControlNo ;
	CString sDelimiter ;
	sDelimiter = _T("\r\n") ;
	bool IsStop ;
	
	for(INT i = 0 ;i < 2; i++)
	{
		IsStop = FALSE;

		if(i == 0 )strLine = strBasicBody;
		else strLine = strLastBody ;
		
		while( TRUE )
		{
			nIndex = strLine.Find(sDelimiter);
			
			if( nIndex < 0 )
			{
				if(strLine.Find (_T("[!--")) == -1)
				{
					IsStop = TRUE;
					//ontinue ;
				}
				else
				{
					nPos = strLine.Find (_T("[!--")) ;
					strFeildData = strLine.Mid (0,nPos) ;
					strControlNo = strLine.Mid (nPos+4,23) ;
					
					if(i == 0)map_Basic_Mokcha.SetAt (strControlNo,strFeildData) ;
					else map_Last_Mokcha.SetAt (strControlNo,strFeildData) ;
					
				}
				IsStop = TRUE;
			}
			else
			{
				if(strLine.Find (_T("[!--")) == -1)
				{
					//continue ;
				}
				else
				{
					nPos = strLine.Find (_T("[!--")) ;
					strFeildData = strLine.Mid (0,nPos) ;
					strControlNo = strLine.Mid (nPos+4,23) ;
			
					strFeildData.TrimLeft () ; strFeildData.TrimRight () ;
					
					if(i== 0)map_Basic_Mokcha.SetAt (strControlNo,strFeildData) ;
					else map_Last_Mokcha.SetAt (strControlNo,strFeildData) ;
					
				}

				strLine = strLine.Mid( nIndex+2 , strLine.GetLength()-nIndex );
				strLine.TrimLeft () ;strLine.TrimRight () ;
			}
			
			if( IsStop ) break;
		}
		
	}





	//¸ñÂ÷ ¼öÁ¤µÈ ³»¿ë Ã¼Å© ÈÄ ¼öÁ¤ÇÏ±â 
	INT ids ;
	CString strCompareLine ;
	CString strUpdateLine ;
	CString strUpdateControlNo ;
	CStringArray arrUpdateData ;
	BOOL bFirstCheck  ;
	BOOL bSecondCheck ;
	
	POSITION pos ;
	for(pos = map_Basic_Mokcha.GetStartPosition () ; pos != NULL ;)
	{
		bFirstCheck = false ;
		bSecondCheck = false ;

		map_Basic_Mokcha.GetNextAssoc (pos,strUpdateControlNo,strCompareLine ) ;
		
		ids = map_Last_Mokcha.Lookup (strUpdateControlNo,strUpdateLine);
		if(ids != 0 && strCompareLine.Compare (strUpdateLine) != 0)
		{
            if(strUpdateLine.Find (_T("/")) != -1) bFirstCheck = true ;
			if(strUpdateLine.Find (_T("=")) != -1) bSecondCheck = true ;
			
			arrUpdateData.RemoveAll () ;
			TCHAR seps[2] = {'/','='} ;
			TCHAR *token;
			
			token = _tcstok( strUpdateLine.GetBuffer (0), seps );
			while( token != NULL )
			{
				arrUpdateData.Add (token) ;
				token = _tcstok( NULL, seps );
			}
			
			CString strAticle,strAuthor,strPage ;
            
			if(bFirstCheck == true && bSecondCheck == true )
			{
				strAticle = arrUpdateData.GetAt (0) ;
				strAuthor = arrUpdateData.GetAt (1) ;
				strPage = arrUpdateData.GetAt (2) ;
			}
			else if ( bFirstCheck == true && bSecondCheck == false)
			{
				strAticle = arrUpdateData.GetAt (0) ;
				strAuthor = arrUpdateData.GetAt (1) ;
				strPage = _T("") ;	
			}
			else if ( bFirstCheck == false && bSecondCheck == true)
			{
				strAticle = arrUpdateData.GetAt (0) ;
				strAuthor = _T("") ;
				strPage = arrUpdateData.GetAt (1) ;
			}
			else if ( bFirstCheck == false && bSecondCheck == false && arrUpdateData.GetSize () == 1)
			{
				strAticle = arrUpdateData.GetAt (0) ;
				strAuthor = _T("") ;
				strPage = _T("") ;
			}

			MakeFrameQuery(pDM,strUpdateControlNo,strAticle,strAuthor,strPage) ;
			
		}
		
	}
	
	return 0 ;
}

INT CCONTENTMGR_SE_EDIT::MakeFrameQuery(CESL_DataMgr *pDM, CString strKey, CString strArticle, CString strAuthor, CString strPage)
{
    INT ids ;
	CString strIndexArticle,strIndexAuthor ,strAllItem ;

	CMakeSearchData *pMakeData = NULL; 
    pMakeData = new CMakeSearchData(this);


	strIndexArticle = strArticle ;
	strIndexAuthor  = strAuthor ;

	INT iCh = pMakeData->ChangeHanjaToHangul( strIndexArticle );	//ÇÑÀÚ ÇÑ±Û º¯È¯
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexArticle ) error") );
		return -1;
	}
	iCh = pMakeData->GetKeyWord( strIndexArticle );				//°ø¹é, Æ¯¼ö¹®ÀÚ Á¦°Å
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexArticle ) error") );
		return -2;
	}

	iCh = pMakeData->ChangeHanjaToHangul( strIndexAuthor );	//ÇÑÀÚ ÇÑ±Û º¯È¯
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexAuthor ) error") );
		return -1;
	}
		   
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


	strAllItem.Format (_T("%s %s"),strIndexArticle,strIndexAuthor) ;


	CString strArticleKey = _T("") ;
	CString strQuery ;
	strQuery.Format (_T("select rec_key from idx_se_toc_tbl where article_control_no = '%s'"),strKey);
	pDM->GetOneValueQuery (strQuery ,strArticleKey ) ;
    
	
	//1. IDX_SE_TOC_TBL
	pDM->InitDBFieldData ();	
	pDM->AddDBFieldData (_T("IDX_ARTICLE"),_T("STRING"),strIndexArticle) ;
	pDM->AddDBFieldData (_T("IDX_IARTICLE"),_T("STRING"),strIndexArticle) ;
	
	pDM->AddDBFieldData (_T("IDX_ARTICLE_KEYWORD"),_T("STRING"),strIndexArticle);
	pDM->AddDBFieldData (_T("IDX_ARTICLE_AUTHOR"),_T("STRING"),strIndexAuthor );
	pDM->AddDBFieldData (_T("IDX_ALL_ITEM"),_T("STRING"),strAllItem );
	
	pDM->AddDBFieldData (_T("ARTICLE_INFO"),_T("STRING"),strArticle );
	pDM->AddDBFieldData (_T("ARTICLE_AUTHOR_INFO"),_T("STRING"),strAuthor  );
	pDM->AddDBFieldData (_T("ARTICLE_PAGE_INFO"),_T("STRING"),strPage );
	
	pDM->MakeUpdateFrame (_T("IDX_SE_TOC_TBL"),_T("REC_KEY"),_T("NUMERIC"),strArticleKey);

	
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{   
		CString strData ;
		strData.Format(_T("ECO$P_MAKE_SETOC$I(%s);"), strArticleKey);
		pDM->AddFrame(strData);
	}

	//2. SE_ARTICLE_TBL
	
	pDM->InitDBFieldData ();
	pDM->AddDBFieldData (_T("ARTICLE_NAME"),_T("STRING"),strArticle );
	pDM->AddDBFieldData (_T("ARTICLE_AUTHOR"),_T("STRING"),strAuthor);
	pDM->AddDBFieldData (_T("START_PAGE"),_T("STRING"),strPage);
	pDM->MakeUpdateFrame  (_T("SE_ARTICLE_TBL"),_T("REC_KEY"),_T("NUMERIC"),strArticleKey);
	

	
	CString strTmp_Key ;
	CString system_Date ;
	CTime t = CTime::GetCurrentTime ();
	system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
	
	//3. IDX_TMP_SE_TOC_TBL
     
	pDM->MakeRecKey (strTmp_Key) ;
	pDM->InitDBFieldData ();
	pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
	pDM->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),strArticleKey);
	pDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
	pDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
	
	ids = pDM->MakeInsertFrame (_T("IDX_TMP_SE_TOC_TBL"));
	if(ids<0) 
	{
		AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_SE_TOC_TBL) Error~!"));
		return -1;
	}


	delete  pMakeData ; 
    pMakeData = NULL ;

				
    return 0 ;
}


HBRUSH CCONTENTMGR_SE_EDIT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

//void CCONTENTMGR_SE_EDIT::OnbtnDcmAdd() 
//{
//	CESL_DataMgr *pContentDM = FindDM(m_CurrentDM) ;
//	if(pContentDM == NULL) {
//		AfxMessageBox(_T("FindDM() Error~!")) ;
//		return ;
//	}
//
//// 	INT nRow;
//	CString strVolKey;
//	strVolKey = pContentDM->GetCellData(_T("±ÇÈ£Å°"), 0);
//	if( strVolKey.IsEmpty() )
//	{
//		AfxMessageBox(_T("¿ø¹® Ãß°¡ÇÒ Ã¥À» ¼±ÅÃÇØ ÁÖ¼¼¿ä!"));
//		return ;
//	}
//
//	CDCM_MANAGER dlg(this);
//	dlg.SetParentDM( pContentDM );
//// 	dlg.SetParentGRID(this->m_pGrid);
//	dlg.SetDbFlag(_T("A"));
//	dlg.DoModal();
//	
//	return ;
//}

void CCONTENTMGR_SE_EDIT::OnbtnAddDcm() 
{
	CESL_DataMgr *pContentDM = FindDM(m_CurrentDM) ;
	if(pContentDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

	CString strVolKey;
	strVolKey = pContentDM->GetCellData(_T("±ÇÈ£Å°"), 0);
	if( strVolKey.IsEmpty() )
	{
		AfxMessageBox(_T("¿ø¹® Ãß°¡ÇÒ Ã¥À» ¼±ÅÃÇØ ÁÖ¼¼¿ä!"));
		return ;
	}

	CDCM_MANAGER dlg(this);
	dlg.SetParentDM( pContentDM );
// 	dlg.SetParentGRID(this->m_pGrid);
	dlg.SetDbFlag(_T("A"));
	dlg.DoModal();
	
	return ;
	
}
