// CLASS_SIGN_MANAGER_03.cpp : implementation file
//

#include "stdafx.h"
#include "CLASS_SIGN_MANAGER_03.h"
#include "CLASS_SIGN_MANAGER_01.h"
#include "CLASS_SIGN_MANAGER_04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define T_KDC 0
#define T_DDC 1
#define T_IDX 2

/////////////////////////////////////////////////////////////////////////////
// CCLASS_SIGN_MANAGER_03 dialog


CCLASS_SIGN_MANAGER_03::CCLASS_SIGN_MANAGER_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCLASS_SIGN_MANAGER_03)
	m_idx_KDCDDC = 0;
	//}}AFX_DATA_INIT
}

BOOL CCLASS_SIGN_MANAGER_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CCLASS_SIGN_MANAGER_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLASS_SIGN_MANAGER_03)
	DDX_Radio(pDX, IDC_rKDC, m_idx_KDCDDC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCLASS_SIGN_MANAGER_03, CDialog)
	//{{AFX_MSG_MAP(CCLASS_SIGN_MANAGER_03)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bClassSignHelp, OnbClassSignHelp)
	ON_BN_CLICKED(IDC_rKDC, OnrKDC)
	ON_BN_CLICKED(IDC_rDDC, OnrDDC)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLASS_SIGN_MANAGER_03 message handlers

BOOL CCLASS_SIGN_MANAGER_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CCLASS_SIGN_MANAGER_03") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CCLASS_SIGN_MANAGER_03") );
		return false;
	}

	m_checkedRow = NULL;
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CCLASS_SIGN_MANAGER_03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	CCLASS_SIGN_MANAGER_01 *pParent = ( CCLASS_SIGN_MANAGER_01 * )pParentMgr;
	m_type = pParent->m_type;
	

	INT ids;
	if( T_IDX == m_type )
	{
		ids = InitSetIDX();
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2004.06.10 김소정 추가
		GetDlgItem(IDC_rKDC)->EnableWindow(TRUE);
		GetDlgItem(IDC_rDDC)->EnableWindow(TRUE);

		if ( m_cnt_1<=0 ) m_idx_KDCDDC = 1;	// 색인어에서 KDC:0,  DDC:1
		else m_idx_KDCDDC = 0;	
		UpdateData(FALSE);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		SetReverse(_T("CM_CCLASS_SIGN_MANAGER_01"), _T("그리드_3"), m_checkedRow[m_index]);
			
	}
	else
	{
		ids = InitSetEX();
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2004.06.10 김소정 추가
		GetDlgItem(IDC_rKDC)->EnableWindow(FALSE);
		GetDlgItem(IDC_rDDC)->EnableWindow(FALSE);

		if ( T_KDC == m_type ) m_idx_KDCDDC = 0;	// 색인어에서 KDC:0,  DDC:1
		else m_idx_KDCDDC = 1;	
		UpdateData(FALSE);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}
	
	if( -1 == ids )
		PostMessage( WM_CLOSE );

	
}

VOID CCLASS_SIGN_MANAGER_03::OnbBEFORE() 
{
EFS_BEGIN
	m_index--;
	if( -1 == m_index )
	{
		AfxMessageBox( _T("맨처음 자료입니다.") );
		m_index++;
		return;
	}
	
	if( T_IDX == m_type )	//IDX
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2004.06.10 김소정 수정
		// DDC이면 KDC전까지만 이동한다.
		if( (m_idx_KDCDDC ==  1) && ((m_cnt_1-1) == m_index) )
		{
			AfxMessageBox( _T("맨처음 자료입니다.") );
			m_index++;
			return;
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		ShowContolIDXData();
		
	}	
	else
	{
		ShowControlData();
		
	}


EFS_END
}

VOID CCLASS_SIGN_MANAGER_03::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	m_index++;
	if( m_rowCnt == m_index )
	{
		AfxMessageBox( _T("마지막 자료입니다.") );
		m_index--;
		return;
	}

	if( T_IDX == m_type )	// IDX
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2004.06.10 김소정 수정
		// KDC이면 DDC전까지만 이동한다.
		if( (m_idx_KDCDDC ==  0) && (m_cnt_1 == m_index) )
		{
			AfxMessageBox( _T("마지막 자료입니다.") );
			m_index--;
			return;
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		ShowContolIDXData();
		
	}
	else
	{
		ShowControlData();
	}
}

VOID CCLASS_SIGN_MANAGER_03::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	if( m_checkedRow )
		free( m_checkedRow );
	m_checkedRow = NULL;
	
	CDialog::DestroyWindow();
}

