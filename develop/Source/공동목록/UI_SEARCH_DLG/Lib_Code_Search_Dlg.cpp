// Lib_Code_Search_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lib_Code_Search_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLib_Code_Search_Dlg dialog

CLib_Code_Search_Dlg::CLib_Code_Search_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmd_Search_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CLib_Code_Search_Dlg::~CLib_Code_Search_Dlg()
{
	
}


BOOL CLib_Code_Search_Dlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CLib_Code_Search_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLib_Code_Search_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLib_Code_Search_Dlg, CDialog)
	//{{AFX_MSG_MAP(CLib_Code_Search_Dlg)
	ON_BN_CLICKED(IDC_RADIO_CHECK_1, OnRadioCheck1)
	ON_BN_CLICKED(IDC_RADIO_CHECK_2, OnRadioCheck2)
	ON_BN_CLICKED(IDC_Close, OnClose)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLib_Code_Search_Dlg message handlers


BOOL CLib_Code_Search_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	 
	if(InitESL_Mgr(_T("SM_LIB_CODE_SEARCH"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	CButton *m_Check_1 = (CButton*)GetDlgItem(IDC_RADIO_CHECK_1) ;
    m_Check_1->SetCheck (1) ;
    OnRadioCheck1() ;

    AllControlDisplay (_T("CM_LIB_CODE_SEARCH"),-1) ;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CLib_Code_Search_Dlg::OnOK() 
{
	// TODO: Add extra validation here

	CESL_ControlMgr *pCM = FindCM(_T("CM_LIB_CODE_SEARCH"));

	CButton *m_Check_1 = (CButton*)GetDlgItem(IDC_RADIO_CHECK_1) ;
    CButton *m_Check_2 = (CButton*)GetDlgItem(IDC_RADIO_CHECK_2) ;

    CString sGubun_1,sGubun_2,sLib_Code,sLib_Name ;

	CString sWhere ;
 	if(m_Check_1->GetCheck () == TRUE)
	{
	    pCM->GetControlMgrData (_T("관종"),sGubun_1 ,-1) ;
		pCM->GetControlMgrData (_T("지역"),sGubun_2,-1) ;

		if(!sGubun_1.IsEmpty () && sGubun_1.IsEmpty ())
		{
             sWhere += _T("CLASS_CODE LIKE '")+sGubun_1+_T("%'") ;
		}
		else if(!sGubun_1.IsEmpty () && !sGubun_1.IsEmpty ())
		{
			 sWhere += _T("CLASS_CODE LIKE '")+sGubun_1+sGubun_2+_T("%'") ;
		}
		else sWhere ;      		
	}
	else
	{
		pCM->GetControlMgrData (_T("도서관부호"),sLib_Code,-1) ;
        pCM->GetControlMgrData (_T("도서관명"),sLib_Name,-1) ;

        if(!sLib_Code.IsEmpty ())
		{
			sWhere = _T(" CLASS_CODE = '") + sLib_Code+_T("'")  ;
		    
			if(!sLib_Name.IsEmpty ())
			{
				sWhere += _T("AND INDEX_DESCRIPTION LIKE '") + sLib_Name +_T("%'")  ;
			}

		}
        else if(sLib_Code.IsEmpty () && !sLib_Name.IsEmpty ())
		{
            sWhere += _T("INDEX_DESCRIPTION LIKE '") + sLib_Name +_T("%'")  ;
		}		
	}
	    
	CESL_DataMgr *pLIB_DM = FindDM(_T("DM_LIB_CODE_SEARCH"));
    if(pLIB_DM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

	pLIB_DM->RefreshDataManager (sWhere) ;
   
    if(pLIB_DM->GetRowCount () ==0 )
	{
		AfxMessageBox(_T("검색 결과가 없습니다."));
		return ;
	}

	AllControlDisplay (_T("CM_LIB_CODE_SEARCH"),-1) ;
 
}

VOID CLib_Code_Search_Dlg::OnRadioCheck1() 
{
	// TODO: Add your control notification handler code here
		(CComboBox *)GetDlgItem(IDC_COMBO_GUBUN1)->EnableWindow(TRUE);
        (CComboBox *)GetDlgItem(IDC_COMBO_GUBUN2)->EnableWindow(TRUE);
        (CButton *)GetDlgItem(IDC_eLIBCODE)->EnableWindow(FALSE);
        (CButton *)GetDlgItem(IDC_eLIB_NAME)->EnableWindow(FALSE);
}

VOID CLib_Code_Search_Dlg::OnRadioCheck2() 
{
	// TODO: Add your control notification handler code here
	    (CComboBox *)GetDlgItem(IDC_COMBO_GUBUN1)->EnableWindow(FALSE);
        (CComboBox *)GetDlgItem(IDC_COMBO_GUBUN2)->EnableWindow(FALSE);
        (CButton *)GetDlgItem(IDC_eLIBCODE)->EnableWindow(TRUE);
        (CButton *)GetDlgItem(IDC_eLIB_NAME)->EnableWindow(TRUE);
}

VOID CLib_Code_Search_Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	GetDlgItem(IDC_eSELECT_LIBCODE)->GetWindowText(m_code);
    if(m_code.IsEmpty ())
	{
		AfxMessageBox(_T("도서관 부호를 선택하시지 않으셨습니다.")) ;
	}

	CDialog::OnCancel();
}

BEGIN_EVENTSINK_MAP(CLib_Code_Search_Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CLib_Code_Search_Dlg)
	ON_EVENT(CLib_Code_Search_Dlg, IDC_LIBCODE_GRID, -600 /* Click */, OnClickLibcodeGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CLib_Code_Search_Dlg::OnClickLibcodeGrid() 
{
	// TODO: Add your control notification handler code here

	CESL_ControlMgr *pCM = FindCM(_T("CM_LIB_CODE_SEARCH"));
	if(pCM == NULL) 
	{
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid_Lib"));//MainGrid_Lib
	if(pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Error~!"));
		return ;
	}

	INT nRowIndex ;

	nRowIndex = pGrid->GetIdx () ;

	CESL_DataMgr *pDM = FindDM(_T("DM_LIB_CODE_SEARCH"));
    if(pDM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}
	
	CString libCode; 
	pDM->GetCellData (_T("도서관부호"),nRowIndex,libCode);
    if( pGrid->GetAt(nRowIndex,0) == _T("V") )
	{
		GetDlgItem(IDC_eSELECT_LIBCODE)->GetWindowText(m_code);
		m_code.TrimLeft(',');
		m_code.TrimRight(',');
		m_code.TrimLeft();
		m_code.TrimRight();
		if( m_code.GetLength() > 1 )
		{
			if(m_code.Find(libCode) < 0)
				libCode = m_code + _T(", ") + libCode;
			else  
				libCode = m_code;
		}
	}
	else
	{
		GetDlgItem(IDC_eSELECT_LIBCODE)->GetWindowText(m_code);
		if( m_code.GetLength() > 1 )
		{
			if( m_code == libCode )
				;			
			else if( m_code.Find(libCode ) >= 0 )
			{
				m_code.Replace(_T(" ,")+libCode,_T(""));
				m_code.Replace(_T(",")+libCode,_T(""));
				m_code.Replace(libCode,_T(""));
				m_code.TrimLeft(',');
				m_code.TrimRight(',');
				m_code.TrimLeft();
				m_code.TrimRight();
				libCode = m_code;
			}
		}
	}
	
	libCode.TrimLeft(',');
	libCode.TrimRight(',');
	libCode.TrimLeft();
	libCode.TrimRight();
	GetDlgItem(IDC_eSELECT_LIBCODE)->SetWindowText(libCode);

}

VOID CLib_Code_Search_Dlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

HBRUSH CLib_Code_Search_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
