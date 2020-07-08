
#include "Stdafx.h"
#include "MNG_USER_MANAGER_06.h"
#include "..\..\..\단행\열람\장서관리\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_06 dialog


CMNG_USER_MANAGER_06::CMNG_USER_MANAGER_06(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_USER_MANAGER_06::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_MANAGER_06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nMode = 0;
}


void CMNG_USER_MANAGER_06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_MANAGER_06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_USER_MANAGER_06, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_MANAGER_06)
	ON_BN_CLICKED(IDC_BTN_CREATE, OnBtnCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_06 message handlers

BOOL CMNG_USER_MANAGER_06::Create(CWnd* pParentWnd)
{	
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

EFS_END
return FALSE;
}	

BOOL CMNG_USER_MANAGER_06::GetEssentialControlData(CString &strManageCode, CString &strCompanyName, CString &strDeviceName, BOOL bShowEmptyMsg/*=TRUE*/)
{
EFS_BEGIN

	CESL_ControlMultiComboBox* pCB = (CESL_ControlMultiComboBox*)FindControl(_T("CM_MNG_USER_MANAGER_06"), _T("관리구분"));
	ASSERT(pCB);
	pCB->GetData(strManageCode);
	if (bShowEmptyMsg && strManageCode.IsEmpty() && !this->m_pInfo->MANAGE_CODE.IsEmpty())
	{
		AfxMessageBox(_T("관리구분이 선택되지 않았습니다."));
		return FALSE;
	}
	
	ASSERT(GetDlgItem(IDC_EDT_COMPANYNAME));
	GetDlgItem(IDC_EDT_COMPANYNAME)->GetWindowText(strCompanyName);
	if (bShowEmptyMsg && strCompanyName.IsEmpty())
	{
		AfxMessageBox(_T("회사명이 입력되지 않았습니다."));
		return FALSE;
	}

	ASSERT(GetDlgItem(IDC_EDT_DEVICENAME));
	GetDlgItem(IDC_EDT_DEVICENAME)->GetWindowText(strDeviceName);
	if (bShowEmptyMsg && strDeviceName.IsEmpty())
	{
		AfxMessageBox(_T("장비명이 입력되지 않았습니다."));
		return FALSE;
	}

	return TRUE;

EFS_END
return FALSE;
}

BOOL CMNG_USER_MANAGER_06::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	if (InitESL_Mgr( _T("MNG_USER_MANAGER_06") ) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error : MNG_USER_MANAGER_06"));
		return FALSE;
	}

	switch (GetMode())
	{
	case 0:
		{
			SetWindowText(_T("API인증키 등록"));

			if		(this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("U")) == 0) 
			{
				ASSERT(FALSE);
			}
			else if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("A")) == 0) 
			{
				CESL_ControlMultiComboBox* pCB = (CESL_ControlMultiComboBox*)FindControl(_T("CM_MNG_USER_MANAGER_06"), _T("관리구분"));
				ASSERT(pCB);
				pCB->SelectItem(this->m_pInfo->MANAGE_CODE);
				pCB->EnableWindow(FALSE);
			}
			else if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("S")) == 0) 
			{

			}
		}
		break;
	case 1:
		{
			SetWindowText(_T("API인증키 수정"));

			if		(this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("U")) == 0) 
			{
				ASSERT(FALSE);
			}
			else if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("A")) == 0) 
			{
				CESL_ControlMultiComboBox* pCB = (CESL_ControlMultiComboBox*)FindControl(_T("CM_MNG_USER_MANAGER_06"), _T("관리구분"));
				ASSERT(pCB);
				pCB->EnableWindow(FALSE);
			}
			else if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("S")) == 0) 
			{

			}

			CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_USER_MANAGER_05"), _T("그리드"));
			ASSERT(pGrid);

			pGrid->SelectMakeList();
			INT nCount = pGrid->SelectGetCount();
			if (nCount == 0)
			{
				AfxMessageBox(_T("수정 대상 API인증키 정보를 선택해야 합니다."));
				EndDialog(IDCANCEL);
				return FALSE;
			}
			if (nCount > 1)
			{
				AfxMessageBox(_T("수정 대상 API인증키 정보를 한 개만 선택해야 합니다."));
				EndDialog(IDCANCEL);
				return FALSE;
			}

			SetIndex(pGrid->SelectGetIdx());

			AllControlDisplay(_T("CM_MNG_USER_MANAGER_06"), GetIndex());
		}
		break;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;

EFS_END
	return FALSE;
}

