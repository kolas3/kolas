// BO_LOC_3130.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3130.h"
#include "ReturnAppendixListDlg.h"
#include "BL_LOC_LOAN_PROC.h"
#include "KL_UserSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBO_LOC_3130::CBO_LOC_3130(CESL_Mgr* pParent , INT IDD_MODE , CBL_LOC_LOAN_PROC* pSpfWork , BOOL bShowWindow, BOOL bIsILL)
	: CESL_Mgr(IDD_MODE , pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3130)
	m_strEdtPayment = _T("");
	
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sBasicArrear = _T("");

	if( IDD_MODE == IDD )
		m_nReturnType = 0;
	else if( IDD_MODE == IDD_BO_LOC_3131 )
		m_nReturnType = 1;
	else if( IDD_MODE == IDD_BO_LOC_3132 )
		m_nReturnType = 2;
	if( IDD_MODE == IDD_BO_LOC_3130_2 || IDD_MODE == IDD_BO_LOC_3130_3 )
	{
		CString sReturnType;
		GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType );
		m_nReturnType = _ttoi( sReturnType );
		pSpfWork->m_pManageValue->GetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("연체료"), m_sBasicArrear );
	}
	m_bIsILL = bIsILL;

	m_bShowWindow = bShowWindow;

	m_pSpfWork = pSpfWork;
	m_pCheckLoan = pSpfWork->m_pCheckLoan;

	m_strUserID = pParent->GetUserID();
	m_strLibCode=_T("");

	m_GetLoanStopDateCnt = 0;

	m_bLastWork = TRUE;
	m_bIsReturnCancel = FALSE;

	m_nCurrentPos = 0;
	m_nFirstPos = 0;
	m_nLastPos = 0;
	m_nSelectGetCount = 0;
	m_nArrearPaymentStep = 0;
	m_nLoanStopUseStatus = 0;

	m_sAllArrearDay = _T("");
	m_sAllArrear = _T("");
	m_sAllPayment = _T("");
	m_sAllApplyArrearDay = _T("");	
	m_strEdtPayment = _T("");
	m_sLoanStopDate = _T("");
	m_strTransNo = _T("");
	m_strDisplayShelfLocCode = _T("");

	m_pParentMgr = pParent;
		
	m_bIsForceILL = FALSE;

	m_n3130ILLReturnProcCnt = 0;
	m_strReturnMatSpKeyList = _T("");

	m_n3130KLRetrunProcCnt = 0;
}

CBO_LOC_3130::~CBO_LOC_3130()
{
}

void CBO_LOC_3130::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3130)
	DDX_Control(pDX, IDC_edtMONEYPAID2, m_edtPayment);
	DDX_Text(pDX, IDC_edtMONEYPAID2, m_strEdtPayment);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3130, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3130)
	ON_BN_CLICKED(IDC_btnRETURNNEXT, OnbtnRETURNNEXT)
	ON_BN_CLICKED(IDC_btnRETURNPREV, OnbtnRETURNPREV)
	ON_BN_CLICKED(IDC_radRETURN1, OnradRETURN1)
	ON_BN_CLICKED(IDC_radRETURN2, OnradRETURN2)
	ON_BN_CLICKED(IDC_radRETURN3, OnradRETURN3)
	ON_BN_CLICKED(IDC_radSTEP1, OnradSTEP1)
	ON_BN_CLICKED(IDC_radSTEP2, OnradSTEP2)
	ON_BN_CLICKED(IDC_radSTEP3, OnradSTEP3)
	ON_BN_CLICKED(IDC_btnRETURNCLOSE, OnbtnRETURNCLOSE)
	ON_BN_CLICKED(IDC_btnRETURNSAVE, OnbtnRETURNSAVE)
	ON_EN_KILLFOCUS(IDC_edtRETURN7, OnKillfocusedtRETURN7)
	ON_EN_KILLFOCUS(IDC_edtMONEYPAID2, OnKillfocusedtMONEYPAID2)
	ON_EN_CHANGE(IDC_edtMONEYPAID2, OnChangeedtMONEYPAID2)
	ON_EN_CHANGE(IDC_edtRETURN4, OnChangeedtRETURN4)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CBO_LOC_3130, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3130)
	ON_EVENT(CBO_LOC_3130, IDC_gridARREAR, 69 /* SelChange */, OnSelChangeMaingridUser, VTS_NONE)
	ON_EVENT(CBO_LOC_3130, IDC_gridARREAR, -600 /* Click */, OnClickMaingridUser, VTS_NONE)
	//ON_EVENT(CBO_LOC_3130, IDC_gridARREAR, -601 /* DblClick */, OnDblClickMaingridUser, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3130 message handlers

