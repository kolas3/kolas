// ViewUploadMarc.cpp : implementation file
//

#include "stdafx.h"
//#include _T("check_dup_api.h")
#include "ViewUploadMarc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewUploadMarc dialog


CViewUploadMarc::CViewUploadMarc(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CViewUploadMarc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewUploadMarc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    AfxInitRichEdit();
    
	m_iCheckValue = 0;
}

BOOL CViewUploadMarc::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CViewUploadMarc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewUploadMarc)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewUploadMarc, CDialog)
	//{{AFX_MSG_MAP(CViewUploadMarc)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewUploadMarc message handlers





BOOL CViewUploadMarc::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
	INT ids;
  /*
    if (InitESL_Mgr(_T("찾기")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
  */

    ids = Init();
    if ( ids < 0 ) {
		EndDialog( IDCANCEL );
		return FALSE;
    }

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CViewUploadMarc::Init()
{

/*    
    TCHAR cCode;
	if      ( _tcscmp( m_sFormCode, _T("A")  ) == 0 ) cCode = 'A';
	else if ( _tcscmp( m_sFormCode, _T("I")  ) == 0 ) cCode = 'I';
    else {
		if ( GetPublicateFormClass() == PF_MONOGRAPH ) {
			if      ( _tcscmp( m_sFormCode, _T("BK") ) == 0 ) cCode = 'M';
			else cCode = 'N';
		} else   cCode = 'S';
	}
    
     

    m_marcEditor_0.SubclassDlgItem(IDC_CDA_RICHEDIT22, -1, this);
    ids = 	m_pDataMgr->SetMarcMgr(m_pInfo->pMarcMgr);
	
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

*/  
	CString strTagCode,strErrMsg,strHelpMsg ;
    
	//---------------------------------------------------------------------------//
    m_MarcEdit_1.SubclassDlgItem (IDC_CDA_RICHEDIT1,-1,this);
	if(m_pInfo->pMarcMgr->Decoding (m_sBibMarc,&m_marc1))
	{
       	POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}
    m_MarcEdit_1.InitUserAlias();
	m_MarcEdit_1.Init(m_pInfo->pMarcMgr, &m_marc1);
	m_MarcEdit_1.Display();
    
    //---------------------------------------------------------------------------//

    m_MarcEdit_2.SubclassDlgItem (IDC_CDA_RICHEDIT2,-1,this);
	if(m_pInfo->pMarcMgr->Decoding (m_sHoldingMarc,&m_marc2))
	{
       	POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}
    m_MarcEdit_2.InitUserAlias();
	m_MarcEdit_2.Init(m_pInfo->pMarcMgr, &m_marc2);
	m_MarcEdit_2.Display();
  
	  
    //---------------------------------------------------------------------------//
	m_MarcEdit_5.SubclassDlgItem (IDC_CDA_RICHEDIT5,-1,this);
	if(m_pInfo->pMarcMgr->Decoding (m_sNokMarc,&m_marc5))
	{
       	POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}
    m_MarcEdit_5.InitUserAlias();
	m_MarcEdit_5.Init(m_pInfo->pMarcMgr, &m_marc5);
	m_MarcEdit_5.Display();

 
     
    // show status
    GetDlgItem( IDC_STATIC_3 )->SetWindowText( m_sBibStatus );
    GetDlgItem( IDC_STATIC_4 )->SetWindowText( m_sHoldingStatus );

    return 0;
}



  
VOID CViewUploadMarc::SetBibMarc( CString upload_status, CString marc, CString form_code )
{
	 m_sBibStatus = upload_status ;
     m_sBibMarc = marc ;
     m_sFormCode = form_code ;
      
}
VOID CViewUploadMarc::SetHoldingMarc( CString upload_status, CString marc )
{
	m_sHoldingStatus = upload_status ;
    m_sHoldingMarc = marc ;
}
VOID CViewUploadMarc::SetNokMarc( CString marc ) // 형식구분은 BIB MARC와 동일
{
    m_sNokMarc = marc ;
}
VOID CViewUploadMarc::GetBibMarc( CString marc )
{
    marc = m_sBibMarc ;
}
VOID CViewUploadMarc::GetHoldingMarc( CString marc )
{
   marc = m_sHoldingMarc ;
}
INT  CViewUploadMarc::GetUpdateCheck( )
{

	return  m_iCheckValue ;
}

HBRUSH CViewUploadMarc::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(pWnd->GetDlgCtrlID()){
        case IDC_STATIC_3:
        {
            if ( _tcscmp ( m_sBibStatus , _T("수정") ) == 0 ) {
	            // RED
		        pDC->SetTextColor(RGB(255,0,0));
            } else {
		        pDC->SetTextColor(RGB(0,0,255));
            }
		    break;
        }
	    case IDC_STATIC_4:
	    {
            if ( _tcscmp ( m_sHoldingStatus , _T("수정") ) == 0 ) {
	            // RED
		        pDC->SetTextColor(RGB(255,0,0));
            } else {
		        pDC->SetTextColor(RGB(0,0,255));
            }
		    break;
	    }
    }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

VOID CViewUploadMarc::OnOK() 
{
	// TODO: Add extra validation here

    
    INT ids = m_MarcEdit_1.ApplyEdit();
    if (ids <0) {
		AfxMessageBox (_T("서지MARC :: Syntax Check Error"));
		
	}
    else
	{   /*
		ids = m_MarcEdit_2.ApplyEdit ();
		if(ids <0) {
        AfxMessageBox (_T("소장MARC :: Syntax Check Error"));
		}
	    
		else
		*/
		CDialog::OnOK();
	}
    
	m_iCheckValue = ((CButton *)GetDlgItem(IDC_chCLASS1))->GetCheck();

}
