#include "stdafx.h"
#include "KL_UserManagerDlg.h"
#include "FrameManager.h"

#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKL_UserManagerDlg::CKL_UserManagerDlg(CESL_Mgr* pParent)
: CESL_Mgr(CKL_UserManagerDlg::IDD, pParent)
{
	
	m_pParentMgr = pParent;

	m_nMode = 0;

	m_bIsTCTInput = FALSE;
	m_bIsRealNameCert = FALSE;
	m_bIsRunningThread = FALSE;
	m_bIsUpdateKLMemberLibManageInfo = FALSE;

	m_strUserNo = _T("");
	m_strRFIDUID = _T("");
	m_strCivilNo = _T("");
	m_strUserName = _T("");
	m_strGPIN_ASCII = _T("");
	m_strGPIN_HASH = _T("");
	m_strGPIN_SEX = _T("");
	m_strBeforeUserNo = _T("");
	m_strBirthYear = _T("");
	m_bIsUserNoDuplicate = FALSE;

	m_bShowBtn = FALSE;
	m_rBtn[0] = CRect(220, 195, 305, 235);
 	m_rBtn[1] = CRect(305, 195, 390, 235);
	m_nBtnMode[0] = 0;
	m_nBtnMode[1] = 0;

	m_bInitDlg =  FALSE;
 	m_bIsRetry = FALSE;
	m_bIsKLSecede = FALSE;

	
	m_strCI = _T("");
}
CKL_UserManagerDlg::~CKL_UserManagerDlg()
{
}

void CKL_UserManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CKL_UserManagerDlg) 
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CKL_UserManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_UserManagerDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnRetry, OnbtnRetry)
	ON_BN_CLICKED(IDC_btnLOC_USERCLOSE, OnbtnCLOSE)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CKL_UserManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());

	if(0>InitESL_Mgr(_T("SM_KL_USER_MANAGER")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	if(!m_pInfo->IsConnectUnityLoanService())
	{
		SetTimer(1, 100, NULL);
		return TRUE;
	}

	if(!m_MemDC.GetSafeHdc())
	{
		CDC* pDC = GetDC();
		m_MemDC.CreateCompatibleDC(pDC);
		m_MemBitmap.CreateCompatibleBitmap(pDC, 400, 250);
		m_MemDC.SelectObject(&m_MemBitmap);
		m_MemDC.SetBkMode(TRANSPARENT);

		m_BGDC.CreateCompatibleDC(pDC);
		m_BGBitmap.LoadBitmap(IDB_KL_MSG);		
		m_BGDC.SelectObject(&m_BGBitmap);
		
		ReleaseDC(pDC);
	}
	SetWindowPos(NULL, 0, 0, 400, 250, SWP_NOMOVE|SWP_NOZORDER);
	
	CRgn Rgn;
	Rgn.CreateRoundRectRgn(2, 2, 399, 248, 9, 9);
	SetWindowRgn(Rgn, TRUE);
	
	m_bInitDlg = TRUE;
	StartSearch();

	return TRUE;  
	              
}

BOOL CKL_UserManagerDlg::OnEraseBkgnd(CDC* pDC) 
{
	
	if(!m_MemDC.GetSafeHdc())
	{
		return FALSE;
	}
	TRACE(_T("++++++++++%s+++++++++++++\r\n"), m_strMsg);
	m_MemDC.BitBlt(0, 0, 400, 250, &m_BGDC, 0, 0, SRCCOPY);
	m_MemDC.DrawText(m_strMsg, CRect(52, 152, 352, 170), DT_SINGLELINE|DT_VCENTER|DT_CENTER);
  	if(m_bShowBtn)
	{
		m_MemDC.BitBlt(m_rBtn[0].left, m_rBtn[0].top, m_rBtn[0].Width(), m_rBtn[0].Height(), &m_BGDC, m_nBtnMode[0] * 85, 250, SRCCOPY);
		m_MemDC.BitBlt(m_rBtn[1].left, m_rBtn[1].top, m_rBtn[1].Width(), m_rBtn[1].Height(), &m_BGDC, m_nBtnMode[1] * 85, 290, SRCCOPY);
	}

	pDC->BitBlt(0, 0, 400, 250, &m_MemDC, 0, 0, SRCCOPY);
	

	return TRUE;
}

BOOL CKL_UserManagerDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnRetry)->m_hWnd == pMsg->hwnd ) OnbtnRetry();
			else if(GetDlgItem(IDC_btnLOC_USERCLOSE)->m_hWnd == pMsg->hwnd ) OnbtnCLOSE();
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			if(TRUE == m_bIsRunningThread) return TRUE;
		}
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

void CKL_UserManagerDlg::OnbtnRetry() 
{
	if(TRUE == m_bIsRunningThread) return;
	m_bIsRetry = TRUE;
	StartSearch();
}