BOOL CBO_LOC_3130::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CBO_LOC_3130::INIT_ESL()
{
	EnableThemeDialogTexture(GetSafeHwnd());
	
	if(InitESL_Mgr(_T("BO_LOC_3130")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	INT ids = InitValueSetting();
	if(0 > ids) 
	{
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못하였습니다."));
		return FALSE;
	}
	if( ids > 0 ) 
	{
		OnCancel();
		return -1;
	}

	SetReturnType(0);

	ids = ViewScreen();
	if(0 > ids)
	{
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못하였습니다."));
		return FALSE;
	}

	((CButton*)GetDlgItem( IDC_radRETURN1 ))->SetCheck(1);

	if( m_pSpfWork->m_pManageValue->m_sFeeMngSys == _T("Y") && FALSE == m_bIsILL)
	{
		CString sGetData;
		sGetData = m_sBasicArrear;
		SetMoneyFormat(sGetData);
		((CEdit*)GetDlgItem( IDC_edtBASICARREAR ))->SetWindowText(sGetData);

		ids = ArrearCopyDM();
		if(0 > ids)
		{
			m_bIsReturnCancel = TRUE;
			m_pSpfWork->SelfCloseMessageBox( _T("반납이 취소되었습니다.") );
			OnCancel();
			return FALSE;
		}

		OnradSTEP2();
		m_nLoanStopUseStatus = 0;

		m_edtPayment.SetLimitText(9);

		AllControlDisplay( _T("CM_BO_LOC_3130_2") );
	}

	LoadReturnShelfLocCodeDisplay(m_strDisplayShelfLocCode);

	if(TRUE == m_bIsILL)
	{
		const int DISABLE_COUNT = 4;
		int nDisableControlID[DISABLE_COUNT] = 
		{
			IDC_radRETURN2, 
			IDC_radRETURN3, 
			IDC_edtRETURN4, 
			IDC_edtRETURN5
		};
		
		for(int nCnt = 0; nCnt < DISABLE_COUNT; nCnt++)
		{
			CWnd* pWnd = (CWnd*)GetDlgItem(nDisableControlID[nCnt]);
			if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
			{
				pWnd->EnableWindow(FALSE);
			}
		}
	}

	return TRUE;
}

BOOL CBO_LOC_3130::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	if(FALSE == INIT_ESL())
	{
		return TRUE;
	}

	if( !m_bShowWindow )
	{
		OnbtnRETURNSAVE();
		OnbtnRETURNCLOSE();
	}

	GetDlgItem(IDC_btnRETURNSAVE)->SetFocus();

	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
return FALSE;
}

INT CBO_LOC_3130::ViewScreen()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3130");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );

	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::ViewScreen()") );

	ids = pCM->AllControlDisplay( m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::ViewScreen()") );

	ControlSetting();

	SetSelect( m_nCurrentPos );

	if( m_pSpfWork->m_pManageValue->m_sFeeMngSys != _T("Y") )
	{
		SetDelayMoney();
	}

	if( (((CButton*)GetDlgItem(IDC_radRETURN3))->GetCheck()==1) || 
		(((CButton*)GetDlgItem(IDC_radRETURN2))->GetCheck()==1) )
	{
		SetRepairMeney();
	}

	CString sValue;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("단행/연속구분"), sValue , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CBO_LOC_3130::ViewScreen()") );

	if( sValue.Compare(_T("NB"))==0)
	{
		CString sMsg;
		CString sRegNo;
		CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
		ids = GetDataMgrData( sDMAlias , _T("등록번호"), sRegNo , m_nCurrentPos );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("CBO_LOC_3130::ViewScreen()") );
		sMsg.Format( _T("반납 - 등록번호가 '%s'인 책은 긴급대출된 자료입니다!!"), sRegNo);
		AfxMessageBox( sMsg );
	}
	else
		SetWindowText(_T("반납"));
	
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::InitValueSetting(BOOL bShowMSG)
{
	EFS_BEGIN

	CString strFuncName = _T("");

	CESL_Grid *pGrid = NULL;
	INT ids = FindGridControl(&pGrid);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::InitValueSetting()") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	m_nCurrentPos = pGrid->SelectGetIdx();
	m_nLastPos = pGrid->SelectGetTailPosition();
	m_nFirstPos = pGrid->SelectGetHeadPosition();
	m_nSelectGetCount = pGrid->SelectGetCount();

	if( m_nSelectGetCount < 1 )
	{
		if(bShowMSG)
		{
			m_pSpfWork->SelfCloseMessageBox( _T("반납할 자료를 먼저 선택해주십시오") );
		}
		return 1;
	}

	SetSelect( m_nCurrentPos );

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3130::OnbtnRETURNNEXT() 
{
	EFS_BEGIN

	MoveRecord(1);	

	EFS_END
}

VOID CBO_LOC_3130::OnbtnRETURNPREV() 
{
	EFS_BEGIN

	MoveRecord(0);

	EFS_END
}

INT CBO_LOC_3130::SetSelect( INT n )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid;
	ids = FindGridControl(&pGrid);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::SetSelect( INT n )") );
	
	pGrid->m_nReverse = n;
	pGrid->SetReverse(n);

	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3130::MoveRecord( INT nMode )
{
	EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid;
	ids = FindGridControl(&pGrid);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::MoveRecord( INT nMode )") );


	if( nMode == 0 )
	{
		if( m_nCurrentPos == m_nFirstPos ) return 0;
		m_nCurrentPos = pGrid->SelectGetPrev();
	}
	else
	{
		if( m_nCurrentPos == m_nLastPos ) return 0;
		m_nCurrentPos = pGrid->SelectGetNext();
	}

	ids = ViewScreen();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::MoveRecord( INT nMode )") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::FindGridControl( CESL_Grid **pGrid )
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::FindGridControl( CESL_Grid **pGrid )") );
	*pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( &pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::FindGridControl( CESL_Grid **pGrid )") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::ControlSetting()
{
	EFS_BEGIN

	if( m_pSpfWork->m_pManageValue->m_sFeeMngSys != _T("Y") )
	{
		GetDlgItem(IDC_btnRETURNPREV)->EnableWindow(true);
		GetDlgItem(IDC_btnRETURNNEXT)->EnableWindow(true);

		if( m_nFirstPos == m_nCurrentPos ) 
			GetDlgItem(IDC_btnRETURNPREV)->EnableWindow(false);
		if( m_nLastPos == m_nCurrentPos )
			GetDlgItem(IDC_btnRETURNNEXT)->EnableWindow(false);
	}

	CString sCMAlias = _T("CM_BO_LOC_3130");
	INT ids = SetControlData( sCMAlias , _T("배상비"), _T("0") );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::ControlSetting()") );


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::SetReturnType( INT nControl )
{
	EFS_BEGIN

	INT ids;

	CString sReturnType;
	sReturnType.Format( _T("%d"), nControl );
	ids = SetControlData( _T("CM_BO_LOC_3130"), _T("반납책상태"), sReturnType); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::SetReturnType( INT nControl )") );

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3130::OnradRETURN1() 
{
	EFS_BEGIN

	INT ids;
	SetReturnType(0);
	ids = SetControlData( _T("CM_BO_LOC_3130"), _T("배상비"), _T("0") );
	if(0 > ids) ERROR_MSG_RETURN_VOID2(  -1 , _T("CBO_LOC_3130::OnradRETURN1") );

	EFS_END
}

VOID CBO_LOC_3130::OnradRETURN2() 
{
	EFS_BEGIN

	SetReturnType(1);

	SetRepairMeney();

	EFS_END
}

VOID CBO_LOC_3130::OnradRETURN3() 
{
	EFS_BEGIN

	SetReturnType(2);

	SetRepairMeney();

	EFS_END
}

VOID CBO_LOC_3130::OnbtnRETURNCLOSE() 
{
	EFS_BEGIN

	INT ids;
	ids = DeleteReturnedBook();
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-1 , _T("OnbtnRETURNCLOSE") );
	
	OnCancel();	

	EFS_END
}


VOID CBO_LOC_3130::OnbtnRETURNSAVE() 
{
EFS_BEGIN

	CString sStatus;

	ReturnSave(sStatus);

	if( m_bShowWindow )
	{
		if( m_nReturnType == 0 )
		{
			if( m_nCurrentPos == m_nLastPos )
			{
				if( (sStatus.Compare(_T("1"))==0) || (sStatus.Compare(_T("반납"))==0) )
				{
					OnbtnRETURNCLOSE();
				}
			}
		}
		else
		{
			OnbtnRETURNCLOSE();	
		}
	}

EFS_END
}

VOID CBO_LOC_3130::ReturnSave(CString &sStatus)
{
	INT ids;

	m_pSpfWork->SetBEGIN_SBL();

	INT nCount;	
	m_bLastWork = FALSE;
	BOOL bAppendixWithNo;
	INT nAppendixReturn;

	if( m_nReturnType == 0 && m_pSpfWork->m_pManageValue->m_sFeeMngSys != _T("Y") ) 
	{
		nCount = 1;
	}
	else 
	{
		nCount = m_nSelectGetCount;
	}

	for( INT i = 0; i < nCount; i++ )
	{
		bAppendixWithNo = FALSE;

		ids = ReturnBook();
		nAppendixReturn = ids;

		if( CANCEL_RETURN_BOOK == ids )
		{
			return;
		}
		else if(0 > ids) 
		{
			AfxMessageBox(_T("기능을 정상적으로 수행하지 못하였습니다."));
			return;
		}
		else if( ids > 0 && (m_nReturnType == 0) )
		{
			if(2000 == ids)
			{
				InitValueSetting(FALSE);				
				if(0 >= m_nSelectGetCount)
				{					
					sStatus = _T("1");
				}
				else
				{
					ViewScreen();
				}				
			}
			return ;
		}
		else if( ids > 0 && (m_nReturnType == 1 || m_nReturnType == 2) )
		{
			if( nAppendixReturn == 1000 )
			{
				bAppendixWithNo = TRUE;
			}
			ids = UpdateChildScreen();
			if(0 > ids) 
			{
				return ;
			}
		}
		else
		{
			if( nAppendixReturn == 1000 )
			{
				bAppendixWithNo = TRUE;
			}	
			m_bLastWork = TRUE;
		}
	}

	if( !m_bLastWork )
	{
		m_bLastWork;
		return;
	}
	
	CString strQuery = _T(""), strResult = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	strQuery.Format(_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE SPECIES_KEY IN ( %s ) AND STATE IN ('0001', '0002','0004', '0014') AND LOAN_KEY IS NULL AND BOOK_APPENDIX_FLAG='A'"), m_strReturnMatSpKeyList);
	if(m_strReturnMatSpKeyList.IsEmpty() == FALSE){
		ids = TmpDM.GetOneValueQuery( strQuery, strResult );
		
		if ( _ttoi(strResult) > 0 ) {
			CString strMsg;
			strMsg.Format(_T("반납처리된 모체 자료중에 %s건의 부록자료가 대출을 위해 책두레진행중입니다.\n부록만 별도로 대출하려는 경우가 아니라면 확인하십시오."), strResult);
			AfxMessageBox(strMsg);
		}
	}

	CESL_Mgr* pMSGParent = pParentMgr;

	if ( m_bShowWindow && m_nReturnType == 0 )
	{
		if ( m_nCurrentPos != m_nLastPos )
		{
			pMSGParent = this;
		}
		else
		{			
			ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출상태"), sStatus , m_nCurrentPos );
		    if (0 > ids) 
			{
				ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnRETURNSAVE") );
			}
		    if ( !( (sStatus.Compare(_T("1"))==0) || (sStatus.Compare(_T("반납"))==0) ) ) 
			{
				pMSGParent = this;			   
			}
		}
	}

	if( bAppendixWithNo == TRUE )
	{
		m_pSpfWork->SelfCloseMessageBox( _T("반납이 취소되었습니다."), pMSGParent );
	}
	else
	{
		m_pSpfWork->SelfCloseMessageBox( _T("반납이 완료되었습니다."), pMSGParent );
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
	INT nRowCount = pDM->GetRowCount();
	m_pSpfWork->SetEND_SBL(nRowCount);

}

INT CBO_LOC_3130::ReturnBook(BOOL bAppendixReturn/*=FALSE*/)
{
EFS_BEGIN

	INT ids;
		
 	ids = IsReturn();
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ReturnBook"));
	}
	if( ids > 0 ) 
	{
		return 1;
	}
	

	if(_T("Y") == m_pSpfWork->m_pManageValue->m_sIsUnityLoanService)
	{
		CString strInterlibraryLoan;
		ids = GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("지역내상호대차상태"), strInterlibraryLoan, m_nCurrentPos);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("ReturnBook"));
		if(_T("Y") == strInterlibraryLoan)
		{
			AfxMessageBox(_T("지역내 상호대차 자료입니다.\r\n지역내 상호대차 프로그램을 이용해 주십시오."));
			return 1;
		}
	}
	INT	nReturnMode = 0;
	
	
	BOOL bReturn = FALSE;
	INT nReturnTypeMode = 0;
	int nOtherType = 0, nOtherReturn = 0;
	CString strRegNo, strLoanLibCode, strOtherLoanKey, strLCTKEY;
	
	GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출도서관부호"), strLoanLibCode, m_nCurrentPos);
	GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("등록번호"), strRegNo, m_nCurrentPos);			
	GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("타관대출KEY"), strOtherLoanKey, m_nCurrentPos);
	GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("LCT_KEY"), strLCTKEY, m_nCurrentPos);
	
	nReturnTypeMode = m_pSpfWork->BookReturnTypeCheck(strRegNo);

	CString strMessage = _T("");
	
	CKL_UserSearchDlg dlg(this, m_pSpfWork);
	
	switch(nReturnTypeMode)
	{
		case _RETURN_OTHER_MANAGE:
		case _RETURN_OTHER_MANAGE_HOLD:
			nReturnMode = 1;
			break;
		case _RETURN_NORMAL_TO_KL:
		case _RETURN_KL_TO_KL:
			dlg.m_strRegNo = strRegNo;
			dlg.m_strLibCode = strLoanLibCode;
			dlg.m_strOtherFreeYN = m_pSpfWork->m_strOtherFreeYN;
			dlg.m_strLocalFreeYN = m_pSpfWork->m_strLocalFreeYN;
			dlg.m_nMode = 3;
			if(IDOK == dlg.DoModal())
			{
				ids = m_pSpfWork->KL_UserInfoView();
				return 2000;
			}else{
				nOtherReturn = CANCEL_RETURN_BOOK;
			}
			bReturn = TRUE;
			break;
		case _RETURN_LILL_TO_BOOKLIB:
			break;
		case _RETURN_LILL_TO_LILL:
		
			if(!strOtherLoanKey.IsEmpty())
			{
				m_pSpfWork->OtherReturn();
				nOtherReturn = 2000;
			}
			else if( !strLCTKEY.IsEmpty() && !strLoanLibCode.IsEmpty() && strLoanLibCode != m_pInfo->LIB_CODE )
			{
				m_pSpfWork->OtherReturn(1);
				nOtherReturn = 2000;
			}			
			bReturn = TRUE;
			break;
		case _RETURN_FAIL:
			nOtherReturn = -1;
			bReturn = TRUE;
			break;
		case _MESSAGE_NO_USE_KL_LILL:
			strMessage.Format(	_T("책이음상호대차 자료입니다.\r\n")
				_T("대출 도서관에 반납하시기 바랍니다."));
			m_pSpfWork->SelfCloseMessageBox(strMessage, NULL, 4000);
			bReturn = TRUE;
			break;
		case _MESSAGE_NO_USE_ILL:
			strMessage.Format(	_T("책두레 자료입니다.\r\n")
				_T("대출 도서관에 반납하시기 바랍니다."));
			m_pSpfWork->SelfCloseMessageBox(strMessage, NULL, 4000);
			bReturn = TRUE;
			break;
		case _MESSAGE_NO_USE_KL:
			strMessage.Format(	_T("책이음 자료입니다.\r\n")
				_T("대출 도서관에 반납하시기 바랍니다."));
			m_pSpfWork->SelfCloseMessageBox(strMessage, NULL, 4000);
			bReturn = TRUE;
			break;
			//
		default :
			break;
	}

	if(bReturn == TRUE){
		return nOtherReturn;
	}


	if( m_pSpfWork->m_pManageValue->m_sFeeMngSys != _T("Y") )
	{	
		ids = AppendixReturn();
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("ReturnBook"));
		}
		if( ids == 1 ) 
		{
			return 1000;
		}
		else if( ids > 0 )
		{
			return 1; 
		}
	}

 	if(TRUE == m_bIsILL) 
 	{
 		nReturnMode = 0;
 	}
	ids = ReturnDBUpdate(nReturnMode, nReturnTypeMode);
	if(ids == 2)
		return CANCEL_RETURN_BOOK;
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("ReturnBook"));
	}
	if( CANCEL_RETURN_BOOK == ids )
	{
		return CANCEL_RETURN_BOOK;
	}
	
	CString	strShelfLocCode;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("자료실"), strShelfLocCode , m_nCurrentPos );	
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1060, _T("ReturnBook"));
	}

	ids = UpdateParentScreen();
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1070, _T("ReturnBook"));
	}

	if( 1 != nReturnMode )
	{
		if( !bAppendixReturn )
		{
			ids = RecordReservationExpireDate();
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1080, _T("ReturnBook"));
			}
		}
	}

	if( !bAppendixReturn )
	{
		ids = UpdateChildScreen();
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1090, _T("ReturnBook"));
		}
	}
	
	CString	strMsg;
	CString	strConvertShelfLocCode;
	m_pInfo->pCodeMgr->ConvertDescToCode( _T("자료실구분"), strShelfLocCode , strConvertShelfLocCode );
	if ( m_strDisplayShelfLocCode.Find(strConvertShelfLocCode) >= 0 )
	{
		CString	strMsg;
		strMsg.Format(_T("반납된 자료의 자료실은 [ %s ] 입니다."), strShelfLocCode);
		AfxMessageBox(strMsg);
	}
	if( nReturnTypeMode == _RETURN_OTHER_MANAGE || nReturnTypeMode == _RETURN_OTHER_MANAGE_HOLD)
	{
		CString strQuery, strResult, strLoanManageCode;

		ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("관리구분"), strLoanManageCode , m_nCurrentPos );	
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("ReturnBook"));

		strQuery.Format( _T("SELECT MAX(DESCRIPTION) FROM CD_CODE_TBL WHERE CLASS=1 AND CODE='%s'"), strLoanManageCode );

		CESL_DataMgr DM;
		DM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
		ids = DM.GetOneValueQuery( strQuery, strResult );
		if(0 > ids&& -4007 != ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1100, _T("ReturnBook"));
		}
	
		CString strRegNo;
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("등록번호"), strRegNo , m_nCurrentPos );	
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1110, _T("ReturnBook"));
		}

		if( 0 == strResult.GetLength() )
		{
			strResult = strLoanManageCode;
		}

		CString strMsg;
		strMsg.Format( _T("반납된 자료( %s )는 [ %s ] 자료입니다."), strRegNo, strResult );
		AfxMessageBox(strMsg);
	}

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3130::ReturnDBUpdate(INT nReturnMode, INT nReturnTypeMode)
{
EFS_BEGIN

	INT ids;
	CString strBookManageCode;

	CESL_DataMgr *pUpdateDM = FindDM( _T("DM_BO_LOC_3100_DB") );
	if( pUpdateDM == NULL ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ReturnDBUpdate"));
	}

	CESL_DataMgr *pKLDM = FindDM(_T("DM_KL_RETURN"));
	if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ReturnDBUpdate"));
	CESL_DataMgr *pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("ReturnDBUpdate"));
	CString strKLUserYN;
	pMemberDM->GetCellData(_T("통합회원여부"), 0, strKLUserYN);
	if(_T("Y") != strKLUserYN)
	{
		strKLUserYN = _T("N");
	}

	CString strOtherLoanYN;

	if(_T("Y") == m_pSpfWork->m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{

		GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("타관대출상태"), strOtherLoanYN, m_nCurrentPos);
		if (_T("Y") == strOtherLoanYN) {
			CString strAppendixSendYN;
			GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록발송유무"), strAppendixSendYN, m_nCurrentPos);
			if (_T("Y") == strAppendixSendYN) {
				CString strMsg, strRegNo, strTitle, strAppendixRegNoList;
				GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("등록번호"), strRegNo, m_nCurrentPos);
				GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("본표제"), strTitle, m_nCurrentPos);
				GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록등록번호리스트"), strAppendixRegNoList, m_nCurrentPos);
				strMsg.Format(
					_T("부록자료와 함께 대출된 자료입니다.\r\n")
					_T("\r\n")
					_T("[ %s:%s ]\r\n")
					_T("부록자료를 확인 하셨습니까?\r\n")
					_T(" - 부록등록번호 : %s\r\n")
					_T("( 예 : 반납,  아니오 : 반납취소 )")
					, strRegNo, strTitle, strAppendixRegNoList);
				if (IDYES != MessageBox(strMsg, _T("타관대출>자관반납>부록확인"), MB_ICONINFORMATION|MB_YESNO)) {
					return CANCEL_RETURN_BOOK;
				}
			}
		}

		pKLDM->FreeData();
		pKLDM->InsertRow(-1);
	}
	m_strLibCode = m_pInfo->LIB_CODE;

	CString sLoanKey, sUserKey;
	
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sUserKey , 0 );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("ReturnDBUpdate"));
	}

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출/반납정보KEY"), sLoanKey , m_nCurrentPos );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("ReturnDBUpdate"));
	}
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , pUpdateDM );

	ids = LoanInfoDBUpdate(pMobileApi, nReturnMode, strKLUserYN);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("ReturnDBUpdate"));
	}
	CString strTransNo;
	BOOL bInsertTrans = FALSE;
	BOOL bIsEndTrans = FALSE; 

	if(TRUE == m_bIsILL)
	{
		CString strQuery = _T("");		
		CString strValue[2];

		strQuery.Format(_T("SELECT MAX(TRANS_NO), COUNT(1) FROM ILL_TRANS_TBL WHERE LOAN_KEY = %s"), sLoanKey);
		ids = pUpdateDM->GetValuesQuery(strQuery, strValue, 2);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("ReturnDBUpdate"));
		}

		strTransNo = strValue[0];

		if(TRUE == strTransNo.IsEmpty())
		{
			ids = ILL_InsertTransInfo();
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("ReturnDBUpdate"));
			}
			if(ids == 2)
				return 2;
			strTransNo = m_strTransNo;
			bInsertTrans = TRUE;
		}
		else if(_T("1") == strValue[1])
		{   	
			ids = ILL_UpdateTransInfo(bIsEndTrans);
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1060, _T("ReturnDBUpdate"));
			}			
			if(ids == 2)
				return 2;				
		}
		else
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1070, _T("ReturnDBUpdate"));
		}
	}

	if( m_pSpfWork->m_pManageValue->m_sFeeMngSys == _T("Y") && FALSE == m_bIsILL && m_pSpfWork->m_pManageValue->m_ArrearCheckYN != _T("Y"))
	{
		ids = FeeInfoDBUpdate(pMobileApi, sLoanKey, strKLUserYN);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1080, _T("ReturnDBUpdate"));
		}
	}
	else
	{
		ids = LoanUserInfoDBUpdate(pMobileApi, strKLUserYN);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1090, _T("ReturnDBUpdate"));
		}
		if( CANCEL_RETURN_BOOK == ids )
		{
			return CANCEL_RETURN_BOOK;
		}
	}

 	if( nReturnMode == 0								||
		nReturnTypeMode == _RETURN_NORMAL				||
		nReturnTypeMode == _RETURN_ILL_TO_BOOKLIB		||
		nReturnTypeMode == _RETURN_LILL_TO_BOOKLIB		
		)
	{
		BOOL bIsSetILLState = FALSE;
		if(TRUE == m_bIsILL)
	{
			bIsSetILLState = (TRUE == bIsEndTrans) ? FALSE : TRUE;
		}		
		if(nReturnTypeMode == _RETURN_PRESERVATION_TO_LIB)
			bIsSetILLState = FALSE;

		ids = LoanBookInfoDBUpdate( pMobileApi, bIsSetILLState);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1100, _T("ReturnDBUpdate"));
		}
	}
	ids = pMobileApi->SendFrame();
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1110, _T("ReturnDBUpdate"));
	}
	pMobileApi->EndFrame();
	
	CString sLoanStatus;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출상태"), sLoanStatus , m_nCurrentPos );
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("IsReturn"));
	}

	CString strBookLibCode, strQuery;
	strQuery.Format(_T("SELECT GET_BOOK_LIB_CODE(PUBLISH_FORM_CODE, BOOK_KEY ) FROM LS_WORK_T01 WHERE REC_KEY = %s "), sLoanKey);
	ids = pUpdateDM->GetOneValueQuery(strQuery, strBookLibCode);

	if ( strBookLibCode.GetLength() == 0 ) {
		ERROR_MSG_RETURN_ERROR_CODE2(-2000, _T("IsReturn"));
	}

	if(nReturnTypeMode == _RETURN_LILL_TO_BOOKLIB){
		CString strUserNo, strLocalKey, strReturnDate, strLoanStopDate;
		pMemberDM->GetCellData(_T("대출자번호"), 0, strUserNo);
		pKLDM->GetCellData(_T("로컬KEY"), 0, strLocalKey);
		pKLDM->GetCellData(_T("반납일"), 0, strReturnDate);
		pKLDM->GetCellData(_T("대출정지일"), 0, strLoanStopDate);


		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("LILL_K07_LOC_B02_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("LOCAL_KEY"),strLocalKey);
		Writer.addElement(_T("RETURN_DATE"),strReturnDate);
		Writer.addElement(_T("RETURN_LIB_CODE"),m_pInfo->LIB_CODE);
		Writer.addElement(_T("USER_NO"),strUserNo);
		if(!strLoanStopDate.IsEmpty())
		{
			Writer.addElement(_T("LOAN_STOP_DATE"), strLoanStopDate);
			Writer.addElement(_T("LOAN_STOP_SET_DATE"), strReturnDate);
		}

 		CFrameSender Sender(m_pInfo);
 		Sender.SendTempTbl(&Writer, GetWorkLogMsg(_T("책이음지역상호대차 자관반납"), __WFILE__, __LINE__));

	}
	else
	{
		if(_T("Y") == m_pSpfWork->m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
		{
			// 프레임생성
			CFrameWriter Writer(m_pInfo);
			Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
			Writer.addRecord();
			CString strGpinHash;
			GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("G-PINHASH인증코드"), strGpinHash, 0);
			Writer.addElement(_T("G-PINHASH인증코드"), strGpinHash);
			CString strGetData;
			pMemberDM->GetCellData(_T("대출자번호"), 0, strGetData);
			Writer.addElement(_T("통합대출자번호"), strGetData);
			
			Writer.addElement(_T("대출도서관부호"), strBookLibCode);
			Writer.addElement(_T("반납도서관부호"), strBookLibCode);

			CString strReturnDate;
			pKLDM->GetCellData(_T("반납일"), 0, strReturnDate);
			Writer.addElement(_T("반납일"), strReturnDate);
			ids = GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("등록번호"), strGetData, m_nCurrentPos);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("ReturnDBUpdate"));
			Writer.addElement(_T("등록번호"), strGetData);
			Writer.addElement(_T("상태"), _T("1"));
			CString strLoanStopDate;
			pKLDM->GetCellData(_T("대출정지일"), 0, strLoanStopDate);
			
			if(!strLoanStopDate.IsEmpty())
			{
				Writer.addElement(_T("회원상태"), _T("1"));
				Writer.addElement(_T("대출정지일"), strLoanStopDate);
				Writer.addElement(_T("대출정지부여일"), strReturnDate);
			}
			else
			{
				Writer.addElement(_T("회원상태"), _T("0"));
			}
			pKLDM->GetCellData(_T("로컬KEY"), 0, strGetData);
			Writer.addElement(_T("로컬KEY"), strGetData);
			if ( m_bIsILL == TRUE && m_bIsForceILL == FALSE ) {
				Writer.addElement(_T("지역내상호대차상태"), _T("Y"));
			}
			CFrameSender Sender(m_pInfo);
			Sender.SendTempTbl(&Writer, GetWorkLogMsg(_T("통합회원반납"), __WFILE__, __LINE__));
		}
	}

	CLocCommonAPI::MobileApiEndLog( pMobileApi );
	
	CString strLoanManageCode;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("관리구분"), strLoanManageCode , m_nCurrentPos );
	if (0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1120, _T("ReturnDBUpdate"));
	}

	m_strLibCode = m_pInfo->LIB_CODE;

	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sLoanKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS,RETURN_TYPE_CODE,REPARATION,ARREAR,RETURN_DATE,R_DEVICE,R_WORKER");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	m_pSpfWork->m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sUserKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("LOAN_COUNT,LOAN_STOP_DATE,LOAN_STOP_SET_DATE,DELAY_COUNT,DELAYDAY_COUNT");		
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	m_pSpfWork->m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3130::ILL_RETURN_MESSAGE()
{
	
	INT ids;
	CString sBookRegNo,sBookName,strTemp,strQuery,strValue;
	CString strFuncName = _T("RFID_Return");

	if(ILL_ALERT == TRUE){
		pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
		
		pGrid = (CESL_Grid*)(pCM->FindControl(_T("MainGrid")));

		pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		ids = pGrid->GetIdx();
		pDM->GetCellData(_T("등록번호"),ids,sBookRegNo);
		pDM->GetCellData(_T("본표제"),ids,sBookName);
		strTemp.Format(_T("해당 자료는 책두레 자료입니다.\r\n\r\n등록번호 : %s\r\n서　　명 : %s\r\n\r\n반납처리 하시겠습니까?"),sBookRegNo,sBookName);
		if(AfxMessageBox(strTemp, MB_YESNO) != IDYES){
			return -1;
		}
	}

	return 0;
}

