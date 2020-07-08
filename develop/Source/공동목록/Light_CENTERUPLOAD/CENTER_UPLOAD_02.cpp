// CENTER_UPLOAD_02.cpp : implementation file
//

#include "stdafx.h"

#include "CENTER_UPLOAD_02.h"
#include "..\..\공동목록\check_dup_api\rs_sql.h"
#include "..\..\include\공동목록\CU_ComAPI.h"
//==========================================================================
#include "..\CenterUpload_api\CenterUpload.h"
//==========================================================================


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_02 dialog
CString BASE_TITLE = _T("일괄 UPLOAD");

CCENTER_UPLOAD_02::CCENTER_UPLOAD_02(CESL_Mgr* pParent ,BOOL bMarcSyntaxCheck /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCENTER_UPLOAD_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
     
	 m_bMarcSyntaxCheck = bMarcSyntaxCheck;

	 // KOL.UDF.022 시큐어코딩 보완
	 m_pBaseDataMgr = NULL;
	 m_pDupDataMgr = NULL;
	 pMainGrid = NULL;
	 m_iRowIndex = 0;
	 m_iDupRowIndex = 0; 
	 m_iUploadLoc = 0;  
	 
	 iSelectedCnt = 0;
	 m_idx  = 0;
	 m_iSpeciesCnt= 0;
	 m_DupDBAlias = _T("");   
	 m_pDBAlias = _T("");     
}

CCENTER_UPLOAD_02::~CCENTER_UPLOAD_02()
{
}


VOID CCENTER_UPLOAD_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCENTER_UPLOAD_02)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCENTER_UPLOAD_02, CDialog)
	//{{AFX_MSG_MAP(CCENTER_UPLOAD_02)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_02 message handlers
BOOL CCENTER_UPLOAD_02::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}



BOOL CCENTER_UPLOAD_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

