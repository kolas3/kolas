// InputSanFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputSanFileDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputSanFileDlg dialog


CInputSanFileDlg::CInputSanFileDlg(CESL_Mgr* pParent /* = NULL*/)   // standard constructor
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputSanFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//===================================================
	//2010.11.17 ADD BY PJW : 제적, 파손에 대한 조건 추가
	m_bChkDiscardDamage = FALSE;
	m_strStartAcqDate = _T("");
	m_strEndAcqDate = _T("");
	m_strStartLastLoanDate = _T("");
	m_strEndLastLoanDate = _T("");
	//===================================================

	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239)); 
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CInputSanFileDlg::~CInputSanFileDlg()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
	m_Dlg_brush.DeleteObject();
}

VOID CInputSanFileDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputSanFileDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CInputSanFileDlg, CDialog)
	//{{AFX_MSG_MAP(CInputSanFileDlg)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_BN_CLICKED(IDC_SELECTFILE, OnSelectfile)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkDiscardDamage, OnchkDiscardDamage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputSanFileDlg message handlers

BOOL CInputSanFileDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();		
	
//	EnableThemeDialogTexture(GetSafeHwnd());
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("OPEN_SAN_FILE")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
//	INT nIds = 0;
//	nIds = ;
	if(0 > init() )
	{
		return TRUE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

INT CInputSanFileDlg::GetSanFileDM(CESL_DataMgr *pDM)
{
	EFS_BEGIN

	CESL_DataMgr *pSourceDM = FindDM(_T("DM_OPEN_SAN_FILE") );
	pSourceDM->m_nCols = 1;
	pDM->CopyDM( pSourceDM );

	return 0;

	EFS_END
	return -1;

}


VOID CInputSanFileDlg::OnbtnINIT() 
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_OPEN_SAN_FILE") );
	if( pDM == NULL ) return;

	ids = pDM->FreeData();
	if( ids < 0 ) return;

	ids = AllControlDisplay( _T("CM_OPEN_SAN_FILE") );
	if( ids < 0 ) return;

	m_sFileName = _T("");
	GetDlgItem( IDC_edtFILE_NAME )->SetWindowText( m_sFileName );


	EFS_END
}

VOID CInputSanFileDlg::OnSelectfile() 
{
	EFS_BEGIN

	INT ids;
	ids = LoadFile();	

	if( ids < 0 ) return;

	ids = DeleteDuplicateRegNo();
	if( ids < 0 ) return;

	// GRID 보여주기
	ids = AllControlDisplay( _T("CM_OPEN_SAN_FILE") );
	if( ids < 0 ) return ;

	EFS_END
}

INT CInputSanFileDlg::LoadFile()
{
	EFS_BEGIN

    CString sFileName;
    INT     ids;

	TCHAR    sCurrentDirectory[_MAX_PATH];

	//  현 디렉토리를 저장한다.
    _tgetcwd( sCurrentDirectory, _MAX_PATH ) ;

	if ( _tcscmp( m_sWorkingDirectory, _T("")) )
		_tchdir( m_sWorkingDirectory );

	// 2010.11.17 UPDATE BY PJW : 텍스트 파일만 불러 오도록 한다.
//	CFileDialog dlg(TRUE, _T("*.*"), _T("*.*"),
//                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//                    _T("파일형식All Files(*.*)|*.*||") );
	// 2010.01.13 UPDATE BY PWR : 불러오기 화면이 아닌 열기화면으로 나오도록 수정
	CFileDialog dlg(TRUE, _T(".txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("파일형식(*.txt)|*.txt|") );
// 	CFileDialog dlg(FALSE, _T(".txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("파일형식(*.txt)|*.txt|") );
    
    if ( IDOK != dlg.DoModal() ) return 0;

	// 원래 디렉토리로 복귀
    _tgetcwd( m_sWorkingDirectory.GetBuffer(0), _MAX_PATH ) ;
	_tchdir( sCurrentDirectory ) ;

    // 파일명이 선택되지않고 '확인'을 click했을 경우.
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return -1;
    sFileName = dlg.GetPathName();

	ids = FileToDM( sFileName );
	if( ids < 0 ) 
	{
		AfxMessageBox( _T("파일을 여는데 문제가 발생하였습니다") );
		return -1;
	}

	if( m_sFileName.GetLength() > 0 ) m_sFileName +=_T(" , ");
	m_sFileName += sFileName;
	GetDlgItem( IDC_edtFILE_NAME )->SetWindowText( m_sFileName );

	return 0;


	EFS_END
	return -1;

}

INT CInputSanFileDlg::FileToDM( CString &sFileName )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_OPEN_SAN_FILE") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FileToDM( CString &sFileName )") );

	ids = SanFileAPI::RegNoFileLoad( pDM , this , sFileName);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FileToDM( CString &sFileName )") );
	if( ids > 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}


