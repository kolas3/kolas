// UserList.cpp: implementation of the CUserList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccessControl.h"
#include "UserList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "ESL_DataMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserList::CUserList()
{

}

CUserList::~CUserList()
{
	FreeData();
}

VOID CUserList::AddUser(CString name)
{
	CString *pName = new CString;
	name.TrimLeft();			name.TrimRight();
	pName->Format(_T("%s"), name);
	USER_LIST.AddTail((CObject*)pName);
	return;
}
VOID CUserList::AddMethod(CString name)
{
	CString *pName = new CString;
	name.TrimLeft();			name.TrimRight();
	pName->Format(_T("%s"), name);
	METHOD_LIST.AddTail((CObject*)pName);
	return;
}


INT CUserList::FreeData(VOID)
{
	INT count = USER_LIST.GetCount();
	INT count_method = METHOD_LIST.GetCount();

	if (count == 0 && count_method ) return 0;
	CString * pName;
	for (INT i = 0; i < count; i++) {
		pName = (CString*)USER_LIST.RemoveHead();
		if (pName) delete pName;
	}
	USER_LIST.RemoveAll();

	if (count_method == 0) return 0;
	for ( i = 0; i < count_method; i++) {
		pName = (CString*)METHOD_LIST.RemoveHead();
		if (pName) delete pName;
	}
	METHOD_LIST.RemoveAll();


	return(count);
}

INT CUserList::GetCount(VOID)
{
	return USER_LIST.GetCount();
}
INT CUserList::GetMethodCount(VOID)
{
	return METHOD_LIST.GetCount();
}

CString CUserList::GetAt(INT idx)
{
	POSITION pos;
	pos = USER_LIST.FindIndex(idx);
	if (!pos) return _T("");

	CString *pName = (CString*)USER_LIST.GetAt(pos);
	CString result;
	result.Format(_T("%s"), pName->GetBuffer(0));
	return result;
}

CString CUserList::GetMethodAt(INT idx)
{
	POSITION pos;
	pos = METHOD_LIST.FindIndex(idx);
	if (!pos) return _T("");

	CString *pName = (CString*)METHOD_LIST.GetAt(pos);
	CString result;
	result.Format(_T("%s"), pName->GetBuffer(0));
	return result;
}

INT CUserList::Refresh(CString method_name)
{
	ReadCfgFile.ReadCfgFile(_T(""));
	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return -1;
	pDM->SetDB(ReadCfgFile.m_sDBKIND);
	pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	// 메소드를 선택한 경우는 해당 메소드를 사용하는 유져 리스트를 읽어온다.
	CString query;
	
	if( method_name == _T("Administrator"))
	{
		query.Format(_T("SELECT USER_ID FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' "));
		pDM->RestructDataManager(query);
		FreeData();
		INT nRow = pDM->GetRowCount();
		for (INT i = 0; i < nRow; i++) {
			AddUser(pDM->GetCellData(i, 0));
		}

		query.Format(_T("SELECT METHOD_NAME FROM ESL_MENU_METHOD_TBL WHERE FLAG='M' "));
		pDM->RestructDataManager(query);
		nRow = pDM->GetRowCount();
		for (i = 0; i < nRow; i++) {
			AddMethod(pDM->GetCellData(i, 0));
		}


	}
	else	
	{
		query.Format(_T("SELECT USER_ID FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' AND USER_METHOD_NAME='%s'"), method_name);
		pDM->RestructDataManager(query);
		FreeData();
		INT nRow = pDM->GetRowCount();
		for (INT i = 0; i < nRow; i++) {
			AddUser(pDM->GetCellData(i, 0));
		}
	}

	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
	return 0;
}




