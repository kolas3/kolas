// MNG_AC_MANAGER_03.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_AC_MANAGER_03.h"
#include "MNG_AC_MANAGER_DOBORA.h"
#include "..\..\..\공통\MarcAdjustApi\CatApi_BO.h"

//고정길이
#include "..\..\..\공통\CAT_FIXED_FIELD\Cat008FixedField_AC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_03 dialog


CMNG_AC_MANAGER_03::CMNG_AC_MANAGER_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_AC_MANAGER_03)
	m_bDobora	= FALSE;
	m_pMarc		= NULL;
	m_sACType	= _T("0");
	//}}AFX_DATA_INIT
}

BOOL CMNG_AC_MANAGER_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

BEGIN_EVENTSINK_MAP(CMNG_AC_MANAGER_03, CDialog)
//{{AFX_EVENTSINK_MAP(CMNG_AC_MANAGER_03)
ON_EVENT(CMNG_AC_MANAGER_03, IDC_mngGRID_1, -605 /* MouseDown */, OnMouseDownmngGRID1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_AC_MANAGER_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_AC_MANAGER_03)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_AC_MANAGER_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_AC_MANAGER_03)
	ON_BN_CLICKED(IDC_bADD, OnbADD)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bEDIT_LENGTH, OnbEDITLENGTH)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_CBN_SELCHANGE(IDC_cInfoSource, OnSelchangecInfoSource)
	ON_CBN_SELCHANGE(IDC_cAcCode, OnSelchangecAcCode)
	ON_EN_CHANGE(IDC_eSummery, OnChangeeSummery)
	ON_BN_CLICKED(IDC_bCLEAR_SCREEN, OnbCLEARSCREEN)
	ON_EN_CHANGE(IDC_eSelItem, OnChangeeSelItem)
	ON_BN_CLICKED(IDC_bDOBORA, OnbDOBORA)
	ON_EN_CHANGE(IDC_eYear, OnChangeeYear)
	ON_CBN_SELCHANGE(IDC_cACKind, OnSelchangecACKind)
	ON_EN_CHANGE(IDC_eInfoSource, OnChangeeInfoSource)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_ACEDIT_MODIFY, OnACEditUpdate)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_03 message handlers

VOID CMNG_AC_MANAGER_03::OnOK() 
{
EFS_BEGIN

	if ( !IsCheckSaveInputValue() ) return;

	if ( !InsertToDB() ) 
	{
		AfxMessageBox(_T("DB에 저장중 에러가 발생하였습니다"));
		return;
	}
	
	AfxMessageBox( _T("저장하였습니다.") );

	Display();
	
	CDialog::OnOK();

EFS_END
}

BOOL CMNG_AC_MANAGER_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("MNG_AC_MANAGER_03") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_AC_MANAGER_03") );
		return false;
	}
	
	//생몰년 셋팅 저자일 경우에만 Enable
	if ( m_ACMODE == AUTHOR_MODE )
		GetDlgItem(IDC_eYear)->EnableWindow(TRUE);

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
	m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);

	//전거 표시 기호 셋팅	
	SetAcCombo();

	//정보원 콤보박스 셋팅 
	SetInfoSourceCombo();
	
	//CM, DM Alias 설정
	if ( m_pDM ) m_pDM = NULL;
	m_pDM = FindDM ( _T("DM_MNG_AC_MANAGER_MASTER") );
	if ( m_pDM == NULL ) return FALSE;

	if ( m_pCM ) m_pCM = NULL;
	m_pCM = FindCM ( _T("CM_MNG_AC_MANAGER_03") );
	if ( m_pCM == NULL ) return FALSE;

	m_pDM->FreeData();
	
	//메인 그리드 설정
	m_pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_MNG_AC_MANAGER_03"), _T("RefGrid")));
	if (m_pGrid == NULL) return FALSE;

	OnSelchangecACKind();

	Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//표목추가
VOID CMNG_AC_MANAGER_03::OnbADD() 
{
EFS_BEGIN

	//참조표목 추가

	//입력한 값 유효성 검사
	CString sRefItem = ISCheckInputValue();
	if ( sRefItem == _T("") ) return;

	//입력된 참조 표목 Grid와 Marc에 반영
	InsertRefItem(sRefItem);

	Display();
	return;

EFS_END
}

