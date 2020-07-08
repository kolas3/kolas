#include "Stdafx.h"
#include "MNG_USER_MANAGER_05.h"
#include "MNG_USER_MANAGER_06.h"
#include "..\..\..\단행\열람\장서관리\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_05 dialog


CMNG_USER_MANAGER_05::CMNG_USER_MANAGER_05(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_USER_MANAGER_05::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_MANAGER_05)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMNG_USER_MANAGER_05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_MANAGER_05)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_USER_MANAGER_05, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_MANAGER_05)
	ON_BN_CLICKED(IDC_BTN_INPUT, OnBtnInput)
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_05 message handlers

BOOL CMNG_USER_MANAGER_05::Create(CWnd* pParentWnd)
{	
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

EFS_END
return FALSE;
}	

BOOL CMNG_USER_MANAGER_05::RefreshGrid(BOOL bDataRefresh/*=TRUE*/)
{
	if (bDataRefresh)
	{
		CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_MANAGER_05"));
		ASSERT(pDM);

		CString strWhere;
		if		(this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("U")) == 0) 
		{
			ASSERT(FALSE);
		}
		else if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("A")) == 0) 
		{
			if (this->m_pInfo->MANAGE_CODE.IsEmpty())
				strWhere.Format(_T("MANAGE_CODE IS NULL"));
			else
				strWhere.Format(_T("MANAGE_CODE=\'%s\'"), this->m_pInfo->MANAGE_CODE);
		}

		pDM->RefreshDataManager(strWhere);
	}

	ControlDisplay(_T("CM_MNG_USER_MANAGER_05"), _T("그리드"));

	return TRUE;
}

BOOL CMNG_USER_MANAGER_05::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	if (InitESL_Mgr( _T("MNG_USER_MANAGER_05") ) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error : MNG_USER_MANAGER_05"));
		return FALSE;
	}

	RefreshGrid();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;

EFS_END
	return FALSE;
}

void CMNG_USER_MANAGER_05::OnBtnInput() 
{
	CMNG_USER_MANAGER_06 dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		RefreshGrid();
	}
}

void CMNG_USER_MANAGER_05::OnBtnModify() 
{
	CMNG_USER_MANAGER_06 dlg(this);
	dlg.SetMode(1);
	if (dlg.DoModal() == IDOK)
	{
		RefreshGrid();
	}
}

void CMNG_USER_MANAGER_05::OnBtnDelete() 
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_USER_MANAGER_05"), _T("그리드"));
	ASSERT(pGrid);

	pGrid->SelectMakeList();
	INT nCount = pGrid->SelectGetCount();
	if (nCount == 0)
	{
		AfxMessageBox(_T("삭제 대상 API인증키 정보를 선택해야 합니다."));
		return;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_MANAGER_05"));
	ASSERT(pDM);

	pDM->StartFrame();

	INT idx = -1;
	do
	{
		if (idx < 0) idx = pGrid->SelectGetHeadPosition();
		else		 idx = pGrid->SelectGetNext();

		CString strRecKey = pDM->GetCellData(_T("REC_KEY"), idx);
		if (!strRecKey.IsEmpty())
		{
			CString strQuery;
			strQuery.Format(_T("DELETE FROM MN_API_CERTIFYKEY_TBL WHERE REC_KEY=%s;"), strRecKey);

			TRACE(strQuery);
			pDM->AddFrame(strQuery);

		}

	}
	while (idx > -1);

	INT ids = pDM->SendFrame();
	if (ids < 0) ERROR_MSG_RETURN_VOID2(-10, _T("CMNG_USER_MANAGER_05::OnBtnDelete()"));

	pDM->EndFrame();

	AfxMessageBox(_T("선택된 API인증키 정보가 성공적으로 삭제되었습니다."));

	RefreshGrid();
}
