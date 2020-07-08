// SE_CENTER_UPLOAD_02.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CENTER_UPLOAD_02.h"
#include "..\..\include\공동목록\CU_ComApi.h"
#include "..\..\공동목록\check_dup_api\rs_sql.h"

#include "..\CenterUpload_api\CenterUpload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString BASE_TITLE = _T("일괄 UPLOAD");

/////////////////////////////////////////////////////////////////////////////
// CSE_CENTER_UPLOAD_02 dialog


CSE_CENTER_UPLOAD_02::CSE_CENTER_UPLOAD_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CENTERUPLOAD_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_SE_CENTERUPLOAD = new CSE_CENTERUPLOAD_API(this) ;
	m_bMarcSyntaxCheck = FALSE ;

	// KOL.UDF.022 시큐어코딩 보완
	m_pBaseDataMgr = NULL;
	m_pDupDataMgr = NULL;
	pMainGrid = NULL;
	m_iRowIndex = 0;
	m_iDupRowIndex = 0; 
    m_iUploadLoc = 0; 
	iSelectedCnt = 0;
    m_idx = 0;
    m_iSpeciesCnt = 0;
    m_DupDBAlias = _T(""); 
	m_pDBAlias = _T("") ;
}

CSE_CENTER_UPLOAD_02::~CSE_CENTER_UPLOAD_02()
{
	delete m_SE_CENTERUPLOAD ;
}

BOOL CSE_CENTER_UPLOAD_02::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSE_CENTER_UPLOAD_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CENTER_UPLOAD_02)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CENTER_UPLOAD_02, CDialog)
	//{{AFX_MSG_MAP(CSE_CENTER_UPLOAD_02)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CENTER_UPLOAD_02 message handlers

