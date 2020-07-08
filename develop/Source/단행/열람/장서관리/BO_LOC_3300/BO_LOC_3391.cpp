// BO_LOC_3391.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3391.h"
#include "..\\..\\..\\..\\공통\\SearchZipCodeAPI\\SearchZipCodeDlg.h"
#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3391 dialog


CBO_LOC_3391::CBO_LOC_3391(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3391)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pSpfWork = NULL;
	m_sIsUnityLoanService = _T("");
	m_nMode = 0;
	m_bIsDoModal = FALSE;
	m_nViewMode = 0;
	m_nCurrentIndex = 0;
	m_nFirstIndex = 0;
	m_nLastIndex = 0;
	bFirstView = FALSE;
		
}

CBO_LOC_3391::~CBO_LOC_3391()
{
	if( m_pSpfWork != NULL )
		delete m_pSpfWork;
	m_pSpfWork = NULL;
}


VOID CBO_LOC_3391::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3391)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3391, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3391)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnFilePREV, OnbtnFilePREV)
	ON_BN_CLICKED(IDC_btnFileNEXT, OnbtnFileNEXT)
	ON_BN_CLICKED(IDC_btnFileSAVE, OnbtnFileSAVE)
	ON_BN_CLICKED(IDC_btnFileCLOSE, OnbtnFileCLOSE)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnFileDUPLICATE1, OnbtnFileDUPLICATE1)
	ON_BN_CLICKED(IDC_btnFileDUPLICATE2, OnbtnFileDUPLICATE2)
	ON_BN_CLICKED(IDC_btnDUPLICATE4, OnbtnDUPLICATE4)
	ON_BN_CLICKED(IDC_btnFileFIND1, OnbtnFileFIND1)
	ON_BN_CLICKED(IDC_btnFileFIND2, OnbtnFileFIND2)
	ON_EN_SETFOCUS(IDC_edtFileUSER21, OnSetfocusedtFileUSER21)
	ON_EN_SETFOCUS(IDC_edtFileUSER4, OnSetfocusedtFileUSER4)
	ON_EN_SETFOCUS(IDC_edtFileUSER16, OnSetfocusedtFileUSER16)
	ON_EN_SETFOCUS(IDC_edtFileUSER13, OnSetfocusedtFileUSER13)
	ON_EN_SETFOCUS(IDC_edtFileUSER10, OnSetfocusedtFileUSER10)
	ON_EN_SETFOCUS(IDC_edtFileUSER22, OnSetfocusedtFileUSER22)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3391 message handlers

BOOL CBO_LOC_3391::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_3391")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new BL_LOC_USER_PROC(this , m_pLoanShareManager );

	CLocCommonAPI::MakeHPCorpNumList( GetDlgItem( IDC_cmbUSER5 ) );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3391::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);

	bFirstView = FALSE;
	SetScreen();
	bFirstView = TRUE;


	EFS_END
}


VOID CBO_LOC_3391::OnbtnFileCLOSE()
{
	EFS_BEGIN

	if( m_bIsDoModal ) OnCancel();
	else OnClose();

	EFS_END
}

VOID CBO_LOC_3391::SetMode( INT nMode , bool IsDoModal/*=FALSE*/)
{
	EFS_BEGIN

	m_nMode = nMode;
	m_bIsDoModal = IsDoModal;

	EFS_END
}

VOID CBO_LOC_3391::SetScreen()
{
	EFS_BEGIN

	if( m_nMode == 0 )
	{
		const INT nCnt = 9;
		INT nUnVisibleGroup[nCnt] = 
		{
			IDC_btnFileDUPLICATE1 , IDC_btnFileDUPLICATE2 , IDC_STATIC_PASS , IDC_edtFileUSER5 , IDC_STATIC_PASS_CONFIRM , 
			IDC_edtFileUSER6 , IDC_btnFileFIND1 , IDC_btnFileFIND2 , IDC_btnFileSAVE
		};

		for( INT i = 0 ; i < nCnt ; i++ )
		{
			GetDlgItem( nUnVisibleGroup[i] )->ShowWindow( SW_SHOW );
		}

		SetWindowText( _T("수정") );
	}
	else if( m_nMode == 1 )
	{
		SetWindowText( _T("상세보기") );
	}
	SetCurrentIndex();
	DisplayWindow();

	EFS_END
}


