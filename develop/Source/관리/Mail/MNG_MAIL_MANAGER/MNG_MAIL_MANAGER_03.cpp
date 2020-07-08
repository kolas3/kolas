// MNG_MAIL_MANAGER_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MAIL_MANAGER_03.h"
#include "MNG_MAIL_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MAIL_MANAGER_03 dialog


CMNG_MAIL_MANAGER_03::CMNG_MAIL_MANAGER_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MAIL_MANAGER_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_MAIL_MANAGER_03::~CMNG_MAIL_MANAGER_03()
{
	
}
BOOL CMNG_MAIL_MANAGER_03::Create(CWnd* pParentWnd)
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

	EFS_END
	return FALSE;

}	
	
VOID CMNG_MAIL_MANAGER_03::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MAIL_MANAGER_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMNG_MAIL_MANAGER_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_MAIL_MANAGER_03)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MAIL_MANAGER_03 message handlers

BOOL CMNG_MAIL_MANAGER_03::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	/////////////////////////////////////////////////////////////////////
	// 2005.10.11 ADD BY PDW(Jr)
	// 메일타입 정보를 가진 SM을 초기화 한다.
	/////////////////////////////////////////////////////////////////////
	if( InitESL_Mgr( _T("SM_BO_LOC_3800_MailGroup") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : CMNG_MAIL_MANAGER_03") );
		return false;
	}

	InitSetControls();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMNG_MAIL_MANAGER_03::OnClose() 
{
	EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	
	CDialog::DestroyWindow();

	EFS_END
}


VOID CMNG_MAIL_MANAGER_03::OnOK() 
{
	EFS_BEGIN

	// TODO: Add extra validation here
	
	const INT ctrlCnt = 2;
	CString ctrlData[ ctrlCnt ];
	GetCtrlData( ctrlData );
	
	CMNG_MAIL_MANAGER_01 *pParent = ( CMNG_MAIL_MANAGER_01 * )pParentMgr;
	pParent->SearchData( MakeSelectQuery( ctrlData ) );

	// CDialog::OnOK();

	EFS_END
}

VOID CMNG_MAIL_MANAGER_03::InitSetControls()
{
	EFS_BEGIN

	CComboBox *pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	CString list[ 4 ] = { _T("☞ 선택"), _T("수서"), _T("열람"), _T("정리") };
	for( INT i = 0; i < 4; i++ )
		pCombo->InsertString( i, list[ i ] );

	/////////////////////////////////////////////////////////////////////
	// 2005.10.11 ADD BY PDW(Jr)
	// 기본정보 이외의 메일 TYPE를 추가한다.
	/////////////////////////////////////////////////////////////////////
	// 1. 메일 타입 DM을 초기화 한다.
	/////////////////////////////////////////////////////////////////////
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3800_MailGroup") );
	if (pDM == NULL)
	{
		CString error;
		error.Format(_T(" CMNG_MAIL_MANAGER_03::InitSetControls() \r\n'")
					 _T(" FindDM( DM_BO_LOC_3820_MailGroup ) :: error"));
		AfxMessageBox(error);
		return;
	}
	
	pDM->RefreshDataManager( _T("") );
	
	INT ids=0, cnt=0;
	
	cnt = pDM->GetRowCount();
	if (cnt < 1)
	{
		AfxMessageBox(_T("메일 그룹정보가 없습니다"));
		return;
	}

	/////////////////////////////////////////////////////////////////////
	// 2. 콤보박스에 없는 DM 정보 추가
	/////////////////////////////////////////////////////////////////////
	INT j = 0;

	for(i = 0;i < cnt;i++)
	{
		CString sTemp,sTemp2;
		pDM->GetCellData(_T("메일타입"), i, sTemp);
		
		INT row = pCombo->GetCount();
		for (j = 0;j < row;j++)
		{
			pCombo->GetLBText(j,sTemp2);
			if (sTemp == sTemp2)
				break;
		}

		if (j == row)
			pCombo->InsertString(row, sTemp);
	}

	pCombo->SetCurSel( 0 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	pCombo->EnableWindow( false );

	EFS_END
}

CString CMNG_MAIL_MANAGER_03::GetWorkGroup()
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );

	/////////////////////////////////////////////////////////////
	// 2005.10.11 UPD BY PDW(Jr)
	// 콤보박스의 내용을 리턴하도록 변경
	/////////////////////////////////////////////////////////////
	// 수정전
	/*
	switch( pCombo->GetCurSel() )
	{
		case 1 :
			return _T("수서");
			break;
		case 2 :
			return _T("열람");
			break;
		case 3 :
			return _T("정리");
			break;
		default :
			return _T("");
			break;
	}
	*/
	/////////////////////////////////////////////////////////////
	// 수정후
	CString sTemp;
	pCombo->GetWindowText(sTemp);
	
	if (sTemp == _T("☞ 선택"))
		return _T("");
	else
		return sTemp;

	EFS_END
	return _T("");

}

VOID CMNG_MAIL_MANAGER_03::ViewMngList( CString group )
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	pCombo->ResetContent();
	
	CMNG_MAIL_MANAGER_01 *pParent = ( CMNG_MAIL_MANAGER_01 * )pParentMgr;
	
	INT cnt = 0;
	CString *list = pParent->MakeMngGroup( group, cnt );
	if( !cnt )
	{
		pCombo->EnableWindow( false );
		return;
	}
		
	pCombo->InsertString( 0, _T("☞ 선택") );
	for( INT i = 0; i < cnt; i++ )
		pCombo->InsertString( i + 1, list[ i ] );
	pCombo->SetCurSel( 0 );
	pCombo->EnableWindow( true );
	
	delete [] list;

	EFS_END
}

VOID CMNG_MAIL_MANAGER_03::OnSelchangeCombo1() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	CString group = GetWorkGroup();
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );

	if( !group.GetLength() )
	{
		pCombo->ResetContent();
		pCombo->EnableWindow( false );
	}
	else
		ViewMngList( group );

	EFS_END
}

VOID CMNG_MAIL_MANAGER_03::GetCtrlData( CString data[] )
{
	EFS_BEGIN

	data[ 0 ] = GetWorkGroup();

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	if( pCombo->GetCurSel() )
		pCombo->GetWindowText( data[ 1 ] );
	else
		data[ 1 ].Empty();

	EFS_END
}

CString CMNG_MAIL_MANAGER_03::MakeSelectQuery( CString data[] )
{
	EFS_BEGIN

	CString query, tmp;

	if( data[ 0 ].GetLength() )
	{
		tmp.Format( _T(" G.M_TYPE = '%s' AND"), data[ 0 ] );
		query += tmp;
	}

	if( data[ 1 ].GetLength() )
	{
		tmp.Format( _T(" G.MNG_GROUP = '%s'"), data[ 1 ] );
		query += tmp;
	}

	if( _T("AND") == query.Right( 3 ) )
	{
		tmp = query.Left( query.GetLength() - 3 );
		query = tmp;
	}

	return query;

	EFS_END
	return _T("");

}

BOOL CMNG_MAIL_MANAGER_03::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) 
		{
			OnOK();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CMNG_MAIL_MANAGER_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
