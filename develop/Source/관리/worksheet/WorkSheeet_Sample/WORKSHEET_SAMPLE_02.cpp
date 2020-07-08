// WORKSHEET_SAMPLE_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "WORKSHEET_SAMPLE_02.h"
#include "WORKSHEET_SAMPLE_01.h"
#include "WORKSHEET_SAMPLE_04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWORKSHEET_SAMPLE_02 dialog


CWORKSHEET_SAMPLE_02::CWORKSHEET_SAMPLE_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CWORKSHEET_SAMPLE_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWORKSHEET_SAMPLE_02::~CWORKSHEET_SAMPLE_02()
{
	
}

BOOL CWORKSHEET_SAMPLE_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CWORKSHEET_SAMPLE_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWORKSHEET_SAMPLE_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWORKSHEET_SAMPLE_02, CDialog)
	//{{AFX_MSG_MAP(CWORKSHEET_SAMPLE_02)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_WORKSHEET, OnWorksheet)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWORKSHEET_SAMPLE_02 message handlers

BOOL CWORKSHEET_SAMPLE_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MAIN_SAMPLE_INSERT") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MAIN_SAMPLE_INSERT") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CWORKSHEET_SAMPLE_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	CWORKSHEET_SAMPLE_01 *pParent = ( CWORKSHEET_SAMPLE_01 * )pParentMgr;
	
	m_index = 0;
	if( 2 == m_mode )
	{
		m_checkedRow = pParent->GetCheckedRowInfo( m_cnt );
	
		if( !m_cnt )
		{
			AfxMessageBox( _T("선택 목록이 없습니다.") );
			PostMessage( WM_CLOSE );
		}

		ShowControls();
	}

	CWORKSHEET_SAMPLE_04 dlg( this );
	dlg.m_mode = m_mode;	
	dlg.m_workSheetInitFlag = 1;
	
	dlg.DoModal();
}

VOID CWORKSHEET_SAMPLE_02::OnOK() 
{
	// TODO: Add extra validation here
	
	CString data[ 7 ];
	CString alias[ 7 ] = {
		_T("ID"), _T("PASSWORD"), _T("NAME"), _T("TEL"), _T("BIRTHDAY"), _T("GRADE"), _T("ADDRESS") };
	for( INT i = 0; i < 7; i++ )
		data[ i ] = TakeControlData( alias[ i ] );

	CESL_DataMgr *pDM = FindDM( _T("DM_MAIN_SAMPLE") );
	pDM->StartFrame();
	pDM->InitDBFieldData();

	CString fieldType[ 7 ] = { _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };
	for( i = 0; i < 7; i++ )
		pDM->AddDBFieldData( alias[ i ], fieldType[ i ], data[ i ] );

	pDM->SendFrame();
	
	if( 1 == m_mode )
	{
		pDM->MakeInsertFrame( _T("ESL_TEST") );	
		pDM->SetCellData( alias, 7, data, -1 );
	}
	else
		pDM->MakeUpdateFrame( _T("ESL_TEST"), _T("ID"), _T("STRING"), data[ 0 ], m_checkedRow[ m_index ] );

	ControlDisplay( _T("CM_MAIN_SAMPLE"), _T("MainGrid") );
	
	CDialog::OnOK();
}

CString CWORKSHEET_SAMPLE_02::TakeControlData(CString alias, INT option)
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MAIN_SAMPLE_INSERT"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MAIN_SAMPLE_INSERT"), _T("K2Up") );
		return _T("");
	}
	
	// 코드값이 _T("0") 이거나 설명값이 _T("적용안함")일때 _T("")를 반환한다. 
	switch( option )
	{
		case 0 :
			if( _T("0") == result )
				return _T("");
			break;
		case 1 :
			if( _T("적용안함") == result )
				return _T("");
			break;
		default :
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_MAIN_SAMPLE_INSERT"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

VOID CWORKSHEET_SAMPLE_02::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::DestroyWindow();
}

VOID CWORKSHEET_SAMPLE_02::ShowControls()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MAIN_SAMPLE") );
	CString alias[ 7 ] = { _T("ID"), _T("PASSWORD"), _T("NAME"), _T("ADDRESS"), _T("TEL"), _T("BIRTHDAY"), _T("GRADE") };
	CString data[ 7 ];
	pDM->GetCellData( alias, 7, data, m_checkedRow[ m_index ] );

	for( INT i = 0; i < 7; i++ )
		SendControlData( alias[ i ], data[ i ] );
}

VOID CWORKSHEET_SAMPLE_02::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	if( !m_index )
	{
		AfxMessageBox( _T("맨 처음 목록입니다.") );
		return;
	}

	m_index--;
	ShowControls();
}

VOID CWORKSHEET_SAMPLE_02::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	if( m_cnt - 1 == m_index )
	{
		AfxMessageBox( _T("마지막 목록입니다.") );
		return;
	}

	m_index++;
	ShowControls();
}

INT CWORKSHEET_SAMPLE_02::SendControlData(CString alias, CString data)
{
	SetControlData( _T("CM_MAIN_SAMPLE_INSERT"), alias, data, -1, 0 );

	return 0;
}

VOID CWORKSHEET_SAMPLE_02::OnWorksheet() 
{
	// TODO: Add your control notification handler code here
	
	OnWorksheetInput();
}

VOID CWORKSHEET_SAMPLE_02::OnWorksheetInput()
{
	CWORKSHEET_SAMPLE_04 dlg( this );
	dlg.m_mode = m_mode;	// 입력
	dlg.m_workSheetInitFlag = 2;

	dlg.DoModal();
}


HBRUSH CWORKSHEET_SAMPLE_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
