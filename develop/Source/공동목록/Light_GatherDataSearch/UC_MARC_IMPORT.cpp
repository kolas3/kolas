// UC_MARC_IMPORT.cpp : implementation file
//

#include "stdafx.h"
#include "UC_MARC_IMPORT.h"
#include "MARC_VIEW.h"


#include "..\UI_SEARCH_DLG\Detail_Search_Dlg.h"
#include "..\UI_SEARCH_DLG\Search_Main_Dlg.h"

#include "..\check_dup_api\RS_SQL.h"

#include "UC_KOLISNetUseChecker.h"

#include "KolisNetService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUC_MARC_IMPORT dialog


CUC_MARC_IMPORT::CUC_MARC_IMPORT(CESL_Mgr* pParent /*=NULL*/)
	:  CESL_Mgr(IDD, pParent)
{

	m_strTitle = _T("");
	m_strAuthor = _T("");
	m_strPublisher = _T("");
	m_strPubYear = _T("");
	//{{AFX_DATA_INIT(CUC_MARC_IMPORT)
	//}}AFX_DATA_INIT
}

CUC_MARC_IMPORT::~CUC_MARC_IMPORT()
{
}

BOOL CUC_MARC_IMPORT::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CUC_MARC_IMPORT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUC_MARC_IMPORT)
	DDX_Control(pDX, IDC_SE_PROGRESS, m_SeProgressBar);
	DDX_Control(pDX, IDC_BO_PROGRESS, m_BoProgressBar);
	DDX_Control(pDX, IDC_TAB, m_TabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUC_MARC_IMPORT, CDialog)
	//{{AFX_MSG_MAP(CUC_MARC_IMPORT)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bMARC, OnbMARC)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_BN_CLICKED(IDC_bMokcha, OnbMokcha)
	ON_BN_CLICKED(IDC_bChorok, OnbChorok)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_THREAD_END, OnSearchFinished)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUC_MARC_IMPORT message handlers

