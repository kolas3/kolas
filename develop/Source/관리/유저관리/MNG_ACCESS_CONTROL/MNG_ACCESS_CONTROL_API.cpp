// MNG_ACCESS_CONTROL_API.cpp: implementation of the CMNG_ACCESS_CONTROL_API class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNG_ACCESS_CONTROL_API.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMNG_ACCESS_CONTROL_API::CMNG_ACCESS_CONTROL_API()
{
	m_nTopBaseItemData = 1000000;
	m_nTopMenuOrder = 0;
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_수서대상관리메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_구입메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_기증메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_등록메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_정리메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP단행_열람메뉴");

	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_구입메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_기증메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_제본메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_등록메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_정리메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP연속_열람메뉴");

	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_관리메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_대출반납메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_목차초록메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_전거통제메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_분류기호메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_이용자자료찾기메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_업무용자료찾기메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("UP기타_공동목록메뉴");

	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("대출반납메뉴");
	m_strTopMenuOrder[m_nTopMenuOrder++] = _T("관리메뉴");

}

CMNG_ACCESS_CONTROL_API::~CMNG_ACCESS_CONTROL_API()
{

}

INT CMNG_ACCESS_CONTROL_API::ShowMenuTree(CESL_DataMgr *pDM, CTreeCtrl	*pMenuTree, TCHAR cFLAG, CString strName)
{
	if (pMenuTree == NULL) return -1;
	if (pDM == NULL) return -2;

	pMenuTree->DeleteAllItems();

	CString strQuery;

	CESL_DataMgr DM_MENUGROUP, DM_MENU;
	DM_MENUGROUP.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	DM_MENU.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	// 메뉴 그룹 정보를 읽어온다.
	//                        0       1        2
	strQuery.Format(_T("SELECT ALIAS, CAPTION, GROUPTYPE FROM ESL_MENUGROUP_TBL ORDER BY ALIAS"));
	DM_MENUGROUP.RestructDataManager(strQuery);
	INT nRows = DM_MENUGROUP.GetRowCount();

	// 메뉴 정보를 읽어온다.
	//                        0         1        2      3        4      5
	strQuery.Format(_T("SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, ACTION, TARGET FROM ESL_MENU_TBL WHERE ")
					_T("IDX >= 0 AND FLAG='%c' AND USER_ID='%s' ORDER BY IDX"), 
					cFLAG, strName
					);
	DM_MENU.RestructDataManager(strQuery);
	INT nMenuRows = DM_MENU.GetRowCount();

	HTREEITEM hItem, hItemMenu;
	HTREEITEM hRoot = pMenuTree->GetRootItem();
	TVINSERTSTRUCT tvInsert;

	INT i;
	m_arrTopMenuIdx.RemoveAll();
	for (i = 0; i < nRows; i++) {
		// GROUPTYPE -> 2
		if (DM_MENUGROUP.GetCellData(i, 2) == _T("T")) {
			m_arrTopMenuIdx.Add(i);
		}
	}

	CString strMenuGroupCaption, strMenuGroupAlias, strMenu, strMenuAlias;
	INT j;
	INT nDMRowIdx;
	for (i = 0; i < m_nTopMenuOrder; i++) {
		nDMRowIdx = GetTopMenuIdx(&DM_MENUGROUP, m_strTopMenuOrder[i]);
		if (nDMRowIdx < 0) continue;
		//if (DM_MENUGROUP.GetCellData(i, 2) != _T("T")) continue;		// GROUPTYPE
		strMenuGroupCaption = DM_MENUGROUP.GetCellData(nDMRowIdx, 1);		// CAPTION
		strMenuGroupAlias = DM_MENUGROUP.GetCellData(nDMRowIdx, 0);		// ALIAS
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		tvInsert.item.lParam = 0;
		tvInsert.item.pszText = strMenuGroupCaption.GetBuffer(0);
		tvInsert.item.iImage = 0;
		tvInsert.item.iSelectedImage = 0;
				
		hItem = pMenuTree->InsertItem(&tvInsert);
		pMenuTree->SetItemData(hItem, m_nTopBaseItemData+nDMRowIdx);

		for (j = 0; j < nMenuRows; j++) {
			if (DM_MENU.GetCellData(j, 1) == strMenuGroupAlias) {		// GROUP_ALIAS
				strMenu = DM_MENU.GetCellData(j, 3);					// CAPTION
				strMenuAlias = DM_MENU.GetCellData(j, 0);				// ALIAS
				//if (strMenuAlias.Find(_T("작업선택")) >= 0 || strMenuAlias == _T("홈")) continue;
				tvInsert.hParent = hItem;
				tvInsert.hInsertAfter = NULL;
				tvInsert.item.mask = TVIF_TEXT;
				tvInsert.item.lParam = 0;
				tvInsert.item.pszText = strMenu.GetBuffer(0);
				tvInsert.item.iImage = 0;
				tvInsert.item.iSelectedImage = 0;

				hItemMenu = pMenuTree->InsertItem(&tvInsert);
				pMenuTree->SetItemData(hItemMenu, (DWORD)j);

				REFRESH_AddMenuTree(pMenuTree, &DM_MENUGROUP, &DM_MENU, hItemMenu, strMenuAlias);
			}
		}
	}
	return 0;
}