VOID CInputSanFileDlg::OnOK() 
{
	EFS_BEGIN
	
	INT nIds = 0;
	nIds = SaveMemberData();
	if( 0 > nIds )
	{
		return;
	}

	CDialog::OnOK();

	EFS_END
}

INT CInputSanFileDlg::DeleteDuplicateRegNo()
{
	EFS_BEGIN

	CString sDMAlias = _T("DM_OPEN_SAN_FILE");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) return -1;

	CString sRegNo;
	for( INT i = pDM->GetRowCount()-1; i >= 0 ; i-- )
	{
		pDM->GetCellData( i , 0 , sRegNo );
		for( INT j = i-1 ; j >= 0 ; j-- )
		{
			CString sTmpRegNo;
			pDM->GetCellData( j , 0 , sTmpRegNo );

			if( sRegNo.Compare( sTmpRegNo ) == 0 )
				pDM->DeleteRow( j );
		}
	}
	
	
	return 0;

	EFS_END
	return -1;

}
HBRUSH CInputSanFileDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetTextColor(RGB(0, 0, 0));		
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(239, 239, 239));		
		
		return m_Dlg_brush;		
	}
	// 여기까지 추가
	return hbr;
}

INT CInputSanFileDlg::init()
{
	// TODO: Add your control notification handler code here
	INT nIds = 0;
	CButton* pBtnDiscardDamage = (CButton*)GetDlgItem(IDC_chkDiscardDamage);

	pBtnDiscardDamage->SetCheck(FALSE);

	nIds = SetEnableTimeControl();
	nIds = InitDate();
	return 0;
}

INT CInputSanFileDlg::SetEnableTimeControl()
{
	// TODO: Add your control notification handler code here
	CButton* pBtnDiscardDamage = (CButton*)GetDlgItem(IDC_chkDiscardDamage);

	CDateTimeCtrl* pPickStartAcqDate = (CDateTimeCtrl*)GetDlgItem(IDC_pickStartAcqDate);
	CDateTimeCtrl* pPpickEndAcqDate = (CDateTimeCtrl*)GetDlgItem(IDC_pickEndAcqDate);
	CDateTimeCtrl* pPpickStartLastLoanDate = (CDateTimeCtrl*)GetDlgItem(IDC_pickStartLastLoanDate);
	CDateTimeCtrl* pPpickEndLastLoanDate = (CDateTimeCtrl*)GetDlgItem(IDC_pickEndLastLoanDate);

	BOOL bDiscardDamageCheckYN = pBtnDiscardDamage->GetCheck();

	if( TRUE != bDiscardDamageCheckYN )
	{
		pPickStartAcqDate->EnableWindow(FALSE);
		pPpickEndAcqDate->EnableWindow(FALSE);
		pPpickStartLastLoanDate->EnableWindow(FALSE);
		pPpickEndLastLoanDate->EnableWindow(FALSE);
	}
	else
	{
		pPickStartAcqDate->EnableWindow(TRUE);
		pPpickEndAcqDate->EnableWindow(TRUE);
		pPpickStartLastLoanDate->EnableWindow(TRUE);
		pPpickEndLastLoanDate->EnableWindow(TRUE);
	}

	return 0;
}

INT CInputSanFileDlg::InitDate()
{
	SYSTEMTIME systime;	
	CDateTimeCtrl* pPickStartAcqDate = (CDateTimeCtrl*)GetDlgItem(IDC_pickStartAcqDate);
	CDateTimeCtrl* pPpickEndAcqDate = (CDateTimeCtrl*)GetDlgItem(IDC_pickEndAcqDate);
	CDateTimeCtrl* pPpickStartLastLoanDate = (CDateTimeCtrl*)GetDlgItem(IDC_pickStartLastLoanDate);
	CDateTimeCtrl* pPpickEndLastLoanDate = (CDateTimeCtrl*)GetDlgItem(IDC_pickEndLastLoanDate);

	pPickStartAcqDate->GetTime(&systime);
	DateTime_SetSystemtime(pPickStartAcqDate->m_hWnd , GDT_NONE , &systime );

	pPpickEndAcqDate->GetTime(&systime);
	DateTime_SetSystemtime(pPpickEndAcqDate->m_hWnd , GDT_NONE , &systime );

	pPpickStartLastLoanDate->GetTime(&systime);
	DateTime_SetSystemtime(pPpickStartLastLoanDate->m_hWnd , GDT_NONE , &systime );

	pPpickEndLastLoanDate->GetTime(&systime);
	DateTime_SetSystemtime(pPpickEndLastLoanDate->m_hWnd , GDT_NONE , &systime );
	
	return 0;
}

