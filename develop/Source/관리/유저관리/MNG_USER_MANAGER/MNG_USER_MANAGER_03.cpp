// MNG_USER_MANAGER_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_USER_MANAGER_03.h"
#include "MNG_USER_MANAGER_01.h"
#include "PermisionCFGDlg.h"
#include "..\..\..\공통\SearchZipCodeAPI\SearchZipCodeDlg.h"
#include "..\MNG_ACCESS_CONTROL\MNG_ACCESS_CONTROL_API.h"

#include "MNG_USER_CHANGEPASSWORD.h"

#include "..\..\..\공통\SearchZipCodeAPI\SearchZipCodeDlgOpenAPI.h"

#include "MNG_USER_MANAGER_03_GPKI_REG.h"
#include "UserRegIPDlg.h"
#include "UserRegTimeoutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_03 dialog


CMNG_USER_MANAGER_03::CMNG_USER_MANAGER_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_MANAGER_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGroupViewDlg = new CMNG_USER_MANAGER_04(this);
	m_pParent = pParent;
	m_modifyList = NULL;
	m_strAuthorityType = _T("");
	m_arrAuthority[0] = _T("");
	m_arrAuthority[1] = _T("");
	m_arrAuthority[2] = _T("");
	m_arrAuthority[3] = _T("");
	m_arrAuthority[4] = _T("");
	m_arrAuthority[5] = _T("");
	m_arrAuthority[6] = _T("");
	m_arrAuthority[7] = _T("");
	m_arrAuthority[8] = _T("");
	m_arrAuthority[9] = _T("");
	m_arrAuthority[10] = _T("");
	m_arrAuthority[11] = _T("");
	m_arrAuthority[12] = _T("");
	m_arrAuthority[13] = _T("");
	m_arrAuthority[14] = _T("");
	m_arrAuthority[15] = _T("");
	m_arrAuthority[16] = _T("");
	m_arrAuthority[17] = _T("");
	m_arrAuthority[18] = _T("");
	m_arrAuthority[19] = _T("");
	m_bRSC = FALSE;
	m_strCryptoPassword = _T("");

	m_strUserUID = _T("");
	m_strUserKey = _T("");
	m_bGPKIDirty = FALSE;
	m_bGPKIUSEDirty = FALSE;
	m_nGPKIMode = 0;

	m_strUserIP = _T("");
	m_strOldUserIP = _T("");
	m_nTimeout = 10;

	m_strUserID = _T("");	
}


CMNG_USER_MANAGER_03::~CMNG_USER_MANAGER_03()
{
	if(m_stOriUserAuthority.pStrMenuPermit)
	{
		delete [] m_stOriUserAuthority.pStrMenuPermit;
		m_stOriUserAuthority.pStrMenuPermit = NULL;
	}
	if(m_stDesUserAuthority.pStrMenuPermit)
	{
		delete [] m_stDesUserAuthority.pStrMenuPermit;
		m_stDesUserAuthority.pStrMenuPermit = NULL;
	}
}


BOOL CMNG_USER_MANAGER_03::Create(CWnd* pParentWnd)
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

	EFS_END
	return FALSE;

}	

