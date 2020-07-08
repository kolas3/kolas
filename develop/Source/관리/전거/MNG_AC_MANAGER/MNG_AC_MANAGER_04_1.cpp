// MNG_AC_MANAGER_04_1.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_AC_MANAGER_04_1.h"

//고정길이
#include "..\..\..\공통\CAT_FIXED_FIELD\Cat008FixedField_AC.h"
#include "..\..\..\공통\MarcAdjustApi\CatApi_BO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_04_1 dialog


CMNG_AC_MANAGER_04_1::CMNG_AC_MANAGER_04_1(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_AC_MANAGER_04_1)
		// NOTE: the ClassWizard will add member initialization here
	m_pMarc			= NULL;
	m_nCurIndex		= 0;
	m_nTotalIndex	= 0;
	m_sACType		= _T("");
	//}}AFX_DATA_INIT
}

CMNG_AC_MANAGER_04_1::~CMNG_AC_MANAGER_04_1()
{
EFS_BEGIN

	if(m_pMarc != NULL)
		delete m_pMarc;

	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));


EFS_END
}

BOOL CMNG_AC_MANAGER_04_1::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CMNG_AC_MANAGER_04_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_AC_MANAGER_04_1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_AC_MANAGER_04_1, CDialog)
	//{{AFX_MSG_MAP(CMNG_AC_MANAGER_04_1)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_CBN_SELCHANGE(IDC_cAcCode, OnSelchangecAcCode)
	ON_CBN_SELCHANGE(IDC_cInfoSource, OnSelchangecInfoSource)
	ON_BN_CLICKED(IDC_bEDIT_LENGTH, OnbEDITLENGTH)
	ON_BN_CLICKED(IDC_bADD, OnbADD)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_CBN_SELCHANGE(IDC_cACKind, OnSelchangecACKind)
	ON_EN_CHANGE(IDC_euSelItem, OnChangeeuSelItem)
	ON_EN_CHANGE(IDC_eSummery, OnChangeeSummery)
	ON_EN_CHANGE(IDC_eYear, OnChangeeYear)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_EN_CHANGE(IDC_eInfoSource, OnChangeeInfoSource)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_04_1 message handlers

