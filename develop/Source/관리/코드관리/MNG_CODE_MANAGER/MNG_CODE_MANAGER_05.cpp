// MNG_CODE_MANAGER_05.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_MANAGER_05.h"
#include "MNG_CODE_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_05 dialog


CMNG_CODE_MANAGER_05::CMNG_CODE_MANAGER_05(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_MANAGER_05)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bModify = false;
}

BOOL CMNG_CODE_MANAGER_05::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	


VOID CMNG_CODE_MANAGER_05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_MANAGER_05)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_MANAGER_05, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_MANAGER_05)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_05 message handlers

BOOL CMNG_CODE_MANAGER_05::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_MANAGER_05") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_04") );
		return false;
	}

	// 2005.09.13 ADD BY PDJ
	// 이용자구분코드는 7자리가 MAX
	// 예외처리 들어감.	
	// 2005.10.25 UPD BY PDW(Jr)
	// 8자리로 늘림
	if ( m_codeClass == 69 )
	{
		( (CEdit*) GetDlgItem (IDC_EDIT1) )->SetLimitText(8);
	}

	m_index = -1;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_CODE_MANAGER_05::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	switch( m_type )
	{
		case 1 :
			SetWindowText( _T("코드 입력") );

			CButton *pBtn;
			pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
			pBtn->ShowWindow( false );
			pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
			pBtn->ShowWindow( false );

			break;
		case 2 :
			SetWindowText( _T("코드 수정") );
			DisplayModifyData( 0 );
			break;
		default :
			break;
	}
}

VOID CMNG_CODE_MANAGER_05::OnOK() 
{
	// TODO: Add extra validation here
	
	CString code, desc;
	switch( GetCodeData( code, desc ) )
	{
		case 1 :
			if( _T("적용안함") != desc )
			{
				AfxMessageBox( _T("코드를 입력하여 주십시오") );
				return;
			}
			break;
		case 2 :
			AfxMessageBox( _T("코드 설명값을 입력하여 주십시오") );
			return;
		case 3 :
			return;
		case 4 :
			return;
		default :
			break;
	}

	CString data[ 20 ];
	data[ 0 ] = code;
	data[ 1 ] = desc;
	
	CMNG_CODE_MANAGER_01 *pParent = ( CMNG_CODE_MANAGER_01 * )pParentMgr;
	
	INT ids;
	ids = pParent->SaveCodeData( data, m_index );
	if( ids < 0 )	
	{
		AfxMessageBox( _T("코드 입력에 실패하였습니다.") );
		return;
	}
	else if ( ids == 100 )
	{
		AfxMessageBox( _T("코드가 중복되었습니다."));
		return;
	}
	
	m_bModify = true;
	switch( m_type )
	{
		case 1 :
			AfxMessageBox( _T("코드가 저장되었습니다.") );
			CDialog::OnOK();
			break;
		case 2 :
			AfxMessageBox( _T("코드가 수정되었습니다.") );
	}
	
	//CDialog::OnOK();
}

VOID CMNG_CODE_MANAGER_05::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

