// NewButtonList.cpp: implementation of the CNewButtonList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewButtonList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "NewButton.h"
#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewButtonList::CNewButtonList()
{

}

CNewButtonList::~CNewButtonList()
{
	FreeData();
}

INT CNewButtonList::FreeData(VOID)
{
EFS_BEGIN

	INT count = LIST.GetCount();
	if (count == 0) return 0;
	CNewButton *pButton;
	for (INT i = 0; i < count; i++) {
		pButton = NULL;
		pButton = (CNewButton*)LIST.RemoveHead();
		if (pButton) delete pButton;
	}
	LIST.RemoveAll();
	return 0;

EFS_END
return -1;

}

INT CNewButtonList::Add(UINT ButtonID, CWnd *pParent, UINT UP, UINT DOWN, UINT FOCUS)
{
EFS_BEGIN

	CNewButton *pButton = new CNewButton;
	if (!pButton) return -1;
	pButton->SubclassDlgItem(ButtonID, pParent);
	pButton->LoadBitmap(UP, DOWN, FOCUS);

	LIST.AddTail((CObject*)pButton);
	return 0;

EFS_END
return -1;

}