INT CBO_LOC_3130::LoanInfoDBUpdate(CKolas2up_MobileApi *pMobileApi, INT	nReturnMode/*=0*/, CString strKLUserYN/*=_T("N")*/)
{
EFS_BEGIN

	INT		ids;
	CString strLoanStatus = _T("1");
	CString strReturnType;
	CString strRepairMoney;
	CString strArrearMoney;
	CString strPaymentArrear;
	CString strReturnDate;
	CString strRecKey;
	CString strRegNo;
	CString strLog;
	CString strBuf;	
	CString strLoanInfoManageCode;

	ids = GetControlData( _T("CM_BO_LOC_3130"), _T("배상비"), strRepairMoney );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -1, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );

	if( m_pSpfWork->m_pManageValue->m_sFeeMngSys == _T("Y") && FALSE == m_bIsILL)
	{
		CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3130_2") );
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("등록번호"), strRegNo, m_nCurrentPos );
		for( INT i = 0; i< pDM->GetRowCount(); i++ )
		{
			CString sGetData;
			pDM->GetCellData( _T("등록번호"), i, sGetData );
			if( strRegNo == sGetData )
			{
				pDM->GetCellData( _T("납부연체료"), i, strPaymentArrear );
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
				break;
			}	
		}
	}
	else
	{
		ids = GetControlData( _T("CM_BO_LOC_3130"), _T("연체료"), strArrearMoney );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
	}

	if( 1 == m_nReturnType || 2 == m_nReturnType )
	{
		if( !strRepairMoney.IsEmpty() ) 
			strRepairMoney.Format( _T("%d"), _ttoi(strRepairMoney)/m_nSelectGetCount );
		if( !strArrearMoney.IsEmpty() )
			strArrearMoney.Format( _T("%d"), _ttoi(strArrearMoney)/m_nSelectGetCount );
	}

	if( 2 == m_nReturnType )
	{
		ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("소급반납일"), strReturnDate );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );	
	}
	else
	{
		ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("반납일"), strReturnDate );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
	}
	
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출/반납정보KEY"), strRecKey , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("관리구분"), strLoanInfoManageCode , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );

	CString strSpeciesKey = _T("");
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("종KEY"), strSpeciesKey , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -7, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
	if ( m_strReturnMatSpKeyList.Find(strSpeciesKey) < 0 ) {
		if ( m_strReturnMatSpKeyList.GetLength() == 0 ) {
			m_strReturnMatSpKeyList = strSpeciesKey;
		}else {
			m_strReturnMatSpKeyList += _T(",") + strSpeciesKey;
		}
	}

	CESL_DataMgr* pUpdateDM = FindDM( _T("DM_BO_LOC_3100_DB") );
	if( !pUpdateDM ) ERROR_MSG_RETURN_ERROR_CODE2( -7, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );

	pUpdateDM->StartFrame();
	pUpdateDM->InitDBFieldData();
	
	if( 1 == nReturnMode )
	{
		CString strUserManageCode;
	
		ids = m_pCheckLoan->GetUserManageCode( strUserManageCode );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );

		if( strLoanInfoManageCode != strUserManageCode )
		{
			strLoanStatus = _T("6");
		}
	}

	if(TRUE == m_bIsILL)
	{
		strLoanStatus = _T("R");
	}

	ids = pUpdateDM->AddDBFieldData( _T("STATUS"), _T("STRING"), strLoanStatus , TRUE );
	if( !pUpdateDM ) ERROR_MSG_RETURN_ERROR_CODE2( -6, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );

	strReturnType.Format( _T("%d"), m_nReturnType );
	
	ids = pUpdateDM->AddDBFieldData( _T("RETURN_TYPE_CODE"), _T("STRING"), strReturnType , TRUE );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -7, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
	
	if( !strRepairMoney.IsEmpty() )
	{
		ids = pUpdateDM->AddDBFieldData( _T("REPARATION"), _T("NUMERIC"), strRepairMoney , TRUE );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -8, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
	}
	
	if ( _T("Y") == m_pSpfWork->m_pManageValue->m_sUseReceipt )
	{
		CString strLoanFlag, strReturnFlag;
	
		if ( m_pSpfWork->m_pManageValue->m_sUseReceiptType.GetAt(1) == 'Y' ) 
			strReturnFlag = _T("N");
		else
			strReturnFlag = _T("C");
		
		ids = pUpdateDM->AddDBFieldData( _T("RECEIPT_PRINT_RETURN_YN"), _T("STRING"), strReturnFlag );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -81, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );	
	}

	if( m_pSpfWork->m_pManageValue->m_sFeeMngSys == _T("Y") && FALSE == m_bIsILL)
	{
		if( !strPaymentArrear.IsEmpty() )
		{
			ids = pUpdateDM->AddDBFieldData( _T("ARREAR"), _T("NUMERIC"), strPaymentArrear, TRUE );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -9, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
		}
	}
	else
	{
		if( !strArrearMoney.IsEmpty() )
		{
			if(_T("Y") != m_pSpfWork->m_pManageValue->m_sIsIncludeLoanCnt || _T("Y") != m_pSpfWork->m_pManageValue->m_sIsAppendixLoanStop)
			{
				CString strAppendixFlag;
				ids = GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록여부"), strAppendixFlag, m_nCurrentPos);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -9, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
				if(_T("B") != strAppendixFlag) strArrearMoney = _T("0");
			}
			ids = pUpdateDM->AddDBFieldData( _T("ARREAR"), _T("NUMERIC"), strArrearMoney, TRUE );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -9, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
		}
	}
	
	CTime tCurrentTime = CTime::GetCurrentTime();
	strBuf.Format(_T("%s %02d:%02d:%02d"), strReturnDate, tCurrentTime.GetHour(), tCurrentTime.GetMinute(), tCurrentTime.GetSecond());
	strReturnDate.Format( _T("TO_DATE( '%s','YYYY/MM/DD HH24:MI:SS' )"), strBuf);

	ids = pUpdateDM->AddDBFieldData( _T("RETURN_DATE"), _T("NUMERIC"), strReturnDate , TRUE );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -10, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );

	ids = pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -11, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
	

	if( m_pInfo->MODE == 10000 )
	{
		ids = pMobileApi->AddDBFieldData( _T("R_DEVICE"), _T("STRING"), _T("KOLASIII-이동도서관"));
	}
	else
	{
		ids = pMobileApi->AddDBFieldData( _T("R_DEVICE"), _T("STRING"), _T("KOLASIII"));
	}
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -11, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
	}

	ids = pMobileApi->AddDBFieldData( _T("R_WORKER"), _T("STRING"), m_pInfo->USER_ID);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -11, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );
	}
	
	ids = pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -14, _T("CBO_LOC_3130::LoanInfoDBUpdate()") );

	pUpdateDM->InitDBFieldData();

	m_pSpfWork->Send3000ApiSocket(_T("반납"), strRecKey);

	if(_T("Y") == m_pSpfWork->m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		CESL_DataMgr *pKLDM = FindDM(_T("DM_KL_RETURN"));
		if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE2(-14, _T("LoanInfoDBUpdate"));
		pKLDM->SetCellData(_T("반납일"), strBuf, 0);
		pKLDM->SetCellData(_T("상태"), strLoanStatus, 0);
		pKLDM->SetCellData(_T("로컬KEY"), strRecKey, 0);
	}

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_3130::LoanUserInfoDBUpdate(CKolas2up_MobileApi *pMobileApi, CString strKLUserYN/*=_T("N")*/)
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pUpdateDM = FindDM( _T("DM_BO_LOC_3100_DB") );
	if( pUpdateDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::LoanUserInfoDBUpdate()") );
	
	pUpdateDM->InitDBFieldData();
	
	CString sBookCnt;
	CString sUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = GetDataMgrData( sUserDMAlias , _T("총대출책수"), sBookCnt , 0 );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::LoanUserInfoDBUpdate()") );

	CString sIsAppendix;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록여부"), sIsAppendix , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("LoanUserInfoDBUpdate") );
	if(_T("A") == sIsAppendix)
	{
		if(TRUE == m_pCheckLoan->IsAppendixIncludeLoanCnt())
		{
			sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)-1);
		}
	}
	else
	{
		sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)-1);	
	}
	if(1 > _ttoi(sBookCnt))
	{
		sBookCnt = _T("0");
	}
	ids = pUpdateDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sBookCnt, TRUE);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("CBO_LOC_3130::LoanUserInfoDBUpdate()"));
	}

	CString sLoanStopDate;
	m_GetLoanStopDateCnt++;

	CString sDelayDay;
	ids = GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("연체일수"), sDelayDay, m_nCurrentPos);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("GetLoanStopDate"));
	}

	ids = m_pCheckLoan->IsSetLoanStopDate();
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("GetLoanStopDate"));
	}
	if( ids > 0 )
	{
		sDelayDay.Empty();
	}
	if(sDelayDay.IsEmpty())
	{
		sDelayDay = _T("0");
	}
	if(0 < _ttoi(sDelayDay))
	{
		CString sExpLoanDate;
		CString sReturnDate;

		ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("반납예정일"), sExpLoanDate, m_nCurrentPos );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("FeeInfoDBUpdate()") );
		if( sExpLoanDate.IsEmpty() ) return 0;

		CString sReturnType;
		ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FeeInfoDBUpdate()") );

		CString sReturnDateControlAlias = _T("반납일");
		if( sReturnType.Compare( _T("2") )== 0 )
			sReturnDateControlAlias = _T("소급반납일");

		ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias , sReturnDate );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("FeeInfoDBUpdate()") );

		if( sReturnDate.IsEmpty() )
		{
			AfxMessageBox( _T("반납일을 지정하십시오") );
			return 0;
		}

		CString sQuery, sValue, sLoanKey;
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출/반납정보KEY"), sLoanKey , m_nCurrentPos );
		sQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(%s,TO_DATE('%s','YYYY/MM/DD'),UDF_MANAGE_CODE),'YYYY/MM/DD') FROM DUAL"), sLoanKey, sReturnDate);
		pUpdateDM->GetOneValueQuery(sQuery, sValue);
		CString strTempDate = _T("");
		if(sValue.IsEmpty())
		{
			sLoanStopDate = _T("NULL");
		}
		else
		{
			sLoanStopDate.Format(_T("TO_DATE('%s','YYYY/MM/DD')"), sValue);
			strTempDate = sValue + _T(" 00:00:00");
		}
		if(_T("NULL") != sLoanStopDate)
		{
			CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
			CESL_Control_Element *pCE = (CESL_Control_Element*)pCM->FindControlElement(_T("대출정지수동"));
			CButton *pCheck = (CButton*)pParentMgr->GetDlgItem(pCE->CTRL_ID);
			if( pCheck->GetCheck())
			{
				if( m_GetLoanStopDateCnt % 2 )
				{
					CBO_LOC_3133 dlg(sValue, this);
					if(IDOK == dlg.DoModal())
					{
						sLoanStopDate = dlg.m_sLoanStopDate;
						m_sLoanStopDate = sLoanStopDate;
						strTempDate = sLoanStopDate + _T(" 00:00:00");
					}
					else
					{
						m_GetLoanStopDateCnt = 0;
						return CANCEL_RETURN_BOOK;
					}
				}		
				else
				{
					sLoanStopDate = m_sLoanStopDate;
					m_sLoanStopDate.Empty();
					strTempDate = sLoanStopDate + _T(" 00:00:00");
				}
				ids = pUpdateDM->AddDBFieldData(_T("LOAN_STOP_DATE"), _T("STRING"), sLoanStopDate, TRUE);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("FeeInfoDBUpdate()"));
			}
			else
			{
				m_sLoanStopDate = sValue;
				strTempDate = sValue + _T(" 00:00:00");
				ids = pUpdateDM->AddDBFieldData(_T("LOAN_STOP_DATE"), _T("NUMERIC"), sLoanStopDate, TRUE);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("FeeInfoDBUpdate()"));
			}

			INT nTotalDelayCnt = 0;
 			ids = GetTotalDelayCnt(nTotalDelayCnt);
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("CBO_LOC_3130::LoanUserInfoDBUpdate()") );
 			if( nTotalDelayCnt != 0 )
 			{
 				CString sTotalDelayCnt;
 				sTotalDelayCnt.Format(_T("%d"), nTotalDelayCnt);
 				pUpdateDM->AddDBFieldData(_T("DELAY_COUNT"), _T("NUMERIC"), sTotalDelayCnt, TRUE);			
 			}
			ids = pUpdateDM->AddDBFieldData(_T("USER_CLASS"), _T("NUMERIC"), _T("DECODE(USER_CLASS,'0','1',USER_CLASS)"), TRUE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("FeeInfoDBUpdate()"));
 			pUpdateDM->AddDBFieldData(_T("LOAN_STOP_SET_DATE"), _T("NUMERIC"), _T("SYSDATE"), TRUE);


			CString sLog;
			sLog.Format(_T("DELAYDAY_COUNT+%s"), sDelayDay);
			pUpdateDM->AddDBFieldData(_T("DELAYDAY_COUNT"), _T("NUMERIC"), sLog, TRUE);

			if(_T("Y") == m_pSpfWork->m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
			{
				CESL_DataMgr *pKLDM = FindDM(_T("DM_KL_RETURN"));
				if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("LoanUserInfoDBUpdate"));
				pKLDM->SetCellData(_T("회원상태"), _T("1"), 0);
				pKLDM->SetCellData(_T("대출정지일"), strTempDate, 0);
			}
		}
	}

	CString sRecKey;
	ids = GetDataMgrData( sUserDMAlias , _T("대출자KEY"), sRecKey , 0 );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -7, _T("CBO_LOC_3130::LoanUserInfoDBUpdate()") );

	pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("반납"), __WFILE__, __LINE__ ), TRUE );
	ids = pMobileApi->MakeUpdateFrame( _T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -8 , _T("CBO_LOC_3130::LoanUserInfoDBUpdate()") );
	
	pUpdateDM->InitDBFieldData();

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3130::LoanBookInfoDBUpdate(CKolas2up_MobileApi *pMobileApi, BOOL bIsSetILLState)
{
EFS_BEGIN

	INT ids;
	INT nBookType = 0;
	BOOL bOtherReturn = FALSE;
	CString strGetPrevStatus, strUpStatus, sPublishForm, sTableName, strScreenStatus, strLoanLibCode, sPreWorkingStatus, sRecKey;
	
	CESL_DataMgr *pUpdateDM = FindDM( _T("DM_BO_LOC_3100_DB") );
	if( pUpdateDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );
	
	pUpdateDM->InitDBFieldData();

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("단행/연속구분"), sPublishForm , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );	
	
	if( sPublishForm.Compare(_T("MO"))==0 ) {
		sTableName = _T("BO_BOOK_TBL");
		nBookType = 1;
	}
	else if( sPublishForm.Compare(_T("SE"))==0 ) {
		sTableName = _T("SE_BOOK_TBL");
		nBookType = 2;
	}
	else if( sPublishForm.Compare(_T("NB"))==0 )
		sTableName = _T("CO_NON_DB_BOOK_TBL");

	ids = GetControlData( _T("CM_BO_LOC_3130"), _T("반납책상태"), strScreenStatus );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );

	ids = GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출도서관부호"), strLoanLibCode, m_nCurrentPos);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("이전자료상태"), strGetPrevStatus , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );
	
	if(!strLoanLibCode.IsEmpty() && strLoanLibCode != m_pInfo->LIB_CODE) bOtherReturn = TRUE;

	if( sPublishForm.Compare(_T("MO"))==0 ) {
		if( strScreenStatus.Compare(_T("0") )== 0 ) {
			strUpStatus = _T("BOL112N");
		}
		else if( strScreenStatus.Compare(_T("1")) == 0 ) {
			strUpStatus = _T("BOL114O");
		}
		else {
			strUpStatus = _T("BOL252N");
		}
	}
	else if( sPublishForm.Compare(_T("SE"))==0 ) {			
		if( strScreenStatus.Compare(_T("0") )== 0 ) {
			if ( strGetPrevStatus == _T("SEL112N") ) strUpStatus = _T("SEL112N");
			else	strUpStatus = _T("SEL212N");
		}
		else if( strScreenStatus.Compare(_T("1")) == 0 ) {
			strUpStatus = _T("SEL214N");
		}
		else {
			strUpStatus = _T("SEL216O");
		}
	}
	else if( sPublishForm.Compare(_T("NB"))==0 ) {
		if( strScreenStatus.Compare(_T("0") )== 0 ) {
			strUpStatus = _T("BOL112N");
		}
		else if( strScreenStatus.Compare(_T("1")) == 0 ) {
			strUpStatus = _T("BOL114O");
		}
		else {
			strUpStatus = _T("BOL252N");
		}
	}	
	
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("상태"), strGetPrevStatus , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );

	if(TRUE == bIsSetILLState)
	{	
		strGetPrevStatus = strUpStatus;
		if(_T("MO") == sPublishForm) {
			strUpStatus = _T("BOL411O");
		}
		else if(_T("SE") == sPublishForm) {
			strUpStatus = _T("SEL411O");
		}
		else if(_T("NB") == sPublishForm) {
			strUpStatus = _T("BOL411O");
		}
	}

#ifdef __K2UP__
	pUpdateDM->AddDBFieldData( _T("PREV_WORKING_STATUS"), _T("STRING"), strGetPrevStatus , TRUE );
