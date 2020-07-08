// LoanAppendixListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoanAppendixListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanAppendixListDlg dialog


CLoanAppendixListDlg::CLoanAppendixListDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CLoanAppendixListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_pPreRegNoMgr = NULL;
}

CLoanAppendixListDlg::~CLoanAppendixListDlg()
{
}

VOID CLoanAppendixListDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanAppendixListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLoanAppendixListDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanAppendixListDlg)
	ON_BN_CLICKED(IDC_btnLOAN_CANCEL, OnbtnCANCEL)
	ON_BN_CLICKED(IDC_btnSELECT, OnbtnSELECT)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanAppendixListDlg message handlers

BOOL CLoanAppendixListDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	if(InitESL_Mgr(_T("SM_APPENDIX_INFO_LIST")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	ViewGrid();
	
  	GetDlgItem(IDC_edtAPPENDIX_BARCODE)->SetFocus();

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_APPENDIX_INFO"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_APPENDIX_INFO_LIST"), _T("AppendixGrid"));
	if(NULL != pDM && NULL != pGrid)
	{
		const int ROW_COUNT = pDM->GetRowCount();
		if(1 == ROW_COUNT)
		{
			pGrid->SetAt(
				0,
				pGrid->m_nSelectIdx,
				pGrid->m_sSelectString
				);
		}
	}	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

INT CLoanAppendixListDlg::ViewGrid()
{
	EFS_BEGIN

	INT ids = AllControlDisplay( _T("CM_APPENDIX_INFO_LIST") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("ViewGrid") );
	return 0;

	EFS_END
	return -1;

}


VOID CLoanAppendixListDlg::OnbtnCANCEL() 
{
	EFS_BEGIN

	CString sDMAlias = _T("DM_BO_LOC_3100_APPENDIX_INFO");
	
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_VOID2(  -1 , _T("SelectCMToDM") );

	pDM->FreeData();

	OnCancel();

	EFS_END
}


VOID CLoanAppendixListDlg::OnbtnSELECT() 
{
	EFS_BEGIN

	INT ids;

	ids = SelectCMToDM();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -1 , _T("OnbtnCANCEL") );

	if( ids == 0 ) 
	{
		AfxMessageBox(_T("부록자료중 대출할 자료를 선택해주십시오!!"));
		return;
	}
	else if( ids == 1000 )
		return;
	
	OnCancel();

	EFS_END
}

INT CLoanAppendixListDlg::SelectCMToDM()
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_BO_LOC_3100_APPENDIX_INFO");
	CString sCMAlias = _T("CM_APPENDIX_INFO_LIST");
	CString sGridAlias = _T("AppendixGrid");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("SelectCMToDM") );

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("SelectCMToDM") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("SelectCMToDM") );

	pGrid->SelectMakeList();

	INT nRowCount = pDM->GetRowCount();

	INT nCheckColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs( this , pGrid , _T("선정") , nCheckColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("SelectCMToDM") ); 

	CString sCheck;
	CString sWorkingStatus;

	INT nCheck = 0;

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		if( nCheckColIndex == -1 ) continue;
		
		pGrid->GetAt( i , nCheckColIndex , sCheck );
		if( sCheck.IsEmpty() ) continue;

		nCheck++;

		pDM->GetCellData( _T("자료상태") , i , sWorkingStatus );

		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );

		if( ( sWorkingStatus.Compare(_T("BOL112N")) == 0 ) || 
			( sWorkingStatus.Compare(_T("SEL112N")) == 0 ) || 
			( sWorkingStatus.Compare(_T("SEL212N")) == 0 )
		)
		{
		}
		else
		{
			AfxMessageBox( _T("비치중인 자료만 선택해주십시오.") );
			return 1000;
		}

		
	}

	if ( nCheck > 0 )
	{
		for( i = nRowCount-1 ; i > -1 ; i-- )
		{
			if( nCheckColIndex == -1 ) continue;

			pGrid->GetAt( i , nCheckColIndex , sCheck );
			if( !sCheck.IsEmpty() ) continue;
			ids = pDM->DeleteRow(i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("SelectCMToDM") );
		}

		CString sCode;
		for( i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("자료실") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("SelectCMToDM") );

			m_pInfo->pCodeMgr->ConvertDescToCode( _T("자료실구분") , sCode , sCode );

			ids = pDM->SetCellData( _T("자료실") , sCode , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("SelectCMToDM") );
		}


		return pDM->GetRowCount();
	}

	return nCheck;
	EFS_END
	return -1;

}