BOOL CUC_MARC_IMPORT::OnInitDialog() 
{
	CDialog::OnInitDialog();
   
	m_TabCtrl.InsertItem (0,_T("KOLIS-NET"));
	m_TabCtrl.InsertItem (1,_T("출판유통진흥원"));	

   	CMSHFlexGrid *pGrid1 = (CMSHFlexGrid*)GetDlgItem(IDC_gridBo);
    CMSHFlexGrid *pGrid2 = (CMSHFlexGrid*)GetDlgItem(IDC_gridSe);
	CMSHFlexGrid *pGrid3 = (CMSHFlexGrid*)GetDlgItem(IDC_gridChulpan);

	pGrid1->ShowWindow(SW_SHOW);
    pGrid2->ShowWindow(SW_HIDE);
	pGrid3->ShowWindow(SW_HIDE);

	// TODO: Add extra initialization here

	if (InitESL_Mgr(_T("찾기")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pDbDelete;
	pDbDelete=(CButton *)GetDlgItem(IDC_rAll);
	pDbDelete->SetCheck(1);

	m_pBoResultCtl = GetDlgItem(IDC_eBO_COUNT);
    m_pSeResultCtl = GetDlgItem(IDC_eSE_COUNT);
	m_pChResultCtl = GetDlgItem(IDC_eChulpan_COUNT);

	(CButton*)GetDlgItem(IDC_bMokcha)->EnableWindow(FALSE);
    (CButton*)GetDlgItem(IDC_bChorok)->EnableWindow(FALSE);
    (CButton *)GetDlgItem(IDC_CHECK_Cho)->EnableWindow(FALSE);
    (CButton *)GetDlgItem(IDC_CHECK_Moc)->EnableWindow(FALSE);
    
 
	m_TocEditdlg.MyCreate(this,CRect(0,10,11,11),_DAN_SINGLE);
  	m_ChorokDlg.MyCreate(this,&m_TocEditdlg,CRect(0,10,11,11));
 
	m_TabCtrl.SetCurFocus (0) ;
    m_TabCtrl.SetCurSel (0);
	ChangedTab();
    
	m_CurrentDM =_T("DM_단행자료_DB") ;
	m_Master_DM =_T("DM_서지마스터_단행");

    SetDlgItemText (IDC_eBO_COUNT,_T("0"));
    SetDlgItemText (IDC_eSE_COUNT,_T("0"));
	SetDlgItemText (IDC_eChulpan_COUNT,_T("0"));
    	
    CESL_DataMgr * pDataMgr;

	pDataMgr = FindDM(m_Master_DM) ;

    CRS_SQL m_RS_SQL ;
	CString strQuery;


	UC_KOLISNetUseChecker KolisnetUseChk(this);
	if ( KolisnetUseChk.GetUseFlag() == FALSE )
	{
		CDialog::OnCancel();
		return FALSE;
	}
    
	m_CheckFirst = FALSE ;

	CComboBox *pCtrlCombo = (CComboBox *)GetDlgItem (IDC_COMBO_SELECT) ; 
	pCtrlCombo->ResetContent() ;
	pCtrlCombo->AddString(_T("YES"));
	pCtrlCombo->AddString(_T("NO"));	
	pCtrlCombo->SetCurSel(0) ;

	SetTimer(1, 300, NULL);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CUC_MARC_IMPORT::OnbSEARCH() 
{   
	CKolisNetSearchParam* pSearchParam = m_BoSearchFish.GetKolisNetSearchParam();
	pSearchParam->ReleaseParam();

	CSearch_Main_Dlg *pDlg = NULL;
	pDlg = new CSearch_Main_Dlg(pSearchParam, this);
	pDlg->SetTitleString(_T("자료찾기"));
	pDlg->SetSearchCondition(m_strTitle, m_strAuthor, m_strPublisher, m_strPubYear);
    pDlg->SetDialogType(_WORK_SEARCH);
    pDlg->SetDispAlias(_T("찾기"));
    
	INT ids = pDlg->DoModal();
	if (ids != IDOK ) 
	{    
		delete pDlg; 
		return;
	}
    
	TCHAR  szSortInfo[128], cDanCollectionName[128];    
	INT    CollectionType , GetQueryRang;
	TCHAR  m_szBoQuery[1024], m_szSeQuery[1024], m_szChQuery[1024];

    pDlg->GetCollectionType(&CollectionType);
	pDlg->GetQueryRang(&GetQueryRang);
	pDlg->GetQuery(m_szBoQuery , _DAN_);
	pDlg->GetQuery(m_szSeQuery , _YON_);
	pDlg->GetQuery(m_szChQuery , _CHULPAN_);
	pDlg->GetSortInfo(szSortInfo);
	pDlg->GetCollection(cDanCollectionName);

    switch ( CollectionType )
	{
	case _DAN_ :
		{
			pSearchParam->SetPUB_FORM_TYPE(CKolisNetSearchParam::PUB_FORM_TYPE::PUB_FORM_TYPE_UB);
			m_BoSearchFish.SetSortInfo(szSortInfo);
			
			CKolisNetDataMapper* pMapper = m_BoSearchFish.GetKolisNetDataMapper();
			pMapper->ReleaseData();
			pMapper->AddData(KOLISNET_PARAM_AUTHOR_INFO, _T("저작자"));
			pMapper->AddData(KOLISNET_PARAM_CONTROL_NO, _T("제어번호"));
			pMapper->AddData(KOLISNET_PARAM_FORM_CODE, _T("형식구분"));
			pMapper->AddData(KOLISNET_PARAM_MAT_CODE, _T("자료구분"));
			pMapper->AddData(KOLISNET_PARAM_MEDIA_CODE, _T("매체구분"));	
			pMapper->AddData(KOLISNET_PARAM_PUB_FREQ, _T("간행빈도"));
			pMapper->AddData(KOLISNET_PARAM_PUB_INFO, _T("발행자"));
			pMapper->AddData(KOLISNET_PARAM_PUB_YEAR_INFO, _T("발행년"));
			pMapper->AddData(KOLISNET_PARAM_TITLE_INFO, _T("표제"));
			pMapper->AddData(KOLISNET_PARAM_ID, _T("서지마스터키"));
			pMapper->AddData(KOLISNET_PARAM_TOC_YN, _T("목차유무"));
			pMapper->AddData(KOLISNET_PARAM_ABS_YN, _T("초록유무"));
			pMapper->AddData(KOLISNET_PARAM_ISBN, _T("ISBN"));
			
			ids = m_BoSearchFish.Initialize( this , &m_BoProgressBar , _T("찾기") , _T("DM_단행자료_DB") , _T("CM_찾기") 
											, _T("MainBoGrid") , cDanCollectionName,m_szBoQuery, m_pBoResultCtl 
											, 2000 , 2000 , FALSE , pSearchParam , pMapper );
		
			if (ids != 0)
			{
				delete pDlg;
				return ;
			}

			m_BoSearchFish.InitDiaplay();		
			m_BoSearchFish.SetAppendDisplay(FALSE);
			m_pBoThread = m_BoSearchFish.SearchDiaplay();
        }
        break;
	case _CHULPAN_ :
		{
			CKolisNetSearchParam* pSearchParam2 = m_ChSearchFish.GetKolisNetSearchParam();
			pSearchParam2->ReleaseParam();

			pSearchParam->SetPUB_FORM_TYPE(CKolisNetSearchParam::PUB_FORM_TYPE::PUB_FORM_TYPE_UT);

			*pSearchParam2 = *pSearchParam;

			m_ChSearchFish.SetSortInfo( szSortInfo );
			
			CKolisNetDataMapper* pMapper = m_ChSearchFish.GetKolisNetDataMapper();
			pMapper->ReleaseData();
			pMapper->AddData(KOLISNET_PARAM_AUTHOR_INFO, _T("저작자"));
			pMapper->AddData(KOLISNET_PARAM_CONTROL_NO, _T("제어번호"));
			pMapper->AddData(KOLISNET_PARAM_FORM_CODE, _T("형식구분"));
			pMapper->AddData(KOLISNET_PARAM_MAT_CODE, _T("자료구분"));
			pMapper->AddData(KOLISNET_PARAM_MEDIA_CODE, _T("매체구분"));	
			pMapper->AddData(KOLISNET_PARAM_PUB_FREQ, _T("간행빈도"));
			pMapper->AddData(KOLISNET_PARAM_PUB_INFO, _T("발행자"));
			pMapper->AddData(KOLISNET_PARAM_PUB_YEAR_INFO, _T("발행년"));
			pMapper->AddData(KOLISNET_PARAM_TITLE_INFO, _T("표제"));
			pMapper->AddData(KOLISNET_PARAM_ID, _T("서지마스터키"));
			pMapper->AddData(KOLISNET_PARAM_TOC_YN, _T("목차유무"));
			pMapper->AddData(KOLISNET_PARAM_ABS_YN, _T("초록유무"));

			ids = m_ChSearchFish.Initialize ( this , &m_SeProgressBar, _T("찾기") , _T("DM_출판유통자료_DB") , _T("CM_찾기") 
											, _T("MainChulpanGrid") , _T("test-btp-01") ,m_szChQuery,  m_pChResultCtl
											, 2000 , 2000 , FALSE , pSearchParam2 , pMapper );
			if (ids != 0) {
				delete pDlg;
				return ;
			}
			m_ChSearchFish.InitDiaplay();
			m_ChSearchFish.SetAppendDisplay(FALSE);
			m_pChThread = m_ChSearchFish.SearchDiaplay();        
		}
        break;

    case _ALL_ :
		{
			pSearchParam->SetPUB_FORM_TYPE(CKolisNetSearchParam::PUB_FORM_TYPE::PUB_FORM_TYPE_UB);
			m_BoSearchFish.SetSortInfo( szSortInfo );

			CKolisNetDataMapper* pMapper = m_BoSearchFish.GetKolisNetDataMapper();
			pMapper->ReleaseData();
			pMapper->AddData(KOLISNET_PARAM_AUTHOR_INFO, _T("저작자"));
			pMapper->AddData(KOLISNET_PARAM_CONTROL_NO, _T("제어번호"));
			pMapper->AddData(KOLISNET_PARAM_FORM_CODE, _T("형식구분"));
			pMapper->AddData(KOLISNET_PARAM_MAT_CODE, _T("자료구분"));
			pMapper->AddData(KOLISNET_PARAM_MEDIA_CODE, _T("매체구분"));	
			pMapper->AddData(KOLISNET_PARAM_PUB_FREQ, _T("간행빈도"));
			pMapper->AddData(KOLISNET_PARAM_PUB_INFO, _T("발행자"));
			pMapper->AddData(KOLISNET_PARAM_PUB_YEAR_INFO, _T("발행년"));
			pMapper->AddData(KOLISNET_PARAM_TITLE_INFO, _T("표제"));
			pMapper->AddData(KOLISNET_PARAM_ID, _T("서지마스터키"));
			pMapper->AddData(KOLISNET_PARAM_TOC_YN, _T("목차유무"));
			pMapper->AddData(KOLISNET_PARAM_ABS_YN, _T("초록유무"));
			pMapper->AddData(KOLISNET_PARAM_ISBN, _T("ISBN"));

			ids = m_BoSearchFish.Initialize ( this , &m_BoProgressBar, _T("찾기") , _T("DM_단행자료_DB") , _T("CM_찾기") 
											, _T("MainBoGrid") , cDanCollectionName,m_szBoQuery, m_pBoResultCtl
											, 2000 , 2000 , FALSE , pSearchParam , pMapper );
			if ( ids != 0 ) {
				delete pDlg;
				return ;
			}
			m_BoSearchFish.InitDiaplay( );
			m_BoSearchFish.SetAppendDisplay( FALSE );
			m_pBoThread = m_BoSearchFish.SearchDiaplay( );

			CKolisNetSearchParam* pSearchParam2 = m_ChSearchFish.GetKolisNetSearchParam();
			pSearchParam2->ReleaseParam();

			*pSearchParam2 = *pSearchParam;
			pSearchParam2->SetPUB_FORM_TYPE(CKolisNetSearchParam::PUB_FORM_TYPE::PUB_FORM_TYPE_UT);

			m_ChSearchFish.SetSortInfo( szSortInfo );

			pMapper = m_ChSearchFish.GetKolisNetDataMapper();
			pMapper->ReleaseData();
			pMapper->AddData(KOLISNET_PARAM_AUTHOR_INFO, _T("저작자"));
			pMapper->AddData(KOLISNET_PARAM_CONTROL_NO, _T("제어번호"));
			pMapper->AddData(KOLISNET_PARAM_FORM_CODE, _T("형식구분"));
			pMapper->AddData(KOLISNET_PARAM_MAT_CODE, _T("자료구분"));
			pMapper->AddData(KOLISNET_PARAM_MEDIA_CODE, _T("매체구분"));	
			pMapper->AddData(KOLISNET_PARAM_PUB_FREQ, _T("간행빈도"));
			pMapper->AddData(KOLISNET_PARAM_PUB_INFO, _T("발행자"));
			pMapper->AddData(KOLISNET_PARAM_PUB_YEAR_INFO, _T("발행년"));
			pMapper->AddData(KOLISNET_PARAM_TITLE_INFO, _T("표제"));
			pMapper->AddData(KOLISNET_PARAM_ID, _T("서지마스터키"));
			pMapper->AddData(KOLISNET_PARAM_TOC_YN, _T("목차유무"));
			pMapper->AddData(KOLISNET_PARAM_ABS_YN, _T("초록유무"));
	
			ids = m_ChSearchFish.Initialize ( this , &m_SeProgressBar, _T("찾기") , _T("DM_출판유통자료_DB") , _T("CM_찾기") 
											, _T("MainChulpanGrid") , _T("test-btp-01") ,m_szChQuery,  m_pChResultCtl
											, 2000 , 2000 , FALSE , pSearchParam2 , pMapper );
			if ( ids != 0 ) {
				delete pDlg;
				return ;
			}
			m_ChSearchFish.InitDiaplay( );
			m_ChSearchFish.SetAppendDisplay( FALSE );
			m_pChThread = m_ChSearchFish.SearchDiaplay( );
		}
        break;
    }  
	
    CESL_ControlMgr *pCM= FindCM(_T("CM_찾기"));
	if(CollectionType == _DAN_) 
	{
		m_TabCtrl.SetCurFocus(0);
		m_TabCtrl.SetCurSel (0);
		ChangedTab();
	}
	else
	{
		m_TabCtrl.SetCurFocus(1) ;
		m_TabCtrl.SetCurSel (1);
		ChangedTab();
	}

	m_CheckFirst = TRUE ;
	delete pDlg;
	return ;	    
}

VOID CUC_MARC_IMPORT::OnOK() 
{
	// TODO: Add extra validation here
	INT ids ;
    CESL_DataMgr * pDataMgr  ;
	CESL_DataMgr * pDataMgr2 ;	



	INT nSelectCnt =0 ;
    CESL_ControlMgr *pCM ;
	pCM = FindCM(_T("CM_찾기"));
    
	CESL_Grid *pGrid ;
	if(m_TabCtrl.GetCurSel () == 0)
	{
		pGrid = (CESL_Grid*)pCM->FindControl (_T("MainBoGrid")) ;
	}
	else 
    {
		pGrid = (CESL_Grid*)pCM->FindControl (_T("MainChulpanGrid")) ;	
	}
    pGrid->SelectMakeList () ;

	nSelectCnt = pGrid->SelectGetCount () ;

	if(nSelectCnt != 1 ) 
	{
		AfxMessageBox(_T("한 항목만 선택하여 주십시요.")) ;
		return ;
	}
	
	m_idx = pGrid->SelectGetIdx () ;
    
	pDataMgr = FindDM(m_CurrentDM ) ;
    
	CString temp = _T("") ;
    ids = pDataMgr->GetCellData(_T("서지마스터키"),m_idx ,temp ) ;
    if(ids <0) {
		AfxMessageBox(_T("항목을 체크 해 주세요 ~!"));
	    return ;
	}
    
	pDataMgr2 = FindDM(m_Master_DM) ;
    CString query ;
 
	CRS_SQL m_rs_sql(this);

	CString sRec_key = temp;


	if( m_TabCtrl.GetCurSel () == 0 ){
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

		CKolisNetService::GetBibInfo(this, sRec_key, pDataMgr2, &Mapper);
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
		
		CKolisNetService::GetBtpSpeciesInfo(this, sRec_key, pDataMgr2, &Mapper);
	}

	pDataMgr2->GetCellData(_T("MARC"),0,temp) ;

	if( temp == _T("") )
	{
		AfxMessageBox(_T("마크가 없는 자료입니다."));
		return;
	}
	CString lib_code ;
	CString system_date;
	m_rs_sql.GetLibraryInfo (lib_code) ;
	m_rs_sql.GetSytemDate (system_date) ;
    
    CString sCount ;
	CString sUpDownFlag  = _T("") ;
	CString sWorkFlag;
	CString sClientType = _T("K3") ;
	CString sScreenType  = _T("") ;
	CString	sDataKey;
	CString sStatisticsTblName;
	INT		nUpdateFieldCount;
	
	sCount = _T("1") ;    

	
	CButton *pTocChek_C ;
	pTocChek_C = (CButton*)GetDlgItem(IDC_CHECK_Moc) ;

	CButton *pTocChek_A ;
	pTocChek_A = (CButton*)GetDlgItem(IDC_CHECK_Cho) ;
    
	m_TocCheck_CON = FALSE ;
    m_TocChek_ABS = FALSE ;

	if(pTocChek_C ->GetCheck () == 1)
		m_TocCheck_CON = TRUE ;
	
	if(pTocChek_A->GetCheck () == 1)
		m_TocChek_ABS = TRUE ;

	if (m_TabCtrl.GetCurSel() == 1)
	{
		CKolisNetService::MakeBtpStatitics(this, sRec_key, lib_code, sClientType, sScreenType, _T("D"), _T("M"), CKolisNetService::GetSysdate(FALSE), sCount);
	}
	else
	{
		CKolisNetService::MakeUploadStatitics(this, sRec_key, lib_code, sClientType, sScreenType, _T("D"), _T(""), CKolisNetService::GetSysdate(FALSE), sCount);
	}

	CButton * pBasic;
	pBasic=(CButton *)GetDlgItem(IDC_rBasic);
	CButton * pAll;
	pAll=(CButton *)GetDlgItem(IDC_rAll);
	CButton * pSelect;
	pSelect=(CButton *)GetDlgItem(IDC_rSelect);
	
	if ( pBasic->GetCheck() == 1 ) {
		m_check = 0;
		ReMakeMarc (temp, 0 ,sMarc) ;
	}
	else if ( pAll->GetCheck() == 1 ) {
		m_check = 1;
		ReMakeMarc (temp,1,sMarc) ;
	}
	else {
		m_check = 2;
		ReMakeMarc (temp,2,sMarc) ;
	}   

	EndDialog(IDOK);
}

VOID CUC_MARC_IMPORT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	
	CDialog::OnCancel();
	//EndDialog(IDOK);
}

BEGIN_EVENTSINK_MAP(CUC_MARC_IMPORT, CDialog)
    //{{AFX_EVENTSINK_MAP(CUC_MARC_IMPORT)
	ON_EVENT(CUC_MARC_IMPORT, IDC_gridBo, -600 /* Click */, OnClickgridBo, VTS_NONE)
	ON_EVENT(CUC_MARC_IMPORT, IDC_gridSe, -600 /* Click */, OnClickgridSe, VTS_NONE)
	ON_EVENT(CUC_MARC_IMPORT, IDC_gridChulpan, -600 /* Click */, OnClickgridChulpan, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CUC_MARC_IMPORT::OnClickgridBo() 
{
	// TODO: Add your control notification handler code here
	
    CESL_ControlMgr *pCM ;
	pCM = FindCM(_T("CM_찾기"));
    INT idx ; 

 
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainBoGrid")) ;
	idx = pGrid->GetRow();

    CESL_DataMgr *pDM = FindDM(_T("DM_단행자료_DB"));
	if(pDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!!")) ;
		return ;
	}
	
	if(pDM->GetRowCount () == 0) 
	{
		return ;
	}
    
	m_idx = idx;

	CComboBox *pCtrlCombo = (CComboBox *)GetDlgItem (IDC_COMBO_SELECT) ; 
	if(pCtrlCombo->GetCurSel() == 0) 
	{
		Toc_Check(m_idx) ;
	}
}

VOID CUC_MARC_IMPORT::OnClickgridChulpan() 
{
	CESL_ControlMgr *pCM ;
	pCM = FindCM(_T("CM_찾기"));
    INT idx ; 

 
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainChulpanGrid")) ;
	idx = pGrid->GetRow();
	
    CESL_DataMgr *pDM = FindDM(_T("DM_출판유통자료_DB"));
	if(pDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!!")) ;
		return ;
	}
	
	if(pDM->GetRowCount () == 0) 
	{
		return ;
	}
    
	m_idx = idx;

	CComboBox *pCtrlCombo = (CComboBox *)GetDlgItem (IDC_COMBO_SELECT) ; 
	if(pCtrlCombo->GetCurSel() == 0) 
	{
		Toc_Check(m_idx) ;
	}	
}


VOID CUC_MARC_IMPORT::OnbMARC() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr * pDM;
	if(m_TabCtrl.GetCurSel () == 0)
	{
		pDM = FindDM(_T("DM_단행자료_DB"));
	}

	else
	{
		pDM = FindDM(_T("DM_출판유통자료_DB"));
	}

	if( pDM->GetRowCount() < 1)
	{
		AfxMessageBox(_T("검색된 결과가 없습니다"));
		return;
	}

    CMARC_VIEW dlg(this) ;
	INT ids = -1;
	ids = dlg.DoModal();
	if( ids >= 0 )
	{
		CButton * btnMoc = (CButton *)GetDlgItem(IDC_CHECK_Moc);
		CButton * btnAbs = (CButton *)GetDlgItem(IDC_CHECK_Cho);

		btnMoc->SetCheck(dlg.m_bTocYN);
		btnAbs->SetCheck(dlg.m_bAbsYN);

		((CButton *)GetDlgItem(IDC_rBasic))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_rAll))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_rSelect))->SetCheck(FALSE);

		if ( dlg.m_nCheck == 0 ) ((CButton *)GetDlgItem(IDC_rBasic))->SetCheck(TRUE);
		else if ( dlg.m_nCheck == 1 ) ((CButton *)GetDlgItem(IDC_rAll))->SetCheck(TRUE);
		else if ( dlg.m_nCheck == 2 ) ((CButton *)GetDlgItem(IDC_rSelect))->SetCheck(TRUE);

		INT nSelectCnt =0 ;
		CESL_ControlMgr *pCM ;
		pCM = FindCM(_T("CM_찾기"));
		
		CESL_Grid *pGrid ;
		if(m_TabCtrl.GetCurSel () == 0)
		{
			pGrid = (CESL_Grid*)pCM->FindControl (_T("MainBoGrid")) ;
		}
		else
		{
			pGrid = (CESL_Grid*)pCM->FindControl (_T("MainChulpanGrid")) ;	
		}
		pGrid->SelectMakeList () ;

		nSelectCnt = pGrid->SelectGetCount () ;

		if( nSelectCnt < 1 )
		{
			pGrid->SetTextMatrix(ids+1,1,_T("V"));
		}
		if(nSelectCnt > 1 ) 
		{
			INT nPos = pGrid->SelectGetHeadPosition();
				
			while( nPos >= 0 )
			{
				pGrid->SetAt(nPos,0,_T(""));
				nPos = pGrid->SelectGetNext();
			}

			pGrid->SetAt(ids,0,_T("V"));
			pGrid->SetReverse(ids);
		}
		
		if(m_TabCtrl.GetCurSel () == 0)
		{
			OnClickgridBo();	
		}
		else
		{
			OnClickgridChulpan();
		}
		OnOK();		
	}
}