#endif	

	ids = pUpdateDM->AddDBFieldData( _T("WORKING_STATUS"), _T("STRING"), strUpStatus , TRUE );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );

	if( _ttoi(strScreenStatus) == 2 )
	{
		CString sShelfStatusChangeDate=_T("SYSDATE");
		pUpdateDM->AddDBFieldData( _T("LAST_SHELF_STATUS_CHANGE_DATE"), _T("NUMERIC"), sShelfStatusChangeDate , TRUE );
	}

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("책KEY"), sRecKey , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );

	pUpdateDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );

	ids = pMobileApi->MakeUpdateFrame( sTableName , _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	
	pUpdateDM->InitDBFieldData();

	CString sSpeciesKey;
	GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO") , _T("종KEY") , sSpeciesKey , 0 );
	m_pSpfWork->AddIdxQueryFrame( pUpdateDM, nBookType, sSpeciesKey );

	CString strAppendixSendYN;
	GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록발송유무"), strAppendixSendYN, m_nCurrentPos);
	if ("Y" == strAppendixSendYN) {
		CString strAppendixRegNoList;
		GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록등록번호리스트"), strAppendixRegNoList, m_nCurrentPos);
		if (!strAppendixRegNoList.IsEmpty()) {
			CStringArray asAppendixRegNoList;
			CLocCommonAPI::MakeArrayList(strAppendixRegNoList, _T(","), asAppendixRegNoList);
			CString strQuery;
			for (INT i = 0; i < asAppendixRegNoList.GetSize(); i++) {
				strQuery.Format(
					_T("UPDATE BO_BOOK_TBL ")
					   _T("SET WORKING_STATUS='%s',")
						   _T("PREV_WORKING_STATUS=WORKING_STATUS,")
						   _T("LAST_WORK='%s' ")
					 _T("WHERE REG_NO='%s';")
						 , strUpStatus, GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), asAppendixRegNoList.GetAt(i));
				pMobileApi->AddFrame(strQuery);
			}
		}
	}

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_3130::UpdateParentScreen()
{
EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid;
	ids = FindGridControl(&pGrid);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("UpdateParentScreen"));
	}

	INT nStatusColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs( this, pGrid, _T("대출상태"), nStatusColIndex );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("UpdateParentScreen"));
	}

	if( nStatusColIndex != -1 )
	{
		ids = pGrid->SetAt( m_nCurrentPos, nStatusColIndex, _T("1") );
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("UpdateParentScreen"));
		}
	}

	ids = SetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출상태"), _T("1"), m_nCurrentPos );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("UpdateParentScreen"));
	}

 	CString sBookCnt;
 	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("총대출책수"), sBookCnt, 0 );
 	if(0 > ids) 
 	{
 		ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("UpdateParentScreen"));
 	}
 
 	CString sIsAppendix;
 	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록여부"), sIsAppendix, m_nCurrentPos );
 	if(0 > ids) 
 	{
 		ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("UpdateParentScreen"));
 	}
 	if( sIsAppendix.Compare(_T("A")) == 0  )
 	{
 		if( m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0 )
 		{
 			sBookCnt.Format( _T("%d"), _ttoi(sBookCnt)+1 );
 		}
 	}
 	sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)-1); 
 
 	if ( _T("Y") == m_pCheckLoan->m_pManageValue->m_sIsUnionLoanCnt )
 	{
 		CString strUnionCnt;
 
 		ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합대출책수"), strUnionCnt, 0 );
 		if(0 > ids) 
 		{
 			ERROR_MSG_RETURN_ERROR_CODE2(-1060, _T("UpdateParentScreen"));
 		}
 
 		if( !strUnionCnt.IsEmpty() )
 		{
 			if( 0 == sIsAppendix.Compare( _T("A") ) )
 			{
 				if( 0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() )
 				{
 					strUnionCnt.Format( _T("%d"), _ttoi(strUnionCnt)-1 );
 				}
 			}
 			strUnionCnt.Format( _T("%d"), _ttoi(strUnionCnt)+1 );
 		}
	}	
 
 	CString sReturnType;
 	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType );
 	if(0 > ids) 
 	{
 		ERROR_MSG_RETURN_ERROR_CODE2(-1070, _T("UpdateParentScreen"));
 	}
 
 	CString sReturnDateControlAlias = _T("반납일");
 	if( sReturnType.Compare(_T("2"))== 0 )
 	{
 		sReturnDateControlAlias = _T("소급반납일");
 	}
 
 	CString sReturnDate;
 	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate );
 	if(0 > ids) 
 	{
 		ERROR_MSG_RETURN_ERROR_CODE2(-1080, _T("UpdateParentScreen"));
 	}
 
 	ids = SetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("총대출책수"), sBookCnt, 0 );
 	if(0 > ids) 
 	{
 		ERROR_MSG_RETURN_ERROR_CODE2(-1090, _T("UpdateParentScreen"));
 	}

 	CTime		time	=	CTime::GetCurrentTime();
 	CString		strReturnDateTemp;
 	strReturnDateTemp.Format( _T("%s %02d:%02d") , sReturnDate , time.GetHour() , time.GetMinute() );
 	ids = SetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO") , _T("반납일") , strReturnDateTemp , m_nCurrentPos );
 	if(0 > ids) 
 	{
 		ERROR_MSG_RETURN_ERROR_CODE2(-1100, _T("UpdateParentScreen"));
 	}
 
	if(!m_sLoanStopDate.IsEmpty())
 	{
 		ids = SetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출정지일"), m_sLoanStopDate, 0 );
 		if(0 > ids)
 		{
 			ERROR_MSG_RETURN_ERROR_CODE2(-1130, _T("UpdateParentScreen"));
 		}
 	}

	ids = m_pSpfWork->AddMemberInfo( TRUE );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1140, _T("UpdateParentScreen"));
	}

	ids = AllControlDisplay( _T("CM_BO_LOC_3100_MEMBER_INFO"), 0 );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1150, _T("UpdateParentScreen"));
	}
	
	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3130::UpdateChildScreen()
{
EFS_BEGIN

	MoveRecord( 1 );

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3130::IsReturn()
{
	EFS_BEGIN

	INT ids;
	CString strQuery, strNLoanToIllReturn, strLoanKey, strAplLibCode;

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);	
	strQuery.Format(
			_T("SELECT VALUE_1 ")
			_T("FROM ILL_MANAGE_TBL ")
			_T("WHERE VALUE_NAME = '타관반납기능사용유무' AND MANAGE_CODE = '%s'"),
			m_pInfo->MANAGE_CODE);	
	ids = TmpDM.GetOneValueQuery(strQuery, strNLoanToIllReturn);
	if( ids == -4007) strNLoanToIllReturn = _T("Y");
	else if (ids < 0 ) {
		ERROR_MSG_RETURN_ERROR_CODE2(-900, _T("IsReturn"));
	}
	if (strNLoanToIllReturn.GetLength() == 0 ) strNLoanToIllReturn = _T("Y");
	
	
	CString sLoanStatus;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출상태"), sLoanStatus , m_nCurrentPos );
	
	INT ids2;
	CString strOtherLoanKey, strTranCode;
	ids2 = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("타관대출KEY"), strOtherLoanKey , m_nCurrentPos );
	ids2 = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("트랜잭션상태"), strTranCode , m_nCurrentPos );

	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("IsReturn"));
	}

	CString sRegNo;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("등록번호"), sRegNo , m_nCurrentPos );
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("IsReturn"));
	}

	CString sMsg;
	if(_T("1") == sLoanStatus)
	{
		sMsg.Format( _T("등록번호가 '%s'인 이 책은 이미 반납된 책입니다."), sRegNo );
		m_pSpfWork->ViewMessage( sMsg );
		return 1;
	}
	else if(_T("3") == sLoanStatus)
	{
		if(strOtherLoanKey.IsEmpty())
		{
			sMsg.Format( _T("등록번호가 '%s'인 이 책은 예약된 책이기 때문에 반납을 할 수 없습니다."), sRegNo );
			m_pSpfWork->ViewMessage( sMsg );
			return 1;
		}
		else if(!strOtherLoanKey.IsEmpty())
		{
			sMsg.Format( _T("등록번호가 '%s'인 이 책은 반납이 불가능한 상태입니다."), sRegNo );
			m_pSpfWork->ViewMessage( sMsg );
			return 1;
		}
	}
	
	else if(_T("4") == sLoanStatus)
	{
			CString sMsg;
			sMsg.Format( _T("등록번호가 '%s'인 이 책은 예약 취소된 책이기 때문에 반납을 할 수 없습니다."), sRegNo );
			m_pSpfWork->ViewMessage( sMsg );
			return 1;
	}
	
	if(!strOtherLoanKey.IsEmpty())
	{
		if(_T("0") == strTranCode || _T("1") == strTranCode || _T("3") == strTranCode)
		{
			CString sMsg;
			sMsg.Format( _T("등록번호가 '%s'인 이 책은 반납이 불가능한 상태입니다."), sRegNo );
			m_pSpfWork->ViewMessage( sMsg );
			return 1;				
		}
	}	
	if(FALSE == m_bIsILL)
	{
		
 		if(_T("L") == sLoanStatus || _T("D") == sLoanStatus)
 		{
 			sMsg.Format( _T("등록번호가 '%s'인 이 책은 책두레대상자료입니다.\r\n책두레 반납메뉴를 통해 반납해 주십시오."), sRegNo );
 			m_pSpfWork->ViewMessage( sMsg );
 			return 1;
 		}
 		else if(_T("R") == sLoanStatus)
 		{
 			sMsg.Format( _T("등록번호가 '%s'인 이 책은 책두레대상자료로 이미 반납된 자료입니다."), sRegNo );
 			m_pSpfWork->ViewMessage( sMsg );
 			return 1;
 		}
	}
	else
	{
		if(_T("L") != sLoanStatus && _T("D") != sLoanStatus)
		{
			if(_T("0") == sLoanStatus || _T("2") == sLoanStatus)
			{
				CString strManageCode;
				ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("관리구분"), strManageCode, m_nCurrentPos );
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("IsReturn"));
				}

				if(strManageCode == m_pInfo->MANAGE_CODE)
				{
					sMsg.Format(
						_T("등록번호가 '%s'인 이 책은 자관대출된 자료입니다. 일반 반납메뉴를 통해 반납해 주십시요."), sRegNo);
					m_pSpfWork->ViewMessage( sMsg );
					return 1;
				}
				else{
					BOOL bReturn = FALSE;
					ids = m_pSpfWork->BookReturnTypeCheck(sRegNo);
					switch(ids)
					{
					case _MESSAGE_NO_USE_KL_LILL:
						sMsg.Format(	_T("책이음상호대차 자료입니다.\r\n")
										_T("대출 도서관에 반납하시기 바랍니다."));
						m_pSpfWork->ViewMessage( sMsg );
						bReturn = TRUE;
						break;
					case _MESSAGE_NO_USE_ILL:
						sMsg.Format(	_T("책두레 자료입니다.\r\n")
										_T("대출 도서관에 반납하시기 바랍니다."));
						m_pSpfWork->ViewMessage( sMsg );
						bReturn = TRUE;
						break;
					case _MESSAGE_NO_USE_KL:
						sMsg.Format(	_T("책이음 자료입니다.\r\n")
										_T("대출 도서관에 반납하시기 바랍니다."));
						m_pSpfWork->ViewMessage( sMsg );
						bReturn = TRUE;
						break;
					case _RETURN_FAIL:
						bReturn = TRUE;
						break;
					default :
						break;
					}

					if(bReturn == TRUE){
						return 1;
					}
				}
			}
			else
			{
				CString strStatus = _T("");
				if(_T("1") == sLoanStatus)
				{
					strStatus = _T("반납된 자료");
				}
				else if(_T("3") == sLoanStatus)
				{
					strStatus = _T("예약된 자료");
				}
				else if(_T("4") == sLoanStatus)
				{
					strStatus = _T("예약취소된 자료");
				}
				else if(_T("R") == sLoanStatus)
				{
					strStatus = _T("책두레반납된 자료");
				}
				else 
				{
					strStatus.Format(_T("정의되지 않은 상태값(%s)"), sLoanStatus);
				}

				sMsg.Format(_T("등록번호가 '%s'인 이 책의 상태는 [%s]이며 책두레에서 반납가능한 자료가 아닙니다.")
							, sRegNo, strStatus);
				m_pSpfWork->ViewMessage( sMsg );
				return 1;
			}
		}
		else {
			if ( strNLoanToIllReturn == _T("N") && _T("N") == m_pSpfWork->m_pManageValue->m_sIsUnityLoanService ) {
				ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출/반납정보KEY"), strLoanKey , m_nCurrentPos );
				if(0 > ids) {
					ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("IsReturn"));
				}

				strQuery.Format( _T("SELECT (SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL L WHERE L.LIB_CODE=T.APL_LIBCODE) ")
					             _T("FROM ILL_TRANS_TBL T WHERE LOAN_KEY=%s"), strLoanKey);
								
				ids = TmpDM.GetOneValueQuery(strQuery, strAplLibCode);
				if( ids == -4007) strAplLibCode = _T("");
				else if (ids < 0 ) {
					ERROR_MSG_RETURN_ERROR_CODE2(-900, _T("IsReturn"));
				}
				CString strBookManageCode;
				strQuery.Format( _T("SELECT MANAGE_CODE ")
					             _T("FROM BO_BOOK_TBL ")
								 _T("WHERE REC_KEY=(SELECT BOOK_KEY FROM ILL_TRANS_TBL WHERE LOAN_KEY=%s)"), strLoanKey);
								
				ids = TmpDM.GetOneValueQuery(strQuery, strBookManageCode);
				if( ids == -4007) strBookManageCode = _T("");
				else if (ids < 0 ) {
					ERROR_MSG_RETURN_ERROR_CODE2(-900, _T("IsReturn"));
				}				

				if(strBookManageCode != m_pInfo->MANAGE_CODE) {
					if ( strAplLibCode != m_pInfo->MANAGE_CODE) {
						sMsg.Format(_T("등록번호가 '%s'인 자료는 우리관에서 대출한 책두레자료가 아니여서 반납받을 수 없습니다.") , sRegNo);
						m_pSpfWork->ViewMessage( sMsg );
						return 1;
					}
				}
			}
		}
	}
	m_bIsForceILL = FALSE;
	
	if(FALSE == m_bIsILL)
	{
	CString sBookManageCode;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("관리구분"), sBookManageCode , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("IsReturn"));
	
	ids =  m_pSpfWork->m_pCheckLoan->IsUseReturnManageCode();
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("IsReturn"));
	}
	if( ids == 1 ) 
	{
		CString sLoanInfo;
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("현황구분"), sLoanInfo , m_nCurrentPos );
		if(sLoanInfo.IsEmpty() || _T("로컬") == sLoanInfo)
		{
			ids = m_pSpfWork->m_pCheckLoan->IsReturnManageCode(sBookManageCode);
			if(0 > ids)
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("IsReturn"));
			}
			if( ids == 0 ) 
			{
					if(_T("Y") == m_pSpfWork->m_pManageValue->m_sBookCooperativeSys && (_T("L") == sLoanStatus || _T("D") == sLoanStatus) )
					{
						sMsg.Format( _T("등록번호가 '%s'인 이 책은 책두레대상자료입니다.\r\n책두레 반납메뉴를 통해 반납해 주십시오."), sRegNo );
						m_pSpfWork->ViewMessage( sMsg );
						return 1;
					}
					else if(_T("Y") == m_pSpfWork->m_pManageValue->m_sIsUnityLoanService) {
						CString strLctKey = _T("");
						GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("타관대출KEY"), strLctKey , m_nCurrentPos );						

						if ( strLctKey.GetLength() == 0  ) {
							if ( m_pSpfWork->m_pManageValue->m_sBookCooperativeSys == _T("Y") && (_T("0") == sLoanStatus || _T("2") == sLoanStatus) && strNLoanToIllReturn == _T("Y") ) {

								BOOL bReturn = FALSE;
								ids = m_pSpfWork->BookReturnTypeCheck(sRegNo);
								switch(ids)
				{ 
								case _MESSAGE_NO_USE_KL_LILL:
									sMsg.Format(	_T("책이음상호대차 자료입니다.\r\n")
													_T("대출 도서관에 반납하시기 바랍니다."));
									m_pSpfWork->ViewMessage( sMsg );
									bReturn = TRUE;
									break;
								case _MESSAGE_NO_USE_ILL:
									sMsg.Format(	_T("책두레 자료입니다.\r\n")
													_T("대출 도서관에 반납하시기 바랍니다."));
									m_pSpfWork->ViewMessage( sMsg );
									bReturn = TRUE;
									break;
								case _MESSAGE_NO_USE_KL:
									sMsg.Format(	_T("책이음 자료입니다.\r\n")
													_T("대출 도서관에 반납하시기 바랍니다."));
									m_pSpfWork->ViewMessage( sMsg );
									bReturn = TRUE;
									break;
								case _RETURN_FAIL:
									bReturn = TRUE;
									break;
								default :
									break;
								}

								if(bReturn == TRUE){
									return 1;
								}
								m_bIsILL = TRUE;
								m_bIsForceILL = TRUE;
							}
						}
					}
					else {
						if ( m_pSpfWork->m_pManageValue->m_sBookCooperativeSys == _T("Y") && (_T("0") == sLoanStatus || _T("2") == sLoanStatus) && strNLoanToIllReturn == _T("Y") ) {
							
							BOOL bReturn = FALSE;
							ids = m_pSpfWork->BookReturnTypeCheck(sRegNo);
							switch(ids)
							{
							case _MESSAGE_NO_USE_KL_LILL:
								sMsg.Format(	_T("책이음상호대차 자료입니다.\r\n")
												_T("대출 도서관에 반납하시기 바랍니다."));
								m_pSpfWork->ViewMessage( sMsg );
								bReturn = TRUE;
								break;
							case _MESSAGE_NO_USE_ILL:
								sMsg.Format(	_T("책두레 자료입니다.\r\n")
												_T("대출 도서관에 반납하시기 바랍니다."));
								m_pSpfWork->ViewMessage( sMsg );
								bReturn = TRUE;
								break;
							case _MESSAGE_NO_USE_KL:
								sMsg.Format(	_T("책이음 자료입니다.\r\n")
												_T("대출 도서관에 반납하시기 바랍니다."));
								m_pSpfWork->ViewMessage( sMsg );
								bReturn = TRUE;
								break;
							case _RETURN_FAIL:
								bReturn = TRUE;
								break;
							default :
								break;
							}

							if(bReturn == TRUE){
								return 1;
							}
							m_bIsILL = TRUE;
							m_bIsForceILL = TRUE;
						}
						else {
							m_pSpfWork->ViewMessage( _T("관리구분값이 다르므로 반납을 할 수 없습니다. ") );	
							return 1;
						}
				}
								
				}					
			}
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3130::GetLoanStopDate( CString &sLoanStopDate, CString &delayDays )
{
EFS_BEGIN

	INT ids;

	m_GetLoanStopDateCnt++;

	CString sPreLoanStopDate;
	CTime tPreLoanStopDate( 1971, 1, 1, 0, 0, 0 );
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출정지일"), sPreLoanStopDate, 0 );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetLoanStopDate"));
	}
	
	if( !sPreLoanStopDate.IsEmpty() )
	{
		CTime tTempDate( _ttoi(sPreLoanStopDate.Mid(0,4)), _ttoi(sPreLoanStopDate.Mid(5,2)), _ttoi(sPreLoanStopDate.Mid(8,2)), 0 , 0 , 0 );
		tPreLoanStopDate = tTempDate;
	}

	CString sExpLoanDate; 
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("반납예정일"), sExpLoanDate, m_nCurrentPos );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("GetLoanStopDate"));
	}
	if( sExpLoanDate.IsEmpty() ) 
	{
		return 0;
	}
	
	CString sReturnType;
	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("GetLoanStopDate"));
	}

	CString sReturnDateControlAlias = _T("반납일");
	if( sReturnType.Compare(_T("2"))== 0 )
	{
		sReturnDateControlAlias = _T("소급반납일");
	}
	
	CString sReturnDate;
	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("GetLoanStopDate"));
	}
	if( sReturnDate.IsEmpty() ) 
	{
		return 0;
	}


	CTime tExpLoanDate( _ttoi(sExpLoanDate.Mid(0,4)), _ttoi(sExpLoanDate.Mid(5,2)), _ttoi(sExpLoanDate.Mid(8,2)), 0 , 0 , 0 );
	CTime tReturnDate( _ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0 , 0 , 0 );
	CTimeSpan tDelayDay = tReturnDate - tExpLoanDate;
	delayDays.Format( _T("%d"), tDelayDay.GetDays() );

	
	INT nDelayDay = tDelayDay.GetDays();
	CString strValue = _T("");
	m_pSpfWork->m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체기간휴관일포함유무"), strValue);	
	if(!strValue.IsEmpty() && strValue != _T("Y"))
	{	
		CString strQuery = _T("");
		CString strHolidayCount = _T("");
		INT nHolidayCount = 0;

		CESL_DataMgr pTempDM;
		pTempDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );		

		strQuery.Format(_T("SELECT GET_HOLIDAYCOUNT(TO_DATE('%s','YYYY/MM/DD'), TO_DATE('%s','YYYY/MM/DD'), '%s') FROM DUAL"), sExpLoanDate , sReturnDate, m_pInfo->MANAGE_CODE);
		pTempDM.GetOneValueQuery(strQuery, strHolidayCount );
		nHolidayCount = _ttoi( strHolidayCount );

		if(nHolidayCount < nDelayDay)
		{
			nDelayDay -= nHolidayCount;
			delayDays.Format( _T("%d"), nDelayDay );
		}
	}
	if( nDelayDay < 1 )
	{
		sLoanStopDate.Empty();
		return 0;
	}

	if( nDelayDay > 0 )
	{
		INT nDelayAddDay;
		ids = m_pCheckLoan->GetDelayAddDay( nDelayAddDay );
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("GetLoanStopDate"));
		}
		INT nDelayDayCount = m_pCheckLoan->GetDelayDayCnt();
		INT nLoanStopDayCount = m_pCheckLoan->GetLoanStopDayCnt();
		nDelayDay = ( nDelayDay + nLoanStopDayCount ) * nDelayAddDay + nDelayDayCount;
	}
	if( nDelayDay < 1 )
	{
		sLoanStopDate.Empty();
		return 0;
	}
	tDelayDay = CTimeSpan( nDelayDay, 0, 0, 0 );
	if( tDelayDay.GetDays() < 1 ) 
	{
		sLoanStopDate.Empty();
		return 0;
	}
	
	ids = m_pCheckLoan->IsSetLoanStopDate();
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("GetLoanStopDate"));
	}
	if( ids > 0 )
	{
		sLoanStopDate.Empty();
		return 100;
	}

	CTimeSpan tOneDay( 1, 0 , 0 , 0 );

	INT nDelayMode;
	ids = m_pCheckLoan->GetLoanStopDateOption( nDelayMode );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1060, _T("GetLoanStopDate"));
	}
	
	if( nDelayMode == 1 )
	{
		if( tPreLoanStopDate < tReturnDate )
		{
			if( _T("Y") == m_pCheckLoan->m_pManageValue->m_sIsAddHolyDayLoanStop )
			{
				nDelayDay--;
				m_pCheckLoan->GetAddTotalHolyDay( nDelayDay, tReturnDate );
				tDelayDay = CTimeSpan( nDelayDay, 0, 0, 0 );
			}
			else
			{
				tDelayDay = tDelayDay - tOneDay;				
			}
			CTime tExpLoanStopDate = tReturnDate + tDelayDay;
			sLoanStopDate = tExpLoanStopDate.Format( _T("%Y/%m/%d") );
		}
		else
		{
			if( _T("Y") == m_pCheckLoan->m_pManageValue->m_sIsAddHolyDayLoanStop )
			{
				m_pCheckLoan->GetAddTotalHolyDay( nDelayDay, tPreLoanStopDate );
				tDelayDay = CTimeSpan( nDelayDay, 0, 0, 0 );
			}
			CTime tExpLoanStopDate = tPreLoanStopDate + tDelayDay;			
			sLoanStopDate = tExpLoanStopDate.Format( _T("%Y/%m/%d") );
		}
	}
	else
	{
		tDelayDay = tDelayDay - tOneDay;

		if( _T("Y") == m_pCheckLoan->m_pManageValue->m_sIsAddHolyDayLoanStop )
		{
			nDelayDay--;
			m_pCheckLoan->GetAddTotalHolyDay( nDelayDay, tReturnDate );
			tDelayDay = CTimeSpan( nDelayDay, 0, 0, 0 );			
		}
		CTime tExpLoanStopDate = tReturnDate + tDelayDay;

		if( tPreLoanStopDate < tExpLoanStopDate )
		{
			sLoanStopDate = tExpLoanStopDate.Format(_T("%Y/%m/%d"));
		}
		else
		{
			sLoanStopDate = tPreLoanStopDate.Format(_T("%Y/%m/%d"));
		}
	}

	INT nMaxDay = _ttoi(m_pSpfWork->m_pManageValue->m_strMaxLoanStopDay);
	if( nMaxDay > 0 )
	{ 
		CTimeSpan tMaxDay = CTimeSpan(nMaxDay-1, 0, 0, 0);
		CTime tMaxLoanStopDate = tReturnDate + tMaxDay;
		CTime tLoanStopDate( _ttoi(sLoanStopDate.Mid(0,4)), _ttoi(sLoanStopDate.Mid(5,2)), _ttoi(sLoanStopDate.Mid(8,2)), 0, 0, 0 );
		if(tLoanStopDate > tMaxLoanStopDate)
		{
			sLoanStopDate = tMaxLoanStopDate.Format( _T("%Y/%m/%d") );
		}
		else
		{
			sLoanStopDate = tLoanStopDate.Format( _T("%Y/%m/%d") );
		}
	}
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	CESL_Control_Element *pCE = (CESL_Control_Element*)pCM->FindControlElement(_T("대출정지수동"));
	CButton *pCheck = (CButton*)pParentMgr->GetDlgItem(pCE->CTRL_ID);
	if( pCheck->GetCheck() && !sLoanStopDate.IsEmpty() )
	{
		if( m_GetLoanStopDateCnt % 2 )
		{
			CBO_LOC_3133 dlg( sLoanStopDate, this );
			if( IDOK == dlg.DoModal() )
			{
				sLoanStopDate = dlg.m_sLoanStopDate;
			}
			else
			{
				m_GetLoanStopDateCnt = 0;
				return CANCEL_RETURN_BOOK;
			}
			
			m_sLoanStopDate = sLoanStopDate;
		}		
		else
		{
			sLoanStopDate = m_sLoanStopDate;
			m_sLoanStopDate.Empty();
		}
	}

	return 0;