CString CMNG_AC_MANAGER_03::ISCheckInputValue()
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

BOOL CMNG_AC_MANAGER_03::InsertRefItem(CString sRefItem)
{
EFS_BEGIN

	//참조표목 Grid에 입력
	INT ids = m_pDM->GetRowCount();
	m_pDM->SetCellData(_T("참조표목"), sRefItem, -1 );

	//참조표목 Marc에 입력
	INT iSelInfoSource = m_pMCombo->GetCurSel();

	//Marc에 반영
	SetACMarc ( iSelInfoSource );
	
	return TRUE;

EFS_END
return FALSE;
}

BOOL CMNG_AC_MANAGER_03::RefItem_AddToGrid(CString sRefItem)
{
EFS_BEGIN

	//그리드에 참조표목 값 입력 
	return TRUE;

EFS_END
return FALSE;
}

BOOL CMNG_AC_MANAGER_03::RefItem_AddToMarc(CString sRefItem)
{
EFS_BEGIN
	//Marc에 참조표목 값 입력 
	return TRUE;

EFS_END
return FALSE;
}

VOID CMNG_AC_MANAGER_03::OnbCLOSE() 
{
EFS_BEGIN

	CDialog::OnCancel();

EFS_END	
}

VOID CMNG_AC_MANAGER_03::OnbEDITLENGTH() 
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

	return;
	
EFS_END
}

VOID CMNG_AC_MANAGER_03::Display()
{
EFS_BEGIN

	CString sTmp;
	m_pInfo->pMarcMgr->Encoding(m_pMarc, sTmp);
	m_pInfo->pMarcMgr->Decoding(sTmp, m_pMarc);

	m_marcEditor.Display();

	CString strText;
	if ( m_ACMODE == AUTHOR_MODE )
		strText = _T("전거 통제 편집");
	else if ( m_ACMODE == TITLE_MODE )
		strText = _T("전거 통제 편집");
	else if ( m_ACMODE == SUBJECT_MODE )
		strText = _T("전거 통제 편집");
	
	//strText = _T("저자 전거 통제 편집");
	SetWindowText(strText);


	// 전거표목, 그리드 Display
	GetDlgItem(IDC_eRefItem)->SetWindowText(_T(""));
	m_pCM->AllControlDisplay();
	return;

EFS_END
		
}

CMNG_AC_MANAGER_03::~CMNG_AC_MANAGER_03()
{
EFS_BEGIN
	
	if(m_pMarc != NULL)
		delete m_pMarc;
	if(m_pMCombo != NULL)
		delete m_pMCombo;
	if(m_pInfoSourceCombo != NULL)
		delete m_pInfoSourceCombo;	
	if(m_pACTypeCombo != NULL)
		delete m_pACTypeCombo;

	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));


EFS_END
}

VOID CMNG_AC_MANAGER_03::OnbMODIFY() 
{
EFS_BEGIN

	// 수정
	CString sRefItem = ISCheckInputValue();
	if ( sRefItem == _T("") ) return;

	UpdateRefItem(sRefItem);


	Display();
	return;

EFS_END	
}

BOOL CMNG_AC_MANAGER_03::UpdateRefItem(CString sRefItem)
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

VOID CMNG_AC_MANAGER_03::OnbDelete() 
{
EFS_BEGIN

	long nRowLocation;
	nRowLocation = m_pGrid->GetRow()-1;

	// 2011.09.19 ADD BY PWR : 건수없을때의 에러수정
	if(1 > m_pDM->GetRowCount()) return;

	m_pDM->DeleteRow(nRowLocation);

	//참조표목 Marc에 입력
	INT iSelInfoSource = m_pMCombo->GetCurSel();
	
	//Marc에 반영
	SetACMarc ( iSelInfoSource );

	Display();
	return;

EFS_END
}

VOID CMNG_AC_MANAGER_03::OnSelchangecInfoSource() 
{
EFS_BEGIN

	//정보원 선택 변경
	SetInfoSource();
	return;
EFS_END	
}

