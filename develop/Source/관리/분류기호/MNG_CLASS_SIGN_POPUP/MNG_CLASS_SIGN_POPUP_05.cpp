// MNG_CLASS_SIGN_POPUP_05.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_CLASS_SIGN_POPUP_05.h"
#include "MNG_CLASS_SIGN_POPUP_01.h"

#include "..\MNG_CLASS_SIGN_MANAGER\CLASS_SIGN_MANAGER_04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define T_KDC 0
#define T_DDC 1
#define T_IDX 2



/////////////////////////////////////////////////////////////////////////////
// CMNG_CLASS_SIGN_POPUP_05 dialog


CMNG_CLASS_SIGN_POPUP_05::CMNG_CLASS_SIGN_POPUP_05(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CLASS_SIGN_POPUP_05)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_SMName = _T("MNG_CCLASS_SIGN_MANAGER_03");
	m_CMName = _T("CM_MNG_CLASS_SIGN_POPUP_01");
	m_DMName_IDX = _T("DM_MNG_CLASS_SIGN_POPUP_01_IDX");
	m_DMName_DDC = _T("DM_MNG_CLASS_SIGN_POPUP_01_DDC");
	m_DMName_KDC = _T("DM_MNG_CLASS_SIGN_POPUP_01_KDC");
}
CMNG_CLASS_SIGN_POPUP_05::~CMNG_CLASS_SIGN_POPUP_05()
{
	
}

VOID CMNG_CLASS_SIGN_POPUP_05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CLASS_SIGN_POPUP_05)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CLASS_SIGN_POPUP_05, CDialog)
	//{{AFX_MSG_MAP(CMNG_CLASS_SIGN_POPUP_05)
	ON_BN_CLICKED(IDC_bClose, OnbClose)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bClassSignHelp, OnbClassSignHelp)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CLASS_SIGN_POPUP_05 message handlers

VOID CMNG_CLASS_SIGN_POPUP_05::OnbClose() 
{
	CDialog::OnCancel();	
}

BOOL CMNG_CLASS_SIGN_POPUP_05::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( m_SMName ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CCLASS_SIGN_MANAGER_03") );
		return false;
	}

	m_checkedRow = NULL;
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_CLASS_SIGN_POPUP_05::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	CMNG_CLASS_SIGN_POPUP_01 *pParent = ( CMNG_CLASS_SIGN_POPUP_01 * )pParentMgr;
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

		SetReverse(m_CMName, _T("그리드_3"), m_checkedRow[m_index]);
			
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

VOID CMNG_CLASS_SIGN_POPUP_05::OnbBEFORE() 
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

VOID CMNG_CLASS_SIGN_POPUP_05::OnbNEXT() 
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

VOID CMNG_CLASS_SIGN_POPUP_05::OnbClassSignHelp() 
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
VOID CMNG_CLASS_SIGN_POPUP_05::ShowContolIDXData()
{
	CESL_DataMgr *pDM = FindDM( m_DMName_IDX );
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

	SetReverse(m_CMName, _T("그리드_3"), m_checkedRow[m_index]);

}

INT CMNG_CLASS_SIGN_POPUP_05::InitSetEX()
{
	CMNG_CLASS_SIGN_POPUP_01 *pParent = ( CMNG_CLASS_SIGN_POPUP_01 * )pParentMgr;
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

INT CMNG_CLASS_SIGN_POPUP_05::InitSetIDX()
{
	CMNG_CLASS_SIGN_POPUP_01 *pParent = ( CMNG_CLASS_SIGN_POPUP_01 * )pParentMgr;
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

VOID CMNG_CLASS_SIGN_POPUP_05::ShowControlData()
{
	CESL_DataMgr *pDM=NULL;
	switch( m_type )
	{
		case T_KDC :
			pDM = FindDM( m_DMName_KDC );
			SetReverse(m_CMName, _T("그리드_1"), m_checkedRow[m_index]);
			break;
		case T_DDC :
			pDM = FindDM( m_DMName_DDC );
			SetReverse(m_CMName, _T("그리드_2"), m_checkedRow[m_index]);
			break;
		default :
			break;
	}

	if ( NULL == pDM )
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return;
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



HBRUSH CMNG_CLASS_SIGN_POPUP_05::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