EFS_END
return -1;
}


INT CBO_LOC_3130::SetDelayMoney()
{
EFS_BEGIN

	INT ids, i, nPos;
	CString sFeeMngSys = m_pSpfWork->m_pManageValue->m_sFeeMngSys;
	if( sFeeMngSys != _T("Y") || TRUE == m_bIsILL)
	{
		ids = m_pCheckLoan->IsSetDelayMoney();
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SetDelayMoney"));
		}
		if( ids > 0 )
		{
			GetDlgItem(IDC_edtRETURN4)->EnableWindow(FALSE);
			return 0;
		}
	}
	
	CString sReturnType;
	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType );
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("SetDelayMoney"));
	}

	CString sReturnDateControlAlias = _T("반납일");
	if( sReturnType.Compare(_T("2"))== 0 )
	{
		sReturnDateControlAlias = _T("소급반납일");
	}

	CString sReturnDate;
	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate );
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("SetDelayMoney"));
	}

	if( sReturnDate.IsEmpty() ) 
	{
		return 0;
	}

	CTime tReturnDate( _ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0 , 0 , 0 );

	CString strQuery = _T("");
	CString strHolidayCount = _T("");
	INT nHolidayCount = 0;
	INT nTotalHolidayCount = 0;
	CESL_DataMgr pTempDM;
	pTempDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );	

	CString strValue = _T("");
	m_pSpfWork->m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체기간휴관일포함유무"), strValue);	

	CString sExpReturnDate;
	CTimeSpan tDelayDay(0, 0, 0, 0);
	if( m_nReturnType == 0 && sFeeMngSys != _T("Y") )
	{
		ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("반납예정일"), sExpReturnDate , m_nCurrentPos );
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("SetDelayMoney"));
		}

		if( sExpReturnDate.IsEmpty() ) 
		{
			return 0;
		}

		CTime tExpReturnDate( _ttoi(sExpReturnDate.Mid(0,4)), _ttoi(sExpReturnDate.Mid(5,2)), _ttoi(sExpReturnDate.Mid(8,2)), 0 , 0 , 0 );
		if( tReturnDate > tExpReturnDate )
		{

			if ( _T("Y") != m_pCheckLoan->m_pManageValue->m_sIsAppendixLoanStop )
			{
				CString sAppendixFlag;
				ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록여부"), sAppendixFlag, m_nCurrentPos );
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("SetDelayMoney"));
				}
				if ( sAppendixFlag != _T("A") ) 
				{
					tDelayDay += tReturnDate - tExpReturnDate;
				}
			}
			else{
				tDelayDay = tReturnDate - tExpReturnDate;
			}			
			if(!strValue.IsEmpty() && strValue != _T("Y"))
			{
				strQuery.Format(_T("SELECT GET_HOLIDAYCOUNT(TO_DATE('%s','YYYY/MM/DD'), TO_DATE('%s','YYYY/MM/DD'), '%s') FROM DUAL"), sExpReturnDate , sReturnDate, m_pInfo->MANAGE_CODE);				
				pTempDM.GetOneValueQuery(strQuery, strHolidayCount );
				nHolidayCount = _ttoi( strHolidayCount );
				nTotalHolidayCount += nHolidayCount;
			}
		}
	}
	else
	{
		CESL_Grid *pGrid;
		ids = FindGridControl( &pGrid );

		pGrid->SelectGetHeadPosition();
		INT nPos;
		for( INT i=0 ; i<m_nSelectGetCount ; i++ )
		{
			nPos = pGrid->SelectGetIdx();

			ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO") , _T("반납예정일"), sExpReturnDate , nPos );
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("SetDelayMoney"));
			}

			if( sExpReturnDate.IsEmpty() )
			{
				pGrid->SelectGetNext();
				continue;
			}

			CTime tExpReturnDate( _ttoi(sExpReturnDate.Mid(0,4)), _ttoi(sExpReturnDate.Mid(5,2)), _ttoi(sExpReturnDate.Mid(8,2)), 0 , 0 , 0 );
			
			if(_T("N") == m_pSpfWork->m_pManageValue->m_ArrearCheckYN && sFeeMngSys == _T("Y") && FALSE == m_bIsILL)	
			{
				if( tReturnDate > tExpReturnDate )
				{
					if ( _T("Y") != m_pCheckLoan->m_pManageValue->m_sIsAppendixLoanStop )
					{
						CString sAppendixFlag;
						ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록여부"), sAppendixFlag, nPos );
						if(0 > ids)
						{
							ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("SetDelayMoney"));
						}
						if ( sAppendixFlag != _T("A") ) 
						{
							tDelayDay += tReturnDate - tExpReturnDate;
						}
					}
					else
					{
						tDelayDay += tReturnDate - tExpReturnDate;
					}
				}
				
				pGrid->SelectGetNext();
			}
			else
			{
				if( tReturnDate > tExpReturnDate )
				{
					tDelayDay += tReturnDate - tExpReturnDate;
				}
				
				pGrid->SelectGetNext();
			}
			if(!strValue.IsEmpty() && strValue != _T("Y"))
			{
				strQuery.Format(_T("SELECT GET_HOLIDAYCOUNT(TO_DATE('%s','YYYY/MM/DD'), TO_DATE('%s','YYYY/MM/DD'), '%s') FROM DUAL"), sExpReturnDate , sReturnDate, m_pInfo->MANAGE_CODE);				
				pTempDM.GetOneValueQuery(strQuery, strHolidayCount );
				nHolidayCount = _ttoi( strHolidayCount );
				nTotalHolidayCount += nHolidayCount;
			}
		}
		
		pGrid->SelectGetHeadPosition();
		for( i=0 ; i<m_nSelectGetCount ; i++ )
		{
			nPos = pGrid->SelectGetIdx();
			if( nPos == m_nCurrentPos ) break;
			pGrid->SelectGetNext();
		}
	}

	long nDelayDay = tDelayDay.GetDays();

	if(nTotalHolidayCount < nDelayDay)
	{
		nDelayDay -= nTotalHolidayCount;
	}

	long nDelayMoney;
	if(sFeeMngSys == _T("Y") && FALSE == m_bIsILL)
	{
		m_sAllArrearDay.Format( _T("%d"), tDelayDay.GetDays() );

		if(nTotalHolidayCount < tDelayDay.GetDays())
		{
			m_sAllArrearDay.Format( _T("%d"), tDelayDay.GetDays() - nTotalHolidayCount);
		}

		m_sAllApplyArrearDay.Format(_T("%d"), nDelayDay);
		nDelayMoney = nDelayDay * _ttoi(m_sBasicArrear);
		
		CString sDelayMoney;
		sDelayMoney.Format( _T("%i"), nDelayMoney );

		m_sAllArrear = sDelayMoney;
	}
	else
	{
		long nDelayMoneyPerDay;
		ids = m_pCheckLoan->GetDelayMoney( nDelayMoneyPerDay );
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1060, _T("SetDelayMoney"));
		}

		INT nDelayAddDay;
		ids = m_pCheckLoan->GetDelayAddDay( nDelayAddDay );
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1070, _T("SetDelayMoney"));
		}

		nDelayMoney = nDelayDay*nDelayMoneyPerDay*nDelayAddDay;	
	}

	CString sDelayMoney;
	sDelayMoney.Format( _T("%i"), nDelayMoney );
	ids = SetControlData( _T("CM_BO_LOC_3130"), _T("연체료"), sDelayMoney );
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1080, _T("SetDelayMoney"));
	}

	if(_T("Y") == m_pSpfWork->m_pManageValue->m_ArrearCheckYN)
	{
		if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
		{
			GetDlgItem(IDC_edtRETURN4)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_edtRETURN4)->EnableWindow(TRUE);
		}
	}

	return 0;

EFS_END
return -1;
}


VOID CBO_LOC_3130::OnKillfocusedtRETURN7() 
{
	EFS_BEGIN

	SetDelayMoney();	

	EFS_END
}

VOID CBO_LOC_3130::OnKillfocusedtMONEYPAID2() 
{
	EFS_BEGIN
		
	InsertPaymentArrear();

	EFS_END
}

