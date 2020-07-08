// CENTER_UPLOAD_03.cpp : implementation file
//

#include "stdafx.h"

#include "CENTER_UPLOAD_03.h"
#include "TitleString.h"
#include "..\..\include\공동목록\CU_ComAPI.h"
#include "..\CenterUpload_api\CenterUpload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_BackupMarc;

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_03 dialog



CCENTER_UPLOAD_03::CCENTER_UPLOAD_03(CESL_Mgr* pParent , BOOL bMarcSyntaxCheck  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCENTER_UPLOAD_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    
	m_bMarcSyntaxCheck = bMarcSyntaxCheck;

	AfxInitRichEdit();

	// KOL.UDF.022 시큐어코딩 보완
	m_pBaseDataMgr = NULL;
	m_pDupDataMgr = NULL;
	m_iRowIndex = 0;
	m_iDupRowIndex = 0;
	m_iUploadLoc = 0;
    
	m_ReSpeciesKey = _T("");
	m_ReStreamMarc = _T("");
	m_DupDBAlias = _T("");   
    m_pDBAlias = _T("");
    
	m_strStreamMarc = _T("");
	m_strStreamMarc2 = _T("");
	m_idx  = 0;
    
	m_sResultMsg  = _T("");

}

CCENTER_UPLOAD_03::~CCENTER_UPLOAD_03()
{
}


VOID CCENTER_UPLOAD_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCENTER_UPLOAD_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP  	

}


BEGIN_MESSAGE_MAP(CCENTER_UPLOAD_03, CDialog)
	//{{AFX_MSG_MAP(CCENTER_UPLOAD_03)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bNEW, OnbNEW)
	ON_BN_CLICKED(IDC_bHOLDING, OnbHOLDING)
	ON_BN_CLICKED(IDC_bPRE_SEL, OnbPRESEL)
	ON_BN_CLICKED(IDC_bNEXT_SEL, OnbNEXTSEL)
	ON_BN_CLICKED(IDC_bREQUEST, OnbREQUEST)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_03 message handlers


