// KL_UserImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KL_UserImportDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "..\\..\\..\\..\\공통\\IndexToolKit\\Index.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_UserImportDlg dialog


CKL_UserImportDlg::CKL_UserImportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_UserImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_UserImportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_strLibName = _T("");
	for(INT i = 0; i < 15; i++)
	{
		m_strGetData[i] = _T("");
	}
	m_strGPIN_ASCII = _T("");
	m_strGPIN_HASH = _T("");
	m_strGPIN_SEX = _T("");
	m_strCrytoCivilNo = _T("");
	m_strCivilNo = _T("");
	m_bIsCertification = FALSE;
	m_asLocalUserKey.RemoveAll();
	m_asLocalUserKey.FreeExtra();
}
CKL_UserImportDlg::~CKL_UserImportDlg()
{
}

void CKL_UserImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_UserImportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_UserImportDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_UserImportDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnKUI_Certification, OnbtnKUICertification)
	ON_BN_CLICKED(IDC_btnKUI_SetUserNo, OnbtnKUISetUserNo)
	ON_BN_CLICKED(IDC_btnKUI_Save, OnbtnKUISave)
	ON_BN_CLICKED(IDC_btnKUI_Cancel, OnbtnKUICancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_UserImportDlg message handlers

BOOL CKL_UserImportDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnKUI_Certification)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUICertification();
			}
			else if(GetDlgItem(IDC_btnKUI_SetUserNo)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUISetUserNo();
			}
			else if(GetDlgItem(IDC_btnKUI_Save)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUISave();
			}
			else if(GetDlgItem(IDC_btnKUI_Cancel)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUICancel();
			}
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnKUICancel();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CKL_UserImportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_KL_USER_IMPORT")) < 0)
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

HBRUSH CKL_UserImportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_edtKUI_LastLoan == pWnd->GetDlgCtrlID() || 
	   IDC_edtKUI_LastEdit == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(236,233,216));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

void CKL_UserImportDlg::OnbtnKUICertification() 
{
	if(TRUE == m_bIsCertification)
	{
		AfxMessageBox(_T("이미 본인인증 되었습니다."), MB_ICONINFORMATION);	
		return;
	}
	if(1 == Certification()) OnCancel();
}