INT CUC_MARC_IMPORT::insert_data( VOID *pAppend_data_search, INT RowCount ,CString DMAlias ,INT ColCount)
{   
	DB_DATA_ARRAY_TYPE_SEARCH *pAppend_data = (DB_DATA_ARRAY_TYPE_SEARCH*)pAppend_data_search;

    CString temp_m_szDataMgrAlias;
    temp_m_szDataMgrAlias.Format (_T("%s"),DMAlias);

	CESL_DataMgr *pDM = FindDM(temp_m_szDataMgrAlias);
	if (pDM == NULL) return -1;

	pDM->FreeData();

	INT ColumnCount = pDM->RefList.GetCount();
	INT row, col;
	TCHAR *cell;
	CString strData;
	
	INT nColIndex = 0;

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	
	for (row = 0; row < RowCount; row++) {
		pDM->InsertRow(-1);
		nColIndex = 0;
		for (col = 0; col < ColCount; col++) {
			cell = NULL;
			cell = pAppend_data[col].db_data[row].stringData;
			if (cell) {
				
				while(TRUE)
				{
					pDM->GetColumnRefInfo( nColIndex , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
					
					sDataType.MakeLower();
					if( sDataType.Compare(_T("udf"))!=0 )
						 break;
					nColIndex++;
				}
				strData.Format(_T("%s"), cell);
				pDM->SetCellData(row, nColIndex , strData);
			}
			nColIndex++;
		}
	}
	return 0;
}

VOID CUC_MARC_IMPORT::OnSearchFinished(WPARAM wParam, LPARAM lParam)
{

	INT nSel;
	switch ( (INT)wParam )
	{
		case _DAN_ :
			nSel = 0;
			break;
		case _YON_ :
			nSel = 1;
			break;
		default:
			nSel = 2;
			break;

	}	 
	m_TabCtrl.SetCurSel(nSel);		
	ChangedTab();
     
	CMSHFlexGrid *pGrid1 = (CMSHFlexGrid*)GetDlgItem(IDC_gridBo);
    CMSHFlexGrid *pGrid2 = (CMSHFlexGrid*)GetDlgItem(IDC_gridSe);
	CMSHFlexGrid *pGrid3 = (CMSHFlexGrid*)GetDlgItem(IDC_gridChulpan);

	pGrid1->ShowWindow(SW_SHOW);
    pGrid2->ShowWindow(SW_HIDE);
	pGrid3->ShowWindow(SW_HIDE);
		
	
	INT nResultCnt1 = 0;
	INT nResultCnt2 = 0;
	INT nResultCnt3 = 0;
	
	if(nSel == 0) 
	{
      	pGrid1->ShowWindow(SW_SHOW);
        pGrid2->ShowWindow(SW_HIDE);
		pGrid3->ShowWindow(SW_HIDE);
 
		CESL_DataMgr *pDM = FindDM(_T("DM_단행자료_DB"));
		nResultCnt1 = pDM->GetRowCount() ;
		CString msg ;
		msg.Format (_T("%d"),nResultCnt1);
					
		GetDlgItem(IDC_eBO_COUNT)->SetWindowText(msg);
	}
	else
	{
		pGrid1->ShowWindow(SW_HIDE);
        pGrid2->ShowWindow(SW_HIDE);
		pGrid3->ShowWindow(SW_SHOW);
  	    CESL_DataMgr *pDM = FindDM(_T("DM_출판유통자료_DB"));

		nResultCnt3 = pDM->GetRowCount() ;
		CString msg ;
		msg.Format (_T("%d"),nResultCnt3);
		GetDlgItem(IDC_eChulpan_COUNT)->SetWindowText(msg);
	}

	AllControlDisplay (_T("CM_찾기"),-1) ;

	if( nResultCnt1+nResultCnt2+nResultCnt3 == 1 )
	{
		if( nSel == 0 && nResultCnt1 == 1)
		{
			pGrid1->SetTextMatrix(1,1,_T("V"));
			OnbMARC();
		}
		else if( nSel == 1 && nResultCnt2 == 1)
		{
			pGrid2->SetTextMatrix(1,1,_T("V"));	
			OnbMARC();
		}
		else 
		{
			pGrid3->SetTextMatrix(1,1,_T("V"));	
			OnbMARC();
		}
	}	
}

VOID CUC_MARC_IMPORT::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	ChangedTab();   	
	*pResult = 0;
}

