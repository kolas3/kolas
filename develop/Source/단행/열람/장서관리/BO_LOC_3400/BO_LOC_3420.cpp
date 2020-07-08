// BO_LOC_3420.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3420.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3420 dialog


CBO_LOC_3420::CBO_LOC_3420(CESL_Mgr* pParent,CLonaManageValue *pLoanMangeValue,INT nMode,CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3420)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanMangeValue = pLoanMangeValue;
	m_nMode = nMode;

	m_pLoanShareManager = pLoanShareManager;
	// KOL.UDF.022
	m_pSpfWork = NULL;
	bFirstView = FALSE;
	m_nViewMode = 0;
	m_nCurrentIndex = 0;
	m_nLastIndex = 0;
	m_nFirstIndex = 0;



}

CBO_LOC_3420::~CBO_LOC_3420()
{
	
}

VOID CBO_LOC_3420::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3420)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3420, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3420)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnDDSAVE, OnbtnDSAVE)
	ON_BN_CLICKED(IDC_btnDDFINDUSER, OnbtnDFINDUSER)
	ON_BN_CLICKED(IDC_btnDDFINDBOOK, OnbtnDFINDBOOK)
	ON_BN_CLICKED(IDC_btnDDCLOSE, OnbtnDCLOSE)
	ON_BN_CLICKED(IDC_btnDDPREV, OnbtnDPREV)
	ON_BN_CLICKED(IDC_btnDDNEXT, OnbtnDNEXT)
	ON_BN_CLICKED(IDC_btnDDCLEAR, OnbtnDDCLEAR)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3420 message handlers

BOOL CBO_LOC_3420::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_3420::OnClose() 
{
	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;

	CDialog::OnClose();
}