INT CMNG_ACCESS_CONTROL_API::REFRESH_AddMenuTree(CTreeCtrl	*pMenuTree, CESL_DataMgr *pDM_MENUGROUP, CESL_DataMgr *pDM_MENU, HTREEITEM hItemParent, CString strMenuGroupAlias)
{
	if (pDM_MENUGROUP == NULL) return -1;
	INT nRows = pDM_MENUGROUP->GetRowCount();

	if (pDM_MENU == NULL) return -1;
	INT nMenuRows = pDM_MENU->GetRowCount();

	INT j;
	CString strMenu, strMenuAlias;
	TVINSERTSTRUCT tvInsert;
	HTREEITEM hItemMenu;
	for (j = 0; j < nMenuRows; j++) {
		if (pDM_MENU->GetCellData(j, 1) == strMenuGroupAlias) {		// GROUP_ALIAS
			strMenu = pDM_MENU->GetCellData(j, 3);					// CAPTION
			strMenuAlias = pDM_MENU->GetCellData(j, 0);				// ALIAS
			//if (strMenuAlias.Find(_T("작업선택")) >= 0 || strMenuAlias == _T("홈")) continue;
			tvInsert.hParent = hItemParent;
			tvInsert.hInsertAfter = NULL;
			tvInsert.item.mask = TVIF_TEXT;
			tvInsert.item.lParam = 0;
			tvInsert.item.pszText = strMenu.GetBuffer(0);
			tvInsert.item.iImage = 0;
			tvInsert.item.iSelectedImage = 0;

			hItemMenu = pMenuTree->InsertItem(&tvInsert);
			pMenuTree->SetItemData(hItemMenu, (DWORD)j);

			if (pDM_MENU->GetCellData(j, 4) == _T("P")) {			// ACTION
				REFRESH_AddMenuTree(pMenuTree, pDM_MENUGROUP, pDM_MENU, hItemMenu, pDM_MENU->GetCellData(j, 5));		// TARGET
			}
		}
	}

	return 0;
}