VOID CBO_LOC_3391::OnbtnFileDUPLICATE1() 
{
	EFS_BEGIN

	INT ids;
	bool IsDuplicate = false;
	ids = IsDuplicateUserNo( IsDuplicate );	
	if( ids < 0 ) return ;
	if( ids > 0 ) return;

	if( IsDuplicate )
	{
		AfxMessageBox( _T("중복된 대출자 번호입니다. 다른 번호를 입력하십시오") );
	}
	else
	{
		AfxMessageBox( _T("중복된 대출자 번호가 존재하지 않습니다. ") );
	}

	EFS_END
}

INT CBO_LOC_3391::IsDuplicateUserNo( bool &IsDuplicate )
{
	EFS_BEGIN

	INT ids;
	CString sSql;

	CString sGetData;
	ids = GetControlData( _T("CM_BO_LOC_3391") , _T("대출자번호") , sGetData );
	if( ids < 0 ) return -1;	

	if( sGetData.IsEmpty() )
	{
		AfxMessageBox( _T("대출자 번호를 입력하시고 중복 조회 기능을 사용하십시오") );
		IsDuplicate = FALSE;
		return 2;
	}
	
	sSql.Format( _T("SELECT count(*) FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'") , sGetData );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3390") );
	if( pDM == NULL ) return -1;

	CString sResult;
	ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sResult );
	if( ids < 0 ) return -1;

	if( sResult.Compare(_T("0")) != 0 ) IsDuplicate = true;

	return 0;	

	EFS_END
	return -1;

}


VOID CBO_LOC_3391::OnbtnFileDUPLICATE2() 
{
	EFS_BEGIN

	INT ids;
	bool IsDuplicate = false;
	ids = IsDuplicateUserID( IsDuplicate );	
	if( ids < 0 ) return ;
	if( ids > 0 ) return;

	if( IsDuplicate )
	{
		AfxMessageBox( _T("중복된 대출자 ID입니다. 다른 번호를 입력하십시오") );
	}
	else
	{
		AfxMessageBox( _T("중복된 대출자 ID가 존재하지 않습니다. ") );
	}	

	EFS_END
}


INT CBO_LOC_3391::IsDuplicateUserID( bool &IsDuplicate )
{
	EFS_BEGIN

	INT ids;
	CString sSql;

	CString sGetData;
	ids = GetControlData( _T("CM_BO_LOC_3391") , _T("사용자ID") , sGetData );
	if( ids < 0 ) return -1;	

	if( sGetData.IsEmpty() )
	{
		AfxMessageBox( _T("대출자 ID를 입력하시고 중복 조회 기능을 사용하십시오") );
		IsDuplicate = FALSE;
		return 2;
	}
	
	sSql.Format( _T("SELECT count(*) FROM CO_LOAN_USER_TBL WHERE USER_ID = '%s'") , sGetData );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3390") );
	if( pDM == NULL ) return -1;

	CString sResult;
	ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sResult );
	if( ids < 0 ) return -1;

	if( sResult.Compare(_T("0")) != 0 ) IsDuplicate = true;

	return 0;	

	EFS_END
	return -1;

}


INT CBO_LOC_3391::DisplayWindow()
{
	EFS_BEGIN

	INT ids;

	ids = ViewScreenLinkDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::DisplayWindow()") );
	
	ids = SetSelect();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3320::DisplayWindow()") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3391::SetCurrentIndex()
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = m_nCurrentIndex;

	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();
	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;

	if( m_nViewMode == 0 )
	{
		m_nCurrentIndex = pGrid->GetIdx();
		m_nLastIndex = pGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}
	else
	{
		m_nLastIndex = pGrid->SelectGetTailPosition();
		m_nFirstIndex = pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCount-1 ; i++ )
		{
			if( nTmpIndex == pGrid->SelectGetIdx() )
				break;
			pGrid->SelectGetNext();
		}
		if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
		else m_nCurrentIndex = pGrid->SelectGetIdx();
	}
	
	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3391::SetPreNextButton()
{
	EFS_BEGIN

	GetDlgItem(IDC_btnFilePREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnFileNEXT)->EnableWindow(TRUE);

	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btnFilePREV)->EnableWindow(FALSE);
	}
	if( m_nCurrentIndex == m_nLastIndex )
	{
		GetDlgItem(IDC_btnFileNEXT)->EnableWindow(FALSE);
	}

	EFS_END
}

