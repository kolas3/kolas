#include "stdafx.h"
#include "UserCertifyDlg.h"
#include "CertifyApi.h"
#include "UserDuplecateDlg.h"
#include "Imm.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\\..\\..\\..\\공통\\IndexToolKit\\Index.h"

#include "BO_LOC_3000Api.h"

#include "KL_UserManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUserCertifyDlg::CUserCertifyDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CUserCertifyDlg::IDD, pParent)
{
	m_strCertifyCode = _T("");
	m_strCertifySex = _T("");
	m_strCertifyDate = _T("");
	m_strCertifyWorker = _T("");
	m_strCertifyFailCode = _T("");
	m_strCertifyName = _T("");
	m_strCertifyCivilNo = _T("");

	m_nMode = 0;
	m_strUserKey = _T("");
	m_strName = _T("");
	m_strCivilNo1 = _T("");
	m_strCivilNo2 = _T("");

	
	m_bKLMemeberSearch = FALSE;
	m_strKlUserNo = _T("");

	
	m_strFailCivilNo = _T("");

	
	m_strBeforeCode = _T("");
	m_strBeforeFailCode = _T("");
	m_strBeforeDate = _T("");
	m_strBeforeSex = _T("");
	m_strBeforeWorker = _T("");
	m_strBeforeName = _T("");
	m_strBeforeCivilNo = _T("");
	m_bIsKLMemberYN = FALSE;
}
CUserCertifyDlg::~CUserCertifyDlg()
{
	
}

void CUserCertifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CUserCertifyDlg)
	//}}AFX_DATA_MAP	
	
}


BEGIN_MESSAGE_MAP(CUserCertifyDlg, CDialog)
	//{{AFX_MSG_MAP(CUserCertifyDlg)
	ON_BN_CLICKED(IDC_btnCertify, OnbtnCertify)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnCertifyClose, OnbtnCertifyClose)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_edtCertifyCvilNo1, OnChangeedtCertifyCvilNo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CUserCertifyDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnCertifyClose();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			CString strGetData;
			if(GetDlgItem(IDC_btnCertify)->m_hWnd == pMsg->hwnd )
			{
				OnbtnCertify();
			}
			else if(GetDlgItem(IDC_btnCertifyClose)->m_hWnd == pMsg->hwnd )
			{
				OnbtnCertifyClose();
			}
			else if(GetDlgItem(IDC_edtCertifyName)->m_hWnd == pMsg->hwnd )
			{
				GetDlgItem(IDC_edtCertifyName)->GetWindowText(strGetData);
				if(0 < strGetData.GetLength())
				{
					GetDlgItem(IDC_edtCertifyCvilNo1)->GetWindowText(strGetData);
					if(6 == strGetData.GetLength())
					{
						GetDlgItem(IDC_edtCertifyCvilNo2)->GetWindowText(strGetData);
						if(7 == strGetData.GetLength())
						{
							OnbtnCertify();
						}
						else
						{
							GetDlgItem(IDC_edtCertifyCvilNo2)->SetFocus();
						}
					}
					else
					{
						GetDlgItem(IDC_edtCertifyCvilNo1)->SetFocus();
					}
				}
			}
			else if(GetDlgItem(IDC_edtCertifyCvilNo1)->m_hWnd == pMsg->hwnd )
			{
				GetDlgItem(IDC_edtCertifyCvilNo1)->GetWindowText(strGetData);
				if(6 == strGetData.GetLength())
				{
					GetDlgItem(IDC_edtCertifyCvilNo2)->GetWindowText(strGetData);
					if(7 == strGetData.GetLength())
					{
						OnbtnCertify();
					}
					else
					{
						GetDlgItem(IDC_edtCertifyCvilNo2)->SetFocus();
					}
				}
			}
			else if(GetDlgItem(IDC_edtCertifyCvilNo2)->m_hWnd == pMsg->hwnd )
			{
				GetDlgItem(IDC_edtCertifyCvilNo2)->GetWindowText(strGetData);
				if(7 == strGetData.GetLength())
				{
					OnbtnCertify();
				}
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CUserCertifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	
	CRect rect;
	GetWindowRect(&rect);
	ClientToScreen(rect);
	m_nHeight = rect.Height();
	m_nWidth = rect.Width();
	m_nFailHeight = m_nHeight + 222;
	m_nFailWidth = m_nWidth + 285;
	
	GetDlgItem(IDC_edtCertifyName)->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.right += 1;
	GetDlgItem(IDC_edtCertifyName)->MoveWindow(rect);
	
	if(_T("Y") == m_strCivilNoDisplayCheck)
	{
		ViewMsg(_T("※ 이름과 주민등록번호를 입력하여 주십시오."));
	}
	else
	{
		ViewMsg(_T("※ 본인인증을 위해서만 사용되며 수집되지 않습니다."));	
	}
	
	((CEdit*)GetDlgItem(IDC_edtCertifyCvilNo1))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_edtCertifyCvilNo2))->SetLimitText(7);
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;
    hIME = ::ImmGetContext(GetDlgItem(IDC_edtCertifyName)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_HANGUL;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence);
        ImmReleaseContext(GetDlgItem(IDC_edtCertifyName)->m_hWnd,hIME); 
    }
	
	if(!m_strName.IsEmpty()) SetDlgItemText(IDC_edtCertifyName, m_strName);
	if(!m_strCivilNo1.IsEmpty()) SetDlgItemText(IDC_edtCertifyCvilNo1, m_strCivilNo1);
	if(!m_strCivilNo2.IsEmpty()) SetDlgItemText(IDC_edtCertifyCvilNo2, m_strCivilNo2);

	return TRUE;  
	              
}