INT CBO_LOC_3130::RecordReservationExpireDate()
{
	EFS_BEGIN

	if(TRUE == m_bIsILL)
	{
		int ids = 0;
		CString strFuncName = _T("CBO_LOC_3130::RecordReservationExpireDate()");
		CString strLoanKey;

		ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출/반납정보KEY"), strLoanKey , m_nCurrentPos );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

		CString strQuery = _T("");
		CString strGiveLibCode;
		strQuery.Format(
			_T("SELECT GIVE_LIBCODE FROM ILL_TRANS_TBL WHERE LOAN_KEY = '%s'"),
			strLoanKey);
		ids = TmpDM.GetOneValueQuery(strQuery, strGiveLibCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
		if(strGiveLibCode != m_strLibCode) return 0;		
	}
	m_pSpfWork->SPFExcutePath(_T("반납된자료예약만기일부여"));

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::ViewReturnAppendixInfo()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM;
	pDM = MakeAppendixDM();
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::ViewReturnAppendixInfo()") );

	if( pDM->GetRowCount() < 1 ) return 0;

 	CReturnAppendixListDlg AppendixDlg(this);
 	AppendixDlg.DoModal();
	

 	INT m_nCloseMode = AppendixDlg.GetCloseMode();

 	if( m_nCloseMode == 1 ) return 1;

	ids = ReturnAppendixDBProc();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::ViewReturnAppendixInfo()") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::ReturnAppendixDBProc()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_RETURN_APPENDIX_INFO");
	CESL_DataMgr *pUpdateDM = FindDM( sDMAlias );
	if( pUpdateDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3130::ReturnAppendixDBProc()") );
	
	for( INT i = 0 ; i < pUpdateDM->GetRowCount() ; i++ )
	{
		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , pUpdateDM );

		pUpdateDM->StartFrame();

		ids =  AppedixBookInfoUpdateDBProc( pUpdateDM , i , pMobileApi );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::ReturnAppendixDBProc()") );

		ids =  AppedixLoanInfoUpdateDBProc( pUpdateDM , i , pMobileApi );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CBO_LOC_3130::ReturnAppendixDBProc()") );

		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );

		pUpdateDM->EndFrame();

		CLocCommonAPI::MobileApiEndLog( pMobileApi );

	}	

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::AppedixBookInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex , CKolas2up_MobileApi *pMobileApi)
{
	EFS_BEGIN

	INT ids;
	pUpdateDM->InitDBFieldData();

	CString sPublishForm;
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("단행/연속구분"), sPublishForm , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("AppedixBookInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );

	CString sTableName;
	if( sPublishForm.Compare(_T("MO"))==0 )
		sTableName = _T("BO_BOOK_TBL");
	else
		sTableName = _T("SE_BOOK_TBL");

	CString sBookState;
	ids = pUpdateDM->GetCellData( _T("부록상태"), nIndex , sBookState );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("AppedixBookInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );

	if( sBookState.Compare(_T("0") )== 0 )
	{
		if( sPublishForm.Compare(_T("MO"))==0 )
			sBookState = _T("BOL112N");
		else
			sBookState = _T("SEL232N");
	}
	else if( sBookState.Compare(_T("1")) == 0 )
	{
		if( sPublishForm.Compare(_T("MO"))==0 )
			sBookState = _T("BOL254N");
		else
			sBookState = _T("SEL232N");
	}
	else
	{
		if( sPublishForm.Compare(_T("MO"))==0 )
			sBookState = _T("BOL252N");
		else
			sBookState = _T("SEL252N");
	}	


	ids = pUpdateDM->AddDBFieldData( _T("WORKING_STATUS"), _T("STRING"), sBookState , TRUE );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("AppedixBookInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );

	CString sRecKey;
	ids = pUpdateDM->GetCellData( _T("부록KEY"), nIndex , sRecKey );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("AppedixBookInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );

	pUpdateDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	ids = pMobileApi->MakeUpdateFrame( sTableName , _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("CBO_LOC_3130::LoanBookInfoDBUpdate()") );
	
	pUpdateDM->InitDBFieldData();

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::AppedixLoanInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex , CKolas2up_MobileApi *pMobileApi)
{
	EFS_BEGIN

	INT ids;

	pUpdateDM->InitDBFieldData();
	
	CString sLoanState = _T("1");
	CString sReturnType = _T("0");
	CString sRepairMoney;
	CString sArrearMoney;
	CString sReturnDate = _T("SYSDATE");

	ids = pUpdateDM->GetCellData(_T("배상비"), nIndex ,  sRepairMoney );
	if(0 > ids&& (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("AppedixLoanInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );

	ids = pUpdateDM->GetCellData( _T("연체료"), nIndex , sArrearMoney );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("AppedixLoanInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );
	
	ids = pUpdateDM->AddDBFieldData( _T("STATUS"), _T("STRING"), sLoanState , TRUE );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("AppedixLoanInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );

	if( !sRepairMoney.IsEmpty() )
	{
		ids = pUpdateDM->AddDBFieldData( _T("REPARATION"), _T("NUMERIC"), sRepairMoney , TRUE );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("AppedixLoanInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );
	}

	if( !sArrearMoney.IsEmpty() )
	{
		ids = pUpdateDM->AddDBFieldData( _T("ARREAR"), _T("NUMERIC"), sArrearMoney , TRUE );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("AppedixLoanInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );
	}
	
	CString sRecKey;
	ids = pUpdateDM->GetCellData(  _T("대출/반납정보KEY"), nIndex , sRecKey  );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -9 , _T("AppedixLoanInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );
	
	pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	ids = pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -10 , _T("AppedixLoanInfoUpdateDBProc( CESL_DataMgr *pUpdateDM , INT nIndex )") );

	pUpdateDM->InitDBFieldData();

	return 0;

	EFS_END
	return -1;

}

CESL_DataMgr* CBO_LOC_3130::MakeAppendixDM()
{
	EFS_BEGIN

	INT ids;

	CString sBookLoanDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sAppedixLoanDMAlias = _T("DM_BO_LOC_3100_RETURN_APPENDIX_INFO");
	CString sAppedixLoanSEMAlias = _T("DM_BO_LOC_3100_RETURN_APPENDIX_INFO_SE");

	CESL_DataMgr *pDM = FindDM( sAppedixLoanDMAlias );
	if( pDM == NULL ) return NULL;

	CESL_DataMgr *pSEDM = FindDM( sAppedixLoanSEMAlias );
	if( pDM == NULL ) return NULL;
	
	CESL_DataMgr *pLoanBookDM = FindDM( sBookLoanDMAlias );
	if( pLoanBookDM == NULL ) return NULL;

	CString sLoanKey;
	ids = GetDataMgrData( sBookLoanDMAlias , _T("대출/반납정보KEY"), sLoanKey , m_nCurrentPos );
	if(0 > ids) return NULL;

	CString sPublishFormCode;
	ids = GetDataMgrData( sBookLoanDMAlias , _T("단행/연속구분"), sPublishFormCode , m_nCurrentPos );
	if(0 > ids) return NULL;

	CString sWhere;
	sWhere.Format( _T("CL.LOAN_KEY = %s"), sLoanKey );
	pDM->FreeData();
	if( sPublishFormCode.Compare(_T("MO")) == 0 )
		ids = CLocCommonAPI::RefreshDataManager( this , pDM , sWhere );
	else
	{
		ids = CLocCommonAPI::RefreshDataManager( this , pSEDM , sWhere );
		if(0 > ids) return NULL;
		ids = CLocCommonAPI::AddDM( pSEDM , pDM , this );
		if(0 > ids) return NULL;
	}
	if(0 > ids) return NULL;

	return pDM;	

	EFS_END
	return NULL;

}

INT CBO_LOC_3130::SetRepairMeney()
{
	EFS_BEGIN


	INT ids;

	CString sParentDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias = _T("CM_BO_LOC_3130");

	long nRepairMoney = 0;
	CString sBookPrice;
	
	if( m_nReturnType == 0 )
	{
		ids = GetDataMgrData( sParentDMAlias , _T("책값"), sBookPrice , m_nCurrentPos );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CBO_LOC_3130::SetRepairMeney()") );

		if( sBookPrice.IsEmpty() ) return 0;

		nRepairMoney += _ttoi( sBookPrice );
	}
	else
	{
		CESL_Grid *pGrid;
		ids = FindGridControl( &pGrid );

		pGrid->SelectGetHeadPosition();
		INT nPos;
		for( INT i=0 ; i<m_nSelectGetCount ; i++ )
		{
			nPos = pGrid->SelectGetIdx();

			ids = GetDataMgrData( sParentDMAlias , _T("책값"), sBookPrice , nPos );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CBO_LOC_3130::SetRepairMeney()") );

			if( sBookPrice.IsEmpty() ){
				pGrid->SelectGetNext();
				continue;
			}
			
			nRepairMoney += _ttoi( sBookPrice );

			pGrid->SelectGetNext();
		}
		
		pGrid->SelectGetHeadPosition();
		for( i=0 ; i<m_nSelectGetCount ; i++ )
		{
			nPos = pGrid->SelectGetIdx();
			if( nPos == m_nCurrentPos ) break;
			pGrid->SelectGetNext();
		}
	}
	
	CString sRepairMoney;
	sRepairMoney.Format( _T("%i"), nRepairMoney );
	ids = SetControlData( sCMAlias , _T("배상비"), sRepairMoney );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("CBO_LOC_3130::SetRepairMeney()") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::DeleteReturnedBook()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeleteReturnedBook") );

	INT nRowCount = pDM->GetRowCount();

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_Grid* pGrid = (CESL_Grid*)(FindControl( sCMAlias , sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeleteReturnedBook") );

	CString sWorkingStatus;
	for( INT i = nRowCount-1 ; i > -1 ; i-- )
	{
		ids = pDM->GetCellData( _T("대출상태"), i , sWorkingStatus );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeleteReturnedBook") );

		if( (sWorkingStatus.Compare( _T("1") ) == 0) || ( sWorkingStatus.Compare( _T("반납") ) == 0) )
		{
			CESL_DataMgr* pReturnBasket = FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));	
			if( pReturnBasket == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DeleteReturnedBook") );

			INT nInsertIdx = pReturnBasket->GetRowCount();
			pReturnBasket->InsertRow(-1);

			INT nColCnt = pReturnBasket->GetRefColumnCount();
			for( INT j=0; j<nColCnt; j++ )
			{
				CString strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type;
				CString strResult;

				ids = pReturnBasket->GetColumnRefInfo(j, strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DeleteReturnedBook") );

				ids = pDM->GetCellData(strField_Alias, i, strResult);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DeleteReturnedBook") );

				ids = pReturnBasket->SetCellData(strField_Alias, strResult, nInsertIdx);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("DeleteReturnedBook") );
			}

			if( m_pInfo->MODE == 10000 )
			{
				ids = pReturnBasket->SetCellData(_T("R_DEVICE"), _T("KOLASIII-이동도서관"), nInsertIdx);
			}
			else
			{
				ids = pReturnBasket->SetCellData(_T("R_DEVICE"), _T("KOLASIII"), nInsertIdx);
			}
			if(0 > ids)
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -106 , _T("DeleteReturnedBook") );
			}

			ids = pReturnBasket->SetCellData(_T("R_WORKER"), m_pInfo->USER_ID, nInsertIdx);
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -106 , _T("DeleteReturnedBook") );
			}

			pDM->DeleteRow(i);
			if( pDM->GetRowCount() != 0 )
			{
				((CMSHFlexGrid*)pGrid)->RemoveItem(i+1);

				CString sIndex;
				for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
				{
					sIndex.Format(_T("%d"),i+1);
					pGrid->SetTextMatrix( i+1 , 0 , sIndex);
				}
			}
		}
	}

	if( pDM->GetRowCount() == 0 )
	{
		AllControlDisplay( _T("CM_BO_LOC_3100_LOAN_INFO") );
	}
	else
	{
		ControlDisplay(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid2"));
	}

	CESL_Grid* pGrid2 = (CESL_Grid*)(FindControl( sCMAlias , _T("MainGrid2") ));
	pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0 );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::GetTotalDelayCnt( INT &nTotalDelayCnt )
{
	EFS_BEGIN

	INT ids;

	CString sUserRecKey;
	CString sCnt;
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetTotalDelayCnt") );

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sUserRecKey , 0 );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetTotalDelayCnt") ); 

	CString sSql;
	sSql.Format( _T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s AND TO_DATE(LOAN_STOP_SET_DATE,'YYYY/MM/DD')=TO_DATE(SYSDATE,'YYYY/MM/DD')"), sUserRecKey);
	ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sCnt );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetTotalDelayCnt") ); 

	CString sValue;
	m_pCheckLoan->GetMethodDelayCntPlus( sValue );

	if(_T("N") == m_pSpfWork->m_pManageValue->m_ArrearCheckYN && m_pSpfWork->m_pManageValue->m_sFeeMngSys == _T("Y") && FALSE == m_bIsILL)
	{
		sValue = _T("1");
	}
	if(_T("1") == sValue) 
	{
		sCnt = _T("0");
	}

 	if( _ttoi(sCnt) > 0 )
	{
 		nTotalDelayCnt = 0;
 	}
 	else
 	{
 		CString sSql;
 		sSql.Format( _T("SELECT DELAY_COUNT FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s ") 
 			, sUserRecKey );
 
 		ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sCnt );
 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetTotalDelayCnt") ); 
 
 		nTotalDelayCnt = _ttoi( sCnt ) + 1;
 	}

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3130::LoadReturnShelfLocCodeDisplay(CString &f_strShelfLocCode)
{	
	CString	strShelfLocCode;
	
	CStdioFile fFile;
	if( fFile.Open( _T("..\\cfg\\반납자료실설정.cfg"), CFile::modeRead | CFile::typeBinary ) )
	{
		TCHAR cUni;
		fFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fFile.SeekToBegin();
		}
		CString sIsCheck;
		fFile.ReadString(strShelfLocCode);
		fFile.Close();
	}

	strShelfLocCode.TrimLeft(_T(" "));
	strShelfLocCode.TrimRight(_T(" "));

	f_strShelfLocCode = strShelfLocCode;

	return 0;
}

BOOL CBO_LOC_3130::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			HWND MsgH = pMsg->hwnd;
			
			if(	GetDlgItem(IDC_edtMONEYPAID2)->m_hWnd==MsgH )
			{
				((CEdit*)GetDlgItem( IDC_btnRETURNSAVE ))->SetFocus();
			}
			else if( GetDlgItem(IDC_btnRETURNSAVE)->m_hWnd==MsgH || GetDlgItem(IDC_edtRETURN4)->m_hWnd==MsgH)
			{
				OnbtnRETURNSAVE();
			}
			return TRUE;
		}
		else if ( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

INT CBO_LOC_3130::InsertBBankRecord(CKolas2up_MobileApi *pMobileApi, CString sReturnKey)
{
	INT ids;
	ids = m_pSpfWork->InsertBBankRecord( pMobileApi, sReturnKey, 1 );
	if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2 ( -1, _T("InsertBBankRecord()") );
	return 0;
}

INT CBO_LOC_3130::IsReturnAppendix()
{
EFS_BEGIN
	
	return m_pCheckLoan->IsAppendixReturn();

EFS_END
	return -1;
}

INT CBO_LOC_3130::AppendixReturn()
{
EFS_BEGIN

	INT		ids;
	CString strBookAppendixFlag;
	INT		nAppendixCount;
	BOOL    bAppendixAble = FALSE;

	if( IsReturnAppendix() ) return 0;
	
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3130::AppendixReturn()") );

	ids = pDM->GetCellData( _T("부록여부"), m_nCurrentPos , strBookAppendixFlag );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3130::AppendixReturn()") );
	if( 0 == strBookAppendixFlag.Compare(_T("A")) ) return 0;

	CESL_DataMgr* pAppendixDM = m_pSpfWork->MakeAppendixDM( m_nCurrentPos );
	if( !pAppendixDM ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3130::AppendixReturn()") );
	
	if( 0 == (nAppendixCount = pAppendixDM->GetRowCount()) ) return 0;

	CString strAppendixDMKey;
	CString strLoanDMKey;
	CString strStatus;
	CStringArray arrLoanDMIdx;
	
	for( INT i=0;i<nAppendixCount;i++ )
	{
		ids = pAppendixDM->GetCellData( _T("부록KEY"), i, strAppendixDMKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3130::AppendixReturn()") );

		for( INT j=0;j<pDM->GetRowCount();j++ )
		{
			ids = pDM->GetCellData( _T("책KEY"), j, strLoanDMKey );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3130::AppendixReturn()") );

			if( strAppendixDMKey == strLoanDMKey )
			{
				ids = pDM->GetCellData( _T("대출상태"), j, strStatus );
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3130::AppendixReturn()") );

				if( (strStatus == _T("1")) || (strStatus == _T("3")) || (strStatus == _T("4")) ) continue;

				bAppendixAble = true;

				CString strIndex;
				strIndex.Format( _T("%d"), j );
				arrLoanDMIdx.Add( strIndex );				
			}
		}
	}

	if( bAppendixAble )
	{
		CString strMessage;
		CString strRegNoBuf;
		CString strRegNo;
		CString strTitle, strTemp;
		for( i=0;i<arrLoanDMIdx.GetSize();i++ )
		{
			ids = pDM->GetCellData( _T("등록번호"), _ttoi(arrLoanDMIdx.GetAt(i)), strRegNo );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_3130::AppendixReturn()") );

			ids = pDM->GetCellData( _T("본표제"), _ttoi(arrLoanDMIdx.GetAt(i)), strTemp );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_3130::AppendixReturn()") );
			strTitle = m_pSpfWork->getLimitWord( strTemp.GetBuffer(0), 18, FALSE );
			strRegNoBuf += _T("[") + strRegNo + _T(":") + strTitle + _T("]\r\n");
		}

		strMessage.Format( _T("부록자료와 함께 대출된 자료입니다.\r\n\r\n%s\r\n")
			               _T("부록자료를 확인 하셨습니까?\r\n")
						   _T(" ( 예 : 반납,  아니오 : 반납취소 )"), strRegNoBuf);

		if( IDYES == MessageBox(strMessage , _T("부록자료반납"), MB_ICONINFORMATION | MB_YESNO) )
		{
			INT		nCurLoanPos = m_nCurrentPos;

			for( i=0;i<arrLoanDMIdx.GetSize();i++ )
			{
				m_nCurrentPos = _ttoi( arrLoanDMIdx.GetAt(i) );
				ids = ReturnBook(TRUE);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3130::AppendixReturn()") );
			}

			m_nCurrentPos = nCurLoanPos;
		}
		else
		{
			return 1;
		}
	}

	return 0;
EFS_END
	return -1;
}


HBRUSH CBO_LOC_3130::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

INT CBO_LOC_3130::ArrearCopyDM()
{
EFS_BEGIN
	
	INT ids;
	
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ArrearCopyDM") );
		
	CESL_DataMgr *pCopyDM = FindDM( _T("DM_BO_LOC_3130_2") );
	if( pCopyDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ArrearCopyDM") );
	pCopyDM->FreeData();

	const INT nCopyCnt = 4;
		
	LPTSTR sField[nCopyCnt][2] =
	{
		{ _T("등록번호"), _T("등록번호")	},
		{ _T("본표제")	, _T("본표제")		},
		{ _T("연체일수"), _T("연체일수")	},
		{ _T("대출일")	, _T("대출일")		}
	};
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid") );

	INT nMode = 0;
	INT nSelectCnt = m_nSelectGetCount;

	CString sIsReturnAppendix;
	m_pSpfWork->m_pManageValue->GetManageValue( _T("열람"), _T("공통"), _T("부록관리"), _T("부록반납여부"), sIsReturnAppendix );
	if( sIsReturnAppendix == _T("Y") )
	{
		BOOL    bIsAppendixReturn = FALSE;
		CString strBookAppendixFlag;
		INT		nAppendixCount;
		
		nAppendixCount =pSrcDM->GetRowCount(); 
		for(INT i=0; i< pSrcDM->GetRowCount(); i++)
		{
			CString sSelect;
			ids = pSrcDM->GetCellData( _T("선정"), i , sSelect );
			if(sSelect == _T("V") )
			{
				ids = pSrcDM->GetCellData( _T("부록여부"), i, strBookAppendixFlag );
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3130::AppendixReturn()") );
				if( 0 == strBookAppendixFlag.Compare( _T("B") ) )
				{
					CESL_DataMgr* pAppendixDM = m_pSpfWork->MakeAppendixDM( i );
					if( !pAppendixDM ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3130::AppendixReturn()") );
					
					if( 0 != (nAppendixCount = pAppendixDM->GetRowCount()) )
					{
						CString strAppendixDMKey;
						CString strLoanDMKey;
						CString strStatus;
						CStringArray arrLoanDMIdx;

						for( INT j = 0; j < nAppendixCount; j++ )
						{
							ids = pAppendixDM->GetCellData( _T("부록KEY"), j, strAppendixDMKey );
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3130::AppendixReturn()") );
							
							for( INT k = 0; k < pSrcDM->GetRowCount(); k++ )
							{
								ids = pSrcDM->GetCellData( _T("책KEY"), k, strLoanDMKey );
								if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3130::AppendixReturn()") );
								
								if( strAppendixDMKey == strLoanDMKey )
								{
									ids = pSrcDM->GetCellData( _T("대출상태"), k, strStatus );
									if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3130::AppendixReturn()") );
									
									if( ( strStatus == _T("1") ) || ( strStatus == _T("3") ) || ( strStatus == _T("4") ) ) continue;
																		
									CString strIndex;
									strIndex.Format( _T("%d"), k );
									arrLoanDMIdx.Add( strIndex );				
								}
							}
						}
						CString strMessage;
						CString strRegNoBuf;
						CString strRegNo;
						CString strTitle, strTemp;
						for( INT l = 0; l < arrLoanDMIdx.GetSize(); l++ )
						{
							ids = pSrcDM->GetCellData( _T("등록번호"), _ttoi(arrLoanDMIdx.GetAt(l)), strRegNo );
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_3130::AppendixReturn()") );
							
							ids = pSrcDM->GetCellData( _T("본표제"), _ttoi(arrLoanDMIdx.GetAt(l)), strTemp );
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_3130::AppendixReturn()") );
							strTitle = m_pSpfWork->getLimitWord( strTemp.GetBuffer(0), 18, FALSE );
							strRegNoBuf += _T("[") + strRegNo + _T(":") + strTitle + _T("]\r\n");
						}

						if( strRegNo.IsEmpty() )	continue;

						strMessage.Format(  _T("부록자료와 함께 대출된 자료입니다.\r\n\r\n%s\r\n")
											_T("부록자료를 확인 하셨습니까?\r\n")
											_T(" ( 예 : 반납,  아니오 : 반납취소 )"), strRegNoBuf);
						
						if( IDYES != MessageBox(strMessage , _T("부록자료반납"), MB_ICONINFORMATION | MB_YESNO) )
						{
							return -1;
						}
						for( INT m = 0; m < nAppendixCount; m++ )
						{
							ids = pAppendixDM->GetCellData( _T("부록KEY"), m, strAppendixDMKey );
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3130::AppendixReturn()") );
							
							for( INT n = 0; n < pSrcDM->GetRowCount(); n++ )
							{
								ids = pSrcDM->GetCellData( _T("책KEY"), n, strLoanDMKey );
								if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3130::AppendixReturn()") );
								
								if( strAppendixDMKey == strLoanDMKey )
								{
									ids = pSrcDM->GetCellData( _T("대출상태"), n, strStatus );
									if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3130::AppendixReturn()") );
									
									if( ( strStatus == _T("1") ) || ( strStatus == _T("3") ) || ( strStatus == _T("4") ) ) continue;
									
									ids = pSrcDM->GetCellData( _T("선정"), n, sSelect);
									if( sSelect != _T("V") )
									{
										ids = pSrcDM->SetCellData( _T("선정"), _T("V"), n );
										if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_3130::AppendixReturn()") );	
										pGrid->SetAt(n, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
										
										nSelectCnt++;
										bIsAppendixReturn = TRUE;
									}
								}
							}
						}
					}
					if( bIsAppendixReturn )
					{
						InitValueSetting();
					}
				}
			}
		}
	}
	
	nMode = 0;
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();

	CString sData;
	INT		nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pCopyDM->InsertRow(-1);
		
		for( int j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			ids = pCopyDM->SetCellData( sField[j][1] , sData , i );
		}

		if( nMode == 1)
			pGrid->SelectGetNext();
	}

	pGrid->SelectGetHeadPosition();
	for( i=0; i<nSelectCnt; i++ )
	{
		if( m_nCurrentPos == pGrid->SelectGetIdx() )
			break;

		pGrid->SelectGetNext();
	}
	
	return 0;
	
EFS_END
	return -1;
}

VOID CBO_LOC_3130::OnradSTEP1() 
{
EFS_BEGIN
	
	((CEdit*)GetDlgItem(IDC_edtMONEYPAID2))->SetReadOnly();

	m_nArrearPaymentStep = 0;

	((CButton*)GetDlgItem( IDC_radSTEP1 ))->SetCheck(1);
	((CButton*)GetDlgItem( IDC_radSTEP2 ))->SetCheck(0);
	((CButton*)GetDlgItem( IDC_radSTEP3 ))->SetCheck(0);

	((CEdit*)GetDlgItem( IDC_edtARREARDAY ))->SetWindowText( m_sAllArrearDay );

	((CEdit*)GetDlgItem( IDC_edtARREARDAY3 ))->SetWindowText( _T("0") );
	CString sGetData;
	sGetData = m_sAllArrear;
	SetMoneyFormat(sGetData);
	((CEdit*)GetDlgItem( IDC_edtMONEYPAID ))->SetWindowText( sGetData );
	CString sLoanStopDate, delayDays;
	((CEdit*)GetDlgItem( IDC_edtARREARDAY3 ))->GetWindowText( delayDays );
	GetLoanStopDate2( sLoanStopDate, delayDays );
	((CEdit*)GetDlgItem( IDC_datLOANSTOPDATE ))->SetWindowText( sLoanStopDate );
	

	SetDMToClass(m_nArrearPaymentStep);

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3130_2"), _T("MainGrid") );
	pGrid->SetReverse(0);

	SetSelUserChange(0);

EFS_END
}

VOID CBO_LOC_3130::OnradSTEP2() 
{
EFS_BEGIN
	
	((CEdit*)GetDlgItem(IDC_edtMONEYPAID2))->SetReadOnly();

	SetDelayMoney();

	m_nArrearPaymentStep = 1;
	m_nLoanStopUseStatus = 1;

	((CButton*)GetDlgItem( IDC_radSTEP1 ))->SetCheck(0);
	((CButton*)GetDlgItem( IDC_radSTEP2 ))->SetCheck(1);
	((CButton*)GetDlgItem( IDC_radSTEP3 ))->SetCheck(0);

	((CEdit*)GetDlgItem( IDC_edtARREARDAY ))->SetWindowText( m_sAllArrearDay );
	((CEdit*)GetDlgItem( IDC_edtARREARDAY3 ))->SetWindowText( m_sAllApplyArrearDay );
	((CEdit*)GetDlgItem( IDC_edtMONEYPAID ))->SetWindowText( _T("0") );

	CString sLoanStopDate, delayDays;
	((CEdit*)GetDlgItem( IDC_edtARREARDAY3 ))->GetWindowText( delayDays );
	GetLoanStopDate2( sLoanStopDate, delayDays );
	((CEdit*)GetDlgItem( IDC_datLOANSTOPDATE ))->SetWindowText( sLoanStopDate );
	
	SetDMToClass(m_nArrearPaymentStep);

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3130_2"), _T("MainGrid") );
	pGrid->SetReverse(0);

	SetSelUserChange(0);

EFS_END
}

VOID CBO_LOC_3130::OnradSTEP3()
{
EFS_BEGIN

	m_nArrearPaymentStep = 2;

	((CButton*)GetDlgItem( IDC_radSTEP1 ))->SetCheck(0);
	((CButton*)GetDlgItem( IDC_radSTEP2 ))->SetCheck(0);
	((CButton*)GetDlgItem( IDC_radSTEP3 ))->SetCheck(1);

	CString sLoanStopDate, delayDays;
	((CEdit*)GetDlgItem( IDC_edtARREARDAY3 ))->GetWindowText( delayDays );
	GetLoanStopDate2( sLoanStopDate, delayDays );
	((CEdit*)GetDlgItem( IDC_datLOANSTOPDATE ))->SetWindowText( sLoanStopDate );

	SetDMToClass(m_nArrearPaymentStep);

EFS_END
}

VOID CBO_LOC_3130::OnSelChangeMaingridUser() 
{
EFS_BEGIN

	CESL_Grid* pGrid = (CESL_Grid*)(FindControl("CM_BO_LOC_3130_2", "MainGrid"));
	INT nIndex = pGrid->GetIdx();
	SetSelUserChange( nIndex );
	return;

EFS_END
}

VOID CBO_LOC_3130::OnClickMaingridUser()
{
EFS_BEGIN

	CString sArrear;
	((CEdit*)GetDlgItem( IDC_edtRETURN9 ))->GetWindowText( sArrear );
	sArrear.Replace( _T(","), _T("") );
	if( sArrear == _T("0") )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEYPAID2))->SetReadOnly();
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_edtMONEYPAID2))->SetReadOnly(FALSE);
		((CEdit*)GetDlgItem(IDC_edtMONEYPAID2))->SetFocus();
		
		INT nLen = m_strEdtPayment.GetLength();
		m_edtPayment.SetSel(0, nLen);
	}
	return;

EFS_END
}

INT CBO_LOC_3130::SetSelUserChange(INT nIndex)
{
EFS_BEGIN
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3130_2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetSelUserChange") );

	CString sArrearDay;
	CString sArrear;
	CString sPaymentArrear;
	CString sApplyArrearDay;

	pDM->GetCellData( _T("연체일수"), nIndex, sArrearDay );
	((CEdit*)GetDlgItem( IDC_edtARREARDAY1 ))->SetWindowText( sArrearDay );

	pDM->GetCellData( _T("예정연체료"), nIndex, sArrear);
	SetMoneyFormat(sArrear);
	((CEdit*)GetDlgItem( IDC_edtRETURN9 ))->SetWindowText( sArrear );

	pDM->GetCellData( _T("납부연체료"), nIndex, sPaymentArrear);
	((CEdit*)GetDlgItem( IDC_edtMONEYPAID2 ))->SetWindowText( sPaymentArrear );

	pDM->GetCellData( _T("적용연체일"), nIndex, sApplyArrearDay);
	((CEdit*)GetDlgItem( IDC_edtARREARDAY2 ))->SetWindowText( sApplyArrearDay );
	
	return 0;

EFS_END	
return -1;
}

INT CBO_LOC_3130::SetDMToClass(INT nClass)
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3130_2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetDMToClass") );

	CString sArrearDay, sArrear;

	if( nClass == 0 )
	{
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData( _T("연체일수"), i, sArrearDay );
			sArrear.Format( _T("%d"), _ttoi(m_sBasicArrear) * _ttoi(sArrearDay) );
			pDM->SetCellData( _T("예정연체료"), sArrear, i );
			pDM->SetCellData( _T("납부연체료"), sArrear, i );
			pDM->SetCellData( _T("적용연체일"), _T("0"), i );
		}
	}

	else if( nClass == 1 )
	{
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData( _T("연체일수"), i, sArrearDay );
			sArrear.Format( _T("%d"), _ttoi(m_sBasicArrear) * _ttoi(sArrearDay) );
			pDM->SetCellData( _T("예정연체료"), sArrear, i );
			pDM->SetCellData( _T("납부연체료"), _T("0"), i );
			pDM->SetCellData( _T("적용연체일"), sArrearDay, i );
		}
	}
	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3130::GetLoanStopDate2( CString &sLoanStopDate, CString &delayDays )
{
EFS_BEGIN

	INT ids;

	CString sPreLoanStopDate;
	CTime tPreLoanStopDate( 1971, 1, 1, 0, 0, 0 );
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출정지일"), sPreLoanStopDate, 0 );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetLoanStopDate2"));
	}	
	if( !sPreLoanStopDate.IsEmpty() )
	{
		CTime tTempDate(_ttoi(sPreLoanStopDate.Mid(0,4)), _ttoi(sPreLoanStopDate.Mid(5,2)), _ttoi(sPreLoanStopDate.Mid(8,2)), 0, 0, 0);
		tPreLoanStopDate = tTempDate;
	}

	CString sExpLoanDate; 
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("반납예정일"), sExpLoanDate, m_nCurrentPos );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("GetLoanStopDate2"));
	}
	if( sExpLoanDate.IsEmpty() )
	{
		return 0;
	}

	CString sReturnType;
	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("GetLoanStopDate2"));
	}

	CString sReturnDateControlAlias = _T("반납일");
	if( sReturnType.Compare( _T("2") ) == 0 )
	{
		sReturnDateControlAlias = _T("소급반납일");
	}

	CString sReturnDate;
	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate );
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("GetLoanStopDate2"));
	}
	if( sReturnDate.IsEmpty() )
	{
		return 0;
	}

	CTime tExpLoanDate( _ttoi(sExpLoanDate.Mid(0,4)), _ttoi(sExpLoanDate.Mid(5,2)), _ttoi(sExpLoanDate.Mid(8,2)), 0, 0, 0 );
	CTime tReturnDate( _ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0, 0, 0 );
	CTimeSpan tDelayDay = tReturnDate - tExpLoanDate;

	INT nDelayDay;
	if( delayDays.IsEmpty() )
	{
		delayDays.Format( _T("%d"), tDelayDay.GetDays() );
		nDelayDay = tDelayDay.GetDays();

		CString strValue = _T("");
		m_pSpfWork->m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체기간휴관일포함유무"), strValue);
		if(!strValue.IsEmpty() && strValue != _T("Y"))
		{
			CString strQuery = _T("");
			CString strHolidayCount = _T("");
			INT nHolidayCount = 0;
			CESL_DataMgr pTempDM;
			pTempDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );		

			strQuery.Format(_T("SELECT GET_HOLIDAYCOUNT(TO_DATE('%s','YYYY/MM/DD'), TO_DATE('%s','YYYY/MM/DD'), '%s') FROM DUAL"), sExpLoanDate , sReturnDate, m_pInfo->MANAGE_CODE);
			pTempDM.GetOneValueQuery(strQuery, strHolidayCount );
			nHolidayCount = _ttoi( strHolidayCount );

			if(nHolidayCount < nDelayDay)
			{
				nDelayDay -= nHolidayCount;
			}
		}
	}
	else
	{
		nDelayDay = _ttoi(delayDays);
	}
	if( nDelayDay < 1 )
	{
		sLoanStopDate.Empty();
		return 0;
	}
	tDelayDay = CTimeSpan( nDelayDay , 0 , 0 , 0 );
	if( tDelayDay.GetDays() < 1 ) 
	{
		sLoanStopDate.Empty();
		return 0;
	}

	
	CTimeSpan tOneDay( 1, 0, 0, 0 );
	if( tPreLoanStopDate < tReturnDate ) 
	{
		if( _T("Y") == m_pCheckLoan->m_pManageValue->m_sIsAddHolyDayLoanStop )
		{
			nDelayDay--;
			m_pCheckLoan->GetAddTotalHolyDay( nDelayDay, tReturnDate );
			tDelayDay = CTimeSpan( nDelayDay, 0, 0, 0 );
		}
		else
		{
			tDelayDay = tDelayDay - tOneDay;				
		}
		CTime tExpLoanStopDate = tReturnDate + tDelayDay;
		sLoanStopDate = tExpLoanStopDate.Format( _T("%Y/%m/%d") );
	}
	else
	{
		if( _T("Y") == m_pCheckLoan->m_pManageValue->m_sIsAddHolyDayLoanStop )
		{
			m_pCheckLoan->GetAddTotalHolyDay( nDelayDay, tPreLoanStopDate );
			tDelayDay = CTimeSpan( nDelayDay, 0, 0, 0 );
		}
		CTime tExpLoanStopDate = tPreLoanStopDate + tDelayDay;			
		sLoanStopDate = tExpLoanStopDate.Format( _T("%Y/%m/%d") );
	} 	

	INT nMaxDay = _ttoi(m_pSpfWork->m_pManageValue->m_strMaxLoanStopDay);
	if( nMaxDay > 0 )
	{
		CTimeSpan tMaxDay = CTimeSpan(nMaxDay-1, 0, 0, 0);
		CTime tMaxLoanStopDate = tReturnDate + tMaxDay;

		CTime tLoanStopDate( _ttoi(sLoanStopDate.Mid(0,4)), _ttoi(sLoanStopDate.Mid(5,2)), _ttoi(sLoanStopDate.Mid(8,2)), 0, 0, 0 );

		if(tLoanStopDate > tMaxLoanStopDate)
		{
			sLoanStopDate = tMaxLoanStopDate.Format( _T("%Y/%m/%d") );
		}
		else
		{
			sLoanStopDate = tLoanStopDate.Format( _T("%Y/%m/%d") );
		}
	}

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3130::InsertPaymentArrear()
{
EFS_BEGIN

	CString sArrearDay, sPlan, sPayment, sTotalPayment;
	((CEdit*)GetDlgItem( IDC_edtRETURN9 ))->GetWindowText( sPlan );
	((CEdit*)GetDlgItem( IDC_edtMONEYPAID2 ))->GetWindowText( sPayment );
	sPlan.Replace( _T(","), _T("") );	sPayment.Replace( _T(","), _T("") );

	INT nPayment = _ttoi(sPayment);
	INT nBasicArrear = _ttoi(m_sBasicArrear);

	if( _ttoi(sPayment) <= 0 )
	{
		sPayment = _T("0");
		((CEdit*)GetDlgItem( IDC_edtMONEYPAID2 ))->SetWindowText( sPayment );
		return;
	}

	float nDiv;

	nDiv = (float)(nPayment % nBasicArrear);

	if( nDiv != 0 )
	{	
		AfxMessageBox( _T("기본연체료는 ") + m_sBasicArrear + _T("원 입니다.\r\n") +
						m_sBasicArrear + _T(" 원 단위로 입력하여 주십시오.") );
		((CEdit*)GetDlgItem( IDC_edtMONEYPAID2 ))->SetFocus();

		INT nLen = m_strEdtPayment.GetLength();
		m_edtPayment.SetSel(0, nLen);

		return;		
	}
	
	if( _ttoi(sPlan) < _ttoi(sPayment) )
	{		
		AfxMessageBox( _T("연체료는 ") + sPlan + _T("원 입니다.\r\n") +
						sPlan + _T("원 이하로 입력하세요") );
		((CEdit*)GetDlgItem( IDC_edtMONEYPAID2 ))->SetFocus();

		INT nLen = m_strEdtPayment.GetLength();
		m_edtPayment.SetSel(0, nLen);

		return;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3130_2"), _T("MainGrid") );
	INT index = pGrid->GetIdx();

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3130_2") );

	sArrearDay.Format( _T("%d"), ( _ttoi(sPlan)-_ttoi(sPayment) ) / _ttoi(m_sBasicArrear) );
	((CEdit*)GetDlgItem( IDC_edtARREARDAY2 ))->SetWindowText( sArrearDay );

	pDM->SetCellData( _T("납부연체료"), sPayment, index );
	pDM->SetCellData( _T("적용연체일"), sArrearDay, index );

	m_sAllApplyArrearDay = _T("");
	m_sAllPayment = _T("");
	CString sBookLoanDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanBookInfo = FindDM(sBookLoanDMAlias);
	CString strRegNo, strTempRegNo;
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		pDM->GetCellData( _T("납부연체료"), i, sPayment );
		pDM->GetCellData( _T("적용연체일"), i, sArrearDay );

		if( _T("Y") != m_pCheckLoan->m_pManageValue->m_sIsAppendixLoanStop )
		{
			CString sAppendixFlag;
			pDM->GetCellData( _T("등록번호"), i, strRegNo );
			sAppendixFlag = _T("");
			for( INT y = 0 ; y < pLoanBookInfo->GetRowCount() ; y++ )
			{
				pLoanBookInfo->GetCellData( _T("등록번호"), y, strTempRegNo);
				if ( strTempRegNo == strRegNo )
				{
					pLoanBookInfo->GetCellData( _T("부록여부"), y, sAppendixFlag);
					break;
				}				
			}			
			
			if ( sAppendixFlag != _T("A") ) 
			{
				m_sAllApplyArrearDay.Format( _T("%d"), _ttoi(m_sAllApplyArrearDay) + _ttoi(sArrearDay) );
				m_sAllPayment.Format( _T("%d"), _ttoi(m_sAllPayment) + _ttoi(sPayment) );
			}
			else{}
		}
		else
		{
			m_sAllApplyArrearDay.Format( _T("%d"), _ttoi(m_sAllApplyArrearDay) + _ttoi(sArrearDay) );
			m_sAllPayment.Format( _T("%d"), _ttoi(m_sAllPayment) + _ttoi(sPayment) );
		}
	}

	((CEdit*)GetDlgItem( IDC_edtARREARDAY3 ))->SetWindowText( m_sAllApplyArrearDay );
	CString sGetData;
	sGetData = m_sAllPayment;
	SetMoneyFormat(sGetData);
	((CEdit*)GetDlgItem( IDC_edtMONEYPAID ))->SetWindowText( sGetData );

	m_nLoanStopUseStatus = 1;

	CString sLoanStopDate, delayDays;
	((CEdit*)GetDlgItem( IDC_edtARREARDAY3 ))->GetWindowText( delayDays );
	GetLoanStopDate2( sLoanStopDate, delayDays );
	if( sLoanStopDate.IsEmpty() ) sLoanStopDate = _T("");
	((CEdit*)GetDlgItem( IDC_datLOANSTOPDATE ))->SetWindowText( sLoanStopDate );

	((CButton*)GetDlgItem( IDC_radSTEP1 ))->SetCheck(0);
	((CButton*)GetDlgItem( IDC_radSTEP2 ))->SetCheck(0);
	((CButton*)GetDlgItem( IDC_radSTEP3 ))->SetCheck(1);

	((CEdit*)GetDlgItem( IDC_btnRETURNSAVE ))->SetFocus();

	return;

EFS_END
}