void CKL_UserManagerDlg::OnbtnCLOSE() 
{
	if(TRUE == m_bIsRunningThread) return;
	SetTimer(1, 100, NULL);
}

VOID CKL_UserManagerDlg::SetMessage(BOOL bButtonShow, CString strMsg)
{
	m_strMsg = strMsg;
	m_bShowBtn = bButtonShow;

	if(m_bInitDlg)
	{
		Invalidate();
	}
}

INT CKL_UserManagerDlg::StartSearch()
{
EFS_BEGIN

	CString strFuncName = _T("StartSearch");

	
 	if(!m_pInfo->IsConnectUnityLoanService() && FALSE == m_bIsRetry)
 	{		
 		SetMessage(TRUE, _T("미들웨어서비스가 실행중이 아닙니다."));
 		return 1010;
 	}

	if(m_bIsRetry)
	{
		m_pInfo->SetConnectUnityLoanService(TRUE);
	}

	SetTimer(m_nMode+2, 100, NULL);

	return 0;

EFS_END
return -1;
}

void CKL_UserManagerDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	if(0==nIDEvent)
	{
		OnOK();
	}
	else if(1==nIDEvent)
	{
		OnCancel();
	}
	else if(2==nIDEvent)
	{
		AfxBeginThread(RealNameCertThread, this);
	}
	else if(3==nIDEvent)
	{
		AfxBeginThread(MemberInsertThread, this);
	}
	else if(4==nIDEvent)
	{
		AfxBeginThread(MemberChangeThread, this);
	}
	else if(5==nIDEvent)
	{
		AfxBeginThread(CreateCurrentLoanInfoThread, this);
	}
	else if(6==nIDEvent)
	{
		AfxBeginThread(MemberUpdateThread, this);
	}
	else if(7==nIDEvent)
	{
		AfxBeginThread(UpdateKLMemberLibManageInfoThread, this);
	}
	else if(8==nIDEvent)
	{
		AfxBeginThread(KLIssueMembershipcardThread, this);
	}
	else if(9==nIDEvent)
	{
		AfxBeginThread(KLUserNoDuplicateCheckThread, this);
	}
	else if(10==nIDEvent)
	{
		AfxBeginThread(KLUserDeleteThread, this);
	}
	else if(11==nIDEvent)
	{
		AfxBeginThread(KLIsSecedeMemberThread, this);
	}

	CDialog::OnTimer(nIDEvent);
}

UINT CKL_UserManagerDlg::RealNameCertThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_RealNameCert();
	pDlg->m_bIsRunningThread = FALSE;

	
	if(0>ids) 
	{
		return 0;
	}
	else if(0 < ids)
	{
		pDlg->SetTimer(1, 200, NULL);
	}
	else
	{
		pDlg->SetTimer(0, 200, NULL);
	}



	return 0;
}

UINT CKL_UserManagerDlg::MemberInsertThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_MemberInsert();
	pDlg->m_bIsRunningThread = FALSE;
	if(0>ids) return 0;

	pDlg->SetTimer(0, 200, NULL);

	return 0;
}

UINT CKL_UserManagerDlg::MemberChangeThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_MemberChange();
	pDlg->m_bIsRunningThread = FALSE;
	if(0>ids) return 0;

	pDlg->SetTimer(0, 200, NULL);

	return 0;
}

UINT CKL_UserManagerDlg::CreateCurrentLoanInfoThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_CreateCurrentLoanInfo();
	pDlg->m_bIsRunningThread = FALSE;
	if(0>ids) return 0;

	pDlg->SetTimer(0, 200, NULL);

	return 0;
}

UINT CKL_UserManagerDlg::MemberUpdateThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_MemberUpdate();
	pDlg->m_bIsRunningThread = FALSE;
	if(0>ids) return 0;

	pDlg->SetTimer(0, 200, NULL);

	return 0;
}

UINT CKL_UserManagerDlg::UpdateKLMemberLibManageInfoThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_UpdateKLMemberLibManageInfo();
	pDlg->m_bIsRunningThread = FALSE;
	if(0>ids) return 0;

	pDlg->SetTimer(0, 200, NULL);

	return 0;
}

UINT CKL_UserManagerDlg::KLIssueMembershipcardThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_Membershipcard();
	pDlg->m_bIsRunningThread = FALSE;
	if(0>ids) return 0;

	pDlg->SetTimer(0, 200, NULL);
	return 0;
}

UINT CKL_UserManagerDlg::KLUserNoDuplicateCheckThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_UserNoDuplicateCheck();
	pDlg->m_bIsRunningThread = FALSE;
	if(0>ids) return 0;

	pDlg->SetTimer(0, 200, NULL);
	return 0;
}

UINT CKL_UserManagerDlg::KLUserDeleteThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_UserDelete();
	pDlg->m_bIsRunningThread = FALSE;
	if(0>ids) return 0;

	pDlg->SetTimer(0, 200, NULL);
	return 0;
}