BOOL CLoanAppendixListDlg::PreTranslateMessage(MSG* pMsg) 
{
	HWND MsgH = pMsg->hwnd;

	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN )
		{
			if( GetDlgItem( IDC_edtAPPENDIX_BARCODE )->m_hWnd == MsgH )
			{
				CheckBarCodeRegNo();
				return 1;
			}
			else
			{
				OnbtnSELECT();
				return 1;
			}
		}
		else if(pMsg->wParam == VK_TAB )
		{
			OnbtnSELECT();
			return 1;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnCANCEL();
		}
	}

			
	return CDialog::PreTranslateMessage(pMsg);
}


INT CLoanAppendixListDlg::CheckBarCodeRegNo()
{
	INT ids;

	CString sInputRegNo;
	GetDlgItem(IDC_edtAPPENDIX_BARCODE)->GetWindowText( sInputRegNo );
	sInputRegNo.TrimLeft(); sInputRegNo.TrimRight();

	CLocCommonAPI::MakeRegNoSpaceToZero(this, sInputRegNo, m_pPreRegNoMgr);
		
	CString sDMAlias = _T("DM_BO_LOC_3100_APPENDIX_INFO");
	CString sCMAlias = _T("CM_APPENDIX_INFO_LIST");
	CString sGridAlias = _T("AppendixGrid");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CheckBarCodeRegNo") );

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CheckBarCodeRegNo") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CheckBarCodeRegNo") );


	CString sRegNo;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("등록번호") , i , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CheckBarCodeRegNo") );

		if( sRegNo.Compare(sInputRegNo) != 0 ) continue;

		CString strCheck;
		pGrid->GetAt( i, 0, strCheck);

		if(_T("V") == strCheck)
		{
			pGrid->SelectMakeList();
			int nSelCnt = pGrid->SelectGetCount();
			if(1 < nSelCnt)
			{
				CString strMsg;
				strMsg.Format(
					_T("두 개 이상의 선정된 자료가 있습니다. 선정된 자료를 모두 대출하시겠습니까?\r\n")
					_T("예 : 모두대출, 아니오 : 현재 입력된 부록(%s)만 대출\r\n"), sInputRegNo);

				UINT nFlag = AfxMessageBox(strMsg, MB_YESNOCANCEL);
				
				switch(nFlag)
				{
					case IDCANCEL:
						return 0;
						break;
					case IDNO:
						pGrid->UnSelectAll();
						pGrid->SetAt( i , 0 , _T("V") );
					case IDYES:
					default:
						OnbtnSELECT();	
						break;
				}				
			}
			else
			{
				OnbtnSELECT();				
			}

			GetDlgItem(IDC_edtAPPENDIX_BARCODE)->SetWindowText( _T("") );
		}
		else
		{
			pGrid->SetAt( i , 0 , _T("V") );
			pGrid->SetReverse( i );
			GetDlgItem(IDC_edtAPPENDIX_BARCODE)->SetWindowText( _T("") );
		}

		return 0;
	}

	AfxMessageBox( _T("부록자료가 존재하지 않습니다.") );


	return 0;
}

HBRUSH CLoanAppendixListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}

void CLoanAppendixListDlg::OnClose() 
{
	OnbtnCANCEL();
	
	CDialog::OnClose();
}

void CLoanAppendixListDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	

	if(TRUE == bShow)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_APPENDIX_INFO"));
		CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_APPENDIX_INFO_LIST"), _T("AppendixGrid"));
		if(NULL != pDM && NULL != pGrid)
		{
			const int ROW_COUNT = pDM->GetRowCount();
			if(1 == ROW_COUNT)
			{
 				CWnd* pWnd = GetDlgItem(IDC_btnSELECT);
 				if(NULL != pWnd)
 				{
 					pWnd->SetFocus();
 				}
			}
			else
			{
				GetDlgItem(IDC_edtAPPENDIX_BARCODE)->SetFocus();
			}
		}
	}
}