BOOL CMNG_AC_MANAGER_04_1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("MNG_AC_MANAGER_03") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_AC_MANAGER_03") );
		return false;
	}
	
	//CM, DM Alias 설정
	if ( m_pCM ) m_pCM = NULL;
	m_pCM = FindCM ( _T("CM_MNG_AC_MANAGER_03") );
	if ( m_pCM == NULL ) return FALSE;

	if ( m_pDM ) m_pDM = NULL;
	m_pDM = FindDM ( _T("DM_MNG_AC_MANAGER_MASTER") );
	if ( m_pDM == NULL ) return FALSE;
	

	//메인 그리드 설정
	m_pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_MNG_AC_MANAGER_03"), _T("RefGrid")));
	if(m_pGrid == NULL) return FALSE;
	

	//Marc Editor 셋팅
	if (!m_marcEditor.SubclassDlgItem(IDC_MarcRichEdit, -1, this))
	{
		AfxMessageBox(_T("Subclass Marc Editor Failed"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//마크 값 초기화
	if (m_pMarc)
		m_pMarc = NULL;
	m_pMarc = new CMarc;

	m_pInfo->pMarcMgr->SetMarcGroup(_T("전거"));
	
	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);
	
	m_pInfoSourceCombo.SubclassDlgItem( IDC_cInfoSource, this );
	m_pMCombo.SubclassDlgItem( IDC_cAcCode, this );
	m_pACTypeCombo.SubclassDlgItem( IDC_cACKind, this );
	
	Init(0);

	Display();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 마스터 키를 이용해 DM을 구성한다.
INT CMNG_AC_MANAGER_04_1::InitDMbyKeyList(INT nIdx)
{
	m_pDM->FreeData();
	
	CString strWhere;
	strWhere = _T("");
	
	m_sModifyRecKey = m_sArrayKeyList[nIdx];
	if(m_sModifyRecKey.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("마스터 키가 없습니다."));
		return -1;
	}

	strWhere = _T(" REC_KEY = ");

	strWhere += m_sModifyRecKey;	

	m_pDM->RefreshDataManager(strWhere);

	return 0;
}

BOOL CMNG_AC_MANAGER_04_1::Init(INT nIdx)
{
	//DM을 초기화하고
	InitDMbyKeyList(nIdx);

	//마크 설정
	SetMarc();

	//ACMODE설정//저자일 경우 생몰년 세팅
	SetACMode();

	//전거 표시 기호 셋팅	
	SetAcCombo();
	
	//정보원 콤보박스 셋팅 
	SetInfoSourceCombo();

	m_pDM->FreeData();

	//수정 화면 마크 디코딩
	MarcDecodingToDM();

	return TRUE;
}

BOOL CMNG_AC_MANAGER_04_1::SetMarc()
{
	CString strMarc;
	m_pDM->GetCellData(_T("MARC"),0,strMarc);

	m_pDM->GetCellData(_T("REC_KEY"),0,m_sModifyRecKey);

	m_pDM->GetCellData(_T("AC_TYPE"),0,m_sACType);
	
	m_pInfo->pMarcMgr->Decoding(strMarc, m_pMarc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);
	
	return TRUE;
}

BOOL CMNG_AC_MANAGER_04_1::SetACMode() 
{
	CString strMode;
	m_pDM->GetCellData(_T("AC_TYPE"),0,strMode);
	if(strMode == _T("0") || strMode == _T("1") || strMode == _T("2"))
	{
		m_ACMODE = AUTHOR_MODE;
		GetDlgItem(IDC_eYear)->EnableWindow(TRUE);
		SetYear();
	}
	else if(strMode == _T("3"))
	{
		GetDlgItem(IDC_eYear)->EnableWindow(FALSE);
		m_ACMODE = TITLE_MODE;
	}
	else
	{
		GetDlgItem(IDC_eYear)->EnableWindow(FALSE);
		m_ACMODE = SUBJECT_MODE;
	}

	return TRUE;
}

VOID CMNG_AC_MANAGER_04_1::SetYear()
{
	CString sMarc, sYear;
	m_pInfo->pMarcMgr->GetItem(m_pMarc,_T("100$d"),sYear);
	if(sYear.IsEmpty() == FALSE) GetDlgItem(IDC_eYear)->SetWindowText(sYear);
	m_pInfo->pMarcMgr->GetItem(m_pMarc,_T("110$d"),sYear);
	if(sYear.IsEmpty() == FALSE) GetDlgItem(IDC_eYear)->SetWindowText(sYear);
	m_pInfo->pMarcMgr->GetItem(m_pMarc,_T("111$d"),sYear);
	if(sYear.IsEmpty() == FALSE) GetDlgItem(IDC_eYear)->SetWindowText(sYear);
}

VOID CMNG_AC_MANAGER_04_1::Display()
{
	EFS_BEGIN
		

	m_marcEditor.Display();
	
	CString strText;
	if ( m_ACMODE == AUTHOR_MODE )
		strText = _T("저자 전거 통제 편집");
	else if ( m_ACMODE == TITLE_MODE )
		strText = _T("서명 전거 통제 편집");
	else if ( m_ACMODE == SUBJECT_MODE )
		strText = _T("주제어 전거 통제 편집");
	
	strText = _T("저자 전거 통제 편집");
	SetWindowText(strText);
	
	
	// 전거표목, 그리드 Display
	GetDlgItem(IDC_eRefItem)->SetWindowText(_T(""));

	if(m_nCurIndex == 0 || m_nTotalIndex == 1)
		GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);
	
	if(m_nCurIndex == m_nTotalIndex-1 || m_nTotalIndex == 1)
		GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
	else 
		GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);

	m_pCM->AllControlDisplay();
		
EFS_END
}


