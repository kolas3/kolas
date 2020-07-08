// CO_CENTER_USER_MANAGE_05.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_MANAGE_05.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGE_05 dialog


CCO_CENTER_USER_MANAGE_05::CCO_CENTER_USER_MANAGE_05(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_MANAGE_05)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CCO_CENTER_USER_MANAGE_05::~CCO_CENTER_USER_MANAGE_05()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}
VOID CCO_CENTER_USER_MANAGE_05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_MANAGE_05)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_MANAGE_05, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_MANAGE_05)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnSAVEUSER, OnbtnSAVEUSER)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGE_05 message handlers

BOOL CCO_CENTER_USER_MANAGE_05::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("CO_CENTER_USER_MANAGE_05")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCO_CENTER_USER_MANAGE_05::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	bFirstView = FALSE;
	SetScreen();
	bFirstView = TRUE;	
}



VOID CCO_CENTER_USER_MANAGE_05::SetScreen()
{
	EFS_BEGIN

	SetCurrentIndex();
	DisplayWindow();

	EFS_END
}



INT CCO_CENTER_USER_MANAGE_05::DisplayWindow()
{
	EFS_BEGIN

	INT ids;


	// 2. DISPLAY한다.
	ids = ViewScreenLinkDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CCO_CENTER_USER_MANAGE_05::DisplayWindow()") );
	
	// 3. 부모 그리드에 표시한다.
	ids = SetSelect();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CCO_CENTER_USER_MANAGE_05::DisplayWindow()") );

	return 0;

	EFS_END
	return -1;

}

INT CCO_CENTER_USER_MANAGE_05::SetCurrentIndex()
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	// 이전 다음이
	// 'V'체크가 된 것이 있다면 'V'체크 된것만 보여주며
	// 'V'체크가 된 것이 없다면 모두 보여준다.
	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = pGrid->GetIdx();

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

VOID CCO_CENTER_USER_MANAGE_05::SetPreNextButton()
{
	EFS_BEGIN

	GetDlgItem(IDC_btnPREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT)->EnableWindow(TRUE);

	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btnPREV)->EnableWindow(FALSE);
	}
	if( m_nCurrentIndex == m_nLastIndex )
	{
		GetDlgItem(IDC_btnNEXT)->EnableWindow(FALSE);
	}

	EFS_END
}

VOID CCO_CENTER_USER_MANAGE_05::OnbtnPREV() 
{
	EFS_BEGIN

	MoveIndex(0);	
	DisplayWindow();

	EFS_END
}


VOID CCO_CENTER_USER_MANAGE_05::OnbtnNEXT() 
{
	EFS_BEGIN

	MoveIndex(1);	
	DisplayWindow();

	EFS_END
}



VOID CCO_CENTER_USER_MANAGE_05::OnbtnCLOSE() 
{
	OnCancel();	
}