VOID CMNG_AC_MANAGER_03::SetInfoSource()
{
EFS_BEGIN
	
	//정보원 선택 변경
	INT iSelInfoSource = m_pInfoSourceCombo->GetCurSel();

	//기존에 670 Tag 삭제
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("670"));

	// 0 - 적용안함
	// 1 - 백과사전
	// 2 - 영어사전
	// 3 - 서명
	GetDlgItem(IDC_eInfoSource)->SetWindowText(_T(""));

	if ( iSelInfoSource == 0 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T(""));
		GetDlgItem(IDC_eInfoSource)->EnableWindow(TRUE);
	}
	else if ( iSelInfoSource == 1 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T("백과사전"));	
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	}
	else if ( iSelInfoSource == 2 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T("영어사전"));	
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	}
	else if ( iSelInfoSource == 3 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("670$a"), _T("서명"));	
		GetDlgItem(IDC_eInfoSource)->EnableWindow(FALSE);
	}

	//빈테그 삭제
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);

	Display();
	return;

EFS_END	
}

VOID CMNG_AC_MANAGER_03::OnMouseDownmngGRID1(short Button, short Shift, long x, long y) 
{
EFS_BEGIN

	//그리드에 선택된 값을 참조 표목 Edit 
	long nRowLocation;
	nRowLocation = m_pGrid->GetRow()-1;
	CString sSelRefItem;
	m_pDM->GetCellData(nRowLocation, 0, sSelRefItem);
	
	GetDlgItem(IDC_eRefItem)->SetWindowText(sSelRefItem);
	return;

EFS_END
}

VOID CMNG_AC_MANAGER_03::SetACMarc(INT iACCode)
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
		
	GetDlgItem(IDC_eSelItem)->GetWindowText(sRefItem);
	
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

	
	//도보라표목 세팅
	sRefItem = _T("");
	CString sDobora = _T("");

	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("500$a"), sRefItem );
	if(sRefItem.IsEmpty() == FALSE) sDobora = sRefItem;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("510$a"), sRefItem );
	if(sRefItem.IsEmpty() == FALSE) sDobora = sRefItem;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("511$a"), sRefItem );
	if(sRefItem.IsEmpty() == FALSE) sDobora = sRefItem;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("530$a"), sRefItem );
	if(sRefItem.IsEmpty() == FALSE) sDobora = sRefItem;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("550$a"), sRefItem );
	if(sRefItem.IsEmpty() == FALSE) sDobora = sRefItem;

	if( sDobora.IsEmpty() == TRUE )
		return;

	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("500"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("510"));
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("511"));	
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("530"));	
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("550"));	
	
	if ( iACCode == 0)
	{
		if ( m_ACMODE == AUTHOR_MODE )
		{
			m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("500$a"), sDobora );
		}
		else if ( m_ACMODE == TITLE_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("530$a"), sDobora );
		else if ( m_ACMODE == SUBJECT_MODE ) m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("550$a"), sDobora);
	}
	else if ( iACCode == 1 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("510$a"), sDobora );
	}
	else if ( iACCode == 2 )
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("511$a"), sDobora );
	}
	
	return;

EFS_END
}

VOID CMNG_AC_MANAGER_03::OnSelchangecAcCode() 
{
EFS_BEGIN

	//참조표목 Marc에 입력
	INT iSelInfoSource = m_pMCombo->GetCurSel();
	if( iSelInfoSource == 0 )
		m_sACType = _T("0");
	else if( iSelInfoSource == 1 )
		m_sACType = _T("1");
	else if( iSelInfoSource == 2 )
		m_sACType = _T("2");
	
	//Marc에 반영
	SetACMarc ( iSelInfoSource );

	Display();
	return;

EFS_END	
}

VOID CMNG_AC_MANAGER_03::OnChangeeSummery() 
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
	return;
	
EFS_END	
}