BOOL CMNG_AC_MANAGER_04_1::InsertACTBL(CString sACMasterKey)
{
EFS_BEGIN
		
	INT ids = -1;
	CESL_DataMgr *pDM = NULL;
	CString strTBLNAME;
	if ( m_ACMODE == AUTHOR_MODE )
		pDM = FindDM( _T("DM_MNG_AC_MANAGER_AUTHOR") );
	else if ( m_ACMODE == TITLE_MODE )
		pDM = FindDM( _T("DM_MNG_AC_MANAGER_TITLE") );
	else if ( m_ACMODE == SUBJECT_MODE )
		pDM = FindDM( _T("DM_MNG_AC_MANAGER_SUBJECT") );
	
	if ( pDM == NULL ) return FALSE;
	
	ids = pDM->StartFrame();
	if ( ids < 0 ) return FALSE;
	
	// 전거 테이블에 입력
	CString sACReckey;

	INT nCnt = m_pDM->GetRowCount();

	for( INT i = 0; i <= nCnt ; i++ )
	{	
		m_pDM->MakeRecKey(sACReckey);
		pDM->InsertRow(-1);
		pDM->SetCellData(_T("REC_KEY"), sACReckey , i);
		pDM->SetCellData(_T("AC_MASTER_MAT_KEY"), sACMasterKey , i);

		CString sKeyWordTmp = _T("");
		
		if( i == nCnt )
		{//채택표목이면
			GetDlgItem(IDC_euSelItem)->GetWindowText(sKeyWordTmp);
			pDM->SetCellData(_T("SELECT_YN"), _T("Y") , i);
		}
		else
		{//참조표목이면
			m_pDM->GetCellData(_T("참조표목"),i,sKeyWordTmp);
			pDM->SetCellData(_T("SELECT_YN"), _T("N") , i);
		}
		
		pDM->SetCellData(_T("KEYWORD"), sKeyWordTmp , i);

		pDM->SetCellData(_T("FIRST_WORK"), GetWorkLogMsg(_T("전거입력"),WORK_LOG) , i);
		pDM->SetCellData(_T("LAST_WORK"), GetWorkLogMsg(_T("전거입력"),WORK_LOG) , i);

		CArray<CString, CString> RemoveAliasList;
		RemoveAliasList.RemoveAll();

		ids = pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, i, pDM);
		if ( ids < 0) return FALSE;
	}

	ids = pDM->SendFrame();
	if ( ids < 0) return FALSE;
	
	ids = pDM->EndFrame();
	if ( ids < 0 ) return FALSE;
	
	return true;
	
EFS_END
return false;
}
BOOL CMNG_AC_MANAGER_04_1::InsertACMASTERTBL(CString sACMasterKey)
{
EFS_BEGIN
	
	INT ids = -1;
	
//	CTime t = CTime::GetCurrentTime();
//	CString strMsg;
//	strMsg.Format(_T("%04d%02d%02d %02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute());

	// 도서관 부호 가져오기
	CCatApi_BO *pCatApi = NULL;
	pCatApi = new CCatApi_BO(this);

	CString strLibCode  = _T("");
	CString strItemData = _T("");
	ids = pCatApi->GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);
	
	// 도서관 부호 가져왔나 체크
	if (ids < 0 || strLibCode.IsEmpty())
	{
		AfxMessageBox(_T("도서관 부호가 없습니다. 수정해주세요"));
		return -1;
	}

	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("040$a"), strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("040$a"), strLibCode);
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("040$c"), strLibCode);
	}
	// 도서관 부호가 다른경우 040$d로 넣어준다. 반복 안되게..
	else if (strItemData != strLibCode)
	{
		// 삭제하고 넣는다.
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("040$d"), strLibCode);
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("040$d"), strLibCode);
	}

	// 최종수정일자 생성 005 무조건 갱신..
	CTime time;
	time = CTime::GetCurrentTime();
	CString strItem;
	strItem.Format(_T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("005"));
	m_pInfo->pMarcMgr->InsertField(m_pMarc, _T("005") + strItem);

	// 전거 MASTER에 입력 하는 작업
	CString sStreamMarc;
	m_pInfo->pMarcMgr->Encoding(m_pMarc, sStreamMarc );

	ids = m_pDM->StartFrame();
	if ( ids < 0 ) return FALSE;
	
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), sACMasterKey);
	m_pDM->AddDBFieldData(_T("MARC"), _T("STRING"), sStreamMarc);
	m_pDM->AddDBFieldData(_T("INPUT_DATE"), _T("STRING"), strItem , 0);
	m_pDM->AddDBFieldData(_T("UPDATE_DATE"), _T("STRING"), strItem , 0);
	
	//m_pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("전거입력"),WORK_LOG));
	m_pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("전거수정"),WORK_LOG));
	
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	ids = m_pDM->MakeUpdateFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), sACMasterKey );

	if ( ids < 0) return FALSE;
	
	ids = m_pDM->SendFrame();
	if ( ids < 0) return FALSE;
	
	ids = m_pDM->EndFrame();
	if ( ids < 0 ) return FALSE;
	
	return true;
	