INT CKL_UserImportDlg::Certification()
{
	CString strUserName;
	GetDlgItem(IDC_edtKUI_UserName)->GetWindowText(strUserName);
	strUserName.TrimLeft(); strUserName.TrimRight();
	strUserName.Replace(_T(" "), _T(""));
	GetDlgItem(IDC_edtKUI_UserName)->SetWindowText(strUserName);
	if(strUserName.IsEmpty())
	{
		AfxMessageBox(_T("이름을 입력해주십시오."), MB_ICONINFORMATION);
		GetDlgItem(IDC_edtKUI_UserName)->SetFocus();
		return -1;
	}
	CString strCivilNo, strCivilNo1, strCivilNo2;
	GetDlgItem(IDC_edtKUI_CivilNo1)->GetWindowText(strCivilNo1);
	if(6 != strCivilNo1.GetLength())
	{
		AfxMessageBox(_T("주민번호 앞자리는 6자리입니다."), MB_ICONINFORMATION);
		GetDlgItem(IDC_edtKUI_CivilNo1)->SetFocus();
		return -2;
	}
	GetDlgItem(IDC_edtKUI_CivilNo2)->GetWindowText(strCivilNo2);
	if(7 != strCivilNo2.GetLength())
	{
		AfxMessageBox(_T("주민번호 뒷자리는 7자리입니다."), MB_ICONINFORMATION);
		GetDlgItem(IDC_edtKUI_CivilNo2)->SetFocus();
		return -3;
	}
	strCivilNo.Format(_T("%s-%s"), strCivilNo1, strCivilNo2);
	INT ids = 0;
	ids = CLocCommonAPI::CivilNoCheck(strCivilNo);
	if(0 != ids)
	{
		AfxMessageBox(_T("잘못된 주민등록번호입니다.\r\n올바른 주민등록번호를 입력해주십시오."), MB_ICONINFORMATION);
		GetDlgItem(IDC_edtKUI_CivilNo2)->SetFocus();
		return -4;
	}
	CString strMsg, strLog;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	ids = CLocCommonAPI::KL_RealNameCertification(this, strUserName, strCivilNo, TmpDM);
	if(0 != ids)
	{
		if(-1010 == ids)
		{
			CString strTemp;
			TmpDM.GetCellData(0, 0, strTemp);
			if(-1 < strTemp.Find(_T("SY02")))
			{
				strMsg.Format(_T("이름을 확인하십시오."));
			}
			else if(-1 < strTemp.Find(_T("SY04")))
			{
				strMsg.Format(_T("이름, 주민번호 정보가 불일치합니다."));
			}			
			else if(-1 < strTemp.Find(_T("SY06")))
			{
				strMsg.Format(_T("명의도용 차단한 이용자입니다."));
			}
			else
			{
				strMsg.Format(_T("본인인증에 실패하였습니다."));			
			}

			strLog.Format(_T("본인인증(1) ERROR CODE : %s"), strTemp);		
		}
		else if(-1020 == ids || -1030 == ids)
		{
			strMsg = _T("본인인증 시스템 오류가 발생하였습니다.");
			strLog.Format(_T("본인인증(1) ERROR CODE : %d"), ids);		
		}
		else
		{
			strMsg = _T("미들웨어 접속 시도중입니다.");
			strLog.Format(_T("본인인증(1) ERROR CODE : %d"), ids);		
		}
		TmpDM.QueryLog(strLog.GetBuffer(0));
		AfxMessageBox(strMsg, MB_ICONINFORMATION);
		return -5;
	}
	CString strUserNo;
	TmpDM.GetCellData(_T("G-PINASCII인증코드"), 0, m_strGPIN_ASCII);
	TmpDM.GetCellData(_T("G-PINHASH인증코드"), 0, m_strGPIN_HASH);
	TmpDM.GetCellData(_T("G-PIN성별"), 0, m_strGPIN_SEX);
	TmpDM.GetCellData(_T("통합대출자번호"), 0, strUserNo);
	if(!strUserNo.IsEmpty())
	{
		strMsg.Format(  _T("대출자번호 [ %s ]으로 책이음회원에 가입된 이용자입니다.\r\n")
						_T("대출반납 화면에서 해당 대출자번호로 이용자를 조회하시거나\r\n")
						_T("이용자 입력화면에서 본인인증을 하시면\r\n")
						_T("책이음회원을 자동반입 받을 수 있습니다."), strUserNo);
		
		AfxMessageBox(strMsg, MB_ICONINFORMATION);
		return 1;
	}
	if(m_strGPIN_HASH.IsEmpty() || m_strGPIN_SEX.IsEmpty())
	{
		strLog.Format(_T("본인인증(1) ERROR CODE : null"));
		TmpDM.QueryLog(strLog.GetBuffer(0));
		strMsg.Format(_T("본인인증에 실패하였습니다."));
		AfxMessageBox(strMsg, MB_ICONINFORMATION);	
		return -6;
	}
	m_strCivilNo = strCivilNo;
	ids = ESL_DES_Encryption(strCivilNo, m_strCrytoCivilNo);
	if(m_strCrytoCivilNo.IsEmpty() || 14 == m_strCrytoCivilNo.GetLength())
	{
		strLog.Format(_T("주민등록번호암호화 ERROR CODE : %s"), ids);
		TmpDM.QueryLog(strLog.GetBuffer(0));
		strMsg.Format(_T("주민등록번호 암호화에 실패하였습니다."));
		AfxMessageBox(strMsg, MB_ICONINFORMATION);
		return -7;
	}
	m_bIsCertification = TRUE;
	CString strWhere = _T("");
	strWhere.Format(_T("CRYPTO_CIVIL_NO='%s' OR GPIN_HASH='%s'"), m_strCrytoCivilNo, m_strGPIN_HASH);
	GetDlgItem(IDC_edtKUI_UserNo)->GetWindowText(strUserNo);
	if(!strUserNo.IsEmpty())
	{
		strMsg.Format(_T(" OR USER_NO='%s'"), strUserNo);
		strWhere += strMsg;
	}
	CString strQuery;
	strQuery.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE %s"), strWhere);
	TmpDM.RestructDataManager(strQuery);
	m_asLocalUserKey.RemoveAll(); m_asLocalUserKey.FreeExtra();
	for(INT i = 0; i < TmpDM.GetRowCount(); i++)
	{
		TmpDM.GetCellData(i, 0, strMsg);
		m_asLocalUserKey.Add(strMsg);
	}

	return 0;
}

