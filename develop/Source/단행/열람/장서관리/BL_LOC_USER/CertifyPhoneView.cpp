#include "Stdafx.h"
#include "CertifyPhoneView.h"
#include "CertifyPhoneInputDlg.h"
#include "CertifyApi.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CERTIFY_PHONE_NATIVE_LOCAL	_T("내국인")
#define CERTIFY_PHONE_NATIVE_ALIEN	_T("외국인")

typedef struct _PHONE_SERVICE_INFO_
{
	TCHAR SID[5];
	TCHAR CODE[5];
	TCHAR MVNO[5];
} _PHONE_SERVICE_INFO_;

_PHONE_SERVICE_INFO_ PHONE_SERVICE_INFO[] = 
{
	{_T("SKT"),  _T("SKT"), _T("SKM")},
	{_T("KT"),   _T("KTF"), _T("KTM")},
	{_T("LGU+"), _T("LGT"), _T("LGM")},
};

#define CERTIFY_PHONE_NUMBER_LIMIT	4

#define CERTIFY_RESULT_TYPE_SYS		0
#define CERTIFY_RESULT_TYPE_MSG		1
#define CERTIFY_RESULT_TYPE_CODE	2


WNDPROC BIRTHWPROC;
HBRUSH  BIRTHBRUSH;


CCertifyPhoneView::CCertifyPhoneView(CESL_Mgr* pParent /*=NULL*/)
	: CCertifyView(CCertifyPhoneView::IDD, pParent)
{
	m_pBirthCtrl = NULL;
}

CCertifyPhoneView::~CCertifyPhoneView()
{
	if (m_pBirthCtrl)
	{
		delete m_pBirthCtrl;
		m_pBirthCtrl = NULL;
	}
}

void CCertifyPhoneView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCertifyPhoneView)
	DDX_Control(pDX, IDC_EDT_NAME, m_edtName);
	DDX_Control(pDX, IDC_RAD_MAN, m_radGenderMan);
	DDX_Control(pDX, IDC_RAD_WOMAN, m_radGenderWoman);
	DDX_Control(pDX, IDC_CMB_NATIVE, m_cmbNative);
	DDX_Control(pDX, IDC_CMB_PHONE_NUMBER1, m_cmbPhoneNumber);	
	DDX_Control(pDX, IDC_EDT_PHONE_NUMBER1, m_edtPhoneNumber1);
	DDX_Control(pDX, IDC_EDT_PHONE_NUMBER2, m_edtPhoneNumber2);
	DDX_Control(pDX, IDC_CMB_PHONESERVICE, m_cmbPhoneService);		
	DDX_Control(pDX, IDC_CHK_MVNO, m_chkMvno);
	 //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCertifyPhoneView, CCertifyView)
	//{{AFX_MSG_MAP(CCertifyPhoneView)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

LRESULT CALLBACK WndBirthProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_ERASEBKGND:
		{
			HDC hDC = (HDC)wParam;
			RECT rc;
			::GetClientRect(hWnd, &rc);			
			::FillRect(hDC, &rc, BIRTHBRUSH);

			return TRUE;
		}
		break;
	}
	return CallWindowProc(BIRTHWPROC, hWnd, nMsg, wParam, lParam);
}

BOOL CCertifyPhoneView::PreTranslateMessage(MSG* pMsg) 
{	
	return CCertifyView::PreTranslateMessage(pMsg);
}

