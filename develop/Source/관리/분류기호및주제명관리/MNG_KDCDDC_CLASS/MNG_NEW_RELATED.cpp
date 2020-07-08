// MNG_NEW_RELATED.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_NEW_RELATED.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_RELATED dialog


CMNG_NEW_RELATED::CMNG_NEW_RELATED(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_NEW_RELATED)
	//m_TITLE = _T("");
	//}}AFX_DATA_INIT
}

CMNG_NEW_RELATED::~CMNG_NEW_RELATED()
{
	
}


BOOL CMNG_NEW_RELATED::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}
VOID CMNG_NEW_RELATED::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_NEW_RELATED)
	DDX_Text(pDX, IDC_RELATED_TITLE, m_TITLE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_NEW_RELATED, CDialog)
	//{{AFX_MSG_MAP(CMNG_NEW_RELATED)
	ON_BN_CLICKED(IDC_SAVE_CODE, OnSaveCode)
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_RELATED message handlers

BOOL CMNG_NEW_RELATED::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetCode();

	if (InitESL_Mgr(_T("SM_관리_분류기호_주제명_관련어")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	
	if(OpenMode == 1)	// 수정
	{
		CESL_DataMgr *pDM;
		pDM = FindDM(_T("DM_관리_분류기호_주제명_관련어"));
		if ( NULL == pDM ) 
		{
			AfxMessageBox(_T("DM 초기화 에러"));
			return FALSE;
		}

		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_관리_분류기호_주제명_관련어_입력"));
		if ( NULL == pCM ) 
		{
			AfxMessageBox(_T("CM 초기화 에러"));
			return FALSE;
		}

		

		INT n_idx = -1;
		SelectMakeList(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));
		n_idx = SelectGetHeadPosition(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));
		if ( n_idx < 0 )
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));
			return FALSE;
		}
//		n_idx = SelectGetNext(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));

 		CString strTitle, strCode, strSubjectKey, strRecKey;

		strRecKey = pDM->GetCellData(_T("REC_KEY"), n_idx);
		strTitle = pDM->GetCellData(_T("관련어"), n_idx);
		strCode = pDM->GetCellData(_T("관련어코드"), n_idx);
		strSubjectKey = pDM->GetCellData(_T("주제명키"), n_idx);
		

		AllControlDisplay(_T("CM_관리_분류기호_주제명_관련어_입력"), n_idx);
				
		m_pMCombo->SelectString( strCode );
	//	m_ctrCODE.SelectString( 0,m_strCode);
	}
	else	// 입력
	{

		

		//m_ctrCODE.SelectString( 0,_T("SN"));
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_NEW_RELATED::OnSaveCode() 
{
EFS_BEGIN	
	
	if(OpenMode == 1)
		Update_Related();
	else
		Insert_Related();

	CDialog::OnOK();

EFS_END
}

INT CMNG_NEW_RELATED::SetCode()
{
EFS_BEGIN
	m_pMCombo = new CJComboBox;

	m_pMCombo->SubclassDlgItem( IDC_RELATED_CODE, this );
	m_pMCombo->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMCombo->RequireFlag = FALSE;

	m_pMCombo->AddItem( _T("SN"), 0, 0 );
	m_pMCombo->AddItem( _T("USE"), 0, 1 );
	m_pMCombo->AddItem( _T("UF"), 0, 2 );
	m_pMCombo->AddItem( _T("TT"), 0, 3 );
	m_pMCombo->AddItem( _T("BT"), 0, 4 );
	m_pMCombo->AddItem( _T("NT"), 0, 5 );
	m_pMCombo->AddItem( _T("RT"), 0, 6 );
	m_pMCombo->AddItem( _T("SA"), 0, 7 );

	m_pMCombo->SetCurSel( 0 );

	return 0;
EFS_END
	return -1;
}
VOID CMNG_NEW_RELATED::Insert_Related()
{
EFS_BEGIN
	UpdateData(TRUE);

	INT ids =-1;

	CESL_DataMgr *pDM = FindDM(_T("DM_관리_분류기호_주제명_관련어"));
	if ( NULL == pDM )
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return;
	}

	//REC_KEY
	CString strRecKey, strCode, strTitle;
	ids = pDM->MakeRecKey(strRecKey);
	if ( ids<0 )
	{
		AfxMessageBox(_T("REC_KEY를 생성할 수 없습니다."));
		return;
	}

	INT iSelInfoSource  = m_pMCombo->GetCurSel();
	if ( iSelInfoSource < 0 )
	{
		AfxMessageBox(_T("관련어 코드를 선택하세요."));
		return;
	}
	strCode = m_pMCombo->GetItemDataString(iSelInfoSource, 0);

	GetControlData( _T("CM_관리_분류기호_주제명_관련어_입력"), _T("주제명"), strTitle );
	
	INT n_Index =pDM->GetRowCount();
	
	pDM->InsertRow(-1);
	pDM->SetCellData(_T("REC_KEY"),strRecKey,n_Index);
	pDM->SetCellData(_T("주제명키"),strSubjectKey,n_Index);
	pDM->SetCellData(_T("관련어"),strTitle,n_Index);
	pDM->SetCellData(_T("관련어코드"),strCode,n_Index);
	pDM->SetCellData(_T("작업구분"),_T("I"),n_Index);

	
	ControlDisplay(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));

EFS_END
}

VOID CMNG_NEW_RELATED::Update_Related()
{
EFS_BEGIN
	UpdateData(TRUE);

	CESL_DataMgr *pDM = FindDM(_T("DM_관리_분류기호_주제명_관련어"));
	if ( NULL == pDM )
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return;
	}

	//FIND REC_KEY INDEX
	CESL_Grid *p_grid = (CESL_Grid*)FindControl(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));
	INT n_idx = p_grid->SelectGetIdx();
	if ( n_idx <0 ) return;

	CString strRecKey, strCode, strTitle;

	INT iSelInfoSource  = m_pMCombo->GetCurSel();
	if ( iSelInfoSource < 0 )
	{
		AfxMessageBox(_T("관련어 코드를 선택하세요."));
		return;
	}
	strCode = m_pMCombo->GetItemDataString(iSelInfoSource, 0);
	GetControlData( _T("CM_관리_분류기호_주제명_관련어_입력"), _T("주제명"), strTitle );
	strRecKey = pDM->GetCellData(_T("REC_KEY"),n_idx);

	pDM->SetCellData(_T("REC_KEY"),strRecKey,n_idx);
	pDM->SetCellData(_T("주제명키"),strSubjectKey,n_idx);
	pDM->SetCellData(_T("관련어"),strTitle,n_idx);
	pDM->SetCellData(_T("관련어코드"),strCode,n_idx);
	pDM->SetCellData(_T("작업구분"),_T("U"),n_idx);

	// Grid만 Display
	ControlDisplay(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));

EFS_END
}
HBRUSH CMNG_NEW_RELATED::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