BOOL CBO_LOC_3420::OnInitDialog() 
{
	EFS_BEGIN
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	CString sSMAlias = _T("BO_LOC_3420");
	if( m_nMode == 1 )
		sSMAlias = _T("BO_LOC_3420_1");

	if(InitESL_Mgr(sSMAlias) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	
	m_pSpfWork = new CBL_BO_LOC_34(this,m_pLoanMangeValue);
	
	DisplayControl();

	if( m_nMode == 1 )
	{
		InitSetStartPos();
		ViewCurrentWindow();
	}

	EnableDisableFromStatus();



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;
}

VOID CBO_LOC_3420::OnbtnDSAVE() 
{
	if( m_nMode ==  0 )
		m_pSpfWork->SPFExcutePath( INSERT_DELIVERY );
	else
	{
		m_pSpfWork->SetTempData( _T("선택인덱스") , CComVariant(m_nCurrentIndex) );
		m_pSpfWork->SPFExcutePath( UPDATE_DELIVERY );
	}
}

VOID CBO_LOC_3420::OnbtnDFINDUSER() 
{
	SelectUser();
}

INT CBO_LOC_3420::SelectUser()
{
	CLoanUserManage UserDlg(this,m_pLoanShareManager , m_pLoanMangeValue,TRUE);
	UserDlg.pMain = this->pMain;
	UserDlg.pParentMgr = this;
	UserDlg.m_pInfo = this->m_pInfo;
	UserDlg.DoModal();

	CString sUserKey = UserDlg.m_sSelectRecKey;

	if( sUserKey.IsEmpty() ) return 0;

	INT ids = SearchUserAddDM( sUserKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SelectBook") );

	return 0;

}

VOID CBO_LOC_3420::OnbtnDFINDBOOK() 
{
	SelectBook();

}

INT CBO_LOC_3420::SelectBook()
{
	CBO_LOC_3110 Dlg(this,m_pLoanMangeValue,1,m_pLoanShareManager);
	Dlg.pMain = this->pMain;
	Dlg.pParentMgr = this;
	Dlg.m_pInfo = this->m_pInfo;

	Dlg.DoModal();

	CString sRegNo , sPublishForm;

	sRegNo = Dlg.m_sRegNo;
	sPublishForm = Dlg.m_sPublishForm;

	if( sRegNo.IsEmpty() ) return 0;

	INT ids = SearchBookAddDM( sRegNo , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SelectBook") );

	return 0;
}

INT CBO_LOC_3420::SearchBookAddDM( CString sRegNo , CString sPublishForm )
{
	CString sDMAlias = _T("DM_BO_LOC_3420_BOOK");
	CString sCMAlias = _T("CM_BO_LOC_3420");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchBookAddDM") );

	CString sTableName;
	CString sCondition;
	if( sPublishForm.Compare( _T("MO") ) == 0 )
	{
		sTableName = _T("BO_BOOK_TBL B,IDX_BO_TBL I");
		sCondition = _T("I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG = 'B'");
		sPublishForm = _T("MO");

		pDM->SetDataMgrRef( 13 , _T("매체구분") , _T("B.MEDIA_CODE") , _T("str") , _T("") , _T("") );
	}
	else if( sPublishForm.Compare( _T("SE") ) == 0 )
	{
		sTableName = _T("SE_BOOK_TBL B,IDX_SE_TBL I,SE_VOL_TBL V");
		sCondition = _T("I.REC_KEY=B.SPECIES_KEY AND B.VOL_KEY = V.REC_KEY ");
		sPublishForm = _T("SE");

		pDM->SetDataMgrRef( 13 , _T("매체구분") , _T("V.MEDIA_CODE") , _T("str") , _T("") , _T("") );
	}

	pDM->CONDITION = sCondition;
	pDM->TBL_NAME = sTableName;

	CString sWhere;
	sWhere.Format( _T(" B.REG_NO = '%s' AND I.MANAGE_CODE=UDF_MANAGE_CODE") , sRegNo );

	INT ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchBookAddDM") );

	if( pDM->GetRowCount() > 0 ) 
	{
		pDM->SetCellData( _T("발행형태구분") , sPublishForm , 0 );		
	}

	CLocCommonAPI::MakeShelfCode( this , sDMAlias );

	CString sWorkingStatus;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("배가상태") , i , sWorkingStatus );

		BOOL bAble = FALSE;
		if( 0 == sPublishForm.Compare(_T("MO")) )
		{
			if( 0 == sWorkingStatus.Compare(_T("BOL112N")) ||
				0 == sWorkingStatus.Compare(_T("BOL211O")) ||
				0 == sWorkingStatus.Compare(_T("BOL212O")) ||				
				0 == sWorkingStatus.Compare(_T("BOL213O")) ||
				0 == sWorkingStatus.Compare(_T("BOL214O")) ||
				0 == sWorkingStatus.Compare(_T("BOL215O")) ||
				0 == sWorkingStatus.Compare(_T("BOL217O")) ||
				0 == sWorkingStatus.Compare(_T("BOL218O")) ||
				0 == sWorkingStatus.Compare(_T("BOL219O")) ||
				0 == sWorkingStatus.Compare(_T("BOL411O")) ||
				0 == sWorkingStatus.Compare(_T("BOL511O")) ||
				0 == sWorkingStatus.Compare(_T("BOL611O")) )
			{
				bAble = TRUE;
			}
		}
		else if( 0 == sPublishForm.Compare(_T("SE")) )
		{
			if( 0 == sWorkingStatus.Compare(_T("SEL112N")) ||
				0 == sWorkingStatus.Compare(_T("SEL212N")) ||
				0 == sWorkingStatus.Compare(_T("BOL311O")) ||
				0 == sWorkingStatus.Compare(_T("BOL312O")) ||
				0 == sWorkingStatus.Compare(_T("BOL313O")) ||
				0 == sWorkingStatus.Compare(_T("BOL314O")) ||
				0 == sWorkingStatus.Compare(_T("BOL315O")) ||
				0 == sWorkingStatus.Compare(_T("BOL317O")) ||
				0 == sWorkingStatus.Compare(_T("BOL318O")) )
			{
				bAble = TRUE;
			}
		}

		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );

		if( !bAble )
		{
			pDM->FreeData();
			CString strMsg;
			strMsg.Format(_T("배가상태가 '%s'인 자료는 대출할 수 없는 자료입니다. 다른 자료를 입력하십시오."), sWorkingStatus); 
			AfxMessageBox( strMsg );
			ids = AllControlDisplay( sCMAlias , 0 );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchBookAddDM") );
			return 0;
		}
		ids = pDM->SetCellData( _T("배가상태") , sWorkingStatus , i);
	}

	CString sMediaCode;
	ids = pDM->GetCellData( _T("매체구분") , 0 , sMediaCode );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("SearchBookAddDM") );

	if( sMediaCode.Compare(_T("OF")) == 0 )
	{
		pDM->FreeData();
		AfxMessageBox( _T("매체구분이 'OF'인 자료는 대출할 수 없는 자료입니다. 다른 자료를 입력하십시오.") );
	}

	const int nControlCnt = 6;
	TCHAR* cControl[nControlCnt] = 
	{
		_T("등록번호"), _T("발행자"), _T("청구기호"),
		_T("배가상태"), _T("서명"), _T("저작자")
	};
	for( i = 0; i < nControlCnt; i++ )
	{
		ids = ControlDisplay( sCMAlias, cControl[i], 0 );
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchBookAddDM") );
	}

	return 0;
}