INT CCLASS_SIGN_MANAGER_03::InitSetIDX()
{
	CCLASS_SIGN_MANAGER_01 *pParent = ( CCLASS_SIGN_MANAGER_01 * )pParentMgr;
	m_checkedRow = pParent->GetCheckedIDXRow( m_cnt_1, m_cnt_2 );

	m_rowCnt = m_cnt_1 + m_cnt_2;
	if( !m_rowCnt )
	{
		AfxMessageBox( _T("선택항목이 없습니다.") );
		return -1;
	}

	m_index = 0;
	ShowContolIDXData();

	return 0;
}

INT CCLASS_SIGN_MANAGER_03::InitSetEX()
{
	CCLASS_SIGN_MANAGER_01 *pParent = ( CCLASS_SIGN_MANAGER_01 * )pParentMgr;
	m_checkedRow = pParent->GetCheckedDMRow( m_rowCnt );
	if( !m_rowCnt )
	{
		AfxMessageBox( _T("선택항목이 없습니다.") );
		return -1;
	}
	
	m_index = 0;
	ShowControlData();
	
	return 0;
}

VOID CCLASS_SIGN_MANAGER_03::ShowContolIDXData()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_IDX") );
	CString sign, desc;

	if( m_index < m_cnt_1 )
	{
		pDM->GetCellData( _T("I_KDC"), m_checkedRow[ m_index ], sign );
		pDM->GetCellData( _T("K_CLASS_DESC"), m_checkedRow[ m_index ], desc );
	}
	else
	{
		pDM->GetCellData( _T("I_DDC"), m_checkedRow[ m_index ], sign );
		pDM->GetCellData( _T("D_CLASS_DESC"), m_checkedRow[ m_index ], desc );
	}

	
	
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->SetWindowText( sign );
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->SetWindowText( desc );

	SetReverse(_T("CM_CCLASS_SIGN_MANAGER_01"), _T("그리드_3"), m_checkedRow[m_index]);

}

VOID CCLASS_SIGN_MANAGER_03::ShowControlData()
{
	CESL_DataMgr *pDM;
	switch( m_type )
	{
		case T_KDC :
			pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_KDC") );
			SetReverse(_T("CM_CCLASS_SIGN_MANAGER_01"), _T("그리드_1"), m_checkedRow[m_index]);
			break;
		case T_DDC :
			pDM = FindDM( _T("DM_CCLASS_SIGN_MANAGER_DDC") );
			SetReverse(_T("CM_CCLASS_SIGN_MANAGER_01"), _T("그리드_2"), m_checkedRow[m_index]);
			break;
		default :
			break;
	}

	CString sign, desc;
	pDM->GetCellData( _T("CLASS_VAL"),  m_checkedRow[ m_index ], sign );
	pDM->GetCellData( _T("CLASS_DESC"), m_checkedRow[ m_index ], desc );

	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->SetWindowText( sign );
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->SetWindowText( desc );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2004.06.10 김소정 추가
VOID CCLASS_SIGN_MANAGER_03::OnbClassSignHelp() 
{
EFS_BEGIN

	CCLASS_SIGN_MANAGER_04 *pDlg = NULL;
	pDlg = new CCLASS_SIGN_MANAGER_04( this );
	pDlg->pMain = this->pMain;
		
	pDlg->m_pInfo = this->m_pInfo;
	
	if( pDlg->Create( this ) )
	{
		pDlg->ShowWindow( SW_SHOW );
		pDlg->UpdateWindow();
		pDlg->CenterWindow();
		return ;
	}
	else
	{
		delete pDlg;
		return ;
	}
EFS_END	
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.10 김소정 추가
VOID CCLASS_SIGN_MANAGER_03::OnrKDC() 
{
EFS_BEGIN
	if( m_cnt_1 == 0 )
	{
		AfxMessageBox( _T("선택항목이 없습니다.") );
		m_idx_KDCDDC = 1;
		UpdateData(FALSE);
		return ;
	}

	UpdateData(TRUE);

	m_index=0;

	ShowContolIDXData();
	
EFS_END	
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.10 김소정 추가
VOID CCLASS_SIGN_MANAGER_03::OnrDDC() 
{
EFS_BEGIN	
	if( m_cnt_2 == 0 )
	{
		AfxMessageBox( _T("선택항목이 없습니다.") );
		m_idx_KDCDDC = 0;
		UpdateData(FALSE);
		return ;
	}
	UpdateData(TRUE);

	m_index = m_cnt_1;

	ShowContolIDXData();
	
EFS_END
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

VOID CCLASS_SIGN_MANAGER_03::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}
