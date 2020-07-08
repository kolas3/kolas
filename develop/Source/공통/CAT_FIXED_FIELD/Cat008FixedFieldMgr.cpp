// Cat008FixedFieldMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Cat008FixedFieldMgr.h"

#include "FixedFieldInfo.h"
#include "..\..\ESL\ESL\DlgClassCode.h"

#include "..\..\관리\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedFieldMgr dialog


CCat008FixedFieldMgr::CCat008FixedFieldMgr(UINT IDD, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat008FixedFieldMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_lpsz008InfoFileName = _T("..\\CFG\\008TAG코드.txt");
	m_lpszLeaderInfoFileName = _T("..\\CFG\\리더코드.txt");

	m_pMarc = NULL;
	m_pCM = NULL;
}

CCat008FixedFieldMgr::~CCat008FixedFieldMgr()
{
	FreeData();
}

VOID CCat008FixedFieldMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat008FixedFieldMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

VOID CCat008FixedFieldMgr::FreeData()
{
	CFixedFieldInfo *pInfo = NULL;

	// 008 지우기
	POSITION pos = m_listInfo.GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_listInfo.GetNext(pos));
		delete pInfo;
	}
	m_listInfo.RemoveAll();
}

BEGIN_MESSAGE_MAP(CCat008FixedFieldMgr, CDialog)
	//{{AFX_MSG_MAP(CCat008FixedFieldMgr)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedFieldMgr Operations

INT CCat008FixedFieldMgr::Init()
{
	if (!m_pMarc)
	{
		m_strLastErrMsg = _T("MARC Pointer가 NULL입니다.");
		return -1;
	}
	
	INT ids = -1;
	FreeData();
	ids  = LoadInfo(m_lpsz008InfoFileName);
	if (ids < 0) return ids;
	ids = LoadInfo(m_lpszLeaderInfoFileName);
	if (ids < 0) return ids;

	return 0;
}

INT CCat008FixedFieldMgr::LoadInfo(LPCTSTR lpszFileName)
{
	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary)) return -1;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT nFind = -1;
	CString strLine;
	CString strGroupName;
	DWORD dwPos = 0;
	CFixedFieldInfo *pInfo = NULL;
	while (file.ReadString(strLine))
	{
		//strLine.TrimLeft();
		//strLine.TrimRight();

		if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;

		// Group
		if (strLine.Left(1) == _T("["))
		{
			// Position 저장
			dwPos = file.GetPosition();

			strGroupName = strLine;
			// Item
			while (file.ReadString(strLine))
			{
				//strLine.TrimLeft();
				//strLine.TrimRight();
				if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
				if (strLine.Left(1) == _T("[")) 
				{
					// Position 돌려놓기
					file.Seek(dwPos, CFile::begin);
					break;
				}
				if (strLine.Left(1) == _T("("))
				{
					// Position 저장
					dwPos = file.GetPosition();

					// Item 추가
					pInfo = new CFixedFieldInfo(strGroupName, strLine);
					m_listInfo.AddTail(pInfo);

					// Code, Desc 넣기
					while (file.ReadString(strLine))
					{
						//strLine.TrimLeft();
						//strLine.TrimRight();
						if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
						if (strLine.Left(1) == _T("(") || strLine.Left(1) == _T("[")) 
						{
							// Position 돌려놓기
							file.Seek(dwPos, CFile::begin);
							break;
						}

						nFind = strLine.Find(_T(":"));
						if (nFind < 0) continue;

						pInfo->AddItem(strLine.Mid(0, nFind), strLine.Mid(nFind + 1));
					}
				}
			}
		}		
	}

	file.Close();
	return 0;
}

