// EnvironComboBox_MultiState.cpp : implementation file
//

#include "stdafx.h"
#include "EnvironComboBox_MultiState.h"
#include "ChangeValue.h"
#include "SettingGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvironComboBox_MultiState

CEnvironComboBox_MultiState::CEnvironComboBox_MultiState()
{
}

CEnvironComboBox_MultiState::~CEnvironComboBox_MultiState()
{
}


BEGIN_MESSAGE_MAP(CEnvironComboBox_MultiState, CComboBox)
	//{{AFX_MSG_MAP(CEnvironComboBox_MultiState)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvironComboBox_MultiState message handlers

VOID CEnvironComboBox_MultiState::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	if(m_pDM == NULL && m_nIdx < 0) return ;
	CString strData;
	this->GetWindowText(strData);
	((CESL_Grid*)m_pParentWnd)->SetAt(m_nIdx, 3, strData);

	CString sAbility;
	m_pDM->GetCellData( _T("기능"), m_nIdx, sAbility );
	
	// 해당기능의 설정값의 설명을 코드로 변경한다
	CChangeValue *pMng = new CChangeValue();
	CString sCode = pMng->DescToCode( sAbility, strData );

	// DM에 코드값 저장
	m_pDM->SetCellData(_T("설정값"), sCode, m_nIdx);

	CSettingGrid *pSet = new CSettingGrid();
	if( m_nMng == 1 )
		pSet->SettingGrid01(m_pDM, m_pGrid);
	else if( m_nMng == 2 )
		pSet->SettingGrid02(m_pDM, m_pGrid);

	delete pSet;
	pSet = NULL;

	delete pMng;
	pMng = NULL;
	
}

VOID CEnvironComboBox_MultiState::OnKillfocus() 
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_HIDE);
}

BOOL CEnvironComboBox_MultiState::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CComboBox::PreCreateWindow(cs);
}

VOID CEnvironComboBox_MultiState::Init(CStringArray *pData/*=NULL*/)
{
	if(pData == NULL) return;
	this->ResetContent();
	INT cnt = pData->GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		AddString(pData->GetAt(i));
	}
}

VOID CEnvironComboBox_MultiState::SetCurSelData(CString strData)
{
	CString sAbility;
	m_pDM->GetCellData( _T("기능"), m_nIdx, sAbility );

	// 해당기능의 설정값의 코드를 설명으로 바꾼다
	CChangeValue *pMng = new CChangeValue();
	CString sDesc = pMng->CodeToDesc( sAbility, strData );

	// 해당 설명과 같은 위치로 콤보박스 SetCurSel한다
	INT nIdx = this->FindString(0, sDesc);
	if(nIdx >= 0)
		this->SetCurSel(nIdx);
	else
		this->SetCurSel(0);

	delete pMng;
	pMng = NULL;
}

VOID CEnvironComboBox_MultiState::InitMngLocMgr(INT nMng)
{
	m_nMng = nMng;
}