VOID CBO_LOC_3391::OnbtnFilePREV() 
{
	EFS_BEGIN

	MoveIndex(0);	
	DisplayWindow();

	EFS_END
}


VOID CBO_LOC_3391::OnbtnFileNEXT() 
{
	EFS_BEGIN

	MoveIndex(1);	
	DisplayWindow();

	EFS_END
}


INT CBO_LOC_3391::ViewScreenLinkDM()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3391" );

	CESL_ControlMgr *pChildCM = FindCM( sCMAlias );
	if( pChildCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_3320::ViewScreenLinkDM()") );

	ids = SettingDisplayBefore();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3320::ViewScreenLinkDM()") );

	ids = pChildCM->AllControlDisplay(m_nCurrentIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::ViewScreenLinkDM()") );

	ids = SettingDisplayAfter();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3320::ViewScreenLinkDM()") );

	SetPreNextButton();

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3391::ViewCodeToDesc()
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_3391");
	CString sDMAlias = _T("DM_BO_LOC_3390");

	
	const INT nFieldCnt = 2;
	TCHAR *sDMFieldAlias[nFieldCnt] =
	{
		_T("우편발송지역") , _T("회원구분") 
	};
	
	CString sGetData[nFieldCnt];

	for( INT i = 0 ; i < nFieldCnt ; i++ )
	{
		ids = GetDataMgrData( sDMAlias , sDMFieldAlias[i] , sGetData[i] , m_nCurrentIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3320::ViewCodeToDesc()") );
	}

	if( sGetData[0].Compare(_T("H"))==0)
		sGetData[0] = _T("0");
	else if( sGetData[0].Compare(_T("W"))==0)
		sGetData[0] = _T("1");

	if( sGetData[1].Compare(_T("정회원"))==0)
		sGetData[1] = _T("0");
	else if( sGetData[1].Compare(_T("비회원"))==0)
		sGetData[1] = _T("1");
	else if( sGetData[1].Compare(_T("준회원"))==0)
		sGetData[1] = _T("2");


	INT nCMIDC[nFieldCnt] =
	{
		IDC_cmbFileUSER3 , IDC_cmbFileUSER4
	};

	for( i = 0 ; i < nFieldCnt ; i++ )
	{
		((CComboBox*)GetDlgItem( nCMIDC[i] ))->SetCurSel( _ttoi(sGetData[i]) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::ViewCodeToDesc()") );

	}

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3391::ChangeSUMFieldToControl( TCHAR **sControlField , CString sSeperator , CString sData , INT nFieldCnt )
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_3391");
	CString sDMAlias = _T("DM_BO_LOC_3390");
	
	if( !sData.IsEmpty() )
	{
		CString sTmp;
		sTmp = sData;
		CString sLine;
		INT nSIndex=0;
		INT i = 0;

		while(true)
		{
			nSIndex = sTmp.Find( sSeperator , 0 );
			if( nSIndex < 0 && sTmp.IsEmpty() ) break;
			if( nSIndex > -1 ) sLine = sTmp.Mid( 0 , nSIndex );
			else
			{
				sLine = sTmp;
				sTmp.Empty();
			}

			if( i== 0 )
			{
				CLocCommonAPI::SetPhoneNumCel( GetDlgItem( IDC_cmbFileUSER5 ), sLine );				
			}
			else 
			{
				ids = SetControlData( sCMAlias , sControlField[i] , sLine );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeSUMFieldToControl( TCHAR **sControlField , CString sSeperator , CString sData , INT nFieldCnt )") );
			}

			sLine.Empty();
			sTmp = sTmp.Mid( nSIndex+1 );
			i++;
		}
	}
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3391::SettingEmail()
{
	EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_3391");
	CString sDMAlias = _T("DM_BO_LOC_3390");

	
	TCHAR *sDMFieldAlias =
	{
		_T("E_MAIL")
	};
	
	CString sGetData;

	ids = GetDataMgrData( sDMAlias , sDMFieldAlias , sGetData , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3320::ViewCodeToDesc()") );

	TCHAR *sCMEmailFieldAlias[2] = 
	{
		_T("E_MAIL1") , _T("E_MAIL2") 
	};

	if( sGetData.IsEmpty() ) return 0;

	CString sTmp = sGetData;
	CString sLine;
	INT nSIndex=0;
	INT i = 0;

	while(true)
	{
		nSIndex = sTmp.Find( _T("@") , 0 );
		if( nSIndex < 0 && sTmp.IsEmpty() ) break;
		if( nSIndex > -1 ) sLine = sTmp.Mid( 0 , nSIndex );
		else
		{
			sLine = sTmp;
			sTmp.Empty();
		}


		if( i ==  1 ) 
		{
			CComboBox *pCombo = (CComboBox*)(GetDlgItem( IDC_cmbFileUSER6 ));
			INT nCount = pCombo->GetCount();
			CString sGetComboData;
			bool bEqual = false;
			for( INT j = 0 ; j < nCount ; j++ )
			{
				pCombo->GetLBText( j , sGetComboData );
				if( sGetComboData.Compare(sLine) == 0 )
				{
					sLine.Format( _T("%d") , j );
					bEqual = true;
				}
			}
			if( !bEqual )
			{
				pCombo->AddString( sLine );
				sLine.Format( _T("%d") , 0 );
			}
			pCombo->SetCurSel( _ttoi(sLine) );
		}
		else
		{
			ids = SetControlData( sCMAlias , sCMEmailFieldAlias[i] , sLine ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeSUMFieldToControl( TCHAR **sControlField , CString sSeperator , CString sData , INT nFieldCnt )") );
		}
		
		sLine.Empty();
		sTmp = sTmp.Mid( nSIndex+1 );
		i++;
	}
	return 0;	

	EFS_END
	return -1;

}

INT CBO_LOC_3391::SettingDisplayBefore()
{
	EFS_BEGIN

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3391::SettingDisplayAfter()
{
	EFS_BEGIN

	INT ids;

	CString sGetData;
	ids = GetDataMgrData( _T("DM_BO_LOC_3390") , _T("휴대폰") , sGetData , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3320::SettingDisplayBefore()") );

	TCHAR *sCMHandPhoneFieldAlias[3] = 
	{
		_T("휴대폰1") , _T("휴대폰2") , _T("휴대폰3")
	};	

	ids = ChangeSUMFieldToControl( sCMHandPhoneFieldAlias , _T("-") , sGetData , 3 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::SettingDisplayBefore()") );

	ids = SettingEmail();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3320::SettingDisplayBefore()") );

	ids = ViewCodeToDesc();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3320::SettingDisplayBefore()") );

	ids = GetDataMgrData( _T("DM_BO_LOC_3390") , _T("생일타입") , sGetData , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_3320::SettingDisplayBefore()") );
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbUSER8);
	
	CString sStr;
	for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sStr);
		if( sStr.Compare(sGetData) == 0 )
			pCombo->SetCurSel(i);
	}
	
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3391::SetSelect()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3390_GRID");
	CString sGridAlias = _T("FileUseInfoGrid");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3320::SetSelect()") );

	CESL_Grid *pGrid = ((CESL_Grid*)(pCM->FindControl( sGridAlias )));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::SetSelect()") );

	ids = pGrid->SetReverse( m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3320::SetSelect()") );

	return 0;

	EFS_END
	return -1;

}

CESL_Grid* CBO_LOC_3391::GetMainGridPtr()
{
	EFS_BEGIN


	CString sParentCMAlias = _T("CM_BO_LOC_3390_GRID");
	CString sControlAlias = _T("FileUseInfoGrid");


	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sControlAlias));
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}

INT CBO_LOC_3391::MoveIndex( INT nMode )
{	
	EFS_BEGIN

	INT ids;
	ids = SetCurrentIndex();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3320::MoveIndex( INT nMode )") );

	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3391::CheckParentDMCount()
{
	EFS_BEGIN

	CString sParentDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );

	return pParentDM->GetRowCount();

	EFS_END
	return -1;

}

VOID CBO_LOC_3391::OnClose() 
{
	EFS_BEGIN

	DestroyWindow();

	EFS_END
}

VOID CBO_LOC_3391::OnbtnFileSAVE() 
{
	EFS_BEGIN

	INT ids;
	
	ids = CMToDM();
	if(0>ids) ERROR_MSG_RETURN_VOID2(-1 , _T("CBO_LOC_3391::OnbtnFileSAVE()") );
	if(0<ids) return;

	AfxMessageBox( _T("수정이 완료되었습니다.") );
	GridDisplay();

	EFS_END
}

INT CBO_LOC_3391::CMToDM()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3391");
	CString sDMAlias = _T("DM_BO_LOC_3390");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("CBO_LOC_3391::CMToDM()") );

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3391::CMToDM()") );

	const INT nControlAliasCnt = 30;

	TCHAR *sControlAlias[ nControlAliasCnt ] =
	{
		_T("E_MAIL1") , _T("E_MAIL2") , _T("교번") , _T("근무지_우편번호") , _T("근무지_전화"),
		_T("근무지_주소") , _T("대출자번호") , _T("반") , _T("발급횟수") , _T("비고") , 
		_T("비밀번호") , _T("비밀번호확인") , _T("사용자ID") , _T("생년월일") , _T("생일타입") , 
		_T("소속") , _T("우편발송지역") , _T("이름") , _T("자료실") , _T("주민등록번호") , 
		_T("직급") , _T("집_우편번호") , _T("집_전화") , _T("집_주소") , _T("학교명") , 
		_T("학년") , _T("회원구분") , _T("휴대폰1") , _T("휴대폰2") , _T("휴대폰3")
	};

	CString sGetData[nControlAliasCnt];
	
	for( INT i = 0 ; i < nControlAliasCnt ; i++ )
	{
		ids = pCM->GetControlMgrData( sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3391::CMToDM()") );
	}

	CString strTemp;
	strTemp = sGetData[19];
	strTemp.TrimLeft();	strTemp.TrimRight();
	strTemp.Replace(_T("-"),_T(""));
	if( 6 < strTemp.GetLength() )
	{
		sGetData[19] = strTemp.Mid(0,6) + _T("-") + strTemp.Mid(6);
	}

	if(_T("Y")==m_sIsUnityLoanService)
	{
		CString strDMCivilNo, strCivilNoIndex, strCryptoCivilNo;
		ids = pDM->GetCellData(_T("주민등록번호"), m_nCurrentIndex, strDMCivilNo);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CMToDM"));

		if(strDMCivilNo != sGetData[19])
		{
			strCivilNoIndex = MakeCivilNumIndex(sGetData[19]);
			if(13==strCivilNoIndex.GetLength())
			{
				this->ESL_DES_Encryption(sGetData[19],strCryptoCivilNo);
				ids = pDM->SetCellData( _T("주민등록번호_CRYPTO") , strCryptoCivilNo ,  m_nCurrentIndex );
				CLocCommonAPI::ChangeSecuriyCivilNo(this, sGetData[19] , m_sIsUnityLoanService);
			}
			else
			{
				AfxMessageBox(_T("잘못된 주민등록번호입니다.\r\n")
							  _T("올바른 주민등록번호를 입력해주십시오."));
				GetDlgItem(IDC_edtFileUSER8)->SetFocus();
				return 1010;
			}
		}
	}

	const INT nDMFieldAliasCnt = 30;
	TCHAR *sDMFieldAlias[ nDMFieldAliasCnt ] =
	{
		_T("E_MAIL1") ,		_T("E_MAIL2") ,		_T("교번") ,		_T("근무지_우편번호") , _T("근무지_전화번호") , 
		_T("근무지_주소") , _T("대출자번호") ,	_T("반") ,			_T("재발급횟수") ,		_T("비고") ,
		_T("비밀번호") ,	_T("비밀번호") ,	_T("대출자ID") ,	_T("생년월일") ,		_T("생일타입") , 
		_T("소속키") ,		_T("우편발송지역") ,_T("이름") ,		_T("자료실") ,			_T("주민등록번호") , 
		_T("직급키") ,		_T("집_우편번호") , _T("집_전화번호") , _T("집_주소") ,			_T("학교") , 
		_T("학년") ,		_T("회원구분") ,	_T("휴대폰1") ,		_T("휴대폰2") ,			_T("휴대폰3")
	};

	INT nRow = m_nCurrentIndex;
	for( i = 0 ; i < nDMFieldAliasCnt ;i++ )
	{

		ids = pDM->SetCellData( sDMFieldAlias[i] , sGetData[i] ,  nRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , sDMFieldAlias[i] );
	}

	CString sPhoneNumber;
	if( sGetData[27].Compare(_T("적용안함")) != 0 )
		sPhoneNumber.Format( _T("%s-%s-%s") , sGetData[27] , sGetData[28] , sGetData[29] );
	ids = pDM->SetCellData( _T("휴대폰") , sPhoneNumber ,  nRow );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("CBO_LOC_3391::CMToDM()") );

	CString sEMail;
	if( !sGetData[0].IsEmpty() && !sGetData[1].IsEmpty() )
		sEMail.Format( _T("%s@%s") , sGetData[0] , sGetData[1] );
	ids = pDM->SetCellData( _T("E_MAIL") , sEMail ,  nRow );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("CBO_LOC_3391::CMToDM()") );
	
	CString sMailSendArea;
	if( sGetData[16].Compare( _T("자택") ) == 0 )
		sMailSendArea = _T("H");
	else
		sMailSendArea = _T("W");
	ids = pDM->SetCellData( _T("우편발송지역") , sMailSendArea ,  nRow );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("CBO_LOC_3391::CMToDM()") );

	return 0;

	EFS_END
	return -1;
}

VOID CBO_LOC_3391::GridDisplay()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3390");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_LOC_3391::GridDisplay()") );

	const INT nCnt = 2;

	TCHAR *sCodeAlias[nCnt][2] = { 
		{ _T("대출소속정보") , _T("소속키") } , 
		{ _T("대출직급정보") , _T("직급키") }
	};

	for( INT i = 0 ;i < nCnt ; i++ )
	{
		ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , sCodeAlias[i][0] , sCodeAlias[i][1]  );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -2 , _T("CBO_LOC_3391::GridDisplay()") );
	}

	ids = AllControlDisplay( _T("CM_BO_LOC_3390_GRID") );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_LOC_3390::GridDisplay()") );

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid ==  NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("CBO_LOC_3390::GridDisplay()") );

	pGrid->SetReverse( m_nCurrentIndex );

	EFS_END
}


