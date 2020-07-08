// BO_LOC_3822.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3822.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3822 dialog


CBO_LOC_3822::CBO_LOC_3822(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pCBOLOC3820 = (CBO_LOC_3820*)pParent;

	// KOL.UDF.022
	m_sMailClass = _T("");
	m_nMailClass = 0;
	m_nMailKey = 0;
	m_sIsUnityLoanService = _T("");
	//{{AFX_DATA_INIT(CBO_LOC_3822)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3822::~CBO_LOC_3822()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3822::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3822)
	DDX_Control(pDX, IDC_cMAIL, m_cMail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3822, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3822)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cMAIL, OnSelchangecMAIL)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3822 message handlers

BOOL CBO_LOC_3822::Create(CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3822::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids;

	ids = InitESL_Mgr(_T("SM_BO_LOC_3822"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitESL_Mgr") );
	
	ids = InitGroupCombo();
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitGroupCombo") );

	ids = InitMailCombo();
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitMailCombo") );

	// 2010.09.07 ADD BY KSJ : 통합도서서비스 사용 할 경우 주민번호 컨트롤 Setting
	if(_T("Y")==m_sIsUnityLoanService)
	{
		CRect rCivilNoStatic, rCivilNoEdit;
		CWnd *pStaCivilNo = GetDlgItem(IDC_staCivilNo);
		pStaCivilNo->GetWindowRect(&rCivilNoStatic);
		ScreenToClient(&rCivilNoStatic);
		rCivilNoStatic.right += 35;
		pStaCivilNo->MoveWindow(rCivilNoStatic);
		pStaCivilNo->SetWindowText(_T("주민번호 앞자리"));

		CEdit *pEditCivilNo = (CEdit*)GetDlgItem(IDC_eCIVILNUM_FROM);
		pEditCivilNo->SetLimitText(6);
		pEditCivilNo->ModifyStyle(NULL,ES_NUMBER);
		pEditCivilNo->GetWindowRect(&rCivilNoEdit);
		ScreenToClient(&rCivilNoEdit);
		rCivilNoEdit.left += 30;
		pEditCivilNo->MoveWindow(rCivilNoEdit);

		((CEdit*)GetDlgItem(IDC_eCIVILNUM_TO))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_staCivilNoTilde))->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3822::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

INT CBO_LOC_3822::InitGroupCombo()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3822_GROUP") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3822::InitGroupCombo() := FindDM()") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3822"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3822::InitGroupCombo() := FindCM()") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("그룹")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("CBO_LOC_3822::InitGroupCombo() := FindControl()") );

	CString sWhere=_T("");
	//if(m_nMailClass == 17){//비회원메일
	//	sWhere.Format(_T(" LOAN_USER_YN = 'N' "));
	//}else{
	//	sWhere.Format(_T(" LOAN_USER_YN <> 'N' "));
	//}
	pDM->FreeData();

	ids = pDM->RefreshDataManager(sWhere);
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3822::InitGroupCombo() := RefreshDataManager()") );

	CString sInsertCode[2];
	
	//CString stemp;
	//stemp.Format(_T("[%d][%d]"), pDM->GetRowCount() , pCMCB->GetRowCount());
	//AfxMessageBox(stemp);
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3822::InitGroupCombo() := SetData()") );

	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("이름") , i , sInsertCode[1] );

		sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
		if( sInsertCode[0].IsEmpty() ) continue;
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitControl") );
	}

	pCMCB->SetCurSel(0);

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3822::InitMailCombo()
{
	EFS_BEGIN

	INT i=0, ids=0, cnt=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3822") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3822::InitMailCombo() := FindDM()") );

	ids = pDM->RefreshDataManager(_T(""));
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3822::InitMailCombo() := RefreshDataManager()") );

	cnt = pDM->GetRowCount();

	if(cnt > 0)
	{
		for(i=0;i<cnt;i++)
		{
			// 2009.10.21 UPDATE BY KSJ : 탈퇴처리안내 제외
			CString sMailAlias = pDM->GetCellData(_T("메일종류"),i);
			if( _T("정기반납독촉") != sMailAlias &&
				_T("탈퇴처리안내") != sMailAlias )
			{
				m_cMail.AddString(pDM->GetCellData(_T("메일종류"), i));
			}
		}
	}

	m_cMail.SetCurSel(0);
	OnSelchangecMAIL();

	return 0;

	EFS_END
	return -1;
}

