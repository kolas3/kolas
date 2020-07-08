// BO_LOC_3872.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3872.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3872 dialog


CBO_LOC_3872::CBO_LOC_3872(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3872)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3872::~CBO_LOC_3872()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3872::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3872)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3872, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3872)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3872 message handlers

BOOL CBO_LOC_3872::Create( CWnd* pParentWnd ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3872::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("SM_BO_LOC_3872") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	InitGroupCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

BOOL CBO_LOC_3872::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_LOC_3872::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

VOID CBO_LOC_3872::OnbCLOSE() 
{
EFS_BEGIN
	OnCancel();
EFS_END
}

VOID CBO_LOC_3872::OnbSEARCH() 
{
EFS_BEGIN
	SearchProc();
EFS_END	
}

INT CBO_LOC_3872::SearchProc()
{
EFS_BEGIN
	
	INT ids = 0;

	CString sName,sName2;
	CString sCivilno,sCivilno2;
	CString sHandphone, sHandphone2;
	CString sEmail, sEmail2;
	CString sGroup;

	CString sCMAlias = _T("CM_BO_LOC_3872");
	// 이름
	ids = GetControlData( sCMAlias , _T("이름") , sName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchProc") );
	ids = GetControlData( sCMAlias , _T("이름2") , sName2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SearchProc") );

	// 주민등록번호
	ids = GetControlData( sCMAlias , _T("주민등록번호") , sCivilno );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SearchProc") );
	ids = GetControlData( sCMAlias , _T("주민등록번호2") , sCivilno2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("SearchProc") );

	// 핸드폰
	ids = GetControlData( sCMAlias , _T("핸드폰") , sHandphone);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("SearchProc") );
	ids = GetControlData( sCMAlias , _T("핸드폰2") , sHandphone2);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("SearchProc") );

	// 이메일
	ids = GetControlData( sCMAlias , _T("이메일") , sEmail );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("SearchProc") );
	ids = GetControlData( sCMAlias , _T("이메일2") , sEmail2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("SearchProc") );

	// 그룹코드
	ids = GetControlData( sCMAlias , _T("그룹코드") , sGroup );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("SearchProc") );

	CString sWhere = _T(" 1>0");
	CString sTemp;

	// 이름
	sTemp.Empty();
	if(!sName.IsEmpty() && !sName2.IsEmpty()){
		sTemp.Format(_T(" AND NAME between '%s' and '%s'"), sName.GetBuffer(0), sName2.GetBuffer(0));
	}else if(!sName.IsEmpty() && sName2.IsEmpty()){
		sTemp.Format(_T(" AND NAME = '%s'"), sName.GetBuffer(0));
	}else if(sName.IsEmpty() && !sName2.IsEmpty()){
		sTemp.Format(_T(" AND NAME = '%s'"), sName2.GetBuffer(0));
	}
	sWhere += sTemp;

	//주민등록번호
	sTemp.Empty();
	if(!sCivilno.IsEmpty() && !sCivilno2.IsEmpty()){
		sTemp.Format(_T(" AND CIVIL_NO between '%s' and '%s'"), sCivilno.GetBuffer(0), sCivilno2.GetBuffer(0));
	}else if(!sCivilno.IsEmpty() && sCivilno2.IsEmpty()){
		sTemp.Format(_T(" AND CIVIL_NO = '%s'"), sCivilno.GetBuffer(0));
	}else if(sCivilno.IsEmpty() && !sCivilno2.IsEmpty()){
		sTemp.Format(_T(" AND CIVIL_NO = '%s'"), sCivilno2.GetBuffer(0));
	}
	sWhere += sTemp;

	//핸드폰
	sTemp.Empty();
	if(!sHandphone.IsEmpty() && !sHandphone2.IsEmpty()){
		sTemp.Format(_T(" AND HANDPHONE between '%s' and '%s'"), sHandphone.GetBuffer(0), sHandphone2.GetBuffer(0));
	}else if(!sHandphone.IsEmpty() && sHandphone2.IsEmpty()){
		sTemp.Format(_T(" AND HANDPHONE = '%s'"), sHandphone.GetBuffer(0));
	}else if(sHandphone.IsEmpty() && !sHandphone2.IsEmpty()){
		sTemp.Format(_T(" AND HANDPHONE = '%s'"), sHandphone2.GetBuffer(0));
	}
	sWhere += sTemp;

	//이메일
	sTemp.Empty();
	if(!sEmail.IsEmpty() && !sEmail2.IsEmpty()){
		sTemp.Format(_T(" AND E_MAIL between '%s' and '%s'"), sEmail.GetBuffer(0), sEmail2.GetBuffer(0));
	}else if(!sEmail.IsEmpty() && sEmail2.IsEmpty()){
		sTemp.Format(_T(" AND E_MAIL = '%s'"), sEmail.GetBuffer(0));
	}else if(sEmail.IsEmpty() && !sEmail2.IsEmpty()){
		sTemp.Format(_T(" AND E_MAIL = '%s'"), sEmail2.GetBuffer(0));
	}
	sWhere += sTemp;

	//그룹코드
	sTemp.Empty();
	if(!sGroup.IsEmpty()){
		sTemp.Format(_T(" AND GROUP_CODE = '%s'"), sGroup.GetBuffer(0));
	}
	sWhere += sTemp;


	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3870") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("SearchProc") );

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("SearchProc") ); 

	ids = AllControlDisplay( _T("CM_BO_LOC_3870") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -33 , _T("SearchProc") ); 

	return 0;
EFS_END
	return -1;
}

INT CBO_LOC_3872::InitGroupCombo()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3870_GROUP") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitGroupCombo()") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3872"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitGroupCombo()") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("그룹코드")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitGroupCombo()") );

	ids = pDM->RefreshDataManager(_T(""));
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitGroupCombo()") );


	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3872::InitGroupCombo() := SetData()") );

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

HBRUSH CBO_LOC_3872::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