INT CUC_MARC_IMPORT::ReMakeMarc(CString tmpMarc ,INT checkSelect ,CString &resultMarc)
{
    m_pMarcMgr = m_pInfo->pMarcMgr ;
    
    INT TabSel ;
	TabSel = m_TabCtrl.GetCurSel () ;
	if(TabSel == 0 ) 
	{
		m_pMarcMgr->SetMarcGroup (_T("통합마크")) ;
	}
	else if(TabSel == 1) 
	{
		m_pMarcMgr->SetMarcGroup (_T("통합마크")) ;
	}
	else if(TabSel == 2) 
	{
		m_pMarcMgr->SetMarcGroup (_T("통합마크")) ;
	}
    

    m_pMarcMgr ->Decoding (tmpMarc,&m_marc ) ;

    INT nTagNo;
	POSITION pos = m_marc.m_listTag.GetHeadPosition() ;
    INT ids ;
	CString f_pUBNum ,strLeader;
    CString sResultMarc ;
 
	CArray<CString, CString&> array;
    
	if( checkSelect == 0) 
	{
		while( pos != NULL ) 
		{   
			CString tmp_Value;
			CString strField ;
			
			CTag *pTag = (CTag*)m_marc.m_listTag.GetNext(pos) ;			
			if (pTag == NULL) continue;

			nTagNo = _ttoi(pTag->GetTagCode());
            if(TabSel == 0) 
            {
				switch(nTagNo)
				{
				case 8:
							m_pMarcMgr->GetItem (&m_marc,_T("008*"),strField) ;
							m_pMarcMgr->SetItem (&m_ResultMarc,_T("008*"),strField);
							break;
                case 20 :
						    ids = m_pMarcMgr ->GetField  (&m_marc,pTag->GetTagCode (),strField,&array) ;
							if( ids < 0 ){ return -1 ; }
                            
							if(array.GetSize () == 1)
							{
							   ids = m_pMarcMgr->InsertField(&m_ResultMarc ,  pTag->GetTagCode() + strField);
							   if( ids < 0 ){ return -2; }
							}
							else
							{
							  
							}
                             

							break ;

				case 23 :
				case 245 :
				case 260 :
				case 300 :
				case 440 :
				case 490 :
				case 950 :
							ids = m_pMarcMgr ->GetField (&m_marc,pTag->GetTagCode(), strField) ;
							if( ids < 0 ){ return -1 ; }

							ids = m_pMarcMgr->InsertField(&m_ResultMarc ,  pTag->GetTagCode() + strField);
							if( ids < 0 ){ return -2; }
							
					        break ;
			
						   
										
				}
			}
            else 
			{
                switch(nTagNo)
				{
				case 022 :
				case 030 :
				case 245 :
				case 300 :
				case 310 :
				case 440 :
				case 490 :
							ids = m_pMarcMgr ->GetField (&m_marc,pTag->GetTagCode(), strField) ;
							if( ids < 0 ){ return -1 ; }

							ids = m_pMarcMgr->InsertField(&m_ResultMarc ,  pTag->GetTagCode() + strField);
							if( ids < 0 ){ return -2; }
							
							break ;				
				}	

			}

		}
 
		m_pMarcMgr->GetItem (&m_marc,_T("001*"),f_pUBNum) ;
		m_pMarcMgr->SetItem (&m_ResultMarc,_T("035$a"),f_pUBNum);

		m_marc.GetLeader (strLeader) ;
		m_ResultMarc.SetLeader (strLeader.GetBuffer (0)) ;
		
		m_pMarcMgr->Encoding(&m_ResultMarc,sResultMarc);

    }
    else 
	{
		m_pMarcMgr->GetItem (&m_marc,_T("001*"),f_pUBNum) ;

       	while( pos != NULL ) 
		{   
			CString tmp_Value;
			CString strField ;
			
			CTag *pTag = (CTag*)m_marc.m_listTag.GetNext(pos) ;			
			if (pTag == NULL) continue;	
 			nTagNo = _ttoi(pTag->GetTagCode());
			switch(nTagNo)
			{
			case 001 : 
			case  12 :
			case  35 :
			case  49 :
			case  52 :
			case  90 :
					
				ids =	m_pMarcMgr->DeleteField (&m_marc,pTag->GetTagCode ()) ;
				m_marc.m_listTag.GetPrev(pos) ;
				break ;       
			}   

		}
        m_pMarcMgr->SetItem (&m_marc,_T("035$a"),f_pUBNum);
        
		if(checkSelect == 2)
		{
			ids = m_pMarcMgr->DeleteField (&m_marc,_T("505")) ;
		}
      
		m_pMarcMgr->Encoding(&m_marc,sResultMarc);
        
	}

	resultMarc = sResultMarc ;

	return  0;
}