VOID CBO_LOC_3391::OnbtnDUPLICATE4() 
{
	EFS_BEGIN

	INT ids;
	bool IsDuplicate = false;
	ids = IsDuplicateCivilNo( IsDuplicate );	
	if( ids < 0 ) return ;
	if( ids > 0 ) return;

	if( IsDuplicate )
	{
		AfxMessageBox( _T("중복된 주민등록번호입니다. 다른 번호를 입력하십시오") );
	}
	else
	{
		AfxMessageBox( _T("중복된 주민등록번호가 존재하지 않습니다. ") );
	}	
	

	EFS_END
}

INT CBO_LOC_3391::IsDuplicateCivilNo( bool &bIsDuplicate )
{
	EFS_BEGIN

	INT ids;
	CString sSql;

	CString strCivilNo , strHalfLineCivilNo;
	ids = GetControlData( _T("CM_BO_LOC_3391") , _T("주민등록번호") , strCivilNo );
	if( ids < 0 ) return -1;	

	if( strCivilNo.IsEmpty() )
	{
		AfxMessageBox( _T("주민등록 번호를 입력하시고 중복 조회 기능을 사용하십시오") );
		bIsDuplicate = FALSE;
		return 2;
	}
	
	strCivilNo.Replace(_T("-"),_T(""));
	strHalfLineCivilNo = strCivilNo;
	if( 6 < strHalfLineCivilNo.GetLength() )
	{
		strHalfLineCivilNo = strHalfLineCivilNo.Mid( 0 , 6 ) + _T("-") + strHalfLineCivilNo.Mid( 6 );
	}

	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			CStringArray asEnc;
			asEnc.Add(strCivilNo);
			asEnc.Add(strHalfLineCivilNo);
			if(TRUE == p3000Api->ConnectSocket())
			{
				if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asEnc))
				{
					strCivilNo = asEnc.GetAt(0);
					strHalfLineCivilNo = asEnc.GetAt(1);
				}
				asEnc.RemoveAll(); asEnc.FreeExtra();
				p3000Api->CloseSocket();
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}

	sSql.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL")
				 _T(" WHERE CIVIL_NO IN ('%s','%s')") 
				 _T(" AND (USER_CLASS!='3' OR MEMBER_CLASS!='1')")
				 , strCivilNo , strHalfLineCivilNo );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3390") );
	if( pDM == NULL ) return -1;

	if(_T("Y")==m_sIsUnityLoanService)
	{
		CString strCivilNo;
		pDM->GetCellData(_T("주민등록번호"), m_nCurrentIndex, strCivilNo);
		if(strHalfLineCivilNo==strCivilNo)
		{
			pDM->GetCellData(_T("주민등록번호_CRYPTO"), m_nCurrentIndex, strCivilNo);
		}
		else
		{
			strCivilNo = MakeCivilNumIndex(strHalfLineCivilNo);
			if(13==strCivilNo.GetLength())
			{
				strCivilNo.Format( _T("%s-%s") , strHalfLineCivilNo.Left(6) , strHalfLineCivilNo.Right(7) );
				this->ESL_DES_Encryption(strCivilNo,strCivilNo);
			}
			else
			{
				AfxMessageBox(_T("잘못된 주민등록번호입니다.\r\n")
							  _T("올바른 주민등록번호를 입력해주십시오."));
				GetDlgItem(IDC_edtFileUSER8)->SetFocus();
				return 1010;
			}
		}
		
		sSql.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL")
					 _T(" WHERE CRYPTO_CIVIL_NO = '%s'") 
					 _T(" AND (USER_CLASS!='3' OR MEMBER_CLASS!='1')")
					 , strCivilNo);
	}

	CString sResult;
	ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sResult );
	if( ids < 0 ) return -1;

	if( sResult.Compare(_T("0")) != 0 ) bIsDuplicate = true;

	return 0;	

	EFS_END
	return -1;

}