UINT CKL_UserManagerDlg::KLIsSecedeMemberThread(LPVOID pParam)
{
	CKL_UserManagerDlg* pDlg = (CKL_UserManagerDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->KL_IsSecedeMember();
	pDlg->m_bIsRunningThread = FALSE;
	if(0>ids) return 0;

	pDlg->SetTimer(0, 200, NULL);
	return 0;
}


INT CKL_UserManagerDlg::KL_RealNameCert()
{
EFS_BEGIN

	INT ids;

	SetMessage(FALSE, _T("회원의 본인인증을 시작합니다."));
	
	
	CESL_DataMgr TmpDM;
	ids = CLocCommonAPI::KL_RealNameCertification(this, m_strUserName, m_strCivilNo, TmpDM);
	if(0 > ids)
	{
		CString strMsg;
		if(-1010==ids)
		{
			CString strTemp;
			TmpDM.GetCellData(0, 0, strTemp);
			
			if(-1 < strTemp.Find(_T("SY04")))
			{
				strMsg.Format(_T("이름, 주민번호 불일치"));
			}
			else if(-1 < strTemp.Find(_T("SY02")))
			{
				strMsg.Format(_T("이름을 확인하십시오."));
			}
			
			else if(-1 < strTemp.Find(_T("SY06")))
			{
				strMsg.Format(_T("명의도용 차단한 이용자입니다."));
			}
			else
			{
				strMsg.Format(_T("시스템 오류"));
			}

			SetMessage(TRUE, strMsg);

			strMsg.Format(_T("본인인증(1) ERROR CODE : %s"), strTemp);			
		}
		else if(-1020 == ids || -1030 == ids)
		{
			strMsg = _T("시스템 오류");
			SetMessage(TRUE, strMsg);

			strMsg.Format(_T("본인인증(1) ERROR CODE : %d"), ids);		
		}
		else
		{
			strMsg = _T("미들웨어 접속오류");
			SetMessage(TRUE, strMsg);

			strMsg.Format(_T("본인인증(1) ERROR CODE : %d"), ids);	
		}

		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return ids;
	}

	m_strGPIN_ASCII.Empty();
	m_strGPIN_HASH.Empty();
	m_strGPIN_SEX.Empty();
	m_strUserNo.Empty();

	
	m_strCI.Empty();

	TmpDM.GetCellData(_T("G-PINASCII인증코드"), 0, m_strGPIN_ASCII);
	TmpDM.GetCellData(_T("G-PINHASH인증코드"), 0, m_strGPIN_HASH);
	TmpDM.GetCellData(_T("G-PIN성별"), 0, m_strGPIN_SEX);
	TmpDM.GetCellData(_T("통합대출자번호"), 0, m_strUserNo);

	TmpDM.GetCellData(_T("연계정보"), 0, m_strCI);

	if(!m_strUserNo.IsEmpty())
	{
		CESL_DataMgr pDM;
		pDM.SetCONNECTION_INFO(_T(""));
		CString strQuery, strValue;
		
		strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_NO='%s' AND KL_MEMBER_YN='Y'"), m_strUserNo);
		ids = pDM.GetOneValueQuery(strQuery, strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("KL_RealNameCert"));
		if(0 < _ttoi(strValue))
		{
			strValue.Format(_T("본인인증한 이용자는 자관의 회원입니다.\r\n대출자번호 : %s"), m_strUserNo);
			
			AfxMessageBox(strValue);
			return 1;
		}
	}
	
	m_bIsRealNameCert = TRUE;

	if(!m_strUserNo.IsEmpty())
	{		
		ids = KL_GetMemberInfo(m_strUserNo);
		return ids;
	}

	return 0;
EFS_END
return -1;
}


INT CKL_UserManagerDlg::KL_MemberInsert()
{
EFS_BEGIN

	INT ids;
	CString strViewMsg = _T("통합 회원의 정보를 생성합니다.");
	SetMessage(FALSE, strViewMsg);
	
	CString strCommand = _T("IL_K02_LOC_B01_SERVICE");
	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("통합대출자키"), m_strKLUserKey);
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);
	Writer.addElement(_T("대출자이름"), m_strUserName);
	Writer.addElement(_T("생성도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);	
	m_strCreateDate = KL_CreateDate();
	Writer.addElement(_T("최초생성일"), m_strCreateDate);
	Writer.addElement(_T("핸드폰"), m_strHandphone);
	Writer.addElement(_T("G-PINASCII인증코드"), m_strGPIN_ASCII);
	Writer.addElement(_T("G-PINHASH인증코드"), m_strGPIN_HASH);
	Writer.addElement(_T("G-PIN성별"), m_strGPIN_SEX);
	Writer.addElement(_T("회원상태"), _T("0"));
	Writer.addElement(_T("회원구분"), _T("0"));

	if(!m_strBirthYear.IsEmpty())
	{
		Writer.addElement(_T("생년"), m_strBirthYear);
	}
	else
	{
		Writer.addElement(_T("생년"), KL_GetBirthYearToCivilNo(m_strCivilNo));
	}

	Writer.addElement(_T("연계정보"), m_strCI);

	CFrameSender Sender(m_pInfo);
 	
	CFrameReader Reader;
	INT nRetryCnt = 3;
	CString strMsg;
	CESL_DataMgr TempDM;
	BOOL bComplete = FALSE;
	for(INT i = 0; i < nRetryCnt; i++)
	{
 		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			bComplete = FALSE;
			strViewMsg = _T("미들웨어와의 통신에 실패하였습니다.");
			strMsg.Format(_T("신규회원등록(2) ERROR CODE : %d"), ids);
			break;
		}
		else
		{
 			Reader.makeDM(&TempDM);
			CString strResult;
 			strResult = Reader.getCommand();
 			if(_T("ERROR") == strResult)
 			{
				bComplete = FALSE;
				strResult = Reader.getElement(0, 0);
				strViewMsg.Format(_T("서비스 오류(%s)"), strResult);
				strMsg.Format(_T("신규회원등록(2) ERROR CODE : %s"), strResult);
				
				if(-1 < strResult.Find(_T("SY")) || -1 < strResult.Find(_T("DB")) || -1 < strResult.Find(_T("DT")))
				{
					break;
				}
 			}
			else if(1 > TempDM.GetRowCount())
			{
				bComplete = FALSE;
				strViewMsg.Format(_T("요청결과 없음"));
				strMsg.Format(_T("신규회원등록(2) ERROR CODE : null"));
			}
			else
			{
				TempDM.GetCellData(_T("처리결과"), 0, strResult);
				if(_T("OK") == strResult)
				{
					bComplete = TRUE;
					break;
				}
				else
				{
					bComplete = FALSE;
					if(strResult.IsEmpty())
					{
						strViewMsg.Format(_T("처리결과 없음"));
						strMsg.Format(_T("신규회원등록(2) ERROR CODE : 처리결과없음"));
					}
					else
					{
						strViewMsg.Format(_T("처리결과 오류(%s)"), strResult);
						strMsg.Format(_T("신규회원등록(2) ERROR CODE : %s"), strResult);
					}
				}
			}
		}
	}
	if(FALSE == bComplete)
	{	
		TempDM.QueryLog(strMsg.GetBuffer(0));
		SetMessage(TRUE, strViewMsg);
		return -1;
	}
	SetMessage(FALSE, _T("신규가입완료"));

	return 0;

EFS_END
return -1;
}


