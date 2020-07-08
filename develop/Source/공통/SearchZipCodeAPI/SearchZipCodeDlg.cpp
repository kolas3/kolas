// SearchZipCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SearchZipCodeDlg.h"
#include "SearchZipCode02Dlg.h"
#include "AddressImpExpDlg.h"
#include "SearchZipCodeDlgNew.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCodeDlg dialog


CSearchZipCodeDlg::CSearchZipCodeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchZipCodeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sDMAlias = _T("DM_SEARCH_ZIP_CODE");
	m_sGridCMAlias = _T("CM_SEARCH_ZIP_CODE_GRID");
	m_sGridAlias = _T("주소그리드");
	m_sSearchCMAlias = _T("CM_SEARCH_ZIP_CODE_SEARCH_INFO");

	m_nCloseMode = 0;
}

CSearchZipCodeDlg::~CSearchZipCodeDlg()
{
	
}

VOID CSearchZipCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchZipCodeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSearchZipCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchZipCodeDlg)
	ON_BN_CLICKED(IDC_btnZIP_CODE_SEARCH, OnbtnZIPCODESEARCH)
	ON_BN_CLICKED(IDC_btnZIP_CODE_SEARCH2, OnbtnZIPCODESEARCH2)
	ON_BN_CLICKED(IDC_btnZIP_CODE_SEARCH3, OnbtnZIPCODESEARCH3)
	ON_BN_CLICKED(IDC_btnZIP_CODE_SEARCH4, OnbtnZIPCODESEARCH4)
	ON_BN_CLICKED(IDC_btnZIP_CODE_SEARCH5, OnbtnZIPCODESEARCH5)
	ON_BN_CLICKED(IDC_btnZIP_CODE_SEARCH6, OnbtnZIPCODESEARCH6)
	ON_BN_CLICKED(IDC_btnZIP_CODE_SEARCH7, OnbtnZIPCODESEARCH7)
	ON_WM_SHOWWINDOW()
	ON_EN_SETFOCUS(IDC_edtZIP_CODE, OnSetfocusedtZIPCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCodeDlg message handlers

BOOL CSearchZipCodeDlg::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CSearchZipCodeDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SM_SEARCH_ZIP_CODE")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pGridCM = FindCM( m_sGridCMAlias );
	if( m_pGridCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pGrid = (CESL_Grid*)(m_pGridCM->FindControl( m_sGridAlias ));
	if( m_pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	m_pDM = FindDM( m_sDMAlias );
	if( m_pDM == NULL )ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE


	EFS_END
	return FALSE;

}

VOID CSearchZipCodeDlg::OnbtnZIPCODESEARCH() 
{
	EFS_BEGIN

	INT ids;

	ids = SearchZipCode();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnZIPCODESEARCH") );

	if( ids > 0 ) 
		AfxMessageBox(_T("검색 조건과 일치하는 자료가 존재하지 않습니다.") );

	EFS_END
}

INT CSearchZipCodeDlg::SearchZipCode()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( m_sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchZipCode") );

	CESL_ControlMgr *pSearchCM = FindCM( m_sSearchCMAlias );
	if( pSearchCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchZipCode") );

	CString sWhere;

	CString sDong;
	ids = pSearchCM->GetControlMgrData( _T("동") , sDong );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchZipCode") ); 
	sDong.TrimLeft(); sDong.TrimRight();

	if( sDong.IsEmpty() )
	{
		AfxMessageBox( _T("검색어를 입력하십시오") );
		return 0;
	}
	
	// 2006-05-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	// 리 단위 검색이 되도록 부분일치로 변경
	/*
	sWhere.Format( _T(" (DONG LIKE '%s%%') OR ")
					_T(" (SIDO LIKE '%s%%') OR ")
					_T(" (GUGUN LIKE '%s%%') ")
					, sDong , sDong , sDong );
	*/
	sWhere.Format( _T(" (DONG LIKE '%%%s%%') OR ")
					_T(" (SIDO LIKE '%%%s%%') OR ")
					_T(" (GUGUN LIKE '%%%s%%') ")
					, sDong , sDong , sDong );
	// ----------------------------------------------------------------------------------------

	ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SearchZipCode") ); 

	ids = AllControlDisplay( m_sGridCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SearchZipCode") ); 

	if( pDM->GetRowCount() < 1 ) return 1;

	return 0;

	EFS_END
	return -1;

}

BOOL CSearchZipCodeDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		HWND MsgH = pMsg->hwnd;
		if(	GetDlgItem(IDC_edtZIP_CODE)->m_hWnd==MsgH	)
		{
			OnbtnZIPCODESEARCH();
			return TRUE;
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

BEGIN_EVENTSINK_MAP(CSearchZipCodeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSearchZipCodeDlg)
	ON_EVENT(CSearchZipCodeDlg, IDC_gridZIP_CODE, -600 /* Click */, OnClickgridZIPCODE, VTS_NONE)
	ON_EVENT(CSearchZipCodeDlg, IDC_gridZIP_CODE, -601 /* DblClick */, OnDblClickgridZIPCODE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


VOID CSearchZipCodeDlg::OnClickgridZIPCODE() 
{
	EFS_BEGIN

	if( m_pDM->GetRowCount() < 1 ) return ;
	m_pGrid->SetReverse( m_pGrid->GetIdx() );

	EFS_END
}

// 입력
VOID CSearchZipCodeDlg::OnbtnZIPCODESEARCH2() 
{
	EFS_BEGIN

	CSearchZipCode02Dlg InputDlg(this);
	InputDlg.SetMode(0);
	InputDlg.DoModal();

	EFS_END
}

// 수정
VOID CSearchZipCodeDlg::OnbtnZIPCODESEARCH3() 
{
	EFS_BEGIN

	if( m_pDM->GetRowCount() < 1 ) return ;

	CSearchZipCode02Dlg InputDlg(this);
	InputDlg.SetMode(1,m_pGrid->GetIdx());
	InputDlg.DoModal();	

	EFS_END
}

// 삭제
VOID CSearchZipCodeDlg::OnbtnZIPCODESEARCH4() 
{
	EFS_BEGIN

	INT ids;
	ids = DeletRow();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnZIPCODESEARCH4") );

	EFS_END
}

// 선택
VOID CSearchZipCodeDlg::OnbtnZIPCODESEARCH5() 
{
	EFS_BEGIN

	SelectProc();

	EFS_END
}

INT CSearchZipCodeDlg::DeletRow()
{
	EFS_BEGIN

	if( m_pDM->GetRowCount() < 1 ) return 0;

	INT ids = MessageBox( _T("정말로 삭제하시겠습니까?") , _T("우편번호 삭제") , MB_YESNO | MB_ICONQUESTION );
	if( ids == IDNO ) 
		return 0;

	INT nIndex = m_pGrid->GetIdx();

	CString sSql;
	CString sRecKey;

	ids = m_pDM->GetCellData( _T("REC_KEY") , nIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeletRow") ); 

	sSql.Format( _T("DELETE FROM CO_ZIP_CODE_TBL WHERE REC_KEY = %s ;") , sRecKey );
	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();
	m_pDM->AddFrame( sSql );
	ids = m_pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeletRow") ); 

	m_pDM->EndFrame();

	m_pDM->DeleteRow( nIndex );

	ids = AllControlDisplay( m_sGridCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeletRow") ); 

	AfxMessageBox(_T("삭제되었습니다."));
	
	return 0;

	EFS_END
	return -1;

}

VOID CSearchZipCodeDlg::OnbtnZIPCODESEARCH6() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CSearchZipCodeDlg::OnDblClickgridZIPCODE() 
{
	EFS_BEGIN

	// 2007.08.27 add by pdw
	// 그리드 헤더 클릭시 정렬이 되도록 
	// 선정 Proc 취소
	if(m_pGrid->GetMouseRow() == 0)
		return;

	m_nCloseMode = 0;
	SelectProc();

	EFS_END
}

VOID CSearchZipCodeDlg::SelectProc()
{
	EFS_BEGIN

	if( m_pDM->GetRowCount() < 1 ) return ;
		
	m_nCloseMode = 1;

	INT ids;
	
	ids = m_pDM->GetCellData( _T("ZIP_CODE") , m_pGrid->GetIdx() , m_sZipCode );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SelectProc") );

	CString sAddress;
	ids = m_pDM->GetCellData( _T("SIDO") , m_pGrid->GetIdx() , sAddress );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SelectProc") );
	m_sAddress = sAddress;

	ids = m_pDM->GetCellData( _T("GUGUN") , m_pGrid->GetIdx() , sAddress );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SelectProc") );
	m_sAddress += _T(" ") + sAddress;

	// 2011.05.24 UPDATE BY PWR : 컨트롤에 적용시에 동에서 숫자제외, 번지제외 (상세주소는직접입력되도록)
	ids = m_pDM->GetCellData( _T("DONG") , m_pGrid->GetIdx() , sAddress);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SelectProc") );
// 	m_sAddress += _T(" ") + sAddress;
	if(!sAddress.IsEmpty())
	{
		if(2 < sAddress.GetLength())
		{
			CString strValue;
			strValue = sAddress.Mid(sAddress.GetLength()-2, 2);
			if(_T("동)") == strValue)
			{
				INT nStart = 0;
				CString strGetData;
				for(INT i = sAddress.GetLength()-1; i > 1; i--)
				{
					strGetData = sAddress.Mid(i, 1);
					if(_T("(") == strGetData)
					{
						nStart = i;
						break;
					}
				}
				if(0 < nStart)
				{
					strValue = sAddress.Mid(0, nStart);
					if(!strValue.IsEmpty())
					{
						sAddress = strValue;
					}
				}
			}
		}
		m_sAddress += _T(" ") + sAddress;
	}
// 	ids = m_pDM->GetCellData( _T("BUNJI") , m_pGrid->GetIdx() , sAddress );
// 	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SelectProc") );
// 	if( !sAddress.IsEmpty() )
// 		m_sAddress += _T(" ") + sAddress;
// 		m_sAddress += _T(" ");

	OnCancel();

	EFS_END
}

VOID CSearchZipCodeDlg::OnbtnZIPCODESEARCH7() 
{
	EFS_BEGIN

	CAddressImpExpDlg Address(this);
	Address.DoModal();

	EFS_END
}

VOID CSearchZipCodeDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_edtZIP_CODE );
	if( bShow && pEdt->GetSafeHwnd() )
		pEdt->SetFocus();
}

INT CSearchZipCodeDlg::GetLanguageMode()
{
	// 1 : 한글
	// 2 : 영어

	INT mode = 2;
	HWND hFocusWnd = ::GetFocus();  
	HIMC hIMC = NULL;  
	if( hIMC = ImmGetContext( hFocusWnd ) )  
	{  
		DWORD fdwConversion, fdwSentence;  
		BOOL bRet = ImmGetConversionStatus( hIMC, &fdwConversion, &fdwSentence );  
		if( bRet )
			mode = ( fdwConversion & IME_CMODE_NATIVE ? 1 : 2 );  
		
		bRet = ImmReleaseContext( hFocusWnd, hIMC );  
	}  

	return mode;
}

VOID CSearchZipCodeDlg::SetHanMode( HWND hWnd )
{
	if( !hWnd )
		hWnd = this->GetSafeHwnd();

	HIMC hIMC = ImmGetContext( hWnd );
    DWORD dwConv, dwSent;
    DWORD dwTemp;

    ImmGetConversionStatus( hIMC, &dwConv, &dwSent );

    dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
    dwTemp |= IME_CMODE_NATIVE;
    dwConv = dwTemp;
    ImmSetConversionStatus( hIMC, dwConv, dwSent );
    ImmReleaseContext( hWnd, hIMC );
}

VOID CSearchZipCodeDlg::SetEngMode( HWND hWnd )
{
	if( !hWnd )
		hWnd = this->GetSafeHwnd();
    
	HIMC hIMC = ImmGetContext( hWnd );
    DWORD dwConv, dwSent;
    DWORD dwTemp;

    ImmGetConversionStatus( hIMC, &dwConv, &dwSent );

    dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
    dwTemp |= IME_CMODE_ALPHANUMERIC;
    dwConv = dwTemp;
	ImmSetConversionStatus( hIMC, dwConv, dwSent );

	ImmReleaseContext( hWnd, hIMC );
}

VOID CSearchZipCodeDlg::OnSetfocusedtZIPCODE() 
{
	// TODO: Add your control notification handler code here
	
	if( 2 == GetLanguageMode() )
		SetHanMode();			
}

HBRUSH CSearchZipCodeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