VOID CBO_LOC_3391::OnbtnFileFIND1() 
{
	EFS_BEGIN

	CSearchZipCodeDlg SearchZipCodeDlg(this);
	SearchZipCodeDlg.DoModal();
	
	if( SearchZipCodeDlg.m_nCloseMode == 0 ) return;

	GetDlgItem(IDC_edtFileUSER9)->SetWindowText(SearchZipCodeDlg.m_sZipCode);
	GetDlgItem(IDC_edtFileUSER10)->SetWindowText(SearchZipCodeDlg.m_sAddress);	

	EFS_END
}

VOID CBO_LOC_3391::OnbtnFileFIND2() 
{
	EFS_BEGIN

	CSearchZipCodeDlg SearchZipCodeDlg(this);
	SearchZipCodeDlg.DoModal();
	
	if( SearchZipCodeDlg.m_nCloseMode == 0 ) return;

	GetDlgItem(IDC_edtFileUSER12)->SetWindowText(SearchZipCodeDlg.m_sZipCode);
	GetDlgItem(IDC_edtFileUSER13)->SetWindowText(SearchZipCodeDlg.m_sAddress);

	EFS_END
}

VOID CBO_LOC_3391::OnSetfocusedtFileUSER21() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFileUSER21)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFileUSER21)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3391::OnSetfocusedtFileUSER4() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFileUSER4)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFileUSER4)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3391::OnSetfocusedtFileUSER16() 
{
	// TODO: Add your control notification handler code here
		HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFileUSER16)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFileUSER16)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3391::OnSetfocusedtFileUSER13() 
{
	// TODO: Add your control notification handler code here
		HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFileUSER13)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFileUSER13)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3391::OnSetfocusedtFileUSER10() 
{
	// TODO: Add your control notification handler code here
		HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFileUSER10)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFileUSER10)->m_hWnd,hIME); 
    }
}

VOID CBO_LOC_3391::OnSetfocusedtFileUSER22() 
{
	// TODO: Add your control notification handler code here
		HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFileUSER22)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFileUSER22)->m_hWnd,hIME); 
    }
}

HBRUSH CBO_LOC_3391::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

CString CBO_LOC_3391::MakeCivilNumIndex( CString civilNum )
{
EFS_BEGIN
	
	INT nLength = civilNum.GetLength();

	CString sIndex;
	TCHAR cNum;
	INT		i;
	for( i=0 ; i<nLength ; i++ )
	{
		cNum = civilNum.GetAt( i );
		if( 47<cNum && 58>cNum ) sIndex += cNum;
	}

	return sIndex;

EFS_END
return _T("");
}