INT CKL_UserManagerDlg::KL_MemberChange()
{
EFS_BEGIN

	
	INT ids = KL_MemberInsert();
	if(0>ids) return ids;

	
	ids = KL_CreateCurrentLoanInfo();
	if(0>ids) return ids;

	return ids;
EFS_END
return -1;
}


INT CKL_UserManagerDlg::KL_CreateCurrentLoanInfo()
{
EFS_BEGIN
	
	m_nMode = 3;
	INT ids;
	CString strFuncName = _T("KL_CreateCurrentLoanInfo");
	CString strUserKey, strUserNo;
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300") );
	if(NULL==pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = pDM->GetCellData(_T("REC_KEY"), 0, strUserKey);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);

	ids = pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);

	SetMessage(FALSE, _T("자관의 현황 정보를 올리는 중입니다."));
	CString strMsg;
	ids = CLocCommonAPI::KL_CreateCurrentLoanInfo(this, strUserKey, strUserNo);
	if(0>ids)
	{
		strMsg.Format(_T("대출현황정보생성 ERROR CODE : %d"), ids);
		pDM->QueryLog(strMsg.GetBuffer(0));	
	}

	return ids;
EFS_END
return -1;
}

INT CKL_UserManagerDlg::KL_MemberUpdate()
{
EFS_BEGIN
	
	INT ids;
	const INT nColCnt = 21;
	CString strFuncName = _T("KL_MemberUpdate");
	CString strData;
	CString strDMAlias[nColCnt] = 
	{
		_T("대출자번호"),	_T("이름"),				_T("IDX_이름"),			_T("휴대폰"),			_T("IDX_핸드폰"),
		_T("E_MAIL"),		_T("근무지내선번호"),	_T("부서명"),			_T("직급명"),			_T("학교"),
		_T("학년"),			_T("반"),				_T("교번/순번/군번"),	_T("근무처"),			_T("WORKNO"),
		_T("자택주소"),		_T("자택전화"),			_T("자택우편번호"),		_T("근무지우편번호"),	_T("근무지전화"),
		_T("근무지주소")
	};
	
	CString strFieldAlias[nColCnt] = 
	{
		_T("통합대출자번호"),	_T("대출자이름"),	_T("대출자색인이름"),	_T("핸드폰"),			_T("검색용핸드폰"),
		_T("EMAIL"),			_T("내선번호"),		_T("근무지부서명"),		_T("근무지직급명"),		_T("학교"),
		_T("학년"),				_T("반"),			_T("학번"),				_T("근무지"),			_T("회원증RFID"),
		_T("집주소"),			_T("집전화번호"),	_T("집우편번호"),		_T("근무지우편번호"),	_T("근무지전화번호"),
		_T("근무지주소")
	};
	
	CString strCommand = _T("IL_K24_LOC_B01_SERVICE");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300") );
	if(NULL==pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetMessage(FALSE, _T("통합 회원의 정보를 수정합니다."));
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();

	for(INT i=0; i<nColCnt; i++)
	{
		ids = pDM->GetCellData(strDMAlias[i], 0, strData);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);

		if(0==i)
		{
			m_strUserNo  = strData;
			if(!m_strBeforeUserNo.IsEmpty())
			{
				Writer.addElement(_T("재부여통합대출자번호"),strData);
				strData = m_strBeforeUserNo;
			}
		}
	
		Writer.addElement(strFieldAlias[i], strData);
	}

	
	CString strModifyDate;
	
	
	CTime t = CLocCommonAPI::GetCurrentDateTime(this, strModifyDate);
	Writer.addElement(_T("수정일자"), strModifyDate);
	Writer.addElement(_T("수정도서관부호"), m_pInfo->LIB_CODE);
	
	CString strGpinHash;
	pDM->GetCellData(_T("GPIN_HASH"), 0, strGpinHash);
	Writer.addElement(_T("G-PINHASH인증코드"), strGpinHash);
	
	Writer.addElement("검색용CI", m_strCI);
	CStringArray astrFrames;
	astrFrames.Add(Writer.getFrame());

	
	
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	if(!m_strBeforeUserNo.IsEmpty())
	{
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0>ids)
		{
			ids = Sender.SendTempTbl(astrFrames, GetWorkLogMsg( _T("통합회원정보갱신"), __WFILE__, __LINE__ ));
		}
		else
		{
			CESL_DataMgr TmpDM;
			Reader.makeDM(&TmpDM);
			CString strResult = Reader.getCommand();
			if(_T("ERROR")==strResult  || 1>TmpDM.GetRowCount())
			{
				ids = Sender.SendTempTbl(astrFrames, GetWorkLogMsg( _T("통합회원정보갱신"), __WFILE__, __LINE__ ));
			}
		}
	}
	else
	{
		ids = Sender.SendTempTbl(astrFrames, GetWorkLogMsg( _T("통합회원정보갱신"), __WFILE__, __LINE__ ));
	}

	if(m_bIsUpdateKLMemberLibManageInfo)
	{
		ids = KL_UpdateKLMemberLibManageInfo();
		return ids;
	}

	return 0;
