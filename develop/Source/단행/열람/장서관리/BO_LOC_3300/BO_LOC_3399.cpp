// BO_LOC_3399.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3399.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3399 dialog


CBO_LOC_3399::CBO_LOC_3399(CESL_Mgr* pParent)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3399)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_LOC_3399::~CBO_LOC_3399()
{
	
}

VOID CBO_LOC_3399::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3399)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3399, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3399)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabMAILTITLE, OnSelchangetabMAILTITLE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnUPLOAD, OnbtnUPLOAD)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3399 message handlers

BOOL CBO_LOC_3399::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();		
	EnableThemeDialogTexture(GetSafeHwnd()); 
	InitControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

INT CBO_LOC_3399::InitControl()
{
	EFS_BEGIN

	INT ids;

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMAILTITLE);
	pTab->InsertItem( 0 , _T("신규 회원 메일 발송") );
	pTab->InsertItem( 1 , _T("환경 설정") );
	
	((CButton*)GetDlgItem(IDC_radGRIDSELECT))->SetCheck(1);

	ids = ShowHideControlFromTab();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );

	ids = ShowControlFromDB();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );
	

	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_3399::ShowHideControlFromTab()
{
	EFS_BEGIN

	const INT nMailSendCnt = 3;

	INT nMailSendIDC[nMailSendCnt] =
	{
		IDC_staMODE , IDC_radGRIDSELECT , IDC_radCOND 
	};
	
	const INT nEnvCnt = 3;
	INT nEnvIDC[nEnvCnt] =
	{
		IDC_staENV , IDC_staMAILSEND , IDC_cmbMAILSENDYN
	};

	INT nMode;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMAILTITLE);
	nMode = pTab->GetCurSel();


	INT nShowMailSendYN;
	INT nShowEnvYN;
	CString sButtonName;

	if( nMode == 0 )
	{
		nShowMailSendYN = SW_SHOW;
		nShowEnvYN = SW_HIDE;
		sButtonName = _T("발 송");
	}
	else
	{
		nShowMailSendYN = SW_HIDE;
		nShowEnvYN = SW_SHOW;
		sButtonName = _T("저 장");
	}

	for( INT i = 0 ; i < nMailSendCnt ; i++ )
		GetDlgItem(nMailSendIDC[i])->ShowWindow(nShowMailSendYN);

	for( i = 0 ; i < nEnvCnt ; i++ )
		GetDlgItem(nEnvIDC[i])->ShowWindow(nShowEnvYN);

	GetDlgItem(IDC_btnUPLOAD)->SetWindowText(sButtonName);

	return 0;

	EFS_END

	return -1;
}

VOID CBO_LOC_3399::OnSelchangetabMAILTITLE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowHideControlFromTab();
	
	*pResult = 0;
}

VOID CBO_LOC_3399::OnbtnCLOSE() 
{
	OnCancel();	
}


INT CBO_LOC_3399::ShowControlFromDB()
{
	EFS_BEGIN

	INT ids;

	CString sYN;

	ids = GetManageValue( _T("열람") , _T("공통") , _T("이용자관리") , _T("입력시신규회원메일발송여부") , sYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowControlFromDB") );

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbMAILSENDYN);
	
	CString sCombo;
	for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sCombo);
		if( sCombo.Compare(sYN) == 0 )
		{
			pCombo->SetCurSel(i);
		}

	}
	return 0;

	EFS_END

	return -1;

}

VOID CBO_LOC_3399::OnbtnUPLOAD() 
{
	EFS_BEGIN

	INT nMode;
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMAILTITLE);
	nMode = pTab->GetCurSel();

	if( nMode == 0 )
	{
		SendMail();
	}
	else
	{
		SaveEnvValue();
	}

	EFS_END
	
}