EFS_END
	return false;
}
///////////////////////////////////////////////////////////////////////
// 전거 저장
BOOL CMNG_AC_MANAGER_04_1::InsertToDB()
{
EFS_BEGIN
		INT ids = -1;
	
	if ( !InsertACTBL(m_sModifyRecKey) ) return FALSE;
	
	if ( !InsertACMASTERTBL(m_sModifyRecKey) ) return FALSE;
	
	return TRUE;
	
EFS_END
return FALSE;
}
///////////////////////////////////////////////////////////////////////
// 정보원 콤보박스 셋팅
VOID CMNG_AC_MANAGER_04_1::SetInfoSourceCombo()
{
EFS_BEGIN

	m_pInfoSourceCombo.FormatList( 2, DT_LEFT, false , EDGE );
	m_pInfoSourceCombo.RequireFlag = FALSE;
	
	m_pInfoSourceCombo.AddItem( _T("0"), 0, 0 );
	m_pInfoSourceCombo.AddItem( _T("적용안함"), 1, 0 );
	
	m_pInfoSourceCombo.AddItem( _T("1"), 0, 1 );
	m_pInfoSourceCombo.AddItem( _T("백과사전"), 1, 1 );
	
	m_pInfoSourceCombo.AddItem( _T("2"), 0, 2 );
	m_pInfoSourceCombo.AddItem( _T("영어사전"), 1, 2 );
	
	m_pInfoSourceCombo.AddItem( _T("3"), 0, 3 );
	m_pInfoSourceCombo.AddItem( _T("서명"), 1, 3 );

	CString strInfoSource;
	strInfoSource = _T("");

	m_pInfo->pMarcMgr->GetItem(m_pMarc,_T("670$a"),strInfoSource);
		
	if(strInfoSource == _T(""))
	{
		m_pInfoSourceCombo.SetCurSel( 0 );
		GetDlgItem(IDC_eInfoSource)->EnableWindow(TRUE);
	}
	else if(strInfoSource == _T("백과사전"))
	{
		m_pInfoSourceCombo.SetCurSel( 1 );
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	}
	else if(strInfoSource == _T("영어사전"))
	{
		m_pInfoSourceCombo.SetCurSel( 2 );
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	}
	else if(strInfoSource == _T("서명"))
	{
		m_pInfoSourceCombo.SetCurSel( 3 );
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	}
	else
	{
		m_pInfoSourceCombo.SetCurSel( 0 );
		GetDlgItem(IDC_eInfoSource)->SetWindowText(strInfoSource);
		GetDlgItem(IDC_eInfoSource)->EnableWindow(TRUE);
	}		
	
EFS_END
}
///////////////////////////////////////////////////////////////////////
// 전거 표기기호 콤보 박스 셋팅
VOID CMNG_AC_MANAGER_04_1::SetAcCombo()
{
EFS_BEGIN

	m_pMCombo.FormatList( 2, DT_LEFT, false , EDGE );
	m_pMCombo.RequireFlag = FALSE;
	
	if ( m_ACMODE < 0 ) return;

	INT defaultCel = 0;
	CString code, desc;

	if ( m_ACMODE == AUTHOR_MODE )
	{
		m_pMCombo.AddItem( _T("100"), 0, 0 );
		m_pMCombo.AddItem( _T("개인명"), 1, 0 );
		
		m_pMCombo.AddItem( _T("110"), 0, 1 );
		m_pMCombo.AddItem( _T("단체명"), 1, 1 );
		
		m_pMCombo.AddItem( _T("111"), 0, 2 );
		m_pMCombo.AddItem( _T("그룹명"), 1, 2 );
		
		if( m_pDM->GetCellData(_T("AC_TYPE"),0) == _T("0") )
			m_pMCombo.SetCurSel( 0 );
		else if( m_pDM->GetCellData(_T("AC_TYPE"),0) == _T("1") )
			m_pMCombo.SetCurSel( 1 );
		else
			m_pMCombo.SetCurSel( 2 );
	}
	else if ( m_ACMODE == TITLE_MODE )
	{
		m_pMCombo.AddItem( _T("130"), 0, 0 );
		m_pMCombo.AddItem( _T("서명"), 1, 0 );	
		m_pMCombo.SetCurSel( 0 );
	}		
	else if ( m_ACMODE == SUBJECT_MODE )
	{
		m_pMCombo.AddItem( _T("150"), 0, 0 );
		m_pMCombo.AddItem( _T("주제명"), 1, 0 );
		m_pMCombo.SetCurSel( 0 );
	}
	else 
	{
		AfxMessageBox(_T("전거모드가 설정되지 않았습니다."));
	}

	m_pACTypeCombo.FormatList( 1, DT_LEFT, false , EDGE );
	m_pACTypeCombo.RequireFlag = FALSE;
	
	if( m_ACMODE == AUTHOR_MODE )
		m_pACTypeCombo.AddItem( _T("저자명"), 0, 0 );
	else if( m_ACMODE == TITLE_MODE )
		m_pACTypeCombo.AddItem( _T("서명"), 0, 0 );
	else if( m_ACMODE == SUBJECT_MODE )
		m_pACTypeCombo.AddItem( _T("주제명"), 0, 0 );
	else
	{
		AfxMessageBox(_T("전거모드가 설정되지 않았습니다."));
		m_pACTypeCombo.AddItem( _T("적용안함"), 0, 0 );
	}
	
	m_pACTypeCombo.SetCurSel( 0 );

EFS_END
}