VOID CUC_MARC_IMPORT::OnbMokcha() 
{
	// TODO: Add your control notification handler code here
		m_TocEditdlg.SetTocFromString(m_Contents );
		m_TocEditdlg.ShowViewer(FALSE,FALSE);
}

VOID CUC_MARC_IMPORT::OnbChorok() 
{
	// TODO: Add your control notification handler code here
		m_ChorokDlg.SetChorok(m_Abstracts );
		m_ChorokDlg.ShowViewer(FALSE,FALSE);
}


INT CUC_MARC_IMPORT::Toc_Check(INT nRowIndex)
{
    CESL_DataMgr * pDataMgr  ;
    CESL_DataMgr * pDataMgr2 ;


	pDataMgr = FindDM(m_CurrentDM ) ;
    
	INT ids ;

	CString szRec_Key = _T("") ;
    ids = pDataMgr->GetCellData(_T("서지마스터키"),m_idx-1 ,szRec_Key ) ;
    if(ids <0) {
		AfxMessageBox(_T("항목을 체크 해 주세요 ~!"));
	    return -1;
	}

	pDataMgr2 = FindDM(m_Master_DM) ;
    CString query ;

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

	CKolisNetService::GetBibInfo(this, szRec_Key, pDataMgr2, &Mapper);
    
	CString CONTENTS_YN;
	CString ABSTRACTS_YN;
     
	CString REC_KEY ,CONTROL_NO;

	pDataMgr2 ->GetCellData (_T("제어번호"),0,CONTROL_NO) ;


    if(m_TabCtrl.GetCurSel () == 0 )
	{

		pDataMgr2->GetCellData (_T("목차유무"),0,CONTENTS_YN);

		pDataMgr2->GetCellData (_T("초록유무"),0,ABSTRACTS_YN);


		if(CONTENTS_YN == _T("Y"))
		{
			(CButton*)GetDlgItem(IDC_bMokcha)->EnableWindow(TRUE);
			(CButton *)GetDlgItem(IDC_CHECK_Moc)->EnableWindow(TRUE);

			GetTocData(CONTROL_NO,0);
		}
		else
		{
			(CButton*)GetDlgItem(IDC_bMokcha)->EnableWindow(FALSE);
			(CButton *)GetDlgItem(IDC_CHECK_Moc)->EnableWindow(FALSE);
		}

		if(ABSTRACTS_YN == _T("Y"))
		{
			(CButton*)GetDlgItem(IDC_bChorok)->EnableWindow(TRUE);
			(CButton *)GetDlgItem(IDC_CHECK_Cho)->EnableWindow(TRUE);
			GetTocData(CONTROL_NO,1);
		}
		else
		{
			(CButton*)GetDlgItem(IDC_bChorok)->EnableWindow(FALSE);
			(CButton *)GetDlgItem(IDC_CHECK_Cho)->EnableWindow(FALSE);
		}
	}

	else if(m_TabCtrl.GetCurSel () == 1 )
	{
		pDataMgr2->GetCellData (_T("목차유무"),0,CONTENTS_YN);
		if(CONTENTS_YN == _T("Y"))
		{
			(CButton*)GetDlgItem(IDC_bMokcha)->EnableWindow(TRUE);
		}			
		else
		{
			(CButton*)GetDlgItem(IDC_bMokcha)->EnableWindow(FALSE);
		}
	}


	return 0 ;
}