VOID CMNG_AC_MANAGER_03::OnbCLEARSCREEN() 
{
EFS_BEGIN

	m_pDM->FreeData();

	//참조표목 Marc에 입력
	INT iSelInfoSource = m_pMCombo->GetCurSel();

	//전거표기 기호 초기화
	m_pMCombo->SetCurSel(0);
	
	//정보원
	m_pInfoSourceCombo->SetCurSel(0);

	//008 Tag삭제
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("008"));

	//정보원 설정 초기화
	SetInfoSource();

	//Marc에 반영
	SetACMarc ( iSelInfoSource );

	Display();
	return;
EFS_END	
}

BOOL CMNG_AC_MANAGER_03::IsCheckSaveInputValue()
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

VOID CMNG_AC_MANAGER_03::OnChangeeSelItem() 
{
EFS_BEGIN

	CString sSelItem = _T("");
	GetDlgItem(IDC_eSelItem)->GetWindowText(sSelItem);

	m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("100$a"));
	m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("110$a"));
	m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("111$a"));
	m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("130$a"));
	m_pInfo->pMarcMgr->DeleteItem(m_pMarc, _T("150$a"));

	//정보원 선택 변경
	INT iSelInfoSource = m_pMCombo->GetCurSel();
	
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
	
	//빈테그 삭제
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);

	Display();
	return;