///////////////////////////////////////////////////////////////////////
// 전거 마크 삭제
VOID CMNG_AC_MANAGER_04_1::DeleteACMarcField()
{
EFS_BEGIN
	
	if ( m_ACMODE < 0 ) return;
	
	if ( m_ACMODE == AUTHOR_MODE )
	{
		// 저자명 - 채택 표목
		// 1xx - 100, 110, 111
		// 4xx - 400, 410, 411
		// 5xx - 500, 510, 511
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("100"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("110"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("111"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("400"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("410"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("411"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("500"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("510"));
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("511"));
	}
	else if ( m_ACMODE ==  TITLE_MODE )
	{
		// 서명 - 채택 표목
		// 1xx - 130
		// 4xx - 430
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("130"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("430"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("530"));
	}
	else if ( m_ACMODE == SUBJECT_MODE )
	{
		// 주제명 - 채택 표목
		// 1xx - 150
		// 4xx - 450
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("150"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("450"));
		
		m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("550"));
	}
	
	return;
	
	EFS_END
}
BOOL CMNG_AC_MANAGER_04_1::IsCheckSaveInputValue()
{
EFS_BEGIN
		
	//008 있는지 Check
	CString sTmp008Field = _T("");
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("008"), sTmp008Field);
	if ( sTmp008Field == _T("") )
	{
		AfxMessageBox( _T("008 Tag를 입력해 주세요") );
		return FALSE;
	}
	
	return TRUE;
	
EFS_END
return FALSE;
}


VOID CMNG_AC_MANAGER_04_1::SetACMarc(INT iACCode)
{
EFS_BEGIN

	if ( iACCode < 0 ) return;

	//0 - 개인명
	//1 - 단체명
	//2 - 그룹명
	CString sRefItem;

	//기존에 부여된 400, 410, 411 Marc 삭제
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("400"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("410"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("411"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("430"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("450"));

	//DM에서 값을 가져와 셋팅
	for ( INT i = 0; i < m_pDM->GetRowCount(); i++ )
	{
	
		m_pDM->GetCellData(_T("참조표목"), i , sRefItem );

		if ( iACCode == 0)
		{
			if ( m_ACMODE == AUTHOR_MODE )	m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("400$a"), sRefItem );
			else if ( m_ACMODE == TITLE_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("430$a"), sRefItem );
			else if ( m_ACMODE == SUBJECT_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("450$a"), sRefItem );
		}
		else if ( iACCode == 1 )
		{
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("410$a"), sRefItem );
		}
		else if ( iACCode == 2 )
		{
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("411$a"), sRefItem );
		}
		
		
	}

	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("100"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("110"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("111"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("130"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("150"));
	
	//채택표목 세팅
	sRefItem = _T("");
	CString sYear = _T("");	
	GetDlgItem(IDC_euSelItem)->GetWindowText(sRefItem);
	GetDlgItem(IDC_eYear)->GetWindowText(sYear);

	if ( iACCode == 0)
	{
		if ( m_ACMODE == AUTHOR_MODE )
		{
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$a"), sRefItem );
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$d"), sYear );
		}
		else if ( m_ACMODE == TITLE_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("130$a"), sRefItem );
		else if ( m_ACMODE == SUBJECT_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("150$a"), sRefItem );
	}
	else if ( iACCode == 1 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$a"), sRefItem );
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$d"), sYear );
	}
	else if ( iACCode == 2 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$a"), sRefItem );
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$d"), sYear );
	}

	return;

EFS_END
}


BOOL CMNG_AC_MANAGER_04_1::UpdateRefItem(CString sRefItem)
{
EFS_BEGIN
		
	CString sTmpRefItem = _T("");
	GetDlgItem(IDC_eRefItem)->GetWindowText(sTmpRefItem);
	if ( sTmpRefItem == _T("") )
	{
		AfxMessageBox(_T("참조 표목 항목을 선택해 주세요"));
		return FALSE;
	}
	
	//그리드에 수정
	long nRowLocation;
	nRowLocation = m_pGrid->GetRow()-1;
	
	//m_pGrid->SetAt( nRowLocation, 0, sRefItem );
	m_pDM->SetCellData(_T("참조표목"), sRefItem, nRowLocation);
	
	return TRUE;
	
EFS_END
return FALSE;
}


BOOL CMNG_AC_MANAGER_04_1::InsertRefItem(CString sRefItem)
{
EFS_BEGIN
		
	//참조표목 Grid에 입력
	INT ids = m_pDM->GetRowCount();
	m_pDM->SetCellData(_T("참조표목"), sRefItem, -1 );
	
	//참조표목 Marc에 입력
	INT iSelInfoSource = m_pMCombo.GetCurSel();
	
	//Marc에 반영
	SetACMarc ( iSelInfoSource );
	
	return TRUE;
	
EFS_END
return FALSE;
}

CString CMNG_AC_MANAGER_04_1::ISCheckInputValue()
{
EFS_BEGIN
		
	CString sTmpItem = _T("");
	GetDlgItem(IDC_eRefItem)->GetWindowText(sTmpItem);
	
	if ( sTmpItem == _T("") )
	{
		AfxMessageBox(_T("참조표목을 입력해 주세요"));
		return _T("");
	}
	return sTmpItem;
	
EFS_END
return _T("");
}


VOID CMNG_AC_MANAGER_04_1::OnbCLOSE() 
{
EFS_BEGIN
	
	CDialog::OnCancel();
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04_1::ControlClear()
{
EFS_BEGIN
	GetDlgItem(IDC_eYear)->SetWindowText(_T(""));
	GetDlgItem(IDC_eSummery)->SetWindowText(_T(""));
EFS_END
}

VOID CMNG_AC_MANAGER_04_1::OnOK() 
{
EFS_BEGIN
		
	if ( !IsCheckSaveInputValue() ) return;
	
	if ( !InsertToDB() ) 
	{
		AfxMessageBox(_T("DB에 저장중 에러가 발생하였습니다"));
		return;
	}
	
	AfxMessageBox( _T("저장하였습니다.") );
	
EFS_END
}

VOID CMNG_AC_MANAGER_04_1::OnSelchangecAcCode() 
{
EFS_BEGIN
		
	//참조표목 Marc에 입력
	INT iSelInfoSource = m_pMCombo.GetCurSel();
	
	//Marc에 반영
	SetACMarc ( iSelInfoSource );
	
	Display();
	
EFS_END		
	
}

VOID CMNG_AC_MANAGER_04_1::OnSelchangecInfoSource() 
{
EFS_BEGIN
		
	//정보원 선택 변경
	SetInfoSource();
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04_1::OnbEDITLENGTH() 
{
EFS_BEGIN
		
	// 마크 생성
	CCat008FixedField_AC dlg(this, m_pMarc);
	
	if ( dlg.DoModal() == IDOK ) 
	{
		// Check m_pMarc
		if (m_pMarc == NULL)
		{
			AfxMessageBox(_T("pMarc Is NULL"));
			EndDialog(IDCANCEL);
			return;
		}
		Display();
	}
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04_1::OnbADD() 
{
EFS_BEGIN
		
		//참조표목 추가
		
		//입력한 값 유효성 검사
	CString sRefItem = ISCheckInputValue();
	if ( sRefItem == _T("") ) return;
	
	//m_iSetMode = ADD;
	//입력된 참조 표목 Grid와 Marc에 반영
	InsertRefItem(sRefItem);
	
	Display();
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04_1::OnbMODIFY() 
{
EFS_BEGIN
		
	// 수정
	CString sRefItem = ISCheckInputValue();
	if ( sRefItem == _T("") ) return;
	
	//m_iSetMode = MODIFY;
	
	UpdateRefItem(sRefItem);
	
	
	Display();
	
EFS_END	
	
}

VOID CMNG_AC_MANAGER_04_1::OnbDelete() 
{
EFS_BEGIN
		
	long nRowLocation;
	nRowLocation = m_pGrid->GetRow()-1;
	m_pDM->DeleteRow(nRowLocation);
	
	//참조표목 Marc에 입력
	INT iSelInfoSource = m_pMCombo.GetCurSel();
	
	//Marc에 반영
	SetACMarc ( iSelInfoSource );
	
	Display();
	
EFS_END	
}

VOID CMNG_AC_MANAGER_04_1::SetInfoSource()
{
EFS_BEGIN
	
	//정보원 선택 변경
	INT iSelInfoSource = m_pInfoSourceCombo.GetCurSel();

	//기존에 670 Tag 삭제
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("670"));
	
	// 0 - 적용안함
	// 1 - 백과사전
	// 2 - 영어사전
	// 3 - 서명
	GetDlgItem(IDC_eInfoSource)->SetWindowText(_T(""));

	if ( iSelInfoSource == 0 )
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T(""));	
	else if ( iSelInfoSource == 1 )
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T("백과사전"));	
	else if ( iSelInfoSource == 2 )
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T("영어사전"));	
	else if ( iSelInfoSource == 3 )
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T("서명"));	
	
	if ( iSelInfoSource == 0 )
		GetDlgItem(IDC_eInfoSource)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	//빈테그 삭제
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);
	
	Display();
	
EFS_END	
}



VOID CMNG_AC_MANAGER_04_1::OnSelchangecACKind() 
{
EFS_BEGIN
	
	INT iSelInfoSource; 
	iSelInfoSource = m_pACTypeCombo.GetCurSel();

	m_pMCombo.ResetContent();

	m_pMCombo.FormatList( 2, DT_LEFT, false , EDGE );
	m_pMCombo.RequireFlag = FALSE;

	switch( iSelInfoSource )
	{
	case 0 :
		m_ACMODE = AUTHOR_MODE;
		
		m_pMCombo.AddItem( _T("100"), 0, 0 );
		m_pMCombo.AddItem( _T("개인명"), 1, 0 );
		
		m_pMCombo.AddItem( _T("110"), 0, 1 );
		m_pMCombo.AddItem( _T("단체명"), 1, 1 );
		
		m_pMCombo.AddItem( _T("111"), 0, 2 );
		m_pMCombo.AddItem( _T("그룹명"), 1, 2 );
		
		m_pMCombo.SetCurSel( 0 );
		
		break;
	case 1 :
		m_ACMODE = TITLE_MODE;
		
		m_pMCombo.AddItem( _T("130"), 0, 0 );
		m_pMCombo.AddItem( _T("서명"), 1, 0 );	
		m_pMCombo.SetCurSel( 0 );
		
		break;
	case 2 :
		m_ACMODE = SUBJECT_MODE;
		
		m_pMCombo.AddItem( _T("150"), 0, 0 );
		m_pMCombo.AddItem( _T("주제명"), 1, 0 );
		m_pMCombo.SetCurSel( 0 );
		
		break;
	default :
		return;
		break;
	}




EFS_END	
}

VOID CMNG_AC_MANAGER_04_1::MarcDecodingToDM()
{
EFS_BEGIN

	// Marc 디코딩
	if ( m_pMarc == NULL ) return;

	CString sTmpItem = _T("");

	//1XX 저장
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("100$a"), sTmpItem );
	if ( sTmpItem.IsEmpty() == FALSE ) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("110$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("111$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("130$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("150$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_euSelItem)->SetWindowText(sTmpItem);


	//4XX 저장
	CString sTmpField;
	CArray<CString, CString&> pArray4XX;
	CString strData;
	CString sIndL;
	sIndL.Format( _T("%c"), 30 );

	//400
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("400") , sTmpField, &pArray4XX);
	INT nCount = pArray4XX.GetSize();
	INT i;
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("참조표목"), strData, -1);
	}

	//410
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("410") , sTmpField, &pArray4XX);
	nCount = pArray4XX.GetSize();
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("참조표목"), strData, -1 );
	}

	//411
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("411") , sTmpField, &pArray4XX);
	nCount = pArray4XX.GetSize();
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("참조표목"), strData, -1 );
	}

	//430
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("430") , sTmpField, &pArray4XX);
	nCount = pArray4XX.GetSize();
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("참조표목"), strData, -1 );
	}

	//450
	pArray4XX.RemoveAll();
	m_pInfo->pMarcMgr->GetField(m_pMarc, _T("450") , sTmpField, &pArray4XX);
	nCount = pArray4XX.GetSize();
	for (i = 0; i < nCount; i++)
	{
		strData = pArray4XX.GetAt(i);
		strData = strData.Mid(4);
		INT iPos = strData.Find(sIndL);
		strData = strData.Left(iPos);
		m_pDM->SetCellData(_T("참조표목"), strData, -1 );
	}

	// 678 요약테그 저장
	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("678$a"), sTmpItem );
	if ( sTmpItem != _T("")) GetDlgItem(IDC_eSummery)->SetWindowText(sTmpItem);

	Display();

	return;