BOOL CCENTER_UPLOAD_03::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CCENTER_UPLOAD_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
     m_ReSpeciesKey = _T("");
     m_ReStreamMarc = _T("");
    
	if (InitESL_Mgr(_T("SM_Light_CENTERUPLOAD_03")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
    
	if(m_ReSpeciesKey.IsEmpty() && m_ReStreamMarc.IsEmpty() )
	{        
          m_DupDBAlias = _T("DM_CENTERUPLOAD_복본자료");   
          m_pDBAlias = _T("DM_CENTERUPLOAD_01") ;

		m_pBaseDataMgr = FindDM(m_pDBAlias);
		if(m_pBaseDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ;

		m_pDupDataMgr  = FindDM(m_DupDBAlias);
		if(m_pDupDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ;
	}
    
	else
	{ 
        m_pBaseDataMgr = FindDM(m_pDBAlias);
		if(m_pBaseDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ;

		m_pDupDataMgr  = FindDM(m_DupDBAlias);
		if(m_pDupDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ; 
	}
	
    MoveDataAndDisplay(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCENTER_UPLOAD_03::MoveDataAndDisplay(INT nDir)
{
	return 0 ;
}


INT CCENTER_UPLOAD_03::SetMarcEdit_01 () 
{
    INT ids ;

    m_marcEditor_0.SubclassDlgItem(IDC_RICHEDIT_CENTERUP, -1, this);
    ids = 	m_pBaseDataMgr->SetMarcMgr(m_pInfo->pMarcMgr);
	
	CString tmp_Stream ;
	ids = m_pBaseDataMgr->GetCellData(_T("MARC"),m_idx,tmp_Stream );
	if(ids <0) 
	{
		AfxMessageBox(_T("MARC 를 가져오지 못했습니다.!!"));
	}

	m_strStreamMarc=tmp_Stream;
	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	

	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc1))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}



	//insert marc
	m_marcEditor_0.InitUserAlias();
	m_marcEditor_0.Init(m_pInfo->pMarcMgr, &m_marc1);
	m_marcEditor_0.Display();

    CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_CENTERUPLOAD_03_01"));
	pCM->AllControlDisplay(m_idx);	

	return 0 ;
}


INT  CCENTER_UPLOAD_03::MoveDupDataAndDisplay(INT nDir) 
{   

	CString    tmpData;
    INT     ids;
    CESL_ControlMgr *pCM2 ;
	pCM2 = FindCM(_T("CM_CENTERUPLOAD_03_02"));
	
	CRichEditCtrl *pRich = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_DUP) ;
	pRich->SetWindowText (_T("")) ;
	
	// 복본자료가 없을때의 처리
	if ( m_pDupDataMgr->GetRowCount() == 0 ) {
		GetDlgItem(IDC_bPRE_SEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_bNEXT_SEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_bHOLDING)->EnableWindow(FALSE);
		GetDlgItem(IDC_bREQUEST)->EnableWindow(FALSE);
        GetDlgItem(IDOK)->EnableWindow(FALSE);


		pCM2->SetControlMgrData (_T("째"),_T("0"),-1);
	    pCM2->SetControlMgrData (_T("자료유형"),_T(""),-1);
		pCM2->SetControlMgrData (_T("형식구분"),_T(""),-1);
		pCM2->SetControlMgrData (_T("매체구분"),_T(""),-1);

        g_BackupMarc.Empty();

	}
    else{

	 GetDlgItem(IDOK)->EnableWindow(TRUE);
	 GetDlgItem(IDC_bPRE_SEL)->EnableWindow(TRUE);
	 GetDlgItem(IDC_bNEXT_SEL)->EnableWindow(TRUE);
    //
	}
	switch( nDir ) {
        case 0:
            break;
        case -1:
            m_iDupRowIndex--;
            break;
        case 1:
            m_iDupRowIndex++;
            break;
        default:
            break;
    }

    ids = AllControlDisplay( _T("CM_CENTERUPLOAD_03_02"), m_iDupRowIndex);
     
    ids = SetMarcEdit_02 () ;
    if ( ids ) return -4;
     

    // 화면의 째 Display.
    tmpData.Format ( _T("%d"), m_iDupRowIndex+1 );
    
	ids = pCM2->SetControlMgrData (_T("째"),tmpData,-1);
    if(ids <0) return -555;

    if( m_iDupRowIndex >= m_pDupDataMgr->GetRowCount()-1 )
	{
		GetDlgItem(IDC_bNEXT_SEL)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_bNEXT_SEL)->EnableWindow(TRUE);
	}

	if(m_iDupRowIndex <= 0){
		GetDlgItem(IDC_bPRE_SEL)->EnableWindow(FALSE);        
	}	
	else {
        GetDlgItem(IDC_bPRE_SEL)->EnableWindow(TRUE);
	}
    
    if ( m_pDupDataMgr->GetRowCount() > 0 ) {
    //  GetDlgItem(IDC_bUPDATE )->EnableWindow(TRUE);
        GetDlgItem(IDC_bHOLDING)->EnableWindow(TRUE);
		GetDlgItem(IDC_bREQUEST)->EnableWindow(TRUE);
     
    } else {
    //  GetDlgItem(IDC_bUPDATE )->EnableWindow(FALSE);
        GetDlgItem(IDC_bHOLDING)->EnableWindow(FALSE);
		GetDlgItem(IDC_bREQUEST)->EnableWindow(FALSE);       
       
    }    
    
    if ( m_pDupDataMgr->GetRowCount() > 1 ) {
       
    } else {
       
    }


    // 서지치환취소시를 위한 백업용
	ids = m_pDupDataMgr->GetCellData (_T("MARC"),m_iDupRowIndex,g_BackupMarc);
    if ( ids ) return -5;

	return 0 ;
}


INT  CCENTER_UPLOAD_03::SetMarcEdit_02()
{
    INT ids ;
   
    m_marcEditor_1.SubclassDlgItem(IDC_RICHEDIT_DUP, -1, this);
	m_pDupDataMgr->SetMarcMgr (m_pInfo->pMarcMgr );
	
	CString tmp_Stream ;
	ids = m_pDupDataMgr->GetCellData(_T("MARC"),m_iDupRowIndex,tmp_Stream );
	if(tmp_Stream.IsEmpty() ) 
	{
		return 0 ;
	 	//AfxMessageBox(_T("복본 MARC 를 가져오지 못했습니다.!!"));
	}

	m_strStreamMarc2=tmp_Stream;
	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc2, &m_marc2))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	//insert marc
	m_marcEditor_1.InitUserAlias();
	m_marcEditor_1.Init(m_pInfo->pMarcMgr, &m_marc2);
	m_marcEditor_1.Display();
	
    m_strStreamMarc2 = _T("") ;
	return 0 ;
}



VOID CCENTER_UPLOAD_03::OnbPREV() 
{
	// TODO: Add your control notification handler code here
	INT ids ;
	ids = MoveDataAndDisplay(-1);
//	if (ids<0) AfxMessageBox(_T("MoveDataAndDisplay() Error!!!"));

}

VOID CCENTER_UPLOAD_03::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
    INT ids ;
    ids = MoveDataAndDisplay(1);
	//if (ids<0) AfxMessageBox(_T("MoveDataAndDisplay() Error!!!"));
	
}

VOID CCENTER_UPLOAD_03::OnbNEW() 
{
	// TODO: Add your control notification handler code here
	
    INT ids;
	//m_marcEditor_0.ApplyEdit() ;
    m_marcEditor_0.ApplyEdit() ;
    ids = UploadProcess( TRUE );
    //if ( ids < 0 ) AfxMessageBox(_T("UploadProcess() Error!!!"));
    
    
   // ControlDisplay (_T("CM_CENTERUPLOAD_01"),_T("MainGrid"),-1);

}

VOID CCENTER_UPLOAD_03::OnOK() 
{
	// TODO: Add extra validation here
	INT ids ;
	m_marcEditor_0.ApplyEdit() ;
    ids = UploadProcess( FALSE );

}