VOID CBO_LOC_3822::OnSelchangecMAIL() 
{
	EFS_BEGIN

	INT ids = 0, idx=0, i=0;
	CString sTemp;

	idx = m_cMail.GetCurSel();

	m_cMail.GetLBText(idx, m_sMailClass);

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3822") );

	idx = (-1);
	for(i=0;i<pDM->GetRowCount();i++){
		GetDataMgrData(_T("DM_BO_LOC_3822"), _T("메일종류"), sTemp, i );
		if(m_sMailClass == sTemp){
			idx = i;
			break;
		}
	}
	
	if(idx < 0){
		AfxMessageBox(_T("메일종류를 찾을 수 없습니다."));
	}

	GetDataMgrData(_T("DM_BO_LOC_3822"), _T("메일클래스"), sTemp, idx );
	m_nMailClass = (INT)_ttoi(sTemp.GetBuffer(0));

	GetDataMgrData(_T("DM_BO_LOC_3822"), _T("메일KEY"), sTemp, idx );
	m_nMailKey = (INT)_ttoi(sTemp.GetBuffer(0)); 

	m_pCBOLOC3820->ChangeGridMode(m_sMailClass, m_nMailClass, m_nMailKey);

	ids = ShowControl();

	FreeAllDM();

	// 2009.05.15 DELETE BY KSJ : 그리드 갱신 필요 없음
	//DEL ids = m_pCBOLOC3820->DisplayGrid();

	//ids = InitGroupCombo();

	return;
	EFS_END
	return;
}

