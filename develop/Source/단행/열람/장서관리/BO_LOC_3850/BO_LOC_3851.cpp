// BO_LOC_3851.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3851.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3851 dialog


CBO_LOC_3851::CBO_LOC_3851(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3851)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3851::~CBO_LOC_3851()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3851::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3851)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3851, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3851)
	ON_BN_CLICKED(IDC_btn3850_CLOSE, Onbtn3850CLOSE)
	ON_BN_CLICKED(IDC_btn3850_INSERT, Onbtn3850INSERT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3851 message handlers

BOOL CBO_LOC_3851::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr(_T("BO_LOC_3851"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	
	CComboBox * pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_combo3851);
	pCombo->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_LOC_3851::Onbtn3850CLOSE() 
{
EFS_BEGIN

	
	OnCancel();
	

EFS_END
}



VOID CBO_LOC_3851::Onbtn3850INSERT() 
{
EFS_BEGIN


	AfxGetApp()->DoWaitCursor(1);

	InsertGroup();

	AfxGetApp()->DoWaitCursor(-1);
	

EFS_END
}

INT CBO_LOC_3851::InsertGroup()
{
EFS_BEGIN

	INT ids;

	// 1. 잘 입력되어 있는지 검사
	ids = ErrorCheckInputValue();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertGroup") );
	if( ids > 0 ) return ids;



	// 2. 중복되어 있는지 검사
	ids = CheckDuplicateValue();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertGroup") );
	if( ids > 0 ) return ids;


	// 3. 인서트
	ids = InsertToDB();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertGroup") );
	if( ids > 0 ) return ids;

	
	AfxMessageBox( _T("저장되었습니다.") );


	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3851::ErrorCheckInputValue()
{
EFS_BEGIN

	INT ids;

	CString sCode , sName , sDesc;

	ids = GetInputValue( sCode , sName , sDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ErrorCheckInputValue") );


	if( sCode.IsEmpty() )
	{
		AfxMessageBox( _T("코드를 입력해주십시오.") );
		return 1;
	}

	if( sName.IsEmpty() )
	{
		AfxMessageBox( _T("이름을 입력해주십시오.") );
		return 2;
	}

	if( sCode.GetLength() != 3 )
	{
		AfxMessageBox( _T("코드는 반드시 세자리이어야 합니다.") );
		return 3;
	}



	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3851::CheckDuplicateValue()
{
EFS_BEGIN

	INT ids;

	CString sCode , sName , sDesc;

	ids = GetInputValue( sCode , sName , sDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ErrorCheckInputValue") );

	CString sSql;
	sSql.Format( _T("SELECT COUNT(*) FROM MN_LOAN_USER_GROUP_TBL WHERE CODE = '%s'") , sCode );

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CheckDuplicateValue") );

	CString sCount;
	ids = pDM->GetOneValueQuery(sSql,sCount);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CheckDuplicateValue") );


	if( _ttoi(sCount) > 0 ) 
	{
		AfxMessageBox( _T("중복된 코드가 존재합니다. 다른 코드를 입력하십시오.") );
		return 1;
	}



	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3851::InsertToDB()
{
EFS_BEGIN

	INT ids;

	CString sCode , sName , sDesc;

	ids = GetInputValue( sCode , sName , sDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ErrorCheckInputValue") );

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CheckDuplicateValue") );

	CString sRecKey;
	pDM->MakeRecKey(sRecKey);

	pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("CODE") , _T("STRING") , sCode );
	pDM->AddDBFieldData( _T("NAME") , _T("STRING") , sName );
	pDM->AddDBFieldData( _T("DESCRIPTION") , _T("STRING") , sDesc );

	pDM->AddDBFieldData( _T("FIRST_WORK") , _T("STRING") , GetWorkLogMsg( _T("그룹입력") , __WFILE__ , __LINE__ ) );

	CString sLoanUserYN;
	CComboBox * pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_combo3851);
	pCombo->GetLBText(pCombo->GetCurSel(), sLoanUserYN);
	pDM->AddDBFieldData( _T("LOAN_USER_YN") , _T("STRING") , sLoanUserYN );

	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );

	ids = pDM->MakeInsertFrame( _T("MN_LOAN_USER_GROUP_TBL") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertToDB") );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertToDB") );

	pDM->EndFrame();

	
	/// DM에 적용
	INT nInsertIndex = pDM->GetRowCount();
	pDM->InsertRow(-1);

	ids = pDM->SetCellData( _T("코드") , sCode , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertToDB") );
	ids = pDM->SetCellData( _T("이름") , sName , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("InsertToDB") );
	ids = pDM->SetCellData( _T("설명") , sDesc , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("InsertToDB") );
	ids = pDM->SetCellData( _T("그룹정보코드키") , sRecKey , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("InsertToDB") );
	ids = pDM->SetCellData( _T("대출회원여부") , sLoanUserYN , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("InsertToDB") );

	/// 화면에 보여주기
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3850") , _T("메인그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("InsertToDB") );

	pGrid->DisplayLine( nInsertIndex );
	pGrid->DisplayNumber();

	return 0;

EFS_END
return -1;

}

INT CBO_LOC_3851::GetInputValue( CString &sCode , CString &sName , CString &sDesc )
{
EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3851");

	ids = GetControlData( sCMAlias , _T("코드") , sCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetInputValue") );

	ids = GetControlData( sCMAlias , _T("이름") , sName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetInputValue") );

	ids = GetControlData( sCMAlias , _T("설명") , sDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetInputValue") );

	sCode.TrimLeft(); sCode.TrimRight();
	sCode.Replace(_T(" ") , _T("") );
	sName.TrimLeft(); sName.TrimRight();

	return 0;

EFS_END
return -1;

}

HBRUSH CBO_LOC_3851::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