EFS_END
}

VOID CMNG_AC_MANAGER_04_1::OnChangeeuSelItem() 
{
EFS_BEGIN
		
	CString sSelItem = _T("");
	GetDlgItem(IDC_euSelItem)->GetWindowText(sSelItem);
	
	INT iSelInfoSource = -1;
	if ( sSelItem != _T("") )
	{
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("100$a"));
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("110$a"));
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("111$a"));
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("130$a"));
		m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("150$a"));
		
		//정보원 선택 변경
		iSelInfoSource = m_pMCombo.GetCurSel();
	}
	
	
	
	if ( m_ACMODE == AUTHOR_MODE )
	{
		if ( iSelInfoSource == 0 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$a"), sSelItem);	
		if ( iSelInfoSource == 1 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$a"), sSelItem);	
		if ( iSelInfoSource == 2 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$a"), sSelItem);	
	}
	else if ( m_ACMODE == TITLE_MODE )
	{
		if ( iSelInfoSource == 0 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("130$a"), sSelItem);	
	}
	else if ( m_ACMODE == SUBJECT_MODE )
	{
		if ( iSelInfoSource == 0 ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("150$a"), sSelItem);	
	}
	
	if ( sSelItem != _T("") )
	{
		//빈테그 삭제
		m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
		m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);
	}
	
	Display();
	
	EFS_END	
}

VOID CMNG_AC_MANAGER_04_1::OnChangeeSummery() 
{
EFS_BEGIN
	
	CString sSummeryItem = _T("");
	GetDlgItem(IDC_eSummery)->GetWindowText(sSummeryItem);
	
	//m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("678"));
	CString sTmpItem = _T("") ;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("678$a"), sTmpItem );
	if ( sTmpItem == _T("") )
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("678$a"), sSummeryItem);
	else 
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("678$a"), sSummeryItem, _T(""), MODIFY_ITEM);
	
	//빈테그 삭제
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);
	
	Display();
	