void CUserCertifyDlg::OnbtnCertify() 
{
	m_strCertifyCode = _T("");
	m_strCertifySex = _T("");
	m_strCertifyDate = _T("");
	m_strCertifyWorker = _T("");
	m_strCertifyFailCode = _T("");
	m_strCertifyName = _T("");
	m_strCertifyCivilNo = _T("");

	
	m_strFailCivilNo = _T("");

	CString strName;
	GetDlgItem(IDC_edtCertifyName)->GetWindowText(strName);
	if(strName.IsEmpty())
	{
		ViewMsg(_T("이름을 입력하십시오."));
		GetDlgItem(IDC_edtCertifyName)->SetFocus();
		return;
	}
	CString strCivilNo1;
	GetDlgItem(IDC_edtCertifyCvilNo1)->GetWindowText(strCivilNo1);
	if(6 != strCivilNo1.GetLength())
	{
		ViewMsg(_T("주민번호 앞자리는 6자리입니다."));
		GetDlgItem(IDC_edtCertifyCvilNo1)->SetFocus();
		return;
	}
	CString strCivilNo2;
	GetDlgItem(IDC_edtCertifyCvilNo2)->GetWindowText(strCivilNo2);
	if(7 != strCivilNo2.GetLength())
	{
		ViewMsg(_T("주민번호 뒷자리는 7자리입니다."));
		GetDlgItem(IDC_edtCertifyCvilNo2)->SetFocus();
		return;
	}
	CString strCivilNo = strCivilNo1 + strCivilNo2;

	
	
	BOOL bBeforeSuccess = FALSE;
	if(1 == m_nMode && TRUE == m_pInfo->m_bKLUse && TRUE == m_bIsKLMemberYN) if(!m_strBeforeCode.IsEmpty()) bBeforeSuccess = TRUE;

	CString strCommand = _T("MW_SERVICE_B01");
	CCertifyFrameWriter Writer(m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("도서관부호"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("이름"), strName);
	Writer.addElement(_T("주민등록번호"), strCivilNo);
	CCertifyFrameSender Sender(m_pInfo);
	CCertifyFrameReader Reader;
	CESL_DataMgr TempDM;
	TempDM.SetCONNECTION_INFO(_T(""));
	CString strViewMsg;
 	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		strViewMsg.Format(_T("%d"), ids);
		ViewErrorMsg(0, strViewMsg, _T(""));
		return;
	}
	else
	{
		
		CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime(this, m_strCertifyDate);
		
		m_strCertifyWorker = m_pInfo->USER_ID;
		
		if(m_strCertifySex.IsEmpty())
		{
			TCHAR checkDegit = strCivilNo2.Left(1).GetAt(0);
			if(checkDegit == '1' || checkDegit == '3' || checkDegit == '5' || checkDegit == '7' || checkDegit == '9')
			{
				m_strCertifySex = _T("0");
			}
			else
			{
				m_strCertifySex = _T("1");
			}
		}
 		Reader.makeDM(&TempDM);
		CString strResult;
 		strResult = Reader.getCommand();
 		if(_T("ERROR") == strResult)
 		{
			CString strMsg;
			TempDM.GetCellData(_T("ID"), 0, strResult);
			TempDM.GetCellData(_T("MSG"), 0, strMsg);
			ViewErrorMsg(1, strResult, strMsg);
 		}
		else
		{
			TempDM.GetCellData(_T("처리결과"), 0, strResult);
			CString strDI = _T("");
			CString strSex = _T("");
			if(_T("OK") == strResult)
			{ 
				
				TempDM.GetCellData(_T("실명인증코드"), 0, m_strCertifyCode);
				if(m_strCertifyCode.IsEmpty())
				{
					ViewErrorMsg(1, strResult, _T("본인인증코드 없음"));				
					return;
				}

				
				if(2 != m_nMode)
				{
					
					if(0 != CheckDuplecate())
					{ 
						m_strCertifyCode = _T("");
						m_strCertifySex = _T("");
						m_strCertifyDate = _T("");
						m_strCertifyWorker = _T("");
						m_strCertifyFailCode = _T("");
						m_strCertifyName = _T("");
						m_strCertifyCivilNo = _T("");

						
						m_strFailCivilNo = _T("");

						SetTimer(1, 100, NULL);
						return;
					}
					if(TRUE == m_pInfo->m_bKLUse)
					{
						m_strKlUserNo = _T("");
						m_bKLMemeberSearch = FALSE;
						CString strUserNo;
						CKL_UserManagerDlg dlg(this);
						ids = dlg.GetKlUserSearch(m_strCertifyCode, strCivilNo, strUserNo);
						if(0 == ids)
						{
							m_strKlUserNo = strUserNo;
							m_bKLMemeberSearch = TRUE;
						}
						else
						{
							
							m_bKLMemeberSearch = FALSE;
						}
					}
				}
				
				TempDM.GetCellData(_T("성별"), 0, m_strCertifySex);				
				m_strCertifyName = strName;				
				m_strCertifyCivilNo = strCivilNo;
				ViewMsg(_T("본인인증에 성공하였습니다."));							
				m_strFailCivilNo = _T("");
				SetTimer(0, 1500, NULL);
			}
			else
			{
				ViewErrorMsg(2, strResult, _T(""));				
				m_strCertifyFailCode = strResult;				
				TempDM.GetCellData(_T("성별"), 0, m_strCertifySex);

				if(0 == m_nMode)
				{ 
					m_strCertifyName = strName;
					m_strCertifyCivilNo = strCivilNo;
				}

				
				m_strFailCivilNo = strCivilNo;
			}			
			
			if(1 == m_nMode)
			{				
				if(TRUE == bBeforeSuccess && m_strCertifyCode.IsEmpty())

				{
					m_strCertifyCode = m_strBeforeCode;
					m_strCertifyFailCode = m_strBeforeFailCode;
					m_strCertifyDate = m_strBeforeDate;
					m_strCertifySex = m_strBeforeSex;
					m_strCertifyWorker = m_strBeforeWorker;
					m_strCertifyName = m_strBeforeName;
					m_strCertifyCivilNo = m_strBeforeCivilNo;
				}
				else
				{
					TempDM.StartFrame();
					TempDM.InitDBFieldData();
					TempDM.AddDBFieldData(_T("IPIN_HASH"), _T("STRING"), m_strCertifyCode);
					TempDM.AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), m_strCertifySex);
					CString strDate;
					strDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), m_strCertifyDate);
					TempDM.AddDBFieldData(_T("IPIN_DATE"), _T("NUMERIC"), strDate);
					TempDM.AddDBFieldData(_T("IPIN_RESULT"), _T("STRING"), m_strCertifyFailCode);
					TempDM.AddDBFieldData(_T("CERTIFY_WORKER"), _T("STRING"), m_strCertifyWorker);
					TempDM.AddDBFieldData(_T("NAME"), _T("STRING"), strName);
					m_strCertifyName = strName;
					
					strCivilNo = strCivilNo1 + _T("-") + strCivilNo2;
					CString strBirthDay = _T("DECODE(BIRTHDAY,NULL,TO_DATE('") + SetBirthDay(strCivilNo) + _T("','YYYY/MM/DD'),BIRTHDAY)");
					TempDM.AddDBFieldData(_T("BIRTHDAY"), _T("NUMERIC"), strBirthDay);
					CIndex index;
					index.MakeIndex(strName);
					TempDM.AddDBFieldData(_T("IDX_USER_NAME"), _T("STRING"), strName);
					if(_T("Y") == m_strCivilNoDisplayCheck)
					{
						m_strCertifyCivilNo = strCivilNo;
						
						if(TRUE == m_pInfo->m_bLibSecuUse)
						{
							CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
							if(NULL != p3000Api)
							{
								if(TRUE == p3000Api->ConnectSocket()) 
								{
									CStringArray DataArray;
									DataArray.Add(strCivilNo);
									if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray))
									{
										strCivilNo = DataArray.GetAt(0);
									}
									DataArray.RemoveAll(); DataArray.FreeExtra();
									p3000Api->CloseSocket();
								}
								delete p3000Api;
								p3000Api = NULL;
							}
						}
						
						if(TRUE == m_pInfo->m_bKLUse)
						{
							CString strCryptoCivilNo;
							this->ESL_DES_Encryption(strCivilNo, strCryptoCivilNo);
							CLocCommonAPI::ChangeSecuriyCivilNo(this, strCivilNo, _T("Y"));
							TempDM.AddDBFieldData(_T("CRYPTO_CIVIL_NO"), _T("STRING"), strCryptoCivilNo);
						}
						else
						{
							if (m_strCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0)
								TempDM.AddDBFieldData(_T("CIVIL_NO"), _T("STRING"), strCivilNo);
						}
					}
					else
					{
						m_strCertifyCivilNo = _T("");
					}
					TempDM.AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("실명인증(이용자수정)"),__WFILE__,__LINE__));
					TempDM.MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_strUserKey);
					ids = TempDM.SendFrame();
					if(0 > ids)
					{
						ViewErrorMsg(1, strResult, _T("본인인증 정보 저장 실패"));
						return;
					}
					TempDM.EndFrame();
				}
			}
		}
	}
}