INT CUC_MARC_IMPORT::GetTocData(CString tmpRecKey ,INT nType)
{
	
	INT m_Type ;

	m_Type = nType ;
	
	CRS_SQL m_CS_SQL(this) ;
	CESL_DataMgr *pDM = FindDM(_T("DM_목차초록_KOLISNET"));
	if(pDM == NULL){
		AfxMessageBox (_T("FindDM() Error~!"));
	}


	CKolisNetDataMapper Mapper;
	Mapper.AddData(KOLISNET_PARAM_UB_CONTROL_NO, _T("제어번호"));
	Mapper.AddData(KOLISNET_PARAM_BTC_KEY, _T("레코드키"));
	Mapper.AddData(KOLISNET_PARAM_SPECIES_CLASS, _T("종구분"));
	Mapper.AddData(KOLISNET_PARAM_SPECIES_KEY, _T("종키"));
	Mapper.AddData(KOLISNET_PARAM_ABSTRACTS, _T("초록"));
	Mapper.AddData(KOLISNET_PARAM_ABS_CREATE_DATE, _T("초록생성일"));
	Mapper.AddData(KOLISNET_PARAM_ABS_UPDATE_DATE, _T("초록수정일"));
	Mapper.AddData(KOLISNET_PARAM_CONTENTS, _T("목차"));
	Mapper.AddData(KOLISNET_PARAM_CON_CREATE_DATE, _T("목차생성일"));
	Mapper.AddData(KOLISNET_PARAM_CON_UPDATE_DATE, _T("목차수정일"));
	Mapper.AddData(KOLISNET_PARAM_ABSTRACTS_KEY, _T("초록키"));
	Mapper.AddData(KOLISNET_PARAM_CONTENTS_KEY, _T("목차키"));

	CKolisNetService::GetTConAbsInfo(this, tmpRecKey, pDM, &Mapper);

	INT RowCnt ;
	RowCnt = pDM->GetRowCount () ;

	if(nType == 0 ) 
	{
		pDM->GetCellData (_T("목차"),0,m_Contents );
	}
    else
	{
		pDM->GetCellData (_T("초록"),0,m_Abstracts) ;
	}


	return 0 ;
}