void CKL_UserImportDlg::OnbtnKUISetUserNo() 
{
	INT ids;
	CString strUserNoClass;
	GetControlData(_T("CM_KL_USER_IMPORT"), _T("이용자번호구분"), strUserNoClass);
	strUserNoClass.TrimLeft(); strUserNoClass.TrimRight();
	if(strUserNoClass.IsEmpty())
	{
		AfxMessageBox(_T("이용자번호구분을 선택해주십시오."), MB_ICONINFORMATION);
		return;
	}
	if(8 != strUserNoClass.GetLength())
	{
		AfxMessageBox(_T("이용자번호구분을 확인해주십시오.\r\n도서관부호6자리와 년도2자리의 조합이어야합니다."), MB_ICONINFORMATION);
		return;
	}
	CString strUserNo;
	GetDlgItem(IDC_edtKUI_UserNo)->GetWindowText(strUserNo);
	if(!strUserNo.IsEmpty())
	{
		if(IDYES != AfxMessageBox(_T("대출자번호를 부여하시겠습니까?"), MB_ICONQUESTION|MB_YESNO)) return;
	}
	ids = GetAutoUserNo(strUserNo, strUserNoClass);
	if(0 > ids) 
	{
		AfxMessageBox(_T("대출자번호 생성에 실패하였습니다."), MB_ICONSTOP);
		return;
	}
	GetDlgItem(IDC_edtKUI_UserNo)->SetWindowText(strUserNo);
}

void CKL_UserImportDlg::OnbtnKUISave() 
{
	if(FALSE == m_bIsCertification)
	{
		if(1 == Certification()) OnCancel();
	}
	CString strUserNo;
	GetDlgItem(IDC_edtKUI_UserNo)->GetWindowText(strUserNo);
	if(14 != strUserNo.GetLength() || FALSE == IsNumericText(strUserNo))
	{
		AfxMessageBox(_T("통합대출자번호는 숫자 14자리입니다."), MB_ICONSTOP);
		return;
	}
	CString strUserPosition;
	GetControlData(_T("CM_KL_USER_IMPORT"), _T("소속"), strUserPosition);
	strUserPosition.TrimLeft(); strUserPosition.TrimRight();
	if(strUserPosition.IsEmpty())
	{
		AfxMessageBox(_T("소속을 선택해 주십시오."), MB_ICONINFORMATION);
		return;
	}
	CString strUserClass;
	GetControlData(_T("CM_KL_USER_IMPORT"), _T("직급"), strUserClass);
	strUserClass.TrimLeft(); strUserClass.TrimRight();
	if(strUserClass.IsEmpty())
	{
		AfxMessageBox(_T("직급을 선택해 주십시오."), MB_ICONINFORMATION);
		return;
	}
	CString strHandPhone1;
	GetDlgItem(IDC_edtKUI_Phone1)->GetWindowText(strHandPhone1);
	CString strHandPhone2;
	GetDlgItem(IDC_edtKUI_Phone2)->GetWindowText(strHandPhone2);
	CString strHandPhone3;
	GetDlgItem(IDC_edtKUI_Phone3)->GetWindowText(strHandPhone3);
	if(!strHandPhone1.IsEmpty() && !strHandPhone2.IsEmpty() && !strHandPhone3.IsEmpty())
	{
		if(3 > strHandPhone1.GetLength() || 3 > strHandPhone2.GetLength() || 4 > strHandPhone3.GetLength())
		{
			AfxMessageBox(_T("핸드폰번호를 정확히 입력하세요."), MB_ICONINFORMATION);
			return;
		}
	}
	
	INT ids = 0;
	if(0 < m_asLocalUserKey.GetSize())
	{
		ids = UpdateUser();
	}
	else
	{
		ids = InsertUser();
	}
	if(0 != ids)
	{
		AfxMessageBox(_T("이용자 저장에 실패하였습니다."), MB_ICONSTOP);
		return;
	}

	m_strUserNo = strUserNo;
	OnOK();
}

