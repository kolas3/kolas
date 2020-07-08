// BO_LOC_21010.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_21001.h"
#include "BO_LOC_21010.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_21010 dialog


CBO_LOC_21010::CBO_LOC_21010(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_21010)
	m_nAscDesc = 0;
	//}}AFX_DATA_INIT
}


void CBO_LOC_21010::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_21010)
	DDX_Control(pDX, IDC_LIST2, m_lb_Select);
	DDX_Control(pDX, IDC_LIST1, m_lb_Support);
	DDX_Radio(pDX, IDC_RADIO1, m_nAscDesc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_21010, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_21010)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_21010 message handlers


BOOL CBO_LOC_21010::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	m_nAscDesc = 0;

	if( m_pDM != NULL )			
		RegSupport_DM();
	else 	if( m_pGrid != NULL )	
		RegSupport_Grid();

	LoadRecentState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void	CBO_LOC_21010::ClearFilter()
{
	m_arCol_Support_Filter.RemoveAll();
}

void	CBO_LOC_21010::AddFilter( CString str )
{
	m_arCol_Support_Filter.Add( str );
}

void	CBO_LOC_21010::SetGrid(CESL_Grid*	p)
{
	m_pDM = NULL;
	m_pGrid = p;
}

void	CBO_LOC_21010::SetDM(CESL_DataMgr*	p , CESL_Mgr*	pEslMgr , CString	strViewCM  )
{
	m_pDM = p;
	m_pGrid = NULL;

	cblMapData_Mem.m_pEslMgr_Sort = pEslMgr;
	cblMapData_Mem.m_strSort_ViewCM = strViewCM;

}

void	CBO_LOC_21010::RegSupport_Grid()
{
	POSITION pos;
	for( pos = m_pGrid->m_arrColumnInfo.GetHeadPosition() ; pos != NULL ;  )
	{
		CESL_GridColumnInfo* pCol = (CESL_GridColumnInfo*)m_pGrid->m_arrColumnInfo.GetNext(pos);
		AddSupport( pCol->strColumnDMFieldAlias );
	}


	InitSupport();
}

void	CBO_LOC_21010::RegSupport_DM()
{
	int c_alias = 0;
	c_alias = m_pDM->GetColCount();

	if( c_alias < 0 )
	{
		c_alias = m_pDM->RefList.GetCount();
	}

	if( c_alias < 1 ) 
		return;

	CString* alias_str = new CString[c_alias];
	m_pDM->GetAllAlias(alias_str, c_alias);
	for( int i = 0 ; i < c_alias ; i++ )
	{
		AddSupport( alias_str[i] );
	}	
	InitSupport();

	delete[] alias_str;
}

void	CBO_LOC_21010::InitSupport()
{
	int i = 0 ;

	m_lb_Support.ResetContent();
	m_lb_Select.ResetContent();

	for( i = 0 ; i < m_arCol_Support.GetSize() ; i++ )
	{
		CString str = m_arCol_Support[i];
		m_lb_Support.AddString( str );
	}

	UpdateData(FALSE);
}

int		CBO_LOC_21010::AddSupport( CString str )
{
	if( m_arCol_Support_Filter.GetSize() < 1 )
	{
		m_arCol_Support.Add(str);
		return 1;
	}

	int i = 0;
	for( i = 0 ; i < m_arCol_Support_Filter.GetSize() ; i++ )
	{
		if( m_arCol_Support_Filter[i] == str )
		{
			m_arCol_Support.Add(str);
			return 1;
		}
	}
	return 0;
}

void	CBO_LOC_21010::AddSelect()
{
	int idx =	m_lb_Support.GetCurSel();
	if( idx < 0 ) return;

	CString str;
	m_lb_Support.GetText( idx , str );
	m_lb_Support.DeleteString( idx );
	m_lb_Select.AddString( str );
}

void	CBO_LOC_21010::DelSelect()
{
	int idx =	m_lb_Select.GetCurSel();
	if( idx < 0 ) return;

	CString str;
	m_lb_Select.GetText( idx , str );
	m_lb_Select.DeleteString( idx );
	m_lb_Support.AddString( str );
}