VOID CUC_MARC_IMPORT::OnClickgridSe() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM ;
	pCM = FindCM(_T("CM_찾기"));
    INT idx ; 
     

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainSeGrid")) ;
	idx = pGrid->GetRow();
	pGrid->SetReverse( idx-1 );

	CESL_DataMgr *pDM = FindDM(_T("DM_연속간행자료_DB"));
	if(pDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!!")) ;
		return ;
	}
	
	if(pDM->GetRowCount () == 0) 
	{
		return ;
	}
   
	m_idx = idx;

	CComboBox *pCtrlCombo = (CComboBox *)GetDlgItem (IDC_COMBO_SELECT) ; 
	if(pCtrlCombo->GetCurSel() == 0) 
	{
		Toc_Check(m_idx) ;
	}
}

VOID CUC_MARC_IMPORT::OnTimer(UINT nIDEvent) 
{
	
	if (nIDEvent == 1) {
		KillTimer(1);
		OnbSEARCH();
	}

	CDialog::OnTimer(nIDEvent);
}

INT CUC_MARC_IMPORT::SetSearchCondition(CString strTitle /*=NULL*/,
										CString strAuthor /*=NULL*/,
										CString strPublisher /*=NULL*/,
										CString strPubYear /*=NULL*/)
{

	m_strTitle = strTitle;
	m_strAuthor = strAuthor;
	m_strPublisher = strPublisher;
	m_strPubYear = strPubYear;
	return 0;
}