EFS_END
return -1;
}

INT CKL_UserManagerDlg::KL_UpdateKLMemberLibManageInfo()
{
EFS_BEGIN

	if(m_strUserNo.IsEmpty()) return 0;

	m_nMode = 5;
	CString strFuncName = _T("KL_UpdateKLMemberLibManageInfo");
	CString strUserClass, strMemberClass, strLoanStopData, strSetLoanStopData , strCommand;
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300") );
	if(NULL==pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	INT ids = pDM->GetCellData(_T("불량회원구분"), 0, strUserClass);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);

	if(_T("정상회원")==strUserClass) strUserClass = _T("0");
	else if(_T("대출정지")==strUserClass) strUserClass = _T("1");
	else if(_T("제적회원")==strUserClass) strUserClass = _T("2");
	else if(_T("탈퇴회원")==strUserClass) strUserClass = _T("3");
	else strUserClass = _T("0");

	ids = pDM->GetCellData(_T("회원구분"), 0, strMemberClass);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);

	if(_T("정회원")==strMemberClass) strMemberClass = _T("0");
	else if(_T("비회원")==strMemberClass) strMemberClass = _T("1");
	else if(_T("준회원")==strMemberClass) strMemberClass = _T("2");
	else strMemberClass = _T("0");

	ids = pDM->GetCellData(_T("대출정지일"), 0, strLoanStopData);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1040, strFuncName);

	ids = pDM->GetCellData(_T("대출정지부여일"), 0, strSetLoanStopData);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1050, strFuncName);

	strCommand = _T("IL_K28_LOC_B01_SERVICE");
	SetMessage(FALSE, _T("통합 회원의 정보를 수정합니다."));

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);
	Writer.addElement(_T("회원상태"), strUserClass);
	Writer.addElement(_T("회원구분"), strMemberClass);
	Writer.addElement(_T("최종정보수정일"), KL_CreateDate());
	Writer.addElement(_T("대출정지일"), strLoanStopData);
	Writer.addElement(_T("대출정지부여일"), strSetLoanStopData);
	Writer.addElement(_T("수정도서관부호"), m_pInfo->LIB_CODE);

	CStringArray astrFrames;
	astrFrames.Add(Writer.getFrame());

	
	CFrameSender Sender(m_pInfo);
	ids = Sender.SendTempTbl(astrFrames, GetWorkLogMsg( _T("통합회원도서관관리정보수정"), __WFILE__, __LINE__ ));
	if(0>ids)
	{
		CString strMsg;


		strMsg.Format(_T("통합회원도서관관리정보수정(28) ERROR CODE : %d"),ids);
		CESL_DataMgr TmpDM;
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return -1040;
	}

	return 0;
