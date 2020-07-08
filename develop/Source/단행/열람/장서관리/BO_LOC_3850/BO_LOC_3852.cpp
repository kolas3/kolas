// BO_LOC_3852.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3852.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3852 dialog


CBO_LOC_3852::CBO_LOC_3852(CESL_Mgr* pParent)
	: CPreNextDlg(pParent,IDD)
{
	//{{AFX_DATA_INIT(CBO_LOC_3852)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3852::~CBO_LOC_3852()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3852::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3852)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3852, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3852)
	ON_BN_CLICKED(IDC_btn3852_PREV, Onbtn3852PREV)
	ON_BN_CLICKED(IDC_btn3852_NEXT, Onbtn3852NEXT)
	ON_BN_CLICKED(IDC_btn3852_CLOSE, Onbtn3852CLOSE)
	ON_BN_CLICKED(IDC_btn3852_SAVE, Onbtn3852SAVE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3852 message handlers

BOOL CBO_LOC_3852::OnInitDialog() 
{
EFS_BEGIN

	CPreNextDlg::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr(_T("BO_LOC_3852"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	InitControl();
	ViewScreen();
	CheckGroupList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}




INT CBO_LOC_3852::ViewScreen()
{
EFS_BEGIN


	INT ids;

	ids = AllControlDisplay(_T("CM_BO_LOC_3852") , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ViewScreen") );

	CComboBox * pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_combo3852);

	CString sLoanUserYN;
	GetDataMgrData(_T("DM_BO_LOC_3850"),_T("대출회원여부"), sLoanUserYN , m_nCurrentIndex );
	if(sLoanUserYN.Compare(_T("Y"))==0){
		pCombo->SetCurSel(0);
	}else{
		pCombo->SetCurSel(1);
	}

	return 0;

EFS_END
return -1;

}



INT CBO_LOC_3852::InitControl()
{
EFS_BEGIN


	if( CPreNextDlg::m_nDlgType == __ECO_NOT_GRID_MODE )
	{
		GetDlgItem(IDC_btn3852_PREV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btn3852_NEXT)->ShowWindow(SW_HIDE);
	}

	return 0;



EFS_END
return -1;

}




VOID CBO_LOC_3852::Onbtn3852PREV() 
{
EFS_BEGIN

	CPreNextDlg::MovePrev();
	ViewScreen();
	

EFS_END
}

VOID CBO_LOC_3852::Onbtn3852NEXT() 
{
EFS_BEGIN

	CPreNextDlg::MoveNext();
	ViewScreen();
	

EFS_END
}

VOID CBO_LOC_3852::Onbtn3852CLOSE() 
{
EFS_BEGIN

	OnCancel();	

EFS_END
}

VOID CBO_LOC_3852::Onbtn3852SAVE() 
{
EFS_BEGIN


	AfxGetApp()->DoWaitCursor(1);

	ModifyGroup();

	AfxGetApp()->DoWaitCursor(-1);
		

EFS_END
}



INT CBO_LOC_3852::ModifyGroup()
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
	ids = UpdateToDB();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertGroup") );
	if( ids > 0 ) return ids;

	
	AfxMessageBox( _T("저장되었습니다.") );


	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3852::ErrorCheckInputValue()
{
EFS_BEGIN

	INT ids;

	CString sCode , sName , sDesc;

	// 적용안함인지 검사 
	CString sPreCode;
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CheckDuplicateValue") );
	ids = pDM->GetCellData( _T("코드") , m_nCurrentIndex , sPreCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ErrorCheckInputValue") );

	sPreCode.TrimLeft(); sPreCode.TrimRight();
	if( sPreCode.IsEmpty() )
	{
		AfxMessageBox( _T("삭제할 수 없는 코드입니다.") );
		return 4;
	}

	ids = GetInputValue( sCode , sName , sDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ErrorCheckInputValue") );


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


INT CBO_LOC_3852::CheckDuplicateValue()
{
EFS_BEGIN

	INT ids;

	CString sCode , sName , sDesc;
	CString sPreCode;

	ids = GetInputValue( sCode , sName , sDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ErrorCheckInputValue") );

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CheckDuplicateValue") );

	ids = pDM->GetCellData( _T("코드") , m_nCurrentIndex , sPreCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CheckDuplicateValue") );

	if( sCode.Compare(sPreCode) == 0 ) return 0;

	CString sSql;
	sSql.Format( _T("SELECT COUNT(*) FROM MN_LOAN_USER_GROUP_TBL WHERE CODE = '%s'") , sCode );

	CString sCount;
	ids = pDM->GetOneValueQuery(sSql,sCount);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CheckDuplicateValue") );


	if( _ttoi(sCount) > 0 ) 
	{
		AfxMessageBox( _T("중복된 코드가 존재합니다. 다른 코드를 입력하십시오.") );
		return 1;
	}



	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3852::UpdateToDB()
{
EFS_BEGIN

	INT ids;

	CString sCode , sName , sDesc;

	ids = GetInputValue( sCode , sName , sDesc );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UpdateToDB") );

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("UpdateToDB") );

	CString sRecKey;
	ids = pDM->GetCellData( _T("그룹정보코드키") , m_nCurrentIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("UpdateToDB") ); 

	pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("CODE") , _T("STRING") , sCode );
	pDM->AddDBFieldData( _T("NAME") , _T("STRING") , sName );
	pDM->AddDBFieldData( _T("DESCRIPTION") , _T("STRING") , sDesc );

	pDM->AddDBFieldData( _T("FIRST_WORK") , _T("STRING") , GetWorkLogMsg( _T("그룹입력") , __WFILE__ , __LINE__ ) );

	CString sLoanUserYN;
	CComboBox * pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_combo3852);
	pCombo->GetLBText(pCombo->GetCurSel(), sLoanUserYN);
	pDM->AddDBFieldData( _T("LOAN_USER_YN") , _T("STRING") , sLoanUserYN );

	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );

	ids = pDM->MakeUpdateFrame( _T("MN_LOAN_USER_GROUP_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("UpdateToDB") );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("UpdateToDB") );

	pDM->EndFrame();

	
	/// DM에 적용
	INT nInsertIndex = m_nCurrentIndex;

	ids = pDM->SetCellData( _T("코드") , sCode , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("UpdateToDB") );
	ids = pDM->SetCellData( _T("이름") , sName , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("UpdateToDB") );
	ids = pDM->SetCellData( _T("설명") , sDesc , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("UpdateToDB") );
	ids = pDM->SetCellData( _T("그룹정보코드키") , sRecKey , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("UpdateToDB") );
	ids = pDM->SetCellData( _T("대출회원여부") , sLoanUserYN , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("UpdateToDB") );

	/// 화면에 보여주기
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3850") , _T("메인그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("InsertToDB") );

	pGrid->DisplayLine( nInsertIndex );
	

	return 0;

EFS_END
return -1;

}

INT CBO_LOC_3852::GetInputValue( CString &sCode , CString &sName , CString &sDesc )
{
EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3852");

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

INT CBO_LOC_3852::CheckGroupList()
{
	EFS_BEGIN

	CString sTemp = _T("");
	GetDlgItemText(IDC_edt3852_INPUT1, sTemp);
	//////////////////////////////////////////////////////////////////////
	// 1. DM 초기화
	//////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CheckGroupList") );
	

	//////////////////////////////////////////////////////////////////////
	// 2. 대출여부의 값에 따라 참조 테이블이 달라진다.
	//////////////////////////////////////////////////////////////////////
	CString query;

	query.Format( _T("select loan_user_yn from mn_loan_user_group_tbl where code = '%s' "), sTemp);
	pDM->GetOneValueQuery(query, sTemp);
	sTemp.TrimLeft();sTemp.TrimRight();
	
	if (sTemp == _T("Y"))
		query.Format( _T(" select count(*) from mn_loan_user_group_list_tbl where group_code = '%s'"), sTemp);
	else
		query.Format( _T(" select count(*) from co_nonmember_tbl where group_code = '%s'"), sTemp);

	pDM->GetOneValueQuery(query, sTemp);
	
	if ( _ttoi(sTemp) > 0 )
	{
		AfxMessageBox(_T("회원 목록을 가진 그룹정보는 수정할 수 없습니다."));
		GetDlgItem(IDC_btn3852_SAVE)->EnableWindow(FALSE);
	}

	return 0;
	EFS_END
	return -1;
}

HBRUSH CBO_LOC_3852::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