VOID CUC_MARC_IMPORT::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{		
	*pResult = 0;
}

VOID CUC_MARC_IMPORT::ChangedTab()
{
	CMSHFlexGrid *pGrid1 = (CMSHFlexGrid*)GetDlgItem(IDC_gridBo);
    CMSHFlexGrid *pGrid2 = (CMSHFlexGrid*)GetDlgItem(IDC_gridSe);
	CMSHFlexGrid *pGrid3 = (CMSHFlexGrid*)GetDlgItem(IDC_gridChulpan);
	INT count ;

    count = m_TabCtrl.GetCurSel ();
   
    CESL_ControlMgr *pCM = FindCM (_T("CM_찾기"));


	switch(count) 
	{
      case 0 :
                m_CurrentDM =_T("DM_단행자료_DB") ;
				m_Master_DM =_T("DM_서지마스터_단행");
	        	pGrid1->ShowWindow(SW_SHOW);	
				pGrid2->ShowWindow(SW_HIDE);
				pGrid3->ShowWindow(SW_HIDE);
                pCM->ControlDisplay(_T("MainBoGrid"));
				( (CButton*) GetDlgItem(IDC_CHECK_Moc) )->ShowWindow(TRUE);
				( (CButton*) GetDlgItem(IDC_CHECK_Cho) )->ShowWindow(TRUE);
				( (CButton*) GetDlgItem(IDC_bMokcha) )->ShowWindow(TRUE);
				( (CButton*) GetDlgItem(IDC_bChorok) )->ShowWindow(TRUE);
				( (CStatic*) GetDlgItem(IDC_STATIC_USE) )->ShowWindow(TRUE);
				( (CComboBox*) GetDlgItem(IDC_COMBO_SELECT) )->ShowWindow(TRUE);				
				break ;
	case 1 : 
				m_CurrentDM =_T("DM_출판유통자료_DB") ;
				m_Master_DM =_T("DM_서지마스터_출판유통진흥원");
				pGrid3->ShowWindow(SW_SHOW);
	        	pGrid1->ShowWindow(SW_HIDE);     
				pGrid2->ShowWindow(SW_HIDE);  
                pCM->ControlDisplay(_T("MainChulpanGrid"));
				( (CButton*) GetDlgItem(IDC_CHECK_Moc) )->ShowWindow(FALSE);
				( (CButton*) GetDlgItem(IDC_CHECK_Cho) )->ShowWindow(FALSE);
				( (CButton*) GetDlgItem(IDC_bMokcha) )->ShowWindow(FALSE);
				( (CButton*) GetDlgItem(IDC_bChorok) )->ShowWindow(FALSE);
				( (CStatic*) GetDlgItem(IDC_STATIC_USE) )->ShowWindow(FALSE);
				( (CComboBox*) GetDlgItem(IDC_COMBO_SELECT) )->ShowWindow(FALSE);				
				break ;
	}
}

HBRUSH CUC_MARC_IMPORT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
