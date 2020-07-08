// MarcEditor.cpp: implementation of the CMarcEditor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcEditor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcEditor::CMarcEditor(CMarcMgr* pMarcMgr /* = NULL */)
{
	m_pParentWnd = NULL;

	m_pMarc = NULL;
	m_pMarcMgr = pMarcMgr;

	m_pInfo = NULL;

	m_pMarcEditCtrl = NULL;
	m_pMarcGridCtrl = NULL;

	m_hWndExample = NULL;
	m_hWndLeader = NULL;
}

CMarcEditor::~CMarcEditor()
{
	if (m_pMarcEditCtrl != NULL)
	{
		delete m_pMarcEditCtrl;
		m_pMarcEditCtrl = NULL;
	}

	if (m_pMarcGridCtrl != NULL)
	{
		delete m_pMarcGridCtrl;
		m_pMarcGridCtrl = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// Operations
VOID CMarcEditor::Init(CMarcMgr* pMarcMgr, CMarc* pMarc, CESL_Information *pInfo /*= NULL*/)
{
	SetMarcMgr(pMarcMgr);
	SetMarc(pMarc);
	SetESLInfo(pInfo);

	if (m_pMarcEditCtrl) m_pMarcEditCtrl->Init(m_pMarcMgr, pMarc);
	if (m_pMarcGridCtrl) m_pMarcGridCtrl->Init(m_pMarcMgr, pMarc);
}

BOOL CMarcEditor::SubclassDlgItem(UINT nEditID, UINT nGridID, CWnd* pParentDlg, CWnd *pSuperParentWnd /*= NULL */)
{
	if (pParentDlg == NULL) return FALSE;

	if (pSuperParentWnd)
		m_pParentWnd = pSuperParentWnd;
	else
		m_pParentWnd = pParentDlg;	

	if (nEditID != -1)
	{
		if (m_pMarcEditCtrl == NULL)
		{
			m_pMarcEditCtrl = new CMarcEditCtrl;

			if (!m_pMarcEditCtrl->SubclassDlgItem(nEditID, pParentDlg)) return FALSE;
			
			m_pMarcEditCtrl->SetParentEditor(this);
		}
		else
			return FALSE;

	}
	
	if (nGridID != -1)
	{
		if (m_pMarcGridCtrl == NULL)
		{
			m_pMarcGridCtrl = new CMarcGridCtrl;

			if (!m_pMarcGridCtrl->SubclassDlgItem(nGridID, pParentDlg)) return FALSE;
			
			m_pMarcGridCtrl->SetParetEditor(this);
		}
		else
			return FALSE;
	}

	return TRUE;
}

VOID CMarcEditor::InitUserAlias()
{
	if (m_pMarcGridCtrl) m_pMarcGridCtrl->InitUserAlias();
}

VOID CMarcEditor::AddUserAlias(CString strAlias)
{
	if (m_pMarcGridCtrl) m_pMarcGridCtrl->AddUserAlias(strAlias);
}

VOID CMarcEditor::InitLimitTag()
{
	if (m_pMarcEditCtrl) m_pMarcEditCtrl->InitLimitTag();
	if (m_pMarcGridCtrl) m_pMarcGridCtrl->InitLimitTag();
}

VOID CMarcEditor::AddLimitTag(CString strTagCode, CString strIndicator /*= _T("__")*/)
{
	if (m_pMarcEditCtrl) m_pMarcEditCtrl->AddLimitTag(strTagCode, strIndicator);
	if (m_pMarcGridCtrl) m_pMarcGridCtrl->AddLimitTag(strTagCode);
}

VOID CMarcEditor::Display()
{
	if (m_pMarcMgr == NULL || m_pMarc == NULL) return;

	m_pMarcMgr->SortMarc(m_pMarc);

	if (m_pMarcEditCtrl) m_pMarcEditCtrl->Display();
	if (m_pMarcGridCtrl) m_pMarcGridCtrl->Display();
	if (m_hWndLeader) 
	{
		CString strLeader;
		m_pMarc->GetLeader(strLeader);
		::SetWindowText(m_hWndLeader, strLeader);
	}
}

INT CMarcEditor::ApplyEdit(INT nMode /*= 0*/)
{
	if (m_pMarcEditCtrl) 
		return m_pMarcEditCtrl->ApplyEdit(nMode);
	else 
		return -1;
}

VOID CMarcEditor::UpdateEditCtrls(INT nCtrl)
{
	if (nCtrl == GRID_CTRL)
	{
		if (m_pMarcEditCtrl) 
			m_pMarcEditCtrl->Display();
	}
	else if (nCtrl == EDIT_CTRL)
	{
		if (m_pMarcGridCtrl) 
			m_pMarcGridCtrl->Display();
	}	

	// 부모에게 에디터가 업데이트됨을 알린다.
	if (m_pParentWnd)
	{

/* 수정 사항 : 그리드 깜빡거림을 없앰 +++
 * 수정 날짜 : 2003.08.20
 * 수정 사람 : 김홍수 
 */

		/*
		if (nCtrl == GRID_CTRL)
			m_pParentWnd->PostMessage(WM_GRID_UPDATE, 0, 0);
		else if (nCtrl == EDIT_CTRL)
			m_pParentWnd->PostMessage(WM_EDIT_UPDATE, 0, 0);
		*/

		if (nCtrl == EDIT_CTRL)
			m_pParentWnd->PostMessage(WM_EDIT_UPDATE, 0, 0);


// 수정 사항 : 그리드 깜빡거림을 없앰 +++

	}
}

VOID CMarcEditor::ShowExample(CString strExample)
{
	if (m_hWndExample == NULL) return;
	::SetWindowText(m_hWndExample, strExample);
}


VOID CMarcEditor::SetGridFocus()
{		
	if (m_pMarcGridCtrl) 
	{
		m_pMarcGridCtrl->SendMessage(WM_LBUTTONDOWN, NULL, NULL);
	}

}
VOID CMarcEditor::SetEditFocus()
{
	if (m_pMarcEditCtrl) m_pMarcEditCtrl->SetFocus();
}

VOID CMarcEditor::SetVolFocus()
{
	m_pParentWnd->SendMessage(WM_USER+1000, NULL, NULL);
}

VOID CMarcEditor::MoveAdjustWindow()
{
	if (m_pMarcEditCtrl != NULL ) 
	{
		m_pMarcEditCtrl->MoveMain();
	}
}