INT CCO_CENTER_USER_MANAGE_05::ViewScreenLinkDM()
{
	EFS_BEGIN
		
	INT ids;
	
	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGE_05" );
	
	CESL_ControlMgr *pChildCM = FindCM( sCMAlias );
	if( pChildCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_3320::ViewScreenLinkDM()") );
	
	
	// DM에 있는 자료 CM에 보여주기
	ids = pChildCM->AllControlDisplay(m_nCurrentIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::ViewScreenLinkDM()") );
	
	
	CodeToDesc();

	// 이전 / 다음 버튼 셋팅
	SetPreNextButton();
	
	return 0;
	
	EFS_END
	return -1;

}


INT CCO_CENTER_USER_MANAGE_05::SetSelect()
{
	EFS_BEGIN
		
	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::SetSelect()") );
	
	ids = pGrid->SetReverse( m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3320::SetSelect()") );
	
	return 0;
	
	EFS_END
		return -1;
	
}


CESL_Grid* CCO_CENTER_USER_MANAGE_05::GetMainGridPtr()
{
	EFS_BEGIN
		
		
	CString sParentCMAlias = _T("CM_CO_CENTER_USER_MANAGER_01");
	CString sControlAlias = _T("센터이용자정보그리드");
		
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return NULL;
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sControlAlias));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;
	
	EFS_END
		return NULL;
	
}


// -------------------------------------------------
// nMode 0 -> PREV
//		 1 -> NEXT
INT CCO_CENTER_USER_MANAGE_05::MoveIndex( INT nMode )
{	
	EFS_BEGIN
		
		INT ids;
	ids = SetCurrentIndex();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MoveIndex( INT nMode )") );
	
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

/// 우편발송지역 , 회원 구분 변경 
INT CCO_CENTER_USER_MANAGE_05::CodeToDesc()
{
	EFS_BEGIN
		
	INT ids;
	
//	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGE_05");
	CString sDMAlias = _T("DM_CO_CENTER_USER_MANAGER_01");

	CString sData;
//	ids = GetControlData( sCMAlias , _T("우편발송지역") , sData );
	ids = GetDataMgrData( sDMAlias , _T("우편발송지역") , sData , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CodeToDesc") );

	if( sData.Compare(_T("H")) == 0 )
		sData = _T("자택");
	else
		sData = _T("근무지");
/*
	ids = SetControlData( sCMAlias , _T("우편발송지역") , sData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CodeToDesc") );
*/
	// Combo에 맞게 수정
	CComboBox* pCombo = NULL;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbUSER3);

	CString sTmpData;
	for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText( i , sTmpData );

		if( sTmpData.Compare( sData ) == 0 )
			pCombo->SetCurSel(i);
	}

//	ids = GetControlData( sCMAlias , _T("회원구분") , sData );
	ids = GetDataMgrData( sDMAlias , _T("회원구분") , sData , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CodeToDesc") );
	
	if( sData.Compare(_T("0")) == 0 )
		sData = _T("정회원");
	else
		sData = _T("비회원");
/*
	ids = SetControlData( sCMAlias , _T("회원구분") , sData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CodeToDesc") );
*/	
		
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbUSER4);
	
	for( i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText( i , sTmpData );
		
		if( sTmpData.Compare( sData ) == 0 )
			pCombo->SetCurSel(i);
	}

	return 0;
	
	
	EFS_END
		
		return -1;
}

VOID CCO_CENTER_USER_MANAGE_05::OnbtnSAVEUSER() 
{
	EFS_BEGIN

	AfxGetApp()->DoWaitCursor(-1);

	INT ids = ModifyUser();
	if( ids == 0 )
	{
		AfxMessageBox(_T("저장하였습니다."));
	}

	AfxGetApp()->DoWaitCursor(1);

	EFS_END
}


/// 수정
INT CCO_CENTER_USER_MANAGE_05::ModifyUser()
{
	EFS_BEGIN

	INT ids;

	/// 1. Tmp DM 생성

	CESL_DataMgr TmpDM;
	
	ids = CopyODMtoTempDM(&TmpDM);
	if( ids > 0 ) return ids;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ModifyUser") );


	/// 2. CM의 변경된 부분 DM에 저장
	ids = CMToDM(&TmpDM);
	if( ids > 0 ) return ids;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ModifyUser") );
	

	/// 3. DM으로 UpdateFrame생성 -> SendFrame
	ids = DMToUpdateFrame(&TmpDM);
	if( ids > 0 ) return ids;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ModifyUser") );

	/// 4. DM복사
	ids = CopyTempDMtoODM(&TmpDM);
	if( ids > 0 ) return ids;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ModifyUser") );
	
	/// 4. 끝

	return 0;

	EFS_END


	return -1;
}

INT CCO_CENTER_USER_MANAGE_05::CopyODMtoTempDM( CESL_DataMgr* pTmpDM )
{
	EFS_BEGIN

	INT ids;

	CString sODMAlias = _T("DM_CO_CENTER_USER_MANAGER_01");

	CESL_DataMgr* pSrcDM = FindDM( sODMAlias );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CopyODMtoTempDM") );

	ids = CLocCommonAPI::MakeCopyDM( this , pSrcDM , pTmpDM );
	if( ids , 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CopyODMtoTempDM") );

	pTmpDM->InsertRow(-1);

	ids = CLocCommonAPI::AddDM( pSrcDM , pTmpDM , m_nCurrentIndex , 0 , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CopyODMtoTempDM") );

	return 0;

	EFS_END

	return -1;
}


INT CCO_CENTER_USER_MANAGE_05::CMToDM( CESL_DataMgr* pTmpDM )
{
	EFS_BEGIN

	INT ids;

	const INT nUpdateFieldCnt = 22;

	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGE_05");
	
	CString sControlAlias[nUpdateFieldCnt] =
	{
		_T("대출자번호") , _T("사용자ID") , _T("발급횟수") , _T("이름") , _T("주민등록번호") , 
		_T("생일") , _T("생일TYPE") , _T("휴대폰") , _T("E_MAIL") , _T("우편발송지역") , 
		_T("소속") , _T("직급") , _T("자택_우편번호") , _T("자택_주소") , _T("자택_전화번호") ,
		_T("근무지_우편번호") , _T("근무지_주소") , _T("근무지_전화번호") , _T("학교") ,_T("학년") , 
		_T("반") , _T("회원구분")
	};
	
	
	CString sDMFieldAlias[nUpdateFieldCnt] =
	{
		_T("대출자번호") , _T("대출자ID") , _T("재발급횟수") , _T("이름") , _T("주민등록번호") , 
		_T("생일") , _T("생일_TYPE") , _T("핸드폰") , _T("E_MAIL") , _T("우편발송지역") , 
		_T("대출소속정보") , _T("대출직급정보") , _T("자택_우편번호") , _T("자택_주소") , _T("자택_전화번호") ,
		_T("근무지_우편번호") , _T("근무지_주소") , _T("근무지_전화번호") , _T("학교") ,_T("학년") , 
		_T("반") , _T("회원구분")
	};

	CString sValue[nUpdateFieldCnt];

	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToDM") );
	
	for( INT i = 0 ; i < nUpdateFieldCnt ; i++ )
	{
		ids = pCM->GetControlMgrData( sControlAlias[i] , sValue[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( - 1, _T("CMToDM") );

		/// 우편발송지역 처리
		if( i == 9 )
		{
			if( sValue[i].Compare(_T("자택")) == 0 )
				sValue[i] = _T("H");
			else
				sValue[i] = _T("W");
		}
		/// 회원구분처리
		else if( i == 21 )
		{
			if( sValue[i].Compare(_T("정회원")) == 0 )
				sValue[i] = _T("0");
			else
				sValue[i] = _T("1");			
		}

		ids = pTmpDM->SetCellData( sDMFieldAlias[i] , sValue[i] , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( - 2, _T("CMToDM") );
	}
	
	

	return 0;
	
	EFS_END
		
	return -1;
}

INT CCO_CENTER_USER_MANAGE_05::DMToUpdateFrame( CESL_DataMgr* pTmpDM )
{
	EFS_BEGIN

	INT ids;

	const INT nMasterUpdateFieldCnt = 17;
	const INT nLibUserUpdateFieldCnt = 5;
	
	/// 2개로 나뉠수 있다.
	CString sMasterTableName = _T("CO_MASTER_LOAN_USER_TBL");
	CString sLibUserTableName = _T("CO_LIBRARY_USER_TBL");

	CString sMasterDMFieldAlias[nMasterUpdateFieldCnt] =
	{
		_T("대출자번호") , _T("이름") , _T("주민등록번호") , _T("생일") , _T("생일_TYPE") , 
		_T("핸드폰") , _T("E_MAIL") , _T("우편발송지역") , _T("자택_우편번호") , _T("자택_주소") , 
		_T("자택_전화번호") , _T("근무지_우편번호") , _T("근무지_주소") , _T("근무지_전화번호") , _T("학교") ,
		_T("학년") , _T("반")  
	};

	CString sMasterDBFieldName[nMasterUpdateFieldCnt] =
	{
		_T("USER_NO") , _T("NAME") , _T("CIVIL_NO") , _T("BIRTHDAY") , _T("BIRTHDAY_TYPE") ,
		_T("HANDPHONE") , _T("E_MAIL") , _T("MAIL_SEND_AREA") , _T("H_ZIPCODE") , _T("H_ADDR1") , 
		_T("H_PHONE") , _T("W_ZIPCODE") , _T("W_ADDR1") , _T("W_PHONE") , _T("SCHOOL") , 
		_T("GRADE") , _T("CLASS")

	};

	CString sMasterDBFieldType[nMasterUpdateFieldCnt] =
	{
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") 
	};
	
	
	CString sLibUserDMFieldAlias[nLibUserUpdateFieldCnt] =
	{
		_T("대출자ID") , _T("재발급횟수") , _T("대출소속정보") , _T("대출직급정보") , _T("회원구분")
	};
	
	CString sLibUserDBFieldName[nLibUserUpdateFieldCnt] =
	{
		_T("USER_ID") , _T("REISSUE_CNT") , _T("USER_CLASS_CODE") , _T("USER_POSITION_CODE") , _T("MEMBER_CLASS")
	};


	CString sLibUserDBFieldType[nLibUserUpdateFieldCnt] =
	{
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") 
	};

	CString sMasterRecKey , sLibUserRecKey;

	pTmpDM->GetCellData( _T("유저통합관리KEY") , 0 , sMasterRecKey );
	pTmpDM->GetCellData( _T("도서관별대출자정보KEY") , 0 , sLibUserRecKey );
	
	pTmpDM->StartFrame();
	pTmpDM->InitDBFieldData();

	CString sValue;
	for( INT i = 0 ; i < nMasterUpdateFieldCnt ; i++ )
	{
		ids = pTmpDM->GetCellData( sMasterDMFieldAlias[i] , 0 , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DMToUpdateFrame") );

		pTmpDM->AddDBFieldData( sMasterDBFieldName[i] , sMasterDBFieldType[i] , sValue , TRUE );
	}

	pTmpDM->MakeUpdateFrame( sMasterTableName  , _T("REC_KEY") , _T("NUMERIC") , sMasterRecKey );

	pTmpDM->InitDBFieldData();
	
	for( i = 0 ; i < nLibUserUpdateFieldCnt ; i++ )
	{
		ids = pTmpDM->GetCellData( sLibUserDMFieldAlias[i] , 0 , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DMToUpdateFrame") );
		
		pTmpDM->AddDBFieldData( sLibUserDBFieldName[i] , sLibUserDBFieldType[i] , sValue , TRUE );
	}
	
	pTmpDM->MakeUpdateFrame( sLibUserTableName  , _T("REC_KEY") , _T("NUMERIC") , sLibUserRecKey );

	pTmpDM->InitDBFieldData();
	
	ids = pTmpDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DMToUpdateFrame") )


	pTmpDM->EndFrame();

	return 0;
	
	EFS_END
		
	return -1;
}

INT CCO_CENTER_USER_MANAGE_05::CopyTempDMtoODM( CESL_DataMgr* pTmpDM )
{
	EFS_BEGIN

	INT ids;

	const INT nMasterUpdateFieldCnt = 17;
	CString sMasterRecKey;


	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_CO_CENTER_USER_MANAGER_01"),_T("센터이용자정보그리드"));
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CopyTempDMtoODM") );


	pTmpDM->GetCellData( _T("유저통합관리KEY") , 0 , sMasterRecKey );

	CString sODMAlias = _T("DM_CO_CENTER_USER_MANAGER_01");
	
	CESL_DataMgr* pSrcDM = FindDM( sODMAlias );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CopyODMtoTempDM") );
	
	CString sMasterDMFieldAlias[nMasterUpdateFieldCnt] =
	{
		_T("대출자번호") , _T("이름") , _T("주민등록번호") , _T("생일") , _T("생일_TYPE") , 
		_T("핸드폰") , _T("E_MAIL") , _T("우편발송지역") , _T("자택_우편번호") , _T("자택_주소") , 
		_T("자택_전화번호") , _T("근무지_우편번호") , _T("근무지_주소") , _T("근무지_전화번호") , _T("학교") ,
		_T("학년") , _T("반")  
	};

	CString sValue[nMasterUpdateFieldCnt];

	pTmpDM->GetCellData( sMasterDMFieldAlias , nMasterUpdateFieldCnt , sValue , 0 );
	
	CString sCMasterRecKey;
	for( INT i = 0 ; i < pSrcDM->GetRowCount() ; i++ )
	{
		ids = pSrcDM->GetCellData( _T("유저통합관리KEY") , i , sCMasterRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CopyTempDMtoODM") );

		if( sCMasterRecKey.Compare( sMasterRecKey )  != 0 ) continue;

		for( INT j = 0 ; j < nMasterUpdateFieldCnt ; j++ )
		{
			ids = pSrcDM->SetCellData( sMasterDMFieldAlias[j] , sValue[j] , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( - 3 , _T("CopyTempDMtoODM") );
		}

		pParentGrid->DisplayLine(i);
	}
		
	return 0;
	
	EFS_END
		
	return -1;
}

HBRUSH CCO_CENTER_USER_MANAGE_05::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
		
/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 여기까지 추가

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
