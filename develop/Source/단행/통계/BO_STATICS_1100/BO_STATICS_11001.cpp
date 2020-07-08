// BO_STATICS_11001.cpp : implementation file
//

#include "stdafx.h"
#include "BO_STATICS_11001.h"
#include "..\BO_STATICS_5163\BO_5163STATICS.h"
#include "..\BO_STATICS_ND\BO_STATICS_ND1.h"
#include "BO_LOAN_CNT_STATICS.h"
#include "BO_LOAN_JOURNAL_STATICS.h"

#include "SBL.h"

#include "BO_STATICS_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_11001 dialog

CBO_STATICS_11001::CBO_STATICS_11001(CString strMenuAlias,CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_STATICS_11001)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sSMAlias = _T("BO_STATICS_1100");
	m_strMenuName = strMenuAlias;
	m_pTmpDlg = NULL;
}

CBO_STATICS_11001::~CBO_STATICS_11001()
{	
	
	if( m_pTmpDlg )
	{	
		delete m_pTmpDlg;
		m_pTmpDlg = NULL;
	}
}

VOID CBO_STATICS_11001::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATICS_11001)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_STATICS_11001, CDialog)
	//{{AFX_MSG_MAP(CBO_STATICS_11001)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmbSTATICS_CLASS, OnSelchangecmbSTATICSCLASS)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnDMVIEW, OnbtnDMVIEW)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabMAIN_OPTION, OnSelchangetabMAINOPTION)
	ON_BN_CLICKED(IDC_chkDUP_DEL, OnchkDUPDEL)
	ON_BN_CLICKED(IDC_chkSUMZERODEL, OnchkSUMZERODEL)
	ON_BN_CLICKED(IDC_BTN_CHANGE_TITLE, OnBtnChangeTitle)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RUN_S, OnRunS)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_11001 message handlers

VOID CBO_STATICS_11001::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	SetControlPos();

EFS_END
}