INT CBO_LOC_3420::SearchUserAddDM( CString sUserKey )
{
	CString sDMAlias = _T("DM_BO_LOC_3420_USER");
	CString sCMAlias = _T("CM_BO_LOC_3420");

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SearchBookAddDM") );


	CString sWhere;
	sWhere.Format( _T(" U.REC_KEY = %s ") , sUserKey );

	INT ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SearchBookAddDM") );

	CStringArray asDes1,asDes2,asDes3;

	CString strValue1;
	ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("SearchBookAddDM") );
	strValue1.TrimLeft(); strValue1.TrimRight(); strValue1.MakeUpper();
	INT nRowCnt = pDM->GetRowCount();
	if(_T("Y") != strValue1 || TRUE == m_pInfo->m_bLibSecuUse)
	{
		for(INT i = 0; i < nRowCnt; i++)
		{
			if(_T("Y") != strValue1)
			{
				CString strUserNo;
				ids = pDM->GetCellData( _T("대출자번호") , i, strUserNo );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("SearchBookAddDM") );
				CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_pLoanMangeValue->m_sIsUnityLoanService );
				ids = pDM->SetCellData( _T("대출자번호"), strUserNo , i);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("SearchBookAddDM") );
			}

			if(TRUE == m_pInfo->m_bLibSecuUse)
			{
				CString strGetData;
				pDM->GetCellData(_T("주소"), i, strGetData);
				asDes1.Add(strGetData);
				pDM->GetCellData(_T("연락처"), i, strGetData);
				asDes2.Add(strGetData);
				pDM->GetCellData(_T("이메일"), i, strGetData);
				asDes3.Add(strGetData);
			}
		}
	}

	if(0 < nRowCnt && TRUE == m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes2);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes3);
					pApi->CloseSocket();
				}
				for(INT i = 0; i < nRowCnt; i++)
				{
					pDM->SetCellData(_T("주소"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("연락처"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("이메일"), asDes3.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
	}

	ids = AllControlDisplay( sCMAlias , 0 );
	if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("SearchBookAddDM") );

	return 0;
}

VOID CBO_LOC_3420::OnbtnDCLOSE() 
{
	OnCancel();
}

INT CBO_LOC_3420::DisplayControl()
{
	if( m_nMode == 0 )
	{
		SetWindowText( _T("신청 자료 입력") );
		const INT nUnVisibleCnt = 2;
		INT nIDC[nUnVisibleCnt] =
		{
			IDC_btnDDPREV , IDC_btnDDNEXT 
		};

		for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
			GetDlgItem( nIDC[i] )->ShowWindow(SW_HIDE );

	}
	else
	{
		SetWindowText( _T("수정") );

		const INT nUnVisibleCnt = 2;
		INT nIDC[nUnVisibleCnt] =
		{
			IDC_btnDDFINDUSER , IDC_btnDDFINDBOOK
		};

		for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
			GetDlgItem( nIDC[i] )->ShowWindow(SW_HIDE );

		CRect WndPos;
		GetWindowRect(WndPos);
		MoveWindow( WndPos.left , WndPos.right , WndPos.Width() - 87 , WndPos.Height() );
		CenterWindow();
	}

	return 0;
}

INT CBO_LOC_3420::EnableDisableFromStatus()
{
	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3420");
	if( m_nMode == 1 ) 
		sCMAlias = _T("CM_BO_LOC_3420_1");

	CString sStatus;
	ids = GetControlData( sCMAlias , _T("배달상태") , sStatus , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("EnableDisableFromStatus") );

	if( sStatus.Compare( _T("접수완료") ) == 0 || (sStatus.Compare( _T("신청자료") )==0) )
	{
		GetDlgItem(IDC_datDINPUT3)->EnableWindow(FALSE);
	}
	if(sStatus.Compare( _T("신청자료") )==0 )
	{
		GetDlgItem(IDC_datDINPUT2)->EnableWindow(FALSE);
	}

	return 0;
}



INT CBO_LOC_3420::ViewCurrentWindow()
{
	EFS_BEGIN

	INT ids;

	CESL_Grid* pGrid = m_pSpfWork->GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	ids = AllControlDisplay( _T("CM_BO_LOC_3420_1") , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewCurrentWindow") );

	ids = DisplayAfter();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewCurrentWindow") );

	GetDlgItem(IDC_btnDDPREV)->EnableWindow( TRUE );
	GetDlgItem(IDC_btnDDNEXT)->EnableWindow( TRUE );

	if( m_nCurrentIndex == m_nFirstIndex )
		GetDlgItem(IDC_btnDDPREV)->EnableWindow( FALSE );	
	if( m_nCurrentIndex == m_nLastIndex )
		GetDlgItem(IDC_btnDDNEXT)->EnableWindow( FALSE );
	
	pGrid->SetReverse( m_nCurrentIndex );


	return 0;


	EFS_END
	return -1;

}


INT CBO_LOC_3420::MoveIndex( INT nMode )
{
	EFS_BEGIN

	INT ids;
	ids = InitSetStartPos();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1540::MoveIndex( INT nMode )") );

	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = m_pSpfWork->GetMainGridPtr();
		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}

	ViewCurrentWindow();

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3420::InitSetStartPos()
{
	EFS_BEGIN

	CESL_Grid *pGrid = m_pSpfWork->GetMainGridPtr();
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

VOID CBO_LOC_3420::OnbtnDPREV() 
{
	MoveIndex(0);	
}

VOID CBO_LOC_3420::OnbtnDNEXT() 
{
	MoveIndex(1);	
}


INT CBO_LOC_3420::DisplayAfter()
{
	INT ids;

	CString sStatus;
	ids = GetDataMgrData( _T("DM_BO_LOC_3400") , _T("배달상태") , sStatus , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayAfter") );

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbDINPUT);

	CString sComboStatus;
	for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i , sComboStatus);

		if( sComboStatus.Compare(sStatus) == 0 )
			pCombo->SetCurSel(i);
	}

	return 0;
}
VOID CBO_LOC_3420::OnbtnDDCLEAR() 
{
	// TODO: Add your control notification handler code here
	
		
	CleanAllCtrls();
}

VOID CBO_LOC_3420::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow && m_nMode )
		ShowCleanBtn( false );
}