INT CCat008FixedFieldMgr::ShowCodeDlg(CString &strGroupName, CString &strItemName, CString &strCode, CString &strControlAlias)
{
	strCode.Empty();

	// 발행국부호
	if (strItemName == _T("발행국부호"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(0, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
	else if (strItemName == _T("한국대학부호"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(1, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
	else if (strItemName == _T("언어"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(2, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
	else if (strItemName == _T("정부기관부호"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(3, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
/*	else if (strItemName == _T("(로마자번자표)"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(3, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
*/
	// Code Dialog로 보여주기
	CFixedFieldInfo *pInfo = NULL;
	POSITION pos = m_listInfo.GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_listInfo.GetNext(pos));
		if (!pInfo) continue;

		if (pInfo->m_strGroupName == strGroupName && pInfo->m_strItemName == strItemName)
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

INT CCat008FixedFieldMgr::CheckCode(CString &strGroupName, CString &strItemName, CString &strCode)
{
	if (strCode.IsEmpty()) return 0;

	CString strOriginCode;
	CString strDesc;
	CFixedFieldInfo *pInfo = NULL;
	POSITION pos = m_listInfo.GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_listInfo.GetNext(pos));
		if (!pInfo) continue;

		if (pInfo->m_strGroupName == strGroupName && 
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

INT CCat008FixedFieldMgr::Display()
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

INT CCat008FixedFieldMgr::ValidateControlData(CStringArray *pArrStrControlMgrAlias)
{
	if (!pArrStrControlMgrAlias) return -1;

	CString strMessage;
	CString strControlAlias;
	CString strControlData;
	INT nCount = pArrStrControlMgrAlias->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		/*
		strControlAlias = pArrStrControlMgrAlias->GetAt(idx);
        
		m_pCM->GetControlMgrData(strControlAlias, strControlData);
		if (strControlData.IsEmpty())
		{
			strMessage += _T("[") + strControlAlias + _T("] ");
		}
		*/
			strControlAlias = pArrStrControlMgrAlias->GetAt(idx);
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.13 이학중 수정.
		// 고정길이 입력에서 유형이 n이면 발행년1을 입력하지 않아도 저장이 되어야 한다.
		// 
			
		bool bPublishYearForm = false ;

        if(m_pCM->CM_ALIAS == _T("CM_BO_CAT_PROC_0323") || m_pCM->CM_ALIAS == _T("CM_BO_CAT_PROC_0324") )
		{
			m_pCM->GetControlMgrData(_T("유형"), strControlData);
			//if(strControlData == _T("n") || strControlData == _T("m") )
			if( strControlData != _T("s") )
			{
				bPublishYearForm = true ;
			}
		}
        else
		{
			m_pCM->GetControlMgrData(_T("발행년유형"), strControlData);
			//if(strControlData == _T("n") || strControlData == _T("m") )
			if( strControlData != _T("s") )
			{
				bPublishYearForm = true ;
			}
		}

		m_pCM->GetControlMgrData(strControlAlias, strControlData);
		if(strControlAlias== _T("발행년1") && bPublishYearForm == true )
		{
			continue ;
		}
		else if (strControlData.IsEmpty())
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

INT CCat008FixedFieldMgr::ApplyControlData()
{
	INT ids = -1;
	// 008 을 지운다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("008"));

	CString strMarcAlias;
	CString strControlMgrAlias;
	CString strItemData;
	POSITION pos = m_mapMarcAlias_ControlAlias.GetStartPosition();
	while (pos)
	{		
		m_mapMarcAlias_ControlAlias.GetNextAssoc(pos, strMarcAlias, strControlMgrAlias);

		ids = m_pCM->GetControlMgrData(strControlMgrAlias, strItemData);
		if (ids >= 0)
			ids = m_pInfo->pMarcMgr->SetItem(m_pMarc, strMarcAlias, strItemData);
	}
	return 0;
}

INT CCat008FixedFieldMgr::GetItemNameFromControlHwnd(HWND hWnd, CString &strItemName, CString &strControlAlias)
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

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedFieldMgr message handlers

BOOL CCat008FixedFieldMgr::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 || pMsg->wParam == MK_RBUTTON)
	{
		CString strCode;
		CString strItemName;
		CString strControlAlias;
		
		INT ids = GetItemNameFromControlHwnd(pMsg->hwnd, strItemName, strControlAlias);
		if (ids >= 0)
		{
			ids = ShowCodeDlg(m_strGroupName, strItemName, strCode, strControlAlias);
			// 닫기(Cancle)버튼 누르면 리터값이 -400 (소정수정 2004.10.08)
			if ((ids < 0) && (ids != -400) )
				ids = ShowCodeDlg(CString(_T("[AV]")), strItemName, strCode, strControlAlias);	// 파일에 없다.. AV가 온전해서..ㅡ.ㅡ;
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
			ids = CheckCode(m_strGroupName, strItemName, strCode);
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