INT CKL_UserImportDlg::InsertUser()
{
	CString strGetData;
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	DM.InitDBFieldData();
	DM.StartFrame();	
	DM.AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
	GetDlgItem(IDC_edtKUI_UserNo)->GetWindowText(strGetData);
	DM.AddDBFieldData(_T("USER_NO"), _T("STRING"), strGetData);
	GetDlgItem(IDC_edtKUI_UserName)->GetWindowText(strGetData);
	DM.AddDBFieldData(_T("NAME"), _T("STRING"), strGetData);
	CIndex index;
	index.MakeIndex(strGetData);
	DM.AddDBFieldData(_T("IDX_USER_NAME"), _T("STRING"), strGetData);
	DM.AddDBFieldData(_T("H_ZIPCODE"), _T("STRING"), m_strGetData[7]);
	DM.AddDBFieldData(_T("W_ZIPCODE"), _T("STRING"), m_strGetData[10]);
	DM.AddDBFieldData(_T("SECOND_PHONE"), _T("STRING"), m_strGetData[4]);
	strGetData = _T("N");
	if(TRUE == ((CButton*)GetDlgItem(IDC_chkKUI_SMS))->GetCheck()) strGetData = _T("Y");
	DM.AddDBFieldData(_T("SMS_USE_YN"), _T("STRING"), strGetData);
	strGetData = _T("N");
	if(TRUE == ((CButton*)GetDlgItem(IDC_chkKUI_MAIL))->GetCheck()) strGetData = _T("Y");
	DM.AddDBFieldData(_T("MAILING_USE_YN"), _T("STRING"), strGetData);
	DM.AddDBFieldData(_T("USER_CLASS"), _T("STRING"), _T("0"));
	DM.AddDBFieldData(_T("MEMBER_CLASS"), _T("STRING"), _T("0"));
	GetControlData(_T("CM_KL_USER_IMPORT"), _T("소속"), strGetData);
	strGetData.TrimLeft(); strGetData.TrimRight();
	DM.AddDBFieldData(_T("USER_POSITION_CODE"), _T("STRING"), strGetData);
	GetControlData(_T("CM_KL_USER_IMPORT"), _T("직급"), strGetData);
	strGetData.TrimLeft(); strGetData.TrimRight();
	DM.AddDBFieldData(_T("USER_CLASS_CODE"), _T("STRING"), strGetData);
	GetDlgItem(IDC_edtKUI_Remark)->GetWindowText(strGetData);
	DM.AddDBFieldData(_T("REMARK"), _T("STRING"), strGetData);
	DM.AddDBFieldData(_T("KL_MEMBER_YN"), _T("STRING"), _T("Y"));
	DM.AddDBFieldData(_T("GPIN_ASCII"), _T("STRING"), m_strGPIN_ASCII);
	DM.AddDBFieldData(_T("GPIN_HASH"), _T("STRING"), m_strGPIN_HASH);
	DM.AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), m_strGPIN_SEX);
	DM.AddDBFieldData(_T("CRYPTO_CIVIL_NO"), _T("STRING"), m_strCrytoCivilNo);
	strGetData = SetBirthDay(m_strCivilNo);
	DM.AddDBFieldData(_T("BIRTHDAY"), _T("STRING"), strGetData);
	DM.AddDBFieldData(_T("BIRTHDAY_TYPE"), _T("STRING"), _T("+"));
	DM.AddDBFieldData(_T("REG_DATE"), _T("NUMERIC"), _T("SYSDATE"));
	DM.AddDBFieldData(_T("LAST_MODIFY_DATE "), _T("NUMERIC"), _T("SYSDATE"));
	DM.AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));
	DM.AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("일반회원자동반입"), __WFILE__, __LINE__), TRUE);
	CString strHandPhone;
	GetDlgItem(IDC_edtKUI_Phone1)->GetWindowText(strHandPhone);
	strHandPhone += _T("-");
	GetDlgItem(IDC_edtKUI_Phone2)->GetWindowText(strGetData);
	strHandPhone += strGetData;
	strHandPhone += _T("-");
	GetDlgItem(IDC_edtKUI_Phone3)->GetWindowText(strGetData);
	strHandPhone += strGetData;
	CString strIdxHandPhone = strHandPhone;
	index.MakeIndex(strIdxHandPhone);
	CString strHAdd;
	GetDlgItem(IDC_edtKUI_Address)->GetWindowText(strHAdd);
	CString strHPhone;
	GetDlgItem(IDC_edtKUI_HomePhone)->GetWindowText(strHPhone);
	CString strMail;
	GetDlgItem(IDC_edtKUI_Mail)->GetWindowText(strMail);
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				CStringArray saEnc1, saEnc2, saEnc3, saEnc4, saEnc5, saEnc6, saEnc7, saEnc8;
				saEnc1.Add(strHandPhone);
				saEnc2.Add(strIdxHandPhone);
				saEnc3.Add(strHAdd);
				saEnc4.Add(strHPhone);
				saEnc5.Add(m_strGetData[11]);
				saEnc6.Add(m_strGetData[12]);
				saEnc7.Add(strMail);
				saEnc8.Add(m_strCivilNo);
				if(TRUE == p3000Api->ConnectSocket()) 
				{	
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), saEnc1))
					{
						p3000Api->CloseSocket();
						strHandPhone = saEnc1.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), saEnc2))
					{
						p3000Api->CloseSocket();
						strIdxHandPhone = saEnc2.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), saEnc3))
					{
						p3000Api->CloseSocket();
						strHAdd = saEnc3.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), saEnc4))
					{
						p3000Api->CloseSocket();
						strHPhone = saEnc4.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), saEnc5))
					{
						p3000Api->CloseSocket();
						m_strGetData[11] = saEnc5.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), saEnc6))
					{
						p3000Api->CloseSocket();
						m_strGetData[12] = saEnc6.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), saEnc7))
					{
						p3000Api->CloseSocket();
						strMail = saEnc7.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), saEnc8))
					{
						p3000Api->CloseSocket();
						m_strCivilNo = saEnc8.GetAt(0);
					}
				}
				saEnc1.RemoveAll(); saEnc1.FreeExtra();
				saEnc2.RemoveAll(); saEnc2.FreeExtra();
				saEnc3.RemoveAll(); saEnc3.FreeExtra();
				saEnc4.RemoveAll(); saEnc4.FreeExtra();
				saEnc5.RemoveAll(); saEnc5.FreeExtra();
				saEnc6.RemoveAll(); saEnc6.FreeExtra();
				saEnc7.RemoveAll(); saEnc7.FreeExtra();
				saEnc8.RemoveAll(); saEnc8.FreeExtra();
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}
	DM.AddDBFieldData(_T("HANDPHONE"), _T("STRING"), strHandPhone);
	DM.AddDBFieldData(_T("IDX_HANDPHONE"), _T("STRING"), strIdxHandPhone);
	DM.AddDBFieldData(_T("H_ADDR1"), _T("STRING"), strHAdd);
	DM.AddDBFieldData(_T("H_PHONE"), _T("STRING"), strHPhone);
	DM.AddDBFieldData(_T("W_ADDR1"), _T("STRING"), m_strGetData[11]);
	DM.AddDBFieldData(_T("W_PHONE"), _T("STRING"), m_strGetData[12]);
	DM.AddDBFieldData(_T("E_MAIL"), _T("STRING"), strMail);
	DM.AddDBFieldData(_T("CIVIL_NO"), _T("STRING"), m_strCivilNo);
	DM.MakeInsertFrame(_T("CO_LOAN_USER_TBL"));
	if(0 > DM.SendFrame()) return -2;
	DM.EndFrame();

	return 0;
}

