// CatCodeGetterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "CatCodeGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatCodeGetterDlg dialog


CCatCodeGetterDlg::CCatCodeGetterDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatCodeGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CCatCodeGetterDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CCatCodeGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatCodeGetterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCatCodeGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CCatCodeGetterDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatCodeGetterDlg message handlers

BOOL CCatCodeGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("CatCodeGetterDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : CatCodeGetterDlg") );
		return false;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCatCodeGetterDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}

VOID CCatCodeGetterDlg::OnbFIND() 
{

	CString condition[ 2 ];
	if( GetSearchCondition( condition ) )
		return;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_CatCodeGetterDlg") );
	pDM->FreeData();
	if( SearchCatCode( MakeFilePath( condition[ 0 ] ), condition[ 1 ], pDM ) )
	{
		AfxMessageBox( _T("파일을 찾을 수 없습니다.") );
		return;
	}
	else
		ShowGrid();
}

INT CCatCodeGetterDlg::GetSearchCondition( CString condition[] )
{
	GetControlData( _T("CM_CatCodeGetterDlg"), _T("저자기호"), condition[ 0 ], -1, 1 );

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eAUTHOR_CCG );
	pEdt->GetWindowText( condition[ 1 ] );

	if( ConvertCondition( condition[ 1 ] ) )
	{
		AfxMessageBox( _T("저작자를 올바른 형태로 입력하십시오") );
		return -1;
	}
	
	return 0;
}

CString CCatCodeGetterDlg::MakeFilePath( CString condition )
{
	CString path;
	path.Format( _T("..\\cfg\\%s.txt"), condition );

	return path;
}	

INT CCatCodeGetterDlg::SearchCatCode( CString path, CString condition, CESL_DataMgr *pDM )
{
	CStdioFile fp;
	if( !fp.Open( path, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary ) )
		return -1;
		
			TCHAR cUni;
			fp.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				fp.SeekToBegin();
			}

	CString alias[ 2 ] = { _T("AUTHOR"), _T("SIGN") };
	CString data[ 2 ];
	
	CString row;
	INT locate = 0;
	INT cnt = 0;
	INT ids = 0;

	CString strPrevCode;
	while( fp.ReadString( row ) )
	{
		locate = row.Find( _T("|"), 0 );
		if( locate > 0 )
		{
			data[ 0 ] = row.Left( locate );
			data[ 0 ].Replace( _T(" "), _T("") );

			data[ 1 ] = row.Right( row.GetLength() - locate );
			data[ 1 ].Replace( _T(" "), _T("") );
			data[ 1 ].Replace( _T("|"), _T("") );

			ids = _tcscmp(condition, data[ 0 ]);
			if (ids == 0)
			{
				data[ 0 ] = m_author;
				pDM->SetCellData( alias, 2, data, -1 );
				break;
			}
			else if (ids < 0)
			{
				data[ 0 ] = m_author;
				if (!strPrevCode.IsEmpty())
					data[1] = strPrevCode;
				pDM->SetCellData( alias, 2, data, -1 );
				break;
			}

			strPrevCode = data[1];
		}
	}

	fp.Close();
	
	return 0;	
}

VOID CCatCodeGetterDlg::ShowGrid()
{
	ControlDisplay( _T("CM_CatCodeGetterDlg"), _T("그리드") );
}

INT CCatCodeGetterDlg::ConvertCondition( CString &condition )
{
	RemoveEndglish( condition );
	m_author = condition;
	
	return 0;

	INT length = condition.GetLength();
	if( length < 3 )
		return -1;

	condition = condition.Right( length - 2 );
	if( _T(",") == condition.Left( 1 ) )
		condition = condition.Left( 3 );
	else
		condition = condition.Left( 2 );

	return 0;
}

VOID CCatCodeGetterDlg::RemoveEndglish( CString &string )
{
	string.Replace( _T(" "), _T("") );
	
	CString tmp;
	for( INT i = 33; i < 127; i++ )
		if( 44 == i )
			continue;
		else
		{
			tmp.Format( _T("%c"), i );
			string.Replace( tmp, _T("") );
		}
}