BOOL CCertifyPhoneView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());	
	AddColorControl(IDC_EDT_NAME, _T(""), _T(""));
	AddColorControl(IDC_EDT_PHONE_NUMBER1, _T(""), _T(""));
	AddColorControl(IDC_EDT_PHONE_NUMBER2, _T(""), _T(""));
	
	m_pBirthCtrl = new CECO_ControlDateEdit(NULL);
	m_pBirthCtrl->SubclassDlgItem(IDC_EDT_BIRTHDATE, this);
	m_pBirthCtrl->InitEdit();
	m_pBirthCtrl->ECO_SetReadOnly(FALSE);
	
	m_pBirthCtrl->m_bkR = m_bkR;
	m_pBirthCtrl->m_bkG = m_bkG;
	m_pBirthCtrl->m_bkB = m_bkB;
	m_pBirthCtrl->m_BkBrush.CreateSolidBrush(RGB(m_bkR, m_bkG, m_bkB));
	m_pBirthCtrl->RequireFlag = TRUE;
	BIRTHBRUSH = (HBRUSH)m_pBirthCtrl->m_BkBrush.m_hObject;
	BIRTHWPROC = (WNDPROC)SetWindowLong(m_pBirthCtrl->GetSafeHwnd(), GWL_WNDPROC, (LONG)WndBirthProc);
		
	m_cmbNative.AddString(CERTIFY_PHONE_NATIVE_LOCAL);
	m_cmbNative.AddString(CERTIFY_PHONE_NATIVE_ALIEN);
	m_cmbNative.SetCurSel(0);

	CLocCommonAPI::MakeHPCorpNumList(&m_cmbPhoneNumber);

	INT nCount = sizeof(PHONE_SERVICE_INFO)/sizeof(_PHONE_SERVICE_INFO_);
	for (int i=0; i<nCount; i++)
	{
		m_cmbPhoneService.AddString(PHONE_SERVICE_INFO[i].SID);
	}
	m_cmbPhoneService.SetCurSel(0);
	m_edtPhoneNumber1.SetLimitText(CERTIFY_PHONE_NUMBER_LIMIT);
	m_edtPhoneNumber2.SetLimitText(CERTIFY_PHONE_NUMBER_LIMIT);
	
	CString strName = GetUserName();
	if (!strName.IsEmpty())
	{
		m_edtName.SetWindowText(strName);
	}
	CString strBirthDay = GetBirthDay();
	if (!strBirthDay.IsEmpty())
	{
		m_pBirthCtrl->SetData(strBirthDay);
	}

	CString strGender = GetCertifyGender();
	(strGender.CompareNoCase(_T("1")) == 0) ? m_radGenderWoman.SetCheck(TRUE) : m_radGenderMan.SetCheck(TRUE);

	CString strHandPhone = GetHandPhone();
	if (!strHandPhone.IsEmpty())
	{
		CStringArray PhoneNumList;
		CLocCommonAPI::MakeArrayList(strHandPhone, _T("-"), PhoneNumList);		
		CLocCommonAPI::SetPhoneNumCel(&m_cmbPhoneNumber, strHandPhone);		
		CString strLastNumber;
		for (int i=0 ; i<PhoneNumList.GetSize(); i++)
		{
			switch (i)
			{
			case 0:
				break;
			case 1:
				m_edtPhoneNumber1.SetWindowText(PhoneNumList.GetAt(i));
				break;
			default:
				strLastNumber += PhoneNumList.GetAt(i);
				break;
			}
		}

		if (strLastNumber.GetLength() <= CERTIFY_PHONE_NUMBER_LIMIT)
		{
			m_edtPhoneNumber2.SetWindowText(strLastNumber);
		}
		else
		{
			m_edtPhoneNumber2.SetWindowText(strLastNumber.Left(CERTIFY_PHONE_NUMBER_LIMIT));
		}
	}
	
	return TRUE; 
}

CString CCertifyPhoneView::GetInputPhoneNumber(BOOL bNonSeparator/*=TRUE*/)
{
	CString strNumber1;
	CString strNumber2;
	CString strNumber3;
	CString strInputNumber;

	m_cmbPhoneNumber.GetLBText(m_cmbPhoneNumber.GetCurSel(), strNumber1);
	m_edtPhoneNumber1.GetWindowText(strNumber2);
	m_edtPhoneNumber2.GetWindowText(strNumber3);

	
	if (!strNumber1.IsEmpty() && !strNumber2.IsEmpty() && !strNumber3.IsEmpty())
	{
		CString strFomat = bNonSeparator ? _T("%s%s%s") : _T("%s-%s-%s");
		strInputNumber.Format(strFomat, strNumber1, strNumber2, strNumber3);
	}

	return strInputNumber;
}