EFS_END	
}
///////////////////////////////////////////////////////////////////////
// 메세지 보내기 
// Modify Or Calcel
BOOL CMNG_AC_MANAGER_03::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_RETURN ) {
			NotifyModify(0);
			return TRUE;
			
		} else if ( pMsg->wParam == VK_ESCAPE ) {
			NotifyCancel();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////
// 변경 메세지를 받고 처리 할 함수
VOID CMNG_AC_MANAGER_03::NotifyModify(UINT nChar)
{
	SendMessage(WM_ACEDIT_MODIFY, 1, nChar);
}

///////////////////////////////////////////////////////////////////////
// 취소 메세지를 받고 처리할 함수
VOID CMNG_AC_MANAGER_03::NotifyCancel()
{
	SendMessage(WM_ACEDIT_MODIFY, 0, 0);
}

///////////////////////////////////////////////////////////////////////
// 마크 에디트 업데이트 WM_ACEDIT_MODIFY 
UINT CMNG_AC_MANAGER_03::OnACEditUpdate(WPARAM wParam, LPARAM lParam)
{
EFS_BEGIN
		
	return 0;	
EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// 전거 마크 삭제
VOID CMNG_AC_MANAGER_03::DeleteACMarcField()
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

///////////////////////////////////////////////////////////////////////
// 전거 표기기호 콤보 박스 셋팅
VOID CMNG_AC_MANAGER_03::SetAcCombo()
{
EFS_BEGIN

	// 040915_HSS 수정 Init시 에러(함수백업-아래) ++++++++++++++++++++
	m_pACTypeCombo = new CJComboBox;
	m_pACTypeCombo->SubclassDlgItem( IDC_cACKind, this );
	m_pACTypeCombo->FormatList( 1, DT_LEFT, false , EDGE );
	m_pACTypeCombo->RequireFlag = FALSE;

	m_pMCombo = new CJComboBox;
	m_pMCombo->SubclassDlgItem( IDC_cAcCode, this );

	INT defaultCel = 0;
	CString code, desc;
	
	m_pACTypeCombo->AddItem( _T("저자명"), 0, 0 );
	m_pACTypeCombo->AddItem( _T("서명"), 0, 1 );
	m_pACTypeCombo->AddItem( _T("주제명"), 0, 2 );

	if ( m_ACMODE == AUTHOR_MODE )
	{
		m_pACTypeCombo->SetCurSel( 0 );
		//GetDlgItem(IDC_bInput)->SetWindowText(_T("저자명"));
	}
	else if ( m_ACMODE == TITLE_MODE )
	{
		m_pACTypeCombo->SetCurSel( 1 );
	}		
	else if ( m_ACMODE == SUBJECT_MODE )
	{
		m_pACTypeCombo->SetCurSel( 2 );
	}
	else 
	{
		//관리에서 호출할대는 전거모드를 세팅안하기때문에 강제로 서명으로 초기화한다.
		GetDlgItem(IDC_cACKind)->EnableWindow(TRUE);
		m_pACTypeCombo->SetCurSel( 0 );
		m_ACMODE = AUTHOR_MODE;

	}
	// ----------------------------------------------------------

	return;
EFS_END
}

///////////////////////////////////////////////////////////////////////
// 정보원 콤보박스 셋팅
VOID CMNG_AC_MANAGER_03::SetInfoSourceCombo()
{
EFS_BEGIN
	if ( m_pInfoSourceCombo ) m_pInfoSourceCombo = NULL;

	m_pInfoSourceCombo = new CJComboBox;
	m_pInfoSourceCombo->SubclassDlgItem( IDC_cInfoSource, this );
	m_pInfoSourceCombo->FormatList( 2, DT_LEFT, false , EDGE );
	m_pInfoSourceCombo->RequireFlag = FALSE;

	m_pInfoSourceCombo->AddItem( _T("0"), 0, 0 );
	m_pInfoSourceCombo->AddItem( _T("적용안함"), 1, 0 );

	m_pInfoSourceCombo->AddItem( _T("1"), 0, 1 );
	m_pInfoSourceCombo->AddItem( _T("백과사전"), 1, 1 );

	m_pInfoSourceCombo->AddItem( _T("2"), 0, 2 );
	m_pInfoSourceCombo->AddItem( _T("영어사전"), 1, 2 );

	m_pInfoSourceCombo->AddItem( _T("3"), 0, 3 );
	m_pInfoSourceCombo->AddItem( _T("서명"), 1, 3 );

	m_pInfoSourceCombo->SetCurSel( 0 );

	GetDlgItem(IDC_eInfoSource)->EnableWindow(TRUE);

	return;
EFS_END
}

//도보라 버튼 클릭 이벤트
// 도보라는 채택표목에 해당하는 Marc를 찾고 만약 있다면 
// 해당 마크를 채택할 수 있다.
VOID CMNG_AC_MANAGER_03::OnbDOBORA() 
{
EFS_BEGIN
	
	ShowDoboraDlg();
	return;
EFS_END
}

// 도보라 검색창 띄우기
VOID CMNG_AC_MANAGER_03::ShowDoboraDlg()
{
EFS_BEGIN

	GetDlgItem(IDC_eSelItem)->GetWindowText(m_sRefItem);
	if( m_sRefItem.IsEmpty() == TRUE )
	{
		AfxMessageBox(_T("조회 대상 표목을 입력하십시오"));
		return;
	}

	CMNG_AC_MANAGER_DOBORA dlg(this);

	dlg.SetRefItem(m_sRefItem);

	if ( m_ACMODE == AUTHOR_MODE) 
		dlg.SetACMode(AUTHOR_MODE);
	if ( m_ACMODE == TITLE_MODE) 
		dlg.SetACMode(TITLE_MODE);
	if ( m_ACMODE == SUBJECT_MODE) 
		dlg.SetACMode(SUBJECT_MODE);


	if( IDOK == dlg.DoModal() )
	{// 도보라조회시 확인을 누르면 도보라태그에 추가가된다.
	 // 참조표목은 4xx에 추가되고 채택표목은 5xx에 추가된다.
     // 도보라마크가 추가되면서 기본 grid도 추가되어야 한다. 
	    CString sMarc, str;
		CMarc marc;
		INT nACType, idxCnt;
		idxCnt = 0;
		sMarc = dlg.GetMarc();
		nACType = dlg.GetACType();

		m_pInfo->pMarcMgr->Decoding(sMarc,&marc);

//		CArray < CString, CString& > idxArray;

		switch(nACType)
		{
			case 0 :
				
				SetACMarc(0);

				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("100$a"), str );
				if( idxCnt > 0 )
					m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("500$a"), str );
				
/*				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("400$a"), str, &idxArray );
				for( i = 0; i < idxCnt; i++ )
					m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("500$a"), str );
				idxArray.RemoveAll();
				
				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("500$a"), str, &idxArray );
				for( i = 0; i < idxCnt; i++ )
				idxArray.RemoveAll();
*/
				break;

			case 1 :
				
				SetACMarc(1);
				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("110$a"), str );
				if( idxCnt > 0 )
					m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("510$a"), str );

/*				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("410$a"), str, &idxArray );
				for( i = 0; i < idxCnt; i++ )
				idxArray.RemoveAll();

				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("510$a"), str, &idxArray );
				for( i = 0; i < idxCnt; i++ )
					
				idxArray.RemoveAll();
*/
				break;
				
			case 2 :
				
				SetACMarc(2);
				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("111$a"), str);
				if( idxCnt > 0 )
					m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("511$a"), str );
