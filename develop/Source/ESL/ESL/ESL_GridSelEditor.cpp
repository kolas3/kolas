// ESL_GridSelEditor.cpp : implementation file
//

#include "stdafx.h"
#include "ESL_GridSelEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CESL_GridSelEditor

CESL_GridSelEditor::CESL_GridSelEditor()
{
}

CESL_GridSelEditor::~CESL_GridSelEditor()
{
}


BEGIN_MESSAGE_MAP(CESL_GridSelEditor, CEdit)
	//{{AFX_MSG_MAP(CESL_GridSelEditor)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESL_GridSelEditor message handlers
BOOL CESL_GridSelEditor::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_RETURN ) {
			NotifyEdit();
			return true;
			
		} else if ( pMsg->wParam == VK_ESCAPE ) {
			NotifyCancel();
			return true;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}

VOID CESL_GridSelEditor::OnKillfocus() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	NotifyEdit();

EFS_END
}


VOID CESL_GridSelEditor::NotifyEdit()
{
EFS_BEGIN

	::PostMessage(parent->GetSafeHwnd(), ESL_GRID_MODIFY, 1, 0);

EFS_END
}

VOID CESL_GridSelEditor::NotifyCancel()
{
EFS_BEGIN

	::PostMessage(parent->GetSafeHwnd(), ESL_GRID_MODIFY, 0, 0);

EFS_END
}



BOOL CESL_GridSelEditor::PreCreateWindow(CREATESTRUCT& cs) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CEdit::PreCreateWindow(cs);

EFS_END
return FALSE;

}