VOID CCENTER_UPLOAD_03::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CCENTER_UPLOAD_03::OnbHOLDING() 
{

}

VOID CCENTER_UPLOAD_03::OnbPRESEL() 
{
	// TODO: Add your control notification handler code here
		INT ids = MoveDupDataAndDisplay( -1 );
}

VOID CCENTER_UPLOAD_03::OnbNEXTSEL() 
{
	// TODO: Add your control notification handler code here
		INT ids = MoveDupDataAndDisplay( 1 );
}

INT CCENTER_UPLOAD_03::UploadProcess( BOOL bNewUpload )
{
    INT ids;	
  
	//==============================================================================
	//2004-02-04 이학중 추가 :: CENTER_UPLOAD_YN CHECK 
	CCenterUpload Check_Upload_api(this) ;
	CString str_DIRECT_UPLOAD_YN ;
	Check_Upload_api.Check_Direct_Upload (str_DIRECT_UPLOAD_YN) ;
    //==============================================================================

	CString strRegNos , strUseLimitCodes , strSpeciesLimitCode;

	if(str_DIRECT_UPLOAD_YN == _T("N"))
	{   
		CString m_pDBAlias = _T("DM_CENTERUPLOAD_01") ;
		CString strSpeciesKey ;
		CString strMarc ;

		// 2012.11.26 ADD BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (제어번호추가)
		CString strControlNo;	

		m_pBaseDataMgr->GetCellData (_T("REC_KEY"),m_idx,strSpeciesKey) ;
		m_pBaseDataMgr->GetCellData (_T("MARC"),m_idx,strMarc) ;

		// 2012.11.26 ADD BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (제어번호추가)
		m_pBaseDataMgr->GetCellData (_T("제어번호"), m_idx, strControlNo);
		
		m_pBaseDataMgr->GetCellData (_T("이용제한구분"), m_idx, strSpeciesLimitCode);
		MakeStringData( strControlNo, strRegNos , strUseLimitCodes );
		ids = Check_Upload_api.Input_Temp_Mater_tbl (strSpeciesKey, strMarc, _T(""), strControlNo ,strRegNos , strUseLimitCodes , strSpeciesLimitCode);

		if(ids < 0 )
		{
			ids = UpdateUploadStatus( this, m_pDBAlias, m_idx, _T("4") );
			if (ids<0) AfxMessageBox (_T("UpdateUploadStatus(4) Error!!!"), FALSE);
			
			m_pBaseDataMgr->DeleteRow ( m_idx );
		    return -2;
		}
        else
		{
			ids = UpdateUploadStatus( this, m_pDBAlias, m_idx, _T("0") );
            Check_Upload_api.Update_Temp_Lib_Statistics_Tbl(1,_T("U")) ;
			AfxMessageBox( _T("선택된 자료의 UPLOAD가 완료되었습니다.") );
		}	

	}
    else if(str_DIRECT_UPLOAD_YN == _T("Y"))
    {
			
		CString m_pDBAlias = _T("DM_CENTERUPLOAD_01") ;
		CString msg;
		if ( bNewUpload ) {
			msg = _T("신규로 UPLOAD하겠습니까?");
		} else {
			msg = _T("소장추기/소장신규 작업을 하겠습니까?");
		}
		
		if ( IDNO == AfxMessageBox( msg, MB_YESNO ) ) return 0;
		
		
		if ( m_bMarcSyntaxCheck ) {
			ids = m_marcEditor_0.ApplyEdit() ;
			
			if (ids) {
				if ( ids <0 ) {
					AfxMessageBox( _T("마크에 문제가 있는 자료입니다. 확인후 UPLOAD하십시오.") );
					return 0;
				}
				else return -1;
			}
		}
		
		ids = UpdateUploadStatus( this, m_pDBAlias, m_idx, _T("0") );
		
	
		AfxMessageBox( _T("선택된 자료의 UPLOAD가 완료되었습니다.") );
	}


	CESL_ControlMgr *pCM = FindCM (_T("CM_CENTERUPLOAD_01")) ;
	if(pCM == NULL) 
	{
		AfxMessageBox (_T("FindCM() Fail")) ;
		return -1;
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid")) ;
	if(pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Fail")) ;
		return -1 ;
	}


	MoveDataAndDisplay(1);

	if(pGrid != NULL)
	{			
		pGrid->DisplayNumber () ;
	}
	
    //================================================== 
	ControlDisplay (_T("CM_CENTERUPLOAD"),_T("MainGrid"),-1) ;

	if ( m_pBaseDataMgr->GetRowCount() <= 0 )
	{
		OnCancel();
		return 0;
	}
  
    
    return 0 ;
}

VOID CCENTER_UPLOAD_03::OnbREQUEST() 
{

}

HBRUSH CCENTER_UPLOAD_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}

VOID CCENTER_UPLOAD_03::MakeStringData( CString ControlNo , CString &Reg_nos , CString &UseLimitCodes )
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