INT CInputSanFileDlg::GetControlDate( CString &strStartDate, CString &strEndDate, INT nDateType )
{
	CDateTimeCtrl* pStartDate	= NULL;
	CDateTimeCtrl* pEndDate		= NULL;
	SYSTEMTIME systime;

	strStartDate = _T("");
	strEndDate = _T("");

	if( 0 == nDateType )
	{
		pStartDate	= (CDateTimeCtrl*) GetDlgItem(IDC_pickStartAcqDate);
		pEndDate	= (CDateTimeCtrl*) GetDlgItem(IDC_pickEndAcqDate);
	}
	else if( 1 == nDateType )
	{
		pStartDate	= (CDateTimeCtrl*) GetDlgItem(IDC_pickStartLastLoanDate);
		pEndDate	= (CDateTimeCtrl*) GetDlgItem(IDC_pickEndLastLoanDate);
	}
	else
	{
		pStartDate	= (CDateTimeCtrl*) GetDlgItem(IDC_pickStartAcqDate);
		pEndDate	= (CDateTimeCtrl*) GetDlgItem(IDC_pickEndAcqDate);
	}

	pStartDate->GetWindowText(strStartDate);
	strStartDate.Replace(_T("-"),_T(""));
	pEndDate->GetWindowText(strEndDate);
	strEndDate.Replace(_T("-"),_T(""));

	pStartDate->GetTime(&systime);
	if( DateTime_GetSystemtime(pStartDate->m_hWnd ,&systime) == GDT_NONE )
	{
		
		strStartDate.Empty();		

	}
	else
	{
		pStartDate->GetWindowText(strStartDate);
		strStartDate.Replace(_T("-"),_T("/"));
	}

	pEndDate->GetTime(&systime);
	if( DateTime_GetSystemtime(pEndDate->m_hWnd ,&systime) == GDT_NONE )
	{
		strEndDate.Empty();	
	}
	else
	{
		pEndDate->GetWindowText(strEndDate);
		strEndDate.Replace(_T("-"),_T("/"));					
	}	

//	if( strStartDate.IsEmpty() && strEndDate.IsEmpty() )
//	{
//		strStartDate = _T("0001/01/01 00:00:00");
//		strEndDate	 = _T("9999/12/31 23:59:59");
//	}
//	else
//	{
//		if( strStartDate.IsEmpty() )
//		{
//			strStartDate = strEndDate;
//		}
//		else if( strEndDate.IsEmpty() )
//		{
//			strEndDate = strStartDate;
//		}
//
//		strStartDate = strStartDate + _T(" 00:00:00");
//		strEndDate = strEndDate + _T(" 23:59:59");
//	}
	if( !strStartDate.IsEmpty() || !strEndDate.IsEmpty() )
	{
		if( strStartDate.IsEmpty() )
		{
			strStartDate = strEndDate;
		}
		else if( strEndDate.IsEmpty() )
		{
			strEndDate = strStartDate;
		}

		strStartDate = strStartDate + _T(" 00:00:00");
		strEndDate = strEndDate + _T(" 23:59:59");
	}

	return 0;
}

INT CInputSanFileDlg::SaveMemberData()
{
	CButton* pBtnDiscardDamage = (CButton*)GetDlgItem(IDC_chkDiscardDamage);

	BOOL bDiscardDamageYN = pBtnDiscardDamage->GetCheck();

	CString strStartDate = _T("");
	CString strEndDate = _T("");

	SetChkDiscardDamage(bDiscardDamageYN);

	if( TRUE == bDiscardDamageYN )
	{
		GetControlDate( strStartDate, strEndDate, 0 );
		SetStartAcqDate(strStartDate);
		SetEndAcqDate(strEndDate);

		GetControlDate( strStartDate, strEndDate, 1 );
		SetStartLastLoanDate(strStartDate);
		SetEndLastLoanDate(strEndDate);
	}

	return 0;
}

void CInputSanFileDlg::OnchkDiscardDamage() 
{
	// TODO: Add your control notification handler code here
	INT nIds = 0;
	nIds = SetEnableTimeControl();
	
}