BOOL CCertifyPhoneView::Certify(CString &strMsg)
{
	CString strName;
	CString strGender;
	CString strNative;
	CString strBirthDay;
	CString strPhoneNumber;	
	CString	strPhoneService;

	CString strCertifyInfo;
	CString strCertifySort;
		
	
	m_edtName.GetWindowText(strName);	
	strGender = m_radGenderMan.GetCheck() ? _T("M") : _T("F");
	
	m_cmbNative.GetLBText(m_cmbNative.GetCurSel(), strNative);
	strNative = (strNative.CompareNoCase(CERTIFY_PHONE_NATIVE_LOCAL) == 0) ? _T("1") : _T("2");

	m_pBirthCtrl->GetData(strBirthDay);

	strPhoneNumber = GetInputPhoneNumber();

	if (m_chkMvno.GetCheck())
	{
		strPhoneService.Format(_T("%s"), PHONE_SERVICE_INFO[m_cmbPhoneService.GetCurSel()].MVNO);
	}
	else
	{
		strPhoneService.Format(_T("%s"), PHONE_SERVICE_INFO[m_cmbPhoneService.GetCurSel()].CODE);
	}

	if (strName.IsEmpty())
	{
		strMsg.Format(_T("이름을 입력해주세요."));
		m_edtName.SetFocus();
		return FALSE;
	}

	if (strBirthDay.IsEmpty())
	{
		strMsg.Format(_T("생일을 입력해주세요."));
		m_pBirthCtrl->SetFocus();
		return FALSE;
	}

	if (strPhoneNumber.IsEmpty())
	{
		strMsg.Format(_T("휴대폰번호를 입력해주세요."));
		m_cmbPhoneNumber.SetFocus();
		return FALSE;
	}

	
	CString strBirthDayReplace = strBirthDay;
	strBirthDayReplace.Replace(_T("/"), _T(""));
	
	{
#ifndef TEST_CI
		CCertifyFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("MW_SERVICE_B03"));
		Writer.addRecord();

		Writer.addElement(_T("이름"), strName);
		Writer.addElement(_T("내외국인구분"), strNative);
		Writer.addElement(_T("휴대폰번호"), strPhoneNumber);
		Writer.addElement(_T("성별"), strGender);
		Writer.addElement(_T("이통사구분"), strPhoneService);
		Writer.addElement(_T("생년월일"), strBirthDayReplace);
		
		Writer.addElement(_T("도서관부호"), m_pInfo->LIB_CODE);

		CCertifyFrameReader Reader;
		CCertifyFrameSender Sender(m_pInfo);
		INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_SYS, ids, _T("")));
			return FALSE;
		}
#endif
		
		CESL_DataMgr TempDM;
		TempDM.SetCONNECTION_INFO(_T(""));
		CString strResult = _T("");

#ifndef TEST_CI
		Reader.makeDM(&TempDM);

		strResult = Reader.getCommand();
		if (strResult.CompareNoCase(_T("ERROR")) == 0)
		{
			strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_MSG, 0, TempDM.GetCellData(_T("MSG"), 0)));
			return FALSE;
		}
#endif
		
		SetUserName(strName);
		
		CString strTmp = _T("0");
		if(strGender.Compare(_T("F")) == 0)		
			strTmp = _T("1");
		
		SetCertifyGender(strTmp);
		
		SetBirthDay(strBirthDay);
		SetHandPhone(GetInputPhoneNumber(FALSE));