void	CBO_LOC_21010::UpDownSelect( int updown )
{
	int idx =	m_lb_Select.GetCurSel();
	if( idx < 0 ) return; 

	if( updown == -1 && idx == 0 ) return; 
	if( updown ==  1 && idx == m_lb_Select.GetCount()-1 ) return; 

	CString str;
	m_lb_Select.GetText( idx , str );

	if( updown == -1 )
	{
		m_lb_Select.InsertString( idx + updown , str );
		m_lb_Select.DeleteString( idx + 1 );
		m_lb_Select.SetCurSel( idx + updown );
	}
	else if( updown == 1 )
	{
		m_lb_Select.DeleteString( idx );
		m_lb_Select.InsertString( idx + updown , str );
		m_lb_Select.SetCurSel( idx + updown );
	}
}

void	CBO_LOC_21010::PrcSelectString()
{
	m_arCol_Select.RemoveAll();
	int i = 0;
	for( i = 0 ; i < m_lb_Select.GetCount() ; i++ )
	{
		CString str;
		m_lb_Select.GetText( i , str );
		m_arCol_Select.Add(str);
	}
}

void	CBO_LOC_21010::StartSort()
{
	UpdateData(TRUE);

	PrcSelectString();

	if( m_arCol_Support.GetSize() < 1 ) 
	{
		AfxMessageBox( _T("지원항목이 없습니다.") );
		return;
	}

	if( m_arCol_Select.GetSize() < 1 ) 
	{
		AfxMessageBox( _T("선택항목이 없습니다.") );
		return;
	}

	cblMapData_Mem.SetSort_HeaderAlias( m_arCol_Support );
	cblMapData_Mem.SetSort_SortAlias( m_arCol_Select , m_nAscDesc );
	cblMapData_Mem.StartSort( m_pDM , m_pGrid );
}


void	CBO_LOC_21010::SetFunc_FP_MultiSort_UserCompear( FP_MultiSort_UserCompear p )
{
	cblMapData_Mem.SetFunc_FP_MultiSort_UserCompear(p);
}

void CBO_LOC_21010::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	AddSelect();
}

void CBO_LOC_21010::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	DelSelect();
}

void CBO_LOC_21010::OnButtonUp() 
{
	// TODO: Add your control notification handler code here
	UpDownSelect( -1 );
}

void CBO_LOC_21010::OnButtonDown() 
{
	// TODO: Add your control notification handler code here
	UpDownSelect( 1 );
}

void CBO_LOC_21010::OnOK() 
{
	// TODO: Add extra validation here
	
	StartSort();
}

void CBO_LOC_21010::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	SaveRecentState();

	CDialog::OnCancel();
}


void CBO_LOC_21010::SaveRecentState()
{
	UpdateData(TRUE);

	CStdioFile* pFile = _OpenFileWrite_UniText( m_cstr_RecentFile );
	if( pFile == NULL )
	{
		return;
	}
	CString cstr;
	cstr.Format( _T("%d\r\n") , m_nAscDesc );
	pFile->WriteString( cstr );

	int i=0;
	for( i = 0 ; i < m_lb_Select.GetCount() ; i++ )
	{
		m_lb_Select.GetText( i , cstr );

		cstr += _T("\n");

		pFile->WriteString( cstr );
	}
	pFile->Close();
	delete pFile;
}

void CBO_LOC_21010::LoadRecentState()
{
	CStdioFile* pFile = _OpenFileRead_UniText( m_cstr_RecentFile );
	if( pFile == NULL )
	{
		return;
	}

	int idx=0;
	CString cstr;

	pFile->ReadString(cstr);

	m_nAscDesc = _ttoi( cstr.GetBuffer(0) );

	while( pFile->ReadString(cstr) )
	{
		idx = m_lb_Support.FindString( 0 , cstr );
		if( idx > -1 )
		{
			m_lb_Support.DeleteString( idx );
		}
		m_lb_Select.AddString( cstr );
	}

	pFile->Close();
	delete pFile;


	UpdateData(FALSE);
}