HBRUSH CUserCertifyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcCertifyMassage == pWnd->GetDlgCtrlID() || IDC_stcCertifyReason == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }
	else if(IDC_stcCertifyFailMassage == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(242,59,12));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

void CUserCertifyDlg::ViewMsg(CString strMsg)
{
	GetDlgItem(IDC_stcCertifyMassage)->ShowWindow(TRUE);
	GetDlgItem(IDC_stcCertifyFailMassage)->ShowWindow(FALSE);

	CRect rect;
	GetWindowRect(&rect);
	ClientToScreen(rect);
	rect.right = rect.left + m_nWidth;
	rect.bottom = rect.top + m_nHeight;
	ScreenToClient(rect);
	MoveWindow(rect);

	GetDlgItem(IDC_stcCertifyMassage)->SetWindowText(strMsg);
}

void CUserCertifyDlg::ViewErrorMsg(int nMode, CString strCode, CString strMsg) 
{
	
	GetDlgItem(IDC_stcCertifyFailMassage)->ShowWindow(TRUE);
	GetDlgItem(IDC_stcCertifyMassage)->ShowWindow(FALSE);
	
	CRect rect;
	GetWindowRect(&rect);
	ClientToScreen(rect);
	rect.right = rect.left + m_nFailWidth;
	rect.bottom = rect.top + m_nFailHeight;
	if(1 == nMode) rect.bottom -= 168;
	ScreenToClient(rect);
	MoveWindow(rect);
	
	CString strValue;
	strValue.Format(_T("본인인증에 실패하였습니다."));
	
	GetDlgItem(IDC_stcCertifyFailMassage)->SetWindowText(strValue);
	if(0 == nMode)
	{ 
		strValue.Format(_T("미들웨어와의 통신에 실패하였습니다.[%s]"), strCode);
		GetDlgItem(IDC_edtCertifyReason)->SetWindowText(strValue);
		strValue.Format(_T("미들웨어의 동작 여부를 확인하여주십시오.\r\n미들웨어를 재가동 한후에도 상황이 지속된다면 관리자에게 문의하여 주시기 바랍니다."));
		GetDlgItem(IDC_edtCertifyAction)->SetWindowText(strValue);
	}
	else if(1 == nMode)
	{ 
		GetDlgItem(IDC_edtCertifyReason)->SetWindowText(strMsg);
	}
	else if(2 == nMode)
	{ 
		strValue.Format(_T("%d"), m_pInfo->m_nCertifyAgency);
		CString strDesc = _T(""), strAction = _T("");
		m_pInfo->GetCertifyFailCodeToDesc(strValue, strCode, strDesc, strAction);
		GetDlgItem(IDC_edtCertifyReason)->SetWindowText(strDesc);
		GetDlgItem(IDC_edtCertifyAction)->SetWindowText(strAction);
	}
}