/*					
				idxArray.RemoveAll();

				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("411$a"), str, &idxArray);
				for( i = 0; i < idxCnt; i++ )
					
				idxArray.RemoveAll();

				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("511$a"), str, &idxArray);
				for( i = 0; i < idxCnt; i++ )
					
				idxArray.RemoveAll();
*/
				break;

			case 3 :
				
				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("130$a"), str );
				if( idxCnt > 0 )
					m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("530$a"), str );	
/*
				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("430$a"), str, &idxArray );
				for( i = 0; i < idxCnt; i++ )
					
				idxArray.RemoveAll();

				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("530$a"), str, &idxArray );
				for( i = 0; i < idxCnt; i++ )
					
				idxArray.RemoveAll();
*/
				break;

			case 4 :

				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("150$a"), str );
				if( idxCnt > 0 )
					m_pInfo->pMarcMgr->SetItem( m_pMarc, _T("550$a"), str );
/*
				m_title450Cnt = idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("450$a"), str, &idxArray );
				for( i = 0; i < idxCnt; i++ )
					
				idxArray.RemoveAll();

				idxCnt = m_pInfo->pMarcMgr->GetItem( &marc, _T("530$a"), str, &idxArray );
				for( i = 0; i < idxCnt; i++ )
					
				idxArray.RemoveAll();
*/
				break;
		}
		
		m_bDobora = TRUE;
		Display();
	}
	return;

EFS_END
}

///////////////////////////////////////////////////////////////////////
// 전거 저장
BOOL CMNG_AC_MANAGER_03::InsertToDB()
{
EFS_BEGIN
	INT ids = -1;

	CString sACMasterReckey;
	m_pDM->MakeRecKey(sACMasterReckey);

	if ( !InsertACTBL(sACMasterReckey) ) return FALSE;

	if ( !InsertACMASTERTBL(sACMasterReckey) ) return FALSE;
	// MASTER TBL에 쿼리 생성해서 실행
	// 이유 : 전거 테이블(서명, 저자, 주제어)에 정상적으로 입력이 되었을 경우 MASTER TABLE에 입력한다.
		
	return TRUE;

EFS_END
return FALSE;
}

BOOL CMNG_AC_MANAGER_03::InsertACMASTERTBL(CString sACMasterKey)
{
EFS_BEGIN

	INT ids = -1;

	ids = m_pDM->StartFrame();
	if ( ids < 0 ) return FALSE;

	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("REC_KEY"), sACMasterKey , 0);

	//전거 제어번호 구하기 
	CString sACConNo = GetAcControlNo();
	if ( sACConNo == _T("") ) return FALSE;
	m_pDM->SetCellData(_T("AC_CONTROL_NO"), sACConNo , 0);

	//MARC에 제어번호 입력
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("001"));
	m_pInfo->pMarcMgr->InsertField(m_pMarc, _T("001") + sACConNo);

	
	// 최종수정정보 생성 040
	CString strLibCode;
	CString strItemData;

	// 도서관 부호 가져오기
	CCatApi_BO *pCatApi = NULL;
	pCatApi = new CCatApi_BO(this);

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

	// STREAM MARC 입력
	CString sStreamMarc;
	m_pInfo->pMarcMgr->Encoding(m_pMarc, sStreamMarc );
	m_pDM->SetCellData(_T("MARC"), sStreamMarc , 0);

	//전거형식 구하기

	CString ACType = GetACType();
	if ( ACType == _T("") ) return FALSE;
	
	m_pDM->SetCellData(_T("AC_TYPE"), ACType , 0);
	m_pDM->SetCellData(_T("AC_CLASS"), _T("") , 0);
	
	if(m_bDobora)
		m_pDM->SetCellData(_T("SEE_ALSO_REFERENCE_YN"), _T("Y") , 0);
	else
		m_pDM->SetCellData(_T("SEE_ALSO_REFERENCE_YN"), _T("N") , 0);

	CTime t = CTime::GetCurrentTime();
	CString strMsg;
	strMsg.Format(_T("%04d%02d%02d %02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute());

	
	m_pDM->SetCellData(_T("INPUT_DATE"), strMsg , 0);
	m_pDM->SetCellData(_T("UPDATE_DATE"), strMsg , 0);
	
	m_pDM->SetCellData(_T("FIRST_WORKER"), m_pInfo->USER_ID , 0);
	m_pDM->SetCellData(_T("LAST_WORKER"), m_pInfo->USER_ID , 0);

	m_pDM->SetCellData(_T("FIRST_WORK"), GetWorkLogMsg(_T("전거입력"),WORK_LOG) , 0);
	m_pDM->SetCellData(_T("LAST_WORK"), GetWorkLogMsg(_T("전거입력"),WORK_LOG) , 0);
	
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	ids = m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM);
	if ( ids < 0) return FALSE;
	
	ids = m_pDM->SendFrame();
	if ( ids < 0) return FALSE;
	
	ids = m_pDM->EndFrame();
	if ( ids < 0 ) return FALSE;

	return true;

EFS_END
return false;
}