#ifndef TEST_CI
		TempDM.GetCellData(_T("처리결과"), 0, strResult);
		if (strResult.CompareNoCase(_T("OK")) != 0)
		{
			strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_CODE, 0, _T("")));
			return FALSE;
		}
		
		TempDM.GetCellData(_T("인증정보"), 0, strCertifyInfo);
		if (strCertifyInfo.IsEmpty())
		{
			strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_MSG, 0, _T("인증정보가 올바르지 않습니다.")));
			return FALSE;
		}
		
		TempDM.GetCellData(_T("인증순번"), 0, strCertifySort);
		if (strCertifySort.IsEmpty())
		{
			strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_MSG, 0, _T("인증정보가 올바르지 않습니다.")));
			return FALSE;
		}
#endif
	}


	CCertifyPhoneInputDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		strMsg.Format(_T("인증번호가 입력되지 않았습니다."));
		return FALSE;
	}
	
	{

#ifndef TEST_CI
		CString strCertifyNumber = dlg.GetCertifyNumber();
		ASSERT(!strCertifyNumber.IsEmpty()); 

 		CCertifyFrameWriter Writer(m_pInfo);
 		Writer.setCommand(_T("MW_SERVICE_B04"));
 		Writer.addRecord();
 		Writer.addElement(_T("인증번호"), strCertifyNumber);
 		Writer.addElement(_T("인증정보"), strCertifyInfo);
 		Writer.addElement(_T("인증순번"), strCertifySort);
		
		Writer.addElement(_T("도서관부호"), m_pInfo->LIB_CODE);

		CCertifyFrameReader Reader;
		CCertifyFrameSender Sender(m_pInfo);
		INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_SYS, ids, _T("")));
			return FALSE;
		}

		CESL_DataMgr TempDM;
		TempDM.SetCONNECTION_INFO(_T(""));

		Reader.makeDM(&TempDM);

		CString strResult = Reader.getCommand();
		if (strResult.CompareNoCase(_T("ERROR")) == 0)
		{
			strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_MSG, 0, TempDM.GetCellData(_T("MSG"), 0)));
			return FALSE;
		}

		TempDM.GetCellData(_T("처리결과"), 0, strResult);
		if (strResult.CompareNoCase(_T("OK")) != 0)
		{
			strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_CODE, 0, _T("")));

			if (GetUserMode() == 1 && m_pInfo->m_bKLUse && GetKLMemberYN() && !GetCertifyCI().IsEmpty())
			{
				
			}
			else
			{
				SetCertifyFailCode(strResult);
			}

			return FALSE;
		}
#endif		
		
		CString strCI = _T("");
#ifndef TEST_CI
		TempDM.GetCellData(_T("Ci"), 0, strCI);
#else								
		strCI.Format(_T("xptmxm%080d=="),  _ttoi(strPhoneNumber));
#endif

		if (strCI.IsEmpty())
		{
			strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_MSG, 0, _T("본인인증코드 없음")));		
			return FALSE;
		}

		
		
		SetCertifyCI(strCI);
		
		CString strCertifyDate;
		CLocCommonAPI::GetCurrentDateTime(this, strCertifyDate);
		SetCertifyDate(strCertifyDate);
		
		SetCertifyWorker(m_pInfo->USER_ID);
		
	}

	return TRUE;
}

CString CCertifyPhoneView::MakeResultMsg(INT nMode, INT nCode, CString strExtraMsg)
{
	CString strMsg;

	switch (nMode)
	{
	case CERTIFY_RESULT_TYPE_SYS:
		strMsg.Format(_T("미들웨어와의 통신에 실패하였습니다.[%d]"), nCode);
		break;
	case CERTIFY_RESULT_TYPE_MSG:
		strMsg.Format(_T("%s"), strExtraMsg);
		break;
	case CERTIFY_RESULT_TYPE_CODE:
		strMsg.Format(_T("알수 없는 오류입니다"));
		break;
	}

	return strMsg;
}