INT CKL_UserImportDlg::UpdateUser()
{
	CString strGetData;
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	DM.StartFrame();
	// 중복제거
	CString strQuery;
	if(1 < m_asLocalUserKey.GetSize())
	{
		for(INT i = 1; i < m_asLocalUserKey.GetSize(); i++)
		{
			strQuery.Format(_T("UPDATE LS_WORK_T01 SET USER_KEY=%s WHERE USER_KEY=%s;"), m_asLocalUserKey.GetAt(0), m_asLocalUserKey.GetAt(i));
			DM.AddFrame(strQuery);
			strQuery.Format(_T("UPDATE CO_LOAN_TBL SET USER_KEY=%s WHERE USER_KEY=%s;"), m_asLocalUserKey.GetAt(0), m_asLocalUserKey.GetAt(i));
			DM.AddFrame(strQuery);
			strQuery.Format(_T("DELETE CO_LOAN_USER_TBL WHERE REC_KEY=%s;"), m_asLocalUserKey.GetAt(i));
			DM.AddFrame(strQuery);
		}
	}
	DM.InitDBFieldData();
	GetDlgItem(IDC_edtKUI_UserNo)->GetWindowText(strGetData);
	DM.AddDBFieldData(_T("USER_NO"), _T("STRING"), strGetData);
	GetDlgItem(IDC_edtKUI_UserName)->GetWindowText(strGetData);
	DM.AddDBFieldData(_T("NAME"), _T("STRING"), strGetData);
	CIndex index;
	index.MakeIndex(strGetData);
	DM.AddDBFieldData(_T("IDX_USER_NAME"), _T("STRING"), strGetData);
	DM.AddDBFieldData(_T("H_ZIPCODE"), _T("STRING"), m_strGetData[7]);
	DM.AddDBFieldData(_T("W_ZIPCODE"), _T("STRING"), m_strGetData[10]);
	DM.AddDBFieldData(_T("SECOND_PHONE"), _T("STRING"), m_strGetData[4]);
	strGetData = _T("N");
	if(TRUE == ((CButton*)GetDlgItem(IDC_chkKUI_SMS))->GetCheck()) strGetData = _T("Y");
	DM.AddDBFieldData(_T("SMS_USE_YN"), _T("STRING"), strGetData);
	strGetData = _T("N");
	if(TRUE == ((CButton*)GetDlgItem(IDC_chkKUI_MAIL))->GetCheck()) strGetData = _T("Y");
	DM.AddDBFieldData(_T("MAILING_USE_YN"), _T("STRING"), strGetData);
	DM.AddDBFieldData(_T("USER_CLASS"), _T("STRING"), _T("0"));
	DM.AddDBFieldData(_T("MEMBER_CLASS"), _T("STRING"), _T("0"));
	GetControlData(_T("CM_KL_USER_IMPORT"), _T("소속"), strGetData);
	strGetData.TrimLeft(); strGetData.TrimRight();
	DM.AddDBFieldData(_T("USER_POSITION_CODE"), _T("STRING"), strGetData);
	GetControlData(_T("CM_KL_USER_IMPORT"), _T("직급"), strGetData);
	strGetData.TrimLeft(); strGetData.TrimRight();
	DM.AddDBFieldData(_T("USER_CLASS_CODE"), _T("STRING"), strGetData);
	GetDlgItem(IDC_edtKUI_Remark)->GetWindowText(strGetData);
	DM.AddDBFieldData(_T("REMARK"), _T("STRING"), strGetData);
	DM.AddDBFieldData(_T("KL_MEMBER_YN"), _T("STRING"), _T("Y"));
	DM.AddDBFieldData(_T("GPIN_ASCII"), _T("STRING"), m_strGPIN_ASCII);
	DM.AddDBFieldData(_T("GPIN_HASH"), _T("STRING"), m_strGPIN_HASH);
	DM.AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), m_strGPIN_SEX);
	DM.AddDBFieldData(_T("CRYPTO_CIVIL_NO"), _T("STRING"), m_strCrytoCivilNo);
	strGetData = SetBirthDay(m_strCivilNo);
	DM.AddDBFieldData(_T("BIRTHDAY"), _T("STRING"), strGetData);
	DM.AddDBFieldData(_T("BIRTHDAY_TYPE"), _T("STRING"), _T("+"));
	DM.AddDBFieldData(_T("REG_DATE"), _T("NUMERIC"), _T("SYSDATE"));
	DM.AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));
	DM.AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("일반회원자동반입"), __WFILE__, __LINE__), TRUE);
	CString strHandPhone;
	GetDlgItem(IDC_edtKUI_Phone1)->GetWindowText(strHandPhone);
	strHandPhone += _T("-");
	GetDlgItem(IDC_edtKUI_Phone2)->GetWindowText(strGetData);
	strHandPhone += strGetData;
	strHandPhone += _T("-");
	GetDlgItem(IDC_edtKUI_Phone3)->GetWindowText(strGetData);
	strHandPhone += strGetData;
	CString strIdxHandPhone = strHandPhone;
	index.MakeIndex(strIdxHandPhone);
	CString strHAdd;
	GetDlgItem(IDC_edtKUI_Address)->GetWindowText(strHAdd);
	CString strHPhone;
	GetDlgItem(IDC_edtKUI_HomePhone)->GetWindowText(strHPhone);
	CString strMail;
	GetDlgItem(IDC_edtKUI_Mail)->GetWindowText(strMail);
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				CStringArray saEnc1, saEnc2, saEnc3, saEnc4, saEnc5, saEnc6, saEnc7, saEnc8;
				saEnc1.Add(strHandPhone);
				saEnc2.Add(strIdxHandPhone);
				saEnc3.Add(strHAdd);
				saEnc4.Add(strHPhone);
				saEnc5.Add(m_strGetData[11]);
				saEnc6.Add(m_strGetData[12]);
				saEnc7.Add(strMail);
				saEnc8.Add(m_strCivilNo);
				if(TRUE == p3000Api->ConnectSocket()) 
				{	
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), saEnc1))
					{
						p3000Api->CloseSocket();
						strHandPhone = saEnc1.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), saEnc2))
					{
						p3000Api->CloseSocket();
						strIdxHandPhone = saEnc2.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), saEnc3))
					{
						p3000Api->CloseSocket();
						strHAdd = saEnc3.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), saEnc4))
					{
						p3000Api->CloseSocket();
						strHPhone = saEnc4.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), saEnc5))
					{
						p3000Api->CloseSocket();
						m_strGetData[11] = saEnc5.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), saEnc6))
					{
						p3000Api->CloseSocket();
						m_strGetData[12] = saEnc6.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), saEnc7))
					{
						p3000Api->CloseSocket();
						strMail = saEnc7.GetAt(0);
					}
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), saEnc8))
					{
						p3000Api->CloseSocket();
						m_strCivilNo = saEnc8.GetAt(0);
					}
				}
				saEnc1.RemoveAll(); saEnc1.FreeExtra();
				saEnc2.RemoveAll(); saEnc2.FreeExtra();
				saEnc3.RemoveAll(); saEnc3.FreeExtra();
				saEnc4.RemoveAll(); saEnc4.FreeExtra();
				saEnc5.RemoveAll(); saEnc5.FreeExtra();
				saEnc6.RemoveAll(); saEnc6.FreeExtra();
				saEnc7.RemoveAll(); saEnc7.FreeExtra();
				saEnc8.RemoveAll(); saEnc8.FreeExtra();
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}
	DM.AddDBFieldData(_T("HANDPHONE"), _T("STRING"), strHandPhone);
	DM.AddDBFieldData(_T("IDX_HANDPHONE"), _T("STRING"), strIdxHandPhone);
	DM.AddDBFieldData(_T("H_ADDR1"), _T("STRING"), strHAdd);
	DM.AddDBFieldData(_T("H_PHONE"), _T("STRING"), strHPhone);
	DM.AddDBFieldData(_T("W_ADDR1"), _T("STRING"), m_strGetData[11]);
	DM.AddDBFieldData(_T("W_PHONE"), _T("STRING"), m_strGetData[12]);
	DM.AddDBFieldData(_T("E_MAIL"), _T("STRING"), strMail);
	DM.AddDBFieldData(_T("CIVIL_NO"), _T("STRING"), m_strCivilNo);
	DM.MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_asLocalUserKey.GetAt(0));
	if(0 > DM.SendFrame()) return -2;
	DM.EndFrame();

	return 0;
}