INT CBO_LOC_3130::FeeInfoDBUpdate(CKolas2up_MobileApi *pMobileApi, CString sRecKey, CString strKLUserYN/*=_T("N")*/)
{
EFS_BEGIN
	
	INT ids;

	pMobileApi->InitDBFieldData();

	CString sParentRegNo, sChildRegNo, sMoneyPaid, sPlanArrear, sInsertRecKey, sSysDate, sQuery;
	CString sDelayDay;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3130_2") );
	GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("등록번호"), sParentRegNo, m_nCurrentPos );
	
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("등록번호"), i, sChildRegNo );
		if( sParentRegNo == sChildRegNo )
		{
			pDM->GetCellData( _T("납부연체료"), i, sMoneyPaid );
			pDM->GetCellData( _T("예정연체료"), i, sPlanArrear );
			pDM->GetCellData( _T("적용연체일"), i, sDelayDay );
			break;
		}
	}

	CString sExpLoanDate;
	CString sReturnDate;
	
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("반납예정일"), sExpLoanDate, m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("FeeInfoDBUpdate()") );
	if( sExpLoanDate.IsEmpty() ) return 0;

	CString sReturnType;
	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FeeInfoDBUpdate()") );

	CString sReturnDateControlAlias = _T("반납일");
	if( sReturnType.Compare( _T("2") )== 0 )
		sReturnDateControlAlias = _T("소급반납일");

	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias , sReturnDate );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("FeeInfoDBUpdate()") );

	if( sReturnDate.IsEmpty() )
	{
		AfxMessageBox( _T("반납일을 지정하십시오") );
		return 0;
	}

	if( !sMoneyPaid.IsEmpty() )
	{
		sInsertRecKey	= _T("ESL_SEQ.NEXTVAL");
		sSysDate		= _T("SYSDATE");
		CString sUserID = m_pInfo->USER_ID;
		CString sFirstWork = GetWorkLogMsg( _T("반납시수수료"), __WFILE__, __LINE__ );

		if( sMoneyPaid == _T("0") )
		{
			COleDateTime tExpLoanDate( _ttoi(sExpLoanDate.Mid(0,4)), _ttoi(sExpLoanDate.Mid(5,2)), _ttoi(sExpLoanDate.Mid(8,2)), 0, 0, 0 );
			COleDateTime tReturnDate( _ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0, 0, 0 );
			COleDateTimeSpan tDelayDay = tReturnDate - tExpLoanDate;
			INT nDelayDay = tDelayDay.GetDays();

			if( 0 < nDelayDay )
			{
				sQuery.Format(  _T("INSERT INTO CO_FEE_PAYMENT_INFO_TBL")
								_T("( REC_KEY, CLASS_CODE, LOAN_INFO_KEY, PAYMENT_DATE, MONEY_PAID, STATUS")
								_T(", CREATE_DATE, RECEIPT_PRINT_YN, USER_ID, MANAGE_CODE, FIRST_WORK ) ")
								_T("VALUES( %s, '0', '%s', %s, '%s', 'Y', %s, 'N', '%s', '%s', '%s' );")
								, sInsertRecKey, sRecKey, sSysDate, sMoneyPaid, sSysDate, sUserID, m_pInfo->MANAGE_CODE,sFirstWork );
				pMobileApi->AddFrame( sQuery );
			}
		}
		else
		{
			sQuery.Format(  _T("INSERT INTO CO_FEE_PAYMENT_INFO_TBL")
							_T("( REC_KEY, CLASS_CODE, LOAN_INFO_KEY, PAYMENT_DATE, MONEY_PAID, STATUS")
							_T(", CREATE_DATE, RECEIPT_PRINT_YN, USER_ID, MANAGE_CODE, FIRST_WORK ) ")
							_T("VALUES( %s, '0', '%s', %s, '%s', 'N', %s, 'N', '%s', '%s', '%s' );")
							, sInsertRecKey, sRecKey, sSysDate, sMoneyPaid, sSysDate, sUserID, m_pInfo->MANAGE_CODE,sFirstWork );
			pMobileApi->AddFrame( sQuery );
		}
	}

	CESL_DataMgr *pUpdateDM = FindDM( _T("DM_BO_LOC_3100_DB") );
	if( pUpdateDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("FeeInfoDBUpdate()") );
	pUpdateDM->InitDBFieldData();

	CString sLoanStopDate, sIsAppendix, sUserKey;

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록여부"), sIsAppendix, m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -7, _T("FeeInfoDBUpdate()") );
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sUserKey, 0 );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -7, _T("FeeInfoDBUpdate()") );
	
 	CString sBookCnt;
 	CString sUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
 	ids = GetDataMgrData( sUserDMAlias , _T("총대출책수"), sBookCnt , 0 );
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("FeeInfoDBUpdate()") );

	if(_T("A") == sIsAppendix)
	{
		if(TRUE == m_pCheckLoan->IsAppendixIncludeLoanCnt())
		{
			sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)-1);
		}
	}
	else
	{
		sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)-1);	
	}
	if(1 > _ttoi(sBookCnt))
	{
		sBookCnt = _T("0");
	}
	ids = pUpdateDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sBookCnt, TRUE);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("CBO_LOC_3130::LoanUserInfoDBUpdate()"));
	}
	if(sDelayDay.IsEmpty())
	{
		sDelayDay = _T("0");
	}
	if(0 < _ttoi(sDelayDay))
	{
		CString sValue;
		sQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(%s,TO_DATE('%s','YYYY/MM/DD'),UDF_MANAGE_CODE,%s),'YYYY/MM/DD') FROM DUAL"), sRecKey, sReturnDate, sDelayDay);
		pUpdateDM->GetOneValueQuery(sQuery, sValue);
		CString strTempDate = _T("");
		if(sValue.IsEmpty())
		{
			sLoanStopDate = _T("NULL");
		}
		else
		{
			sLoanStopDate.Format(_T("TO_DATE('%s','YYYY/MM/DD')"), sValue);
			strTempDate = sValue + _T(" 00:00:00");
		}
		if(_T("NULL") != sLoanStopDate)
		{
			m_sLoanStopDate = sValue;
			ids = pUpdateDM->AddDBFieldData(_T("LOAN_STOP_DATE"), _T("NUMERIC"), sLoanStopDate, TRUE);
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("FeeInfoDBUpdate()"));
			}

			INT nTotalDelayCnt=0;
 			ids = GetTotalDelayCnt( nTotalDelayCnt );
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("CBO_LOC_3130::LoanUserInfoDBUpdate()") );
 			if( nTotalDelayCnt != 0 )
 			{
 				CString sTotalDelayCnt;
 				sTotalDelayCnt.Format(_T("%d"), nTotalDelayCnt );
 				pUpdateDM->AddDBFieldData(_T("DELAY_COUNT"), _T("NUMERIC"), sTotalDelayCnt, TRUE);			
 			}
 			pUpdateDM->AddDBFieldData(_T("LOAN_STOP_SET_DATE"), _T("NUMERIC"), _T("SYSDATE"), TRUE);
			CString sLog;
			sLog.Format(_T("DELAYDAY_COUNT+%s"), sDelayDay);
			pUpdateDM->AddDBFieldData(_T("DELAYDAY_COUNT"), _T("NUMERIC"), sLog, TRUE);

			if(_T("Y") == m_pSpfWork->m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
			{
				CESL_DataMgr *pKLDM = FindDM(_T("DM_KL_RETURN"));
				if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("LoanUserInfoDBUpdate"));
				pKLDM->SetCellData(_T("회원상태"), _T("1"), 0);
				pKLDM->SetCellData(_T("대출정지일"), strTempDate, 0);
			}
		}
	}
	pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("반납"), __WFILE__, __LINE__ ), TRUE );
	ids = pMobileApi->MakeUpdateFrame( _T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(  -8, _T("FeeInfoDBUpdate()") );
	pUpdateDM->InitDBFieldData();

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3130::OnChangeedtMONEYPAID2() 
{
	UpdateData(TRUE);

	m_strEdtPayment.Replace( _T(","), _T("") );

	CString strBuf = m_strEdtPayment;
	strBuf.Replace( _T(","), _T("") );

	INT nSize = ( strBuf.GetLength() * 3 ) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize - 1);

	m_strEdtPayment = pszFormattedNumber;

	if (pszFormattedNumber)
	delete [] pszFormattedNumber;

	UpdateData(FALSE);

	INT nLen = m_strEdtPayment.GetLength();
	m_edtPayment.SetSel(nLen, -1);
}