//	INT ids ;
	//ids = SetErrorShowFlag( FALSE );    

	if (InitESL_Mgr(_T("SM_Light_CENTERUPLOAD_02")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
    
	m_DupDBAlias = _T("DM_CENTERUPLOAD_복본자료");   
	m_pDBAlias = _T("DM_CENTERUPLOAD_01") ;

	m_pBaseDataMgr = FindDM(m_pDBAlias);
	if(m_pBaseDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ;

	m_pDupDataMgr  = FindDM(m_DupDBAlias);
	if(m_pDupDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ;

    
    
	if ( m_pBaseDataMgr->GetRowCount() > 0 ) {
	SetWindowText( BASE_TITLE + _T(" - 그리드에 선택된 자료를 일괄업로드 합니다.") );
	} else {
	SetWindowText( BASE_TITLE + _T(" - 센터에 올리지 않은 모든 자료를 찾아 일괄업로드 합니다.") );
	}
    

    CESL_ControlMgr * pCM = FindCM(_T("CM_CENTERUPLOAD_01"));
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
	else
	{
		if ( IDYES == AfxMessageBox(_T("선정된 자료를 자동업로드 하시겠습니까?"), MB_YESNO) )
		{
			MakeUploadFile();
		}
		OnCancel();
		return TRUE;		
	}


    ShowStatus( iSelectedCnt, 0, 0, 0, 0 );
    m_idx = pMainGrid->SelectGetHeadPosition () ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



VOID CCENTER_UPLOAD_02::ShowStatus( INT iSelectedCnt, INT iFinishedCnt, INT iDupMatCnt, INT iSyntaxErrorCnt, INT iEtcErrorCnt )
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


VOID CCENTER_UPLOAD_02::OnOK() 
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

    if ( m_pBaseDataMgr->GetRowCount() == 0 ) {
        SetWindowText( BASE_TITLE + _T(" - 자료 검색중...") );

        
		CString strQuery ;
		strQuery = _T("s.CENTER_UPLOAD_STATUS <> '0'") ;
        
        ids = m_pBaseDataMgr->RefreshDataManager (strQuery) ;
        if (ids<0) AfxMessageBox (_T("OptionChange() Error!!!"), FALSE);

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
 
    SetWindowText( BASE_TITLE + _T(" - 작업중...") );
    ShowStatus( iSelectedCnt, iFinishedCnt, iDupMatCnt, iSyntaxErrorCnt, iEtcErrorCnt );
    iTotalCnt = iSelectedCnt ;

	CRS_SQL m_RS_SQL(this) ;
    ids  = m_RS_SQL.GetLibraryInfo (lib_code) ;
    
	CCenterUpload Check_Upload_api(this) ;
	CString str_DIRECT_UPLOAD_YN ;
	Check_Upload_api.Check_Direct_Upload (str_DIRECT_UPLOAD_YN) ;

	CString strSpeciesKey ;
	CString strMarc ;

	// 2012.11.26 ADD BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (제어번호추가)
	CString strControlNo;

	CString strRegNos , strUseLimitCodes , strSpeciesLimitCode;

    pMainGrid->SelectGetTailPosition () ;
    for ( i=0; i < iTotalCnt; i++ ) 
	{		
        iCurIndex = pMainGrid->SelectGetIdx () ;
	    //2004-02-04 CENTER_UPLOAD_YN 체크 
		if(str_DIRECT_UPLOAD_YN == _T("N"))
		{   			
			m_pBaseDataMgr->GetCellData (_T("REC_KEY"),iCurIndex,strSpeciesKey) ;
			m_pBaseDataMgr->GetCellData (_T("MARC"),iCurIndex,strMarc) ;		
			
			// 2012.11.26 ADD BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (제어번호추가)
			m_pBaseDataMgr->GetCellData (_T("제어번호"), iCurIndex, strControlNo);

			m_pBaseDataMgr->GetCellData (_T("이용제한구분"), i, strSpeciesLimitCode);
			MakeStringData( strControlNo, strRegNos , strUseLimitCodes );
			ids = Check_Upload_api.Input_Temp_Mater_tbl (strSpeciesKey, strMarc, _T(""), strControlNo ,strRegNos , strUseLimitCodes , strSpeciesLimitCode);
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
			ids = UpdateUploadStatus( this, m_pDBAlias, iCurIndex, _T("5") );

			m_iSpeciesCnt++;
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
	}

   
    SetWindowText( BASE_TITLE + _T(" - 작업완료") );
    if ( iDupMatCnt + iSyntaxErrorCnt + iEtcErrorCnt > 0 ) {
        CString temp;

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
    
   
    ids = ControlDisplay (_T("CM_CENTERUPLOAD_01"),_T("MainGrid"),-1) ;

	CDialog::OnOK();
}

VOID CCENTER_UPLOAD_02::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CCENTER_UPLOAD_02::MakeUploadFile()
{
	CStdioFile file;
	INT			nRowCount;
	CString		strLine;
	CString		strValue;
	INT			iCurIndex;

	CTime t = CTime::GetCurrentTime();
	CString filename, msg;
	filename.Format(_T("Upload_tmp\\upload_%04d%02d%02d%02d%02d%02d.txt"), t.GetYear(), t.GetMonth(), t.GetDay(),t.GetHour(), t.GetMinute(), t.GetSecond());	
	
	if ( file.Open(filename, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )
	{
		nRowCount = pMainGrid->SelectGetCount () ;
		pMainGrid->SelectGetTailPosition () ;
		for ( int i=0 ; i<nRowCount ; i++ )
		{
			iCurIndex = pMainGrid->SelectGetIdx () ;
			m_pBaseDataMgr->GetCellData(_T("제어번호"), iCurIndex, strValue );
			m_pBaseDataMgr->DeleteRow(iCurIndex);
			strLine.Format(_T("%s\r\n"), strValue);
			file.WriteString(strLine);
			pMainGrid->SelectGetPrev();
		}
		file.Close();
		
		AllControlDisplay (_T("CM_CENTERUPLOAD_01"),-1) ;
		
		CString strCmd;		

		strCmd.Format(_T("CenterUploadManager.exe %s %s %s %s"), m_pInfo->USER_ID, m_pInfo->MANAGE_CODE, filename, m_pInfo->LOCAL_IP);
	

		int len; 
		len = WideCharToMultiByte(CP_ACP, 0, strCmd, -1, NULL, 0, NULL,NULL);
		char* ctmp = new char[len]; 
		// 실제 변환
		WideCharToMultiByte(CP_ACP, 0, strCmd, -1, ctmp, len, NULL, NULL);
		//strCmd.Format();
		::WinExec(ctmp, SW_SHOW);
		delete []ctmp;
	}
		
	return FALSE;
}

HBRUSH CCENTER_UPLOAD_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}

VOID CCENTER_UPLOAD_02::MakeStringData( CString ControlNo , CString &Reg_nos , CString &UseLimitCodes )
{
	if(ControlNo.IsEmpty()) return;
	
	Reg_nos = _T("");
	UseLimitCodes = _T("");
	
	CESL_DataMgr pDM;
	CString strQuery;
	int ids;
	
	strQuery.Format(
		_T("SELECT BB.REG_NO , BB.USE_LIMIT_CODE FROM BO_BOOK_TBL BB ")
		_T("WHERE BB.SPECIES_KEY = ( SELECT MAX(BS.REC_KEY) FROM BO_SPECIES_TBL BS WHERE BS.CONTROL_NO = '%s' AND BS.MANAGE_CODE = UDF_MANAGE_CODE  ) ")
		,ControlNo
		);
	
	pDM.SetCONNECTION_INFO(m_pBaseDataMgr->CONNECTION_INFO);
	
	ids = pDM.RestructDataManager(strQuery);
	
	if (ids < 0 ) return;
	if ( pDM.GetRowCount() <= 0 ) return;
	
	CString Reg_No = _T(""), UseLimitCode = _T("");
	
	for ( int i=0 ; i < pDM.GetRowCount()  ; i++ )
	{
		Reg_No = _T("");
		UseLimitCode = _T("");
		
		pDM.GetCellData(  i , 0 , Reg_No );		
		
		if ( Reg_nos.IsEmpty() ) { Reg_nos += Reg_No; }
		else 
		{
			if (!Reg_No.IsEmpty()) Reg_nos += _T(",") + Reg_No;
		}
		
		pDM.GetCellData( i , 1 , UseLimitCode );
		
		if ( UseLimitCodes.IsEmpty() ){ UseLimitCodes += UseLimitCode; }
		else
		{
			if (!UseLimitCode.IsEmpty())  UseLimitCodes += _T(",") + UseLimitCode;
		}
		
	}
	
}
