// SelectMailModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelectMailModeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectMailModeDlg dialog


CSelectMailModeDlg::CSelectMailModeDlg(CESL_Mgr* pParent,CLocMailInfo *pLocMailInfo)
	: CESL_Mgr(CSelectMailModeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectMailModeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLocMailInfo = pLocMailInfo;
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CSelectMailModeDlg::~CSelectMailModeDlg()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-14]
}

VOID CSelectMailModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectMailModeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CSelectMailModeDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectMailModeDlg)
	ON_BN_CLICKED(IDC_btnSELECT_MODE, OnbtnSELECTMODE)
	ON_BN_CLICKED(IDC_btnCLOSE_MODE, OnbtnCLOSEMODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectMailModeDlg message handlers

BOOL CSelectMailModeDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-14]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	m_nCloseMode = 0;
	
	InitMailClass();
	SetMailClass();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END

	return FALSE;

}



VOID CSelectMailModeDlg::OnbtnSELECTMODE() 
{

	EFS_BEGIN

	INT ids;
	
	SetMailClass();

	if( m_nMailClass == -1 )
	{
		ids = MessageBox( _T("선택된 메일은 알수 없는 메일 종류입니다. 작업을 중지하시겠습니까?") , _T("메일 선택") , MB_YESNO | MB_ICONQUESTION );
		if( ids == IDYES )
			OnbtnCLOSEMODE();
		else
			return ;
	}

	m_nCloseMode = 1;
	OnCancel();


	EFS_END
}

VOID CSelectMailModeDlg::OnbtnCLOSEMODE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END

}

INT CSelectMailModeDlg::GetMailClass()
{
	EFS_BEGIN

	return m_nMailClass;

	EFS_END

	return -1;
}

// 콤보 박스에서 선택된 것의 메일 클래스를 구성한다.
INT CSelectMailModeDlg::SetMailClass()
{
	EFS_BEGIN

	CString sMailName;

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbMAILCLASS);

	if( pCombo->GetCurSel() < 1 ) m_nMailClass = -1;

	pCombo->GetLBText( pCombo->GetCurSel() , sMailName );

	m_nMailClass = m_pLocMailInfo->GetMailClassFromName( sMailName );

	if( m_nMailClass == 100 ) m_nMailClass = -1;

	return 0;

	EFS_END

	return -1;

}

INT CSelectMailModeDlg::GetCloseMode()
{
	EFS_BEGIN

	return m_nCloseMode;

	EFS_END

	return -1;

}

// 메일 종류를 콤보 박스에 집어 넣는다.
INT CSelectMailModeDlg::InitMailClass()
{

	EFS_BEGIN

	INT ids;

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbMAILCLASS);

	CString sMailName;
	for( INT i = 0 ; i <= 10 ; i++ )
	{
		if ( i==8 ) continue;
		ids = m_pLocMailInfo->IsUseMailClass(i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClass") );
		if( ids > 0 ) continue;

		sMailName = m_pLocMailInfo->GetMailName(i);
		if ( sMailName==_T("") ) continue;

		pCombo->AddString( sMailName );
	}

	if( pCombo->GetCount() > 0 )
		pCombo->SetCurSel(0);

	return 0;

	EFS_END

	return -1;

}

HBRUSH CSelectMailModeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}