INT CBO_LOC_3822::ShowControl()
{
	EFS_BEGIN

	m_pCBOLOC3820->nDirectCount = 0;
	m_pCBOLOC3820->sDirectQuery.Empty();

	GetDlgItem(IDC_cGROUP)->EnableWindow( TRUE );
	GetDlgItem(IDC_cPOSITION)->EnableWindow( TRUE );
	GetDlgItem(IDC_cRANK)->EnableWindow( TRUE );
	GetDlgItem(IDC_cINFERIOR)->EnableWindow( TRUE );
	GetDlgItem(IDC_eLOAN_NUM_FROM)->EnableWindow( TRUE );
	GetDlgItem(IDC_eLOAN_NUM_TO)->EnableWindow( TRUE );
	GetDlgItem(IDC_eUSER_NAME_FROM)->EnableWindow( TRUE );
	GetDlgItem(IDC_eUSER_NAME_TO)->EnableWindow( TRUE );
	GetDlgItem(IDC_eCIVILNUM_FROM)->EnableWindow( TRUE );
	GetDlgItem(IDC_eCIVILNUM_TO)->EnableWindow( TRUE );

	GetDlgItem(IDC_dRESERVATION_DATE_FROM)->EnableWindow( FALSE );
	GetDlgItem(IDC_dRESERVATION_DATE_TO)->EnableWindow( FALSE );
	GetDlgItem(IDC_dRESERVE_CANCEL_DATE_FROM)->EnableWindow( FALSE );
	GetDlgItem(IDC_dRESERVE_CANCEL_DATE_TO)->EnableWindow( FALSE );
	GetDlgItem(IDC_dRESERVATION_EXPIRE_DATE_FROM)->EnableWindow( FALSE );
	GetDlgItem(IDC_dRESERVATION_EXPIRE_DATE_TO)->EnableWindow( FALSE );
	GetDlgItem(IDC_dLOAN_DATE_FROM)->EnableWindow( FALSE );
	GetDlgItem(IDC_dLOAN_DATE_TO)->EnableWindow( FALSE );
	GetDlgItem(IDC_dRETURN_PLAN_DATE_FROM)->EnableWindow( FALSE );
	GetDlgItem(IDC_dRETURN_PLAN_DATE_TO)->EnableWindow( FALSE );
	GetDlgItem(IDC_dRETURN_DATE_FROM)->EnableWindow( FALSE );
	GetDlgItem(IDC_dRETURN_DATE_TO)->EnableWindow( FALSE );

	GetDlgItem(IDC_cMAT_CODE)->EnableWindow( FALSE );
	GetDlgItem(IDC_cMANAGE_CODE)->EnableWindow( FALSE );
	GetDlgItem(IDC_cMEDIA_CODE)->EnableWindow( FALSE );
	GetDlgItem(IDC_eTITLE)->EnableWindow( FALSE );
	GetDlgItem(IDC_eAUTHOR)->EnableWindow( FALSE );
	GetDlgItem(IDC_dSHELF_DATE_FROM)->EnableWindow( FALSE );
	GetDlgItem(IDC_dSHELF_DATE_TO)->EnableWindow( FALSE );

	GetDlgItem(IDC_dREG_DATE_FROM)->EnableWindow( FALSE );
	GetDlgItem(IDC_dREG_DATE_TO)->EnableWindow( FALSE );
	GetDlgItem(IDC_cRetry)->EnableWindow( FALSE );

	if(m_sMailClass.Compare(_T("예약안내(반납시)"))==0 || m_sMailClass.Compare(_T("예약안내(비치자료예약)"))==0){
		GetDlgItem(IDC_dRESERVATION_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRESERVATION_DATE_TO)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRESERVATION_EXPIRE_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRESERVATION_EXPIRE_DATE_TO)->EnableWindow( TRUE );
	}else if(m_sMailClass.Compare(_T("반납독촉"))==0 || m_sMailClass.Compare(_T("반납연기"))==0 || m_sMailClass.Compare(_T("반납예정안내"))==0 || m_sMailClass.Compare(_T("정기반납독촉"))==0){
		GetDlgItem(IDC_dLOAN_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dLOAN_DATE_TO)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRETURN_PLAN_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRETURN_PLAN_DATE_TO)->EnableWindow( TRUE );
	}else if(m_sMailClass.Compare(_T("대출안내"))==0){
		GetDlgItem(IDC_dLOAN_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dLOAN_DATE_TO)->EnableWindow( TRUE );
	}else if(m_sMailClass.Compare(_T("반납확인(연체안된자료)"))==0){
		GetDlgItem(IDC_dRETURN_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRETURN_DATE_TO)->EnableWindow( TRUE );
	}else if(m_sMailClass.Compare(_T("반납확인(연체된자료)"))==0){
		GetDlgItem(IDC_dRETURN_PLAN_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRETURN_PLAN_DATE_TO)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRETURN_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRETURN_DATE_TO)->EnableWindow( TRUE );
	}else if(m_sMailClass.Compare(_T("예약취소"))==0){
		GetDlgItem(IDC_dRESERVATION_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRESERVATION_DATE_TO)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRESERVE_CANCEL_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dRESERVE_CANCEL_DATE_TO)->EnableWindow( TRUE );
	}else if(m_sMailClass.Compare(_T("신규회원안내"))==0){
		GetDlgItem(IDC_dREG_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dREG_DATE_TO)->EnableWindow( TRUE );
		GetDlgItem(IDC_cRetry)->EnableWindow( TRUE );
	}else if(m_sMailClass.Compare(_T("신착자료안내"))==0){
		GetDlgItem(IDC_cMAT_CODE)->EnableWindow( TRUE );
		GetDlgItem(IDC_cMANAGE_CODE)->EnableWindow( TRUE );
		GetDlgItem(IDC_cMEDIA_CODE)->EnableWindow( TRUE );
		GetDlgItem(IDC_eTITLE)->EnableWindow( TRUE );
		GetDlgItem(IDC_eAUTHOR)->EnableWindow( TRUE );
		GetDlgItem(IDC_dSHELF_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dSHELF_DATE_TO)->EnableWindow( TRUE );
		// 2009.10.21 UPDATE BY KSJ : Alias 명 변경
	//DEL }else if(m_sMailClass.Compare(_T("비치희망안내"))==0){
		}else if(  m_sMailClass.Compare(_T("비치희망예약안내"))==0 
				|| m_sMailClass.Compare(_T("비치희망안내(배가시)"))==0  ){
		GetDlgItem(IDC_cGROUP)->EnableWindow( FALSE );
		GetDlgItem(IDC_cPOSITION)->EnableWindow( FALSE );
		GetDlgItem(IDC_cRANK)->EnableWindow( FALSE );
		GetDlgItem(IDC_cINFERIOR)->EnableWindow( FALSE );
		GetDlgItem(IDC_eLOAN_NUM_FROM)->EnableWindow( FALSE );
		GetDlgItem(IDC_eLOAN_NUM_TO)->EnableWindow( FALSE );
		GetDlgItem(IDC_eTITLE)->EnableWindow( TRUE );
		GetDlgItem(IDC_eAUTHOR)->EnableWindow( TRUE );
		GetDlgItem(IDC_dSHELF_DATE_FROM)->EnableWindow( TRUE );
		GetDlgItem(IDC_dSHELF_DATE_TO)->EnableWindow( TRUE );
	}else if(m_sMailClass.Compare(_T("비회원메일"))==0){
		GetDlgItem(IDC_cPOSITION)->EnableWindow( FALSE );
		GetDlgItem(IDC_cRANK)->EnableWindow( FALSE );
		GetDlgItem(IDC_cINFERIOR)->EnableWindow( FALSE );
		GetDlgItem(IDC_eLOAN_NUM_FROM)->EnableWindow( FALSE );
		GetDlgItem(IDC_eLOAN_NUM_TO)->EnableWindow( FALSE );
	}else if(m_sMailClass.Compare(_T("생일축하메일"))==0){
	}else if(m_sMailClass.Compare(_T("공지사항"))==0){
	}else{
	}

	return 0;
	EFS_END
	return -1;
}