INT CBO_LOC_3399::SaveEnvValue()
{
	EFS_BEGIN

	INT ids;

	CString sYN;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbMAILSENDYN);

	pCombo->GetLBText( pCombo->GetCurSel() , sYN );

	ids = SetManageValue( _T("열람") , _T("공통") , _T("이용자관리") , _T("입력시신규회원메일발송여부") , sYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveEnvValue") );

	AfxMessageBox(_T("저장하였습니다."));
	
	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_3399::SendMail()
{
	EFS_BEGIN

	INT ids;

	INT nMode;
	nMode = ((CButton*)GetDlgItem(IDC_radGRIDSELECT))->GetCheck();

	if( nMode == 1 )
	{
		ids = SendGridUser();
	}
	else
	{
		ids = SendNoSendUser();
	}
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SendMail") );
	if( ids > 0 ) return ids;
	


	return 0;

	EFS_END

	return -1;	
}

INT CBO_LOC_3399::SendGridUser()
{
	EFS_BEGIN

	INT ids;

	CStringArray saWhere;
	CString sWhere;
	CString sParentDM = _T("DM_BO_LOC_3300_LIST");
	CString sParentCM = _T("CM_BO_LOC_3300");
	CString sParentGrid = _T("MainGrid");
	CESL_DataMgr* pParentDM = FindDM( sParentDM );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SendGridUser") );
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( sParentCM , sParentGrid );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SendGridUser") );
	
	if( pParentDM->GetRowCount() == 0 )
	{
		AfxMessageBox(_T("이용자를 먼저 검색해 주십시오"));
		return 1;
	}
	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox(_T("메일을 보낼 이용자를 먼저 선택해 주십시오"));
		return 1;
	}

	pGrid->SelectGetHeadPosition();

	INT	nSendCnt = 0;
	CString sUserRecKey,sRecKeyList, sMailingUseYN , strMail;
	INT nIndex;
	INT nMaxSendCnt = 5;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pParentDM->GetCellData( _T("REC_KEY") , nIndex , sUserRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SendGridUser") );

		ids = pParentDM->GetCellData( _T("메일수신여부") , nIndex , sMailingUseYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SendGridUser") );
		if ( sMailingUseYN == _T("N") ) 
		{
			pGrid->SelectGetNext();
			continue;
		}

		ids = pParentDM->GetCellData( _T("E_MAIL") , nIndex , strMail );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SendGridUser") );
		if ( TRUE == strMail.IsEmpty() ) 
		{
			pGrid->SelectGetNext();
			continue;
		}
		
		if( !sRecKeyList.IsEmpty() ) sRecKeyList += _T(",");
		sRecKeyList += sUserRecKey;

		if( (i == (nSelectCnt-1)) || (((i+1)%nMaxSendCnt) == 0) )  
		{
			sWhere.Format( _T(" CLU.REC_KEY IN ( %s ) ") , sRecKeyList );
			sRecKeyList.Empty();
			saWhere.Add(sWhere);
		}

		nSendCnt++;

		pGrid->SelectGetNext();
	}

	if ( nSendCnt <= 0 )
	{
		AfxMessageBox( _T("선택하신 자료중에 메일을 보낼수 있는 자료가 존재하지 않습니다.") );
		return 0;
	}

	
	CLocMailMgrAPI MailMgr(this);
	MailMgr.CreateMailInfo();
	
	ids = MailMgr.MakeRecord( 8 , saWhere , TRUE );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SendGridUser") );
	
	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_3399::SendNoSendUser()
{
	EFS_BEGIN

	INT ids;
	CString sWhere;

	sWhere.Format( _T(" (SELECT COUNT(*) FROM CO_LOAN_MAIL_SEND_TBL CLMS WHERE CLMS.LOAN_KEY=CLU.REC_KEY ) = 0  AND MAILING_USE_YN != 'N' AND E_MAIL IS NOT NULL "));
	
	CLocMailMgrAPI MailMgr(this);
	MailMgr.CreateMailInfo();
	
	ids = MailMgr.MakeRecord( 8 , sWhere , TRUE );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SendNoSendUser") );

	return 0;

	EFS_END
	return -1;
}
HBRUSH CBO_LOC_3399::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