void CKL_UserImportDlg::OnbtnKUICancel() 
{
	OnCancel();
}

INT CKL_UserImportDlg::InitControl()
{
	CString strMsg;
	strMsg.Format(_T("[%s]도서관의 정보입니다."), m_strLibName);
	GetDlgItem(IDC_stcKUI_MSG)->SetWindowText(strMsg);
	GetDlgItem(IDC_edtKUI_UserNo)->SetWindowText(m_strGetData[0]);
	GetDlgItem(IDC_edtKUI_UserName)->SetWindowText(m_strGetData[1]);
	TCHAR cNum;
	CString strCivilNo;
	INT nLength = m_strGetData[2].GetLength();
	for(INT i = 0; i < nLength; i++)
	{
		cNum = m_strGetData[2].GetAt(i);
		if(47 < cNum && 58 > cNum)
		{
			strCivilNo += cNum;
		}
	}
	nLength = strCivilNo.GetLength();
	if(6 < nLength)
	{
		GetDlgItem(IDC_edtKUI_CivilNo1)->SetWindowText(strCivilNo.Left(6));
		if(13 < nLength)
		{
			GetDlgItem(IDC_edtKUI_CivilNo2)->SetWindowText(strCivilNo.Mid(6, 13));
		}
		else
		{
			GetDlgItem(IDC_edtKUI_CivilNo2)->SetWindowText(strCivilNo.Mid(6, nLength));
		}
	}
	else
	{
		GetDlgItem(IDC_edtKUI_CivilNo1)->SetWindowText(strCivilNo);
	}
	ViewHandPhoneNum(m_strGetData[3]);
	GetDlgItem(IDC_edtKUI_Mail)->SetWindowText(m_strGetData[5]);
	GetDlgItem(IDC_edtKUI_Remark)->SetWindowText(m_strGetData[6]);
	GetDlgItem(IDC_edtKUI_Address)->SetWindowText(m_strGetData[8]);
	GetDlgItem(IDC_edtKUI_HomePhone)->SetWindowText(m_strGetData[9]);
	GetDlgItem(IDC_edtKUI_LastLoan)->SetWindowText(m_strGetData[13]);
	GetDlgItem(IDC_edtKUI_LastEdit)->SetWindowText(m_strGetData[14]);

	return 0;
}