INT CMNG_CODE_MANAGER_05::GetCodeData( CString &code, CString &desc )
{
	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->GetWindowText( code );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->GetWindowText( desc );
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분코드일경우 코드자릿수를 6자 이하로 제한한다.
// 	if( code.GetLength() > 12 )
// 	{
// 		AfxMessageBox( _T("코드는 12자리로 자리 제한을 합니다.") );
// 		return 3;
// 	}
	if( m_codeClass == 4 )
	{
		if( code.GetLength() > 6 )
		{
			AfxMessageBox( _T("코드는 6자리로 자리 제한을 합니다.") );
			return 3;
		}
	}
	else
	{
		if( code.GetLength() > 12 )
		{
			AfxMessageBox( _T("코드는 6자리로 자리 제한을 합니다.") );
			return 3;
		}
	}
	//=====================================================
	

	if( desc.GetLength() > 255 )
	{
		AfxMessageBox( _T("설명은 255자리로 자리 제한을 합니다.") );
		return 3;		
	}

	if( !code.GetLength() )
		return 1;

	if( !desc.GetLength() )
		return 2;

	// 2005.10.24 ADD BY PDW(Jr) ///////////////////////////////////////////
	// 이용자번호구분 코드는 중복될수 없다.
	////////////////////////////////////////////////////////////////////////
	if (m_codeClass == 69)
	{
		CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
		INT cnt = pDM->GetRowCount();
		CString sTemp = _T("");

		for (INT i = 0;i < cnt;i++)
		{
			if ( i == m_index ) continue;

			pDM->GetCellData(_T("C_CODE"), i, sTemp);
			if ( code.Compare(sTemp) == 0 )
			{
				if (m_type == 1)			// 입력
					AfxMessageBox(_T("이용자번호구분 코드는 중복 입력할 수 없습니다."));
				else if (m_type == 2)		// 수정
					AfxMessageBox(_T("사용중인 이용자번호구분 코드 입니다."));

				return 3;
			}
		}
	}

	//++2008.12.09 ADD BY PWR {{++
	// 대출직급정보의 코드와 설명은 중복될수 없다.
	if (m_codeClass == 31)
	{
		CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
		INT cnt = pDM->GetRowCount();
		CString sCode = _T("");
		CString sDescription = _T("");

		for (INT i = 0; i < cnt; i++)
		{
			if ( i == m_index ) continue;

			pDM->GetCellData(_T("C_CODE"), i, sCode);
			pDM->GetCellData(_T("C_DESCRIPTION"), i, sDescription);

			if ( code == sCode )
			{
				if (m_type == 1)			// 입력
					AfxMessageBox(_T("대출직급정보의 코드는 중복 입력할 수 없습니다."));
				else if (m_type == 2)		// 수정
					AfxMessageBox(_T("사용중인 대출직급정보 코드 입니다."));

				return 3;
			}

			if ( desc == sDescription )
			{
				if (m_type == 1)			// 입력
					AfxMessageBox(_T("대출직급정보의 설명은 중복 입력할 수 없습니다."));
				else if (m_type == 2)		// 수정
					AfxMessageBox(_T("사용중인 대출직급정보 설명 입니다."));

				return 3;
			}
		}
	}
	//--2008.12.09 ADD BY PWR --}}

	return 0;
}

// 이전
VOID CMNG_CODE_MANAGER_05::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	DisplayModifyData( -1 );
	
}

// 다음
VOID CMNG_CODE_MANAGER_05::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	DisplayModifyData( 1 );
}


VOID CMNG_CODE_MANAGER_05::DisplayModifyData(INT flag)
{
	switch( flag )	
	{
		case 0 :

			SelectMakeList( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );
			m_index = SelectGetHeadPosition( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );

			if( m_index < 0 )
			{
				AfxMessageBox( _T("선택된 코드가 없습니다.") );
				PostMessage( WM_CLOSE );
				return;
			}

			m_idxCnt = GetSelectedRowCount();

			break;

		case -1 :

			m_index = SelectGetPrev( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );

			if( -1 == m_index )
			{
				m_index = SelectGetHeadPosition( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );
				AfxMessageBox( _T(" 맨 처음 코드입니다.") );
				return;
			}

			break;

		case 1 :

			m_index = SelectGetNext( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );
			if( -1 == m_index )
			{
				m_index = SelectGetTailPosition( _T("CM_MNG_CODE_MANAGER_01"), _T("그리드") );
				AfxMessageBox( _T("마지막 코드입니다.") );
				return;
			}

			break;

		default :

			break;
	}

	AllControlDisplay( _T("CM_MNG_CODE_MANAGER_05"), m_index );
}

INT CMNG_CODE_MANAGER_05::GetSelectedRowCount()
{
	CESL_ControlMgr *pCM = ( CESL_ControlMgr * )FindCM( _T("CM_MNG_CODE_MANAGER_01") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( _T("그리드") );

	return pGrid->SelectGetCount();
}