VOID CMNG_USER_MANAGER_03::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_MANAGER_03)
	DDX_Control(pDX, IDC_COMBO4, m_codePattern);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMNG_USER_MANAGER_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_MANAGER_03)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bGET_ZIP_CODE_1, OnbGETZIPCODE1)
	ON_BN_CLICKED(IDC_bGET_ZIP_CODE_2, OnbGETZIPCODE2)
	ON_BN_CLICKED(IDC_chkVIEWGROUP, OnchkVIEWGROUP)
	ON_WM_MOVE()
	ON_CBN_SELCHANGE(IDC_COMBO5, OnSelchangeCombo5)
	ON_WM_CTLCOLOR()   
	ON_BN_CLICKED(IDC_BTN_PERMISION, OnBtnPermision)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)
	ON_BN_CLICKED(IDC_btnCHANGEPW, OnbtnCHANGEPW)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)	
	ON_BN_CLICKED(IDC_BUTTON_GPKI, OnButtonGpki)
	ON_BN_CLICKED(IDC_CHECK_SECU_SINGLOG, OnCheckSecuSinglog)
	ON_BN_CLICKED(IDC_CHECK_SECU_TIMEOUT, OnCheckSecuTimeout)
	ON_BN_CLICKED(IDC_chkGPKI, OnchkGPKI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_03 message handlers

BOOL CMNG_USER_MANAGER_03::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();	
	if( InitESL_Mgr( _T("MNG_USER_MANAGER_03") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_MANAGER_03") );
		return false;
	}

	TCHAR userPermition = GetUserPermition();
	CString error;
	
	if( ('A' != userPermition) && ('M' != userPermition) && ('S' != userPermition) && (m_mode == 1) )
	{
		error.Format( _T("%s 님은 사용자 입력 및 삭제에 대한 권한이 없습니다."), GetUserID() );
		AfxMessageBox( error );

		PostMessage( WM_CLOSE );
	}
	else
	{
		m_index = 0;
		MakeMehodList();
		MakeLoanerPermitionCombo();
	}

	
	m_pGroupViewDlg->Create(this);

	SettingScreen();

	((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->InsertColumn(1,_T("번호"),LVCFMT_CENTER,45);
	((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->InsertColumn(1,_T("권한명"),LVCFMT_LEFT ,219);
	((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->SetExtendedStyle(LVS_EX_GRIDLINES );

	CString strValue;
	GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("주민등록번호사용유무"), strValue);
	if (strValue.CompareNoCase(_T("Y")) != 0)
	{
		((CStatic*)GetDlgItem(IDC_staticCIVIL))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staticDASH))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT5))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT6))->ShowWindow(SW_HIDE);
		CheckDlgButton(IDC_chkCIVILNUM, FALSE);
		((CButton*)GetDlgItem(IDC_chkCIVILNUM))->ShowWindow(SW_HIDE);
	}

	if(TRUE == m_pInfo->m_bUserPwEncryption && 1 != m_mode)
	{
		((CStatic*)GetDlgItem(IDC_staPASS))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staPASSC))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT2))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT3))->ShowWindow(SW_HIDE);
		if(2 == m_mode)
		{
			((CButton*)GetDlgItem(IDC_btnCHANGEPW))->ShowWindow(SW_SHOW);
		}
	}

	GetDlgItem(IDC_EDIT16)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHK_USERINFO_EXP)->EnableWindow(FALSE);

	( (CEdit*)GetDlgItem(IDC_EDIT2) )->SetLimitText(20);
	( (CEdit*)GetDlgItem(IDC_EDIT3) )->SetLimitText(20);	
	
	if		(this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("S")) == 0)	
	{

	}
	else if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("A")) == 0)
	{
		CESL_ControlMultiComboBox* pWnd = (CESL_ControlMultiComboBox*)FindControl(_T("CM_MNG_USER_MANAGER_03"), _T("관리구분"));
		ASSERT(pWnd);
		pWnd->EnableWindow(FALSE);

		if (m_mode == 1)
		{
			pWnd->SelectItem(this->m_pInfo->MANAGE_CODE);
		}
		
	}
	else if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("U")) == 0)
	{
		switch (m_mode)
		{
		case 1:
			{
				ASSERT(FALSE);
			}
			break;
		case 2:
		case 3:
			{
				const INT nCtrlCount = 13;
				INT nCtrlID[nCtrlCount] = {
					IDC_COMBO3, IDC_EDIT16, IDC_EDIT1, IDC_EDIT4, IDC_COMBO1, IDC_COMBO2, IDC_COMBO7, IDC_COMBO4,
					IDC_BTN_PERMISION, IDC_COMBO6, IDC_CHECK_SECU_TIMEOUT, IDC_CHECK_SECU_SINGLOG, IDC_CHK_USERINFO_EXP
				};
				for (INT i=0; i<nCtrlCount; i++)
				{
					if (nCtrlID[i] == IDC_EDIT16)
					{
						CECO_ControlDateEdit* pWnd = (CECO_ControlDateEdit*)GetDlgItem(nCtrlID[i]);
						pWnd->ECO_SetReadOnly(TRUE); 
					}
					else
					{
						GetDlgItem(nCtrlID[i])->EnableWindow(FALSE);
					}
				}
			}
			break;
		}		
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMNG_USER_MANAGER_03::InitAuthority(VOID)
{
 	this->m_arrAuthority[0]=_T("");
 	this->m_arrAuthority[1]=_T("");
 	this->m_arrAuthority[2]=_T("");
 	this->m_arrAuthority[3]=_T("");
 	this->m_arrAuthority[4]=_T("");
	this->m_arrAuthority[5]=_T("");
 	this->m_arrAuthority[6]=_T("");
 	this->m_arrAuthority[7]=_T("");
 	this->m_arrAuthority[8]=_T("");
 	this->m_arrAuthority[9]=_T("");
	this->m_arrAuthority[10]=_T("");
 	this->m_arrAuthority[11]=_T("");
 	this->m_arrAuthority[12]=_T("");
 	this->m_arrAuthority[13]=_T("");
 	this->m_arrAuthority[14]=_T("");
	this->m_arrAuthority[15]=_T("");
 	this->m_arrAuthority[16]=_T("");
 	this->m_arrAuthority[17]=_T("");
 	this->m_arrAuthority[18]=_T("");
 	this->m_arrAuthority[19]=_T("");
	((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->DeleteAllItems();

	CString strID;
	GetDlgItemText(IDC_EDIT1,strID);
	if(!strID.IsEmpty())
	{
		CESL_DataMgr* pDM3 = FindDM(_T("DM_MNG_USER_MANAGER_03"));
		CString option;
		option.Format(_T("ID='%s'"),strID);
		pDM3->RefreshDataManager(option);
		
		if(pDM3->GetCellData(_T("AU_TOTAL"),0)==_T("T"))
		{
			this->m_arrAuthority[0]=_T("AU_TOTAL");
		}
		else
		{
			if(pDM3->GetCellData(_T("AU_A"),0)==_T("T"))
			{
				this->m_arrAuthority[0]=_T("AU_A");
			}
			
			if(pDM3->GetCellData(_T("AU_B"),0)==_T("T"))
			{
				this->m_arrAuthority[1]=_T("AU_B");
			}
			
			if(pDM3->GetCellData(_T("AU_C"),0)==_T("T"))
			{
				this->m_arrAuthority[2]=_T("AU_C");
			}
			
			if(pDM3->GetCellData(_T("AU_D"),0)==_T("T"))
			{
				this->m_arrAuthority[3]=_T("AU_D");
			}
			
			if( (pDM3->GetCellData(_T("USER_AU_A"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_B"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_C"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_D"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_E"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_F"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_G"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_H"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_I"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_J"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_K"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_L"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_M"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_N"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_O"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_P"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_Q"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_R"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_S"),0)==_T("T")) ||
				(pDM3->GetCellData(_T("USER_AU_T"),0)==_T("T"))
				)
			{
				CString strUserAuthorityType;
				if(pDM3->GetCellData(_T("USER_AU_A"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_A");								
				else if(pDM3->GetCellData(_T("USER_AU_B"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_B");
				else if(pDM3->GetCellData(_T("USER_AU_C"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_C");
				else if(pDM3->GetCellData(_T("USER_AU_D"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_D");
				else if(pDM3->GetCellData(_T("USER_AU_E"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_E");
				else if(pDM3->GetCellData(_T("USER_AU_F"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_F");
				else if(pDM3->GetCellData(_T("USER_AU_G"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_G");
				else if(pDM3->GetCellData(_T("USER_AU_H"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_H");
				else if(pDM3->GetCellData(_T("USER_AU_I"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_I");
				else if(pDM3->GetCellData(_T("USER_AU_J"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_J");
				else if(pDM3->GetCellData(_T("USER_AU_K"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_K");
				else if(pDM3->GetCellData(_T("USER_AU_L"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_L");
				else if(pDM3->GetCellData(_T("USER_AU_M"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_M");
				else if(pDM3->GetCellData(_T("USER_AU_N"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_N");
				else if(pDM3->GetCellData(_T("USER_AU_O"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_O");
				else if(pDM3->GetCellData(_T("USER_AU_P"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_P");
				else if(pDM3->GetCellData(_T("USER_AU_Q"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_Q");
				else if(pDM3->GetCellData(_T("USER_AU_R"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_R");
				else if(pDM3->GetCellData(_T("USER_AU_S"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_S");
				else if(pDM3->GetCellData(_T("USER_AU_T"),0)==_T("T")) this->m_arrAuthority[4]=_T("USER_AU_T");				
			}			
		}
		
		pDM3 = FindDM(_T("DM_MNG_USER_MANAGER_03_permision"));
		CString alias;
		for(INT i=0;i<20;i++)
		{
			CString str;
			if(m_arrAuthority[i].IsEmpty()) continue;
			str.Format(_T("AUTHORITY='%s'"),m_arrAuthority[i]);
			pDM3->RefreshDataManager(str);
			
			int index = ((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->GetItemCount();
			CString number;
			number.Format(_T("%d"),index+1);
			
			((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->InsertItem(index , number);
			((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->SetItemText(index , 1 , pDM3->GetCellData(0,1));
			
		}
	}
}

VOID CMNG_USER_MANAGER_03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow( bShow, nStatus );
	
	// TODO: Add your message handler code here

	ViewCodePattern();
	ShowUserPermition();

	CProgressCtrl* pPrgCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgINSERT);
	if( m_mode == 1 )
	{		
		SetWindowText( _T("사용자 입력") );
		
		CButton *pBtn;
		pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
		pBtn->ShowWindow( false );
		pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
		pBtn->ShowWindow( false );

		CString strValue;
		GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("주민등록번호사용유무"), strValue);
		if (strValue.CompareNoCase(_T("Y")) == 0)	
		{
			pBtn = ( CButton * )GetDlgItem( IDC_chkCIVILNUM );
			pBtn->SetCheck( true );
			pBtn->ShowWindow( SW_SHOW );
		}
		m_idxCnt = 0;

		ShowHideGroupDlg();

		CESL_DataMgr* pDM3 = FindDM(_T("DM_MNG_USER_MANAGER_03_permision"));
		CString alias;
		CString str;
		str.Format(_T("AUTHORITY='%s'"),_T("AU_TOTAL"));
		pDM3->RefreshDataManager(str);
		alias +=_T("[");
		alias += pDM3->GetCellData(0,1);
		alias +=_T("]\r\n");
		((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->InsertItem(0 , _T("1"));
		((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->SetItemText(0 , 1 , pDM3->GetCellData(0,1));
	}
	else if( m_mode == 2 )
	{
		SetWindowText( _T("사용자 수정") );

		CWnd *pEdt = GetDlgItem( IDC_EDIT1 );
		((CEdit*)pEdt)->SetReadOnly(TRUE);
		
		CString strValue;
		GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("주민등록번호사용유무"), strValue);
		if (strValue.CompareNoCase(_T("Y")) == 0)
		{
			CButton *pBtn;
			pBtn = ( CButton * )GetDlgItem( IDC_chkCIVILNUM );
			pBtn->SetCheck( true );
			pBtn->ShowWindow( SW_SHOW );
		}

		MakeModifyList();
		
		ViewUserCodePattern();
		ViewUserPermition();
		DisplayModifyData( 0 );

		ViewLoanerPermition();
		
		CheckMoveToDM();

		InitAuthority();

		GetAuthorityData(m_stOriUserAuthority);

		DlgInfo_ToDM( &m_cDM_DetailLog_Src );
	}
	else if( m_mode == 3 )
	{
		SetWindowText( _T("사용자 상세보기") );

		CWnd *pEdt2 = GetDlgItem( IDC_EDIT1 );
		((CEdit*)pEdt2)->SetReadOnly(TRUE);

		CButton *pBtn;
		pBtn = ( CButton * )GetDlgItem( IDC_chkCIVILNUM );
		pBtn->SetCheck( true );
		pBtn->ShowWindow( SW_HIDE );
		

		MakeModifyList();
					
		ViewUserCodePattern();
		ViewUserPermition();
		DisplayModifyData( 0 );

		ViewLoanerPermition();

		InitAuthority();
		
		((CButton*)GetDlgItem(IDC_BTN_PERMISION))->EnableWindow(FALSE);
		pPrgCtrl->ShowWindow(FALSE);
	}

	EFS_END
}

BOOL CMNG_USER_MANAGER_03::ISUserGPKI(CString strID)
{
	CString query, result;
	query.Format( _T("SELECT GPKI_UID_VALUE FROM MN_USER_TBL WHERE ID = '%s'"), strID );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03") );
	int nRtn = CLocCommonAPI::GetOneValueQuery( this , pDM , query , result );

	if(nRtn == 0)
	{
		if(result.Compare(_T("")) == 0)
		{
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CMNG_USER_MANAGER_03::ISUserGPKIUSE(CString strID)
{
	CString query, result;
	query.Format( _T("SELECT GPKI_USE FROM MN_USER_TBL WHERE ID = '%s'"), strID );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03") );
	int nRtn = CLocCommonAPI::GetOneValueQuery( this , pDM , query , result );

	if(nRtn == 0)
	{
		if(result.Compare(_T("Y")) == 0)
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}
}

TCHAR CMNG_USER_MANAGER_03::GetUserPermition()
{
	CString query, result;
	query.Format( _T("SELECT PERMITION FROM MN_USER_TBL WHERE ID = '%s'"), GetUserID() );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03") );
	CLocCommonAPI::GetOneValueQuery( this , pDM , query , result );

	TCHAR permition;
	result.MakeUpper();
	wmemcpy( &permition, &result.GetBuffer( 0 )[ 0 ], 1 );
		
	return permition;
}

VOID CMNG_USER_MANAGER_03::OnOK() 
{
	EFS_BEGIN

	DlgInfo_ToDM( &m_cDM_DetailLog_Modify );

	AfxGetApp()->DoWaitCursor(1);
	
	
	INT ids;

	CString sCodePattern;

	ids = CheckIsInputToDB( sCodePattern );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("CMNG_USER_MANAGER_03::OnOK() - CheckIsInputToDB") );
	
	if( ids > 0 ) return ;
	
	ids = SaveToDB( sCodePattern );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -2 , _T("CMNG_USER_MANAGER_03::OnOK() - SaveToDB") );
	
	
	AfxGetApp()->DoWaitCursor(-1);

	CESL_DataMgr* pDM2 = FindDM(_T("DM_MNG_USER_MANAGER_03"));
	CString query,id;
	GetDlgItemText(IDC_EDIT1,id);

	query.Format(_T("UPDATE MN_USER_TBL SET AU_TOTAL='F',")
		_T("USER_AU_A='F',USER_AU_B='F',USER_AU_C='F',USER_AU_D='F',USER_AU_E='F',")
		_T("USER_AU_F='F',USER_AU_G='F',USER_AU_H='F',USER_AU_I='I',USER_AU_J='F',")
		_T("USER_AU_K='F',USER_AU_L='F',USER_AU_M='F',USER_AU_N='F',USER_AU_O='F',")
		_T("USER_AU_P='F',USER_AU_Q='F',USER_AU_R='F',USER_AU_S='F',USER_AU_T='F',")		
		_T("AU_A='F',AU_B='F',AU_C='F',AU_D='F' WHERE ID='%s';"),id);
	pDM2->StartFrame();
	pDM2->AddFrame(query);
	pDM2->SendFrame();
	pDM2->EndFrame();

	if( ( m_arrAuthority[0].IsEmpty() && m_arrAuthority[1].IsEmpty() && m_arrAuthority[2].IsEmpty() && m_arrAuthority[3].IsEmpty() && m_arrAuthority[4].IsEmpty() &&
		  m_arrAuthority[5].IsEmpty() && m_arrAuthority[6].IsEmpty() && m_arrAuthority[7].IsEmpty() && m_arrAuthority[8].IsEmpty() && m_arrAuthority[9].IsEmpty() &&
		  m_arrAuthority[10].IsEmpty() && m_arrAuthority[11].IsEmpty() && m_arrAuthority[12].IsEmpty() && m_arrAuthority[13].IsEmpty() && m_arrAuthority[14].IsEmpty() &&
		  m_arrAuthority[15].IsEmpty() && m_arrAuthority[16].IsEmpty() && m_arrAuthority[17].IsEmpty() && m_arrAuthority[18].IsEmpty() && m_arrAuthority[19].IsEmpty() ) 
		  || m_arrAuthority[0] == _T("AU_TOTAL") )
	{
		query.Format(_T("UPDATE MN_USER_TBL SET AU_TOTAL='T' WHERE ID='%s';"),id);
		pDM2->StartFrame();
		pDM2->AddFrame(query);
		pDM2->SendFrame();
		pDM2->EndFrame();
	}
	else
	{
		pDM2->StartFrame();
		for(int i=0;i<20;i++)
		{
			if(m_arrAuthority[i].IsEmpty()) continue;
			query.Format(_T("UPDATE MN_USER_TBL SET %s='T' WHERE ID='%s';"),m_arrAuthority[i],id);
			pDM2->AddFrame(query);
		}
		pDM2->SendFrame();
		pDM2->EndFrame();
	}

	query.Format( _T("SELECT SECU_LOGIN_IP FROM MN_USER_TBL WHERE ID = '%s'"), id );		
	CLocCommonAPI::GetOneValueQuery( this , pDM2 , query , m_strUserIP );
	
	if( m_mode == 1)
	{		
		CString UserName = TakeControlData(_T("이름"));		
		CString strResult;
		query.Format(_T("SELECT REC_KEY FROM MN_USER_TBL WHERE ID='%s'"), id );
		CLocCommonAPI::GetOneValueQuery(this, pDM2, query, strResult);

		CString strTemp = _T(""), strCotents = _T("");
		strTemp = m_cDM_DetailLog_Modify.GetCellData(_T("유저권한") , 0 );

		if ( !strTemp.IsEmpty() && strTemp.CompareNoCase(_T("일반 사용자")) == 0 )
		{
		
			const int aliasCnt = 4;
			CString arrAlias[aliasCnt] = { _T("접속제한"), _T("이용자정보 출력/반출 허용"), _T("대출자접근권한"), _T("메뉴접근권한") };
		
			for(int i = 0 ; i < aliasCnt ; i++){
				m_cDM_DetailLog_Modify.GetCellData(arrAlias[i] , 0 , strTemp);
				if(strTemp.IsEmpty()) continue;

				if ( strCotents.IsEmpty() )
				{
					strCotents += arrAlias[i] + _T(" : ") + strTemp;
				}
				else{
					strCotents += _T(", ") + arrAlias[i] + _T(" : ") + strTemp;
				}

			}
						

		}
		else if( !strTemp.IsEmpty() && ( strTemp.CompareNoCase(_T("관리자")) == 0 || strTemp.CompareNoCase(_T("통합관리자")) == 0 ))
		{
			strCotents.Format(_T("%s 입력"), strTemp);
			
		}else
		{
			strCotents = _T("사용자 입력");
		}
	
		SetAuthLog(0, strResult, UserName, strCotents);		

	}
	else if( m_mode == 2 )
	{
		GetAuthorityData(m_stDesUserAuthority);
		CompareAuthority(m_stOriUserAuthority, m_stDesUserAuthority);

		Insert_UserEditLogTable();
	}

	EFS_END
}

VOID CMNG_USER_MANAGER_03::OnbBEFORE() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	DisplayModifyData( -1 );

	EFS_END
}

VOID CMNG_USER_MANAGER_03::OnbNEXT() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here

	DisplayModifyData( 1 );	

	EFS_END
}

INT CMNG_USER_MANAGER_03::SaveUserData(CESL_DataMgr* pDM)
{
	EFS_BEGIN

	const INT ctrlCnt = 20;

	CString userInfo[ ctrlCnt ];
	
	GetUserInfo( userInfo );

	const INT fieldCnt = 28;

	CString fieldData[ fieldCnt ];

	for( INT i = 0; i < 16; i++ )
		fieldData[ i ] = userInfo[ i ];
	
	fieldData[ 16 ].Format( _T("%s-%s"), userInfo[ 16 ], userInfo[ 17 ] );
	fieldData[ 17 ].Format( _T("Y") );
	fieldData[ 18 ].Format( _T("%s"), GetTodaybyString() );
	fieldData[ 19 ].Format( _T("%s"), GetCodePattern() );
	fieldData[ 20 ].Format( _T("%s"), GetScreenGroup() );
	fieldData[ 24 ] = userInfo[18];		// 관리구분

	fieldData[ 26 ] = userInfo[19];

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_USERINFO_EXP);
	ASSERT(pBtn);
	fieldData[ 27 ] = pBtn->GetCheck() ? _T("Y") : _T("N");
	
	if( 1 == m_mode )
	{
		fieldData[ 23 ].Format( _T("%s"), GetPK() );
		fieldData[ 21 ] = GetWorkLogMsg( _T("유저관리"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		fieldData[ 23 ].Format( _T("%s"), GetUpdatePK() );
	m_userKey = fieldData[ 23 ];

	fieldData[ 22 ] = GetWorkLogMsg( _T("유저관리"), __WFILE__, __LINE__ );
	
	CString fieldName[ fieldCnt ] = {
		_T("ID"),					
		_T("PASSWD"),				
		_T("PERMITION"),			
		_T("DIVISION_NAME"),		
		_T("CLASS_NAME"),			
		_T("NAME"),					
		_T("BIRTHDAY"),				
		_T("HOME_ZIP_CODE"),		
		_T("HOME_TEL"),				
		_T("HOME_ADDRESS"),			
		_T("BUSINESS_ZIP_CODE"),	
		_T("BUSINESS_TEL"),			
		_T("BUSINESS_ADDRESS"),		
		_T("EXCHANGE_TEL"),			
		_T("E_MAIL"),				
		_T("REMARK"),				
		_T("CIVIL_NO"),				
		_T("STATUS"),				
		_T("ACCESSION_DATE"),		
		_T("CODE_PATTERN"),			
		_T("SCREEN_GROUP"),			
		_T("FIRST_WORK"),			
		_T("LAST_WORK"),			
		_T("REC_KEY") ,				
		_T("MANAGE_CODE"),			
		_T("PW_RENEWAL_DATE"),
		_T("EXPIRY_DATE"),			
		_T("USERGRID_EXPORT_YN")
	};

	CString fieldType[ fieldCnt ] = {
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC") , _T("STRING"),
		_T("NUMERIC"),
		_T("STRING"), _T("STRING")
	};

	INT replaceIndex = -1;
	if( 2 == m_mode )
		replaceIndex = m_modifyList[ m_index ];

	for( i = 0; i < fieldCnt; i++ )
	{
		if( i == 23 || (i==10) || (i==12) || (i==25)) continue;
		if( i == 19 && (m_mode==2) && ( fieldData[i].IsEmpty() || (fieldData[i].Compare(_T(" - 선 택 - "))==0)|| (fieldData[i].Compare(_T(" - 미수정 - "))==0) ) )
		{
			pDM->GetCellData( _T("CODE_PATTERN") , m_modifyList[ m_index ] , fieldData[i] );
			continue;
		}
		if( fieldData[ i ].GetLength() > 0 )
		{
			if(1 == i && TRUE == m_pInfo->m_bUserPwEncryption)
			{
				CString strPassword;
				((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowText(strPassword);

				if(_T("") != m_strCryptoPassword)
				{
					strPassword = m_strCryptoPassword;
				}
				if(64 > strPassword.GetLength())
				{
					strPassword = CLocCommonAPI::StringToSHA256(strPassword);
				}

				pDM->AddDBFieldData( fieldName[ i ] , fieldType[ i ], strPassword, replaceIndex);
			}
			else
			{
				pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], fieldData[ i ], replaceIndex );
			}
		}
		
		else if( fieldData[ i ].GetLength() == 0 && ( i == 7 || i == 8 || i == 9 || i == 11 || i == 13 || i == 14 || i == 15 || i == 26))
		{
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], fieldData[ i ], replaceIndex );
		}

		if( 2 == m_mode )
			pDM->SetCellData( fieldName[ i ], fieldData[ i ], replaceIndex );

	}

	CString strRSC;
	if ( m_bRSC )
	{
		( (CEdit*) GetDlgItem (IDC_edtRSC) )->GetWindowText(strRSC);
		pDM->AddDBFieldData(_T("RSC"), _T("STRING"), strRSC);
	}

	if( 1 == m_mode )
	{
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , fieldData[23] );
		pDM->AddDBFieldData( _T("PW_RENEWAL_DATE") , _T("NUMERIC") , _T("SYSDATE") );
		pDM->MakeInsertFrame( _T("MN_USER_TBL") );
	}
	else if( 2 == m_mode )
	{
		if( m_cDM_DetailLog_Src.GetCellData(_T("비밀번호변경"), 0) != m_cDM_DetailLog_Modify.GetCellData(_T("비밀번호변경"),0) ) 
		{
			pDM->AddDBFieldData( _T("PW_RENEWAL_DATE") , _T("NUMERIC") , _T("SYSDATE") );
		}

		pDM->MakeUpdateFrame( _T("MN_USER_TBL") , _T("REC_KEY"), _T("NUMERIC"), fieldData[ 23 ] , replaceIndex );

		CString query;				
		query.Format(_T("UPDATE ESL_MANAGE_TBL SET MANAGE_CODE='%s' WHERE CLASS_ALIAS ='대출자접근권한' AND VALUE_NAME='%s';"),fieldData[ 24 ], fieldData[ 0 ]);
		pDM->AddFrame(query);
	}

	
	if ( m_bRSC )
	{
		CString strRSCQuery;
		strRSCQuery.Format(_T("UPDATE MN_USER_TBL SET RSC=NULL WHERE RSC='%s' AND REC_KEY <> %s;"), strRSC, fieldData[23]);
		pDM->AddFrame(strRSCQuery);
	}

	if( 1 == m_mode )
		AddTo01DM( fieldName, fieldCnt, fieldData );
	
	CString strValue;	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO6 );

	strValue.Format( _T("%d"), pCombo->GetCurSel() );
	SetLoanPermissonValue(userInfo[ 0 ], strValue);
	
	return 0;

	EFS_END
	return -1;

}

CString CMNG_USER_MANAGER_03::TakeControlData( CString alias, INT option )
{
	EFS_BEGIN

	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MNG_USER_MANAGER_03"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MNG_USER_MANAGER_03"), _T("K2Up") );
		return _T("");
	}
	
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CMNG_USER_MANAGER_03"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;

	EFS_END
	return _T("");

}

VOID CMNG_USER_MANAGER_03::GetUserInfo( CString userInfo[] )
{
	EFS_BEGIN
	
	const INT ctrlCnt = 20;	
	CString ctrlAlias[ ctrlCnt ] = {
		_T("id"),					
		_T("암호"),					
		_T("등급"),					
		_T("부서"),					
		_T("직급"),					
		_T("이름"),					
		_T("생년월일"),				
		_T("자택_우편번호"),		
		_T("자택_전화번호"),		
		_T("자택_주소"),			
		_T("근무지_우편번호"),		
		_T("근무지_전화번호"),		
		_T("근무지_주소"),			
		_T("내선번호"),				
		_T("이메일"),				
		_T("비고"),					
		_T("주민번호_앞자리"),		
		_T("주민번호_뒷자리"),		
		_T("관리구분"),				
		_T("유효기간")				
	};	

	for( INT i = 0; i < ctrlCnt; i++ )
		if( (2 == i) || (6 == i))
			continue;
		else
			userInfo[ i ] = TakeControlData( ctrlAlias[ i ] );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	switch( pCombo->GetCurSel() )
	{
		case 0 :
			userInfo[ 2 ] = _T("");
			break;
		case 1 :
			userInfo[ 2 ] = _T("A");
			break;
		case 2 :
			userInfo[ 2 ] = _T("U");
			break;
		case 3 :
			userInfo[ 2 ] = _T("S");
			break;
			
			break;
	}

	EFS_END
}

CTime CMNG_USER_MANAGER_03::GetToDay()
{
	CTime day;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03") );
	day = pDM->GetDBTime();

	return day;
}

CString CMNG_USER_MANAGER_03::GetTodaybyString()
{
	EFS_BEGIN

	CTime day = GetToDay();
	CString today;
	today.Format( _T("%d/%d/%d"), day.GetYear(), day.GetMonth(), day.GetDay() );

	return today;

	EFS_END
	return _T("");

}

CString CMNG_USER_MANAGER_03::GetCodePattern()
{
	EFS_BEGIN

	CString codePattern;

	m_codePattern.GetWindowText( codePattern );

	return codePattern;

	EFS_END
	return _T("");

}

CString CMNG_USER_MANAGER_03::GetScreenGroup()
{
	EFS_BEGIN

	CString screenGroup;

	return screenGroup;

	EFS_END
	return _T("");

}

CString CMNG_USER_MANAGER_03::GetPK()
{
	EFS_BEGIN


	CString pk;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03") );
	pDM->MakeRecKey( pk );

	return pk;

	EFS_END
	return _T("");

}

VOID CMNG_USER_MANAGER_03::DisplayModifyData( INT flag )
{
	EFS_BEGIN

	switch( flag )	
	{
		case 0 :

			if( !m_idxCnt )
			{
				AfxMessageBox( _T("선택된 사용자가 없습니다.") );
				PostMessage( WM_CLOSE );
				return;
			}

			break;

		case -1 :

			m_index--;

			if( -1 == m_index )
			{
				AfxMessageBox( _T(" 맨 처음 사용자입니다.") );
				m_index++;
				return;
			}

			break;

		case 1 :

			m_index++;
			
			if( m_idxCnt == m_index )
			{
				AfxMessageBox( _T("마지막 사용자입니다.") );
				m_index--;
				return;
			}

			break;

		default :

			break;
	}

	AllControlDisplay( _T("CM_MNG_USER_MANAGER_03"), m_modifyList[ m_index ] );
	ViewUserMethod();
	ViewUserCodePattern();
	ViewCivilNum();
	ViewGroupInfo();
	ViewUserPermition();
	ParentGridReverse( m_modifyList[ m_index ] );

	if(m_mode == 1)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_GPKI_STATE))->SetWindowText(_T("* GPKI 미등록"));
		((CButton*)GetDlgItem(IDC_BUTTON_GPKI))->SetWindowText(_T("GPKI 등록"));		
	}
	else if(m_mode == 2)
	{
		m_nGPKIMode = 1;
		CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
		pCombo->SetCurSel(0);
		CString strID;
		GetDlgItemText(IDC_EDIT1,strID);
		if(ISUserGPKI(strID))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_GPKI_STATE))->SetWindowText(_T("* GPKI 등록"));
			((CButton*)GetDlgItem(IDC_BUTTON_GPKI))->SetWindowText(_T("GPKI 수정"));
		}
		else
		{
			((CStatic*)GetDlgItem(IDC_STATIC_GPKI_STATE))->SetWindowText(_T("* GPKI 미등록"));
			((CButton*)GetDlgItem(IDC_BUTTON_GPKI))->SetWindowText(_T("GPKI 등록"));
		}	
		
		((CButton*)GetDlgItem(IDC_chkGPKI))->SetCheck(ISUserGPKIUSE(strID));
		
	}
	else if(m_mode == 3)
	{
		CString strID;
		GetDlgItemText(IDC_EDIT1,strID);
		if(ISUserGPKI(strID))
		{
			((CStatic*)GetDlgItem(IDC_STATIC_GPKI_STATE))->SetWindowText(_T("* GPKI 등록"));		
		}
		else
		{
			((CStatic*)GetDlgItem(IDC_STATIC_GPKI_STATE))->SetWindowText(_T("* GPKI 미등록"));			
		}	
		((CButton*)GetDlgItem(IDC_chkGPKI))->SetCheck(ISUserGPKIUSE(strID));
	}

	if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("U")) != 0)
	{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_MANAGER_01"));
	CString strExpYN;
	pDM->GetCellData(_T("USERGRID_EXPORT_YN"), m_modifyList[m_index], strExpYN);

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_USERINFO_EXP);
	ASSERT(pBtn);
	pBtn->SetCheck( (strExpYN.CompareNoCase(_T("Y")) == 0) );

	if (m_mode != 1)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO3);
		ASSERT(pCombo != NULL);

		int nIdx = pCombo->GetCurSel();

		GetDlgItem(IDC_EDIT16)->EnableWindow( (nIdx == 2) );
		GetDlgItem(IDC_CHK_USERINFO_EXP)->EnableWindow( (nIdx == 2) );
		
		if		(nIdx == 1) pBtn->SetCheck(TRUE);
		else if (nIdx == 3) pBtn->SetCheck(FALSE);
	}

	pDM->GetCellData(_T("SECU_TIMEOUT"), m_modifyList[m_index], strExpYN);
	pBtn = (CButton*)GetDlgItem(IDC_CHECK_SECU_TIMEOUT);
	ASSERT(pBtn);
	BOOL bBOOL = (strExpYN.CompareNoCase(_T("")) != 0) && (strExpYN.CompareNoCase(_T("0")) != 0);
	pBtn->SetCheck( (strExpYN.CompareNoCase(_T("")) != 0) && (strExpYN.CompareNoCase(_T("0")) != 0) );
	m_nTimeout = _ttoi(strExpYN);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO3);
	ASSERT(pCombo != NULL);
	int nIdx = pCombo->GetCurSel();

	if(nIdx == 2)
	{
		((CButton*)GetDlgItem(IDC_CHECK_SECU_SINGLOG))->EnableWindow(TRUE);
		pDM->GetCellData(_T("SECU_LOGIN_IP"), m_modifyList[m_index], m_strUserIP);
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_SECU_SINGLOG);
		ASSERT(pBtn);	
		pBtn->SetCheck( (m_strUserIP.Compare(_T("")) != 0) );		
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_SECU_SINGLOG))->EnableWindow(FALSE);
	}
	}

	if( m_mode == 2 )
	{
		GetAuthorityData(m_stOriUserAuthority);
	}	

	EFS_END
}

VOID CMNG_USER_MANAGER_03::ViewCivilNum()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	INT civilNum_1, civilNum_2;
	CString result, civilNum;

	pDM->GetCellData( _T("CIVIL_NO"), m_modifyList[ m_index ], result );

	CEdit *pEdt_1 = ( CEdit * )GetDlgItem( IDC_EDIT5 );
	CEdit *pEdt_2 = ( CEdit * )GetDlgItem( IDC_EDIT6 );

	
	if( !result.GetLength() || _T("-") == result )
	{
		pEdt_1->SetWindowText( _T("") );
		pEdt_2->SetWindowText( _T("") );	
	}
	else
	{
		_stscanf( result.GetBuffer( 0 ), _T("%d-%d"), &civilNum_1, &civilNum_2 );

		civilNum.Format( _T("%d"), civilNum_1 );
		pEdt_1->SetWindowText( civilNum );

		civilNum.Format( _T("%d"), civilNum_2 );
		pEdt_2->SetWindowText( civilNum );
	}

	EFS_END
}

INT CMNG_USER_MANAGER_03::CheckUserID()
{
	EFS_BEGIN

	INT ids;

	CString id = TakeControlData( _T("id") );
	id.TrimLeft();
	id.TrimRight();

	if( !id.GetLength() )
		return -1;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03") );

	if( m_mode == 2 )
	{
		CESL_DataMgr *pDM2 = FindDM( _T("DM_MNG_USER_MANAGER_01") );

		CString sPreID;
		ids = pDM2->GetCellData( _T("ID") , m_modifyList[ m_index ] , sPreID );
		if( ids < 0 ) 
		{
			CString sEMsg;
			sEMsg.Format( _T("pDM->GetCellData( id , %d , %s );") , m_modifyList[ m_index ] , sPreID );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CheckUserID") );

		if( sPreID.Compare(id) == 0 ) 
			return 0;
	}

	CString query, result;
	query.Format( _T("SELECT COUNT( * ) FROM MN_USER_TBL WHERE ID = '%s' AND STATUS = 'Y'"), id );
	CLocCommonAPI::GetOneValueQuery( this , pDM , query , result );

	if( _ttoi( result ) )
		return 1;

	return 0;

	EFS_END
	return -1;

}

INT CMNG_USER_MANAGER_03::CheckUserPasswd()
{
	EFS_BEGIN

	if( TakeControlData(_T("암호")).IsEmpty() || TakeControlData(_T("암호확인")).IsEmpty() )
	{
		AfxMessageBox( _T("암호를 입력해주십시오.") );
		return 1;
	}

	if( TakeControlData( _T("암호") ) != TakeControlData( _T("암호확인") ) )
	{
		AfxMessageBox( _T("암호가 일치하지 않습니다. 다시 입력하여 주십시오") );
		return 2;
	}
	
	INT ids = CheckPasswordCreateRule(TakeControlData( _T("암호") ));
	if(0 > ids)
	{
		AfxMessageBox( _T("비밀번호는 영문, 숫자, 특수문자 중 2개이상을 조합,\r\n10자리 이상을 사용해야 합니다.") );
		return 3;
	}

	return 0;

	EFS_END
	return -1;

}

INT CMNG_USER_MANAGER_03::GetSelectedRowCount()
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = ( CESL_ControlMgr * )FindCM( _T("CM_MNG_USER_MANAGER_01") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( _T("그리드") );

	return pGrid->SelectGetCount();

	EFS_END
	return -1;

}

CString CMNG_USER_MANAGER_03::GetUpdatePK()
{
	EFS_BEGIN

	CString pk;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	pDM->GetCellData( _T("REC_KEY"), m_modifyList[ m_index ], pk );
	
	return pk;

	EFS_END
	return _T("");

}

INT CMNG_USER_MANAGER_03::AddTo01DM( CString fieldName[], INT cnt, CString data[] )
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	pDM->SetCellData( fieldName, cnt, data, -1 );

	return 0;

	EFS_END
	return -1;

}

VOID CMNG_USER_MANAGER_03::ViewCodePattern()
{
	EFS_BEGIN

	m_codePattern.ResetContent();
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03_codePatternList") );

	CLocCommonAPI::RefreshDataManager(this,pDM , _T("") );
	
	CString codePatternName;

	if( m_mode == 1 )
		m_codePattern.InsertString( 0, _T(" - 선 택 - ") );
	else if( m_mode == 2 )
		m_codePattern.InsertString( 0, _T(" - 미수정 - ") );
	else if( m_mode == 3 )
		m_codePattern.InsertString( 0, _T(" - 선 택 - ") );
	
	for( INT i = 0 ;i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("DISTINCT PATTERN_NAME"), i, codePatternName );

		m_codePattern.InsertString( i + 1, codePatternName );
	}
	m_codePattern.SetCurSel( 0 );	

	EFS_END
}

INT CMNG_USER_MANAGER_03::MakeUserCode( CESL_DataMgr* pSaveDM )
{
	EFS_BEGIN

	CString codePattern;
	m_codePattern.GetWindowText( codePattern );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03_codePattern") );
	
	CString where;
	where.Format( _T(" PATTERN_NAME = '%s' "), codePattern );

	CLocCommonAPI::RefreshDataManager( this , pDM , where );

	const INT fieldCnt = 8;
	
	CString fieldName[ fieldCnt ] = {
		_T("CODE_KEY"),			//	0
		_T("ACTIVE"),			//	1
		_T("DEFAULT_VALUE"),	//	2
		_T("REC_KEY"),			//	3
		_T("USER_KEY"),			//	4
		_T("PATTERN"),			//	5
		_T("FIRST_WORK"),		//	6
		_T("LAST_WORK") };		//	7

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") };

	CString data[ fieldCnt ];

	data[ 4 ] = m_userKey;
	data[ 5 ] = codePattern;

	pSaveDM->MakeDeleteFrame( _T("MN_USER_CODE_TBL") , _T("USER_KEY") , _T("NUMERIC") , data[4] );

	pSaveDM->InitDBFieldData();

	CString sRecKey;



	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		if( pSaveDM->m_dbkind == 1 )
			sRecKey = _T("ESL_SEQ.NEXTVAL");
		else
			pSaveDM->MakeRecKey(sRecKey);

		pDM->GetCellData( fieldName, 3, data, i );

		data[ 6 ] = GetWorkLogMsg( _T("유저관리"), __WFILE__, __LINE__ );
	
		for( INT j = 0; j < fieldCnt; j++ )
		{
			if( j == 3 ) continue;
			pSaveDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ] );
		}

		pSaveDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pSaveDM->MakeInsertFrame( _T("MN_USER_CODE_TBL") );
	}

	return 0;	

	EFS_END
	return -1;

}

INT CMNG_USER_MANAGER_03::GetUserCodePK()
{
	EFS_BEGIN

	CString pk;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03_codePattern") );

	CLocCommonAPI::GetOneValueQuery( this , pDM , _T("SELECT MAX( REC_KEY ) FROM MN_USER_CODE_TBL") , pk );

	return _ttoi( pk ) + 1;

	EFS_END
	return -1;

}

VOID CMNG_USER_MANAGER_03::ViewUserCodePattern()
{
	EFS_BEGIN

	if( m_mode != 3 ) return ;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	CString sResult;
	pDM->GetCellData( _T("CODE_PATTERN"), m_modifyList[ m_index ], sResult );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	
	CString sCodePattern;
	for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		sCodePattern.Empty();
		pCombo->GetLBText( i , sCodePattern );
		
		if( sCodePattern.Compare( sResult ) == 0 )
		{
			pCombo->SetCurSel(i);
			return ;
		}
	}
	
	pCombo->SetCurSel(0);

	return;

	EFS_END
}

VOID CMNG_USER_MANAGER_03::ShowUserPermition()
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->ResetContent();
	pCombo->InsertString( 0, _T(" - 선 택 - ") );

	pCombo->InsertString( 1, _T("관리자") );
	pCombo->InsertString( 2, _T("일반 사용자") );	
	pCombo->InsertString( 3, _T("통합 관리자") );
	pCombo->SetCurSel( 0 );

	EFS_END
}

VOID CMNG_USER_MANAGER_03::ViewUserPermition()
{
	EFS_BEGIN

 	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
 	CString result;
 	pDM->GetCellData( _T("PERMITION"), m_modifyList[ m_index ], result );
 
 	result.MakeUpper();
 
 	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
 
 	if( result.Compare(_T("A")) == 0 || (result.Compare(_T("시스템관리자")) == 0) )
 		pCombo->SetCurSel(1);
 	else if(result.Compare(_T("M")) == 0 || (result.Compare(_T("사용자관리자")) == 0))
 		pCombo->SetCurSel(1);
 	else if(result.Compare(_T("A")) == 0 || (result.Compare(_T("관리자")) == 0))
 		pCombo->SetCurSel(1);
 	else if(result.Compare(_T("U")) == 0 || (result.Compare(_T("일반사용자"))==0) )
 		pCombo->SetCurSel(2);
	 else if(result.Compare(_T("S")) == 0 || (result.Compare(_T("통합관리자"))==0) )
 		pCombo->SetCurSel(3);
 	else
 		pCombo->SetCurSel(0);

		InitAuthority();

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO6 );
	if ( pCombo == NULL ) return;
	pDM->GetCellData( _T("LOAN_PERMITION"), m_modifyList[ m_index ], result );
	
	if( result.Compare(_T("0")) == 0 || (result.Compare(_T("모든 권한")) == 0) )
		pCombo->SetCurSel(0);
	else if(result.Compare(_T("1")) == 0 || (result.Compare(_T("권한 없음")) == 0))
		pCombo->SetCurSel(1);
	else if(result.Compare(_T("2")) == 0 || (result.Compare(_T("입력 및 수정가능")) == 0))
		pCombo->SetCurSel(2);
	else
		pCombo->SetCurSel(0);

	EFS_END
}

INT CMNG_USER_MANAGER_03::CheckCivilNum()
{
	EFS_BEGIN

	INT ids;
		
	CString civilNum, tmp1, tmp2;
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT5 );
	pEdt->GetWindowText( tmp1 );
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT6 );
	pEdt->GetWindowText( tmp2 );

	if( tmp1.IsEmpty() || tmp2.IsEmpty() )
	{
		AfxMessageBox( _T("주민등록번호를 입력해주십시오.") );
		return false;
	}

	civilNum.Format( _T("%s-%s"), tmp1, tmp2 );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03") );

	if( m_mode == 2 )
	{
		CString sPreCivilNo;

		CESL_DataMgr *pDM2 = FindDM( _T("DM_MNG_USER_MANAGER_01") );

		ids = pDM2->GetCellData( _T("CIVIL_NO") , m_modifyList[ m_index ] , sPreCivilNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CheckCivilNum") );

		if( civilNum.Compare( sPreCivilNo ) == 0 ) 
			return true;
	}
	CString query, result;
	query.Format( _T("SELECT COUNT( * ) FROM MN_USER_TBL WHERE CIVIL_NO = '%s' AND STATUS = 'Y'"), civilNum );
	CLocCommonAPI::GetOneValueQuery( this , pDM , query , result );

	if( _ttoi(result) )
	{
		AfxMessageBox( _T("주민등록번호가 중복됩니다. 다시 입력하여 주시시오.") );
		return false;
	}	
	
	return true;

	EFS_END
	return -1;

}

VOID CMNG_USER_MANAGER_03::OnbGETZIPCODE1() 
{
	EFS_BEGIN

	CString data[ 2 ];
	if( GetZipCode( data ) )
		return;
	
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT7 );
	pEdt->SetWindowText( data[ 0 ] );
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT9 );
	pEdt->SetWindowText( data[ 1 ] );

	EFS_END
}

VOID CMNG_USER_MANAGER_03::OnbGETZIPCODE2() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	CString data[ 2 ];
	if( GetZipCode( data ) )
		return;
	
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT10 );
	pEdt->SetWindowText( data[ 0 ] );
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT12 );
	pEdt->SetWindowText( data[ 1 ] );

	EFS_END
}

INT CMNG_USER_MANAGER_03::GetZipCode( CString data[] )
{
	EFS_BEGIN

CSearchZipCodeDlgOpenAPI dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		data[ 0 ] = dlg.GetApplyZipCode();
		data[ 1 ] = dlg.GetApplyAddress();		
	}

	return 0;

	EFS_END
	return -1;

}

VOID CMNG_USER_MANAGER_03::MakeMehodList()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_METHOD_LIST") );

	CLocCommonAPI::RefreshDataManager( this , pDM , _T("") );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO5 );

	if( m_mode == 1 )
		pCombo->InsertString( 0, _T(" - 선 택 - ") );
	else if( m_mode == 2 )
		pCombo->InsertString( 0, _T(" - 미수정 - ") );
	else
		pCombo->InsertString( 0, _T(" - 선 택 - ") );

	CString method;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("METHOD_NAME"), i, method );
		pCombo->InsertString( i + 1, method );
	}
	pCombo->SetCurSel( 0 );

	EFS_END
}

INT CMNG_USER_MANAGER_03::MakeUserMethod( CESL_DataMgr* pDM )
{
	EFS_BEGIN

	CString method = GetCurrentMethod();

	if( method.IsEmpty() && (m_mode == 2 ) )
		return 0;

	if( !method.GetLength() )
		return -1;
	
	CString id = TakeControlData( _T("id") );
	
	if( 2 == m_mode )
	{
		DeleteBeforeUserMethod( id , pDM );
	}
	SaveUserMethod( id, method , pDM );
	
	return 0;	

	EFS_END
	return -1;

}

CString CMNG_USER_MANAGER_03::GetCurrentMethod()
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO5 );
	INT row = pCombo->GetCurSel();
	if( !row )
		return _T("");

	CString method;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_METHOD_LIST") );
	pDM->GetCellData( _T("METHOD_NAME"), row - 1, method );

	return method;

	EFS_END
	return _T("");

}

VOID CMNG_USER_MANAGER_03::SaveUserMethod( CString id, CString method , CESL_DataMgr* pDM )
{
	EFS_BEGIN

	CProgressCtrl* pPrgCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgINSERT);

	CMNG_ACCESS_CONTROL_API AccessApi;
	INT ids = AccessApi.METHOD_New( pDM , 'U' , id , 'M' , method , pPrgCtrl , FALSE );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SaveUserMethod AccessApi.METHOD_New") );
	

	EFS_END
}

VOID CMNG_USER_MANAGER_03::DeleteBeforeUserMethod( CString id , CESL_DataMgr* pDM )
{
	EFS_BEGIN

	CString query;
	query.Format( _T("DELETE FROM ESL_MENU_TBL WHERE USER_ID = '%s' AND FLAG = 'U';"), id );
	pDM->AddFrame( query );
	query.Format( _T("DELETE FROM ESL_TOOLBAR_TBL WHERE USER_ID = '%s' AND FLAG = 'U';"), id );
	pDM->AddFrame( query );
	query.Format( _T("DELETE FROM ESL_MENU_METHOD_TBL WHERE USER_ID = '%s' AND FLAG = 'U';"), id );
	pDM->AddFrame( query );

	EFS_END
}

VOID CMNG_USER_MANAGER_03::SaveMenuInfo( CString id, CString method )
{
	EFS_BEGIN

		
	EFS_END
}

VOID CMNG_USER_MANAGER_03::SaveToolBarInfo( CString id, CString method )
{
	EFS_BEGIN

	EFS_END
}

VOID CMNG_USER_MANAGER_03::SaveMethodInfo( CString id, CString method )
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_METHOD") );

	const INT cnt = 4;
	CString field[ cnt ] = {
		_T("PK"),
		_T("FLAG"),
		_T("USER_ID"),
		_T("USER_METHOD_NAME") };

	CString type[ cnt ] = { _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") };
	
	CString data[ cnt ];
	if( pDM->m_dbkind == 1 )
		data[0] = _T("ESL_SEQ.NextVal");
	else
		pDM->MakeRecKey( data[0] );
	data[1].Format( _T("U") );
	data[2].Format( _T("%s"), id );
	data[3].Format( _T("%s"), method );

	
	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < cnt; i++ )
	{
		if( data[i].GetLength() )
			pDM->AddDBFieldData( field[i], type[i], data[i] );
	}

	pDM->MakeInsertFrame( _T("ESL_MENU_METHOD_TBL") );

	pDM->SendFrame();
	pDM->EndFrame();	

	EFS_END
}

VOID CMNG_USER_MANAGER_03::GetMenuDMAlias( CString alias[] )
{
	EFS_BEGIN

	alias[  0 ] = _T("PK");
	alias[  1 ] = _T("IDX");
	alias[  2 ] = _T("USER_ID");
	alias[  3 ] = _T("ALIAS");
	alias[  4 ] = _T("GROUP_ALIAS");
	alias[  5 ] = _T("CAPTION");
	alias[  6 ] = _T("VISIBLE");
	alias[  7 ] = _T("STYLE");
	alias[  8 ] = _T("ACTION");
	alias[  9 ] = _T("TARGET");
	alias[ 10 ] = _T("FLAG");
	alias[ 11 ] = _T("SPF_USE_FLAG");
	alias[ 12 ] = _T("PROGRAME_USE_FLAG");

	EFS_END
}

CString CMNG_USER_MANAGER_03::MakeMethodSelectQuery( CString method, CString id, INT type )
{
	EFS_BEGIN

	CString where;

	switch( type )
	{
	case 1 :
		where.Format( _T("USER_ID = '%s' AND FLAG = 'M'"), method );
		break;
	default :
		where.Empty();
		break;
	}
	
	return where;

	EFS_END
	return _T("");

}

VOID CMNG_USER_MANAGER_03::GetToolBarDMAlias( CString alias[] )
{
	EFS_BEGIN

	alias[  0 ] = _T("PK");
	alias[  1 ] = _T("IDX");
	alias[  2 ] = _T("USER_ID");
	alias[  3 ] = _T("FLAG");
	alias[  4 ] = _T("ALIAS");
	alias[  5 ] = _T("MENU_ALIAS");
	alias[  6 ] = _T("STYLE");
	alias[  7 ] = _T("TYPE");
	alias[  8 ] = _T("SM");
	alias[  9 ] = _T("BITMAP_NORMAL");
	alias[ 10 ] = _T("BITMAP_HOT");
	alias[ 11 ] = _T("BITMAP_DISABLED");
	alias[ 12 ] = _T("TOOLTIP");
	alias[ 13 ] = _T("SPF_USE_FLAG");
	alias[ 14 ] = _T("PROGRAME_USE_FLAG");

	EFS_END
}

VOID CMNG_USER_MANAGER_03::ViewUserMethod()
{
	EFS_BEGIN

	if( m_mode != 3 ) return ;

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	CString id;
	pDM->GetCellData( _T("ID"), m_modifyList[ m_index ], id );

	CString query, result, method;
	query.Format( _T("SELECT USER_METHOD_NAME FROM ESL_MENU_METHOD_TBL WHERE USER_ID = '%s' AND FLAG = 'U'"), id );
	CLocCommonAPI::GetOneValueQuery( this , pDM , query , result );

	pDM = FindDM( _T("DM_MNG_USER_METHOD_LIST") );
	INT row = 0;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("METHOD_NAME"), i, method );

		if( method == result )
		{
			row = i + 1;
			break;
		}
	}

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO5 );
	pCombo->SetCurSel( row );

	EFS_END
}

VOID CMNG_USER_MANAGER_03::MakeLoanerPermitionCombo()
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO6 );
	pCombo->InsertString( 0, _T("모든 권한") );
	pCombo->InsertString( 1, _T("권한 없음") );

	pCombo->InsertString( 2, _T("입력 및 수정가능") );
	pCombo->SetCurSel( 1 );

	EFS_END
}

VOID CMNG_USER_MANAGER_03::SaveLoanerPermition( CString id , CESL_DataMgr* pDM )
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO6 );
	
	CString value;
	INT		ids;
	BOOL	bInsert;

	bInsert = FALSE;
	ids = GetManageValue( _T("열람"), _T("공통"), _T("대출자접근권한"), id, value );
	if ( !ids )
	{	
		if ( !value.GetLength() ) value = _T("1");

		if( value.GetLength() )
		{
			value.Format( _T("%d"), pCombo->GetCurSel() );
			SetManageValue( _T("열람"), _T("공통"), _T("대출자접근권한"), id, value );
			return;
		}
	}
	else
	{
		bInsert = TRUE;
		value.Format( _T("%d"), pCombo->GetCurSel() );
	}

	if ( bInsert )
	{
		CString sRecKey;
		
		if( pDM->m_dbkind == 1 )
			sRecKey = _T("ESL_SEQ.NextVal");
		else
			pDM->MakeRecKey(sRecKey);
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("GROUP_1") , _T("STRING") , _T("L") );
		pDM->AddDBFieldData( _T("GROUP_2") , _T("STRING") , _T("열람") );
		pDM->AddDBFieldData( _T("GROUP_3") , _T("STRING") , _T("공통") );
		pDM->AddDBFieldData( _T("CLASS_ALIAS") , _T("STRING") , _T("대출자접근권한") );
		pDM->AddDBFieldData( _T("CLASS_DESC") , _T("STRING") , _T("대출자접근권한") );
		pDM->AddDBFieldData( _T("VALUE_NAME") , _T("STRING") , id );
		pDM->AddDBFieldData( _T("VALUE_1") , _T("STRING") , value );
	
		CString strManageCode;
		ids = GetControlData( _T("CM_MNG_USER_MANAGER_03"), _T("관리구분"), strManageCode );
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , strManageCode ); 
		
		pDM->MakeInsertFrame( _T("ESL_MANAGE_TBL") );
	}

	EFS_END
}

VOID CMNG_USER_MANAGER_03::ViewLoanerPermition()
{
	EFS_BEGIN

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	CString id, value;
	pEdt->GetWindowText( id );

	GetLoanPermissonValue(id, value);

	if ( !value.GetLength() ) value = _T("1");
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO6 );
	pCombo->SetCurSel( _ttoi( value ) );	

	EFS_END
}

VOID CMNG_USER_MANAGER_03::OnchkVIEWGROUP() 
{
	EFS_BEGIN

	EFS_END
}



INT CMNG_USER_MANAGER_03::ShowHideGroupDlg()
{
	EFS_BEGIN

	CButton *pCheck = (CButton*)(GetDlgItem(IDC_chkVIEWGROUP));

	if( pCheck->GetCheck() == 1 ) 
	{
		m_pGroupViewDlg->ShowWindow(SW_SHOW);
	}
	else
		m_pGroupViewDlg->ShowWindow(SW_HIDE);

	CRect WndRect1,WndRect2;
	GetWindowRect( &WndRect1 );
	m_pGroupViewDlg->GetWindowRect( &WndRect2 );
	m_pGroupViewDlg->MoveWindow( WndRect1.right , WndRect1.top , WndRect2.Width() , WndRect2.Height() );

	return 0;

	EFS_END
	return -1;

}

VOID CMNG_USER_MANAGER_03::OnMove(INT x, INT y) 
{
	EFS_BEGIN

	CDialog::OnMove(x, y);
	

	if( m_pGroupViewDlg->GetSafeHwnd() == NULL ) 
		return;

	ShowHideGroupDlg();	

	EFS_END
}

VOID CMNG_USER_MANAGER_03::OnCancel() 
{	
	EFS_BEGIN

	if( m_pGroupViewDlg != NULL ) 
		m_pGroupViewDlg->ShowWindow(SW_HIDE);

	if( m_pGroupViewDlg != NULL ) 
		delete m_pGroupViewDlg ;
	m_pGroupViewDlg = NULL;

	CDialog::OnCancel();

	EFS_END
}

VOID CMNG_USER_MANAGER_03::OnSelchangeCombo5() 
{
	EFS_BEGIN

	OnchkVIEWGROUP();

	EFS_END
}

INT CMNG_USER_MANAGER_03::ViewGroupInfo()
{
	EFS_BEGIN

	if( m_pGroupViewDlg == NULL ) return 0;

	if( !m_pGroupViewDlg->IsWindowVisible() ) return 0;

	CString sUserID;

	INT nMode = 0;

	CWnd* pUser = GetDlgItem(IDC_EDIT1);

	pUser->GetWindowText(sUserID);

	CString sMethod = GetCurrentMethod();

	if( !sMethod.IsEmpty() && (m_mode != 3) )
	{
		nMode = 1;
		sUserID = sMethod;
	}

	m_pGroupViewDlg->ChangeGroup(sUserID,nMode);

	return 0;

	EFS_END
	return -1;

}

INT CMNG_USER_MANAGER_03::SettingScreen()
{
	EFS_BEGIN

	if( m_mode == 3 )
	{
		GetDlgItem(IDC_BUTTON_GPKI)->ShowWindow( SW_HIDE );

		if(TRUE == m_pInfo->m_bUserPwEncryption)
		{
			const INT nUnVisibleCnt = 8;
			INT nUnVisibleIDC[nUnVisibleCnt] =
			{
				IDC_bGET_ZIP_CODE_1 , IDC_bGET_ZIP_CODE_2 , IDOK , IDC_staPASS , IDC_EDIT2 , 
				IDC_staPASSC , IDC_EDIT3, IDC_btnCHANGEPW
			};

			for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
			{
				GetDlgItem( nUnVisibleIDC[i] )->ShowWindow( SW_HIDE );
			}
		}
		else
		{
			const INT nUnVisibleCnt = 7;
			INT nUnVisibleIDC[nUnVisibleCnt] =
			{
				IDC_bGET_ZIP_CODE_1 , IDC_bGET_ZIP_CODE_2 , IDOK , IDC_staPASS , IDC_EDIT2 , 
				IDC_staPASSC , IDC_EDIT3
			};

			for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
			{
				GetDlgItem( nUnVisibleIDC[i] )->ShowWindow( SW_HIDE );
			}
		}
	}
	return 0;

	EFS_END
	return -1;

}

INT CMNG_USER_MANAGER_03::ParentGridReverse( INT nIndex )
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( _T("CM_MNG_USER_MANAGER_01") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( _T("그리드") );
	pGrid->SetReverse( nIndex );	

	return 0;

	EFS_END
	return -1;

}

INT CMNG_USER_MANAGER_03::CheckMoveToDM()
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( _T("CM_MNG_USER_MANAGER_01") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( _T("그리드") );

	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_MANAGER_01"));

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->SetCellData(_T("선정"),_T(""),i);
		if( pGrid->GetAt( i , 0 ).Compare(_T("V")) == 0 )
			pDM->SetCellData(_T("선정"),_T("V"),i);
	}

	return 0;

	EFS_END
	return -1;

}

VOID CMNG_USER_MANAGER_03::MakeModifyList()
{
	EFS_BEGIN

	FreeModifyList();

	CMNG_USER_MANAGER_01 *pParent = ( CMNG_USER_MANAGER_01 * )m_pParent;
	m_modifyList = pParent->GetCheckedDMRow( m_idxCnt, m_index, 1 );

	EFS_END
}


VOID CMNG_USER_MANAGER_03::FreeModifyList()
{
	EFS_BEGIN

	if( m_modifyList )
	{
		free( m_modifyList );
		m_modifyList = NULL;
	}
	m_idxCnt = 0;
	m_index = 0;

	EFS_END
}

INT CMNG_USER_MANAGER_03::CheckIsInputToDB( CString &sCodePattern )
{
	EFS_BEGIN

	switch( CheckUserID() )
	{
		case -1 :
			AfxMessageBox( _T("ID를 입력하여 주십시오") );
			return 1;
			break;
		case 1 :
			AfxMessageBox( _T("존재하는 ID입니다. 다시 입력하여 주십시오") );
			return 2;
			break;
	}

	CButton *pBtn = ( CButton * )GetDlgItem( IDC_chkCIVILNUM );

	if( pBtn->GetCheck() == 1 )
	{
		if( !CheckCivilNum() )
			return 3;
		
		CString sCivilNo1,sCivilNo2;
		GetDlgItem( IDC_EDIT5 )->GetWindowText( sCivilNo1 );
		GetDlgItem( IDC_EDIT6 )->GetWindowText( sCivilNo2 );
		INT ids = CLocCommonAPI::CivilNoCheck( sCivilNo1 , sCivilNo2);
		
		if( ids != 0 && pBtn->GetCheck() )
		{
			AfxMessageBox( _T("올바른 주민등록번호를 입력하십시오.") );
			return 4;
		}
		
	}

	if( CheckUserPasswd() > 0 )
	{
		return 5;
	}

	CString sClassName;
	CComboBox* pCombo = ((CComboBox*)GetDlgItem(IDC_COMBO4));
	pCombo->GetLBText( pCombo->GetCurSel() , sCodePattern );
	if( sCodePattern.Compare(_T(" - 선 택 - "))==0 || (sCodePattern.Compare(_T(" - 미수정 - "))==0) )
		sCodePattern = _T("");
	
	if( sCodePattern.IsEmpty() && (m_mode == 1) )
	{
		AfxMessageBox( _T("코드패턴을 입력해주십시오") );
		return 6;
	}

	pCombo = ((CComboBox*)GetDlgItem(IDC_COMBO3));
	pCombo->GetLBText( pCombo->GetCurSel() , sClassName );
	if( sClassName.Compare(_T(" - 선 택 - ")) == 0 )
	{
		AfxMessageBox( _T("등급을 입력해주십시오") );
		return 8;
	}

	if( TakeControlData( _T("이름")).IsEmpty() )
	{
		AfxMessageBox( _T("이름을 입력해주십시오") );
		return 9;
	}

	return 0;

	EFS_END

	return -1;
}


INT CMNG_USER_MANAGER_03::SaveToDB( CString sCodePattern )
{
	EFS_BEGIN

	INT ids;


	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveToDB") );

	pDM->StartFrame();

	if( MakeUserMethod(pDM) )
	{

	}

	CString error;
	ids = SaveUserData(pDM);
	if( ids )
	{
		error.Format( _T("Error %d : SaveUserData()") );
		AfxMessageBox( error );
		return 2;
	}

	if( !sCodePattern.IsEmpty() )
	{
		if( MakeUserCode(pDM) )
		{
			AfxMessageBox( _T("Error : MakeUserCode()") );
			return 3;
		}
	}

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("SaveToDB") );

	pDM->EndFrame();

	UpdateGPKIData();
	UpdateSecuData();
	
	switch( m_mode )
	{
		case 1 :
			AfxMessageBox( _T("사용자 정보가 등록되었습니다.") );
			break;
		case 2 :
			CMNG_USER_MANAGER_01 *pParent = ( CMNG_USER_MANAGER_01 * )m_pParent;
			pParent->ShowGrid( true );
			AfxMessageBox( _T("사용자 정보가 수정되었습니다.") );
			
			break;
	}

	CMNG_USER_MANAGER_01 *pParent = ( CMNG_USER_MANAGER_01 * )pParentMgr;
	pParent->CodeToDesc();
	
	ids = ControlDisplay( _T("CM_MNG_USER_MANAGER_01"), _T("그리드") );
	if( ids )
	{
		AfxMessageBox( _T("Error : controlDisplay()") );
		return 4;
	}
	
	pParent->OnEnterCellmngGRID1();

	if( m_modifyList != NULL )
		ParentGridReverse( m_modifyList[ m_index ] );

	if( 2 == m_mode && m_idxCnt > 1 )
		return 5;	



	return 0;

	EFS_END

	return -1;
}



HBRUSH CMNG_USER_MANAGER_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

VOID CMNG_USER_MANAGER_03::OnBtnPermision()
{
	// TODO: Add your control notification handler code here

	CPermisionCFGDlg cfgDlg(this->m_pParent);
	if( IDOK == cfgDlg.DoModal())
	{
			((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->DeleteAllItems();
		 	m_arrAuthority[0]=_T("");
			m_arrAuthority[1]=_T("");
			m_arrAuthority[2]=_T("");
			m_arrAuthority[3]=_T("");
			m_arrAuthority[4]=_T("");
			m_arrAuthority[5]=_T("");
			m_arrAuthority[6]=_T("");
			m_arrAuthority[7]=_T("");
			m_arrAuthority[8]=_T("");
			m_arrAuthority[9]=_T("");
			m_arrAuthority[10]=_T("");
			m_arrAuthority[11]=_T("");
			m_arrAuthority[12]=_T("");
			m_arrAuthority[13]=_T("");
			m_arrAuthority[14]=_T("");
			m_arrAuthority[15]=_T("");
			m_arrAuthority[16]=_T("");
			m_arrAuthority[17]=_T("");
			m_arrAuthority[18]=_T("");
			m_arrAuthority[19]=_T("");

		this->m_strAuthorityType=cfgDlg.m_strAuthorityType;
		INT i=0;
		while(m_strAuthorityType.Find(_T("@"))!=-1)
		{
			int index = m_strAuthorityType.Find(_T("@"));
			this->m_arrAuthority[i]=m_strAuthorityType.Left(index);
			m_strAuthorityType=m_strAuthorityType.Mid(index+1);
			i++;
		}
	
		CESL_DataMgr* pDM= FindDM( _T("DM_MNG_USER_MANAGER_03_permision"));
		for(i=0;i<20;i++)
		{
			CString str;
			if(m_arrAuthority[i].IsEmpty()) continue;
			str.Format(_T("AUTHORITY='%s'"),m_arrAuthority[i]);
			pDM->RefreshDataManager(str);
			
			int index = ((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->GetItemCount();
			CString number;
			number.Format(_T("%d"),index+1);
			
			((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->InsertItem(index , number);
			((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->SetItemText(index , 1 , pDM->GetCellData(0,1));
			
		}
	}
	else return;
}

INT CMNG_USER_MANAGER_03::GetLoanPermissonValue( CString UserID, CString &value )
{
EFS_BEGIN
	
	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_MANAGER_03"));

	CString query, strTmp;

	query.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='대출자접근권한' AND VALUE_NAME='%s'"), UserID);		
	pDM->GetOneValueQuery( query, value );

	if ( value.IsEmpty() )
	{		
		return 100;
	}

	return 0;
EFS_END
return -1;
}

INT CMNG_USER_MANAGER_03::SetLoanPermissonValue( CString UserID, CString value )
{
EFS_BEGIN
	
	BOOL	bInsert;
	CString strQuery;
	CString strValue;
	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_MANAGER_01"));	

	strQuery.Format(_T("SELECT COUNT(1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS ='대출자접근권한' AND VALUE_NAME='%s'"), UserID );
	pDM->GetOneValueQuery(strQuery, strValue);

	bInsert = FALSE;
	if ( _ttoi(strValue) <= 0 )
	{
		bInsert = TRUE;
	}	

	if ( bInsert )
	{
		CString sRecKey;
		
		if( pDM->m_dbkind == 1 )
			sRecKey = _T("ESL_SEQ.NextVal");
		else
			pDM->MakeRecKey(sRecKey);
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->AddDBFieldData( _T("GROUP_1") , _T("STRING") , _T("L") );
		pDM->AddDBFieldData( _T("GROUP_2") , _T("STRING") , _T("열람") );
		pDM->AddDBFieldData( _T("GROUP_3") , _T("STRING") , _T("공통") );
		pDM->AddDBFieldData( _T("CLASS_ALIAS") , _T("STRING") , _T("대출자접근권한") );
		pDM->AddDBFieldData( _T("CLASS_DESC") , _T("STRING") , _T("대출자접근권한") );
		pDM->AddDBFieldData( _T("VALUE_NAME") , _T("STRING") , UserID );
		pDM->AddDBFieldData( _T("VALUE_1") , _T("STRING") , value );
	
		CString strManageCode;
		GetControlData( _T("CM_MNG_USER_MANAGER_03"), _T("관리구분"), strManageCode );
		pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , strManageCode ); 
		
		pDM->MakeInsertFrame( _T("ESL_MANAGE_TBL") );
	}
	else
	{
		CString query;
		query.Format(_T("UPDATE ESL_MANAGE_TBL SET VALUE_1 = '%s' WHERE CLASS_ALIAS='대출자접근권한' AND VALUE_NAME='%s';"), value, UserID);
		pDM->AddFrame(query);
	}


	return 0;
EFS_END
return -1;
}

void CMNG_USER_MANAGER_03::OnKillfocusEdit1() 
{
	INT nNeedByte;
	CString strData;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);

	pEdit->GetWindowText( strData );

	if( !strData.IsEmpty() )
	{
		nNeedByte = WideCharToMultiByte( CP_UTF8	, 
								0				,
								strData.GetBuffer(0), 
								-1				,
								NULL			, 
								0				,
								NULL			, 
								NULL      );

		if( 18 < nNeedByte-1 )
		{
			AfxMessageBox(_T("유효 글자수를 초과했습니다."));
			pEdit->SetWindowText( _T("") );
			pEdit->SetFocus();
		}
		
	}	
}

INT CMNG_USER_MANAGER_03::CheckPasswordCreateRule(CString strPassword) 
{
	INT ids1 = CLocCommonAPI::CheckUpperEnglish(strPassword);
	INT ids2 = CLocCommonAPI::CheckSpecial(strPassword);
	INT ids3 = CLocCommonAPI::CheckNumber(strPassword);

	if(10 > strPassword.GetLength())
	{
		return -100;
	}
	else if(2 > (ids1+ids2+ids3))
	{
		return -200;
	}
	return 0;
}

void CMNG_USER_MANAGER_03::OnbtnCHANGEPW() 
{
	CMNG_USER_CHANGEPASSWORD dlg(this);
	if(IDOK != dlg.DoModal())
	{
		 return;
	}	
	AfxMessageBox( _T("비밀번호가 변경 되었습니다.") );
}

VOID CMNG_USER_MANAGER_03::OnSelchangeCombo3()
{
	if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("U")) == 0)
		return;	

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO3);
	ASSERT(pCombo != NULL);

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_USERINFO_EXP);
	ASSERT(pBtn != NULL);

	CButton* pBtnSingLog = (CButton*)GetDlgItem(IDC_CHECK_SECU_SINGLOG);
	ASSERT(pBtnSingLog != NULL);

	int nIdx = pCombo->GetCurSel();

	GetDlgItem(IDC_EDIT16)->EnableWindow( (nIdx == 2) );
	pBtn->EnableWindow( (nIdx == 2) );
	pBtnSingLog->EnableWindow( (nIdx == 2) );

	if (m_mode == 1 || m_mode == 2)
	{
		CECO_ControlDateEdit* pCtrl = (CECO_ControlDateEdit*)GetDlgItem(IDC_EDIT16);
		pCtrl->SetData(_T(""));

		if		(nIdx == 1 || nIdx == 3)	pBtn->SetCheck(TRUE);
		else if (nIdx == 0)					pBtn->SetCheck(FALSE);

		if (m_mode == 2 && nIdx == 2)
		{
			ControlDisplay(_T("CM_MNG_USER_MANAGER_03"), _T("유효기간"), m_modifyList[m_index]);

			CESL_DataMgr *pDM = FindDM(_T("DM_MNG_USER_MANAGER_01"));
			CString strExpYN;
			pDM->GetCellData(_T("USERGRID_EXPORT_YN"), m_modifyList[m_index], strExpYN);

			CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_USERINFO_EXP);
			ASSERT(pBtn);
			pBtn->SetCheck( (strExpYN.CompareNoCase(_T("Y")) == 0) );
		}
	}
}

void CMNG_USER_MANAGER_03::OnButtonGpki() 
{
	// TODO: Add your control notification handler code here
	CMNG_USER_MANAGER_03_GPKI_REG dlg(this);

	if(m_mode == 1 && m_strUserUID.Compare(_T("")) == 0)
	{
		dlg.m_nMode = 0;
	}
	else if(m_mode == 1 && m_strUserUID.Compare(_T("")) != 0)
	{
		dlg.m_nMode = 0;
		dlg.m_strUserUID = m_strUserUID;
		dlg.m_strUserKey = m_strUserKey;
		dlg.m_bGPKIDirty = m_bGPKIDirty;
	}
	else if(m_nGPKIMode == 2)
	{
		dlg.m_nMode = 2;
	}
	else
	{
		dlg.m_nMode = 1;
	}

	CString id;
	GetDlgItemText(IDC_EDIT1,id);

	dlg.m_strID = id;
	
	if(dlg.DoModal() == IDOK)
	{		
		m_strUserUID = dlg.m_strUserUID;
		m_strUserKey = dlg.m_strUserKey;
		m_bGPKIDirty = dlg.m_bGPKIDirty;
		m_nGPKIMode = dlg.m_nMode;
		
		if(dlg.m_nMode == 2)
		{
			((CStatic*)GetDlgItem(IDC_STATIC_GPKI_STATE))->SetWindowText(_T("* GPKI 미등록"));
			((CButton*)GetDlgItem(IDC_BUTTON_GPKI))->SetWindowText(_T("GPKI 등록"));
		}
		else 
		{
			((CStatic*)GetDlgItem(IDC_STATIC_GPKI_STATE))->SetWindowText(_T("* GPKI 등록"));
			((CButton*)GetDlgItem(IDC_BUTTON_GPKI))->SetWindowText(_T("GPKI 수정"));
		}		
	}	
}

int CMNG_USER_MANAGER_03::UpdateSecuData()
{
	int ids = 0;	

EFS_BEGIN

	CButton* pBtnTimeOut = (CButton*)GetDlgItem(IDC_CHECK_SECU_TIMEOUT);	
	int nChkTimeOut = pBtnTimeOut->GetCheck();

	CButton* pBtnSingLogin = (CButton*)GetDlgItem(IDC_CHECK_SECU_SINGLOG);	
	int nChkSingLogin = pBtnSingLogin->GetCheck();

	CString id;
	GetDlgItemText(IDC_EDIT1,id);

	if(id.Compare(_T("")) == 0)
		return 0;
	
	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_MANAGER_01"));
					
	ids = pDM->StartFrame();

	CString strTimeout;
	strTimeout.Format(_T("%d"), m_nTimeout);
			
	CString strQuery = _T("UPDATE MN_USER_TBL ");
	strQuery += _T("SET SECU_TIMEOUT = '");				
	strQuery += nChkTimeOut ? strTimeout : _T("");
	strQuery += _T("',");
	strQuery += _T("SECU_LOGIN_IP = '");				
	strQuery += nChkSingLogin ? m_strUserIP : _T("");
	strQuery += _T("' WHERE ID = '");
	strQuery += id;
	strQuery += _T("' ;");
					
	ids = pDM->AddFrame(strQuery);
	ids = pDM->SendFrame();
	ids = pDM->EndFrame();	

	if( 2 == m_mode )
	{				
		pDM->SetCellData( _T("SECU_TIMEOUT"), nChkTimeOut ? strTimeout : _T(""), m_modifyList[ m_index ]);
		pDM->SetCellData( _T("SECU_LOGIN_IP"), nChkSingLogin ? m_strUserIP : _T(""), m_modifyList[ m_index ]);
	}
	else
	{
		pDM->SetCellData( _T("SECU_TIMEOUT"), nChkTimeOut ? strTimeout : _T(""), pDM->GetRowCount()-1);
		pDM->SetCellData( _T("SECU_LOGIN_IP"), nChkSingLogin ? m_strUserIP : _T(""), pDM->GetRowCount()-1);
	}
	
EFS_END

	return ids;
}

int CMNG_USER_MANAGER_03::UpdateGPKIData()
{
	int ids = 0;

	if(!m_bGPKIDirty && !m_bGPKIUSEDirty )
		return 0;

EFS_BEGIN

	CString id;
	GetDlgItemText(IDC_EDIT1,id);

	if(id.Compare(_T("")) == 0)
		return 0;
	
	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_MANAGER_03"));
					
	ids = pDM->StartFrame();

	CString strGPKIUse = _T("N");

	if(((CButton*)GetDlgItem(IDC_chkGPKI))->GetCheck())
		strGPKIUse = _T("Y");
			
	CString strQuery = _T("UPDATE MN_USER_TBL ");
	strQuery += _T("SET ");
	if(m_bGPKIDirty)
	{
		strQuery += _T("GPKI_UID_VALUE = '");				
		strQuery += m_strUserUID;
		strQuery += _T("',");
		strQuery += _T("GPKI_KEY_VALUE = '");				
		strQuery += m_strUserKey;
		strQuery += _T("'");
	}
	if(m_bGPKIDirty && m_bGPKIUSEDirty)
	{
		strQuery += _T(",");
	}
	if(m_bGPKIUSEDirty)
	{
		strQuery += _T("GPKI_USE = '");				
		strQuery += strGPKIUse;
		strQuery += _T("'");
	}
	strQuery += _T(" WHERE ID = '");
	strQuery += id;
	strQuery += _T("' ;");
					
	ids = pDM->AddFrame(strQuery);
	ids = pDM->SendFrame();
	ids = pDM->EndFrame();

	m_bGPKIDirty = FALSE;
	m_bGPKIUSEDirty = FALSE;
	
EFS_END

	return ids;
}

void CMNG_USER_MANAGER_03::OnCheckSecuSinglog() 
{
	// TODO: Add your control notification handler code here	
	CButton* pBtnSingLogin = (CButton*)GetDlgItem(IDC_CHECK_SECU_SINGLOG);	
	int nChkSingLogin = pBtnSingLogin->GetCheck();

	if(nChkSingLogin)
	{		
		CUserRegIPDlg dlg;
		if(m_mode == 3)
			dlg.m_bReadOnly = TRUE;
		
		dlg.m_strUserIP = m_strUserIP;
		if(dlg.m_strUserIP.Compare(_T("")) == 0)
		{				
			dlg.m_strUserIP = m_strOldUserIP;
			m_strUserIP = dlg.m_strUserIP;
		}

		if(dlg.DoModal() == IDOK)
		{
			m_strUserIP = dlg.m_strUserIP;
			m_strOldUserIP = m_strUserIP;
		}		
	}
	else
	{
		if(m_strUserIP.Compare(_T("")) != 0)
		{
			m_strOldUserIP = m_strUserIP;
		}
		m_strUserIP = _T("");
	}
}
void CMNG_USER_MANAGER_03::OnCheckSecuTimeout() 
{
	// TODO: Add your control notification handler code here

	CButton* pBtnTimeout = (CButton*)GetDlgItem(IDC_CHECK_SECU_TIMEOUT);	
	int nChkTimeout = pBtnTimeout->GetCheck();

	if(nChkTimeout)
	{
		CUserRegTimeoutDlg dlg;
		if(m_mode == 3)
			dlg.m_bReadOnly = TRUE;
		
		dlg.m_nTimeout = m_nTimeout;
		if(dlg.DoModal() == IDOK)
		{
			m_nTimeout = dlg.m_nTimeout;
		}		
	}
}

void CMNG_USER_MANAGER_03::CompareAuthority(ST_USER_AUTHORITY &stOriUserAuthority, ST_USER_AUTHORITY &stDesUserAuthority)
{
	BOOL bModify = FALSE, bFirst = TRUE;
	CString strContent = _T(""), strTemp = _T("");
	if(stOriUserAuthority.strPermit.Compare(stDesUserAuthority.strPermit) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		if(MODIFY_INFO)
			strTemp.Format(_T("사용자 등급 변경([%s]에서[%s])"), stOriUserAuthority.strPermit, stDesUserAuthority.strPermit);
		else
			strTemp.Format(_T("사용자 등급 변경"));
		stOriUserAuthority.strPermit = stDesUserAuthority.strPermit;
		strContent += strTemp;
	}	

	if(stOriUserAuthority.strLoanPermit.Compare(stDesUserAuthority.strLoanPermit) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		if(MODIFY_INFO)
			strTemp.Format(_T("이용자 관리권한 변경([%s]에서[%s])"), stOriUserAuthority.strLoanPermit, stDesUserAuthority.strLoanPermit);
		else
			strTemp.Format(_T("이용자 관리권한 변경"));
		stOriUserAuthority.strLoanPermit = stDesUserAuthority.strLoanPermit;
		strContent += strTemp;
	}	
  
	CString strDesUserAuthList = _T(""), strOriUserAuthList = _T(""), strAuthTemp = _T("");

	if(stOriUserAuthority.nMenuCount == stDesUserAuthority.nMenuCount)
	{
		for(int i=0; i<stOriUserAuthority.nMenuCount; i++)
		{
			if(stOriUserAuthority.pStrMenuPermit[i].Compare(stDesUserAuthority.pStrMenuPermit[i]) != 0)
			{		
				if ( MODIFY_INFO )
				{
					if ( strAuthTemp.IsEmpty() ) strAuthTemp.Format(_T(" %s -> %s "), stOriUserAuthority.pStrMenuPermit[i], stDesUserAuthority.pStrMenuPermit[i]);
					else strAuthTemp += _T(", ") + stOriUserAuthority.pStrMenuPermit[i] + _T(" -> ") + stDesUserAuthority.pStrMenuPermit[i];
				}
				else
				{
					if ( strAuthTemp.IsEmpty() ) strAuthTemp.Format(_T(" %s "), stDesUserAuthority.pStrMenuPermit[i]);
					else strAuthTemp += _T(", ") + stOriUserAuthority.pStrMenuPermit[i];
				}
				
			}
		}
			bModify = TRUE;
			if(!bFirst)
			{
				strContent += _T(", ");
			}
			else
			{
				bFirst = FALSE;
			}

			if(!strAuthTemp.IsEmpty()) strTemp.Format(_T("사용자 메뉴 접근권한 변경 [%s] "), strAuthTemp );
			strContent += strTemp;
			

			if(stOriUserAuthority.pStrMenuPermit)
			{
				delete [] stOriUserAuthority.pStrMenuPermit;
				stOriUserAuthority.pStrMenuPermit = NULL;
			}

			stOriUserAuthority.pStrMenuPermit = stDesUserAuthority.pStrMenuPermit;
			stDesUserAuthority.pStrMenuPermit = NULL;
	}
	else
	{	
		if ( MODIFY_INFO )
		{		
			for( int i=0; i<stOriUserAuthority.nMenuCount; i++)
			{
				if(i == 0) strOriUserAuthList += stOriUserAuthority.pStrMenuPermit[i];
				else strOriUserAuthList += _T(", ") + stOriUserAuthority.pStrMenuPermit[i];		
			}
		}
		
		for( int j=0; j<stDesUserAuthority.nMenuCount; j++)
		{
			if(j == 0) strDesUserAuthList += stDesUserAuthority.pStrMenuPermit[j];
			else strDesUserAuthList += _T(", ") + stDesUserAuthority.pStrMenuPermit[j];		
		}

		stOriUserAuthority.nMenuCount = stDesUserAuthority.nMenuCount;

		if(stOriUserAuthority.pStrMenuPermit)
		{
			delete [] stOriUserAuthority.pStrMenuPermit;
			stOriUserAuthority.pStrMenuPermit = NULL;
		}
		

		stOriUserAuthority.pStrMenuPermit = stDesUserAuthority.pStrMenuPermit;
		stDesUserAuthority.pStrMenuPermit = NULL;

		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		
		if(MODIFY_INFO)
		{

			strTemp.Format(_T("사용자 메뉴 접근권한 변경 [%s] 에서 [%s] "), strOriUserAuthList, strDesUserAuthList);
			strContent += strTemp;
		}
		else
		{
			strTemp.Format(_T("사용자 메뉴 접근권한 변경 [ %s ]"), strDesUserAuthList );
			strContent += strTemp;				
		}
	}	

	if(stOriUserAuthority.bExportExcel != stDesUserAuthority.bExportExcel)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		if(stDesUserAuthority.bExportExcel)
		{
			strTemp.Format(_T("그리드 엑셀 반출 허용으로 변경"));
		}
		else
		{
			strTemp.Format(_T("그리드 엑셀 반출 제한으로 변경"));
		}

		stOriUserAuthority.bExportExcel = stDesUserAuthority.bExportExcel;

		strContent += strTemp;
	}
	


	if(stOriUserAuthority.nTimeout != stDesUserAuthority.nTimeout)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		if(stDesUserAuthority.nTimeout == 0)
		{
			strTemp.Format(_T("자동잠김 미사용으로 변경"));
		}
		else
		{
			if(MODIFY_INFO)
				strTemp.Format(_T("자동잠김 시간([%d]에서[%d])"), stOriUserAuthority.nTimeout, stDesUserAuthority.nTimeout);
			else
				strTemp.Format(_T("자동잠김 시간변경"));
		}

		stOriUserAuthority.nTimeout = stDesUserAuthority.nTimeout;

		strContent += strTemp;
	}

	if(stOriUserAuthority.strIPPermit.Compare(_T("")) == 0)
	{
		stOriUserAuthority.strIPPermit.Format(_T("0.0.0.0;0.0.0.0;0.0.0.0;"));
	}
	if(stDesUserAuthority.strIPPermit.Compare(_T("")) == 0)
	{
		stDesUserAuthority.strIPPermit.Format(_T("0.0.0.0;0.0.0.0;0.0.0.0;"));		
	}

	if(stOriUserAuthority.strIPPermit.Compare(stDesUserAuthority.strIPPermit) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		if(stDesUserAuthority.strIPPermit.Compare(_T("0.0.0.0;0.0.0.0;0.0.0.0;")) == 0)
		{
			strTemp.Format(_T("접속 제한 미사용으로 변경"));
		}
		else
		{
			if(MODIFY_INFO)
				strTemp.Format(_T("접속 허용 IP 변경([%s]에서[%s])"), stOriUserAuthority.strIPPermit, stDesUserAuthority.strIPPermit);
			else
				strTemp.Format(_T("접속 허용 IP 변경"));
		}

		stOriUserAuthority.strIPPermit = stDesUserAuthority.strIPPermit;

		strContent += strTemp;
	}
	


	if(stOriUserAuthority.bPrintPermit != stDesUserAuthority.bPrintPermit)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		if(stDesUserAuthority.bPrintPermit)
		{
			strTemp.Format(_T("이용자 정보출력 허용으로 변경"));
		}
		else
		{
			strTemp.Format(_T("이용자 정보출력 제한으로 변경"));
		}

		stOriUserAuthority.bPrintPermit = stDesUserAuthority.bPrintPermit;
		strContent += strTemp;		
	}
	
	
	if(bModify)
	{		
		CString UserID, UserName;	
		UserID = TakeControlData(_T("id"));
		UserName = TakeControlData(_T("이름"));
		CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_03") );
		CString query, strResult;
		query.Format(_T("SELECT REC_KEY FROM MN_USER_TBL WHERE ID='%s'"), UserID );
		pDM->GetOneValueQuery(query, strResult);
		
		SetAuthLog( 1, strResult, UserName, strContent);
	}
}

void CMNG_USER_MANAGER_03::GetAuthorityData(ST_USER_AUTHORITY &stUserAuthority)
{
	((CComboBox*)GetDlgItem(IDC_COMBO3))->GetWindowText(stUserAuthority.strPermit);
	((CComboBox*)GetDlgItem(IDC_COMBO6))->GetWindowText(stUserAuthority.strLoanPermit);

	stUserAuthority.bExportExcel = ((CButton*)GetDlgItem(IDC_CHK_USERINFO_EXP))->GetCheck();
	stUserAuthority.bPrintPermit = ((CButton*)GetDlgItem(IDC_CHECK_SECU_INFOPRINT))->GetCheck();

	if(!((CButton*)GetDlgItem(IDC_CHECK_SECU_TIMEOUT))->GetCheck())
	{
		stUserAuthority.nTimeout = 0;
	}
	else
	{
		stUserAuthority.nTimeout = m_nTimeout;
	}

	if(!((CButton*)GetDlgItem(IDC_CHECK_SECU_SINGLOG))->GetCheck())
	{
		stUserAuthority.strIPPermit = _T("");
	}
	else
	{
		stUserAuthority.strIPPermit = m_strUserIP;
	}

	if(stUserAuthority.pStrMenuPermit)
	{
		delete [] stUserAuthority.pStrMenuPermit;
		stUserAuthority.pStrMenuPermit = NULL;
	}
	
	int index = ((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->GetItemCount();
	
	stUserAuthority.nMenuCount = index;

	stUserAuthority.pStrMenuPermit = new CString[index];	
	
	for(int i=0; i<index; i++)
	{
		stUserAuthority.pStrMenuPermit[i] = ((CListCtrl*)GetDlgItem(IDC_LIST_AUTHOR))->GetItemText(i, 1);
	}
}

void CMNG_USER_MANAGER_03::SetAuthLog(int nType, CString strKey, CString strName, CString strContent)
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_AUTH_LOG") );

	const int nCnt = 10;

	CString field[nCnt] = {
		_T("REC_KEY"),
		_T("TYPE"),
		_T("WORKER_IP"),
		_T("WORKER_ID"),
		_T("WORKER_NAME"),
		_T("USER_KEY"), 
		_T("WORK_DATE"),
		_T("FIRST_WORK"),
		_T("WORKER_SYS"),
		_T("WORKER_DO") };	

	CString type[nCnt] = {
		_T("NUMERIC"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("NUMERIC"),
		_T("NUMERIC"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING") };

	CString data[nCnt];
	if( pDM->m_dbkind == 1 )
	{
		data[0] = _T("ESL_SEQ.NextVal");
	}
	else
	{
		pDM->MakeRecKey( data[0] );
	}

	if(nType == 0)
	{
		data[1].Format(_T("I"));
	}
	else if(nType == 1)
	{
		data[1].Format(_T("U"));
	}
	else if(nType == 2)
	{
		data[1].Format(_T("D"));
	}

	data[2] = m_pInfo->LOCAL_IP;
	data[3] = m_pInfo->USER_ID;
	data[4] = m_pInfo->USER_NAME;
	data[5].Format(_T("%s"),strKey);
	data[6].Format(_T("SYSDATE"));
	data[7] = GetWorkLogMsg( _T("유저관리"), __WFILE__, __LINE__ );
	data[8] = _T("KOLASIII");
	data[9].Format(_T("%s"),strContent);

	pDM->StartFrame();
	
	pDM->InitDBFieldData();	
	for(int i = 0; i< nCnt; i++)
	{
		if( data[i].GetLength() )
			pDM->AddDBFieldData( field[i], type[i], data[i] );
	}

	int nRtn = pDM->MakeInsertFrame( _T("MN_USER_AUTH_LOG_TBL") );

	pDM->SendFrame();
	pDM->EndFrame();	

	EFS_END
}

void CMNG_USER_MANAGER_03::OnchkGPKI() 
{
	// TODO: Add your control notification handler code here
	m_bGPKIUSEDirty = TRUE;
}

void	_cTreeCtrl_GetAllText( CTreeCtrl* pTree , HTREEITEM hCurrItem , CStringArray& str_ar )
{
	while( hCurrItem )
	{
		if( pTree->ItemHasChildren( hCurrItem ) )
		{
			_cTreeCtrl_GetAllText( pTree , pTree->GetChildItem( hCurrItem ) , str_ar );
		}
		hCurrItem = pTree->GetNextSiblingItem( hCurrItem );
		CString txt =	pTree->GetItemText( hCurrItem );
		str_ar.Add( txt );
	}
}

void	_CtrlText_ToDM( CWnd* pPar , CESL_DataMgr* pDM , CString field_name , int ctrl_id )
{
	CString txt;
	CWnd* pWnd = pPar->GetDlgItem(ctrl_id);
	pWnd->GetWindowText( txt );
	pDM->SetCellData( field_name, txt ,0 );
}

void	_CtrlText_ToDM_ComboBox( CWnd* pPar , CESL_DataMgr* pDM , CString field_name , int ctrl_id )
{
	CString txt;
	CComboBox* pCombo = (CComboBox*)pPar->GetDlgItem(ctrl_id);
	pCombo->GetLBText(pCombo->GetCurSel(), txt);
	pDM->SetCellData( field_name, txt ,0 );
}

void	_CtrlText_ToDM_CheckBotton( CWnd* pPar , CESL_DataMgr* pDM , CString field_name , int ctrl_id )
{
	CString txt;
	CButton* pBtn = (CButton*)pPar->GetDlgItem(ctrl_id);	
	if(	pBtn->GetCheck() )
	{
		txt = _T("Y");
	}else{
		txt = _T("N");
	}
	pDM->SetCellData( field_name, txt ,0 );
}

CString		CheckDM_DifferentData( CESL_DataMgr* pS , CESL_DataMgr* pT ) 
{
	int i;
	CString		result_str;
	CString		data_s , data_t;
	int c_alias = pS->RefList.GetCount();
	CString* alias_str	= new CString[c_alias];
	pS->GetAllAlias(alias_str, c_alias);
	for( i = 0 ; i < c_alias ; i++ )
	{
		pS->GetCellData( alias_str[i] ,0 , data_s );
		pT->GetCellData( alias_str[i] ,0 , data_t );
		if( data_s != data_t )
		{
			if( result_str.GetLength() > 0 ) result_str += _T(",");			
			result_str += alias_str[i];
		}
	}
	delete [] alias_str;
	OutputDebugString( result_str + _T("\n") );
	return result_str;
}


void	CMNG_USER_MANAGER_03::DlgInfo_ToDM(CESL_DataMgr* pDM)
{
	int i;
	CString val;

	int ref_count = 18;

	CString		alias_ar[] = { 
		_T("코드패턴")	, _T("메뉴접근권한")	, _T("대출자접근권한")	, _T("환경설정")	, _T("유저권한")	,	// 5
		_T("사용자ID")	, _T("비밀번호변경")	, _T("이름")			, _T("부서")		, _T("직급")		,	// 10
		_T("관리구분")	, _T("전화번호")		, _T("내선번호")		, _T("이메일")		, _T("자동잠김기능"),	// 15
		_T("접속제한")	, _T("이용자정보 출력/반출 허용") , _T("비고")												// 18
	};

	pDM->InitDataMgrRef(ref_count);
	for(i = 0 ;i < ref_count ; i++ )
	{
		pDM->SetDataMgrRef(i,	alias_ar[i],alias_ar[i],_T("str"),	_T(""),	_T(""));
	}

	pDM->FreeData();

	pDM->InsertRow(-1);

	m_codePattern.GetLBText(m_codePattern.GetCurSel(), val);
	pDM->SetCellData( _T("코드패턴"), val ,0 );

	CESL_DataMgr* pDM3 = FindDM(_T("DM_MNG_USER_MANAGER_03_permision"));
	ASSERT(pDM3);
	val = _T("");
	{
		CString alias;
		for(INT i=0;i<20;i++)
		{
			CString str;
			if(m_arrAuthority[i].IsEmpty()) continue;
			str.Format(_T("AUTHORITY='%s'"),m_arrAuthority[i]);
			pDM3->RefreshDataManager(str);
	
			if( alias.GetLength() > 0 ) alias += _T("|");
			alias += pDM3->GetCellData(0,1);
		}

		val = _T("[") + alias + _T("]");
		pDM->SetCellData( _T("메뉴접근권한"), val ,0 );
	}	

	_CtrlText_ToDM_ComboBox( this,pDM,_T("대출자접근권한")	,IDC_COMBO6 );
	_CtrlText_ToDM_ComboBox( this,pDM,_T("환경설정")		,IDC_COMBO5 ); //?
	_CtrlText_ToDM_ComboBox( this,pDM,_T("유저권한")		,IDC_COMBO3 );

	_CtrlText_ToDM(this,pDM, _T("사용자ID")		, IDC_EDIT1 );
	_CtrlText_ToDM(this,pDM, _T("비밀번호변경") , IDC_EDIT2 );
	_CtrlText_ToDM(this,pDM, _T("이름")			, IDC_EDIT4 );

	_CtrlText_ToDM_ComboBox( this,pDM,_T("부서")		,IDC_COMBO1 );
	_CtrlText_ToDM_ComboBox( this,pDM,_T("직급")		,IDC_COMBO2 );
	_CtrlText_ToDM_ComboBox( this,pDM,_T("관리구분")	,IDC_COMBO7 );

	_CtrlText_ToDM(this,pDM, _T("전화번호")		, IDC_EDIT11 );
	_CtrlText_ToDM(this,pDM, _T("내선번호")		, IDC_EDIT13 );
	_CtrlText_ToDM(this,pDM, _T("이메일")		, IDC_EDIT14 );

	_CtrlText_ToDM_CheckBotton( this,pDM, _T("자동잠김기능")		, IDC_CHECK_SECU_TIMEOUT );
	_CtrlText_ToDM_CheckBotton( this,pDM, _T("접속제한")			, IDC_CHECK_SECU_SINGLOG );
	_CtrlText_ToDM_CheckBotton( this,pDM, _T("이용자정보 출력/반출 허용")	, IDC_CHK_USERINFO_EXP );

	_CtrlText_ToDM(this,pDM, _T("비고")			, IDC_EDIT15 );
}


void CMNG_USER_MANAGER_03::Insert_UserEditLogTable()
{
	CString	sLog_Detail;
	sLog_Detail = CheckDM_DifferentData( &m_cDM_DetailLog_Src , &m_cDM_DetailLog_Modify );
	CString strQuery;
	strQuery.Format(_T("INSERT INTO CO_LOAN_USER_EDIT_TBL")
			_T("(REC_KEY,TYPE,WORKER_IP,WORKER_ID,WORKER_NAME,WORKER_SYS,USER_KEY,WORK_DATE,FIRST_WORK,LOG_DETAIL,USER_TYPE) ")
			_T("VALUES(ESL_SEQ.NEXTVAL,'U','%s','%s','%s','KOLASIII',%s,SYSDATE,'%s' , '%s' , '%s' );")
			, pParentMgr->m_pInfo->LOCAL_IP
			, pParentMgr->m_pInfo->USER_ID
			, pParentMgr->m_pInfo->USER_NAME
			, m_userKey
			, pParentMgr->GetWorkLogMsg(_T("사용자수정"),__WFILE__,__LINE__)
			, sLog_Detail
			, _T("A")
			);

	m_cDM_DetailLog_Modify.SetCONNECTION_INFO( pParentMgr->m_pInfo->CONNECTION_INFO );

	m_cDM_DetailLog_Modify.StartFrame();
	m_cDM_DetailLog_Modify.AddFrame(strQuery);
	m_cDM_DetailLog_Modify.SendFrame();
	m_cDM_DetailLog_Modify.EndFrame();

}