VOID CKL_UserImportDlg::ViewHandPhoneNum(CString strHandPhone)
{
	CStringArray asPhoneNum;
	MakeArrayList(strHandPhone, _T("-"), asPhoneNum);
	if(2 < asPhoneNum.GetSize())
	{
		GetDlgItem(IDC_edtKUI_Phone1)->SetWindowText(asPhoneNum.GetAt(0));
		GetDlgItem(IDC_edtKUI_Phone2)->SetWindowText(asPhoneNum.GetAt(1));
		GetDlgItem(IDC_edtKUI_Phone3)->SetWindowText(asPhoneNum.GetAt(2));
	}
}

INT CKL_UserImportDlg::MakeArrayList(CString sLine, CString sDelimiter, CStringArray &asArray)
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

INT CKL_UserImportDlg::MakeArrayList(CString sLine, CString sDelimiter, CArray <CString,CString> &asArray)
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

INT CKL_UserImportDlg::GetAutoUserNo(CString &strUserNo, CString strNumType)
{
	INT	ids;
	CString strQuery, strResult;
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	strQuery.Format(_T("SELECT COUNT(1) FROM CO_LAST_NUMBER_TBL WHERE DIVISION_VALUE='%s' AND PUBLISH_FORM_CODE='CO' AND KIND_CODE='USR_NO' AND MANAGE_CODE=UDF_MANAGE_CODE "), strNumType);
	ids = DM.GetOneValueQuery(strQuery, strResult);
	if(0 > ids) return -1;
	if(1 > _ttoi(strResult))
	{
		DM.InitDBFieldData();
		DM.StartFrame();
		DM.AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
		DM.AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), _T("CO"));
		DM.AddDBFieldData(_T("KIND_CODE"), _T("STRING"), _T("USR_NO"));
		DM.AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), strNumType);
		DM.AddDBFieldData(_T("LAST_NUMBER"), _T("STRING"), _T("0"));
		DM.AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));
		DM.AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("대출자번호부여"), __WFILE__, __LINE__), TRUE);
		DM.MakeInsertFrame(_T("CO_LAST_NUMBER_TBL"));
		ids = DM.SendFrame();
		if(0 > ids) return -2;
		DM.EndFrame();
	}
	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE DIVISION_VALUE='%s' AND PUBLISH_FORM_CODE='CO' AND KIND_CODE='USR_NO' AND MANAGE_CODE=UDF_MANAGE_CODE FOR UPDATE"), strNumType);
	ids = DM.GetOneValueQuery(strQuery, strResult);
	if(0 > ids) return -3;
	INT nLastNumber = _ttoi(strResult);
	while(TRUE)
	{
		nLastNumber++;
		strUserNo.Format(_T("%s%06d"), strNumType, nLastNumber);
		ids = GetUserNoDuplicate(strUserNo);
		if(0 > ids) return -4;
		if(0 == ids) break;
	}
	strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d WHERE PUBLISH_FORM_CODE='CO' AND KIND_CODE='USR_NO' AND DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;"), nLastNumber, strNumType);
	DM.InitDBFieldData();
	DM.StartFrame();
	DM.AddFrame(strQuery);
	ids = DM.SendFrame();
	if(0 > ids) return -5;
	DM.EndFrame();
	
	return 0;
}