VOID CBO_LOC_3822::OnOK() 
{
	EFS_BEGIN

	INT ids = 0;

	ids = SearchUser();
	if(ids<0) AfxMessageBox(_T("ERROR CBO_LOC_3822::OnOK() := SearchUser()"));

	//CDialog::OnOK();

	return;
	EFS_END
	return;
}

INT CBO_LOC_3822::MakeQueryUser(CString &query)
{
	EFS_BEGIN

	INT ids = 0;
	CString sTemp, sTemp2;
	CString sQueryTemp;
	CEdit *pEdt = NULL;

	query.Empty();

	// 2009.10.21 ADD BY KSJ : 신규회원 검색시 자관도서관의 신규회원만 검색
	query += _T(" AND U.MANAGE_CODE=UDF_MANAGE_CODE ");

	// 1.0 그룹
	ids = GetControlData( _T("CM_BO_LOC_3822"), _T("그룹"), sTemp, -1, 0 );
	if(sTemp.Compare(_T("0")) && sTemp.GetLength() > 0){
		sQueryTemp.Format(_T(" AND ( SELECT COUNT(*) FROM MN_LOAN_USER_GROUP_LIST_TBL MLUGLT WHERE MLUGLT.USER_KEY = U.REC_KEY AND MLUGLT.GROUP_CODE='%s' ) > 0"), sTemp);
		query += sQueryTemp;
	}

	// 1.1 소속
	ids = GetControlData( _T("CM_BO_LOC_3822"), _T("소속"), sTemp, -1, 0 );
	if(sTemp.Compare(_T("0")) && sTemp.GetLength() > 0){
		sQueryTemp.Format(_T(" AND U.USER_POSITION_CODE='%s'"), sTemp);
		query += sQueryTemp;
	}

	// 1.2 직급
	ids = GetControlData( _T("CM_BO_LOC_3822"), _T("직급"), sTemp, -1, 0 );
	if(sTemp.Compare(_T("0")) && sTemp.GetLength() > 0){
		sQueryTemp.Format(_T(" AND U.USER_CLASS_CODE='%s'"), sTemp);
		query += sQueryTemp;
	}

	// 1.3 회원상태
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cINFERIOR );
	if( pCombo->GetCurSel() ){
		sQueryTemp.Format(_T(" AND U.USER_CLASS='%d'"), pCombo->GetCurSel() - 1);
		query += sQueryTemp;
	}


	// 1.4 대출자번호
	pEdt = ( CEdit * )GetDlgItem( IDC_eLOAN_NUM_FROM );
	pEdt->GetWindowText( sTemp );
	pEdt = ( CEdit * )GetDlgItem( IDC_eLOAN_NUM_TO );
	pEdt->GetWindowText( sTemp2 );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND U.USER_NO = '%s'"), sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND U.USER_NO BETWEEN '%s' AND '%s'"), sTemp, sTemp2 );
		query += sQueryTemp;
	}

	// 1.5 대출자명
	pEdt = ( CEdit * )GetDlgItem( IDC_eUSER_NAME_FROM );
	pEdt->GetWindowText( sTemp );
	pEdt = ( CEdit * )GetDlgItem( IDC_eUSER_NAME_TO );
	pEdt->GetWindowText( sTemp2 );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND U.NAME = '%s'"), sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND U.NAME BETWEEN '%s' AND '%s'"), sTemp, sTemp2 );
		query += sQueryTemp;
	}

	// 1.6 주민등록번호
	pEdt = ( CEdit * )GetDlgItem( IDC_eCIVILNUM_FROM );
	pEdt->GetWindowText( sTemp );
	pEdt = ( CEdit * )GetDlgItem( IDC_eCIVILNUM_TO );
	pEdt->GetWindowText( sTemp2 );

	// 2010.09.07 ADD BY KSJ : 통합도서서비스 사용 할 경우 검색 방식 변경
	if(_T("Y")==m_sIsUnityLoanService)
	{
		if( !sTemp.IsEmpty() )
		{
			sQueryTemp.Format( _T(" AND U.CIVIL_NO LIKE '%s%%'"), sTemp );
			query += sQueryTemp;
		}
	}
	else
	{
		if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
		{
			sQueryTemp.Format( _T(" AND U.CIVIL_NO = '%s'"), sTemp );
			query += sQueryTemp;
		}
		else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
		{
			sQueryTemp.Format( _T(" AND U.CIVIL_NO BETWEEN '%s' AND '%s'"), sTemp, sTemp2 );
			query += sQueryTemp;
		}
	}

	// 3.1 가입일
	sTemp  = GetCtrlDate( IDC_dREG_DATE_FROM );
	sTemp2 = GetCtrlDate( IDC_dREG_DATE_TO );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND U.REG_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND U.REG_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp2 );
		query += sQueryTemp;
	}
	
	// 3.2 재발송허용여부
	CButton* pCheck;
	pCheck = (CButton *)GetDlgItem(IDC_cRetry);
	if(pCheck->GetCheck() != 1 && pCheck->IsWindowEnabled()){
		sQueryTemp.Format( _T(" AND U.NEW_USER_MAIL_DATE IS NULL") );
		query += sQueryTemp;
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3822::MakeQueryLoan(CString &query)
{
	EFS_BEGIN

	INT ids = 0;
	CString sTemp, sTemp2;
	CString sQueryTemp;
	CEdit *pEdt = NULL;

	query.Empty();

	// 2.1 예약일
	sTemp  = GetCtrlDate( IDC_dRESERVATION_DATE_FROM );
	sTemp2 = GetCtrlDate( IDC_dRESERVATION_DATE_TO );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RESERVATION_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RESERVATION_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp2 );
		query += sQueryTemp;
	}
	
	// 2.2 예약취소일
	sTemp  = GetCtrlDate( IDC_dRESERVE_CANCEL_DATE_FROM );
	sTemp2 = GetCtrlDate( IDC_dRESERVE_CANCEL_DATE_TO );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RESERVE_CANCEL_DATEBETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RESERVE_CANCEL_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp2 );
		query += sQueryTemp;
	}

	// 2.3 예약만기일
	sTemp  = GetCtrlDate( IDC_dRESERVATION_EXPIRE_DATE_FROM );
	sTemp2 = GetCtrlDate( IDC_dRESERVATION_EXPIRE_DATE_TO );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RESERVATION_EXPIRE_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RESERVATION_EXPIRE_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp2 );
		query += sQueryTemp;
	}

	// 2.4 대출일
	sTemp  = GetCtrlDate( IDC_dLOAN_DATE_FROM );
	sTemp2 = GetCtrlDate( IDC_dLOAN_DATE_TO );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp2 );
		query += sQueryTemp;
	}

	// 2.5 반납예정일
	sTemp  = GetCtrlDate( IDC_dRETURN_PLAN_DATE_FROM );
	sTemp2 = GetCtrlDate( IDC_dRETURN_PLAN_DATE_TO );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RETURN_PLAN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RETURN_PLAN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp2 );
		query += sQueryTemp;
	}

	// 2.6 반납일
	sTemp  = GetCtrlDate( IDC_dRETURN_DATE_FROM );
	sTemp2 = GetCtrlDate( IDC_dRETURN_DATE_TO );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND L.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp2 );
		query += sQueryTemp;
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3822::MakeQueryBook(CString &query)
{
	EFS_BEGIN

	INT ids = 0;
	CString sTemp, sTemp2;
	CString sQueryTemp;
	CEdit *pEdt = NULL;

	query.Empty();
	
	// 3.1 자료구분
	ids = GetControlData( _T("CM_BO_LOC_3822"), _T("자료구분"), sTemp, -1, 0 );
	if(sTemp.Compare(_T("0")) && sTemp.GetLength() > 0 && GetDlgItem(IDC_cMAT_CODE)->IsWindowEnabled()){
		sQueryTemp.Format(_T(" AND I.MAT_CODE='%s'"), sTemp);
		query += sQueryTemp;
	}

	// 3.2 관리구분
	ids = GetControlData( _T("CM_BO_LOC_3822"), _T("관리구분"), sTemp, -1, 0 );
	if(sTemp.Compare(_T("0")) && sTemp.GetLength() > 0 && GetDlgItem(IDC_cMANAGE_CODE)->IsWindowEnabled()){
		sQueryTemp.Format(_T(" AND B.MANAGE_CODE='%s'"), sTemp);
		query += sQueryTemp;
	}

	// 3.3 매체구분
	ids = GetControlData( _T("CM_BO_LOC_3822"), _T("매체구분"), sTemp, -1, 0 );
	if(sTemp.Compare(_T("0")) && sTemp.GetLength() > 0 && GetDlgItem(IDC_cMEDIA_CODE)->IsWindowEnabled()){
		sQueryTemp.Format(_T(" AND B.MEDIA_CODE='%s'"), sTemp);
		query += sQueryTemp;
	}

	// 3.4 서명
	pEdt = ( CEdit * )GetDlgItem( IDC_eTITLE );
	pEdt->GetWindowText( sTemp );
	if( !sTemp.IsEmpty() )
	{
		if( m_pInfo->GetSearchIndexType() == 1 ){
			sQueryTemp.Format( _T(" AND &ECOSEARCH( I.REC_KEY, BO_IDX_TITLE, '%s*' )"), sTemp );
			query += sQueryTemp;
		}else{
			sQueryTemp.Format( _T(" AND CATSEARCH( I.IDX_TITLE, '%s*', NULL ) > 0"), sTemp );
			query += sQueryTemp;
		}		
	}

	// 3.5 저작자
	pEdt = ( CEdit * )GetDlgItem( IDC_eAUTHOR );
	pEdt->GetWindowText( sTemp );
	if( !sTemp.IsEmpty() )
	{
		if( m_pInfo->GetSearchIndexType() == 1 ){
			sQueryTemp.Format( _T(" AND &ECOSEARCH( I.REC_KEY, BO_IDX_AUTHOR, '%s*' )"), sTemp );
			query += sQueryTemp;
		}else{
			sQueryTemp.Format( _T(" AND CATSEARCH( I.IDX_AUTHOR, '%s*', NULL ) > 0"), sTemp );
			query += sQueryTemp;
		}		
	}

	// 3.6 비치일자
	sTemp  = GetCtrlDate( IDC_dSHELF_DATE_FROM );
	sTemp2 = GetCtrlDate( IDC_dSHELF_DATE_TO );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND B.SHELF_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND B.SHELF_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp2 );
		query += sQueryTemp;
	}


	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3822::MakeQueryFurnish(CString &query)
{
	EFS_BEGIN

	INT ids = 0;
	CString sTemp, sTemp2;
	CString sQueryTemp;
	CEdit *pEdt = NULL;

	query.Empty();
	
	// 1.5 대출자명
	pEdt = ( CEdit * )GetDlgItem( IDC_eUSER_NAME_FROM );
	pEdt->GetWindowText( sTemp );
	pEdt = ( CEdit * )GetDlgItem( IDC_eUSER_NAME_TO );
	pEdt->GetWindowText( sTemp2 );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND F.APPLICANT_NAME = '%s'"), sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND F.APPLICANT_NAME BETWEEN '%s' AND '%s'"), sTemp, sTemp2 );
		query += sQueryTemp;
	}

	// 1.6 주민등록번호
	pEdt = ( CEdit * )GetDlgItem( IDC_eCIVILNUM_FROM );
	pEdt->GetWindowText( sTemp );
	pEdt = ( CEdit * )GetDlgItem( IDC_eCIVILNUM_TO );
	pEdt->GetWindowText( sTemp2 );

	// 2010.09.07 ADD BY KSJ : 통합도서서비스 사용 할 경우 검색 방식 변경
	if(_T("Y")==m_sIsUnityLoanService)
	{
		if( !sTemp.IsEmpty() )
		{
			sQueryTemp.Format( _T(" AND F.CIVIL_NO LIKE '%s%%'"), sTemp );
			query += sQueryTemp;
		}
	}
	else
	{
		if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
		{
			sQueryTemp.Format( _T(" AND F.CIVIL_NO = '%s'"), sTemp );
			query += sQueryTemp;
		}
		else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
		{
			sQueryTemp.Format( _T(" AND F.CIVIL_NO BETWEEN '%s' AND '%s'"), sTemp, sTemp2 );
			query += sQueryTemp;
		}
	}

	// 3.4 서명
	pEdt = ( CEdit * )GetDlgItem( IDC_eTITLE );
	pEdt->GetWindowText( sTemp );
	if( !sTemp.IsEmpty() )
	{
		if( m_pInfo->GetSearchIndexType() == 1 ){
			sQueryTemp.Format( _T(" AND &ECOSEARCH( I.REC_KEY, BO_IDX_TITLE, '%s*' )"), sTemp );
			query += sQueryTemp;
		}else{
			sQueryTemp.Format( _T(" AND CATSEARCH( I.IDX_TITLE, '%s*', NULL ) > 0"), sTemp );
			query += sQueryTemp;
		}		
	}

	// 3.5 저작자
	pEdt = ( CEdit * )GetDlgItem( IDC_eAUTHOR );
	pEdt->GetWindowText( sTemp );
	if( !sTemp.IsEmpty() )
	{
		if( m_pInfo->GetSearchIndexType() == 1 ){
			sQueryTemp.Format( _T(" AND &ECOSEARCH( I.REC_KEY, BO_IDX_AUTHOR, '%s*' )"), sTemp );
			query += sQueryTemp;
		}else{
			sQueryTemp.Format( _T(" AND CATSEARCH( I.IDX_AUTHOR, '%s*', NULL ) > 0"), sTemp );
			query += sQueryTemp;
		}		
	}

	// 3.6 비치일자
	sTemp  = GetCtrlDate( IDC_dSHELF_DATE_FROM );
	sTemp2 = GetCtrlDate( IDC_dSHELF_DATE_TO );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND F.FURNISH_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND F.FURNISH_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') and TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sTemp, sTemp2 );
		query += sQueryTemp;
	}


	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3822::MakeQueryNonUser(CString &query)
{
	EFS_BEGIN

	INT ids = 0;
	CString sTemp, sTemp2;
	CString sQueryTemp;
	CEdit *pEdt = NULL;

	query.Empty();
	
	// 1.0 그룹
	ids = GetControlData( _T("CM_BO_LOC_3822"), _T("그룹"), sTemp, -1, 0 );
	sTemp.TrimLeft();
	sTemp.TrimRight();
	if(sTemp.GetLength() > 0){
		sQueryTemp.Format(_T(" AND GROUP_CODE='%s' "), sTemp);
		query += sQueryTemp;
	}


	// 1.5 대출자명
	pEdt = ( CEdit * )GetDlgItem( IDC_eUSER_NAME_FROM );
	pEdt->GetWindowText( sTemp );
	pEdt = ( CEdit * )GetDlgItem( IDC_eUSER_NAME_TO );
	pEdt->GetWindowText( sTemp2 );
	if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND NAME = '%s'"), sTemp );
		query += sQueryTemp;
	}
	else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
	{
		sQueryTemp.Format( _T(" AND NAME BETWEEN '%s' AND '%s'"), sTemp, sTemp2 );
		query += sQueryTemp;
	}

	// 1.6 주민등록번호
	pEdt = ( CEdit * )GetDlgItem( IDC_eCIVILNUM_FROM );
	pEdt->GetWindowText( sTemp );
	pEdt = ( CEdit * )GetDlgItem( IDC_eCIVILNUM_TO );
	pEdt->GetWindowText( sTemp2 );

	// 2010.09.07 ADD BY KSJ : 통합도서서비스 사용 할 경우 검색 방식 변경
	if(_T("Y")==m_sIsUnityLoanService)
	{
		if( !sTemp.IsEmpty() )
		{
			sQueryTemp.Format( _T(" AND CIVIL_NO LIKE '%s%%'"), sTemp );
			query += sQueryTemp;
		}
	}
	else
	{
		if( !sTemp.IsEmpty() && sTemp2.IsEmpty() )
		{
			sQueryTemp.Format( _T(" AND CIVIL_NO = '%s'"), sTemp );
			query += sQueryTemp;
		}
		else if( !sTemp.IsEmpty() && !sTemp2.IsEmpty() )
		{
			sQueryTemp.Format( _T(" AND CIVIL_NO BETWEEN '%s' AND '%s'"), sTemp, sTemp2 );
			query += sQueryTemp;
		}
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3822::SearchUser()
{
	EFS_BEGIN

	INT ids = 0;
	CString sQueryUser;
	CString sQueryLoan;
	CString sQueryBook;
	CString sQueryFurnish;
	CString sQueryNonUser;

	m_pCBOLOC3820->nDirectCount = 0;
	m_pCBOLOC3820->sDirectQuery.Empty();

	if(m_pCBOLOC3820->m_nGridMode == 0){
		ids = MakeQueryUser(sQueryUser);
		ids = MakeQueryLoan(sQueryLoan);
		ids = m_pCBOLOC3820->SearchLoanUser(sQueryUser+sQueryLoan, m_sMailClass);
	}else if(m_pCBOLOC3820->m_nGridMode == 1){
		ids = MakeQueryUser(sQueryUser);
		ids = m_pCBOLOC3820->SearchUser(sQueryUser, m_sMailClass);
	}else if(m_pCBOLOC3820->m_nGridMode == 2){
		ids = MakeQueryUser(sQueryUser);
		ids = MakeQueryBook(sQueryBook);
		ids = m_pCBOLOC3820->SearchUserNBook(sQueryUser, sQueryBook, m_sMailClass);
	}else if(m_pCBOLOC3820->m_nGridMode == 3){
		ids = MakeQueryFurnish(sQueryFurnish);
		ids = m_pCBOLOC3820->SearchFurnish(sQueryFurnish, m_sMailClass);
	}else if(m_pCBOLOC3820->m_nGridMode == 4){
		ids = MakeQueryUser(sQueryUser);
		ids = m_pCBOLOC3820->SearchUserNPoll(sQueryUser, m_sMailClass);
	}else if(m_pCBOLOC3820->m_nGridMode == 5){
		ids = MakeQueryNonUser(sQueryNonUser);
		ids = m_pCBOLOC3820->SearchNonUser(sQueryNonUser, m_sMailClass);
	}
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3822::SearchUser()") );


	return 0;
	EFS_END
	return -1;
}



CString CBO_LOC_3822::GetCtrlDate(UINT id)
{
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( id );
	if(!pDate->IsWindowEnabled()) return _T("");
	CTime time;
	if( GDT_VALID != pDate->GetTime( time ) )
		return _T("");

	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	return date;
}


BOOL CBO_LOC_3822::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnOK();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_LOC_3822::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// 2009.05.15 DELETE BY KSJ : 그리드 갱신 필요 없음
	//DEL OnSelchangecMAIL();	
}