BOOL CBO_STATICS_11001::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CBO_STATICS_11001::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	INT ids;

	ids = InitESL_Mgr( m_sSMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_sJobClass = m_strMenuName;

	GetManageValue(_T("열람"),_T("공통"),_T("대출자료관리"),_T("야간대출예약기능유무"),m_strNightReservationLoan);
	m_strNightReservationLoan.TrimLeft();m_strNightReservationLoan.TrimRight();
	m_strNightReservationLoan.MakeUpper();

	MakeDMInfo();

	InitScreen();
	
	InitEtcDialog();

	if( (m_strMenuName.Compare(WORKING_STATICS) == 0) || (m_strMenuName.Compare(HOLDING_STATICS) == 0) || 
		(m_strMenuName.Compare(LOAN_STATICS) == 0 ) )
		((CButton*)GetDlgItem(IDC_chkDUP_DEL))->SetCheck(1);

	FILE *fd = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if( fd != NULL )
		fclose(fd);
	else
		GetDlgItem(IDC_btnDMVIEW)->ShowWindow(SW_HIDE);

	((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->SetCheck(1);	

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if( m_strMenuName.Compare(WORKING_STATICS)==0 || ( m_strMenuName.Compare(HOLDING_STATICS)==0 ) )
		pGrid->SetRowHeightMin(410);
	else if ( m_strMenuName.Compare(SE_HOLDING_STATICS) == 0 )
		pGrid->SetRowHeightMin(550);
	else
		pGrid->SetRowHeightMin(300);

	if (m_strMenuName.CompareNoCase(WORKING_STATICS) == 0 ||
		m_strMenuName.CompareNoCase(HOLDING_STATICS) == 0 )
	{	
		CWnd* pWnd = GetDlgItem(IDC_stc_UNIT);
		ASSERT(pWnd);
		pWnd->ShowWindow(SW_SHOW);
	}
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  

EFS_END
	return FALSE;
}

VOID CBO_STATICS_11001::InitEtcDialog()
{
EFS_BEGIN

	CString sMenuName = m_strMenuName;

	m_pSearchCreateDlg1 = NULL;
	m_pSearchCreateDlg2 = NULL;
	m_pSearchCreateDlg3 = NULL;
	m_pSearchCreateDlg4 = NULL;
	
	if ( sMenuName.Compare(LOAN_STATICS) == 0 )
	{
		m_pSearchCreateDlg1 = new CBO_STATICS_1110(this,m_pSInfoMgr);
		m_pSearchCreateDlg1->pMain = this->pMain;
		m_pSearchCreateDlg1->pParentMgr = (CESL_Mgr*)this;
		m_pSearchCreateDlg1->m_pInfo = this->m_pInfo;
	}
	else if ( sMenuName.Compare(WORKING_STATICS) == 0 )
	{
		m_pSearchCreateDlg2 = new CBO_STATICS_1111(this,m_pSInfoMgr);
		m_pSearchCreateDlg2->pMain = this->pMain;
		m_pSearchCreateDlg2->pParentMgr = (CESL_Mgr*)this;
		m_pSearchCreateDlg2->m_pInfo = this->m_pInfo;
	}
	else if ( sMenuName.Compare(HOLDING_STATICS) == 0 )
	{		
		m_pSearchCreateDlg3 = new CBO_STATICS_1112(this,m_pSInfoMgr);
		m_pSearchCreateDlg3->pMain = this->pMain;
		m_pSearchCreateDlg3->pParentMgr = (CESL_Mgr*)this;
		m_pSearchCreateDlg3->m_pInfo = this->m_pInfo;
		
	}
	else if ( sMenuName.Compare(SE_HOLDING_STATICS) == 0 )
	{		
		m_pSearchCreateDlg4 = new CBO_STATICS_1113(this,m_pSInfoMgr);
		m_pSearchCreateDlg4->pMain = this->pMain;
		m_pSearchCreateDlg4->pParentMgr = (CESL_Mgr*)this;
		m_pSearchCreateDlg4->m_pInfo = this->m_pInfo;
	}

	m_pXYCreateDlg = new CBO_STATICS_1120(this,m_pSInfoMgr);
	m_pXYCreateDlg->pMain = this->pMain;
	m_pXYCreateDlg->pParentMgr = (CESL_Mgr*)this;
	m_pXYCreateDlg->m_pInfo = this->m_pInfo;

EFS_END
}

INT CBO_STATICS_11001::InitScreen()
{
EFS_BEGIN

	INT ids;

	ids = MakeStaticsClass();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_STATICS_11001::InitScreen()") );

	ChangeScreen();

	return 0;

EFS_END
	return -1;
}


INT CBO_STATICS_11001::MakeStaticsClass()
{
	EFS_BEGIN

	INT ids;

	CComboBox *pCombo = ((CComboBox*)GetDlgItem(IDC_cmbSTATICS_CLASS));

	CString sJobClass , sStaticsClass;

	for( INT i = 0 ; i < m_pStaticsMainInfoDM->GetRowCount() ; i++ )
	{
		ids = m_pStaticsMainInfoDM->GetCellData( _T("업무구분") , i , sJobClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

		if( m_sJobClass.Compare( sJobClass ) != 0 ) continue;

		ids = m_pStaticsMainInfoDM->GetCellData( _T("통계종류") , i , sStaticsClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );

		if( _T("야간대출자료통계")==sStaticsClass && _T("Y")!=m_strNightReservationLoan )
			continue;

		if( SE_HOLDING_STATICS == m_strMenuName &&
			( _T("자료실별이용통계")==sStaticsClass || _T("신규구입도서이용통계")==sStaticsClass ) )
			continue;

		pCombo->AddString( sStaticsClass );
	}

	pCombo->SetCurSel(0);

	pCombo->GetLBText(0, sStaticsClass);
	SetControlData(_T("CM_BO_STATICS_1100") , _T("통계제목") , sStaticsClass , 0 , 0 );

	return 0;

	EFS_END
	return -1;

}

INT CBO_STATICS_11001::MakeDMInfo()
{
	EFS_BEGIN

	m_pSInfoMgr = new CStaticsInfoMgr(this,m_strMenuName);
	m_pSInfoMgr->MakeDMInfo();

	m_pStaticsMainInfoDM = m_pSInfoMgr->m_pStaticsMainInfoDM;

	return 0;

	EFS_END
	return -1;

}

VOID CBO_STATICS_11001::OnSelchangecmbSTATICSCLASS() 
{
EFS_BEGIN

	INT ids;
	
	CString sStaticsClass;
	GetControlData( _T("CM_BO_STATICS_1100") , _T("통계종류") , sStaticsClass , -1 , 0 );
	if( m_sLastStat != sStaticsClass )
	{
		CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
		pGrid->Clear();
		pGrid->SetRows( 2 );
		pGrid->SetCols( 0 , 2 );
	}
	
	ids = ChangeScreen();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_STATICS_11001::OnSelchangecmbSTATICSCLASS()") );

	ids = SetInitData();	
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_STATICS_11001::OnSelchangecmbSTATICSCLASS()") );
	
	SetControlData(_T("CM_BO_STATICS_1100") , _T("통계제목") , sStaticsClass , 0 , 0 );

EFS_END
}

INT CBO_STATICS_11001::SetInitData()
{
EFS_BEGIN

	INT ids;
	
	CString sStaticsClass;
	GetControlData( _T("CM_BO_STATICS_1100") , _T("통계종류") , sStaticsClass , -1 , 0 );

	if ( sStaticsClass == _T("자료통계") )
	{
		STATICS_5163( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("전자/시청각통계") )
	{
		STATICS_5163( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("자료유형통계") )
	{
		STATICS_5163( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("수입구분별통계(국방대)") )
	{
		STATICS_ND( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("관리구분별통계") )
	{
		STATICS_ND( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("자료류별통계") )
	{
		STATICS_ND( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("장서보유현황") )
	{
		STATICS_ND( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("교범통계") )
	{
		STATICS_ND( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("교내발간자료통계") )
	{
		STATICS_ND( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("비도서자료통계") )
	{
		STATICS_ND( sStaticsClass );
		return 0;
	}
	else if ( sStaticsClass == _T("대출횟수통계") || sStaticsClass == _T("대출횟수통계(책두레포함)") )
	{
		CBO_LOAN_CNT_STATICS dlg(this);
		dlg.m_sStaticsClass = sStaticsClass;
		dlg.m_pParentGrid=(CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
		dlg.DoModal();
		return 0;
	}

	CString sMenuName = m_strMenuName;

	if ( sMenuName.Compare(LOAN_STATICS) == 0 )
	{
		if ( m_pSearchCreateDlg1->GetSafeHwnd() != NULL )
		{
			ids = m_pSearchCreateDlg1->InitScreen();
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_STATICS_11001::SetInitData()") );
		}
	}
	else if ( sMenuName.Compare(WORKING_STATICS) == 0 )
	{
		if ( m_pSearchCreateDlg2->GetSafeHwnd() != NULL )
		{
			ids = m_pSearchCreateDlg2->InitScreen();
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_STATICS_11001::SetInitData()") );
		}
	}
	else if ( sMenuName.Compare(HOLDING_STATICS) == 0 )
	{
		if ( m_pSearchCreateDlg3->GetSafeHwnd() != NULL )
		{
			ids = m_pSearchCreateDlg3->InitScreen();
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_STATICS_11001::SetInitData()") );
		}
	}
	else if ( sMenuName.Compare(SE_HOLDING_STATICS) == 0 )
	{
		if ( m_pSearchCreateDlg4->GetSafeHwnd() != NULL )
		{
			ids = m_pSearchCreateDlg4->InitScreen();
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_STATICS_11001::SetInitData()") );
		}
	}

	if( m_pXYCreateDlg->GetSafeHwnd() != NULL )
	{
		ids = m_pXYCreateDlg->InitScreen();
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_STATICS_11001::SetInitData()") );
	}

	return 0;

EFS_END
	return -1;
}

VOID CBO_STATICS_11001::OnShowWindow(BOOL bShow, UINT nStatus) 
{
EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
		SetControlPos();

EFS_END
}

INT CBO_STATICS_11001::RunStatics()
{
EFS_BEGIN

	BEGIN_SBL();

	INT ids;
	
	CString sMenuName = m_strMenuName;

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_STATICS_1100") );
	if ( !pCM ) ERROR_MSG_RETURN_ERROR_CODE2( -41, _T("CBO_STATICS_11001::RunStatics()") );

	CString sStaticsClass;
	ids = pCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_STATICS_11001::RunStatics()") );

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	CProgressCtrl *pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgSTATICS);
	CProgressCtrl *pTotalProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgSTATICS2);
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabMAIN_OPTION);

	pGrid->Clear();
	pGrid->SetRow(1);
	pGrid->SetCol(1);

	if( sMenuName.Compare(LOAN_STATICS)==0 )
	{ 
		if( !m_nMode )
		{
			ids = m_pSInfoMgr->MakeStaticsCondition(sStaticsClass);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_STATICS_11001::RunStatics()") );
			if ( ids > 0 ) return ids;

			ids = m_pSInfoMgr->RunLoanStatics(sStaticsClass,pProgressCtrl,pTotalProgressCtrl);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_STATICS_11001::RunStatics()") );

			ShowGrid();
		}
		else 
		{
			ids = m_pSInfoMgr->MakeOrderStaticsDM( sStaticsClass , pProgressCtrl , pTotalProgressCtrl  );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_STATICS_11001::RunStatics()") );
			if ( ids > 0 ) return ids;

			ids = m_pSInfoMgr->OrderStaticsView( sStaticsClass , pGrid , 0 );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_STATICS_11001::RunStatics()") );

			ids = m_pSInfoMgr->ViewTab( sStaticsClass , pTab );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_STATICS_11001::RunStatics()") );

			
		}
	}
	else if ( sMenuName.Compare(WORKING_STATICS) == 0 || (sMenuName.Compare(HOLDING_STATICS) == 0) )
	{
		ids = m_pSInfoMgr->MakeStaticsCondition(sStaticsClass);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CBO_STATICS_11001::RunStatics()") );
		if ( ids > 0 ) return ids;

		ids = m_pSInfoMgr->RunBoStatics( sStaticsClass,pProgressCtrl,pTotalProgressCtrl);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_STATICS_11001::RunStatics()") );
		if ( ids > 0 ) return ids;

		ShowGrid();
	}
	else if ( sMenuName.Compare( SE_HOLDING_STATICS ) == 0 )
	{
		ids = m_pSInfoMgr->MakeStaticsCondition(sStaticsClass);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CBO_STATICS_11001::RunStatics()") );
		if ( ids > 0 ) return ids;

		ids = m_pSInfoMgr->RunSeStatics( sStaticsClass,pProgressCtrl,pTotalProgressCtrl);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_STATICS_11001::RunStatics()") );
		if ( ids > 0 ) return ids;

		ShowGrid();
	}

	INT nRowCount = m_pSInfoMgr->m_pRealResultDM->GetRowCount();
	END_SBL(nRowCount);

	AfxMessageBox( _T("완료되었습니다."));

	CESL_Grid* pGrid_MainGrid_ALL = (CESL_Grid*)FindControl(_T("CM_BO_STATICS_1100"), _T("통계결과그리드"));
	if( pGrid_MainGrid_ALL == NULL) return -1;
	if(!m_pInfo->m_bUserInfoExportYN && ( sStaticsClass == _T("다독자") || sStaticsClass == _T("다독자") || sStaticsClass == _T("가족별다독자") || sStaticsClass == _T("다독자(책두레포함)") || sStaticsClass == _T("가족별다독자(책두레포함)")))
	{
		pGrid_MainGrid_ALL->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid_MainGrid_ALL->m_bPrivacyPrintLogRecord = TRUE;
		pGrid_MainGrid_ALL->m_strPrintInfo = _T("이용자관리 엑셀출력");
	}

	m_sLastStat = sStaticsClass;

	return 0;

EFS_END
	return -1;
}

VOID CBO_STATICS_11001::OnbtnDMVIEW()
{
EFS_BEGIN

	m_pSInfoMgr->DeBugDM();

EFS_END
}

INT CBO_STATICS_11001::ChangeScreen()
{
EFS_BEGIN

	INT ids;

	CString sStaticsClass , sXAlias , sYAlias , sOption , sOrder;
	
	ids = GetControlData( _T("CM_BO_STATICS_1100") , _T("통계종류") , sStaticsClass , -1 , 0 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_STATICS_11001::ChangeScreen()") );
	
	ids = m_pSInfoMgr->GetXYAlias( sStaticsClass , sXAlias , sYAlias , sOption , sOrder );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_STATICS_11001::ChangeScreen()") );

	if ( sOrder.Compare( _T("-1") ) == 0 ) m_nMode = 0;
	else                               m_nMode = 1;

	if( _T("대출횟수통계")==sStaticsClass || _T("대출횟수통계(책두레포함)")==sStaticsClass  )
	{
		m_nMode = 0;
	}

	if ( sStaticsClass==_T("자료실별이용통계") || sStaticsClass==_T("신규구입도서이용통계") ) 
	{
		((CStatic*) GetDlgItem (IDC_stc_UNIT))->SetWindowText(_T("단위:대출이력이있는책(전체책)"));
	}
	else
	{
		((CStatic*) GetDlgItem (IDC_stc_UNIT))->SetWindowText(_T("단위:종(책)"));
	}
	
	GetDlgItem(IDC_tabMAIN_OPTION)->ShowWindow( m_nMode );
	
	GetDlgItem(IDC_chkDUP_DEL)->EnableWindow( !m_nMode );
	GetDlgItem(IDC_chkSUMZERODEL)->EnableWindow( !m_nMode );

	SetControlPos();
	
	return 0;

EFS_END
	return -1;
}

VOID CBO_STATICS_11001::OnSelchangetabMAINOPTION(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	INT ids;

	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabMAIN_OPTION);
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);

	CString sStaticsClass , sXAlias , sYAlias , sOption , sOrder;
	
	ids = GetControlData( _T("CM_BO_STATICS_1100") , _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnSelchangetabMAINOPTION") );

	ids = m_pSInfoMgr->OrderStaticsView( sStaticsClass , pGrid , pTab->GetCurSel() );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnSelchangetabMAINOPTION") );


	*pResult = 0;

	EFS_END
}

VOID CBO_STATICS_11001::OnchkDUPDEL() 
{
	EFS_BEGIN

	if (m_pSInfoMgr && m_pSInfoMgr->m_pRealResultDM && m_pSInfoMgr->m_pStaticsMainInfoDM)
	{
		if (m_strMenuName.CompareNoCase(LOAN_STATICS) == 0 || m_strMenuName.CompareNoCase(WORKING_STATICS) == 0 || m_strMenuName.CompareNoCase(HOLDING_STATICS) == 0 ) 
		{
			if (m_pSInfoMgr->m_pRealResultDM->GetRowCount() > 0) 
			{
				CString strStaticsClass;
				INT ids = GetControlData(_T("CM_BO_STATICS_1100"), _T("통계종류"), strStaticsClass, -1, 0);
				if (ids < 0) ERROR_MSG_RETURN_VOID2(-1, _T("CBO_STATICS_11001::OnchkDUPDEL()"));
				
				CString strX, strY, strOpt, strOrder;
				ids = m_pSInfoMgr->GetXYAlias(strStaticsClass, strX, strY, strOpt, strOrder);
				if (ids < 0) ERROR_MSG_RETURN_VOID2(-1, _T("CBO_STATICS_11001::OnchkDUPDEL()"));

				if (_ttoi(strOrder) < 0)
				{
					CString strDupStandard;
					ids = m_pSInfoMgr->GetCellData(m_pSInfoMgr->m_pStaticsMainInfoDM, _T("통계종류"), strStaticsClass, _T("중복결과설명"), strDupStandard);
					if (ids < 0) ERROR_MSG_RETURN_VOID2(-1, _T("CBO_STATICS_11001::OnchkDUPDEL()"));
					
					CString strDupYN = (((CButton*)GetDlgItem(IDC_chkDUP_DEL))->GetCheck() == 1) ? _T("포함된") : _T("포함되지 않은");

					CString strMsg;
					strMsg.Format(_T("%s/%s 중복이 %s %s 합계를 표시합니다."), strX, strY, strDupYN, strDupStandard);
					AfxMessageBox(strMsg);					
				}
			}
		}
	}

	ShowGrid();


	EFS_END
}


VOID CBO_STATICS_11001::ShowGrid()
{
	EFS_BEGIN

	INT ids;

	CString sStaticsClass;
	CString sMenuName = m_strMenuName;

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);

	ids = GetControlData( _T("CM_BO_STATICS_1100") , _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnSelchangetabMAINOPTION") );

	bool bDupDel = TRUE;
	bool bZeroShow = TRUE;

	if( ((CButton*)GetDlgItem(IDC_chkDUP_DEL))->GetCheck() == 1 || _T("가격합산통계") == sStaticsClass )
	{
		bDupDel = FALSE;
	}
	
	if( ((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->GetCheck() == 1 )
		bZeroShow = FALSE;

	
	ids = m_pSInfoMgr->StaticsResultViewGrid(sStaticsClass,pGrid,bDupDel , bZeroShow , sMenuName);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -3 , _T("RunStatics") );	

	EFS_END
}

VOID CBO_STATICS_11001::OnchkSUMZERODEL() 
{
	EFS_BEGIN

	ShowGrid();	

	EFS_END
}


INT CBO_STATICS_11001::ViewCondWindow()
{
	CString sMenuName = m_strMenuName;

	if( sMenuName.Compare(LOAN_STATICS)== 0 )
	{
		if( m_pSearchCreateDlg1->GetSafeHwnd() == NULL )
		{
			m_pSearchCreateDlg1->Create(this);
			m_pSearchCreateDlg1->ShowWindow(SW_SHOW);
			m_pSearchCreateDlg1->UpdateWindow();
			m_pSearchCreateDlg1->CenterWindow();
		}
		else
		{
			m_pSearchCreateDlg1->ShowWindow(SW_SHOW);
		}
	}
	else if( sMenuName.Compare(WORKING_STATICS)== 0 )
	{
		if( m_pSearchCreateDlg2->GetSafeHwnd() == NULL )
		{
			m_pSearchCreateDlg2->Create(this);
			m_pSearchCreateDlg2->ShowWindow(SW_SHOW);
			m_pSearchCreateDlg2->UpdateWindow();
			m_pSearchCreateDlg2->CenterWindow();
		}
		else
		{
			m_pSearchCreateDlg2->ShowWindow(SW_SHOW);
		}
	}
	else if( sMenuName.Compare(HOLDING_STATICS)== 0 )
	{		
		if( m_pSearchCreateDlg3->GetSafeHwnd() == NULL )
		{
			m_pSearchCreateDlg3->Create(this);
			m_pSearchCreateDlg3->ShowWindow(SW_SHOW);
			m_pSearchCreateDlg3->UpdateWindow();
			m_pSearchCreateDlg3->CenterWindow();
		}
		else
		{
			m_pSearchCreateDlg3->ShowWindow(SW_SHOW);
		}
	}
	else if( sMenuName.Compare(SE_HOLDING_STATICS)== 0 )
	{		
		if( m_pSearchCreateDlg3->GetSafeHwnd() == NULL )
		{
			m_pSearchCreateDlg3->Create(this);
			m_pSearchCreateDlg3->ShowWindow(SW_SHOW);
			m_pSearchCreateDlg3->UpdateWindow();
			m_pSearchCreateDlg3->CenterWindow();
		}
		else
		{
			m_pSearchCreateDlg3->ShowWindow(SW_SHOW);
		}
	}

	return 0;
	
}

INT CBO_STATICS_11001::ViewXYWindow()
{
	if( m_pXYCreateDlg->GetSafeHwnd() == NULL )
	{
		m_pXYCreateDlg->Create(this);
		m_pXYCreateDlg->ShowWindow(SW_SHOW);
		m_pXYCreateDlg->UpdateWindow();
		m_pXYCreateDlg->CenterWindow();
	}
	else
		m_pXYCreateDlg->ShowWindow(SW_SHOW);


	return 0;
}


LRESULT CBO_STATICS_11001::OnRunS(WPARAM wParam, LPARAM lParam) 
{
	EFS_BEGIN
		
	INT ids;
	
	AfxGetApp()->DoWaitCursor( 1 );
	ids = RunStatics();
	AfxGetApp()->DoWaitCursor( -1 );
	
	if( ids > 0 )
	{
		if( ids == 101 )
			ViewCondWindow();
		else if( ids == 102 )
			ViewXYWindow();
	}	
		
	return 0;
	
	EFS_END
		
	return -1;
}


INT CBO_STATICS_11001::PrintStat()
{
	EFS_BEGIN

	INT ids;

	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabMAIN_OPTION);

	CString sStaticsClass;
	GetControlData( _T("CM_BO_STATICS_1100") , _T("통계종류") , sStaticsClass , -1 , 0 );
	if( m_sLastStat != sStaticsClass || m_sLastStat.IsEmpty())
	{
		AfxMessageBox( _T("통계를 구성하십시오") );
		return 1;
	}

	bool bDupDel = TRUE;
	bool bZeroShow = TRUE;
	
	if( ((CButton*)GetDlgItem(IDC_chkDUP_DEL))->GetCheck() == 1 )
		bDupDel = FALSE;
	
	if( ((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->GetCheck() == 1 )
		bZeroShow = FALSE;

	ids = m_pSInfoMgr->PrintResult( m_sLastStat , bDupDel , bZeroShow , pTab  );
 	if( ids > 0 ) return TRUE;


	EFS_END

	return -1;

}

VOID CBO_STATICS_11001::STATICS_5163( CString strStaticsName )
{
	CBO_5163STATICS dlg (this);
	dlg.SetStaticsName (strStaticsName);
	dlg.DoModal();

	return;

}

INT CBO_STATICS_11001::STATICS_ND(CString sStaticsName)
{
	CBO_STATICS_ND dlg(this);
	dlg.SetStaticsName( sStaticsName );
	dlg.DoModal();
	return 0;
}

INT CBO_STATICS_11001::ShowTempDlg(BOOL bReadOnly)
{
	CString sStaticsClass;
	GetControlData( _T("CM_BO_STATICS_1100") , _T("통계종류") , sStaticsClass , -1 , 0 );
		
	if( _T("자료통계") == sStaticsClass || 
		_T("전자/시청각통계") == sStaticsClass ||
		_T("자료유형통계") == sStaticsClass ||
		_T("수입구분별통계(국방대)") == sStaticsClass ||
		_T("관리구분별통계") == sStaticsClass ||
		_T("자료류별통계") == sStaticsClass ||
		_T("장서보유현황") == sStaticsClass ||
		_T("교범통계") == sStaticsClass ||
		_T("교내발간자료통계") == sStaticsClass ||
		_T("비도서자료통계") == sStaticsClass ||					
		_T("대출횟수통계") == sStaticsClass ||
		_T("대출횟수통계(책두레포함)") == sStaticsClass )
	{
		SetInitData();
		return 0;
	}

	if (sStaticsClass.CompareNoCase(_T("관내업무일지통계")) == 0)
	{
		CBO_LOAN_JOURNAL_STATICS dlg(this);
		dlg.DoModal();
		 return 0;
	}

	CString sMenuName = m_strMenuName;
	CWnd* pCondWnd;

	if( sMenuName == LOAN_STATICS )
	{
		pCondWnd = m_pSearchCreateDlg1;
	}
	else if( sMenuName == WORKING_STATICS )
	{
		pCondWnd = m_pSearchCreateDlg2;
	}
	else if( sMenuName == HOLDING_STATICS )
	{
		pCondWnd = m_pSearchCreateDlg3;
	}
	else if( sMenuName == SE_HOLDING_STATICS )
	{
		pCondWnd = m_pSearchCreateDlg4;
	}
	
	if( !m_pTmpDlg )
	{
		m_pTmpDlg = new CTmpDialog(this , pCondWnd , this->m_pXYCreateDlg , sMenuName  );
		m_pTmpDlg->pMain = this->pMain;
		m_pTmpDlg->pParentMgr = (CESL_Mgr*)this;
		if (bReadOnly) m_pTmpDlg->SM_STYLE = 'R';
		m_pTmpDlg->m_pInfo = this->m_pInfo;
		if (m_pTmpDlg->Create(this))
		{
			m_pTmpDlg->ShowWindow(SW_SHOW);
			m_pTmpDlg->UpdateWindow();
			m_pTmpDlg->CenterWindow();					
		}
		else
		{
			delete m_pTmpDlg;		
		}
	}
	else
	{
		m_pTmpDlg->ShowWindow(SW_SHOW);
		m_pTmpDlg->SetFocus();
	}
	
	return 0;
}

INT CBO_STATICS_11001::SetControlPos()
{
EFS_BEGIN

	CRect rc;
	this->GetWindowRect(&rc);
	ScreenToClient(rc);
	
	INT cx = rc.Width();
	INT cy = rc.Height();

	CWnd* pStatic1		= GetDlgItem(IDC_staTITLE1);
	CWnd* pStatic2		= GetDlgItem(IDC_staTITLE2);
	CWnd* pGuideLine1	= GetDlgItem(IDC_STA_GUIDE_LINE1);
	CWnd* pTab			= GetDlgItem(IDC_tabMAIN_OPTION);	
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	
	if( NULL == pStatic1->GetSafeHwnd() )		return 0;
	if( NULL == pStatic2->GetSafeHwnd() )		return 0;
	if( NULL == pGuideLine1->GetSafeHwnd() )	return 0;
	if( NULL == pTab->GetSafeHwnd() )			return 0;
	if( NULL == pGrid->GetSafeHwnd() )			return 0;

	CRect Bar1;
	CRect Bar2;
	CRect Line1;

	pStatic1->GetWindowRect(&Bar1);
	pStatic2->GetWindowRect(&Bar2);
	pGuideLine1->GetWindowRect(&Line1);

	ScreenToClient(Bar1);
	ScreenToClient(Bar2);
	ScreenToClient(Line1);

	pStatic1->MoveWindow(0, Bar1.top, cx, Bar1.Height());
	pStatic2->MoveWindow(0, Bar2.top, cx, Bar2.Height());
	pGuideLine1->MoveWindow(0, Line1.top, cx, Line1.Height());

	INT nUnderBarPos = Bar1.Height() + Bar2.Height() + 1;

	pTab->MoveWindow(0, nUnderBarPos, cx, cy - nUnderBarPos);

	if( !pTab->IsWindowVisible() )
		pGrid->MoveWindow(5, nUnderBarPos + 1, cx - 5, cy - (nUnderBarPos+1));
	else
		pGrid->MoveWindow(5, (nUnderBarPos+1) + 20, cx - 5, cy - (nUnderBarPos+1) - 20);	

	pGrid->SetRowHeight(0,500);

	CWnd* pUnit = GetDlgItem(IDC_stc_UNIT);
	CWnd* pCheckBox1 = GetDlgItem(IDC_chkSUMZERODEL);
	CWnd* pCheckBox2 = GetDlgItem(IDC_chkDUP_DEL);

	if( NULL == pUnit->GetSafeHwnd() )		return 0;
	if( NULL == pCheckBox1->GetSafeHwnd() )	return 0;
	if( NULL == pCheckBox2->GetSafeHwnd() )	return 0;

	CRect rcUnit;
	CRect rcChkBox1;
	CRect rcChkBox2;

	pUnit->GetWindowRect(&rcUnit);
	pCheckBox1->GetWindowRect(&rcChkBox1);
	pCheckBox2->GetWindowRect(&rcChkBox2);

	ScreenToClient(rcUnit);
	ScreenToClient(rcChkBox1);
	ScreenToClient(rcChkBox2);

	INT nEndPos = cx - 5;

	pUnit->MoveWindow(nEndPos - rcUnit.Width(), rcUnit.top, rcUnit.Width(), rcUnit.Height());
	pCheckBox1->MoveWindow(nEndPos - rcChkBox1.Width(), rcChkBox1.top, rcChkBox1.Width(), rcChkBox1.Height());
	pCheckBox2->MoveWindow(( nEndPos - rcChkBox1.Width() - rcChkBox2.Width() - 10 ), rcChkBox2.top, rcChkBox2.Width(), rcChkBox2.Height());

	return 0;
EFS_END
return -1;
}

void CBO_STATICS_11001::OnBtnChangeTitle() 
{
	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_EDT_STATIC_TITLE);

	if( NULL == pEdt->GetSafeHwnd() )	return;

	if( pEdt->GetStyle() & ES_READONLY )
	{
		pEdt->SetReadOnly(FALSE);
		SetDlgItemText(IDC_BTN_CHANGE_TITLE, _T("변경 적용"));
	}
	else
	{
		pEdt->SetReadOnly(TRUE);
		SetDlgItemText(IDC_BTN_CHANGE_TITLE, _T("제목 변경"));
	}
}

HBRUSH CBO_STATICS_11001::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
					
BEGIN_EVENTSINK_MAP(CBO_STATICS_11001, CESL_Mgr)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_PROC_0400_08)
	ON_EVENT(CBO_STATICS_11001, IDC_gridMAIN, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)	
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_STATICS_11001::OnClickMshflexgrid() 
{
EFS_BEGIN
	if ( m_pSInfoMgr->m_sIs64lib == _T("Y") || m_pSInfoMgr->m_sIsNDlib == _T("Y") || m_pSInfoMgr->m_sIs5163lib == _T("Y") )  
	{
		return ; 
	}
	if(m_pSInfoMgr->m_pRealResultDM == NULL || m_pSInfoMgr->m_pRealResultDM->GetRowCount() < 1)
	{
		return ;
	}

	CString sStaticsClass = _T("");
	INT ids = RunStaticsList(sStaticsClass);
	if(ids >= 0)
	{
		if(m_pSInfoMgr->m_pResultListDM->GetRowCount() > 0)
		{
			CString strTitleList;
			ids = m_pSInfoMgr->GetCellData( m_pSInfoMgr->m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("출력물타이틀") , strTitleList );
			if ( ids >= 0 )
			{
				CBO_STATICS_LIST dlg(this, m_pSInfoMgr->m_pResultListDM);
				dlg.m_strGridTitle = strTitleList;
				dlg.DoModal();
			}
		}
		else
		{

		}
	}
EFS_END
}

INT CBO_STATICS_11001::RunStaticsList(CString &sStaticsClass)
{
EFS_BEGIN
	INT ids;
	
	CString sMenuName = m_strMenuName;

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_STATICS_1100") );
	if ( !pCM ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_STATICS_11001::RunStaticsList()"));

	ids = pCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_STATICS_11001::RunStaticsList()"));

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(!pGrid) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_STATICS_11001::RunStaticsList()"));
	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	CString sXAlias , sYAlias , sOption , sOrder;
	m_pSInfoMgr->GetXYAlias( sStaticsClass , sXAlias , sYAlias , sOption , sOrder );
	
	CString sXValue, sYValue;
	CStringArray saXColumnName;

	m_pSInfoMgr->GetColumnNames( sStaticsClass , sXAlias , saXColumnName , 2 );
	if(nCol <= saXColumnName.GetSize())
	{
		sXValue = saXColumnName.GetAt(nCol-1);
		if(sXValue.IsEmpty()) 
		{
			sXValue = _T("NULL");
		}
	}

	sYValue = pGrid->GetTextMatrix( nRow , 0 );	
	ConvertDescToCode(sStaticsClass, sYAlias, sYValue);
	if(sYValue == _T("합계"))
	{
		sYValue = _T("");
	}
	else if(sYValue.IsEmpty())
	{
		sYValue = _T("NULL");
	}
	
	bool bDupDel = TRUE;
	if( ((CButton*)GetDlgItem(IDC_chkDUP_DEL))->GetCheck() == 1 )
		bDupDel = FALSE;

	if( sMenuName.Compare(LOAN_STATICS)==0 )
	{
		if( !m_nMode )
		{
			ids = m_pSInfoMgr->RunLoanStaticsList(sStaticsClass, sXValue, sYValue, bDupDel);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_STATICS_11001::RunStaticsList()") );
		}
		else 
		{
			return -1;
		}
	}
	else if ( sMenuName.Compare(WORKING_STATICS) == 0 || (sMenuName.Compare(HOLDING_STATICS) == 0) )
	{
		ids = m_pSInfoMgr->RunBoStaticsList( sStaticsClass, sXValue, sYValue );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_STATICS_11001::RunStaticsList()") );
		if ( ids > 0 ) return ids;
	}
	else if ( sMenuName.Compare( SE_HOLDING_STATICS ) == 0 )
	{
		ids = m_pSInfoMgr->RunSeStaticsList( sStaticsClass, sXValue, sYValue );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_STATICS_11001::RunStaticsList()") );
		if ( ids > 0 ) return ids;
	}	
	return 0;

EFS_END
	return -1;
}

VOID CBO_STATICS_11001::ConvertDescToCode(CString sStaticsClass, CString sYAlias, CString &sCode)
{
	INT ids;
	CString sYCode;
	ids = m_pSInfoMgr->GetCellData( m_pSInfoMgr->m_pColumnAliasInfoDM , _T("알리아스명") , sYAlias , _T("설명") , sYCode );

	if( sYAlias.Compare(_T("회원구분별"))==0 )
	{
		if( sCode.Compare(_T("정회원"))==0 ) sCode = _T("0");
		if( sCode.Compare(_T("비회원"))==0 ) sCode = _T("1");
		if( sCode.Compare(_T("준회원"))==0 ) sCode = _T("2");	
	}
	else if( sYAlias.Compare(_T("분류기호별"))==0 ||
			(sYAlias.Compare(_T("KDC별"))==0) || 
			(sYAlias.Compare(_T("DDC별"))==0) || 
			(sYAlias.Compare(_T("LC별"))==0)  ||
			(sYAlias.Compare(_T("분류기호별(DDC)"))==0) ||
			(sYAlias.Compare(_T("분류기호별(DDC)"))==0) )
	{
		ids = ConvertClassNo(sStaticsClass,sCode,sCode,sYAlias);
	}
	else if( sYAlias.Compare(_T("배가상태별")) == 0 )
	{
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 0 );
	}
	else if(sYAlias.Compare(_T("진행상태별")) == 0 )
	{
		if(_T("신청된자료") == sCode)
		{
			sCode = _T("0001");
		}
		else if(_T("요청된자료") == sCode)
		{
			sCode = _T("0002");
		}
		else if(_T("발송된자료") == sCode)
		{
			sCode = _T("0004");
		}
		else if(_T("입수된자료") == sCode)
		{
			sCode = _T("0014");
		}
		else if(_T("복귀중자료") == sCode)
		{
			sCode = _T("0008");
		}
		else if(_T("완료된자료") == sCode)
		{
			sCode = _T("0009");
		}
		else if(_T("발송거절된자료") == sCode)
		{
			sCode = _T("0011");
		}
		else if(_T("요청거절된자료") == sCode)
		{
			sCode = _T("0012");
		}
		else if(_T("신청취소된자료(이용자)") == sCode)
		{
			sCode = _T("0016");
		}
	}
	else if(sYAlias.Compare(_T("취소유형별")) == 0 )
	{
		if(_T("이용자취소") == sCode)
		{
			sCode = _T("0001");
		}
		else if(_T("요청거절") == sCode)
		{
			sCode = _T("0002");
		}
		else if(_T("무응답/미대출") == sCode)
		{
			sCode = _T("0003");
		}
		else if(_T("발송거절(도서대출불가)") == sCode)
		{
			sCode = _T("0004");
		}
		else if(_T("완료") == sCode)
		{
			sCode = _T("0010");
		}
		else if(_T("기타") == sCode)
		{
			sCode = _T("0099");
		}
	}
	else if(_T("통합회원구분별") == sYAlias)
	{
		if(_T("통합회원") == sCode)
		{
			sCode = _T("Y");
		}
		else if(_T("일반회원") == sCode)
		{
			sCode = _T("N");
		}
		else
		{
			sCode = _T("알수없음");
		}
	}
	else if(	_T("등록구분별")		== sYAlias ||
				_T("자료구분별")		== sYAlias ||
				_T("별치기호별")		== sYAlias ||
				_T("자료실구분별")		== sYAlias ||
				_T("수입구분별")		== sYAlias ||
				_T("보조등록구분별")	== sYAlias ||
				_T("자료실별이용")		== sYAlias ||
				_T("신규구입도서이용")	== sYAlias ||
				_T("제어자료구분별")	== sYAlias ||
				_T("이용대상구분별")	== sYAlias ||
				_T("매체구분별")		== sYAlias ||
				_T("간행빈도별")		== sYAlias ||
				_T("직급별")			== sYAlias ||
				_T("소속별")			== sYAlias
			)
	{
		int find = sCode.ReverseFind( _T('(') );
		if( find >= 0 && find <= sCode.GetLength() + 1 )
		{
			sCode = sCode.Mid( find + 1 );
			sCode.Replace( _T(')') , _T('') );
		}
	}
	else
	{
		ids = m_pInfo->pCodeMgr->ConvertDescToCode( sYCode , sCode , sCode );
	}
}

INT CBO_STATICS_11001::ConvertClassNo(CString sStaticsClass, CString sDesc, CString &sCode, CString sClassNoType)
{
	EFS_BEGIN

 	INT ids;

	if( sClassNoType.IsEmpty() )
		return 0;

	CStringArray asColumnCode;
	CStringArray asColumnName;
	ids = m_pSInfoMgr->GetColumnNames( sStaticsClass , sClassNoType, asColumnCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("ConvertClassNo") );
	ids = m_pSInfoMgr->GetColumnNames( sStaticsClass , sClassNoType , asColumnName , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("ConvertClassNo") );

	for( INT i = 0 ; i < asColumnCode.GetSize() ; i++ )
	{
		if( asColumnName.GetAt(i).Compare(sDesc)==0 )
		{
			sCode = asColumnCode.GetAt(i);
			return 0;
		}
	}

	return 0;

	EFS_END
	return -1;

}