// return values
// -10100 : 이미 같은 이름을 가진 method(user) 가 있음
INT CMNG_ACCESS_CONTROL_API::METHOD_New(CESL_DataMgr *pDM, TCHAR cFlag, CString strName, TCHAR cImportSourceFlag, CString strImportSourceName, CProgressCtrl *pBar, BOOL bSendFrame /*=TRUE*/)
{
	INT ids;
	if (pDM == NULL) return -1;
	
	CString strQuery, str, strPK;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	INT i, nRows;

	if (cFlag == 'U') strQuery.Format(_T("SELECT COUNT(*) FROM ESL_MENU_METHOD_TBL WHERE FLAG='U' AND USER_ID='%s'"), strName);
	if (cFlag == 'M') strQuery.Format(_T("SELECT COUNT(*) FROM ESL_MENU_METHOD_TBL WHERE FLAG='M' AND METHOD_NAME='%s'"), strName);
	
	if (pBar) pBar->SetPos(10);

	// 2004년 05월 15일 조병걸 수정
	// 모든 것이 한 트랜잭션이므로 DELETE문도 트랜잭션에 묶여서 
	// 이런 짓을 하면 오류가 나기 때문에 주석 처리
	/*
	str = _T("");
	pDM->GetOneValueQuery(strQuery, str);
	if (String_to_Integer(str) > 0) return -10100;			// 이미 같은 이름을 가진 method(user) 가 있음
	*/

	CString sRecKey;
	// ESL_MENU_METHOD_TBL에 Insert
	str.Format(_T("%c"), cFlag);
	if (strName != _T("Administrator")) {
		if( bSendFrame )
		{
			pDM->StartFrame();
		}

		if( pDM->m_dbkind == 1 )
		{
			sRecKey = _T("ESL_SEQ.NEXTVAL");
		}
		else
		{
			pDM->MakeRecKey(sRecKey);
		}

//		dm.MakeRecKey(strPK);
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("PK"),						_T("NUMERIC"),			sRecKey );
		pDM->AddDBFieldData(_T("FLAG"),						_T("STRING"),			str);
		if (cFlag == 'U') {
			pDM->AddDBFieldData(_T("USER_ID"),				_T("STRING"),			strName);
			pDM->AddDBFieldData(_T("METHOD_NAME"),			_T("STRING"),			_T(""));
			pDM->AddDBFieldData(_T("USER_METHOD_NAME"),		_T("STRING"),			strImportSourceName);
		}
		if (cFlag == 'M') {
			pDM->AddDBFieldData(_T("USER_ID"),				_T("STRING"),			_T(""));
			pDM->AddDBFieldData(_T("METHOD_NAME"),			_T("STRING"),			strName);
			pDM->AddDBFieldData(_T("USER_METHOD_NAME"),		_T("STRING"),			strImportSourceName);
		}

		pDM->MakeInsertFrame(_T("ESL_MENU_METHOD_TBL"));

		if( bSendFrame )
		{
			ids = pDM->SendFrame();
			pDM->EndFrame();
			if (ids < 0) return ids;
		}
	}
	if (strImportSourceName.GetLength() == 0) {
		return ids;
	}
	if (pBar) pBar->SetPos(20);

	if( bSendFrame )
	{
		pDM->StartFrame();
	}
	// ESL_MENU_TBL 을 복사함
	strQuery.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG = '%c' AND USER_ID='%s';") ,cFlag, strName);
	pDM->AddFrame(strQuery);
	//                       0    1        2         3      4        5       6      7        8     9         10               11              12
	strQuery.Format(_T("SELECT PK, ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, FLAG, SPF_USE_FLAG, PROGRAME_USE_FLAG, TITLE_DESC FROM ESL_MENU_TBL WHERE FLAG='%c' AND USER_ID='%s'"), cImportSourceFlag, strImportSourceName);
	dm.RestructDataManager(strQuery);
	nRows = dm.GetRowCount();
	INT nFrames = 0;
	CString strTmp;
	for (i = 0;i < nRows; i++) {

		if( pDM->m_dbkind == 1 )
		{
			sRecKey = _T("ESL_SEQ.NEXTVAL");
		}
		else
		{
			pDM->MakeRecKey(sRecKey);
		}
		//pDM->MakeRecKey(strPK);
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("PK"),					_T("NUMERIC"),			sRecKey);
		str.Format(_T("%c"), cFlag);
		pDM->AddDBFieldData(_T("FLAG"),					_T("STRING"),			str);
		pDM->AddDBFieldData(_T("USER_ID"),				_T("STRING"),			strName);
		pDM->AddDBFieldData(_T("ALIAS"),				_T("STRING"),			dm.GetCellData(i, 1));
		pDM->AddDBFieldData(_T("GROUP_ALIAS"),			_T("STRING"),			dm.GetCellData(i, 2));
		pDM->AddDBFieldData(_T("IDX"),					_T("STRING"),			dm.GetCellData(i, 3));
		pDM->AddDBFieldData(_T("CAPTION"),				_T("STRING"),			dm.GetCellData(i, 4));
		pDM->AddDBFieldData(_T("VISIBLE"),				_T("STRING"),			dm.GetCellData(i, 5));
		pDM->AddDBFieldData(_T("STYLE"),				_T("STRING"),			dm.GetCellData(i, 6));
		pDM->AddDBFieldData(_T("ACTION"),				_T("STRING"),			dm.GetCellData(i, 7));
		pDM->AddDBFieldData(_T("TARGET"),				_T("STRING"),			GetMysqlString(&dm, dm.GetCellData(i, 8)));
		pDM->AddDBFieldData(_T("SPF_USE_FLAG"),			_T("STRING"),			dm.GetCellData(i, 10));
		pDM->AddDBFieldData(_T("PROGRAME_USE_FLAG"),	_T("STRING"),			dm.GetCellData(i, 11));
		pDM->AddDBFieldData(_T("TITLE_DESC"),			_T("STRING"),			dm.GetCellData(i, 12));
		pDM->MakeInsertFrame(_T("ESL_MENU_TBL"));
		nFrames++;

	}

	if( bSendFrame )
	{
		ids = pDM->SendFrame();
		if (ids < 0) return ids;
		pDM->EndFrame();
		pDM->StartFrame();
	}

	if (pBar) pBar->SetPos(60);

	// ESL_TOOLBAR_TBL 을 복사함
	if( bSendFrame )
	{
		pDM->StartFrame();
	}

	strQuery.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE FLAG='%c' AND USER_ID='%s';"), cFlag, strName);
	pDM->AddFrame(strQuery);
	//                         0       1        2             3            4    5      6       7      8    9      10           11            12         13              14              15               16
	strQuery.Format(_T("SELECT USER_ID, ALIAS, MENU_ALIAS, TOOLGROUP_ALIAS, FLAG, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED, TOOLTIP, SPF_USE_FLAG, PROGRAME_USE_FLAG, ACCECLERATOR FROM ESL_TOOLBAR_TBL WHERE FLAG='%c' AND USER_ID='%s'"), cImportSourceFlag, strImportSourceName);
	ids = dm.RestructDataManager(strQuery);

	nFrames = 0;
	nRows = dm.GetRowCount();
	str.Format(_T("%c"), cFlag);
	for (i = 0; i < nRows; i++) {

		if( pDM->m_dbkind == 1 )
		{
			sRecKey = _T("ESL_SEQ.NEXTVAL");
		}
		else
		{
			pDM->MakeRecKey(sRecKey);
		}
//		pDM->MakeRecKey(strPK);
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("PK"),					_T("NUMERIC"),			sRecKey);
		pDM->AddDBFieldData(_T("USER_ID"),				_T("STRING"),			strName);
		pDM->AddDBFieldData(_T("ALIAS"),				_T("STRING"),			dm.GetCellData(i, 1));
		pDM->AddDBFieldData(_T("MENU_ALIAS"),			_T("STRING"),			dm.GetCellData(i, 2));
		pDM->AddDBFieldData(_T("TOOLGROUP_ALIAS"),		_T("STRING"),			dm.GetCellData(i, 3));
		pDM->AddDBFieldData(_T("FLAG"),					_T("STRING"),			str);
		pDM->AddDBFieldData(_T("IDX"),					_T("STRING"),			dm.GetCellData(i, 5));
		pDM->AddDBFieldData(_T("CAPTION"),				_T("STRING"),			dm.GetCellData(i, 6));
		pDM->AddDBFieldData(_T("STYLE"),				_T("STRING"),			dm.GetCellData(i, 7));
		pDM->AddDBFieldData(_T("TYPE"),					_T("STRING"),			dm.GetCellData(i, 8));
		pDM->AddDBFieldData(_T("SM"),					_T("STRING"),			dm.GetCellData(i, 9));
		pDM->AddDBFieldData(_T("BITMAP_NORMAL"),		_T("STRING"),			GetMysqlString(&dm, dm.GetCellData(i, 10)));
		pDM->AddDBFieldData(_T("BITMAP_HOT"),			_T("STRING"),			GetMysqlString(&dm, dm.GetCellData(i, 11)));
		pDM->AddDBFieldData(_T("BITMAP_DISABLED"),		_T("STRING"),			GetMysqlString(&dm, dm.GetCellData(i, 12)));
		pDM->AddDBFieldData(_T("TOOLTIP"),				_T("STRING"),			dm.GetCellData(i, 13));
		pDM->AddDBFieldData(_T("SPF_USE_FLAG"),			_T("STRING"),			dm.GetCellData(i, 14));
		pDM->AddDBFieldData(_T("PROGRAME_USE_FLAG"),	_T("STRING"),			dm.GetCellData(i, 15));
		pDM->AddDBFieldData(_T("ACCECLERATOR"),			_T("STRING"),			dm.GetCellData(i, 16));
		pDM->MakeInsertFrame(_T("ESL_TOOLBAR_TBL"));
		nFrames++;

	}

	if( bSendFrame )
	{
		ids = pDM->SendFrame();
		if (ids < 0) return ids;
		pDM->EndFrame();
	}

	if (pBar) pBar->SetPos(100);

	return 0;
}

INT CMNG_ACCESS_CONTROL_API::String_to_Integer(CString str)
{
	if (str.GetLength() == 0) return 0;
	
	return _ttoi(str);
}

CString CMNG_ACCESS_CONTROL_API::GetMysqlString(CESL_DataMgr *pDM, CString str)
{
	if (pDM == NULL) return str;

	if (pDM->m_dbkind == 2) {
		str.Replace(_T("\\"), _T("\\\\"));
		return str;
	} else {
		return str;
	}
}

INT CMNG_ACCESS_CONTROL_API::GetTopMenuIdx(CESL_DataMgr *pDM_MENUGROUP, CString strAlias)
{
	// GROUPTYPE 2
	// ALIAS 0
	if (pDM_MENUGROUP == NULL) return -1;
	INT nRows = m_arrTopMenuIdx.GetSize();
	INT nDMRowIdx;
	for (INT i = 0; i < nRows; i++) {
		nDMRowIdx = m_arrTopMenuIdx.GetAt(i);
		if (pDM_MENUGROUP->GetCellData(nDMRowIdx, 2) != _T("T")) continue;
		if (pDM_MENUGROUP->GetCellData(nDMRowIdx, 0) == strAlias) return nDMRowIdx;
	}

	return -1;
}