VOID CBO_LOC_3420::ShowCleanBtn( bool flag )
{
	CButton *pBtn = ( CButton * )GetDlgItem( IDC_btnDDCLEAR );
	if( pBtn->GetSafeHwnd() )
		pBtn->ShowWindow( false );
}

VOID CBO_LOC_3420::CleanAllCtrls()
{
	const INT ctrlCnt = 12;
	CString ctrlAlias[ ctrlCnt ] = {
		_T("대출자"),		
		_T("대출자번호"),	
		_T("이메일"),		
		_T("등록번호"),		
		_T("발행자"),		
		_T("배가상태"),		
		_T("서명"),					
		_T("연락처"),		
		_T("우편번호"),		
		_T("저작자"),		
		_T("주소"),			
		_T("청구기호") };	

	for( INT i = 0; i < ctrlCnt; i++ )
		INT ids = SetControlData( _T("CM_BO_LOC_3420"), ctrlAlias[ i ], _T("") );

	const INT DMCnt = 2;
	CString DMAlias[ DMCnt ] = {
		_T("DM_BO_LOC_3420_USER"), 
		_T("DM_BO_LOC_3420_BOOK") };
	CESL_DataMgr *pDM = NULL;
	for( i = 0; i < DMCnt; i++ )
	{
		pDM = FindDM( DMAlias[ i ] );
		pDM->FreeData();
	}	
}

HBRUSH CBO_LOC_3420::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