void CMNG_USER_MANAGER_06::OnBtnCreate() 
{
	CString strManageCode;
	CString strCompanyName;
	CString strDeviceName;

	if (!GetEssentialControlData(strManageCode, strCompanyName, strDeviceName))
	{
		return;
	}

	CString strCertifyKey;
	strCertifyKey.Format(_T("%s%s%s"), strManageCode, strCompanyName, strDeviceName);
	strCertifyKey = CLocCommonAPI::StringToSHA256(strCertifyKey);
	
	ASSERT(GetDlgItem(IDC_EDT_CERTIFYKEY));
	GetDlgItem(IDC_EDT_CERTIFYKEY)->SetWindowText(strCertifyKey);
}

void CMNG_USER_MANAGER_06::OnOK() 
{
	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_MANAGER_05"));
	ASSERT(pDM);

	CString strManageCode;

	CString strCompanyName;

	CString strDeviceName;

	GetEssentialControlData(strManageCode, strCompanyName, strDeviceName);

	CString strCertifyKey;
	ASSERT(GetDlgItem(IDC_EDT_CERTIFYKEY));
	GetDlgItem(IDC_EDT_CERTIFYKEY)->GetWindowText(strCertifyKey);
	if (strCertifyKey.IsEmpty())
	{
		AfxMessageBox(_T("인증키가 생성되지 않았습니다."));
		return;
	}
	CString strRemark;
	ASSERT(GetDlgItem(IDC_EDT_REMARK));
	GetDlgItem(IDC_EDT_REMARK)->GetWindowText(strRemark);

	CString strRecKey;
	if (GetMode() == 1)
	{
		strRecKey= pDM->GetCellData(_T("REC_KEY"), GetIndex());
		ASSERT(!strRecKey.IsEmpty());
		if (strRecKey.IsEmpty())
		{
			AfxMessageBox(_T("수정 대상 API인증키 정보에 레코드키가 존재하지 않았습니다."));
			return;
		}
	}

	CString strQuery;

	switch(GetMode())
	{
	case 0:
		strQuery.Format(_T("SELECT COUNT(1) FROM MN_API_CERTIFYKEY_TBL WHERE CERTIFY_KEY=\'%s\'"), strCertifyKey);
		break;
	case 1:
		strQuery.Format(_T("SELECT COUNT(1) FROM MN_API_CERTIFYKEY_TBL WHERE CERTIFY_KEY=\'%s\' AND REC_KEY!=%s"), strCertifyKey, strRecKey);
		break;
	}

	CString strResult;
	INT ids = pDM->GetOneValueQuery(strQuery, strResult);
	if (ids < 0) ERROR_MSG_RETURN_VOID2(-10, _T("CMNG_USER_MANAGER_06::OnOK()"));

	if (strResult.CompareNoCase(_T("0")) != 0)
	{
		AfxMessageBox(_T("중복된 API인증키 정보가 존재합니다. 입력된 회사명, 장비명을 확인해주시기 바랍니다."));
		return;
	}

	pDM->StartFrame();
	
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), strManageCode);
	pDM->AddDBFieldData(_T("COMPANY_NAME"), _T("STRING"), strCompanyName);
	pDM->AddDBFieldData(_T("DEVICE_NAME"), _T("STRING"), strDeviceName);
	pDM->AddDBFieldData(_T("CERTIFY_KEY"), _T("STRING"), strCertifyKey);
	pDM->AddDBFieldData(_T("REMARK"), _T("STRING"), strRemark);

	switch(GetMode())
	{
	case 0:
		{
			pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
			pDM->AddDBFieldData(_T("APPROVED_DATE"), _T("NUMERIC"), _T("SYSDATE"));
			pDM->AddDBFieldData(_T("WORKER_KEY"), _T("NUMERIC"), this->m_pInfo->USER_PK);
			pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T(""), __WFILE__, __LINE__ ));

			pDM->MakeInsertFrame(_T("MN_API_CERTIFYKEY_TBL"));
		}
		break;
	case 1:
		{
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T(""), __WFILE__, __LINE__ ));

			pDM->MakeUpdateFrame(_T("MN_API_CERTIFYKEY_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, GetIndex());
		}
		break;
	}

	ids = pDM->SendFrame();
	if (ids < 0) ERROR_MSG_RETURN_VOID2(-20, _T("CMNG_USER_MANAGER_06::OnOK()"));

	pDM->EndFrame();

	AfxMessageBox(_T("API인증키 정보가 성공적으로 저장되었습니다."));

	CDialog::OnOK();
}
