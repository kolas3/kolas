// Cat007FixedFieldMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Cat007FixedFieldMgr.h"

#include "FixedFieldInfo.h"
#include "..\..\ESL\ESL\DlgClassCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedFieldMgr dialog


CCat007FixedFieldMgr::CCat007FixedFieldMgr(UINT IDD, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat007FixedFieldMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = NULL;
	m_pCM = NULL;
	m_pListInfo = NULL;
}
CCat007FixedFieldMgr::~CCat007FixedFieldMgr()
{

}

VOID CCat007FixedFieldMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat007FixedFieldMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CCat007FixedFieldMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CCat007FixedFieldMgr, CDialog)
	//{{AFX_MSG_MAP(CCat007FixedFieldMgr)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedFieldMgr Operations

INT CCat007FixedFieldMgr::Init()
{
	if (!m_pMarc)
	{
		m_strLastErrMsg = _T("MARC Pointer가 NULL입니다.");
		return -1;
	}

	if (!m_pListInfo)
	{
		m_strLastErrMsg = _T("007 Info List Pointer가 NULL입니다.");
		return -1;
	}

	return 0;
}

INT CCat007FixedFieldMgr::ShowCodeDlg(CString &strGroupName, CString &strSubGroupName, CString &strItemName, CString &strCode)
{
	strCode.Empty();

	CFixedFieldInfo *pInfo = NULL;
	POSITION pos = m_pListInfo->GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_pListInfo->GetNext(pos));
		if (!pInfo) continue;

		if (pInfo->m_strGroupName == strGroupName && 
			pInfo->m_strSubGroupName == strSubGroupName &&
			pInfo->m_strItemName == strItemName)
		{
			// 코드 다이얼로그 보여주기
			CString strDesc;
			CDlgClassCode dlg(this);
			dlg.InitData();

			POSITION posItem = pInfo->GetHeadPositionItem();
			while (posItem)
			{
				pInfo->GetNextItem(posItem, strCode, strDesc);
				dlg.AddData(strCode, strDesc);
			}

			if (dlg.DoModal() == IDOK)
			{
				strCode = dlg.m_strSelCode;
				return 0;
			}
			
			return -400;
		}
	}

	return -1;
}

INT CCat007FixedFieldMgr::CheckCode(CString &strGroupName, CString &strSubGroupName, CString &strItemName, CString &strCode)
{
	if (strCode.IsEmpty()) return 0;

	CString strOriginCode;
	CString strDesc;
	CFixedFieldInfo *pInfo = NULL;
	POSITION pos = m_pListInfo->GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_pListInfo->GetNext(pos));
		if (!pInfo) continue;

		if (pInfo->m_strGroupName == strGroupName && 
			pInfo->m_strSubGroupName == strSubGroupName &&
			pInfo->m_strItemName == strItemName)
		{
			// 코드가 있는지 체크
			POSITION posItem = pInfo->GetHeadPositionItem();
			while (posItem)
			{
				pInfo->GetNextItem(posItem, strOriginCode, strDesc);
				if (strCode == strOriginCode)
					return 0;
			}

			return -1;
		}
	}

	return 0;
}

INT CCat007FixedFieldMgr::Display()
{
	INT ids = -1;
	CString strMarcAlias;
	CString strControlMgrAlias;
	CString strItemData;
	POSITION pos = m_mapMarcAlias_ControlAlias.GetStartPosition();
	while (pos)
	{
		m_mapMarcAlias_ControlAlias.GetNextAssoc(pos, strMarcAlias, strControlMgrAlias);
		ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, strMarcAlias, strItemData);
		strItemData.TrimLeft();
		strItemData.TrimRight();
		if (!strItemData.IsEmpty())
			ids = m_pCM->SetControlMgrData(strControlMgrAlias, strItemData);
	}
	return 0;
}

INT CCat007FixedFieldMgr::ValidateControlData(CStringArray *pArrStrControlMgrAlias)
{
	if (!pArrStrControlMgrAlias) return -1;

	CString strMessage;
	CString strControlAlias;
	CString strControlData;
	INT nCount = pArrStrControlMgrAlias->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		strControlAlias = pArrStrControlMgrAlias->GetAt(idx);

		m_pCM->GetControlMgrData(strControlAlias, strControlData);
		if (strControlData.IsEmpty())
		{
			strMessage += _T("[") + strControlAlias + _T("] ");
		}
	}

	if (!strMessage.IsEmpty())
	{
		strMessage += _T("항목을 입력해주세요");
		AfxMessageBox(strMessage);
		return -1;
	}

	return 0;
}

INT CCat007FixedFieldMgr::ApplyControlData()
{
	INT ids = -1;
	CString strMarcAlias;
	CString strControlMgrAlias;
	CString strItemData;
	POSITION pos = m_mapMarcAlias_ControlAlias.GetStartPosition();
	while (pos)
	{		
		m_mapMarcAlias_ControlAlias.GetNextAssoc(pos, strMarcAlias, strControlMgrAlias);

		ids = m_pCM->GetControlMgrData(strControlMgrAlias, strItemData);
		if (ids >= 0)
			ids = m_pInfo->pMarcMgr->SetItem(m_pMarc, strMarcAlias, strItemData, _T(""), ADD_SUBFIELD);
	}
	return 0;
}

INT CCat007FixedFieldMgr::GetItemNameFromControlHwnd(HWND hWnd, CString &strItemName, CString &strControlAlias)
{
	if (!hWnd) return -1;

	INT ids = -1;
	CWnd *pWnd = NULL;
	CString strItemData;
	POSITION pos = m_mapControlAlias_ItemName.GetStartPosition();
	while (pos)
	{		
		m_mapControlAlias_ItemName.GetNextAssoc(pos, strControlAlias, strItemName);

		pWnd = (CWnd*)m_pCM->FindControl(strControlAlias);
		if (!pWnd) continue;

		if (pWnd->GetSafeHwnd() == hWnd)
			return 0;
	}

	return -1;
}

INT CCat007FixedFieldMgr::Get007()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedFieldMgr message handlers

BOOL CCat007FixedFieldMgr::PreTranslateMessage(MSG* pMsg) 
{
	if ( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9) || (pMsg->wParam == MK_RBUTTON) )
	{
		CString strCode;
		CString strItemName;
		CString strControlAlias;
		
		INT ids = GetItemNameFromControlHwnd(pMsg->hwnd, strItemName, strControlAlias);
		if (ids >= 0)
		{
			ids = ShowCodeDlg(m_strGroupName, m_strSubGroupName, strItemName, strCode);
			if (ids >= 0)
			{
				m_pCM->SetControlMgrData(strControlAlias, strCode);
			}	
			
			return TRUE;
		}
	}
	else if (pMsg->message == WM_KEYUP)
	{
		CString strCode;
		CString strItemName;
		CString strControlAlias;

		INT ids = GetItemNameFromControlHwnd(pMsg->hwnd, strItemName, strControlAlias);
		if (ids >= 0)
		{
			ids = m_pCM->GetControlMgrData(strControlAlias, strCode);
			ids = CheckCode(m_strGroupName, m_strSubGroupName, strItemName, strCode);
			if (ids < 0)
			{
				AfxMessageBox(_T("잘못된 값입니다."));
				m_pCM->SetControlMgrData(strControlAlias, _T(""));
				((CWnd*)m_pCM->FindControl(strControlAlias))->SetFocus();
				return TRUE;
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
