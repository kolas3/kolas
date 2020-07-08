// Cat007UnionFixedFieldMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Cat007UnionFixedFieldMgr.h"

#include "FixedFieldInfo.h"
#include "..\..\ESL\ESL\DlgClassCode.h"
#include "DLG_FIXED_007.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat007UnionFixedFieldMgr dialog
CDLG_FIXED_007 *m_pParent;

CCat007UnionFixedFieldMgr::CCat007UnionFixedFieldMgr(UINT IDD, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat007UnionFixedFieldMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = NULL;
	m_pCM = NULL;
	m_pListInfo = NULL;
	m_pParent = (CDLG_FIXED_007*)pParent;
}

CCat007UnionFixedFieldMgr::~CCat007UnionFixedFieldMgr()
{	
}

void CCat007UnionFixedFieldMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat007UnionFixedFieldMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat007UnionFixedFieldMgr, CDialog)
	//{{AFX_MSG_MAP(CCat007UnionFixedFieldMgr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat007UnionFixedFieldMgr message handlers

BOOL CCat007UnionFixedFieldMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


INT CCat007UnionFixedFieldMgr::Init()
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

INT CCat007UnionFixedFieldMgr::ShowCodeDlg(CString &strGroupName, CString &strSubGroupName, CString &strItemName, CString &strCode)
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

INT CCat007UnionFixedFieldMgr::CheckCode(CString &strGroupName, CString &strSubGroupName, CString &strItemName, CString &strCode)
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

INT CCat007UnionFixedFieldMgr::Display()
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

INT CCat007UnionFixedFieldMgr::ValidateControlData(CStringArray *pArrStrControlMgrAlias)
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

INT CCat007UnionFixedFieldMgr::ApplyControlData()
{
	INT ids = -1;
	CString strMarcAlias;
	CString strControlMgrAlias;
	CString strItemData;
	POSITION pos = m_mapMarcAlias_ControlAlias.GetStartPosition();

	// 자료범주는 공통으로 기본 표시한다.
	m_pParent->m_MultiComboBox.GetData(strItemData);
	ids = m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("007@0"), strItemData, _T(""), ADD_SUBFIELD);
	
	while (pos)
	{		
		m_mapMarcAlias_ControlAlias.GetNextAssoc(pos, strMarcAlias, strControlMgrAlias);

		ids = m_pCM->GetControlMgrData(strControlMgrAlias, strItemData);
		if (ids >= 0)
			ids = m_pInfo->pMarcMgr->SetItem(m_pMarc, strMarcAlias, strItemData, _T(""), ADD_SUBFIELD);
	}
	return 0;
}

INT CCat007UnionFixedFieldMgr::GetItemNameFromControlHwnd(HWND hWnd, CString &strItemName, CString &strControlAlias)
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

INT CCat007UnionFixedFieldMgr::Get007()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCat007UnionFixedFieldMgr message handlers

BOOL CCat007UnionFixedFieldMgr::PreTranslateMessage(MSG* pMsg) 
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
				CObject *obj;
				obj = m_pCM->FindControl(strControlAlias);
				((CEdit*)obj)->SetFocus();
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
			if( strControlAlias == _T("이미지의비트깊이") ||
				strControlAlias == _T("필름검사일자") )
			{
				// 숫자체크 및 0으로 세자리 채워주기
				CString strNum;
				INT nNum;
				nNum = _ttoi(strCode.GetBuffer(0));
				if ( nNum > 0 )
				{
					
				}
				else
				{
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
			else
			{
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
	}
	
	// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
	// 통합마크 개정 적용
	// 에디트박스에서 ESC키를 누르면 안의 화면이 사라지는 현상 수정
	/*//NEW------------------------------------------------------------------------*/
	else if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam) {
		return m_pParent->PreTranslateMessage(pMsg);
	}
/*//END------------------------------------------------------------------------*/

	return CDialog::PreTranslateMessage(pMsg);
}