EFS_END
return -1;
}


INT CKL_UserManagerDlg::KL_GetMemberInfo(CString strUserNo, BOOL bMsgShow/*=TRUE*/)
{
EFS_BEGIN

	if(TRUE == bMsgShow) SetMessage(FALSE, _T("통합 센터에서 회원 정보를 반입 받습니다."));

	CString strCommand = _T("IL_K08_LOC_B01_SERVICE");
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), strUserNo);

	CESL_DataMgr TmpDM;
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
 	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0>ids)
	{
		CString strMsg;
		strMsg.Format(_T("미들웨어와의 통신에 실패하였습니다."));
		
		if(TRUE == bMsgShow) SetMessage(TRUE, strMsg);

		strMsg.Format(_T("통합회원조회(8) ERROR CODE : %d"),ids);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		m_bIsRealNameCert = FALSE;
		return ids;
	}

	Reader.makeDM(&TmpDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp || 1>TmpDM.GetRowCount())
	{
		CString strMsg,strTemp;
		TmpDM.GetCellData(0, 0, strTemp);
		if(0<strTemp.Find(_T("DT")))
		{
			return 0;
		}

		strMsg.Format(_T("미들웨어와의 통신에 실패하였습니다."));
		
		if(TRUE == bMsgShow) SetMessage(TRUE, strMsg);

		strMsg.Format(_T("통합회원조회(8) ERROR CODE : %s"),strTemp);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		m_bIsRealNameCert = FALSE;
		return -1010;
	}
	if(1>TmpDM.GetRowCount())
	{
		TmpDM.QueryLog(_T("통합회원조회(8) return is null"));
		m_bIsRealNameCert = FALSE;
		return -1020;
	}

	const INT nColCnt = 25;
 	CString strFieldAlias[nColCnt] =
 	{
		_T("통합대출자번호"),	_T("대출자이름"),	_T("생일"),			_T("생일타입"),		_T("주민등록번호"),
		_T("내선번호"),			_T("근무지부서명"),	_T("근무지직급명"),	_T("핸드폰"),		_T("EMAIL"),
		_T("학교"),				_T("학년"),			_T("반"),			_T("학번"),			_T("우편발송지역"), 
		_T("비고"),				_T("집주소"),		_T("집전화번호"),	_T("집우편번호"),	_T("근무지우편번호"),
		_T("근무지전화번호"),	_T("근무지주소"),	_T("근무지"),		_T("통합대출자키"),	_T("회원증RFID")
 	};

 	CString strDMAlias[nColCnt] = 
	{
		_T("대출자번호"),		_T("이름"),		_T("생일"),		_T("생일TYPE"),			_T("주민등록번호"),
		_T("근무지내선번호"),	_T("부서명"),	_T("직급명"),	_T("휴대폰"),			_T("E_MAIL"),
		_T("학교"),				_T("학년"),		_T("반"),		_T("교번/순번/군번"),	_T("우편발송지역"), 
		_T("비고"),				_T("자택주소"),	_T("자택전화"),	_T("자택우편번호"),		_T("근무지우편번호"), 
		_T("근무지전화"),		_T("근무지주소"), _T("근무처"),	_T("통합대출자키"),		_T("회원증RFID")
	};
	
	CString strData;
	CESL_DataMgr* pDM = FindDM(_T("DM_KL_USER_MANAGER_MEMBER_INFO"));
	pDM->InsertRow(-1);
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0; i<nColCnt; i++)
	{
		strData = TmpDM.GetCellData(strFieldAlias[i], 0);
		INT ids = pDM->SetCellData(strDMAlias[i], strData, 0);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1070, _T("KL_GetMemberInfo"));
	}

	m_bIsTCTInput = TRUE;

	return 0;

EFS_END
return -1;
}