EFS_END		
}

VOID CMNG_AC_MANAGER_04_1::OnChangeeYear() 
{
EFS_BEGIN

	CString sYearItem = _T("");
	GetDlgItem(IDC_eYear)->GetWindowText(sYearItem);

	CString sTmpItem = _T("") ;


	if( m_ACMODE == AUTHOR_MODE )
	{
		if(m_sACType == _T("0"))
		{
			m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("100$d"), sTmpItem );
			if ( sTmpItem == _T("") )
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$d"), sYearItem);
			else 
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("100$d"), sYearItem, _T(""), MODIFY_ITEM);
		}
		else if(m_sACType == _T("1"))
		{
			m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("110$d"), sTmpItem );
			if ( sTmpItem == _T("") )
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$d"), sYearItem);
			else 
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("110$d"), sYearItem, _T(""), MODIFY_ITEM);

		}
		else
		{
			m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("111$d"), sTmpItem );
			if ( sTmpItem == _T("") )
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$d"), sYearItem);
			else 
				m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("111$d"), sYearItem, _T(""), MODIFY_ITEM);
		}
	}
	//빈테그 삭제
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);

	Display();

EFS_END		
}

VOID CMNG_AC_MANAGER_04_1::SetKeyList(CString * sKeyList, INT idx)
{
	m_sArrayKeyList = new CString[idx];
	for(INT i = 0; i<idx ; i++)
		m_sArrayKeyList[i] = sKeyList[i];

	m_nTotalIndex = idx;
}