INT CBO_LOC_3822::FreeAllDM()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3820") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3822::ViewInitGrid() := FindDM()") );

	CESL_DataMgr *pDM_USER = FindDM( _T("DM_BO_LOC_3820_USER") );
	if(pDM_USER==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3822::ViewInitGrid() := FindDM()") );

	CESL_DataMgr *pDM_BO = FindDM( _T("DM_BO_LOC_3820_BO_BOOK") );
	if(pDM_BO==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3822::ViewInitGrid() := FindDM()") );

	CESL_DataMgr *pDM_SE = FindDM( _T("DM_BO_LOC_3820_SE_BOOK") );
	if(pDM_SE==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3822::ViewInitGrid() := FindDM()") );

	CESL_DataMgr *pDM_FURNISH = FindDM( _T("DM_BO_LOC_3820_FURNISH") );
	if(pDM_FURNISH==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3822::ViewInitGrid() := FindDM()") );

	CESL_DataMgr *pDM_POLL = FindDM( _T("DM_BO_LOC_3820_POLL") );
	if(pDM_POLL==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3822::ViewInitGrid() := FindDM()") );

	CESL_DataMgr *pDM_NONUSER = FindDM( _T("DM_BO_LOC_3820_NONUSER") );
	if(pDM_NONUSER==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3822::ViewInitGrid() := FindDM()") );

	pDM->FreeData();
	pDM_USER->FreeData();
	pDM_BO->FreeData();
	pDM_SE->FreeData();
	pDM_FURNISH->FreeData();
	pDM_POLL->FreeData();
	pDM_NONUSER->FreeData();

	return 0;
	EFS_END
	return -1;
}

HBRUSH CBO_LOC_3822::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