INT CKL_UserManagerDlg::KL_Membershipcard()
{
EFS_BEGIN

	SetMessage(FALSE, _T("통합 회원증 발급 처리 중 입니다."));
	
	CStringArray astrFrames;
	CString strCommand = _T("IL_K07_LOC_B01_SERVICE");
	CFrameWriter Writer(m_pInfo);

	Writer.clear();
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);
	
	Writer.addElement(_T("G-PINHASH인증코드"), m_strGPIN_HASH);
	Writer.addElement(_T("발급도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
	Writer.addElement(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg( _T("통합회원증발급"), __WFILE__, __LINE__ ));
	Writer.addElement(_T("발급일"), KL_CreateDate());
	Writer.addElement(_T("회원증발급비고"), _T(""));

	astrFrames.Add(Writer.getFrame());
	
	CFrameSender Sender(m_pInfo);
	INT ids = Sender.SendTempTbl(astrFrames, GetWorkLogMsg( _T("통합회원정보갱신"), __WFILE__, __LINE__ ));
	if(0>ids)
	{
		CString strMsg;


		strMsg.Format(_T("통합회원증발급(7) ERROR CODE : %d"),ids);
		CESL_DataMgr TmpDM;
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return -1040;
	}

	return 0;
EFS_END
return -1;
}

INT CKL_UserManagerDlg::KL_UserNoDuplicateCheck()
{
EFS_BEGIN

	SetMessage(FALSE, _T("통합대출자번호 중복 검사를 합니다."));
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K49_LOC_B02_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo );

	CESL_DataMgr TmpDM;
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0>ids)
	{
		CString strMsg;
		strMsg.Format(_T("미들웨어와의 통신에 실패하였습니다."));
		SetMessage(TRUE, strMsg);
		strMsg.Format(_T("쿼리PROC(26) ERROR CODE : %d"),ids);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return ids;
	}

	Reader.makeDM(&TmpDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp)
	{
		CString strMsg,strTemp;
		TmpDM.GetCellData(0, 0, strTemp);
		strMsg.Format(_T("미들웨어와의 통신에 실패하였습니다."));
		SetMessage(TRUE, strMsg);
		strMsg.Format(_T("쿼리PROC(26) ERROR CODE : %s"),strTemp);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return -1010;
	}
	if(1>TmpDM.GetRowCount())
	{
		TmpDM.QueryLog(_T("쿼리PROC(26) return is null"));
		return 1010;
	}

	m_bIsUserNoDuplicate = FALSE;
	if( TmpDM.GetCellData(_T("책이음회원여부"),0) == _T("Y") ) m_bIsUserNoDuplicate = TRUE;

	return 0;
EFS_END
return -1;
}

INT CKL_UserManagerDlg::KL_UserDelete()
{
EFS_BEGIN

	INT ids;
	SetMessage(FALSE, _T("통합 회원 삭제 처리 중 입니다."));
	
	CStringArray astrFrames;
	CString strCommand = _T("IL_K31_LOC_B01_SERVICE");
	CFrameWriter Writer(m_pInfo);

	Writer.clear();
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);
	Writer.addElement(_T("도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);

	astrFrames.Add(Writer.getFrame());

	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	BOOL bComplete = FALSE;
	CString strGetData;
	CESL_DataMgr TmpDM;
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		bComplete = FALSE;
	}
	else
	{
		TmpDM.FreeData();
		Reader.makeDM(&TmpDM);
		if(1 > TmpDM.GetRowCount())
		{
			bComplete = FALSE;
		}
		else
		{
 			CString strTemp = Reader.getCommand();
 			if(_T("ERROR") == strTemp)
 			{
 				bComplete = FALSE;
 			} 
			else
			{
				TmpDM.GetCellData(_T("처리결과"), 0, strTemp);
				if(_T("OK") == strTemp)
				{
					bComplete = TRUE;
				}
				else
				{
					bComplete = FALSE;
				}
			}
		}
	}
	if(FALSE == bComplete)
	{
		SetMessage(TRUE, _T("통합이용자 삭제에 실패하였습니다."));
		return -1;
	}

	return 0;
EFS_END
return -1;
}