VOID CBO_LOC_3130::SetMoneyFormat(CString &sMoney)
{
	sMoney.Replace( _T(","), _T("") );

	CString strBuf = sMoney;
	strBuf.Replace( _T(","), _T("") );

	INT nSize = ( strBuf.GetLength() * 3 ) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize - 1);

	sMoney = pszFormattedNumber;

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;
}

void CBO_LOC_3130::OnChangeedtRETURN4() 
{
	CString sGetData;
	((CEdit*)GetDlgItem(IDC_edtRETURN4))->GetWindowText(sGetData);
	SetMoneyFormat(sGetData);
	((CEdit*)GetDlgItem(IDC_edtPLANARREAR))->SetWindowText(sGetData);
}

INT CBO_LOC_3130::ILL_UpdateTransInfo(BOOL& bIsEndTrans)
{
	INT ids = 0;
	CString strLoanKey, strAppendixFlag;

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출/반납정보KEY"), strLoanKey , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4, _T("CBO_LOC_3130::ILL_UpdateTransInfo()") );
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록여부"), strAppendixFlag , m_nCurrentPos );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -4, _T("CBO_LOC_3130::ILL_UpdateTransInfo()") );
	CESL_DataMgr* pUpdateDM = FindDM( _T("DM_BO_LOC_3100_DB") );
	if( !pUpdateDM ) ERROR_MSG_RETURN_ERROR_CODE2( -7, _T("CBO_LOC_3130::ILL_UpdateTransInfo()") );

	CString strQuery = _T("");
	CString strGiveLibCode;
	strQuery.Format(
		_T("SELECT GIVE_LIBCODE FROM ILL_TRANS_TBL WHERE LOAN_KEY = '%s'"),
		strLoanKey);
	ids = pUpdateDM->GetOneValueQuery(strQuery, strGiveLibCode);

	if(strGiveLibCode != m_strLibCode && ILL_ALERT == TRUE && strAppendixFlag == _T("B")){
		ids = ILL_RETURN_MESSAGE();
		if(0 > ids) return 2;
	}

	if(strGiveLibCode == m_strLibCode)
	{
		strQuery.Format(
			_T("UPDATE ILL_TRANS_TBL ")
			_T("SET ")
				_T("(STATE,END_DIV,END_DATE,END_USERID,RETURN_LIBCODE,")
				_T("RETURN_PLAN_DATE,LS_RETURN_DATE,LAST_WORK) = ")
				_T("(SELECT ")
					_T("'0009','0010',SYSDATE,'%s','%s',")
					_T("RETURN_DATE,RETURN_DATE,'%s' ")
				_T("FROM LS_WORK_T01 WHERE REC_KEY = %s) ")
			_T("WHERE LOAN_KEY = %s;"),	
			m_pInfo->USER_ID,m_strLibCode, 
			GetWorkLogMsg( _T("책두레본관반납"), __WFILE__, __LINE__ ), 
			strLoanKey,
			strLoanKey);

		bIsEndTrans = TRUE;
	}
	else
	{
		strQuery.Format(
			_T("UPDATE ILL_TRANS_TBL ")
			_T("SET ")
				_T("(RETURN_LIBCODE,RETURN_PLAN_DATE,LS_RETURN_DATE,LAST_WORK) = ")
				_T("(SELECT ")
					_T("'%s',RETURN_DATE,RETURN_DATE,'%s' ")
				_T("FROM LS_WORK_T01 WHERE REC_KEY = %s) ")
			_T("WHERE LOAN_KEY = %s;"),
			m_strLibCode, GetWorkLogMsg( _T("반납"), __WFILE__, __LINE__ ), 
			strLoanKey,
			strLoanKey);

		bIsEndTrans = FALSE;
		m_n3130ILLReturnProcCnt++;
	}

	pUpdateDM->AddFrame(strQuery);

	return 0;
}

INT CBO_LOC_3130::ILL_InsertTransInfo()
{
EFS_BEGIN
 
 	INT ids = 0;
 	CString strFuncName = _T("ILL_InsertTransInfo");
 	CString strLoanKey;
 	ids = GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출/반납정보KEY"), strLoanKey, m_nCurrentPos);
 	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	}
 	CString strUserNo;
 	ids = GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strUserNo, 0);
 	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	}
 	CString strPublishFormCode;
 	ids = GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("단행/연속구분"), strPublishFormCode, m_nCurrentPos);
 	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}
 	CString strBookKey;
 	ids = GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("책KEY"), strBookKey, m_nCurrentPos);
 	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	}
 	CString strQuery;
 	CString strGiveLibCode;
 	CString strTblName;
 	if(_T("MO") == strPublishFormCode)
	{
		strTblName = _T("BO_BOOK_TBL");
	}
 	else if(_T("SE") == strPublishFormCode)
	{
		strTblName = _T("SE_BOOK_TBL");
	}
 	else if(_T("NB") == strPublishFormCode)
	{
		strTblName = _T("CO_NON_DB_BOOK_TBL");
	}
	CESL_DataMgr* pUpdateDM = FindDM(_T("DM_BO_LOC_3100_DB"));
 	if(NULL == pUpdateDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1040, strFuncName);
	}
	if(_T("I") == m_pSpfWork->m_pManageValue->m_sBookCooperativeMode)
	{
 		strQuery.Format(_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE ")
						_T("MANAGE_CODE=(SELECT MANAGE_CODE FROM %s WHERE REC_KEY=%s)")
						, strTblName, strBookKey);
	}
	else if(_T("D") == m_pSpfWork->m_pManageValue->m_sBookCooperativeMode)
	{
 		CString strManageCode;
 		ids = GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("관리구분"), strManageCode, m_nCurrentPos);
 		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1070, strFuncName);
		}
		strQuery.Format(_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE ")
						_T("MANAGE_CODE = '%s'")
						, strManageCode);
	}
	ids = pUpdateDM->GetOneValueQuery(strQuery, strGiveLibCode);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1050, strFuncName);
	}
	if(ILL_ALERT == TRUE){
		ids = ILL_RETURN_MESSAGE();
		if(0 > ids) return 2;
	}
	
	CString strSEQData;
	CESL_DataMgr TmpDM;
  	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	if(_T("I") == m_pSpfWork->m_pManageValue->m_sBookCooperativeMode)
	{
		TmpDM.GetOneValueQuery(
				_T("SELECT TRANS_NO_SEQ.NEXTVAL FROM DUAL"),
				strSEQData
				);
	}
	else
	{
		ids = m_pSpfWork->GetTransNo(strSEQData);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1060, strFuncName);
		}
	}
	m_strTransNo = strSEQData;


	CString sFirstWork = GetWorkLogMsg(_T("타관반납"), __WFILE__, __LINE__);
	strQuery.Format(
			_T("INSERT INTO ILL_TRANS_TBL")
			_T("(TRANS_NO,STATE,USER_NO,GIVE_LIBCODE,RETURN_LIBCODE,LOAN_KEY,LS_LOAN_DATE,")
			_T("RETURN_PLAN_DATE,LS_RETURN_DATE,USER_KEY,BOOK_KEY,PUBLISH_FORM_CODE,REG_NO,")
			_T("CALL_NO,TITLE_INFO,AUTHOR_INFO,PUBLISHER_INFO,VOL_INFO,FIRST_WORK,LAST_WORK) ")
			_T("SELECT ")
			_T("'%s','0014','%s','%s','%s',LS.REC_KEY,LS.LOAN_DATE,LS.RETURN_DATE,LS.RETURN_DATE,LS.USER_KEY,")
			_T("LS.BOOK_KEY,LS.PUBLISH_FORM_CODE,LS.REG_NO,LS.CALL_NO,LS.TITLE,LS.AUTHOR,LS.PUBLISHER,LS.VOL_INFO,'%s','%s' ")
			_T("FROM LS_WORK_T01 LS WHERE LS.REC_KEY=%s;")
			, strSEQData, strUserNo, strGiveLibCode, m_strLibCode, sFirstWork, sFirstWork, strLoanKey);

	

	pUpdateDM->AddFrame(strQuery);

	m_n3130ILLReturnProcCnt++;

	return 0;

EFS_END
return -1;
}