INT CKL_UserImportDlg::GetUserNoDuplicate(CString strUserNo)
{
	INT ids;
	CString strQuery, strResult;
	strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_NO='%s'"), strUserNo);
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	ids = DM.GetOneValueQuery(strQuery, strResult);
	if(0 > ids) return ids;
	ids = _ttoi(strResult);
	if(0 < ids) return ids;

	return 0;
}

BOOL CKL_UserImportDlg::IsNumericText(CString strValue)
{
	INT nLength = strValue.GetLength();
	if(1 > nLength) return FALSE;
	BOOL bCheck = FALSE;
	for(INT i = 0; i < strValue.GetLength(); i++)
	{
		TCHAR cValue = strValue.Mid(i, 1).GetAt(0);
		if(cValue >= '0' && cValue <= '9')
		{
			bCheck = TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return bCheck;
}

CString CKL_UserImportDlg::SetBirthDay(CString strCivilNo)
{
	strCivilNo.Replace(_T("-"), _T(""));
	strCivilNo.TrimLeft(); strCivilNo.TrimRight();
	if(8 > strCivilNo.GetLength()) return _T("");
	CString strBirthDay;
	INT nSex = _ttoi(strCivilNo.Mid(6,1));
	if(nSex==1||nSex==2||nSex==5||nSex==6) 
	{
		strBirthDay = _T("19");
	}
	else if(nSex==3||nSex==4||nSex==7||nSex==8) 
	{
		strBirthDay = _T("20");
	}
	else if(nSex==9||nSex==0) 
	{
		strBirthDay = _T("18");
	}
	else 
	{
		return _T("");
	}
	INT nYear = _ttoi(strBirthDay + strCivilNo.Left(2));
	INT nMonth = _ttoi(strCivilNo.Mid(2,2));
	INT nDay = _ttoi(strCivilNo.Mid(4,2));
	BOOL bVaildDate = FALSE;
	if(1 <= nMonth && 12 >= nMonth)
	{
		INT nMonthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
		if( !(nYear % 400) || ((nYear % 100) && !(nYear % 4)) )
		{
			nMonthDays[1] = 29;
		}
		if( 1 <= nDay && nMonthDays[nMonth-1] >= nDay )
		{
			bVaildDate = TRUE;
		}
	}
	if(!bVaildDate) return _T("");
	strBirthDay.Format(_T("%d/%d/%d"), nYear, nMonth, nDay);
	return strBirthDay;
}