INT CKL_UserManagerDlg::KL_IsSecedeMember()
{
EFS_BEGIN

	CFrameReader Reader;
	Reader.clear();

	CFrameWriter Writer(m_pInfo);
	Writer.clear();
	Writer.setCommand(_T("IL_K48_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);

	CESL_DataMgr TmpDM;
	CFrameSender Sender(m_pInfo);
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0>ids)
	{
		CString strMsg;
		strMsg.Format(_T("미들웨어와의 통신에 실패하였습니다."));
		SetMessage(TRUE, strMsg);
		strMsg.Format(_T("가입도서관조회(5) ERROR CODE : %d"),ids);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		m_bIsKLSecede = FALSE;
		return ids;
	}

	Reader.makeDM(&TmpDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp)
	{
		m_bIsKLSecede = FALSE;
		CString strMsg,strTemp;
		TmpDM.GetCellData(0, 0, strTemp);
		if(0<strTemp.Find(_T("DT")))
		{
			return 0;
		}

		strMsg.Format(_T("미들웨어와의 통신에 실패하였습니다."));
		SetMessage(TRUE, strMsg);
		strMsg.Format(_T("가입도서관조회(5) ERROR CODE : %s"),strTemp);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return -1010;
	}
	if(1>TmpDM.GetRowCount())
	{
		m_bIsKLSecede = FALSE;
		return 1010;
	}

	for(INT i = 0; i<TmpDM.GetRowCount(); i++)
	{
		CString strValue;
		TmpDM.GetCellData(_T("가입도서관부호"), i, strValue);
		strValue.TrimLeft(); strValue.TrimRight();
		if(strValue == m_pInfo->LIB_CODE)
		{
			TmpDM.GetCellData(_T("회원상태"), i, strValue);
			strValue.TrimLeft(); strValue.TrimRight();
			if(_T("3") == strValue)
			{
				m_bIsKLSecede = TRUE;
				break;
			}
		}
	}	

	return 0;	
EFS_END
return -1;	
}

CString CKL_UserManagerDlg::KL_CreateDate()
{
	
	CString strGetData;
	CTime t = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	return strGetData;
}

CString CKL_UserManagerDlg::KL_GetBirthYearToCivilNo(CString strCivilNo)
{
	strCivilNo.TrimLeft();	strCivilNo.TrimRight();
	strCivilNo.Replace(_T("-"), _T(""));
	if(7 > strCivilNo.GetLength()) return _T("");

	INT nSex = _ttoi(strCivilNo.Mid(6,1));
	BOOL bIsMale;
	
	if( 1==nSex || 3==nSex || 9==nSex || 5==nSex || 7==nSex ) bIsMale = TRUE;	
	else if( 2==nSex || 4==nSex || 0==nSex || 6==nSex || 8==nSex ) bIsMale = FALSE;	
	else return _T("");		
	CString strBirthyear;
	
	if(9==nSex||0==nSex) strBirthyear = _T("18");	
	else if(1==nSex||2==nSex||5==nSex||6==nSex) strBirthyear = _T("19");
	else if(3==nSex||4==nSex||7==nSex||8==nSex) strBirthyear = _T("20");
	else return _T("");

	CString strTemp;
	strTemp.Format(_T("%s%s"),strBirthyear,strCivilNo.Left(2));

	return strTemp;
}

CESL_DataMgr* CKL_UserManagerDlg::GetMemberInfoDM()
{

	return FindDM(_T("DM_KL_USER_MANAGER_MEMBER_INFO"));
}

BOOL CKL_UserManagerDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	
	InvalidateRect(CRect(220, 195, 390, 235));

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CKL_UserManagerDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	BOOL bFind = FALSE;
	for(INT nCnt = 0; nCnt < 2; nCnt++)
	{
		if(TRUE == m_rBtn[nCnt].PtInRect(point))
		{
			bFind = TRUE;
			m_nBtnMode[nCnt] = 1;
		}
		else
		{
			m_nBtnMode[nCnt] = 0;
		}
	}
	if(bFind)
	{
		return;
	}

	CRect	rDlg;
	GetClientRect(rDlg);
	if( (nFlags & MK_LBUTTON) && FALSE == bFind ) 
	{
		CRect	rCap		=	CRect(0, 0, rDlg.right, 60);
		if(rCap.PtInRect(point))
		{
			HCURSOR cur;
			cur = ::LoadCursor(NULL,IDC_SIZEALL);
			SetCursor(cur);
			this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
			this->UpdateWindow();
			return;
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CKL_UserManagerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL bFind = FALSE;
	for(INT nCnt = 0; nCnt < 2; nCnt++)
	{
		if(TRUE == m_rBtn[nCnt].PtInRect(point))
		{
			bFind = TRUE;
			m_nBtnMode[nCnt] = 2;
		}
		else
		{
			m_nBtnMode[nCnt] = 0;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CKL_UserManagerDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	
	BOOL bFind = FALSE;
	for(INT nCnt = 0; nCnt < 2; nCnt++)
	{
		if(TRUE == m_rBtn[nCnt].PtInRect(point))
		{
			if(0 == nCnt) OnbtnRetry();
			else OnbtnCLOSE();

			m_nBtnMode[nCnt] = 0;
		}
	}
	CDialog::OnLButtonUp(nFlags, point);
}


INT CKL_UserManagerDlg::GetKlUserSearch(CString strCI, CString strCivilNo, CString &strUserNo)
{
	if(strCI.IsEmpty()) return 1;

	CFrameReader Reader;
	CFrameWriter Writer(m_pInfo);
	Writer.clear();
	Writer.setCommand(_T("IL_K49_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("연계정보"), strCI);
	Writer.addElement(_T("주민등록번호"), strCivilNo);
	CESL_DataMgr TmpDM;
	CFrameSender Sender(m_pInfo);
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) return ids;
	Reader.makeDM(&TmpDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp) return 3;
	TmpDM.GetCellData(_T("통합대출자번호"), 0, strUserNo);
	return 0;
}