//채택 표목별로 인덱스 테이블에 저장한다.
BOOL CMNG_AC_MANAGER_03::InsertACTBL(CString sACMasterKey)
{
EFS_BEGIN

	INT ids = -1;
	CESL_DataMgr *pDM = NULL;
	if ( m_ACMODE == AUTHOR_MODE )
		pDM = FindDM( _T("DM_MNG_AC_MANAGER_AUTHOR") );
	else if ( m_ACMODE == TITLE_MODE )
		pDM = FindDM( _T("DM_MNG_AC_MANAGER_TITLE") );
	else if ( m_ACMODE == SUBJECT_MODE )
		pDM = FindDM( _T("DM_MNG_AC_MANAGER_SUBJECT") );

	if ( pDM == NULL ) return FALSE;

	pDM->FreeData();

	ids = pDM->StartFrame();
	if ( ids < 0 ) return FALSE;

	INT nCnt = m_pDM->GetRowCount();

	CString sACReckey;
	

	for( INT i = 0; i <= nCnt ; i++ )
	{	
		m_pDM->MakeRecKey(sACReckey);
		pDM->InsertRow(-1);
		pDM->SetCellData(_T("REC_KEY"), sACReckey , i);
		pDM->SetCellData(_T("AC_MASTER_MAT_KEY"), sACMasterKey , i);

		CString sKeyWordTmp = _T("");
		
		if( i == nCnt )
		{//채택표목이면
			GetDlgItem(IDC_eSelItem)->GetWindowText(sKeyWordTmp);
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

//생몰년이 수정되었을 경우
VOID CMNG_AC_MANAGER_03::OnChangeeYear() 
{
EFS_BEGIN

	CString sYearItem = _T("");
	GetDlgItem(IDC_eYear)->GetWindowText(sYearItem);

	CString sTmpItem = _T("") ;
	
	if( m_ACMODE == AUTHOR_MODE )
	{
		//INT nCurSel = m_pACTypeCombo->GetCurSel();

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
	return;
EFS_END	
}

//전거제어번호를 얻는다.
CString CMNG_AC_MANAGER_03::GetAcControlNo()
{
EFS_BEGIN

	CTime t = CTime::GetCurrentTime();

	CString sYear;
	sYear.Format(_T("%04d"), t.GetYear());

	CString strQuery;
	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE MAKE_YEAR = '%s' AND DIVISION_VALUE = 'KAC' AND KIND_CODE = 'CON_NO'"), sYear );

	//제어번호 검색
	CString sACConNo = _T("");
	m_pDM->GetOneValueQuery(strQuery, sACConNo);	

	if ( sACConNo == _T("") )
	{
		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR, DIVISION_VALUE, LAST_NUMBER, CREATE_DATE, CREATE_WORKER, FIRST_WORK, LAST_WORK) VALUES ( ESL_SEQ.NEXTVAL, 'CON_NO', 'CO', '%s', 'KAC', '1', '','','','');"), sYear );
		m_pDM->AddFrame(strQuery);
		sACConNo = _T("1");
	}
	else {
		INT iTmp = _ttoi(sACConNo);
		iTmp++;
		sACConNo.Format(_T("%d"), iTmp);

		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s' WHERE KIND_CODE = 'CON_NO' AND  PUBLISH_FORM_CODE = 'CO' AND MAKE_YEAR = '%s' AND DIVISION_VALUE = 'KAC' ;"), sACConNo, sYear );
		m_pDM->AddFrame(strQuery);
	}

	CString sTmp0 = _T("");
	for ( INT i = sACConNo.GetLength() ; i < 9 ; i ++)
	{
		sTmp0 += _T("0");
	}
	sACConNo = _T("KAC") + sTmp0 + sACConNo;

	return sACConNo;

EFS_END
return _T("");
}

//전거 종류를 얻는다.
CString CMNG_AC_MANAGER_03::GetACType()
{
EFS_BEGIN
	//저자 :개인
	CString sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("100$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("0");
	//저자 :단체
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("110$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("1");
	//저자 :그룹
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("111$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("2");
	//저자 :서명
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("130$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("3");
	//저자 :주제명
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("150$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("4");


EFS_END
return _T("");
}

//전거종류 콤보박스 변경 시 이벤트
VOID CMNG_AC_MANAGER_03::OnSelchangecACKind() 
{
EFS_BEGIN

	INT iSelInfoSource; 
	iSelInfoSource = m_pACTypeCombo->GetCurSel();

	m_pMCombo->ResetContent();

	m_pMCombo->FormatList( 2, DT_LEFT, false , EDGE );
	m_pMCombo->RequireFlag = FALSE;

	switch( iSelInfoSource )
	{
	case 0 :
		m_ACMODE = AUTHOR_MODE;

		m_pMCombo->AddItem( _T("100"), 0, 0 );
		m_pMCombo->AddItem( _T("개인명"), 1, 0 );
		
		m_pMCombo->AddItem( _T("110"), 0, 1 );
		m_pMCombo->AddItem( _T("단체명"), 1, 1 );
		
		m_pMCombo->AddItem( _T("111"), 0, 2 );
		m_pMCombo->AddItem( _T("그룹명"), 1, 2 );
		
		m_pMCombo->SetCurSel( 0 );

		GetDlgItem(IDC_eYear)->EnableWindow(TRUE);

		break;
	case 1 :
		m_ACMODE = TITLE_MODE;

		m_pMCombo->AddItem( _T("130"), 0, 0 );
		m_pMCombo->AddItem( _T("서명"), 1, 0 );	
		m_pMCombo->SetCurSel( 0 );

		GetDlgItem(IDC_eYear)->EnableWindow(FALSE);

		break;
	case 2 :
		m_ACMODE = SUBJECT_MODE;

		m_pMCombo->AddItem( _T("150"), 0, 0 );
		m_pMCombo->AddItem( _T("주제명"), 1, 0 );
		m_pMCombo->SetCurSel( 0 );

		GetDlgItem(IDC_eYear)->EnableWindow(FALSE);

		break;
	default :
		return;
		break;
	}
	
	SetACMarc(0);
	return;
EFS_END
}

VOID CMNG_AC_MANAGER_03::OnChangeeInfoSource() 
{
	CString sInfoSource = _T("");
	GetDlgItem(IDC_eInfoSource)->GetWindowText(sInfoSource);

	m_pInfo->pMarcMgr->DeleteField(m_pMarc,_T("670"));

	m_pInfo->pMarcMgr->SetItem(m_pMarc,_T("670$a"),sInfoSource);

	//빈테그 삭제
	m_pInfo->pMarcMgr->DeleteEmptySubfield(m_pMarc);
	m_pInfo->pMarcMgr->DeleteEmptyField(m_pMarc);

	Display();
	
}

HBRUSH CMNG_AC_MANAGER_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