BOOL CSE_CENTER_UPLOAD_02::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DupDBAlias = _T("DM_SE_CENTERUPLOAD_복본자료");   
	m_pDBAlias = _T("DM_SE_CENTERUPLOAD_01") ;

	m_pBaseDataMgr = FindDM(m_pDBAlias);
	if(m_pBaseDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ;

	m_pDupDataMgr  = FindDM(m_DupDBAlias);
	if(m_pDupDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ;

    
    
	if ( m_pBaseDataMgr->GetRowCount() > 0 ) {
	SetWindowText( BASE_TITLE + _T(" - 그리드에 선택된 자료를 일괄업로드 합니다.") );
	} else {
	SetWindowText( BASE_TITLE + _T(" - 센터에 올리지 않은 모든 자료를 찾아 일괄업로드 합니다.") );
	}
    

    CESL_ControlMgr * pCM = FindCM(_T("CM_SE_CENTERUPLOAD_01"));
	if (!pCM) AfxMessageBox (_T("FindCM Error~!")) ;

	pMainGrid = (CESL_Grid *)pCM->FindControl(_T("MainGrid"));
    pMainGrid->SelectMakeList () ; 
     
	iSelectedCnt = 0 ;
    m_idx = 0 ;

    iSelectedCnt = pMainGrid->SelectGetCount () ;
 
	if(iSelectedCnt == 0)
	{
		AfxMessageBox(_T("선택 목록을 구성할 수 없습니다."));
		OnCancel() ;
	}


    ShowStatus( iSelectedCnt, 0, 0, 0, 0 );

    m_idx = pMainGrid->SelectGetHeadPosition () ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
VOID CSE_CENTER_UPLOAD_02::ShowStatus( INT iSelectedCnt, INT iFinishedCnt, INT iDupMatCnt, INT iSyntaxErrorCnt, INT iEtcErrorCnt )
{
    if ( iFinishedCnt == 0 ) {
        m_ProgressCtrl.SetRange( 0, iSelectedCnt );
        m_ProgressCtrl.SetStep( 1 );
        m_ProgressCtrl.SetPos( 0 );
    } else {
        m_ProgressCtrl.StepIt();
    }

    CString frm;
    frm.Format( _T("%d / %d"), iFinishedCnt, iSelectedCnt );
    GetDlgItem(IDC_STATIC_1)->SetWindowText( frm );
    frm.Format( _T("%d 건"), iDupMatCnt );
    GetDlgItem(IDC_STATIC_2)->SetWindowText( frm );
    frm.Format( _T("%d 건"), iSyntaxErrorCnt );
    GetDlgItem(IDC_STATIC_3)->SetWindowText( frm );
    frm.Format( _T("%d 건"), iEtcErrorCnt );
    GetDlgItem(IDC_STATIC_4)->SetWindowText( frm );
}

VOID CSE_CENTER_UPLOAD_02::OnOK() 
{
    INT ids, i;
    INT iTotalCnt = 0;
    INT iSelectedCnt = 0;
    INT iFinishedCnt = 0;
    INT iDupMatCnt = 0;
    INT iSyntaxErrorCnt = 0;
    INT iEtcErrorCnt = 0;
    INT iCurIndex = 0;
    CString sFlag;
    CString lib_code;
    CString frm;


	m_iSpeciesCnt = 0;
    


	//CESL_ControlMgr * pCM = FindCM(_T("CM_CENTERUPLOAD_01"));
	//if (!pCM) AfxMessageBox (_T("FindCM Error~!")) ;
	//CESL_Grid *pMainGrid = (CESL_Grid *)pCM->FindControl(_T("MainGrid"));

    //
    if ( m_pBaseDataMgr->GetRowCount() == 0 ) {
        SetWindowText( BASE_TITLE + _T(" - 자료 검색중...") );

        
		CString strQuery ;
		strQuery = _T("s.CENTER_UPLOAD_STATUS <> '0'") ;
        
        ids = m_pBaseDataMgr->RefreshDataManager (strQuery) ;
        if (ids<0) AfxMessageBox (_T("OptionChange() Error!!!"), FALSE);

       // 중복되는 자료 삭제
       // ids = DeleteDupSpeciesData( this, m_pDBAlias );
       // if (ids) AfxMessageBox (_T("DeleteDupSpeciesData() Error!!!"), FALSE);

        SetWindowText( BASE_TITLE ) ;

        iSelectedCnt = pMainGrid->SelectGetCount () ;

        if ( iSelectedCnt == 0 ) {
            AfxMessageBox( _T("센터에 올리지 않은 자료가 없습니다.") ) ;
            return;
        }

        frm.Format( _T("검색된 %d건의 자료를 업로드 하겠습니까?"), iSelectedCnt ) ;
        if ( IDYES != AfxMessageBox(frm, MB_YESNO) ) {
            m_pBaseDataMgr->FreeData () ;
            return;
        }
    }
    else {
  		
			iSelectedCnt = pMainGrid->SelectGetCount () ;
    }

    // DB작업을 하는데 필요한 정보를 구해 DISP ALIAS의 KEYWORD에 저장해 놓는다.
  
    CRS_SQL m_RS_SQL(this) ;
    ids  = m_RS_SQL.GetLibraryInfo (lib_code) ;
   
	//==============================================================================
	//2004-02-04 이학중 추가 :: CENTER_UPLOAD_YN CHECK 
	CCenterUpload Check_Upload_api(this) ;
	CString str_DIRECT_UPLOAD_YN ;
	Check_Upload_api.Check_Direct_Upload (str_DIRECT_UPLOAD_YN) ;
    //==============================================================================
   
    SetWindowText( BASE_TITLE + _T(" - 작업중...") );
    ShowStatus( iSelectedCnt, iFinishedCnt, iDupMatCnt, iSyntaxErrorCnt, iEtcErrorCnt );

    iTotalCnt = iSelectedCnt ;
    pMainGrid->SelectGetTailPosition () ;
    for ( i=0; i < iTotalCnt; i++ )
	{
		
        iCurIndex = pMainGrid->SelectGetIdx () ;
		
		if(str_DIRECT_UPLOAD_YN == _T("N"))
		{   
			CString strSpeciesKey ;
			CString strMarc ;

			// 2012.11.26 ADD BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (제어번호추가)
			CString strControlNo;
            
			m_pBaseDataMgr->GetCellData (_T("REC_KEY"),iCurIndex,strSpeciesKey) ;
			m_pBaseDataMgr->GetCellData (_T("MARC"),iCurIndex,strMarc) ;
			
			// 2012.11.26 ADD BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (제어번호추가)
			m_pBaseDataMgr->GetCellData (_T("제어번호"), iCurIndex, strControlNo);
	
			ids = Check_Upload_api.Input_Temp_Mater_tbl (strSpeciesKey, strMarc, _T(""), strControlNo, _T("") , _T("") , _T(""));

			if(ids < 0 )
			{
				ids = UpdateUploadStatus( this, m_pDBAlias, iCurIndex, _T("4") );
				if (ids<0) AfxMessageBox (_T("UpdateUploadStatus(4) Error!!!"), FALSE);
				
				iEtcErrorCnt ++ ;
				iFinishedCnt ++ ;
				pMainGrid->SelectGetPrev() ;
				ShowStatus( iSelectedCnt, iFinishedCnt, iDupMatCnt, iSyntaxErrorCnt, iEtcErrorCnt );
				m_pBaseDataMgr->DeleteRow ( iCurIndex );
				continue;
			}
			ids = UpdateUploadStatus( this, m_pDBAlias, iCurIndex, _T("0") );
			m_iSpeciesCnt++;
			//ids = UpdateStatisticsInfo( this, m_pDBAlias, m_pDBAlias, m_iSpeciesCnt);
			//if (ids < 0) AfxMessageBox (_T("UpdateStatisticsInfo() Error!!!"), FALSE);
			
			iFinishedCnt ++ ;
			pMainGrid->SelectGetPrev() ;
			ShowStatus( iSelectedCnt, iFinishedCnt, iDupMatCnt, iSyntaxErrorCnt, iEtcErrorCnt );
			m_pBaseDataMgr->DeleteRow ( iCurIndex );
			continue ;
			
		}
		else 
		{			
			AfxMessageBox(_T("해당 도서관 부호가 공동목록 도서관 부호에 등록되어 있지 않습니다.")) ;
			return ;	
		}
    }
   
   	if(str_DIRECT_UPLOAD_YN == _T("N"))
	{
		ids = Check_Upload_api.Update_Temp_Lib_Statistics_Tbl(m_iSpeciesCnt,_T("U")) ;
        //if( ids <0 ) return  ;
	}

    //
    SetWindowText( BASE_TITLE + _T(" - 작업완료") );
    if ( iDupMatCnt + iSyntaxErrorCnt + iEtcErrorCnt > 0 ) {
        CString temp;
        //frm.Format( _T("작업이 완료되었습니다. 총 %d건의 자료가 UPLOAD되었습니다.\n\n"), iFinishedCnt );
		frm.Format( _T("총 %d건의 자료 중에서 %d건이 센터로 업로드 되었습니다.\n\n"), iFinishedCnt, m_iSpeciesCnt);
        if ( iDupMatCnt > 0 ) {
            temp.Format( _T("확인을 요하는자료 : %d건\n"), iDupMatCnt );
            frm += temp;
        }
        if ( iSyntaxErrorCnt > 0 ) {
            temp.Format( _T("MARC 오류자료     : %d건\n"), iSyntaxErrorCnt );
            frm += temp;
        }
        if ( iEtcErrorCnt > 0 ) {
            temp.Format( _T("기타 오류자료     : %d건\n"), iEtcErrorCnt );
            frm += temp;
        }
        //frm += _T("은 업로드 되지 않았습니다. 건별로 작업하십시오.");
    }
    else {
        frm.Format( _T("작업이 완료되었습니다.") );
    }
    AfxMessageBox( frm );
    
   
    ids = ControlDisplay (_T("CM_SE_CENTERUPLOAD_01"),_T("MainGrid"),-1) ;

	CDialog::OnOK();
}

VOID CSE_CENTER_UPLOAD_02::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

HBRUSH CSE_CENTER_UPLOAD_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