void CUserCertifyDlg::OnbtnCertifyClose() 
{
	OnCancel();
}

void CUserCertifyDlg::OnTimer(UINT nIDEvent) 
{
	if(0 == nIDEvent)
	{
		KillTimer(0);
		OnOK();
	}
	if(1 == nIDEvent)
	{
		KillTimer(1);
		OnbtnCertifyClose();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CUserCertifyDlg::OnChangeedtCertifyCvilNo1() 
{
	CString strGetData;
	GetDlgItemText(IDC_edtCertifyCvilNo1, strGetData);
	if(6 == strGetData.GetLength()) GetDlgItem(IDC_edtCertifyCvilNo2)->SetFocus();
}

int CUserCertifyDlg::CheckDuplecate()
{
	CESL_DataMgr pDM;
	pDM.SetCONNECTION_INFO(_T(""));
	CString strQuery, strWhere, strValue;
	if(!m_strCertifyCode.IsEmpty())
	{
		if(0 == m_nMode) strWhere.Format(_T("IPIN_HASH='%s'"), m_strCertifyCode);
		else strWhere.Format(_T("(IPIN_HASH='%s' AND REC_KEY<>%s)"), m_strCertifyCode, m_strUserKey);
	}
	
	CString strCivilNo = _T("");
	if(_T("Y") == m_strCivilNoDisplayCheck)
	{
		CString strCivilNo1, strCivilNo2;
		GetDlgItem(IDC_edtCertifyCvilNo1)->GetWindowText(strCivilNo1);
		GetDlgItem(IDC_edtCertifyCvilNo2)->GetWindowText(strCivilNo2);
		strCivilNo.Format(_T("%s-%s"), strCivilNo1, strCivilNo2);
		
		if(TRUE == m_pInfo->m_bKLUse)
		{
			CString strCryptoCivilNo;
			this->ESL_DES_Encryption(strCivilNo, strCryptoCivilNo);
			if(!strWhere.IsEmpty()) strWhere += _T(" OR ");
			if(0 == m_nMode) strValue.Format(_T("CRYPTO_CIVIL_NO='%s'"), strCryptoCivilNo);
			else strValue.Format(_T("(CRYPTO_CIVIL_NO='%s' AND REC_KEY<>%s)"), strCryptoCivilNo, m_strUserKey);
		}
		else
		{
			
			if(TRUE == m_pInfo->m_bLibSecuUse)
			{
				CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
				if(NULL != p3000Api)
				{
					if(TRUE == p3000Api->ConnectSocket()) 
					{
						CStringArray DataArray;
						DataArray.Add(strCivilNo);
						if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray))
						{
							strCivilNo = DataArray.GetAt(0);
						}
						DataArray.RemoveAll(); DataArray.FreeExtra();
						p3000Api->CloseSocket();
					}
					delete p3000Api;
					p3000Api = NULL;
				}
			}
			if(!strWhere.IsEmpty()) strWhere += _T(" OR ");
			if(0 == m_nMode) strValue.Format(_T("CIVIL_NO='%s'"), strCivilNo);
			else strValue.Format(_T("(CIVIL_NO='%s' AND REC_KEY<>%s)"), strCivilNo, m_strUserKey);
		}
		strWhere += strValue;
	}
	strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE %s"), strWhere);
	int ids = pDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) return ids;
	if(0 < _ttoi(strValue))
	{
		CUserDuplecateDlg dlg(this);
		dlg.m_nMode = m_nMode;
		dlg.m_strUserKey = m_strUserKey;
		dlg.m_strCertifyCode = m_strCertifyCode;
		dlg.m_strCivilNo = strCivilNo;
		if(IDOK != dlg.DoModal()) return 1;
	}
	return 0;
}

