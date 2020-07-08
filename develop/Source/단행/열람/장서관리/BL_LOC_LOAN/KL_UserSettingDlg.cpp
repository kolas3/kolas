// KL_UserSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KL_UserSettingDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_UserSettingDlg dialog


CKL_UserSettingDlg::CKL_UserSettingDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_UserSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_UserSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strUserName = _T("");
	m_strHandPhone = _T("");
	m_strEmail = _T("");
	m_strHomeAddress = _T("");
	m_strRemark = _T("");
	m_bEditHandPhone = FALSE;
	m_bEditEmail = FALSE;
	m_bEditHomeAddress = FALSE;
	m_bEditRemark = FALSE;
	m_strGetLibCode = _T("");
	m_strLibCodeList = _T("");
}
CKL_UserSettingDlg::~CKL_UserSettingDlg()
{
}

void CKL_UserSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_UserSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_UserSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_UserSettingDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnKLUserSettingOK, OnbtnKLUserSettingOK)
	ON_BN_CLICKED(IDC_btnKLUserSettingClose, OnbtnKLUserSettingClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_UserSettingDlg message handlers

BOOL CKL_UserSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnKLUserSettingOK)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKLUserSettingOK();
			}
			else if(GetDlgItem(IDC_btnKLUserSettingClose)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKLUserSettingClose();
			}
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnKLUserSettingClose();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CKL_UserSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	if(InitESL_Mgr(_T("SM_KL_USER_SETTING")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	INT ids;
	ids = InitControl();
	if(0 > ids) return FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CKL_UserSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcKL_USER_SETTING == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(242,59,12));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

void CKL_UserSettingDlg::OnbtnKLUserSettingOK() 
{
	INT ids;
	ids = CMtoDM();
	if(0 > ids) return;

	OnOK();
}

void CKL_UserSettingDlg::OnbtnKLUserSettingClose() 
{
	OnCancel();
}

INT CKL_UserSettingDlg::InitControl()
{
	INT ids;
	CString strFuncName = _T("InitControl");

	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_edtKLUserSettingPhone);
	pEdit->SetLimitText(4);
	pEdit = (CEdit*)GetDlgItem(IDC_edtKLUserSettingPhone2);
	pEdit->SetLimitText(4);
	pEdit = (CEdit*)GetDlgItem(IDC_edtKLUserSettingPhone3);
	pEdit->SetLimitText(4);
	pEdit = (CEdit*)GetDlgItem(IDC_edtKLUserSettingMail);
	pEdit->SetLimitText(30);
	pEdit = (CEdit*)GetDlgItem(IDC_edtKLUserSettingAdd);
	pEdit->SetLimitText(200);
	pEdit = (CEdit*)GetDlgItem(IDC_edtKLUserSettingRemark);
	pEdit->SetLimitText(300);
	CESL_DataMgr *pDM = FindDM(_T("DM_KL_USER_SETTING"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

	CString strGetData;
	CString strUserInfo;
	if(m_strUserName.IsEmpty())
	{
		strUserInfo = _T("이용자정보");
	}
	else
	{
		strUserInfo.Format(_T("[%s]님의 정보"), m_strUserName);
	}
	GetDlgItem(IDC_stcKUS_UserInfo)->SetWindowText(strUserInfo);
	CButton *pBtn1;
	CButton *pBtn2;
	pBtn1 = (CButton*)GetDlgItem(IDC_radKLUserSettingSMSY);
	pBtn2 = (CButton*)GetDlgItem(IDC_radKLUserSettingSMSN);
	ids = pDM->GetCellData(_T("핸드폰"), 0, strGetData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strGetData.IsEmpty())
	{
		pBtn1->SetCheck(FALSE);
		pBtn1->EnableWindow(FALSE);
		pBtn2->SetCheck(TRUE);
		pBtn2->EnableWindow(FALSE);
	}
	else
	{
		pBtn1->SetCheck(TRUE);
		pBtn1->EnableWindow(TRUE);
		pBtn2->SetCheck(FALSE);
		pBtn2->EnableWindow(TRUE);
		ViewHandPhoneNum(strGetData);
	}
	m_strHandPhone = strGetData;
	pBtn1 = (CButton*)GetDlgItem(IDC_radKLUserSettingMAILY);
	pBtn2 = (CButton*)GetDlgItem(IDC_radKLUserSettingMAILN);
	ids = pDM->GetCellData(_T("EMAIL"), 0, strGetData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strGetData.IsEmpty())
	{
		pBtn1->SetCheck(FALSE);
		pBtn1->EnableWindow(FALSE);
		pBtn2->SetCheck(TRUE);
		pBtn2->EnableWindow(FALSE);
	}
	else
	{
		pBtn1->SetCheck(TRUE);
		pBtn1->EnableWindow(TRUE);
		pBtn2->SetCheck(FALSE);
		pBtn2->EnableWindow(TRUE);
	}
	m_strEmail = strGetData;
	ids = pDM->GetCellData(_T("집주소"), 0, strGetData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	m_strHomeAddress = strGetData;
	ids = pDM->GetCellData(_T("비고"), 0, strGetData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	m_strRemark = strGetData;

	CESL_ControlMgr *pCM = FindCM(_T("CM_KL_USER_SETTING"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	pCM->AllControlDisplay(0);

	CString strStatement = _T("");
	
	strStatement.Format(_T("책이음회원의 자관등록을 위하여 [%s]에 등록된 개인정보가 공유됩니다. 개인정보 반입에 동의하시겠습니까?"), m_strGetLibName);


	GetDlgItem(IDC_edtKUS_STATEMENT)->SetWindowText(strStatement);

	return 0;
}

INT CKL_UserSettingDlg::CMtoDM()
{
	INT ids;
	CString strFuncName = _T("CMtoDM");

	CString strGetData;
	CESL_ControlMgr *pCM = FindCM(_T("CM_KL_USER_SETTING"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	CESL_DataMgr *pDM = FindDM(_T("DM_KL_USER_SETTING"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	ids = pCM->GetControlMgrData(_T("소속"), strGetData, -1, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	strGetData.TrimLeft(); strGetData.TrimRight();
	if(strGetData.IsEmpty())
	{
		AfxMessageBox(_T("소속을 입력해 주십시오."), MB_ICONINFORMATION);
		return -1;
	}
	ids = pDM->SetCellData(_T("소속"), strGetData, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	ids = pCM->GetControlMgrData(_T("직급"), strGetData, -1, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	strGetData.TrimLeft(); strGetData.TrimRight();
	if(strGetData.IsEmpty())
	{
		AfxMessageBox(_T("직급을 입력해 주십시오."), MB_ICONINFORMATION);
		return -1;
	}
	ids = pDM->SetCellData(_T("직급"), strGetData, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	CString strSMSYN = _T("N");
	if(TRUE == ((CButton*)GetDlgItem(IDC_radKLUserSettingSMSY))->GetCheck())
	{
		strSMSYN = _T("Y");
	}
	ids = pDM->SetCellData(_T("SMS수신여부"), strSMSYN, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	CString strMAILYN = _T("N");
	if(TRUE == ((CButton*)GetDlgItem(IDC_radKLUserSettingMAILY))->GetCheck())
	{
		strMAILYN = _T("Y");
	}
	ids = pDM->SetCellData(_T("MAIL수신여부"), strMAILYN, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	CString strHandPhone;
	CString strHandPhone1;
	GetDlgItem(IDC_edtKLUserSettingPhone)->GetWindowText(strHandPhone1);
	CString strHandPhone2;
	GetDlgItem(IDC_edtKLUserSettingPhone2)->GetWindowText(strHandPhone2);
	CString strHandPhone3;
	GetDlgItem(IDC_edtKLUserSettingPhone3)->GetWindowText(strHandPhone3);
	if(!strHandPhone1.IsEmpty() && !strHandPhone2.IsEmpty() && !strHandPhone3.IsEmpty())
	{
		if(3 > strHandPhone1.GetLength() || 3 > strHandPhone2.GetLength() || 4 > strHandPhone3.GetLength())
		{
			AfxMessageBox(_T("핸드폰번호를 정확히 입력하세요."), MB_ICONINFORMATION);
			return -1;
		}
		strHandPhone = strHandPhone1 + _T("-") + strHandPhone2 + _T("-") + strHandPhone3;
		if(strHandPhone == m_strHandPhone)
		{
			m_bEditHandPhone = FALSE;
		}
		else
		{
			m_bEditHandPhone = TRUE;
			m_strHandPhone = strHandPhone;
		}
	}
	GetDlgItem(IDC_edtKLUserSettingMail)->GetWindowText(strGetData);
	if(strGetData == m_strEmail)
	{
		m_bEditEmail = FALSE;
	}
	else
	{
		m_bEditEmail = TRUE;
		m_strEmail = strGetData;
	}
	GetDlgItem(IDC_edtKLUserSettingAdd)->GetWindowText(strGetData);
	if(strGetData == m_strHomeAddress)
	{
		m_bEditHomeAddress = FALSE;
	}
	else
	{
		m_bEditHomeAddress = TRUE;
		m_strHomeAddress = strGetData;
	}
	GetDlgItem(IDC_edtKLUserSettingRemark)->GetWindowText(strGetData);
	if(strGetData == m_strRemark)
	{
		m_bEditRemark = FALSE;
	}
	else
	{
		m_bEditRemark = TRUE;
		m_strRemark = strGetData;
	}
	if(TRUE != ((CButton*)GetDlgItem(IDC_chkKLUserSettingOK))->GetCheck())
	{
		AfxMessageBox(_T("개인정보 제3자제공에 대한 이용자의 동의를 확인하십시오."), MB_ICONINFORMATION);
		return -1;
	}

	return 0;
}

VOID CKL_UserSettingDlg::ViewHandPhoneNum(CString strHandPhone)
{
	CStringArray asPhoneNum;
	MakeArrayList(strHandPhone, _T("-"), asPhoneNum);
	if(2 < asPhoneNum.GetSize())
	{
		GetDlgItem(IDC_edtKLUserSettingPhone)->SetWindowText(asPhoneNum.GetAt(0));
		GetDlgItem(IDC_edtKLUserSettingPhone2)->SetWindowText(asPhoneNum.GetAt(1));
		GetDlgItem(IDC_edtKLUserSettingPhone3)->SetWindowText(asPhoneNum.GetAt(2));
	}
}

INT CKL_UserSettingDlg::MakeArrayList(CString sLine, CString sDelimiter, CStringArray &asArray)
{
	asArray.RemoveAll();
	CArray <CString,CString> asTmpArray;
	MakeArrayList(sLine, sDelimiter, asTmpArray);
	for(INT i = 0; i < asTmpArray.GetSize(); i++)
	{
		asArray.Add(asTmpArray.GetAt(i));
	}

	return 0;
}

INT CKL_UserSettingDlg::MakeArrayList(CString sLine, CString sDelimiter, CArray <CString,CString> &asArray)
{
	asArray.RemoveAll();
	sLine.TrimLeft(); sLine.TrimRight();

	INT nIndex;
	CString sFieldName;
	BOOL IsStop = FALSE;
	while(TRUE)
	{
		nIndex = sLine.Find(sDelimiter);

		if(nIndex < 0)
		{
			asArray.Add(sLine);
			IsStop = TRUE;
		}
		else
		{
			sFieldName = sLine.Mid(0, nIndex);
			sLine = sLine.Mid( nIndex+1 , sLine.GetLength()-nIndex );
			sFieldName.TrimLeft(); sFieldName.TrimRight();
			asArray.Add(sFieldName);
		}
		if(IsStop) break;
	}
	return 0;
}