VOID CMNG_AC_MANAGER_04_1::OnbPREV() 
{
	if( m_nCurIndex == 0 )
	{
		AfxMessageBox(_T("처음 정보입니다."));
		return;
	}
	ControlClear();
	m_pMCombo.ResetContent();
	m_pInfoSourceCombo.ResetContent();
	m_pACTypeCombo.ResetContent();
	Init(--m_nCurIndex);
}

VOID CMNG_AC_MANAGER_04_1::OnbNEXT() 
{
	if( m_nCurIndex == m_nTotalIndex-1 )
	{
		AfxMessageBox(_T("마지막 정보입니다."));
		return;
	}
	ControlClear();
	m_pMCombo.ResetContent();
	m_pInfoSourceCombo.ResetContent();
	m_pACTypeCombo.ResetContent();
	Init(++m_nCurIndex);
}

VOID CMNG_AC_MANAGER_04_1::OnChangeeInfoSource() 
{
	//정보위치 변경

	//정보원 선택 
	INT iSelInfoSource = m_pInfoSourceCombo.GetCurSel();
	
	//기존에 670 Tag 삭제
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("670"));
	
	CString sInfoSource = _T("");
	GetDlgItem(IDC_eInfoSource)->GetWindowText(sInfoSource);	

	m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), sInfoSource);
	
	//빈테그 삭제
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);
	
	Display();
	
}

HBRUSH CMNG_AC_MANAGER_04_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