CString CUserCertifyDlg::SetBirthDay(CString strCivilNo)
{
	strCivilNo.Replace( _T("-") , _T("") );
	strCivilNo.TrimLeft();	strCivilNo.TrimRight();
	if(13 != strCivilNo.GetLength()) return _T("");
	CString sBirthDay;
	INT nSex = _ttoi(strCivilNo.Mid(6,1));
	if(nSex==1||nSex==2||nSex==5||nSex==6) 
	{ 
		sBirthDay = _T("19");
	}
	else if(nSex==3||nSex==4||nSex==7||nSex==8) 
	{ 
		sBirthDay = _T("20");
	}
	else if(nSex==9||nSex==0) 
	{ 
		sBirthDay = _T("18");
	}
	else 
	{ 
		return _T("");
	}
	INT nYear = _ttoi(sBirthDay + strCivilNo.Left(2));
	INT nMonth = _ttoi(strCivilNo.Mid(2,2));
	INT nDay = _ttoi(strCivilNo.Mid(4,2));

	BOOL bVaildDate = FALSE;
	if( 1 <= nMonth && 12 >= nMonth )
	{
		INT nMonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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
	sBirthDay.Format(_T("%04d/%02d/%02d"), nYear, nMonth, nDay);
	return sBirthDay;
}